#ifndef _BASE_MATH_H_
#define _BASE_MATH_H_



#ifndef TOOLMATH_DLL
	#define TOOLMATH_DLL  __declspec(dllimport)
#endif


#define MATH_MAXCOUNT		100

enum MATHDATASIGNenum
{
	POSITIVE_DATA,
	NEGATIVE_DATA
};

class TOOLMATH_DLL CBaseMath
{
public:
	CBaseMath();
	virtual ~CBaseMath();

public:
	int  Add(char *src1, char *src2, char *dst, int n1, int n2, int n3 = 0);
	int  Sub(char *src1, char *src2, char *dst, int n1, int n2, int n3 = 0);
	void Mul(char *src1, char *src2, char *dst, int n1, int n2, int n3 = 0);
	void Div(char *src1, char *src2, char *result, char *res, 
				int n1, int n2, int n3 = 0, int n4 = 0);
	void Val2DecStr(char *src, int n, char *str);

public:
	int  Abs(char *src, char *dst, int n1, int n2 = 0);

protected:
	int addsub(char *src1, char *src2, char *dst, int n1, int n2, 
		int n3 = 0, int mode = ADD_OPCODE);
	int	getDataLen(char *src, int len);
};


#endif


