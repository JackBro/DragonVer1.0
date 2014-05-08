
#include "internal.h"

const int gTableV2R[]  =
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

const int gTableU2G[]  =
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

const int gTableV2G[]  =
{
	0,   0,   1,   2,   2,   3,   4,   5,   5,   6,   7,   7,   8,   9,  10,  10, 
	11,  12,  12,  13,  14,  15,  15,  16,  17,  17,  18,  19,  20,  20,  21,  22, 
	22,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  30,  31,  32,  32,  33, 
	34,  35,  35,  36,  37,  37,  38,  39,  40,  40,  41,  42,  42,  43,  44,  45, 
	45,  46,  47,  47,  48,  49,  50,  50,  51,  52,  52,  53,  54,  55,  55,  56, 
	57,  57,  58,  59,  60,  60,  61,  62,  62,  63,  64,  65,  65,  66,  67,  67, 
	68,  69,  70,  70,  71,  72,  72,  73,  74,  75,  75,  76,  77,  77,  78,  79, 
	80,  80,  81,  82,  82,  83,  84,  85,  85,  86,  87,  87,  88,  89,  90,  90, 
	91,  92,  92,  93,  94,  95,  95,  96,  97,  97,  98,  99,  100, 100, 101, 102, 
	102, 103, 104, 105, 105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 112, 113, 
	114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 125, 
	125, 126, 127, 127, 128, 129, 130, 130, 131, 132, 132, 133, 134, 135, 135, 136, 
	137, 137, 138, 139, 140, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 147, 
	148, 149, 150, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 
	160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 
	171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 180, 180, 181, 182
};

