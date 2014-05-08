//#include "..\inc\def.h"
#include "v558api.h"
#include "vjpeg.h"
#include "yuv2bmp.h"
#include "string.h"

const UINT16 Value_Vr[]  =
{
  0,   1,   2,   4,   5,   7,   8,   9,  11,  12,  14,  15,  16,  18,  19,  21, 
 22,  23,  25,  26,  28,  29,  30,  32,  33,  35,  36,  37,  39,  40,  42,  43, 
 44,  46,  47,  49,  50,  51,  53,  54,  56,  57,  58,  60,  61,  63,  64,  65, 
 67,  68,  70,  71,  72,  74,  75,  77,  78,  79,  81,  82,  84,  85,  86,  88, 
 89,  91,  92,  93,  95,  96,  98,  99, 100, 102, 103, 105, 106, 107, 109, 110, 
112, 113, 114, 116, 117, 119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 
134, 136, 137, 138, 140, 141, 143, 144, 145, 147, 148, 150, 151, 152, 154, 155, 
157, 158, 159, 161, 162, 164, 165, 166, 168, 169, 171, 172, 173, 175, 176, 178, 
179, 180, 182, 183, 185, 186, 187, 189, 190, 192, 193, 194, 196, 197, 199, 200, 
201, 203, 204, 206, 207, 208, 210, 211, 213, 214, 215, 217, 218, 220, 221, 222, 
224, 225, 227, 228, 229, 231, 232, 234, 235, 236, 238, 239, 241, 242, 244, 245, 
246, 248, 249, 251, 252, 253, 255, 256, 258, 259, 260, 262, 263, 265, 266, 267,
269, 270, 272, 273, 274, 276, 277, 279, 280, 281, 283, 284, 286, 287, 288, 290,
291, 293, 294, 295, 297, 298, 300, 301, 302, 304, 305, 307, 308, 309, 311, 312,
314, 315, 316, 318, 319, 321, 322, 323, 325, 326, 328, 329, 330, 332, 333, 335, 
336, 337, 339, 340, 342, 343, 344, 346, 347, 349, 350, 351, 353, 354, 356, 357 
};

const UINT16 Value_Ug[]  =
{
  0,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   3,   4,   4,   4,   5, 
  5,   5,   6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,  10, 
 11,  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,  15,  16, 
 16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,  21, 
 22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  25,  26,  26,  26,  27, 
 27,  27,  28,  28,  28,  29,  29,  29,  30,  30,  30,  31,  31,  31,  32,  32, 
 33,  33,  33,  34,  34,  34,  35,  35,  35,  36,  36,  36,  37,  37,  37,  38, 
 38,  38,  39,  39,  39,  40,  40,  40,  41,  41,  41,  42,  42,  42,  43,  43, 
 44,  44,  44,  45,  45,  45,  46,  46,  46,  47,  47,  47,  48,  48,  48,  49, 
 49,  49,  50,  50,  50,  51,  51,  51,  52,  52,  52,  53,  53,  53,  54,  54, 
 55,  55,  55,  56,  56,  56,  57,  57,  57,  58,  58,  58,  59,  59,  59,  60, 
 60,  60,  61,  61,  61,  62,  62,  62,  63,  63,  63,  64,  64,  64,  65,  65, 
 66,  66,  66,  67,  67,  67,  68,  68,  68,  69,  69,  69,  70,  70,  70,  71, 
 71,  71,  72,  72,  72,  73,  73,  73,  74,  74,  74,  75,  75,  75,  76,  76, 
 77,  77,  77,  78,  78,  78,  79,  79,  79,  80,  80,  80,  81,  81,  81,  82, 
 82,  82,  83,  83,  83,  84,  84,  84,  85,  85,  85,  86,  86,  86,  87,  87
};

const UINT16 Value_Vg[]  =
{
  0,   0,   1,   2,   2,   3,   4,   5,   5,   6,   7,   7,   8,   9,  10,  10, 
 11,  12,  12,  13,  14,  15,  15,  16,  17,  17,  18,  19,  20,  20,  21,  22, 
 22,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  30,  31,  32,  32,  33, 
 34,  35,  35,  36,  37,  37,  38,  39,  40,  40,  41,  42,  42,  43,  44,  45, 
 45,  46,  47,  47,  48,  49,  50,  50,  51,  52,  52,  53,  54,  55,  55,  56, 
 57,  57,  58,  59,  60,  60,  61,  62,  62,  63,  64,  65,  65,  66,  67,  67, 
 68,  69,  70,  70,  71,  72,  72,  73,  74,  75,  75,  76,  77,  77,  78,  79, 
 80,  80,  81,  82,  82,  83,  84,  85,  85,  86,  87,  87,  88,  89,  90,  90, 
 91,  92,  92,  93,  94,  95,  95,  96,  97,  97,  98,  99, 100, 100, 101, 102, 
102, 103, 104, 105, 105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 112, 113, 
114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 125, 
125, 126, 127, 127, 128, 129, 130, 130, 131, 132, 132, 133, 134, 135, 135, 136, 
137, 137, 138, 139, 140, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 147, 
148, 149, 150, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 
160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 
171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 180, 180, 181, 182
};

