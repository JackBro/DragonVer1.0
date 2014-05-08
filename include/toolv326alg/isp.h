#ifndef _CMODEL_326ISP_ALG_H_
#define _CMODEL_326ISP_ALG_H_



#ifndef TOOLV326ALG_DLL
	#define TOOLV326ALG_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"
#include "toolconvert/LineBayerBuf.h"

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
	EDGEENHANCE = BIT8,
	COLORCONVERT = BIT9,
	AUTOFOCUS	= BIT10
};

static inline int InverseBit(int x)
{
	return x ? 0 : 1;
}

#if 0
static inline unsigned char GetSingleBit(unsigned char X, int whichBit)
{
	return (( X & (1<<whichBit) ) != 0 ) ;
}

static inline unsigned short ColMul(unsigned char P, unsigned char A)
{
	unsigned short	AxP;	// [9:0], output
	int	Neg128;
	unsigned char AN, AbsValNeg, AF;	// [7:0]
	// all of the following are [9:0]
	unsigned short	A1p01, A1p02, A1p04, A1p08, A1p16, A1p32,
					s1p01N1p02, s1p04N1p08, s1p16N1p32,
					s1p64, S01x08, S01x64, S01x64P, S01x64N,
					S16x64;

	Neg128 = (A == 0x80);
	AN = ~(A - 1);

//	assign  AbsValNeg = Neg128 ? 8'hff : {A[7],AN[6:0]};	
	AbsValNeg = Neg128 ? 0xff : ((GetSingleBit(A,7)<<7)+(AN&0x7f));

//	assign  AF = A[7] ? AbsValNeg : A;	
	AF = GetSingleBit(A,7) ? AbsValNeg : A;

//	wire    [9:0]   A1p01 = AF[6] ? {1'b0,P,1'b0} : 10'h000;
//	wire    [9:0]   A1p02 = AF[5] ? {2'b0,P} : 10'h000;
//	wire    [9:0]   s1p01N1p02 = A1p01 + A1p02;
	A1p01 = GetSingleBit(AF,6) ? (P<<1) : 0;
	A1p02 = GetSingleBit(AF,5) ? P : 0;
	s1p01N1p02 = (A1p01 + A1p02) & 0x3ff;
	
//	wire    [9:0]   A1p04 = AF[4] ? {3'b0,P[7:1]} : 10'h000;
//	wire    [9:0]   A1p08 = AF[3] ? {4'b0,P[7:2]} : 10'h000;
//	wire    [9:0]   s1p04N1p08 = A1p04 + A1p08;
	A1p04 = GetSingleBit(AF,4) ? (P>>1) : 0 ;
	A1p08 = GetSingleBit(AF,3) ? (P>>2) : 0 ;
	s1p04N1p08 = (A1p04 + A1p08) & 0x3ff ;
	
//	wire    [9:0]   A1p16 = AF[2] ? {5'b0,P[7:3]} : 10'h000;
//	wire    [9:0]   A1p32 = AF[1] ? {6'b0,P[7:4]} : 10'h000;
//	wire    [9:0]   s1p16N1p32 = A1p16 + A1p32;
	A1p16 = GetSingleBit(AF,2) ? (P>>3) : 0 ;
	A1p32 = GetSingleBit(AF,1) ? (P>>4) : 0 ;
	s1p16N1p32 = (A1p16 + A1p32) & 0x3ff ;
	
//	wire    [9:0]   s1p64 = AF[0] ? P[7:5] : 10'h000;
	s1p64 = GetSingleBit(AF,0) ? (P>>5) : 0 ;
	
//	wire    [9:0]   S01x08 = s1p01N1p02 + s1p04N1p08; 
//	wire    [9:0]   S16x64 = s1p16N1p32 + s1p64;
	S01x08 = (s1p01N1p02 + s1p04N1p08) & 0x3ff; 
	S16x64 = (s1p16N1p32 + s1p64) & 0x3ff;
	
//	wire    [9:0]   S01x64 = S01x08 + S16x64;
	S01x64 = (S01x08 + S16x64) & 0x3ff;

//	wire    [9:0]   S01x64P = {1'b0,S01x64[9:1]};
//	wire    [9:0]   S01x64N =  ~S01x64P + 1;
	S01x64P = (S01x64>>1) & 0x3ff ;
	S01x64N =  (~S01x64P + 1) & 0x3ff;
	
//	wire    [9:0]   AxP = AF[7] ? S01x64N : S01x64P;
	AxP = ((GetSingleBit(AF,7) ? S01x64N : S01x64P) & 0x3ff);

	return AxP ;
}
#endif

//=============== class CIspModule326 =================

