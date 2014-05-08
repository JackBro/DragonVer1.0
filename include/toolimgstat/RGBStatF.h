#ifndef __TOOLIMGSTAT_RGBSTATF_H__
#define __TOOLIMGSTAT_RGBSTATF_H__


#ifndef TOOLIMGSTAT_DLL
	#define TOOLIMGSTAT_DLL  __declspec(dllimport)
#endif


#include "RGBStat.h"

class TOOLIMGSTAT_DLL CRgbStatF : public CRgbStat
{
public:
	CRgbStatF(void);
	virtual ~CRgbStatF(void);

public:
	//void printRgbfmtMsg(int * des, char * src,int srcfmt, int width, int height, int unit);
    
protected:
	/*
	void Rgb24Msg(int * des, char * src, int srcfmt, int width, int height, int unit);
	void Rgb32Msg(int * des, char * src, int srcfmt, int width, int height, int unit);
	void Rgb555Msg(int * des, char * src, int srcfmt, int width, int height, int unit);
	void Rgb565Msg(int * des, char * src, int srcfmt, int width, int height, int unit);
	void updateRgb(int * des, int * ndes);
	*/
};

#endif
