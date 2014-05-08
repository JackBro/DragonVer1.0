#include "internal.h"
#include "vjpegtools.h"


#include "internal.h"
#include "vjpegtools.h"

//Jpeg Parse
static UINT16 GetCompFrmSOF(UINT8 *buf);
static UINT16 GetComment(PTJpegIndex pinfo, UINT8 *ptr);
static UINT16 GetSOF0(PTJpegIndex pinfo, UINT8 * ptr);
static UINT16 GetSOF_Unsupport(PTJpegIndex pinfo, UINT8 * ptr);
static UINT16 ByPassMarker(UINT8 * ptr);
static UINT16 GetDQT(PTJpegIndex pinfo, UINT8 * ptr);
static UINT16 GetDHT(PTJpegIndex pinfo, UINT8 * ptr);
static UINT16 GetSOS(PTJpegIndex pinfo, UINT8 * ptr);
static UINT16 GetJfifInfo(UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo);

static UINT16 JfxxPutRGB(UINT8* pdst, PTDataBuf pthumbuf, UINT8 thumbfmt, TSize thumbsize);
static UINT16 AddJfxxInfo(UINT8* pdst, PTDataBuf pthumbuf, UINT8 thumbfmt, TSize thumbsize);
static UINT16 AddJfifInfo(UINT8* pdst);

//Exif Parse
static UINT16 ExifGetWord(UINT8* ptr, UINT8 order);
static UINT32 ExifGetDword(UINT8* ptr, UINT8 order);
static UINT16 GetExifInfo(UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo);
static void ParseExif(UINT8* ptr, PTThumbNail pthumbinfo);
static UINT32 GetIFD0( UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo);
static UINT32 GetIFD1( UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo);


#define GETBYTE(base,offset) ((UINT8)(*((UINT8 *)base+offset)))
#define GETWORD(base,offset) (((UINT16)(*((UINT8 *)base+offset))<< 8) + (UINT16)*((UINT8 *)base+offset+1))
#define GETWORD_INV(base,offset) (((UINT16)(*((UINT8 *)base+offset+1))<< 8) + (UINT16)*((UINT8 *)base+offset))
#define GETDWORD(base,offset) (((UINT32)(*((UINT8 *)base+offset))<< 24) + ((UINT32)(*((UINT8 *)base+offset+1))<<16) + 	\
								((UINT32)(*((UINT8 *)base+offset+2))<< 8) + ((UINT32)(*((UINT8 *)base+offset+3))))
#define GETDWORD_INV(base,offset) (((UINT32)(*((UINT8 *)base+offset+3))<< 24) + ((UINT32)(*((UINT8 *)base+offset+2))<<16) + 	\
								((UINT32)(*((UINT8 *)base+offset+1))<< 8) + ((UINT32)(*((UINT8 *)base+offset))))

#define SETBYTE(base, offset, val)	\
		{												\
			(*((UINT8 *)base+offset)) = (UINT8)val;		\
			offset++;									\
		}

#define SETWORD(base, offset, val)		\
		{								\
			(*((UINT8 *)base+offset)) = (UINT8)((((UINT16)val)>> 8)&0xff);	\
			(*((UINT8 *)base+offset+1)) = (UINT8)val;						\
			offset += 2;													\
		}

#define SETWORD_INV(base, offset, val)		\
		{								\
			(*((UINT8 *)base+offset+1)) = (UINT8)((((UINT16)val)>> 8)&0xff);	\
			(*((UINT8 *)base+offset)) = (UINT8)val;						\
			offset += 2;												\
		}

#define SETDWORD(base, offset, val)		\
		{								\
			(*((UINT8 *)base+offset)) = (UINT8)((((UINT32)val)>> 24)&0xff);	\
			(*((UINT8 *)base+offset+1)) = (UINT8)((((UINT32)val)>> 16)&0xff);	\
			(*((UINT8 *)base+offset+2)) = (UINT8)((((UINT32)val)>> 8)&0xff);	\
			(*((UINT8 *)base+offset+3)) = (UINT8)val;						\
			offset += 4;													\
		}

#define SETDWORD_INV(base, offset, val)		\
		{								\
			(*((UINT8 *)base+offset+3)) = (UINT8)((((UINT32)val)>> 24)&0xff);	\
			(*((UINT8 *)base+offset+2)) = (UINT8)((((UINT32)val)>> 16)&0xff);	\
			(*((UINT8 *)base+offset+1)) = (UINT8)((((UINT32)val)>> 8)&0xff);	\
			(*((UINT8 *)base+offset)) = (UINT8)val;						\
			offset += 4;												\
		}

static void yuv422_convert2points( UINT8 *yuv, UINT8 *pdst )
{
	yuv2rgb(pdst,	pdst+1,	pdst+2,	yuv[0],	yuv[1],	yuv[2]);
	yuv2rgb(pdst+3,	pdst+4,	pdst+5,	yuv[3],	yuv[1],	yuv[2]);
}

static void yuv411_convert4points( UINT8 *yuv, UINT8 *pdst)
{
	yuv2rgb(pdst,	pdst+1,	pdst+2,	yuv[0],	yuv[1],	yuv[2]);
	yuv2rgb(pdst+3,	pdst+4,	pdst+5,	yuv[3],	yuv[1],	yuv[2]);
	yuv2rgb(pdst+6,	pdst+7,	pdst+8,	yuv[4],	yuv[1],	yuv[2]);
	yuv2rgb(pdst+9,	pdst+10,pdst+11,yuv[5],	yuv[1],	yuv[2]);
}

static void yuv420_convert4points( UINT8 *yuv, UINT8 *pdst1, UINT8 *pdst2)
{
	yuv2rgb(pdst1,	 pdst1+1, pdst1+2, yuv[0], yuv[1], yuv[2]);
	yuv2rgb(pdst1+3, pdst1+4, pdst1+5, yuv[3], yuv[1], yuv[2]);
	yuv2rgb(pdst2,	 pdst2+1, pdst2+2, yuv[4], yuv[1], yuv[2]);
	yuv2rgb(pdst2+3, pdst2+4, pdst2+5, yuv[5], yuv[1], yuv[2]);
}

static void yuv400_convert1points( UINT8 *yuv, UINT8 *pdst)
{
	yuv2rgb(pdst, pdst+1, pdst+2, yuv[0], 128, 128);
}

static void yuv444_convert1points( UINT8 *yuv, UINT8 *pdst)
{
	yuv2rgb(pdst, pdst+1, pdst+2, yuv[0], yuv[1], yuv[2]);
}


/*****************************************************************************/
//V558 yuv type


