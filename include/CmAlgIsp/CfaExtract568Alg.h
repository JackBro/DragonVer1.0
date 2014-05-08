
#ifndef _CMODEL_CFAEXTRACT568_ALG_H_
#define _CMODEL_CFAEXTRACT568_ALG_H_


#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linebuf.h"
#include "toolconvert/linebayerbuf.h"



class CMODELISPALG_DLL CCfaExtract568Alg : public CImageAlgBase
{
public:
	CCfaExtract568Alg();
	~CCfaExtract568Alg();

public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	void	SetSize(int width, int height, int unit = 1);
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
	char*   GetCoefEdge();
	void	SetDivEdge(int div);
	int     GetDivEdge();

public:
	virtual int Read(int index, char *buf, int len);
	virtual int Write(int index, char *buf, int len);

protected:
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
	void Init(void);
private:
	int			m_foreRead5line;
	int         m_pos;
	int         m_mode;		//1:edge or 0:interpolate or 2: new edge
	//interpolate
	int			m_Gblock[5][5];
	int			*m_pGblock[5];
	int			m_unit;
	int         m_width;
	int         m_height;
	int         m_fmt;
	int         m_bgline;
	int         m_gpixel;

	CLineBayerBuf	*m_pLinebuf[9];
	CLineBayerBuf	m_Linebuf[9];

	CLineBuf	m_LinebufEdge;

	//edge extract
	int         m_femm;
	int         m_femp;
	int         m_femmin;
	int         m_femmax;
	int         m_femx1;
	int         m_femx2;
	int 		m_divedge;
	char		m_coefedge[5][5];
};



#endif