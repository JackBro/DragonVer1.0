#include "internal.h"






CDib::CDib()
{
	m_pBitmap = NULL;
	m_pPalette = NULL;
}

CDib::~CDib()
{
	if(m_pPalette != NULL)
		DeleteObject((HPALETTE)m_pPalette);
	if(m_pBitmap != NULL)
		DeleteObject((HBITMAP)m_pBitmap);

	m_pBitmap = NULL;
	m_pPalette = NULL;
}

BOOL CDib::Load(UINT8* dibfilename)
{
	if(! CDibBase::Load(dibfilename) )
		return FALSE;

	CreateDIBPalette(m_pDib);

	if(! CreateDIBBitmap(m_pDib))
		return FALSE;

	return TRUE;
}

BOOL CDib::Load(UINT16 uIDS)
{
	HINSTANCE hInst;
	HRSRC   hRes;   
	HGLOBAL hData;
	LPBYTE lpBuf;
	LPBYTE lpDIB;

	hInst = AfxGetInstanceHandle();
	hRes  = ::FindResource(hInst,  MAKEINTRESOURCE(uIDS), "DAT");
//	hRes  = ::FindResource(hInst,  MAKEINTRESOURCE(uIDS), RT_BITMAP);
	hData = ::LoadResource(hInst, hRes);

	// if resource ok?
	if (hRes == NULL || hData == NULL)
		return FALSE;

	// get resource buffer
	lpBuf = (LPBYTE)::LockResource(hData);
	// is DIB ?
	if (((PTBITMAPFILEHEADER)lpBuf)->bfType != TDIB_HEADER_MARKER/*"BM"*/)
		return FALSE;

	// use this buffer to create CDib
	lpDIB = lpBuf + sizeof(TBITMAPFILEHEADER);
	return Create(lpDIB);

}

BOOL CDib::Create(UINT8* lpbi)
{
	UINT32 size;
	UINT8* hdib;

	if (lpbi == NULL)
		return FALSE;
	
	size = GetDibBlockSize(lpbi);

	hdib = (UINT8*)malloc(size);

     // Check that DIB handle is valid 
    if (hdib == NULL) 
        return FALSE; 
 
	memcpy(hdib, lpbi, size);

	if(m_pDib != NULL)
		free(m_pDib);

	m_pDib = hdib;

	CreateDIBPalette(m_pDib);

	if(! CreateDIBBitmap(m_pDib))
		return FALSE;

	return TRUE;
}

BOOL CDib::CreateDIBPalette(UINT8* lpbi)
{
	PLOGPALETTE pal = (PLOGPALETTE)CDibBase::CreateDibPalette(lpbi);

	if(m_pPalette != NULL)
	{
		free(m_pPalette);
		m_pPalette = NULL;
	}

	if(pal == NULL)
		return FALSE;

	m_pPalette = (UINT8*)CreatePalette(pal);

	if(m_pPalette == NULL)
		return FALSE;
	return TRUE;
}

BOOL CDib::CreateDIBBitmap(UINT8* lpbi)
{
	PBITMAPINFO pBmpSrc;
	HDC hDC, hSrcDC;
    HBITMAP hSourceBitmap, hOldSourceBitmap; 
	UINT8* pSourceBits;

	if(m_pBitmap != NULL)
		free(m_pBitmap);

	pBmpSrc = (PBITMAPINFO)lpbi;

	hDC = GetDC(NULL);
	hSourceBitmap = CreateDIBSection(hDC, pBmpSrc, DIB_RGB_COLORS, (void**)&pSourceBits, NULL, 0);
	hSrcDC = CreateCompatibleDC(hDC);
	
	memcpy(pSourceBits, FindDibBits(lpbi), pBmpSrc->bmiHeader.biSizeImage);

    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSrcDC, hSourceBitmap ); 

	if(pBmpSrc->bmiHeader.biBitCount <= 8 )
        SetDIBColorTable( hSrcDC, 0, 1 << pBmpSrc->bmiHeader.biBitCount, pBmpSrc->bmiColors ); 

    SelectObject( hSrcDC, hOldSourceBitmap ); 
    DeleteDC( hSrcDC ); 
    ReleaseDC( NULL, hDC ); 

    GdiFlush();

	m_pBitmap = (UINT8*)hSourceBitmap;

	if(m_pBitmap == NULL)
		return FALSE;
	return TRUE;

}

