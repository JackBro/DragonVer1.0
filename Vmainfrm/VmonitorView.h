// VmonitorView.h : CVmonitorView 类的接口
//

#pragma once

#include "BaseClass/MClipB.h"

class CVmonitorView : public CEditView
{
public: // 仅从序列化创建
	CVmonitorView();
	DECLARE_DYNCREATE(CVmonitorView)

// 属性
public:
	CVmonitorDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
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

// 生成的消息映射函数
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


#ifndef _DEBUG  // VmonitorView.cpp 的调试版本
inline CVmonitorDoc* CVmonitorView::GetDocument() const
   { return reinterpret_cast<CVmonitorDoc*>(m_pDocument); }
#endif

