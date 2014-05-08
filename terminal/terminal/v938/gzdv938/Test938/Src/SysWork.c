#include <string.h>
#include "..\inc\def.h"
#include "..\inc\44b.h"
#include "..\inc\44blib.h"
#include "..\inc\serial.h"
#include "..\inc\timer.h"
#include "..\inc\intcontrol.h"
#include "..\inc\test.h"

#include "..\..\driver\vdef.h"
#include "..\..\driver\vdevdef.h"
#include "..\..\driver\vdev.h"
#include "..\..\driver\vhostctrl.h"

extern UINT32 g_band;

unsigned int RxState = 0;
int iCmdReady = 0;
int iCmdDataNum = 0;
int iRecDataNum = 0;
UINT32 iCurrentBlockNum = 0;
UINT32 iTotalBlockNum = 0;

UINT8  uImageBuffer[DATA_FILE_LEN];
UINT32 iBufferEndPointer;
UINT32 iBufferImageEndPointer;

UINT8 uEventBuf[EVENT_FILE_LEN];
UINT32 uPtEventCur = 0;
UINT32 uPtEventEnd = 0;

UINT8 uStream0Buf[STREAM0_FILE_LEN];
UINT32 uPtStream0Cur = 0;
UINT32 uPtStream0End = 0;

UINT8 uStream1Buf[STREAM1_FILE_LEN];
UINT32 uPtStream1Cur = 0;
UINT32 uPtStream1End = 0;

UINT8 uStream2Buf[STREAM2_FILE_LEN];
UINT32 uPtStream2Cur = 0;
UINT32 uPtStream2End = 0;

UINT8 uStream3Buf[STREAM3_FILE_LEN];
UINT32 uPtStream3Cur = 0;
UINT32 uPtStream3End = 0;

UINT8 uCommandBuf[COMMAND_FILE_LEN];
UINT32 uPtCommandCur = 0;
UINT32 uPtCommandEnd = 0;

UINT8  RecData[UART_PACKET_LENGTH];

OBW g_OBW;
RSW g_RSW;

static void DrvInitInt(void);
static void UartTx(void){}
static void InitUart0FroReciveCommand(UINT32 baud, void (*func)(UINT8 uOneByte));
static void Timer0Deal(void);
static void ReciveOnePacketData(void);
static void Uart_parase(UINT8 OneByte);

/////////////////////////////////////////////////////////
//initialize Uart0, Timer0, VC0938 hardware reset and 
//VC0938 driver init
/////////////////////////////////////////////////////////
extern int FFS_Read_Page(unsigned int startSec,unsigned int Num,unsigned char *pData);
extern int FFS_Write_Page(unsigned int startSec,unsigned int Num,unsigned char *pData);

void Syswork(void)
{
	g_RSW.bySignature_b1 = 'A';
	g_RSW.bySignature_b2 = 'R';
	g_RSW.bySignature_b3 = 'M';
	g_RSW.bySignature_b4 = 'S';

	g_RSW.Status = 1;

	Uart_TxEmpty(0);

	InitUart0FroReciveCommand(g_band,(void (*)(UINT8 uOneByte)) Uart_parase);
	InitTimer0ForCommandParase(Timer0Deal);
	
	Led_Display(0xf);
	Beep(1);

	Port_V938ChipReset();
	Delay(20);

	DrvInitInt();
	Delay(100);
	Led_Display(0x0);
	Beep(0);
}

////initialize Uart0
void 	InitUart0FroReciveCommand(UINT32 baud, void (*func)(UINT8 uOneByte))
{
	SerialIntInit(0,UART_MODE_DEFAULTS ,UART_CTRL_DEFAULTS|UART_CTRL_REV_TOUT_INTR_ON,
		UART_FIFO_ENSABLE|UART_RX_FIFO_TRLVL_12,baud, (void (*)(UINT8 uOneByte))func, UartTx);
}


////////UART 0 TEST////////////////
void Uart_parase(UINT8 OneByte)
{
	switch (RxState)
	{
			case BEG_STATE:
				if (OneByte== 'A')
				{
					iCmdDataNum = 1;
					RxState = R_STATE;
				}
				break;
			case R_STATE:
				if (OneByte == 'R')
				{
					iCmdDataNum = 2;
					RxState = M_STATE;
				}
				else 
				{ 
					RxState = BEG_STATE;
				}
				break;
			case M_STATE:
				if (OneByte == 'M')
				{
					iCmdDataNum = 3;
					RxState = C_STATE;
				}
				else 
				{ 
					RxState = BEG_STATE;
				}
				break;
			case C_STATE:
				if (OneByte == 'C')
				{
					iCmdDataNum = 4;
					RxState = CMDDATA_STATE;
				}
				else 
				{ 
					RxState = BEG_STATE;
				}
				break;
			case CMDDATA_STATE:
				if(iCmdDataNum < SIZE_OBW)
				{
					*((UINT8*)(&g_OBW)+iCmdDataNum) = OneByte; 
					iCmdDataNum++;
					if(iCmdDataNum >= SIZE_OBW)
					{
						if(g_OBW.byDataLength > 0 && g_OBW.byDataLength <= UART_PACKET_LENGTH)
						{
							iRecDataNum = 0;
							RxState = RECIVEDATA_STATE;
						}
						else
						{
							iCmdReady = 1;
							iCmdDataNum = 0;
							RxState = BEG_STATE;
						}

					}
				}
				else//Exception.
				{

					iCmdDataNum = 0;
					RxState = BEG_STATE;
				}
				break;
			case RECIVEDATA_STATE:
				if(iRecDataNum < g_OBW.byDataLength)
				{
					RecData[iRecDataNum] = OneByte;
					iRecDataNum++;
					if(iRecDataNum >= g_OBW.byDataLength)
					{
						iRecDataNum = 0;
						iCmdReady = 1;
						RxState = BEG_STATE;
					}
				}
				else//Exception.
				{
					iRecDataNum = 0;
					RxState = BEG_STATE;
				}
				break;
			default:
				break;
	}
}

