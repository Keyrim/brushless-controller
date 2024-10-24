set(CORE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(INCLUDE_PATH_DIRS
    ${INCLUDE_PATH_DIRS}
    ${CORE_DIR}/Inc
)

set(SRCS_FILES
    ${SRCS_FILES}
    ${CORE_DIR}/Src/main.c
    ${CORE_DIR}/Src/gpio.c
    ${CORE_DIR}/Src/usart.c
    ${CORE_DIR}/Src/adc.c
    ${CORE_DIR}/Src/tim.c
    ${CORE_DIR}/Src/dma.c
    ${CORE_DIR}/Src/stm32f3xx_hal_msp.c
    ${CORE_DIR}/Src/stm32f3xx_it.c
    ${CORE_DIR}/Src/syscalls.c
    ${CORE_DIR}/Src/sysmem.c
    ${CORE_DIR}/Src/system_stm32f3xx.c
    ${CORE_DIR}/Startup/startup_stm32f302r8tx.s
)
