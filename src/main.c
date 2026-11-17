/*
    UreTechVizor monitor controller firmware for 8051 based MStar TSUM series SoC's
    https://github.com/UreTech/
*/

#include <stdint.h>
#include <registers.h>

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
    REGBK = 0x00;
    for (;;) {
        reg_write(0x1, 0x0);
        // ...
    }
}