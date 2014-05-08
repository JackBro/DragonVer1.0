#ifndef __SILLGUI_IMAGEDIFF_H_
#define __SILLGUI_IMAGEDIFF_H_


#ifndef SILLGUI_DLL
	#define SILLGUI_DLL  _declspec(dllimport)
#endif

#include "toolbuf/bufferbase.h"


class SILLGUI_DLL CImageDiff
{
public:
	CImageDiff();
	virtual ~CImageDiff();

private:
	int		m_imgwidth;
	int		m_imgheight;
	int		m_format;
	int		m_simgsize;
	int     m_curpixel_x;
	int     m_curpixel_y;
	CBufferBase m_pool;
	BOOL	m_bDiffOn;

public:
	void SetImgParm(int width, int height);
	int	 GetImgWidth(void);
	int  GetImgHeight(void);
	int	 GetImgSize(void);
	BOOL IsDiffOn();

public:
	void LoadRawImg(UINT8 *buf, int len);
	void LoadRawImg(char *src);
	void GotoPixel(int x, int y);
	void GotoNextDiffPixel(void);
	void GotoPreviousDiffPixel(void);
	void GotoFirstDiffPixel(void);
	void GotoLastDiffPixel(void);
	int  GetAllDiffPixelText(char *buf, int len);
	void ImageDiff(UINT8 *buf1, UINT8 *buf2, int len);
	void GetCurrentPixel(int *x, int *cy);
	char* GetPoolBuf(void);

};

#endif