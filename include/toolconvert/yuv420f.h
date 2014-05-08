#ifndef _YUV420F_H_
#define _YUV420F_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif


#include "yuv420.h"

class TOOLCONVERT_DLL CYuv420F : public CYuv420
{
public:
	//CYuv420F();
	//~CYuv420F();

public:
	void Rgb2Yyuv(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yuyv(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Uyvy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Vyuy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yvyu(char *src, char *dest, int width, int height, int unit = 1);

	void Rgb2Yuvy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yvuy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Uvyy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Vuyy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yyvu(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yuv420Blk(char * src, char * dest, int width, int height, int unit = 1);
	void Rgb2Jpeg420(char *src, char *dest, int width, int height,int unit = 1, int qfactor = 40);

//private:
//	Rgb2YuvCall	m_pcall[20];


public:
	void Yyuv2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yuyv2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Uyvy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Vyuy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yvyu2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	
	void Yuvy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yvuy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Uvyy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Vuyy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yyvu2Rgb(char *src, char *dest, int width, int height, int unit = 1);

public:
	void Yyuv2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Yuyv2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Uyvy2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Vyuy2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Yvyu2Yuv444(char *src, char *dest, int width, int height, int unit = 1);

	void Yuvy2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Yvuy2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Uvyy2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Vuyy2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Yyvu2Yuv444(char *src, char *dest, int width, int height, int unit = 1);

public:
	void Yuv444_Yyuv(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Yuyv(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Uyvy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Vyuy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Yvyu(char *src, char *dest, int width, int height, int unit = 1);

	void Yuv444_Yuvy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Yvuy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Uvyy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Vuyy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Yyvu(char *src, char *dest, int width, int height, int unit = 1);

public:
	void Yyuv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Yuyv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Uyvy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Vyuy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Yvyu2Blk(char *src, char *dst, int width, int height, int unit = 1);

	void Yuvy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Yvuy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Uvyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Vuyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Yyvu2Blk(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Blk2Yyuv(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yuyv(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Uyvy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Vyuy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yvyu(char *src, char *dst, int width, int height, int unit = 1);
	
	void Blk2Yuvy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yvuy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Uvyy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Vuyy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yyvu(char *src, char *dst, int width, int height, int unit = 1);

	void Blk2Rgb24(char *src, char *dest, int width, int height);

public:
	void Yyuv2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Yuyv2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Uyvy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Vyuy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Yvyu2Lbuf(char *src, char *dst, int width, int height, int unit = 1);

	void Yuvy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Yvuy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Uvyy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Vuyy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Yyvu2Lbuf(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Lbuf2Yyuv(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yuyv(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Uyvy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Vyuy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yvyu(char *src, char *dst, int width, int height, int unit = 1);
	
	void Lbuf2Yuvy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yvuy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Uvyy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Vuyy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yyvu(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Yyuv2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Yuyv2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Uyvy2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Vyuy2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Yvyu2Frame(char *src, char *dst, int width, int height, int unit = 1);

	void Yuvy2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Yvuy2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Uvyy2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Vuyy2Frame(char *src, char *dst, int width, int height, int unit = 1);
	void Yyvu2Frame(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Frame2Yyuv(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Yuyv(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Uyvy(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Vyuy(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Yvyu(char *src, char *dst, int width, int height, int unit = 1);
	
	void Frame2Yuvy(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Yvuy(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Uvyy(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Vuyy(char *src, char *dst, int width, int height, int unit = 1);
	void Frame2Yyvu(char *src, char *dst, int width, int height, int unit = 1);

public:
	void YyuvResize(char *src, char *dest, int srcw, int srch, 
					int dstw, int dsth, int unit = 1);
	void YuyvResize(char *src, char *dest, int srcw, int srch, 
					int dstw, int dsth, int unit = 1);
	void UyvyResize(char *src, char *dest, int srcw, int srch, 
					int dstw, int dsth, int unit = 1);
	void VyuyResize(char *src, char *dest, int srcw, int srch, 
					int dstw, int dsth, int unit = 1);
	void YvyuResize(char *src, char *dest, int srcw, int srch, 
					int dstw, int dsth, int unit = 1);
	void YuvyResize(char *src, char *dest, int srcw, int srch, 
						int dstw, int dsth, int unit = 1);
	void YvuyResize(char *src, char *dest, int srcw, int srch, 
						int dstw, int dsth, int unit = 1);
	void UvyyResize(char *src, char *dest, int srcw, int srch, 
						int dstw, int dsth, int unit = 1);
	void VuyyResize(char *src, char *dest, int srcw, int srch, 
						int dstw, int dsth, int unit = 1);
	void YyvuResize(char *src, char *dest, int srcw, int srch, 
						int dstw, int dsth, int unit = 1);

public:
	void Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);
	void Blk2Mpeg2I(char *src, char *dest, int width, int height);

protected:
	void rgb2yuv420order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv420_rgborder(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv444_420order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv420_444order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv420_blkorder(char *src, char *dst, int width, int height, int order, int unit = 1);
	void blk2yuv420order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv420_blkorder1(char *src, char *dst, int width, int height, int order, int unit = 1);
	void blk2yuv420order1(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv420_lbuforder(char *src, char *dst, int width, int height, int order, int unit = 1);
	void lbuf2yuv420order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv420_resizeorder(char *src, char *dest, int srcw, int srch, 
					int dstw, int dsth, int order, int unit = 1);
	void yuv420_frmorder(char *src, char *dst, int width, int height, int order, int unit = 1);
	void frm2yuv420order(char *src, char *dst, int width, int height, int order, int unit = 1);

private:
	CDbugTools	m_dbug;

};


#endif


