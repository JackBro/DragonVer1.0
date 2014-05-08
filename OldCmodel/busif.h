
#ifndef _BUSIF_H_
#define _BUSIF_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


//=======	host control opmode setting ============
enum { 
	MODE_VIEW					= 0x00,
	MODE_CAP_VIDEO				= 0x01, 
	MODE_DISPLAY				= 0x02,
	MODE_ENCODER				= 0x03,
	MODE_DECODER				= 0x04,
	MODE_DIRECTDISPLAY         = 0x05,
	MODE_THROUGH				= 0x06,
};

//=======	host control register address ============
#define REG_BIU_OP_MODE			0x84
#define REG_BIU_SEL_PORT	    0x88



class OLDCMODEL_DLL CBusIF : public CChipBase1
{
public:
	CBusIF(void);
	virtual ~CBusIF(void);

	void	RegInit(void);

	int NextModule(CChipBase1 * pSrc, CChipBase1 ** pDst);

	CChipBase1		*gpSif;
	CChipBase1		*gpIsp;
	CChipBase1		*gpIpp;
	CChipBase1		*gpJbuf;
	CChipBase1		*gpJpeg;
	CChipBase1		*gpLbuf;
	CChipBase1		*gpLcdIF;

protected:

private:
};

#endif

