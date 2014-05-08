
#include "internal.h"



#define BMP24HEADERLENG	(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
#define IDC_IMAGEBOX_ZOOM 6001
//#define BMP8HEADERLENG	(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))

IMPLEMENT_DYNAMIC(CImageBox, CWnd)

BEGIN_MESSAGE_MAP(CImageBox, CWnd)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_IMAGEBOX_NEXTDIFFPIXEL, OnMenuNextDiffPixel)
	ON_COMMAND(ID_IMAGEBOX_PREVIOUSDIFFPIXEL, OnMenuPreviousDiffPixel)
	ON_COMMAND(ID_IMAGEBOX_FIRSTDIFFPIXEL, OnMenuFirstDiffPixel)
	ON_COMMAND(ID_IMAGEBOX_LASTDIFFPIXEL, OnMenuLastDiffPixel)
	ON_COMMAND(ID_IMAGEBOX_STRETCHIMAGE, OnMenuStretchImage)
	ON_COMMAND(ID_IMAGEBOX_SAVEALLDIFFPIXELTEXT, OnMenuSaveAllDiffPixelText)
	ON_COMMAND(ID_IMAGEBOX_SHOWFIRSTIMAGE, OnMenuShowFirstImage)
	ON_COMMAND(ID_IMAGEBOX_SHOWSECONDIMAGE, OnMenuShowSecondImage)
	ON_COMMAND(ID_IMAGEBOX_SHOWDIFF, OnMenuShowDiff)
	//ON_UPDATE_COMMAND_UI(ID_IMAGEBOX_NEXTDIFFPIXEL, OnUpdateMenuItem) 
	//ON_UPDATE_COMMAND_UI(ID_IMAGEBOX_PREVIOUSDIFFPIXEL, OnUpdateMenuItem) 
	//ON_UPDATE_COMMAND_UI(ID_IMAGEBOX_FIRSTDIFFPIXEL, OnUpdateMenuItem) 
	//ON_UPDATE_COMMAND_UI(ID_IMAGEBOX_LASTDIFFPIXEL, OnUpdateMenuItem) 
END_MESSAGE_MAP()

CImageBox::CImageBox()
{
	CImageDiff();
}

CImageBox::~CImageBox()
{	
}


void CImageBox::ShowImageDiff(UINT8 *buf1, UINT8 *buf2, int len) //assume buf1, buf2 format are all rgb24
{

	ImageDiff(buf1, buf2, len);
	m_pFirstbuf = buf1;
	m_pSecondbuf = buf2;
	
	this->Invalidate();
	this->UpdateWindow();
}

void CImageBox::DrawCurPixel(Graphics *graphics)
{
	WCHAR wstrXY[100];
	int x,y;
	REAL rx, ry;
	
	CImageDiff::GetCurrentPixel(&x, &y);
	swprintf(wstrXY, L"(%d,%d)", x, y);

	Pen RedPen(Color(255, 0, 0), 1);
	Point point1( x - 10, y);
	Point point2( x + 10, y);
	Point point3( x, y - 10);
	Point point4( x, y + 10);
	graphics->DrawLine(&RedPen, point1, point2);
	graphics->DrawLine(&RedPen, point3, point4);

	Font myFont(L"ËÎÌו",10);
	if (x > (GetImgWidth() - 40))
		 rx = (REAL) (x - 100);
	else 
		rx = (REAL) (x + 10);

    if (y < 40)
		ry = (REAL) (y + 10);
	else 
		ry = (REAL) (y - 20);
	PointF origin(rx, ry);
	SolidBrush redBrush(Color(255, 0, 0));
	graphics->DrawString(wstrXY, (INT) wcslen(wstrXY), &myFont, origin, &redBrush);
}

void CImageBox::ShowNextDiffPixel(void)
{
	GotoNextDiffPixel();
	//this->OnPaint();
	this->Invalidate();
	this->UpdateWindow();
}

