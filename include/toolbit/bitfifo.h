#ifndef _BITFIFO_H_
#define _BITFIFO_H_


#ifndef TOOLBIT_DLL
	#define TOOLBIT_DLL  __declspec(dllimport)
#endif


#include "bitstream.h"

class TOOLBIT_DLL CBitFifo : public CBitStream
{
public:
	CBitFifo(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);

public:
	int		Write(char *buf, int bitlen, int offset = 0);	//return bitlen
	int		Read(char *buf, int bitlen, int offset = 0);	//return bitlen

public:
	int		Write(int val, int bitlen);
	int		Read(int val, int bitlen, int vsize = 0);
	int		Read(int bitlen);
	int		Query(int bitlen);

public:
	void	ClearPoint(void);
	void	SetSize(int size = BUFFER_DEFAULT_SIZE);
	void	SetSize(char *buf, int size = BUFFER_DEFAULT_SIZE);
	int		IsFifoEmpty(void);

public:
	int		GetDataSize(void);	//return bit size (unit is bit)
	int		GetFreeSize(void);	//return bit size
	void	wAlignBits(int align = 8);	//bits alignment
	void	rAlignBits(int align = 8);	//bits alignment

protected:
	void	SetReadPoint(int x);
	void	SetWritePoint(int x);
	void	ValidPoint(void);
	int		GetReadPoint(void);
	int		GetWritePoint(void);

protected:
	int		m_valid;
	int		m_rpoint;
	int		m_wpoint;
};

#endif


