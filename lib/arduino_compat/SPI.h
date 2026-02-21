#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stddef.h>

// SPI mode definitions
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

// SPI clock speeds
#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03

// Bit order
#define MSBFIRST 0
#define LSBFIRST 1

// Output/Input definitions
#define OUTPUT 1
#define INPUT 0

// SPISettings class for beginTransaction
class SPISettings {
public:
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) 
        : clock(clock), bitOrder(bitOrder), dataMode(dataMode) {}
    
    uint32_t clock;
    uint8_t bitOrder;
    uint8_t dataMode;
};

// SPI class interface
class SPIClass {
public:
    SPIClass();
    void begin();
    void end();
    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t mode);
    void setClockDivider(uint8_t divider);
    void beginTransaction(SPISettings settings);
    void endTransaction();
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void *buf, size_t count);
};

// Global SPI instance
extern SPIClass SPI;

#endif