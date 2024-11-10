/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#define TAG "bsp_motor"

#include <stdbool.h>
#include <stdint.h>

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_tim.h"
#include "stm32f302x8.h"
#include "bsp_adc.h"
#include "log.h"
#include "tim.h"

#include "bsp_motor.h"

/******************************************************************************
 * PRIVATE Configuration
 ******************************************************************************/
#define BSP_MOTOR_TIM_PWM             (htim1)
#define BSP_MOTOR_TIM_CHANNEL_PHASE_A (TIM_CHANNEL_1)
#define BSP_MOTOR_TIM_CHANNEL_PHASE_B (TIM_CHANNEL_2)
#define BSP_MOTOR_TIM_CHANNEL_PHASE_C (TIM_CHANNEL_3)
#define BSP_MOTOR_EN_GPIO_PORT        (GPIOC)
#define BSP_MOTOR_EN1_GPIO_PIN        (GPIO_PIN_10)
#define BSP_MOTOR_EN2_GPIO_PIN        (GPIO_PIN_11)
#define BSP_MOTOR_EN3_GPIO_PIN        (GPIO_PIN_12)

/******************************************************************************
 * PRIVATE Macro
 ******************************************************************************/
#define BSP_MOTOR_SET_CCR(pwma, pwmb, pwmc)                                    \
    do {                                                                       \
        __HAL_TIM_SET_COMPARE(                                                 \
            &BSP_MOTOR_TIM_PWM,                                                \
            BSP_MOTOR_TIM_CHANNEL_PHASE_A,                                     \
            (pwma));                                                           \
        __HAL_TIM_SET_COMPARE(                                                 \
            &BSP_MOTOR_TIM_PWM,                                                \
            BSP_MOTOR_TIM_CHANNEL_PHASE_B,                                     \
            (pwmb));                                                           \
        __HAL_TIM_SET_COMPARE(                                                 \
            &BSP_MOTOR_TIM_PWM,                                                \
            BSP_MOTOR_TIM_CHANNEL_PHASE_C,                                     \
            (pwmc));                                                           \
    } while (0)

/******************************************************************************
 * PRIVATE type definitions
 ******************************************************************************/

typedef struct {
    uint16_t en_pins;
    uint16_t dis_pin;
    bool chan1;
    bool chan2;
    bool chan3;
} bsp_motor_com_t;

typedef enum {
    BSP_MOTOR_PHASE_A = 0, /**< Phase A */
    BSP_MOTOR_PHASE_B,     /**< Phase B */
    BSP_MOTOR_PHASE_C,     /**< Phase C */
    BSP_MOTOR_PHASE_COUNT  /**< Number of phases */
} bsp_motor_phase_t;

typedef struct {
    bool enabled;  /**< true if the phase is enabled, false otherwise */
    bool positive; /**< true if the phase is positive, false otherwise */
} bsp_motor_comm_t;

/******************************************************************************
 * PRIVATE function prototypes
 ******************************************************************************/
static void bsp_motor_update_phases(void);
static void bsp_motor_timer_underflow(void);
static void bsp_motor_timer_overflow(void);
/******************************************************************************
 * PRIVATE constant data definitions
 ******************************************************************************/
static const bsp_motor_com_t bsp_motor_com[BSP_MOTOR_STEP_COUNT] = {
    /* Step 1 */
    {
        .en_pins = BSP_MOTOR_EN1_GPIO_PIN | BSP_MOTOR_EN2_GPIO_PIN,
        .dis_pin = BSP_MOTOR_EN3_GPIO_PIN,
        .chan1   = true,
        .chan2   = false,
        .chan3   = false,
    },
    /* Step 2 */
    {
        .en_pins = BSP_MOTOR_EN1_GPIO_PIN | BSP_MOTOR_EN3_GPIO_PIN,
        .dis_pin = BSP_MOTOR_EN2_GPIO_PIN,
        .chan1   = true,
        .chan2   = false,
        .chan3   = false,
    },
    /* Step 3 */
    {
        .en_pins = BSP_MOTOR_EN2_GPIO_PIN | BSP_MOTOR_EN3_GPIO_PIN,
        .dis_pin = BSP_MOTOR_EN1_GPIO_PIN,
        .chan1   = false,
        .chan2   = true,
        .chan3   = false,
    },
    /* Step 4 */
    {
        .en_pins = BSP_MOTOR_EN2_GPIO_PIN | BSP_MOTOR_EN1_GPIO_PIN,
        .dis_pin = BSP_MOTOR_EN3_GPIO_PIN,
        .chan1   = false,
        .chan2   = true,
        .chan3   = false,
    },
    /* Step 5 */
    {
        .en_pins = BSP_MOTOR_EN3_GPIO_PIN | BSP_MOTOR_EN1_GPIO_PIN,
        .dis_pin = BSP_MOTOR_EN2_GPIO_PIN,
        .chan1   = false,
        .chan2   = false,
        .chan3   = true,
    },
    /* Step 6 */
    {
        .en_pins = BSP_MOTOR_EN3_GPIO_PIN | BSP_MOTOR_EN2_GPIO_PIN,
        .dis_pin = BSP_MOTOR_EN1_GPIO_PIN,
        .chan1   = false,
        .chan2   = false,
        .chan3   = true,
    },
};

