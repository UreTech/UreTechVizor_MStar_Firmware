#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#define ONEBIT(x) (uint8_t)(0x1 << (x))

#define MAKE_REG(reg_addr) (*(volatile __xdata unsigned char *)(reg_addr))

// TSUM1PFR MCU register base address
#define MCU_REGISTER_BASE_ADDRESS (0xC000)

// MCU control registers
#define MCU_CONTROL_REGISTER_WD_SWZ1 MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x8) // watchdog timer software disable key low byte
#define MCU_CONTROL_REGISTER_WD_SWZ2 MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x9) // watchdog timer software disable key high byte

#define MCU_CONTROL_REGISTER_ADC_INT_FLAG MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x11) // interrupt flags
#define MCU_CONTROL_REGISTER_ADC_INT_ID_FLAG ONEBIT(0)
#define MCU_CONTROL_REGISTER_ADC_INT_ADR_FLAG ONEBIT(1)
#define MCU_CONTROL_REGISTER_ADC_INT_RW_FLAG ONEBIT(2)
#define MCU_CONTROL_REGISTER_ADC_INT_DATW_FLAG ONEBIT(3)
#define MCU_CONTROL_REGISTER_ADC_INT_DATR_FLAG ONEBIT(4)
#define MCU_CONTROL_REGISTER_ADC_INT_STOP_FLAG ONEBIT(5)
#define MCU_CONTROL_REGISTER_ADC_INT_START_FLAG ONEBIT(6)

#define MCU_CONTROL_REGISTER_ADC_WBUF_RDP MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x12) // DDCCI
#define MCU_CONTROL_REGISTER_ADC_RBUF_WDP MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x13) // DDCCI
#define MCU_CONTROL_REGISTER_DDC2BI_CTRL MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x18) // maybe for dvi? i didnt confirmed yet, this is for edid i think
#define MCU_CONTROL_REGISTER_MCU_P0_OUT_EN MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x30) // P0 enable force mode
#define MCU_CONTROL_REGISTER_MCU_P0_FMD MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x31) // P0 force mode (0: input, 1: output)
#define MCU_CONTROL_REGISTER_MCU_P1_OUT_EN MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x33) // P1 enable force mode
#define MCU_CONTROL_REGISTER_MCU_P1_FMD MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x34) // P1 force mode (0: input, 1: output)
#define MCU_CONTROL_REGISTER_MCU_P2_OUT_EN MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x36) // P2 enable output? or force mode?
#define MCU_CONTROL_REGISTER_MCU_P2_FMD MAKE_REG(MCU_REGISTER_BASE_ADDRESS + 0x37) // P2 force mode (0: input, 1: output)

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
#define SWITCH_DIP_BK() REGBK = 0x00;
#define SWITCH_ADC_BK() REGBK = 0x01;
#define SWITCH_TCON_BK() REGBK = 0x02;
#define SWITCH_MWE_BK() REGBK = 0x03;

#define REGBK_PORR ONEBIT(7) // power on reset ready
#define REGBK_B6 ONEBIT(6) // idk what is this
#define REGBK_B5 ONEBIT(5) // idk what is this
#define REGBK_B4 ONEBIT(4) // idk what is this

static void reg_write(uint8_t reg, uint8_t val){
    SC[reg] = val;
}
static void reg_write16(uint8_t reg, uint16_t val){
    SC[reg] = val;
    SC[reg + 1] = (val >> 8);
}
static uint8_t reg_read(uint8_t reg){ 
    return SC[reg];
}
static uint16_t reg_read16(uint8_t reg){ 
    return (uint16_t)SC[reg] | ((uint16_t)SC[reg + 1] << 8);
}

__xdata __at(0x0400) volatile uint8_t EXT_MEM[1024];

// hardware registers

// BANK 0 - DIP
#define DIP_DBFC (0x01) // double buffer control

#define DIP_DEVST (0x10) // Output DE Vertical Start (16bit: 0x10 & 0x11)
#define DIP_DEHST (0x12) // Output DE Horizontal Start (16bit: 0x12 & 0x13)
#define DIP_DEVEND (0x14) // Output DE Vertical End (16bit: 0x14 & 0x15)
#define DIP_DEHEND (0x16) // Output DE Horizontal End (16bit: 0x16 & 0x17)
#define DIP_OIHST (0x18) // Output Image window Horizontal Start (16bit: 0x18 & 0x19)
#define DIP_OIVEND (0x1A) // Output Image window Vertical END (16bit: 0x1A & 0x1B)
#define DIP_OIHEND (0x1C) // Output Image window Horizontal END (16bit: 0x1C & 0x1D)
#define DIP_VDTOT (0x1E) // Output Vertical Total (16bit: 0x1E & 0x1F)
#define DIP_VSST (0x20) // Output VSYNC start (only useful when AOVS=1) (16bit: 0x20 & 0x21)
#define DIP_VSEND (0x22) // Output VSYNC END (only useful when AOVS=1) (16bit: 0x20 & 0x21)
#define DIP_HDTOT (0x24) // Output Horizontal Total (16bit: 0x24 & 0x25)

