/**
 * @file    timer_config.h
 * @brief   Timer configuration for the robot arm
 * 
 * Define timer instances and their purposes here.
 * Configure timers in CubeMX, then reference them by logical name.
 */

#ifndef TIMER_CONFIG_H
#define TIMER_CONFIG_H

#include "main.h"

/* Timer instances from CubeMX */
extern TIM_HandleTypeDef htim2;
#define TIMER_MILLIS_HANDLE   htim2    // Timer for millis/micros
#define TIMER_MILLIS_FREQ     1000000  // 1 MHz clock for microsecond accuracy

#endif /* TIMER_CONFIG_H */