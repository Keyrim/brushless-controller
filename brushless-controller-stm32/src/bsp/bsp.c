/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "bsp.h"
#include "bsp_uart.h"

/******************************************************************************
 * PRIVATE Configuration
 ******************************************************************************/

/******************************************************************************
 * PRIVATE Macro
 ******************************************************************************/

/******************************************************************************
 * PRIVATE type definitions
 ******************************************************************************/

/******************************************************************************
 * PRIVATE function prototypes
 ******************************************************************************/

/******************************************************************************
 * PRIVATE constant data definitions
 ******************************************************************************/

/******************************************************************************
 * PRIVATE variable definitions
 ******************************************************************************/

/******************************************************************************
 * PUBLIC implicit variable definitions (e.g. when no .h file)
 ******************************************************************************/

/******************************************************************************
 * PRIVATE Callback function
 ******************************************************************************/

/******************************************************************************
 * PRIVATE function
 ******************************************************************************/

/******************************************************************************
 * PUBLIC Callback function
 ******************************************************************************/

/******************************************************************************
 * PUBLIC function
 ******************************************************************************/

void bsp_init(void) {
    UART_init();
}

uint32_t bsp_get_time_us(void) {
    uint32_t t_us;
    static uint32_t previous_t_us = 0;
    __disable_irq();
    t_us = HAL_GetTick() * 1000 + 1000 - SysTick->VAL / 72;
    __enable_irq();

    if (previous_t_us > t_us) {

        t_us += 1000;
    }
    previous_t_us = t_us;

    return t_us;
}