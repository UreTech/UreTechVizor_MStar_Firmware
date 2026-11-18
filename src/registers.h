#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#define ONEBIT(x) (uint8_t)(0x1 << (x))

#define MAKE_REG(reg_addr) (*(volatile __xdata unsigned char *)(reg_addr))

// TSUM1PFR register base address
#define REGISTER_BASE_ADDRESS (0xC000)

// MCU control registers
#define MCU_CONTROL_REGISTER_WD_SWZ1 MAKE_REG(REGISTER_BASE_ADDRESS + 0x8) // watchdog timer software disable key low byte
#define MCU_CONTROL_REGISTER_WD_SWZ2 MAKE_REG(REGISTER_BASE_ADDRESS + 0x9) // watchdog timer software disable key high byte
#define MCU_CONTROL_REGISTER_DDC2BI_CTRL MAKE_REG(REGISTER_BASE_ADDRESS + 0x18) // maybe for dvi? i didnt confirmed yet, this is for edid i think
#define MCU_CONTROL_REGISTER_MCU_P0_OUT_EN MAKE_REG(REGISTER_BASE_ADDRESS + 0x30) // P0 enable force mode
#define MCU_CONTROL_REGISTER_MCU_P0_FMD MAKE_REG(REGISTER_BASE_ADDRESS + 0x31) // P0 force mode (0: input, 1: output)
#define MCU_CONTROL_REGISTER_MCU_P1_OUT_EN MAKE_REG(REGISTER_BASE_ADDRESS + 0x33) // P1 enable force mode
#define MCU_CONTROL_REGISTER_MCU_P1_FMD MAKE_REG(REGISTER_BASE_ADDRESS + 0x34) // P1 force mode (0: input, 1: output)
#define MCU_CONTROL_REGISTER_MCU_P2_OUT_EN MAKE_REG(REGISTER_BASE_ADDRESS + 0x36) // P2 enable output? or force mode?
#define MCU_CONTROL_REGISTER_MCU_P2_FMD MAKE_REG(REGISTER_BASE_ADDRESS + 0x37) // P2 force mode (0: input, 1: output)

#define DDC2BI_CTRL_EN_HOLD_CK ONEBIT(0)
#define DDC2BI_CTRL_EN_NO_ACK ONEBIT(1)
#define DDC2BI_CTRL_STD_DVI_IO ONEBIT(4)
#define DDC2BI_CTRL_STD_ADC_IO ONEBIT(5)
#define DDC2BI_CTRL_STD_ROM_IO ONEBIT(6)

#define PCON_SMOD ONEBIT(7) // uart double bauderate?

#define SCON_RI ONEBIT(0) // receive completed flag
#define SCON_TI ONEBIT(1) // trasmite completed flag
#define SCON_RB8 ONEBIT(2) // 9th bit
#define SCON_TB8 ONEBIT(3) // 9th bit
#define SCON_REN ONEBIT(4) // uart receiver enable
#define SCON_SM2 ONEBIT(5) // multiprocessor comm
#define SCON_SM1 ONEBIT(6) // UART mode1
#define SCON_SM0 ONEBIT(7) // UART mode

// SFRs (standard 8051)
__sfr __at(0x80) P0;
__sfr __at(0x90) P1;
__sfr __at(0xA0) P2;
__sfr __at(0xB0) P3; // P3.5 = flash bank select (A16)
__sfr __at(0xA8) IE;
__sfr __at(0xEA) CCAP1L;
__sbit __at(0xAF) EA;
__sfr __at(0x88) TCON;
__sbit __at(0x88) IT0;
__sbit __at(0x89) IE0;
__sbit __at(0x8A) IT1;
__sbit __at(0x8B) IE1;
__sbit __at(0xA8) EX0;
__sbit __at(0xAA) EX1;
__sfr __at(0xB8) IP;
__sbit __at(0xB8) PX0;
__sbit __at(0xBA) PX1;
__sbit __at(0xA9) ET0;
__sfr __at(0x8C) TH0;
__sfr __at(0x8A) TL0;
__sbit __at(0x8D) TF0;
__sbit __at(0xAB) ET1;
__sfr __at(0x8D) TH1;
__sfr __at(0x8B) TL1;
__sbit __at(0x8F) TF1;
__sfr __at(0x89) TMOD;
__sbit __at(0x8C) TR0;
__sbit __at(0x8E) TR1;
__sfr __at(0x87) PCON;
__sfr __at(0x98) SCON;
__sbit __at(0x98) RI;
__sbit __at(0x99) TI;
__sbit __at(0xAC) ES;
__sbit __at(0xB7) RD;