/******************************************************************************
******************************************************************************/
void JpegParse(UINT8 *ptr, UINT32 length, PTJpegIndex pinfo)
{
	UINT32 i = 0;
	UINT16 len;

	memset(pinfo, 0, sizeof(TJpegIndex));
	pinfo->YUVType = JPEG_UNKNOWN;
	pinfo->thumbinfo.thumbtype = NO_THUMBNAIL;

	//try to find the 1st marker SOI
	while(i < length)
	{
		if(ptr[i]==0xff)
		{
			if(ptr[i+1]==0xd8)
			{
				break;
			}
		}
		i++;
	}

	//find the SOI marker, start parsing
	while(i < length)
	{
		if(ptr[i] == 0xff)
		{
			do
			{
				i++;
			}
			while(ptr[i]==0xff);
			
			switch(ptr[i])
			{
			case M_SOI:								/* jpeg start header */
				pinfo->sop = i-1;
				break;
			case M_APP1:							/* App1 is used for EXIF */
				len = GetExifInfo(ptr, i+1, &pinfo->thumbinfo);
				i += len;
				break;
			case M_APP0:							/* App0 is used for JFIF */
				len = GetJfifInfo(ptr, i+1, &pinfo->thumbinfo);
				i += len;
				break;
			case M_SOF0:							/* Baseline */
				len = GetSOF0(pinfo, ptr+i+1);
				if(len == 0)
				{
					pinfo->YUVType = JPEG_UNKNOWN;
					pinfo->eop = i;
					return;
				}
				i += len;
				break;
			case M_DQT:								/* segment DQT */
				len = GetDQT(pinfo, ptr+i+1);
				if(len == 0)
				{
					pinfo->YUVType = JPEG_UNKNOWN;
					pinfo->eop = i;
					return;
				}
				i += len;
				break;
			case M_DHT:								/* segment DHT */
				len = GetDHT(pinfo, ptr+i+1);
				if(len == 0)
				{
					pinfo->YUVType = JPEG_UNKNOWN;
					pinfo->eop = i;
					return;
				}
				i += len;
				break;
			case M_SOS:								/* segment SOS */
				if (pinfo->HTCount != 4)
				{
					pinfo->YUVType = JPEG_UNKNOWN;
					pinfo->eop = i;
					return;
				}
				len = GetSOS(pinfo, ptr+i+1);
				if(len)
				{
					i += len;
					pinfo->offset = i+1;
				}
				//after SOS, there is only EOI exist;
				while(i < (length-1))
				{
					if(ptr[i]==0xff)
					{
						if(ptr[i+1]==0xd9)
						{
							pinfo->eop = i+2;
							return;
						}
						//{
						//	int j;
						//	j = (i+1) - pinfo->offset;
						//	if( ( j >= 4 ) && ((j % 4) == 0) )
						//	{
						//		if((ptr[i-3] & 0xf8) == 0xd0)
						//		{
						//			j = i - pinfo->offset-3;;
						//		}
						//	}
						//}
					}
					i++;
				}
				break;
			case M_EOI:
				pinfo->eop = i+1;
				return;
			case M_SOF1:		/* Extended sequential, Huffman */
			case M_SOF2:		/* Progressive, Huffman */
			case M_SOF9:		/* Extended sequential, arithmetic */
			case M_SOF10:		/* Progressive, arithmetic */
			case M_SOF3:		/* Lossless, Huffman */
			case M_SOF5:		/* Differential sequential, Huffman */
			case M_SOF6:		/* Differential progressive, Huffman */
			case M_SOF7:		/* Differential lossless, Huffman */
			case M_SOF11:		/* Lossless, arithmetic */
			case M_SOF13:		/* Differential sequential, arithmetic */
			case M_SOF14:		/* Differential progressive, arithmetic */
			case M_SOF15:		/* Differential lossless, arithmetic */
			case M_JPG:			/* Reserved for JPEG extensions */
								/* Currently unsupported SOFn types */
								/* set YUV_TYPE = unknown */
				len = GetSOF_Unsupport(pinfo, ptr+i+1);
				i += len;
				break;
			case M_COM:			/* Comment */
				len = GetComment(pinfo, ptr+i+1);
				i += len;
				break;
			case M_DAC:			/* Define arithmetic coding conditioning */
			case M_DRI:			/* Define restart interval */
			case M_APP2:		/* Reserved for application segments */
			case M_APP3:
			case M_APP4:
			case M_APP5:
			case M_APP6:
			case M_APP7:
			case M_APP8:
			case M_APP9:
			case M_APP10:
			case M_APP11:
			case M_APP12:
			case M_APP13:
			case M_APP14:
			case M_APP15:
			case M_RST0:		/* these are all parameterless */
			case M_RST1:
			case M_RST2:
			case M_RST3:
			case M_RST4:
			case M_RST5:
			case M_RST6:
			case M_RST7:
			case M_DNL:			/* Ignore DNL ... perhaps the wrong thing */
								/* Currently unsupported, bypass! */
				len = ByPassMarker(ptr+i+1);
				i += len;
				break;
			default:
								/* error! You could show error here */
								/* set YUVType = Unknown, and return */
				break;
			}
		}
		i++;
	}
	pinfo->YUVType = JPEG_UNKNOWN;
	pinfo->eop = i;
}

void JfifParse(UINT8 *ptr, UINT32 length, PTThumbNail pthumbinfo)
{
	UINT32 i = 0;
	UINT16 len;
	//pinfo->isThumb    =   0;
	memset(pthumbinfo, 0, sizeof(PTThumbNail));
	while(i < length)
	{
		if(ptr[i] == 0xff)
		{
			do{
				i++;
			} while(ptr[i] == 0xff);
			
			switch(ptr[i])
			{
			case 0xE0:
				len = GetJfifInfo(ptr, i+1, pthumbinfo);
				i  += len;
				break;
			case 0xd8:		//jpeg start header
				//pinfo->startpos = i-1;
				//i++;
				//break;
			default:	
				break;
			}
		}
		i++;
	}
}

//DOM-IGNORE-BEG
/*=============================================================================*/
/*                         static     functions                                */
/*=============================================================================*/
//DOM-IGNORE-END

UINT16 GetComment(PTJpegIndex pinfo, UINT8 *ptr)
{
	UINT16 len = GETWORD(ptr, 0);

	pinfo->frmcnt = GETWORD_INV(ptr, 2);
	pinfo->qf = GETBYTE(ptr, 25);

	return len;
}

UINT16 GetCompFrmSOF(UINT8 *buf)
{
	UINT8	h, v, n;
	UINT16	val = 0;
	
	h = (buf[0] & 0x70) >> 4;
	v = buf[0] & 0x3;
	n = buf[1];
	val = (UINT16)(h | (v << 4) | (n << 6)) << 8;
	n = h*v << 2;
	val |= (UINT16)n;
	return val;
}

