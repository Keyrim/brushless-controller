set(DRIVERS_DIR ${CMAKE_CURRENT_LIST_DIR})

message("Drivers directory: " ${DRIVERS_DIR})

set(INCLUDE_PATH_DIRS
    ${INCLUDE_PATH_DIRS}
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Inc
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Inc/Legacy
    ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F3xx/Include
    ${DRIVERS_DIR}/CMSIS/Include
)

set(SRCS_FILES
    ${SRCS_FILES}
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc_ex.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
    ${DRIVERS_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
)
