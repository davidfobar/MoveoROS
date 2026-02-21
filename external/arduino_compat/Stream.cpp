#include "Stream.h"
#include <string.h>

Stream::Stream() {
}

Stream::~Stream() {
}

size_t Stream::write(const uint8_t *buffer, size_t size) {
    size_t count = 0;
    for (size_t i = 0; i < size; i++) {
        count += write(buffer[i]);
    }
    return count;
}

int Stream::peek() {
    // TODO: Implement if needed by TMCStepper
    return -1;
}

int Stream::readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length && available() > 0) {
        int c = read();
        if (c < 0) break;
        buffer[count++] = (char)c;
    }
    return count;
}

int Stream::readBytesUntil(char terminator, char *buffer, size_t length) {
    size_t count = 0;
    while (count < length && available() > 0) {
        int c = read();
        if (c < 0) break;
        if (c == terminator) break;
        buffer[count++] = (char)c;
    }
    return count;
}