
#ifndef _YUVT565_H_
#define _YUVT565_H_

#ifdef __cplusplus
extern "C" {
#endif

extern const UINT16 gYuv2Bgr565Table[];
extern const UINT16 gYuv2Rgb565Table[];

UINT32 YuvTo565(UINT8* pOut,UINT8* pIn, UINT8 yuvFormat,V558_SIZE size);

#ifdef __cplusplus
}
#endif

#endif