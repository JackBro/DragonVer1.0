#ifndef _RDK_VREGDEF938_H_
#define _RDK_VREGDEF938_H_


#define V9_SRAM_BASE							0x100000
#define V9_SRAM_TOTALSIZE						0x60000

#define V9_REG_PAGE0_BASE						0x0
#define V9_REG_PAGE1_BASE						0x100
#define V9_REG_PAGE2_BASE						0x200
#define V9_REG_PAGE3_BASE						0x300
#define V9_REG_PAGE4_BASE						0x400
#define V9_REG_PAGE5_BASE						0x500


#define V9_REG_PAGE_NUM                         0x0

/*****************************************************************************/
/********************************** page 0 register **************************/
/************************** asynchonuse registers for CPM ********************/
/*****************************************************************************/

/*****************************************************************************/
/**************************************** CPM ********************************/
/****************************** Power and PLL control ************************/
/*****************************************************************************/
#define V9_REG_PAGE0_PMCTRL                     (V9_REG_PAGE0_BASE + 0x12)
#define V9_REG_PAGE0_PACFG                      (V9_REG_PAGE0_BASE + 0x13)
#define V9_REG_PAGE0_DACCFG                     (V9_REG_PAGE0_BASE + 0x14)
#define V9_REG_PAGE0_SPVOL_CTL                  (V9_REG_PAGE0_BASE + 0x15)
#define V9_REG_PAGE0_DACCTRL                    (V9_REG_PAGE0_BASE + 0x16)
#define V9_REG_PAGE0_SPVOL                      (V9_REG_PAGE0_BASE + 0x17)
#define V9_REG_PAGE0_TEST_O                     (V9_REG_PAGE0_BASE + 0x18)
#define V9_REG_PAGE0_STRAP                      (V9_REG_PAGE0_BASE + 0x19)
#define V9_REG_PAGE0_CINFO_REN_ADD              (V9_REG_PAGE0_BASE + 0x1A)
#define V9_REG_PAGE0_CINFO_ADD                  (V9_REG_PAGE0_BASE + 0x1B)
#define V9_REG_PAGE0_DUMMY_REG0                 (V9_REG_PAGE0_BASE + 0x1C)
#define V9_REG_PAGE0_DUMMY_REG1                 (V9_REG_PAGE0_BASE + 0x1D)
#define V9_REG_PAGE0_DUMMY_REG2                 (V9_REG_PAGE0_BASE + 0x1E)
#define V9_REG_PAGE0_DUMMY_REG3                 (V9_REG_PAGE0_BASE + 0x1F)
#define V9_REG_PAGE0_MISC_CON                   (V9_REG_PAGE0_BASE + 0x24)
#define V9_REG_PAGE0_I2SCFG                     (V9_REG_PAGE0_BASE + 0x25)
#define V9_REG_PAGE0_FUNC_SEL                   (V9_REG_PAGE0_BASE + 0x26)
#define V9_REG_PAGE0_SRD                        (V9_REG_PAGE0_BASE + 0x27)
#define V9_REG_PAGE0_UM_CTRL                    (V9_REG_PAGE0_BASE + 0x28)
#define V9_REG_PAGE0_GPIO_CFG                   (V9_REG_PAGE0_BASE + 0x29)
#define V9_REG_PAGE0_P0_MODE                    (V9_REG_PAGE0_BASE + 0x2A)
#define V9_REG_PAGE0_P0_DIR                     (V9_REG_PAGE0_BASE + 0x2B)
#define V9_REG_PAGE0_PORT0                      (V9_REG_PAGE0_BASE + 0x2C)
#define V9_REG_PAGE0_D_REN                      (V9_REG_PAGE0_BASE + 0x2D)
#define V9_REG_PAGE0_GPIO_REN                   (V9_REG_PAGE0_BASE + 0x2E)
#define V9_REG_PAGE0_MISC_TEST                  (V9_REG_PAGE0_BASE + 0x2F)
#define V9_REG_PAGE0_CLKCTRL0                   (V9_REG_PAGE0_BASE + 0x30)
#define V9_REG_PAGE0_CLKCTRL1                   (V9_REG_PAGE0_BASE + 0x31)
#define V9_REG_PAGE0_CLKCTRL2                   (V9_REG_PAGE0_BASE + 0x32)
#define V9_REG_PAGE0_CLKCTRL3                   (V9_REG_PAGE0_BASE + 0x33)
#define V9_REG_PAGE0_CLKCTRL4                   (V9_REG_PAGE0_BASE + 0x34)
#define V9_REG_PAGE0_DATA_DLY_SEL               (V9_REG_PAGE0_BASE + 0x40)
#define V9_REG_PAGE0_A0_DLY_SEL                 (V9_REG_PAGE0_BASE + 0x41)
#define V9_REG_PAGE0_DATA_OEN_DLY_SEL           (V9_REG_PAGE0_BASE + 0x42)
#define V9_REG_PAGE0_REF_DLY_SEL                (V9_REG_PAGE0_BASE + 0x43)
#define V9_REG_PAGE0_PHASE_DLY_SEL              (V9_REG_PAGE0_BASE + 0x44)
#define V9_REG_PAGE0_REF_PHASE_SEL              (V9_REG_PAGE0_BASE + 0x45)
#define V9_REG_PAGE0_IE_DLY_SEL                 (V9_REG_PAGE0_BASE + 0x46)


