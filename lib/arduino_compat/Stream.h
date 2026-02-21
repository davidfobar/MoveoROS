#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <stddef.h>

class Stream {
public:
    virtual ~Stream();
    
    virtual int read() = 0;
    virtual int available() = 0;
    virtual void flush() = 0;
    
    virtual size_t write(uint8_t byte) = 0;
    virtual size_t write(const uint8_t* buffer, size_t length) = 0;
    int peek();
    int readBytes(char* buffer, size_t length);
    int readBytesUntil(char terminator, char* buffer, size_t length);
};

#endif