UINT16 GetSOF0(PTJpegIndex pinfo, UINT8 * ptr)
{
	UINT16 len = GETWORD(ptr,0);

	pinfo->data_precision = ptr[2];
	pinfo->ImageSize.cy = GETWORD(ptr,3);
	pinfo->ImageSize.cx = GETWORD(ptr,5);
	pinfo->CompCount = ptr[7];
	if(pinfo->CompCount == 1)
	{
		if((ptr[8]-1) > 3)
			return 0;
		pinfo->YUVType = JPEG_400;
		pinfo->Comp[ ptr[8]-1 ] = GetCompFrmSOF(ptr+9);;
	}
	else
	{
		if( ((ptr[11] - 1) > 3) || ((ptr[14] - 1) > 3) || ((ptr[8] - 1) > 3) )
			return 0;
		pinfo->Comp[ ptr[8]-1 ] = GetCompFrmSOF(ptr+9);
		if((ptr[9] == 0x21) || (ptr[9] == 0x12))
			pinfo->YUVType = JPEG_422;
		else if(ptr[9] == 0x22)
			pinfo->YUVType = JPEG_420;
		else if(ptr[9] == 0x41)
			pinfo->YUVType = JPEG_411;
		else if(ptr[9] == 0x11)
			pinfo->YUVType = JPEG_444;
		else
			return 0;
		pinfo->Comp[ ptr[11]-1 ] = GetCompFrmSOF(ptr+12);
		pinfo->Comp[ ptr[14]-1 ] = GetCompFrmSOF(ptr+15);
	}
	
	GetJpegSize(pinfo->YUVType, pinfo->ImageSize, &(pinfo->JpgSize));
	pinfo->vwc = GetImageVwc(pinfo->YUVType, pinfo->JpgSize);
	pinfo->blkcount = GetJpegBlockNum(pinfo->YUVType, pinfo->JpgSize);

	return len;
}

UINT16 GetSOF_Unsupport(PTJpegIndex pinfo, UINT8 * ptr)
{
	UINT16 len = ((UINT16)ptr[0] << 8) + (UINT16)ptr[1];
	
	//unsupported pic, set everything to zero, YUVType to unknown and bypass the length
	pinfo->data_precision = 0;
	pinfo->ImageSize.cy = 0;
	pinfo->ImageSize.cx = 0;
	pinfo->CompCount = 0;
	pinfo->YUVType = JPEG_UNKNOWN;
	return len;
}

UINT16 ByPassMarker(UINT8 * ptr)
{
	UINT16 len = GETWORD(ptr,0);
	return len;
}

UINT16 GetDQT(PTJpegIndex pinfo, UINT8 * ptr)
{
	UINT16 len, i = 0;
	UINT8	n;
	
	len = GETWORD(ptr,0);
	
	i += 2;
	while(i < len)
	{
		if(ptr[i] & 0xf0)		//16 bit precision
		{
			n = ptr[i] & 0xf;
			if(n > 3)
				return 0;
			pinfo->QTPrecision[n] = 16;
			pinfo->QT[n] = ptr+i+1;
			i += 1 + (64 << 1);
		}
		else		//8 bit precision
		{
			n = ptr[i] & 0xf;
			if(n > 3)
				return 0;
			pinfo->QTPrecision[n] = 8;
			pinfo->QT[n] = ptr+i+1;
			i += 1 + 64;
		}
		pinfo->QTCount++;
	}
	return len;
}

UINT16 GetDHT(PTJpegIndex pinfo, UINT8 * ptr)
{
	UINT16 len, j, n, k, HTLen;
//	UINT16 tablen[4] = {0x1c, 0xb2, 0x1c, 0xb2};
	UINT8 *pd = ptr+2;	
	
	j = 0;
	len = GETWORD(ptr,0);

	while(j < len-3)
	{
		HTLen = 0;
		n = pd[j] & 0xf;
		k = pd[j] & 0xf0;
		n <<= 1;
		n += k >> 4;
		if(n > 3)
			return 0;
		j++;
		pinfo->HT[n] = pd+j;
		k = j + 16;
		while(j < k)
		{
			HTLen = HTLen + (UINT16)(pd[j]);
			j++;
		}
		pinfo->HTLen[n] = (UINT8)(16 + HTLen);
//		if (pinfo->HTLen[n] != tablen[n])
//			return 0;
		j = j+HTLen;
		pinfo->HTCount++;
	}
	return len;
}

UINT16 GetSOS(PTJpegIndex pinfo, UINT8 * ptr)
{
	UINT16 len;
	UINT8 i, k, n, ac, dc, count, *pbuf = ptr+3;	
	
	len = GETWORD(ptr,0);
	count = ptr[2];
	if(len != 6 + (count << 1))
		return 0;
	
	for(i = 0; i < count; i++)
	{
		k = i << 1;
		n = pbuf[k]-1;
		if(n > 3)
			return 0;
		ac = pbuf[k+1] & 0xf;
		dc = pbuf[k+1] & 0xf0;
		if(!ac)
			pinfo->Comp[n] |= 0x2;
		if(!dc)
			pinfo->Comp[n] |= 0x1;
	}
	return len;
}

void AddJfif(PTDataBuf pjpgbuf, PTDataBuf pthumbuf, PTDataBuf pdst, UINT8 thumbfmt, TSize thumbsize)
{
	UINT8 add_flag;
	UINT8 *psrc, *pdest;
	int i = 0;

	psrc = pjpgbuf->pData;
	pdest = pdst->pData;

	add_flag = 0;

	switch(thumbfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV400:
	case YUV444:
	case RGB24:
	case RGB8:
	case JPEG422:
		add_flag = 1;
		break;
	default:
		break;
	}

	if( (thumbsize.cx == 0) || (thumbsize.cy == 0) )
	{
		add_flag = 0;
	}

	if(add_flag == 0)
	{
		//thumbfmt unknow, just copy the src to dst
		memcpy(pdest,psrc, (unsigned int)pjpgbuf->Len);
		//write the total length
		pdst->Pos = pjpgbuf->Len;
	}
	else
	{
		//copy SOI
		pdest[i] = psrc[0];
		i++;
		pdest[i] = psrc[1];
		i++;

		//add JFIF
		pdest[i] = 0xff;
		i++;
		pdest[i] = 0xe0;
		i++;
		i += AddJfifInfo(pdest+i);

		//add JFXX
		pdest[i] = 0xff;
		i++;
		pdest[i] = 0xe0;
		i++;
		i += AddJfxxInfo(pdest+i, pthumbuf, thumbfmt, thumbsize);

		// copy the rest of jpg data
		memcpy(pdest+i, psrc+2, (unsigned int)(pjpgbuf->Len-2));

		// write the total length
		pdst->Pos = pjpgbuf->Len-2 + i;
	}
}

/*****************************************************************************
* I want to make the simplest JFIF creator, so if the thumbnail is in RGB24 format,
* just put the informat into JFXX segment rather than JFIF segment, that will 
* simplify the AddJfifInfo() routine
*****************************************************************************/
UINT16 AddJfifInfo(UINT8* pdst)
{
	UINT8	*ptr;

	ptr = pdst;

	/* insert JFIF len = 0x10 */
	ptr[0] = 0;
	ptr[1] = 0x10;

	/* insert JFIF identifier 5 bytes */
	ptr[2] = 'J';
	ptr[3] = 'F';
	ptr[4] = 'I';
	ptr[5] = 'F';
	ptr[6] = 0;

	/* insert JFIF version 2 bytes */
	ptr[7] = 0x01;
	ptr[8] = 0x02;

	/* insert JFIF units 1bytes */
	ptr[9] = 0;

	/* insert JFIF Xdensity 2 bytes */
	ptr[10] = 0;
	ptr[11] = 0;

	/* insert JFIF Ydensity 2 bytes */
	ptr[12] = 0;
	ptr[13] = 0;

	/* insert JFIF Xthumbnail 1 bytes */
	ptr[14] = 0;

	/* insert JFIF Ythumbnail 1 bytes */
	ptr[15] = 0;

	/* return length */
	return (UINT16)0x10;
}

