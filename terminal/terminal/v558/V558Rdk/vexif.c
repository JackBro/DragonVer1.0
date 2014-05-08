/*************************************************************************/
/*									 */
/* The last modification date:  02-23-2004                               */
/* REMARKS:  Created initial version 1.0                                 */
/*                                                                       */
/*                Copyright (C) 2004 Vimicro CO.,LTD     		 */
/*************************************************************************/
//#define JPEG_INTERNALS
//#include "jinclude.h"
//#include "jpeglib.h"
#include "v558api.h"
#include "vexif.h"	
#include "string.h"	

//   --------------------------------------------------------------------------

#define IFD0thOFS    0x08
#define IFD0thNEXT   0x9A
#define IFD1thOFS    0x03E6
#define IFDexifOFS   0x0166
#define IFDexifNEXT  0x02E8
#define IFDinteOFS   0x03C8
#define IFDinteNEXT  0x03E2
#define IFD1thOFS    0x03E6
#define IFD1thNEXT   0x043C
#define THUMBNAILOFS 0x0450
/////////////////////////
//***********

#define dim(x) (sizeof(x) / sizeof(x[0]))   
IFDData defIFD0th [] = {
	{ 0x000A, EXIFTAG_IMAGEDESCRIPTION,        EXIF_ASCII,     0x14, 0x9E,                  "Vimicro Exif", 0 ,0, 0 ,0},
	{ 0x0016, EXIFTAG_MAKE,                    EXIF_ASCII,     0x20, 0xB2,                  "Vimicro Corporation", 0,0,0, 0},
	{ 0x0022, EXIFTAG_MODEL,                   EXIF_ASCII,     0x10, 0xD2,                  "VC0558", 0,0,0, 0},
	{ 0x002E, EXIFTAG_ORIENTATION,             EXIF_SHORT,     0x01, ORIENTATION_TOPLEFT,   0, 0, 0},
	{ 0x003A, EXIFTAG_XRESOLUTION,             EXIF_RATIONAL,  0x01, 0xE2,                  0, 0x00000001, 0x00000048,0, 0},
	{ 0x0046, EXIFTAG_YRESOLUTION,             EXIF_RATIONAL,  0x01, 0xEA,                  0, 0x00000001, 0x00000048,0, 0},
	{ 0x0052, EXIFTAG_RESOLUTIONUNIT,          EXIF_SHORT,     0x01, RESUNIT_INCH,          0, 0,0,0, 0},
	{ 0x005E, EXIFTAG_SOFTWARE,                EXIF_ASCII,     0x20, 0xF2,                  "Vimicro VC0558 Driver", 0,0,0, 0},
	{ 0x006A, EXIFTAG_DATETIME,                EXIF_ASCII,     0x14, 0x0112,                "YYYY:MM:DD HH:MM:SS", 0,0,0, 0},
	{ 0x0076, EXIFTAG_YCBCRPOSITIONING,        EXIF_SHORT,     0x01, YCBCRPOSITION_COSITED, 0, 0,0,0, 0},
	{ 0x0082, EXIFTAG_COPYRIGHT,               EXIF_ASCII,     0x40, 0x0126,                "Vimicro Corporation Copyright (C) 2004-2008", 0,0,0, 0},
	{ 0x008E, EXIFTAG_EXIFIFDPOINTER,          EXIF_LONG,      0x01, 0x0166,                0, 0,0,0, 0},
};

