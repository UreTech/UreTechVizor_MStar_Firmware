#include <init/init.h>
#include <variables.h>
#include <registers.h>

void boot_init(void){
    EA = 0; // disable intterupts
    boot_init_mcu();
    boot_init_interrupts();
    boot_init_timers();

    EA = 0; // enable intterupts

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
    MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x93) = 3;
    MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x91) = 1;

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

void boot_init_uart(void){
    // idk what these for
    UART_VAR0 = 0xFF;
    UART_VAR1 = 0xA3;

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

void init_software_registers(void){
    REGBK = 0x00; // use bank0 DIP
    reg_write(DIP_SWRST, 0); // clear DIP reset register
    RD = 1;
    delay_ms(5);
    RD = 0;
    REGBK = 0x00; // use bank0 DIP
    reg_write(DIP_DBFC, 0); // double buffer disable
    reg_write(DIP_SWRST, DIP_SWRST_SWR | DIP_SWRST_OSDR | DIP_SWRST_BIUR | DIP_SWRST_DPR | DIP_SWRST_GPR | DIP_SWRST_ADCR); // reset
    delay_ms(2);
    reg_write(DIP_SWRST, 0); // clear DIP reset register
    reg_write(DIP_PDMD, DIP_PDMD_GCLK_APDLA); // power down mode
    reg_write(DIP_TEST, 0); // disable test mode

    REGBK = 0x02; // use bank2 TCON

    // this thing is probably init sequance for timers & clocks but i have no documentataion for it
    reg_write(TCON_UNKNOWN_REGISTER, 0x20);
    uint8_t read_value;
    read_value = reg_read(TCON_UNKNOWN_REGISTER);
    reg_write(TCON_UNKNOWN_REGISTER, read_value | 0x40);
    REGBK = 0x00; // use bank0 DIP

    return;
}

void init_something_0(void){
    INTMEM(0x5C) = 10;
    
}
