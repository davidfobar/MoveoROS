# Arduino compatibility layer
add_subdirectory(micro_ros)
add_subdirectory(arduino_compat)

# AccelStepper wrapper
add_library(AccelStepper STATIC
    AccelStepper/src/AccelStepper.cpp
)

target_include_directories(AccelStepper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/AccelStepper/src
)

target_compile_definitions(AccelStepper PUBLIC
    ARDUINO=100
)

target_link_libraries(AccelStepper PUBLIC Arduino_Compat stm32cubemx)

# TMCStepper wrapper
file(GLOB TMCStepper_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/TMCStepper/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMCStepper/src/source/*.cpp
)

add_library(TMCStepper STATIC
    ${TMCStepper_SOURCES}
)

target_include_directories(TMCStepper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/TMCStepper/src
    ${CMAKE_CURRENT_SOURCE_DIR}/TMCStepper/src/source
)

target_compile_definitions(TMCStepper PUBLIC
    ARDUINO=100
)

target_link_libraries(TMCStepper PUBLIC Arduino_Compat stm32cubemx)