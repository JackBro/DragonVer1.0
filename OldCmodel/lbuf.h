
#ifndef _LBUF_H_
#define _LBUF_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_LBUF_STATUS			0x00
#define REG_LBUF_YUVMODE		0x01
#define REG_LBUF_WORKMODE		0x02

#define REG_LBUF_DSTWIDTH_H		0x03
#define REG_LBUF_DSTWIDTH_L		0x04
#define REG_LBUF_DSTHEIGHT_H	0x05
#define REG_LBUF_DSTHEIGHT_L	0x06

#define REG_LBUF_SRCWIDTH_H		0x07
#define REG_LBUF_SRCWIDTH_L		0x08
#define REG_LBUF_SRCHEIGHT_H	0x09
#define REG_LBUF_SRCHEIGHT_L	0x0A


//================ class CLbufZoom =========

class OLDCMODEL_DLL CLbufZoom : public CYuv422Base
{
public:
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	int		FillImageInfo(ImageStream *pIn, ImageStream *pOut);
};


//================ class CLbufConvert =========

class OLDCMODEL_DLL CLbuf2Block : public CYuv422Base
{
public:
	int		Process(ImageStream *pIn, ImageBlock *pOut);

protected:
	int		FillImageInfo(ImageStream *pIn, ImageBlock *pOut);
	void	OutputBlock(ImageStream *pIn, ImageBlock *pOut);

private:
	ImageStream		gISbuf[8];
};


//================ class CLbufConvert =========

class OLDCMODEL_DLL CLbuf : public CYuv422Base
{
public:
	CLbuf(void);
	virtual ~CLbuf(void);
	void	RegInit(void);
	void	SetReg(int adr, int val);

	int		Process(ImageStream *pIn, ImageBlock *pOut);
	int		Process(ImageBlock *pIn, ImageStream *pOut);

private:
	CLbufZoom	gZoom;
	CLbuf2Block gBlock;

private:
	ImageStream		gISbuf;
};

#endif


