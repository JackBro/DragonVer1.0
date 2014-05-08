// PictureBox.cpp : 实现文件
//
#include "internal.h"



IMPLEMENT_DYNAMIC(CPictureBox, CWnd)
CPictureBox::CPictureBox()
{
	this->m_width = 0;
	this->m_height = 0;
	this->Clear();
}

CPictureBox::~CPictureBox()
{
	this->ReleasePic();
}

BEGIN_MESSAGE_MAP(CPictureBox, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPictureBox 消息处理程序
void CPictureBox::ReleasePic(void)
{
	if(this->m_pPic)
		this->m_pPic->Release();
	this->Clear();
}

void CPictureBox::Clear(void)
{
	this->m_pPic = NULL;
}

void CPictureBox::LoadPicture(char *buf, int len)
{
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
	LPVOID pvData = GlobalLock( hGlobal );
	LPSTREAM pStream = NULL;

	this->ReleasePic();
	memcpy(pvData, buf, len);
	GlobalUnlock(hGlobal);
	HRESULT hr = CreateStreamOnHGlobal( hGlobal, TRUE, &pStream );
	if(FAILED(hr))
		return;
	OleLoadPicture(pStream, 0, FALSE, IID_IPicture, (void **)&this->m_pPic);
	pStream->Release();
	this->m_pPic->get_Width(&(this->m_picwidth));
	this->m_pPic->get_Height(&(this->m_picheight));
	this->Invalidate();
}

void CPictureBox::LoadPicture(char *filename)
{
	CFileBin fb;
	CBufferBase pool;
	int len;

	fb.Open(filename);
	len = fb.GetFileLength();
	if(len == 0)
		return;
	pool.SetSize(len);
	fb.Read(pool.m_buf, len);
	this->LoadPicture(pool.m_buf, len);
}

void CPictureBox::RenderPic(HDC dc)
{
	if(this->m_pPic == NULL)
		return;
	this->m_pPic->Render(dc, 0, 0, this->m_width, this->m_height, 
		0, 0, this->m_picwidth, this->m_picheight, NULL);
}

BOOL CPictureBox::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	this->m_width = rect.right - rect.left;// + 1;
	this->m_height = rect.bottom - rect.top;// + 1;
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, pParentWnd, nID, NULL);
}

void CPictureBox::OnPaint()
{
	CPaintDC dc(this);

	this->RenderPic(dc);
}
