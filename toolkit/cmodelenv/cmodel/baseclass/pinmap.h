

#ifndef _CMODEL_PINMAP_H_
#define _CMODEL_PINMAP_H_

#include "../../../tools/global.h"
#include "../../../toolbuf/srambuf.h"
#include "pinbase.h"

typedef struct tag_TPinMap
{
	CPinBase * pin;
//	int		type;
} TPinMap, *PTPinMap;

class CPinMap : public CSramBuf
{
public:
	CPinMap(int count = 0);

public:
	virtual void	SetSize(int size = 0);
	virtual int		Write(TPinMap *map, int index);
	virtual int		Read(TPinMap *map, int index);
	virtual void	DeleteModule(CPinBase *ppin);
	virtual void	DeleteModule(int index);
	virtual void	Reset(void);
	virtual void	Clear(void);

protected:

public:
};

#endif


