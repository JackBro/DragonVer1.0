// VmonitorDoc.cpp :  CVmonitorDoc ���ʵ��
//

#include "internal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVmonitorDoc

IMPLEMENT_DYNCREATE(CVmonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CVmonitorDoc, CDocument)
END_MESSAGE_MAP()


// CVmonitorDoc ����/����

CVmonitorDoc::CVmonitorDoc()
{
	// TODO���ڴ����һ���Թ������
}

CVmonitorDoc::~CVmonitorDoc()
{
}

BOOL CVmonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}




// CVmonitorDoc ���л�

void CVmonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO���ڴ���Ӵ洢����
	}
	else
	{
		// TODO���ڴ���Ӽ��ش���
	}
}


// CVmonitorDoc ���

#ifdef _DEBUG
void CVmonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVmonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVmonitorDoc ����
void CVmonitorDoc::DisplayContent(char *buf, int size)
{
	CView *pView;
	POSITION pos;
	CMDIChildWnd *pChild;

	if(size == 0)
		return;
	if(!gFrame)
		return;
	
	pChild = (CMDIChildWnd *)(gFrame->GetActiveFrame());
	if (pChild == NULL)		
		return;
	pView = pChild->GetActiveView();
	if (pView == NULL)		
		return;
	if ( pView->IsKindOf(RUNTIME_CLASS(CVmonitorView)) )
	{
	//	((CVmonitorView *)pView)->OnUpdate(NULL, 0, 0);
		((CVmonitorView *)pView)->AddContent(buf, size);
		return;
	}
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if(pView == NULL)
			continue;
		if( pView->IsKindOf(RUNTIME_CLASS(CVmonitorView)) )
		{
	//		((CVmonitorView *)pView)->OnUpdate(NULL, 0, 0);
			((CVmonitorView *)pView)->AddContent(buf, size);
			return;
		}
	}
}

void CVmonitorDoc::OnCloseDocument()
{
	CVmonitorApp *pApp = NULL;

	pApp = (CVmonitorApp *)AfxGetApp();
	if(!pApp)
		return;

	pApp->m_pDoc = NULL;
	CDocument::OnCloseDocument();
}


BOOL CVmonitorDoc::SaveModified()
{
	CView *pView;
	CMDIChildWnd *pChild;

	pChild = (CMDIChildWnd *)(gFrame->GetActiveFrame());
	if(pChild == NULL)		
		return true;
	pView = pChild->GetActiveView();
	if(pView == NULL)		
		return true;
	if( pView->IsKindOf(RUNTIME_CLASS(CVmonitorView)) )
		return true;
	if( pView->IsKindOf(RUNTIME_CLASS(CEditView)) )
		return CDocument::SaveModified();
	return true;
}
