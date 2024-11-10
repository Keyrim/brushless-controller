/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/* Protection against multiple inclusions */
#ifndef __BSP_MOTOR_H__
#   define __BSP_MOTOR_H__ __FILE__

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/******************************************************************************
 * PUBLIC Configuration
 ******************************************************************************/

/******************************************************************************
 * PUBLIC Macro
 ******************************************************************************/

/******************************************************************************
 * PUBLIC type definitions
 ******************************************************************************/

/**
 * @brief A pwm value for the motor
 * @note the value must be between in [0, 500[
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



/******************************************************************************
 * PUBLIC constant data definitions
 ******************************************************************************/

/******************************************************************************
 * PUBLIC variable definitions
 ******************************************************************************/

/******************************************************************************
 * PUBLIC Callback function
 ******************************************************************************/

/**
 * @brief Called on a timer update event interrupt
 */
void bsp_motor_timer_update_event_cb(void);

/******************************************************************************
 * PUBLIC function prototypes
 ******************************************************************************/

/**
 * @brief Initialize the motor by setting the capture compare values to 0
 */
void bsp_motor_init(void);

/**
 * @brief Start the pwm timer
 */
void bsp_motor_start(void);

/**
 * @brief Stop the pwm timer
 */
void bsp_motor_stop(void);

/**
 * @brief Set the PWM value of the motor
 * @param pwm PWM value to set
 */
void bsp_motor_set_pwm(bsp_motor_pwm_t pwm);

/**
 * @brief Set the current active phase for the motor
 * @param step Step to set @see bsp_motor_step_t
 */
void bsp_motor_set_step(bsp_motor_step_t step);

#endif /* __BSP_MOTOR_H__ */
