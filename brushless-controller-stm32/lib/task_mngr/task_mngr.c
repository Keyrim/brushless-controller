/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/
#define TAG "task_mngr"

#include "task_mngr.h"
#include "log.h"
#include "sft_def.h"
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 * PRIVATE Configuration
 ******************************************************************************/

/******************************************************************************
 * PRIVATE Macro
 ******************************************************************************/
#define TASK_MNGR_PROCESS_COMPUTE_VAR()                                        \
    uint32_t start    = TASK_MNGR_GET_TIME_US_FUNC();                          \
    uint32_t end      = 0;                                                     \
    uint32_t duration = 0;

#define TASK_MNGR_PROCESS_COMPUTE_DURATION(_process)                           \
    end                   = TASK_MNGR_GET_TIME_US_FUNC();                      \
    duration              = end - start;                                       \
    _process.duration     = duration * 1e-3;                                   \
    _process.duration_max = MAX(_process.duration_max, duration);              \
    _process.duration_min = MIN(_process.duration_min, duration);              \
    start                 = end;

/******************************************************************************
 * PRIVATE type definitions
 ******************************************************************************/
/**
 * @brief Process structure definition
 */
typedef struct {
    float duration;        /**< Duration of the process */
    uint32_t duration_max; /**< Maximum duration of the process */
    uint32_t duration_min; /**< Minimum duration of the process */
} process_info_t;

/**
 * @brief Task structure definition
 */
typedef struct {
    char *name;       /**< Name of the task */
    task_init_t init; /**< Initialization function */
    task_run_t run;   /**< Run function */
    task_it_t it;     /**< Interrupt function */
    /**< Process information related to the run function */
    process_info_t process_run;
    /**< Process information related to the interrupt function */
    process_info_t process_it;
} task_t;

/******************************************************************************
 * PRIVATE function prototypes
 ******************************************************************************/

/******************************************************************************
 * PRIVATE constant data definitions
 ******************************************************************************/
static const char *unknown_task = "nnknown task";

/******************************************************************************
 * PRIVATE variable definitions
 ******************************************************************************/
static task_t _tasks[TASK_MNGR_MAX_TASK]; /**< Tasks array */
static uint8_t _tasks_count = 0;          /**< Number of tasks */

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

void task_mngr_register_task(char *name,
                             task_init_t init,
                             task_run_t run,
                             task_it_t it) {
    if (_tasks_count >= TASK_MNGR_MAX_TASK) {
        loge("Cannot register task %s: too many tasks", name);
        return;
    }

    if (name == NULL) {
        logw("Task name is NULL, using default name");
        name = (char*)unknown_task;
    }

    _tasks[_tasks_count].name = name;
    _tasks[_tasks_count].init = init;
    _tasks[_tasks_count].run  = run;
    _tasks[_tasks_count].it   = it;
    _tasks_count++;
}

void task_mngr_init(void) {
    for (uint8_t i = 0; i < _tasks_count; i++) {
        if (_tasks[i].init != NULL) {
            _tasks[i].init();
        }
    }
    logi("Initialized %d tasks", _tasks_count);
}

void task_mngr_main(void) {
    TASK_MNGR_PROCESS_COMPUTE_VAR();
    for (uint8_t i = 0; i < _tasks_count; i++) {
        if (_tasks[i].run != NULL) {
            _tasks[i].run();
            TASK_MNGR_PROCESS_COMPUTE_DURATION(_tasks[i].process_run);
        }
    }
}

void task_mngr_it(int period_ms) {

    TASK_MNGR_PROCESS_COMPUTE_VAR();
    for (uint8_t i = 0; i < _tasks_count; i++) {
        if (_tasks[i].it != NULL) {
            _tasks[i].it(period_ms);
            TASK_MNGR_PROCESS_COMPUTE_DURATION(_tasks[i].process_it);
        }
    }
}

void print_task_info(void) {
    logi("Tasks information:");
    for (uint8_t i = 0; i < _tasks_count; i++) {
        logi("Task %s:"
             "process main duration %f ms (min %d ms, max %d ms)"
             "process it duration %f ms (min %d ms, max %d ms)",
             _tasks[i].name,
             _tasks[i].process_run.duration,
             _tasks[i].process_run.duration_min,
             _tasks[i].process_run.duration_max,
             _tasks[i].process_it.duration,
             _tasks[i].process_it.duration_min,
             _tasks[i].process_it.duration_max);
    }
}
