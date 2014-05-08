

#ifndef _CMODEL_326IPP_ALG_H_
#define _CMODEL_326IPP_ALG_H_


#ifndef TOOLV326ALG_DLL
	#define TOOLV326ALG_DLL  __declspec(dllimport)
#endif



#include "tools/baseitem.h"
#include "toolconvert/line422buf.h"
#include "toolconvert/LineBayerBuf.h"

//
//	use "COM" conception later to modify cmodel
//

enum IPPDOWNRATIOenum
{
	RATIO_HALF,
	RATIO_QUARTER,
	RATIO_1X,
	RATIO_2X,
	RATIO_4X
};

enum IPPPREFILTERenum
{
	PREF_NONE,
	PREF_Y,
	PREF_UV
};

//=============== class CIppModule =================

class TOOLV326ALG_DLL C326Ipp : public CBaseObject
{
public:
	C326Ipp(int width = 640, int height = 480, int ratio = RATIO_HALF, int format = YUV422_YUYV);
	virtual ~C326Ipp();

public:
	void	SetSize(int width, int height);
	void	SetRatio(int ratio);
	void	SetFormat(int format);
	void	SetFilter(int *pfilter);
	int		Process(char *src, char *dst);

private:
	int		m_width, m_height, m_ratio, m_format;
	int		m_filter[5];
};


//=============== class CFilterModule =================

class TOOLV326ALG_DLL C326Filter : public CBaseObject
{
public:
	C326Filter(int width = 640, int height = 480, int ratio = RATIO_HALF, int format = YUV422_YUYV);
	virtual ~C326Filter();

public:
	int				Process(char *src, char *dst);
	virtual void	SetFilter(int *filter);

protected:
	virtual void	Init(void);
	virtual void	Clear(void);
	virtual void	GetBlock(void);
	virtual void	LoopLinebuf(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);
	virtual int		YProc(char *buf, int len);
	virtual int		UvProc(char *buf, int len);

private:
	int				Filter(char *src, char *dst);

protected:
	CLine422Buf		m_linebuf[5];

private:
	int				m_yfilter[5];
	int				m_uvfilter[5];
	CLine422Buf		*m_plinebuf[5];
	int				m_block[5][5*3];
	int				*m_pblock[5];
	int				m_flg, m_line, m_pos;
	int				m_width, m_height, m_ratio, m_format, m_cnum;
};

//=============== class CSizerModule =================

class TOOLV326ALG_DLL C326Sizer : public CBaseObject
{
public:
	C326Sizer(int width = 640, int height = 480, int ratio = RATIO_HALF, int format = YUV422_YUYV);
	virtual ~C326Sizer();

public:
	int		Process(char *src, char *dst);

private:
	int		DownScale(char *src, char *dst);
	int		UpScale(char *src, char *dst);

private:
	int		m_width, m_height, m_ratio, m_format;
};

//=============== class C326SpecialEffect =================

enum V326IPPSPECIALenum
{
	IPPSEPHIA = 0x0,
	IPPSPECIALCOLOR = 0x1,
	IPPNEGATIVE = 0x2,
	IPPSKETCH = 0x3,
	IPPRELIEF = 0x4,
	IPPCANVAS = 0x5,
	IPPTV	= 0x6,
	IPPSHIFT = 0x7,
	IPPWINDBLOW = 0x8, 
	IPPBINARY = 0x9,
	IPPPAINT = 0xa,
	IPPPARTICLE = 0xb
};

#define NEGATIVE_RGB
#undef NEGATIVE_RGB

