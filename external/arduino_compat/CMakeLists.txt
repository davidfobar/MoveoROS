cmake_minimum_required(VERSION 3.22)
project(Arduino_Compat)

add_library(Arduino_Compat STATIC
    Arduino.c
    SPI.cpp
    Stream.cpp
)

target_include_directories(Arduino_Compat PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)