/******************************************************************************
 * PRIVATE variable definitions
 ******************************************************************************/
static volatile bsp_motor_pwm_t bsp_motor_current_pwm   = 0;
static volatile bsp_motor_step_t bsp_motor_current_step = 0;
static uint16_t bsp_motor_meas_current[6]               = {0};
static uint16_t bsp_motor_meas_bemf[6]                  = {0};

/******************************************************************************
 * PUBLIC implicit variable definitions (e.g. when no .h file)
 ******************************************************************************/

/******************************************************************************
 * PRIVATE Callback function
 ******************************************************************************/

/******************************************************************************
 * PRIVATE function
 ******************************************************************************/
void bsp_motor_update_phases(void) {

    const bsp_motor_com_t *com = &bsp_motor_com[bsp_motor_current_step];

    /* Disable the disabled phase */
    BSP_MOTOR_EN_GPIO_PORT->BRR = com->dis_pin;

    /* Set the enabled phases */
    BSP_MOTOR_EN_GPIO_PORT->BSRR = com->en_pins;

    /* Set the PWM */
    bsp_motor_pwm_t pwm1 = bsp_motor_current_pwm * com->chan1;
    bsp_motor_pwm_t pwm2 = bsp_motor_current_pwm * com->chan2;
    bsp_motor_pwm_t pwm3 = bsp_motor_current_pwm * com->chan3;
    BSP_MOTOR_SET_CCR(pwm1, pwm2, pwm3);
}

void bsp_motor_timer_underflow(void) {
    /* TODO */
    bsp_adc_start_recording(bsp_motor_meas_bemf);
}

void bsp_motor_timer_overflow(void) {
    /* TODO */
    bsp_adc_start_recording(bsp_motor_meas_current);
}

/******************************************************************************
 * PUBLIC Callback function
 ******************************************************************************/
void bsp_motor_timer_update_event_cb(void) {

    // Check the direction. The logic is inverted because the timer changes
    // the direction bit before we can read it after the update event.
    // If we read that we are counting up, it means that we have underflowed and
    // that we are NOW counting up but we were counting down before.
    if (TIM1->CR1 & TIM_CR1_DIR) {
        // Overflow event
        bsp_motor_timer_overflow();
    } else {
        // Underflow event
        bsp_motor_timer_underflow();
    }
}
/******************************************************************************
 * PUBLIC function
 ******************************************************************************/

void bsp_motor_init(void) {
    /* Set the PWM to 0 */
    bsp_motor_current_pwm = 100;
    bsp_motor_update_phases();
}

void bsp_motor_start(void) {
    /* Start the PWM timer */
    HAL_TIM_Base_Start_IT(&BSP_MOTOR_TIM_PWM);
    HAL_TIM_PWM_Start(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_A);
    HAL_TIM_PWM_Start(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_B);
    HAL_TIM_PWM_Start(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_C);
}

void bsp_motor_stop(void) {
    /* Stop the PWM timer */
    HAL_TIM_PWM_Stop(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_A);
    HAL_TIM_PWM_Stop(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_B);
    HAL_TIM_PWM_Stop(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_C);
    HAL_TIM_Base_Stop_IT(&BSP_MOTOR_TIM_PWM);
}

void bsp_motor_set_pwm(bsp_motor_pwm_t pwm) {
    bsp_motor_current_pwm = pwm;
}

void bsp_motor_set_step(bsp_motor_step_t step) {
    bsp_motor_current_step = step;
}