class TOOLV326ALG_DLL CIspModule326 : public CBaseObject
{
public:
	CIspModule326(int width, int height, int unit);
	int Process(char *src, char *dst);

protected:
	void Bit8ToBit10(char *src, char *dst);
	void RGB2BGR(char *buf);
	//virtual int Read(char *buf, int len);
	//virtual int Write(char *buf, int len);
public:
	void	SetSize(int width, int height, int unit);
	void SetDpdthd(int dpdthd);
	void SetDither(int dither);
	void SetRawData(int raw);
	void SetGain(int gain);
	void SetIspCtrl(int ctrl);
	int GetIspCtrl(void);
	void SetBGLine(int bgline);
	int GetBGLine(void);
	void SetGpixel(int gpixel);
	int GetGpixel(void);
	void SetIspModuleEn(int flag, int en);
	void SetDpdVersion(int ver);
	void SetOutputFile(int val);
	void SetNewEdge(int val);
	void	SetCoefEdge(int *coef);
	void	SetDivEdge(int div);
	void	SetMaxEdge(int val);
	unsigned int GetEdgeSum(void);
	void	SetAFH(int *afh);
	void	SetAFV(int *afv);
	void	SetAFW(int *afw);

private:
	int ispctrl;
	int	bgline;
	int	gpixel;
	int m_width;
	int m_height;
	int m_unit;
	int m_size;
	int m_rawdata;
	int m_gain;
	int m_dither;
	int m_dpdthd;
	int m_ver;
	int m_outputfile;
	int m_newedge;
	char m_coefedge[25];
	int m_divedge;
	int m_maxedge;
	unsigned int m_edgesum;
	int	m_afh[5];
	int m_afv[5];
	int m_afw[16];
};

//=============== class CIspTestBar326 =================

class TOOLV326ALG_DLL CIspTestBar326 : public CBaseObject
{
public:
	CIspTestBar326();

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

};

//=============== class CIspBlackLevel326 =================

