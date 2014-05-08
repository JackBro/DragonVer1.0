#ifndef _CMODEL_STREAM_H_
#define _CMODEL_STREAM_H_

#include "../../../tools/global.h"
#include "../../../toolbuf/srambuf.h"

class CStream : public CSramBuf
{
public:
	CStream(int count = 0);

public:
	virtual void	SetSize(int size = 0);
	virtual int		Write(TStream *stream, int index);
	virtual int		Read(TStream *stream, int index);

protected:
	virtual void	Clear(void);
};

#endif

