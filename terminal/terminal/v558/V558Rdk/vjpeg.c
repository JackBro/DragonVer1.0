#include "v558api.h"
#include "v558def.h"
#include "vjpeg.h"
#include "vregdef.h"
#include "string.h"
#include "yuv2bmp.h"
V558_JPEG_CONTEXT gJpegContext;
V558_JFIFTEMPLE   jfiftemple =
{
	0xE0FF, //
	0,
	0x4649464A,
	0x0100,
	0x0002,
	0,
	0,
	0x1e28,
};
/*****************************************************************************
Description:
	Create a jfif format image.
Parameters:
	pOut: The output buffer's pointer.
	pjpegin:The input jpeg buffer's pointer.
	jpeglen:The length of jpeg image.
	pthumbin:The input thumbnail buffer's pointer.
	yuvthumblen:The lenght of thumbnail.(data format:yyuv)
Notes:
	NULL
Return:
	The jfif image file length.
Remarks:
	NULL
*******************************************************************************/
UINT32  V558_CreateJFIF(UINT8* pOut, UINT8* pjpegin, UINT32 jpeglen, UINT8* pthumbin, UINT32 yuvthumblen)
{
	UINT32 i,backoffset,srcadd,dstadd,rgbthumblen,len;
	UINT16    fieldlen;
	UINT8     temp;
	YUVType jpg0,jpg1;
	RGBType rgb;
	rgbthumblen =  yuvthumblen + (yuvthumblen>>1);
	backoffset  =  rgbthumblen + 0x12;
	fieldlen    =  rgbthumblen + 0x10;
	srcadd      =  jpeglen -3;
	dstadd      =  backoffset +srcadd;
	memcpy(pOut+rgbthumblen + 0x14,pjpegin + 2, jpeglen-2);
	*pOut       =  0xFF;
	*(pOut + 1) =  0xD8;
	memcpy(pOut+2, &jfiftemple, sizeof(jfiftemple));
	*(pOut + 4) =  fieldlen>>8;
	*(pOut + 5) =  fieldlen&0x00FF;

	for(i=0; i<(yuvthumblen>>2) ; i++)
	{
		srcadd    =  (i<<2);
		dstadd    =  0x14+ (i<<2) + (i<<1);
		jpg0.y  = *(pthumbin+srcadd+0);
		jpg1.y  = *(pthumbin+srcadd+1);
		jpg1.u  = jpg0.u  = *(pthumbin+srcadd+2);
		jpg1.v  = jpg0.v  = *(pthumbin+srcadd+3);
		//change from yuv to rgb
		YUV2RGB(&rgb,&jpg0);
		temp   =   rgb.r;
		rgb.r =   rgb.b;
		rgb.b =   temp;
		StoreRgb(pOut+dstadd , &rgb);
		YUV2RGB(&rgb,&jpg1);
		temp   =   rgb.r;
		rgb.r =   rgb.b;
		rgb.b =   temp;
		StoreRgb(pOut+dstadd+3 , &rgb);
	}
	len = 0x12 + rgbthumblen + jpeglen;
	return len;
}
static UINT32 GetYuvLbufSize(UINT16 *line)
{
	UINT32 len = 0;
	
	switch(gJpegContext.mode)
	{
	case V558_DECODE_ONLY:
		*line = 8;
		switch(gJpegContext.header.YUVType)
		{
		case V558_YUV_422:
			len = gJpegContext.header.JpgSize.cx;
			len <<= 4;
			break;
		case V558_YUV_420:
			len = gJpegContext.header.JpgSize.cx;
			len *= 3;
			len <<= 3;
			*line = 16;
			break;
		case V558_YUV_411:
			len = gJpegContext.header.JpgSize.cx;
			len *= 3;
			len <<= 2;
			break;
		case V558_YUV_400:
			len = gJpegContext.header.JpgSize.cx;
			len <<= 3;
			break;
		default:
			break;
		}
		break;
	case V558_DECODE_IPP:
	case V558_DECODE_LCDIF:
			*line = V558_LbufGetStatus();
			len = gJpegContext.decodewidth << 1;
			*line &= 0x3c;
			*line >>= 2;
			len *= *line;
			break;
		default:
			break;
	}
	return len;
}
static void GetYuv422Data(UINT8 *pdat)		//used in mode0
{
	UINT16 i, j;
	UINT32 adr;
	UINT8 *ptr =  pdat;
	UINT8 temp0,temp1,temp2,temp3;
	for(j=0; j<8;j++)
	{
		adr    =  LBUF0BASEADR + j*LBUF_UNITSIZE;
		V558_ReadSram(adr, ptr, gJpegContext.header.JpgSize.cx << 1);
		for(i=0 ; i< (gJpegContext.header.JpgSize.cx  >> 1) ; i++)
		{
			adr =  (i<<2);
			switch(gJpegContext.decdatatype)
			{
			case V558_DATATYPE_YYUV:
				temp0        =     *(ptr+adr);
				temp1        =     *(ptr+adr+1);
				temp2        =     *(ptr+adr+2);
				temp3        =     *(ptr+adr+3);
				*(ptr+adr)   =     temp1;
				*(ptr+adr+1) =     temp3;
				*(ptr+adr+2) =     temp0;
				*(ptr+adr+3) =     temp2;
			break;	
			case V558_DATATYPE_YUYV:
				temp0        =     *(ptr+adr);
				*(ptr+adr)   =     *(ptr+adr+1);
				*(ptr+adr+1) =     temp0;
				temp0        =     *(ptr+adr+2);
				*(ptr+adr+2) =     *(ptr+adr+3);
				*(ptr+adr+3) =     temp0;
			break;
			default:	break;
			}
		}
		ptr += (gJpegContext.header.JpgSize.cx << 1);
	}
}
static void GetYuv420Data(UINT8 *pdat)
{
	UINT16 i, j;
	UINT32 adr;
	UINT8 *ptr = pdat;
	UINT8 temp0,temp1,temp2,temp3;	
	for(j=0; j<8;j++)
	{
		adr  = LBUF0BASEADR + j*LBUF_UNITSIZE*2;
		V558_ReadSram(adr, ptr, gJpegContext.header.JpgSize.cx << 1);
		for(i=0 ; i< (gJpegContext.header.JpgSize.cx  >> 1) ; i++)
		{
			adr          =     (i<<2);
			temp0        =     *(ptr+adr);
			temp1        =     *(ptr+adr+1);
			temp2        =     *(ptr+adr+2);
			temp3        =     *(ptr+adr+3);
			*(ptr+adr)   =     temp1;
			*(ptr+adr+1) =     temp3;
			*(ptr+adr+2) =     temp0;
			*(ptr+adr+3) =     temp2;
		}
		ptr += (gJpegContext.header.JpgSize.cx << 1);
		adr  = LBUF0BASEADR + j*LBUF_UNITSIZE*2 + LBUF_UNITSIZE;
		V558_ReadSram(adr, ptr, gJpegContext.header.JpgSize.cx << 1);
		for( i= 0 ; i < gJpegContext.header.JpgSize.cx ; i++)
		{
			*(ptr +  i)  =  *(ptr +  (i<<1)+1);
		}
		ptr += gJpegContext.header.JpgSize.cx;
	}
	
}
static void GetYuv411Data(UINT8 *pdat)  //used in mode0
{	
	UINT16  i, j;
	UINT32 adr;
	UINT8 *ptr = pdat;
	UINT8 temp0,temp1,temp2,temp3,temp5,temp7;
	for(j=0; j<8;j++)
	{
		adr        = LBUF0BASEADR +  j*LBUF_UNITSIZE;
		V558_ReadSram(adr, ptr, (gJpegContext.header.JpgSize.cx << 1));
		for(i=0; i<(UINT16)(gJpegContext.header.JpgSize.cx >>2) ;i++)
		{
			adr              =   (i<<3);
			temp0            =   *(ptr + adr    );
			temp1            =   *(ptr + adr +1 );
			temp2            =   *(ptr + adr +2 );
			temp3            =   *(ptr + adr +3 );
			temp5            =   *(ptr + adr +5 );
			temp7            =   *(ptr + adr +7 );
			adr              =   (i<<1)  +  (i<<2);
			*(ptr + adr   )  =   temp1;
			*(ptr + adr +1)  =   temp3;
			*(ptr + adr +2)  =   temp5;
			*(ptr + adr +3)  =   temp7;
			*(ptr + adr +4)  =   temp0;
			*(ptr + adr +5)  =   temp2;
  		}
		ptr += gJpegContext.header.JpgSize.cx + (gJpegContext.header.JpgSize.cx>>1);
	}
}

static void GetYuv400Data(UINT8 *pdat)  //used in mode0  1x1
{	
	UINT16 i, j;
	UINT32 adr;
	UINT8 *ptr = pdat;
	
	for(j=0; j<8;j++)
	{
		adr        = LBUF0BASEADR +  j*LBUF_UNITSIZE;
		V558_ReadSram(adr, ptr, (gJpegContext.header.JpgSize.cx << 1));
		for(i=0; i<gJpegContext.header.JpgSize.cx ;i++)
		{
			*(ptr  + i )     =   *(ptr  + (i<<1)+1);
		}
		ptr  += gJpegContext.header.JpgSize.cx;
	}
}

UINT8 V558_YUV422PointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,PV558_SIZE psize,PV558_SIZE pnewsize,UINT8 encdatatype)
{
	UINT16         i,j,k;
	UINT32         h;
	UINT32         srcadr,desadr;
	V558_SIZE      OldSize,NewSize;
	V558_JPEGBUF   src,dst;
	src            =   *psrc;
	dst            =   *pdst;
	
	OldSize.cx       =  psize->cx;
	OldSize.cy       =  psize->cy; 
	NewSize.cx      =   (((OldSize.cx+15)>>4)<<4);
	NewSize.cy      =   (((OldSize.cy+7)>>3)<<3);
	if( dst.Len < ((OldSize.cx * OldSize.cy)<<1))
		return 0;
	
	if((OldSize.cx != NewSize.cx) || (OldSize.cy != NewSize.cy) )
	{
		for( h=0 ; h < ((NewSize.cx * NewSize.cy)<<1) ; h++)
			*(dst.pData+h) = 0;
	}
	for( k=0 ; k< OldSize.cy ; k++)
	{
		for( i=0 ; i<(OldSize.cx<<1) ; i += (16<<1))
		{
			srcadr = i+ ((k*OldSize.cx)<<1);
			desadr =  (((((k >>3)*NewSize.cx)<<1) + k%8) +i)<<3;
			for( j = 0 ; j<4 ; j++)
			{
				if((i + (j<<2)+1)>= (OldSize.cx<<1))
					break;
				switch(encdatatype)
				{
				case  V558_DATATYPE_YYUV: 
					*(dst.pData +desadr+(j<<1)) = *(src.pData+srcadr + (j <<2) );
					*(dst.pData +desadr+(j<<1)+1) = *(src.pData+srcadr + (j <<2)+1);
					break;
				case  V558_DATATYPE_YUYV:
					*(dst.pData +desadr+(j<<1)) = *(src.pData+srcadr + (j <<2) );
					*(dst.pData +desadr+(j<<1)+1) = *(src.pData+srcadr + (j <<2)+2);
					break;
				case  V558_DATATYPE_UYVY:
					*(dst.pData +desadr+(j<<1)) = *(src.pData+srcadr + (j <<2) +1);
					*(dst.pData +desadr+(j<<1)+1) = *(src.pData+srcadr + (j <<2)+3);
					break;
				default:break;
				}
			}
		}
		for( i=0 ; i<(OldSize.cx<<1) ; i += (16<<1))
		{
			srcadr = i+ 16+((k*OldSize.cx)<<1);
			desadr =  (((((k>>3)*NewSize.cx)<<1) + k%8) +i)<<3;
			desadr += 64;
			for( j = 0 ; j<4 ; j++)
			{
				if((i + 16 + (j<<2)+1)>= (OldSize.cx<<1))
					break;
				switch(encdatatype)
				{
				case  V558_DATATYPE_YYUV: 
					*(dst.pData +desadr+(j<<1)) = *(src.pData+srcadr + (j <<2) );
					*(dst.pData +desadr+(j<<1)+1) = *(src.pData+srcadr + (j <<2)+1);
					break;
				case  V558_DATATYPE_YUYV:
					*(dst.pData +desadr+(j<<1)) = *(src.pData+srcadr + (j <<2) );
					*(dst.pData +desadr+(j<<1)+1) = *(src.pData+srcadr + (j <<2)+2);
					break;
				case  V558_DATATYPE_UYVY:
					*(dst.pData +desadr+(j<<1)) = *(src.pData+srcadr + (j <<2) +1);
					*(dst.pData +desadr+(j<<1)+1) = *(src.pData+srcadr + (j <<2)+3);
					break;
				default:break;
				}
			}
		}
		for( i=0 ; i<(OldSize.cx<<1) ; i += (16<<1))
		{
			srcadr = i+ ((k*OldSize.cx)<<1);
			desadr =  (((((k>>3)*NewSize.cx)<<1) + k%8) +i)<<3;
			desadr += 128;
			for( j = 0 ; j<8 ; j++)
			{
				if((i + (j<<2)+2)>= (OldSize.cx<<1))
					break;
				*(dst.pData +desadr+j) = *(src.pData+srcadr + (j <<2)+1 );
				switch(encdatatype)
				{
				case  V558_DATATYPE_YYUV: 
					*(dst.pData +desadr+j) = *(src.pData+srcadr + (j <<2)+2 );					break;
				case  V558_DATATYPE_YUYV:
					*(dst.pData +desadr+j) = *(src.pData+srcadr + (j <<2)+1 );					break;
				case  V558_DATATYPE_UYVY:
					*(dst.pData +desadr+j) = *(src.pData+srcadr + (j <<2)+0 );					break;
				default:break;
				}				
			}
		}
		for( i=0 ; i<(OldSize.cx<<1) ; i += (16<<1))
		{
			srcadr = i+ ((k*OldSize.cx)<<1);
			desadr =  (((((k>>3)*NewSize.cx)<<1) + k%8) +i)<<3;
			desadr += 192;
			for( j = 0 ; j<8 ; j++)
			{
				if((i + (j<<2)+3)>= (OldSize.cx<<1))
					break;
				switch(encdatatype)
				{
				case  V558_DATATYPE_YYUV: 
					*(dst.pData +desadr+j) = *(src.pData+srcadr + (j <<2)+3 );					break;
				case  V558_DATATYPE_YUYV:
					*(dst.pData +desadr+j) = *(src.pData+srcadr + (j <<2)+3 );					break;
				case  V558_DATATYPE_UYVY:
					*(dst.pData +desadr+j) = *(src.pData+srcadr + (j <<2)+2 );					break;
				default:break;
				}
			}
		}
	}
	pdst->ActLen   =   ((NewSize.cx * NewSize.cy) <<1);
	pnewsize->cx       =    NewSize.cx;   
	pnewsize->cy       =    NewSize.cy;   
	return 1;
}
UINT8 V558_YUV420PointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,PV558_SIZE psize,PV558_SIZE pnewsize)
{
	UINT16 i, j, k;
	UINT32        srcadr,dstadr,h;
	V558_JPEGBUF  src,dst;
	V558_SIZE      OldSize,NewSize;
	OldSize     =      *psize;
	src      =      *psrc;
	dst      =      *pdst;	
	
	NewSize.cx      =   (((OldSize.cx+15)>>4)<<4);
	NewSize.cy      =   (((OldSize.cy+15)>>4)<<4);
	
	if( dst.Len < (((OldSize.cx * OldSize.cy)*3)>>1))
		return 0;
	if((OldSize.cx != NewSize.cx) || (OldSize.cy != NewSize.cy) )
	{
		for( h=0 ; h < (((NewSize.cx * NewSize.cy)*3)>>1) ; h++)
			*(dst.pData+h) = 0;
	}
	
	for( k=0 ; k< OldSize.cy ; k = k+2)
	{
		for( i=0 ; i<(OldSize.cx<<1) ; i += 32)             //first line 'y0'
		{
			srcadr = i+ ((k*OldSize.cx*3)>>1);
			dstadr =  (((((k >>4)*NewSize.cx)*3)<<3) + ((k%8)<<3)+ (i*12));
			if((k%16)>=8)
				dstadr += 128;
			for( j = 0 ; j<4 ; j++)
			{
				//if((i + (j<<2)+1)>= (OldSize.cx<<1))
				//break;
				*(pdst->pData +dstadr+(j<<1))     = *(psrc->pData+srcadr + (j<<2));
				*(pdst->pData +dstadr+(j<<1)+1)   = *(psrc->pData+srcadr + (j<<2)+1);
			}
		}
		for( i=0 ; i<(OldSize.cx<<1) ; i += 32)            //first line'y1'
		{
			srcadr = i+ 16+((k*OldSize.cx*3)>>1);
			dstadr =  (((((k >>4)*NewSize.cx)*3)<<3) + ((k%8)<<3)+ (i*12));
			if((k%16)>=8)
				dstadr += 128;
			dstadr += 64;
			for( j = 0 ; j<4 ; j++)
			{
				*(pdst->pData +dstadr+(j<<1))     = *(psrc->pData+srcadr + (j<<2));
				*(pdst->pData +dstadr+(j<<1)+1)   = *(psrc->pData+srcadr + (j<<2)+1);
			}
		}
		for( i=0 ; i<(OldSize.cx) ; i += 16)             //next line 'y0'
		{
			srcadr = (i+ ((k*OldSize.cx*3)>>1));
			srcadr += (OldSize.cx<<1);
			dstadr =  (((((k >>4)*NewSize.cx)*3)<<3) + (((k+1)%8)<<3)+ (i*24));
			if((k%16)>=8)
				dstadr += 128;
			
			for( j = 0 ; j<8 ; j++)
			{
				//if((i + (j<<2)+2)>= (OldSize.cx<<1))
				//break;
				*(pdst->pData +dstadr+j)   = *(psrc->pData+srcadr + j);
			}
		}
		for( i=0 ; i<(OldSize.cx) ; i += 16)             //next line 'y1'
		{
			srcadr = (i+ 8 + ((k*OldSize.cx*3)>>1));
			srcadr += (OldSize.cx<<1);
			dstadr =  (((((k >>4)*NewSize.cx)*3)<<3) + (((k+1)%8)<<3)+ (i*24));
			if((k%16)>=8)
				dstadr += 128;
			
			dstadr += 64;
			for( j = 0 ; j<8 ; j++)
			{
				//if((i + (j<<2)+2)>= (OldSize.cx<<1))
				//break;
				*(pdst->pData +dstadr+j)   = *(psrc->pData+srcadr + j);
			}
		}
		for( i=0 ; i<(OldSize.cx<<1) ; i += 32)		//first line 'u'
		{
			srcadr = (i+ ((k*OldSize.cx*3)>>1));
			dstadr = (((((k >>4)*NewSize.cx)*3)<<3) + ((k%16)<<2)+ (i*12));
			dstadr += 256;
			for( j = 0 ; j<8 ; j++)
			{
				//if((i + (j<<2)+1)>= (OldSize.cx<<1))
				//break;
				*(pdst->pData +dstadr+j)   = *(psrc->pData+srcadr + (j<<2)+2);
			}
		}
		for( i=0 ; i<(OldSize.cx<<1) ; i += 32)		//first line 'v'
		{
			srcadr = (i+ ((k*OldSize.cx*3)>>1));
			dstadr =  (((((k >>4)*NewSize.cx)*3)<<3) + ((k%16)<<2)+ (i*12));
			dstadr += 320;
			for( j = 0 ; j<8 ; j++)
			{
				//if((i + (j<<2)+1)>= (OldSize.cx<<1))
				//break;
				*(pdst->pData +dstadr+j)   = *(psrc->pData+srcadr + (j<<2)+3);
			}
		}
	}
	pdst->ActLen   =   (((NewSize.cx * NewSize.cy)*3)>>1);
	pnewsize->cx       =    NewSize.cx;   
	pnewsize->cy       =    NewSize.cy;   
	
	return 1;
}