IFDData defIFDexif [] = {
	{ 0x0168, EXIFTAG_EXPOSURETIME,            EXIF_RATIONAL,  0x01, 0x02EC,                0, 0x00000001,0x0000003C,0, 0},
	{ 0x0174, EXIFTAG_FNUMBER,                 EXIF_RATIONAL,  0x01, 0x02F4,                0, 0x00000001,0x00000004,0, 0},
	{ 0x0180, EXIFTAG_EXPOSUREPROGRAM,         EXIF_SHORT,     0x01, 0x02,                  0, 0,0,0, 0},
	{ 0x018C, EXIFTAG_ISOSPEEDRATINGS,         EXIF_SHORT,     0x01, 0x7D,                  0, 0,0,0, 0},
	{ 0x0198, EXIFTAG_EXIFVERSION,             EXIF_UNDEFINED, 0x04, 0x30313230,            0, 0,0,0, 0}, // "0210"
	{ 0x01A4, EXIFTAG_DATETIMEORIGINAL,        EXIF_ASCII,     0x14, 0x02FC,                "YYYY:MM:DD HH:MM:SS", 0,0,0, 0},
	{ 0x01B0, EXIFTAG_DALETIMEDIGITIZED,       EXIF_ASCII,     0x14, 0x0310,                "YYYY:MM:DD HH:MM:SS", 0,0,0, 0},
	{ 0x01BC, EXIFTAG_COMPONENTSCONFIGURATION, EXIF_UNDEFINED, 0x04, 0x00030201,            0, 0,0,0, 0},
	{ 0x01C8, EXIFTAG_COMPRESSEDBITSPERPIXEL,  EXIF_RATIONAL,  0x01, 0x0324,                0, 0x00000001,0x00000002,0, 0},
	{ 0x01D4, EXIFTAG_SHUTTERSPEEDVALUE,       EXIF_SRATIONAL, 0x01, 0x032C,                0, 0, 0,0x00000001,0x00000006},
	{ 0x01E0, EXIFTAG_APERTUREVALUE,           EXIF_RATIONAL,  0x01, 0x0334,                0, 0x00000001,0x00000004,0, 0},
	{ 0x01EC, EXIFTAG_BRIGHTNESSVALUE,         EXIF_SRATIONAL, 0x01, 0x033C,                0, 0,0, 0x00000001,0x00000000},
	{ 0x01F8, EXIFTAG_EXPOSUREBIASVALUE,       EXIF_SRATIONAL, 0x01, 0x0344,                0, 0,0, 0x00000001,0x00000000},
	{ 0x0204, EXIFTAG_MAXAPERTUREVALUE,        EXIF_RATIONAL,  0x01, 0x034C,                0, 0x00000001,0x00000001, 0,0},
	{ 0x0210, EXIFTAG_SUBJECTDISTANCE,         EXIF_RATIONAL,  0x01, 0x0354,                0, 0x0000000A,0x0000000F, 0,0},
	{ 0x021C, EXIFTAG_METERINGMODE,            EXIF_SHORT,     0x01, 0x01,                  0, 0, 0,0,0},
	{ 0x0228, EXIFTAG_LIGHTSOURCE,             EXIF_SHORT,     0x01, 0x01,                  0, 0, 0,0,0},
	{ 0x0234, EXIFTAG_FLASH,                   EXIF_SHORT,     0x01, 0x00,                  0, 0, 0,0,0},
	{ 0x0240, EXIFTAG_FOCALLENGTH,             EXIF_RATIONAL,  0x01, 0x035C,                0, 0x0000000A,0x0000000F, 0,0},
	{ 0x024C, EXIFTAG_MAKERNOTE,               EXIF_UNDEFINED, 0x3C, 0x0364,                "Vimicro Imaging Format 1.0a", 0,0,0, 0},
	{ 0x0258, EXIFTAG_USERCOMMENT,             EXIF_UNDEFINED, 0x20, 0x03A0,                0, 0x00000000,0x0053494A, 0,0},
	{ 0x0264, EXIFTAG_SUBSECTIME,              EXIF_ASCII,     0x04, 0x00,                  0, 0,0,0, 0},
	{ 0x0270, EXIFTAG_SUBSECTIMEORIGINAL,      EXIF_ASCII,     0x04, 0x00,                  0, 0, 0,0,0},
	{ 0x027C, EXIFTAG_SUBSECTIMEDIGITIZED,     EXIF_ASCII,     0x04, 0x00,                  0, 0, 0,0,0},
	{ 0x0288, EXIFTAG_FLASHPIXVERSION,         EXIF_UNDEFINED, 0x04, 0x30303130,            0, 0, 0,0,0},
	{ 0x0294, EXIFTAG_COLORSPACE,              EXIF_SHORT,     0x01, 0x01,                  0, 0, 0,0,0},
	{ 0x02A0, EXIFTAG_PIXELXDIMENSION,         EXIF_LONG,      0x01, 0x0280,                0, 0, 0,0,0}, //640
	{ 0x02AC, EXIFTAG_PIXELYDIMENSION,         EXIF_LONG,      0x01, 0x01E0,                0, 0, 0,0,0}, //480
	{ 0x02B8, EXIFTAG_INTEROPERABILITYIFDPTR,  EXIF_LONG,      0x01, 0x03C8,                0, 0, 0,0,0},
	{ 0x02C4, EXIFTAG_FILESOURCE,              EXIF_UNDEFINED, 0x01, 0x03,                  0, 0, 0,0,0},
	{ 0x02D0, EXIFTAG_SCENETYPE,               EXIF_UNDEFINED, 0x01, 0x01,                  0, 0, 0,0,0},
	{ 0x02DC, EXIFTAG_CFAPATTERN,              EXIF_UNDEFINED, 0x08, 0x03C0,                0, 0x01020001,0x02000200, 0,0},
};