UINT16 AddJfxxInfo(UINT8* pdst, PTDataBuf pthumbuf, UINT8 thumbfmt, TSize thumbsize)
{
	UINT8	*ptr;
	UINT16	len;

	len = 0;
	ptr = pdst;

	/* insert JFXX identifier 5 bytes */
	ptr[2] = 'J';
	ptr[3] = 'F';
	ptr[4] = 'X';
	ptr[5] = 'X';
	ptr[6] = 0;

	switch(thumbfmt)
	{
	case RGB8:
		ptr[7] = 0x11;
		ptr[8] = (UINT8)thumbsize.cx;
		ptr[9] = (UINT8)thumbsize.cy;
		memcpy( ptr+10, pthumbuf->pData, (unsigned int)pthumbuf->Len );
		len = (UINT16)pthumbuf->Len+10;
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		ptr[7] = 0x10;
		memcpy( ptr+8, pthumbuf->pData, (unsigned int)pthumbuf->Len );
		len = (UINT16)pthumbuf->Len+8;
		break;
	case RGB24:
		ptr[7] = 0x13;
		ptr[8] = (UINT8)thumbsize.cx;
		ptr[9] = (UINT8)thumbsize.cy;
		memcpy( ptr+10, pthumbuf->pData, (unsigned int)pthumbuf->Len );
		len = (UINT16)pthumbuf->Len+10;
		break;
	default:
		ptr[7] = 0x13;
		ptr[8] = (UINT8)thumbsize.cx;
		ptr[9] = (UINT8)thumbsize.cy;
		len = JfxxPutRGB(ptr+10,pthumbuf,thumbfmt,thumbsize);
		len += 10;
		break;
	}
	ptr[0] = (UINT8)(len >> 8);
	ptr[1] = (UINT8)(len & 0xFF);

	return len;
}

static const int IncDelta[][2] =
{
	{ 1, 2 },			//YUV422 UYVY
	{ 1, 2 },			//YUV422 VYUY
	{ 1, 2 },			//YUV422 YUYV
	{ 1, 2 },			//YUV422 YVYU
	{ 1, 2 },			//YUV422 YUVY

	{ 1, 2 },			//YUV422 YVUY
	{ 1, 2 },			//YUV422 UVYY
	{ 1, 2 },			//YUV422 VUYY
	{ 1, 2 },			//YUV422 YYVU
	{ 1, 2 },			//YUV422 YYUV

	{ 0 },				//RAW_BG
	{ 0 },				//RAW_GB
	{ 0 },				//RAW_RG
	{ 0 },				//RAW_GR

	{ 0 },					//RGB1
	{ 0 },					//RGB2
	{ 0 },					//RGB4
	{ 0 },					//RGB8
	{ 0 },					//RGB444

	{ 0 },					//RGB555
	{ 0 },					//RGB565
	{ 0 },					//RGB666
	{ 0 },					//RGB24
	{ 0 },					//RGB32

	{ 2, 2 },			//YUV420 UYVY
	{ 2, 2 },			//YUV420 VYUY
	{ 2, 2 },			//YUV420 YUYV
	{ 2, 2 },			//YUV420 YVYU
	{ 2, 2 },			//YUV420 YUVY

	{ 2, 2 },			//YUV420 YVUY
	{ 2, 2 },			//YUV420 UVYY
	{ 2, 2 },			//YUV420 VUYY
	{ 2, 2 },			//YUV420 YYVU
	{ 2, 2 },			//YUV420 YYUV

	{ 1, 4 },	//YUV411_YYYYUV
	{ 1, 4 },	//YUV411_YYYYVU,
	{ 1, 4 },	//YUV411_YUYVYY,
	{ 1, 4 },	//YUV411_YVYUYY,
	{ 1, 4 },	//YUV411_UVYYYY,
	{ 1, 4 },	//YUV411_VUYYYY,

	{ 1, 1 },					//YUV400
	{ 1, 1 },					//YUV444

	{ 0 },					//YUV422BLK
	{ 0 },					//YUV411BLK
	{ 0 },					//YUV420BLK
	{ 0 },					//YUV400BLK
	{ 0 },					//YUV444BLK

	{ 0 },					//YUV422LBUF
	{ 0 },					//YUV411LBUF
	{ 0 },					//YUV420LBUF
	{ 0 },					//YUV400LBUF
	{ 0 },					//YUV444LBUF

	{ 0 },					//BMP
	{ 0 }					//JPEGDATA
};

static const int DataFormat[][6] =
{
	{ 1, 0, 2, 3 },			//YUV422 UYVY
	{ 2, 0, 1, 3 },			//YUV422 VYUY
	{ 0, 1, 3, 2 },			//YUV422 YUYV
	{ 0, 2, 3, 1 },			//YUV422 YVYU
	{ 0, 1, 2, 3 },			//YUV422 YUVY

	{ 0, 2, 1, 3 },			//YUV422 YVUY
	{ 1, 2, 0, 3 },			//YUV422 UVYY
	{ 2, 1, 0, 3 },			//YUV422 VUYY
	{ 0, 3, 2, 1 },			//YUV422 YYVU
	{ 0, 3, 1, 2 },			//YUV422 YYUV

	{ 0 },				//RAW_BG
	{ 0 },				//RAW_GB
	{ 0 },				//RAW_RG
	{ 0 },				//RAW_GR

	{ 0 },					//RGB1
	{ 0 },					//RGB2
	{ 0 },					//RGB4
	{ 0 },					//RGB8
	{ 0 },					//RGB444

	{ 0 },					//RGB555
	{ 0 },					//RGB565
	{ 0 },					//RGB666
	{ 0 },					//RGB24
	{ 0 },					//RGB32

	{ 1, 0, 2, 3 },			//YUV420 UYVY
	{ 2, 0, 1, 3 },			//YUV420 VYUY
	{ 0, 1, 3, 2 },			//YUV420 YUYV
	{ 0, 2, 3, 1 },			//YUV420 YVYU
	{ 0, 1, 2, 3 },			//YUV420 YUVY

	{ 0, 2, 1, 3 },			//YUV420 YVUY
	{ 1, 2, 0, 3 },			//YUV420 UVYY
	{ 2, 1, 0, 3 },			//YUV420 VUYY
	{ 0, 3, 2, 1 },			//YUV420 YYVU
	{ 0, 3, 1, 2 },			//YUV420 YYUV

	{ 0, 3, 4, 5, 1, 2 },	//YUV411_YYYYUV
	{ 0, 3, 4, 5, 2, 1 },	//YUV411_YYYYVU,
	{ 0, 1, 3, 2, 4, 5 },	//YUV411_YUYVYY,
	{ 0, 2, 3, 1, 4, 5 },	//YUV411_YVYUYY,
	{ 1, 2, 0, 3, 4, 5 },	//YUV411_UVYYYY,
	{ 2, 1, 0, 3, 4, 5 },	//YUV411_VUYYYY,

	{ 0 },					//YUV400
	{ 0 },					//YUV444

	{ 0 },					//YUV422BLK
	{ 0 },					//YUV411BLK
	{ 0 },					//YUV420BLK
	{ 0 },					//YUV400BLK
	{ 0 },					//YUV444BLK

	{ 0 },					//YUV422LBUF
	{ 0 },					//YUV411LBUF
	{ 0 },					//YUV420LBUF
	{ 0 },					//YUV400LBUF
	{ 0 },					//YUV444LBUF

	{ 0 },					//BMP
	{ 0 }					//JPEGDATA
};

