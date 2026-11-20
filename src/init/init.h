#ifndef INIT_H
#define INIT_H

#include <registers.h>

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
void init_software_registers(void);

// ...
void init_something_in_MCU(void);

#endif