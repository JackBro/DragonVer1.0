#ifndef _TRM_YUV2RGB_H_
#define _TRM_YUV2RGB_H_

#ifdef __cplusplus
extern "C" {
#endif

int Yuv420T444(char *src, char *dst, int width, int height);
int Yuv444T420(char *src, char *dst, int width, int height);
int Yuv444_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth);
int Yuv420_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth);
int Yuv422_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth);
void yuv2rgb(unsigned char *rgb_r, unsigned char *rgb_g, unsigned char *rgb_b,
			 unsigned char jpeg_y, unsigned char jpeg_u, unsigned char jpeg_v);

#ifdef __cplusplus
}
#endif

#endif



