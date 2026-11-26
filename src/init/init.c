#include <init/init.h>
#include <variables.h>
#include <registers.h>
#include <unknown/util.h>
#include <timer/timer.h>

void boot_init(void){
    EA = 0; // disable intterupts
    boot_init_mcu();
    boot_init_interrupts();
    boot_init_timers();

    EA = 1; // enable intterupts

    return;
}

void boot_init_mcu(void){
    P0 = 0xFF;    
    P1 = 0xFF;    
    P2 = 0xFF;
    CCAP1L = 0x00;
    MCU_CONTROL_REGISTER_DDC2BI_CTRL = DDC2BI_CTRL_EN_HOLD_CK | DDC2BI_CTRL_EN_NO_ACK | DDC2BI_CTRL_STD_DVI_IO; // NOTE: my controller is only have adc not dvi but orginal firmware uses dvi bit

    MCU_CONTROL_REGISTER_MCU_P1_OUT_EN |= ONEBIT(2) | ONEBIT(5); // enable mode force P1.2 & P1.5
    MCU_CONTROL_REGISTER_MCU_P1_FMD |= ONEBIT(2) | ONEBIT(5); // force P1.2 & P1.5 as output? (i think P1.2 & P1.5 is inicator leds on the keyboard)

    MCU_CONTROL_REGISTER_MCU_P2_OUT_EN |= ONEBIT(2) | ONEBIT(3) | ONEBIT(7); // enable P2.2, P2.3 & P2.7
    MCU_CONTROL_REGISTER_MCU_P2_FMD |= ONEBIT(2) | ONEBIT(3) | ONEBIT(7); // set P2.2, P2.3 & P2.7 as output (i have no idea what these ports are)

    MCU_CONTROL_REGISTER_MCU_P0_OUT_EN = ONEBIT(6) | ONEBIT(7); // enable mode force P0.6 & P0.7
    MCU_CONTROL_REGISTER_MCU_P0_FMD = ONEBIT(6) | ONEBIT(7); // set P0.6 & P0.7 as output (i have no idea what these ports are, maybe leds are these?!?!)

    // ??? i have no idea about these
    XDATA[MCU_REGISTER_BASE_ADDRESS + 0x93] = 3;
    XDATA[MCU_REGISTER_BASE_ADDRESS + 0x91] = 1;

    P2 &= ~ONEBIT(7); // P2.7 low
    P0 &= ~ONEBIT(7); // P0.7 low
    P0 &= ~ONEBIT(6); // P0.6 low
    P1 |= ONEBIT(2); // P1.2 high
    P1 |= ONEBIT(0); // P1.0 high
    return;
}

void boot_init_interrupts(void){
    IT0 = 1; // INT0 edge trigger
    IE0 = 0; // clear interrupt flag
    EX0 = 1; // enable INT0
    PX0 = 1; // high priority

    IT1 = 1; // INT1 edge trigger
    IE1 = 0; // clear interrupt flag
    EX1 = 1; // enable INT1
    PX1 = 1; // high priority

    return;
}

void boot_init_timers(void){
    // timer0 (2386 cycle timer?)
    ET0 = 1; // enable timer0
    TH0 = 0xf6;
    TL0 = 0xae;
    TF0 = 0; // clear overflow flag

    // timer1
    ET1 = 0; // disable timer1
    TH1 = 0;
    TL1 = 0;
    TF1 = 0;

    TMOD = TMOD_M0_0 | TMOD_M1_1; // setup timers

    TR0 = 1; // start timer0
    TR1 = 0; // stop timer1

    return;
}

uint8_t uart_var0;
uint8_t uart_var1;
void boot_init_uart(void){
    // idk what these for
    uart_var0 = 0xFF;
    uart_var1 = 0xA3;

    PCON = PCON_SMOD; // double baudrate?
    SCON = SCON_TI | SCON_REN | SCON_SM1;

    // use timer1 for uart
    TH1 = 0xf0; // 16 cycle
    TR1 = 1; // start timer1
    RI = 0;
    TI = 0;

    ES = 0; // serial interrupt disabled
    return;
}

void software_reset_DIP(void){
    SWITCH_DIP_BK(); // use bank0 DIP
    reg_write(DIP_SWRST, 0); // clear DIP reset register
    RD = 1;
    delay_ms(5);
    RD = 0;    
    SWITCH_DIP_BK(); // use bank0 DIP

    reg_write(DIP_DBFC, 0); // double buffer disable
    reg_write(DIP_SWRST, DIP_SWRST_SWR | DIP_SWRST_OSDR | DIP_SWRST_BIUR | DIP_SWRST_DPR | DIP_SWRST_GPR | DIP_SWRST_ADCR); // reset
    delay_ms(2);
    reg_write(DIP_SWRST, 0); // clear DIP reset register
    reg_write(DIP_PDMD, DIP_PDMD_GCLK_APDLA); // power down mode
    reg_write(DIP_TEST, 0); // disable test mode

    SWITCH_TCON_BK(); // use bank2 TCON

    // this thing is probably init sequance for timers & clocks but i have no documentataion for it
    reg_write(TCON_UNKNOWN_REGISTER, 0x20);
    uint8_t read_value;
    read_value = reg_read(TCON_UNKNOWN_REGISTER);
    reg_write(TCON_UNKNOWN_REGISTER, read_value | 0x40);

    SWITCH_DIP_BK(); // use bank0 DIP

    return;
}

