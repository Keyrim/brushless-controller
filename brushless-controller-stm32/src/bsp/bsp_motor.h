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

/******************************************************************************
 * PUBLIC constant data definitions
 ******************************************************************************/

/******************************************************************************
 * PUBLIC variable definitions
 ******************************************************************************/

/******************************************************************************
 * PUBLIC Callback function
 ******************************************************************************/

/******************************************************************************
 * PUBLIC function prototypes
 ******************************************************************************/

/**
 * @brief Initialize the motor
 */
void bsp_motor_init(void);

/**
 * @brief Enable or disable the motor
 * @param enable true to enable the motor, false to disable it
 */
void bsp_motor_enable(bool enable);

/**
 * @brief Perform a step of the motor
 */
void bsp_motor_step(void);

/**
 * @brief Set the PWM value of the motor
 * @param pwm PWM value to set
 */
void bsp_motor_set_pwm(uint16_t pwm);

/**
 * @brief Set the direction of the motor
 * @param is_forward true to set the motor in forward direction, false for backward
 */
void bsp_motor_set_direction(bool is_forward);

#endif /* __BSP_MOTOR_H__ */
