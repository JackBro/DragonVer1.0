#ifndef _TOOLS_MPEG2DEC_BASE_H_
#define _TOOLS_MPEG2DEC_BASE_H_


#ifndef TOOLMPEG2I_DLL
	#define TOOLMPEG2I_DLL  __declspec(dllimport)
#endif

#include "toolbit/jmbitfifo.h"
#include "tooljpg/jpegtable.h"
#include "toolbuf/framebuf.h"
#include "mpeg2base.h"


/*
typedef struct tag_TFrameBuf {
	CFrameBaseBuf	YBuf;
	CFrameBaseBuf	UBuf;
	CFrameBaseBuf	VBuf;

} TFrameBuf, *PTFrameBuf;
*/
class TOOLMPEG2I_DLL CMpeg2DecBase : public CJmBitFifo, public CMpeg2IBase
{
public:
	CMpeg2DecBase();
	virtual ~CMpeg2DecBase();
public:
	void Mpeg2Parse(PTMpeg2Option pinfo);
	void ParseHdr(PTMpeg2Option pinfo);

protected:
	void ParseSeqHdr(PTMpeg2Option pinfo);
	void ParseExtHdr(PTMpeg2Option pinfo);
	void ParsePicHdr(PTMpeg2Option pinfo);
	void ParseGopHdr(PTMpeg2Option pinfo);
	void ParseUserData(PTMpeg2Option pinfo);
	void ParseMBhdr(PTMpeg2Option pinfo);
	void ParseMBmodehdr(PTMpeg2Option pinfo);
	void ParseMVhdr(PTMpeg2Option pinfo, int s);
	int  GetMBaddrinc(void);

	void ParseSeqExtHdr(PTMpeg2Option pinfo);
	void ParseSeqDisExtHdr(PTMpeg2Option pinfo);
	void ParseSeqSlbExtHdr(PTMpeg2Option pinfo);
	void ParsePicCodExtHdr(PTMpeg2Option pinfo);
	void ParseQmatExtHdr(PTMpeg2Option pinfo);
	void ParsePicDisExtHdr(PTMpeg2Option pinfo);
	void ParsePicTemExtHdr(PTMpeg2Option pinfo);
	void ParsePicSpaExtHdr(PTMpeg2Option pinfo);

	void decMV(PTMpeg2Option pinfo, int num, int s, int t);
	void MotionVector(PTMpeg2Option pinfo, int num, int s);
	void getQtable(int quant);
	int  NextStartCode(void);
	
	int	 GetHuffIndex(PTHuffValue table, int bitlen, int tsize);	
	char GetMotionCode(void);
	char GetcbpCode(void);

	void SetBufSize(int width, int height, int format);

public:
	TFrameBuf	m_buf[2];

protected:
	int		m_quant;
	int		m_cur, m_ref;
	int		m_IntraQt[64], m_InterQt[64];

};







#endif