UINT8 V558_YUV411PointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,PV558_SIZE psize,PV558_SIZE pnewsize)
{
	UINT16 i, j, k;
	UINT32        srcadr,dstadr,h;
	V558_JPEGBUF  src,dst;
	V558_SIZE      OldSize,NewSize;
	OldSize     =      *psize;
	src      =      *psrc;
	dst      =      *pdst;	
	
	NewSize.cx      =   (((OldSize.cx+31)>>5)<<5);
	NewSize.cy      =   (((OldSize.cy+7)>>3)<<3);
	
	if( dst.Len < (((OldSize.cx * OldSize.cy)*3)>>1))
		return 0;
	if((OldSize.cx != NewSize.cx) || (OldSize.cy != NewSize.cy) )
	{
		for( h=0 ; h < (((NewSize.cx * NewSize.cy)*3)>>1) ; h++)
			*(dst.pData+h) = 0;
	}
	
	for( k=0 ; k< OldSize.cy ; k++)
	{
		for( i=0 ; i<((OldSize.cx*3)>>1) ; i += (16*3))
		{
			srcadr = i+ ((k*OldSize.cx*3)>>1);
			dstadr =  ((((((k >>3)*NewSize.cx)*3)>>1) + k%8) +i)<<3;
			for( j = 0 ; j<2 ; j++)
			{
				//if((i + (j<<2)+1)>= (OldSize.cx<<1))
				//break;
				*(dst.pData +dstadr+(j<<2))   = *(src.pData+srcadr + (j*6)  );
				*(dst.pData +dstadr+(j<<2)+1) = *(src.pData+srcadr + (j*6)+1);
				*(dst.pData +dstadr+(j<<2)+2) = *(src.pData+srcadr + (j*6)+2);
				*(dst.pData +dstadr+(j<<2)+3) = *(src.pData+srcadr + (j*6)+3);
			}
		}
		for( i=0 ; i<((OldSize.cx*3)>>1) ; i += (16*3))
		{
			srcadr = i+ 12+((k*OldSize.cx*3)>>1);
			dstadr =  ((((((k >>3)*NewSize.cx)*3)>>1) + k%8) +i)<<3;
			dstadr += 64;
			for( j = 0 ; j<2 ; j++)
			{
				//if((i + 16 + (j<<2)+1)>= (OldSize.cx<<1))
				//break;
				*(dst.pData +dstadr+(j<<2))   = *(src.pData+srcadr + (j*6)  );
				*(dst.pData +dstadr+(j<<2)+1) = *(src.pData+srcadr + (j*6)+1);
				*(dst.pData +dstadr+(j<<2)+2) = *(src.pData+srcadr + (j*6)+2);
				*(dst.pData +dstadr+(j<<2)+3) = *(src.pData+srcadr + (j*6)+3);
			}
		}
		for( i=0 ; i<((OldSize.cx*3)>>1) ; i += (16*3))
		{
			srcadr = i+ 24 + ((k*OldSize.cx*3)>>1);
			dstadr =  ((((((k >>3)*NewSize.cx)*3)>>1) + k%8) +i)<<3;
			dstadr += 128;
			for( j = 0 ; j<2 ; j++)
			{
				//if((i + (j<<2)+2)>= (OldSize.cx<<1))
				//break;
				*(dst.pData +dstadr+(j<<2))   = *(src.pData+srcadr + (j*6)  );
				*(dst.pData +dstadr+(j<<2)+1) = *(src.pData+srcadr + (j*6)+1);
				*(dst.pData +dstadr+(j<<2)+2) = *(src.pData+srcadr + (j*6)+2);
				*(dst.pData +dstadr+(j<<2)+3) = *(src.pData+srcadr + (j*6)+3);
			}
		}
		for( i=0 ; i<((OldSize.cx*3)>>1) ; i += (16*3))
		{
			srcadr = i+36+ ((k*OldSize.cx*3)>>1);
			dstadr =  ((((((k >>3)*NewSize.cx)*3)>>1) + k%8) +i)<<3;
			dstadr += 192;
			for( j = 0 ; j<2 ; j++)
			{
				//if((i + (j<<2)+2)>= (OldSize.cx<<1))
				//break;
				*(dst.pData +dstadr+(j<<2))   = *(src.pData+srcadr + (j*6)  );
				*(dst.pData +dstadr+(j<<2)+1) = *(src.pData+srcadr + (j*6)+1);
				*(dst.pData +dstadr+(j<<2)+2) = *(src.pData+srcadr + (j*6)+2);
				*(dst.pData +dstadr+(j<<2)+3) = *(src.pData+srcadr + (j*6)+3);
			}
		}
		for( i=0 ; i<((OldSize.cx*3)>>1) ; i += (16*3))
		{
			srcadr = i + ((k*OldSize.cx*3)>>1);
			dstadr =  ((((((k >>3)*NewSize.cx)*3)>>1) + k%8) +i)<<3;
			dstadr += 256;
			for( j = 0 ; j<8 ; j++)
			{
				//if((i + (j<<2)+2)>= (OldSize.cx<<1))
				//break;
				*(dst.pData +dstadr+j)   = *(src.pData+srcadr + (j*6)+4);
			}
		}
		for( i=0 ; i<((OldSize.cx*3)>>1) ; i += (16*3))
		{
			srcadr = i + ((k*OldSize.cx*3)>>1);
			dstadr =  ((((((k >>3)*NewSize.cx)*3)>>1) + k%8) +i)<<3;
			dstadr += 320;
			for( j = 0 ; j<8 ; j++)
			{
				//if((i + (j<<2)+2)>= (OldSize.cx<<1))
				//break;
				*(dst.pData +dstadr+j)   = *(src.pData+srcadr + (j*6)+5);
			}
		}
		
		
	}
	pdst->ActLen   =   (((NewSize.cx * NewSize.cy)*3)>>1);
	pnewsize->cx       =    NewSize.cx;   
	pnewsize->cy       =    NewSize.cy;   
	
	return 1;
	
}

UINT8 V558_YUV400PointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,PV558_SIZE psize,PV558_SIZE pnewsize)
{
	UINT16 i, j, k;
	UINT32        srcadr,dstadr,h;
	V558_JPEGBUF  src,dst;
	V558_SIZE      OldSize,NewSize;
	OldSize     =      *psize;
	src         =      *psrc;
	dst         =      *pdst;	
	
	NewSize.cx      =   (((OldSize.cx+7)>>3)<<3);
	NewSize.cy      =   (((OldSize.cy+7)>>3)<<3);
	if( dst.Len < ((OldSize.cx * OldSize.cy)))
		return 0;
	if((OldSize.cx != NewSize.cx) || (OldSize.cy != NewSize.cy) )
	{
		for( h=0 ; h < (NewSize.cx * NewSize.cy) ; h++)
			*(dst.pData+h) = 0;
	}
	for( k=0 ; k< OldSize.cy ; k++)
	{
		for( i=0 ; i<OldSize.cx ; i += 8)
		{
			srcadr = i+ k*OldSize.cx;
			dstadr =  ((((k >>3)*NewSize.cx) + k%8) +i)<<3;
			for( j = 0 ; j<8 ; j++)
			{
				//if((i + (j<<2)+1)>= (OldSize.cx<<1))
				//break;
				*(dst.pData +dstadr+j)   = *(src.pData+srcadr + j);
			}
		}
	}	
	pdst->ActLen   =   (NewSize.cx*NewSize.cy);
	pnewsize->cx       =    NewSize.cx;   
	pnewsize->cy       =    NewSize.cy;   
	
	return 1;
	
}
/******************************************************************************
  Description:
	将YUV422/YUV420/YUV411/YUV400的以点来排列的数据转换成以8*8 block排列的数据。
  Parameters:
	1,psrc:输入的数据块。用于存放转换之前的数据。
	2,pdst:输出的数据块。用于存放转换之后的数据。
	3,psize:将要转换的原始数据的图像大小。
	4,pnewsize:经过补数后，数据的图像大小。
	5,YUVType:输入数据的格式。 0:YUV422 ; 1:YUV420 ; 2:YUV411 ; 3:YUV400。
  Returns:
	如果转换成功，返回1，否则，返回0。
  Remarks:
	因为jpeg的图像是由8*8 block的YUV数据压缩后生成的，所以，它对输入要压缩的数据的
	大小有限制。当为YUV422的时候，width是16的倍数，height是8的倍数；当为YUV420的时
	候，width是16的倍数，height是16的倍数；当为YUV411的时候，width是32的倍数，height
	是8的倍数；当为YUV400的时候，width是8的倍数，height是8的倍数。因此，此函数会根据
	不同的YUV输入格式，将不是相应数据倍数的width和height补齐。然后，psize将是相应补齐
	的width和height。
******************************************************************************/
UINT8 V558_YUVPointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,PV558_SIZE psize,PV558_SIZE pnewsize, UINT8 YUVType)
{
	UINT8 value;
	switch(YUVType)
	{
	case V558_YUV_422: value = V558_YUV422PointToBlock(psrc , pdst, psize,pnewsize, V558_DATATYPE_YUYV);break;
	case V558_YUV_420: value = V558_YUV420PointToBlock(psrc , pdst, psize,pnewsize);break;
	case V558_YUV_411: value = V558_YUV411PointToBlock(psrc , pdst, psize,pnewsize);break;
	case V558_YUV_400: value = V558_YUV400PointToBlock(psrc , pdst, psize,pnewsize);break;
	default: break;
	}
	return value;
}

/******************************************************************************
  Description:
	将YUV422/YUV420/YUV411/YUV400的以点来排列的数据转换成以8*8 block排列的数据，其中，当YUV422时，还支持不同的yuv排列。
  Parameters:
	1,psrc:输入的数据块。用于存放转换之前的数据。
	2,pdst:输出的数据块。用于存放转换之后的数据。
	3,psize:将要转换的原始数据的图像大小。
	4,pnewsize:经过补数后，数据的图像大小。
	5,YUVType:输入数据的格式。 0:YUV422 ; 1:YUV420 ; 2:YUV411 ; 3:YUV400。
	6,encdatatype:输入yuv的排列格式。 0:yyuv ; 1:yuyv ; 2:uyvy。
  Returns:
	如果转换成功，返回1，否则，返回0。
  Remarks:
	因为jpeg的图像是由8*8 block的YUV数据压缩后生成的，所以，它对输入要压缩的数据的
	大小有限制。当为YUV422的时候，width是16的倍数，height是8的倍数；当为YUV420的时
	候，width是16的倍数，height是16的倍数；当为YUV411的时候，width是32的倍数，height
	是8的倍数；当为YUV400的时候，width是8的倍数，height是8的倍数。因此，此函数会根据
	不同的YUV输入格式，将不是相应数据倍数的width和height补齐。然后，psize将是相应补齐
	的width和height。
******************************************************************************/
UINT8 V558_YUVTypePointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, PV558_SIZE psize, PV558_SIZE pnewsize, UINT8 YUVType, UINT8 encdatatype)
{
	UINT8 value;
	switch(YUVType)
	{
	case V558_YUV_422: value = V558_YUV422PointToBlock(psrc , pdst, psize,pnewsize, encdatatype);break;
	case V558_YUV_420: value = V558_YUV420PointToBlock(psrc , pdst, psize,pnewsize);break;
	case V558_YUV_411: value = V558_YUV411PointToBlock(psrc , pdst, psize,pnewsize);break;
	case V558_YUV_400: value = V558_YUV400PointToBlock(psrc , pdst, psize,pnewsize);break;
	default: break;
	}
	return value;
}
void _ISR_LbufFull(void)
{
	UINT32 len,adr;
	UINT16 line = 0;
	UINT8  *ptr,temp0,temp1,temp2,temp3;
	UINT16 i,j;
	
	if(!gJpegContext.pcall)
		return;
	
	len = GetYuvLbufSize(&line);
	gJpegContext.decline  +=  line;
	gJpegContext.declinesum  +=  line;
	if(gJpegContext.dst.Len < gJpegContext.dst.ActLen + len)
	{
		gJpegContext.pcall(MSG_READ_YUV, &gJpegContext.dst);
		gJpegContext.dst.ActLen = 0;
	}
	ptr = gJpegContext.dst.pData + gJpegContext.dst.ActLen;
	
	switch(gJpegContext.mode)
	{
	case V558_DECODE_ONLY:
		{
			switch(gJpegContext.header.YUVType)
			{
			case V558_YUV_422:
				GetYuv422Data(ptr);
				break;
			case V558_YUV_420:
				GetYuv420Data(ptr);
				break;
			case V558_YUV_411:
				GetYuv411Data(ptr);
				break;
			case V558_YUV_400:
				GetYuv400Data(ptr);
				break;
			default:
				break;
			}
		}
		break;
	case V558_DECODE_IPP:
	case V558_DECODE_LCDIF:
		{	
			gJpegContext.header.YUVType  =  V558_YUV_422;
			for(j = 0; j < line; j++)
			{
				adr = LBUF1BASEADR;
				adr += j * LBUF_UNITSIZE;
				V558_ReadSram(adr, ptr, gJpegContext.decodewidth << 1);
				for(i=0 ; i< (gJpegContext.decodewidth  >> 1) ; i++)
				{
					adr =  (i<<2);
					switch(gJpegContext.decdatatype)
					{
					case V558_DATATYPE_YYUV:
						temp0        =     *(ptr+adr);
						temp1        =     *(ptr+adr+1);
						temp2        =     *(ptr+adr+2);
						temp3        =     *(ptr+adr+3);
						*(ptr+adr)   =     temp1;
						*(ptr+adr+1) =     temp3;
						*(ptr+adr+2) =     temp0;
						*(ptr+adr+3) =     temp2;
					break;	
					case V558_DATATYPE_YUYV:
						temp0        =     *(ptr+adr);
						*(ptr+adr)   =     *(ptr+adr+1);
						*(ptr+adr+1) =     temp0;
						temp0        =     *(ptr+adr+2);
						*(ptr+adr+2) =     *(ptr+adr+3);
						*(ptr+adr+3) =     temp0;
					break;
					default:	break;
					}
				}
				ptr += (gJpegContext.decodewidth << 1);
			}
		}break;
	}
	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_H, 0);	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_L, 0);
	V558_SetReg((UINT16)V558_REG_BIU_MEM_HIGH_WORD, 0);	
	
	gJpegContext.dst.ActLen += len;
	if(gJpegContext.declinesum  >=  gJpegContext.decMaxlinesum)
	{
		PV558_JPEGCALLBACK pcall;

		pcall = gJpegContext.pcall;
		gJpegContext.pcall = NULL;
		V558_CtrlIntEnable(V558_INT_8BUF_FULL, 0);
		//		gJpegContext.declinesum  =   0;
		gJpegContext.State = V558_JPEG_INITED;
		pcall(MSG_READ_YUV_DONE, &gJpegContext.dst);
	}
	//if(gJpegContext.decline  >=  gJpegContext.decMaxline)
	//{
	//	gJpegContext.pcall(MSG_READ_YUV, &gJpegContext.dst);
	//	//gJpegContext.pcall = NULL;
	//	//V558_CtrlIntEnable(V558_INT_8BUF_FULL, 0);
	//	gJpegContext.decline  =   0;
	//}
	else
		V558_JpegRestartDecoder();
	
}




