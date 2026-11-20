#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdint.h>
#include <registers.h>

#define INTMEM(addr) (*(volatile uint8_t*)(addr))

// fixed global varriables that i extracted from original firmware
#define TIME_COUNTER_HIGH INTMEM(0x15)
#define TIME_COUNTER_LOW INTMEM(0x16)
#define GATE20 INTMEM(0x17)
#define UART_VAR0 INTMEM(0x21)
#define UART_VAR1 INTMEM(0x22)
#define IRQ_STATE INTMEM(0x69)

#define EVENT_FLAGS XDATA[0x0475]   // bit0: INT0 event, bit1/3: ???
#define EVENT_042D XDATA[0x042D]    // ???
#define UART_CNT XDATA[0x0626]  // received byte count
#define UART_TMO XDATA[0x0627]  // uart receive timeout

#endif