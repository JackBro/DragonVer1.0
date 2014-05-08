
#ifndef _IPP_H_
#define _IPP_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define	REG_IPP_SPECTRL			    0x00
#define	REG_IPP_SIZCTRL			    0x01
#define	REG_IPP_IMGWD			    0x02
#define	REG_IPP_IMGHT   		    0x04
#define REG_IPP_SIZWD               0x06
#define REG_IPP_SIZHT               0x08
#define REG_IPP_SIZSTX              0x0A
#define REG_IPP_SIZSTY              0x0C
#define REG_IPP_FIRCF               0x0E
#define REG_IPP_PIXRATE             0x13
#define REG_IPP_CPUPFT              0x16
#define REG_IPP_CPDWFT              0x17
#define REG_IPP_DPUPFT              0x18
#define REG_IPP_DPDWFT              0x19
#define REG_IPP_CAPWD               0x1A
#define REG_IPP_CAPHT               0x1C
#define REG_IPP_DISWD               0x1E
#define REG_IPP_DISHT               0x20

/*Spe effect */
#define REG_IPP_YOFFSET				0x22
#define REG_IPP_UOFFSET				0x24
#define REG_IPP_VOFFSET				0x25
#define REG_IPP_UUPTHRS				0x26
#define REG_IPP_UDWTHRS				0x27
#define REG_IPP_VUPTHRS				0x28
#define REG_IPP_VDWTHRS				0x29
#define REG_IPP_FRMDP				0x30


//================ class CIppZoom =========

class OLDCMODEL_DLL CIppZoom : public CYuv422Base
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	int		FillImageInfo(ImageStream *pIn, ImageStream *pOut);
	void	Zoom(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIppSpecial =========

class OLDCMODEL_DLL CIppSpecial : public CYuv422Base
{
public:
	int		Process(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut);

protected:
	void	Special(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut);

	void	Sephia(ImageStream *pIn, ImageStream *pOut);
	void	ColorRange(ImageStream *pIn, ImageStream *pOut);
	void	Negative(ImageStream *pIn, ImageStream *pOut);

};


//================ class CIppPrefilter =========

class OLDCMODEL_DLL CIppPrefilter : public CYuv422Base
{
public:
	CIppPrefilter(void);
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	Prefilter(ImageStream *pIn, ImageStream *pOut);
	void	HorPrefilter(ImageStream *pIn, int num);
	void	VerPrefilter(ImageStream *pOut);
	int		HorFirstUnit(ImageStream *pIn, pYuv444Type dat);
	int		HorNextUnit(UINT8 y, UINT8 u, UINT8 v, int pos, pYuv444Type dat);
	Yuv444Type GetFilterResult(pYuv444Type dat, int *filter, int pos);
	void	FillFilter(void);

private:
	ImageStream		gISbuf[7];
	ImageStream		*gpISbuf[7];
	int				gFilter[7];
};


//================ class CIppDisplaySizer =========

class OLDCMODEL_DLL CIppSizerBase : public CYuv422Base
{
public:
	CIppSizerBase(void);

protected:
	void	Sizer(ImageStream *pIn, ImageStream *pOut);
	void	CheckFactor(int up, int down);
	void	SizerOut(ImageStream *pOut);
//	UINT8	GetYuv(UINT16 ul, UINT16 ur, UINT16 dl, UINT16 dr);
	UINT8	GetYuv(UINT8 ul, UINT8 ur, UINT8 dl, UINT8 dr);


private:
	ImageStream		gISbuf[2];
	ImageStream		*gpISbuf[2];

	int	gUp, gDown;
	int	gVer, gHor;
};


//================ class CIppDisplaySizer =========

class OLDCMODEL_DLL CIppDisplaySizer : public CIppSizerBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIppCaptureSizer =========

class OLDCMODEL_DLL CIppCaptureSizer : public CIppSizerBase
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);
};


//================ class CIpp =========

class OLDCMODEL_DLL CIpp : public CYuv422Base
{
public:
	CIpp(void);
	virtual ~CIpp(void);

	void	SetReg(int adr, int val);
	void	RegInit(void);
	int		Process(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOutLcd, ImageStream *pOutLbuf);

protected:

private:
	CIppZoom	gZoom;
	CIppSpecial gSpecial;
	CIppPrefilter gPrefilter;
	CIppDisplaySizer gDisplay;
	CIppCaptureSizer gCapture;

private:
	ImageStream	gISbuf[3];
};


#endif


