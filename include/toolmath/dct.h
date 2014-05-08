#ifndef _MX_DCT_H_
#define _MX_DCT_H_


#ifndef TOOLMATH_DLL
	#define TOOLMATH_DLL  __declspec(dllimport)
#endif

#include "toolmath/matrixtool.h"

#define CBASEMATRIX	CMatrixTool<double, double>

#define CBASEMATRIX1 CMatrixTool<int, int>

class TOOLMATH_DLL CDctEnc : public CBASEMATRIX
{
public:
	CDctEnc();
	virtual ~CDctEnc();

public:
	virtual int		Write(double *buf, int len);

public:
	virtual void operator = (CDctEnc &a);
	virtual void operator = (double *a);

protected:
	virtual void	Init(void);

private:
	CBASEMATRIX		m_dct;
};


class TOOLMATH_DLL CDctDec : public CBASEMATRIX
{
public:
	CDctDec();
	virtual ~CDctDec();

public:
	virtual int		Read(double *buf, int len);

public:
	virtual void operator = (CDctDec &a);
	virtual void operator = (double *a);

protected:
	virtual void	Init(void);

private:
	CBASEMATRIX		m_dct;
};




class TOOLMATH_DLL CDctDec1 : public CBASEMATRIX1
{
public:
	CDctDec1();
	virtual ~CDctDec1();

public:
	virtual int		Read(int *buf, int len);	

protected:
	virtual void	fidct8x8(int *inblk);
};


class TOOLMATH_DLL CDctEnc1 : public CBASEMATRIX1
{
public:
	CDctEnc1();
	virtual ~CDctEnc1();

public:
	virtual int		Write(int *buf, int len);

	friend void CDctDec1::fidct8x8(int *inblk);

protected:
	virtual void	fdct8x8(int *inblk);

	 static int		roundc1(int input);
	 static int		roundc2(int input);
	 static int		roundc3(int input);
	 static int		roundc4(int input);
	 static int		roundc5(int input);
	 static int		roundc6(int input);
	 static int		roundc7(int input);
};

#endif