class TOOLV326ALG_DLL C326SpecialEffect : public CBaseObject
{
public:
	C326SpecialEffect(int width = 640, int height = 480, int format = YUV422_YUYV);
	virtual ~C326SpecialEffect();

public:
	int		Process(char *src, char *dst, char *edge = NULL);
	void	SetEffect(int spe);
	void	SetUOffset(int val);
	void	SetVOffset(int val);
	void	SetUUp(int val);
	void	SetUDown(int val);
	void	SetVUp(int val);
	void	SetVDown(int val);
	void	SetSize(int width, int height);

private:
	int		Sephia(char *src, char *dst);
	int		SpecialColor(char *src, char *dst);
	int		Negative(char *src, char *dst);
	int		Relief(char *src, char *dst);
	int		Sketch(char *src, char *dst, char *edge);
	char	SketchAddEdge(short edge, char y);

private:
	int		m_width, m_height, m_format, m_spe;
	int		m_uoffset, m_voffset, m_uup, m_udown, m_vup, m_vdown;
};

//=============== class C326Relief =================

#define LAST3X3LINES	BIT6
#define RELIEF_RGB
#undef RELIEF_RGB
#define RELIEF_HW
//#undef RELIEF_HW

class TOOLV326ALG_DLL C326Relief : public CBaseObject
{
public:
	C326Relief(int width = 640, int height = 480);
	virtual ~C326Relief();

public:
	int		Process(char *src, char *dst);
	void	SetSize(int width, int height);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	
	void	Clear(void);
	void	GetFirstBlock(void);
	void	GetNextBlock(void);
	void	LastLinePro(void);
	void	LoopLinebuf(void);
	int		Relief(char *buf, int len);
	
private:
	CLineBayerBuf	*m_pLinebuf[3];
	CLineBayerBuf	m_Linebuf[3];
	int			m_Block[3][3];
	int			*m_pBlock[3];
	int			m_line, m_pos, m_curline;
	int			m_width, m_height;
};


//=============== class C326Canvas =================

#define OUTPUT_AVERAGEPATTERN
#undef OUTPUT_AVERAGEPATTERN

class TOOLV326ALG_DLL C326Canvas : public CBaseObject
{
public:
	C326Canvas(int width = 640, int height = 480);
	virtual ~C326Canvas();

public:
	int		Process(char *src, char *src2, char *dst);
	void	SetSize(int width, int height);
	void	SetPatternSize(int width, int height);
	void	SetPatternFormat(int format = YUV422_YUYV);

protected:
	virtual int Read(char *buf, char *src2, int len);
	virtual int Write(char *buf, int len);
	void		PatternAvg(char *buf);
	
private:
	int			m_width, m_height;
	int			m_patternwidth, m_patternheight;
	int			m_patternformat;
	int			m_line;
	CLineBuf	m_linebuf;
};


//=============== class C326Tv =================

class TOOLV326ALG_DLL C326Tv : public CBaseObject
{
public:
	C326Tv(int width = 640, int height = 480);
	virtual ~C326Tv();

public:
	int		Process(char *src, char *dst);
	void	SetSize(int width, int height);
	void	SetDeg(int value);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	void		SetDelta(void);
	void		rgb2yuv(int *rgb, int *yuv);
	
private:
	int			m_width, m_height;
	int			m_line;
	int			m_delta_yuv[3][3];
	int			m_deg;
	CLineBuf	m_linebuf;
};


//=============== class C326Shift =================

class TOOLV326ALG_DLL C326Shift : public CBaseObject
{
public:
	C326Shift(int width = 640, int height = 480);
	virtual ~C326Shift();

public:
	int		Process(char *src, char *dst);
	void	SetSize(int width, int height);
	void	SetMaxMove(int value);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	int		Rand(void);
	
private:
	int			m_width, m_height;
	int			m_line;
	int			m_maxmove, m_currand;
	CLineBuf	m_linebuf;
};


//=============== class C326WindBlow =================

class TOOLV326ALG_DLL C326WindBlow : public CBaseObject
{
public:
	C326WindBlow(int width = 640, int height = 480);
	virtual ~C326WindBlow();

public:
	int		Process(char *src, char *dst);
	void	SetSize(int width, int height);
	void	SetThreshold(int value);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	void		GetEdge(void);
	double		Coeff(int n, int x);
	
private:
	int			m_width, m_height;
	int			m_line;
	int			m_thd;
	CLineBuf	m_linebuf;
	CLineBuf	m_edgebuf;
};


