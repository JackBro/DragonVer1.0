#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_

#ifndef TOOLBIT_DLL
	#define TOOLBIT_DLL  __declspec(dllimport)
#endif



#include "toolbuf/srambuf.h"
#include "bittools.h"

enum BITORDERenum
{
	SMALLBIT_ORDER,
	BIGBIT_ORDER,
};


class TOOLBIT_DLL CBitStream : public CSramBuf, public CBitTools
{
public:
	CBitStream(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);

public:
	void	SetSize(int size = BUFFER_DEFAULT_SIZE);
	void	SetSize(char *buf, int size = BUFFER_DEFAULT_SIZE);
	void	SetBitOrder(int order = SMALLBIT_ORDER);

public:
	int		Write(char *buf, int bitlen, int offset = 0, int wstart = 0, int wend = 0);
	int		Read(char *buf, int bitlen, int offset = 0, int rstart = 0, int rend = 0);

	int		Write(int val, int bitlen, int wstart = 0);
	int		Read(int val, int bitlen, int vsize, int rstart = 0);	//vsize: value bit size

private:
	void	adjustsizeinfo(void);

private:
	int		m_bitorder;
};

#endif