static void WriteQT(void)
{
	UINT8 i, j;
	
	if(gJpegContext.header.QTCount == 0)
		return;
	
	V558_JpegStartWrQTable();
	for(i = 0; i < gJpegContext.header.QTCount; i++)
	{
		for(j = 0; j < 64; j++)
			V558_JpegWriteQtable(gJpegContext.header.QT[i][j]);
	}
}

static void WriteHT(void)
{
	UINT8 i, j;
	
	if(gJpegContext.header.HTCount == 0)
		return;
	
	V558_JpegStartWrHTable();
	for(i = 0; i < gJpegContext.header.HTCount; i++)
	{
		V558_JpegSelectHTable(i);
		for(j = 0; j < gJpegContext.header.HTLen[i]; j++)
			V558_JpegWriteHtable(gJpegContext.header.HT[i][j]);
	}
}

static void WriteComponents(void)
{
	UINT8 i;
	
	if( (gJpegContext.header.QTCount == 0) || (gJpegContext.header.HTCount == 0) )
		return;
	
	V558_JpegSetComponentNum(gJpegContext.header.CompCount | (gJpegContext.header.QTCount << 4));
	for(i = 0; i < 4; i++)
		V558_JpegSetComponentParm(i, gJpegContext.header.Comp[i]);
}

//====================================================
//
//		Layer 3 function of Jpeg part
//
//====================================================

static void EncodeJpg(UINT8 mode, PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall)
{
	V558_SIZE	size;
	V558_RECT	rect;
	UINT32		vwc;
	UINT8		ratio;
	
	gJpegContext.mode = mode;
	gJpegContext.buf = *pbuf;
	//memcpy(gJpegContext.buf, pbuf, sizeof(V558_JPEGBUF));
	gJpegContext.pcall = pcall;
	pbuf->ActLen = 0;
	
	V558_LbufSetYuvMode(0);
	V558_CtrlSwReset(V558_SW_RESET_JPG);
	switch(gJpegContext.mode)
	{
	case V558_CAPTURE_FRAME:
	case V558_CAPTURE_FRAMEVIDEO:
		V558_LcdGetALDisRect(&rect);
		size.cx = rect.width;
		size.cy = rect.height;
		V558_LbufSetWorkmode(4);
		break;
		
	case V558_CAPTURE_STILLBRC:
	case V558_CAPTURE_STILLBRC_THUMBNAIL:
	case V558_CAPTURE_VIDEO:
		V558_IppSelectPrefilter(1);
		V558_IppGetCapSize(&size);
		V558_LbufSetWorkmode(0);
		break;
	default:
		return;
	}
	
	V558_JpegSetImageSize(size);
	size.cx += 15;
	size.cx >>= 4;
	size.cx <<= 4;
	size.cy += 7;
	size.cy >>= 3;
	size.cy <<= 3;
	V558_LbufSetImageSize(size);
	vwc = size.cx;
	vwc *= size.cy;
	vwc >>= 1;
	V558_JpegSetVWC(vwc);
	
	if(gJpegContext.entcr)
	{
		if(gJpegContext.tcr > 5)
		{
			ratio = gJpegContext.tcr - 1;
			ratio >>= 2;
			vwc /= ratio;
			V558_JpegSetTWC(vwc);
			gJpegContext.entcr = 1;
		}
	}
	else
	{
		if(gJpegContext.twc)
		{
			vwc /= gJpegContext.twc;
			vwc <<= 2;
			vwc += 1;
			V558_JpegSetTCR((UINT8)vwc);
			gJpegContext.entcr = 0;
		}
	}
	
	V558_CtrlIntEnable(V558_INT_CAP_DONE, 1);
	V558_CtrlIntEnable(V558_INT_CAP_FIFO_FULL, 1);
	gJpegContext.State = V558_JPEG_ISWORKING;
}
/******************************************************************************
Description:
用于capture still。
Parameters:
1,pbuf:输出的数据块。用于存放采集到的图像数据。
2,pcall:回调指针。
Returns:
Null
Remarks:
NULL
Example:
UINT8 g_dataBuf[0x10000];
UINT32	g_bufoffset;
void TestCaptureStill(void)
{
V558_JPEGBUF g_buf;
g_buf.ActLen = 0;
g_buf.Len = 0x10000;
g_buf.pData = g_dataBuf;
g_bufoffset = 0;
V558_CaptureStill(&g_buf, JpegNote);
}
******************************************************************************/
void V558_CaptureStill(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall)
{
	//V558_IppSelectPrefilter(1);
	gJpegContext.qf = V558_JpegGetQfactor();
	V558_CtrlDisableBRC();
	
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);
	EncodeJpg(V558_CAPTURE_STILLBRC, pbuf, pcall);
	V558_CtrlSetWorkMode(V558_MODE_CAP_STILL);	//start capture
}
/******************************************************************************
Description:
用于capture still with thumbnail。558芯片除了生成一个普通静止图片以外，
还可以生成一个40*30的缩略图(thumbnail)，因此，如果是调用此函数，那么采
集的图像大小只能是4:3的。
Parameters:
1,pbuf:输出的数据块。用于存放采集到的图像数据(还没有加上thumbnail)。
2,pcall:回调指针。
Returns:
Null
Remarks:
Null
Example:
UINT8 g_dataBuf[0x10000];
UINT32	g_bufoffset;
void TestCaptureStillThumb(void)
{
V558_JPEGBUF g_buf;
g_buf.ActLen = 0;
g_buf.Len = 0x10000;
g_buf.pData = g_dataBuf;
g_bufoffset = 0;
V558_CaptureStillThumb(&g_buf, JpegNote);
}
******************************************************************************/
void V558_CaptureStillThumb(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall)
{
//	V558_IppSelectPrefilter(1);
	gJpegContext.qf = V558_JpegGetQfactor();
	V558_CtrlDisableBRC();
	EncodeJpg(V558_CAPTURE_STILLBRC_THUMBNAIL, pbuf, pcall);
	V558_IppSetThumb(gJpegContext.thumbwidth);
	V558_IppGetDispSize(&gJpegContext.thumb);
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE - DEFAULT_THUMB_SIZE - 1);
	V558_JbufSetThumbAddr(DEFAULT_THUMB_ADDR);
	V558_JbufSetThumbSize(DEFAULT_THUMB_SIZE-1);
	V558_CtrlSetWorkMode(V558_MODE_CAP_STILL_THUMB);
}
/******************************************************************************
Description:
用于开始Capture still with frame。
Parameters:
1,pbuf:输出的数据块。用于存放采集到的图像数据。
2,pcall:回调指针。
Note:
NULL
Returns:
Null
Remarks:
Null
Example:
UINT8 g_DataBuf[0x10000];
UINT32	g_StillBufOffset;
void TestCaptureStillFrame(void)
{
V558_JPEGBUF g_JpegBuf;
g_JpegBuf.ActLen = 0;
g_JpegBuf.Len = 0x8000;
g_JpegBuf.pData = g_DataBuf;
g_StillBufOffset = 0;
V558_CaptureFrame(&g_JpegBuf, JpegNote);
}
******************************************************************************/
void V558_CaptureFrame(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall)
{
	gJpegContext.qf = V558_JpegGetQfactor();
	V558_CtrlDisableBRC();
	
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);
	EncodeJpg(V558_CAPTURE_FRAME, pbuf, pcall);
	gJpegContext.lcdctrl = V558_LcdDisableGamma();
	V558_CtrlSetWorkMode(V558_MODE_CAP_STILL_FRAM);
}
/******************************************************************************
Description:
用于开始Capture video with frame。
Parameters:
1,pbuf:输出的数据块。用于存放采集到的图像数据。
2,pcall:回调指针。
Note:
NULL
Returns:
Null
Remarks:
Null
Example:
UINT8 g_DataBuf[0x10000];
UINT32	g_StillBufOffset;
void TestCaptureVideoFrame(UINT8 bFrm)
{
V558_JPEGBUF g_JpegBuf;
g_JpegBuf.ActLen = 0;
g_JpegBuf.Len = 0x8000;
g_JpegBuf.pData = g_DataBuf;
g_StillBufOffset = 0;
V558_CaptureFrameVideo(&g_JpegBuf, JpegNote);
}
******************************************************************************/
void V558_CaptureFrameVideo(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall)
{
	gJpegContext.qf = V558_JpegGetQfactor();
	V558_CtrlDisableBRC();
	
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);
	EncodeJpg(V558_CAPTURE_FRAMEVIDEO, pbuf, pcall);
	gJpegContext.lcdctrl = V558_LcdDisableGamma();
	V558_CtrlSetWorkMode(V558_MODE_CAP_VIDEO_FRAM);
	V558_JbufSetCapViewConf(0x4);
}
/******************************************************************************
Description:
用于开始capture video。
Parameters:
1,pbuf:输出的数据块。用于存放采集到的图像数据。
2,pcall:回调指针。
Returns:
Null
Remarks:
Null
Example:
UINT8 g_dataBuf[0x10000];
UINT32	g_bufoffset;
void TestCaptureVideo(void)
{
V558_JPEGBUF g_buf;
g_buf.ActLen = 0;
g_buf.Len = 0x10000;
g_buf.pData = g_dataBuf;
g_bufoffset = 0;
V558_StartCapVideo(&g_buf, JpegNote);
}
******************************************************************************/
void V558_StartCapVideo(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall)
{
//	V558_IppSelectPrefilter(1);
	gJpegContext.qf = V558_JpegGetQfactor();
	V558_CtrlDisableBRC();
	
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);
	EncodeJpg(V558_CAPTURE_VIDEO, pbuf, pcall);
	V558_CtrlSetWorkMode(V558_MODE_CAP_VIDEO);
	V558_JbufSetCapViewConf(0x4);
}
/******************************************************************************
Description:
用于结束capture video。
Parameters:
NULL
Note:
在开始capture video之后，调用此函数即可停止capture video。
Returns:
Null
Remarks:
Null
******************************************************************************/
void V558_StopCapVideo(void)
{
	V558_JbufSetCapViewConf(0x8);
}



void V558_SetThumbWidth(UINT16 width)
{
	gJpegContext.thumbwidth = width;
}

UINT16 V558_GetThumbWidth(void)
{
	return gJpegContext.thumbwidth;
}

static UINT16 GetCompFrmSOF(UINT8 *buf)
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

static UINT16 GetSOF0(PV558_JPEG_INDEX pinfo, UINT8 * ptr)
{
	UINT8 len = ((UINT16)ptr[0] << 8) + (UINT16)ptr[1];
	UINT32 temp;
	
	pinfo->ImageSize.cy = ((UINT16)ptr[3] << 8) + (UINT16)ptr[4];
	pinfo->ImageSize.cx = ((UINT16)ptr[5] << 8) + (UINT16)ptr[6];
	pinfo->CompCount = ptr[7];
	if(pinfo->CompCount == 1)
	{
		pinfo->YUVType = V558_YUV_400;
		pinfo->Comp[ ptr[8]-1 ] = GetCompFrmSOF(ptr+9);;
		pinfo->JpgSize.cx = ((pinfo->ImageSize.cx + 7) >> 3) << 3;
		pinfo->JpgSize.cy = ((pinfo->ImageSize.cy + 7) >> 3) << 3;
		pinfo->vwc = pinfo->JpgSize.cx;
		pinfo->vwc *= pinfo->JpgSize.cy;
		pinfo->vwc >>= 2;
		pinfo->blkcount = (UINT8)(pinfo->JpgSize.cx >> 3);
	}
	else
	{
		pinfo->Comp[ ptr[8]-1 ] = GetCompFrmSOF(ptr+9);
		if(ptr[9] == 0x21)
		{
			pinfo->YUVType = V558_YUV_422;
			pinfo->JpgSize.cx = ((pinfo->ImageSize.cx + 15) >> 4) << 4;
			pinfo->JpgSize.cy = ((pinfo->ImageSize.cy + 7) >> 3) << 3;
			
			pinfo->vwc = pinfo->JpgSize.cx;
			pinfo->vwc *= pinfo->JpgSize.cy;
			pinfo->vwc >>= 1;
			pinfo->blkcount = (UINT8)(pinfo->JpgSize.cx >> 2);
		}
		else if(ptr[9] == 0x22)
		{
			pinfo->YUVType = V558_YUV_420;
			pinfo->JpgSize.cx = ((pinfo->ImageSize.cx + 15) >> 4) << 4;
			pinfo->JpgSize.cy = ((pinfo->ImageSize.cy + 15) >> 4) << 4;
			
			temp = pinfo->JpgSize.cx;
			temp *= pinfo->JpgSize.cy;
			pinfo->vwc = (temp >> 3) + (temp >> 2);
			pinfo->blkcount = (UINT8)((pinfo->JpgSize.cx >> 2) + (pinfo->JpgSize.cx >> 3));
		}
		else if(ptr[9] == 0x41)
		{
			pinfo->YUVType = V558_YUV_411;
			pinfo->JpgSize.cx = ((pinfo->ImageSize.cx + 31) >> 5) << 5;
			pinfo->JpgSize.cy = ((pinfo->ImageSize.cy + 7) >> 3) << 3;
			
			temp = pinfo->JpgSize.cx;
			temp *= pinfo->JpgSize.cy;
			pinfo->vwc = (temp >> 3) + (temp >> 2);
			pinfo->blkcount = (UINT8)( (pinfo->JpgSize.cx >> 4) + (pinfo->JpgSize.cx >> 3) );
		}
		else
			return 0;
		pinfo->Comp[ ptr[11]-1 ] = GetCompFrmSOF(ptr+12);
		pinfo->Comp[ ptr[14]-1 ] = GetCompFrmSOF(ptr+15);
	}
	pinfo->LbufSize.cx = pinfo->JpgSize.cx;
	pinfo->LbufSize.cy = pinfo->JpgSize.cy;
	
	switch(pinfo->YUVType)
	{
	case  V558_YUV_422:
		//if(pinfo->LbufSize.cx < 160)
		//	pinfo->LbufSize.cx = 204;
		if(pinfo->LbufSize.cx < 408)
			pinfo->LbufSize.cx = 408;
		break;
	case  V558_YUV_420:
		if(pinfo->LbufSize.cx < 160)
			pinfo->LbufSize.cx = 220;
		else if(pinfo->LbufSize.cx < 440 )
			pinfo->LbufSize.cx = 440;  //464，出现了fifo warning ,图像左右颠倒
		//466，出现了变绿的情况
		break;
	case  V558_YUV_411:
		if(pinfo->LbufSize.cx < 160)
			pinfo->LbufSize.cx  =  272;
		else if(pinfo->LbufSize.cx < 544)
			pinfo->LbufSize.cx = 544;
		break;
	case  V558_YUV_400:
		if(pinfo->LbufSize.cx < 240)
			pinfo->LbufSize.cx = 300;
		else if(pinfo->LbufSize.cx < 600)
			pinfo->LbufSize.cx = 600;
		break;
	default:
		break;
	}
	return len;
}