BOOL CDib::Display(CDC * pDC, TRect Dst, UINT32 xSrc, UINT32 ySrc, UINT32 Rop)
{
	CDC MemDC;
	HBITMAP pOldBmp;
	HPALETTE pOldPal;

	MemDC.CreateCompatibleDC(pDC);

	pOldBmp = (HBITMAP)SelectObject(pDC->m_hDC, (HBITMAP)m_pBitmap);

	pOldPal = SelectPalette(pDC->m_hDC, (HPALETTE)m_pPalette, TRUE);
    RealizePalette(pDC->m_hDC);

	BOOL bSuccess = pDC->BitBlt( Dst.ltp.x, Dst.ltp.y, Dst.size.cx, Dst.size.cy, &MemDC, xSrc, ySrc, Rop);

	SelectObject(pDC->m_hDC, pOldBmp);

	SelectPalette(pDC->m_hDC, pOldPal, TRUE);

	return bSuccess;
}

BOOL CDib::Display(CDC * pDC, TRect Dst, TRect Src, UINT32 Rop)
{
	CDC MemDC;
	HBITMAP pOldBmp;
	HPALETTE pOldPal;

	MemDC.CreateCompatibleDC(pDC);

	pOldBmp = (HBITMAP)SelectObject(pDC->m_hDC, (HBITMAP)m_pBitmap);

	pOldPal = SelectPalette(pDC->m_hDC, (HPALETTE)m_pPalette, TRUE);
    RealizePalette(pDC->m_hDC);

	BOOL bSuccess = pDC->StretchBlt( Dst.ltp.x, Dst.ltp.y, Dst.size.cx, Dst.size.cy, &MemDC, Src.ltp.x, Src.ltp.y, Src.size.cx, Src.size.cy, Rop);

	SelectObject(pDC->m_hDC, pOldBmp);

	SelectPalette(pDC->m_hDC, pOldPal, TRUE);

	return bSuccess;
}

BOOL CDib::Display(CDC* pDC, UINT32 x, UINT32 y, UINT32 Rop)
{
	CDC MemDC;
	HBITMAP pOldBmp;
	HPALETTE pOldPal;
	UINT32 cx, cy;

	cx = GetDibWidth(m_pDib);
	cy = GetDibHeight(m_pDib);

	MemDC.CreateCompatibleDC(pDC);

	pOldBmp = (HBITMAP)SelectObject(pDC->m_hDC, (HBITMAP)m_pBitmap);

	pOldPal = SelectPalette(pDC->m_hDC, (HPALETTE)m_pPalette, TRUE);
    RealizePalette(pDC->m_hDC);

	BOOL bSuccess = pDC->BitBlt( x, y, cx, cy, &MemDC, 0, 0, Rop);

	SelectObject(pDC->m_hDC, pOldBmp);

	SelectPalette(pDC->m_hDC, pOldPal, TRUE);

	return bSuccess;
}

BOOL CDib::IsEmpty(void)
{
	if(m_pDib == NULL)
		return TRUE;
	else
		return FALSE;
}

TSize CDib::GetDibSize(void)
{
	TSize size;

	size.cx = (UINT16)GetDibWidth(m_pDib);
	size.cy = (UINT16)GetDibHeight(m_pDib);

	return size;
}

BOOL CDib::StretchDisplay(CDC* pDC, TRect Dst, UINT32 Rop)
{
	CDC MemDC;
	HBITMAP pOldBmp;
	HPALETTE pOldPal;
	BOOL bSuccess;

	MemDC.CreateCompatibleDC(pDC);

	pOldBmp = (HBITMAP)SelectObject(MemDC.m_hDC, (HBITMAP)m_pBitmap);

	pOldPal = SelectPalette(pDC->m_hDC, (HPALETTE)m_pPalette, TRUE);
    RealizePalette(pDC->m_hDC);



	bSuccess = pDC->StretchBlt( Dst.ltp.x, Dst.ltp.y, Dst.size.cx, Dst.size.cy, &MemDC, 0, 0, GetDibWidth(m_pDib), GetDibHeight(m_pDib), Rop);

	SelectObject(pDC->m_hDC, pOldBmp);

	SelectPalette(pDC->m_hDC, pOldPal, TRUE);

	return bSuccess;
}



//
//BOOL CDib::Attach(HANDLE hdib)
//{
//	return TRUE;
//}
//
//HANDLE CDib::Detach()
//{
//	return NULL;
//}
//
