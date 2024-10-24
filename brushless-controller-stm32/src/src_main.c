/*
 * SPDX-FileCopyrightText: 2024 Théo Magne <theo.magne.fr@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>

#include "src_main.h"
#include "app_main.h"
#include "bsp.h"

#include "task_mngr.h"


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

/******************************************************************************
 * PRIVATE constant data definitions
 ******************************************************************************/

/******************************************************************************
 * PRIVATE variable definitions
 ******************************************************************************/

/******************************************************************************
 * PUBLIC implicit variable definitions (e.g. when no .h file)
 ******************************************************************************/

/******************************************************************************
 * PRIVATE Callback function
 ******************************************************************************/

/******************************************************************************
 * PRIVATE function
 ******************************************************************************/
static void src_init(void) {
    /* Init the board support package */
    bsp_init();

    /* Register all the tasks */
    task_mngr_register_task("app", app_main_init, app_main_loop, NULL);

    /* Initialize the tasks */
    task_mngr_init();
}

static void src_loop(void) {
    /* Run the tasks */
    task_mngr_main();
}

/******************************************************************************
 * PUBLIC Callback function
 ******************************************************************************/

/******************************************************************************
 * PUBLIC function
 ******************************************************************************/

void src_main(void) {
    src_init();
    for (;;) {
        src_loop();
    }
}
