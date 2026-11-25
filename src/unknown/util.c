#include <unknown/util.h>
#include <variables.h>
#include <timer/timer.h>

void _UNKNOWN0_MCU_register_clear(void)
{
    volatile __xdata uint8_t *reg = &XDATA[MCU_REGISTER_BASE_ADDRESS + 0x40];

    for(uint8_t i = 0; i < 8; i++){
        reg[i] = 0;
    }
}

uint16_t _UNKNOWN0_MCU_register_setup(uint32_t val){
    _UNKNOWN0_MCU_register_clear();

    XDATA[MCU_REGISTER_BASE_ADDRESS + 0x40] = 0xAB;
    XDATA[MCU_REGISTER_BASE_ADDRESS + 0x41] = ((val >> 16) & 0xFF);
    XDATA[MCU_REGISTER_BASE_ADDRESS + 0x42] = ((val >> 8) & 0xFF);
    XDATA[MCU_REGISTER_BASE_ADDRESS + 0x43] = (val & 0xFF);
    XDATA[MCU_REGISTER_BASE_ADDRESS + 0x48] = 0xBD;

    uint8_t timeout = 10;

    while(XDATA[MCU_REGISTER_BASE_ADDRESS + 0x48] & 0x80)
    {
        if(timeout == 0) break;

        timeout--;

        delay_ms(1);
    }

    uint8_t result_low  = XDATA[MCU_REGISTER_BASE_ADDRESS + 0x4C];
    uint8_t result_high = XDATA[MCU_REGISTER_BASE_ADDRESS + 0x4D];

    return ((uint16_t)result_high << 8) | result_low;
}