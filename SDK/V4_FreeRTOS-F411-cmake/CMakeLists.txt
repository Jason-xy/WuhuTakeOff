cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)
project(F411-FreeRTOS C ASM)

# Custom option
set(MCU_FAMILY F4)
set(mcu_family f4)
set(MCU_MODEL F411CE)

# RTOS support
option(USE_CMSIS_RTOS "Use CMSIS RTOS provided by Cube repository" OFF)
option(USE_CMSIS_RTOS_V2 "Use CMSIS RTOS_V2 provided by Cube repository" ON)
option(USE_CUBE_FREERTOS "Use the FreeRTOS kernel provided by the Cube repository" OFF)

if(USE_CUBE_FREERTOS)
    message(STATUS "Using FreeRTOS provided by Cube repository")
else()
    message(STATUS "Using FreeRTOS from kernel repository")
endif()

if(USE_CMSIS_RTOS AND USE_CMSIS_RTOS_V2)
    message(FATAL_ERROR "Can not use USE_CMSIS_RTOS_V2 together with USE_CMSIS_RTOS!")
endif()

if(USE_CMSIS_RTOS)
    message(STATUS "Compiling CMSIS RTOS support")
elseif(USE_CMSIS_RTOS_V2)
    message(STATUS "Compiling CMSIS RTOS V2 support")
endif()

set(SUPPORTED_BOARDS F411CE)
option(FREERTOS_F411CE "Compile FreeRTOS for the F411CE board" ON)

# Prepare compile depends
if(NOT CMSIS_INCLUDE_DIRS OR NOT CMSIS_SOURCES)
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps)
        if(NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/_deps)
            execute_process(COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/deps ${CMAKE_BINARY_DIR}/_deps)
        endif()
        set(CMSIS_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/_deps/stm32-cmsis-${mcu_family}-src/Include)
        set(CMSIS_SOURCES  ${CMAKE_BINARY_DIR}/_deps/stm32-cmsis-${mcu_family}-src/Source)
        set(STM32_CUBE_${MCU_FAMILY}_PATH  ${CMAKE_BINARY_DIR}/_deps/stm32cube${mcu_family}-src)
        set(FREERTOS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS)
    else(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps)    
        if(NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/_deps)
            stm32_fetch_cube(${MCU_FAMILY})
            stm32_fetch_cmsis(${MCU_FAMILY})
            stm32_fetch_hal(${MCU_FAMILY})
            if(IS_DIRECTORY ${CMAKE_BINARY_DIR}/_deps)
                execute_process(COMMAND cp -r ${CMAKE_BINARY_DIR}/_deps ${CMAKE_CURRENT_SOURCE_DIR}/deps)
            endif()
        else(IS_DIRECTORY ${CMAKE_BINARY_DIR}/_deps)
            execute_process(COMMAND cp -r ${CMAKE_BINARY_DIR}/_deps ${CMAKE_CURRENT_SOURCE_DIR}/deps)
        endif()
    endif()
endif()

# Component configuration
set(HAL_COMP_LIST RCC GPIO CORTEX TIM I2C USART DMA UART)
set(CMSIS_COMP_LIST "")
set(FREERTOS_COMP_LIST "")
set(FREERTOS_NAMESPACE FreeRTOS)

if(USE_CMSIS_RTOS)
    list(APPEND CMSIS_COMP_LIST RTOS)
endif()
if(USE_CMSIS_RTOS_V2)
    list(APPEND CMSIS_COMP_LIST RTOS_V2)
endif()

if(FREERTOS_F411CE)
    list(APPEND CMSIS_COMP_LIST STM32F411CE)
    list(APPEND HAL_COMP_LIST STM32F411CE)
    list(APPEND FREERTOS_COMP_LIST ARM_CM4F)
    if(USE_CUBE_FREERTOS)
        # The device family needs to be supplied as a component to use the Cube FreeRTOS sources
        list(APPEND FREERTOS_COMP_LIST STM32F4)
        set(FREERTOS_F4_NAMESPACE ${FREERTOS_NAMESPACE}::STM32::F4)
     else()
        set(FREERTOS_F4_NAMESPACE ${FREERTOS_NAMESPACE})
    endif()
endif()

# Dependence
find_package(CMSIS COMPONENTS ${CMSIS_COMP_LIST} REQUIRED)
find_package(HAL COMPONENTS ${HAL_COMP_LIST} REQUIRED)
find_package(FreeRTOS COMPONENTS ${FREERTOS_COMP_LIST} REQUIRED)

file(GLOB SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/app/main/src/*
    ${CMAKE_CURRENT_SOURCE_DIR}/app/module/src/*
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/src/*
)
set(PROJECT_SOURCES ${SOURCE_FILES})

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/app/main/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/app/module/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/inc
   

)

set(TARGET_NAME ${PROJECT_NAME}-f411ce)
add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE
    ${FREERTOS_F4_NAMESPACE}::Timers
    ${FREERTOS_F4_NAMESPACE}::Heap::4
    ${FREERTOS_F4_NAMESPACE}::ARM_CM4F
    HAL::STM32::F4::RCC
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::DMA
    HAL::STM32::F4::TIM
    HAL::STM32::F4::I2C
    HAL::STM32::F4::USART
    HAL::STM32::F4::UART
    CMSIS::STM32::F411CE 
    STM32::NoSys
)
if(USE_CMSIS_RTOS)
    target_link_libraries(${TARGET_NAME} PRIVATE
        CMSIS::STM32::F4::RTOS
    )
endif()
if(USE_CMSIS_RTOS_V2)
    target_link_libraries(${TARGET_NAME} PRIVATE
        CMSIS::STM32::F4::RTOS_V2
    )
    target_compile_definitions(${TARGET_NAME} PRIVATE
        USE_CMSIS_RTOS_V2
        CMSIS_RTOS_V2_DEVICE_HEADER="stm32f4xx_hal.h"
    )
endif()

stm32_print_size_of_target(${TARGET_NAME})
stm32_generate_binary_file(${TARGET_NAME})
stm32_generate_hex_file(${TARGET_NAME})