/******************************************************************************/
/********************************* page 1 register ****************************/
/************************** midi & global control registre ********************/
/******************************************************************************/

/******************************************************************************/
/**************************** Global Control Registers ************************/
/******************************************************************************/
#define V9_REG_PAGE1_CHIP_L                   (V9_REG_PAGE1_BASE + 0x1)
#define V9_REG_PAGE1_CHIP_H                   (V9_REG_PAGE1_BASE + 0x2)
#define V9_REG_PAGE1_GLBCTRL                  (V9_REG_PAGE1_BASE + 0x3)
#define V9_REG_PAGE1_STOPALL                  (V9_REG_PAGE1_BASE + 0x4)
#define V9_REG_PAGE1_WS_MISC                  (V9_REG_PAGE1_BASE + 0x5)
#define V9_REG_PAGE1_GLBCFG                   (V9_REG_PAGE1_BASE + 0x6)
#define V9_REG_PAGE1_SEL_CONV                 (V9_REG_PAGE1_BASE + 0x7)
#define V9_REG_PAGE1_BUF1_W_MARK              (V9_REG_PAGE1_BASE + 0x8)
#define V9_REG_PAGE1_BUF2_ADDR_L              (V9_REG_PAGE1_BASE + 0x9)
#define V9_REG_PAGE1_BUF2_ADDR_H              (V9_REG_PAGE1_BASE + 0xA)
#define V9_REG_PAGE1_BUF1_ADDR_L              (V9_REG_PAGE1_BASE + 0xB)
#define V9_REG_PAGE1_BUF1_ADDR_H              (V9_REG_PAGE1_BASE + 0xC)
#define V9_REG_PAGE1_E_EVENT_TYPE             (V9_REG_PAGE1_BASE + 0xD)
#define V9_REG_PAGE1_C_EVENT_TYPE             (V9_REG_PAGE1_BASE + 0xE)
#define V9_REG_PAGE1_TMR_L                    (V9_REG_PAGE1_BASE + 0x10)
#define V9_REG_PAGE1_TMR_M                    (V9_REG_PAGE1_BASE + 0x11)
#define V9_REG_PAGE1_TMR_H                    (V9_REG_PAGE1_BASE + 0x12)
#define V9_REG_PAGE1_TMRTGT_L                 (V9_REG_PAGE1_BASE + 0x13)
#define V9_REG_PAGE1_TMRTGT_M                 (V9_REG_PAGE1_BASE + 0x14)
#define V9_REG_PAGE1_TMRTGT_H                 (V9_REG_PAGE1_BASE + 0x15)
#define V9_REG_PAGE1_STMR                     (V9_REG_PAGE1_BASE + 0x16)
#define V9_REG_PAGE1_STMRTGT                  (V9_REG_PAGE1_BASE + 0x17)
#define V9_REG_PAGE1_TMRBASE_L                (V9_REG_PAGE1_BASE + 0x18)
#define V9_REG_PAGE1_TMRBASE_H                (V9_REG_PAGE1_BASE + 0x19)
#define V9_REG_PAGE1_FRATIO_DLT0              (V9_REG_PAGE1_BASE + 0x1A)
#define V9_REG_PAGE1_FRATIO_DLT1              (V9_REG_PAGE1_BASE + 0x1B)
#define V9_REG_PAGE1_FRATIO_DLT2              (V9_REG_PAGE1_BASE + 0x1C)
#define V9_REG_PAGE1_FRATIO_DLT3              (V9_REG_PAGE1_BASE + 0x1D)
#define V9_REG_PAGE1_ANAERRFLAG               (V9_REG_PAGE1_BASE + 0x1E)
#define V9_REG_PAGE1_ANAERRMASK               (V9_REG_PAGE1_BASE + 0x1F)
#define V9_REG_PAGE1_ERRORFLAG                (V9_REG_PAGE1_BASE + 0x20)
#define V9_REG_PAGE1_ERRORMASK                (V9_REG_PAGE1_BASE + 0x21)
#define V9_REG_PAGE1_BUFFLAG0                 (V9_REG_PAGE1_BASE + 0x22)
#define V9_REG_PAGE1_BUFFLAG1                 (V9_REG_PAGE1_BASE + 0x23)
#define V9_REG_PAGE1_DLYTIME                  (V9_REG_PAGE1_BASE + 0x24)
#define V9_REG_PAGE1_CHNCTRL                  (V9_REG_PAGE1_BASE + 0x25)
#define V9_REG_PAGE1_BUF2_CMD_ADDR_L          (V9_REG_PAGE1_BASE + 0x29)
#define V9_REG_PAGE1_BUF2_CMD_ADDR_H          (V9_REG_PAGE1_BASE + 0x2A)
#define V9_REG_PAGE1_V8051_ADDR_HH            (V9_REG_PAGE1_BASE + 0x2D)
#define V9_REG_PAGE1_V8051_ADDR_L             (V9_REG_PAGE1_BASE + 0x2E)
#define V9_REG_PAGE1_V8051_ADDR_H             (V9_REG_PAGE1_BASE + 0x2F)
#define V9_REG_PAGE1_CMDSTATUS                (V9_REG_PAGE1_BASE + 0x30)
#define V9_REG_PAGE1_CMDATA0                  (V9_REG_PAGE1_BASE + 0x31)
#define V9_REG_PAGE1_CMDATA1                  (V9_REG_PAGE1_BASE + 0x32)
#define V9_REG_PAGE1_CMDATA2                  (V9_REG_PAGE1_BASE + 0x33)
#define V9_REG_PAGE1_CMDATA3                  (V9_REG_PAGE1_BASE + 0x34)
#define V9_REG_PAGE1_STARTLOAD                (V9_REG_PAGE1_BASE + 0x35)
#define V9_REG_PAGE1_LOADDONE                 (V9_REG_PAGE1_BASE + 0x36)
#define V9_REG_PAGE1_EFIFO_RMN_L              (V9_REG_PAGE1_BASE + 0x37)
#define V9_REG_PAGE1_EFIFO_RMN_H              (V9_REG_PAGE1_BASE + 0x38)
#define V9_REG_PAGE1_EFIFO_EPT_TT             (V9_REG_PAGE1_BASE + 0x39)
#define V9_REG_PAGE1_VOL_GAIN_L               (V9_REG_PAGE1_BASE + 0x3A)
#define V9_REG_PAGE1_VOL_GAIN_R               (V9_REG_PAGE1_BASE + 0x3B)
#define V9_REG_PAGE1_SWRST1                   (V9_REG_PAGE1_BASE + 0x40)
#define V9_REG_PAGE1_SWRST2                   (V9_REG_PAGE1_BASE + 0x41)
#define V9_REG_PAGE1_SWRST3                   (V9_REG_PAGE1_BASE + 0x42)
#define V9_REG_PAGE1_VERSION_L                (V9_REG_PAGE1_BASE + 0x4E)
#define V9_REG_PAGE1_VERSION_H                (V9_REG_PAGE1_BASE + 0x4F)
#define V9_REG_PAGE1_INTCTRL                  (V9_REG_PAGE1_BASE + 0x50)
#define V9_REG_PAGE1_INTEN0                   (V9_REG_PAGE1_BASE + 0x51)
#define V9_REG_PAGE1_INTFLAG0                 (V9_REG_PAGE1_BASE + 0x52)
#define V9_REG_PAGE1_INTEN1                   (V9_REG_PAGE1_BASE + 0x53)
#define V9_REG_PAGE1_INTFLAG1                 (V9_REG_PAGE1_BASE + 0x54)
#define V9_REG_PAGE1_INTLOCK                  (V9_REG_PAGE1_BASE + 0x55)
#define V9_REG_PAGE1_INTSERV                  (V9_REG_PAGE1_BASE + 0x56)
#define V9_REG_PAGE1_INTEN2                   (V9_REG_PAGE1_BASE + 0x57)
#define V9_REG_PAGE1_INTFLAG2                 (V9_REG_PAGE1_BASE + 0x58)
#define V9_REG_PAGE1_INT_DLY_CNT0             (V9_REG_PAGE1_BASE + 0x59)
#define V9_REG_PAGE1_INT_DLY_CNT1             (V9_REG_PAGE1_BASE + 0x5A)
#define V9_REG_PAGE1_DLY_SEL                  (V9_REG_PAGE1_BASE + 0x5B)
#define V9_REG_PAGE1_SW_INT                   (V9_REG_PAGE1_BASE + 0x5C)
#define V9_REG_PAGE1_AUTOSTOPEN               (V9_REG_PAGE1_BASE + 0x5D)
#define V9_REG_PAGE1_REPEAT_INT_CNT0          (V9_REG_PAGE1_BASE + 0x60)
#define V9_REG_PAGE1_REPEAT_INT_CNT1          (V9_REG_PAGE1_BASE + 0x61)
#define V9_REG_PAGE1_GLB_MOD_SEL              (V9_REG_PAGE1_BASE + 0x62)
#define V9_REG_PAGE1_STREAM_CLR_BYPASS        (V9_REG_PAGE1_BASE + 0x68)
#define V9_REG_PAGE1_SFIFO_STATUS             (V9_REG_PAGE1_BASE + 0x69)