class TOOLV326ALG_DLL CIspBlackLevel326 : public CBaseObject
{
public:
	CIspBlackLevel326(int width, int height, int unit);

public:
	int Process(char *src, char *dst);

public:
	virtual int Read(char *buf, int len);			//len = width
	virtual int Write(char *buf, int len);			//len = width

public:
	void SetSize(int width, int height, int unit = 1);
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

//=============== class CIspLensFallOff326 ================

class TOOLV326ALG_DLL CIspLensFallOff326 : public CBaseObject
{
public:
	CIspLensFallOff326(int width = 640, int height = 480, int unit = 1);

public:
	int Process(char *src, char *dst);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	void	SetSize(int width, int height, int unit = 1);
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

//============== class CIspDpdDpc326 ========================

#define LAST3X3LINES	BIT6
#define FIRST3X3LINES	BIT7

class TOOLV326ALG_DLL CIspDpdDpc326 : public CBaseObject
{
public:
	CIspDpdDpc326(int width = 640, int height = 480, int unit = 1, int ver = 1);
	int Process(char *src, char *dst);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
protected:
	int DpdProcess(char *buf, int len);
	int GetDpdNoise(int *p, int gpixel);
	int GetNoiseValue(int p, int gpixel);
	void LastLinePro(void);
	void LoopLinebuf(void);
	int DpdProcessNew(char *buf, int len);
	int GetDpdNoiseNew(int *p, int gpixel);
public:
	void SetSize(int width, int height, int unit = 1);
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
	int m_curline;
	int m_unit;
	int m_pos;
	int m_bgline;
	int m_gpixel;
	int m_dpdthd;
	int m_noisetable[17];
	CLineBayerBuf m_Linebuf[5];
	CLineBayerBuf *m_pLinebuf[5];
	int m_ver;
};

//============== class CIspDigitalGain326 ========================

class TOOLV326ALG_DLL CIspDigitalGain326 : public CBaseObject
{
public:
	CIspDigitalGain326(int width = 640, int height = 480, int unit = 1);
public:
	int Process(char *src, char *dst);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
public:
	void SetSize(int width, int height, int unit = 1);
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

//============== class CIspCfaExtract326 ========================

#define LAST5X5LINES	BIT6

class TOOLV326ALG_DLL CIspCfaExtract326 : public CBaseObject
{
public:
	CIspCfaExtract326(int width = 640, int height = 480, int mode = 1, int unit = 1);
	int Process(char *src, char *dst);

public:
	void	SetSize(int width, int height, int unit = 1);
	void	SetOrder(int order);
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
	void	SetBGLine(int bgline);
	int		GetBGLine(void);
	void	SetGpixel(int gpixel);
	int		GetGpixel(void);
	void	SetCoefEdge(char *coef);
	void	SetDivEdge(int div);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	
protected:
	void	Clear(void);
	void	GetFirstGblock(void);
	void	GetNextGblock(void);
	void	LastLinePro(void);
	void	LoopLinebuf(void);
	int		Interpolate(char *buf, int len);
	int		GetGpixel(int col, int row);
	int		GetRfromBG(int pos);
	int		GetRfromGR(int pos);
	int		GetRfromGB(int pos);
	int		GetEdge(int gpixel);
	int		GetEdge5x5(int gpixel);
	int		EdgeLine(int num);
	int		EdgeCol(int num);
private:
	int m_mode;		//1:edge or 0:interpolate or 2: new edge
	//interpolate
	CLineBayerBuf	*m_pLinebuf[9];
	CLineBayerBuf	m_Linebuf[9];
	int			m_Gblock[5][5];
	int			*m_pGblock[5];
	int			m_line, m_pos, m_curline;
	int			m_unit, m_width, m_height;
	int			m_Gfirst, m_GBline;
	//edge extract
	int m_femm, m_femp, m_femmin, m_femmax, m_femx1, m_femx2;
	char		m_coefedge[5][5];
	int		m_divedge;
};

//============== class CIspColorCorrect326 ========================

class TOOLV326ALG_DLL CIspColorCorrect326 : public CBaseObject
{
public:
	CIspColorCorrect326(int width = 640, int height = 480, int unit = 1);
	int Process(char *src, char *dst);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
protected:
	int Mul10(int p, int c);
	unsigned char GetSingleBit(unsigned char X, int whichBit);
	int ColMul(int P, unsigned char A);
	int ColSum(int AxR, int AxG, int AxB, int ColOfst);

public:
	void SetSize(int width, int height, int unit = 1);
	void SetColorMatrix(int *p);
	void SetColorOffset(int *p);
private:
	int m_colormatrix[3][3];
	int m_offset[3];
	int m_width, m_height, m_unit;
	int m_r, m_g, m_b;
};

//============== class CIspGammaCorrect326 ========================

class TOOLV326ALG_DLL CIspGammaCorrect326 : public CBaseObject
{
public:
	CIspGammaCorrect326(int width = 640, int height = 480, int unit = 1, int enable = 1);
	int Process(char *src, char *dst);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
protected:
	void GammaCorrect(char *buf);
	int	GetGammaValue(int val, int rgb);
	int Dither(int val);

public:
	void	SetSize(int width, int height, int unit = 1);
	void	SetEnable(int enable);
	void	SetDither(int dither);
	void	SetDitherValue(int val);
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
	int m_dither;
	int m_dither_val;
};

//============== class CIspColorConvert326 ========================

class TOOLV326ALG_DLL CIspColorConvert326 : public CBaseObject
{
public:
	CIspColorConvert326(int width = 640, int height = 480);
	int Process(char *src, char *dst);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	void SetSize(int width, int height);

private:
	int m_width;
	int m_height;
	int m_r[2], m_g[2], m_b[2];
};

//============== class CIspEdgeEnhance326 ========================

class TOOLV326ALG_DLL CIspEdgeEnhance326 : public CBaseObject
{
public:
	CIspEdgeEnhance326(int width = 640, int height = 480);
	int Process(char *src, char *edge, char *dst);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, char *edge, int len);
	void SetSize(int width, int height);

private:
	int m_width;
	int m_height;
	int m_y;
	int m_uv;
	int m_edge;
};


//============= class CIspAutoFocus =================================

#define AF_WINDOW_NUM	16
#define AF_H_AND_V_NUM	5

class TOOLV326ALG_DLL CIspAutoFocus : public CBaseObject
{
public:
	CIspAutoFocus(int width, int height);
	virtual ~CIspAutoFocus();
	int		Process(char *edge, unsigned int *afsum);
	void	SetSize(int width, int height);
	void	SetAFH(int *afh);
	void	SetAFV(int *afv);
	void	SetAFWeight(int *weight);

public:
	virtual int Read(int len);
	virtual int Write(char *buf, int len);
	void	SetInitVal(void);
	int		GetWinCol(int col);
	int		GetWinRow(void);
	unsigned int	GetWinSum(void);

private:
	int m_width, m_height;
	int m_afh[AF_H_AND_V_NUM], m_afv[AF_H_AND_V_NUM];
	int m_afweight[AF_WINDOW_NUM];
	int m_line, m_sum;
	int m_win_sum[AF_H_AND_V_NUM-1][AF_H_AND_V_NUM-1];
	CLineBuf	m_linebuf;
};

#endif


