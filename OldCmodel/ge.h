
#ifndef _GE_H_
#define _GE_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_GE_STATUS					0x00
#define REG_GE_STARTUP					0x01

#define REG_GE_CTRL0					0x06
#define REG_GE_CTRL1					0x07
#define REG_GE_BITWISE					0x08
#define REG_GE_COLORDEPTH				0x09

#define REG_GE_DEST_SPAN_L				0x0A
#define REG_GE_DEST_SPAN_H				0x0B
#define REG_GE_SRC_SPAN_L				0x0C
#define REG_GE_SRC_SPAN_H				0x0D
#define REG_GE_SRC_START_L				0x0E
#define REG_GE_SRC_START_H				0x0F
#define REG_GE_DEST_START_L				0x10
#define REG_GE_DEST_START_H				0x11
#define REG_GE_PTN_START_L				0x12
#define REG_GE_PTN_START_H				0x13

#define REG_GE_PTN_BG_B					0x14
#define REG_GE_PTN_BG_G					0x15
#define REG_GE_PTN_BG_R					0x16
#define REG_GE_PTN_FG_B					0x18
#define REG_GE_PTN_FG_G					0x19
#define REG_GE_PTN_FG_R					0x1A

#define REG_GE_SRC_BG_B					0x1C
#define REG_GE_SRC_BG_G					0x1D
#define REG_GE_SRC_BG_R					0x1E
#define REG_GE_SRC_FG_B					0x20
#define REG_GE_SRC_FG_G					0x21
#define REG_GE_SRC_FG_R					0x22

#define REG_GE_LINE_SABS_L				0x24
#define REG_GE_LINE_SABS_H				0x25
#define REG_GE_LINE_SORD_L				0x26
#define REG_GE_LINE_SORD_H				0x27
#define REG_GE_LINE_EABS_L				0x28
#define REG_GE_LINE_EABS_H				0x29
#define REG_GE_LINE_EORD_L				0x2A
#define REG_GE_LINE_EORD_H				0x2B
#define REG_GE_LINE_B					0x2C
#define REG_GE_LINE_G					0x2D
#define REG_GE_LINE_R					0x2E

#define REG_GE_DEST_WIDTH_L				0x30
#define REG_GE_DEST_WIDTH_H				0x31
#define REG_GE_DEST_HEIGHT_L			0x32
#define REG_GE_DEST_HEIGHT_H			0x33

#define REG_GE_PTNBUF_L					0x34
#define REG_GE_PTNBUF_H					0x35
#define REG_GE_PTNBUF_WR				0x1033



class OLDCMODEL_DLL CGE : public CChipBase1
{
public:
	CGE(void);
	virtual ~CGE(void);

	void	SetReg(int adr, int val);
	void	RegInit(void);

public:

protected:

private:

};

#endif


