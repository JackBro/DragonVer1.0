
#ifndef _TOOLS_MPEG4DEC_BASE_H_
#define _TOOLS_MPEG4DEC_BASE_H_



#ifndef TOOLMPEG4_DLL
	#define TOOLMPEG4_DLL  __declspec(dllimport)
#endif

#include "mpeg4base.h"
#include "toolbit/jmbitfifo.h"
#include "toolbuf/framebuf.h"


/*
typedef struct tag_TFrameBuf {
	CFrameBaseBuf	YBuf;
	CFrameBaseBuf	UBuf;
	CFrameBaseBuf	VBuf;

} TFrameBuf, *PTFrameBuf;
*/
class TOOLMPEG4_DLL CMpeg4DecBase : public CJmBitFifo, public CMpeg4Base
{
public:
	CMpeg4DecBase();
	virtual ~CMpeg4DecBase();

public:
	void		Mpeg4Parse(PTMpeg4Option pinfo);

public:
	void		ParseVisObjSeqHdr(PTMpeg4Option pinfo);
	void		ParseUserData(void);
	void		ParseVisObjHdr(PTMpeg4Option pinfo);
	void		ParseVidObjLayer(PTMpeg4Option pinfo);
	void		ParseGrpVopHdr(PTMpeg4Option pinfo);
	void		ParseVopHdr(PTMpeg4Option pinfo);
	void		ParseVidPackHdr(PTMpeg4Option pinfo);
	void		ParseMBHdr(PTMpeg4Option pinfo, int mbaddr);
	void		ParseInterlaceHdr(PTMpeg4Option pinfo);

	int			NextStartCode(void);
	void		GetQtable(PTMpeg4Option pinfo, int intra);
	int			GetResyncMarker(void);
	int			GetMCBPCcode(void);
	int			GetCBPYcode(void);
	void		GetMotionVector(int mbaddr, int blknum);
	TVector		decMV(void);
	int			GetMVdata(void);
	TVector		GetPMV(int mbaddr, int blknum);
	int			GetDcScaler(int lum);
	void		CoeffPredict(int predictor[8], int mbaddr, int blknum, int dc_scaler);
	void		CoeffAdd(int *block, int predictor[8], int mbaddr, int blknum, int dc_scaler);

	void		SetBufSize(int width, int height);
protected:
	int			log2bin(int val);
	void		SetmbsMV(int num);

public:
	TFrameBuf	m_buf[2];

};


#endif