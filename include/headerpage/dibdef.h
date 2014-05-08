#ifndef _DIBDEF_H_
#define _DIBDEF_H_


#pragma pack(1)

#include "tools/type.h"

/* structures for defining DIBs */
typedef struct tagTBITMAPCOREHEADER {
        UINT32		bcSize;                 /* used to get to color table */
        UINT16		bcWidth;
        UINT16		bcHeight;
        UINT16		bcPlanes;
        UINT16		bcBitCount;
} TBITMAPCOREHEADER, *PTBITMAPCOREHEADER;

typedef struct tagTBITMAPINFOHEADER{
        UINT32		biSize;
        UINT32		biWidth;
        UINT32		biHeight;
        UINT16		biPlanes;
        UINT16		biBitCount;
        UINT32		biCompression;
        UINT32		biSizeImage;
        UINT32		biXPelsPerMeter;
        UINT32		biYPelsPerMeter;
        UINT32		biClrUsed;
        UINT32		biClrImportant;
} TBITMAPINFOHEADER, *PTBITMAPINFOHEADER;

typedef struct tagTRGBTRIPLE {
        UINT8		rgbtBlue;
        UINT8		rgbtGreen;
        UINT8		rgbtRed;
} TRGBTRIPLE, *PTRGBTRIPLE;

typedef struct tagTRGBQUAD {
        UINT8		rgbBlue;
        UINT8		rgbGreen;
        UINT8		rgbRed;
        UINT8		rgbReserved;
} TRGBQUAD, *PTRGBQUAD;

typedef struct tagTBITMAPINFO {
    TBITMAPINFOHEADER    bmiHeader;
    TRGBQUAD             bmiColors[1];
} TBITMAPINFO, *PTBITMAPINFO;

typedef struct tagTBITMAPCOREINFO {
    TBITMAPCOREHEADER    bmciHeader;
    TRGBTRIPLE           bmciColors[1];
} TBITMAPCOREINFO, *PTBITMAPCOREINFO;

typedef struct tagTBITMAPFILEHEADER {
        UINT16    bfType;
        UINT32    bfSize;
        UINT16    bfReserved1;
        UINT16    bfReserved2;
        UINT32    bfOffBits;
} TBITMAPFILEHEADER, *PTBITMAPFILEHEADER;

typedef struct tagTPALETTEENTRY {
    UINT8        peRed;
    UINT8        peGreen;
    UINT8        peBlue;
    UINT8        peFlags;
} TPALETTEENTRY, *PTPALETTEENTRY;

typedef struct tagTLOGPALETTE {
    UINT16        palVersion;
    UINT16        palNumEntries;
    TPALETTEENTRY        palPalEntry[1];
} TLOGPALETTE, *PTLOGPALETTE;


#define TBI_RGB        0
#define TBI_RLE8       1
#define TBI_RLE4       2
#define TBI_BITFIELDS  3

#define TIS_W3_DIB(lpbi)  ((*(UINT32*)(lpbi)) == sizeof(TBITMAPINFOHEADER)) 

// Dib Header Marker - used in writing DIBs to files 
#define TDIB_HEADER_MARKER	((UINT16) ('M' << 8) | 'B') 
/* DIB constants */ 
#define TDIB_W3			0x300 
#define TDIB_W4			0x400 
#define TDIB_W5			0x500 

#pragma pack()


#endif
