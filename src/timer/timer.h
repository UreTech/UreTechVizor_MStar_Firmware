#ifndef TIMER_H
#define TIMER_H

#include <registers.h>

void delay_ms(uint16_t ms);

void wait_int0(uint16_t ms);

#endif