#ifndef _V558HIF_H_
#define _V558HIF_H_

#ifdef __cplusplus
extern "C" {
#endif

void V558_GetReg (UINT32 uAddr, UINT8 *puVal);
void V558_SetReg (UINT32 uAddr, UINT8 uVal);
void V558_ReadSegReg (UINT32* puAddr, UINT8 *puVal, UINT8 uSize);
void V558_WriteSegReg (UINT32* puAddr, UINT8 *puVal, UINT8 uSize);
void V558_ReadSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize);
void V558_WriteSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize);

//Bus bypass
void V558_GetReg_Bypass(UINT16 uAddr, UINT16 *uVal);
void V558_SetReg_Bypass(UINT16 uAddr, UINT16 uVal);
void V558_GetLcdReg_Bypass (UINT16 uAddr, UINT16 *uVal);
void V558_SetLcdReg_Bypass (UINT16 uAddr, UINT16 uVal);

void V558_SetCmd_By(UINT16 uVal);
void V558_SetVal_By(UINT16 uVal);
void V558_SetReg_By(UINT16 uCmd, UINT16 uVal);
void V558_SetData_By(UINT16 uCmd, UINT8* pData, UINT32 uSize);

#ifdef __cplusplus
}
#endif

#endif //_V558HIF_H_
