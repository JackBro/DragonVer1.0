
#ifndef _CMODEL_326DPDDPC_ALG_H_
#define _CMODEL_326DPDDPC_ALG_H_

#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/linebayerbuf.h"


class CMODELISPALG_DLL C326DpdDpcAlg : public CImageAlgBase
{
public:
	C326DpdDpcAlg();
	~C326DpdDpcAlg(void);

public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

	virtual int Read(int index, char *buf, int len);
	virtual int Write(int index, char *buf, int len);

public:
	void SetSize(int width, int height, int unit = 1);
	void SetBGLine(int bgline);
	int  GetBGLine(void);
	void SetGpixel(int gpixel);
	int  GetGpixel(void);
	void SetDpdThd(int thd);
	int  GetDpdThd(void);
	void SetNoiseTable(int *val);
	int* GetNoiseTable(void);
protected:
	int  DpdProcess(char *buf, int len);
	int  GetDpdNoise(int *p, int gpixel);
	int  GetNoiseValue(int p, int gpixel);
	void LastLinePro(void);
	void LoopLinebuf(void);
private:
	void Init(void);
private:
	int m_fmt;
	int m_width;
	int m_height;
	int m_unit;
	int m_pos;
	int m_bgline;
	int m_gpixel;
	int m_dpdthd;
	int m_foreRead3line;     //sign
	int m_noisetable[gTotalNoiseTable];

	CLineBayerBuf m_Linebuf[5];
	CLineBayerBuf *m_pLinebuf[5];

};



#endif