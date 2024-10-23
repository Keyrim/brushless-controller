/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include "log.h"

/******************************************************************************
 * PRIVATE Configuration
 ******************************************************************************/

/**
 * @brief The size of the log buffer
 */
#ifndef LOG_BUFFER_SIZE
#   define LOG_BUFFER_SIZE 256
#endif

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

/**
 * @brief Buffer used to store the log message before sending it
 */
static char log_buffer[LOG_BUFFER_SIZE];

/******************************************************************************
 * PUBLIC implicit variable definitions (e.g. when no .h file)
 ******************************************************************************/

extern void log_puts(char* str, int len);

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
void log_msg(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(log_buffer, LOG_BUFFER_SIZE, fmt, args);
    va_end(args);
    log_puts(log_buffer, len);
}
