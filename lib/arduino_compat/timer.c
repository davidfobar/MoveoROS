/**
 * @file    timer.c
 * @brief   Timer implementation using STM32 HAL
 */

#include "timer.h"
#include "timer_config.h"
#include "main.h"

static volatile uint32_t milliseconds = 0;

/**
 * @brief Timer callback - called every millisecond
 * 
 * This should be called from your timer interrupt in stm32f4xx_it.c
 * For TIM2, this would be called from TIM2_IRQHandler
 */
void timer_tick_ms(void)
{
    milliseconds++;
}

/**
 * @brief Initialize the timer system
 */
void timer_init(void)
{
    // Start the timer
    HAL_TIM_Base_Start_IT(&TIMER_MILLIS_HANDLE);
    milliseconds = 0;
}

/**
 * @brief Get milliseconds since timer_init() was called
 */
unsigned long millis(void)
{
    return milliseconds;
}

/**
 * @brief Get microseconds since timer_init() was called
 * 
 * Combines millisecond counter with timer counter for microsecond precision
 */
unsigned long micros(void)
{
    uint32_t ms = milliseconds;
    uint32_t timer_count = __HAL_TIM_GET_COUNTER(&TIMER_MILLIS_HANDLE);
    
    // Convert to microseconds
    // Assumes TIMER_MILLIS_FREQ is 1MHz for 1:1 mapping
    return (ms * 1000UL) + timer_count;
}

/**
 * @brief Delay for specified milliseconds
 */
void delay(unsigned long ms)
{
    unsigned long start = millis();
    while ((millis() - start) < ms) {
        // Allow other operations to run
    }
}

/**
 * @brief Delay for specified microseconds
 */
void delayMicroseconds(unsigned int us)
{
    unsigned long start = micros();
    while ((micros() - start) < us) {
        // Allow other operations to run
    }
}