#define DIP_HSEND (0x26) // Output HSYNC Pulse width

#define DIP_OSCTRL1 (0x27) // Output signal control register 1?
#define DIP_OSCTRL1_CTRL_FLAG ONEBIT(0)
#define DIP_OSCTRL1_AHRT_FLAG ONEBIT(1) // Auto H total and Read start Tuning enable
#define DIP_OSCTRL1_EHTT_FLAG ONEBIT(3) // Even H Total (1: output h always odd pixels)
#define DIP_OSCTRL1_VSGP_FLAG ONEBIT(4) // VSYNC use GPO9 (1: enable (using Bank 2 register 0x59 – 0x61 to define OVSYNC))
#define DIP_OSCTRL1_HRSM_FLAG ONEBIT(5) // HSYNC Remove Mode (1: Remove HSYNC when GPOA (Bank 2 register 0x62 – 0x6A) is low)
#define DIP_OSCTRL1_UNKNOWN0_FLAG ONEBIT(6) // it is used but idk what that is
#define DIP_OSCTRL1_AOVS_FLAG ONEBIT(7) // Auto Output VSYNC (0: OVSYNC is defined automatically)

#define DIP_OSCTRL2 (0x28) // Output signal control register 2?
#define DIP_OSCTRL2_UNKNOWN0_FLAG ONEBIT(5) // it is used but idk what that is

#define DIP_BRC (0x2A) // Brightness control register?
#define DIP_BRC_BRC_FLAG ONEBIT(0) // Brightness function, reference to register 2Bh, 2Ch, and 2Eh (0: off)

#define DIP_CNTR (0x2E) // Contrast register?
#define DIP_CNTR_CNTR_FLAG ONEBIT(0) // Contrast function (0: off)
#define DIP_CNTR_CNTT_FLAG ONEBIT(1) // Contrast Type select (0: Use 0 as center point, Use 128 as center point)
#define DIP_CNTR_CCLB_FLAG ONEBIT(2) // Contrast Coefficient LSB – Blue color
#define DIP_CNTR_CCLG_FLAG ONEBIT(3) // Contrast Coefficient LSB – Green color
#define DIP_CNTR_CCLR_FLAG ONEBIT(4) // Contrast Coefficient LSB – Red color
#define DIP_CNTR_CNREN (ONEBIT(5) | ONEBIT(6)) // Contrast Noise Rounding Enable

#define DIP_FWC (0x32) // border color control register?
#define DIP_FWC_FWC_FLAG ONEBIT(0) // Border Color (will be used when output is in free-run mode) (0: off)

#define DIP_DITHCTRL (0x36) // Dither control register?
#define DIP_DITHCTRL_DITH_FLAG ONEBIT(0) // Dither function enable
#define DIP_DITHCTRL_OBN_FLAG ONEBIT(1) // Output Bits Number (used for 8/10-bit gamma, 0: 8bit, 1: 6bit)
#define DIP_DITHCTRL_TROT_FLAG ONEBIT(2) // Temporal coefficient Rotate
#define DIP_DITHCTRL_SROT_FLAG ONEBIT(3) // Spatial coefficient Rotate
#define DIP_DITHCTRL_DITHB_MASK (0b00110000) // Dither coefficient for B channel
#define DIP_DITHCTRL_DITHB_00   (0b00000000)
#define DIP_DITHCTRL_DITHB_01   (0b00010000)
#define DIP_DITHCTRL_DITHB_10   (0b00100000)
#define DIP_DITHCTRL_DITHB_11   (0b00110000)
#define DIP_DITHCTRL_DITHG_MASK (0b11000000) // Dither coefficient for G channel
#define DIP_DITHCTRL_DITHG_00   (0b00000000)
#define DIP_DITHCTRL_DITHG_01   (0b01000000)
#define DIP_DITHCTRL_DITHG_10   (0b10000000)
#define DIP_DITHCTRL_DITHG_11   (0b11000000)

#define DIP_DITHCOEF (0x37) // Dither coefficienty
#define DIP_DITHCOEF_TL_11 (0b11000000) // top left
#define DIP_DITHCOEF_TR_10 (0b00100000) // top right
#define DIP_DITHCOEF_BL_01 (0b00000100) // bottom left
#define DIP_DITHCOEF_BR_00 (0b00000000) // bottom right

