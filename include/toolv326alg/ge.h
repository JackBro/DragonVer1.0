#ifndef _CMODULE_GE_H_
#define _CMODULE_GE_H_


#ifndef TOOLV326ALG_DLL
	#define TOOLV326ALG_DLL  __declspec(dllimport)
#endif


#include "tools/baseitem.h"
#include "toolbuf/spanbuf.h"
#include "toolbuf/bufferbase.h"

//============================= GE ==========================================

enum COLORDEPTHenum
{
	GE_MONOCOLOR = 0x0,
	GE_RGB16,
	GE_RGB24
};

enum COLORTRANSSELenum
{
	GE_AFTER_NOTSAME = 0x0,
	GE_BEFORE_NOTSAME,
	GE_AFTER_SAME,
	GE_BEFORE_SAME
};

enum STARTPOINTSELenum
{
	GE_TOPLEFT = 0x0,
	GE_TOPRIGHT,
	GE_BOTTOMLEFT,
	GE_BOTTOMRIGHT
};

#define GE_PATTERN_BUF_SIZE	256
#define GE_GBUF_SIZE	0x40000
#define GE_GBUF_RGB24_SIZE	0x20000
#define GE_GBUF_RGB16_SIZE	0x15555

class TOOLV326ALG_DLL CGeDestSpanBuf : public CSpanBuf
{
public:
	CGeDestSpanBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CGeDestSpanBuf();

public:
	int		GetValue(int index);
	void	SetValue(int value, int index);
	void	SetDestSize(char *buf, int size = 0 , int format = GE_RGB24);
	void	SetDestPosition(int start, int dstWidth, int dstSpan);
};

class TOOLV326ALG_DLL CGeSrcSpanBuf : public CSpanBuf
{
public:
	CGeSrcSpanBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CGeSrcSpanBuf();

public:
	int		GetValue(int index);
	void	SetSrcSize(char *buf, int size = 0, int format = GE_RGB24);
	void	SetDestPosition(int start, int dstWidth, int dstSpan);
	void	SetFormat(int format);
private:
	int		m_format;
};

class TOOLV326ALG_DLL CGe : public CBaseObject
{
public:
	CGe(int width = 128, int height = 160);
	virtual ~CGe();

public:
	void	Process(char *src, char *dst);
	void	StartBitblt(void);
	void	StartLineDraw(void);
	void	PixelDraw(int x, int y);
	void	SetBitbltColorDepth(int depth);
	void	SetPatternColorDepth(int depth);
	void	SetPatternMask(int mask);
	void	SetPatternBg(int bg);
	void	SetPatternFg(int fg);
	void	SetColorTransSel(int sel);
	void	SetColorTrnasEn(int en);
	void	SetSrcColorDepth(int depth);
	void	SetSrcMask(int mask);
	void	SetSrcExtendSel(int sel);
	void	SetSrcBg(int bg);
	void	SetSrcFg(int fg);
	void	SetStartPointSel(int sel);
	void	SetBitWise(int val);
	void	SetDestSize(int width, int height);
	void	SetLinecolor(int color);
	void	SetClipLeftTop(int x, int y);
	void	SetClipRightBottom(int x, int y);
	void	SetLineDrawStart(int x, int y);
	void	SetLineDrawEnd(int x, int y);
	void	SetPattern(char *buf, int size = 64);	//For convenience, 24bits data occupy 32bits
	void	SetDestBuf(char *buf, int size);		//24bits occupy 24bits
	void	SetSrcBuf(char *buf, int size);
	void	GetDestBuf(char *buf, int size);
	char*	GetGBufAddr(void);
	void	SetDestAddr(char *addr);
	void	SetSrcAddr(char *addr);
	void	SetDestAddrOffset(unsigned int offset);
	void	SetSrcAddrOffset(unsigned int offset);
	void	SetSpan(int span);

protected:
	void	InitGBuf(void);
	int		GetSrcValue(int index);
	int		GetDstValue(int index);
	int		GetPatternValue(int index, int mask);
	int		BitBltAlg(int p, int s, int d);
	int		PixelMask(int index);
	int		PixelColorTrans(int	index, int value);
	void	SetDstValue(int value, int index);
	int		ColorExpansion(int monovalue, int flag);		//pattern:flag = 0 source:flag=1
	int		GetStartPointIndex(int i);
	void	LineDrawPlus(void);
	void	LineDrawNeg(void);

private:
	int		m_bitblt_colordepth;
	int		m_pattern_colordepth;
	int		m_pattern_mask;		//avail only when m_pattern_colordepth = MONOCOLOR
	int		m_pattern_bg, m_pattern_fg;  //24 bits
	int		m_color_trans_sel;	//2 bits
	int		m_color_trans_en;
	int		m_src_colordepth;
	int		m_src_mask;
	int		m_src_extend_sel;	//use m_pattern_bg or m_src_bg
	int		m_src_bg, m_src_fg;		//24 bits
	int		m_startpoint_sel;
	int		m_bitwise_sel;		//256 types
	int		m_width, m_height;
	int		m_linecolor;		//24 bits
	int		m_clip_lefttop_x;	//12 bits
	int		m_clip_lefttop_y;
	int		m_clip_rightbottom_x;
	int		m_clip_rightbottom_y;
	int		m_linedraw_start_x;		//12 bits
	int		m_linedraw_start_y;
	int		m_linedraw_end_x;
	int		m_linedraw_end_y;
	int		m_span;
	CBufferBase		m_pattern_buf;
	CBufferBase		m_gbuf;
	unsigned int	m_dst_addr_offset;
	unsigned int	m_src_addr_offset;
	CGeDestSpanBuf	m_dstbuf;
	CGeSrcSpanBuf	m_srcbuf;
};


#endif