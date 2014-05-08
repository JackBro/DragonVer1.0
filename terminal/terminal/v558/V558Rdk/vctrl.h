#ifndef _VCTRL_H_
#define _VCTRL_H_

#ifdef __cplusplus
extern "C" {
#endif
#ifndef CTRLFAILED
	#define CTRLFAILED 0xff
#endif

#ifndef SUCCEED
	#define SUCCEED 0
#endif

#define CHIP_PLLIN_MAX 130000
#define CHIP_PLLIN_MIN 65000

#define CHIP_PLLDIV_MAX 0X20
#define CHIP_PLLDIV_MIN 0X2
#define CHIP_PLLDIV_HL_MAX 0XF
#define CHIP_PLLDIV_HL_MIN 0X1

#define CHIP_IDIV_MAX 0X1F
#define CHIP_IDIV_MIN 0X1

#define CHIP_FDIV_MAX 0X3F
#define CHIP_FDIV_MIN 0X1

#define UINT16 unsigned short
#define UINT8 unsigned char
#define UINT32 unsigned long

typedef struct _CLK_OUT_PARM
{
	UINT32 clkoutput;
	UINT8 idiv, fdiv, plldivl, plldivh;
}CLK_OUT_PARM, * PCLK_OUT_PARM;

void CtrlSetChipSel(UINT8 uSel);
void CtrlGetChipSel(UINT8* uSel);
void CtrlSetInterfaceAsynOrSyc(UINT8 uVal);
UINT8 CtrlGetInterfaceAsynOrSyc(void);
void CtrlSetWeDrive(UINT8 uVal);
UINT8 CtrlGetWeDrive(void);
void CtrlSetCpuPinCtrl(UINT8 uVal);
UINT8 CtrlGetCpuPinCtrl(void);
void CtrlSetLcdPinCtrl(UINT8 uVal);
UINT8 CtrlGetLcdPinCtrl(void);
void CtrlSetSnrPinCtrl(UINT8 uVal);
UINT8 CtrlGetSnrPinCtrl(void);
void CtrlSetUmCtrl(UINT8 uVal);
UINT8 CtrlGetUmCtrl(void);
void CtrlSetMiscCtrl(UINT8 uVal);
UINT8 CtrlGetMiscCtrl(void);


#ifdef __cplusplus
}
#endif

#endif //_VCTRL_H_

