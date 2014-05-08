#include "internal.h"



#define BMP24HEADERLENG	(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))

// CImageFrame
IMPLEMENT_DYNCREATE(CImageFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CImageFrame, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CImageFrame ¹¹Ôì/Ïú»Ù
CImageFrame::CImageFrame()
{
	this->Init();
}

CImageFrame::~CImageFrame()
{
}

void CImageFrame::Init(void)
{
	this->m_width = 0;
	this->m_height = 0;
	this->m_format = RGB24;
	this->m_simgsize = 0;
	this->m_index = 0;
}

void CImageFrame::SetImgParm(int width, int height, int format)
{
	CBaseConvert tools;
	CRaw2RgbF trgb;

	this->m_width = width;
	this->m_height = height;
	this->m_format = format;
	tools.AdjustImgparm(&this->m_width, &this->m_height, this->m_format);
	this->m_simgsize = tools.GetImgSize(this->m_width, this->m_height, this->m_format);
	this->m_pool.SetSize(this->m_width * this->m_height * 3 + BMP24HEADERLENG);
	trgb.addbmp24header(this->m_pool.m_buf, this->m_width, this->m_height);
}

void CImageFrame::LoadRawImg(char *buf, int len)
{
	CRgb24Convert tools;
	CBaseConvert tc;

	//if(this->m_format == BMP)
	//{
	//	memcpy(this->m_pool.m_buf, buf, len);
	//	return;
	//}
	if(len < tc.GetImgSize(this->m_width, this->m_height, this->m_format))
		return;
	tools.Img2Rgb24(buf, this->m_pool.m_buf + BMP24HEADERLENG,
		this->m_width, this->m_height, this->m_format);
//	this->Invalidate();
}

void CImageFrame::LoadRawImg(char *src)
{
	int len;
	CBufferBase pool(this->m_simgsize);

	this->m_fb.Open(src);
	len = this->m_fb.Read(pool.m_buf, this->m_simgsize);
	if(len)
		this->LoadRawImg(pool.m_buf, this->m_simgsize);
}

void CImageFrame::LoadRawImg(int index)
{
	int len;
	CBufferBase pool(this->m_simgsize);

	index &= ~END_OF_SEQUENCE;
	this->m_fb.Fseek(this->m_simgsize * index, SEEK_SET);
	len = this->m_fb.Read(pool.m_buf, this->m_simgsize);
	if(len == 0)
	{
		this->m_index |= END_OF_SEQUENCE;
		return;
	}
	this->m_index = index;
	this->LoadRawImg(pool.m_buf, this->m_simgsize);
}

void CImageFrame::CenterWindow(void)
{
	RECT rc, rw;
	int winw, winh, x, y, bw, bh;

	winw = GetSystemMetrics(SM_CXSCREEN);
	winh = GetSystemMetrics(SM_CYSCREEN);
	this->GetClientRect(&rc);
	this->GetWindowRect(&rw);
	bw = rw.right - rw.left - rc.right;
	bh = rw.bottom - rw.top - rc.bottom;
	x = (winw - this->m_width + bw) >> 1;
	y = (winh - this->m_height + bh) >> 1;
	this->MoveWindow(x, y, this->m_width + bw, this->m_height + bh);
}

void CImageFrame::ShowImage(CDC *dc)
{
	dc->SetStretchBltMode(STRETCH_DELETESCANS);
	StretchDIBits(dc->m_hDC, 0, 0, this->m_width, this->m_height,
				0, 0, this->m_width, this->m_height,
				this->m_pool.m_buf + BMP24HEADERLENG, 
				(BITMAPINFO *)(this->m_pool.m_buf + sizeof(BITMAPFILEHEADER)), 
				DIB_RGB_COLORS, SRCCOPY);
}

void CImageFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	char msg[128];
	
	sprintf(msg, "Frame%d", this->m_index & ~END_OF_SEQUENCE);
	this->SetWindowText(msg);
	this->ShowImage(&dc);
}

BOOL CImageFrame::Create(CWnd* pParentWnd)
{
	return CFrameWnd::Create(NULL, NULL, 
		WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 
		rectDefault, pParentWnd);
}


