#ifndef __BASECLASS_PICTUREBOX_H__
#define __BASECLASS_PICTUREBOX_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


class BASECLASS_DLL CPictureBox : public CWnd
{
	DECLARE_DYNAMIC(CPictureBox)
public:
	CPictureBox();
	virtual ~CPictureBox();

public:
	void LoadPicture(char *buf, int len);
	void LoadPicture(char *filename);

protected:
	DECLARE_MESSAGE_MAP()

private:
	void RenderPic(HDC dc);
	void ReleasePic(void);
	void Clear(void);

private:
	IPicture *m_pPic;
	int		m_width;
	int		m_height;
	OLE_XSIZE_HIMETRIC m_picwidth;
	OLE_YSIZE_HIMETRIC m_picheight;

public:
	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnPaint();
};

#endif
