#ifndef _YIQ2RGB_H_
#define _YIQ2RGB_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "toolbuf/fifobuf.h"

//=============== class CYiq2rgb =================
//#define IMAGEWIDTH 640
//#define IMAGEHEIGHT 480

class TOOLCONVERT_DLL CYiq2rgb : public CFifoBuf
{
public:
	CYiq2rgb(int cap = 256, int width = 640, int height = 480);

public:
	virtual int     Read(char *buf, int len);
	virtual int     Write(char *buf, int len);

public:
	int		GetSize(void);
	int		GetTotalSize(void);
	void	SetSize(int size = BUFFER_DEFAULT_SIZE);

	void  SetCap(int cap);
	int   GetCap(void);
    void  SetImageWidth(int width);
	int   GetImageWidth(void);
	void  SetImageHight(int hight);
	int   GetImageHight(void);

	void  Process(char *yiq ,char *rgb);

private:
	char  Yiq2r(void);
	char  Yiq2g(void);
	char  Yiq2b(void);

private:
	int  m_cap;
	int  m_imageWidth;
	int  m_imageHight;
};

#endif