void ReciveOnePacketData()
{
	UINT16 uLength = g_OBW.byPara2_b2;
	if(uLength > 0)
	{
		uLength *= 0x100;
	}
	uLength += g_OBW.byPara2_b1;

	switch(g_OBW.byPara1_b1)
	{
	case 0://Stop
		if(uLength == iCurrentBlockNum)
		{
			iCurrentBlockNum = 0;
			iBufferImageEndPointer = iBufferEndPointer ;
			iBufferEndPointer = 0;
		}
		else
		{
			g_RSW.Status = 1;
			iBufferEndPointer = 0;
			iCurrentBlockNum = 0;
		}
		break;
	case 1://Start
		iBufferEndPointer = 0;
		iCurrentBlockNum = 0;
		iBufferImageEndPointer = 0;
		if(uLength == iCurrentBlockNum)
		{
			iCurrentBlockNum ++;
			memcpy(uImageBuffer+iBufferEndPointer, RecData, g_OBW.byDataLength);
			iBufferEndPointer = g_OBW.byDataLength;
		}
		else
		{
			g_RSW.Status = 1;
			iBufferEndPointer = 0;
			iCurrentBlockNum = 0;
		}
		break;
	case 2://Middle
		if(uLength == iCurrentBlockNum)
		{
			iCurrentBlockNum ++;
			if(iBufferEndPointer < DATA_FILE_LEN - UART_PACKET_LENGTH)
			{
				memcpy(uImageBuffer+iBufferEndPointer, RecData, g_OBW.byDataLength);
				iBufferEndPointer += g_OBW.byDataLength;
			}
		}
		else
		{
			g_RSW.Status = 1;
			iBufferEndPointer = 0;
			iCurrentBlockNum = 0;
		}
		break;
	default://error
		g_RSW.Status = 1;
		break;
	}

	Uart_SendBuffer((char*)&g_RSW,SIZE_RSW);
}

/////////////////////////////////////////////////////////////////////////
//function read/write registers
/////////////////////////////////////////////////////////////////////////
void CmdGetReg(void)
{
	UINT8 uValue = 0;
	UINT32 uAddress = g_OBW.byPara1_b2 + g_OBW.byPara1_b3*0x100 + g_OBW.byPara1_b4*0x10000;
	uValue = Vreg_ReadReg((UINT8)uAddress);
	g_RSW.byPara1_b1 = uValue;
	g_RSW.byPara1_b2 = 0;
	g_RSW.byPara1_b3 = 0;
	g_RSW.byPara1_b4 = 0;
	Uart_SendBuffer((char*)&g_RSW,SIZE_RSW);
}
void CmdSetReg(void)
{
	UINT32 uAddress = g_OBW.byPara1_b2 + g_OBW.byPara1_b3*0x100 + g_OBW.byPara1_b4*0x10000;
	UINT8 uValue = g_OBW.byPara2_b1;
	Vreg_WriteReg((UINT8)uAddress, uValue);
	Uart_SendBuffer((char*)&g_RSW,SIZE_RSW);
}

/////////////////////////////////////////////////////////////////////////
//Arm Timer0 Interrupt service routine
//Deal with host commands
/////////////////////////////////////////////////////////////////////////

extern UINT8      g_RealMidiEnd;
extern UINT8      g_bPollingPlay;
extern UINT8      g_bPlayEvent;
extern void       VC938_RealStop(void);
extern void		  VC938_RealPlay(void);

void Timer0Deal(void)
{
		if (g_RealMidiEnd)
		{
//			VC938_RealStop();
//			VC938_RealPlay();
//			VC938_Stop();
//			VC938_Play();
		}

		if (g_bPollingPlay & g_bPlayEvent)
			Vdev_MainIsr();
		
		//Chip command.
		if(iCmdReady)
		{
			g_RSW.Status = 0;
			switch(g_OBW.byOpenrationCode)
			{
			case CMD_VMVGET_REG:
				CmdGetReg();
				break;
			case CMD_VMVSET_REG:
				CmdSetReg();
				break;
			case CMD_VMVTRANS_DATA:
				ReciveOnePacketData();
				break;
			case CMD_VMVPLAY:
				TestPlay();
				break;
			case CMD_VMVSTOP:
				TestStop();
				g_bPlayEvent = 0;
				break;
			case CMD_VMVDOWNLOADFILE:
				TestDownloadFile();
				break;
			default:
				g_RSW.Status = 1;
				Uart_SendBuffer((char*)&g_RSW,SIZE_RSW);
				break;
			}

			iCmdReady = 0;
	}
}

//////////////////////////////////////////////////////////////////////
//Arm Exint1 initialize
//////////////////////////////////////////////////////////////////////
static void __irq Exint1(void);		//For deal command.
void DrvInitInt(void)
{
 	rPCONG |= 0xc;		//EINT1
   	rPUPG=0x0;			//pull up enable	    
	rEXTINT &= ~(0xf0);
	rEXTINT |= 0x10; 		//"1" level mode

	pISR_EINT1 = (unsigned)Exint1;

	Enable_Intr(nINT_EXT1);
}

///////////////////////////////////////////////////////////////////////
//Arm Exint1 interrupt service routine.
//Deal with VC0938 interrupt request.
///////////////////////////////////////////////////////////////////////
void Exint1()
{
	rI_ISPC=BIT_EINT1;
	Vdev_MainIsr();
}



