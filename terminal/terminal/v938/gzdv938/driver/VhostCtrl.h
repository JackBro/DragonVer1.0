/** Define Current PlatForm. 
*/

#ifndef	__HOSTCTRL_H_
#define	__HOSTCTRL_H_

#define	ACCESS_PARELLELBUS			1
#define	ACCESS_SERIAL3				2
#define	ACCESS_SERIAL4				3
#define  ACCESS_SPI				4

#define VMD_MAX_WAVE_NUM				2

#define VREG_INT_TRIGER_RISE_EDGE	0x00
#define VREG_INT_TRIGER_FALL_EDGE	0x04
#define VREG_INT_TRIGER_HIGH_LEVEL	0x01
#define VREG_INT_TRIGER_LOW_LEVEL	0x03

#define VREG_INT_WIDTH_MCLK_01		0x00
#define VREG_INT_WIDTH_MCLK_02		0x10
#define VREG_INT_WIDTH_MCLK_03		0x20
#define VREG_INT_WIDTH_MCLK_04		0x30
#define VREG_INT_WIDTH_MCLK_05		0x40
#define VREG_INT_WIDTH_MCLK_06		0x50
#define VREG_INT_WIDTH_MCLK_07		0x60
#define VREG_INT_WIDTH_MCLK_08		0x70
#define VREG_INT_WIDTH_MCLK_09		0x80
#define VREG_INT_WIDTH_MCLK_10		0x90
#define VREG_INT_WIDTH_MCLK_11		0xA0
#define VREG_INT_WIDTH_MCLK_12		0xB0
#define VREG_INT_WIDTH_MCLK_13		0xC0
#define VREG_INT_WIDTH_MCLK_14		0xD0
#define VREG_INT_WIDTH_MCLK_15		0xE0
#define VREG_INT_WIDTH_MCLK_16		0xF0

//////////////////////////////////////// user define //////////////////////////////////////////////
#define VREG_INPUT_CLK_FREQ		24000000
#define VREG_INT_REPEAT_COUNT		0x0100
#define VREG_INT_DELAY_COUNT		0x0100
#define VREG_INT_CTRL_SET			VREG_INT_TRIGER_RISE_EDGE | VREG_INT_WIDTH_MCLK_04
#define	ACCESS_MODE			ACCESS_PARELLELBUS


#define	IIS_SUPPORT	0

#if(ACCESS_MODE == ACCESS_PARELLELBUS)

/* VC0938 register base address */
#define rVmdReg		(*(volatile unsigned char *)0x8000000)
#define rVmdData		(*(volatile unsigned char *)0x8000001)

#elif(ACCESS_MODE == ACCESS_SPI)

	//For SPI

#else

#define SPCLCK_UP_DOWN 	{rPDATF |= (0x01 << 8); rPDATF &= ~(0x01 << 8);}
#define SPCLCK_DOWN_UP 	{rPDATF &= ~(0x01 << 8); rPDATF |= (0x01 << 8);}
#define SPSYNC_UP_DOWN 	{rPDATF |= (0x01 << 7); rPDATF &= ~(0x01 << 7);}

#define SPWBIT(x,bit)	{rPDATF &= ~(0x01 << 5); rPDATF |= (((x>>bit)&0x01)) << 5;}
#define SPRBIT(x,bit)	{*x |= ((rPDATF >> 5) & 0x01) << bit;}

#if(ACCESS_MODE==ACCESS_SERIAL3)

#define	SERIAL_IN		{rPCONF &= ~(0x01 << 10);}
#define	SERIAL_OUT		{rPCONF |= (0x01 << 10);}

#endif

#endif

#define	BUFFER_EMPTY_1_4	0x00
#define	BUFFER_EMPTY_1_2	0x01
#define	BUFFER_EMPTY_3_4	0x02

#define	BUFFER_EVENT_SIZE		BUFFER_EMPTY_1_2
#define	BUFFER_STREAM_SIZE	BUFFER_EMPTY_1_2
#define	BUFFER_MP3_SIZE		BUFFER_EMPTY_1_2

#define	BUFFER_EMPTY_TIME	2

#define	FILESTREAM_SYNC	1
#define	FILESTREAM_ASYNC	2

#define	FILESTREAM_MODE	FILESTREAM_SYNC




UINT8		Vreg_ReadReg		(UINT8 bAddr);
void		Vreg_WriteReg		(UINT8 bAddr, UINT8 bVal);
void		Vreg_WriteMulRegs		(UINT8 bAddr, PUINT8 pbData, UINT16 wSize);
void		Vreg_SetPLL		(void);
void		Vreg_Delay		(UINT32 dTimer);

void 		VPlr_Init(void);
void		VPlr_ResetChip(void);
void		VPlr_EnterCriticalSection(void);
void		VPlr_LeaveCriticalSection(void);

void 		VPlr_Lock(void);
void 		VPlr_Unlock(void);

UINT8		VPlr_GetDeviceState(void);

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)
void 		VPlr_InitGetStream(void);
UINT8 		VPlr_CheckWBuffer(void);
UINT8 		VPlr_CheckRBuffer(void);
UINT32 		VPlr_GetWBufferSize(UINT8 bNum);
void 		VPlr_SetWBufferSize(UINT8 bNum, UINT32 dSize);

void 		VPlr_StartStream(void);
void 		VPlr_StopStream(void);
UINT8 		VPlr_GetStartStream(void);
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////

#endif	//__HOSTCTRL_H_
