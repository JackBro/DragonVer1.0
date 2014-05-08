
#ifndef _TOOLS_JPEGENC_H_
#define _TOOLS_JPEGENC_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif



#include "jpegencbase.h"

//===================================================
//
//		CJpegEncoder class
//
//===================================================
class TOOLJPG_DLL CJpegEncoder : public CJpegEncBase
{
public:
	CJpegEncoder(int size = DEFAULT_JPEGDATASIZE);

public:
	void	SetJpegQf(int qf, int tcr = 41, PTRudebrcOption pinfo = NULL);

protected:
	void	InitJpegTable(TJpegTable *table);
	void	updateBrcTable(TJpegTable *table, int qf);
	void	generateBrc(int *QTable, int sftC, int *output);
	int		getShiftValue(int *QTable);

protected:
	void	encblock(char *block, int *lastDc, int mode);
};




//===================================================
//
//		CJpegEncoder1 class
//
//===================================================

class TOOLJPG_DLL CJpegEncoder1 : public CJpegEncBase
{
public:
	CJpegEncoder1(int size = DEFAULT_JPEGDATASIZE);

public:
	void	SetJpegQf(int qf);

protected:
	void	encblock(char *block, int *lastDc, int mode);

private:
	double		m_Lqt[64], m_Cqt[64];
};



//===================================================
//
//		CJpegEncoder2 class
//
//===================================================

class TOOLJPG_DLL CJpegEncoder2 : public CJpegEncBase
{
public:
	CJpegEncoder2(int size = DEFAULT_JPEGDATASIZE);

public:
	void	SetJpegQf(int qf);

protected:
	void	encblock(char *block, int *lastDc, int mode);

private:
	int		m_LQtbl[64], m_CQtbl[64];
};

#endif

