/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/* Protection against multiple inclusions */
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__ __FILE__

/******************************************************************************
 * Includes
 ******************************************************************************/

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
typedef enum {
    BSP_ADC_CHANN_BEMF1 = 0, /**< BEMF1 channel */
    BSP_ADC_CHANN_BEMF2,     /**< BEMF2 channel */
    BSP_ADC_CHANN_BEMF3,     /**< BEMF3 channel */
    BSP_ADC_CHANN_CURR1,     /**< Current 1 channel */
    BSP_ADC_CHANN_CURR2,     /**< Current 2 channel */
    BSP_ADC_CHANN_CURR3,     /**< Current 3 channel */
    BSP_ADC_CHANN_MAX        /**< Maximum number of channels */
} bsp_adc_channel_t;

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
 * @brief Iinitializes the ADC
 * It actually does nothing for the moment but ^^
 */
void bsp_adc_init(void);

/**
 * @brief Start the recording of the ADC values
 * @param[in] out pointer to the buffer where the values will be stored
 * note the buffer must be big enough to store BSP_ADC_CHANN_MAX values
 */
void bsp_adc_start_recording(uint16_t *out);


#endif /* __BSP_ADC_H__ */
