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

void software_reset_external_devices(void){
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

uint16_t do_something_in_MCU(void){
    UNKNOWN_MCU_REGISTER_SAVED_VAR_042E = _UNKNOWN0_MCU_register_something(0x0);

    if(UNKNOWN_MCU_REGISTER_SAVED_VAR_042E == 0xBF){
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

void init_ADC(void){
    SWITCH_ADC_BK();

    // idk what that does
    // but it look like a some sort of security check or something?
    uint8_t read_unknown_reg = reg_read(ADC_UNKNOWN_INIT_RELATED_REGISTER8);
    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER8, read_unknown_reg | 0x80);

    reg_write(ADC_CLPACE, 0x10);
    reg_write(ADC_CLDUR, 0x8);

    reg_write(ADC_FCOEF, 0x9);

    reg_write(ADC_VCOCTRL, ADC_VCOCTRL_SETCNT_USED0 | ADC_VCOCTRL_DPL_S_USED0);

    reg_write(ADC_SOG_LVL, ADC_SOG_LVL_SOG_LVL_170mV);

    reg_write(ADC_PLLCTRLV, ADC_PLLCTRLV_IQSET_TH_USED0 | ADC_PLLCTRLV_IQCLR_TH_USED0 | ADC_PLLCTRLV_WDTOL_USED0);

    // this register are probably used/named differently for TSUM1PFR
    reg_write(ADC_DVI_PHR, 0);
    reg_write(ADC_DVI_PHG, 0);
    reg_write(ADC_DVI_PHB, 0);
    reg_write(ADC_DVI_ERST, 0);
    reg_write(ADC_DVI_ERTH, ADC_DVI_ERTH_USED0); // this is weird
    reg_write(ADC_TESTEN, ADC_TESTEN_ENABLE); // also this is very weird
    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER1, 0x20); // also this is very weird
    reg_write(ADC_TESTMOD, ADC_TESTMOD_TESTMOD_USED0); // ... i have no words
    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER4, 0x0F);
    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER2, 0);
    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER6, 0x10);
    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER5, 0x40);
    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER3, 0);

    reg_write(ADC_SARCTRL3, 0b110);
    reg_write(ADC_SARCTRL4, 0b100);

    // this writes are quite interesting...
    // looks like it is trying to increase MCU clock
    // and that 0xBF value is seems like a hardware status value that indicates what could be set?
    // maybe 0xBF means hardware is supporting fast spi read?
    uint8_t final_ADC_MISC_value;
    if(UNKNOWN_MCU_REGISTER_SAVED_VAR_042E == 0xBF){
        reg_write(ADC_MISC, ADC_MISC_UNKNOWN_USED0 | ADC_MISC_MCUPLL_CLK_SEL_x1 | ADC_MISC_SPI_FRD_EN_FLAG | ADC_MISC_MCUPLL_CLK_SEL_USED0);
        delay_ms(1);
        reg_write(ADC_MISC, ADC_MISC_UNKNOWN_USED1 | ADC_MISC_MCUPLL_CLK_SEL_x2 | ADC_MISC_SPI_FRD_EN_FLAG | ADC_MISC_MCUPLL_CLK_SEL_USED0); 
        delay_ms(1);
        final_ADC_MISC_value = ADC_MISC_UNKNOWN_USED0 | ADC_MISC_MCUPLL_CLK_SEL_x2 | ADC_MISC_SPI_FRD_EN_FLAG | ADC_MISC_MCUPLL_CLK_SEL_USED0;
    }else{
        reg_write(ADC_MISC, ADC_MISC_UNKNOWN_USED0 | ADC_MISC_MCUPLL_CLK_SEL_x1 | ADC_MISC_MCUPLL_CLK_SEL_USED0);
        delay_ms(1);
        reg_write(ADC_MISC, ADC_MISC_UNKNOWN_USED1 | ADC_MISC_MCUPLL_CLK_SEL_x2 | ADC_MISC_MCUPLL_CLK_SEL_USED0); 
        delay_ms(1);
        final_ADC_MISC_value = ADC_MISC_UNKNOWN_USED0 | ADC_MISC_MCUPLL_CLK_SEL_x2 | ADC_MISC_MCUPLL_CLK_SEL_USED0;
    }
    reg_write(ADC_MISC, final_ADC_MISC_value);

    reg_write(ADC_SARCTRL1, ADC_SARCTRL1_SA_SMPMD_FLAG);

    reg_write(ADC_GPIO_I_SEL, reg_read(ADC_GPIO_I_SEL) | 0x0C); // idk what that is

    reg_write(ADC_UNKNOWN_INIT_RELATED_REGISTER7, 0x18); // ???

    reg_write(ADC_TESTEN, 0);

    SWITCH_DIP_BK();
    return;
}

