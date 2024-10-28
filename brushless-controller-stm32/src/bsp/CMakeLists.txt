set(BSP_DIR ${CMAKE_CURRENT_LIST_DIR})

set(INCLUDE_PATH_DIRS
    ${INCLUDE_PATH_DIRS}
    ${BSP_DIR}
)

set(SRCS_FILES
    ${SRCS_FILES}
    ${BSP_DIR}/bsp.c
    ${BSP_DIR}/bsp_uart.c
    ${BSP_DIR}/bsp_adc.c
)


