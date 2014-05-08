
#ifndef _CMODEL_702IPP_H_
#define _CMODEL_702IPP_H_


#ifndef TOOLV702ALG_DLL
	#define TOOLV702ALG_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"


#define SIZER_COEFF_FMT		9
#define HOR_COEFF_FMT		9
#define VER_COEFF_FMT		8
#define SIZER_RND			0		// 1

enum IMAGESIZEenum{
	CIF_WIDTH		=	352,
	CIF_HEIGHT		=	288,
	VGA_WIDTH		=	640,
	VGA_HEIGHT		=	480
};



//=======================================================================================
//			class C702Sizer
//=======================================================================================

class TOOLV702ALG_DLL C702Sizer : public CBaseObject
{
public:
	C702Sizer(int width = 640, int height = 480, int pal = 1, int interlace = 0, int average = 0, int compon = 1, int fmt = YUV422_YUYV);
	virtual ~C702Sizer();

public:
	void	Init(int width, int height, int pal, int interlace = 0, int average = 0, int compon = 1, int fmt = YUV422_YUYV);
	int		Process(char *src, char *dst, char *verdst);

protected:
	int		VerSizer(char *src, char *dst, int srcw, int srch, int dsth);
	int		HorSizer(char *src, char *dst, int srcw, int srch, int dstw);
	int		Write(char *buf, int len);
	int		Read(char *buf, int len);
	int		VerWrite(char *buf, int len);
	int		VerRead(char *buf, int len);
	int		IntelaceProc(char *src, char *dst, int width, int height);
	int		ProgProc(char *src, char *dst, int width, int height);


private:
	int		GetFactor(int srcsize);	
	int		GetValue(int val0, int val1, int coeff, int precision);
	void	LoopLinebuf(void);

private:
	int		m_width, m_height;
	int		m_fmt, m_pal, m_interlace, m_average, m_compon;
	int		m_coeff5[2], m_coeff9[4], m_coeff45[22];
	int		m_up, m_down, m_phase, *m_pcoeff;
	CLine422Buf		m_linebuf[2];
	CLine422Buf		*m_plinebuf[2];
};

#endif