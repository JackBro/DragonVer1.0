
#ifndef _V5_INITENV_H_
#define _V5_INITENV_H_

#include "../../main/option.h"

#ifdef __cplusplus
extern "C" {
#endif

	enum GBUFMODEenum {
		NORMAL_MODE,
		JBUF_MODE,
		TBUF_MODE,
		IBUF_MODE
	};

	typedef struct tag_TGbufInfo {
		 UINT32 adr;
		 UINT32 jbufadr;
		 UINT32 tbufadr;
		 UINT32 ibufadr;
		 UINT8	mode;
	} TGbufInfo, *PTGbufInfo;

#ifdef	WIN32
	typedef struct tag_TV5PanelContext {
		UINT8	cur_panel;
	} TV5PanelContext, *PTV5PanelContext;
#endif

void V5_InitEnv(void);
void testRegRw(UINT32 adr, UINT32 count);
UINT32 RepeatWriteReg(UINT32 adr, UINT32 times);

void	ClearGbufPoint(void);
UINT32	Sram2Gbuf(UINT32 adr, UINT32 len);
void	SetGbufMode(UINT8 mode);
void	SetGbufAdrInfo(UINT32 adr, UINT32 jadr, UINT32 tadr, UINT32 iadr);
UINT32  RtReqJbuf(UINT32 jbufst, UINT32 jbuflen);

#ifdef	WIN32
void  V5_SetCurPanel(UINT8 idx);
UINT8 V5_GetCurPanel(void);
#endif

#define HJPEG_RT_CAP //For test cap req data in RTOS but others in Monitor,


extern const char *gV568BlayerFmtDesc[];
extern const char *gV568DitherSelDesc[];
extern const char *gV568WorkModeDesc[];
extern const char *gV568PrevYuvmodeDesc[];
extern const char *gV568MemAllocmodeDesc[];
extern const char *gV568BusTypeDesc[];
extern const char *gV568BusModeDesc[];
extern const char *gV568CurSensorDesc[];
extern const char *gV568CurLcdDesc[];

extern const int gV568Bfmtcount;
extern const int gV568DitherCount;
extern const int gV568WmodeCount;
extern const int gV568PrevYuvCount;
extern const int gV568AllocModeCnt;
extern const int gV568BusTypeCount;
extern const int gV568BusModeCount;
extern const int gV568CurSensorCount;
extern const int gV568CurLcdCount;

extern const char* gRstType[];
extern const char* gClkType[];
extern const UINT8 gClknum;
extern const UINT8 gRstnum;

#ifdef	WIN32
extern const PTPanelInfo gW32PanelInfo[];
extern const PTPanelCfg  gW32PanelCfg[];
#endif

#ifdef __cplusplus
}
#endif


#endif


