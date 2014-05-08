

#ifndef _CHIPBASE_H_
#define _CHIPBASE_H_
#include "ex_OldCmodel.h"


#include "global.h"

//
//	use "COM" conception later to modify cmodel
//


//=============== class CChipBase1 =================

class OLDCMODEL_DLL CChipBase1
{
public:
	CChipBase1(void);
	virtual ~CChipBase1(void);

public:
	void	SetReg(int adr, int val);
	int		GetReg(int adr);
	void	RegInit(void);
	int		Process(ImageStream *pIn, ImageStream *pOut);

protected:
	void	SetInterReg(int adr, int val);
	int		GetInterReg(int adr);
	void	ClearCounter(void);
	void	VsyncInitInfo(void);
	void	AdjustISPoint(ImageStream **pIs, int count);
	void	CopyImageStream(ImageStream *pIn, ImageStream *pOut);

	void	AddInline(ImageStream *pIn);
	int		varify(ImageStream *pIn);
	int		VsyncInit(ImageStream *pIn);
	int		AddOutLine(ImageStream *pOut);
	int		IsFrameOver(ImageStream *pOut);
	int		FillImageInfo(ImageStream *pIn, ImageStream *pOut);

	void	AddInline(ImageBlock *pIn);
	int		varify(ImageBlock *pIn);
	int		VsyncInit(ImageBlock *pIn);
	int		AddOutLine(ImageBlock *pOut);
	int		IsFrameOver(ImageBlock *pOut);
	int		FillImageInfo(ImageStream *pIn, ImageBlock *pOut);
	int		FillImageInfo(ImageBlock *pIn, ImageBlock *pOut);

protected:
	void	yuv422ToBlock(ImageStream *pIn, ImageBlock *pOut);

protected:
	UINT8	gReg[256];
	UINT8	gInterReg[256];
	UINT8	gModify;
	int		gInLine;
	int		gOutLine;
	UINT8	gAllow;
	CImageTools	gTools;
};


//=============== class CYuv422Base =================

class OLDCMODEL_DLL CYuv422Base : public CChipBase1
{
protected:
	int		varify(ImageStream *pIn);
};



#endif