/******************************************************************************/
/************************************* FIFO Write *****************************/
/******************************************************************************/
#define V9_REG_PAGE1_EFIFO                    (V9_REG_PAGE1_BASE + 0x70)
#define V9_REG_PAGE1_SFIFO                    (V9_REG_PAGE1_BASE + 0x71)
#define V9_REG_PAGE1_CFIFO                    (V9_REG_PAGE1_BASE + 0x76)
#define V9_REG_PAGE1_SBUF0_FULL               (V9_REG_PAGE1_BASE + 0x7B)
#define V9_REG_PAGE1_SBUF1_FULL               (V9_REG_PAGE1_BASE + 0x7C)

/******************************************************************************/
/***************************** Channel control register ***********************/
/******************************************************************************/
#define V9_REG_PAGE1_CHNSTART0                (V9_REG_PAGE1_BASE + 0x80)
#define V9_REG_PAGE1_CHNSTART1                (V9_REG_PAGE1_BASE + 0x81)
#define V9_REG_PAGE1_CHNSTART2                (V9_REG_PAGE1_BASE + 0x82)
#define V9_REG_PAGE1_CHNSTART3                (V9_REG_PAGE1_BASE + 0x83)
#define V9_REG_PAGE1_CHNSTART4                (V9_REG_PAGE1_BASE + 0x84)
#define V9_REG_PAGE1_CHNSTART5                (V9_REG_PAGE1_BASE + 0x85)
#define V9_REG_PAGE1_CHNSTART6                (V9_REG_PAGE1_BASE + 0x86)
#define V9_REG_PAGE1_CHNSTART7                (V9_REG_PAGE1_BASE + 0x87)
#define V9_REG_PAGE1_CHNSTOP0                 (V9_REG_PAGE1_BASE + 0x88)
#define V9_REG_PAGE1_CHNSTOP1                 (V9_REG_PAGE1_BASE + 0x89)
#define V9_REG_PAGE1_CHNSTOP2                 (V9_REG_PAGE1_BASE + 0x8A)
#define V9_REG_PAGE1_CHNSTOP3                 (V9_REG_PAGE1_BASE + 0x8B)
#define V9_REG_PAGE1_CHNSTOP4                 (V9_REG_PAGE1_BASE + 0x8C)
#define V9_REG_PAGE1_CHNSTOP5                 (V9_REG_PAGE1_BASE + 0x8D)
#define V9_REG_PAGE1_CHNSTOP6                 (V9_REG_PAGE1_BASE + 0x8E)
#define V9_REG_PAGE1_CHNSTOP7                 (V9_REG_PAGE1_BASE + 0x8F)
#define V9_REG_PAGE1_CHNDLY0                  (V9_REG_PAGE1_BASE + 0x90)
#define V9_REG_PAGE1_CHNDLY1                  (V9_REG_PAGE1_BASE + 0x91)
#define V9_REG_PAGE1_CHNDLY2                  (V9_REG_PAGE1_BASE + 0x92)
#define V9_REG_PAGE1_CHNDLY3                  (V9_REG_PAGE1_BASE + 0x93)
#define V9_REG_PAGE1_CHNDLY4                  (V9_REG_PAGE1_BASE + 0x94)
#define V9_REG_PAGE1_CHNDLY5                  (V9_REG_PAGE1_BASE + 0x95)
#define V9_REG_PAGE1_CHNDLY6                  (V9_REG_PAGE1_BASE + 0x96)
#define V9_REG_PAGE1_CHNDLY7                  (V9_REG_PAGE1_BASE + 0x97)
#define V9_REG_PAGE1_DLYEND0                  (V9_REG_PAGE1_BASE + 0x98)
#define V9_REG_PAGE1_DLYEND1                  (V9_REG_PAGE1_BASE + 0x99)
#define V9_REG_PAGE1_DLYEND2                  (V9_REG_PAGE1_BASE + 0x9A)
#define V9_REG_PAGE1_DLYEND3                  (V9_REG_PAGE1_BASE + 0x9B)
#define V9_REG_PAGE1_DLYEND4                  (V9_REG_PAGE1_BASE + 0x9C)
#define V9_REG_PAGE1_DLYEND5                  (V9_REG_PAGE1_BASE + 0x9D)
#define V9_REG_PAGE1_DLYEND6                  (V9_REG_PAGE1_BASE + 0x9E)
#define V9_REG_PAGE1_DLYEND7                  (V9_REG_PAGE1_BASE + 0x9F)
#define V9_REG_PAGE1_OP_MODE                  (V9_REG_PAGE1_BASE + 0xAA)

