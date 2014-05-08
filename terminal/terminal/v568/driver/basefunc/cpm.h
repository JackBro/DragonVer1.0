
#ifndef _RDK_CPM_H_
#define _RDK_CPM_H_

#ifdef __cplusplus
extern "C" {
#endif

extern UINT8 gVC0568ChipID;

UINT32 V5B_CpmGetChipID(void);

void   V5B_CpmModReset(UINT16 uVal);
void   V5B_CpmModGlbReset(void);

void   V5B_CpmSetModClkVal(UINT16 uVal);
UINT16 V5B_CpmGetModClkVal(void);

void   V5B_CpmSetIntCtl(UINT8 val);
UINT8  V5B_CpmGetIntCtl(void);
void   V5B_CpmSetIntEn(UINT8 uVal);
UINT8  V5B_CpmGetIntEn(void);
void   V5B_CpmSetIntServ(UINT8 uVal);
UINT8  V5B_CpmGetIntServ(void);
UINT8  V5B_CpmGetIntFlag(void);
void   V5B_CpmSetIntEnSec(UINT8 offset, UINT16 uVal);
UINT16 V5B_CpmGetIntEnSec(UINT8 offset);
UINT16 V5B_CpmGetIntFlagSec(UINT8 offset);

void   V5B_CpmSetCpuBusDir(UINT8 val);
UINT8  V5B_CpmGetCpuBusDir(void);
void   V5B_CpmSetLcdBusDir(UINT8 val);
UINT8  V5B_CpmGetLcdBusDir(void);
void   V5B_CpmSetSenCtrlBusDir(UINT16 val);
UINT16 V5B_CpmGetSenCtrlBusDir(void);
void   V5B_CpmSetSenDataBusDir(UINT16 val);
UINT16 V5B_CpmGetSenDataBusDir(void);

void   V5B_CpmSetGpioCfg(UINT8 cfg);
UINT8  V5B_CpmGetGpioCfg(void);
void   V5B_CpmSetGpioMode(UINT8 mode);
UINT8  V5B_CpmGetGpioMode(void);
void   V5B_CpmSetGpioDirection(UINT8 dir);
UINT8  V5B_CpmGetGpioDirection(void);
void   V5B_CpmSetGpioData(UINT8 data);
UINT8  V5B_CpmGetGpioData(void);

void   V5B_CpmSetSifFlashCtrl(UINT8 data);
UINT8  V5B_CpmGetSifFlashCtrl(void);
void   V5B_CpmtFlashTriggerEnable(void);
void   V5B_CpmtFlashTriggerDisable(void);
void   V5B_CpmtFlashChargeEnable(void);
void   V5B_CpmtFlashChargeDisable(void);

void V5B_CpmSetGpioRenSwitch(UINT8 val);
UINT8 V5B_CpmGetGpioRenSwitch(void);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RDK_CPM_H_

