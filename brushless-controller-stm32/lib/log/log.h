/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @todo Handle log level (e.g. only log error)
 * @todo Handle module specific log level
 */

/* Protection against multiple inclusions */
#ifndef __LOG_H__
#define __LOG_H__ __FILE__

/******************************************************************************
 * Includes
 ******************************************************************************/

/******************************************************************************
 * PUBLIC Configuration
 ******************************************************************************/

#define LOG_AINSI_RED "\x1b[1;38;5;196m"
#define LOG_AINSI_GREEN "\x1b[38;5;46m"
#define LOG_AINSI_BLUE "\x1b[38;5;33m"
#define LOG_AINSI_YELLOW "\x1b[38;5;226m"
#define LOG_AINSI_ORANGE "\x1b[38;5;202m"
#define LOG_AINSI_RESET "\x1b[0m"
#define LOG_NEWLINE "\n"

#ifdef TAG
#define LOG_TAG (TAG " ")
#else
#define LOG_TAG ""
#endif

/******************************************************************************
 * PUBLIC Macro
 ******************************************************************************/

#define logd(fmt, ...)                                                         \
    log_msg(LOG_AINSI_BLUE LOG_TAG fmt LOG_AINSI_RESET LOG_NEWLINE,            \
            ##__VA_ARGS__)
#define logi(fmt, ...)                                                         \
    log_msg(LOG_AINSI_GREEN LOG_TAG fmt LOG_AINSI_RESET LOG_NEWLINE,           \
            ##__VA_ARGS__)
#define logw(fmt, ...)                                                         \
    log_msg(LOG_AINSI_ORANGE LOG_TAG fmt LOG_AINSI_RESET LOG_NEWLINE,          \
            ##__VA_ARGS__)
#define loge(fmt, ...)                                                         \
    log_msg(LOG_AINSI_RED LOG_TAG fmt LOG_AINSI_RESET LOG_NEWLINE,             \
            ##__VA_ARGS__)

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
 * @brief Log a message.
 * Note that MACROs are available to log with different levels.
 *
 * @param fmt the format string
 * @param ... the arguments
 */
void log_msg(const char *fmt, ...);

#endif /* __LOG_H__ */
