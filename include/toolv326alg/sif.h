#ifndef _CMODEL_326SIF_ALG_H_
#define _CMODEL_326SIF_ALG_H_


#ifndef TOOLV326ALG_DLL
	#define TOOLV326ALG_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"
#include "toolbuf/bufferbase.h"


//=============== class C326SifModule =================

class TOOLV326ALG_DLL C326SifModule : public CBaseObject
{
public :
	C326SifModule();
};

//=============== class C326Rgb2Yiq =================

class TOOLV326ALG_DLL C326Rgb2Yiq : public CBaseObject
{
public :
	C326Rgb2Yiq(int width = 640, int height = 480, int unit = 2);
	virtual ~C326Rgb2Yiq();

public :
	int				Process(char *src, char *dst);
	void			EnableGamma(int flg = TRUE);
	void			SetYStart(int *pval);
	void			SetYSlope(int *pval);

protected :
	void	Ycalc(void);
	void	Icalc(void);
	void	Qcalc(void);
	void	Ygamma(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

protected :
	CLineBuf		m_rgbbuf;

private :
	int				m_width, m_height, m_gamma_en;
	int				m_ygstart[9], m_ygslope[9];
	int				m_hpos, m_vpos, m_unit;
	int				m_rgb[3], m_yiq[3];
};

//=============== class C326HsvAdj =================

class TOOLV326ALG_DLL C326HsvAdj : public CBaseObject
{
public :
	C326HsvAdj(int width = 640, int height = 480);
	virtual ~C326HsvAdj();

public :
	int				Process(char *src, char *dst);

protected :
	void	Rgb2Hsv(void);
	void	HsvAdj(void);
	void	Hsv2Rgb(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

protected :
	CLineBuf		m_rgbbuf;

private :
	int				m_width, m_height;
	int				m_hpos, m_vpos;
	int				m_rgb[3], m_mval, m_vval, m_vsval, m_hint, m_hfract;
};

//=============== class C326Historgram =================

#define		HIST_FRAME_NUM		0x03
#define		HIST_Q0_VAL			0x00
#define		HIST_FRAME_CNT		0x03

class TOOLV326ALG_DLL C326Historgram : public CBaseObject
{
public :
	C326Historgram(int width = 640, int height = 480, int unit = 1);
	virtual ~C326Historgram();

public :
	int				Process(char *src, char *dst);
	void			SetAlphaValue(int alpha = 0x01);
	void			SetTValue(int tval = 192);
	void			SetSValue(int sval = 16);

protected :
	void	Init(void);
	void	CalcQL(void);
	void	GetAera(void);
	void	GetStat(void);
	void	GetXVal(void);
	void	CalcCoe(void);
	void	CalcNewYVal(char *buf);
	void	LoopBuf(void);
	void	Convert(char *buf);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

protected :
	CBufferBase		m_databuf[HIST_FRAME_NUM];

private :
	CBufferBase		*m_pbuf[HIST_FRAME_NUM];
	int				m_width, m_height, m_unit, m_alpha, m_tval, m_sval;
	int				m_step, m_qval, m_coe, m_bufsize, m_frame;
	int				m_area[256][2];
	int				m_qcnt[256];
	int				m_wt[HIST_FRAME_NUM];
	int				m_xval[HIST_FRAME_NUM];
};

//=============== class C326Motion =================

#define		MD_FRAME_NUM	0x02
#define		MD_H_BLOCK		0x04
#define		MD_V_BLOCK		0x04

class TOOLV326ALG_DLL C326Motion : public CBaseObject
{
public :
	C326Motion(int width = 640, int height = 480, int unit = 1);
	virtual ~C326Motion();

public :
	int				Process(char *src, char *dst);
	void			SetHThrld(int val = 0x68);
	void			SetMThrld(int val = 0x10);
	void			SetFrameCnt(int cnt = 4);

protected :
	void	Init(void);
	void	LoopBuf(void);
	void	Calc(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

protected :
	CBufferBase		m_databuf[2];

private :
	CBufferBase		*m_pbuf[2];
	int				m_hval[MD_V_BLOCK][MD_H_BLOCK];
	int				m_width, m_height, m_unit, m_hthrld, m_mthrld, m_motion, m_framecnt;
	int				m_bufsize, m_curframe, m_motion0, m_motion1;
	int				m_mcs, m_mce, m_mrs, m_mre, m_v[3], m_h[3];
};

//=============== class C326AE =================

class TOOLV326ALG_DLL C326AE : public CBaseObject
{
public :
	C326AE(int width = 640, int height = 480, int unit = 1);
	virtual ~C326AE();

public :
	int				Process(char *src, char *dst);
	void			SetWin(int *pwin);
	void			SetWinWt(int *wt);

protected :
	void	Init(void);
	void	Calc(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

public :
	int				m_ymean;
	int				m_ywmean[16];

protected :
	CBufferBase		m_databuf;

private :
	int				m_width, m_height, m_unit;
	int				m_win[2][5], m_bufsize;
	int				m_win_wt[17];
};

//=============== class C326AWB =================

class TOOLV326ALG_DLL C326AWB : public CBaseObject
{
public :
	C326AWB(int width = 640, int height = 480, int unit = 2);
	virtual ~C326AWB();

public :
	int				Process(char *src, char *dst);
	void			SetWin(int *pwin);
	void			SetYThrld(int bot = 0, int top = 0xd0);
	void			SetIThrld(int top = 0x18);
	void			SetQThrld(int top = 0x10);
	void			SetGainThrld(int bot = 0x14, int top = 0x3f);

protected :
	void			Init(void);
	void			GetRgbMean(void);
	void			CalcGain(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

public :
	int				m_rgain, m_bgain;

protected :
	CBufferBase		m_rgbbuf, m_yiqbuf;

private :
	int				m_width, m_height, m_unit;
	int				m_ybot, m_ytop, m_itop, m_qtop, m_gaintop, m_gainbot, m_awbstep, m_awbthd1, m_awbthd2;
	int				m_win[2][5], m_rgb_mean[3], m_rgbbufsize, m_yiqbufsize, m_cnt;
};

//=============== class C326AFlicker =================

class TOOLV326ALG_DLL C326AFlicker : public CBaseObject
{
public :
	C326AFlicker(int width = 640, int height = 480, int unit = 1);
	virtual ~C326AFlicker();

public :
	int				Process(char *src, char *dst);
	void			SetLine(int start = 0x40, int num = 0x3f);

protected :
	void	Init(void);
	void	Calc(void);
	void	LoopBuf(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

public :
	int				m_sum;

protected :
	CBufferBase		m_databuf[2];

private :
	CBufferBase		*m_pbuf[2];
	int				m_width, m_height, m_unit, m_bufsize;
	int				m_start, m_num, m_frame;
};


#endif