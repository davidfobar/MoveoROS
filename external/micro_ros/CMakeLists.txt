cmake_minimum_required(VERSION 3.22)
project(MicroROS)

# Pre-built Micro-ROS static library
add_library(microros STATIC IMPORTED GLOBAL)
set_target_properties(microros PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../micro_ros_stm32cubemx_utils/microros_static_library/libmicroros/libmicroros.a
    IMPORTED_NO_SONAME TRUE
)

target_include_directories(microros INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../micro_ros_stm32cubemx_utils/microros_static_library/libmicroros/microros_include
)

# Micro-ROS utility sources
add_library(microros_utils STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../micro_ros_stm32cubemx_utils/extra_sources/custom_memory_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../micro_ros_stm32cubemx_utils/extra_sources/microros_allocators.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../micro_ros_stm32cubemx_utils/extra_sources/microros_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../micro_ros_stm32cubemx_utils/extra_sources/microros_transports/dma_transport.c
)

target_include_directories(microros_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../micro_ros_stm32cubemx_utils/microros_static_library/libmicroros/microros_include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

target_compile_definitions(microros_utils PUBLIC
    STM32F446xx
)

target_link_libraries(microros_utils PUBLIC microros)