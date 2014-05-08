#include "internal.h"
//#include <string.h>


const int gYuv420_Order[] =	{ 0, 3, 1, 2 };	

static const UINT16 gValue_Vr[]  =
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

static const UINT16 gValue_Ug[]  =
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

static const UINT16 gValue_Vg[]  =
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

static const UINT16 gValue_Ub[]  =
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


void yuv2rgb(unsigned char *rgb_r, unsigned char *rgb_g, unsigned char *rgb_b,
			 unsigned char jpeg_y, unsigned char jpeg_u, unsigned char jpeg_v)
{
	SINT16   r,g,b;
//  	r = (jpeg->y + ((359 * jpeg->v)>>8) - 179);
//   	g = (jpeg->y +   135 - ((88 * jpeg->u + 183 * jpeg->v)>>8));
//   	b = (jpeg->y + ((454 * jpeg->u)>>8) - 227);

  	r = ((unsigned short)jpeg_y +  (gValue_Vr[jpeg_v] - 179));
   	g = ((unsigned short)jpeg_y +   135 - gValue_Ug[jpeg_u]  - gValue_Vg[jpeg_v]);
   	b = ((unsigned short)jpeg_y + (gValue_Ub[jpeg_u] - 227));


   	*rgb_r =(UINT8) ((r > 255) ? 255 : ((r < 0) ? 0 : r));
   	*rgb_g =(UINT8) ((g > 255) ? 255 : ((g < 0) ? 0 : g));
   	*rgb_b =(UINT8) ((b > 255) ? 255 : ((b < 0) ? 0 : b));
}

int yuv420_444(char *src, char *dst, int width)
{
	int i, j, offset, gap1, gap2, k;
	int yuv[12];
	char *pdst = dst;

	gap1 = width << 1;
	gap2 = width * 3;
	for(i = 0, offset = 0; i < width; i += 2, offset += 4, pdst += 6)
	{
		for(k = 0; k < 4; k++)
			yuv[gYuv420_Order[k]] = ((int)src[offset+k]) & 0xff ;
		yuv[4] = yuv[1];
		yuv[5] = yuv[2];
		yuv[6] = src[gap1 + (offset >> 1)];
		yuv[9] = src[gap1 + (offset >> 1) + 1];
		yuv[7] = yuv[1];
		yuv[10] = yuv[1];
		yuv[8] = yuv[2];
		yuv[11] = yuv[2];

		for(j = 0; j < 6; j++)
		{
			pdst[j] = (char)yuv[j];
			pdst[j+gap2] = (char)yuv[j+6];
		}
	}
	return width * 6;
}

int Yuv420T444(char *src, char *dst, int width, int height)
{
	int size, i, len;
	char *psrc = src;
	char *pdst = dst;

	size = width * 3;
	for(i = 0; i < height; i+=2)
	{
		len = yuv420_444(psrc, pdst, width);
		psrc += size;
		pdst += len;
	}
	return width * height * 3;
}

int yuv444_420(char *src, char *dst, int width)
{
	int i, j, k;
	int yuv[12];
	int gap1, gap2, offset;
	char *psrc = src;

	gap1 = width * 3;
	gap2 = width << 1;
	for(i = 0, offset = 0; i < width; i += 2, offset += 4, psrc += 6)
	{
		for(j = 0; j < 6; j++)
		{
			yuv[j] = (int)psrc[j] & 0xff;
			yuv[j+6] = (int)psrc[gap1+j] & 0xff;
		}

		yuv[1] += yuv[4] + yuv[7] + yuv[10];
		yuv[2] += yuv[5] + yuv[8] + yuv[11];
		yuv[1] >>= 2;
		yuv[2] >>= 2;

		for(k = 0; k < 4; k++)
			dst[offset+k] = (char)yuv[gYuv420_Order[k]];

		dst[gap2 + (offset >> 1)] = (char)yuv[6];
		dst[gap2 + (offset >> 1) + 1] = (char)yuv[9];
	}
	return width * 3;
}