#define TMOD_M0_0 ONEBIT(0) // 16bit timer
#define TMOD_M1_0 ONEBIT(1) // timer0 mode1
#define TMOD_T0 ONEBIT(2) // timer0
#define TMOD_GATE0 ONEBIT(3) // timer0 controlled by TR0
#define TMOD_M0_1 ONEBIT(4) // 8bit auto reload
#define TMOD_M1_1 ONEBIT(5) // timer1 mode2
#define TMOD_T1 ONEBIT(6) // timer1
#define TMOD_GATE1 ONEBIT(7) // timer0 controlled by TR1

// scaler register window: XDATA 0x0000..0x00FF, index 0 = REGBK
__xdata __at(0x0000) volatile uint8_t SC[256];
#define REGBK   SC[0x00]

#define REGBK_PORR ONEBIT(7) // power on reset ready
#define REGBK_B6 ONEBIT(6) // idk what is this
#define REGBK_B5 ONEBIT(5) // idk what is this
#define REGBK_B4 ONEBIT(4) // idk what is this

static void reg_write(uint8_t reg, uint8_t val){
    SC[reg] = val;
}
static uint8_t reg_read(uint8_t reg){ 
    return SC[reg];
}

__xdata __at(0x0400) volatile uint8_t EXT_MEM[1024];

// hardware registers

// BANK 0 - DIP
#define DIP_DBFC (0x01)

#define DIP_PDMD (0xF0) // power down mode register
#define DIP_PDMD_NORMAL (0b00) // normal
#define DIP_PDMD_OSD_ONLY (0b01) // only osd (for no input signal screen)
#define DIP_PDMD_PD0 (0b10) // BIU, mode detection, GOUT are functional
#define DIP_PDMD_PD0 (0b11) // all chip power down
#define DIP_PDMD_GCLK_NORMAL (0b00) // normal
#define DIP_PDMD_GCLK_VBLANK (0b01) // vblank for sram?
#define DIP_PDMD_GCLK_HVBLANK (0b10) // h & v blank
#define DIP_PDMD_GCLK_RESERVED (0b11) // idk
#define DIP_PDMD_GCLK_PDDS ONEBIT(4) // power down ddc ram (1: power down)
#define DIP_PDMD_GCLK_PHSRM ONEBIT(5) // power down HDCP SRAM
#define DIP_PDMD_GCLK_APDLA ONEBIT(6) // auto power down when low power using analog pin
#define DIP_PDMD_GCLK_APDLD ONEBIT(7) // auto power down when low power using digital pin

#define DIP_SWRST (0xF1) // software reset register
#define DIP_SWRST_SWR ONEBIT(0) // software reset
#define DIP_SWRST_OSDR ONEBIT(1) // internal osd reset
#define DIP_SWRST_BIUR ONEBIT(2) // bus reset?
#define DIP_SWRST_DPR ONEBIT(3) // display port reset
#define DIP_SWRST_GPR ONEBIT(4) // graphics port reset
#define DIP_SWRST_ADCR ONEBIT(5) // ADC reset
#define DIP_SWRST_UNKNOWN ONEBIT(6) // i have no idea
#define DIP_SWRST_DPDMD ONEBIT(7) // deep power down mode (1: enable, 0: disable)

#define DIP_TEST (0xF8) // test register for self test?
#define DIP_TEST_TESTMD (0b0110) // VS/HS/DE output while LVDS output
// other: i have no idea

#define TCON_UNKNOWN_REGISTER (0x77) // it is used in init sequance but i  have  no idea what that does


#endif