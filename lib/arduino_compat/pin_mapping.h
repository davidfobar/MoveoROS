/**
 * @file    pin_mapping.h
 * @brief   Pin mapping abstraction layer
 */

#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pin_config.h" // from Core/Inc for pin definitions
#include <stdbool.h>
#include <stdint.h>

struct PhysicalPin {
    GPIO_TypeDef* port;
    uint16_t pin;
    const char* name;
};

// Generate enum from pin_config.h
#define X(name, port, pin, str) name,
enum PinIndex : uint8_t {
    PIN_LIST // from pin_config.h
    NUM_PINS
};
#undef X

extern const struct PhysicalPin pinMap[];

// Function declarations
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
void pinMode(uint8_t pin, uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* PIN_MAPPING_H */