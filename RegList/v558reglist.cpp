
#include "internal.h"
#include "../terminal/terminal/v558/v558rdk/vregdef.h"





const TRegUnit gSifReg[] =
{
	{ V558_REGTYPE, "V558_REG_SIF_SNR_MODE", V558_REG_SIF_SNR_MODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNROP_MODE", V558_REG_SIF_SNROP_MODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_YUV_FORMAT", V558_REG_SIF_YUV_FORMAT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_V_CONTROL", V558_REG_SIF_V_CONTROL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_H_CONTROL", V558_REG_SIF_H_CONTROL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRCLK_CNT", V558_REG_SIF_SNRCLK_CNT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_PIXRATE_SEL", V558_REG_SIF_PIXRATE_SEL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SICLK_FACTOR", V558_REG_SIF_SICLK_FACTOR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_I2C_CLKCNTH", V558_REG_SIF_I2C_CLKCNTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_I2C_CLKCNTL", V558_REG_SIF_I2C_CLKCNTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HD_STARTH", V558_REG_SIF_HD_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HD_STARTL", V558_REG_SIF_HD_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HD_STOPH", V558_REG_SIF_HD_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HD_STOPL", V558_REG_SIF_HD_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HSYNC_STARTH", V558_REG_SIF_HSYNC_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HSYNC_STARTL", V558_REG_SIF_HSYNC_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HSYNC_STOPH", V558_REG_SIF_HSYNC_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HSYNC_STOPL", V558_REG_SIF_HSYNC_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HREF_STARTH", V558_REG_SIF_HREF_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HREF_STARTL", V558_REG_SIF_HREF_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HREF_LENGTHH", V558_REG_SIF_HREF_LENGTHH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HREF_LENGTHL", V558_REG_SIF_HREF_LENGTHL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VD_STARTH", V558_REG_SIF_VD_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VD_STARTL", V558_REG_SIF_VD_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VD_STOPH", V558_REG_SIF_VD_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VD_STOPL", V558_REG_SIF_VD_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VSYNC_STARTH", V558_REG_SIF_VSYNC_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VSYNC_STARTL", V558_REG_SIF_VSYNC_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VSYNC_STOPH", V558_REG_SIF_VSYNC_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VSYNC_STOPL", V558_REG_SIF_VSYNC_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VREF_STARTH", V558_REG_SIF_VREF_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VREF_STARTL", V558_REG_SIF_VREF_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VREF_STOPH", V558_REG_SIF_VREF_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VREF_STOPL", V558_REG_SIF_VREF_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HDFALL_STARTH", V558_REG_SIF_HDFALL_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HDFALL_STARTL", V558_REG_SIF_HDFALL_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HDFALL_STOPH", V558_REG_SIF_HDFALL_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_HDFALL_STOPL", V558_REG_SIF_HDFALL_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VDFALL_STARTH", V558_REG_SIF_VDFALL_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VDFALL_STARTL", V558_REG_SIF_VDFALL_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VDFALL_STOPH", V558_REG_SIF_VDFALL_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_VDFALL_STOPL", V558_REG_SIF_VDFALL_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_COLMAXH", V558_REG_SIF_COLMAXH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_COLMAXL", V558_REG_SIF_COLMAXL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_ROWMAXH", V558_REG_SIF_ROWMAXH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_ROWMAXL", V558_REG_SIF_ROWMAXL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_EXTIMEH", V558_REG_SIF_EXTIMEH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_EXTIMEL", V558_REG_SIF_EXTIMEL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_GAIN", V558_REG_SIF_GAIN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_OBREG", V558_REG_SIF_OBREG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_IIC_BYTE", V558_REG_SIF_IIC_BYTE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_IIC_DEVADDR", V558_REG_SIF_IIC_DEVADDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRWRDATAH", V558_REG_SIF_SNRWRDATAH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRWRDATAM", V558_REG_SIF_SNRWRDATAM, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRWRDATAL", V558_REG_SIF_SNRWRDATAL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRACSCTR", V558_REG_SIF_SNRACSCTR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRADDR", V558_REG_SIF_SNRADDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRSTATUS", V558_REG_SIF_SNRSTATUS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRRDDATAH", V558_REG_SIF_SNRRDDATAH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRRDDATAM", V558_REG_SIF_SNRRDDATAM, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_SNRRDDATAL", V558_REG_SIF_SNRRDDATAL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_BUS_STATE", V558_REG_SIF_BUS_STATE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_AEWIN_STARTH", V558_REG_SIF_AEWIN_STARTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_AEWIN_STARTL", V558_REG_SIF_AEWIN_STARTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_AEWIN_STOPH", V558_REG_SIF_AEWIN_STOPH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_AEWIN_STOPL", V558_REG_SIF_AEWIN_STOPL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_AEPIN_CTRL", V558_REG_SIF_AEPIN_CTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_AEBUS_CTRL", V558_REG_SIF_AEBUS_CTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_B_CNTL", V558_REG_SIF_B_CNTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_B_CNTH", V558_REG_SIF_B_CNTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_SIF_AE0_DATA", V558_REG_SIF_AE0_DATA, 0, 0 }
};

