
#ifndef _JBUF_H_
#define _JBUF_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_JBUF_S_ADDR                     0x04
#define REG_JBUF_T_ADDR                     0x0C
#define REG_JBUF_D_ADDR                     0x08
#define REG_JBUF_JPEG_ADDR                  0x10
#define REG_JBUF_S_BUF_SIZE                 0x14
#define REG_JBUF_D_BUF_SIZE                 0x18
#define REG_JBUF_J_BUF_SIZE                 0x1C
#define REG_JBUF_T_BUF_SIZE                 0x28
#define REG_JBUF_BIU_ACC_LENGTH0            0x30
#define REG_JBUF_BIU_ACC_LENGTH1            0x31
#define REG_JBUF_CLR_REG                    0x80
#define REG_JBUF_GE_CLK_COUNT               0xC0
#define REG_JBUF_CAP_VIEW_CONF              0x88

#define REG_JBUF_J_WEN_PT0                  0xA0
#define REG_JBUF_J_WEN_PT1                  0xA1
#define REG_JBUF_J_RDN_PT0                  0xA4
#define REG_JBUF_J_RDN_PT1                  0xA5

#define REG_JBUF_S_WEN_PT0                  0xA8
#define REG_JBUF_S_WEN_PT1                  0xA9
#define REG_JBUF_S_RDN_PT0                  0xAC
#define REG_JBUF_S_RDN_PT1                  0xAD

#define REG_JBUF_D_WEN_PT0                  0xB0
#define REG_JBUF_D_WEN_PT1                  0xB1
#define REG_JBUF_D_RDN_PT0                  0xB4
#define REG_JBUF_D_RDN_PT1                  0xB5

#define REG_JBUF_T_WEN_PT0                  0xB8
#define REG_JBUF_T_WEN_PT1                  0xB9
#define REG_JBUF_T_RDN_PT0                  0xBC
#define REG_JBUF_T_RDN_PT1                  0xBD



class OLDCMODEL_DLL CJbuf : public CChipBase1
{
public:
	CJbuf(void);
	virtual ~CJbuf(void);

	void	SetReg(int adr, int val);
	void	RegInit(void);

private:

};

#endif

