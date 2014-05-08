

#ifndef _CMODEL_REGISTER_H_
#define _CMODEL_REGISTER_H_

#include "../../../tools/global.h"
#include "../../../toolbuf/srambuf.h"

#define REG_FUNCTION_COUNT	0x100
#define REG_GROUP_COUNT		0x200
#define REG_GROUP_MASK		0xFE00

typedef struct tag_TRegister
{
	int		val;
	int		value;
	int		mask;
} TRegister, *PTRegister;

enum REGISTERWRITEenum
{
	HOST_MODIFY_REG,
	CHIP_MODIFY_REG,
	MODIFY_REG
};

class CRegister : public CSramBuf
{
public:
	CRegister(int count = 0, int unit = sizeof(TRegister));

public:
	virtual void	SetSize(int size = 0);
	virtual void	Update(int adr);
	virtual int		Write(int adr, int val, int sel = HOST_MODIFY_REG);
	virtual int		Read(int adr);
	virtual int		SetMask(int adr, int mask);

protected:
	virtual	void	Clear(void);

};

#endif