/******************************************************************************/
/****************************** Audio control register ************************/
/******************************************************************************/
#define V9_REG_PAGE1_ADCTRL                   (V9_REG_PAGE1_BASE + 0xB0)
#define V9_REG_PAGE1_VOLMID_L                 (V9_REG_PAGE1_BASE + 0xB1)
#define V9_REG_PAGE1_VOLMID_R                 (V9_REG_PAGE1_BASE + 0xB2)
#define V9_REG_PAGE1_VOLWAV_L                 (V9_REG_PAGE1_BASE + 0xB3)
#define V9_REG_PAGE1_VOLWAV_R                 (V9_REG_PAGE1_BASE + 0xB4)
#define V9_REG_PAGE1_HPNVOL_L                 (V9_REG_PAGE1_BASE + 0xB6)
#define V9_REG_PAGE1_HPNVOL_R                 (V9_REG_PAGE1_BASE + 0xB7)
#define V9_REG_PAGE1_DACPGG                   (V9_REG_PAGE1_BASE + 0xB8)
#define V9_REG_PAGE1_MUTECTRL                 (V9_REG_PAGE1_BASE + 0xB9)
#define V9_REG_PAGE1_E_W_MARK                 (V9_REG_PAGE1_BASE + 0xBA)
#define V9_REG_PAGE1_DT_RETRI                 (V9_REG_PAGE1_BASE + 0xBB)
#define V9_REG_PAGE1_REG000                   (V9_REG_PAGE1_BASE + 0xC0)
#define V9_REG_PAGE1_REG010                   (V9_REG_PAGE1_BASE + 0xC3)
#define V9_REG_PAGE1_INTPCOEF0                (V9_REG_PAGE1_BASE + 0xC4)
#define V9_REG_PAGE1_INTPCOEF1                (V9_REG_PAGE1_BASE + 0xC5)
#define V9_REG_PAGE1_REG020                   (V9_REG_PAGE1_BASE + 0xC6)
#define V9_REG_PAGE1_ECNTINIT2                (V9_REG_PAGE1_BASE + 0xC8)
#define V9_REG_PAGE1_PREVAL0                  (V9_REG_PAGE1_BASE + 0xCB)
#define V9_REG_PAGE1_PREVAL1                  (V9_REG_PAGE1_BASE + 0xCC)
#define V9_REG_PAGE1_REG111                   (V9_REG_PAGE1_BASE + 0xCD)
#define V9_REG_PAGE1_EVLPCHG20                (V9_REG_PAGE1_BASE + 0xCE)
#define V9_REG_PAGE1_LOADADDR                 (V9_REG_PAGE1_BASE + 0xD6)
#define V9_REG_PAGE1_SFIFO0_WM                (V9_REG_PAGE1_BASE + 0xDC)
#define V9_REG_PAGE1_SFIFO1_WM                (V9_REG_PAGE1_BASE + 0xDD)
#define V9_REG_PAGE1_SFIFO2_WM                (V9_REG_PAGE1_BASE + 0xDE)
#define V9_REG_PAGE1_SFIFO3_WM                (V9_REG_PAGE1_BASE + 0xDF)
#define V9_REG_PAGE1_SFIFO0_AW_BL             (V9_REG_PAGE1_BASE + 0xE0)
#define V9_REG_PAGE1_SFIFO0_AW_BH             (V9_REG_PAGE1_BASE + 0xE1)
#define V9_REG_PAGE1_SFIFO0_AW_EL             (V9_REG_PAGE1_BASE + 0xE2)
#define V9_REG_PAGE1_SFIFO0_AW_EH             (V9_REG_PAGE1_BASE + 0xE3)
#define V9_REG_PAGE1_SFIFO1_AW_BL             (V9_REG_PAGE1_BASE + 0xE4)
#define V9_REG_PAGE1_SFIFO1_AW_BH             (V9_REG_PAGE1_BASE + 0xE5)
#define V9_REG_PAGE1_SFIFO1_AW_EL             (V9_REG_PAGE1_BASE + 0xE6)
#define V9_REG_PAGE1_SFIFO1_AW_EH             (V9_REG_PAGE1_BASE + 0xE7)
#define V9_REG_PAGE1_SFIFO2_AW_BL             (V9_REG_PAGE1_BASE + 0xE8)
#define V9_REG_PAGE1_SFIFO2_AW_BH             (V9_REG_PAGE1_BASE + 0xE9)
#define V9_REG_PAGE1_SFIFO2_AW_EL             (V9_REG_PAGE1_BASE + 0xEA)
#define V9_REG_PAGE1_SFIFO2_AW_EH             (V9_REG_PAGE1_BASE + 0xEB)
#define V9_REG_PAGE1_SFIFO3_AW_BL             (V9_REG_PAGE1_BASE + 0xEC)
#define V9_REG_PAGE1_SFIFO3_AW_BH             (V9_REG_PAGE1_BASE + 0xED)
#define V9_REG_PAGE1_SFIFO3_AW_EL             (V9_REG_PAGE1_BASE + 0xEE)
#define V9_REG_PAGE1_SFIFO3_AW_EH             (V9_REG_PAGE1_BASE + 0xEF)
#define V9_REG_PAGE1_SFIFO0_AR_BL             (V9_REG_PAGE1_BASE + 0xF0)
#define V9_REG_PAGE1_SFIFO0_AR_BH             (V9_REG_PAGE1_BASE + 0xF1)
#define V9_REG_PAGE1_SFIFO0_AR_EL             (V9_REG_PAGE1_BASE + 0xF2)
#define V9_REG_PAGE1_SFIFO0_AR_EH             (V9_REG_PAGE1_BASE + 0xF3)
#define V9_REG_PAGE1_SFIFO1_AR_BL             (V9_REG_PAGE1_BASE + 0xF4)
#define V9_REG_PAGE1_SFIFO1_AR_BH             (V9_REG_PAGE1_BASE + 0xF5)
#define V9_REG_PAGE1_SFIFO1_AR_EL             (V9_REG_PAGE1_BASE + 0xF6)
#define V9_REG_PAGE1_SFIFO1_AR_EH             (V9_REG_PAGE1_BASE + 0xF7)
#define V9_REG_PAGE1_SFIFO2_AR_BL             (V9_REG_PAGE1_BASE + 0xF8)
#define V9_REG_PAGE1_SFIFO2_AR_BH             (V9_REG_PAGE1_BASE + 0xF9)
#define V9_REG_PAGE1_SFIFO2_AR_EL             (V9_REG_PAGE1_BASE + 0xFA)
#define V9_REG_PAGE1_SFIFO2_AR_EH             (V9_REG_PAGE1_BASE + 0xFB)
#define V9_REG_PAGE1_SFIFO3_AR_BL             (V9_REG_PAGE1_BASE + 0xFC)
#define V9_REG_PAGE1_SFIFO3_AR_BH             (V9_REG_PAGE1_BASE + 0xFD)
#define V9_REG_PAGE1_SFIFO3_AR_EL             (V9_REG_PAGE1_BASE + 0xFE)
#define V9_REG_PAGE1_SFIFO3_AR_EH             (V9_REG_PAGE1_BASE + 0xFF)