IFDData defIFDInteroperability [] = {
	{ 0x03CA, EXIFTAG_INTEROPERABILITYINDEX,   EXIF_ASCII,     0x04, 0x00383952,            0, 0,0,0, 0},
	{ 0x03D6, EXIFTAG_INTEROPERABILITYVERSION, EXIF_UNDEFINED, 0x04, 0x30303130,            0, 0,0,0, 0},
};

IFDData defIFD1th [] = {
	{ 0x03E8, EXIFTAG_COMPRESSION,             EXIF_SHORT,     0x01, 0x06,                  0, 0, 0},
	{ 0x03F4, EXIFTAG_XRESOLUTION,             EXIF_RATIONAL,  0x01, 0x0440,                0, 0x00000001,0x00000048, 0,0},
	{ 0x0400, EXIFTAG_YRESOLUTION,             EXIF_RATIONAL,  0x01, 0x0448,                0, 0x00000001,0x00000048, 0,0},
	{ 0x040C, EXIFTAG_RESOLUTIONUNIT,          EXIF_SHORT,     0x01, 0x02,                  0, 0,0,0, 0},
	{ 0x0418, EXIFTAG_JPEGIFOFFSET,            EXIF_LONG,      0x01, 0x0450,                0, 0, 0,0,0}, 
	{ 0x0424, EXIFTAG_JPEGIFBYTECOUNT,         EXIF_LONG,      0x01, 0x10000,               0, 0,0,0, 0}, //default thumbnail len
	{ 0x0430, EXIFTAG_YCBCRPOSITIONING,        EXIF_SHORT,     0x01, 0x02,                  0, 0, 0,0,0},
};


const int TypeLen [] = {
  //0  1  2  3  4  5  6  7  8  9 10 11 12
	0, 1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8,
};

//   --------------------------------------------------------------------------

void put1Bytes(UINT8 *array, UINT32 offset, UINT8 value)
{
	array[offset] = (UINT8) (value);
}

void put2Bytes(UINT8 *array, UINT32 offset, UINT16 value)
{
	array[offset] = (UINT8) ((value) & 0xFF);
	array[offset+1] = (UINT8) (((value) >> 8) & 0xFF);
}

