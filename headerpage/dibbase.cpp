#include "internal.h"


CDibBase::CDibBase()
{
	m_pDib = NULL;
}

CDibBase::~CDibBase()
{
	if(m_pDib != NULL)
		free(m_pDib);

	m_pDib = NULL;
}

BOOL CDibBase::Load(UINT8* dibfilename)
{
	CFileBin fb;
	CWaitCursor cursor;
	TBITMAPFILEHEADER bmfh;
	UINT32 size, cnt;
	PTBITMAPINFOHEADER pbmih;

	fb.Open((char*)dibfilename);
	size = fb.GetFileLength();
	if(size == 0)
		return FALSE;

	fb.Read((char*)&bmfh, sizeof(TBITMAPFILEHEADER));

	if(bmfh.bfType != TDIB_HEADER_MARKER)
		return FALSE;

	if(bmfh.bfSize != size)
		bmfh.bfSize = size;

	size = bmfh.bfSize - sizeof(TBITMAPFILEHEADER);

	pbmih = (PTBITMAPINFOHEADER)malloc(size);

	cnt = fb.Read((char*)pbmih, size);
	if((cnt != size) && (pbmih->biSize < sizeof(TBITMAPINFOHEADER)))
	{
		free((void*)pbmih);
		return FALSE;
	}

	if(pbmih->biBitCount != 24)
	{
		if(pbmih->biClrUsed == 0)
		{
			pbmih->biClrUsed = 1<<pbmih->biBitCount;
		}
	}

	if(pbmih->biSizeImage == 0)
	{
		pbmih->biSizeImage = (((pbmih->biWidth * pbmih->biBitCount + 31) >> 5) <<2) * pbmih->biHeight;
	}

	if(m_pDib != NULL)
		free(m_pDib);

	m_pDib = (UINT8*)pbmih;

	return TRUE;
}

PTLOGPALETTE CDibBase::CreateDibPalette(UINT8* lpbi) 
{
	UINT32 clr, i;
	PTLOGPALETTE palette = NULL;

	PTBITMAPINFOHEADER pbmih;
	PTBITMAPCOREHEADER pbmch;
	PTBITMAPINFO bmi;
	PTBITMAPCOREINFO bmci;

	if(lpbi == NULL)
		return NULL;

	pbmih = (PTBITMAPINFOHEADER)lpbi;
	pbmch = (PTBITMAPCOREHEADER)lpbi;
	bmi = (PTBITMAPINFO)lpbi;
	bmci = (PTBITMAPCOREINFO)lpbi;

	clr = GetDibClrNum(lpbi);

	if(clr)
	{
		palette = (PTLOGPALETTE)malloc(sizeof(TLOGPALETTE) + sizeof(TPALETTEENTRY) * clr);

		palette->palNumEntries = (UINT16)clr;
		palette->palVersion = TDIB_W3;

		for (i = 0; i < clr; i++) 
		{
			if(GetDibVersion(lpbi) == TDIB_W3)
			{
				palette->palPalEntry[i].peRed = bmi->bmiColors[i].rgbRed;
				palette->palPalEntry[i].peGreen = bmi->bmiColors[i].rgbGreen;
				palette->palPalEntry[i].peBlue = bmi->bmiColors[i].rgbBlue;
				palette->palPalEntry[i].peFlags = 0;
			}
			else
			{
				palette->palPalEntry[i].peRed = bmci->bmciColors[i].rgbtRed;
				palette->palPalEntry[i].peGreen = bmci->bmciColors[i].rgbtGreen;
				palette->palPalEntry[i].peBlue = bmci->bmciColors[i].rgbtBlue;
				palette->palPalEntry[i].peFlags = 0;
			}
		}

	}

	return palette;
}

UINT16 CDibBase::GetPaletteSize(UINT8* lpbi)
{
	if(GetDibVersion(lpbi) == TDIB_W3)
	{
		return (GetDibClrNum(lpbi)  * sizeof(TRGBQUAD));
	}
	else
	{
		return (GetDibClrNum(lpbi)  * sizeof(RGBTRIPLE));
	}
}

UINT16 CDibBase:: GetDibVersion(UINT8* lpbi)
{
	if((*(UINT32*)(lpbi)) == sizeof(TBITMAPINFOHEADER))
		return TDIB_W3;
	else
		return TDIB_W4;
}

UINT16 CDibBase:: GetDibClrNum(UINT8* lpbi)
{
	UINT16 clr;
	UINT16 bitcnt;
	UINT16 ver;

	PTBITMAPINFOHEADER pbmih;
	PTBITMAPCOREHEADER pbmch;

	if(lpbi == NULL)
		return NULL;

	pbmih = (PTBITMAPINFOHEADER)lpbi;
	pbmch = (PTBITMAPCOREHEADER)lpbi;

	ver = GetDibVersion(lpbi);

	if((ver == TDIB_W3) && (pbmih->biClrUsed))
	{
		clr = (UINT16)pbmih->biClrUsed;
	}
	else
	{
		if(ver == TDIB_W3)
			bitcnt = pbmih->biBitCount;
		else
			bitcnt = pbmch->bcBitCount;

		switch(bitcnt)
		{
		case 1:
			clr = 2;
			break;
		case 4:
			clr = 16;
			break;
		case 8:
			clr = 256;
			break;
		default:
			clr = 0;
		}
	}

	return clr;

}

UINT32 CDibBase:: GetBytesPerLine(UINT8* lpbi)
{
	PTBITMAPINFOHEADER pbmih;

	pbmih = (PTBITMAPINFOHEADER)lpbi;

	return ((pbmih->biWidth * pbmih->biPlanes * pbmih->biBitCount + 31) >> 5) <<2;
}

UINT8* CDibBase:: FindDibBits(UINT8* lpbi)
{
   return (lpbi + *(LPDWORD)lpbi + GetPaletteSize(lpbi)); 
}

UINT32 CDibBase:: GetDibWidth(UINT8* lpbi)
{
    PTBITMAPINFOHEADER   lpbmi;
    PTBITMAPCOREHEADER   lpbmc;
 
    lpbmi = (PTBITMAPINFOHEADER)lpbi; 
    lpbmc = (PTBITMAPCOREHEADER)lpbi; 
 
	if(GetDibVersion(lpbi) == TDIB_W3)
        return lpbmi->biWidth; 
    else  // it is an OS/2 DIB, so return its width 
        return (UINT32)lpbmc->bcWidth; 
}

UINT32 CDibBase:: GetDibHeight(UINT8* lpbi)
{
    PTBITMAPINFOHEADER   lpbmi;
    PTBITMAPCOREHEADER   lpbmc;
 
    lpbmi = (PTBITMAPINFOHEADER)lpbi; 
    lpbmc = (PTBITMAPCOREHEADER)lpbi; 
 
	if(GetDibVersion(lpbi) == TDIB_W3)
        return lpbmi->biHeight; 
    else  // it is an OS/2 DIB, so return its width 
        return (UINT32)lpbmc->bcHeight; 
}

UINT32 CDibBase:: GetDibBlockSize(UINT8* lpbi)
{
	if (((PTBITMAPINFOHEADER)lpbi)->biSizeImage == 0)
		((PTBITMAPINFOHEADER)lpbi)->biSizeImage = GetBytesPerLine(lpbi) * ((PTBITMAPINFOHEADER)lpbi)->biHeight;
	return ((PTBITMAPINFOHEADER)lpbi)->biSize + GetBytesPerLine(lpbi) + ((PTBITMAPINFOHEADER)lpbi)->biSizeImage;
}
