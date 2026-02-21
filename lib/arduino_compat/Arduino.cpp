#include "Arduino.h"
#include <stdlib.h>

void digitalWrite(uint8_t pin, uint8_t val)
{
  if (pin >= NUM_PINS) {
    return;
  }

  const struct PhysicalPin& p = pinMap[pin];
  GPIO_PinState state = (val == HIGH) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(p.port, p.pin, state);
}

int digitalRead(uint8_t pin) {
    if (pin >= NUM_PINS) {
        return LOW; // Invalid pin, return LOW
    }
    const struct PhysicalPin& p = pinMap[pin];
    return (HAL_GPIO_ReadPin(p.port, p.pin) == GPIO_PIN_SET) ? HIGH : LOW;
}

void pinMode(uint8_t pin, uint8_t mode)
{
  // GPIO pins are already configured in MX_GPIO_Init()
  // This is a no-op for STM32
  (void)pin;
  (void)mode;
}