// VmonitorView.cpp : CVmonitorView 类的实现
//

#include "internal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CMD_CONTENT_LEN		0x10000


IMPLEMENT_DYNCREATE(CVmonitorView, CEditView)

BEGIN_MESSAGE_MAP(CVmonitorView, CEditView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
END_MESSAGE_MAP()

// CVmonitorView 构造/销毁

CVmonitorView::CVmonitorView() : m_pool(CMD_CONTENT_LEN)
{

}

CVmonitorView::~CVmonitorView()
{
}

BOOL CVmonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CEditView::PreCreateWindow(cs);
}

void CVmonitorView::OnDraw(CDC* /*pDC*/)
{
}

BOOL CVmonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CVmonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CVmonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// CVmonitorView 诊断

#ifdef _DEBUG
void CVmonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CVmonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVmonitorDoc* CVmonitorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVmonitorDoc)));
	return (CVmonitorDoc*)m_pDocument;
}
#endif //_DEBUG


void CVmonitorView::OnInitialUpdate()
{
	this->m_clip.Init(this->m_hWnd);
	CEditView::OnInitialUpdate();
//	this->GetParentFrame()->SetIcon(AfxGetApp()->LoadIcon(IDI_CMDWIN), TRUE);
//	this->GetDocument()->SetTitle("Command Window");
}

void CVmonitorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	gComChnl.Write((char *)&nChar, 1);
	nFlags = 0;
	nRepCnt = 0;
}

void CVmonitorView::RemoveContent(int insize)
{
	int len, start = 0, i;
	CEdit &edit = this->GetEditCtrl();

	len = edit.GetWindowTextLength()+1;
	if( len <= (CMD_CONTENT_LEN - (insize << 1)) )
		return;
	this->m_pool.SetSize(len);
	start = edit.GetWindowText(this->m_pool.m_buf, this->m_pool.GetTotalSize());
	for(i = start-1; i > 0; i--)
	{
		if(this->m_pool.m_buf[i] == '\n')
			break;
	}
	edit.SetSel(0, -1);
	edit.ReplaceSel(this->m_pool.m_buf+i+1);
}

void CVmonitorView::AddContent(char *rbuf, int size)
{
	int len, start = 0, i;
	CEdit &edit = this->GetEditCtrl();

	if(size == 0)
		return;
	this->RemoveContent(size);
	start = edit.GetWindowText(this->m_pool.m_buf, this->m_pool.GetTotalSize());
	len = start;
	for(i = 0; i < size; i++)
	{
		switch(rbuf[i])
		{
		case '\b':
			if(len && (this->m_pool.m_buf[len-1] != '\n'))
				len--;
			if(len < start)
				start = len;
			break;
		case '\r':
		case '\n':
			this->m_pool.m_buf[len++] = '\r';
			this->m_pool.m_buf[len++] = '\n';
			break;
		default:
			this->m_pool.m_buf[len++] = rbuf[i];
			break;
		}
	}
	this->m_pool.m_buf[len] = 0;
	edit.SetSel(start, -1);
	edit.ReplaceSel(this->m_pool.m_buf+start);
}

void CVmonitorView::OnEditCut()
{
	CEditView::OnEditCopy();
}

void CVmonitorView::OnEditPaste()
{
	int size;
	CStringTools tools;

	size = this->m_clip.GetLength();
	this->m_clip.Read(this->m_pool.m_buf, size);
	size = tools.FindFirstCR(this->m_pool.m_buf, size);
	gComChnl.Write(this->m_pool.m_buf, size);
}

void CVmonitorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_DELETE)
		return;
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CVmonitorView::OnEditFind()
{
//	CFindReplaceDialog fd;
}



