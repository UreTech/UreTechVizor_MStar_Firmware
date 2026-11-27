#ifndef UNKNOWN_UTIL_H
#define UNKNOWN_UTIL_H

#include <stdint.h>

// i think this function is for some sort of mmio register access but im not sure
uint16_t _UNKNOWN0_MCU_register_something(uint32_t val);
void _UNKNOWN0_MCU_register_clear(void);

#endif