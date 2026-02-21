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
bool pin_write(const struct PhysicalPin* pin, GPIO_PinState state);
GPIO_PinState pin_read(const struct PhysicalPin* pin);
void pin_toggle(const struct PhysicalPin* pin);

static inline void pin_high(const struct PhysicalPin* pin) {
    pin_write(pin, GPIO_PIN_SET);
}

static inline void pin_low(const struct PhysicalPin* pin) {
    pin_write(pin, GPIO_PIN_RESET);
}

const char* pin_get_name(const struct PhysicalPin* pin);

#ifdef __cplusplus
}
#endif

#endif /* PIN_MAPPING_H */