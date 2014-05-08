#include "v558api.h"
#include "vjpeg.h"
#include "yuv2bmp.h"

extern const UINT16 gYuv2Bgr565Table[];
extern const UINT16 gYuv2Rgb565Table[];

UINT32 YuvTo565(UINT8* pOut,UINT8* pIn, UINT8 yuvFormat,V558_SIZE size) 
{
	UINT32 i, j, total, adr, offset = 0;
	UINT16 *pdst = (UINT16 *)pOut;
	UINT8 *psrc = pIn;

	switch(yuvFormat)
	{
	case V558_YUV_422:		//UYVY order
		total = size.cx << 1;
		total *= size.cy;
		for(i = 0; i < total; i += 4, pdst += 2)
		{
			adr = pIn[i+1] << 8;								//y0
			adr |= (pIn[i] & 0xf0) | ((pIn[i+2] & 0xf0) >> 4);	//u0, v0
			pdst[0] = gYuv2Bgr565Table[adr];					
			adr = pIn[i+3] << 8;								//y1
			adr |= (pIn[i] & 0xf0) | ((pIn[i+2] & 0xf0) >> 4);	//u0, v0
			pdst[1] = gYuv2Bgr565Table[adr];
		}
		break;
	case V558_YUV_420:		//UYVY...YY
		total = size.cx << 1;
		for(i = 0; i < size.cy; i += 2)
		{
			for(j = 0, offset = 0; j < size.cx; j += 2, offset += 4)
			{
				adr = pIn[offset+1] << 8;								//y0
				adr |= (psrc[offset] & 0xf0) | ((psrc[offset+2] & 0xf0) >> 4);	//u0, v0
				pdst[j] = gYuv2Bgr565Table[adr];					

				adr = psrc[offset+3] << 8;								//y1
				adr |= (psrc[offset] & 0xf0) | ((psrc[offset+2] & 0xf0) >> 4);	//u0, v0
				pdst[j + 1] = gYuv2Bgr565Table[adr];

				adr = psrc[j+total] << 8;						//y2
				adr |= (psrc[offset] & 0xf0) | ((psrc[offset+2] & 0xf0) >> 4);	//u0, v0
				pdst[j + size.cx] = gYuv2Bgr565Table[adr];

				adr = psrc[j+total+1] << 8;						//y3
				adr |= (psrc[offset] & 0xf0) | ((psrc[offset+2] & 0xf0) >> 4);	//u0, v0
				pdst[j + size.cx] = gYuv2Bgr565Table[adr];
			}
			psrc += total + size.cx;
			pdst += total;
		}
		break;
	case V558_YUV_411:		//YYYYUV order
		total = size.cx * 3;
		total *= size.cy;
		total >>= 1;
		for(i = 0; i < total; i += 6, pdst += 4)
		{
			adr = pIn[i] << 8;
			adr |= (pIn[i+4] & 0xf0) | ((pIn[i+5] & 0xf0) >> 4);
			pdst[0] = gYuv2Bgr565Table[adr];
			adr = pIn[i+1] << 8;
			adr |= (pIn[i+4] & 0xf0) | ((pIn[i+5] & 0xf0) >> 4);
			pdst[1] = gYuv2Bgr565Table[adr];
			adr = pIn[i+2] << 8;
			adr |= (pIn[i+4] & 0xf0) | ((pIn[i+5] & 0xf0) >> 4);
			pdst[2] = gYuv2Bgr565Table[adr];
			adr = pIn[i+3] << 8;
			adr |= (pIn[i+4] & 0xf0) | ((pIn[i+5] & 0xf0) >> 4);
			pdst[3] = gYuv2Bgr565Table[adr];
		}
		break;
	case V558_YUV_400:
		total = size.cx;
		total *= size.cy;
		for(i = 0; i < total; i++, pdst++)
		{
			adr = pIn[i] << 8;
			adr |= 0x88;
			pdst[0] = gYuv2Bgr565Table[adr];
		}
		break;
	default: 
		break;
	}
	return (size.cx * size.cy) << 1;
}