const TRegUnit gCtrlReg[] =
{
	{ V558_REGTYPE, "V558_REG_HCTRL_RSTCTRL1", V558_REG_HCTRL_RSTCTRL1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_RSTCTRL2", V558_REG_HCTRL_RSTCTRL2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CLKOFF", V558_REG_HCTRL_CLKOFF, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CHIPCTRL", V558_REG_HCTRL_CHIPCTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CLKCTRL0", V558_REG_HCTRL_CLKCTRL0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CLKCTRL1", V558_REG_HCTRL_CLKCTRL1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CLKCTRL2", V558_REG_HCTRL_CLKCTRL2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CLKCTRL3", V558_REG_HCTRL_CLKCTRL3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CPU_PC", V558_REG_HCTRL_CPU_PC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_LCD_PC", V558_REG_HCTRL_LCD_PC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_CS_PC", V558_REG_HCTRL_CS_PC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_STRAP", V558_REG_HCTRL_STRAP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_GPIO_CFG", V558_REG_HCTRL_GPIO_CFG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_GPIO_MODE", V558_REG_HCTRL_GPIO_MODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_GPIO_DIR", V558_REG_HCTRL_GPIO_DIR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_GPIO_P0", V558_REG_HCTRL_GPIO_P0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTCTRL", V558_REG_HCTRL_INTCTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTSERV", V558_REG_HCTRL_INTSERV, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTEN", V558_REG_HCTRL_INTEN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTFLAG", V558_REG_HCTRL_INTFLAG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_UMCTRL", V558_REG_HCTRL_UMCTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_MISC_CON", V558_REG_HCTRL_MISC_CON, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTEN0", V558_REG_HCTRL_INTEN0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTFLAG0", V558_REG_HCTRL_INTFLAG0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTEN1", V558_REG_HCTRL_INTEN1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTEN2", V558_REG_HCTRL_INTEN2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTEN3", V558_REG_HCTRL_INTEN3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_HCTRL_INTEN4", V558_REG_HCTRL_INTEN4, 0, 0 }
};

const TRegUnit gBiuReg[] =
{
	{ V558_REGTYPE, "V558_REG_BIU_CMD", V558_REG_BIU_CMD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_MEM_LOW_WORD_L", V558_REG_BIU_MEM_LOW_WORD_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_MEM_LOW_WORD_H", V558_REG_BIU_MEM_LOW_WORD_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_MEM_HIGH_WORD", V558_REG_BIU_MEM_HIGH_WORD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_INCREMENT", V558_REG_BIU_INCREMENT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_MEM_FLG", V558_REG_BIU_MEM_FLG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_MUL_U2IA", V558_REG_BIU_MUL_U2IA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_OP_MODE", V558_REG_BIU_OP_MODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_SEL_PORT", V558_REG_BIU_SEL_PORT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_SEL_8_16", V558_REG_BIU_SEL_8_16, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_BYPASS_SEL", V558_REG_BIU_BYPASS_SEL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_SEL_ASYN_SYN", V558_REG_BIU_SEL_ASYN_SYN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_WE_DRIVE", V558_REG_BIU_WE_DRIVE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_MUL_CLR_AUTO", V558_REG_BIU_MUL_CLR_AUTO, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_V558_REG_8_FLG", V558_REG_BIU_V558_REG_8_FLG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_V558_REG_8_LOW_WORD", V558_REG_BIU_V558_REG_8_LOW_WORD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_V558_REG_8_HIGH_WORD", V558_REG_BIU_V558_REG_8_HIGH_WORD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_BIU_MEM_8_FLG", V558_REG_BIU_MEM_8_FLG, 0, 0 }
};

const TRegUnit gIspReg[] =
{
	{ V558_REGTYPE, "V558_REG_ISP_BASCTL", V558_REG_ISP_BASCTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_WINWD", V558_REG_ISP_WINWD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_WINHT", V558_REG_ISP_WINHT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_IMGWD", V558_REG_ISP_IMGWD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_IMGHT", V558_REG_ISP_IMGHT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_FEM_M", V558_REG_ISP_FEM_M, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_FEM_P", V558_REG_ISP_FEM_P, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_FEM_X1", V558_REG_ISP_FEM_X1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_FEM_X2", V558_REG_ISP_FEM_X2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_FEM_MIN", V558_REG_ISP_FEM_MIN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_FEM_MAX", V558_REG_ISP_FEM_MAX, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_LFCENX", V558_REG_ISP_LFCENX, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_LFCENY", V558_REG_ISP_LFCENY, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_LFFREC", V558_REG_ISP_LFFREC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFLNCTL", V558_REG_ISP_AFLNCTL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFTHD", V558_REG_ISP_AFTHD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFLNSTEP", V558_REG_ISP_AFLNSTEP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_RGAIN", V558_REG_ISP_RGAIN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_GGAIN", V558_REG_ISP_GGAIN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_BGAIN", V558_REG_ISP_BGAIN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_GLBGAIN", V558_REG_ISP_GLBGAIN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_CRADJ", V558_REG_ISP_CRADJ, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_CBADJ", V558_REG_ISP_CBADJ, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_CROFS", V558_REG_ISP_CROFS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_CBOFS", V558_REG_ISP_CBOFS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_CMAT11", V558_REG_ISP_CMAT11, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_OFSR", V558_REG_ISP_OFSR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_OFSG", V558_REG_ISP_OFSG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_OFSB", V558_REG_ISP_OFSB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_YGMST0", V558_REG_ISP_YGMST0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_YGASP0", V558_REG_ISP_YGASP0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_RGMST0", V558_REG_ISP_RGMST0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_GGMST0", V558_REG_ISP_GGMST0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_BGMST0", V558_REG_ISP_BGMST0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFCTRL", V558_REG_ISP_AFCTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFWHA", V558_REG_ISP_AFWHA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFWHB", V558_REG_ISP_AFWHB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFWHC", V558_REG_ISP_AFWHC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFWVA", V558_REG_ISP_AFWVA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFWVB", V558_REG_ISP_AFWVB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AFWVC", V558_REG_ISP_AFWVC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AEWF", V558_REG_ISP_AEWF, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_YBOT", V558_REG_ISP_YBOT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_YTOP", V558_REG_ISP_YTOP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_QTOP", V558_REG_ISP_QTOP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_ITOP", V558_REG_ISP_ITOP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_GBOT", V558_REG_ISP_GBOT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_GTOP", V558_REG_ISP_GTOP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AWBCTRL", V558_REG_ISP_AWBCTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_RMEAN", V558_REG_ISP_RMEAN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_GMEAN", V558_REG_ISP_GMEAN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_BMEAN", V558_REG_ISP_BMEAN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_YMEAN", V558_REG_ISP_YMEAN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_AUTO_STATUS", V558_REG_ISP_AUTO_STATUS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_DPCTHD", V558_REG_ISP_DPCTHD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_ISP_NT1", V558_REG_ISP_NT1, 0, 0 }
};

const TRegUnit gIppReg[] =
{
	{ V558_REGTYPE, "V558_REG_IPP_SPECTRL", V558_REG_IPP_SPECTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_SIZCTRL", V558_REG_IPP_SIZCTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_IMGWD", V558_REG_IPP_IMGWD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_IMGHT", V558_REG_IPP_IMGHT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_SIZWD", V558_REG_IPP_SIZWD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_SIZHT", V558_REG_IPP_SIZHT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_SIZSTX", V558_REG_IPP_SIZSTX, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_SIZSTY", V558_REG_IPP_SIZSTY, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_FIRCF", V558_REG_IPP_FIRCF, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_PIXRATE", V558_REG_IPP_PIXRATE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_CPUPFT", V558_REG_IPP_CPUPFT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_CPDWFT", V558_REG_IPP_CPDWFT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_DPUPFT", V558_REG_IPP_DPUPFT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_DPDWFT", V558_REG_IPP_DPDWFT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_CAPWD", V558_REG_IPP_CAPWD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_CAPHT", V558_REG_IPP_CAPHT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_DISWD", V558_REG_IPP_DISWD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_IPP_DISHT", V558_REG_IPP_DISHT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_YOFFSET", V558_REG_YOFFSET, 0, 0 },
	{ V558_REGTYPE, "V558_REG_UOFFSET", V558_REG_UOFFSET, 0, 0 },
	{ V558_REGTYPE, "V558_REG_VOFFSET", V558_REG_VOFFSET, 0, 0 },
	{ V558_REGTYPE, "V558_REG_UUPTHRS", V558_REG_UUPTHRS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_UDWTHRS", V558_REG_UDWTHRS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_VUPTHRS", V558_REG_VUPTHRS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_VDWTHRS", V558_REG_VDWTHRS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_FRMDP", V558_REG_FRMDP, 0, 0 }
};

const TRegUnit gLcdIFReg[] =
{
	{ V558_REGTYPE, "V558_REG_LCD_HSA", V558_REG_LCD_HSA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_HEA", V558_REG_LCD_HEA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_HSXA", V558_REG_LCD_HSXA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_HSYA", V558_REG_LCD_HSYA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_HEXA", V558_REG_LCD_HEXA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_HEYA", V558_REG_LCD_HEYA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_WRI", V558_REG_LCD_WRI, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_LC", V558_REG_LCD_LC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_DCE", V558_REG_LCD_DCE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_LCDRST", V558_REG_LCD_LCDRST, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_LCDTEST", V558_REG_LCD_LCDTEST, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_HDP", V558_REG_LCD_HDP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_VDP", V558_REG_LCD_VDP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_AX", V558_REG_LCD_AX, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_AY", V558_REG_LCD_AY, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_AW", V558_REG_LCD_AW, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_AH", V558_REG_LCD_AH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_ADX0", V558_REG_LCD_ADX0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_ADY0", V558_REG_LCD_ADY0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_AMW", V558_REG_LCD_AMW, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_AMH", V558_REG_LCD_AMH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BX", V558_REG_LCD_BX, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BY", V558_REG_LCD_BY, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BW", V558_REG_LCD_BW, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BH", V558_REG_LCD_BH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BDX0", V558_REG_LCD_BDX0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BDY0", V558_REG_LCD_BDY0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BDX1", V558_REG_LCD_BDX1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BDY1", V558_REG_LCD_BDY1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BMW", V558_REG_LCD_BMW, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BMH", V558_REG_LCD_BMH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_ATCB", V558_REG_LCD_ATCB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_ATCG", V558_REG_LCD_ATCG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_ATCR", V558_REG_LCD_ATCR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_ATCF", V558_REG_LCD_ATCF, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BTCB", V558_REG_LCD_BTCB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BTCG", V558_REG_LCD_BTCG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BTCR", V558_REG_LCD_BTCR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BTCF", V558_REG_LCD_BTCF, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_LPM", V558_REG_LCD_LPM, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BMODE", V558_REG_LCD_BMODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_DMAP", V558_REG_LCD_DMAP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_DPAD", V558_REG_LCD_DPAD, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_WCONT", V558_REG_LCD_WCONT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_RCONT", V558_REG_LCD_RCONT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BM", V558_REG_LCD_BM, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_DRS", V558_REG_LCD_DRS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BGDR", V558_REG_LCD_BGDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BGDG", V558_REG_LCD_BGDG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_BGDB", V558_REG_LCD_BGDB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_DM", V558_REG_LCD_DM, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_MODE", V558_REG_LCD_MODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXWEN", V558_REG_LCD_EXWEN, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXW0", V558_REG_LCD_EXW0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXW1", V558_REG_LCD_EXW1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXR0", V558_REG_LCD_EXR0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXRS", V558_REG_LCD_EXRS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IST", V558_REG_LCD_IST, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IW", V558_REG_LCD_IW, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IA", V558_REG_LCD_IA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXW1", V558_REG_LCD_EXW1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXR0", V558_REG_LCD_EXR0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_EXRS", V558_REG_LCD_EXRS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IST", V558_REG_LCD_IST, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IW", V558_REG_LCD_IW, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IA", V558_REG_LCD_IA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IDB", V558_REG_LCD_IDB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IDG", V558_REG_LCD_IDG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IDR", V558_REG_LCD_IDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_OM", V558_REG_LCD_OM, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_UPDATE", V558_REG_LCD_UPDATE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_INDEXSEL", V558_REG_LCD_INDEXSEL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IR", V558_REG_LCD_IR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IRDB", V558_REG_LCD_IRDB, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IRDG", V558_REG_LCD_IRDG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_IRDR", V558_REG_LCD_IRDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR0", V558_REG_LCD_GLTR0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR1", V558_REG_LCD_GLTR1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR2", V558_REG_LCD_GLTR2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR3", V558_REG_LCD_GLTR3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR4", V558_REG_LCD_GLTR4, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR5", V558_REG_LCD_GLTR5, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR6", V558_REG_LCD_GLTR6, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR7", V558_REG_LCD_GLTR7, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR8", V558_REG_LCD_GLTR8, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR9", V558_REG_LCD_GLTR9, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR10", V558_REG_LCD_GLTR10, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR11", V558_REG_LCD_GLTR11, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR12", V558_REG_LCD_GLTR12, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR13", V558_REG_LCD_GLTR13, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR14", V558_REG_LCD_GLTR14, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR15", V558_REG_LCD_GLTR15, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTR16", V558_REG_LCD_GLTR16, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG0", V558_REG_LCD_GLTG0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG1", V558_REG_LCD_GLTG1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG2", V558_REG_LCD_GLTG2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG3", V558_REG_LCD_GLTG3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG4", V558_REG_LCD_GLTG4, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG5", V558_REG_LCD_GLTG5, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG6", V558_REG_LCD_GLTG6, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG7", V558_REG_LCD_GLTG7, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG8", V558_REG_LCD_GLTG8, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG9", V558_REG_LCD_GLTG9, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG10", V558_REG_LCD_GLTG10, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG11", V558_REG_LCD_GLTG11, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG12", V558_REG_LCD_GLTG12, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG13", V558_REG_LCD_GLTG13, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG14", V558_REG_LCD_GLTG14, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG15", V558_REG_LCD_GLTG15, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTG16", V558_REG_LCD_GLTG16, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB0", V558_REG_LCD_GLTB0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB1", V558_REG_LCD_GLTB1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB2", V558_REG_LCD_GLTB2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB3", V558_REG_LCD_GLTB3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB4", V558_REG_LCD_GLTB4, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB5", V558_REG_LCD_GLTB5, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB6", V558_REG_LCD_GLTB6, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB7", V558_REG_LCD_GLTB7, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB8", V558_REG_LCD_GLTB8, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB9", V558_REG_LCD_GLTB9, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB10", V558_REG_LCD_GLTB10, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB11", V558_REG_LCD_GLTB11, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB12", V558_REG_LCD_GLTB12, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB13", V558_REG_LCD_GLTB13, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB14", V558_REG_LCD_GLTB14, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB15", V558_REG_LCD_GLTB15, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LCD_GLTB16", V558_REG_LCD_GLTB16, 0 }
};

const TRegUnit gJbufReg[] =
{
	{ V558_REGTYPE, "V558_REG_JBUF_S_ADDR", V558_REG_JBUF_S_ADDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_T_ADDR", V558_REG_JBUF_T_ADDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_D_ADDR", V558_REG_JBUF_D_ADDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_JPEG_ADDR", V558_REG_JBUF_JPEG_ADDR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_S_BUF_SIZE", V558_REG_JBUF_S_BUF_SIZE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_D_BUF_SIZE", V558_REG_JBUF_D_BUF_SIZE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_J_BUF_SIZE", V558_REG_JBUF_J_BUF_SIZE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_T_BUF_SIZE", V558_REG_JBUF_T_BUF_SIZE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_BIU_ACC_LENGTH0", V558_REG_JBUF_BIU_ACC_LENGTH0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_BIU_ACC_LENGTH1", V558_REG_JBUF_BIU_ACC_LENGTH1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_CLR_REG", V558_REG_JBUF_CLR_REG, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_GE_CLK_COUNT", V558_REG_JBUF_GE_CLK_COUNT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_CAP_VIEW_CONF", V558_REG_JBUF_CAP_VIEW_CONF, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_J_WEN_PT0", V558_REG_JBUF_J_WEN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_J_WEN_PT1", V558_REG_JBUF_J_WEN_PT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_J_RDN_PT0", V558_REG_JBUF_J_RDN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_J_RDN_PT1", V558_REG_JBUF_J_RDN_PT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_S_WEN_PT0", V558_REG_JBUF_S_WEN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_S_WEN_PT1", V558_REG_JBUF_S_WEN_PT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_S_RDN_PT0", V558_REG_JBUF_S_RDN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_S_RDN_PT1", V558_REG_JBUF_S_RDN_PT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_D_WEN_PT0", V558_REG_JBUF_D_WEN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_D_WEN_PT1", V558_REG_JBUF_D_WEN_PT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_D_RDN_PT0", V558_REG_JBUF_D_RDN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_D_RDN_PT1", V558_REG_JBUF_D_RDN_PT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_T_WEN_PT0", V558_REG_JBUF_T_WEN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_T_WEN_PT1", V558_REG_JBUF_T_WEN_PT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_T_RDN_PT0", V558_REG_JBUF_T_RDN_PT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JBUF_T_RDN_PT1", V558_REG_JBUF_T_RDN_PT1, 0 }
};

const TRegUnit gJpegReg[] =
{
	{ V558_REGTYPE, "V558_REG_JPEG_MODE", V558_REG_JPEG_MODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_CTRL", V558_REG_JPEG_CTRL, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_STATUS", V558_REG_JPEG_STATUS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_BRC", V558_REG_JPEG_BRC, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_TCR", V558_REG_JPEG_TCR, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_QF", V558_REG_JPEG_QF, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_TARGET_WC0", V558_REG_JPEG_TARGET_WC0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_TARGET_WC1", V558_REG_JPEG_TARGET_WC1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_TARGET_WC2", V558_REG_JPEG_TARGET_WC2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_TARGET_WC3", V558_REG_JPEG_TARGET_WC3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_VIDEO_WC0", V558_REG_JPEG_VIDEO_WC0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_VIDEO_WC1", V558_REG_JPEG_VIDEO_WC1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_VIDEO_WC2", V558_REG_JPEG_VIDEO_WC2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_VIDEO_WC3", V558_REG_JPEG_VIDEO_WC3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_HUFF_WC0", V558_REG_JPEG_HUFF_WC0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_HUFF_WC1", V558_REG_JPEG_HUFF_WC1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_HUFF_WC2", V558_REG_JPEG_HUFF_WC2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_HUFF_WC3", V558_REG_JPEG_HUFF_WC3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_WIDTH_H", V558_REG_JPEG_WIDTH_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_WIDTH_L", V558_REG_JPEG_WIDTH_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_HEIGHT_H", V558_REG_JPEG_HEIGHT_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_HEIGHT_L", V558_REG_JPEG_HEIGHT_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_B_H", V558_REG_JPEG_B_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_B_L", V558_REG_JPEG_B_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_FRAME_CNT0", V558_REG_JPEG_FRAME_CNT0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_FRAME_CNT1", V558_REG_JPEG_FRAME_CNT1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_DCT_DATA", V558_REG_JPEG_DCT_DATA, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_QUAN_T", V558_REG_JPEG_QUAN_T, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_HUFF_T", V558_REG_JPEG_HUFF_T, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_NUM_COMPQUANT", V558_REG_JPEG_NUM_COMPQUANT, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP1_P1", V558_REG_JPEG_COMP1_P1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP1_P2", V558_REG_JPEG_COMP1_P2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP2_P1", V558_REG_JPEG_COMP2_P1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP2_P2", V558_REG_JPEG_COMP2_P2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP3_P1", V558_REG_JPEG_COMP3_P1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP3_P2", V558_REG_JPEG_COMP3_P2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP4_P1", V558_REG_JPEG_COMP4_P1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_COMP4_P2", V558_REG_JPEG_COMP4_P2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_BLK_WIDTH", V558_REG_JPEG_BLK_WIDTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA0", V558_REG_JPEG_IMGPARA0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA1", V558_REG_JPEG_IMGPARA1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA2", V558_REG_JPEG_IMGPARA2, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA3", V558_REG_JPEG_IMGPARA3, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA4", V558_REG_JPEG_IMGPARA4, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA5", V558_REG_JPEG_IMGPARA5, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA6", V558_REG_JPEG_IMGPARA6, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA7", V558_REG_JPEG_IMGPARA7, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA8", V558_REG_JPEG_IMGPARA8, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA9", V558_REG_JPEG_IMGPARA9, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA10", V558_REG_JPEG_IMGPARA10, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA11", V558_REG_JPEG_IMGPARA11, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA12", V558_REG_JPEG_IMGPARA12, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA13", V558_REG_JPEG_IMGPARA13, 0, 0 },
	{ V558_REGTYPE, "V558_REG_JPEG_IMGPARA14", V558_REG_JPEG_IMGPARA14, 0 }
};

const TRegUnit gLbufReg[] =
{
	{ V558_REGTYPE, "V558_REG_LBUF_STATUS", V558_REG_LBUF_STATUS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LBUF_YUVMODE", V558_REG_LBUF_YUVMODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LBUF_WORKMODE", V558_REG_LBUF_WORKMODE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LBUF_WIDTH_H", V558_REG_LBUF_WIDTH_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LBUF_WIDTH_L", V558_REG_LBUF_WIDTH_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LBUF_HEIGHT_H", V558_REG_LBUF_HEIGHT_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_LBUF_HEIGHT_L", V558_REG_LBUF_HEIGHT_L, 0, 0 }
};

const TRegUnit gGeReg[] =
{
	{ V558_REGTYPE, "V558_REG_GE_STATUS", V558_REG_GE_STATUS, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_STARTUP", V558_REG_GE_STARTUP, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_CTRL0", V558_REG_GE_CTRL0, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_CTRL1", V558_REG_GE_CTRL1, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_BITWISE", V558_REG_GE_BITWISE, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_COLORDEPTH", V558_REG_GE_COLORDEPTH, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_SPAN_L", V558_REG_GE_DEST_SPAN_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_SPAN_H", V558_REG_GE_DEST_SPAN_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_SPAN_L", V558_REG_GE_SRC_SPAN_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_SPAN_H", V558_REG_GE_SRC_SPAN_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_START_L", V558_REG_GE_SRC_START_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_START_H", V558_REG_GE_SRC_START_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_START_L", V558_REG_GE_DEST_START_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_START_H", V558_REG_GE_DEST_START_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_START_L", V558_REG_GE_PTN_START_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_START_H", V558_REG_GE_PTN_START_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_BG_B", V558_REG_GE_PTN_BG_B, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_BG_G", V558_REG_GE_PTN_BG_G, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_BG_R", V558_REG_GE_PTN_BG_R, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_FG_B", V558_REG_GE_PTN_FG_B, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_FG_G", V558_REG_GE_PTN_FG_G, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTN_FG_R", V558_REG_GE_PTN_FG_R, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_BG_B", V558_REG_GE_SRC_BG_B, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_BG_G", V558_REG_GE_SRC_BG_G, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_BG_R", V558_REG_GE_SRC_BG_R, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_FG_B", V558_REG_GE_SRC_FG_B, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_FG_G", V558_REG_GE_SRC_FG_G, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_SRC_FG_R", V558_REG_GE_SRC_FG_R, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_SABS_L", V558_REG_GE_LINE_SABS_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_SABS_H", V558_REG_GE_LINE_SABS_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_SORD_L", V558_REG_GE_LINE_SORD_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_SORD_H", V558_REG_GE_LINE_SORD_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_EABS_L", V558_REG_GE_LINE_EABS_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_EABS_H", V558_REG_GE_LINE_EABS_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_EORD_L", V558_REG_GE_LINE_EORD_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_EORD_H", V558_REG_GE_LINE_EORD_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_B", V558_REG_GE_LINE_B, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_G", V558_REG_GE_LINE_G, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_LINE_R", V558_REG_GE_LINE_R, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_WIDTH_L", V558_REG_GE_DEST_WIDTH_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_WIDTH_H", V558_REG_GE_DEST_WIDTH_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_HEIGHT_L", V558_REG_GE_DEST_HEIGHT_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_DEST_HEIGHT_H", V558_REG_GE_DEST_HEIGHT_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTNBUF_L", V558_REG_GE_PTNBUF_L, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTNBUF_H", V558_REG_GE_PTNBUF_H, 0, 0 },
	{ V558_REGTYPE, "V558_REG_GE_PTNBUF_WR", V558_REG_GE_PTNBUF_WR, 0 }
};

const TRegList g558Reglist[] =
{
	{ "SIF reg", (PTRegUnit)gSifReg, sizeof(gSifReg) / sizeof(TRegUnit)	},
	{ "Ctrl reg", (PTRegUnit)gCtrlReg, sizeof(gCtrlReg) / sizeof(TRegUnit) },
	{ "Biu reg", (PTRegUnit)gBiuReg, sizeof(gBiuReg) / sizeof(TRegUnit) },
	{ "Isp reg", (PTRegUnit)gIspReg, sizeof(gIspReg) / sizeof(TRegUnit) },
	{ "Ipp reg", (PTRegUnit)gIppReg, sizeof(gIppReg) / sizeof(TRegUnit) },
	{ "LcdIF reg", (PTRegUnit)gLcdIFReg, sizeof(gLcdIFReg) / sizeof(TRegUnit) },
	{ "Jbuf reg", (PTRegUnit)gJbufReg, sizeof(gJbufReg) / sizeof(TRegUnit) },
	{ "Jpeg reg", (PTRegUnit)gJpegReg, sizeof(gJpegReg) / sizeof(TRegUnit) },
	{ "Lbuf reg", (PTRegUnit)gLbufReg, sizeof(gLbufReg) / sizeof(TRegUnit) },
	{ "GE reg", (PTRegUnit)gGeReg, sizeof(gGeReg) / sizeof(TRegUnit) }
};

const int g558ReglistCount = sizeof(g558Reglist) / sizeof(TRegList);




