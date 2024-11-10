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
/* Maximum number of scanned values (if u have N channels, u'll have
 * BSP_ADC_MAX_STORED_VALUES * N values stored) */
#ifndef BSP_ADC_MAX_STORED_VALUES
#define BSP_ADC_MAX_STORED_VALUES 8
#endif

/* Default sampling period in us */
#ifndef BSP_ADC_DEFAULT_SAMPLING_PERIOD_US
#define BSP_ADC_DEFAULT_SAMPLING_PERIOD_US 1000
#endif

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
/**
 * @brief Called periodically when a new ADC conversion should be done
 */
void bsp_adc_it_us_cb(void);

/******************************************************************************
 * PUBLIC function prototypes
 ******************************************************************************/

/**
 * @brief Init the bsp adc module
 */
void bsp_adc_init(void);

/**
 * @brief Main loop of the bsp adc module
 */
void bsp_adc_loop(void);

/**
 * @brief Start the recording of the ADC values
 * @param[in] out pointer to the buffer where the values will be stored
 */
void bsp_adc_start_recording(uint16_t *out);


#endif /* __BSP_ADC_H__ */
