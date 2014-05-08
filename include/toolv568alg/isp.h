#ifndef _CMODEL_ISP_ALG_H_
#define _CMODEL_ISP_ALG_H_

#ifndef _WINDLL
#define TOOLV568ALG_DLL
#endif

#ifndef TOOLV568ALG_DLL
	#define TOOLV568ALG_DLL  __declspec(dllimport)
#endif


//
//	use "COM" conception later to modify cmodel
//

enum ISPCONTROLenum
{
	TESTBAR = BIT0,
	BLACKLEVEL = BIT1,
	LENSFALLOFF = BIT2,
	DPDDPC = BIT3,
	DIGITALGAIN = BIT4,
	CFAEXTRACT = BIT5,
	COLORCORRECT = BIT6,
	GAMMACORRECT = BIT7,
	EDGEENHANCE = BIT8
};

static inline int InverseBit(int x)
{
	return x ? 0 : 1;
}

//=============== class CIspModule =================

class TOOLV568ALG_DLL CIspModule : public CBaseObject
{
public:
	CIspModule(int width, int height, int unit);
	int Process(char *src, char *dst);

protected:
	//virtual int Read(char *buf, int len);
	//virtual int Write(char *buf, int len);
public:
	void SetIspCtrl(int ctrl);
	int GetIspCtrl(void);
	void SetBGLine(int bgline);
	int GetBGLine(void);
	void SetGpixel(int gpixel);
	int GetGpixel(void);
	void SetIspModuleEn(int flag, int en);

private:
	int ispctrl;
	int	bgline;
	int	gpixel;
	int m_width;
	int m_height;
	int m_unit;
};

//=============== class CIspTestBar =================

class TOOLV568ALG_DLL CIspTestBar : public CBaseObject
{
public:
	CIspTestBar();

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

};

//=============== class CIspBlackLevel =================

class TOOLV568ALG_DLL CIspBlackLevel : public CBaseObject
{
public:
	CIspBlackLevel(int width, int height, int unit);

public:
	int Process(char *src, char *dst);

protected:
	virtual int Read(char *buf, int len);			//len = width
	virtual int Write(char *buf, int len);			//len = width

public:
	void SetROffset(int roffset);
	void SetG1Offset(int g1offset);
	void SetG2Offset(int g2offset);
	void SetBOffset(int boffset);
	int GetROffset(void);
	int GetG1Offset(void);
	int GetG2Offset(void);
	int GetBOffset(void);
	void SetBGLine(int bgline);
	int GetBGLine(void);
	void SetGpixel(int gpixel);
	int GetGpixel(void);

private:
	CLineBayerBuf	m_Linebuf;
	int m_width;
	int m_height;
	int m_unit;
	int m_rOffset;
	int m_g1Offset;
	int m_g2Offset;
	int m_bOffset;
	int m_bgline;
	int m_gpixel;
};

//=============== class CIspLensFallOff ================

class TOOLV568ALG_DLL CIspLensFallOff : public CBaseObject
{
public:
	CIspLensFallOff(int width, int height, int unit);

public:
	int Process(char *src, char *dst);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

	int GetLensX(void);
	int GetLensY(void);
	int GetLensFocus(void);
	void SetLensX(int x);
	void SetLensY(int y);
	void SetLensFocus(int f);

private:
	CLineBayerBuf	m_Linebuf;
	int m_lensCx;		//0-11bit
	int m_lensCy;		//0-11bit
	int m_lensFocus;	//0-12bit
	int m_unit;
	int m_line;
	int m_width;
	int m_height;
};

//============== class CIspDpdDpc ========================

#define LAST3X3LINES	BIT6

class TOOLV568ALG_DLL CIspDpdDpc : public CBaseObject
{
public:
	CIspDpdDpc(int width, int height, int unit);
	int Process(char *src, char *dst);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	int DpdProcess(char *buf, int len);
	int GetDpdNoise(int *p, int gpixel);
	int GetNoiseValue(int p, int gpixel);
	void LastLinePro(void);
	void LoopLinebuf(void);
public:
	void SetBGLine(int bgline);
	int GetBGLine(void);
	void SetGpixel(int gpixel);
	int GetGpixel(void);
	void SetDpdThd(int thd);
	int GetDpdThd(void);
	void SetNoiseTable(int *val);
	int* GetNoiseTable(void);
private:
	int m_width;
	int m_height;
	int m_line;
	int m_unit;
	int m_pos;
	int m_bgline;
	int m_gpixel;
	int m_dpdthd;
	int m_noisetable[17];
	CLineBayerBuf m_Linebuf[5];
	CLineBayerBuf *m_pLinebuf[5];
};

//============== class CIspDigitalGain ========================

