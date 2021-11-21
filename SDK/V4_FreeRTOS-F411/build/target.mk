###########################################################################
#  !!! This file is Auto-Generated By Embedded IDE, Don't modify it !!!
###########################################################################

# eide version
EIDE_VER = 2

# current target
CUR_TARGET := V4_FreeRTOS-F411

# current compiler
COMPILER_TYPE := AC5

# include folders
INCLUDE_FOLDERS += Core/Inc
INCLUDE_FOLDERS += Drivers/STM32F4xx_HAL_Driver/Inc
INCLUDE_FOLDERS += Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
INCLUDE_FOLDERS += Middlewares/Third_Party/FreeRTOS/Source/include
INCLUDE_FOLDERS += Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
INCLUDE_FOLDERS += Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F
INCLUDE_FOLDERS += Drivers/CMSIS/Device/ST/STM32F4xx/Include
INCLUDE_FOLDERS += Drivers/CMSIS/Include
INCLUDE_FOLDERS += Drivers/Hardware/inc
INCLUDE_FOLDERS += APP/inc
INCLUDE_FOLDERS += .cmsis/include
INCLUDE_FOLDERS += MDK-ARM/RTE/_V4_FreeRTOS-F411
INCLUDE_FOLDERS += .eide/deps

# library search folders

# c source files
C_SOURCES += APP/src/datafusion.c
C_SOURCES += Core/Src/freertos.c
C_SOURCES += Core/Src/gpio.c
C_SOURCES += Core/Src/i2c.c
C_SOURCES += Core/Src/main.c
C_SOURCES += Core/Src/stm32f4xx_hal_msp.c
C_SOURCES += Core/Src/stm32f4xx_hal_timebase_tim.c
C_SOURCES += Core/Src/stm32f4xx_it.c
C_SOURCES += Core/Src/system_stm32f4xx.c
C_SOURCES += Core/Src/tim.c
C_SOURCES += Core/Src/usart.c
C_SOURCES += Drivers/Hardware/src/esp8266.c
C_SOURCES += Drivers/Hardware/src/gy-86.c
C_SOURCES += Drivers/Hardware/src/oled.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
C_SOURCES += Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/croutine.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/list.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/queue.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/tasks.c
C_SOURCES += Middlewares/Third_Party/FreeRTOS/Source/timers.c

# cpp source files

# asm source files
ASM_SOURCES += MDK-ARM/startup_stm32f411xe.s

# object files

# macro defines
DEFINES += USE_HAL_DRIVER
DEFINES += STM32F411xE