#define DIP_TRFN (0x38) // dither type select register?
#define DIP_TRFN_TDFNC_FLAG ONEBIT(0) // Tempo-Dither Frame Number Control (0: Tempo-dither every frame, 1: Tempo-dither every 2 frames)
#define DIP_TRFN_DT1_FLAG ONEBIT(1) // Dither Type 1 (0: Normal, 1: Output data bits 1 and 0 are always 00)
#define DIP_TRFN_DT2_FLAG ONEBIT(2) // Dither Type 2 (0: Output data bits 1 and 0 according to input pixel value, 1: Output data bits 2, 1 and 0 according to input pixel value)
#define DIP_TRFN_DT3_FLAG ONEBIT(3) // Dither Type 2 control (0: Disable dither type 2, 1: Enable dither type 2)
#define DIP_TRFN_DRT_FLAG ONEBIT(4) // Dither Rotate Type (0: EOR, 1: Rotate)
#define DIP_TRFN_DATP_FLAG ONEBIT(5) // Dither based on Auto Phase threshold (0: disable)
#define DIP_TRFN_NDMD_FLAG ONEBIT(6) // Noise Dithering Method
#define DIP_TRFN_PSRD_FLAG ONEBIT(7) // Pseudo Random, resets every 4 frames (0: enable)

#define DIP_OCTRL1 (0x42) // Output control register 1?
#define DIP_OCTRL1_ERBX ONEBIT(0) // Even channel Red/Blue bus Exchange
#define DIP_OCTRL1_ORBX ONEBIT(1) // Odd channel Red/Blue bus Exchange
#define DIP_OCTRL1_EMLX ONEBIT(2) // Even channel MSB/LSB Exchange
#define DIP_OCTRL1_OMLX ONEBIT(3) // Odd channel MSB/LSB Exchange
#define DIP_OCTRL1_LTIM ONEBIT(4) // LVDS TI Mode Enable
#define DIP_OCTRL1_MLXT ONEBIT(5) // MSB/LSB Exchange Type (0: Always reverse bit[7:0], 1: Reverse bit[7:2] when 6-bit panel)
#define DIP_OCTRL1_LCS ONEBIT(6) // LVDS Channel Swap
#define DIP_OCTRL1_LCPS ONEBIT(7) // LVDS Channel Polarity Swap (P/N swap)

#define DIP_OCTRL2 (0x43) // Output control register 2?
#define DIP_OCTRL2_DPO ONEBIT(0) // Dual Pixel Output (1: Dual pixel)
#define DIP_OCTRL2_DPX ONEBIT(1) // A/B Port Swap (only used when DPO=1)
#define DIP_OCTRL2_STO ONEBIT(2) // Stagger Output (only used when DPO=1)
#define DIP_OCTRL2_REV ONEBIT(3) // Reverse luminosity (1: on)
#define DIP_OCTRL2_BLSK ONEBIT(4) // Black Screen (screen off)
#define DIP_OCTRL2_WHTS ONEBIT(5) // White Screen (screen off)
#define DIP_OCTRL2_DOT ONEBIT(6) // Differential Output Type (0: LVDS/RSDS, 1: Reduced-swing LVDS/increased-swing RSDS)
#define DIP_OCTRL2_TCOP ONEBIT(7) // TCON Control Pin port select (only used when )BN=1, 6-bit output)

#define DIP_OCTRL3 (0x44) // Output control register 3???
#define DIP_OCTRL3_CKSEL_MASK (0b11111)
#define DIP_OCTRL3_CKSEL_TTL_OUTPUT (0x0)
#define DIP_OCTRL3_CKSEL_LVDS_OUTPUT (0x1)

#define DIP_FTAPEN (0x65) // Filtering related register?
#define DIP_FTAPEN_FFSEL_MASK (0b00000011) // Filter Function Select
#define DIP_FTAPEN_FFSEL_DISABLE (0b00000000) // disable
#define DIP_FTAPEN_FFSEL_3TAP    (0b00000001) // Enable 3 tap function
#define DIP_FTAPEN_FFSEL_EN_sRGB (0b00000010) // Enable sRGB function
#define DIP_FTAPEN_TPP_FLAG ONEBIT(2) // Test Pattern Position (0: After sRGB, 1: Before sRGB)
#define DIP_FTAPEN_sRGBG_FLAG ONEBIT(3) // sRGB go through Gamma (0: Bypass gamma, 1: Go to gamma)
#define DIP_FTAPEN_sRBGP_FLAG ONEBIT(4) // sRGB Precision (0: Normal, 1: Shift 2 bits)
#define DIP_FTAPEN_sRGBNE_FLAG ONEBIT(5) // sRGB Noise round Enable

#define DIP_sRGB12 (0x66) // Coefficient 12, 1 sign bit, 7 bits
#define DIP_sRGB13 (0x67) // Coefficient 13, 1 sign bit, 7 bits
#define DIP_sRGB21 (0x68) // Coefficient 21, 1 sign bit, 7 bits
#define DIP_sRGB23 (0x69) // Coefficient 23, 1 sign bit, 7 bits

