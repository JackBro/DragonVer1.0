#ifndef __SILLGUI_IMAGEBOX_H__
#define __SILLGUI_IMAGEBOX_H__


#ifndef SILLGUI_DLL
	#define SILLGUI_DLL  _declspec(dllimport)
#endif

#include "tools/type.h"
#include "ImageDiff.h"


class SILLGUI_DLL CImageBox : public CWnd, public CImageDiff
{
	DECLARE_DYNAMIC(CImageBox)

private:
	UINT8	*m_pFirstbuf;
	UINT8	*m_pSecondbuf;
	CImageDiff m_imagediff;

private:
	char	m_filename[200];
	int		m_scaleratio;
	RECT	m_oprect;
	

private:  //for drawing
	CPoint	m_LButtonPt;
	//bool	m_setrect;

public:
	CImageBox();
	virtual ~CImageBox();

public:
	void ShowNextDiffPixel(void);
	void ShowPreviousDiffPixel(void);
	void ShowFirstDiffPixel(void);
	void ShowLastDiffPixel(void);
	void ShowImageDiff(UINT8 *buf1, UINT8 *buf2, int len) ;
	void ShowImageFromBuf(Graphics *graphics);
	void DrawCurPixel(Graphics *graphics);
	void ShowStretchImage(int scale);
	void UpdateMenu(CMenu *menu, BOOL bDiffOn);
	void SaveAllDiffPixelToFile(char *filename);

public:
	void SetDiffFileName(char *filename);
	void SetOPRect(RECT rect);
	int  GetDiffFileName(char *filename, unsigned int size);
	void SetScaleRatio(int ratio);
	int  GetScaleRatio();

private:
	RECT DrawRect(UINT nFlags, CPoint point);
	RectF ComputeStretchRect(int scale);

public:
	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown( UINT, CPoint );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMenuNextDiffPixel();
	afx_msg void OnMenuPreviousDiffPixel();
	afx_msg void OnMenuLastDiffPixel();
	afx_msg void OnMenuFirstDiffPixel();
	afx_msg void OnMenuStretchImage();
	//afx_msg void OnMenuShowAllDiffPixelText();
	afx_msg void OnMenuSaveAllDiffPixelText();
	afx_msg void OnMenuShowFirstImage();
	afx_msg void OnMenuShowSecondImage();
	afx_msg void OnMenuShowDiff();
	afx_msg void OnUpdateMenuItem(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
	
};

#endif
