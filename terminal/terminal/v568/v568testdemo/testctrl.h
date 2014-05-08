
#ifndef _TESTCTRL_H_
#define _TESTCTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum CTRL_CMD_TEST
	{
		CTRL_CMD_INT_INIT = 0,
		CTRL_CMD_TESTPORT,
		CTRL_CMD_BYPASS,
		CTRL_CMD_BY_REG,
		CTRL_CMD_BY_DATA,
		CTRL_CMD_BY_PANEL_INIT,
		CTRL_CMD_INT_RW_SRAM
	};
	
	enum ISR_INTenum
	{
		ISR_INT_SIF		= 0,
		ISR_INT_ISP		,
		ISR_INT_LBUF	,
		ISR_INT_JPEG	,
		ISR_INT_GE		,
		ISR_INT_MARB0	,
		ISR_INT_LCDC	,
		ISR_INT_MARB1
	};

	typedef void PIntIsrFunc(UINT16 flg);

	typedef struct _TIntIsr
	{
		UINT8 ISRtype;
		UINT8 Desc[30];
		UINT8 INTtype;
		PIntIsrFunc* pIsr;
	}TIntIsr;

	extern int gIntnum;
	extern TIntIsr gIntisr[];

	void TestCtrlInit(void);
	void V5_ChipCtrlOpen(void);

	void   testSramRw(UINT32 adr, UINT32 size);
	void   testSramWriteBackRw(UINT32 adr, UINT32 size);
	void   V5_SetRegTest(UINT32 adr, UINT8 val);
	UINT8  V5_GetRegTest(UINT32 adr);
	UINT8  testSramRwTime(UINT32 adr, UINT32 size, UINT8 w);

	UINT32 V5_Mul16NoPortReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
    UINT32 V5_Mul16NoPortWriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
    UINT32 V5_Mul8NoPortReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
    UINT32 V5_Mul8NoPortWriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
    UINT32 V5_SepNoPortReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
    UINT32 V5_SepNoPortWriteSram(UINT32 adr, UINT8 *buf, UINT32 size);

	void   testIntInit(void);
	void   V5_PortSwitchTest(UINT8 port);
    void   testRegConRead(UINT32 addr, UINT32 readCnt);
	UINT8  V5_SetBusTypeMulOrSep(void);

	/*the follow functions written for sram write back test*/
    UINT32 V5_WriteSramTest(UINT32 adr, UINT8 *buf, UINT32 size);
    UINT32 V5_ReadSramTest(UINT32 adr, UINT8 *buf, UINT32 size);


#ifdef __cplusplus
}
#endif

#endif