#define DIP_ATGCTRL (0x78) // Auto gain control register?
#define DIP_ATGCTRL_ATGE_FLAG ONEBIT(0) // Auto Gain Function Enable
#define DIP_ATGCTRL_ATGR_FLAG ONEBIT(1) // Auto Gain Result Ready (1: Result ready)
#define DIP_ATGCTRL_ATGM_FLAG ONEBIT(2) // Auto Gain Mode (0: Normal mode (result will be cleared every frame), 1: History mode (result remains not cleared till ATGE = 0))
#define DIP_ATGCTRL_AGR_FLAG ONEBIT(3) // Auto Gain Result selection (0: Output has max/min value, 1: Output is overflow/underflow)
#define DIP_ATGCTRL_ACE_FLAG ONEBIT(4) // ADC Calibration Enable
#define DIP_ATGCTRL_MAXB_FLAG ONEBIT(5) // Max value flag for blue channel (read only) (0: Normal; 1: Max value (255) value when AGR = 0, Output over max value (255) when AGR = 1)
#define DIP_ATGCTRL_MAXG_FLAG ONEBIT(6) // Max value flag for green channel (read only) (0: Normal; 1: Max value (255) value when AGR = 0, Output over max value (255) when AGR = 1)
#define DIP_ATGCTRL_MAXR_FLAG ONEBIT(7) // Max value flag for red channel (read only) (0: Normal; 1: Max value (255) value when AGR = 0, Output over max value (255) when AGR = 1)

#define DIP_ATOCTRL (0x7B) // Auto position control register?
#define DIP_ATOCTRL_ATOE_FLAG ONEBIT(0) // Auto position function Enable (Disable-to-enable needs at least 2 frame apart for ready bit to settle)
#define DIP_ATOCTRL_ATOR_FLAG ONEBIT(1) // Auto position result Ready (1: Result not ready)
#define DIP_ATOCTRL_ATOM_FLAG ONEBIT(2) // Auto position function Mode (0: Update every frame, 1: Keep the history value)
#define DIP_ATOCTRL_JITR_FLAG ONEBIT(3) // Jitter function Result (0: No jitter, 1: Jitter present)
#define DIP_ATOCTRL_JITM_FLAG ONEBIT(4) // Jitter function Mode (0: Update every frame, 1: Keep the history value)
#define DIP_ATOCTRL_JITS_FLAG ONEBIT(6) // Jitter Software clear (1: Clear)
#define DIP_ATOCTRL_JITLR_FLAG ONEBIT(7) // Jitter function Left / Right result for 86h and 87h (0: Left result, 1: right result)

#define DIP_AOVDV (0x7C) // Auto position Valid Data Value (valid if data >= DIP_AOVDV)

#define DIP_ATPTTH (0x8A) // Auto Phase Text Threshold for ATPV4

#define DIP_ATPCTRL (0x8B) // Auto phase control register?
#define DIP_ATPCTRL_ATPE_FLAG ONEBIT(0) // Auto Phase function Enable
#define DIP_ATPCTRL_ATPR_FLAG ONEBIT(1) // Auto Phase Result ready (1: Result ready)
#define DIP_ATPCTRL_APMASK_MASK (0b00011100) // Nose Mask
#define DIP_ATPCTRL_APMASK_USED (0b00011000) // used in firmware
#define DIP_ATPCTRL_TXT_FLAG ONEBIT(5) // Text detect (read only)
#define DIP_ATPCTRL_GRY_FLAG ONEBIT(6) // Gray scale detect (read only)

#define DIP_LMARGINH (0x9B) // Locking H Total Margin

#define DIP_LMARGINSCC (0x9E) // Locking SSC Margin

#define DIP_DVI_DDCEN (0xB3) // Not used for only analog input controllers like TSUM1PFR

#define DIP_MISCFC (0xBC) // Misc frame control register?
#define DIP_MISCFC_DEGP_FLAG ONEBIT(2) // DE only mode Glitch Protect for position
#define DIP_MISCFC_LBGC_FLAG ONEBIT(3) // LB Clock no gating mode
#define DIP_MISCFC_VSP_FLAG ONEBIT(4) // VSYNC Protect with V total (DE only)
#define DIP_MISCFC_VSGR_FLAG ONEBIT(5) // VSYNC glitch removal with line less than 2 (DE only)
#define DIP_MISCFC_IDHTT_FLAG ONEBIT(6) // DE only mode HTT count by IDCLK
#define DIP_MISCFC_AFT_FLAG ONEBIT(7) // ATP Filter for Text (4 frames)

