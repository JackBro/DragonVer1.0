#ifndef _VIMICRO_JPGCONVERT_H_
#define _VIMICRO_JPGCONVERT_H_
#include "ex_jpgconvert.h"


#define JPEGCONVERT_FAILED	1
#define JPEGCONVERT_SUCCEED	0

int JpegEncoder(char *src, char *dst, int width, int height, int qf, int format);
int JpegDecoder(char *src, int len, char *dst);

#endif