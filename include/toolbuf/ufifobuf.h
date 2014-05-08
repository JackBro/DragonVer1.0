#ifndef _UNIT_FIFO_BUFFER_H_
#define _UNIT_FIFO_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif


#include "fifobuf.h"

class TOOLBUF_DLL CUfifoBuf : public CFifoBuf
{
public:
	CUfifoBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CUfifoBuf();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);

public:
	int		Query(int len);

public:
	int		Write(int val, int vlen);
	int		Read(int len);
};

#endif


