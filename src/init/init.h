#ifndef INIT_H
#define INIT_H

#include <registers.h>

extern uint8_t uart_var0;
extern uint8_t uart_var1;

void boot_init(void);

// inits mcu for boot
void boot_init_mcu(void);

// inits interrupts
void boot_init_interrupts(void);

// inits timers
void boot_init_timers(void);

// inits uart
void boot_init_uart(void);

// init software registers
void software_reset_external_devices(void);

// ...
uint16_t do_something_in_MCU(void);

// init dip
void init_DIP(void);

#endif