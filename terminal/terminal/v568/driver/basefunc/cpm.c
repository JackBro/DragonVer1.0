
#include "basefunc.h"

#ifndef	V578CMD_DISABLE

UINT8 gVC0568ChipID = DESIGNIN_VER;

/*Get chip id*/
UINT32 V5B_CpmGetChipID(void)
{
	UINT32 chipID;

	chipID = V5_GetReg(V5_REG_CPM_CHIP_ID_0, BIT_32_LTOH);

	
	gVC0568ChipID = DESIGNIN_VER;

	if(chipID != MPWCHIPID)
		gVC0568ChipID = MASS_VER;

	return chipID;
}

/*Reset specific module by software*/
void V5B_CpmModReset(UINT16 uVal)
{
	V5_SetReg(V5_REG_CPM_RSTCTRL1_L, uVal, BIT_16_LTOH);
}

/*Reset all module by software*/
void V5B_CpmModGlbReset(void)
{
	V5_SetReg(V5_REG_CPM_RSTCTRL2, (UINT32)(0x80), BIT_8);
}

/*Off clock of specific module by software*/
void V5B_CpmSetModClkVal(UINT16 uVal)
{
	V5_SetReg(V5_REG_CPM_CLKOFF_L, uVal, BIT_16_LTOH);
}

/*Get off clock configuration value*/
UINT16 V5B_CpmGetModClkVal(void)
{
	UINT16 val;

	val = (UINT16)V5_GetReg(V5_REG_CPM_CLKOFF_L, BIT_16_LTOH);

	return val;
}

/*Set interrupt control*/
void V5B_CpmSetIntCtl(UINT8 val)
{
	V5_SetReg(V5_REG_CPM_INTCTRL, val, BIT_8);
}

/*Get interrupt control*/
UINT8 V5B_CpmGetIntCtl(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_INTCTRL, BIT_8);

	return val;
}

/*Set interrupt enable configuration*/
void V5B_CpmSetIntEn(UINT8 uVal)
{
	V5_SetReg(V5_REG_CPM_INTEN, uVal, BIT_8);
}

/*Get interrupt enable configuration*/
UINT8 V5B_CpmGetIntEn(void)
{
	UINT8 val;
	
	val = (UINT8)V5_GetReg(V5_REG_CPM_INTEN, BIT_8);

	return val;
}

/*Set interrupt serve flag*/
void V5B_CpmSetIntServ(UINT8 uVal)
{
	V5_SetReg(V5_REG_CPM_INTSERV, uVal, BIT_8);
}

/*Get interrupt serve flag*/
UINT8 V5B_CpmGetIntServ(void)
{
	UINT8 val;
	
	val = (UINT8)V5_GetReg(V5_REG_CPM_INTSERV, BIT_8);

	return val;
}

/*Get interrupt flag*/
UINT8 V5B_CpmGetIntFlag(void)
{
	UINT8 val;
	
	val = (UINT8)V5_GetReg(V5_REG_CPM_INTFLAG, BIT_8);

	return val;
}

/*Set interrupt enable configuration of specific module*/
void V5B_CpmSetIntEnSec(UINT8 offset, UINT16 uVal)
{
	V5_SetReg(V5_REG_CPM_INTEN0+(offset<<1), (UINT8)(uVal & 0xff), BIT_8);
}

/*Get interrupt enable configuration of specific module*/
UINT16 V5B_CpmGetIntEnSec(UINT8 offset)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_CPM_INTEN0+(offset<<1), BIT_8);

    return (UINT16)x & 0xff;
}

/*Get interrupt flag of specific module*/
UINT16 V5B_CpmGetIntFlagSec(UINT8 offset)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_CPM_INTFLAG0+(offset<<1), BIT_8);

    return (UINT16)x & 0xff;
}

/*Set CPU bus direction configuration*/
void V5B_CpmSetCpuBusDir(UINT8 val)
{
    V5_SetReg(V5_REG_CPM_CPU_PC, val, BIT_8);
}

/*Get CPU bus direction configuration*/
UINT8 V5B_CpmGetCpuBusDir(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_CPU_PC, BIT_8);

	return val;
}

/*Set lcd bus direction configuration*/
void V5B_CpmSetLcdBusDir(UINT8 val)
{
    V5_SetReg(V5_REG_CPM_LCD_PC, val, BIT_8);
}

