
#ifndef _TESTCTRL938_H_
#define _TESTCTRL938_H_

#ifdef __cplusplus
extern "C" {
#endif

	
	//Interrupt type
	enum ISR_INTTYPEenum
    {
		/*** INT section 1 ***/
	    ISR_INT_STMR 	             = 0,
	    ISR_INT_EFIFOEMPTY,
	    ISR_INT_TMR,
	    ISR_INT_EFIFOERROR,
	    ISR_INT_SFIFO0EMPTY,
	    ISR_INT_SFIFO1EMPTY,
	    ISR_INT_SFIFO2EMPTY,
	    ISR_INT_SFIFO3EMPTY,

		/*** INT section 2 ***/
	    ISR_INT_MFIFOEMPTY,
	    ISR_INT_SWI1,
	    ISR_INT_SWI2,
	    ISR_INT_SWI3,
	    ISR_INT_SWI4,
	    ISR_INT_SWI5,
	    ISR_INT_SWI6,
	    ISR_INT_SWI7,
	    
		/*** INT section 3 ***/
		ISR_INT_STM0_END,
	    ISR_INT_STM1_END,
	    ISR_INT_STM2_END,
	    ISR_INT_STM3_END,
	    ISR_INT_MP3_END,

		/*** INT ALL ***/
	    ISR_INT_ALL 	           = 0xFF
	
    };

	extern const int gIntnum938;

	typedef void PIntIsrFunc938(void);

	typedef struct _TIntIsr938
	{
		UINT8            ISRtype;
		UINT8            Desc[30];
		UINT8            INTtype;
		PIntIsrFunc938*  pIsr;
	}TIntIsr938;

	void   V9_ChipCtrlOpen(void);
	void   testIntInit938(void);
	void   testDownAudioFile(UINT8 *pDat, UINT32 datLen);

#ifdef __cplusplus
}
#endif

#endif

