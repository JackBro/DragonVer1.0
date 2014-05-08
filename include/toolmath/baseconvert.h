#ifndef _BASE_CONVERT_H_
#define _BASE_CONVERT_H_


#ifndef TOOLMATH_DLL
	#define TOOLMATH_DLL  __declspec(dllimport)
#endif


#define MAXFILTERCOUNT	20
#define MAXCYCLECOUNT	20

class TOOLMATH_DLL CBaseConvert
{
public:
	int GetMaxDivisor(int a, int b);
	int GetMinMultiple(int a, int b);
	int CheckResizeParm(int src, int dst, int factor);

public:
	void setvalue(char *dst, int val, int unit = 1);
	void setvalue(char *dst, int *val, int count, int unit = 1);
	int  getvalue(char *src, int unit = 1);
	void getvalue(char *src, int *val, int count, int unit = 1);

public:
	void prefilter(int *dat, int cycle, int *filter, int count);
	void MRotate(char *src, char *dest, int width, int height, int rotate, int unit = 1);
	void GetRotateParm(int width, int height, int rotate, int *parm, int unit = 1);

protected:
	int HorExtent(char *src, char *dest, int width, int height, int cycle, int count, int unit = 1);
	int VerExtent(char *src, char *dest, int width, int height, int cycle, int count, int unit = 1);
	int HorPrefilter(char *src, char *dest, int width, int height, int cycle, 
		int count, int *filter, int unit = 1);
	int VerPrefilter(char *src, char *dest, int width, int height, int cycle, 
		int count, int *filter, int unit = 1);
	int Addedge(char *src, char *dest, int width, int height, int cycle, int count, int unit = 1);
	int Prefilter(char *src, char *dest, int width, int height, int cycle, 
			int count, int *filter, int unit = 1);
	int	Calculate(int val, int opval, int opcode);
	int DataCompare(char *src, char *dst, int len);

	void DataCalSpan(char *buf, int len, int adr, int span, int size, int opcode, int opval, int unit = 2);
	void DataCal(char *buf, int len, int adr, int opcode, int opval, int unit = 2);

public:
	int  GetImgSize(int width, int height, int fmt);
	int  AdjustImgparm(int *width, int *height, int fmt);
};

#endif