/*Get lcd bus direction configuration*/
UINT8 V5B_CpmGetLcdBusDir(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_LCD_PC, BIT_8);

	return val;
}

/*Set sensor interface signals direction and pullup resistance configuration*/
void V5B_CpmSetSenCtrlBusDir(UINT16 val)
{
    V5_SetReg(V5_REG_CPM_CS_PC_L, val, BIT_16_LTOH);
}

/*Get sensor interface signals direction and pullup resistance configuration*/
UINT16 V5B_CpmGetSenCtrlBusDir(void)
{
	UINT16 val;

	val = (UINT16)V5_GetReg(V5_REG_CPM_CS_PC_L, BIT_16_LTOH);

	return val;
}

/*Set sensor data bus pulldown resistance configuration*/
void V5B_CpmSetSenDataBusDir(UINT16 val)
{
    V5_SetReg(V5_REG_CPM_CS_DATA_PC_L, val, BIT_16_LTOH);
}

/*Get sensor data bus pulldown resistance configuration*/
UINT16 V5B_CpmGetSenDataBusDir(void)
{
	UINT16 val;

	val = (UINT16)V5_GetReg(V5_REG_CPM_CS_DATA_PC_L ,BIT_16_LTOH);

	return val;
}

/*Set GPIO configuration*/
void V5B_CpmSetGpioCfg(UINT8 cfg)
{
	V5_SetReg(V5_REG_CPM_GPIO_CFG, cfg, BIT_8);
}

/*Get GPIO configuration*/
UINT8 V5B_CpmGetGpioCfg(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_GPIO_CFG, BIT_8);

	return val;
}

/*Set GPIO mode*/
void V5B_CpmSetGpioMode(UINT8 mode)
{
	V5_SetReg(V5_REG_CPM_GPIO_MODE, mode, BIT_8);
}

/*Get GPIO mode*/
UINT8 V5B_CpmGetGpioMode(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_GPIO_MODE, BIT_8);

	return val;
}

/*Set GPIO direction*/
void V5B_CpmSetGpioDirection(UINT8 dir)
{
	V5_SetReg(V5_REG_CPM_GPIO_DIR, dir, BIT_8);
}

/*Get GPIO direction*/
UINT8 V5B_CpmGetGpioDirection(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_GPIO_DIR, BIT_8);

	return val;
}

/*Set GPIO data*/
void V5B_CpmSetGpioData(UINT8 data)
{
	V5_SetReg(V5_REG_CPM_GPIO_P0, data, BIT_8);
}

/*Get GPIO data*/
UINT8 V5B_CpmGetGpioData(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_GPIO_P0, BIT_8);
	
	return val;
}

/*Set configuration of SIF flash*/
void V5B_CpmSetSifFlashCtrl(UINT8 data)
{
	V5_SetReg(V5_REG_CPM_SIF_FLASH, data, BIT_8);
}

/*Get configuration of SIF flash*/
UINT8 V5B_CpmGetSifFlashCtrl(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_SIF_FLASH, BIT_8);
	
	return val;
}

/*Enable flash trigger*/
void V5B_CpmtFlashTriggerEnable(void)
{
	V5B_SetRegMaskBit8(V5_REG_CPM_SIF_FLASH, (UINT8)BIT0, OR_OPCODE);
}

/*Disable flash trigger*/
void V5B_CpmtFlashTriggerDisable(void)
{
	V5B_SetRegMaskBit8(V5_REG_CPM_SIF_FLASH, (UINT8)~BIT0, AND_OPCODE);
}

/*Enable flash charge*/
void V5B_CpmtFlashChargeEnable(void)
{
	V5B_SetRegMaskBit8(V5_REG_CPM_SIF_FLASH, (UINT8)BIT1, OR_OPCODE);
}

/*Disable flash charge*/
void V5B_CpmtFlashChargeDisable(void)
{
	V5B_SetRegMaskBit8(V5_REG_CPM_SIF_FLASH, (UINT8)~BIT1, AND_OPCODE);
}

/*Set control of GPIO pullup resistance*/
void V5B_CpmSetGpioRenSwitch(UINT8 val)
{
    V5_SetReg(V5_REG_CPM_GPIO_REN_PC, val, BIT_8);
}

/*Get control of GPIO pullup resistance*/
UINT8 V5B_CpmGetGpioRenSwitch(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_CPM_GPIO_REN_PC, BIT_8);

	return val;
}

#endif //V578CMD_DISABLE

