/*
    UreTechVizor monitor controller firmware for 8051 based MStar TSUM series SoC's
    https://github.com/UreTech/
*/

#include <stdint.h>
#include <registers.h>
#include <init/init.h>

// boot init for SoC
uint8_t __sdcc_external_startup(void)
{
    __xdata uint8_t *p = (__xdata uint8_t *)0x0400;
    uint16_t n = 0x0400;

    P3 &= (uint8_t)~0x20; // stay in flash bank 0
    MCU_CONTROL_REGISTER_WD_SWZ1 = 0x55;
    MCU_CONTROL_REGISTER_WD_SWZ2 = 0xAA;
    while (n--) *p++ = 0; // we need to fill 0x0400-0x07FF with zero
    return 0;
}

// interrupt handlers
void ex0_isr(void)   __interrupt(0) { }   // external interrupt 0
void timer0_isr(void)__interrupt(1) { }   // timer 0
void ex1_isr(void)   __interrupt(2) { }   // external interrupt 1 or 0x0013 (scaler / DDC interrupt)
void serial_isr(void)__interrupt(4) { }   // serial interrupt

void main(void)
{
    REGBK = 0x00; // use bank0 DIP (digital image processor)
    // REGBK = 0x01; // use bank1 ADC (analog digital converter)
    // REGBK = 0x02; // use bank2 TCON (timer control register)
    // REGBK = 0x03; // use bank3 MWE (??? i have no idea what that short name means but this bank controls some sort of window?)
    
    // init
    boot_init();
    init_software_registers();

    for (;;) {
        reg_write(0x1, 0x0);
        // ...
    }
}