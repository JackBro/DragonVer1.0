#ifndef _RDK_VCTRL_H_
#define _RDK_VCTRL_H_

#ifdef __cplusplus
extern "C" {
#endif


	#define TSTINT(flg, type) (flg & (0x1 << type)) 


	//chip state
	enum CTRL_STATEenum
	{
		CTRL_NOT_INIT			= 0x0,
		CTRL_INIT_CONTEXT		= 0x1,
		CTRL_INIT_INFO			= 0x2,
		CTRL_OPEN				= 0x3
	};

	void	V5_CtrlInitContext(void);
	void	V5_CtrlSetInfo(TCtrlInfo *pinfo);
	TCtrlContext* V5_CtrlGetContext(void);

	void	V5_CtrlOpen(void);
	void	V5_CtrlClose(void);

	void	V5_CtrlSwReset(UINT16 uType);
	UINT16	V5_CtrlSetModClkOn(UINT16 uType);
	UINT16	V5_CtrlSetModClkOff(UINT16 uType);
	void	V5_CtrlSetIntCtrl(UINT8 triggerType, UINT8 levelType, UINT8 edgeType, UINT8 sigWidth);
	void	V5_CtrlGetIntCtrl(UINT8* triggerType, UINT8* levelType, UINT8* edgeType, UINT8* sigWidth);
	void	V5_CtrlIntSetCallback(UINT8 index, PV5IsrCallback pcall);
	void	V5_CtrlIntEnable(UINT8 index);
	void	V5_CtrlIntDisable(UINT8 index);
	void	_ISR_V5IntHandle(void);

	void	V5_CtrlNormalToBypass(void);
	void	V5_CtrlBypassToNormal(void);

	UINT32	V5_CtrlSetChipClk(UINT32 clkin, UINT32 mclk);
	UINT32	V5_CtrlSetChipClkDirect(UINT32 clkin, UINT8 m, UINT8 n, UINT8 od0, UINT8 od1, UINT8 outdiv);
	UINT32	V5_CtrlGetMclk(void);
	void	V5_CtrlPllOn(void);
	void	V5_CtrlPllOff(void);
	void    V5_CtrlAllModReset(void);


#ifdef __cplusplus
}
#endif

#endif 


