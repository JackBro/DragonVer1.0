


#ifndef _RDK_BIU_H_
#define _RDK_BIU_H_

#ifdef __cplusplus
extern "C" {
#endif


	//==============Bypss select type==================
    enum BYPASSSELTYPEenum
    {
	    BY_GPIO = 0x0,
	    BY_REG  = 0x1
     };

	/*Panel number*/
	enum BYPASSPANELSELenum{
        MASTER_PANEL          = 0x0,
	    SLAVE_PANEL
    };


typedef void   SetRegFunc(UINT32 adr, UINT32 val, UINT8 regWType);
typedef UINT32 GetRegFunc(UINT32 adr, UINT8 regWType);
typedef UINT32 ReadSramFunc(UINT32 adr, UINT8 *buf, UINT32 size);
typedef UINT32 WriteSramFunc(UINT32 adr, UINT8 *buf, UINT32 size);
typedef UINT32 WriteSramRevFunc(UINT32 adr, UINT8 *buf, UINT32 size);
typedef UINT32 WriteSramOnWordFunc(UINT32 adr, UINT32 dat, UINT32 size);

typedef void   BySetRSLowFunc(UINT16 val);
typedef UINT16 ByGetRSLowFunc(void);
typedef void   BySetRSHighFunc(UINT16 val);
typedef UINT16 ByGetRSHighFunc(void);

// Biu module context
typedef struct tag_TBiuContext
{
	UINT8                      busType; // Bus type
	UINT8                      bypassSelType; //bypass select type; 1:by register, 0:by GPIO
	UINT8                      bypassRegAddr;//bypass register address
	UINT8                      bypassCsSelCon; //bypass cs select config
	UINT8                      bypassPanelSel; //panel selected in bypass mode
	SetRegFunc*                pSetRegFunc; // Set register function
	GetRegFunc*                pGetRegFunc; // Get register function
	ReadSramFunc*              pReadSramFunc; // Read sram function
	WriteSramFunc*             pWriteSramFunc; //Write sram function
	WriteSramRevFunc*          pWriteSramRevFunc;//Write sram reverse two bytes function
	WriteSramOnWordFunc*       pWriteSramOnWordFunc; //Write sram on word not buffer function
	BySetRSLowFunc*            pBySetRSLowFunc;      //set rs low function in bypass mode
    ByGetRSLowFunc*            pByGetRSLowFunc;      //get rs low function in bypass mode
	BySetRSHighFunc*           pBySetRSHighFunc;     //set rs high function in bypass mode 
    ByGetRSHighFunc*           pByGetRSHighFunc;     //get rs high function in bypass mode
} TBiuContext, *PTBiuContext;

extern TBiuContext gBiuContext;


void   V5B_BiuSetTestReg(UINT16 uVal);
UINT16 V5B_BiuGetTestReg(void);
void   V5B_BiuPortEnable(void);
void   V5B_BiuPortDisable(void);
void   V5B_BiuSelWrite(void);
void   V5B_BiuSelRead(void);
UINT8  V5B_BiuGetRdWr(void);
void   V5B_BiuMul16Init(void);
void   V5B_BiuSetMul_8_16(UINT8 val);
void   V5B_BiuSetBypassType(UINT8 uVal);
UINT32 V5B_BiuGetCurMemAddr(void);

void   V5B_BiuSetPllM(UINT8 val);
UINT8  V5B_BiuGetPllM(void);
void   V5B_BiuSetPllN(UINT8 val);
UINT8  V5B_BiuGetPllN(void);
void   V5B_BiuSetPllOD(UINT8 od0, UINT8 od1);
void   V5B_BiuGetPllOD(UINT8* od0, UINT8* od1);
void   V5B_BiuSetClkDiv(UINT8 val);
UINT8  V5B_BiuGetClkDiv(void);
void   V5B_BiuSetPllTest(UINT8 val);
void   V5B_BiuPllBypassEnable(void);
void   V5B_BiuPllBypassDisable(void);
void   V5B_BiuPllPowerDownEnable(void);
void   V5B_BiuPllPowerDownDisable(void);

void   V5B_BiuSetBypassRegAddr(UINT8 val);
UINT32 V5B_BiuGetBypassRegAddr(void);
void   V5B_BiuSetLcdIfCon(UINT8 val);
UINT8  V5B_BiuGetLcdIfCon(void);

void   V5B_BiuXclkEanble(void);
void   V5B_BiuXclkDisable(void);

void   V5_BySetPanelSel(UINT8 panelSel);
void   V5_BySetRSLow(UINT16 val);
UINT16 V5_ByGetRSLow(void);
void   V5_BySetRSHigh(UINT16 val);
UINT16 V5_ByGetRSHigh(void);

void   V5B_BiuSetAddrDelaySel(UINT8 val);
UINT8  V5B_BiuGetAddrDelaySel(void);
void   V5B_BiuSetDataOutDelaySel(UINT8 val);
UINT8  V5B_BiuGetDataOutDelaySel(void);
void   V5B_BiuSetDataInDelaySel(UINT8 val);
UINT8  V5B_BiuGetDataInDelaySel(void);
void   V5B_BiuSetDataOenDelaySel(UINT8 val);
UINT8  V5B_BiuGetDataOenDelaySel(void);
void   V5B_BiuSetCsDelaySel(UINT8 val);
UINT8  V5B_BiuGetCsDelaySel(void);
void   V5B_BiuSetWrThruDelaySel(UINT8 val);
UINT8  V5B_BiuGetWrThruDelaySel(void);

void   V5B_BiuSetBypassSelType(UINT8 selType);
void   V5B_SetBypassOrNormal(UINT8 byOrNormal);

UINT8  V5B_SetRegMaskBit8(UINT32 adr, UINT8 mask, UINT8 op);
UINT16 V5B_SetRegMaskBit16(UINT32 adr, UINT16 mask, UINT8 op);

void   V5_InitBusType(UINT8 type);
void   V5B_SetBusType(UINT8 type);
void   V5B_Mul16SetBypassOrNormalDir(UINT8 byOrNormal);
void   V5B_Mul16SetBypassOrNormal(UINT8 byOrNormal);
void   V5B_Mul8SetBypassOrNormal(UINT8 byOrNormal);

void   V5_SetReg(UINT32 adr, UINT32 val, UINT8 regWType);
UINT32 V5_GetReg(UINT32 adr, UINT8 regWType);
UINT32 V5_ReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_WriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size);
void   V5_Delay(UINT32 count);


#ifdef __cplusplus
}
#endif

#endif //_RDK_BIU_H_