UINT16 JfxxPutRGB(UINT8* pdst, PTDataBuf pthumbuf, UINT8 thumbfmt, TSize thumbsize)
{
	int x, y, delta_x, delta_y;
	UINT8 yuv[6];
	UINT8 *psrc, *psrc_line2 = 0, *pdst_line1 = 0, *pdst_line2 = 0;

	psrc = pthumbuf->pData;
	x = thumbsize.cx;
	y = thumbsize.cy;

	delta_x = IncDelta[thumbfmt][1];
	delta_y = IncDelta[thumbfmt][0];

	for(y = 0; y < thumbsize.cy; y += delta_y)
	{
		switch(thumbfmt)
		{
		case YUV420_UYVY:
		case YUV420_VYUY:
		case YUV420_YUYV:
		case YUV420_YVYU:
		case YUV420_YUVY:
		case YUV420_YVUY:
		case YUV420_UVYY:
		case YUV420_VUYY:
		case YUV420_YYVU:
		case YUV420_YYUV:
			psrc = pthumbuf->pData + (y >> 1) * thumbsize.cx * 3;
			psrc_line2 = psrc + (thumbsize.cx << 1);
			pdst_line1 = pdst + y*thumbsize.cx * 3;
			pdst_line2 = pdst_line1 + thumbsize.cx * 3;
			break;

		default:
			break;
		}

		for(x = 0; x < thumbsize.cx; x += delta_x)
		{
			switch(thumbfmt)
			{
			case YUV422_UYVY:
			case YUV422_VYUY:
			case YUV422_YUYV:
			case YUV422_YVYU:
			case YUV422_YUVY:
			case YUV422_YVUY:
			case YUV422_UVYY:
			case YUV422_VUYY:
			case YUV422_YYVU:
			case YUV422_YYUV:
				yuv[0] = psrc[DataFormat[thumbfmt][0]];
				yuv[1] = psrc[DataFormat[thumbfmt][1]];
				yuv[2] = psrc[DataFormat[thumbfmt][2]];
				yuv[3] = psrc[DataFormat[thumbfmt][3]];
				yuv422_convert2points(yuv, pdst);
				pdst += 6;//3*2;
				psrc += 4;
				break;

			case YUV420_UYVY:
			case YUV420_VYUY:
			case YUV420_YUYV:
			case YUV420_YVYU:
			case YUV420_YUVY:
			case YUV420_YVUY:
			case YUV420_UVYY:
			case YUV420_VUYY:
			case YUV420_YYVU:
			case YUV420_YYUV:
				yuv[0] = psrc[DataFormat[thumbfmt][0]];
				yuv[1] = psrc[DataFormat[thumbfmt][1]];
				yuv[2] = psrc[DataFormat[thumbfmt][2]];
				yuv[3] = psrc[DataFormat[thumbfmt][3]];
				yuv[4] = psrc_line2[0];
				yuv[5] = psrc_line2[1];
				yuv420_convert4points(yuv, pdst_line1, pdst_line2);
				pdst_line1 += 6;//3*2;
				pdst_line2 += 6;//3*2;
				psrc += 4;
				psrc_line2 += 2;
				break;

			case YUV411_YYYYUV:
			case YUV411_YYYYVU:
			case YUV411_YUYVYY:
			case YUV411_YVYUYY:
			case YUV411_UVYYYY:
			case YUV411_VUYYYY:
				yuv[0] = psrc[DataFormat[thumbfmt][0]];
				yuv[1] = psrc[DataFormat[thumbfmt][1]];
				yuv[2] = psrc[DataFormat[thumbfmt][2]];
				yuv[3] = psrc[DataFormat[thumbfmt][3]];
				yuv[4] = psrc[DataFormat[thumbfmt][4]];
				yuv[5] = psrc[DataFormat[thumbfmt][5]];
				yuv411_convert4points(yuv, pdst);
				pdst += 12;//3*4;
				psrc += 6;
				break;

			case YUV400:
				yuv[0] = *psrc++;
				yuv400_convert1points(yuv, pdst);
				pdst += 3;
				break;

			case YUV444:
				yuv[0] = psrc[0];
				yuv[1] = psrc[1];
				yuv[2] = psrc[2];
				yuv444_convert1points(yuv, pdst);
				pdst += 3;
				psrc += 3;
				break;

			default:
				break;
			}
		}
	}

	return (UINT16)(3*thumbsize.cx*thumbsize.cy);
}


UINT16 GetJfifInfo(UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo)
{
	UINT32  i;
	UINT16 len;
	UINT8 isJFIF, isJFXX;
	UINT8 extensioncode;

	i      =   point;
	len    =   GETWORD(ptr, point);
	i     +=    2;
	//pjfifinfo->fieldLen =  len;
	if( (ptr[i] == 'J') && (ptr[i+1] == 'F') 
		&& (ptr[i+2] == 'I') && (ptr[i+3] == 'F')
		&& (ptr[i+4] == 0) )
		isJFIF = 1;
	else
		isJFIF = 0;
	if(ptr[i] == 'J' && ptr[i+1] == 'F' 
		&& ptr[i+2] == 'X' && ptr[i+3] == 'X'
		&& ptr[i+4] == 0)
		isJFXX = 1;
	else 
		isJFXX = 0;

	i += 5;
	if(isJFIF)
	{	
		i += 7;		
		pthumbinfo->thumbWidth = ptr[i];
		i++;		
		pthumbinfo->thumbHeight = ptr[i];
		i++;
		pthumbinfo->thumblen = len - 0x10;
			
		if(pthumbinfo->thumblen)
		{
			pthumbinfo->thumbtype = THUMBNAIL_JFIF;
			pthumbinfo->thumbfmt = RGB24;
			pthumbinfo->pthumbbuf = ptr + i;			
		}
	}
	if(isJFXX)
	{		
		extensioncode  =  ptr[i];
		switch(extensioncode)
		{
		case 0x10:
			pthumbinfo->thumbfmt = JPEG422;
			pthumbinfo->pthumbbuf = ptr + point + 0x08;
			pthumbinfo->thumblen = len - 0x08;			
			break;
		case 0x11:
			pthumbinfo->thumbfmt = RGB8;
			pthumbinfo->thumbWidth = ptr[i + 1];
			pthumbinfo->thumbHeight = ptr[i + 2];
			pthumbinfo->ppalette = ptr + point + 0x0a;
			pthumbinfo->pthumbbuf = ptr + point + 0x0a + 0x300;
			pthumbinfo->thumblen = len - 0x0a - 0x300;			
			break;
		case 0x13:
			pthumbinfo->thumbfmt = RGB24;
			pthumbinfo->thumbWidth = ptr[i + 1];
			pthumbinfo->thumbHeight = ptr[i + 2];
			pthumbinfo->pthumbbuf = ptr + point + 0x0a;
			pthumbinfo->thumblen = len - 0x0a;			
			break;
		default: 
			break;
		}
		if(pthumbinfo->thumblen)
			pthumbinfo->thumbtype = THUMBNAIL_JFXX;
	}
	return len;
}

