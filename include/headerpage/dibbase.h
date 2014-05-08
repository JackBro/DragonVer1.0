#ifndef _DIBBASE_H_
#define _DIBBASE_H_



#ifndef HEADERPAGE_DLL
	#define HEADERPAGE_DLL __declspec(dllimport)
#endif

#include "tools/type.h"

/////////////////////////////////////////////////////////////////////////////

//   CDibBase	class

class HEADERPAGE_DLL CDibBase
{
public:
// Construction
	CDibBase();

	virtual ~CDibBase();

	virtual BOOL Load(UINT8* filename);
	PTLOGPALETTE CreateDibPalette(UINT8* lpbi);

	UINT16 GetPaletteSize(UINT8* lpbi);
	UINT16 GetDibVersion(UINT8* lpbi);
	UINT16 GetDibClrNum(UINT8* lpbi);
	UINT32 GetBytesPerLine(UINT8* lpbi);
	UINT8* FindDibBits(UINT8* lpbi);

	UINT32 GetDibWidth(UINT8* lpbi);
	UINT32 GetDibHeight(UINT8* lpbi);

	UINT32 GetDibBlockSize(UINT8* lpbi);

// public member data
public:

	UINT8* m_pDib;
};


#endif
