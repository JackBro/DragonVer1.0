#ifndef _DIB_H_
#define _DIB_H_



#ifndef HEADERPAGE_DLL
	#define HEADERPAGE_DLL __declspec(dllimport)
#endif

#include "dibbase.h"
#include "tools/type.h"

//   Dib	class
class HEADERPAGE_DLL CDib : public CDibBase
{
public:
// Construction
	CDib();

	virtual ~CDib();


	BOOL Load(UINT8* dibfilename);
	BOOL Load(UINT16 uIDS);

	BOOL Create(UINT8* lpbi);

	BOOL CreateDIBPalette(UINT8* lpbi);
	BOOL CreateDIBBitmap(UINT8* lpbi);

	BOOL Display(CDC * pDC, TRect Dst, UINT32 xSrc, UINT32 ySrc, UINT32 Rop);
	BOOL Display(CDC * pDC, TRect Dst, TRect Src, UINT32 Rop);
    BOOL Display(CDC* pDC, UINT32 x, UINT32 y, UINT32 Rop);

    BOOL StretchDisplay(CDC* pDC, TRect Dst, UINT32 Rop);

	TSize GetDibSize(void);

	BOOL IsEmpty(void);
// public member data
public:

	UINT8* m_pBitmap;
	UINT8* m_pPalette;

};


#endif