const UINT16 Value_Ub[]  =
{
  0,   1,   3,   5,   7,   8,  10,  12,  14,  15,  17,  19,  21,  23,  24,  26, 
 28,  30,  31,  33,  35,  37,  39,  40,  42,  44,  46,  47,  49,  51,  53,  54, 
 56,  58,  60,  62,  63,  65,  67,  69,  70,  72,  74,  76,  78,  79,  81,  83, 
 85,  86,  88,  90,  92,  93,  95,  97,  99, 101, 102, 104, 106, 108, 109, 111, 
113, 115, 117, 118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 
141, 143, 145, 147, 148, 150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 
170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 
198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223, 225, 
227, 228, 230, 232, 234, 235, 237, 239, 241, 242, 244, 246, 248, 250, 251, 253, 
255, 257, 258, 260, 262, 264, 266, 267, 269, 271, 273, 274, 276, 278, 280, 281, 
283, 285, 287, 289, 290, 292, 294, 296, 297, 299, 301, 303, 305, 306, 308, 310, 
312, 313, 315, 317, 319, 320, 322, 324, 326, 328, 329, 331, 333, 335, 336, 338, 
340, 342, 344, 345, 347, 349, 351, 352, 354, 356, 358, 360, 361, 363, 365, 367, 
368, 370, 372, 374, 375, 377, 379, 381, 383, 384, 386, 388, 390, 391, 393, 395, 
397, 399, 400, 402, 404, 406, 407, 409, 411, 413, 414, 416, 418, 420, 422, 423, 
425, 427, 429, 430, 432, 434, 436, 438, 439, 441, 443, 445, 446, 448, 450, 452
};
/**
Description:
	Store rgb data.
Parameters:
	add:   destination  address.
	rgb:   source data.
Returns:
	NULL
*/
void   StoreRgb(UINT8* add, RGBType* rgb)
{
	*add      =   rgb->b;
	*(add+1)  =   rgb->g;
	*(add+2)  =   rgb->r;
}
/**
Description:
	Store raw data.
Parameters:
	add:   destination  address.
	raw:   source data.
Returns:
	NULL
*/
void   StoreRaw(UINT8* add, RawType* raw)
{
	*add      =   raw->high;
	*(add+1)  =   raw->low;
}

