/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/* Protection against multiple inclusions */
#ifndef __BSP_UART_H__
#define __BSP_UART_H__ __FILE__

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
#include "usart.h"

/******************************************************************************
 * PUBLIC Configuration
 ******************************************************************************/

/******************************************************************************
 * PUBLIC Macro
 ******************************************************************************/

/******************************************************************************
 * PUBLIC type definitions
 ******************************************************************************/
#define LEN_BUFFER 512

/**
 * @brief transmission state enumeration definition
 */
typedef enum {
    TRANSMISSION_STATE_IDLE,
    TRANSMISSION_STATE_IN_PROGRESS
} transmission_state_e;

/**
 * @brief uart enumeration definition
 */
typedef enum {
    UART_1 = 0,
    UART_COUNT,
} uart_e;

/**
 * @brief uart handler structure definition
 */
typedef struct {
    UART_HandleTypeDef *huart;
    HAL_StatusTypeDef hal_state;
    transmission_state_e transmission_state;

    /* Tx stuff */
    uint8_t buffer_tx[LEN_BUFFER];
    uint16_t index_write_tx;
    uint16_t index_read_tx;

    /* Rx stuff */
    uint8_t buffer_rx[LEN_BUFFER];
    /* Volatile because this is changed inside the uart_idle_line it */
    volatile uint16_t index_write_rx;
    uint16_t index_read_rx;
} uart_t;

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
 * @brief Initialize every uarts and start dma reception
 */
void UART_init(void);

/**
 * @brief Re start the dma transfer if there is data to send or reset the
 * transmission state to idle
 * @param [in] id : Id of the uart
 */
void UART_transfer_complete_cb(uart_e id);

/**
 * @brief Update the read index when the idle flag is raised
 * @param [in] id : Id of the uart
 */
void UART_idle_line_cb(uart_e id);

/**
 * @brief Transmit data in dma on the given uart
 * @param [in] id : Id of the uart
 * @param [in] data : Array with the data
 * @param [in] len : Length of the data array
 */
void UART_transmit(uart_e id, uint8_t *data, uint16_t len);

/**
 * @brief Write the next available character in the buffer
 * 		  Does nothing if there are no characters available
 * @param [in] id : Id of the uart
 * @param [in] c : Buffer to write the character into
 */
void UART_get(uart_e id, uint8_t *c);

/**
 * @brief Number of byte available in the RX buffer
 * @param [in] id : Id of the uart we are interested in
 * @return Number of bytes
 */
uint16_t UART_bytes_available(uart_e id);

/**
 * @brief Test l'émission et la réception sur l'UART1
 */
void UART_selftest(void);

#endif /* __BSP_UART_H__ */
