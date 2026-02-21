#include "pin_mapping.h"

#define X(name, port, pin, str) {port, pin, str},
const struct PhysicalPin pinMap[] = {
    PIN_LIST
};
#undef X