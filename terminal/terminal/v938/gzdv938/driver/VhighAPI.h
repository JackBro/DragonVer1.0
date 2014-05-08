#ifndef __HIGHAPI_H_
#define __HIGHAPI_H_

#include "vplayer.h"
#include "vhostctrl.h"


/**************************************************** 938 API ******************************************/
KVRESULT		VPlr_Initialize(void);
KVRESULT		VPlr_Create(void);
KVRESULT		VPlr_Open(void);
KVRESULT		VPlr_Load(HUGE UINT8 *fileptr, UINT32 filesize, UINT8 src, UINT32* func_id);
KVRESULT		VPlr_Play(UINT32 func_id, UINT32 dwSeekPos, UINT32 repeat, PEFUNCTION pFunc);
KVRESULT 	VPlr_Pause(UINT32 func_id);
KVRESULT		VPlr_Resume(UINT32 func_id);
KVRESULT		VPlr_Stop(UINT32 func_id);
KVRESULT		VPlr_Unload(UINT32 func_id);
KVRESULT		VPlr_Close(void);
KVRESULT		VPlr_Destory(void);

UINT32		VPlr_GetCurTime(UINT32 func_id);
UINT32		VPlr_GetTotalTime(UINT32 func_id);
UINT8    	VPlr_GetDeviceState(void);

KVRESULT 	VPlr_SetMp3VolStep(UINT8 bVol);
KVRESULT 	VPlr_SetGlbVolStep(UINT8 bVol);
KVRESULT		VPlr_SetSPVol(UINT8 uVol);
KVRESULT  	VPlr_SetHPVol(UINT8  uVol);
KVRESULT 	VPlr_SetDACVol(UINT8 bVol);
KVRESULT		VPlr_SetMidiVol(UINT8 uVol_L, UINT8 uVol_R);
KVRESULT		VPlr_SetWaveVol(UINT8 uVol_L, UINT8 uVol_R);
KVRESULT 	VPlr_GetVol(KVSOUNDINFO* info);
KVRESULT		VPlr_SetMP3EQ(UINT8 mode);
KVRESULT	 	VPlr_SetMP3EQEx(UINT8* pbBank);
KVRESULT 	VPlr_ShowLed(UINT8 uFlag, UINT8 uMode, UINT8 uChn);
KVRESULT 	VPlr_MotorShake(UINT8 bEn, UINT32 dTimer);
KVRESULT	VPlr_SetHeadphoneFlag(UINT8 bFlag);

KVRESULT		VPlr_GetCurVersion(KVVERSION* ver);
KVRESULT		VPlr_SetGPIOConfig(UINT8 mode);
KVRESULT		VPlr_GPIO_Ctrl(UINT8 uDir, UINT8 uChan, UINT8 *uLevel);

KVRESULT		VPlr_EnableTotalEQ(void);
KVRESULT		VPlr_DisableTotalEQ(void);
KVRESULT		VPlr_SetTotalEQ(UINT8* bank);
KVRESULT		VPlr_GetTotalEQ(UINT8* bank);

KVRESULT 	VPlr_GetMp3Title(UINT8* addr);
KVRESULT 	VPlr_GetMp3Artist(UINT8* addr);
KVRESULT 	VPlr_GetMp3Album(UINT8* addr);
KVRESULT 	VPlr_GetMp3Genre(UINT8* addr);
KVRESULT 	VPlr_GetMp3Comment(UINT8* addr);
KVRESULT 	VPlr_GetMp3Time(UINT32* addr);
KVRESULT	VPlr_GetMP3EQ(UINT8 * pbBank);

KVRESULT 	VPlr_SetMp3AtoB(UINT32 dStart, UINT32 dEnd);
KVRESULT 	VPlr_SetMp3Speed(UINT8 bMode, UINT8 bSpeed);
UINT32		Vplr_GetCurPolyphonyNum(void);

KVRESULT 	VPlr_DriveMotor(UINT8 uFlag, UINT8 uMode, UINT8 uChn, UINT8 uInterval);
KVRESULT		VPlr_PAControl(UINT8 pdwn, UINT8 mute);
KVRESULT 	VPlr_PlayGameEffect(HUGE UINT8* pbBuffer,UINT32 dBuffer_Size);
KVRESULT	VPlr_PlayKeyEffect(UINT8 bKeyId, UINT8 bTimbreId, UINT16 wDuration);



void VPlr_ResetChip(void);

#ifdef IIS_SUPPORT
KVRESULT	VPlr_IISSetMode(UINT8 mode, UINT8 freq);
KVRESULT	VPlr_EnableIIS(void);
KVRESULT	VPlr_DisableIIS(void);
#endif

void		VPlat_MainIsr(void);

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)
UINT32		VPlr_GetStream(void);
#endif
/**************************************************** 938 API ******************************************/


#endif	//__HIGHAPI_H_


