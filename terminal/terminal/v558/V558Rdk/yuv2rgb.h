#ifndef _V558_YUV2RGB_H_
#define _V558_YUV2RGB_H_

	int Yuv420T444(char *src, char *dst, int width, int height);
	int Yuv444T420(char *src, char *dst, int width, int height);
	int Yuv444_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth);
	int Yuv420_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth);


#endif