static UINT16 GetDQT(PV558_JPEG_INDEX pinfo, UINT8 * ptr)
{
	UINT16 len, i = 0;
	UINT8	n;
	
	len = ((UINT16)ptr[0] << 8) + (UINT16)ptr[1];
	
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

static UINT16 GetDHT(PV558_JPEG_INDEX pinfo, UINT8 * ptr)
{
	UINT16 len, j = 0, n, k, HTLen;
	UINT8 *pd = ptr+2;	
	
	len = ((UINT16)ptr[0] << 8) + (UINT16)ptr[1];
	while(j < len-3)
	{
		HTLen = 0;
		n = pd[j] & 0xf;
		k = pd[j] & 0xf0;
		n += k >> 3;
		j++;
		pinfo->HT[n] = pd+j;
		k = j + 16;
		while(j < k)
		{
			HTLen += pd[j];
			j++;
		}
		pinfo->HTLen[n] = 16 + HTLen;
		j += HTLen;
		pinfo->HTCount++;
	}
	return len;
}

static UINT16 GetSOS(PV558_JPEG_INDEX pinfo, UINT8 * ptr)
{
	UINT16 len;
	UINT8 i, k, n, ac, dc, count, *pbuf = ptr+3;	
	
	len = ((UINT16)ptr[0] << 8) + (UINT16)ptr[1];
	count = ptr[2];
	if(len != 6 + (count << 1))
		return 0;
	
	for(i = 0; i < count; i++)
	{
		k = i << 1;
		n = pbuf[k]-1;
		ac = pbuf[k+1] & 0xf;
		dc = pbuf[k+1] & 0xf0;
		if(!ac)
			pinfo->Comp[n] |= 0x2;
		if(!dc)
			pinfo->Comp[n] |= 0x1;
	}
	return len;
}
/*********************************************************************
Description:
	Get the information of jfif segment.
Parameters:
	pinfo:	The pointer of jpeg information.
	ptr:	The pointer of input data buffer.
	point:   The position of jfif segment.
Note:
	NULL
Returns:
	Return the thumbnail's length.
Remarks:
	NULL	
**********************************************************************/
static UINT16 Get_JfifInfo(PV558_JFIFINFO pjfifinfo, UINT8* ptr, UINT32 point)
{
	UINT32  i=0;
	UINT16 len;
	UINT8 isJFIF,isJFXX;
	i      =   point;
	len    =   (*(ptr+point)<<8)  +  *(ptr+point+1);
	i     +=    2;
	pjfifinfo->fieldLen =  len;
	if(*(ptr+i) == 0x4A && *(ptr+i+1) == 0x46 
		&& *(ptr+i+2) == 0x49 && *(ptr+i+3) == 0x46
		&& *(ptr+i+4) == 0)
		isJFIF = 1;
	else
		isJFIF = 0;
	if(*(ptr+i) == 0x4A && *(ptr+i+1) == 0x46 
		&& *(ptr+i+2) == 0x58 && *(ptr+i+3) == 0x58
		&& *(ptr+i+4) == 0)
		isJFXX = 1;
	else 
		isJFXX = 0;
		i     +=   5;
	if(isJFIF)
	{
		pjfifinfo->version   = (*(ptr+i)<<8) + *(ptr+i+1);
		i     +=   2;
		pjfifinfo->units     = *ptr;
		i     ++;
		pjfifinfo->xdensity  = (*(ptr+i)<<8) + *(ptr+i+1);
		i     +=   2;
		pjfifinfo->ydensity  = (*(ptr+i)<<8) + *(ptr+i+1);
		i     +=   2;
		pjfifinfo->xthumbnail  = *(ptr+i);
		i     +=   1;
		pjfifinfo->ythumbnail  = *(ptr+i);
		i     +=   1;
		if(isJFIF)
			pjfifinfo->thumblen  =  len - 0x10;
		if(pjfifinfo->thumblen)
			pjfifinfo->isThumb  =  1;
		pjfifinfo->thumbformat = 2;
		pjfifinfo->pthumbbuf  =  ptr + point + 0x10;
	}
	if(isJFXX)
	{
		pjfifinfo->extensioncode  =  *(ptr+i);
		switch(pjfifinfo->extensioncode)
		{
			case 0x10:	pjfifinfo->thumbformat = 0;
					pjfifinfo->pthumbbuf   = ptr + point + 0x8;
					pjfifinfo->thumblen    = len - 8;
					break;
			case 0x11:	pjfifinfo->thumbformat = 1;
					pjfifinfo->xthumbnail  = *(ptr+i+1);
					pjfifinfo->ythumbnail  = *(ptr+i+2);
					pjfifinfo->pthumbbuf   = ptr+point +0xa;
					pjfifinfo->thumblen    = len - 0xa;
					break;
			case 0x13:	pjfifinfo->thumbformat = 2;
					pjfifinfo->xthumbnail  = *(ptr+i+1);
					pjfifinfo->ythumbnail  = *(ptr+i+2);
					pjfifinfo->pthumbbuf   = ptr+point +0xa;
					pjfifinfo->thumblen    = len - 0xa;
					break;
			default: break;
	}
		if(pjfifinfo->thumblen)
			pjfifinfo->isThumb  = 1;
	}
	return len;
}
/******************************************************************************
Description:
	Change thumb rgb8 to rgb555 or rgb565.
Parameters:
	dst:	The dst buffer.Store rgb555 or rgb565 data.
	src:	The input buffer.
	xthumb:	The width of thumb.
	ythumb:	The height of thumb.
	rawformat:The rawformat. rgb565:1  ;  rgb555:2
Returns:	
	return the lenght of dst buffer.
Notes:
	NULL
Remarks:
	NULL
*******************************************************************************/
UINT16 V558_ConvertThumbRGB8ToRaw(UINT8* dst, UINT8* src, UINT8 xthumb, UINT8 ythumb,UINT8 rawformat)
{
	UINT8* rgbsrc;
	UINT8  rgbdata;
	RawType raw;
	UINT8  i,j;
	rgbsrc  =  src + 0x300;
	for(j=0 ; j<ythumb ; j++)
	{
		for(i=0 ; i<xthumb ; i++)
		{
			rgbdata = *(rgbsrc + j*xthumb + i);
			raw     = RGB8ToRaw(src, rgbdata, rawformat);
			*(dst + ((i+j*xthumb)<<1)) = raw.high;
			*(dst + ((i+j*xthumb)<<1)+1) = raw.low;
		}
	}
	return  xthumb*ythumb*2;
}
/******************************************************************************
Description:
用于解析jpeg图像。
Parameters:
1,pbuf:输入的数据块。用于存放被解析的jpeg图像。
2,pinfo:用于存放jpeg图像的信息。他是jpeg的全局变量指针。
Note:
This function is used for interrupt
Returns:
如果返回0xff，表明解析Fail，如果返回0x0,表明解析success。
Remarks:
Null
Example:
UINT8 g_dataBuf[0x10000];
UINT32	g_StillBufOffset;
V558_JPEG_INDEX info;
UINT8 TestJpegParse(void)
{
UINT8   value;
V558_JPEGBUF g_JpegBuf;
g_JpegBuf.ActLen = 0;
g_JpegBuf.Len    = 0x8000;
g_JpegBuf.pData  = g_DataBuf;
g_StillBufOffset = 0;
value            = V558_JpegParse(&g_JpegBuf, &info);	
return           value;
}
******************************************************************************/
UINT8 V558_JpegParse(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo)
{
	UINT32 i = 0,j=0, length = pbuf->ActLen;
	UINT16 len;
	UINT8 *ptr = pbuf->pData;
	pinfo->flagindex   =   0;
	pinfo->isThumb     =   0;
	memset(pinfo, 0, sizeof(V558_JPEG_INDEX));
	pinfo->YUVType = V558_YUV_UNKNOWN;
	while(i < length)
	{
		if(ptr[i] == 0xff)
		{
			do{
				i++;
			} while(ptr[i] == 0xff);
			
			switch(ptr[i])
			{
			case 0xE1:               //thumbnail segment.
				pinfo->thumbstartpos  =  i-1;
				pinfo->thumblen       = (*(ptr + i +1)<<8) + *(ptr + i +2);
				pinfo->isThumb               = 1;
				i += (pinfo->thumblen-2);   //jump thumbnail segment.
				
				break;
			case 0xE0:
				len = Get_JfifInfo(&pinfo->jfifinfo, ptr, i+1);
				i  += len;
				break;
			case 0xd8:		//jpeg start header
				pinfo->startpos = i-1;
				//i++;
				break;
			case 0xc0:		//segment SOF0
				len = GetSOF0(pinfo, ptr+i+1);
				i += len;
				break;
			case 0xdb:		//segment DQT
				len = GetDQT(pinfo, ptr+i+1);
				i += len;
				break;
			case 0xc4:		//segment DHT
				len = GetDHT(pinfo, ptr+i+1);
				i += len;
				break;
			case 0xda:		//segment SOS
				len = GetSOS(pinfo, ptr+i+1);
				if(len)
				{
					i += len;
					pinfo->offset = i+1;
				}
				break;
			case 0xd9:
				pinfo->endpos = i+1;
				return SUCCEED;
			default:
				j = i - pinfo->offset;
				if( ( j >= 4 ) && (j % 4 == 0) )
				{
					if((ptr[i-4] & 0xf8) == 0xd0)
					{
						pinfo->restartflag = 1;
						pinfo->flagadd[pinfo->flagindex] = i - pinfo->offset-4;
						pinfo->flagindex ++;
					}
				}
				break;
			}
		}
		i++;
	}
	return JPEGFAILED;
}

/******************************************************************************
Description:

	Jfif parse.
Parameters:
	pbuf:jpeg buffer.
	pinfo:jfif information
Note:	
	NULL
Returns:
如果返回0xff，表明解析Fail，如果返回0x0,表明解析success。
Remarks:
Null
******************************************************************************/
UINT8 V558_JfifParse(PV558_JPEGBUF pbuf, PV558_JFIFINFO pinfo)
{
	UINT32 i = 0, length = pbuf->ActLen;
	UINT16 len;
	UINT8 *ptr = pbuf->pData;
	pinfo->isThumb    =   0;
	memset(pinfo, 0, sizeof(V558_JFIFINFO));
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
				len = Get_JfifInfo(pinfo, ptr, i+1);
				i  += len;
				break;
			case 0xd8:		//jpeg start header
				//pinfo->startpos = i-1;
				//i++;
				//break;
			default:	break;
			}
		}
		i++;
	}
	return JPEGFAILED;
}


/************************************************************************
Description:
	Get the jpg info,and decide whether it has thumbnail or not.
Parameters:
	pbuf: the buffer of jpeg file.
	pinfo:the information of jpeg file.
Notes:
	NULL
Return:
	if 0,haven't thumbnail,if 1,have thumbnail.
Remark:
	NULL
*************************************************************************/
UINT8 V558_IsThumb(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo)
{
	V558_JpegParse( pbuf, pinfo);
	return pinfo->isThumb;
}

//========================================================
//
//		Debug Jpeg Header Info
//
//========================================================
UINT8 WriteDecodeData(UINT8 *buf, UINT32 len)
{
	UINT16 value2, value1, value0, flag;

	V558_WriteSram(JBUF_BASEADD, buf, len);

	value0   =   V558_JbufGetWritePointer();
	value1   =   V558_JbufGetReadPointer();
	while(value0 != value1)
	{
		value2 = value1;
		flag = 0xff;
		do{
			if(V558_LbufGetStatus() & 0x3)
				_ISR_LbufFull();
			value1   =   V558_JbufGetReadPointer();
		} while((value1 == value2) && (--flag));
		if(flag == 0)
			return 1;
	}
	return 0;
}

UINT8 SendDecodeData(UINT8 *buf, UINT32 len)
{
#if 0
	UINT32 cnt, i;
	UINT8 *pbuf = buf;
	UINT8	bus;
	
	bus = V558_CtrlGetBusType();
	if( (bus != 1) && (gJpegContext.mode != V558_DECODE_ONLY) )
	{
		cnt = len >> 2;
		for(i = 0; i < cnt; i++, pbuf += 4)
		{
			if(WriteDecodeData(pbuf, 4))
				return 1;
		}
		return 0;
	}
#endif
	return WriteDecodeData(buf, len);
}

UINT8 HostSendJpgData(void)
{
	UINT32 len, len1,i;
	UINT8 *pbuf, dat[4] = {0xff, 0xff, 0xff, 0xd9};
	
	len = gJpegContext.header.endpos - gJpegContext.header.offset;
	len1 = (len >> 2) << 2;
	pbuf = gJpegContext.buf.pData + gJpegContext.header.offset;
	//V558_CtrlIntEnable(V558_INT_8BUF_FULL, 1);
	pbuf[len1 - 1] = 0xd9;
	pbuf[len1 - 2] = 0xff;
	len = len1;
	
	if(gJpegContext.header.restartflag == 1)
	{
		V558_JpegStartDecoder();
		if( SendDecodeData(pbuf, gJpegContext.header.flagadd[0]+4) )
			return JPEGFAILED;
		for( i=0 ; i<gJpegContext.header.flagindex-1 ; i++)
		{
			if( SendDecodeData(pbuf+gJpegContext.header.flagadd[i]+4, 
				gJpegContext.header.flagadd[i+1]-gJpegContext.header.flagadd[i]) )
				return JPEGFAILED;
		}
		if( SendDecodeData(pbuf+gJpegContext.header.flagadd[i]+4, 
			len1-gJpegContext.header.flagadd[i]-4) )
			return JPEGFAILED;
		
		if( (len == len1+1) || (len == len1+2) )
			SendDecodeData(dat, 4);
		else if(len == len1+3)
		{
			dat[0] = pbuf[len1];
			SendDecodeData(dat, 4);
		}
	}
	if(gJpegContext.header.restartflag == 0)
	{
		V558_JpegStartDecoder();
		if( SendDecodeData(pbuf, len1) )
			return JPEGFAILED;
		
		if( (len == len1+1) || (len == len1+2) )
			SendDecodeData(dat, 4);
		else if(len == len1+3)
		{
			dat[0] = pbuf[len1];
			SendDecodeData(dat, 4);
		}
	}
	//flag = 0x400;
	//while(flag--)
	V558_Delay(1);
	{
		if(V558_LbufGetStatus() & 0x3)
		{
			_ISR_LbufFull();
			return SUCCEED;
		}
	}
	
	return SUCCEED;
}

UINT8 SendDisplayData(UINT8 *buf, UINT32 len)
{
	UINT8 *pbuf = buf;
	UINT16 value2, value1, value0, flag;
	
	V558_WriteSram(JBUF_BASEADD, pbuf, len);
	value0   =   V558_JbufGetWritePointer();
	value1   =   V558_JbufGetReadPointer();
	while(value0 != value1)
	{
		value2 = value1;
		flag = 0xff;
		do{
			value1   =   V558_JbufGetReadPointer();
		} while((value1 == value2) && (--flag));
		if(flag == 0)
			return 1;
	}
	return 0;
}