UINT32 GetImageVwc(UINT8 fmt, TSize size)
{
	TSize size1;

	GetJpegSize(fmt, size, &size1);
	return GetYuvSize(fmt, size1) >> 2;
}

UINT16 GetJpegBlockNum(UINT8 fmt, TSize size)
{
	TSize size1;

	GetJpegSize(fmt, size, &size1);
	switch(fmt)
	{
	case JPEG_422:
		size1.cx >>= 2;
		break;
	case JPEG_420:
		size1.cx >>= 2;
		size1.cx += size1.cx >> 1;
		break;
	case JPEG_411:
		size1.cx >>= 3;
		size1.cx += size1.cx >> 1;
		break;
	case JPEG_400:
		size1.cx >>= 3;
		break;
	case JPEG_444:
		size1.cx >>= 2;
		size1.cx += size1.cx >> 1;
		break;
	default:
		break;
	}
	return size1.cx;
}

UINT8 GetComponent(UINT8 fmt, UINT16 *comp)
{
	UINT8 compnum = 0x23;

	comp[0] = 0x120b;
	comp[1] = 0x5104;
	comp[2] = 0x5104;
	switch(fmt)
	{
	case JPEG_422:
		break;
	case JPEG_420:
		comp[0] = 0x2213;
		break;
	case JPEG_411:
		comp[0] = 0x1413;
		break;
	case JPEG_400:
		compnum = 0x11;
		comp[0] = 0x1107;
		comp[1] = 0;
		comp[2] = 0;
		break;
	case JPEG_444:
		compnum = 0x11;
		comp[0] = 0x1107;
		break;
	default:
		break;
	}
	return compnum;
}

void GetJpegSize(UINT8 fmt, TSize size, TSize *size1)
{
	switch(fmt)
	{
	case JPEG_422:
		size1->cx = ((size.cx + 15) >> 4) << 4;
		size1->cy = ((size.cy + 7) >> 3) << 3;
		break;
	case JPEG_420:
		size1->cx = ((size.cx + 15) >> 4) << 4;
		size1->cy = ((size.cy + 15) >> 4) << 4;
		break;
	case JPEG_411:
		size1->cx = ((size.cx + 31) >> 5) << 5;
		size1->cy = ((size.cy + 7) >> 3) << 3;
		break;
	case JPEG_400:
	case JPEG_444:
		size1->cx = ((size.cx + 7) >> 3) << 3;
		size1->cy = ((size.cy + 7) >> 3) << 3;
		break;
	default:
		break;
	}
}


#if 0
typedef struct tag_TExifFormatTable {
	EXIFFORMATenum format;
	const char *name;
	unsigned char size;
} TExifFormatTable, *PTExifFormatTable;

TExifFormatTable ExifFormatTableList[] = {
	{EXIF_FORMAT_BYTE,      "Byte",      1},
	{EXIF_FORMAT_ASCII,     "Ascii",     1},
	{EXIF_FORMAT_SHORT,     "Short",     2},
	{EXIF_FORMAT_LONG,      "Long",      4},
	{EXIF_FORMAT_RATIONAL,  "Rational",  8},
	{EXIF_FORMAT_SBYTE,     "SByte",     1},
	{EXIF_FORMAT_SSHORT,    "SShort",    2},
	{EXIF_FORMAT_SLONG,     "SLong",     4},
	{EXIF_FORMAT_SRATIONAL, "SRational", 8},
	{EXIF_FORMAT_FLOAT,     "Float",     4},
	{EXIF_FORMAT_DOUBLE,    "Double",    8},
	{EXIF_FORMAT_UNDEFINED, "Undefined", 1},
	{0, NULL, 0}
};

static ExifSRational exif_get_srational(const unsigned char *b, UINT8 order);
static ExifRational  exif_get_rational(const unsigned char *b, UINT8 order);
static void exif_set_rational(unsigned char *b, UINT8 order, ExifRational value);
static void exif_set_srational(unsigned char *b, UINT8 order, ExifSRational value);
static void exif_set_short(unsigned char *b, UINT8 order, UINT16 value);
static void exif_set_long(unsigned char *b, UINT8 order, UINT32 value);
static void exif_set_slong(unsigned char *b, UINT8 order, SINT32 value);

void exif_set_short (unsigned char *b, UINT8 order, UINT16 value)
{
	if (!b) return;
	switch (order) {
	case EXIF_BYTE_ORDER_MOTOROLA:
		b[0] = (unsigned char) (value >> 8);
		b[1] = (unsigned char) value;
		break;
	case EXIF_BYTE_ORDER_INTEL:
		b[0] = (unsigned char) value;
		b[1] = (unsigned char) (value >> 8);
		break;
	}
}

ExifSRational exif_get_srational (const unsigned char *buf, UINT8 order)
{
	ExifSRational r;

	r.numerator   = buf ? exif_get_slong (buf, order) : 0;
	r.denominator = buf ? exif_get_slong (buf + 4, order) : 0;

	return (r);
}

ExifRational exif_get_rational (const unsigned char *buf, UINT8 order)
{
	ExifRational r;

	r.numerator   = buf ? exif_get_long (buf, order) : 0;
	r.denominator = buf ? exif_get_long (buf + 4, order) : 0;

	return (r);
}

void exif_set_rational (unsigned char *buf, UINT8 order, ExifRational value)
{
	if (!buf) return;
	exif_set_long (buf, order, value.numerator);
	exif_set_long (buf + 4, order, value.denominator);
}

void exif_set_srational (unsigned char *buf, UINT8 order, ExifSRational value)
{
	if (!buf) return;
	exif_set_slong (buf, order, value.numerator);
	exif_set_slong (buf + 4, order, value.denominator);
}

void exif_set_long (unsigned char *b, UINT8 order, UINT32 value)
{
	exif_set_slong (b, order, value);
}

void exif_set_slong (unsigned char *b, UINT8 order, SINT32 value)
{
	if (!b) return;
	switch (order) {
	case EXIF_BYTE_ORDER_MOTOROLA:
		b[0] = (unsigned char) (value >> 24);
		b[1] = (unsigned char) (value >> 16);
		b[2] = (unsigned char) (value >> 8);
		b[3] = (unsigned char) value;
		break;
	case EXIF_BYTE_ORDER_INTEL:
		b[3] = (unsigned char) (value >> 24);
		b[2] = (unsigned char) (value >> 16);
		b[1] = (unsigned char) (value >> 8);
		b[0] = (unsigned char) value;
		break;
	}
}
#endif

