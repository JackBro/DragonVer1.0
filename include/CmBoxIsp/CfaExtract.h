#ifndef _CMODEL_CFAEXTRACT_H_
#define _CMODEL_CFAEXTRACT_H_


#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/CfaExtract568Alg.h"

class CMODELISP_DLL CCfaExtract :	public CImageUnit
{
public:
	CCfaExtract(void);
	virtual ~CCfaExtract(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
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
	void	SetCoefEdge(char *coef);
	char*	GetCoefEdge();
	void	SetDivEdge(int div);
	int 	GetDivEdge();

protected:	
	virtual void UpdateConfig(void);

private:
	CCfaExtract568Alg m_CfaExtract568Alg;

	int  m_mode;
	//edge extract
	int  m_femm;
	int  m_femp;
	int  m_femmin;
	int  m_femmax;
	int  m_femx1;
	int  m_femx2;
	int  m_divedge;
	char m_coefedge[5][5];

};

#endif
