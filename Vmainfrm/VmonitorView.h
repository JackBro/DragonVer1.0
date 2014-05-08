// VmonitorView.h : CVmonitorView ��Ľӿ�
//

#pragma once

#include "BaseClass/MClipB.h"

class CVmonitorView : public CEditView
{
public: // �������л�����
	CVmonitorView();
	DECLARE_DYNCREATE(CVmonitorView)

// ����
public:
	CVmonitorDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CVmonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void	AddContent(char *str, int len);
protected:
	void	RemoveContent(int insize);

private:
	CMclipB			m_clip;
	CBufferBase		m_pool;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditFind();
};


#ifndef _DEBUG  // VmonitorView.cpp �ĵ��԰汾
inline CVmonitorDoc* CVmonitorView::GetDocument() const
   { return reinterpret_cast<CVmonitorDoc*>(m_pDocument); }
#endif