void DecreaseClk(void)
{
	UINT32 clkin;
	UINT8  factorh,factorl;
	UINT16	temp;

	if(gJpegContext.clkflag == 0)
	{
		gJpegContext.wcy = V558_GetPanelWcy();
		temp = ((((gJpegContext.wcy&0x7c00)+0x400) >> 1) & 0x7c00) | 
			((((gJpegContext.wcy & 0x3e0)+0x20) >> 1) & 0x3e0) | 
			((((gJpegContext.wcy&0x1f)+1) >> 1) & 0x1f);
		V558_SetPanelWcy(temp);

		V558_CtrlGetChipClk(&clkin, &gJpegContext.chipoutclk, &factorh, &factorl);
		temp     =  (gJpegContext.chipoutclk>>1);
		V558_CtrlChangeChipClk(temp);
		gJpegContext.clkflag = 1;
		temp = 0xff;
		while(temp--);
	}
}

UINT8 DecodeProcess(void)
{
	UINT32 len, len1, i;
	UINT8 *pbuf, dat[4] = {0xff, 0xff, 0xff, 0xd9};

	len = gJpegContext.header.endpos - gJpegContext.header.offset;
	len1 = (len >> 2) << 2;
	pbuf = gJpegContext.buf.pData + gJpegContext.header.offset;
	pbuf[len1 - 1] = 0xd9;
	pbuf[len1 - 2] = 0xff;
	len = len1;
	if(gJpegContext.header.restartflag == 1)
	{
		V558_JpegStartDecoder();
#if 1
		DecreaseClk();
#endif
		
		if(SendDisplayData(pbuf, gJpegContext.header.flagadd[0]+4))
			return JPEGFAILED;
		for( i=0 ; i<gJpegContext.header.flagindex-1 ; i++)
		{
			if( SendDisplayData(pbuf+gJpegContext.header.flagadd[i]+4, 
				gJpegContext.header.flagadd[i+1]-gJpegContext.header.flagadd[i]) )
				return JPEGFAILED;
		}
		if( SendDisplayData(pbuf+gJpegContext.header.flagadd[i]+4, 
			len1-gJpegContext.header.flagadd[i]-4) )
			return JPEGFAILED;
		
		if( (len == len1+1) || (len == len1+2) )
		{
			if(SendDisplayData(dat, 4))
				return JPEGFAILED;
		}
		else if(len == len1+3)
		{
			dat[0] = pbuf[len1];
			if(SendDisplayData(dat, 4))
				return JPEGFAILED;
		}
	}
	
	if(gJpegContext.header.restartflag == 0)
	{
		//=======================
//if arm is fast
#if 1
		DecreaseClk();
#endif
		V558_JpegStartDecoder();
//===========================================

		V558_WriteSram(JBUF_BASEADD, pbuf, len1);
		if( (len == len1+1) || (len == len1+2) )
			V558_WriteSram(JBUF_BASEADD, dat, 4);
		else if(len == len1+3)
		{
			dat[0] = pbuf[len1];
			V558_WriteSram(JBUF_BASEADD, dat, 4);
		}
// if arm is slow
#if 0
		DecreaseClk();
		V558_JpegStartDecoder();
#endif
	}
	return SUCCEED;
}

static void StartDecodeJpg(UINT8 mode, PV558_LAYER_PROPERTY proty)
{
	V558_POINT pt = {0, 0};
	
	V558_CtrlSetWorkMode(V558_MODE_DISPLAY);
	V558_CtrlDisableBRC();
	
	gJpegContext.pcall = NULL;
	gJpegContext.mode = mode;
	
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);
	
	V558_CtrlSwReset(V558_SW_RESET_IPP);
	V558_CtrlSwReset(V558_SW_RESET_JPG);
	
	V558_IppDisplay(gJpegContext.header.LbufSize, pt, 
		gJpegContext.header.ImageSize, proty->Size.cx);
	V558_IppGetDispSize(&(proty->Size));
	V558_LcdSetALProperty(proty);
	
	V558_JpegSetWorkMode(9);
	V558_JpegSetVWC(gJpegContext.header.vwc);
	V558_JpegSetBlockNum(gJpegContext.header.blkcount);
	
	V558_LbufSetYuvMode(gJpegContext.header.YUVType);
	V558_LbufSetImageSize(gJpegContext.header.LbufSize);
}

UINT8 DecodeOneFrame(PV558_JPEGBUF pbuf)
{
	gJpegContext.buf = *pbuf;
	WriteQT();
	WriteHT();
	WriteComponents();
	
	gJpegContext.State = V558_JPEG_ISWORKING;
	return DecodeProcess();			//
}

void V558_StartDispVideo(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo, 
						 PV558_LAYER_PROPERTY proty)
{
	if(pinfo->YUVType < V558_YUV_UNKNOWN)
	{
		memcpy(&gJpegContext.header, pinfo, sizeof(V558_JPEG_INDEX));
		StartDecodeJpg(V558_DISPLAY_VIDEO, proty);
		DecodeOneFrame(pbuf);
	}
}

void V558_DispOneFrame(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo)
{
	if(pinfo->YUVType < V558_YUV_UNKNOWN)
	{
		memcpy(&gJpegContext.header, pinfo, sizeof(V558_JPEG_INDEX));
		//		JpegDisplayInit();
		DecodeOneFrame(pbuf);
	}
}
/******************************************************************************
Description:
用于显示静止的图像。
Parameters:
1,pbuf:输入的数据块。用于存放要显示的jpeg图像。
2,pinfo:用于存放要显示的jpeg图像的信息。
3,proty:LCD的属性。
Returns:
Null
Remarks:
Null
Example:
UINT8 g_StillBuf[0x10000];
UINT32	g_StillBufOffset;
void TestDisplayStill(void)
{
V558_JPEG_INDEX info;
V558_LAYER_PROPERTY proty;

		proty.DisPoint.x = 0x0;
		proty.DisPoint.y = 0x0;
		proty.DisRect.height = 0x60;
		proty.DisRect.width = 0x80;
		proty.DisRect.left = 0;
		proty.DisRect.top = 0;
		proty.Size.cx = 0x80;
		proty.Size.cy = 0x60;
		
		  g_JpegBuf.ActLen = g_StillBufOffset ;
		  g_JpegBuf.pData = (UINT8 *)g_StillBuf;
		  g_JpegBuf.Len = 0x8000;
		  
			if(V558_JpegParse(&g_JpegBuf, &info) == 0)
			{
			V558_DisplayStill(&g_JpegBuf, &info, &proty);
			}
			}
******************************************************************************/
void V558_DisplayStill(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo, 
					   PV558_LAYER_PROPERTY proty)
{
	if(pinfo->YUVType < V558_YUV_UNKNOWN)
	{
		memcpy(&gJpegContext.header, pinfo, sizeof(V558_JPEG_INDEX));
		StartDecodeJpg(V558_DISPLAY_STILL, proty);
		DecodeOneFrame(pbuf);
	}
}


UINT8 HostDecode(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, PV558_JPEGCALLBACK pcall)
{
	gJpegContext.pcall = pcall;
	gJpegContext.dst = *pdst;
	pdst->ActLen = 0;
	gJpegContext.buf = *psrc;
	
	V558_LbufSetYuvMode(gJpegContext.header.YUVType);
	V558_CtrlSwReset(V558_SW_RESET_JPG);
	V558_JpegSetWorkMode(0xb);
	V558_JpegSetVWC(gJpegContext.header.vwc);
	V558_JpegSetBlockNum(gJpegContext.header.blkcount);
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);
	
	WriteQT();
	WriteHT();
	WriteComponents();
	
	gJpegContext.State = V558_JPEG_ISWORKING;
	//	V558_CtrlIntEnable(V558_INT_8BUF_FULL, 1);
	return HostSendJpgData();			//
}

UINT8 FillEncodeParm(V558_SIZE size, V558_SIZE *newsize, UINT8 yuvmode, UINT32 *total)
{
	UINT8 blkcount, compnum = 0x23;
	UINT32 temp, vwc;
	UINT16 comp[3] = {0x120b, 0x5104, 0x5104};

	temp = size.cx;
	temp *= size.cy;
	switch(yuvmode)
	{
	case V558_YUV_422:
	case V558_PYUV_422_YYUV:
	case V558_PYUV_422_YUYV:
	case V558_PYUV_422_UYVY:
		newsize->cx = ((size.cx + 15) >> 4) << 4;
		newsize->cy = ((size.cy + 7) >> 3) << 3;
		blkcount = newsize->cx >> 2;
		vwc = newsize->cx;
		vwc *= newsize->cy;
		vwc >>= 1;
		break;
	case V558_YUV_420:
		newsize->cx = ((size.cx + 15) >> 4) << 4;
		newsize->cy = ((size.cy + 15) >> 4) << 4;
		blkcount = newsize->cx >> 2;
		blkcount += blkcount >> 1;
		vwc = newsize->cx;
		vwc *= newsize->cy;
		vwc >>= 2;
		vwc += vwc >> 1;
		break;
	case V558_YUV_411:
		newsize->cx = ((size.cx + 31) >> 5) << 5;
		newsize->cy = ((size.cy + 7) >> 3) << 3;
		blkcount = newsize->cx >> 3;
		blkcount += blkcount >> 1;
		vwc = newsize->cx;
		vwc *= newsize->cy;
		vwc >>= 2;
		vwc += vwc >> 1;
		break;
	case V558_YUV_400:
		newsize->cx = ((size.cx + 7) >> 3) << 3;
		newsize->cy = ((size.cy + 7) >> 3) << 3;
		blkcount = newsize->cx >> 3;
		vwc = newsize->cx;
		vwc *= newsize->cy;
		vwc >>= 2;
		compnum = 0x11;
		comp[0] = 0x1107;
//			comp[1] = 0;
//			comp[2] = 0;
		break;
	default:
		return JPEGFAILED;
	}

	gJpegContext.mode = V558_ENCODE_JPG;
	V558_CtrlSetWorkMode(V558_MODE_ENCODER);
	V558_CtrlSwReset(V558_SW_RESET_JPG);

	V558_JpegSetWorkMode(2);
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);

	V558_JpegSetComponentNum(compnum);
	V558_JpegSetComponentParm(0, comp[0]);
	V558_JpegSetComponentParm(1, comp[1]);
	V558_JpegSetComponentParm(2, comp[2]);
	V558_JpegSetVWC(vwc);
	V558_JpegSetBlockNum(blkcount);
	V558_JpegSetImageSize(size);
	V558_JbufSetJpegAddr(0);
	V558_JbufSetJpegSize(TOTAL_JBUF_SIZE-1);
	*total = vwc << 2;
	V558_CtrlIntEnable(V558_INT_CAP_DONE, 1);
	V558_CtrlIntEnable(V558_INT_CAP_FIFO_FULL, 1);
	return SUCCEED;
}

