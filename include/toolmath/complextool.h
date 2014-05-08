#ifndef _COMPLEXTOOLS_H_
#define _COMPLEXTOOLS_H_



#ifndef TOOLMATH_DLL
	#define TOOLMATH_DLL  __declspec(dllimport)
#endif



#define TEMPLATE_COMPLEX	template <class T1>
#define CCOMPLEXTOOL		CComplexTool <T1>

template <class T1 = int>
class TOOLMATH_DLL CComplexTool
{
public:
	CComplexTool(int real = 0, int img = 0);

public:
	void operator += (CComplexTool &a);
	void operator -= (CComplexTool &a);
	void operator = (CComplexTool &a);
	void operator *= (CComplexTool &a);
	void operator /= (CComplexTool &a);
	void operator *= (T1 a);
	void operator /= (T1 a);
	void operator += (T1 a);
	void operator -= (T1 a);

protected:
	virtual void multi(T1 real, T1 img);
	virtual void divc(T1 a);

private:
	T1	m_real, m_img;
};

TEMPLATE_COMPLEX
CCOMPLEXTOOL::CComplexTool(int real, int img)
{
	this->m_img = img;
	this->m_real = real;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator += (CComplexTool &a)
{
	this->m_img += a.m_img;
	this->m_real = a.m_real;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator = (CComplexTool &a)
{
	this->m_img = a.m_img;
	this->m_real = a.m_real;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator -= (CComplexTool &a)
{
	this->m_img -= a.m_img;
	this->m_real -= a.m_real;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator *= (CComplexTool &a)
{
	this->multi(a.m_real, a.m_img);
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator /= (CComplexTool &a)
{
	this->multi(a.m_real, 0 - a.m_img);
	this->divc(a.m_img * a.m_img + a.m_real * a.m_real);
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator *= (T1 a)
{
	this->m_img *= a;
	this->m_real *= a;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator /= (T1 a)
{
	this->divc(a);
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::multi(T1 real, T1 img)
{
	T1 va, vb, vc;

	va = (this->m_real - this->m_img) * img;
	vb = (real - img) * this->m_real;
	vc = (real + img) * this->m_img;
	this->m_real = va + vb;
	this->m_img = va + vc;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::divc(T1 a)
{
	//ASSERT(a);
	if(a == 0)
		a = 1;
	this->m_img /= a;
	this->m_real /= a;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator += (T1 a)
{
	this->m_real += a;
}

TEMPLATE_COMPLEX
void CCOMPLEXTOOL::operator -= (T1 a)
{
	this->m_real -= a;
}

#endif