//=============== class C326Binary =================

class TOOLV326ALG_DLL C326Binary : public CBaseObject
{
public:
	C326Binary(int width = 640, int height = 480);
	virtual ~C326Binary();

public:
	int		Process(char *src, char *dst);
	void	SetSize(int width, int height);
	void	SetThreshold(int value);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	
private:
	int			m_width, m_height;
	int			m_line;
	int			m_thd, m_sum;
	CLineBuf	m_linebuf;
};

//=============== class C326Paint and Particle =================

class TOOLV326ALG_DLL C326PaintParticle : public CBaseObject
{
public:
	C326PaintParticle(int width = 640, int height = 480, int spe = IPPPAINT);
	virtual ~C326PaintParticle();

public:
	int		Process(char *src, char *dst);
	void	SetSize(int width, int height);
	void	SetSpecial(int spe);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	
	void	Clear(void);
	void	GetFirstBlock(void);
	void	GetNextBlock(void);
	void	LastLinePro(void);
	void	LoopLinebuf(void);
	int		PaintParticle(char *buf, int len);
	int		GetMinPoint(void);
	int		GetRandPoint(void);

private:
	CLineBayerBuf	*m_pLinebuf[3];
	CLineBayerBuf	m_Linebuf[3];
	int			m_Block[3][3];
	int			*m_pBlock[3];
	int			m_line, m_pos, m_curline;
	int			m_width, m_height;
	int			m_spe;
	int			m_rand;
};

//=============== class C578IppSizer =================

#define IPP_SIZER_LINEBUF_SUP	16

class TOOLV326ALG_DLL C578IppSizer : public CBaseObject
{
public:
	C578IppSizer(int srcWidth = 640, int srcHeight = 480, int dstWidth = 160, int dstHeight = 120);
	virtual ~C578IppSizer();

public:
	int		Process(char *src, char *dst);
	void	SetSrcSize(int width, int height);
	void	SetDstSize(int width, int height);
	void	SetSrcW(int w);
	void	SetDstW(int w);
	void	SetCeilW(int	s);
	void	SetFloorW(int s);
	void	SetSepW(int val);
	void	SetSrcH(int w);
	void	SetDstH(int w);
	void	SetCeilH(int	s);
	void	SetFloorH(int s);
	void	SetSepH(int val);

	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

private:
	int		m_srcwidth, m_srcheight, m_dstwidth, m_dstheight;
	int		m_srcw, m_srch;
	int		m_dstw, m_dsth;
	int		m_ceilw, m_ceilh;
	int		m_floorw, m_floorh;
	int		m_sepw, m_seph;
	int		m_read;
	int		m_sum;
	CLineBuf	m_linebuf;
	CLineBuf	m_linebuf_tmp;
};

//=============== class C578IppZoomIn =================


class TOOLV326ALG_DLL C578IppZoomIn : public CBaseObject
{
public:
	C578IppZoomIn(int srcWidth = 640, int srcHeight = 480, int dstWidth = 1280, int dstHeight = 960);
	virtual ~C578IppZoomIn();

public:
	int		Process(char *src, char *dst);
	void	SetSrcSize(int width, int height);
	void	SetDstSize(int width, int height);
	void	SetSrcW(int w);
	void	SetDstW(int w);
	void	SetParaW(int val);
	void	SetSrcH(int w);
	void	SetDstH(int w);
	void	SetParaH(int val);

	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

protected:
	int		ZoomIn(char *buf, int len);
	void	LoopLinebuf(void);

private:
	int		m_srcwidth, m_srcheight, m_dstwidth, m_dstheight;
	int		m_srcw, m_srch;
	int		m_dstw, m_dsth;
	int		m_paraw, m_parah;
	int		m_sum;
	CLineBuf	m_linebuf[2];
	CLineBuf	*m_plinebuf[2];
};



#endif