/**
Description:
	Change yuv data to rgb data
Parameters:
	pOut :   The output buffer of RGB type
	pIn  :   The input  buffer of YUV type
	rgbFormat: The rgb type of output data. We can choose 0:V558_RGB_888.
	yuvFormat: The yuv type of input data.  We can choose 0:V558_YUV_422 ; 1:V558_YUV_420 ; 
							      2:V558_YUV_411 ; 3:V558_YUV_400.
	size: The image size of input data.
	rate: The rate of compression. Usually, we select 1,meaning that no compression.
Returns:
	The value of data length.
*/
UINT32 V558_YuvToRgb(UINT8* pOut,UINT8* pIn,UINT8 rgbFormat,UINT8 yuvFormat,V558_SIZE size,UINT8 rate) 
{
	UINT8  u_out[MAXWIDTH],v_out[MAXWIDTH];
	UINT16 i,j;
	UINT32 srcadd,dstadd,isrcadd,idstadd;
	YUVType jpg0,jpg1;
	RGBType rgb;
	switch(yuvFormat)
	{
	case V558_YUV_422:
		{
			for(i = 0;i<size.cy ;i=i+rate)
			{
				isrcadd   =   ((i*size.cx)<<1);
				idstadd   =   ((i/rate)*size.cx*3)/rate;
				for(j=0;j<(size.cx>>1);j=j+rate)
				{
					srcadd  = isrcadd  + (j<<2);
					//get yuv data
					jpg0.y  = *(pIn+srcadd+0);
					jpg1.y  = *(pIn+srcadd+1);
					jpg1.u  = jpg0.u  = *(pIn+srcadd+2);
					jpg1.v  = jpg0.v  = *(pIn+srcadd+3);
					//change from yuv to rgb
					YUV2RGB(&rgb,&jpg0);
					dstadd  = idstadd + ((j*3)<<1)/rate;
					StoreRgb(pOut+dstadd , &rgb);
					YUV2RGB(&rgb,&jpg1);
					StoreRgb(pOut+dstadd+3 , &rgb);
				}
			}
		}break;
	case V558_YUV_420:
		{
			for (i=0;i<size.cy;i=i+(rate<<1))
			{
				isrcadd        =   ((i*size.cx*3)>>1);
				idstadd        =   (i*size.cx*3);
				for(j=0;j<(size.cx>>1);j=j+rate)
				{
					//get yuv data
					srcadd     =   isrcadd + (j<<2);
					jpg0.y = *(pIn +srcadd  );
					jpg1.y = *(pIn +srcadd+1);
					u_out[j] = jpg1.u = jpg0.u = *(pIn +srcadd+2);
					v_out[j] = jpg1.v = jpg0.v = *(pIn +srcadd+3);
					//change from yuv to rgb				
					YUV2RGB(&rgb,&jpg0);
					dstadd     =   idstadd +  (j<<1) + (j<<2);
					StoreRgb(pOut+dstadd , &rgb);
					YUV2RGB(&rgb,&jpg1);
					StoreRgb(pOut+dstadd+3, &rgb);
				}
				for(j=0 ; j< (size.cx>>1) ; j = j+rate)
				{
					srcadd     =   isrcadd + (size.cx<<1) + (j<<1);
					jpg0.y = *(pIn+srcadd  );
					jpg1.y = *(pIn+srcadd+1);
					jpg0.u = jpg1.u = u_out[j];
					jpg0.v = jpg1.v = v_out[j];
					//change from yuv to rgb
					YUV2RGB(&rgb,&jpg0);
					dstadd     =   idstadd + size.cx + (size.cx<<1)+((j*3)<<1);
					StoreRgb(pOut+dstadd , &rgb);
					YUV2RGB(&rgb,&jpg1);
					StoreRgb(pOut+dstadd+3, &rgb);
				}
			}	
		}break;
	case V558_YUV_411:
		{
			for (i = 0;i<size.cy ;i=i+rate)
			{
				isrcadd     =   (i*size.cx*3>>1);
				idstadd     =   (i*size.cx<<2) + (i*size.cx<<3);
				for(j=0;j<(size.cx>>2);j=j+rate)
				{
					//get yuv data
					srcadd  = isrcadd + (j<<1) + (j<<2);
					jpg0.y  = *(pIn+srcadd  );
					jpg1.y  = *(pIn+srcadd+1);
					jpg1.u  = jpg0.u  = *(pIn+srcadd+4);
					jpg1.v  = jpg0.v  = *(pIn+srcadd+5);
					//change from yuv to rgb
					YUV2RGB(&rgb,&jpg0);
					dstadd  =idstadd + (j<<2)  + (j<<3);
					StoreRgb(pOut+dstadd , &rgb);
					YUV2RGB(&rgb,&jpg1);
					StoreRgb(pOut+dstadd+3 , &rgb);
					jpg0.y  = *(pIn+srcadd+2);
					jpg1.y  = *(pIn+srcadd+3);
					//change from yuv to rgb
					YUV2RGB(&rgb,&jpg0);
					StoreRgb(pOut+dstadd+6 , &rgb);
					YUV2RGB(&rgb,&jpg1);
					StoreRgb(pOut+dstadd+9 , &rgb);
				}
			}
		}break;
	case V558_YUV_400:
		{
			for (i=0 ; i<size.cy ; i=i+rate)
			{
				isrcadd    = i*size.cx;
				idstadd    = isrcadd + (isrcadd<<1);
				for(j=0 ; j<(size.cx>>1) ; j=j+rate)
				{
					//get yuv data
					srcadd = i*size.cx+(j<<1);
					jpg0.y = *(pIn+srcadd);
					jpg1.y = *(pIn+srcadd+1);
					jpg0.u = jpg0.v = jpg1.u = jpg1.v = 128;
					//change from yuv to rgb
					YUV2RGB(&rgb,&jpg0);
					dstadd = idstadd + (j<<2) + (j<<1);
					StoreRgb(pOut+dstadd , &rgb);
					YUV2RGB(&rgb,&jpg1);
					StoreRgb(pOut+dstadd+3 , &rgb);
				}
			}
		}break;
	default: 
		break;
	}
	size.cx   = size.cx/rate;
	size.cy   = size.cy/rate;
	return   size.cx*size.cy*3;
}

