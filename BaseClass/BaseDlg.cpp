// V558TestFpgaDlg.cpp : 实现文件
//
#include "internal.h"

// CBaseDlg 对话框


IMPLEMENT_DYNAMIC(CBaseDlg, CDialog)
CBaseDlg::CBaseDlg(UINT Idd, CWnd* pParent /*=NULL*/)
	: CDialog(Idd, pParent)
{
	//this->InitBaseFont();
}

CBaseDlg::~CBaseDlg()
{
	//this->m_font.DeleteObject();
}

void CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDlg, CDialog)
END_MESSAGE_MAP()


// CBaseDlg 消息处理程序

int CBaseDlg::OpenFileDlg(CString *str)
{
	CFileDialog dlg(TRUE, NULL, NULL, 
					OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST,
					_T("All Files (*.*)|*.*||"), this);

	dlg.m_ofn.lpstrTitle = _T("Open File");
	if (dlg.DoModal() == IDOK) 
	{
		if(dlg.GetFileName().IsEmpty())
			return 0;
		*str = dlg.GetPathName();
		return 1;
	}
	return 0;
}

//void CBaseDlg::InitBaseFont()
//{
//
//	this->m_font.CreateFont(
//							12,                        
//							0,                         
//							0,                         
//							0,                         
//							FW_NORMAL,                 
//							FALSE,                     
//							FALSE,                     
//							0,                         
//							ANSI_CHARSET,              
//							OUT_DEFAULT_PRECIS,        
//							CLIP_DEFAULT_PRECIS,       
//							DEFAULT_QUALITY,           
//							DEFAULT_PITCH | FF_SWISS,  
//							"Arial");
//
//}
//
//CFont* CBaseDlg::GetBaseFont()
//{
//	return &this->m_font;
//}