void CImageBox::ShowFirstDiffPixel(void)
{
	GotoFirstDiffPixel();
	//this->OnPaint();
	this->Invalidate();
	this->UpdateWindow();
}

void CImageBox::ShowLastDiffPixel(void)
{
	GotoLastDiffPixel();
	//this->OnPaint();
	this->Invalidate();
	this->UpdateWindow();
}

void CImageBox::ShowPreviousDiffPixel(void)
{
	GotoPreviousDiffPixel();
	//this->OnPaint();
	this->Invalidate();
	this->UpdateWindow();
}

//void CImageBox::OnMenuShowAllDiffPixelText(void)
//{
//	WCHAR wstrXY[100];
//	PointF origin;
//	char *p, *FirstPos;
//	int i, j, n;
//
//	pOutputWnd->Create(NULL, "DIFF", WS_CHILD | WS_VISIBLE,
//		CRect(0, 0, this->m_imgwidth, this->m_imgheight) , this, IDC_IMAGEBOX_DIFFPIXTEXT);
//
//	Font myFont(L"ËÎÌו",10);
//	Graphics graphics(pOutputWnd->m_hWnd);
//	SolidBrush redBrush(Color(255, 0, 0));
//
//	FirstPos = this->m_pool.m_buf + BMP24HEADERLENG;
//	n = 0;
//	for (i = 0; i < this->m_imgheight; i++)
//		for (j = 0; j < this->m_imgwidth; j++)
//		{
//			p = FirstPos + i*j*3 ; 
//			if (!(*p || *(p+1) || *(p+2)))
//			{
//				n = n + 1; 
//				//ShowOnePixelText(n,j,i);
//				origin.X = 10;
//				origin.Y = 30 * n;
//				swprintf(wstrXY, L"(%d,%d)", j, i);
//				graphics.DrawString(wstrXY, (INT) wcslen(wstrXY), &myFont, origin, &redBrush);
//			}
//		}
//
//	pOutputWnd->UpdateWindow();
//
//}
void CImageBox::SaveAllDiffPixelToFile(char *filename)
{
	int len;

	CFileBin fb;
	CBufferBase pool_temp(0x20000);

	len = GetAllDiffPixelText(pool_temp.m_buf, pool_temp.GetTotalSize());

	fb.Open(filename, "wb");
	fb.Write(pool_temp.m_buf, len);
	fb.Close();
}

void CImageBox::OnMenuSaveAllDiffPixelText()
{
	SaveAllDiffPixelToFile(m_filename);
}

RectF CImageBox::ComputeStretchRect(int scale)
{
	int cxplus, cyplus;
	RECT rcClient; 
	RectF dstrect;
	
	cxplus = ((m_oprect.right- m_oprect.left) * (scale - 1)) >> 1;
	cyplus = ((m_oprect.bottom - m_oprect.top) * (scale - 1)) >> 1;
	dstrect.X = (REAL) (m_oprect.left - cxplus);
	dstrect.Y = (REAL) (m_oprect.top - cyplus);
	dstrect.Width = (REAL) ((m_oprect.right- m_oprect.left)*scale);
	dstrect.Height = (REAL) ((m_oprect.bottom - m_oprect.top) * scale);
	
	this->GetClientRect(&rcClient);

	if (dstrect.X < 0)
		dstrect.X = 0;

	if (dstrect.Y < 0)
		dstrect.Y = 0;
	
	if ((dstrect.X + dstrect.Width) > rcClient.right)
		dstrect.X = rcClient.right -  dstrect.Width;

	if ((dstrect.Y + dstrect.Height) > rcClient.bottom)
		dstrect.Y = rcClient.bottom - dstrect.Height;

	return dstrect;
}


void CImageBox::SetDiffFileName(char *filename)
{
	strcpy(m_filename, filename);
}

int CImageBox::GetDiffFileName(char *filename, unsigned int size)
{
	if (size < (strlen(m_filename) + 1)) 
		return -1;
	strcpy(filename, m_filename);
	return 0;
}

