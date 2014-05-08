
#ifndef _TOOLS_MPEG2IENC_H_
#define _TOOLS_MPEG2IENC_H_


#ifndef TOOLMPEG2I_DLL
	#define TOOLMPEG2I_DLL  __declspec(dllimport)
#endif

#include "mpeg2base.h"
#include "toolbit/bitfifo.h"


#define DEFAULT_MPEGDATASIZE	0x40000
#define DEFAULT_MPEG2INIT_QUANT	20

class TOOLMPEG2I_DLL CMpeg2IEncoder : public CBitFifo, public CMpeg2IBase
{
public:
	CMpeg2IEncoder(int size = DEFAULT_MPEGDATASIZE);

public:
	virtual int	 Write(char *buf, int len);
	virtual int	 Read(char *buf, int size);

public:
	void SetImageSize(int width, int height);
	void SetYuvFmt(int fmt = YUV420BLK);
	void Open(TMpeg2Option *option = NULL);

protected:
	void Init(void);

protected:
	void putSeqHdr(void);
	void putSeqExt(void);
	void putSeqDispExt(void);
	void putUserData(char *buf, int len);
	void putPictHdr(void);
	void putPictCodExt(void);
	int	 calcuMquant(void);
	void GeneratePicHdr(void);
	void GenerateHeader(void);
	void putMbtype(void);
	void putSliceHdr(void);

protected:
	void encSlice(char *buf);
	void encblock(char *block, int *lastDc);
	//void quant(int *inblk, int *oublk, int qf);

private:
	TMpeg2Table		m_table;
	int		m_valid;
	int		m_pos;
	int		m_vwc;
	int		m_frmcount;
	int		m_prevmquant;
	int		m_quant;
	int		m_slice;
	int		m_targetlen;
	int		m_actuallen;
};

#endif

