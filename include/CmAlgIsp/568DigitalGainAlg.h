
#ifndef _CMODEL_568DIGITALGAIN_ALG_H_
#define _CMODEL_568DIGITALGAIN_ALG_H_


#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/linebayerbuf.h"


class CMODELISPALG_DLL C568DigitalGainAlg : public CImageAlgBase
{
public:
	C568DigitalGainAlg();
	~C568DigitalGainAlg();

public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

	virtual int Read(int index, char *buf, int len);
	virtual int Write(int index, char *buf, int len);
public:
	void SetBGLine(int bgline);
	int  GetBGLine(void);
	void SetGpixel(int gpixel);
	int  GetGpixel(void);
	void SetGlobalGain(int gain);
	int  GetGlobalGain(void);
	void SetRGain(int gain);
	int  GetRGain(void);
	void SetGGain(int gain);
	int  GetGGain(void);
	void SetBGain(int gain);
	int  GetBGain(void);
	void SetGlobalEn(int val);
	int  GetGlobalEn();
	void SetRgbEn(int val);
	int  GetRgbEn();
	void SetGainStep(int step);
	int  GetGainStep(void);
private:
	void Init(void);
private:
	int m_unit;
	int m_width;
	int m_height;
	int m_fmt;
	int m_bgline;
	int m_gpixel;
	int m_globalEn;
	int m_rgbEn;
	int m_globalGain;
	int m_gainStep;
	int m_rGain;
	int m_gGain;
	int m_bGain;

	CLineBayerBuf	m_Linebuf;
};

#endif