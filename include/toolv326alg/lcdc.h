
#ifndef _CMODEL_326LCDC_ALG_H_
#define _CMODEL_326LCDC_ALG_H_


#ifndef TOOLV326ALG_DLL
	#define TOOLV326ALG_DLL  __declspec(dllimport)
#endif


#include "tools/baseitem.h"

//=============== class CFrameBuf	=================

enum LCDCFRAMEBUFenum
{
	FRAME_ONE_BUF,
	FRAME_TWO_BUF,
	FRAME_LINE_BUF
};

enum LCDCROTATIONenum
{
	ROTATION_0,
	ROTATION_90,
	ROTATION_180,
	ROTATION_270,
	ROTATION_MIRROR,
	ROTATION_MIRROR_90,
	ROTATION_MIRROR_180,
	ROTATION_MIRROR_270
};

class TOOLV326ALG_DLL CFrameBuf : public CBufferBase
{
public:
	CFrameBuf(int width = 120, int height = 160, int foramt = YUV422_YUYV);
	virtual ~CFrameBuf();

public:
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int line, int width);
	
	int		Process(char *src, char *dst);
	int		ReadPoint(char *buf, int rotx, int roty);
	
	void	SetSize(int width, int height, int format);
	void	SetFormat(int format);
	void	SetRotation(int rot);
	int		IsFull(void);
	int		GetRotWidth(void);
	int		GetRotHeight(void);
	int		GetWidth(void);
	int		GetHeight(void);

private:
	int		m_width, m_height;
	int		m_rotwidth, m_rotheight;
	int		m_line;
	int		m_rot;
	int		m_format;
	int		m_unit;
};

//=============== class CLcdcVBufControl =================

class TOOLV326ALG_DLL CLcdcVBufControl : public CBaseObject
{
public:
	CLcdcVBufControl(int width = 120, int height = 160, int memwidth = 128, int memheight = 160, int pointx = 0, int pointy = 0, int lcdcx = 0, int lcdcy = 0);
	virtual ~CLcdcVBufControl();

public:
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int line, int width);
	
	int		Process(char *src, char *dst);
	int		ReadPoint(char *buf, int x, int y);
	
	void	SetSize(int width, int height);
	void	SetMemSize(int width, int height);
	void	SetPoint(int x, int y);
	void	SetLcdcPoint(int x, int y);
	void	SetRotation(int rot);
	void	SetVBufMode(int mode);
	int		GetWidth(void);
	int		GetHeight(void);
	int		GetLcdcX(int x);
	int		GetLcdcY(int y);
	int		GetMemWidth(void);
	int		GetMemHeight(void);

private:
	CFrameBuf	m_framebuf;
	int		m_width, m_height;
	int		m_memwidth, m_memheight;
	int		m_pointx, m_pointy;
	int		m_lcdcpointx, m_lcdcpointy;
	int		m_line;
	int		m_rot;
	int		m_vbufmode;
};

//=============== class CLcdcGBuf =================

enum LCDCGBUFFORMATenum
{
	INDIRECT_RGB1,
	INDIRECT_RGB2,
	INDIRECT_RGB4,
	INDIRECT_RGB8,
	DIRECT_RGB444,
	DIRECT_RGB555_ALPHA,
	DIRECT_RGB565,
	DIRECT_RGB666,
	DIRECT_RGB888,
	DIRECT_RGB888_ALPHA
};

class TOOLV326ALG_DLL CLcdcGBuf : public CBaseObject
{
public:
	CLcdcGBuf(int width = 120, int height = 160, int memwidth = 128, int pointx = 0, int pointy = 0, int lcdcx = 0, int lcdcy = 0);
	virtual ~CLcdcGBuf();

public:
	//virtual int		Read(char *buf, int line, int width);
	
	int		Process(char *src, char *dst);
	int		ReadPoint(char *buf, int lcdx, int lcdy);

	int		SetGBuf(char *buf, int size);
	void	SetSize(int width, int height);
	void	SetMemWidth(int width);
	void	SetMemSize(int size);
	void	SetPoint(int x, int y);
	void	SetBAddr(int offset);
	void	SetLcdcPoint(int x, int y);
	void	SetRotation(int rot);
	void	SetFormat(int format);
	int		GetWidth(void);
	int		GetHeight(void);

	void	Update(void);
	void	SetBRefreshPoint(int x, int y);
	void	SetBRefreshSize(int width, int height);

protected:
	void	InitGBuf(void);

private:
	CBufferBase	m_gbuf;
	int		m_baddr;
	int		m_width, m_height;
	int		m_memwidth;
	int		m_pointx, m_pointy;
	int		m_lcdcpointx, m_lcdcpointy;
	int		m_refwidth, m_refheight;
	int		m_refpointx, m_refpointy;
	int		m_line;
	int		m_rot;
	int		m_format;
	int		m_memsize;
};

