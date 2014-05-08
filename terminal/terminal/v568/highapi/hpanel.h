#ifndef _RDK_HPANEL_H_
#define _RDK_HPANEL_H_


#ifdef __cplusplus
extern "C" {
#endif

	typedef void (*PPanelCallback)(void);

	typedef struct tag_TPanelParm {
		TRegGroup			poweron;
		TRegGroup			poweroff;
		TRegGroup			sleep;
		TRegGroup			standby;
		TRegGroup			wakeupstb;
		TRegGroup			wakeup;
	} TPanelParm, *PTPanelParm;

	typedef struct tag_TPanelInfo {
		PTPanelParm	master;
		PTPanelParm	slave;
		PPanelCallback	pBacklightOnEx;		//extra backlight on function
	} TPanelInfo, *PTPanelInfo;

	void V5_HPanelSetPanelState(UINT8 state, UINT8 flag);

#ifdef __cplusplus
}
#endif

#endif 
