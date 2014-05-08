#include	"internal.h"


/////////////////////////////////////////////////////////////////////////

static void	NandSet8(int val);
static int	NandGet8(void);
static void NandSet16(int val);
static int	NandGet16(void);

static const TNandAccess gNandAccess = {
	{ NandSet8, NandSet16},
	{ NandGet8, NandGet16}
};

/////////////////////////////////////////////////////////////////////////

static void NandSet8(int val)
{
	NAND_SET_IO8(val);
}

static int NandGet8(void)
{
	return NAND_GET_IO8();
}

static void NandSet16(int val)
{
	NAND_SET_IO16(val);
}

static int NandGet16(void)
{
	return NAND_GET_IO16();
}


/////////////////////////////////////////////////////////////////////////

void NandDelay(int cnt)
{
	int i = 0;
	for(i = 0; i < cnt; i++);
}

#ifdef	WIN32

void NandSetCmdBase(int cmd, int unit)
{
	SimHost_WriteNandCmd(cmd, unit);
}

void NandSetAddrBase(int addr, int unit)
{
	SimHost_WriteNandAddr(addr, unit);
}

void NandSetDataBase(int data, int unit)
{
	SimHost_WriteNandData(data, unit);
}

int NandGetDataBase(int unit)
{
	return SimHost_ReadNandData(unit);
}

#else

void NandSetCmdBase(int cmd, int unit)
{
	NandSetMode(ENABLE_CMD);
//	NandDelay(0);
	if(gNandAccess.pNandSet[unit-1] != NULL)
		gNandAccess.pNandSet[unit-1](cmd);
}

void NandSetAddrBase(int addr, int unit)
{
	NandSetMode(ENABLE_ADR);
//	NandDelay(0);
	if(gNandAccess.pNandSet[unit-1] != NULL)
		gNandAccess.pNandSet[unit-1](addr);
}

void NandSetDataBase(int data, int unit)
{
	NandSetMode(ENABLE_DAT);
//	NandDelay(0);
	if(gNandAccess.pNandSet[unit-1] != NULL)
		gNandAccess.pNandSet[unit-1](data);
}

int NandGetDataBase(int unit)
{
	NandSetMode(ENABLE_DAT);
//	NandDelay(0);
	if(gNandAccess.pNandSet[unit-1] != NULL)
		return gNandAccess.pNandGet[unit-1]();
	return 0;
}

#endif