#define DIP_INTCTROL (0xCA) // Interrupt control register?
#define DIP_INTCTROL_INTT_FLAG ONEBIT(0) // Interrupt Trigger (0: Edge trigger, 1: Level trigger)
#define DIP_INTCTROL_TRGC_FLAG ONEBIT(1) // Trigger Condition (0: Active low for level trigger / tailing edge trigger, 1: Active high for level trigger / leading edge trigger)
#define DIP_INTCTROL_OVSI_FLAG ONEBIT(2) // Output VSYNC interrupt generated by (0: Leading edge, 1: Tailing edge)
#define DIP_INTCTROL_IVSI_FLAG ONEBIT(3) // Input VSYNC Interrupted generated by (0: Leading edge, 1: Tailing edge)
#define DIP_INTCTROL_HSST_FLAG ONEBIT(4) // HS Status (read only) (0: Stable, 1: Not stable)
#define DIP_INTCTROL_HSPM_FLAG ONEBIT(5) // ADC Mode: HSYNC Pin Monitor (read only) (0: HSYNC pin is low, 1: HSYNC pin is high)
#define DIP_INTCTROL_DCMD_FLAG ONEBIT(6) // DVI Clock Missing Detected (not used for only analog SoC's)
#define DIP_INTCTROL_HCHGM_FLAG ONEBIT(7) // HSYNC Changing detect Method (0: Interrupt only occurred at start and end of transition, 1: interrupt occurred at every line)

#define DIP_INTPULSE (0xCB) // Interrupt Pulse Width by reference clock

#define DIP_INTENA (0xCE) // Interrupt Enable control byte A (0: disable interrupt)

#define DIP_PLLCTRL1 (0xD0) // PLL control register 1?
#define DIP_PLLCTRL1_LRTM_FLAG ONEBIT(0) // LVDS/RSDS Test Mode enable
#define DIP_PLLCTRL1_PTEN_FLAG ONEBIT(1) // PLL Test register protect bit
#define DIP_PLLCTRL1_TSTM_FLAG ONEBIT(2) // Test Mode
#define DIP_PLLCTRL1_BPM_FLAG ONEBIT(3) // Bypass clock Mode (IDCLK as ODCLK) (0: disable)
#define DIP_PLLCTRL1_XDIV_MASK (0b00110000) // XTAL clock Divided by
#define DIP_PLLCTRL1_XDIV_16   (0b00000000)
#define DIP_PLLCTRL1_XDIV_08   (0b00010000)
#define DIP_PLLCTRL1_XDIV_04   (0b00100000)
#define DIP_PLLCTRL1_XDIV_01   (0b00110000)
#define DIP_PLLCTRL1_EOCK_FLAG ONEBIT(6) // Use External Clock (pin #) as Output Dot Clock (1: use external dot clock)
#define DIP_PLLCTRL1_XOUT_FLAG ONEBIT(7) // Enable PWM1 as XTAL clock output

#define DIP_PLLCTRL2 (0xD1) // PLL control register 2?
#define DIP_PLLCTRL2_MP_PD ONEBIT(0) // Master PLL Power Down
#define DIP_PLLCTRL2_MP_RST ONEBIT(1) // Master PLL Reset
#define DIP_PLLCTRL2_MP_PORT ONEBIT(2) // Master PLL Power On Reset
#define DIP_PLLCTRL2_MP_K ONEBIT(3) // Master PLL output frequency divided by 2
#define DIP_PLLCTRL2_LP_PD ONEBIT(4) // Output PLL Power Down
#define DIP_PLLCTRL2_LP_RST ONEBIT(5) // Output PLL Reset
#define DIP_PLLCTRL2_LP_POR ONEBIT(6) // Output PLL Power On Reset
#define DIP_PLLCTRL2_MPPDIV ONEBIT(7) // Master PLL Post Divider (0: div 3 (143 MHz) ; 1: div 2.5 (172 MHz), for output dot clock higher than 143 MHz (vertical = 85 MHz))

#define DIP_MPLL_M (0xD2) // Master PLL register?
#define DIP_MPLL_M_CURRENT_CONTROL_MASK (0b11100000) // current control?
#define DIP_MPLL_M_MAKE_CURRENT_CONTROL(val) (((uint8_t)val << 5) & DIP_MPLL_M_CURRENT_CONTROL_MASK)
#define DIP_MPLL_M_PLL_DIVIDER_MASK     (0b00011111) // pll div control?
#define DIP_MPLL_M_MAKE_PLL_DIVIDER(val) ((uint8_t)val & DIP_MPLL_M_PLL_DIVIDER_MASK)

#define DIP_LPLL_M (0xD3) // Output PLL register?
#define DIP_LPLL_M_PLL_DIVIDER_MASK     (0b00011111) // pll div control?
#define DIP_LPLL_M_MAKE_PLL_DIVIDER(val) ((uint8_t)val & DIP_LPLL_M_PLL_DIVIDER_MASK)
#define DIP_LPLL_M_SDMD_FLAG ONEBIT(5) // Output PLL spread spectrum Mode (0: Normal, 1: Reverse for mode 1)
#define DIP_LPLL_M_UNKNOWN0_FLAG ONEBIT(6) // it is used but idk what that is

