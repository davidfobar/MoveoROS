/**
 * @file    timer.h
 * @brief   Timer abstraction for Arduino compatibility
 * 
 * Provides millis(), micros(), delay(), and delayMicroseconds()
 * using configurable STM32 timers.
 */

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Initialize the timer system
 * 
 * Must be called from main() before using millis/micros/delay functions
 */
void timer_init(void);

/**
 * @brief Get milliseconds since timer_init() was called
 * @return Milliseconds elapsed
 */
unsigned long millis(void);

/**
 * @brief Get microseconds since timer_init() was called
 * @return Microseconds elapsed
 */
unsigned long micros(void);

/**
 * @brief Delay for specified milliseconds
 * @param ms Milliseconds to delay
 */
void delay(unsigned long ms);

/**
 * @brief Delay for specified microseconds
 * @param us Microseconds to delay
 */
void delayMicroseconds(unsigned int us);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */