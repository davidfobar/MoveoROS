#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <stddef.h>

class Stream {
public:
    Stream();
    virtual ~Stream();
    
    // Pure virtual functions - must be implemented by subclasses
    virtual size_t write(uint8_t) = 0;
    virtual int read() = 0;
    virtual int available() = 0;
    virtual void flush() = 0;
    
    // Utility functions
    size_t write(const uint8_t *buffer, size_t size);
    int peek();
    int readBytes(char *buffer, size_t length);
    int readBytesUntil(char terminator, char *buffer, size_t length);
};

#endif