const int gTableU2B[]  =
{
	0,   1,   3,   5,   7,   8,  10,  12,  14,   15,  17,  19,  21,  23,  24,  26, 
	28,  30,  31,  33,  35,  37,  39,  40,  42,  44,  46,  47,  49,  51,  53,  54, 
	56,  58,  60,  62,  63,  65,  67,  69,  70,  72,  74,  76,  78,  79,  81,  83, 
	85,  86,  88,  90,  92,  93,  95,  97,  99,  101, 102, 104, 106, 108, 109, 111, 
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

//=============	class CYuv2Rgb ==============================

int CYuv2Rgb::rgb2y(int *bgr, int cap)
{
	int val;
//				R				G				B
	val = (77 * bgr[2] + 150 * bgr[1] + 29 * bgr[0]) >> 8;	//y
	cap -= 1;
	return val > cap ? cap : ( val < 0 ? 0 : val );
}

int CYuv2Rgb::rgb2u(int *bgr, int cap)
{
	int val;

	val = ((128 * bgr[0] - 43 * bgr[2] - 85 * bgr[1]) >> 8) + (cap >> 1); //u
	cap -= 1;
	return val > cap ? cap : ( val < 0 ? 0 : val );
}

int CYuv2Rgb::rgb2v(int *bgr, int cap)
{
	int val;

	val = ((128 * bgr[2] - 107 * bgr[1] - 21 * bgr[0]) >> 8) + (cap >> 1); //v
	cap -= 1;
	return val > cap ? cap : ( val < 0 ? 0 : val );
}

int	CYuv2Rgb::bgr2yuv(int bgr)
{
	int val[3], yuv[3];
	
	val[0] = (bgr >> 16) & 0xff;
	val[1] = (bgr >> 8) & 0xff;
	val[2] = bgr & 0xff;
	this->rgb2yuv(val, yuv);

	return (yuv[0] << 16) | (yuv[1] << 8) | yuv[2];
}

void CYuv2Rgb::rgb2yuv(int *bgr, int *yuv, int cap)
{
	yuv[0] = this->rgb2y(bgr, cap);
	yuv[1] = this->rgb2u(bgr, cap);
	yuv[2] = this->rgb2v(bgr, cap);
}

int	 CYuv2Rgb::yuv2r(int *yuv, int cap)
{
	int val;

//  	val = yuv[0] + ( (359 * yuv[2]) >> 8 ) - 179;		//r
	val = yuv[0] + gTableV2R[yuv[2]] - 179;

	//val = (yuv[2] * 359) >> 8;
	//val += yuv[0];
	//if(val > 0x1ff)
	//	val -= 0x1ff;
	//val -= 179;

	cap -= 1;
	return val > cap ? cap : ( val < 0 ? 0 : val );
}

int	 CYuv2Rgb::yuv2g(int *yuv, int cap)
{
	int val;

   	//val = yuv[0] +   135 - ( (88 * yuv[1] + 183 * yuv[2]) >> 8 );	//g
	val = yuv[0] + 135 - (gTableU2G[yuv[1]] + gTableV2G[yuv[2]]);
	cap -= 1;
	return val > cap ? cap : ( val < 0 ? 0 : val );
}

int	 CYuv2Rgb::yuv2b(int *yuv, int cap)
{
	int val;

   	//val = yuv[0] + ( (454 * yuv[1]) >> 8 ) - 227;	//b
	val = yuv[0] + gTableU2B[yuv[1]] - 227;
	cap -= 1;
	return val > cap ? cap : ( val < 0 ? 0 : val );
}

int CYuv2Rgb::yuv2bgr(int yuv)
{
	int val[3], bgr[3];
	
	val[0] = (yuv >> 16) & 0xff;
	val[1] = (yuv >> 8) & 0xff;
	val[2] = yuv & 0xff;
	this->yuv2rgb(val, bgr);

	return (bgr[0] << 16) | (bgr[1] << 8) | bgr[2];
}

void CYuv2Rgb::yuv2rgb(int *yuv, int *bgr, int cap)
{
	bgr[2] = this->yuv2r(yuv, cap);
	bgr[1] = this->yuv2g(yuv, cap);
	bgr[0] = this->yuv2b(yuv, cap);
}


//
//
//
void CYuv2Rgb::getbgr(char *src, int *bgr, int unit)
{
	//int i;
	//char *psrc = src;

	//for(i = 0; i < 3; i++, psrc += unit)
	//	bgr[i] = this->getvalue(psrc, unit);
	CBaseConvert::getvalue(src, bgr, 3, unit);
}

void CYuv2Rgb::getyuv444(char *src, int *yuv, int unit)
{
	//int i;
	//char *psrc = src;

	//for(i = 0; i < 3; i++, psrc += unit)
	//	yuv[i] = this->getvalue(psrc, unit);
	CBaseConvert::getvalue(src, yuv, 3, unit);
}

void CYuv2Rgb::getyuv422(char *src, int *yuv, int order, int unit)
{
	int i;
	char *psrc = src;

	for(i = 0; i < 4; i++, psrc += unit)
		yuv[gImgStreamOrder[order][i]] = this->getvalue(psrc, unit);
}

void CYuv2Rgb::getyuvfrom422(char *src, int *yuv, int order, int unit)
{
	this->getyuv422(src, yuv, order, unit);
	yuv[4] = yuv[1];
	yuv[5] = yuv[2];
}

void CYuv2Rgb::getyuv400(char *src, int *yuv, int unit)
{
	yuv[0] = this->getvalue(src, unit);
}

void CYuv2Rgb::getyuvfrom400(char *src, int *yuv, int unit)
{
	yuv[0] = this->getvalue(src, unit);
	yuv[1] = 0x80;
	yuv[2] = 0x80;
}

void CYuv2Rgb::getyuv411(char *src, int *yuv, int order, int unit)
{
	int i;
	char *psrc = src;

	for(i = 0; i < 6; i++, psrc += unit)
		yuv[gImgStreamOrder[order][i]] = this->getvalue(psrc, unit);
}

void CYuv2Rgb::getyuvfrom411(char *src, int *yuv, int order, int unit)
{
	this->getyuv411(src, yuv, order, unit);
	yuv[4] = yuv[1];
	yuv[5] = yuv[2];
	yuv[7] = yuv[1];
	yuv[8] = yuv[2];
	yuv[10] = yuv[1];
	yuv[11] = yuv[2];
}

void CYuv2Rgb::getyuv420(char *src, int *yuv, int order, int offset, int gap, int unit)
{
	int i;
	char *psrc = src + offset;

	for(i = 0; i < 4; i++, psrc += unit)
		yuv[gImgStreamOrder[order][i]] = this->getvalue(psrc, unit);
	yuv[6] = this->getvalue(src+gap+(offset >> 1), unit);
	yuv[9] = this->getvalue(src+gap+(offset >> 1) + unit, unit);
}

void CYuv2Rgb::setyuv420(char *dst, int *yuv, int order, int offset, int gap, int unit)
{
	int i;
	char *pdst = dst + offset;

	for(i = 0; i < 4; i++, pdst += unit)
		CBaseConvert::setvalue(pdst, yuv[gImgStreamOrder[order][i]], unit);
	CBaseConvert::setvalue(dst+gap+(offset >> 1), yuv[6], unit);
	CBaseConvert::setvalue(dst+gap+(offset >> 1) + unit, yuv[9], unit);
}

void CYuv2Rgb::getyuvfrom420(char *src, int *yuv, int order, int offset, int gap, int unit)
{
	this->getyuv420(src, yuv, order, offset, gap, unit);
	yuv[4] = yuv[1];
	yuv[5] = yuv[2];
	yuv[7] = yuv[1];
	yuv[8] = yuv[2];
	yuv[10] = yuv[1];
	yuv[11] = yuv[2];
}

void CYuv2Rgb::setbgr(char *dst, int *bgr, int unit)
{
	//int i;
	//char *pdst = dst;

	//for(i = 0; i < 3; i++, pdst += unit)
	//	CBaseConvert::setvalue(pdst, bgr[i], unit);
	CBaseConvert::setvalue(dst, bgr, 3, unit);
}

void CYuv2Rgb::setyuv444(char *dst, int *yuv, int unit)
{
	//int i;
	//char *pdst = dst;

	//for(i = 0; i < 3; i++, pdst += unit)
	//	CBaseConvert::setvalue(pdst, yuv[i], unit);
	CBaseConvert::setvalue(dst, yuv, 3, unit);
}

void CYuv2Rgb::setyuv422(char *dst, int *yuv, int order, int unit)
{
	int i;
	char *pdst = dst;

	for(i = 0; i < 4; i++, pdst += unit)
		CBaseConvert::setvalue(pdst, yuv[gImgStreamOrder[order][i]], unit);
}

void CYuv2Rgb::setyuv411(char *dst, int *yuv, int order, int unit)
{
	int i;
	char *pdst = dst;

	for(i = 0; i < 6; i++, pdst += unit)
		CBaseConvert::setvalue(pdst, yuv[gImgStreamOrder[order][i]], unit);
}

int CYuv2Rgb::prefilter(int *dat, int *filter, int count)
{
	//int k;

	//dat[count] *= filter[0];
	//for(k = 0; k < count; k++)
	//{
	//	dat[k] *= filter[count-k];
	//	dat[(count << 1) - k] *= filter[count-k];
	//	dat[count] += dat[k] + dat[(count << 1) - k];
	//}
	//return dat[count];
	CBaseConvert::prefilter(dat, 1, filter, count);
	return dat[count];
}

void CYuv2Rgb::rgbprefilter(int *dat, int *filter, int count)
{
	//int i, k, start, end, offset;

	//offset = count * 3;
	//for(i = 0; i < 3; i++)
	//	dat[offset + i] *= filter[0];

	//for(k = 0, start = 0, end = (count << 1) * 3; k < count; k++, start += 3, end -= 3)
	//{
	//	for(i = 0; i < 3; i++)
	//	{
	//		dat[start + i] *= filter[count-k];
	//		dat[end + i] *= filter[count-k];
	//		dat[offset + i] += dat[start + i] + dat[end + i];
	//	}
	//}
	CBaseConvert::prefilter(dat, 3, filter, count);
}
