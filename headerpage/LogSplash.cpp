// LogSplash.cpp : 实现文件
//

#include "internal.h"


// CLogSplash

IMPLEMENT_DYNAMIC(CLogSplash, CWnd)
CLogSplash::CLogSplash()
{
}

CLogSplash::~CLogSplash()
{
}


BEGIN_MESSAGE_MAP(CLogSplash, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CLogSplash 消息处理程序

BOOL CLogSplash::Create(char *filename)
{
	RECT rect = {10, 10, 20, 20};
	HBITMAP hBitmap;
	BITMAP bmpinfo;
	int x, y;
	CSize size;

	if(filename == NULL)
	{
		if( !this->m_bmp.LoadBitmap(IDB_LOGSPLASH_BITMAP))
			MessageBox("askdjf");
	}
	else
	{
		hBitmap = (HBITMAP)LoadImage(NULL,
					filename, IMAGE_BITMAP, 0, 0, 
					LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if(hBitmap == NULL)
			return FALSE;
		this->m_bmp.Attach(hBitmap);
	}
	this->m_bmp.GetBitmap(&bmpinfo);
//	size = this->m_bmp.GetBitmapDimension();
//	x = ::GetSystemMetrics(SM_CXSCREEN) - (size.cx >> 1);
//	y = ::GetSystemMetrics(SM_CYSCREEN) - (size.cy >> 1);
	x = (::GetSystemMetrics(SM_CXSCREEN) - bmpinfo.bmWidth) >> 1;
	y = (::GetSystemMetrics(SM_CYSCREEN) - bmpinfo.bmHeight) >> 1;
	rect.left = x;
	rect.top = y;
	rect.right = x + bmpinfo.bmWidth;
	rect.bottom = y + bmpinfo.bmHeight;
//	return CWnd::Create(NULL, NULL, WS_POPUP | WS_VISIBLE | WS_BORDER, 
//		rect, NULL, 0xffff, NULL);
	return this->CreateEx(0, AfxRegisterWndClass(0), "", 
		WS_POPUP | WS_VISIBLE | WS_BORDER, rect, NULL, 0);
//	return TRUE;
}

void CLogSplash::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CDC dctemp;
	BITMAP bmpinfo;

	this->m_bmp.GetBitmap(&bmpinfo);
	dctemp.CreateCompatibleDC(&dc);
	dctemp.SelectObject(&this->m_bmp);
	dc.BitBlt(0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, &dctemp, 0, 0, SRCCOPY);
}
