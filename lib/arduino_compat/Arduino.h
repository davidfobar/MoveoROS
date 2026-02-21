#ifndef ARDUINO_COMPAT_H
#define ARDUINO_COMPAT_H

#include <stdint.h>
#include <math.h>
#include "pin_mapping.h"

#define ARDUINO 100

typedef uint8_t byte;
typedef uint8_t boolean;

#define HIGH 1
#define LOW 0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

// Math functions
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

// Use C math library functions
#define sqrt(x) sqrtf(x)
#define fabs(x) fabsf(x)

// Arduino IO functions
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
void pinMode(uint8_t pin, uint8_t mode);
unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

#endif