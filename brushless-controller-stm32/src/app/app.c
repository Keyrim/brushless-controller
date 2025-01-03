/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/
#define TAG "app"

#include <stdint.h>

#include "main.h"
#include "stm32f3xx_hal_gpio.h"
#include "log.h"
#include "bsp_motor.h"

#include "app.h"


/******************************************************************************
 * PRIVATE Configuration
 ******************************************************************************/
#define APP_BASE_PERIOD 500
#define APP_MIN_PERIOD 1
#define APP_PERIOD_STEP 0.9f

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
static uint16_t app_period = APP_BASE_PERIOD;

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

/**
 * @brief Initialize the main application
 */
void app_init(void) {
    logi("Init ...");

    bsp_motor_start();
    logi("Motor started");
}

/**
 * @brief Main application loop
 */
void app_loop(void) {

    if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
        float new_period = app_period * APP_PERIOD_STEP;
        app_period = new_period > APP_MIN_PERIOD ? new_period : APP_MIN_PERIOD;
    } else {
        app_period = APP_BASE_PERIOD;
    }

    HAL_Delay(app_period);
}