/**
Description:
	Change yuv data to raw data.
Parameters:
	pOut :   The output buffer of RGB type
	pIn  :   The input  buffer of YUV type
	rawFormat: The rgb type of output data. We can choose 1:V558_RGB_565 or 2: V558_RGB_555.
	yuvFormat: The yuv type of input data.  We can choose 0:V558_YUV_422 ; 1:V558_YUV_420 ;
							      2:V558_YUV_411 ; 3:V558_YUV_400.
	size: The image size of input data.
	rate: The rate of compression. Usually, we select 1,meaning that no compression.
Return:
	The value of data length.
*/
UINT32 V558_YuvToRaw(UINT8* pOut,UINT8* pIn,UINT8 rawFormat,UINT8 yuvFormat,V558_SIZE size,UINT8 rate) 
{
	// TODO: Add your control notification handler code here
	UINT8  u_out[MAXWIDTH],v_out[MAXWIDTH];
	UINT32 i,j,srcadd,dstadd,isrcadd,idstadd;
	RawType raw;
	YUVType jpg0,jpg1;
	RGBType rgb;
	switch(yuvFormat)
	{
	case V558_YUV_422:
	case V558_PYUV_422_YYUV:
		{
			for (i = 0;i<size.cy ;i=i+rate)
			{
				isrcadd   =   ((i*size.cx)<<1);
				idstadd   =   ((i/rate)*size.cx<<1)/rate;
				for(j=0;j<(size.cx>>1);j=j+rate)
				{
				srcadd  = isrcadd + (j<<2);
				//get yuv data
				jpg0.y  = *(pIn+srcadd+0);
				jpg1.y  = *(pIn+srcadd+1);
				jpg1.u  = jpg0.u  = *(pIn+srcadd+2);
				jpg1.v  = jpg0.v  = *(pIn+srcadd+3);
				//change from yuv to rgb
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				dstadd  = idstadd +(j<<2)/rate;
				StoreRaw(pOut + dstadd , &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd +2 , &raw);
				}
			}
		}break;
	case V558_YUV_420:
	{
		for ( i=0 ; i<size.cy;i=i+(rate<<1))
		{
			isrcadd        =   ((i*size.cx*3)>>1);
			idstadd        =   (i*size.cx<<1);
			for(j=0;j<(size.cx>>1);j=j+rate)
			{
				//get yuv data
				srcadd     =   isrcadd + (j<<2);
				jpg0.y = *(pIn+srcadd  );
				jpg1.y = *(pIn+srcadd+1);
				u_out[j] = jpg1.u = jpg0.u = *(pIn+srcadd+2);
				v_out[j] = jpg1.v = jpg0.v = *(pIn+srcadd+3);
				//change from yuv to rgb				
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				dstadd     =   idstadd +  (j<<2);
				StoreRaw(pOut + dstadd , &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd+2 , &raw);
			}
			for(j=0;j<(size.cx>>1);j=j+rate)
			{
				srcadd     =   isrcadd + (size.cx<<1) + (j<<1);
				jpg0.y = *(pIn+srcadd  );
				jpg1.y = *(pIn+srcadd+1);
				jpg0.u = jpg1.u = u_out[j];
				jpg0.v = jpg1.v = v_out[j];
				//change from yuv to rgb
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				dstadd     =   idstadd + (size.cx<<1) + (j<<2);
				StoreRaw(pOut + dstadd , &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd+2 , &raw);
			}
		}	
	}	break;
	case V558_YUV_411:
	{
		for (i = 0;i<size.cy ;i=i+rate)
		{
			isrcadd  =  ((i*size.cx*3)>>1);
			idstadd  =  ((i*size.cx)<<3);
			for(j=0;j<(size.cx>>2);j=j+rate)
			{
				//get yuv data
				srcadd  = isrcadd+ (j<<1) + (j<<2);
				
				jpg0.y  = *(pIn+srcadd  );
				jpg1.y  = *(pIn+srcadd+1);
				jpg1.u  = jpg0.u  = *(pIn+srcadd+4);
				jpg1.v  = jpg0.v  = *(pIn+srcadd+5);
				//change from yuv to rgb
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				dstadd  = idstadd + (j<<3);
				StoreRaw(pOut + dstadd , &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd +2 , &raw);
				jpg0.y  = *(pIn+srcadd+2);
				jpg1.y  = *(pIn+srcadd+3);
				//change from yuv to rgb
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd +4, &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd +6, &raw);
			}
		}
	}break;
	case V558_YUV_400:
	{
		for (i=0;i<size.cy;i=i+rate)
		{
			isrcadd   = i*size.cx;
			idstadd   = (isrcadd<<1);
			for(j=0;j<(size.cx>>1);j=j+rate)
			{
				//get yuv data
				srcadd = isrcadd + (j<<1);
				jpg0.y = *(pIn+srcadd);
				jpg1.y = *(pIn+srcadd+1);
				jpg0.u = jpg0.v = jpg1.u = jpg1.v = 128;
				//change from yuv to rgb
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				dstadd = idstadd + (j<<2);
				StoreRaw(pOut + dstadd , &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd+2 , &raw);
			}
		}
	}	break;
	case V558_PYUV_422_YUYV:
		{
			for (i = 0;i<size.cy ;i=i+rate)
			{
				isrcadd =   ((i*size.cx)<<1);
				idstadd =   ((i/rate)*size.cx<<1)/rate;
				for(j=0;j<(size.cx>>1);j=j+rate)
				{
				srcadd  = (((i*size.cx)<<1)  + (j<<2));
				//get yuv data
				jpg0.y  = *(pIn+srcadd+0);
				jpg1.y  = *(pIn+srcadd+2);
				jpg1.u  = jpg0.u  = *(pIn+srcadd+1);
				jpg1.v  = jpg0.v  = *(pIn+srcadd+3);
				//change from yuv to rgb
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				dstadd  = idstadd + (j<<2)/rate;
				StoreRaw(pOut + dstadd , &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd +2, &raw);
				}
			}
		}break;
	case V558_PYUV_422_UYVY:
		{
			for (i = 0;i<size.cy ;i=i+rate)
			{
				isrcadd =   ((i*size.cx)<<1);
				idstadd =   ((i/rate)*size.cx<<1)/rate;
				for(j=0;j<(size.cx>>1);j=j+rate)
				{
				srcadd  = isrcadd + (j<<2);
				//get yuv data
				jpg0.y  = *(pIn+srcadd+1);
				jpg1.y  = *(pIn+srcadd+3);
				jpg1.u  = jpg0.u  = *(pIn+srcadd+0);
				jpg1.v  = jpg0.v  = *(pIn+srcadd+2);
				//change from yuv to rgb
				YUV2RGB(&rgb,&jpg0);
				RGB2RAW(&raw,&rgb,rawFormat);
				dstadd  = idstadd + (j<<2)/rate;
				StoreRaw(pOut + dstadd , &raw);
				YUV2RGB(&rgb,&jpg1);
				RGB2RAW(&raw,&rgb,rawFormat);
				StoreRaw(pOut + dstadd+2 , &raw);
				}
			}
		}break;
	default: break;
	}
	size.cx   = size.cx/rate;
	size.cy  = size.cy/rate;
	return size.cx*size.cy*2;
}
UINT16 CLIPf( int a)
{
        if ( a > 255 )
                return (255);
        else if ( a < 0 )
                return (0);
        else
                return a;

}
void YUV2RGB(RGBType* rgb,YUVType* jpeg)
{
	SINT16   r,g,b;
//  	r = (jpeg->y + ((359 * jpeg->v)>>8) - 179);
//   	g = (jpeg->y +   135 - ((88 * jpeg->u + 183 * jpeg->v)>>8));
//   	b = (jpeg->y + ((454 * jpeg->u)>>8) - 227);

  	r = (jpeg->y +  (Value_Vr[jpeg->v] - 179));
   	g = (jpeg->y +   135 - Value_Ug[jpeg->u]  -Value_Vg[jpeg->v]);
   	b = (jpeg->y + (Value_Ub[jpeg->u] - 227));


   	rgb->r =(UINT8) ((r > 255) ? 255 : ((r < 0) ? 0 : r));
   	rgb->g =(UINT8) ((g > 255) ? 255 : ((g < 0) ? 0 : g));
   	rgb->b =(UINT8) ((b > 255) ? 255 : ((b < 0) ? 0 : b));
}

