#ifndef _HIGH_HWGP_H_
#define _HIGH_HWGP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HWGPFAILED
#define HWGPFAILED		0x0
#endif

#ifndef SUCCEED
#define SUCCEED		0xff
#endif
#define V558_HWGP_NOTINIT		0
#define V558_HWGP_INITED		0X1

typedef struct  tag_V558_HWGP_CONTEXT
{
	UINT8            Valid;
	UINT8            State;
	
	V558_JPEGBUF  YUVBuf;
	V558_SIZE     size;	

	UINT8            inyuvtype;    // yuv422:0 yuv420:1 yuv411:2  yuv400:3
	UINT8            outrgbtype;   // rgb888:0 rgb565:1 rgb555:2
	UINT8            OutDataType;  // yuv422:0  rgb565:1   rgb555:2   rgb 888:3

	UINT8       displaymode;      //lcd display modes
	UINT8 decIsEnd;
	UINT8 ippspectrl;
	//UINT8 sepiaEn;
	UINT8 yoffset;
	UINT8 uoffset;
	UINT8 voffset;
	//UINT8 colorrangeEn;
	UINT8 utop;
	UINT8 ubottom;
	UINT8 vtop;
	UINT8 vbottom;
	SINT8  LumOffset;
	SINT8  ConstrastOffset;
	UINT8	gammar[17];	/* gamma table relation to panel's gamma */
	UINT8	gammag[17];
	UINT8	gammab[17];
}V558_HWGP_CONTEXT,*PV558_HWGP_CONTEXT;

typedef struct tag_V558_HWGP_HANDLE
{
	V558_HWGP_CONTEXT   hwgp;
	V558_LAYER_PROPERTY proty;
	V558_JPEG_INDEX     info;
}V558_HWGP_HANDLE,*PV558_HWGP_HANDLE;

PV558_HWGP_HANDLE  V558_HWGPCreate(void);
PV558_HWGP_HANDLE V558_HWGPInitService(PV558_JPEGBUF pIn,PV558_RECT disrect);
void V558_HWGPTermService(PV558_HWGP_HANDLE h);
void V558_HWGPAdjustBrightness(PV558_HWGP_HANDLE h, SINT8 bright);
void V558_HWGPAdjustContrast(PV558_HWGP_HANDLE h, SINT8 contrast);
void V558_HWGPLCDRecoverGammaTable(void);
UINT8 V558_HWGPDecode(PV558_JPEGBUF pOut, PV558_JPEGBUF pIn, 
				 PV558_HWGP_HANDLE h, PV558_LAYER_PROPERTY proty);
void V558_HWGPDoNegative(PV558_HWGP_HANDLE h);
void V558_HWGPDoHMirror(PV558_HWGP_HANDLE h);
void V558_HWGPDoVMirror(PV558_HWGP_HANDLE h);
void V558_HWGPDoRotation(PV558_HWGP_HANDLE h,UINT8 degree);
void V558_HWGPSetSepia(PV558_HWGP_HANDLE h);
void V558_HWGPSetColorrange(PV558_HWGP_HANDLE h);
//UINT8 V558_HWGpSetSpecialEffect(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty);
//UINT8 V558_HWGpDoNegative(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty);

//UINT8 V558_HWDoHMirror(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty);
//UINT8 V558_HWDoVMirror(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty);

//UINT8 V558_HWDoRotation(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty);

#ifdef __cplusplus
}
#endif

#endif //_HIGH_HWGP_H_