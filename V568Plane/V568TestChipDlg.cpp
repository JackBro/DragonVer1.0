// V568UsbcDlg.cpp : 实现文件
//
#include "internal.h"


#ifndef	V578CMD_DISABLE



// CV568TestChipDlg 对话框

IMPLEMENT_DYNAMIC(CV568TestChipDlg, CBaseCtrlDlg)
CV568TestChipDlg::CV568TestChipDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568TestChipDlg::IDD, pParent)
{
	m_pPicStill = NULL;
	m_pPicThumb = NULL;
}

CV568TestChipDlg::~CV568TestChipDlg()
{
	if(m_pPicStill != NULL)
		m_pPicStill->Release();
	if(m_pPicThumb != NULL)
		m_pPicThumb->Release();

	m_pPicStill = NULL;
	m_pPicThumb = NULL;

}

void CV568TestChipDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV568TestChipDlg, CBaseCtrlDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CV568TestChipDlg 消息处理程序

extern "C" extern char gCurDir[];
#define V568_CHIPTEST
#ifdef V568_CHIPTEST

void CV568TestChipDlg::OnBnClickedButtonProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	PTTestChipContext pChipCnt;
	char strtime[100];
	CWnd* pWnd;

	SYSTEMTIME oldtime, time;
	int minute, sec, msec;
	GetSystemTime(&oldtime);

	pChipCnt = TestChipGetContext();
	this->ChangeYuv422ToBmp1("thumb.bin", pChipCnt->aProty.thumbSize.cx, pChipCnt->aProty.thumbSize.cy);
	this->GetStillInterface1("still.jpg");
	this->GetThumbInterface1("thumb.bin");
	this->Invalidate();

	this->ChangeYuv422ToBmp1("decresult.bin", pChipCnt->aProty.dispSize.cx, pChipCnt->aProty.dispSize.cy);

	GetSystemTime(&time);
	minute = (time.wMinute + 60 - oldtime.wMinute) % 60;
	sec = (time.wSecond + 60 - oldtime.wSecond) % 60;
	msec = (time.wMilliseconds + 1000 - oldtime.wMilliseconds) % 1000;
	sec += minute;

	pWnd = this->GetDlgItem(IDC_STATIC_TIME);
	sprintf(strtime, "Time: sec : msec %d : %d", sec, msec);
	pWnd->SetWindowText(strtime);

	this->CompDecFile1();

//	SendCmd("rm /flash/still.jpg\n");
//	SendCmd("rm /flash/thumb.bin\n");
//	SendCmd("rm /flash/decresult.bin\n");
}

#else

void CV568TestChipDlg::OnBnClickedButtonProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	PTTestChipContext pChipCnt;
	char strtime[100];
	CWnd* pWnd;

	SYSTEMTIME oldtime, time;
	int minute, sec, msec;
	GetSystemTime(&oldtime);

	TestChipOpen();
	pChipCnt = TestChipGetContext();
	
	TestChipSetMode(CAPTURESTILLTHUMB_MODE);
	V5B_JpegDisAbrc();
	TestChipSetCapture();
	TestChipCapture();
	ChangeYuv422ToBmp(pChipCnt->ThumbFileName, pChipCnt->aProty.thumbSize.cx, pChipCnt->aProty.thumbSize.cy);
	GetStillInterface(pChipCnt->StiFileName);
	GetThumbInterface(pChipCnt->ThumbFileName);
	this->Invalidate();
#if 1
	TestChipSetMode(DECODEFRAME_MODE);

	TestChipOverlay();

	TestChipDecodeFrame();
	ChangeYuv422ToBmp(pChipCnt->DecResultFileName, pChipCnt->aProty.dispSize.cx, pChipCnt->aProty.dispSize.cy);

	//TestChipSetMode(PREVIEWBRC_MODE);
//	TestChipSetVGACall();
#else
	TestChipSetMode(DISPLAYSTILL_MODE);
	TestChipOverlay();
	TestChipDisplay();
#endif

	GetSystemTime(&time);
	minute = (time.wMinute + 60 - oldtime.wMinute) % 60;
	sec = (time.wSecond + 60 - oldtime.wSecond) % 60;
	msec = (time.wMilliseconds + 1000 - oldtime.wMilliseconds) % 1000;
	sec += minute;

	pWnd = this->GetDlgItem(IDC_STATIC_TIME);
	sprintf(strtime, "Time: sec : msec %d : %d", sec, msec);
	pWnd->SetWindowText(strtime);