/**
Description:
	Change rgb data to yuv data.
Parameters:
  	pOut:  The output data buffer.
  	pIn:   The input data buffer.
  	pOutDataType:  The yuv type of output data.We can choose 0:V558_YUV_422 ; 1:V558_YUV_420 ;
  								 2:V558_YUV_411 ; 3:V558_YUV_400.
  	pInDataType:   The rgb type of input data.We can choose 1: V558_RGB_565 or 2:V558_RGB_555.
*/
UINT8 V558_RGB2YUV(PV558_JPEGBUF pOut , PV558_JPEGBUF pIn , UINT8 pOutDataType , UINT8 pInDataType)
{
	UINT32 i;
	RawType raw;
	RGBType rgb;
	YUVType jpg0,jpg1;
	if( (pIn->ActLen%4 )!=0)
		return 0;	
	for( i=0 ; i<(pIn->ActLen>>2); i++)
	{
		raw.high = *(pIn->pData + (i<<2)  );
		raw.low  = *(pIn->pData + (i<<2)+1);
		RawToRGB(&rgb , &raw , pInDataType);
		RGB2YUV( &jpg0 ,&rgb);
		raw.high = *(pIn->pData + (i<<2)+2);
		raw.low  = *(pIn->pData + (i<<2)+3);
		RawToRGB(&rgb , &raw , pInDataType);
		RGB2YUV( &jpg1 ,&rgb);
		*(pOut->pData + (i<<2)  )  =  jpg0.y;
		*(pOut->pData + (i<<2)+1)  =  jpg1.y;
		*(pOut->pData + (i<<2)+2)  =  jpg0.u;
		*(pOut->pData + (i<<2)+3)  =  jpg0.v;
	}	
	pOut->ActLen     =   pIn->ActLen;
	return 1;
}