class TOOLV568ALG_DLL CIspDigitalGain : public CBaseObject
{
public:
	CIspDigitalGain(int width, int height, int unit);
public:
	int Process(char *src, char *dst);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
public:
	void SetBGLine(int bgline);
	int GetBGLine(void);
	void SetGpixel(int gpixel);
	int GetGpixel(void);
	void SetGlobalGain(int gain);
	int GetGlobalGain(void);
	void SetRGain(int gain);
	int GetRGain(void);
	void SetGGain(int gain);
	int GetGGain(void);
	void SetBGain(int gain);
	int GetBGain(void);
	void SetGlobalEn(int val);
	void SetRgbEn(int val);
	void SetGainStep(int step);
	int GetGainStep(void);
private:
	CLineBayerBuf	m_Linebuf;
	int m_line;
	int m_unit;
	int m_width;
	int m_height;
	int m_bgline;
	int m_gpixel;
	int m_globalEn;
	int m_rgbEn;
	int m_globalGain;
	int m_gainStep;
	int m_rGain;
	int m_gGain;
	int m_bGain;
};

//============== class CIspCfaExtract ========================

#define LAST5X5LINES	BIT6

class TOOLV568ALG_DLL CIspCfaExtract : public CBaseObject
{
public:
	CIspCfaExtract(int width, int height, int mode, int unit = 1);
	int Process(char *src, char *dst);

public:
	virtual	void	SetSize(int width, int height, int unit = 1);
	virtual void	SetOrder(int order);
	void	SetMode(int mode);
	int		GetMode(void);
	void	SetFemm(int femm);
	int		GetFemm(void);
	void	SetFemp(int femp);
	int		GetFemp(void);
	void	SetFemmin(int femmin);
	int		GetFemmin(void);
	void	SetFemmax(int femmax);
	int		GetFemmax(void);
	void	SetFemx1(int femx1);
	int		GetFemx1(void);
	void	SetFemx2(int femx2);
	int		GetFemx2(void);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	
	virtual void	Clear(void);
	virtual void	GetFirstGblock(void);
	virtual void	GetNextGblock(void);
	virtual void	LastLinePro(void);
	virtual void	LoopLinebuf(void);
	virtual int		Interpolate(char *buf, int len);
	virtual int		GetGpixel(int col, int row);
	virtual int		GetRfromBG(int pos);
	virtual int		GetRfromGR(int pos);
	virtual int		GetRfromGB(int pos);
	virtual int		GetEdge(void);
private:
	int m_mode;		//1:edge or 0:interpolate
	//interpolate
	CLineBayerBuf	*m_pLinebuf[9];
	CLineBayerBuf	m_Linebuf[9];
	int			m_Gblock[5][5];
	int			*m_pGblock[5];
	int			m_line, m_pos;
	int			m_unit, m_width, m_height;
	int			m_Gfirst, m_GBline;
	//edge extract
	int m_femm, m_femp, m_femmin, m_femmax, m_femx1, m_femx2;
};

//============== class CIspColorCorrect ========================

class TOOLV568ALG_DLL CIspColorCorrect : public CBaseObject
{
public:
	CIspColorCorrect(int width, int height, int unit);
	int Process(char *src, char *dst);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

public:
	void SetColorMatrix(int *p);
	void SetColorOffset(int *p);
private:
	int m_colormatrix[3][3];
	int m_offset[3];
	int m_width, m_height, m_unit;
	int m_r, m_g, m_b;
};

//============== class CIspGammaCorrect ========================

class TOOLV568ALG_DLL CIspGammaCorrect : public CBaseObject
{
public:
	CIspGammaCorrect(int width, int height, int unit, int enable = 1);
	int Process(char *src, char *dst);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	void GammaCorrect(char *buf);
	int	GetGammaValue(int val, int rgb);

public:
	void	SetRGamma(int *p);
	int*	GetRGamma(void);
	void	SetGGamma(int *p);
	int*	GetGGamma(void);
	void	SetBGamma(int *p);
	int*	GetBGamma(void);

private:
	int m_rgamma[17];
	int m_ggamma[17];
	int m_bgamma[17];
	int m_unit;
	int m_width;
	int m_height;
	int m_r;
	int m_g;
	int m_b;
	int m_enable;		//1: enable gamma  0:disable gamma,just cut lower 2 bit
};

//============== class CIspColorConvert ========================

class TOOLV568ALG_DLL CIspColorConvert : public CBaseObject
{
public:
	CIspColorConvert(int width, int height);
	int Process(char *src, char *dst);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

private:
	int m_width;
	int m_height;
	int m_r[2], m_g[2], m_b[2];
};

//============== class CIspEdgeEnhance ========================

class TOOLV568ALG_DLL CIspEdgeEnhance : public CBaseObject
{
public:
	CIspEdgeEnhance(int width, int height);
	int Process(char *src, char *edge, char *dst);

protected:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, char *edge, int len);

private:
	int m_width;
	int m_height;
	int m_y;
	int m_uv;
	int m_edge;
};


#endif