void CImageBox::SetScaleRatio(int ratio)
{
	m_scaleratio = ratio;
}

int  CImageBox::GetScaleRatio()
{
	return m_scaleratio;
}

void CImageBox::ShowStretchImage(int scale)
{
	RectF dstrect;

	dstrect = this->ComputeStretchRect(scale);
	this->RedrawWindow();
	CDC* dc = this->GetDC();
	Graphics graphics(dc->m_hDC);
	Bitmap bitmap(
		(BITMAPINFO *)(GetPoolBuf() + sizeof(BITMAPFILEHEADER)),
		(VOID *)(GetPoolBuf() + BMP24HEADERLENG) );
	//graphics.DrawImage( &bitmap, dstrect, srcrect, UnitPixel, NULL);
	graphics.DrawImage( &bitmap, dstrect, (REAL) m_oprect.left, (REAL) m_oprect.top, 
		(REAL) (m_oprect.right - m_oprect.left), (REAL) ( m_oprect.bottom - m_oprect.top), UnitPixel);

}

BOOL CImageBox::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, NULL, WS_CHILD |  WS_VISIBLE, 
		rect, pParentWnd, nID, NULL);
}

void CImageBox::ShowImageFromBuf(Graphics *graphics)
{
	Bitmap bitmap(
		(BITMAPINFO *)(GetPoolBuf() + sizeof(BITMAPFILEHEADER)),
		(VOID *)(GetPoolBuf() + BMP24HEADERLENG) );
	Rect rect(0, 0, GetImgWidth(), GetImgHeight());
	graphics->DrawImage( &bitmap, rect);
}


void CImageBox::OnPaint()
{
	CPaintDC dc(this);
	Graphics graphics(dc.m_hDC);
	
	this->ShowImageFromBuf(&graphics);	
	this->DrawCurPixel(&graphics);
	//CWnd::OnPaint();
}


void CImageBox::OnRButtonDown( UINT nFlags, CPoint point)
{
	CMenu menu, *pPopup;
	CPoint pt;

	CWnd::OnRButtonDown(nFlags, point);
	if (menu.LoadMenu(IDR_IMAGEBOX_MENU))
	{
		pt.x = point.x;
		pt.y = point.y;
		this->ClientToScreen (&pt); 
		pPopup = menu.GetSubMenu(0);
		UpdateMenu(pPopup, this->IsDiffOn());
		pPopup->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, pt.x, pt.y, this);
		menu.DestroyMenu();
	}

}



void CImageBox::UpdateMenu(CMenu *menu, BOOL bDiffOn)
{
	if (bDiffOn)
	{
		menu->EnableMenuItem(ID_IMAGEBOX_NEXTDIFFPIXEL , MF_BYCOMMAND | MF_ENABLED );
		menu->EnableMenuItem(ID_IMAGEBOX_PREVIOUSDIFFPIXEL, MF_BYCOMMAND | MF_ENABLED );
		menu->EnableMenuItem(ID_IMAGEBOX_FIRSTDIFFPIXEL , MF_BYCOMMAND | MF_ENABLED );
		menu->EnableMenuItem(ID_IMAGEBOX_LASTDIFFPIXEL , MF_BYCOMMAND | MF_ENABLED );
		menu->EnableMenuItem(ID_IMAGEBOX_SAVEALLDIFFPIXELTEXT , MF_BYCOMMAND | MF_ENABLED );
	}
	else
	{
		menu->EnableMenuItem(ID_IMAGEBOX_NEXTDIFFPIXEL , MF_BYCOMMAND | MF_GRAYED  );
		menu->EnableMenuItem(ID_IMAGEBOX_PREVIOUSDIFFPIXEL, MF_BYCOMMAND | MF_GRAYED  );
		menu->EnableMenuItem(ID_IMAGEBOX_FIRSTDIFFPIXEL, MF_BYCOMMAND | MF_GRAYED  );
		menu->EnableMenuItem(ID_IMAGEBOX_LASTDIFFPIXEL, MF_BYCOMMAND | MF_GRAYED  );
		menu->EnableMenuItem(ID_IMAGEBOX_SAVEALLDIFFPIXELTEXT , MF_BYCOMMAND | MF_GRAYED );
	}
}