/**
Description:
	Adjust the brightness of yuv data.
Parameters:
	pBuf:   The data buffer.
	pinfo:  The information of jpeg image.
	brightness: The value of adjust brightness.
Return:
	NULL
Remarks:
	NULL
*/
void  AdjustBrightness(PV558_JPEGBUF pBuf, PV558_JPEG_INDEX pinfo,SINT8 brightness)
{
	UINT32 i,j,len;
	UINT32 add;
	SINT16  value;
	len  =  pBuf->ActLen;
	switch(pinfo->YUVType)
	{
		case V558_YUV_422:  
			for( i=0 ; i<(len>>2) ; i++)
			{
				add   =   (i<<2);
				value   =    *(pBuf->pData + add   );
				value  +=    brightness; 
   			        *(pBuf->pData + add   ) =(UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));

				value   =    *(pBuf->pData + add +1);
				value  +=    brightness; 
   			        *(pBuf->pData + add +1) =(UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
			}
			break;
		case V558_YUV_420:
			for( i=0 ; i<(pinfo->ImageSize.cy>>1) ; i++)
			{
				for( j=0 ; j< (pinfo->ImageSize.cx>>1) ; j++)
				{
					add    =   (i * (pinfo->ImageSize.cx + (pinfo->ImageSize.cx<<1))) +  (j<<2);
					value  =   *(pBuf->pData + add  );
					value  +=  brightness;
					*(pBuf->pData + add  )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
					value  =   *(pBuf->pData + add +1 );
					value  +=  brightness;
					*(pBuf->pData + add +1 )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
				}
				for( j=0 ; j< pinfo->ImageSize.cx ; j++)
				{
					add    =  (i * (pinfo->ImageSize.cx + (pinfo->ImageSize.cx<<1))) + (pinfo->ImageSize.cx<<1) + j;
					value  =   *(pBuf->pData + add  );
					value  +=  brightness;
					*(pBuf->pData + add  )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
					value  =   *(pBuf->pData + add +1 );
					value  +=  brightness;
					*(pBuf->pData + add +1 )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
				}
			}
			break;
		case V558_YUV_411:  
			for( i=0 ; i< ((len/3)>>1) ; i++)
			{
				add     =    (i<<2) + (i<<1);
				value   =    *(pBuf->pData + add   );
				value  +=    brightness;
				*(pBuf->pData + add   ) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
				
				value   =    *(pBuf->pData + add +1);
				value  +=    brightness;
				*(pBuf->pData + add +1) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));

				value   =    *(pBuf->pData + add +2);
				value  +=    brightness;
				*(pBuf->pData + add +2) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));

				value   =    *(pBuf->pData + add +3);
				value  +=    brightness;
				*(pBuf->pData + add +3) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
			}
			break;
		case V558_YUV_400:  
			for( i=0 ; i<len ; i++)
			{
				value   =    *(pBuf->pData + i  );
				value  +=    brightness; 
   			        *(pBuf->pData + i   ) =(UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
			}
			break;
		default: break;
	}
}
/**
Description:
	Adjust the contrast of yuv data.
Parameters:
	pBuf:   The data buffer.
	pinfo:  The information of jpeg image.
	brightness: The value of adjust contrast.
Return:
	NULL
Remarks:
	NULL
*/
void  AdjustContrast(PV558_JPEGBUF pBuf, PV558_JPEG_INDEX pinfo,SINT8 contrast)
{
	UINT32 i,j,len;
	UINT32 add;
	SINT16  value;
	len  =  pBuf->ActLen;
	switch(pinfo->YUVType)
	{
		case V558_YUV_422:  
			for( i=0 ; i<(len>>2) ; i++)
			{
				add   =   (i<<2);
				value   =    *(pBuf->pData + add   );
				value  =    ((value - contrast)<<7)/(128 - contrast);
   			        *(pBuf->pData + add   ) =(UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));

				value   =    *(pBuf->pData + add +1);
				value  =    ((value - contrast)<<7)/(128 - contrast);
   			        *(pBuf->pData + add +1) =(UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
			}
			break;
		case V558_YUV_420:
			for( i=0 ; i<(pinfo->ImageSize.cy>>1) ; i++)
			{
				for( j=0 ; j< (pinfo->ImageSize.cx>>1) ; j++)
				{
					add    =   (i * (pinfo->ImageSize.cx + (pinfo->ImageSize.cx<<1))) +  (j<<2);
					value  =   *(pBuf->pData + add  );
					value  =    ((value - contrast)<<7)/(128 - contrast);
					*(pBuf->pData + add  )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
					value  =   *(pBuf->pData + add +1 );
					value  =    ((value - contrast)<<7)/(128 - contrast);
					*(pBuf->pData + add +1 )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
				}
				for( j=0 ; j< pinfo->ImageSize.cx ; j++)
				{
					add    =  (i * (pinfo->ImageSize.cx + (pinfo->ImageSize.cx<<1))) + (pinfo->ImageSize.cx<<1) + j;
					value  =   *(pBuf->pData + add  );
					value  =    ((value - contrast)<<7)/(128 - contrast);
					*(pBuf->pData + add  )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
					value  =   *(pBuf->pData + add +1 );
					value  =    ((value - contrast)<<7)/(128 - contrast);
					*(pBuf->pData + add +1 )  =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
				}
			}
			break;
		case V558_YUV_411:  
			for( i=0 ; i< ((len/3)>>1) ; i++)
			{
				add     =    (i<<2) + (i<<1);
				value   =    *(pBuf->pData + add   );
				value  =    ((value - contrast)<<7)/(128 - contrast);
				*(pBuf->pData + add   ) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
				
				value   =    *(pBuf->pData + add +1);
				value  =    ((value - contrast)<<7)/(128 - contrast);
				*(pBuf->pData + add +1) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));

				value   =    *(pBuf->pData + add +2);
				value  =    ((value - contrast)<<7)/(128 - contrast);
				*(pBuf->pData + add +2) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));

				value   =    *(pBuf->pData + add +3);
				value  =    ((value - contrast)<<7)/(128 - contrast);
				*(pBuf->pData + add +3) =   (UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
			}
			break;
		case V558_YUV_400:  
			for( i=0 ; i<len ; i++)
			{
				value   =    *(pBuf->pData + i  );
				value  =    ((value - contrast)<<7)/(128 - contrast);
   			        *(pBuf->pData + i   ) =(UINT8) ((value > 255) ? 255 : ((value < 0) ? 0 : value));
			}
			break;
		default: break;
	}
}
void RawToRGB(RGBType* rgb,RawType* raw,UINT8 type)
{
	switch(type)
	{
	case V558_RGB_555:	{
		rgb->b    =   ((raw->high & 0x7C)<<1);
		rgb->g    =   (((raw->high & 0x3)<<6) + ((raw->low & 0x0E0)>>2));
		rgb->r    =   ((raw->low & 0x1f)<<3);
		}break;
	case V558_RGB_565:	{
		rgb->b    =    (raw->high & 0x0f8);
		rgb->g    =    (((raw->high & 0x7)<<5)  + ((raw->low & 0x0e0)>>3));
		rgb->r    =    ((raw->low & 0x1f)<<3);
		}break;
	default:
		break;
	}
	
}
void RGB2RAW(RawType* raw,RGBType* rgb,UINT8 rawFormat)
{
	switch(rawFormat)
	{
	case V558_RGB_565: {	
		rgb->r &= 0xf8;
		rgb->g &= 0xfc;
		rgb->b &= 0xf8;
		raw->high =(UINT8)rgb->r+(UINT8)(rgb->g>>5);
		raw->low  =(UINT8)(rgb->g<<3) +(UINT8) (rgb->b>>3);
		}break;
	case V558_RGB_555:{
		rgb->r &= 0xf8;
		rgb->g &= 0xf8;
		rgb->b &= 0xf8;
		raw->high =(UINT8) (rgb->r>>1)+(UINT8)(rgb->g>>6);
		raw->low  =(UINT8) (rgb->g<<2)+(UINT8) (rgb->b>>3);
		raw->high |= 0x80;
		}break;	
	}
	
}
void RGB2YUV(YUVType* jpg , RGBType* rgb)
{
	SINT16 y,u,v;
  	y = (((77 * (rgb->r)) + (150 * rgb->g) + (29 * rgb->b)) >> 8);
  	u = (((128 * rgb->b) - (43 * rgb->r) - (85 * rgb->g)) >> 8) + 128;
  	v = (((128 * rgb->r) - (107 *rgb->g) - (21 * rgb->b)) >> 8) + 128;
 
  	jpg->y =(UINT8) ((y > 255) ? 255 : ((y < 0) ? 0 : y));
  	jpg->u =(UINT8) ((u > 255) ? 255 : ((u < 0) ? 0 : u));
  	jpg->v =(UINT8) ((v > 255) ? 255 : ((v < 0) ? 0 : v));
}



