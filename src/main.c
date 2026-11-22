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
uint8_t irq_state;
void ex0_isr(void)   __interrupt(0){
    if((irq_state & 0x0C) == 0){
        P1 |= ONEBIT(2);
        EVENT_FLAGS |= ONEBIT(0);
        EVENT_042D = 0;
        irq_state |= 0x0C;
    }
    IE0 = 0; // clear interrupt
}

// timer 0
uint16_t timer_counter;
uint8_t gate20;
void timer0_isr(void)__interrupt(1){
    TH0 = 0xF6;
    TL0 = 0xAE;
    timer_counter--;
    if(UART_TMO){
        UART_TMO--;
    }
    if(gate20){
        gate20--;
    }
    
    if(timer_counter == 0){
        if(!((EVENT_FLAGS >> 1) & ONEBIT(0))){
            // this is maybe a global 32bit counter for something
            uint32_t read_val = ((uint32_t)XDATA[0x040B] << 24) | ((uint32_t)XDATA[0x040C] << 16) | ((uint32_t)XDATA[0x040D] << 8) | ((uint32_t)XDATA[0x040E]);
            if(read_val != 0xFFFFFFFF){
                // inc counter
                read_val++;
                XDATA[0x040B] = (read_val >> 24) & 0xFF;
                XDATA[0x040C] = (read_val >> 16) & 0xFF;
                XDATA[0x040D] = (read_val >> 8) & 0xFF;
                XDATA[0x040E] = (read_val) & 0xFF;
            }else{
                // reset counter
                XDATA[0x040B] = 0;
                XDATA[0x040C] = 0;
                XDATA[0x040D] = 0;
                XDATA[0x040E] = 0;
            }
        }

        if(UNKNOWN_COUNTER_0401){
            UNKNOWN_COUNTER_0401--;
            if(UNKNOWN_COUNTER_0401 == 0){
                EVENT_FLAGS |= ONEBIT(7);
            }
        }

        if (UNKNOWN_COUNTER_040F) {
            UNKNOWN_COUNTER_040F--;

            if (UNKNOWN_COUNTER_040F == 0) {
                uint8_t flags_406 = UNKNOWN_FLAGS_0406;
                if (flags_406 & ONEBIT(7)) {
                    UNKNOWN_FLAGS_0405 |= ONEBIT(0);
                }

                UNKNOWN_FLAGS_0406 = flags_406;
            }
        }

        if(UNKNOWN_COUNTER_0402){
            UNKNOWN_COUNTER_0402--;
            if(UNKNOWN_COUNTER_0402 == 0){
                EVENT_FLAGS |= ONEBIT(2);
            }
        }

        UNKNOWN_FLAGS_0474 |= ONEBIT(2);
        timer_counter = 0x03E8;
    }

    if((timer_counter % 0xFA) == 0){
        UNKNOWN_FLAGS_0474 |= ONEBIT(4);
    }

    TF0 = 0;
}

// external interrupt 1 or 0x0013 (scaler / DDC interrupt)
void ex1_isr(void)   __interrupt(2){
    // this is very sketchy logic
    // i think this thing is pointing the source of the interrupt in scaler
    uint8_t unknown_interrupt_state = MCU_CONTROL_REGISTER_UNKNOWN_INTERRUPT_RELATED_C011 & (ONEBIT(4) | ONEBIT(3));
    if(unknown_interrupt_state != 0){
        if(unknown_interrupt_state != ONEBIT(3)){
            if(unknown_interrupt_state != ONEBIT(4)){
                return; // should not reach here? i think?
            }

            
        }

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