/*************************************************************************/
/******************************** page 2 register ************************/
/************************** MP3 & EQ control register ********************/
/*************************************************************************/

/******************************************************************************/
/********************************** Mp3 register ******************************/
/******************************************************************************/
#define V9_REG_PAGE2_CLR_A                    (V9_REG_PAGE2_BASE + 0xF)
#define V9_REG_PAGE2_MP3_RD_PTR_L             (V9_REG_PAGE2_BASE + 0x10)
#define V9_REG_PAGE2_MP3_RD_PTR_H             (V9_REG_PAGE2_BASE + 0x11)
#define V9_REG_PAGE2_MP3_WR_PTR_L             (V9_REG_PAGE2_BASE + 0x12)
#define V9_REG_PAGE2_MP3_WR_PTR_H             (V9_REG_PAGE2_BASE + 0x13)
#define V9_REG_PAGE2_W_MARK                   (V9_REG_PAGE2_BASE + 0x14)
#define V9_REG_PAGE2_MP3_DONE                 (V9_REG_PAGE2_BASE + 0x15)
#define V9_REG_PAGE2_MP3_PARA0                (V9_REG_PAGE2_BASE + 0x16)
#define V9_REG_PAGE2_MP3_PARA1                (V9_REG_PAGE2_BASE + 0x17)
#define V9_REG_PAGE2_MP3_PARA2                (V9_REG_PAGE2_BASE + 0x18)
#define V9_REG_PAGE2_MP3_EQBAND0              (V9_REG_PAGE2_BASE + 0x20)
#define V9_REG_PAGE2_MP3_EQBAND1              (V9_REG_PAGE2_BASE + 0x21)
#define V9_REG_PAGE2_MP3_EQBAND2              (V9_REG_PAGE2_BASE + 0x22)
#define V9_REG_PAGE2_MP3_EQBAND3              (V9_REG_PAGE2_BASE + 0x23)
#define V9_REG_PAGE2_MP3_EQBAND4              (V9_REG_PAGE2_BASE + 0x24)
#define V9_REG_PAGE2_MP3_EQBAND5              (V9_REG_PAGE2_BASE + 0x25)
#define V9_REG_PAGE2_MP3_EQBAND6              (V9_REG_PAGE2_BASE + 0x26)
#define V9_REG_PAGE2_MP3_EQBAND7              (V9_REG_PAGE2_BASE + 0x27)
#define V9_REG_PAGE2_MP3_EQBAND8              (V9_REG_PAGE2_BASE + 0x28)
#define V9_REG_PAGE2_MP3_EQBAND9              (V9_REG_PAGE2_BASE + 0x29)
#define V9_REG_PAGE2_MP3_VOLSTEP              (V9_REG_PAGE2_BASE + 0x2A)
#define V9_REG_PAGE2_MFIFO                    (V9_REG_PAGE2_BASE + 0x30)

