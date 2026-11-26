#include <timer/timer.h>

void delay_ms(uint16_t ms)
{
    while (ms != 0) {
        ms--;
        uint16_t t = 0x00B6; // 1ms
        while (t != 0) {
            t--;
        }
    }
}

void wait_int0(uint16_t ms)
{
    while (ms != 0) {
        ms--;
        uint16_t t = 0x00B6; // 1ms
        while (t != 0) {
            t--;

            if (EVENT_FLAGS & 0x01){
                // INT0
                return;
            }
        }
    }
}