void SendBlockData(UINT32 offset, UINT32 width, UINT32 rest, UINT8 *buf)
{
	UINT32 m, n;

	if(rest == 0)
		return;
	for(m = 0; m < 8; m++, offset += width << 1)	//Y0 block
	{
		for(n = 0; n < rest; n++)
			V558_JpegWriteDctData(buf[offset + (n << 2)] - 128);
		for(n = rest; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
}

void SendYYUVBlockData(UINT32 offset, UINT32 width, UINT32 rest, UINT8 *buf)
{
	UINT32 m, n, x;
	UINT32 pos = offset;

	if(rest == 0)
		return;
	for(m = 0; m < 8; m++, pos += width << 1)	//Y0 block
	{
		x = rest > 4 ? 4 : rest;
		for(n = 0; n < x; n++)
		{
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
			V558_JpegWriteDctData(buf[pos + (n << 2)+1] - 128);
		}
		for(n = x; n < 4; n++)
		{
			V558_JpegWriteDctData(0);
			V558_JpegWriteDctData(0);
		}
	}
	pos = offset + 16;
	if(rest < 5)
	{
		for(n = 0; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
	else
	{
		for(m = 0; m < 8; m++, pos += width << 1)	//Y1 block
		{
			for(n = 0; n < rest - 4; n++)
			{
				V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
				V558_JpegWriteDctData(buf[pos + (n << 2)+1] - 128);
			}
			for(n = rest-4; n < 4; n++)
			{
				V558_JpegWriteDctData(0);
				V558_JpegWriteDctData(0);
			}
		}
	}
	pos = offset + 2;
	for(m = 0; m < 8; m++, pos += width << 1)	//U block
	{
		for(n = 0; n < rest; n++)
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
		for(n = rest; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
	pos = offset + 3;
	for(m = 0; m < 8; m++, pos += width << 1)	//V block
	{
		for(n = 0; n < rest; n++)
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
		for(n = rest; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
}

void SendYUYVBlockData(UINT32 offset, UINT32 width, UINT32 rest, UINT8 *buf)
{
	UINT32 m, n, x;
	UINT32 pos = offset;

	if(rest == 0)
		return;
	for(m = 0; m < 8; m++, pos += width << 1)	//Y0 block
	{
		x = rest > 4 ? 4 : rest;
		for(n = 0; n < x; n++)
		{
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
			V558_JpegWriteDctData(buf[pos + (n << 2)+2] - 128);
		}
		for(n = x; n < 4; n++)
		{
			V558_JpegWriteDctData(0);
			V558_JpegWriteDctData(0);
		}
	}
	pos = offset + 16;
	if(rest < 5)
	{
		for(n = 0; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
	else
	{
		for(m = 0; m < 8; m++, pos += width << 1)	//Y1 block
		{
			for(n = 0; n < rest - 4; n++)
			{
				V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
				V558_JpegWriteDctData(buf[pos + (n << 2)+2] - 128);
			}
			for(n = rest-4; n < 4; n++)
			{
				V558_JpegWriteDctData(0);
				V558_JpegWriteDctData(0);
			}
		}
	}
	pos = offset + 1;
	for(m = 0; m < 8; m++, pos += width << 1)	//U block
	{
		for(n = 0; n < rest; n++)
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
		for(n = rest; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
	pos = offset + 3;
	for(m = 0; m < 8; m++, pos += width << 1)	//V block
	{
		for(n = 0; n < rest; n++)
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
		for(n = rest; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
}

void SendUYVYBlockData(UINT32 offset, UINT32 width, UINT32 rest, UINT8 *buf)
{
	UINT32 m, n, x;
	UINT32 pos = offset;

	if(rest == 0)
		return;
	for(m = 0; m < 8; m++, pos += width << 1)	//Y0 block
	{
		x = rest > 4 ? 4 : rest;
		for(n = 0; n < x; n++)
		{
			V558_JpegWriteDctData(buf[pos + (n << 2)+1] - 128);
			V558_JpegWriteDctData(buf[pos + (n << 2)+3] - 128);
		}
		for(n = x; n < 4; n++)
		{
			V558_JpegWriteDctData(0);
			V558_JpegWriteDctData(0);
		}
	}
	pos = offset + 16;
	if(rest < 5)
	{
		for(n = 0; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
	else
	{
		for(m = 0; m < 8; m++, pos += width << 1)	//Y1 block
		{
			for(n = 0; n < rest - 4; n++)
			{
				V558_JpegWriteDctData(buf[pos + (n << 2)+1] - 128);
				V558_JpegWriteDctData(buf[pos + (n << 2)+3] - 128);
			}
			for(n = rest-4; n < 4; n++)
			{
				V558_JpegWriteDctData(0);
				V558_JpegWriteDctData(0);
			}
		}
	}
	pos = offset;
	for(m = 0; m < 8; m++, pos += width << 1)	//U block
	{
		for(n = 0; n < rest; n++)
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
		for(n = rest; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
	pos = offset + 2;
	for(m = 0; m < 8; m++, pos += width << 1)	//V block
	{
		for(n = 0; n < rest; n++)
			V558_JpegWriteDctData(buf[pos + (n << 2)] - 128);
		for(n = rest; n < 8; n++)
			V558_JpegWriteDctData(0);
	}
}

UINT32 WriteDctData(UINT8 *buf, UINT32 actlen, UINT32 width, V558_SIZE size, UINT8 yuvmode)
{
	UINT32 k = 0, adr = 0, offset;
	UINT32 i, linelength, unit, rest;

	if( (yuvmode == V558_PYUV_422_YYUV) || (yuvmode == V558_PYUV_422_YUYV) || (yuvmode == V558_PYUV_422_UYVY) )
	{
		linelength = width << 4;
		unit = width >> 4;
		rest = width % 16;
		rest >>= 1;
		for(k = 0; k < actlen; k += linelength)
		{
			adr = 0;
			for(i = 0; i < unit; i++, adr += 32)
			{
				switch(yuvmode)
				{
				case V558_PYUV_422_YYUV:
					offset = k + adr;
					SendYYUVBlockData(offset, width, 8, buf);
					break;
				case V558_PYUV_422_YUYV:
					offset = k + adr;
					SendYUYVBlockData(offset, width, 8, buf);
					break;
				case V558_PYUV_422_UYVY:
					offset = k + adr;
					SendUYVYBlockData(offset, width, 8, buf);
				default:
					break;
				}
			}
			switch(yuvmode)
			{
			case V558_PYUV_422_YYUV:
				offset = k + adr;
				SendYYUVBlockData(offset, width, rest, buf);
				break;
			case V558_PYUV_422_YUYV:
				offset = k + adr;
				SendYUYVBlockData(offset, width, rest, buf);
				break;
			case V558_PYUV_422_UYVY:
				offset = k + adr;
				SendUYVYBlockData(offset, width, rest, buf);
			default:
				break;
			}
		}
		return k;
	}
	while(actlen--)
		V558_JpegWriteDctData(buf[k++] - 128);
	return k;
}

/******************************************************************************
Description:
将以8*8 block输入的YUV数据进行encode。
Parameters:
1,psrc:输入的数据块。用于存放将要encode的以8*8 block 排列的YUV数据。
2,pdst:输出的数据块。用于存放encode之后的图像数据
3,pcall:回调指针。
4,psize:将要转换的原始数据的图像大小。
5,yuvmode:将要转换数据的图像格式。支持,
	YUV422,YUV420,YUV411,YUV400, 	//block type
	V558_PYUV_422_YYUV,				//point type
	V558_PYUV_422_YUYV,
	V558_PYUV_422_UYVY。
Returns:
如果encode成功，返回0，否则，返回0xff。
Remarks:

Example:
UINT8    g_dataBuf[0x10000];
UINT8    g_ImageBuf[0x30000];
UINT32   g_bufoffset;
void TestEncode(void)
{
	V558_JPEGBUF src, dst;
	V558_SIZE    size,NewSize;
	UINT8        mode;

	size.cx     =  160;
	size.cy     =  120;
	mode        =  V558_PYUV_422_YYUV;
	g_bufoffset = LoadImageFile("/flash/3.jpg", g_dataBuf,0x30000);
	src.ActLen  =  g_bufoffset;
	src.Len     =  0x30000;
	src.pData   =  g_dataBuf;

	g_bufoffset = 0;
	dst.ActLen  =  g_bufoffset;
	dst.Len     =  0x30000;
	dst.pData   =  g_ImageBuf;
	V558_HostEncode(&src, &dst, JpegNote1, size,mode );
}
******************************************************************************/
UINT8 V558_HostEncode(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, PV558_JPEGCALLBACK pcall,
                      V558_SIZE size, UINT8 yuvmode)
{
	UINT32 total, temp, k;
	V558_SIZE newsize;

	if(FillEncodeParm(size, &newsize, yuvmode, &total) != SUCCEED)
		return JPEGFAILED;
	gJpegContext.pcall = pcall;
	gJpegContext.buf = *pdst;
	pdst->ActLen = 0;
	psrc->ActLen = psrc->ActLen > total ?total : psrc->ActLen  ;
	V558_JpegStartEncoder();
	temp = 0;
	do{
		temp++;
	}while( ((V558_JpegGetStatus() & 8) == 0) && (temp < 0x2000) );
//	if(temp == 0x20000)
//		return JPEGFAILED;
	do{
		temp = total;
		if(total > psrc->ActLen)
			temp = psrc->ActLen;

		k = WriteDctData(psrc->pData, psrc->ActLen, size.cx, newsize, yuvmode);
		total -= k;
		if(total)
			pcall(MSG_WRITE_JPGDATA, psrc);
	}while(total);

	return SUCCEED;
}

/******************************************************************************
Description:
用于从lbuf输出decode的数据。
Parameters:
1,psrc:输入的数据块。用于存放要decode的jpeg图像。
2,pdst:输出的数据块。用于存放decode出来的数据。
3,decdatatype:输出数据的格式。0:yuv;  1:bmp565;  2:bmp555
4,pinfo:解析出来的jpeg图像的信息。
5,pcall:回调指针。
Returns:
如果decode正确，返回0x0。如果decode错误，返回0xff。
Remarks:
输入一幅jpeg图像，调用此函数，就可以得到相应的decode数据。
Example:
UINT8    g_dataBuf[0x10000];
UINT8    g_ImageBuf[0x30000];
UINT32   g_bufoffset;
UINT8 TestDecode(void)
{
	V558_JPEGBUF src, dst;
	UINT8        value;
	UINT8 decdatatype;
	V558_JPEG_INDEX info;		
	g_bufoffset = LoadImageFile("/flash/2.jpg", g_dataBuf,0x20000);
	src.ActLen  = g_bufoffset;
	src.Len     = 0x10000;
	src.pData   = g_dataBuf;

	dst.ActLen  = 0;
	dst.Len     = 0x30000;
	dst.pData   = g_ImageBuf;
	g_bufoffset = 0;
	if(!V558_JpegParse(&src, &info))
	{
		value   =   V558_HostDecode(&src, &dst, decdatatype,&info,JpegNote1);
	}
	return   value;
}
******************************************************************************/
UINT8 V558_HostDecode(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,UINT8 decdatatype, PV558_JPEG_INDEX pinfo, PV558_JPEGCALLBACK pcall)
{
	if(pinfo->YUVType >= V558_YUV_UNKNOWN)
		return JPEGFAILED;
	memcpy(&gJpegContext.header, pinfo, sizeof(V558_JPEG_INDEX));
	gJpegContext.mode = V558_DECODE_ONLY;
	gJpegContext.decdatatype = decdatatype;
	gJpegContext.decMaxlinesum  =  pinfo->ImageSize.cy;
	gJpegContext.decline        =  0;
	gJpegContext.declinesum     =  0;	
	V558_CtrlSetWorkMode(V558_MODE_DECODER);
	V558_CtrlDisableBRC();
	V558_LbufSetWorkmode(0);
	V558_LbufSetImageSize(gJpegContext.header.JpgSize);
	gJpegContext.decodewidth  =   pinfo->ImageSize.cx;
	gJpegContext.decMaxline = (UINT16) ((pdst->Len /(  gJpegContext.decodewidth <<4))<<3);	
	HostDecode(psrc, pdst, pcall);
	return SUCCEED;
}
/******************************************************************************
Description:
用于从ipp输出decode的数据。
Parameters:
1,psrc:输入的数据块。用于存放要decode的jpeg图像。
2,pdst:输出的数据块。用于存放decode出来的YUV数据。
3,decdatatype:定义输出yuv422数据的yuv排列。0: yyuv ; 1:yuyv ; 2:uyvy。
4,pinfo:输入的jpeg图像的信息。
5,pcall:回调指针。
6,psize:取景框的大小。
7,width:设置ipp的输出宽度。
Returns:
如果decode正确，返回0x0。如果decode错误，返回0xff。
Remarks:
输入一幅jpeg图像，调用此函数，就可以得到相应的数据。这个函数和V558_HostDecodeIpp的差别就在于V558_HostDecodeIpp
是对整幅图片进行decode,而V558_HostDecodeIppSize是对整幅图片取一个框，对这框里面的数据进行decode。
Example:
UINT8    g_dataBuf[0x10000];
UINT8    g_ImageBuf[0x30000];
UINT32   g_bufoffset;
UINT8 TestDecodeIpp(void)
{
	V558_JPEGBUF src, dst;
	V558_JPEG_INDEX info;
	V558_SIZE    size;
	UINT8        value,type;
	type        =  1;//yuyv
	size.cx     =  0x80;
	size.cy     =  0x60;
	g_bufoffset = LoadImageFile("/flash/2.jpg", g_dataBuf,0x20000);
	src.ActLen  = g_bufoffset;
	src.Len     = 0x10000;
	src.pData   = g_dataBuf;
	dst.ActLen  = 0;
	dst.Len     = 0x30000;
	dst.pData   = g_ImageBuf;
	g_bufoffset = 0;
	if(!V558_JpegParse(&src, &info))
		value  =  V558_HostDecodeIppSize(&src, &dst, type, &info, JpegNote1, &size, 0x80);
	return      value;
}
******************************************************************************/
UINT8 V558_HostDecodeIppSize(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype, PV558_JPEG_INDEX pinfo, PV558_JPEGCALLBACK pcall, PV558_SIZE psize, UINT16 width)
{
	V558_POINT pt = {0, 0};
	V558_SIZE  tempsize;
	if(pinfo->YUVType >= V558_YUV_UNKNOWN)
		return JPEGFAILED;
	memcpy(&gJpegContext.header, pinfo, sizeof(V558_JPEG_INDEX));
	gJpegContext.mode = V558_DECODE_IPP;

	gJpegContext.declinesum     =  0;	
	gJpegContext.decline  =  0;
	gJpegContext.decdatatype  =  decdatatype;
	V558_CtrlSetWorkMode(V558_MODE_DECODER);
	V558_CtrlDisableBRC();
	V558_CtrlSwReset(V558_SW_RESET_IPP);
	tempsize.cx    =   ( psize->cx > gJpegContext.header.ImageSize.cx) ? gJpegContext.header.ImageSize.cx : psize->cx;
	tempsize.cy    =   ( psize->cy > gJpegContext.header.ImageSize.cy) ? gJpegContext.header.ImageSize.cy : psize->cy;
	V558_IppDisplay(gJpegContext.header.JpgSize, pt, tempsize, width);
	gJpegContext.decMaxlinesum  =  V558_IppGetDispHeight();
	V558_IppDisablePrefilter();
	gJpegContext.decodewidth = V558_IppGetDispWidth();
	V558_LbufSetWorkmode(8);
	V558_LbufSetImageSize(gJpegContext.header.JpgSize);
	gJpegContext.decMaxline = (UINT16) ((pdst->Len /(  gJpegContext.decodewidth <<4))<<3);
	HostDecode(psrc, pdst, pcall);
	return SUCCEED;
}
/******************************************************************************
Description:
用于从ipp输出decode的数据。
Parameters:
1,psrc:输入的数据块。用于存放要decode的jpeg图像。
2,pdst:输出的数据块。用于存放decode出来的YUV数据。
3,decdatatype:定义输出yuv422数据的yuv排列。0: yyuv ; 1:yuyv ; 2:uyvy。
4,pinfo:输入的jpeg图像的信息。
5,pcall:回调指针。
6,width:设置ipp的输出宽度。
Returns:
如果decode正确，返回0x0。如果decode错误，返回0xff。
Remarks:
输入一幅jpeg图像，调用此函数，就可以得到相应的数据。
Example:
UINT8    g_dataBuf[0x10000];
UINT8    g_ImageBuf[0x30000];
UINT32   g_bufoffset;
UINT8 TestDecodeIpp(void)
{
	V558_JPEGBUF src, dst;
	UINT8        value,type;
	V558_JPEG_INDEX info;
	type        = 1; //yuyv
	g_bufoffset = LoadImageFile("/flash/2.jpg", g_dataBuf,0x20000);
	src.ActLen  = g_bufoffset;
	src.Len     = 0x10000;
	src.pData   = g_dataBuf;
	dst.ActLen  = 0;
	dst.Len     = 0x30000;
	dst.pData   = g_ImageBuf;
	g_bufoffset = 0;
	if(!V558_JpegParse(&src, &info))
		value  =  V558_HostDecodeIpp(&src, &dst, type, &info,JpegNote1,0x80);
	return      value;
}
******************************************************************************/
UINT8 V558_HostDecodeIpp(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype, PV558_JPEG_INDEX pinfo, 
						 PV558_JPEGCALLBACK pcall, UINT16 width)
{
	return V558_HostDecodeIppSize(psrc, pdst, decdatatype, pinfo, pcall, &pinfo->ImageSize, width);
}
/******************************************************************************
Description:
	Get the minimal ipp width when decode with ipp.
Parameters:
	1,pinfo:   the pointer of jpeg image information.
Returns:
	Return the minimal width.
Remarks:
	Call this function to check the decode ipp width before decode with ipp
*******************************************************************************/
UINT16 V558_GetDecodeIppMinWidth(PV558_JPEG_INDEX pinfo)
{
	UINT16 temp;
	temp   =  (pinfo->ImageSize.cx>>3);
	return temp;
}

/******************************************************************************
Description:
用于从LCD Interface 输出YUV数据的decode。
Parameters:
1,psrc:输入的数据块。用于存放要decode的jpeg图像。
2,pdst:输出的数据块。用于存放decode出来的YUV数据。
3,decdatatype:定义输出yuv422数据的yuv排列。0: yyuv ; 1:yuyv ; 2:uyvy。
4,pinfo:输入的jpeg图像的信息。
5,pcall:回调指针。
6,proty:设置LCD Interface的属性。
Returns:
如果decode正确，返回0x0。如果decode错误，返回0xff。
Remarks:
输入一幅jpeg图像，调用此函数，就可以得到相应的YUV数据。
Example:
UINT8    g_dataBuf[0x10000];
UINT8    g_ImageBuf[0x30000];
UINT32   g_bufoffset;
void TestHostDecodeLcdIF(void)
{
	V558_JPEGBUF src, dst;
	V558_LAYER_PROPERTY proty;
	V558_JPEG_INDEX info;
	UINT8  type=1;   //yuyv
	proty.DisPoint.x = 0x0;
	proty.DisPoint.y = 0x0;
	proty.DisRect.height = 0x60;
	proty.DisRect.width = 0x80;
	proty.DisRect.left = 0;
	proty.DisRect.top = 0;
	proty.Size.cx = 0x80;
	proty.Size.cy = 0x60;
	g_bufoffset = LoadImageFile("/flash/2.jpg", g_dataBuf,0x20000);
	src.ActLen = g_bufoffset;
	src.Len = 0x10000;
	src.pData = g_dataBuf;
	dst.ActLen = 0;
	dst.Len = 0x10000;
	dst.pData = g_ImageBuf;
	g_bufoffset = 0;
	V558_HostDecodeLcdIF(&src, &dst, type, &info, JpegNote1, &proty);
}
******************************************************************************/
UINT8 V558_HostDecodeLcdIF(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype,  PV558_JPEG_INDEX pinfo, PV558_JPEGCALLBACK pcall, 
						   PV558_LAYER_PROPERTY proty)
{
	return V558_HostDecodeLcdIFSize(psrc, pdst, decdatatype, pinfo, pcall, &pinfo->ImageSize, proty);
}

/******************************************************************************
Description:
用于从LCD Interface 输出YUV数据的decode。
Parameters:
1,psrc:输入的数据块。用于存放要decode的jpeg图像。
2,pdst:输出的数据块。用于存放decode出来的YUV数据。
3,decdatatype:定义输出yuv422数据的yuv排列。0: yyuv ; 1:yuyv ; 2:uyvy。
3,pinfo:输入的jpeg图像的信息。
4,pcall:回调指针。
5,psize:取景框的大小。
6,proty:设置LCD Interface的属性。
Returns:
如果decode正确，返回0x0。如果decode错误，返回0xff。
Remarks:
输入一幅jpeg图像，调用此函数，就可以得到相应的数据。这个函数和V558_HostDecodeLcdIF的差别就在于V558_HostDecodeLcdIF
是对整幅图片进行decode,而V558_HostDecodeLcdIFSize是对整幅图片取一个框，对这框里面的数据进行decode。

Example:
UINT8    g_dataBuf[0x10000];
UINT8    g_ImageBuf[0x30000];
UINT32   g_bufoffset;
void TestHostDecodeLcdIF(void)
{
	V558_JPEGBUF src, dst;
	V558_LAYER_PROPERTY proty;
	V558_JPEG_INDEX info;
	V558_SIZE  size;
	UINT8   type  =  1;  //yuyv
	proty.DisPoint.x = 0x0;
	proty.DisPoint.y = 0x0;
	proty.DisRect.height = 0x60;
	proty.DisRect.width = 0x80;
	proty.DisRect.left = 0;
	proty.DisRect.top = 0;
	proty.Size.cx = 0x80;
	proty.Size.cy = 0x60;
	size.cx       =  0x80;
	size.cy       =  0x60;
	g_bufoffset = LoadImageFile("/flash/2.jpg", g_dataBuf,0x20000);
	src.ActLen = g_bufoffset;
	src.Len = 0x10000;
	src.pData = g_dataBuf;
	dst.ActLen = 0;
	dst.Len = 0x10000;
	dst.pData = g_ImageBuf;
	g_bufoffset = 0;
	V558_HostDecodeLcdIFSize(&src, &dst, type, &info, JpegNote1, &size, &proty);
}
******************************************************************************/
UINT8 V558_HostDecodeLcdIFSize(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype, PV558_JPEG_INDEX pinfo, 
							   PV558_JPEGCALLBACK pcall, PV558_SIZE psize, PV558_LAYER_PROPERTY proty)
{
	V558_POINT pt = {0, 0};
	V558_SIZE tempsize;
	if(pinfo->YUVType >= V558_YUV_UNKNOWN)
		return JPEGFAILED;
	memcpy(&gJpegContext.header, pinfo, sizeof(V558_JPEG_INDEX));
	gJpegContext.mode = V558_DECODE_LCDIF;
	gJpegContext.decline  =  0;
	gJpegContext.decdatatype =  decdatatype;
	V558_CtrlSetWorkMode(V558_MODE_DECODER_FRAME);
	V558_CtrlDisableBRC();
	V558_CtrlSwReset(V558_SW_RESET_IPP);
	tempsize.cx    =   ( psize->cx > gJpegContext.header.ImageSize.cx) ? gJpegContext.header.ImageSize.cx : psize->cx;
	tempsize.cy    =   ( psize->cy > gJpegContext.header.ImageSize.cy) ? gJpegContext.header.ImageSize.cy : psize->cy;
	V558_IppDisplay(gJpegContext.header.LbufSize, pt,
		tempsize, proty->Size.cx);
	V558_IppDisablePrefilter();
	V558_IppGetDispSize(&(proty->Size));
	V558_LcdSetALProperty(proty);
	gJpegContext.decMaxlinesum  =  proty->Size.cy;
	gJpegContext.decline        =  0;
	gJpegContext.declinesum     =  0;	
	gJpegContext.decodewidth = proty->DisRect.width;
	V558_LbufSetWorkmode(0x14);
	V558_LbufSetImageSize(gJpegContext.header.LbufSize);
	gJpegContext.decMaxline =  (UINT16)((pdst->Len /(  gJpegContext.decodewidth <<4))<<3);
	HostDecode(psrc, pdst, pcall);
	return SUCCEED;
}

void V558_AdjustCaptureSize(V558_SIZE size)
{
	UINT32	vwc;
	UINT8	ratio;

	switch(gJpegContext.mode)
	{
	case V558_JPEG_IDEL:
	case V558_CAPTURE_STILLBRC:
	case V558_CAPTURE_STILLBRC_THUMBNAIL:
	case V558_CAPTURE_FRAME:
	case V558_CAPTURE_FRAMEVIDEO:
	case V558_CAPTURE_VIDEO:
		V558_JpegSetImageSize(size);
		size.cx += 15;
		size.cx >>= 4;
		size.cx <<= 4;
		size.cy += 7;
		size.cy >>= 3;
		size.cy <<= 3;
		V558_LbufSetImageSize(size);
		vwc = size.cx;
		vwc *= size.cy;
		vwc >>= 1;
		V558_JpegSetVWC(vwc);
		if(gJpegContext.entcr)
		{
			if(gJpegContext.tcr > 5)
			{
				ratio = gJpegContext.tcr - 1;
				ratio >>= 2;
				vwc /= ratio;
				V558_JpegSetTWC(vwc);
				gJpegContext.entcr = 1;
			}
		}
		else
		{
			if(gJpegContext.twc)
			{
				vwc /= gJpegContext.twc;
				vwc <<= 2;
				vwc += 1;
				V558_JpegSetTCR((UINT8)vwc);
				gJpegContext.entcr = 0;
			}
		}

		break;
	case V558_DISPLAY_STILL:
	case V558_DISPLAY_THUMBNAIL:
	case V558_DISPLAY_VIDEO:
		break;

	default:
		break;
	}
}
/********************************************************************************

LBUF module function

*********************************************************************************/
UINT8 V558_LbufOpen(void)
{
	V558_CtrlIntAdd(V558_INT_8BUF_FULL, _ISR_LbufFull);
	V558_CtrlIntEnable(V558_INT_8BUF_FULL, 0);
	return SUCCEED;
}





/********************************************************************************

JBUF module function

*********************************************************************************/

void ReadCapStillBuf(void)
{
	UINT32 len;

	if(&gJpegContext.buf)
	{
		len = V558_JbufGetBiuAccLength() + 1;
		V558_ReadSram(JBUF_BASEADD, gJpegContext.buf.pData + gJpegContext.buf.ActLen, len);
		gJpegContext.buf.ActLen += len;
	}
}

/******************************************************************************
Description:
用于读取thumbnail。
Parameters:
1,pbuf:输出的数据块。用于存放生成的thumbnail数据。
2,size:thumbnail的大小。
Returns:
Null
Remarks:
Null
Example:
UINT8 g_ThumbBuf[0x2000];
void TestJbufReadThumb(void)
{
V558_JPEGBUF JpegBuf;
V558_SIZE    g_ThumbSize;
JpegBuf.ActLen = 0;
JpegBuf.Len    = 0x8000;
JpegBuf.pData  = g_ThumbBuf;
g_ThumbSize.cx = 40;
g_ThumbSize.cy = 30;
V558_JbufReadThumb(&JpegBuf, &g_ThumbSize);
}
******************************************************************************/
void V558_JbufReadThumb(PV558_JPEGBUF pbuf, V558_SIZE *size)
{
	UINT16   i,j;
	UINT32   adr;
	UINT8*   ptr=pbuf->pData;
	UINT8    temp0,temp1,temp2,temp3;
	ptr     =   pbuf->pData;
	pbuf->ActLen = 0;
	*size = gJpegContext.thumb;
	for(j=0; j<gJpegContext.thumb.cy ; j++)
	{
		adr    =  JBUF_BASEADD + gJpegContext.thumbadr + ((j * gJpegContext.thumb.cx)<<1);
		V558_ReadSram(adr, ptr, gJpegContext.thumb.cx << 1);
		for( i = 0 ; i < (gJpegContext.thumb.cx>>1) ; i ++)
		{
			adr  =  (i<<2);
			temp0        =     *(ptr+adr);
			temp1        =     *(ptr+adr+1);
			temp2        =     *(ptr+adr+2);
			temp3        =     *(ptr+adr+3);
			*(ptr+adr)   =     temp1;
			*(ptr+adr+1) =     temp3;
			*(ptr+adr+2) =     temp0;
			*(ptr+adr+3) =     temp2;
		}
		ptr += (gJpegContext.thumb.cx << 1);
	}
	pbuf->ActLen = (gJpegContext.thumb.cx * gJpegContext.thumb.cy) << 1;
}

void _ISR_CaptureDone(void)
{
	PV558_JPEGCALLBACK pcall;
	UINT8	mode;
	V558_JPEGBUF	buf;

	V558_CtrlIntEnable(V558_INT_CAP_DONE, 0);
	V558_CtrlIntEnable(V558_INT_CAP_FIFO_FULL, 0);
	if(!gJpegContext.pcall)
		return;

	switch(gJpegContext.mode)
	{
	case V558_CAPTURE_FRAME:
	case V558_CAPTURE_FRAMEVIDEO:
		V558_LcdSetIFControl(gJpegContext.lcdctrl);
		break;
	case V558_CAPTURE_STILLBRC:
	case V558_CAPTURE_STILLBRC_THUMBNAIL:
	case V558_CAPTURE_VIDEO:
		V558_IppSelectPrefilter(0);
		break;
	default:
		break;
	}
	mode = gJpegContext.mode;
	gJpegContext.mode = V558_JPEG_IDEL;
	gJpegContext.State = V558_JPEG_INITED;
	pcall = gJpegContext.pcall;
	gJpegContext.pcall = NULL;
	
	switch(mode)
	{
	case V558_CAPTURE_FRAME:
	case V558_CAPTURE_STILLBRC:
	case V558_CAPTURE_STILLBRC_THUMBNAIL:
		buf.ActLen = 0;
		pcall(MSG_JPEG_HEADER, &buf);
		break;
	default:
		break;
	}
	ReadCapStillBuf();
	pcall(MSG_CAPTURE_DONE, &gJpegContext.buf);
}

void _ISR_CapFifoFull(void)
{
	UINT32  length;
	if(!gJpegContext.pcall)
		return;

	if(!gJpegContext.buf.pData)
		return;
	length = TOTAL_JBUF_SIZE;
	if(gJpegContext.mode ==V558_CAPTURE_STILLBRC_THUMBNAIL)
		length = TOTAL_JBUF_SIZE - DEFAULT_THUMB_SIZE;
	length = (length>>1);
	V558_ReadSram(JBUF_BASEADD, gJpegContext.buf.pData + gJpegContext.buf.ActLen, length);
	gJpegContext.buf.ActLen += length;
	if(gJpegContext.buf.ActLen > gJpegContext.buf.Len - length)
	{
		gJpegContext.pcall(MSG_JBUF_FULL, &gJpegContext.buf);
		gJpegContext.buf.ActLen = 0;
	}
}

void V558_JbufGeInit(void)
{
	V558_JbufSetSourceAddr(DEFAULT_SRC_ADR);
	V558_JbufSetDestAddr(DEFAULT_DEST_ADR);
	V558_JbufSetSourceSize(DEFAULT_SRC_SIZE - 1);
	V558_JbufSetDestSize(DEFAULT_DEST_SIZE - 1);
}

UINT8 V558_JbufOpen(void)
{
	V558_JbufGeInit();

	V558_CtrlIntEnable(V558_INT_CAP_DONE, 0);
	V558_CtrlIntEnable(V558_INT_CAP_FIFO_FULL, 0);
	V558_CtrlIntAdd(V558_INT_CAP_DONE, _ISR_CaptureDone);
	V558_CtrlIntAdd(V558_INT_CAP_FIFO_FULL, _ISR_CapFifoFull);

	return SUCCEED;
}








/********************************************************************************

JPEG module function

*********************************************************************************/

void _ISR_JpegError(void)
{
	if(!gJpegContext.pcall)
		return;
	gJpegContext.pcall(MSG_JPEG_ERROR, &gJpegContext.buf);
}

void ResumeClk(void)
{
	UINT8	temp = 0xff;

	if(gJpegContext.clkflag == 1)
	{
		V558_SetPanelWcy(gJpegContext.wcy);
		V558_CtrlChangeChipClk(gJpegContext.chipoutclk);    //return the clock value.			
		gJpegContext.clkflag = 0;
		while(temp--);
	}
}

void _ISR_JpgDone(void)
{
	switch(gJpegContext.mode)
	{
	case V558_DISPLAY_STILL:
		break;
	case V558_DISPLAY_VIDEO:
		break;
	case V558_DISPLAY_THUMBNAIL:
	default:
		break;
	}
#if 1
	ResumeClk();
#endif
	gJpegContext.State = V558_JPEG_INITED;
}

void V558_JpegInitContext(void)
{
	gJpegContext.Valid = FALSE;
	gJpegContext.State = V558_JPEG_NOTINIT;
}

UINT8 V558_JpegOpen(void)
{
	V558_JpegSetBRC(gJpegContext.qf | 0x80);

	if(gJpegContext.entcr)
	{
		V558_JpegSetTWC(gJpegContext.twc);
		V558_JpegSetTCR(gJpegContext.tcr);
	}
	else
	{
		V558_JpegSetTCR(gJpegContext.tcr);
		V558_JpegSetTWC(gJpegContext.twc);
	}

	V558_CtrlIntAdd(V558_INT_JPEG_BUF_ERROR, _ISR_JpegError);
	V558_CtrlIntEnable(V558_INT_JPEG_BUF_ERROR, 1);
	V558_CtrlIntAdd(V558_INT_JPG_DONE, _ISR_JpgDone);
	V558_CtrlIntEnable(V558_INT_JPG_DONE, 1);

	gJpegContext.State = V558_JPEG_INITED;
	return SUCCEED;
}
/******************************************************************************
Description:
设置jpeg的一些属性。
Parameters:
info:输入的jpeg的一些属性值。
Returns:
如果0x0,表明success。
Remarks:
在调用jpep模块之前，必须首先调用此函数。
Example:
UINT8 g_dataBuf[0x10000];
UINT32	g_bufoffset;
void TestCaptureStill(void)
{
V558_JPEGBUF g_buf;
g_buf.ActLen = 0;
g_buf.Len = 0x10000;
g_buf.pData = g_dataBuf;
g_bufoffset = 0;
V558_CaptureStill(&g_buf, JpegNote);
}
******************************************************************************/
UINT16 V558_JpegSetInfo(PV558_JPEGINFO info)
{
	gJpegContext.JpegInfo = info;
	gJpegContext.Valid = TRUE;
	gJpegContext.State = V558_JPEG_NOTINIT;
	gJpegContext.buf.pData = NULL;
	gJpegContext.pcall = NULL;
	gJpegContext.jbufadr = 0;
	gJpegContext.mode = V558_CAPTURE_STILLBRC;
	gJpegContext.twc = gJpegContext.JpegInfo->twc;
	gJpegContext.tcr = gJpegContext.JpegInfo->tcr;
	gJpegContext.entcr = gJpegContext.JpegInfo->entcr;
	gJpegContext.thumbwidth = gJpegContext.JpegInfo->thumbwidth;
	gJpegContext.qf = gJpegContext.JpegInfo->qf;
	gJpegContext.mode = V558_JPEG_IDEL;
	gJpegContext.clkflag = 0;

	return SUCCEED;
}



//===========================================================
//
//		JPEG Base function
//
//===========================================================

void V558_JpegSetWorkMode(UINT8 x)
{
	UINT8 mode;

	switch(gJpegContext.mode)
	{
	case V558_DISPLAY_STILL:
	case V558_DISPLAY_VIDEO:
	case V558_DISPLAY_THUMBNAIL:
	case V558_DECODE_ONLY:
	case V558_DECODE_IPP:
	case V558_DECODE_LCDIF:
	case V558_ENCODE_JPG:
		if( (x & 0x3)  == 0)
		{
			mode = V558_CtrlGetWorkMode();
			if( (mode & 7) == 0)
			{
				V558_CtrlIntEnable(V558_INT_CAP_DONE, 0);
				V558_CtrlIntEnable(V558_INT_CAP_FIFO_FULL, 0);
				gJpegContext.mode = V558_JPEG_IDEL;
				gJpegContext.pcall = NULL;
				gJpegContext.State = V558_JPEG_INITED;
				V558_CtrlSwReset(V558_SW_RESET_JPG);
				ResumeClk();
			}
		}
		break;
	default:
		break;
	}
	V558_SetReg(V558_REG_JPEG_MODE, x & 0x1f);
}

UINT8 V558_JpegGetWorkMode(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_JPEG_MODE, &x);
	return x;
}

UINT8 V558_JpegSimpHeadEnable(void)
{
	UINT8	x;

	x = V558_JpegGetWorkMode();
	V558_JpegSetWorkMode(x | 4);
	return x;
}

UINT8 V558_JpegSimpHeadDisable(void)
{
	UINT8	x;

	x = V558_JpegGetWorkMode();
	V558_JpegSetWorkMode(x & 0xfb);
	return x;
}

UINT8 V558_JpegCustQuantTableEnable(void)
{
	UINT8	x;

	x = V558_JpegGetWorkMode();
	V558_JpegSetWorkMode(x | 8);
	return x;
}

UINT8 V558_JpegCustQuantTableDisable(void)
{
	UINT8	x;

	x = V558_JpegGetWorkMode();
	V558_JpegSetWorkMode(x & 0xf7);
	return x;
}

UINT8 V558_JpegSameQuantTableEnable(void)
{
	UINT8	x;

	x = V558_JpegGetWorkMode();
	V558_JpegSetWorkMode(x | 0x10);
	return x;
}

UINT8 V558_JpegSameQuantTableDisable(void)
{
	UINT8	x;

	x = V558_JpegGetWorkMode();
	V558_JpegSetWorkMode(x & 0xef);
	return x;
}

void V558_JpegSetControl(UINT8 x)
{
	V558_SetReg(V558_REG_JPEG_CTRL, x);
}

UINT8 V558_JpegGetControl(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_JPEG_CTRL, &x);
	return x;
}

void V558_JpegStartEncoder(void)		//only in encoder mode
{
	V558_JpegSetControl(1);
}

void V558_JpegStartDecoder(void)		//only in encoder mode
{
	V558_JpegSetControl(2);
}

void V558_JpegRestartDecoder(void)		//only in encoder mode
{
	V558_JpegSetControl(4);
}

void V558_JpegEnterIdle(void)
{
	V558_JpegSetControl(8);
}

void V558_JpegStartWrQTable(void)
{
	V558_JpegSetControl(0x10);
}

void V558_JpegStartWrHTable(void)
{
	V558_JpegSetControl(0x20);
}

void V558_JpegSelectHTable(UINT8 x)
{
	switch (x)
	{
	case  0x0: 	V558_SetReg(V558_REG_JPEG_CTRL, 0x00); break;
	case  0x1:  V558_SetReg(V558_REG_JPEG_CTRL, 0x80); break;
	case  0x2:  V558_SetReg(V558_REG_JPEG_CTRL, 0x40); break;
	case  0x3:  V558_SetReg(V558_REG_JPEG_CTRL, 0xC0); break;
	default: break;
	}
}

UINT8 V558_JpegGetStatus(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_JPEG_STATUS, &x);
	return x;
}

void V558_JpegSetBRC(UINT8	x)
{
	V558_SetReg(V558_REG_JPEG_BRC, x);
}

UINT8 V558_JpegGetBRC(void)
{
	UINT8	x;
	V558_GetReg(V558_REG_JPEG_BRC, &x);
	return x;
}

UINT8	V558_JpegEnableAutoBRC(void)
{
	UINT8	x;
	x = V558_JpegGetBRC();
	V558_JpegSetBRC(x | 0x80);
	return x;
}

UINT8	V558_JpegDisableAutoBRC(void)
{
	UINT8	x;
	x = V558_JpegGetBRC();
	V558_JpegSetBRC(x & 0x7f);
	return x;
}

void V558_JpegSetTCR(UINT8	x)
{
	V558_SetReg(V558_REG_JPEG_TCR, x);
	gJpegContext.tcr = x;
	gJpegContext.entcr = 1;
}

UINT8 V558_JpegGetTCR(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_JPEG_TCR, &x);
	return x;
}

UINT8 V558_JpegGetQfactor(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_JPEG_QF, &x);
	return x;
}

void V558_JpegSetTWC(UINT32	x)
{
	V558_SetReg(V558_REG_JPEG_TARGET_WC3, (UINT8)x);
	V558_SetReg(V558_REG_JPEG_TARGET_WC2, (UINT8)(x >> 8));
	V558_SetReg(V558_REG_JPEG_TARGET_WC1, (UINT8)(x >> 16));
	V558_SetReg(V558_REG_JPEG_TARGET_WC0, (UINT8)(x >> 24));
	gJpegContext.twc = x;
	gJpegContext.entcr = 0;
}

UINT32 V558_JpegGetTWC(void)
{
	UINT8 x0, x1, x2, x3;

	V558_GetReg(V558_REG_JPEG_TARGET_WC3, &x0);
	V558_GetReg(V558_REG_JPEG_TARGET_WC2, &x1);
	V558_GetReg(V558_REG_JPEG_TARGET_WC1, &x2);
	V558_GetReg(V558_REG_JPEG_TARGET_WC0, &x3);

	return ((UINT32)x3 << 24) | ((UINT32)x2 << 16) | ((UINT32)x1 << 8) | (UINT32)x0;
}

void V558_JpegSetVWC(UINT32	x)
{
	V558_SetReg(V558_REG_JPEG_VIDEO_WC3, (UINT8)x);
	V558_SetReg(V558_REG_JPEG_VIDEO_WC2, (UINT8)(x >> 8));
	V558_SetReg(V558_REG_JPEG_VIDEO_WC1, (UINT8)(x >> 16));
	V558_SetReg(V558_REG_JPEG_VIDEO_WC0, (UINT8)(x >> 24));
}

UINT32 V558_JpegGetVWC(void)
{
	UINT8 x0, x1, x2, x3;

	V558_GetReg(V558_REG_JPEG_VIDEO_WC3, &x0);
	V558_GetReg(V558_REG_JPEG_VIDEO_WC2, &x1);
	V558_GetReg(V558_REG_JPEG_VIDEO_WC1, &x2);
	V558_GetReg(V558_REG_JPEG_VIDEO_WC0, &x3);

	return ((UINT32)x3 << 24) | ((UINT32)x2 << 16) | ((UINT32)x1 << 8) | (UINT32)x0;
}

UINT32 V558_JpegGetHuffmanWC(void)
{
	UINT8 x0, x1, x2, x3;

	V558_GetReg(V558_REG_JPEG_HUFF_WC3, &x0);
	V558_GetReg(V558_REG_JPEG_HUFF_WC2, &x1);
	V558_GetReg(V558_REG_JPEG_HUFF_WC1, &x2);
	V558_GetReg(V558_REG_JPEG_HUFF_WC0, &x3);

	return ((UINT32)x3 << 24) | ((UINT32)x2 << 16) | ((UINT32)x1 << 8) | (UINT32)x0;
}

void V558_JpegSetImageWidth(UINT16	x)
{
	V558_SetReg(V558_REG_JPEG_WIDTH_L, (UINT8)x);
	V558_SetReg(V558_REG_JPEG_WIDTH_H, (UINT8)(x >> 8));
}

UINT16 V558_JpegGetImageWidth(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_JPEG_WIDTH_L, &x1);
	V558_GetReg(V558_REG_JPEG_WIDTH_H, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JpegSetImageHeight(UINT16	x)
{
	V558_SetReg(V558_REG_JPEG_HEIGHT_L, (UINT8)x);
	V558_SetReg(V558_REG_JPEG_HEIGHT_H, (UINT8)(x >> 8));
}

UINT16 V558_JpegGetImageHeight(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_JPEG_HEIGHT_L, &x1);
	V558_GetReg(V558_REG_JPEG_HEIGHT_H, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JpegSetImageSize(V558_SIZE size)
{
	V558_JpegSetImageWidth(size.cx);
	V558_JpegSetImageHeight(size.cy);
}

void V558_JpegGetImageSize(V558_SIZE *size)
{
	size->cx = V558_JpegGetImageWidth();
	size->cy = V558_JpegGetImageHeight();
}

void V558_JpegSetBrcParm(UINT16 x)
{
	V558_SetReg(V558_REG_JPEG_B_L, (UINT8)x);
	V558_SetReg(V558_REG_JPEG_B_H, (UINT8)(x >> 8) & 1);
}

UINT16 V558_JpegGetBrcParm(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JPEG_B_L, &x1);
	V558_GetReg(V558_REG_JPEG_B_H, &x2);

	return ((UINT16)x2 << 8) | (UINT16)x1;
}

UINT16 V558_JpegGetFrameCnt(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JPEG_FRAME_CNT0, &x1);
	V558_GetReg(V558_REG_JPEG_FRAME_CNT1, &x2);

	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JpegWriteDctData(UINT8 x)
{
	V558_SetReg(V558_REG_JPEG_DCT_DATA, x);
}

void V558_JpegWriteQtable(UINT8 x)
{
	V558_SetReg(V558_REG_JPEG_QUAN_T, x);
}

void V558_JpegWriteHtable(UINT8 x)
{
	V558_SetReg(V558_REG_JPEG_HUFF_T, x);
}

void V558_JpegSetComponentNum(UINT8 x)
{
	V558_SetReg(V558_REG_JPEG_NUM_COMPQUANT, x);
}

UINT8 V558_JpegGetComponentNum(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_JPEG_NUM_COMPQUANT, &x);
	return x;
}

void V558_JpegSetComponentParm(UINT8 sel, UINT16 x)
{
	switch(sel)
	{
	case 0:
		V558_SetReg(V558_REG_JPEG_COMP1_P2, (UINT8)x);
		V558_SetReg(V558_REG_JPEG_COMP1_P1, (UINT8)(x >> 8));
		break;
	case 1:
		V558_SetReg(V558_REG_JPEG_COMP2_P2, (UINT8)x);
		V558_SetReg(V558_REG_JPEG_COMP2_P1, (UINT8)(x >> 8));
		break;
	case 2:
		V558_SetReg(V558_REG_JPEG_COMP3_P2, (UINT8)x);
		V558_SetReg(V558_REG_JPEG_COMP3_P1, (UINT8)(x >> 8));
		break;
	case 3:
		V558_SetReg(V558_REG_JPEG_COMP4_P2, (UINT8)x);
		V558_SetReg(V558_REG_JPEG_COMP4_P1, (UINT8)(x >> 8));
		break;
	default:
		break;
	}
}

UINT16 V558_JpegGetComponentParm(UINT8 sel)
{
	UINT8 x1, x2;

	switch(sel)
	{
	case 0:
		V558_GetReg(V558_REG_JPEG_COMP1_P2, &x1);
		V558_GetReg(V558_REG_JPEG_COMP1_P1, &x2);
		break;
	case 1:
		V558_GetReg(V558_REG_JPEG_COMP2_P2, &x1);
		V558_GetReg(V558_REG_JPEG_COMP2_P1, &x2);
		break;
	case 2:
		V558_GetReg(V558_REG_JPEG_COMP3_P2, &x1);
		V558_GetReg(V558_REG_JPEG_COMP3_P1, &x2);
		break;
	case 3:
		V558_GetReg(V558_REG_JPEG_COMP4_P2, &x1);
		V558_GetReg(V558_REG_JPEG_COMP4_P1, &x2);
		break;
	default:
		break;
	}
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JpegSetBlockNum(UINT8 x)
{
	V558_SetReg(V558_REG_JPEG_BLK_WIDTH, x);
}

UINT8 V558_JpegGetBlockNum(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_JPEG_BLK_WIDTH, &x);
	return x;
}

void V558_JpegSetImageParm(UINT8 count, UINT8 *x)
{
	UINT8 i;

	if(count > 15)
		count = 15;

	for(i = 0; i < count; i++)
		V558_SetReg(V558_REG_JPEG_IMGPARA0, x[i]);
}





//===========================================================================
//
//	LBUF module basic function
//
//==========================================================================

UINT8 V558_LbufGetStatus(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_LBUF_STATUS, &x);
	return x;
}

void V558_LbufSetYuvMode(UINT8 x)
{
	V558_SetReg(V558_REG_LBUF_YUVMODE, x & 3);
}

UINT8 V558_LbufGetYuvMode(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_LBUF_YUVMODE, &x);
	return x & 3;
}

void V558_LbufSetWorkmode(UINT8 x)	//only use in decode mode
{
	V558_SetReg(V558_REG_LBUF_WORKMODE, x);
}

UINT8 V558_LbufGetWorkmode(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_LBUF_WORKMODE, &x);
	return x;
}

void V558_LbufSetImageWidth(UINT16 x)
{
	V558_SetReg(V558_REG_LBUF_WIDTH_L, (UINT8)x);
	V558_SetReg(V558_REG_LBUF_WIDTH_H, (UINT8)(x >> 8));
}

UINT16 V558_LbufGetImageWidth(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_LBUF_WIDTH_L, &x1);
	V558_GetReg(V558_REG_LBUF_WIDTH_H, &x2);
	return ((UINT16)(x2 & 1) << 8) | (UINT16)x1;
}

void V558_LbufSetImageHeight(UINT16 x)
{
	V558_SetReg(V558_REG_LBUF_HEIGHT_L, (UINT8)x);
	V558_SetReg(V558_REG_LBUF_HEIGHT_H, (UINT8)(x >> 8));
}

UINT16 V558_LbufGetImageHeight(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_LBUF_HEIGHT_L, &x1);
	V558_GetReg(V558_REG_LBUF_HEIGHT_H, &x2);
	return ((UINT16)(x2 & 1) << 8) | (UINT16)x1;
}

void V558_LbufSetImageSize(V558_SIZE size)
{
	V558_LbufSetImageWidth(size.cx);
	V558_LbufSetImageHeight(size.cy);
}

void V558_LbufGetImageSize(V558_SIZE *size)
{
	size->cx = V558_LbufGetImageWidth();
	size->cy = V558_LbufGetImageHeight();
}




//===========================================================================
//
//	JBUF module basic function
//
//==========================================================================
void V558_JbufSetSourceAddr(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_S_ADDR, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_S_ADDR+1, (UINT8)(x >> 8));
}

UINT16 V558_JbufGetSourceAddr(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_S_ADDR, &x1);
	V558_GetReg(V558_REG_JBUF_S_ADDR+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetThumbAddr(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_T_ADDR, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_T_ADDR+1, (UINT8)(x >> 8));

	gJpegContext.thumbadr = x;
}

UINT16 V558_JbufGetThumbAddr(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_T_ADDR, &x1);
	V558_GetReg(V558_REG_JBUF_T_ADDR+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetDestAddr(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_D_ADDR, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_D_ADDR+1, (UINT8)(x >> 8));
}

UINT16 V558_JbufGetDestAddr(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_D_ADDR, &x1);
	V558_GetReg(V558_REG_JBUF_D_ADDR+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetJpegAddr(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_JPEG_ADDR, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_JPEG_ADDR+1, (UINT8)(x >> 8));
	gJpegContext.jbufadr = x;
}

UINT16 V558_JbufGetJpegAddr(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_JPEG_ADDR, &x1);
	V558_GetReg(V558_REG_JBUF_JPEG_ADDR+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetSourceSize(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_S_BUF_SIZE, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_S_BUF_SIZE+1, (UINT8)(x >> 8));
}

UINT16 V558_JbufGetSourceSize(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_S_BUF_SIZE, &x1);
	V558_GetReg(V558_REG_JBUF_S_BUF_SIZE+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetDestSize(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_D_BUF_SIZE, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_D_BUF_SIZE+1, (UINT8)(x >> 8));
}

UINT16 V558_JbufGetDestSize(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_D_BUF_SIZE, &x1);
	V558_GetReg(V558_REG_JBUF_D_BUF_SIZE+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetJpegSize(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_J_BUF_SIZE, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_J_BUF_SIZE+1, (UINT8)(x >> 8));

	gJpegContext.jbufsize = x+1;
}

UINT16 V558_JbufGetJpegSize(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_J_BUF_SIZE, &x1);
	V558_GetReg(V558_REG_JBUF_J_BUF_SIZE+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetThumbSize(UINT16 x)
{
	V558_SetReg(V558_REG_JBUF_T_BUF_SIZE, (UINT8)x);
	V558_SetReg(V558_REG_JBUF_T_BUF_SIZE+1, (UINT8)(x >> 8));

	gJpegContext.thumbsize = x+1;
}

UINT16 V558_JbufGetThumbSize(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_T_BUF_SIZE, &x1);
	V558_GetReg(V558_REG_JBUF_T_BUF_SIZE+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

UINT16 V558_JbufGetBiuAccLength(void)
{
	UINT8	x1, x2;

	V558_GetReg(V558_REG_JBUF_BIU_ACC_LENGTH0, &x1);
	V558_GetReg(V558_REG_JBUF_BIU_ACC_LENGTH0+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

void V558_JbufSetCapViewConf(UINT8 x)
{
	V558_SetReg(V558_REG_JBUF_CAP_VIEW_CONF, x);
}

UINT16 V558_JbufGetWritePointer(void)
{
	UINT8 x1,x2;
	V558_GetReg(V558_REG_JBUF_J_WEN_PT0, &x1);
	V558_GetReg(V558_REG_JBUF_J_WEN_PT1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

UINT16 V558_JbufGetReadPointer(void)
{
	UINT8 x1,x2;
	V558_GetReg(V558_REG_JBUF_J_RDN_PT0, &x1);
	V558_GetReg(V558_REG_JBUF_J_RDN_PT1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}
