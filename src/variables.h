#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdint.h>
#include <registers.h>

#define INTMEM(addr) (*(volatile uint8_t*)(addr))

// fixed global varriables that i extracted from original firmware
// #define GATE20 INTMEM(0x17)
// #define UART_VAR0 INTMEM(0x21)
// #define UART_VAR1 INTMEM(0x22)
// #define IRQ_STATE INTMEM(0x69)

#define UNKNOWN_COUNTER_0401 XDATA[0x0401]   // ???
#define UNKNOWN_COUNTER_0402 XDATA[0x0402]   // ???
#define UNKNOWN_FLAGS_0405 XDATA[0x0405]   // ??? bit0: ???
#define UNKNOWN_FLAGS_0406 XDATA[0x0406]   // ??? bit7: ???
#define UNKNOWN_COUNTER_040F XDATA[0x040F]   // ???
#define UNKNOWN_FLAGS_0474 XDATA[0x0474]   // ???
#define EVENT_FLAGS XDATA[0x0475]   // bit0: INT0 event, bit1/3: ???, bit6: ???
#define EVENT_042D XDATA[0x042D]    // ???
#define UART_CNT XDATA[0x0626]  // received byte count
#define UART_TMO XDATA[0x0627]  // uart receive timeout

#endif