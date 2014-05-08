#include "internal.h"

//Tcah and Tacc must != 0, The value is varified on arm7 board, liuyong 2003.11.13.

#define B1_TACS		(0x3)	/* 4clk */
#define B1_TCOS		(0x3)	/* 4clk */
#define B1_TACC		(0x7)	/* 14clk */
#define B1_TOCH		(0x3)	/* 4clk */
#define B1_TCAH		(0x3)	/* 4clk */
#define B1_TACP		(0x3)	/* 6clk */
#define B1_PMC		(0x0)	/* no page mode */

#define B3_TACS		(0x3)	/* 0clk */
#define B3_TCOS		(0x3)	/* 0clk */
#define B3_TACC		(0x7)	/* 6clk */
#define B3_TOCH		(0x3)	/* 0clk */
#define B3_TCAH		(0x3)	/* 2clk */
#define B3_TACP		(0x0)	/* 2clk */
#define B3_PMC		(0x0)	/* no page mode */

#define B4_TACS		(0x0)	/* 0clk */
#define B4_TCOS		(0x0)	/* 0clk */
#define B4_TACC		(0x7)	/* 14clk */
#define B4_TOCH		(0x0)	/* 4clk */
#define B4_TCAH		(0x3)	/* 0clk */
#define B4_TACP		(0x0)	/* 2clk */
#define B4_PMC		(0x0)	/* no page mode */

typedef struct tag_TBusContext
{
	int bustype;
	int tacs, tcos, tacc, toch, tcah, tacp, pmc;
} TBusContext, *PTBusContext;

static TBusContext gBusContext[BUS_TOTAL];
const static int BusCfgOff[BUS_TOTAL] = {1, 4, 8, 12, 16, 20, 24, 28};
const static int BusCfgReg[BUS_TOTAL] = {	REG_BANKCON0, REG_BANKCON1, REG_BANKCON2, REG_BANKCON3,
											REG_BANKCON4, REG_BANKCON5, REG_BANKCON6, REG_BANKCON7	};

////////////////////////////////////////////////////////////////////////////////////////////

static void SelectBusType(int idx, int type)
{
	idx = (idx > BUS_IDX_7) ? BUS_IDX_7 : idx;
	switch(type)
	{
	case BUS16 :
		SetMemory(REG_BWSCON, (~(0xf << BusCfgOff[idx])), AND_OPCODE);
		SetMemory(REG_BWSCON, ((BUS16 | SRAMBE03) << BusCfgOff[idx]), OR_OPCODE);
		break;
	case BUS8 :
		SetMemory(REG_BWSCON, (~(0xf << BusCfgOff[idx])), AND_OPCODE);
		SetMemory(REG_BWSCON, ((BUS8 | SRAMBE03) << BusCfgOff[idx]), OR_OPCODE);
		break;
	default:
		return;
	}
	gBusContext[idx].bustype = type;
}

void AdjustBusTiming(int idx, int tacs, int tcos, int tacc, int toch, int tcah, int tacp)
{
	idx = (idx > BUS_IDX_7) ? BUS_IDX_7 : idx;
	tacs = (tacs > 3) ? 3 : tacs;
	tcos = (tcos > 3) ? 3 : tcos;
	tacc = (tacc > 7) ? 7 : tacc;
	toch = (toch > 3) ? 3 : toch;
	tcah = (tcah > 3) ? 3 : tcah;
	tacp = (tacp > 3) ? 3 : tacp;

	WriteMemory( BusCfgReg[idx], ((tacs << 13) + (tcos << 11) + (tacc << 8) + (toch << 6) + (tcah << 4) + (tacp << 2)) );
	gBusContext[idx].tacs = tacs;
	gBusContext[idx].tcos = tcos;
	gBusContext[idx].tacc = tacc;
	gBusContext[idx].toch = toch;
	gBusContext[idx].tcah = tcah;
	gBusContext[idx].tacp = tacp;
}

void BusInit(int idx, int type)
{
	idx = (idx > BUS_IDX_7) ? BUS_IDX_7 : idx;
	memset(&(gBusContext[idx]), 0, sizeof(gBusContext[idx]));
	SelectBusType(idx, type);
	switch(idx)
	{
	case BUS_IDX_1 :
		AdjustBusTiming(BUS_IDX_1, B1_TACS, B1_TCOS, B1_TACC, B1_TOCH, B1_TCAH, B1_TACP);
		break;
	case BUS_IDX_3 :
		AdjustBusTiming(BUS_IDX_3, B3_TACS, B3_TCOS, B3_TACC, B3_TOCH, B3_TCAH, B3_TACP);
		break;
	case BUS_IDX_4 :
		AdjustBusTiming(BUS_IDX_4, B4_TACS, B4_TCOS, B4_TACC, B4_TOCH, B4_TCAH, B4_TACP);
		break;
	default :
		return;
	}
}

void PrintBusTiming(int idx)
{
	idx = (idx > BUS_IDX_7) ? BUS_IDX_7 : idx;

	tmPrintf("Bus Index = %d\n", idx);
	tmPrintf("tacs = 0x%x\n", gBusContext[idx].tacs);
	tmPrintf("tcos = 0x%x\n", gBusContext[idx].tcos);
	tmPrintf("tacc = 0x%x\n", gBusContext[idx].tacc);
	tmPrintf("toch = 0x%x\n", gBusContext[idx].toch);
	tmPrintf("tcah = 0x%x\n", gBusContext[idx].tcah);
	tmPrintf("tacp = 0x%x\n", gBusContext[idx].tacp);
}