void init_TCON(void){
    SWITCH_TCON_BK();

    reg_write(TCON_OFC1, TCON_OFC1_EEF_FLAG);
    reg_write16(TCON_GAVST, 0x06); // ???
    reg_write16(TCON_GAVEND, 0x0408); // ???
    reg_write(TCON_CACTRL, TCON_CACTRL_GAES_FLAG);

    SWITCH_DIP_BK();

    return;
}

void init_DIP(void){
    software_reset_external_devices();
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
    reg_write16(DIP_LPLL_SPAN, 0x01CC); // idk why this value is used here

    reg_write(DIP_DITHCTRL, 0);
    reg_write(DIP_DITHCOEF, DIP_DITHCOEF_TL_11 | DIP_DITHCOEF_TR_10 | DIP_DITHCOEF_BL_01 | DIP_DITHCOEF_BR_00);
    reg_write(DIP_TRFN, DIP_TRFN_DT1_FLAG | DIP_TRFN_DRT_FLAG);

    reg_write(DIP_DVI_DDCEN, 0); // ??? maybe for being sure the firmware does not break the dvi enabled SoC's?

    reg_write(DIP_MISCFC, DIP_MISCFC_DEGP_FLAG | DIP_MISCFC_VSGR_FLAG | DIP_MISCFC_AFT_FLAG);

    reg_write(DIP_BRC, DIP_BRC_BRC_FLAG);

    reg_write(DIP_CNTR, DIP_CNTR_CNTR_FLAG | DIP_CNTR_CNREN);

    reg_write(DIP_FWC, 0);

    reg_write(DIP_FTAPEN, DIP_FTAPEN_FFSEL_3TAP);
    reg_write(DIP_sRGB12, 0);
    reg_write(DIP_sRGB13, 0x40);
    reg_write(DIP_sRGB21, 0x88);
    reg_write(DIP_sRGB23, 0x48);

    reg_write(DIP_ATOCTRL, DIP_ATOCTRL_ATOE_FLAG); // auto pos
    reg_write(DIP_ATPCTRL, DIP_ATPCTRL_ATPE_FLAG | DIP_ATPCTRL_APMASK_USED); // auto phase
    reg_write(DIP_ATGCTRL, 0); // auto gain (disabled)
    reg_write(DIP_ATPTTH, 0x1C); // auto phase related
    reg_write(DIP_AOVDV, 0x40); // auto pos related

    reg_write(DIP_LMARGINH, 0x30);
    reg_write(DIP_LMARGINSCC, 0x20);

    reg_write(DIP_INTCTROL, DIP_INTCTROL_HSPM_FLAG); // interrupt control (this is makes no sense but firmware does it)
    reg_write(DIP_INTPULSE, 0x0F);
    reg_write(DIP_INTENA, 0);

    reg_write(DIP_HSTOL, DIP_HSTOL_HSTOL_USED1);
    reg_write(DIP_VSTOL, DIP_VSTOL_VSTOL_USED1);

    reg_write(DIP_HSPRD_HIGH_BYTE, 0); // ???

    reg_write(DIP_DVICKTH, 0x1E); // im not sure this register is related to dvi

    UNKNOWN_VAR_0409 = reg_read(DIP_UNKNOWN_INIT_SAVED_REGISTER);

    init_ADC();

}