void put4Bytes(UINT8*array, UINT32 offset, UINT32 value)
{
	array[offset] = (UINT8) ((value) & 0xFF);
	array[offset+1] = (UINT8) (((value) >> 8) & 0xFF);
	array[offset+2] = (UINT8) (((value) >> 16) & 0xFF);
	array[offset+3] = (UINT8) (((value) >> 24) & 0xFF); 
}
UINT16  Get2Bytes(UINT8 *pt, UINT8 IsBigEndian)
{
	UINT16 value;
	if(IsBigEndian)
		value = (*pt<<8) + *(pt+1);
	else 
		value = (*(pt+1)<<8) + *pt;
	return value;
}
UINT32  Get4Bytes(UINT8 *pt, UINT8 IsBigEndian)
{
	UINT32 value;
	if(IsBigEndian)
		value = (*pt<<24) + (*(pt+1)<<16) + (*(pt+2)<<8) + *(pt+3);
	else 
		value = *pt + (*(pt+1)<<8) + (*(pt+2)<<16) + (*(pt+3)<<24);
	return value;
}

void GetIFDEntry(UINT8* pt, PIFDEntry entry, UINT8 IsBigEndian)
{
	entry->tag    =  	Get2Bytes(pt,   IsBigEndian);
	entry->type   =   Get2Bytes(pt+2, IsBigEndian);
	entry->count  =   Get4Bytes(pt+4, IsBigEndian);
	entry->offset =   Get4Bytes(pt+8, IsBigEndian);
}

//   --------------------------------------------------------------------------

void createIFDdata(UINT8 * pTiffHeader, PIFDData pIfdData)
{
	UINT32 valuelen;

	put2Bytes((pTiffHeader + pIfdData->offset), 0, pIfdData->entry.tag);
	put2Bytes((pTiffHeader + pIfdData->offset), 2, pIfdData->entry.type);
	put4Bytes((pTiffHeader + pIfdData->offset), 4, pIfdData->entry.count);
	put4Bytes((pTiffHeader + pIfdData->offset), 8, pIfdData->entry.offset);

	valuelen = TypeLen[pIfdData->entry.type] * pIfdData->entry.count;

	if ( valuelen > 4) {
		if (pIfdData->ascii) {
			memcpy((pTiffHeader + pIfdData->entry.offset), pIfdData->ascii, valuelen);
		} else if (pIfdData->rationalH||pIfdData->rationalH) {
		{
			memcpy((pTiffHeader + pIfdData->entry.offset), &pIfdData->rationalL, (valuelen>>2));
			memcpy((pTiffHeader + pIfdData->entry.offset), &pIfdData->rationalH, (valuelen>>2));
		}
		} else if (pIfdData->srationalH||pIfdData->srationalH) {
		{
			memcpy((pTiffHeader + pIfdData->entry.offset), &pIfdData->srationalL, (valuelen>>2));
			memcpy((pTiffHeader + pIfdData->entry.offset), &pIfdData->srationalH, (valuelen>>2));
		}
		}
	}
}

void modifyJPEGIFBYTECOUNT(UINT32 jfiflen)
{
	UINT32 i, interno;
	interno = dim(defIFD1th);

	for (i=0; i<interno; i++) {
		if (defIFD1th[i].entry.tag == EXIFTAG_JPEGIFBYTECOUNT) {
			defIFD1th[i].entry.offset = jfiflen;
			break;
		}
	}
}

