#ifndef _BLOCKBUFFER_H_
#define _BLOCKBUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

#include "srambuf.h"

class TOOLBUF_DLL CBlockBuf : public CSramBuf
{
public:
	CBlockBuf(int block = 0x100, int blocksize = 0x200);

public:
//	virtual int		Write(char *buf, int size);
//	virtual int		Read(char *buf, int size);

public:
	int		Write(char *buf, int size, int start, int end = 0);
	int		Read(char *buf, int size, int start, int end = 0);

public:
	void	SetSize(int block, int blocksize);
	void	SetSize(char *buf, int block, int blocksize);

private:
	int		m_blocksize;
};

#endif