#define DIP_LPLL_CTL2 (0xD4) // Output PLL control register 2?
#define DIP_LPLL_CTL2_LP_ICTROL_MASK (0b00000111) // Output PLL Current Control
#define DIP_LPLL_CTL2_LP_ICTROL_USED_UNKNOWN (0b00000011) // it is used but idk why this value
#define DIP_LPLL_CTL2_LP_K_MASK      (0b00011000) // Output PLL divider 2
#define DIP_LPLL_CTL2_LP_K_DIV8      (0b00000000)
#define DIP_LPLL_CTL2_LP_K_DIV4      (0b00001000)
#define DIP_LPLL_CTL2_LP_K_DIV2      (0b00010000)
#define DIP_LPLL_CTL2_LP_K_DIV1      (0b00011000)
#define DIP_LPLL_CTL2_LP_TP_FLAG ONEBIT(5) // Output PLL Type (0: LVDS, 1: RSDS/TTL)

#define DIP_LPLL_STEP (0xD8) // Output PLL spread spectrum Step (16bit: 0xD8 & 0xD9)
#define DIP_LPLL_SPAN (0xDA) // Output PLL spread spectrum Span (16bit: 0xDA & 0xDB)

#define DIP_HSPRD_HIGH_BYTE (0xE5) // Input HSYNC period Detect Mode??? (NOTE: I THINK THIS REGISTER IS NOT DOING THAT IN TSUM1PFR)

#define DIP_HSTOL (0xE6) // HSYNC Tolerance?
#define DIP_HSTOL_HSTOL_MASK  (0b00111111) // 5: Default value
#define DIP_HSTOL_HSTOL_USED1 (0b00000001) // used in firmware
#define DIP_HSTOL_DEF_FLAG ONEBIT(6) // DE Follow mode (for DE to DE period is not fixed)
#define DIP_HSTOL_VS2HS_FLAG ONEBIT(7) // Input VSYNC too close to input HSYNC

#define DIP_VSTOL (0xE7) // VSYNC Tolerance?
#define DIP_VSTOL_VSTOL_MASK  (0b00000111) // 1: Default value
#define DIP_VSTOL_VSTOL_USED1 (0b00000001) // used in firmware
#define DIP_VSTOL_ANG_FLAG ONEBIT(4) // Auto No signal
#define DIP_VSTOL_ANGF_FLAG ONEBIT(5) // Auto No signal Filter mode

#define DIP_MDCTRL (0xE9) // mode control register?
#define DIP_MDCTRL_IFI_FLAG ONEBIT(0) // Interlace Field Invert
#define DIP_MDCTRL_INTF_FLAG ONEBIT(1) // Interlace Field detect method select (0: Use the HSYNC numbers of a field to judge, 1: Use the relationship of VSYNC and HSYNC to judge)
#define DIP_MDCTRL_VEXF_FLAG ONEBIT(2) // Video External Field (0: Use result of internal circuit detection, 1: Use external field)
#define DIP_MDCTRL_VFIV_FLAG ONEBIT(3) // Video Field Inversion
#define DIP_MDCTRL_SCSEL0_FLAG ONEBIT(4) // soft compat?
#define DIP_MDCTRL_SCSEL1_FLAG ONEBIT(5) // soft compat?
#define DIP_MDCTRL_VERR_FLAG ONEBIT(6) // Video CCIR656 Error correct
#define DIP_MDCTRL_RCFCPB_FLAG ONEBIT(7) // Resolution conversion filter Compatible select (0: Compatible with old filter, 1: Use new filter)

#define DIP_DVICKTH (0xEB) // DVI clock detection threshold

#define DIP_PDMD (0xF0) // power down mode register
#define DIP_PDMD_NORMAL (0b00) // normal
#define DIP_PDMD_OSD_ONLY (0b01) // only osd (for no input signal screen)
#define DIP_PDMD_PD0 (0b10) // BIU, mode detection, GOUT are functional
#define DIP_PDMD_PD1 (0b11) // all chip power down
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

#define DIP_OSCTRL (0xF2) // Output signal control register ???
#define DIP_OSCTRL_OHS_FLAG ONEBIT(0) // Output HSYNC control (0: Active high, 1: Active low)
#define DIP_OSCTRL_OVS_FLAG ONEBIT(1) // Output VSYNC control (0: Active high, 1: Active low)
#define DIP_OSCTRL_ODE_FLAG ONEBIT(2) // Output DE control (0: Active high, 1: Active low)
#define DIP_OSCTRL_OCLK_FLAG ONEBIT(3) // Output CLK control (0: Active high, 1: Active low)
// ...