/********************************************************************************

	Description:
		convert 24bit bmp to 565 format bmp

	Parameters:
		pSrc:	source bmp buffer, include bmp header
		pDst:	destionation bmp buffer, include bmp header
	Returns:
		return the pDst buffer's valid data length
	Remarks:
		state: ok

*********************************************************************************/
SINT32 Bmp24Convert16(UINT8 *pSrc, UINT8 *pDst)
{
	UINT8 *ptr, *pbuf;
	UINT8 r, g, b;
	V558_BITMAPINFOHEADER *pHead;
	SINT16	width, height, x0, x1;
	SINT32 pos, length, size;
	UINT16 count, sub1, sub2;

	memcpy(pDst, pSrc, sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER));
	pHead = (PV558_BITMAPINFOHEADER)(pDst + sizeof(V558_BITMAPFILEHEADER));
	x0 = (SINT16)(pHead->biWidth[0]) & 0Xff;
	x1 = (SINT16)(pHead->biWidth[1]) & 0Xff;
	width = (x1 << 8) | x0;
	x0 = (SINT16)(pHead->biHeight[0]) & 0Xff;
	x1 = (SINT16)(pHead->biHeight[1]) & 0Xff;
	height = (x1 << 8) | x0;
	pHead->biBitCount[0] = 16;
	pHead->biBitCount[1] = 0;
	size = (width * height) << 1;
	pHead->biSizeImage[0] = (UINT8)size;
	pHead->biSizeImage[1] = (UINT8)(size >> 8);
	pHead->biSizeImage[2] = (UINT8)(size >> 16);
	pHead->biSizeImage[3] = (UINT8)(size >> 24);

	ptr = pSrc + sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);
	pbuf = pDst + sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);
	pos = 0;
	length = 0;
	size = (((width * 3 + 3) >> 2) << 2) * height;

	sub1 = 0;
	sub2 = 0;
	count = (UINT16)((width * 3) % 4);
	if(count)
		sub1 = 4 - count;
	count = (UINT16)(width % 2);
	if(count)
		sub2 = 2;

	count = 0;
	while(pos < size)
	{
		r = ptr[pos++] & 0xf8;
		g = ptr[pos++] & 0xfc;
		b = ptr[pos++] & 0xf8;

		pbuf[length++] = b | ((g >> 5) & 0x1f);
		pbuf[length++] = (g << 3) + ((r >> 3) & 0x1f);
		count++;
		if(count == width)
		{
			pos += sub1;
			if(sub2)
			{
				pbuf[length++] = 0;
				pbuf[length++] = 0;
			}
			count = 0;
		}
	}
	return length + sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);
}

