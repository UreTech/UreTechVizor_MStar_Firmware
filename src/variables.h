#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdint.h>
#include <registers.h>

#define INTMEM(addr) (*(volatile uint8_t*)(addr))

// im not sure what these does for now
#define UART_VAR0 INTMEM(0x21)
#define UART_VAR1 INTMEM(0x22)



#endif