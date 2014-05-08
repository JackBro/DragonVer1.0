
#ifndef _ISP_H_
#define _ISP_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_ISP_BASCTL			0x00
#define REG_ISP_WINWD			0x02
#define REG_ISP_WINHT			0x04
#define REG_ISP_IMGWD			0x06
#define REG_ISP_IMGHT			0x08
#define REG_ISP_FEM_M			0x0A
#define REG_ISP_FEM_P			0x0B
#define REG_ISP_FEM_X1			0x0C
#define REG_ISP_FEM_X2			0x0D
#define REG_ISP_FEM_MIN			0x0E
#define REG_ISP_FEM_MAX			0x0F
#define REG_ISP_LFCENX			0x10
#define REG_ISP_LFCENY			0x12
#define REG_ISP_LFFREC			0x14
#define REG_ISP_AFLNCTL			0x16
#define REG_ISP_AFTHD			0x18
#define REG_ISP_AFLNSTEP		0x1B
#define REG_ISP_RGAIN			0x22
#define REG_ISP_GGAIN			0x23
#define REG_ISP_BGAIN			0x24
#define REG_ISP_GLBGAIN			0x26
#define REG_ISP_CRADJ			0x28
#define REG_ISP_CBADJ			0x29
#define REG_ISP_CROFS			0x2A
#define REG_ISP_CBOFS			0x2B
#define REG_ISP_CMAT11			0x2C
#define REG_ISP_OFSR			0x35
#define REG_ISP_OFSG			0x36
#define REG_ISP_OFSB			0x37
#define REG_ISP_YGMST0			0x38
#define REG_ISP_YGASP0			0x41
#define REG_ISP_RGMST0			0x4A
#define REG_ISP_GGMST0			0x5B
#define REG_ISP_BGMST0			0x6C
#define REG_ISP_AFCTRL			0x80
#define REG_ISP_AFWHA			0x82
#define REG_ISP_AFWHB			0x83
#define REG_ISP_AFWHC			0x84
#define REG_ISP_AFWVA			0x85
#define REG_ISP_AFWVB			0x86
#define REG_ISP_AFWVC			0x87
#define REG_ISP_AEWF			0x88
#define REG_ISP_YBOT			0x8A
#define REG_ISP_YTOP			0x8B
#define REG_ISP_QTOP			0x8C
#define REG_ISP_ITOP			0x8D
#define REG_ISP_GBOT			0x8E
#define REG_ISP_GTOP			0x8F
#define REG_ISP_AWBCTRL			0x90
#define REG_ISP_RMEAN			0x92
#define REG_ISP_GMEAN			0x93
#define REG_ISP_BMEAN			0x94
#define REG_ISP_YMEAN			0x95
#define REG_ISP_AUTO_STATUS		0x96
#define REG_ISP_DPCTHD			0xA0
#define REG_ISP_NT0				0xA1

//======================================
#define REG_ISP_RLEVEL			0xC0
#define REG_ISP_BLEVEL			0xC2
#define REG_ISP_GLEVEL1			0xC4
#define REG_ISP_GLEVEL2			0xC6
#define REG_ISP_BLACKNCAP		0xC8

static inline int InverseBit(int x)
{
	return x ? 0 : 1;
}


//================ class CIspRawBase =========

class OLDCMODEL_DLL CIspRawBase : public CChipBase1
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	int		FillImageInfo(ImageStream *pIn, ImageStream *pOut);
	int		VsyncInit(ImageStream *pIn);
	int		varify(ImageStream *pIn);

protected:
	int	gBgline;
	int	gGpixel;
};


//================ class CIspBlackLevel =========

