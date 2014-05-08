#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

#include "bufferbase.h"

typedef struct tag_TChunk
{
	int size;
	int nextpos;
} TChunk, *PTChunk;

class TOOLBUF_DLL CMemPool : public CBufferBase
{
public:
	CMemPool(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CMemPool(void);

public:
	void	SetSize(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int size, int unit = 1);

public:
	void	*Malloc(int size);
	void	Free(void *ptr);

private:
	void	Init(void);
};

#endif


