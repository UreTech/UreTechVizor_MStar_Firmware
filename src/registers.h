#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#define MAKE_REG(reg_addr) (*(volatile __xdata unsigned char *)(reg_addr))

// TSUM1PFR register base address
#define REGISTER_BASE_ADDRESS (0xC000)

// MCU control registers
#define MCU_CONTROL_REGISTER_WD_SWZ1 MAKE_REG(REGISTER_BASE_ADDRESS + 0x8) // watchdog timer software disable key low byte
#define MCU_CONTROL_REGISTER_WD_SWZ2 MAKE_REG(REGISTER_BASE_ADDRESS + 0x9) // watchdog timer software disable key high byte

// SFRs (standard 8051)
__sfr __at(0x80) P0;
__sfr __at(0x90) P1;
__sfr __at(0xA0) P2;
__sfr __at(0xB0) P3; // P3.5 = flash bank select (A16)

// scaler register window: XDATA 0x0000..0x00FF, index 0 = REGBK
__xdata __at(0x0000) volatile uint8_t SC[256];
#define REGBK   SC[0x00]

static void reg_write(uint8_t reg, uint8_t val){
    SC[reg] = val;
}
static uint8_t reg_read(uint8_t reg){ 
    return SC[reg];
}

#endif