class OLDCMODEL_DLL CIspBlackLevel : public CIspRawBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	BlackLevel(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIspTestBar =========

class OLDCMODEL_DLL CIspTestBar : public CIspRawBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIspLensFallOff =========

class OLDCMODEL_DLL CIspLensFallOff : public CIspRawBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	LensFallOff(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIspDpdNoise =========

class OLDCMODEL_DLL CIspDpdNoise : public CIspRawBase
{
public:
	CIspDpdNoise(void);
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	LensFallOff(ImageStream *pIn, ImageStream *pOut);
	void	DpdNoiseInside(ImageStream *pIn, ImageStream *pOut);
	//void	AdjustISPoint(ImageStream **pIs, int count);
	void	DpdNoise(ImageStream *pIn, ImageStream *pOut);
	int		GetDpdNoise(int *p, int g_pixel);
	int		GetNoiseValue(int p, int g_pixel);

private:
	ImageStream		*gpISbuf[4];
	ImageStream		gISbuf[4];
};

//================ class CIspDigitalGain =========

class OLDCMODEL_DLL CIspDigitalGain : public CIspRawBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	GainProcess(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIspInterpolate =========

class OLDCMODEL_DLL CIspInterpolate : public CIspRawBase
{
public:
	CIspInterpolate(void);
	int		Process(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge);

protected:
	void	CopyToLineBuf(ImageStream *pIn, int line);
	void	Interpolate(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge);
	int		FillImageInfo(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge);
	void	OutputStream(ImageStream *pOut, ImageStream *pEdge);
	int		GetGPixel(int col, int row);
	void	GetFirstGBlock5X5(void);
	void	GetNextGBlock5X5(int pos);
	int	GetRfromBG(int pos);
	int	GetRfromGR(int pos);
	int	GetRfromGB(int pos);
	int	GetEdge(void);

	void	OutFirst4Lines(ImageStream *pOut, ImageStream *pEdge);
	void	OutLast4Lines(int height, ImageStream *pOut, ImageStream *pEdge);
	void	OutInsideLines(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge);

	void	GetFirstBlock9X9(SimPoint pt);
	void	GetLastBlock9X9(int height, SimPoint pt);
	void	GetBlock9X9(SimPoint pt, ImageStream *pIn);

	void	GetGBlock5X5(SimPoint pt);

protected:
	void	printGblock(int pos);

private:
	ImageStream		*gpISbuf[9];
	ImageStream		gISbuf[9];
	int			*gpBlock5X5[5];
	int			gGBlock5X5[5][5];
};



//================ class CIspRgbBase =========

class OLDCMODEL_DLL CIspRgbBase : public CChipBase1
{
protected:
	int		varify(ImageStream *pIn);
};


//================ class CIspColorCorrect =========

class OLDCMODEL_DLL CIspColorCorrect : public CIspRgbBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	ColorCorrect(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIspGammaCorrect =========

class OLDCMODEL_DLL CIspGammaCorrect : public CIspRgbBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	int	GetGammaValue(int val, int regadr);
	void	GammaCorrect(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIspColorConvert =========

class OLDCMODEL_DLL CIspColorConvert : public CIspRgbBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	ColorConvert(ImageStream *pIn, ImageStream *pOut);
	int		FillImageInfo(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIspColorConvert =========

class OLDCMODEL_DLL CIspEdgeEnhance : public CYuv422Base
{
public:
	int		Process(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut);

protected:
	void	EdgeEnhance(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut);
};


//================ class CIspZoom =========

class OLDCMODEL_DLL CIspZoom : public CIspRawBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIsp =========

class OLDCMODEL_DLL CIsp : public CChipBase1
{
public:
	CIsp(void);
	virtual ~CIsp(void);

	void	SetReg(int adr, int val);
	void	RegInit(void);
	int		Process(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge);

private:
	CIspRawBase		gSize;

	CIspBlackLevel	gBlack;
	CIspTestBar		gTestBar;
	CIspLensFallOff	gLens;
	CIspDpdNoise	gDpd;
	CIspDigitalGain gDigitalGain;
	CIspInterpolate	gInterpolate;
	CIspColorCorrect gColorCorrect;
	CIspGammaCorrect gGammaCorrect;
	CIspColorConvert gColorConvert;
	CIspEdgeEnhance	gEdgeEnhance;

public:

protected:

private:
	ImageStream	gISbuf[2];
};

#endif


