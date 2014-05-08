#include "basefunc938.h"

#ifndef	V938CMD_DISABLE

#ifdef WIN32
#include <malloc.h>
#include "ComChannel/simhost.h"
#endif

#include "../../userapi/userbaserw938.h"

/*biu context initial setting*/
TBiuContext938 gBiuContext938 = {
	V9_BUS_MULTI8,          // Bus type
	V9_Mul8SetRegByWType,   // Set register function
	V9_Mul8GetRegByWType,   // Get register function
	V9_Mul8WriteSram,       //Write sram function
	V9_Mul8ReadSram,//Read sram function ---fpga version can read
};


void   V9B_BiuSetDatDlySel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_DATA_DLY_SEL, val, BIT_8);
}

UINT8  V9B_BiuGetDatDlySel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_DATA_DLY_SEL, BIT_8);

	return val;
}

void   V9B_BiuSetA0DlySel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_A0_DLY_SEL, val, BIT_8);
}

UINT8  V9B_BiuGetA0DlySel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_A0_DLY_SEL, BIT_8);

	return val;
}

void   V9B_BiuSetRefDlySel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_REF_DLY_SEL, val, BIT_8);
}

UINT8  V9B_BiuGetRefDlySel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_REF_DLY_SEL, BIT_8);

	return val;
}

void   V9B_BiuSetPhaDlySel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_PHASE_DLY_SEL, val, BIT_8);
}

UINT8  V9B_BiuGetPhaDlySel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_PHASE_DLY_SEL, BIT_8);

	return val;
}

void   V9B_BiuSetRefPhaSel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_REF_PHASE_SEL, val, BIT_8);
}

UINT8  V9B_BiuGetRegPhaSel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_REF_PHASE_SEL, BIT_8);

	return val;
}

void   V9B_BiuSetDatInDlySel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_IE_DLY_SEL, val, BIT_8);
}

UINT8  V9B_BiuGetDatInDlySel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_IE_DLY_SEL, BIT_8);

	return val;
}


#ifdef WIN32

void V9B_SetBusType(UINT8 type)
{
    char str[0x100];
	
	sprintf(str, "V9B_SetBusType %x\n", type);
	SendCmd(str);
}

#else

/********************************************************************************
  Description:
	Set chip bus access type
  Parameters:
    type : chip bus access type
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V9B_SetBusType(UINT8 type)
{
	if(type == gBiuContext938.busType)
	{
		return;
	}

	switch(type)
	{
	case V9_BUS_MULTI8:
		gBiuContext938.busType = V9_BUS_MULTI8;
		gBiuContext938.pSetRegFunc = V9_Mul8SetRegByWType;
		gBiuContext938.pGetRegFunc = V9_Mul8GetRegByWType;
		gBiuContext938.pWriteSramFunc = V9_Mul8WriteSram;
		gBiuContext938.pReadSramFunc938 = V9_Mul8ReadSram;
		break;

	default:
		break;

	}
}

#endif


#ifdef WIN32

/********************************************************************************
  Description:
	Delay function
  Parameters:
	count : time(ms)
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V9_Delay(UINT32 count)
{
	SimHost_Delay(count);
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial write address
	buf  : the pointer to data
	size : data buf size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V9_WriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	return SimHost_V9WriteSram(adr, buf, size);
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial read address
	buf  : the pointer to data
	size : data buf size
  Note:
  Returns:
	return read sram actual size
  Remarks:
*********************************************************************************/
UINT32 V9_ReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	return SimHost_V9ReadSram(adr, buf, size);
}

#else //ARM Environment

/********************************************************************************
  Description:
	Delay function
  Parameters:
	count : time(ms)
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V9_Delay(UINT32 count)
{
	V9_HostDelay(count);
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial write address
	buf  : the pointer to data data buffer
	size : data size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V9_WriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 cnt;

	cnt = gBiuContext938.pWriteSramFunc(adr, buf, size);

	return cnt;
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial read address
	buf  : the pointer to data data buffer
	size : data size
  Note:
  Returns:
	return read sram actual size
  Remarks:
*********************************************************************************/
UINT32 V9_ReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 cnt;

	cnt = gBiuContext938.pReadSramFunc938(adr, buf, size);

	return cnt;
}

#endif //WIN32


#ifdef WIN32

#ifndef REGOPER_INWIN32LEVEL //for register debug

/********************************************************************************
  Description:
	Set vaule to chip register, if adr > V9_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr  : register address
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V9_SetReg(UINT32 adr, UINT32 val, UINT8 regWType)
{
	char str[0x100];
	
	sprintf(str, "V9_SetReg %x, %x, %x\n", adr, val, regWType);
	SendCmd(str);
}

/********************************************************************************
  Description:
	Get value from chip register, if adr > V9_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr : register address
  Note:
  Returns:register value 
  Remarks:
*********************************************************************************/
UINT32 V9_GetReg(UINT32 adr, UINT8 regWType)
{
	char cmd[0x100];
	int val;

	sprintf(cmd, "V9_GetReg %x, %x\n", adr, regWType);
	val = SendRetCmd(cmd);

	return (UINT32)val;
}

#endif //REGOPER_INWIN32LEVEL

#else //ARM Environment

/********************************************************************************
  Description:
	Set value to chip register, if adr > V9_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr  : register address
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V9_SetReg(UINT32 adr, UINT32 val, UINT8 regWType)
{
	gBiuContext938.pSetRegFunc(adr, val, regWType);
}

/********************************************************************************
  Description:
	Get value from chip register, if adr > V9_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr  : address
  Note:
  Returns: register value
  Remarks:
*********************************************************************************/
UINT32 V9_GetReg(UINT32 adr, UINT8 regWType)
{
	return gBiuContext938.pGetRegFunc(adr, regWType);
}

#endif //WIN32

UINT8 V9B_SetRegMaskBit8(UINT32 adr, UINT8 mask, UINT8 op)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(adr, BIT_8);
	switch(op)
	{
	case AND_OPCODE:
		val &= mask;
		break;
	case OR_OPCODE:
		val |= mask;
		break;
	default:
		break;;
	}
	V9_SetReg(adr, val, BIT_8);

	return val;
}

#endif //V938CMD_DISABLE



