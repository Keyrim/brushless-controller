/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#define TAG "bsp_adc"

#include <stdbool.h>
#include <stdint.h>

#include "adc.h"
#include "log.h"
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_adc.h"
#include "stm32f3xx_hal_tim.h"
#include "tim.h"

#include "bsp_adc.h"
/******************************************************************************
 * PRIVATE Configuration
 ******************************************************************************/
#define ADC_RECORD_TRIGGER_THRESHOLD 350

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
static uint16_t adc_sampling_period_us = BSP_ADC_DEFAULT_SAMPLING_PERIOD_US;
static uint16_t adc_stored_conv[BSP_ADC_MAX_STORED_VALUES * BSP_ADC_CHANN_MAX] =
    {ADC_RECORD_TRIGGER_THRESHOLD};
static volatile uint16_t adc_nb_stored_conv = 0;
/** True when the recording can be started when the measured is under threshold */
static volatile bool adc_can_record   = true;
static volatile bool adc_is_recording = false;

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
void bsp_adc_it_us_cb(void) {
    if (!adc_is_recording && !adc_can_record) {
        return;
    }

    if (adc_nb_stored_conv >= BSP_ADC_MAX_STORED_VALUES) {
        adc_is_recording = false;
        adc_can_record   = false;
        return;
    }

    uint16_t *adc_conv =
        &adc_stored_conv[adc_nb_stored_conv * BSP_ADC_CHANN_MAX];
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_conv, BSP_ADC_CHANN_MAX);

    if ((adc_conv[0] < ADC_RECORD_TRIGGER_THRESHOLD) && adc_can_record) {
        adc_is_recording = true;
    }

    if (adc_is_recording) {
        adc_nb_stored_conv++;
    }
}

/**
 * @brief Print all the stored conversions
 */
void bsp_adc_print_stored_conv(void) {
    for (uint16_t i = 0; i < adc_nb_stored_conv; i++) {
        uint16_t *adc_conv = &adc_stored_conv[i * BSP_ADC_CHANN_MAX];
        logi("%d\t%d\t%d\t%d", i, adc_conv[0], adc_conv[1], adc_conv[2]);
        HAL_Delay(2);
    }
}

/******************************************************************************
 * PUBLIC function
 ******************************************************************************/

void bsp_adc_test(void) {

    logi("ADC test\n");

    HAL_TIM_Base_Init(&htim2);
    HAL_TIM_Base_Start_IT(&htim2);

    for (;;) {
        if (!adc_is_recording && adc_nb_stored_conv > 0) {
            bsp_adc_print_stored_conv();
            HAL_Delay(2000);
            logw("Restarting the recording");
            adc_nb_stored_conv = 0;
            adc_stored_conv[0] = ADC_RECORD_TRIGGER_THRESHOLD;
            adc_can_record     = true;
        }
    }
}
