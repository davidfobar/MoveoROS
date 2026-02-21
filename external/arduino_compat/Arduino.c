#include "Arduino.h"
#include <stdlib.h>

// TODO: These need STM32 HAL implementation
void digitalWrite(uint8_t pin, uint8_t val) {
    // Use STM32 HAL_GPIO_WritePin()
}

int digitalRead(uint8_t pin) {
    // Use STM32 HAL_GPIO_ReadPin()
    return 0;
}

void pinMode(uint8_t pin, uint8_t mode) {
    // Configure GPIO via STM32 HAL
}

unsigned long millis(void) {
    // Return milliseconds from HAL_GetTick()
    return 0;
}

unsigned long micros(void) {
    // Return microseconds from timer
    return 0;
}

void delay(unsigned long ms) {
    // Use HAL_Delay()
}

void delayMicroseconds(unsigned int us) {
    // Use busy wait or timer
}