#define DIP_ODRV (0xF5) // Output driver register ???
#define DIP_ODRV_EVENDRV_MASK (0b00000011) // Output data Even channel Driving current select
#define DIP_ODRV_EVENDRV_4mA  (0b00000000)
#define DIP_ODRV_EVENDRV_6mA  (0b00000001)
#define DIP_ODRV_EVENDRV_8mA  (0b00000010)
#define DIP_ODRV_EVENDRV_12mA (0b00000011)
#define DIP_ODRV_ODDDRV_MASK (0b00001100) // Output data Odd channel Driving current select
#define DIP_ODRV_ODDDRV_4mA  (0b00000000)
#define DIP_ODRV_ODDDRV_6mA  (0b00000100)
#define DIP_ODRV_ODDDRV_8mA  (0b00001000)
#define DIP_ODRV_ODDDRV_12mA (0b00001100)
#define DIP_ODRV_CLKDRV_MASK (0b00110000) // Output Clock Driving current select
#define DIP_ODRV_CLKDRV_4mA  (0b00000000)
#define DIP_ODRV_CLKDRV_6mA  (0b00010000)
#define DIP_ODRV_CLKDRV_8mA  (0b00100000)
#define DIP_ODRV_CLKDRV_12mA (0b00110000)
#define DIP_ODRV_DEDRV_MASK (0b11000000) // Output DE Driving current select
#define DIP_ODRV_DEDRV_4mA  (0b00000000)
#define DIP_ODRV_DEDRV_6mA  (0b01000000)
#define DIP_ODRV_DEDRV_8mA  (0b10000000)
#define DIP_ODRV_DEDRV_12mA (0b11000000)

#define DIP_TEST (0xF8) // test register for self test?
#define DIP_TEST_TESTMD (0b0110) // VS/HS/DE output while LVDS output
// other: i have no idea

#define DIP_UNKNOWN_INIT_SAVED_REGISTER (0xFD) // this register is saved to xdata[0x409] in the DIP init


#define ADC_CLPACE (0x0A) // Clamp Placement based on ADC clock
#define ADC_CLDUR (0x0B) // Clamp Duration based on ADC clock

#define ADC_FCOEF (0x0E) // PLL loop filter control

#define ADC_VCOCTRL (0x12) // PLL loop filter control
#define ADC_VCOCTRL_SETCNT_MASK  (0b00001111) // Setting time for ADC PLL phase detector, in ADC clock periods
#define ADC_VCOCTRL_SETCNT_USED0 (0b00000101) // value used in firmware
#define ADC_VCOCTRL_DPL_S_MASK   (0b01110000) // VCO range, Sets ADC PLL frequency range
#define ADC_VCOCTRL_DPL_S_USED0  (0b00010000) // value used in firmware
#define ADC_VCOCTRL_PDGT_FLAG ONEBIT(7) // Phase digitizer frequency compensation disable

#define ADC_SOG_LVL (0x14) // SOG level
#define ADC_SOG_LVL_SOG_LVL_MASK  (0b00011111) // SOG trigger level (SOG_LVL Value = (Target / 10mV) - 1)
#define ADC_SOG_LVL_SOG_LVL_10mV  (0b00000000)
#define ADC_SOG_LVL_SOG_LVL_170mV (0b00010000)
#define ADC_SOG_LVL_SOGFLT_FLAG ONEBIT(5) // SOG Filter (low-pass filter on SOG input)
#define ADC_SOG_LVL_BMID_FLAG ONEBIT(6) // Middle clamp of Blue Channel
#define ADC_SOG_LVL_RMID_FLAG ONEBIT(7) // Middle clamp of Red Channel

// maybe this dvi registers are used differently in only analog SoC's
#define ADC_DVI_PHR (0x1B) // Freeze and override DVI red channel PLL phase selection with OVPHR[6:0] (I dont this is used in only analog SoC's)
#define ADC_DVI_PHG (0x1C) // Freeze and override DVI green channel PLL phase selection with OVPHG[6:0] (I dont this is used in only analog SoC's)
#define ADC_DVI_PHB (0x1D) // Freeze and override DVI blue channel PLL phase selection with OVPHG[6:0] (I dont this is used in only analog SoC's)
#define ADC_DVI_ERST (0x1E) // DVI bit error status indicator

#define ADC_DVI_ERTH (0x1F) // DVI bit error tolerance threshold
#define ADC_DVI_ERTH_USED0 (0x02) // value used in firmware

#define ADC_TESTEN (0x20) // Enable Test Mode
#define ADC_TESTEN_ENABLE ONEBIT(7) // enable test

#define ADC_UNKNOWN_INIT_RELATED_REGISTER1 (0x21) // ???
#define ADC_UNKNOWN_INIT_RELATED_REGISTER2 (0x23) // ???
#define ADC_UNKNOWN_INIT_RELATED_REGISTER3 (0x25) // ???

#define ADC_TESTMOD (0x2D) // Test Mode
#define ADC_TESTMOD_TESTMOD_MASK  (0b00011111) // LVDS/RSDS differential output swing control???
#define ADC_TESTMOD_TESTMOD_USED0 (0b00010011)

#define ADC_UNKNOWN_INIT_RELATED_REGISTER4 (0x2E) // ???

