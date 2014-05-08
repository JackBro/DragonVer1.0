#ifndef _COMPRESS_BITFIFO_H_
#define _COMPRESS_BITFIFO_H_


#ifndef TOOLBIT_DLL
	#define TOOLBIT_DLL  __declspec(dllimport)
#endif

#include "bitfifo.h"

class TOOLBIT_DLL CJmBitFifo : public CBitFifo
{
public:
	CJmBitFifo(int size = BUFFER_DEFAULT_SIZE, int order = BIGBIT_ORDER);

public:
	virtual int		Write(char *buf, int len);	//return bytelen
	virtual int		Read(char *buf, int len);	//return bytelen
};

#endif