//=============== class CLcdcGBufControl =================

class TOOLV326ALG_DLL CLcdcGBufControl : public CBaseObject
{
public:
	CLcdcGBufControl();
	virtual ~CLcdcGBufControl();

public:
//	int		Process(char *src, char *dst);
	int		ReadPoint(char *buf, int lcdx, int lcdy, int *sel);

	void	Update(int sel);

public:
	CLcdcGBuf	m_gbuf[2];
};

//=============== class CLcdcModule =================

#define LCDC_COLORPALETTE_LEN	768

enum LCDCOVERLAYenum
{
	OVERLAY_TRANSPARENT,
	OVERLAY_AND,
	OVERLAY_OR,
	OVERLAY_INVERT
};

class TOOLV326ALG_DLL CLcdc : public CBaseObject
{
public:
	CLcdc();
	virtual ~CLcdc();

public:
	virtual	int		Write(char *buf, int len);
	virtual	int		Read(char *buf, int len);
	
public:
	int		Process(char *src, char *dst);
	int		WriteGBuf(char *buf, int len, int sel);

//	A layer proper
	void	SetASize(int width, int height);
	void	SetAMemSize(int width, int height);
	void	SetAPoint(int x, int y);
	void	SetALcdcPoint(int x, int y);
	void	SetARotation(int rot);
	int		GetAMemSize(void);
	int		GetALayerSize(void);

//	B layer proper
	void	SetGBuf(char *buf, int size, int sel);
	void	SetBSize(int width, int height, int sel);
	void	SetBMemWidth(int width, int sel);
	void	SetBMemSize(int size, int sel);
	void	SetBPoint(int x, int y, int sel);
	void	SetBAddr(int offset, int sel);
	void	SetBLcdcPoint(int x, int y, int sel);
	void	SetBRotation(int rot, int sel);
	void	SetBFormat(int format, int sel);

	void	UpdateB(int sel);
	void	SetBRefreshPoint(int x, int y, int sel);
	void	SetBRefreshSize(int width, int height, int sel);

//	Lcdc setting
	void	SetDitherEn(int en);
	void	SetDitherSel(int sel);
	void	SetGammaEn(int en);
	void	SetOverlayMode(int mode, int sel);
	void	SetOverlayEn(int en, int sel);
	void	SetAlphaBlendingEn(int en, int sel);
	void	SetPriority(int pri, int sel);
	void	SetVBufMode(int mode);
	void	SetOverlapMode(int mode);
	void	SetBackgroundPoint(int x, int y);
	void	SetBackgroundSize(int width, int height);
	void	SetKeyColor(int color, int sel);
	void	SetBlendRatio(int ratio, int sel);
	void	SetBackgroundData(int data, int sel);
	void	SetColorPalette(char *buf, int sel);
	void	SetRGamma(int *p);
	void	SetGGamma(int *p);
	void	SetBGamma(int *p);

protected:
	int		Yuv2Rgb(char *yuv, char *rgb);
	int		AlphaBlending(char *al, char *bl, int len, int sel);
	int		Overlay(char *al, char *bl, int len, int sel);
	int		DispLayMux(char *al, char *bl, int len, int sel);
	int		GetGammaValue(char *buf, int rgb);
	void	GammaCorrect(char *buf);
	void	Dither(char *buf ,int x);
	//int		ColorPalette(char *src, char *dst);
	

private:
	CLcdcVBufControl	m_vbuf;
	CLcdcGBufControl	m_gbufctrl;
	int		m_diteren;
	int		m_dithersel;
	int		m_gammaen;
	int		m_overlaymode0, m_overlaymode1;
	int		m_overlayen0, m_overlayen1;
	int		m_alphablenden0, m_alphablenden1;
	int		m_priority0, m_priority1;
	int		m_overlapmode;
	int		m_bgpointx, m_bgpointy;
	int		m_bgwidth, m_bgheight;
	int		m_keycolor0, m_keycolor1;
	int		m_blendratio0, m_blendratio1;
	int		m_bgdata0, m_bgdata1;
	int		m_colorpalette0[LCDC_COLORPALETTE_LEN], m_colorpalette1[LCDC_COLORPALETTE_LEN];
	int		m_rgamma[17], m_ggamma[17], m_bgamma[17];
	int		m_line;
};


#endif