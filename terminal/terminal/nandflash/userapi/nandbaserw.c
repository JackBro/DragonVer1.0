#include	"../../s3c44box/s3c44box.h"
#include	"nandbaserw.h"


#define	NAND_RB		0x01										// PORT C.0
#define	NAND_CE		0x02										// PORT C.1
#define	NAND_CLE	0x04										// PORT C.2
#define	NAND_ALE	0x08										// PORT C.3


///////////////////////////////////////////////////////////////////////////////////
//                                 Nand Base R/W                                 //
///////////////////////////////////////////////////////////////////////////////////
#ifdef	WIN32

void NAND_SET_IO8(int val)
{
	val = val;
}

int NAND_GET_IO8(void)
{
	return 0;
}

void NAND_SET_IO16(int val)
{
	val = val;
}

int NAND_GET_IO16(void)
{
	return 0;
}

#endif


///////////////////////////////////////////////////////////////////////////////////
//                                Host Gpio Control                              //
///////////////////////////////////////////////////////////////////////////////////

void NandSetMode(int mode)
{
	int val = 0;
	const int status[4] = {	(NAND_CLE & (~(NAND_CE | NAND_ALE))),		// CLE high, CE low,  ALE low
							(NAND_ALE & (~(NAND_CE | NAND_CLE))),		// ALE high, CE low,  CLE low
							(~(NAND_CE | NAND_ALE | NAND_CLE)),			// CLE low,  CE low,  ALE low
							(NAND_CE & (~(NAND_ALE | NAND_CLE)))		// CE  high, ALE low, CLE low
	};

	mode = (mode > INVALID_NAND) ? INVALID_NAND : mode;
	val = ReadMemory(REG_PDATC);
	val = (val  & 0xfff1) | status[mode];
	WriteMemory(REG_PDATC, val);
}

void NandWaitReady(void)
{
	int val = 0;
	unsigned cnt0 = 0, cnt1 = 0;

	cnt0 = WdtGetCount();
	while((!(val & NAND_RB)) && (cnt1 < 40))	// 4ms (max time of erase block is 3ms)
	{
		val = ReadMemory(REG_PDATC);
		cnt1 = WdtGetCount();
		cnt1 -= cnt0;
	}
}
