
#ifndef _TOOLS_JPEGDEC_BASE_H_
#define _TOOLS_JPEGDEC_BASE_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif

#include "jpegbase.h"
#include "jpegparse.h"
#include "toolbuf/fifobuf.h"
#include "toolbit/bitfifo.h"
#include "toolfile/dbugtools.h"



class TOOLJPG_DLL CJpegDecBase : public CFifoBuf, public CJpegBase, public CJpegParse
{
public:
	CJpegDecBase(int size = DEFAULT_JPEGDATASIZE);

public:
	void			Open(PTJpegIndex pinfo);
	virtual int		Write(char *buf, int leng);

protected:
	int				ff002ff(char *src, char *dst, int len);
	virtual void	blockdec(int *curdc, int mode) = 0;

protected:
	int		GetHuffIndex(PTHuffValue table, int bitlen, int tsize);
	int		GetHuffDcVal(PTHuffValue dc, int bitlen, int tsize);
	void	GetHuffAcVal(PTHuffValue ac, int *blk);

protected:
	TJpegIndex	m_JpgIndex;
	TJpegTable	m_Table;
	CBitFifo	m_EncBit;
	CDbugTools	m_dbug;
};

#endif

