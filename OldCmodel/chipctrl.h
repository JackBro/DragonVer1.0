
#ifndef _CHIPCTRL_H_
#define _CHIPCTRL_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_HCTRL_RSTCTRL1		0x00
#define REG_HCTRL_RSTCTRL2		0x02
#define REG_HCTRL_CLKOFF		0x04
#define REG_HCTRL_CHIPCTRL		0x06
#define REG_HCTRL_CLKCTRL0		0x08
#define REG_HCTRL_CLKCTRL1		0x0a
#define REG_HCTRL_CLKCTRL2		0x0c
#define REG_HCTRL_CLKCTRL3		0x0e
#define REG_HCTRL_CPU_PC		0x10
#define REG_HCTRL_LCD_PC		0x12
#define REG_HCTRL_CS_PC			0x14
#define REG_HCTRL_STRAP			0x16
#define REG_HCTRL_GPIO_CFG		0x18
#define REG_HCTRL_GPIO_MODE		0x1A
#define REG_HCTRL_GPIO_DIR		0x1C
#define REG_HCTRL_GPIO_P0		0x1E
#define REG_HCTRL_INTCTRL		0x20
#define REG_HCTRL_INTSERV		0x22
#define REG_HCTRL_INTEN			0x24
#define REG_HCTRL_INTFLAG		0x26
#define REG_HCTRL_UMCTRL		0x2C
#define REG_HCTRL_MISC_CON		0x2E
#define REG_HCTRL_INTEN0		0x30
#define REG_HCTRL_INTFLAG0		0x32
#define REG_HCTRL_INTEN1        0x34
#define REG_HCTRL_INTEN2		0x38
#define REG_HCTRL_INTEN3        0x3c
#define REG_HCTRL_INTEN4        0x40



class OLDCMODEL_DLL CChipCtrl : public CChipBase1
{
public:
	CChipCtrl(void);
	virtual ~CChipCtrl(void);

	void	SetReg(int adr, int val);
	void	RegInit(void);

public:
	CChipBase1		*gpSif;
	CChipBase1		*gpIsp;
	CChipBase1		*gpIpp;
	CChipBase1		*gpJbuf;
	CChipBase1		*gpJpeg;
	CChipBase1		*gpLbuf;
	CChipBase1		*gpLcdIF;
	CChipBase1		*gpGE;
	CChipBase1		*gpBusIF;

protected:

private:
//	UINT8	gReg[256];
//	UINT16	gInLine;
//	UINT16	gOutLine;

};

#endif


