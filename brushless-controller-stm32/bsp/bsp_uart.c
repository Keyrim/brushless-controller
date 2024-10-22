/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "bsp_uart.h"
#include "sft_def.h"
#include "usart.h"

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
/**
 * @brief Compute the numbers of byte available in the buffer
 * @param [in] id : Id of the uart we are interested in
 * @return nb of bytes available
 */
static uint16_t UART_get_nb_bytes_available(uart_e id);
/******************************************************************************
 * PRIVATE constant data definitions
 ******************************************************************************/

/******************************************************************************
 * PRIVATE variable definitions
 ******************************************************************************/
static uart_t uart[UART_COUNT] = {0};

/******************************************************************************
 * PUBLIC implicit variable definitions (e.g. when no .h file)
 ******************************************************************************/

/******************************************************************************
 * PRIVATE Callback function
 ******************************************************************************/

/******************************************************************************
 * PRIVATE function
 ******************************************************************************/

static uint16_t UART_get_nb_bytes_available(uart_e id) {
    uint16_t nb = 0;
    if (uart[id].index_write_rx > uart[id].index_read_rx) {
        nb = uart[id].index_write_rx - uart[id].index_read_rx;
    } else if (uart[id].index_write_rx < uart[id].index_read_rx) {
        nb = LEN_BUFFER - uart[id].index_read_rx + uart[id].index_write_rx;
    }
    return nb;
}

/******************************************************************************
 * PUBLIC Callback function
 ******************************************************************************/

void UART_idle_line_cb(uart_e id) {
    if (__HAL_UART_GET_FLAG(uart[id].huart, UART_FLAG_IDLE)) {
        uart[id].index_write_rx =
            LEN_BUFFER - uart[id].huart->hdmarx->Instance->CNDTR;
        __HAL_UART_CLEAR_IDLEFLAG(uart[id].huart);
    }
}
void UART_transfer_complete_cb(uart_e id) {
    if (uart[id].index_write_tx != uart[id].index_read_tx) {
        uint16_t nb_bytes_to_read = 0;
        if (uart[id].index_write_tx > uart[id].index_read_tx) {
            nb_bytes_to_read = uart[id].index_write_tx - uart[id].index_read_tx;
        } else {
            nb_bytes_to_read = LEN_BUFFER - uart[id].index_read_tx;
        }
        HAL_UART_Transmit_DMA(uart[id].huart,
                              &uart[id].buffer_tx[uart[id].index_read_tx],
                              nb_bytes_to_read);
        uart[id].index_read_tx =
            (uart[id].index_read_tx + nb_bytes_to_read) % LEN_BUFFER;
    } else {
        uart[id].transmission_state = TRANSMISSION_STATE_IDLE;
    }
}

/******************************************************************************
 * PUBLIC function
 ******************************************************************************/

void UART_init(void) {
    /* Link huarts to the right uarts structures */
    uart[UART_2].huart = &huart2;

    /* Initialization */
    for (uint8_t u = 0; u < UART_COUNT; u++) {
        __HAL_UART_ENABLE_IT(uart[u].huart, UART_IT_IDLE);
        HAL_UART_Receive_DMA(uart[u].huart, uart[u].buffer_rx, LEN_BUFFER);
    }
}

void UART_transmit(uart_e id, uint8_t *data, uint16_t len) {
    /* On doit au moins avoir un caractère à envoyer */
    if (len == 0) {
        return;
    }
    for (uint16_t c = 0; c < len; c++) {
        uart[id].buffer_tx[(uart[id].index_write_tx + c) % LEN_BUFFER] =
            data[c];
    }
    uart[id].index_write_tx = (uart[id].index_write_tx + len) % LEN_BUFFER;
    if (uart[id].transmission_state == TRANSMISSION_STATE_IDLE) {
        uart[id].transmission_state = TRANSMISSION_STATE_IN_PROGRESS;
        uint16_t nb_bytes_before_end_of_buffer =
            LEN_BUFFER - uart[id].index_read_tx;
        len = MIN(len, nb_bytes_before_end_of_buffer);
        HAL_UART_Transmit_DMA(uart[id].huart,
                              &uart[id].buffer_tx[uart[id].index_read_tx], len);
        uart[id].index_read_tx = (uart[id].index_read_tx + len) % LEN_BUFFER;
    }
}

uint16_t UART_bytes_available(uart_e id) {
    return UART_get_nb_bytes_available(id);
}

void UART_get(uart_e id, uint8_t *c) {
    if (UART_get_nb_bytes_available(id)) {
        *c = uart[id].buffer_rx[uart[id].index_read_rx];
        uart[id].index_read_rx = (uart[id].index_read_rx + 1) % LEN_BUFFER;
    }
}
