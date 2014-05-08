#ifndef __BASECLASS_IMAGEFRAME_H__
#define __BASECLASS_IMAGEFRAME_H__


#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif

#include "toolfile/filebin.h"

#define END_OF_SEQUENCE		0x1000000


class BASECLASS_DLL CImageFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CImageFrame)
public:
	CImageFrame();
	virtual ~CImageFrame();

public:
	void LoadRawImg(char *src);
	void LoadRawImg(int index);
	void LoadRawImg(char *buf, int len);
	void SetImgParm(int width, int height, int format);
	void CenterWindow(void);

private:
	void Init(void);
	void ShowImage(CDC *dc);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	int m_index;

private:
	int m_width;
	int m_height;
	int m_format;
	int m_simgsize;
	CFileBin m_fb;
	CBufferBase m_pool;

public:
	afx_msg void OnPaint();
	virtual BOOL Create(CWnd* pParentWnd = NULL);
};


#endif
