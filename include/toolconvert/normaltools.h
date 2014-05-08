#ifndef _NORMALTOOLS_H_
#define _NORMALTOOLS_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif


#include "toolchar/stringtools.h"
#include "toolmath/baseconvert.h"
#include "toolfile/dbugtools.h"

class TOOLCONVERT_DLL CNormalTools : public CStringTools, public CBaseConvert
{
public:		//file process
	void	ConvertHeader2RegList(char *src, char *dest);
	void	Hex2Bin(char *src, char *dest, int unit = 1);
	void	Bin2Hex(char *src, char *dest, int unit = 1);
	void	Bin2Ascii(char *src, char *dest, int unit = 1);
	void	Compare(char *src, char *dest);
	void	CompareImg(char *src1, char *src2, char *dest);
	void	DropHighByte(char *src, char *dest, int opcode = ADD_OPCODE, 
		int div = 1, int srcunit = 2, int dstunit = 1, int minval = 0, int cap = 255);
	void	DropLowByte(char *src, char *dest, int opcode = ADD_OPCODE, 
		int div = 1, int srcunit = 2, int dstunit = 1, int minval = 0, int cap = 255);
	void	CreateOrderHexFile(char *dest, int size);
	void	MergeFile(char *src1, char *src2, char *dst, int src1unit = 1, int src2unit = 1);
	void	AppendFile(char *src, char *dst);
	
	void	SpanConvert(char *src, char *dst, int len, int adr, int span, int size, int opcode, int opval, int unit = 2);
	void	SeqConvert(char *src, char *dst, int len, int adr, int opcode, int opval, int unit = 2);
	double	Snr(char *src1, char *src2, int unit = 1);

private:
	CDbugTools	m_dbug;
};


#endif