#define JFIF_HEAD	0x10

UINT32 Jpeg2Jfif(unsigned char* jpg, unsigned char* thumb, unsigned char* jfif, UINT32 jpgLen, UINT16 thumbLen, UINT8 thumbX, UINT8 thumbY)
{
	TJpegIndex jpgInfo;
	UINT32 i = 0, j;
	UINT16 val;
	
	memset(&jpgInfo, 0, sizeof(TJpegIndex));

	JpegParse(jpg, jpgLen, &jpgInfo);

	val = 0xFFD8;
	SETWORD(jfif, i, val);					//SOI
	val = 0xFFE0;
	SETWORD(jfif, i, val);					//APP1,jfif head
	val = JFIF_HEAD + thumbLen;
	SETWORD(jfif, i, val);					//jfif head length

	SETBYTE(jfif, i, 'J');					//tag "JFIF0"
	SETBYTE(jfif, i, 'F');			
	SETBYTE(jfif, i, 'I');
	SETBYTE(jfif, i, 'F');
	SETBYTE(jfif, i, 0);

	SETBYTE(jfif, i, 1);					//version 1.02
	SETBYTE(jfif, i, 2);
	
	SETBYTE(jfif, i, 0);					//unit  0--none
	SETWORD(jfif, i, jpgInfo.ImageSize.cx);	//image x density
	SETWORD(jfif, i, jpgInfo.ImageSize.cy);	//image y density
	SETBYTE(jfif, i, thumbX);				//thumbnail x density
	SETBYTE(jfif, i, thumbY);				//thumbnail y density
	
	j = 0;
	while(j < thumbLen)
	{
		*(jfif+i+j) = *(thumb+j);
		j++;
	}
	j = 0;
	while(j < jpgLen)
	{
		*(jfif+i+thumbLen+j) = *(jpg+2+j);
		j++;
	}
	//strcat((char*)jfif+i, (char*)thumb);					//add thumb data 
	//strcat((char*)jfif+i+thumbLen, (char*)jpg+2);		

	return i+thumbLen+jpgLen-2;
}

UINT32 Jpeg2Jfxx(unsigned char* jpg, unsigned char* thumb, unsigned char* jfxx, UINT32 jpgLen, UINT16 thumbLen, UINT8 thumbX, UINT8 thumbY, UINT8 fmt)
{
	TJpegIndex jpgInfo;
	UINT32 i = 0, j;
	UINT16 val;
	
	memset(&jpgInfo, 0, sizeof(TJpegIndex));

	JpegParse(jpg, jpgLen, &jpgInfo);

	val = 0xFFD8;
	SETWORD(jfxx, i, val);				//SOI
	val = 0xFFE0;
	SETWORD(jfxx, i, val);				//APP1,jfif head
	switch(fmt)
	{
	case THUMB_FORMAT_JPEG:
		val = thumbLen + 0x08;
		break;
	case THUMB_FORMAT_RGB8:
	case THUMB_FORMAT_RGB24:
		val = thumbLen + 0x0a;
		break;
	default:
		break;
	}
	SETWORD(jfxx, i, val);				//jfxx head length

	SETBYTE(jfxx, i, 'J');				//tag "JFXX0"
	SETBYTE(jfxx, i, 'F');			
	SETBYTE(jfxx, i, 'X');
	SETBYTE(jfxx, i, 'X');
	SETBYTE(jfxx, i, 0);

	switch(fmt)							//set extension code
	{
	case THUMB_FORMAT_JPEG:
		SETBYTE(jfxx, i, 0x10);
		break;
	case THUMB_FORMAT_RGB8:
		SETBYTE(jfxx, i, 0x11);
		SETBYTE(jfxx, i, thumbX);
		SETBYTE(jfxx, i, thumbY);
		break;
	case THUMB_FORMAT_RGB24:
		SETBYTE(jfxx, i, 0x13);
		SETBYTE(jfxx, i, thumbX);
		SETBYTE(jfxx, i, thumbY);
		break;
	default:
		break;
	}
		
	j = 0;
	while(j < thumbLen)
	{
		*(jfxx+i+j) = *(thumb+j);
		j++;
	}
	j = 0;
	while(j < jpgLen)
	{
		*(jfxx+i+thumbLen+j) = *(jpg+2+j);
		j++;
	}
	
	return i+thumbLen+jpgLen-2;
}

/*
UINT32 GetThumbFromJfif(unsigned char* jfif, UINT32 jfifLen, unsigned char* thumb, unsigned char* fmt, unsigned char* x, unsigned char* y)
{
	TJpegIndex jpgInfo;
	UINT16 i;

	memset((void*)&jpgInfo, 0, sizeof(TJpegIndex));
	JpegParse(jfif, jfifLen, &jpgInfo);

	if ( !( (jpgInfo.thumbinfo.thumbtype  == THUMBNAIL_JFIF) ||
		 (jpgInfo.thumbinfo.thumbtype  == THUMBNAIL_JFXX) ) )
		 return 0;
		
	//strncpy((char*)thumb, (char*)jpgInfo.jfif.pthumbbuf, jpgInfo.jfif.thumblen);
	for(i = 0; i < jpgInfo.thumbinfo.thumblen; i++)
		thumb[i] = jpgInfo.thumbinfo.pthumbbuf[i];
	*fmt = jpgInfo.thumbinfo.thumbfmt;
	*x = (UINT8)jpgInfo.thumbinfo.thumbWidth;
	*y = (UINT8)jpgInfo.thumbinfo.thumbHeight;
	
	return jpgInfo.thumbinfo.thumblen;
}
*/
/*====================================
		Exif parser 
======================================*/
UINT16 ExifGetWord(UINT8* ptr, UINT8 order)
{
	if (!ptr) 
		return 0;
	switch(order)
	{
		case EXIF_BYTE_ORDER_MOTOROLA:
			return GETWORD(ptr, 0);
		case EXIF_BYTE_ORDER_INTEL:
			return GETWORD_INV(ptr, 0);
		default:
			return 0;
	}
}

UINT32 ExifGetDword(UINT8* ptr, UINT8 order)
{
	if (!ptr) 
		return 0;
	switch(order)
	{
        case EXIF_BYTE_ORDER_MOTOROLA:
			return GETDWORD(ptr, 0);
        case EXIF_BYTE_ORDER_INTEL:
			return GETDWORD_INV(ptr, 0);
		default:
			return 0;
	}
}


UINT16 GetExifInfo(UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo)
{
	UINT32 i;
	UINT16 len;

	i = point;
	len = GETWORD(ptr, i);
	i += 2;

	if(ptr[i] == 'E' && ptr[i+1] == 'x' 
		&& ptr[i+2] == 'i' && ptr[i+3] == 'f'
		&& ptr[i+4] == 0 && ptr[i+5] == 0)
	{
		i += 6;
		ParseExif(ptr+i, pthumbinfo);
	}

	return len;
}
/*============================================================================
	Desc: get exif ifd info
	notice: ptr is the point to TIFF header, all of offset is based on it
=============================================================================*/