uint16_t init_something_in_MCU(void){
    uint16_t unknown_return_value;
    unknown_return_value = _UNKNOWN0_MCU_register_setup(0x0);
    uint8_t a_saved_value = (unknown_return_value >> 8);

    if((unknown_return_value >> 8) == 0xBE){
        _UNKNOWN0_MCU_register_clear();
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x40] = 0x50;
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x41] = 0x01;
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x42] = 0x8C;
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x48] = 0xC8;
    }else{
        _UNKNOWN0_MCU_register_clear();
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x40] = 0x06;
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x48] = 0xF8;

        uint8_t timeout_counter = 10;
        while (1){
            if (!(XDATA[0xC048] & 0x80)) break;
        
            uint8_t previous = timeout_counter;
            timeout_counter--;
        
            if (previous == 0) break;
        
            delay_ms(1);
        }

        _UNKNOWN0_MCU_register_clear();
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x40] = 0x01;
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x41] = 0x8C;
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x41] = 0x01;
        XDATA[MCU_REGISTER_BASE_ADDRESS + 0x48] = 0xF9;
    }

    uint8_t timeout = 10;

    while (1)
    {
        if (!(XDATA[0xC048] & 0x80)) break;

        uint8_t previous = timeout--;

        if (previous == 0) break;

        delay_ms(1);
    }

    P2 &= ~ONEBIT(7);
    return unknown_return_value;
}

void init_DIP(void){
    software_reset_DIP();
    SWITCH_DIP_BK();
    reg_write(DIP_PDMD, DIP_PDMD_GCLK_APDLA);
    reg_write(DIP_TEST, 0);
    reg_write16(DIP_DEVST, 0); // vertical output start (0)
    reg_write16(DIP_DEVEND, 899); // vertical output end (899 is end index of 1400x900 panel)
    reg_write16(DIP_DEHST, 75); // horizontal output start (75?)
    reg_write16(DIP_DEHEND, 1514); // horizontal output end (1514 idk why this value)
    reg_write16(DIP_OIHST, 75); // // horizontal output start (75?)
    reg_write16(DIP_OIHEND, 1514); // horizontal output end (1514 idk why this value)
    reg_write16(DIP_OIVEND, 899); // vertical output end (899 is end index of 1400x900 panel)
    reg_write16(DIP_VDTOT, 939); // idk why this value is used here
    reg_write16(DIP_VSST, 900); // vsync related
    reg_write16(DIP_VSEND, 902); // vsync related
    reg_write16(DIP_HDTOT, 1839); // idk why this value is used here

    reg_write(DIP_HSEND, 34); // idk why this value is used here

    // output signal control setup
    reg_write(DIP_OSCTRL1, DIP_OSCTRL1_CTRL_FLAG | DIP_OSCTRL1_AHRT_FLAG | DIP_OSCTRL1_HRSM_FLAG | DIP_OSCTRL1_UNKNOWN0_FLAG);
    reg_write(DIP_OSCTRL2, DIP_OSCTRL2_UNKNOWN0_FLAG);

    // output control setup
    reg_write(DIP_OCTRL1, DIP_OCTRL1_LTIM);
    reg_write(DIP_OCTRL2, DIP_OCTRL2_DPO | DIP_OCTRL2_DPX | DIP_OCTRL2_DOT);
    reg_write(DIP_OCTRL3, DIP_OCTRL3_CKSEL_TTL_OUTPUT); // this is makes no sense

    reg_write(DIP_OSCTRL, 0);

    reg_write(DIP_ODRV, DIP_ODRV_EVENDRV_6mA | DIP_ODRV_ODDDRV_6mA | DIP_ODRV_CLKDRV_6mA | DIP_ODRV_DEDRV_6mA);

    reg_write(DIP_PLLCTRL1, 0);
    reg_write(DIP_PLLCTRL2, 0);

    reg_write(DIP_MDCTRL, DIP_MDCTRL_IFI_FLAG);

    reg_write(DIP_MPLL_M, DIP_MPLL_M_MAKE_CURRENT_CONTROL(3) | DIP_MPLL_M_MAKE_PLL_DIVIDER(15));
    reg_write(DIP_LPLL_M, DIP_LPLL_M_MAKE_PLL_DIVIDER(8) | DIP_LPLL_M_UNKNOWN0_FLAG);
    reg_write(DIP_LPLL_CTL2, DIP_LPLL_CTL2_LP_ICTROL_USED_UNKNOWN | DIP_LPLL_CTL2_LP_K_DIV1);
    
    reg_write16(DIP_LPLL_STEP, 0x44); // idk why this value is used here
    reg_write16(DIP_LPLL_SPAN, 0x1CC); // idk why this value is used here
}