#define ADC_PLLCTRLV (0x30) // PLL Lock stable / watchdog
#define ADC_PLLCTRLV_IQSET_TH_MASK  (0b00000111) // PLL stable lock threshold
#define ADC_PLLCTRLV_IQSET_TH_USED0 (0b00000101) // value used in firmware
#define ADC_PLLCTRLV_IQCLR_TH_MASK  (0b00111000) // PLL unstable lock threshold
#define ADC_PLLCTRLV_IQCLR_TH_USED0 (0b00010000) // value used in firmware
#define ADC_PLLCTRLV_WDTOL_MASK     (0b11000000) // PLL Watchdog threshold
#define ADC_PLLCTRLV_WDTOL_USED0    (0b10000000) // value used in firmware

#define ADC_UNKNOWN_INIT_RELATED_REGISTER5 (0x33) // ???
#define ADC_UNKNOWN_INIT_RELATED_REGISTER6 (0x36) // ???

#define ADC_UNKNOWN_INIT_RELATED_REGISTER7 (0x3F) // ???

#define ADC_UNKNOWN_INIT_RELATED_REGISTER8 (0x50) // this register is used to do some weird shit in the ADC init

#define ADC_SARCTRL1 (0x60) // SARADC Input/GPIO Select
#define ADC_SARCTRL1_SA_SNGMD_FLAG ONEBIT(4) // SARADC Single Mode
#define ADC_SARCTRL1_SA_SMPMD_FLAG ONEBIT(5) // SARADC Sample Mode (0: One shot mode, 1: Free-run mode)
#define ADC_SARCTRL1_SA_PWRDN_FLAG ONEBIT(6) // SARADC Power Down (0: active, 1: power down)
#define ADC_SARCTRL1_SA_SMPSTS_FLAG ONEBIT(7) // SARADC Sample Status

#define ADC_SARCTRL3 (0x62) // SARADC Input/GPIO Select

#define ADC_SARCTRL4 (0x63) // SARADC Input/GPIO Select

#define ADC_MISC (0x66) // a weird register with random bullshit
#define ADC_MISC_UNKNOWN_USED0       (0b00000011) // used in firmware
#define ADC_MISC_UNKNOWN_USED1       (0b00000010) // used in firmware
#define ADC_MISC_MCUPLL_CLK_SEL_MASK (0b00001100) // MCU PLL Clock Select
#define ADC_MISC_MCUPLL_CLK_SEL_x1   (0b00000100)
#define ADC_MISC_MCUPLL_CLK_SEL_x2   (0b00001000)
#define ADC_MISC_MCUPLL_CLK_SEL_x3   (0b00001100)
#define ADC_MISC_SPI_FRD_EN_FLAG ONEBIT(4) // SPI Fast Read Enable
#define ADC_MISC_MCUPLL_CLK_SEL_MASK  (0b11100000) // SPI CSZ high Period+1 (Unit: crystal clock)
#define ADC_MISC_MCUPLL_CLK_SEL_USED0 (0b00100000) // used in firmware

#define ADC_GPIO_I_SEL (0x68) // GPIO_P07/GPIO_P06 current Select


#define TCON_OFC1 (0x02) // TCON function control register 1?
#define TCON_OFC1_TCEN_FLAG ONEBIT(0) // Timing Controller Enable
#define TCON_OFC1_EEF_FLAG ONEBIT(1) // Early End Function
#define TCON_OFC1_DPFE_FLAG ONEBIT(2) // Data Polarity Function Enable
#define TCON_OFC1_DPFC_FLAG ONEBIT(3) // Data Polarity Function Control (0: Data inversion when OPOL/EPOL is 0, 1: Data inversion when OPOL/EPOL is 1)
#define TCON_OFC1_DPFS_FLAG ONEBIT(4) // Data Polarity Function Swap (useful when DPFE = 1)
#define TCON_OFC1_IFE_FLAG ONEBIT(5) // Inversion Function Enable
#define TCON_OFC1_IFS_FLAG ONEBIT(6) // Inversion Function Swap
#define TCON_OFC1_IFC_FLAG ONEBIT(7) // Inversion Function Combined

#define TCON_GAVST (0x62) // Line number that GPOA start (16bit: 0x62 & 0x63)
#define TCON_GAVEND (0x64) // Line number that GPOA ends (16bit: 0x64 & 0x65)

#define TCON_CACTRL (0x6A) // CA control register?
#define TCON_CACTRL_GAOP_FLAG ONEBIT(0) // GPOA Output Polarity
#define TCON_CACTRL_GATC_FLAG ONEBIT(1) // GPOA Toggle Circuit enable
#define TCON_CACTRL_GAES_FLAG ONEBIT(2) // GPOA Early Start function

#define TCON_UNKNOWN_REGISTER (0x77) // it is used in init sequance but i  have  no idea what that does

__xdata __at(0x0000) volatile uint8_t XDATA[0xFFFF];

#endif