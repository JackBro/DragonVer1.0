

#ifndef _CMODEL_568COLORCORRECT_ALG_H_
#define _CMODEL_568COLORCORRECT_ALG_H_


#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/linebayerbuf.h"



class CMODELISPALG_DLL C568ColorCorrectAlg : public CImageAlgBase
{
public:
	C568ColorCorrectAlg();
	~C568ColorCorrectAlg();
public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);
public:
	virtual int Read(int index, char *buf, int len);
	virtual int Write(int index, char *buf, int len);
protected:
	int Mul10(int p, int c);
	int ColMul(int P, unsigned char A);
	int ColSum(int AxR, int AxG, int AxB, int ColOfst);

	unsigned char GetSingleBit(unsigned char X, int whichBit);
public:
	void SetColorMatrix(int *p);
	int* GetColorMatrix();
	void SetColorOffset(int *p);
	int* GetColorOffset();

private:
	int m_colormatrix[3][3];
	int m_offset[3];
	int m_width;
	int m_height;
	int m_unit;
	int m_fmt;
	int m_r;
	int m_g;
	int m_b;
	CLineBayerBuf m_Linebuf;
};

#endif