void CImageBox::OnLButtonDown( UINT nFlags, CPoint point)
{
	RECT rcClient;                 // client area rectangle 
    POINT ptClientUL;              // client upper left corner 
    POINT ptClientLR; 

	::SetCapture(this->m_hWnd); 
	::GetClientRect(this->m_hWnd, &rcClient); 

	ptClientUL.x = rcClient.left; 
	ptClientUL.y = rcClient.top; 
	ptClientLR.x = rcClient.right; 
	ptClientLR.y = rcClient.bottom;

	::ClientToScreen(this->m_hWnd, &ptClientUL); 
	::ClientToScreen(this->m_hWnd, &ptClientLR); 
	::SetRect(&rcClient, ptClientUL.x, ptClientUL.y,  ptClientLR.x, ptClientLR.y); 
	::ClipCursor(&rcClient); 

	m_LButtonPt = point;
	nFlags = nFlags;
}

void CImageBox::OnLButtonUp( UINT nFlags, CPoint point)
{
	SetOPRect(DrawRect(nFlags, point));	
	::ClipCursor(NULL); 
	::ReleaseCapture(); 

}

void CImageBox::OnMenuNextDiffPixel()
{
	ShowNextDiffPixel();
}


void CImageBox::OnMenuPreviousDiffPixel()
{
	ShowPreviousDiffPixel();	
}

void CImageBox::OnMenuLastDiffPixel()
{
	ShowLastDiffPixel();
}

void CImageBox::OnMenuFirstDiffPixel()
{
	ShowFirstDiffPixel();
}

void CImageBox::OnMenuStretchImage()
{
	ShowStretchImage(m_scaleratio);
}

void CImageBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags == MK_LBUTTON)
		this->DrawRect(nFlags, point);
}

void CImageBox::OnMenuShowFirstImage()
{
    LoadRawImg(this->m_pFirstbuf, GetImgSize());
	this->Invalidate();
	this->UpdateWindow();
}

void CImageBox::OnMenuShowSecondImage()
{
	LoadRawImg(this->m_pSecondbuf, GetImgSize());
	this->Invalidate();
	this->UpdateWindow();
}

void CImageBox::OnMenuShowDiff()
{
	ShowImageDiff(this->m_pFirstbuf, this->m_pSecondbuf, GetImgSize());
}

//void CImageBox::OnUpdateMenuItem(CCmdUI *pCmdUI)
//{
//	pCmdUI->Enable(m_bDiffOn);
//}

RECT CImageBox::DrawRect(UINT nFlags, CPoint point)
{
	RECT rect;

	if ( m_LButtonPt.x < point.x)
	{
		rect.left = m_LButtonPt.x;
		rect.right = point.x;
	}
	else{
		rect.left = point.x;
		rect.right = m_LButtonPt.x;
	}
	
	if (m_LButtonPt.y < point.y)
	{
		rect.top =  m_LButtonPt.y;
		rect.bottom = point.y;
	}
	else{
		rect.top =   point.y;
		rect.bottom = m_LButtonPt.y;
	}

	this->RedrawWindow();
	Graphics graphics(this->m_hWnd);
	Pen blackPen(Color(0, 0, 0), 1);
	blackPen.SetDashStyle(DashStyleDash);
	graphics.DrawRectangle(&blackPen, rect.left, rect.top, rect.right - rect.left, rect.bottom- rect.top);	
	nFlags = nFlags;
	return rect;
}

void CImageBox::SetOPRect(RECT rect)
{
	m_oprect = rect;
}