int Yuv444T420(char *src, char *dst, int width, int height)
{
	int size, i, len;
	char *psrc = src;
	char *pdst = dst;

	size = width * 6;
	for(i = 0; i < height; i+=2)
	{
		len = yuv444_420(psrc, pdst, width);
		psrc += size;
		pdst += len;
	}
	return (width * height * 3) >> 1;
}

void Yuv444HorSizer(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, left, right, k;
	int temp1[3], temp2[3], yuv[3];
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
		return;
	}
	for(j = 0; j < srch; j++)
	{
		up = 0;
		for(i = 0; i < srcw; i++, psrc += 3)
		{
			up += dstw;
			if(up < srcw)
				continue;
			right = up - srcw;
			left = srcw - right;
			for(k = 0; k < 3; k++)
			{
				if(right == 0)
				{
					pdst[k] = psrc[k];
				}
				else
				{
					temp1[k] = (int)psrc[k] & 0xff;
					temp2[k] = (int)psrc[k+3] & 0xff;
					yuv[k] = (temp1[k] * left + temp2[k] * right) / srcw;
					pdst[k] = (char)yuv[k];
				}
			}
			pdst += 3;
			up -= srcw;
		}
	}
}

void Yuv444VerSizer(char *src, char *dst, int srcw, int srch, int dsth)
{
	int i, j, up, top, btm, k, offset, gap;
	int temp1[3], temp2[3], yuv[3];
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
		return;
	}
	up = 0;
	gap = srcw * 3;
	for(j = 0; j < srch; j++, psrc += gap)
	{
		up += dsth;
		if(up < srch)
			continue;
		btm = up - srch;
		top = srch - btm;
		for(i = 0, offset = 0; i < srcw; i++, pdst += 3)
		{
			for(k = 0; k < 3; k++, offset ++)
			{
				if(btm == 0)
				{
					pdst[k] = psrc[offset];
				}
				else
				{
					temp1[k] = (int)psrc[offset] & 0xff;
					temp2[k] = (int)psrc[gap + offset] & 0xff;
					yuv[k] = (temp1[k] * top + temp2[k] * btm) / srch;
					pdst[k] = (char)yuv[k];
				}
			}
		}
		up -= srch;
	}
}

int Yuv444_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	if( (dstw > srcw) || (dsth > srch) )
		return 0;
	Yuv444VerSizer(src, src, srcw, srch, dsth);
	Yuv444HorSizer(src, dst, srcw, dsth, dstw);
	return dstw * dsth * 3;
}

int Yuv420_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	Yuv420T444(src, dst, srcw, srch);
	Yuv444_Sizer(dst, src, srcw, srch, dstw, dsth);
	Yuv444T420(src, dst, dstw, dsth);
	return (dstw * dsth * 3) >> 1;
}












void Yuv422HorSizer(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 2);
		return;
	}
	for(j = 0; j < srch; j++)
	{
		up = 0;
		for(i = 0; i < srcw; i += 2, psrc += 4)
		{
			up += dstw;
			if(up < srcw)
				continue;
			pdst[0] = psrc[0];
			pdst[1] = psrc[1];
			pdst[2] = psrc[2];
			pdst[3] = psrc[3];
			pdst += 4;
			up -= srcw;
		}
	}
}

void Yuv422VerSizer(char *src, char *dst, int srcw, int srch, int dsth)
{
	int j, gap, up;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 2);
		return;
	}
	up = 0;
	gap = srcw * 2;
	for(j = 0; j < srch; j++, psrc += gap)
	{
		up += dsth;
		if(up < srch)
			continue;
		memcpy(pdst, psrc, gap);
		pdst += gap;
		up -= srch;
	}
}

int Yuv422_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	if( (dstw > srcw) || (dsth > srch) )
		return 0;
	Yuv422VerSizer(src, src, srcw, srch, dsth);
	Yuv422HorSizer(src, dst, srcw, dsth, dstw);
	return dstw * dsth * 2;
}