#if 0
	CompDecFile();
#endif
}

#endif

void CV568TestChipDlg::CompDecFile()
{
	char str[100];
	char strdst[100];
	PTTestChipContext pChipCnt;
	CGeneralTools tools;
	
	pChipCnt = TestChipGetContext();
	sprintf(str, "%s%s", gCurDir, pChipCnt->DecResultFileName);
	sprintf(strdst, "%s%s", gCurDir, pChipCnt->DecCmpFileName);
	strcat(str, ".bmp");


	if( (strlen(str) == 0) || (strlen(strdst) == 0) )
	{
		::AfxMessageBox("Decode no data");
		return;
	}
	tools.Compare(str, strdst);

}

void CV568TestChipDlg::CompDecFile1()
{
	char str[100];
	char strdst[100];
	CGeneralTools tools;
	
	sprintf(str, "%s/flash/decresult.bin.bmp", gCurDir);
	sprintf(strdst, "%s/flash/golden.bmp", gCurDir);

	if( (strlen(str) == 0) || (strlen(strdst) == 0) )
	{
		::AfxMessageBox("Decode no data");
		return;
	}
	tools.Compare(str, strdst);

}

void CV568TestChipDlg::ChangeYuv422ToBmp(char* filename, int cx, int cy)
{
	CGeneralTools tools;
	char dstfile[100] = "";
	char thumb[100] = "";
	char thumb1[100] = "";
	int srcfmt, desfmt;

	sprintf(dstfile, "%s%s", gCurDir, filename);

	strcpy(thumb, dstfile);

	strcpy(thumb1, dstfile);
	strcat(thumb1, ".rgb");

	srcfmt = YUV422_UYVY;
	desfmt = RGB24;
	tools.DataConvert(thumb, thumb1, cx, cy, srcfmt, desfmt, 1, 0);

	strcpy(thumb, dstfile);
	strcat(thumb, ".bmp");

	srcfmt = RGB24;
	desfmt = BMP;
	tools.DataConvert(thumb1, thumb, cx, cy, srcfmt, desfmt, 1, 0);
}

void CV568TestChipDlg::ChangeYuv422ToBmp1(char* filename, int cx, int cy)
{
	CGeneralTools tools;
	CBufferBase pool;
	char dstfile[100] = "";
	char thumb[100] = "";
	char thumb1[100] = "";
	int srcfmt, desfmt;

	pool.SetSize(0x40000);
	sprintf(dstfile, "%s/flash/%s", gCurDir, filename);
	ArmFile2Pc(filename, dstfile);

	strcpy(thumb, dstfile);

	strcpy(thumb1, dstfile);
	strcat(thumb1, ".rgb");

	srcfmt = YUV422_UYVY;
	desfmt = RGB24;
	tools.DataConvert(thumb, thumb1, cx, cy, srcfmt, desfmt, 1, 0);

	strcpy(thumb, dstfile);
	strcat(thumb, ".bmp");

	srcfmt = RGB24;
	desfmt = BMP;
	tools.DataConvert(thumb1, thumb, cx, cy, srcfmt, desfmt, 1, 0);
}

void CV568TestChipDlg::PaintStill(CDC* pDC)
{
	if(m_pPicStill != NULL)
	{
		OLE_XSIZE_HIMETRIC hmWidth; 
		OLE_YSIZE_HIMETRIC hmHeight; 
		RECT rect, rect1;
		DWORD x, y, cx, cy;
		CWnd* pWnd;

		m_pPicStill->get_Width(&hmWidth); 
		m_pPicStill->get_Height(&hmHeight); 
		pWnd = this->GetDlgItem(IDC_JPEG);
		pWnd->GetWindowRect(&rect);
		this->GetWindowRect(&rect1);
		x = rect.left - rect1.left;
		y = rect.top - rect1.top;
		cx = (UINT16)(rect.right - rect.left);
		cy = (UINT16)(rect.bottom - rect.top);

		m_pPicStill->Render(*pDC, x, y, cx, cy, 0, hmHeight, hmWidth, -hmHeight, NULL);
	}
}