UINT32 createExif(UINT8 * pExifHeader, UINT8* pThumbnail, UINT32 ThumbnailLen)
{
	UINT32 i, interno;
	UINT8* pTiff = pExifHeader + EXIF_HEADER_LEN;

	memcpy(pExifHeader, "Exif", EXIF_HEADER_LEN);
	put2Bytes(pExifHeader, 4, 0x00);

	put2Bytes(pTiff, 0, EXIF_LITTLEENDIAN);
	put2Bytes(pTiff, 2, TIFF_VERSION);
	put4Bytes(pTiff, 4, IFD0thOFS);

	interno = dim(defIFD0th);
	put4Bytes(pTiff, IFD0thOFS, interno);
	for (i=0; i<interno; i++) {
		createIFDdata(pTiff, &defIFD0th[i]);
	}
	put4Bytes(pTiff, IFD0thNEXT, IFD1thOFS);

	interno = dim(defIFDexif);
	put4Bytes(pTiff, IFDexifOFS, interno);
	for (i=0; i<interno; i++) {
		createIFDdata(pTiff, &defIFDexif[i]);
	}
	put4Bytes(pTiff, IFDexifNEXT, 0x00);

	interno = dim(defIFDInteroperability);
	put4Bytes(pTiff, IFDinteOFS, interno);
	for (i=0; i<interno; i++) {
		createIFDdata(pTiff, &defIFDInteroperability[i]);
	}
	put4Bytes(pTiff, IFDinteNEXT, 0x00);

	interno = dim(defIFD1th);
	put4Bytes(pTiff, IFD1thOFS, interno);
	modifyJPEGIFBYTECOUNT(ThumbnailLen);
	for (i=0; i<interno; i++) {
		createIFDdata(pTiff, &defIFD1th[i]);
	}
	put4Bytes(pTiff, IFD1thNEXT, 0x00);

	memcpy((pTiff + THUMBNAILOFS), pThumbnail, ThumbnailLen);

	return (EXIF_HEADER_LEN + THUMBNAILOFS + ThumbnailLen);
}

UINT32 getExifinfo(UINT8 * pExifHeader, PV558_EXIFINFO pinfo, UINT8* pThumbnail, UINT32 ThumbnailLen)
{
	UINT32 j=0, interno=0,valuelen;
	UINT8* pt =  pExifHeader;
	IFDEntry  entry;
	j         += 2;
	pinfo->FieldLen  = Get2Bytes(pt+j, 1);
	j         += 2;
	if(Get4Bytes(pt+j, 1) == 0x45786966)
		pinfo->IsThumb = 1;
	else
		pinfo->IsThumb = 0;
	j         += 6;
	if( Get2Bytes(pt+j, 1) == 0x4D4D )
		pinfo->IsBigEndian = 1;   //is big endian
	else if( Get2Bytes(pt+j, 1) == 0x4949 )
		pinfo->IsBigEndian = 0;   //is little endian
	pinfo->IFD0Offset  =  Get4Bytes(pt + j + 4, pinfo->IsBigEndian);
	j         +=    pinfo->IFD0Offset;
	for(;;)
	{
		GetIFDEntry( pt+j, &entry, pinfo->IsBigEndian);
		j +=  sizeof(IFDEntry);
		valuelen = TypeLen[entry.type] * entry.count;
		if( valuelen > 4 )
		if(entry.type == EXIF_ASCII)
			//entry.count 
		interno++;
	}
}


UINT32 writeExifAPP1(UINT8* pMarkerHead, UINT8* pThumbnail,UINT32 ThumbnailLen)
{
	UINT32 exiflen;
	put1Bytes(pMarkerHead, 0, (char)0xFF);
	put1Bytes(pMarkerHead, 1, (char)0xE1);

	exiflen = createExif((pMarkerHead + 4), pThumbnail, ThumbnailLen) + 2;

	put1Bytes(pMarkerHead, 2, (UINT8) (((exiflen) >> 8) & 0xFF));
	put1Bytes(pMarkerHead, 3, (UINT8) ((exiflen) & 0xFF));

	return (exiflen + 2);
}
UINT32 GetExifApp1HeaderLen(void)
{
	return (EXIF_HEADER_LEN + THUMBNAILOFS + 2);
}
UINT32 writeExifHeader(UINT8* pBuffer)
{	
	UINT32  len; 
	put1Bytes(pBuffer, 0, (char)0xFF);
	put1Bytes(pBuffer, 1, (char)0xD8);
	len   =  2;
	return len;
}