/********************************************************************************

	Description:
		convert 24bit bmp to 555 format bmp

	Parameters:
		pSrc:	source bmp buffer, include bmp header
		pDst:	destionation bmp buffer, include bmp header

	Returns:
		return the pDst buffer's valid data length

	Remarks:
		state: ok

*********************************************************************************/
SINT32 Bmp24Convert15(UINT8 *pSrc, UINT8 *pDst)
{
	UINT8 *ptr, *pbuf;
	UINT8 r, g, b;
	V558_BITMAPINFOHEADER *pHead;
	SINT16	width, height, x0, x1;
	SINT32 pos, length, size;
	UINT16 count, sub1, sub2;

	memcpy(pDst, pSrc, sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER));
	pHead = (PV558_BITMAPINFOHEADER)(pDst + sizeof(V558_BITMAPFILEHEADER));
	x0 = (SINT16)(pHead->biWidth[0]) & 0Xff;
	x1 = (SINT16)(pHead->biWidth[1]) & 0Xff;
	width = (x1 << 8) | x0;
	x0 = (SINT16)(pHead->biHeight[0]) & 0Xff;
	x1 = (SINT16)(pHead->biHeight[1]) & 0Xff;
	height = (x1 << 8) | x0;
	pHead->biBitCount[0] = 15;
	pHead->biBitCount[1] = 0;
	size = (width * height) << 1;
	pHead->biSizeImage[0] = (UINT8)size;
	pHead->biSizeImage[1] = (UINT8)(size >> 8);
	pHead->biSizeImage[2] = (UINT8)(size >> 16);
	pHead->biSizeImage[3] = (UINT8)(size >> 24);

	ptr = pSrc + sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);
	pbuf = pDst + sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);
	pos = 0;
	length = 0;
	size = (((width * 3 + 3) >> 2) << 2) * height;

	sub1 = 0;
	sub2 = 0;
	count = (UINT16)((width * 3) % 4);
	if(count)
		sub1 = 4 - count;
	count = (UINT16)(width % 2);
	if(count)
		sub2 = 2;

	count = 0;
	while(pos < size)
	{
		r = ptr[pos++] & 0xf8;
		g = ptr[pos++] & 0xf8;
		b = ptr[pos++] & 0xf8;

		pbuf[length++] = ((b >> 1) + (g >> 6)) | 0x80;
		pbuf[length++] = (g << 2) + (r >> 3);
		if(count == width)
		{
			pos += sub1;
			if(sub2)
			{
				pbuf[length++] = 0;
				pbuf[length++] = 0;
			}
			count = 0;
		}
	}
	return length + sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);
}

/**********************************************************************************
Description:
	Change rgb8 to raw type(rgb565,rgb555)
Parameters:
	palettebuf:	The pointer of palettebuf.The length of this buffer is 768 bytes.
			The first is red,The second is green,The third is blue.
	rgbdata:		The rgb 8 bit data.
	rawformat:	RGB555 or RGB565.	(V558_RGB_565:1  ;  V558_RGB_555:2)
***********************************************************************************/
RawType RGB8ToRaw(UINT8* palettebuf, UINT8 rgbdata, UINT8 rawformat)
{
	RGBType rgb;
	RawType raw;
	rgb.r  =  *(palettebuf + rgbdata + (rgbdata<<1));
	rgb.g  =  *(palettebuf + rgbdata + (rgbdata<<1)+ 1);
	rgb.b  =  *(palettebuf + rgbdata + (rgbdata<<1) + 2);
	
	RGB2RAW(&raw,&rgb,rawformat);
	return raw;
}