void CV568TestChipDlg::PaintThumb(CDC* pDC)
{
	if(m_pPicThumb != NULL)
	{
		OLE_XSIZE_HIMETRIC hmWidth; 
		OLE_YSIZE_HIMETRIC hmHeight; 
		RECT rect, rect1;
		DWORD x, y, cx, cy;
		CWnd* pWnd;

		m_pPicThumb->get_Width(&hmWidth); 
		m_pPicThumb->get_Height(&hmHeight); 
		pWnd = this->GetDlgItem(IDC_THUMB);
		pWnd->GetWindowRect(&rect);
		this->GetWindowRect(&rect1);
		x = rect.left - rect1.left;
		y = rect.top - rect1.top;
		cx = (UINT16)(rect.right - rect.left);
		cy = (UINT16)(rect.bottom - rect.top);

		m_pPicThumb->Render(*pDC, x, y, cx, cy, 0, hmHeight, hmWidth, -hmHeight, NULL);
	}
}

HRESULT CV568TestChipDlg::GetPicInterface(char* filename, VOID** pIntf)
{
	HRESULT hres;
	IStream *pStm = NULL; 
	CFileStatus fstatus; 
	CFile file; 
	LONG cb;

	if (file.Open(filename, CFile::modeRead)&&file.GetStatus(filename,fstatus)&& ((cb = (LONG)fstatus.m_size) != -1)) 
	{ 
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb); 
		LPVOID pvData = NULL; 
		if (hGlobal != NULL) 
		{ 
			if ((pvData = GlobalLock(hGlobal)) != NULL) 
			{ 
				file.Read(pvData, cb); 
				GlobalUnlock(hGlobal); 
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 
			} 
		} 
	}
	if(pStm == NULL)
		return -1;

	hres = OleLoadPicture(pStm, (LONG)fstatus.m_size, TRUE, IID_IPicture, pIntf);
	
	pStm ->Release();
	return hres;
}

void CV568TestChipDlg::GetStillInterface(char* filename)
{
	char dstfile[100] = "";

	sprintf(dstfile, "%s%s", gCurDir, filename);

	if(m_pPicStill != NULL)
	{
		m_pPicStill->Release();
		m_pPicStill = NULL;
	}

	if(FAILED( GetPicInterface(dstfile, (VOID**)&m_pPicStill) ) )
	{
		m_pPicStill = NULL;
		AfxMessageBox("Load pic from stream error！"); 
	}
}

void CV568TestChipDlg::GetStillInterface1(char* filename)
{
	char dstfile[100] = "";

	sprintf(dstfile, "%s/flash/%s", gCurDir, filename);
	ArmFile2Pc(filename, dstfile);

	if(m_pPicStill != NULL)
	{
		m_pPicStill->Release();
		m_pPicStill = NULL;
	}

	if(FAILED( GetPicInterface(dstfile, (VOID**)&m_pPicStill) ) )
	{
		m_pPicStill = NULL;
		AfxMessageBox("Load pic from stream error！"); 
	}
}

void CV568TestChipDlg::GetThumbInterface(char* filename)
{
	char dstfile[100] = "";

	sprintf(dstfile, "%s%s", gCurDir, filename);
	strcat(dstfile, ".bmp");

	if(m_pPicThumb != NULL)
	{
		m_pPicThumb->Release();
		m_pPicThumb = NULL;
	}

	if(FAILED( GetPicInterface(dstfile, (VOID**)&m_pPicThumb) ) )
	{
		m_pPicThumb = NULL;
		AfxMessageBox("Load pic from stream error！"); 
	}
}

void CV568TestChipDlg::GetThumbInterface1(char* filename)
{
	char dstfile[100] = "";

	sprintf(dstfile, "%s/flash/%s.bmp", gCurDir, filename);
	if(m_pPicThumb != NULL)
	{
		m_pPicThumb->Release();
		m_pPicThumb = NULL;
	}

	if(FAILED( GetPicInterface(dstfile, (VOID**)&m_pPicThumb) ) )
	{
		m_pPicThumb = NULL;
		AfxMessageBox("Load pic from stream error！"); 
	}
}

void CV568TestChipDlg::OnPaint()
{
	CPaintDC dc(this);

	PaintStill(&dc);
	PaintThumb(&dc);
}

void CV568TestChipDlg::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码

}

#else
void nullV568TestChip(void)
{
}
#endif //V578CMD_DISABLE


