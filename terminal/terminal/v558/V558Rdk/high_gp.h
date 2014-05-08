#ifndef _HIGH_GP_H_
#define _HIGH_GP_H_

#ifdef __cplusplus
extern "C" {
#endif




#ifndef GPFAILED
#define GPFAILED		0x0
#endif

#ifndef SUCCEED
#define SUCCEED		0xff
#endif

#ifndef MAX_YUVLENGTH
#define MAX_YUVLENGTH	0x8000
#endif

#ifndef MAX_JPGLENGTH
#define MAX_JPGLENGTH	0x8000
#endif

#ifndef IPK_DISP_WIDTH
#define IPK_DISP_WIDTH   0x80
#endif

#ifndef IPK_DISP_HEIGHT
#define IPK_DISP_HEIGHT  0x60
#endif

#ifndef IPK_TN_WIDTH
#define IPK_TN_WIDTH     0x80
#endif

#ifndef IPK_TN_HEIGHT
#define IPK_TN_HEIGHT    0xa0
#endif


#define V558_GP_NOTINIT		0
#define V558_GP_INITED		0X1


typedef struct tag_V558_GP_CONTEXT{
	UINT8            Valid;        
	UINT8            State;
	
	V558_JPEGBUF     JpgBuf;       //buffer to store the jpeg data
	V558_JPEGBUF     YUVBuf;       //buffer to store the yuv data
	V558_JPEGBUF     YUVBuf1;      //the other buffer to store the jpeg data
	V558_JPEGBUF     OutBuf;       //the output buffer.
	UINT8            decIsEnd;     //the flag that decode is end or no.
	UINT8            inyuvtype;    // yuv422:0 yuv420:1 yuv411:2  yuv400:3
	UINT8            outrgbtype;   // rgb888:0 rgb565:1 rgb555:2
	UINT8            OutDataType;  // yuv422:0  rgb565:1   rgb555:2   rgb 888:3
	V558_SIZE        size;	       // the image size.
}V558_GP_CONTEXT,*PV558_GP_CONTEXT;
typedef struct tag_V558_GP_HANDLE
{
	V558_GP_CONTEXT     gp;
	V558_LAYER_PROPERTY proty;
	V558_JPEG_INDEX     info;
}V558_GP_HANDLE,*PV558_GP_HANDLE;
//*******************************************************************
//								    //
//   the newest version  on SWGP(Software Graphic Processing)	    //	
//								    //	 
//*******************************************************************
PV558_GP_HANDLE  V558_SWGPCreate(void);
PV558_GP_HANDLE  V558_SWGPInitService(PV558_JPEGBUF pdisbuf, PV558_JPEGBUF pIn,PV558_RECT pdisrect);
void             V558_SWGPTermService(PV558_GP_HANDLE h);
UINT8 V558_SWGPDoHMirror(PV558_GP_HANDLE h);
UINT8 V558_SWGPDoVMirror(PV558_GP_HANDLE h);
UINT8 V558_SWGPAdjustLum( PV558_GP_HANDLE h , SINT8 step);
UINT8 V558_SWGPSetGray(PV558_GP_HANDLE h);
UINT8 V558_SWGPDoNegative(PV558_GP_HANDLE h);
UINT8  V558_SWGPDoZoom(PV558_GP_HANDLE h,PV558_SIZE poutsize);
UINT8 V558_SWGPDoRotation(PV558_GP_HANDLE h, PV558_SIZE poutsize,UINT8 degree);
UINT8 V558_SWGPAdjustContrast(PV558_GP_HANDLE h , SINT8 grad);
UINT8 V558_SWGPSetColorRange(PV558_GP_HANDLE h, UINT8 utop, UINT8 ubottom, UINT8 vtop, UINT8 vbottom);
UINT8 V558_SWGPSetSepia(PV558_GP_HANDLE h, UINT8 r, UINT8 g, UINT8 b);
UINT8 V558_SWGPSetFrame(PV558_GP_HANDLE h , PV558_JPEGBUF pInFrameBuf , UINT8 keycolor);


//*******************************************************************
//								    //
//   the old version  on SWGP(Software Graphic Processing)	    //	
//								    //	 
//*******************************************************************void  V558_GpInitContext(void);
UINT8 V558_GpOpen(PV558_JPEGBUF pOutBuf, PV558_JPEGBUF pJpgBuf, PV558_SIZE psize);
UINT8 V558_DoHMirror(PV558_JPEGBUF pOutBuf);
UINT8 V558_DoVMirror(PV558_JPEGBUF pOutBuf );
UINT8 V558_AdjustLum(PV558_JPEGBUF pOutBuf , SINT8 step);
UINT8 V558_SetGray(PV558_JPEGBUF pOutBuf);
UINT8 V558_DoNegative(PV558_JPEGBUF pOutBuf);
UINT8 V558_DoZoom(PV558_JPEGBUF pOutBuf,PV558_SIZE poutsize);
UINT8 V558_DoRotation(PV558_JPEGBUF pOutBuf, PV558_SIZE poutsize,UINT8 degree);
UINT8 V558_AdjustContrast(PV558_JPEGBUF pOutBuf , SINT8 grad);
UINT8 V558_SetColorRange(PV558_JPEGBUF pOutBuf , UINT8 utop,UINT8 ubottom,UINT8 vtop,UINT8 vbottom);
UINT8 V558_SetSepia(PV558_JPEGBUF pOutBuf , UINT8 r, UINT8 g, UINT8 b);
UINT8 V558_SetFrame(PV558_JPEGBUF pOutBuf , PV558_JPEGBUF pInFrameBuf , UINT8 keycolor);
void  V558_GpClose(void);


#ifdef __cplusplus
}
#endif

#endif //_HIGH_GP_H_