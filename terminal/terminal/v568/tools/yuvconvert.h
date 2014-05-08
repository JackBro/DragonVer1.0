#ifndef _YUV_CONVERT_H_
#define _YUV_CONVERT_H_

#ifdef __cplusplus
extern "C" {
#endif

//typedef struct tag_RGBQUAD 
//{ 
//	UINT8 rgbBlue;
//	UINT8 rgbGreen;
//	UINT8 rgbRed;
//	UINT8 rgbReserved;
//} RGBQUAD, *PRGBQUAD;

/***************************************** Base tools **********************************************/

#define MAXFILTERCOUNT	20
#define MAXCYCLECOUNT	20

void CopyBitStreamLow(char *src, char *dst, int size, int dststart, int srcstart);
void BaseSetValueOne(char *dst, int val);
void BaseSetValue(char *dst, int *val, int count);
int BaseGetValueOne(char *src);
void BaseGetValue(char *src, int *val, int count);
void BasePrefilter(int *dat, int cycle, int *filter, int count);

int HorExtent(char *src, char *dest, int width, int height, int cycle, int count);
int VerExtent(char *src, char *dest, int width, int height, int cycle, int count);
int Enlarge(char *src, char *dest, int width, int height, int cycle, int count);
int HorPrefilter(char *src, char *dest, int width, int height, int cycle, int count, int *filter);
int VerPrefilter(char *src, char *dest, int width, int height, int cycle, int count, int *filter);
int Prefilter(char *src, char *dest, int width, int height, int cycle, int count, int *filter);

void YuvAdjustToBlk(char* pbuf, unsigned long len, unsigned char val);

/***************************************** bayer2rgb **********************************************/

int Rgb2Bayer(char *src, char *dst, int order, int width, int height);
#if 0
void Rgb1_Rgb24(int bitvalue, char *dst, char *palette);
void Rgb2_Rgb24(int bitvalue, char *dst, char *palette);
void Rgb4_Rgb24(int bitvalue, char *dst, char *palette);
void Rgb8_Rgb24(int bitvalue, char *dst, char *palette);
#endif
void Rgb444_Rgb24(int bitvalue, char *dst);
void Rgb666_Rgb24(int bitvalue, char *dst);
void Rgb555_Rgb24(char *src, char *dst);
void Rgb565_Rgb24(char *src, char *dst);
void Rgb32_Rgb24(char *src, char *dst);
void Rgb24_Rgb32(char *src, char *dst);
int Rgb24_Rgb444(char *src, char *dst, int width, int height, int startbit, int dither);
int Rgb24_Rgb666(char *src, char *dst, int width, int height, int startbit, int dither);
int Rgb24_Rgb555(char *src, char *dst, int width, int height, int dither);
int Rgb24_Rgb565(char *src, char *dst, int width, int height, int dither);
void Rgb555_Rgb32(char *src, char *dst);
int Rgb32_Rgb555(char *src, char *dst, int width, int height, int dither);
void RgbZinHorSizer(char *src, char *dst, int srcw, int srch, int dstw);
void RgbZinVerSizer(char *src, char *dst, int srcw, int srch, int dsth);
void RgbZoutHorSizer(char *src, char *dst, int srcw, int srch, int dstw);
void RgbZoutVerSizer(char *src, char *dst, int srcw, int srch, int dsth);
int RgbResize(char *src, char *dst, int srcw, int srch, int dstw, int dsth);
int MRgbSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py);
int MRgbPrefilter(char *src, char *dest, int width, int height, int count, int *filter);


/******************************************* yuv2rgb **********************************************/

int  Rgb2Y(int *bgr);
int  Rgb2U(int *bgr);
int  Rgb2V(int *bgr);
void Rgb2Yuv(int *bgr, int *yuv);
int	 Yuv2R(int *yuv);
int  Yuv2B(int *yuv);
int  Yuv2B(int *yuv);
void Yuv2Rgb(int *yuv, int *bgr);

void GetBgr(char *src, int *bgr);
void GetYuv444(char *src, int *yuv);
void GetYuv400(char *src, int *yuv);
void GetYuvfrom400(char *src, int *yuv);
void GetYuv422(char *src, int *yuv, int order);
void GetYuvFrom422(char *src, int *yuv, int order);
void GetYuv420(char *src, int *yuv, int order, int offset, int gap);
void GetYuvFrom420(char *src, int *yuv, int order, int offset, int gap);
void GetYuv411(char *src, int *yuv, int order);
void GetYuvFrom411(char *src, int *yuv, int order);

void SetBgr(char *dst, int *bgr);
void SetYuv444(char *dst, int *yuv);
void SetYuv422(char *dst, int *yuv, int order);
void SetYuv411(char *dst, int *yuv, int order);
void SetYuv420(char *dst, int *yuv, int order, int offset, int gap);

int Yuv2RgbPrefilter(int *dat, int *filter, int count);
void Yuv2RgbRgbPrefilter(int *dat, int *filter, int count);


/******************************************* yuv400 **********************************************/
int Rgb2Yuv400(char *src, char *dst);
int Yuv400_Rgb(char *src, char *dst);

