/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/* Protection against multiple inclusions */
#ifndef __TASK_MNGR_H__
#define __TASK_MNGR_H__ __FILE__

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

#include "lib_config.h"

/******************************************************************************
 * PUBLIC Configuration
 ******************************************************************************/
#ifndef TASK_MNGR_MAX_TASK
#define TASK_MNGR_MAX_TASK 5
#endif

#ifndef TASK_MNGR_GET_TIME_US_FUNC
#error "TASK_MNGR_GET_TIME_US_FUNC must be defined"
#endif

/******************************************************************************
 * PUBLIC Macro
 ******************************************************************************/

/******************************************************************************
 * PUBLIC type definitions
 ******************************************************************************/
typedef void (*task_init_t)(void);
typedef void (*task_run_t)(void);
typedef void (*task_it_t)(uint32_t period_ms);

/******************************************************************************
 * PUBLIC constant data definitions
 ****************************************F**************************************/

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
 * @brief Register a task
 * @param name Name of the task
 * @param init Initialization function
 * @param run Run function
 * @param it Interrupt function
 */
void task_mngr_register_task(char *name,
                             task_init_t init,
                             task_run_t run,
                             task_it_t it);

/**
 * @brief Initialize the tasks
  * call this function once after registering all the tasks
 */
void task_mngr_init(void);

/**
 * @brief Call the tasks in the main loop
 * call this function in an infinite loop
 */
void task_mngr_main(void);

/**
 * @brief Call the tasks periodically
 * call this function in an interrupt
 * @param period_ms Period in ms at which the tasks are called
 */
void task_mngr_it(int period_ms);

/**
 * @brief Print the information of the tasks
 */
void print_task_info(void);

/**
 * @brief Get the time in us
 */
uint32_t TASK_MNGR_GET_TIME_US_FUNC(void);

#endif /* __TASK_MNGR_H__ */