void ParseExif(UINT8* ptr, PTThumbNail pthumbinfo)
{
	UINT32 i = 0;
	UINT16 byteOrder;
	UINT32 ifdOffset;
	
	byteOrder = GETWORD(ptr, i);
	i += 2;

	if(byteOrder == 0x4949)
		pthumbinfo->byteOrder = EXIF_BYTE_ORDER_INTEL;
	else if(byteOrder == 0x4D4D)
		pthumbinfo->byteOrder = EXIF_BYTE_ORDER_MOTOROLA;
	else		
		return;		

	i += 2;												//skip '2A00'
	
	ifdOffset = ExifGetDword(ptr+i, pthumbinfo->byteOrder);

	ifdOffset = GetIFD0(ptr, ifdOffset, pthumbinfo);

	if(ifdOffset != 0)
		ifdOffset = GetIFD1(ptr, ifdOffset, pthumbinfo);
}

UINT32 GetIFD0(UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo)
{
	UINT16 number;
	UINT32 i = point;
	UINT32 ifd1Offset = 0;

	number = ExifGetWord(ptr+i, pthumbinfo->byteOrder);
	i += 2;

	i += number * 12;										//skip the 12-byte field Interoperability arrays

	ifd1Offset = ExifGetDword(ptr+i, pthumbinfo->byteOrder);

	return ifd1Offset;
}

UINT32 GetIFD1(UINT8* ptr, UINT32 point, PTThumbNail pthumbinfo)
{
	UINT8 unit = 0;
	UINT16 number, j;	
	UINT16	tag, type;
	UINT32	i, nextIfdOffset = 0;
	UINT32	count, offset = 0;	

	TExifIFD1 ifd1info;

    memset(&(ifd1info), 0, sizeof(TExifIFD1));
	i = point;
	number = ExifGetWord(ptr+i, pthumbinfo->byteOrder);
	i += 2;
	
	for(j = 0; j < number; j++)
	{
		tag = ExifGetWord(ptr+i, pthumbinfo->byteOrder);
		i += 2;
		type = ExifGetWord(ptr+i, pthumbinfo->byteOrder);
		i += 2;
		count = ExifGetDword(ptr+i, pthumbinfo->byteOrder);
		i += 4;
		switch(type)
		{
			case EXIF_FORMAT_BYTE:
			case EXIF_FORMAT_SBYTE:
			case EXIF_FORMAT_UNDEFINED:
			case EXIF_FORMAT_ASCII:
				unit = 1;
				offset = (UINT32)ptr[i];
				break;
			case EXIF_FORMAT_SHORT:
			case EXIF_FORMAT_SSHORT:
				unit = 2;
				offset = (UINT32)ExifGetWord(ptr+i, pthumbinfo->byteOrder);
				break;
			case EXIF_FORMAT_LONG:
			case EXIF_FORMAT_SLONG:
				unit = 4;
				offset = ExifGetDword(ptr+i, pthumbinfo->byteOrder);
				break;
			case EXIF_FORMAT_RATIONAL:
			case EXIF_FORMAT_SRATIONAL:
			case EXIF_FORMAT_FLOAT:
			case EXIF_FORMAT_DOUBLE:
				//currently, we don't need these cases, just bypass them
			default:
				//error
				unit = 0;
				offset = 0;
				break;
		}
		
		switch(tag)				//current support tag data size less than 4, so offset is not used,but it can be extended easily
		{
			case EXIF_TAG_COMPRESSION:
				ifd1info.compression = (UINT16)offset;
				break;
			case EXIF_TAG_IMAGE_WIDTH:
				pthumbinfo->thumbWidth = ifd1info.thumbWidth = offset;
				break;
			case EXIF_TAG_IMAGE_LENGTH:
				pthumbinfo->thumbHeight = ifd1info.thumbHeight = offset;
				break;			
			case EXIF_TAG_PHOTOMETRIC_INTERPRETATION:
				ifd1info.PhotometricInterpretation = (UINT16)offset;
				break;
			case EXIF_TAG_YCBCR_SUB_SAMPLING:
				ifd1info.YCbCrSubSampling = (UINT16)offset;
				break;
			case EXIF_TAG_JPEG_INTERCHANGE_FORMAT:
				ifd1info.JPEGInterchangeFormat = offset;				
				break;
			case EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH:				
				ifd1info.JPEGInterchangeFormatLength = offset;
				break;			
			case EXIF_TAG_STRIP_OFFSETS:			//do not support strip store
				ifd1info.StripOffsets = offset;				
				break;
			case EXIF_TAG_STRIP_BYTE_COUNTS:				
				ifd1info.StripByteCounts = offset;
				break;
			case EXIF_TAG_PLANAR_CONFIGURATION:
				ifd1info.PlanarConfiguration = (UINT16)offset;
				break;

			case EXIF_TAG_ROWS_PER_STRIP:				
			case EXIF_TAG_BITS_PER_SAMPLE:				
			case EXIF_TAG_SAMPLES_PER_PIXEL:								
			case EXIF_TAG_YCBCR_POSITIONING:			
				break;
			default:									//currently, we don't need others' info
				break;
		}
		i += 4;	
	}	

	pthumbinfo->thumbtype = THUMBNAIL_EXIF;
	if (ifd1info.compression == 6)
	{
		pthumbinfo->thumbfmt = JPEG422;
		
		pthumbinfo->pthumbbuf = (UINT8*)ptr + ifd1info.JPEGInterchangeFormat;
		pthumbinfo->thumblen = ifd1info.JPEGInterchangeFormatLength;
	}
	else if (ifd1info.compression == 1)				//no compression
	{
		pthumbinfo->pthumbbuf = (UINT8*)ptr + ifd1info.StripOffsets;
		pthumbinfo->thumblen = ifd1info.StripByteCounts;

		if (ifd1info.PhotometricInterpretation == 2)	// RGB
		{
			if (ifd1info.PlanarConfiguration == 1)
				pthumbinfo->thumbfmt = RAW_RGB;
			else
				pthumbinfo->thumbfmt = RAW_RGB_RpGpBp;
		}
		else			//YCbCr
		{
			if (ifd1info.YCbCrSubSampling == 0x22)
			{
                if (ifd1info.PlanarConfiguration == 1)
					pthumbinfo->thumbfmt = YCbCr420_YYYYCbCr;
				else
					pthumbinfo->thumbfmt = YCbCr420_YpCbpCrp;
			}
			else
			{
				if (ifd1info.PlanarConfiguration == 1)
					pthumbinfo->thumbfmt = YCbCr422_YYCbCr;
				else
					pthumbinfo->thumbfmt = YCbCr422_YpCbpCrp;
			}
		}
	}
	else
		pthumbinfo->thumbtype = NO_THUMBNAIL;
    
	nextIfdOffset = ExifGetDword(ptr+i, pthumbinfo->byteOrder);

	return nextIfdOffset;
}



