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
#define BSP_MOTOR_TIM_LOW_SPEED       (htim6)

/******************************************************************************
 * PRIVATE Macro
 ******************************************************************************/

/******************************************************************************
 * PRIVATE type definitions
 ******************************************************************************/

/**
 * @brief A pwm value for the motor
 * @note the value must be between in [0, 1000[
 */
typedef uint16_t bsp_motor_pwm_t;

typedef enum {
    BSP_MOTOR_STEP_1 = 0, /**< Step 1 */
    BSP_MOTOR_STEP_2,     /**< Step 2 */
    BSP_MOTOR_STEP_3,     /**< Step 3 */
    BSP_MOTOR_STEP_4,     /**< Step 4 */
    BSP_MOTOR_STEP_5,     /**< Step 5 */
    BSP_MOTOR_STEP_6,     /**< Step 6 */
    BSP_MOTOR_STEP_COUNT  /**< Number of steps */
} bsp_motor_step_t;

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

typedef struct {
    uint16_t en_pins;
    uint16_t dis_pin;
    bool chan1;
    bool chan2;
    bool chan3;
} bsp_motor_com_t;

/******************************************************************************
 * PRIVATE function prototypes
 ******************************************************************************/

/******************************************************************************
 * PRIVATE constant data definitions
 ******************************************************************************/

static const bsp_motor_com_t bsp_motor_com[BSP_MOTOR_STEP_COUNT] = {
    /* Step 1 */
    {
        .en_pins = EN1_Pin | EN2_Pin,
        .dis_pin = EN3_Pin,
        .chan1   = true,
        .chan2   = false,
        .chan3   = false,
    },
    /* Step 2 */
    {
        .en_pins = EN1_Pin | EN3_Pin,
        .dis_pin = EN2_Pin,
        .chan1   = true,
        .chan2   = false,
        .chan3   = false,
    },
    /* Step 3 */
    {
        .en_pins = EN2_Pin | EN3_Pin,
        .dis_pin = EN1_Pin,
        .chan1   = false,
        .chan2   = true,
        .chan3   = false,
    },
    /* Step 4 */
    {
        .en_pins = EN2_Pin | EN1_Pin,
        .dis_pin = EN3_Pin,
        .chan1   = false,
        .chan2   = true,
        .chan3   = false,
    },
    /* Step 5 */
    {
        .en_pins = EN3_Pin | EN1_Pin,
        .dis_pin = EN2_Pin,
        .chan1   = false,
        .chan2   = false,
        .chan3   = true,
    },
    /* Step 6 */
    {
        .en_pins = EN3_Pin | EN2_Pin,
        .dis_pin = EN1_Pin,
        .chan1   = false,
        .chan2   = false,
        .chan3   = true,
    },
};

/******************************************************************************
 * PRIVATE variable definitions
 ******************************************************************************/

static volatile bsp_motor_step_t current_step = BSP_MOTOR_STEP_1;
static volatile bsp_motor_pwm_t current_pwm   = 400; /* over 499 */
static volatile bool is_forward               = true;

/******************************************************************************
 * PUBLIC implicit variable definitions (e.g. when no .h file)
 ******************************************************************************/

/******************************************************************************
 * PRIVATE Callback function
 ******************************************************************************/

/******************************************************************************
 * PRIVATE function
 ******************************************************************************/

static inline bsp_motor_step_t
bsp_motor_get_next_step(bsp_motor_step_t current_step, bool is_forward) {
    if (is_forward) {
        return (current_step + 1) % BSP_MOTOR_STEP_COUNT;
    }
    return (current_step + BSP_MOTOR_STEP_COUNT - 1) % BSP_MOTOR_STEP_COUNT;
}

static inline void bsp_motor_update_pwm(
    bsp_motor_pwm_t pwma,
    bsp_motor_pwm_t pwmb,
    bsp_motor_pwm_t pwmc) {

    __HAL_TIM_SET_COMPARE(
        &BSP_MOTOR_TIM_PWM,
        BSP_MOTOR_TIM_CHANNEL_PHASE_A,
        pwma);
    __HAL_TIM_SET_COMPARE(
        &BSP_MOTOR_TIM_PWM,
        BSP_MOTOR_TIM_CHANNEL_PHASE_B,
        pwmb);
    __HAL_TIM_SET_COMPARE(
        &BSP_MOTOR_TIM_PWM,
        BSP_MOTOR_TIM_CHANNEL_PHASE_C,
        pwmc);
}

static inline void bsp_motor_set_phases(
    bsp_motor_step_t step,
    bsp_motor_pwm_t pwm,
    bool is_forward) {

    /* Get the current commutations infos */
    const bsp_motor_com_t *com = &bsp_motor_com[step];

    /* Disable the disabled phase */
    BSP_MOTOR_EN_GPIO_PORT->BRR = com->dis_pin;

    /* Set the enabled phases */
    BSP_MOTOR_EN_GPIO_PORT->BSRR = com->en_pins;

    /* Set the PWM */
    bsp_motor_update_pwm(pwm * com->chan1, pwm * com->chan2, pwm * com->chan3);
}

/******************************************************************************
 * PUBLIC Callback function
 ******************************************************************************/

/******************************************************************************
 * PUBLIC function
 ******************************************************************************/

void bsp_motor_init(void) {
    /* Start the PWM timer */
    bsp_motor_set_phases(current_step, current_pwm, is_forward);
    HAL_TIM_PWM_Start(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_A);
    HAL_TIM_PWM_Start(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_B);
    HAL_TIM_PWM_Start(&BSP_MOTOR_TIM_PWM, BSP_MOTOR_TIM_CHANNEL_PHASE_C);

    // HAL_TIM_Base_Start(&BSP_MOTOR_TIM_LOW_SPEED);
}

void bsp_motor_enable(bool enable) {}

void bsp_motor_step(void) {
    current_step = bsp_motor_get_next_step(current_step, is_forward);
    bsp_motor_set_phases(current_step, current_pwm, is_forward);
    logi("Step %d", current_step);
}

void bsp_motor_set_pwm(uint16_t pwm) {
    current_pwm = pwm;
}

void bsp_motor_set_direction(bool is_forward) {}
