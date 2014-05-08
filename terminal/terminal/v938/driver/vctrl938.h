#ifndef _RDK_VCTRL938_H_
#define _RDK_VCTRL938_H_

#ifdef __cplusplus
extern "C" {
#endif


	//chip state
	enum CTRL_STATEenum938
	{
		CTRL_NOT_INIT938			= 0x0,
		CTRL_INIT_CONTEXT938		= 0x1,
		CTRL_INIT_INFO938			= 0x2,
		CTRL_OPEN938				= 0x3
	};
    
	//Global control register bit 
	enum CTRL_GLOBALCTRLBitenum
	{
		GLBCTRL_START_SEQ                              = BIT7,
        GLBCTRL_START_TMR                              = BIT6,
        GLBCTRL_WS_PAUSE                               = BIT5,
        GLBCTRL_MONO_OUT                               = BIT4,
        GLBCTRL_DIS_SRCON                              = BIT3,
        GLBCTRL_DIS_FM                                 = BIT2,
        GLBCTRL_DIS_AM                                 = BIT1,
        GLBCTRL_START_STMR                             = BIT0
	};

	//Pll mode
	enum CTRL_PLLMODEenum
	{
		CTRL_PLL_WORK_MODE           = 0x0,
		CTRL_PLL_BYPASS_MODE         = 0x1
	};

	//Interrupt type
	enum CTRL_INTTYPEenum
    {
		/*** INT section 1 ***/
	    CTRL_INT_STMR 	             = 0,
	    CTRL_INT_EFIFOEMPTY,
	    CTRL_INT_TMR,
	    CTRL_INT_EFIFOERROR,
	    CTRL_INT_SFIFO0EMPTY,
	    CTRL_INT_SFIFO1EMPTY,
	    CTRL_INT_SFIFO2EMPTY,
	    CTRL_INT_SFIFO3EMPTY,

		/*** INT section 2 ***/
	    CTRL_INT_MFIFOEMPTY,
	    CTRL_INT_SWI1,
	    CTRL_INT_SWI2,
	    CTRL_INT_SWI3,
	    CTRL_INT_SWI4,
	    CTRL_INT_SWI5,
	    CTRL_INT_SWI6,
	    CTRL_INT_SWI7,
	    
		/*** INT section 3 ***/
		CTRL_INT_STM0_END,
	    CTRL_INT_STM1_END,
	    CTRL_INT_STM2_END,
	    CTRL_INT_STM3_END,
	    CTRL_INT_MP3_END,

		/*** INT ALL ***/
	    CTRL_INT_ALL 	           = 0xFF
	
    };

	//chip work mode
	enum CTRL_WORKMODEenum938
	{
		CTRL_STANDBY			    = 0x0,
		CTRL_IDLE          		    = 0x1,
		CTRL_NORMAL      			= 0x2,
		CTRL_PAUSING				= 0x3
	};

	//IIS mode
	enum CTRL_IISMODEenum938
	{
		CTRL_IIS_MASTER			    = 0x0,
		CTRL_IIS_SLAVE1    		    = 0x1,
		CTRL_IIS_SLAVE2    			= 0x2
	};

	//IIS frequece
	enum CTRL_IISFREQenum938
	{
		CTRL_IIS_FREQ1			    = 0x1,
		CTRL_IIS_FREQ2    		    = 0x2,
		CTRL_IIS_FREQ3    			= 0x3,
		CTRL_IIS_FREQ4    			= 0x4
	};

	void	V9_CtrlInitContext(void);
	void	V9_CtrlSetInfo(TCtrlInfo938 *pinfo);
	TCtrlContext938* V9_CtrlGetContext(void);

	void	V9_CtrlOpen(void);
	void	V9_CtrlClose(void);

	void    V9_CtrlSetEventFifo(void);
	void    V9_CtrlEventInt(void);
	void    V9_CtrlSetStreamFifo(void);
	void    V9_CtrlStreamInt(void);
	void    V9_CtrlSetCommandFifo(void);

	void    V9_CtrlSetTestMod(void);
    void    V9_CtrlSetNormalMod(void);
	void    V9_CtrlSetUmCtrl(UINT8 modSel, UINT8 modGroupSel);
 
	/***************** power ****************/
	void    V9_CtrlSetPmcCtrl(UINT8 reg_pwdb, UINT8 iref_pwdb, UINT8 vref_pwdb);

	/***************** clock *****************/
	void    V9_CtrlSwReset(UINT8 uType);
	void    V9_CtrlResetClk(void);
	void    V9_CtrlSetPll(UINT32 mclk);
    void	V9_CtrlStartClk(UINT8 pllMode);
	void    V9_CtrlStopClk(void);
    UINT32  V9_CtrlGetMclk(void);

	/***************** interrupt *****************/
    void    V9_CtrlSetIntCtrl(UINT8 triggerType, UINT8 levelType, UINT8 edgeType, UINT8 sigWidth);
    void    V9_CtrlGetIntCtrl(UINT8* triggerType, UINT8* levelType, UINT8* edgeType, UINT8* sigWidth);
    void    V9_CtrlIntSetCallback(UINT8 index, PV9IsrCallback pcall);
    void    V9_CtrlIntEnable(UINT8 intType);
    void    V9_CtrlIntDisable(UINT8 intType);
	void    V9_CtrlSetIntLock(void);
	void    V9_CtrlClearIntLock(void);
	void    V9_CtrlSetIntServe(UINT8 intBit);
	void    V9_CtrlClearIntServe(void);
	void    V9_CtrlClearSwi(UINT8 intType);
	void    _ISR_V9IntHandle(void);

	/***************** Timer ****************/
	void    V9_CtrlSetTmrBase(UINT16 val);
	UINT16  V9_CtrlGetTmrBase(void);
	void	V9_CtrlSetTmrTimer(UINT32 val);
	UINT32	V9_CtrlGetTmrTimer(void);
	void    V9_CtrlRstTmrTimer(void);
	void    V9_CtrlStartTmrTimer(void);
	void    V9_CtrlStopTmrTimer(void);
	void	V9_CtrlSetSTmrTimer(UINT8 val);
	UINT8	V9_CtrlGetSTmrTimer(void);
	void    V9_CtrlRstSTmrTimer(void);
	void    V9_CtrlStartSTmrTimer(void);
	void    V9_CtrlStopSTmrTimer(void);

	/******************* IIS ****************/
	void    V9_CtrlEnIIS(void);
    void    V9_CtrlDisIIS(void);
    void    V9_CtrlSetIISMod(UINT8 mod, UINT8 freq);


	/* Three mode transform:   standby <------> idle <-------> normal */
    void    V9_CtrlStandby2Idle(void);
	void    V9_CtrlIdle2Normal(void);
	void    V9_CtrlNormal2Idle(void);
	void    V9_CtrlIdle2Standby(void);

#ifdef __cplusplus
}
#endif

#endif  //  _RDK_VCTRL938_H_ 


