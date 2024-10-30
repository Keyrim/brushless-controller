/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

#include "tim.h"

#include "bsp_motor.h"

/******************************************************************************
 * PRIVATE Configuration
 ******************************************************************************/
#define BSP_MOTOR_TIM_PWM             (htim1)
#define BSP_MOTOR_TIM_CHANNEL_PHASE_A (TIM_CHANNEL_1)
#define BSP_MOTOR_TIM_CHANNEL_PHASE_B (TIM_CHANNEL_2)
#define BSP_MOTOR_TIM_CHANNEL_PHASE_C (TIM_CHANNEL_3)
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
} bsp_motor_phase_comutation_t;

/******************************************************************************
 * PRIVATE function prototypes
 ******************************************************************************/

/******************************************************************************
 * PRIVATE constant data definitions
 ******************************************************************************/
/* Step sequence for a 6-step motor */
static const bsp_motor_phase_comutation_t
    bsp_motor_step_sequence[BSP_MOTOR_STEP_COUNT][BSP_MOTOR_PHASE_COUNT] = {
        /* Step 1 */
        {
            {.enabled = true, .positive = true},  /* Phase A */
            {.enabled = true, .positive = false}, /* Phase B */
            {.enabled = false},                   /* Phase C */
        },
        /* Step 2 */
        {
            {.enabled = true, .positive = true},  /* Phase A */
            {.enabled = false},                   /* Phase B */
            {.enabled = true, .positive = false}, /* Phase C */
        },
        /* Step 3 */
        {
            {.enabled = false},                   /* Phase A */
            {.enabled = true, .positive = true},  /* Phase B */
            {.enabled = true, .positive = false}, /* Phase C */
        },
        /* Step 4 */
        {
            {.enabled = true, .positive = false}, /* Phase A */
            {.enabled = true, .positive = true},  /* Phase B */
            {.enabled = false},                   /* Phase C */
        },
        /* Step 5 */
        {
            {.enabled = true, .positive = false}, /* Phase A */
            {.enabled = false},                   /* Phase B */
            {.enabled = true, .positive = true},  /* Phase C */
        },
        /* Step 6 */
        {
            {.enabled = false},                   /* Phase A */
            {.enabled = true, .positive = false}, /* Phase B */
            {.enabled = true, .positive = true},  /* Phase C */
        },
};

/******************************************************************************
 * PRIVATE variable definitions
 ******************************************************************************/

static volatile bsp_motor_step_t current_step = BSP_MOTOR_STEP_1;
static volatile bsp_motor_pwm_t current_pwm   = 800; /* over 1000 */
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
    const bsp_motor_phase_comutation_t *current_comms =
        bsp_motor_step_sequence[step];

    /* Disable all phases */
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);

    /* Set the phases */
    if (current_comms[BSP_MOTOR_PHASE_A].enabled) {
        HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    }
    if (current_comms[BSP_MOTOR_PHASE_B].enabled) {
        HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    }
    if (current_comms[BSP_MOTOR_PHASE_C].enabled) {
        HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);
    }

    /* Set the PWM */
    bsp_motor_pwm_t pwma = pwm * current_comms[BSP_MOTOR_PHASE_A].positive;
    bsp_motor_pwm_t pwmb = pwm * current_comms[BSP_MOTOR_PHASE_B].positive;
    bsp_motor_pwm_t pwmc = pwm * current_comms[BSP_MOTOR_PHASE_C].positive;
    bsp_motor_update_pwm(pwma, pwmb, pwmc);
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
}

void bsp_motor_set_pwm(uint16_t pwm) {
    current_pwm = pwm;
}

void bsp_motor_set_direction(bool is_forward) {}