/******************************************************************************/
/*********************************** EQ register ******************************/
/******************************************************************************/
#define V9_REG_PAGE2_EQBAND0                  (V9_REG_PAGE2_BASE + 0x40)
#define V9_REG_PAGE2_EQBAND1                  (V9_REG_PAGE2_BASE + 0x41)
#define V9_REG_PAGE2_EQBAND2                  (V9_REG_PAGE2_BASE + 0x42)
#define V9_REG_PAGE2_EQBAND3                  (V9_REG_PAGE2_BASE + 0x43)
#define V9_REG_PAGE2_EQBAND4                  (V9_REG_PAGE2_BASE + 0x44)
#define V9_REG_PAGE2_EQBAND5                  (V9_REG_PAGE2_BASE + 0x45)
#define V9_REG_PAGE2_SAMP_RATE                (V9_REG_PAGE2_BASE + 0x50)
#define V9_REG_PAGE2_VOLSTEP                  (V9_REG_PAGE2_BASE + 0x51)
#define V9_REG_PAGE2_OUTSEL                   (V9_REG_PAGE2_BASE + 0x52)


/************************************************************************/
/******************************** page 3 register ***********************/
/************************** Decrypt control register ********************/
/************************************************************************/
#define V9_REG_PAGE3_AES_CORE_CMD             (V9_REG_PAGE3_BASE + 0x1)
#define V9_REG_PAGE3_MP3_STRB_DLY             (V9_REG_PAGE3_BASE + 0x2)
#define V9_REG_PAGE3_AES_KEY0                 (V9_REG_PAGE3_BASE + 0x10)
#define V9_REG_PAGE3_AES_KEY1                 (V9_REG_PAGE3_BASE + 0x11)
#define V9_REG_PAGE3_AES_KEY2                 (V9_REG_PAGE3_BASE + 0x12)
#define V9_REG_PAGE3_AES_KEY3                 (V9_REG_PAGE3_BASE + 0x13)
#define V9_REG_PAGE3_AES_KEY4                 (V9_REG_PAGE3_BASE + 0x14)
#define V9_REG_PAGE3_AES_KEY5                 (V9_REG_PAGE3_BASE + 0x15)
#define V9_REG_PAGE3_AES_KEY6                 (V9_REG_PAGE3_BASE + 0x16)
#define V9_REG_PAGE3_AES_KEY7                 (V9_REG_PAGE3_BASE + 0x17)
#define V9_REG_PAGE3_AES_KEY8                 (V9_REG_PAGE3_BASE + 0x18)
#define V9_REG_PAGE3_AES_KEY9                 (V9_REG_PAGE3_BASE + 0x19)
#define V9_REG_PAGE3_AES_KEY10                (V9_REG_PAGE3_BASE + 0x1A)
#define V9_REG_PAGE3_AES_KEY11                (V9_REG_PAGE3_BASE + 0x1B)
#define V9_REG_PAGE3_AES_KEY12                (V9_REG_PAGE3_BASE + 0x1C)
#define V9_REG_PAGE3_AES_KEY13                (V9_REG_PAGE3_BASE + 0x1D)
#define V9_REG_PAGE3_AES_KEY14                (V9_REG_PAGE3_BASE + 0x1E)
#define V9_REG_PAGE3_AES_KEY15                (V9_REG_PAGE3_BASE + 0x1F)
#define V9_REG_PAGE3_AES_IV0                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV1                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV2                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV3                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV4                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV5                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV6                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV7                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV8                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV9                  (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV10                 (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV11                 (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV12                 (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV13                 (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV14                 (V9_REG_PAGE3_BASE + 0x20)
#define V9_REG_PAGE3_AES_IV15                 (V9_REG_PAGE3_BASE + 0x20)


