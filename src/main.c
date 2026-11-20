/*
    UreTechVizor monitor controller firmware for 8051 based MStar TSUM series SoC's
    https://github.com/UreTech/
*/

#include <stdint.h>
#include <registers.h>
#include <init/init.h>
#include <variables.h>

// boot init for SoC
uint8_t __sdcc_external_startup(void)
{
    __xdata uint8_t *p = (__xdata uint8_t *)0x0400;
    uint16_t n = 0x0400;

    P3 &= (uint8_t)~0x20; // stay in flash bank 0
    MCU_CONTROL_REGISTER_WD_SWZ1 = 0x55; // wdt disable
    MCU_CONTROL_REGISTER_WD_SWZ2 = 0xAA;
    while (n--) *p++ = 0; // we need to fill 0x0400-0x07FF with zero
    return 0;
}

// interrupt handlers

// external interrupt 0
void ex0_isr(void)   __interrupt(0){
    if((IRQ_STATE & 0x0C) == 0){
        P1 |= ONEBIT(2);
        EVENT_FLAGS |= ONEBIT(0);
        EVENT_042D = 0;
        IRQ_STATE |= 0x0C;
    }
    IE0 = 0; // clear interrupt
}

// timer 0
void timer0_isr(void)__interrupt(1){
    TH0 = 0xF6;
    TL0 = 0xAE;
    if(TIME_COUNTER_LOW-- == 0){
        TIME_COUNTER_HIGH--;
    }
    if(UART_TMO){
        UART_TMO--;
    }
    if(GATE20){
        GATE20--;
    }
    // ??
    uint16_t counter;
}

// external interrupt 1 or 0x0013 (scaler / DDC interrupt)
void ex1_isr(void)   __interrupt(2){
    if(XDATA[MCU_REGISTER_BASE_ADDRESS + 0x11] & 0x18){
        // call something?
    }
    IE1 = 0; // clear interrupt
}

// serial interrupt (not used)
void serial_isr(void)__interrupt(4){ 
    // not used
}

void main(void)
{
    REGBK = 0x00; // use bank0 DIP (digital image processor)
    // REGBK = 0x01; // use bank1 ADC (analog digital converter)
    // REGBK = 0x02; // use bank2 TCON (timer control register)
    // REGBK = 0x03; // use bank3 MWE (??? i have no idea what that short name means but this bank controls some sort of window?)
    
    // init
    boot_init();
    init_software_registers();
    init_something_in_MCU();
    
    for (;;) {
        reg_write(0x1, 0x0);
        // ...
    }
}