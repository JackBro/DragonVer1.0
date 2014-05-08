#ifndef _VARLEN_FIFO_BUFFER_H_
#define _VARLEN_FIFO_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

#include "ufifobuf.h"

class TOOLBUF_DLL CVLfifoBuf : public CUfifoBuf
{
public:
	CVLfifoBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CVLfifoBuf();

public:
	void	Write(int val);
	int		Read();
};

#endif


