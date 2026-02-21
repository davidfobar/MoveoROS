cmake_minimum_required(VERSION 3.22)
project(Arduino_Compat)

add_library(Arduino_Compat STATIC
    Arduino.cpp
    SPI.cpp
    Stream.cpp
    pin_mapping.c
    timer.c
)

target_compile_definitions(Arduino_Compat PUBLIC STM32F446xx)

target_include_directories(Arduino_Compat PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)