#ifndef _YUV2BMP_H_
#define _YUV2BMP_H_

#ifdef __cplusplus
extern "C" {
#endif


#define DEFHEIGHT     96//480
#define DEFWIDTH      128//640
#define MAXHEIGHT     480*3
#define MAXWIDTH      640*3


UINT32 Yuv2Bmp(UINT8* pIn,UINT8* pOut,UINT8 Format,V558_SIZE size,UINT8 rate);
UINT32 V558_YuvToRaw(UINT8* pOut,UINT8* pIn,UINT8 rawFormat,UINT8 yuvFormat,V558_SIZE size,UINT8 rate);
UINT32 V558_YuvToRgb(UINT8* pOut,UINT8* pIn,UINT8 rgbFormat,UINT8 yuvFormat,V558_SIZE size,UINT8 rate);
void RawToRGB(RGBType* rgb,RawType* raw,UINT8 type);
void  AdjustBrightness(PV558_JPEGBUF pBuf, PV558_JPEG_INDEX pinfo,SINT8 brightness);
void  AdjustContrast(PV558_JPEGBUF pBuf, PV558_JPEG_INDEX pinfo,SINT8 contrast);
void RGB2YUV(YUVType* jpg,RGBType* rgb);
void   StoreRgb(UINT8* add, RGBType* rgb);
void   StoreRaw(UINT8* add, RawType* raw);
RawType RGB8ToRaw(UINT8* palettebuf, UINT8 rgbdata, UINT8 rawformat);

UINT16    CLIPf(int a); 
#ifdef __cplusplus
}
#endif



#endif // _YUV2BMP_H_
