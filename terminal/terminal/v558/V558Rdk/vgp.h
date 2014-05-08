#ifndef _VGP_H_
#define _VGP_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "vjpeg.h"

void   SetPartialRgb(RGBType* rgb , UINT8 r, UINT8 g, UINT8 b);
UINT32 V558_GpRgbToYuv(UINT8* pOut,UINT8* pIn,V558_SIZE size);



#ifdef __cplusplus
}
#endif

#endif //_VGP_H_