/****************************************************************************/
/******************************** page 4 register ***************************/
/****************************** SD access register **************************/
/****************************************************************************/
#define V9_REG_PAGE4_SD_CMD0_FIFO             (V9_REG_PAGE4_BASE + 0xF0)
#define V9_REG_PAGE4_SD_CMD1_FIFO             (V9_REG_PAGE4_BASE + 0xF1)
#define V9_REG_PAGE4_SD_DATA_FIFO             (V9_REG_PAGE4_BASE + 0xF2)


/****************************************************************************/
/******************************** page 5 register ***************************/
/************************** Led & motor control register ********************/
/****************************************************************************/
#define V9_REG_PAGE5_REG_TIMESLICE_HIGH       (V9_REG_PAGE5_BASE + 0x1)
#define V9_REG_PAGE5_REG_TIMESLICE_LOW        (V9_REG_PAGE5_BASE + 0x2)
#define V9_REG_PAGE5_REG_PWM_RLED_HIGH        (V9_REG_PAGE5_BASE + 0x3)
#define V9_REG_PAGE5_REG_PWM_RLED_LOW         (V9_REG_PAGE5_BASE + 0x4)
#define V9_REG_PAGE5_REG_PWM_BLED_HIGH        (V9_REG_PAGE5_BASE + 0x5)
#define V9_REG_PAGE5_REG_PWM_BLED_LOW         (V9_REG_PAGE5_BASE + 0x6)
#define V9_REG_PAGE5_REG_PWM_GLED_HIGH        (V9_REG_PAGE5_BASE + 0x7)
#define V9_REG_PAGE5_REG_PWM_GLED_LOW         (V9_REG_PAGE5_BASE + 0x8)
#define V9_REG_PAGE5_REG_PWM_MOT_HIGH         (V9_REG_PAGE5_BASE + 0x9)
#define V9_REG_PAGE5_REG_PWM_MOT_LOW          (V9_REG_PAGE5_BASE + 0xA)
#define V9_REG_PAGE5_REG_LEDMOT_CONF          (V9_REG_PAGE5_BASE + 0xB)
#define V9_REG_PAGE5_REG_CIRCLING_PERIOD      (V9_REG_PAGE5_BASE + 0xC)
#define V9_REG_PAGE5_REG_INDEX                (V9_REG_PAGE5_BASE + 0xD)
#define V9_REG_PAGE5_REG_PATTERN0_HIGH        (V9_REG_PAGE5_BASE + 0xE)
#define V9_REG_PAGE5_REG_PATTERN0_LOW         (V9_REG_PAGE5_BASE + 0xF)
#define V9_REG_PAGE5_REG_PATTERN1_HIGH        (V9_REG_PAGE5_BASE + 0x11)
#define V9_REG_PAGE5_REG_PATTERN1_LOW         (V9_REG_PAGE5_BASE + 0x12)
#define V9_REG_PAGE5_REG_PATTERN2_HIGH        (V9_REG_PAGE5_BASE + 0x13)
#define V9_REG_PAGE5_REG_PATTERN2_LOW         (V9_REG_PAGE5_BASE + 0x14)
#define V9_REG_PAGE5_REG_PATTERN3_HIGH        (V9_REG_PAGE5_BASE + 0x15)
#define V9_REG_PAGE5_REG_PATTERN3_LOW         (V9_REG_PAGE5_BASE + 0x16)
#define V9_REG_PAGE5_REG_PATTERN4_HIGH        (V9_REG_PAGE5_BASE + 0x17)
#define V9_REG_PAGE5_REG_PATTERN4_LOW         (V9_REG_PAGE5_BASE + 0x18)
#define V9_REG_PAGE5_REG_PATTERN5_HIGH        (V9_REG_PAGE5_BASE + 0x19)
#define V9_REG_PAGE5_REG_PATTERN5_LOW         (V9_REG_PAGE5_BASE + 0x1A)
#define V9_REG_PAGE5_REG_PATTERN6_HIGH        (V9_REG_PAGE5_BASE + 0x1B)
#define V9_REG_PAGE5_REG_PATTERN6_LOW         (V9_REG_PAGE5_BASE + 0x1C)
#define V9_REG_PAGE5_REG_PATTERN7_HIGH        (V9_REG_PAGE5_BASE + 0x1D)
#define V9_REG_PAGE5_REG_PATTERN7_LOW         (V9_REG_PAGE5_BASE + 0x1E)
#define V9_REG_PAGE5_REG_MTRCTRL              (V9_REG_PAGE5_BASE + 0x26)
#define V9_REG_PAGE5_REG_MTRSYNC              (V9_REG_PAGE5_BASE + 0x27)
#define V9_REG_PAGE5_REG_LEDCTRL1             (V9_REG_PAGE5_BASE + 0x28)
#define V9_REG_PAGE5_REG_LEDCTRL2             (V9_REG_PAGE5_BASE + 0x29)
#define V9_REG_PAGE5_REG_LEDCTRL3             (V9_REG_PAGE5_BASE + 0x2A)
#define V9_REG_PAGE5_REG_LEDSYNC1             (V9_REG_PAGE5_BASE + 0x2B)
#define V9_REG_PAGE5_REG_LEDSYNC2             (V9_REG_PAGE5_BASE + 0x2C)
#define V9_REG_PAGE5_REG_LEDSYNC3             (V9_REG_PAGE5_BASE + 0x2D)


#endif //_VREGDEF_H_