int Yuv444_400(char *src, char *dst);
int Yuv400_444(char *src, char *dst);

int	Yuv2Blk(char *src, char *dst, int width, int height);
int	Blk2Yuv(char *src, char *dst, int width, int height);
int	Yuv2Lbuf(char *src, char *dst, int width, int height);
int	Lbuf2Yuv(char *src, char *dst, int width, int height);

void Yuv400ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw);
void Yuv400ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw);
void Yuv400ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth);
void Yuv400ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth);
int Yuv400Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth);

int MRgb2Yuv400(char *src, char *dest, int width, int height);
int MYuv400_Rgb(char *src, char *dest, int width, int height);
int MYuv444_400(char *src, char *dest, int width, int height);
int MYuv400_444(char *src, char *dest, int width, int height);

int MYuv400Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
				int px, int py);

int Yuv400HorExtent(char *src, char *dest, int width, int height, int count);
int Yuv400VerExtent(char *src, char *dest, int width, int height, int count);
int Yuv400HorPrefilter(char *src, char *dest, int width, int height, 
				int count, int *filter);
int Yuv400VerPrefilter(char *src, char *dest, int width, int height, 
				int count, int *filter);
int MYuv400Enlarge(char *src, char *dest, int width, int height, int count);
int MYuv400Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter);


/******************************************* yuv411 **********************************************/

int Rgb2Yuv411order(char *src, char *dst, int order);
int Yuv411_Rgborder(char *src, char *dst, int order);
int Yuv444_411order(char *src, char *dst, int order);
int Yuv411_444order(char *src, char *dst, int order);
int Yuv411_Blkorder(char *src, char *dst, int order, int width, int height);
int Blk2Yuv411order(char *src, char *dst, int order, int width, int height);
int Yuv411_Lbuforder(char *src, char *dst, int order, int width, int height);
int Lbuf2Yuv411order(char *src, char *dst, int order, int width, int height);
int Yuv411_Resizeorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int order);

int MRgb2Yuv411order(char *src, char *dst, int width, int height, int order);
int MYuv411_Rgborder(char *src, char *dst, int width, int height, int order);
int MYuv444_411order(char *src, char *dst, int width, int height, int order);
int MYuv411_444order(char *src, char *dst, int width, int height, int order);
int MYuv411Sizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int order);


/******************************************* yuv420 **********************************************/

int Rgb2Yuv420order(char *src, char *dst, int width, int order);
int Yuv420_Rgborder(char *src, char *dst, int width, int order);
int Yuv444_420order(char *src, char *dst, int width, int order);
int Yuv420_444order(char *src, char *dst, int width, int order);
int Yuv420_Blkorder(char *src, char *dst, int order, int width, int height);
int Blk2Yuv420order(char *src, char *dst, int order, int width, int height);
int Yuv420_Lbuforder(char *src, char *dst, int order, int width, int height);
int Lbuf2Yuv420order(char *src, char *dst, int order, int width, int height);
int Yuv420_Resizeorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int order);
	
int MRgb2Yuv420order(char *src, char *dst, int width, int height, int order);
int MYuv420_Rgborder(char *src, char *dst, int width, int height, int order);
int MYuv444_420order(char *src, char *dst, int width, int height, int order);
int MYuv420_444order(char *src, char *dst, int width, int height, int order);


/******************************************* yuv422 **********************************************/

int Rgb2Yuv422order(char *src, char *dst, int order);
int Yuv422_Rgborder(char *src, char *dst, int order);
int Yuv422_Blkorder(char *src, char *dst, int order, int width, int height);
int Blk2Yuv422order(char *src, char *dst, int order, int width, int height);
int Yuv422_Lbuforder(char *src, char *dst, int order, int width, int height);
int Lbuf2Yuv422order(char *src, char *dst, int order, int width, int height);
int Yuv422_444order(char *src, char *dst, int order);
int Yuv444_422order(char *src, char *dst, int order);
int Yuv422_Resizeorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int order);

int MRgb2Yuv422order(char *src, char *dst, int width, int height, int order);
int MYuv422_Rgborder(char *src, char *dst, int width, int height, int order);
int MYuv444_422order(char *src, char *dst, int width, int height, int order);
int MYuv422_444order(char *src, char *dst, int width, int height, int order);
int MYuv422Sizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int order);


/******************************************* yuv444 **********************************************/

int Rgb2Yuv444(char *src, char *dst);
int Yuv444_Rgb(char *src, char *dst);

int Yuv444_Blk(char *src, char *dst, int width, int height);
int Blk2Yuv444(char *src, char *dst, int width, int height);
int Yuv444_Lbuf(char *src, char *dst, int width, int height);
int Lbuf2Yuv444(char *src, char *dst, int width, int height);

int Yuv444Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth);

void Yuv444ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw);
void Yuv444ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth);

void Yuv444ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw);
void Yuv444ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth);

int MRgb2Yuv444(char *src, char *dst, int width, int height);
int MYuv444_Rgb(char *src, char *dst, int width, int height);
int MYuv444Sizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py);

int MYuv444Prefilter(char *src, char *dest, int width, int height, int count, int *filter);


#ifdef __cplusplus
}
#endif

#endif

