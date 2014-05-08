#include "v558api.h"
#include "high_gp.h"
#include "vjpeg.h"
#include "yuv2bmp.h"
#include "string.h"
#include <stdlib.h>

V558_GP_CONTEXT gGpContext;
UINT8  decflag;
void DecNote(UINT8 msg, PV558_JPEGBUF ptr)
{
	gGpContext.YUVBuf.ActLen  +=  ptr->ActLen;
	ptr->ActLen              =   0;
	if(msg == MSG_READ_YUV_DONE)
	{
		decflag      =   1;
		gGpContext.decIsEnd = 1;
	}
}
/**********************************************************************************
Description:
	Create SWGP(Software Graphic Processing) handle
Parameters:
	NULL
Returns:
	Return the pointer of SWGP handle.
Remarks:
	NULL
***********************************************************************************/
PV558_GP_HANDLE  V558_SWGPCreate(void)
{
	PV558_GP_HANDLE   pgp;
	UINT8* pbuf;
	pbuf   =   malloc(sizeof(V558_GP_HANDLE));
	pgp    =   (PV558_GP_HANDLE)pbuf;
	//memcpy(pgp, pbuf, sizeof(V558_GP_HANDLE));
	return pgp;
}
/**********************************************************************************
Description:
	Init the SWGP module.
Parameters:
	pdisbuf:  The display buffer.
	pIn:      The input buffer.It stores a jpeg picture.
	pdisrect: The rectangle for display.
Returns:
	Return the pointer of SWGP handle.
Remarks:
	If want to use SWGP module.You need call this function first.
***********************************************************************************/
PV558_GP_HANDLE  V558_SWGPInitService(PV558_JPEGBUF pdisbuf, PV558_JPEGBUF pIn,PV558_RECT pdisrect)
{
	UINT8    *pbuf,*pbuf1;
	//V558_POINT        orgPt       =  {0,0};
	//V558_SIZE	  dispSize_s  = { IPK_DISP_WIDTH, IPK_DISP_HEIGHT };
	//V558_SIZE	  tnSize_s    = { IPK_TN_WIDTH,   IPK_TN_HEIGHT   };
	V558_SIZE         dissize;
	PV558_GP_HANDLE   h  = NULL;

	pbuf   =  malloc(MAX_JPGLENGTH);
	pbuf1  =  malloc(MAX_YUVLENGTH);
	if((pbuf == 0) || (pbuf1 == 0))   return GPFAILED;

	h            =    V558_SWGPCreate();
	if(h == NULL)
		return   h;

	h->gp.inyuvtype            =    V558_YUV_422;       //input data type
	h->gp.outrgbtype           =    V558_RGB_565;       //output data type
	memcpy(pbuf, pIn->pData , pIn->ActLen);

	h->gp.JpgBuf.pData  =  pbuf;
	h->gp.JpgBuf.ActLen =  pIn->ActLen;
	h->gp.JpgBuf.Len    =  0x10000;
	
	h->gp.YUVBuf.pData  =  pbuf1;
	h->gp.YUVBuf.ActLen =  0;
	h->gp.YUVBuf.Len    =  0x8000;
	
	h->gp.OutBuf.pData  =  pdisbuf->pData;
	h->gp.OutBuf.ActLen =  pdisbuf->ActLen;

	decflag             =  0;
	if(!V558_JpegParse(&h->gp.JpgBuf, &h->info))
		V558_HostDecodeIpp(&h->gp.JpgBuf, &h->gp.YUVBuf, 0, &h->info, DecNote,pdisrect->width);
	else 
		return  GPFAILED;
	while(!decflag)
	{
	}
	if(decflag)
	{
		decflag          =  0;
		h->gp.size.cx    =  pdisrect->width;
		h->gp.size.cy    =  (pdisrect->width*h->info.JpgSize.cy)/h->info.JpgSize.cx;   //??? get from ipp
		pdisrect->height =  h->gp.size.cy;
		dissize.cx       =  pdisrect->width;
		dissize.cy       =  pdisrect->height;
		pdisbuf->ActLen  =  V558_YuvToRaw(pdisbuf->pData, h->gp.YUVBuf.pData,
					h->gp.outrgbtype, h->gp.inyuvtype, dissize,1);
		h->gp.YUVBuf.ActLen  =   pdisbuf->ActLen;
		h->gp.Valid      =  1;
		h->gp.State      =  V558_GP_INITED;
	}
	return h;
}
/*********************************************************************************
Description:
	Terminate the service.
Parameters:
	h:    The SWGP handle.
Returns:
	NULL
Remarks:
	If not to use SWGP module, you need call this function to terminate SWGP module.
**********************************************************************************/
void 	V558_SWGPTermService(PV558_GP_HANDLE h)
{
	free(h->gp.JpgBuf.pData);
	h->gp.JpgBuf.pData   =  NULL;
	free(h->gp.YUVBuf.pData);
	h->gp.YUVBuf.pData   =  NULL;
	//free(h->gp.OutBuf.pData);
	//h->gp.OutBuf.pData   =  NULL;
	free(h);
	h   =   NULL;
}
/************************************************************************************
Description:
	Do horizontal mirror.
Parameters:
	h:   The SWGP¡¡handle.
Returns:
	If success, return 1 , else, return 0.
Remarks:
	NULL
*************************************************************************************/
UINT8    V558_SWGPDoHMirror(PV558_GP_HANDLE h)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoHMirror(pbuf , h->gp.YUVBuf.pData , &h->gp.size);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData ,h->gp.YUVBuf.pData ,
					h->gp.outrgbtype ,h->gp.inyuvtype,h->gp.size,1);
	free(pbuf);
	pbuf   =   NULL;
	return 1;
}
/************************************************************************************
Description:
	Do vertical mirror.
Parameters:
	h:   The SWGP¡¡handle.
Returns:
	If success, return 1 , else, return 0.
Remarks:
	NULL
*************************************************************************************/
UINT8    V558_SWGPDoVMirror(PV558_GP_HANDLE h)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoVMirror(pbuf , h->gp.YUVBuf.pData , &h->gp.size);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData ,h->gp.YUVBuf.pData ,
					h->gp.outrgbtype ,h->gp.inyuvtype,h->gp.size,1);
	free(pbuf);
	pbuf   =   NULL;
	return 1;
}
/******************************************************************************************
Description:
	Adjust brightness of the image.
Parameters:	
	h    :    The SWGP handle.
	step :    add lum value with the value of step.
Returns:
	If success,return 1; else,return 0;
Remarks:	
	y  =  ((y + step) > 0xff) ? 0xff :  (y+ step).
******************************************************************************************/
UINT8 V558_SWGPAdjustLum( PV558_GP_HANDLE h , SINT8 step)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpAdjustLum( pbuf, h->gp.YUVBuf.pData, &h->gp.size, step);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData ,h->gp.YUVBuf.pData ,
					h->gp.outrgbtype ,h->gp.inyuvtype,h->gp.size,1);
	free(pbuf);
	pbuf  =NULL;
	return 1;
}
/********************************************************************************************
Description:
	Change the image from color image to gray image.
Parameters:
	h  : The SWGP handle.
Returns:
	If success, return 1, else,return 0;
Remarks:
	u = v = 0x80
*******************************************************************************************/
UINT8 V558_SWGPSetGray(PV558_GP_HANDLE h)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpSetGray(pbuf, h->gp.YUVBuf.pData, &h->gp.size);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData, h->gp.YUVBuf.pData,
					h->gp.outrgbtype, h->gp.inyuvtype, h->gp.size, 1);
	free(pbuf);
	pbuf  = NULL;
	return 1;
}
/****************************************************************************************
Description:
	Do negative.
Parameters:
	h : The SWGP handle.
Returns:
	If success,return 1;else return 0.
Remarks:
	NULL		
******************************************************************************************/
UINT8 V558_SWGPDoNegative(PV558_GP_HANDLE h)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoNegative(pbuf, h->gp.YUVBuf.pData, &h->gp.size);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData, h->gp.YUVBuf.pData, h->gp.outrgbtype,
					 h->gp.inyuvtype, h->gp.size, 1);
	free(pbuf);
	pbuf  =  NULL;
	return 1;
}
/****************************************************************************************
Description:
	Zoom out or zoom in the image.
Parameters:
	h         :The SWGP handle.
	poutsize  :The image size after zoom in or zoom out.
Returns:
	If success, return 1, else return 0.
Remarks:
	NULL	
*****************************************************************************************/
UINT8  V558_SWGPDoZoom(PV558_GP_HANDLE h,PV558_SIZE poutsize)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoZoom(pbuf, h->gp.YUVBuf.pData, poutsize, &h->gp.size);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData, pbuf,	h->gp.outrgbtype,
					h->gp.inyuvtype, h->gp.size, 1);
	free(pbuf);
	pbuf   =   NULL;
	return 1;
}
/*******************************************************************************************
Description:
	Do rotation.
Parameters:	
	h       : The SWGP handle.
	poutsize: The image size of output data.
	degree:   The degree when we rotate. (0:90 degree ; 1:180 degree ; 2:270 degree)
Returns:
	If success, return 1,else return 0.
Remarks:
	NULL
*******************************************************************************************/
UINT8 V558_SWGPDoRotation(PV558_GP_HANDLE h, PV558_SIZE poutsize,UINT8 degree)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoRotation(pbuf, h->gp.YUVBuf.pData, poutsize, &h->gp.size, degree);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.size  = *poutsize;
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData, pbuf,	h->gp.outrgbtype,
					h->gp.inyuvtype, h->gp.size, 1);
	free(pbuf);
	pbuf  =NULL;
	return 1;
}

/********************************************************************************************
Description:
	Adjust the contrast of image.
Parameters:	
	h   :   The SWGP handle.
	grad:   The degree when we adjust contrast. (128>grad >-127)
Returns:
	If success, return 1, else return 0.
Remarks:
	NULL	
*******************************************************************************************/
UINT8 V558_SWGPAdjustContrast(PV558_GP_HANDLE h , SINT8 grad)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpAdjustContrast(pbuf, h->gp.YUVBuf.pData, &h->gp.size, grad);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData, pbuf,	h->gp.outrgbtype,
					h->gp.inyuvtype, h->gp.size, 1);
	free(pbuf);
	pbuf  =  NULL;
	return 1;	
}
/********************************************************************************************
Description:
	Set the color range of image.
Parameters:
	h      :  The SWGP handle.
	utop   :  The max value of u.
	ubottom:  The min value of u.
	vtop   :  The max value of v.
	vbottom:  The min value of v.
Returns:
	If success,return 1,else,return 0.
Remarks:
	             u(out)       =         0x80        .     u(in) < ubottom
	             u(out)       =         u(in)       .     ubottom < u(in) < utop
	             u(out)       =         0x80        .     u(in) > utop

	             v(out)       =         0x80        .     v(in) < vbottom
	             v(out)       =         v(in)       .     vbottom < v(in) < vtop
	             v(out)       =         0x80        .     v(in) > vtop
	             
*******************************************************************************************/
UINT8 V558_SWGPSetColorRange(PV558_GP_HANDLE h, UINT8 utop, UINT8 ubottom, UINT8 vtop, UINT8 vbottom)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpSetColorRange(pbuf, h->gp.YUVBuf.pData, &h->gp.size, utop, ubottom, vtop, vbottom);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData, pbuf,	h->gp.outrgbtype,
					h->gp.inyuvtype, h->gp.size, 1);
	free(pbuf);
	pbuf  = NULL;
	return 1;	
}

/********************************************************************************************
Description:
	Set sepia.
Parameters:
	h        :  The SWGP handle.
	colortype:  Set the type of color.(Red: 0  ;    Green: 1    ;      Blue:2 )
	offset:     The value will add to pointed colortype.
Returns:
	If success, return 1, else return 0.
Remarks:
	NULL
*******************************************************************************************/
UINT8 V558_SWGPSetSepia(PV558_GP_HANDLE h, UINT8 r, UINT8 g, UINT8 b)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpSetSepia(pbuf, h->gp.YUVBuf.pData, &h->gp.size, r, g, b);
	memcpy(h->gp.YUVBuf.pData, pbuf, len);
	h->gp.OutBuf.ActLen  = V558_YuvToRaw(h->gp.OutBuf.pData, pbuf,	h->gp.outrgbtype,
					h->gp.inyuvtype, h->gp.size, 1);
	free(pbuf);
	pbuf    =  NULL;
	return 1;
}
/********************************************************************************************
Description:
	Set frame to the image.
Parameters:
	h           : The SWGP handle.
	pInFrameBuf : The input frame buffer,the data format is 1:V558_RGB_565 or 2: V558_RGB_555
	keycolor    : If the data value of iInFrameBuf is keycolor,it will be changed by the yuvimagbuf.
Returns:
	If success, return 1, else, return 0.
Remarks:
	NULL
*********************************************************************************************/
UINT8 V558_SWGPSetFrame(PV558_GP_HANDLE h , PV558_JPEGBUF pInFrameBuf , UINT8 keycolor)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((h->gp.size.cx  * h->gp.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_YuvToRaw(pbuf, h->gp.YUVBuf.pData, h->gp.outrgbtype,
					h->gp.inyuvtype, h->gp.size, 1);
	h->gp.OutBuf.ActLen  = V558_GpSetFrame(h->gp.OutBuf.pData, pbuf, pInFrameBuf->pData, &h->gp.size, keycolor);
	//memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	//pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,pbuf ,	gGpContext.outrgbtype,
	//				gGpContext.inyuvtype, gGpContext.size, 1);
	free(pbuf);
	pbuf  =  NULL;
	return 1;
}

void V558_GpInitContext(void)
{
	gGpContext.Valid = FALSE;
	gGpContext.State = V558_GP_NOTINIT;
}


/************************************************************************************
Description:
	Begin to do Gp.Get the yuv data after decode.
Parameters:
	pOutBuf:      output  decode raw data(16bit: 1:V558_RGB_565 ; 2: V558_RGB_555)
	pJpgBuf:      input   jpg file.
	psize:        the output size of file.But only input the width of size.
Returns:
	If success, return 0;else if fail, return 1
Remarks:
	If we want to do Gp function,We must call this function first.
************************************************************************************/
UINT8 V558_GpOpen(PV558_JPEGBUF pOutBuf, PV558_JPEGBUF pJpgBuf, PV558_SIZE psize)
{
	UINT8    *pbuf,*pbuf1;
	V558_JPEG_INDEX info;
	pbuf   =  malloc(MAX_JPGLENGTH);
	pbuf1  =  malloc(MAX_YUVLENGTH);
	if((pbuf == 0) || (pbuf1 == 0))   return GPFAILED;
	
	gGpContext.inyuvtype    =    V558_YUV_422;       //input data type
	gGpContext.outrgbtype   =    V558_RGB_565;       //output data type
	memcpy(pbuf, pJpgBuf->pData , pJpgBuf->ActLen);

	gGpContext.JpgBuf.pData  =  pbuf;
	gGpContext.JpgBuf.ActLen =  pJpgBuf->ActLen;
	gGpContext.JpgBuf.Len    =  0x10000;
	
	gGpContext.YUVBuf.pData  =  pbuf1;
	gGpContext.YUVBuf.ActLen =  0;
	gGpContext.YUVBuf.Len    =  0x8000;
	gGpContext.decIsEnd       =  0;
	if(!V558_JpegParse(&gGpContext.JpgBuf, &info))	
		V558_HostDecodeIpp(&gGpContext.JpgBuf, &gGpContext.YUVBuf, 0, &info, DecNote,psize->cx);
	else 
		return  GPFAILED;
	while(! gGpContext.decIsEnd)
	{
	}
	if(gGpContext.decIsEnd)
	{
		gGpContext.size.cx  =  psize->cx;
		gGpContext.size.cy  =  (psize->cx*info.JpgSize.cy)/info.JpgSize.cx;   //??? get from ipp
		psize->cy           =  gGpContext.size.cy;
		pOutBuf->ActLen     =  V558_YuvToRaw(pOutBuf->pData ,gGpContext.YUVBuf.pData ,
					gGpContext.outrgbtype ,gGpContext.inyuvtype,*psize,1);
		gGpContext.Valid    =  1;
		gGpContext.State    = V558_GP_INITED;
	}
	return SUCCEED;
}

/****************************************************************************************
Description:
	Do horizontall mirror.
Parameters:
	pOutBuf: Output the databuf after the horizontal mirror.
Returns:
	If success,return 1;else return 0
Remarks:
	NULL
****************************************************************************************/
UINT8 V558_DoHMirror(PV558_JPEGBUF pOutBuf)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoHMirror(pbuf , gGpContext.YUVBuf.pData , &gGpContext.size);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,gGpContext.YUVBuf.pData ,
					gGpContext.outrgbtype ,gGpContext.inyuvtype,gGpContext.size,1);
	free(pbuf);
	pbuf   =   NULL;
	return 1;
}

/****************************************************************************************
Description:
	Do vertical mirror
Parameters:
	pOutBuf: Output the databuf after the vertical mirror.
Returns:
	If success,return 1,else return 0;
Remarks:
	NULL
****************************************************************************************/
UINT8 V558_DoVMirror(PV558_JPEGBUF pOutBuf)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	
	V558_GpDoVMirror(pbuf , gGpContext.YUVBuf.pData , &gGpContext.size);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,gGpContext.YUVBuf.pData ,
					gGpContext.outrgbtype ,gGpContext.inyuvtype,gGpContext.size,1);
	free(pbuf);
	pbuf   =   NULL;
	return 1;
}

/******************************************************************************************
Description:
	Adjust brightness of the image.
Parameters:	
	pOutBuf: Output the databuf after adjust lum.
	step:    add lum value with the value of step.
Returns:
	If success,return 1; else,return 0;
Remarks:	
	y  =  ((y + step) > 0xff) ? 0xff :  (y+ step).
******************************************************************************************/
UINT8 V558_AdjustLum( PV558_JPEGBUF pOutBuf , SINT8 step)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpAdjustLum( pbuf, gGpContext.YUVBuf.pData, &gGpContext.size, step);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,gGpContext.YUVBuf.pData ,
					gGpContext.outrgbtype ,gGpContext.inyuvtype,gGpContext.size,1);
	free(pbuf);
	pbuf  =NULL;
	return 1;
}
/********************************************************************************************
Description:
	Change the image from color image to gray image.
Parameters:
	pOutBuf: Output the databuf after be gray.
Returns:
	If success, return 1, else,return 0;
Remarks:
	u = v = 0x80
*******************************************************************************************/
UINT8 V558_SetGray(PV558_JPEGBUF pOutBuf)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpSetGray(pbuf, gGpContext.YUVBuf.pData, &gGpContext.size);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,gGpContext.YUVBuf.pData ,
					gGpContext.outrgbtype ,gGpContext.inyuvtype,gGpContext.size,1);
	free(pbuf);
	pbuf  = NULL;
	return 1;
}
/****************************************************************************************
Description:
	Do negative.
Parameters:
	pOutBuf: Output the databuf after be negative.
Returns:
	If success,return 1;else return 0.
Remarks:
	NULL		
******************************************************************************************/
UINT8 V558_DoNegative(PV558_JPEGBUF pOutBuf)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoNegative(pbuf, gGpContext.YUVBuf.pData, &gGpContext.size);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,gGpContext.YUVBuf.pData ,gGpContext.outrgbtype,
					 gGpContext.inyuvtype,gGpContext.size,1);
	free(pbuf);
	pbuf  =  NULL;
	return 1;
}
/****************************************************************************************
Description:
	Zoom out or zoom in the image.
Parameters:
	pOutBuf: Output the databuf after zoom in or zoom out the image.
	poutsize:The image size after zoom in or zoom out.
Returns:
	If success, return 1, else return 0.
Remarks:
	NULL	
*****************************************************************************************/
UINT8  V558_DoZoom(PV558_JPEGBUF pOutBuf,PV558_SIZE poutsize)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoZoom(pbuf, gGpContext.YUVBuf.pData, poutsize, &gGpContext.size);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,pbuf ,	gGpContext.outrgbtype,
					gGpContext.inyuvtype, gGpContext.size, 1);
	free(pbuf);
	pbuf   =   NULL;
	return 1;
}
/*******************************************************************************************
Description:
	Do rotation.
Parameters:	
	pOutBuf: Output the data buffer after rotation
	poutsize: The image size of output data.
	degree:   The degree when we rotate. (0:90 degree ; 1:180 degree ; 2:270 degree)
Returns:
	If success, return 1,else return 0.
Remarks:
	NULL
*******************************************************************************************/
UINT8 V558_DoRotation(PV558_JPEGBUF pOutBuf, PV558_SIZE poutsize,UINT8 degree)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpDoRotation(pbuf, gGpContext.YUVBuf.pData, poutsize, &gGpContext.size, degree);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	gGpContext.size  = *poutsize;
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,pbuf ,	gGpContext.outrgbtype,
					gGpContext.inyuvtype, gGpContext.size, 1);
	free(pbuf);
	pbuf  =NULL;
	return 1;
}

/********************************************************************************************
Description:
	Adjust the contrast of image.
Parameters:	
	pOutBuf: Output the data buffer after adjusting contrast.
	grad:   The degree when we adjust contrast. (128>grad >-127)
Returns:
	If success, return 1, else return 0.
Remarks:
	NULL	
*******************************************************************************************/
UINT8 V558_AdjustContrast(PV558_JPEGBUF pOutBuf , SINT8 grad)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpAdjustContrast(pbuf, gGpContext.YUVBuf.pData, &gGpContext.size, grad);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,pbuf ,	gGpContext.outrgbtype,
					gGpContext.inyuvtype, gGpContext.size, 1);
	free(pbuf);
	pbuf  =  NULL;
	return 1;	
}
/********************************************************************************************
Description:
	Set the color range of image.
Parameters:
	pOutBuf:  Output the data buffer after set color range
	utop   :  The max value of u.
	ubottom:  The min value of u.
	vtop   :  The max value of v.
	vbottom:  The min value of v.
Returns:
	If success,return 1,else,return 0.
Remarks:
	             u(out)       =         0x80        .     u(in) < ubottom
	             u(out)       =         u(in)       .     ubottom < u(in) < utop
	             u(out)       =         0x80        .     u(in) > utop

	             v(out)       =         0x80        .     v(in) < vbottom
	             v(out)       =         v(in)       .     vbottom < v(in) < vtop
	             v(out)       =         0x80        .     v(in) > vtop
	             
*******************************************************************************************/
UINT8 V558_SetColorRange(PV558_JPEGBUF pOutBuf, UINT8 utop, UINT8 ubottom, UINT8 vtop, UINT8 vbottom)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpSetColorRange(pbuf, gGpContext.YUVBuf.pData, &gGpContext.size, utop, ubottom, vtop, vbottom);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,pbuf ,	gGpContext.outrgbtype,
					gGpContext.inyuvtype, gGpContext.size, 1);
	free(pbuf);
	pbuf  = NULL;
	return 1;	
}

/********************************************************************************************
Description:
	Set sepia.
Parameters:
	pOutBuf:    Output the data buffer after set color .
	colortype:  Set the type of color.(Red: 0  ;    Green: 1    ;      Blue:2 )
	offset:     The value will add to pointed colortype.
Returns:
	If success, return 1, else return 0.
Remarks:
	NULL
*******************************************************************************************/
UINT8 V558_SetSepia(PV558_JPEGBUF pOutBuf, UINT8 r, UINT8 g, UINT8 b)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_GpSetSepia(pbuf, gGpContext.YUVBuf.pData, &gGpContext.size, r, g, b);
	memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,pbuf ,	gGpContext.outrgbtype,
					gGpContext.inyuvtype, gGpContext.size, 1);
	free(pbuf);
	pbuf    =  NULL;
	return 1;
}

/********************************************************************************************
Description:
	Set frame to the image.
Parameters:
	pOutBuf     : The output data buffer.
	pInFrameBuf : The input frame buffer,the data format is 1:V558_RGB_565 or 2: V558_RGB_555
	keycolor    : If the data value of iInFrameBuf is keycolor,it will be changed by the yuvimagbuf.
Returns:
	If success, return 1, else, return 0.
Remarks:
	NULL
*********************************************************************************************/
UINT8 V558_SetFrame(PV558_JPEGBUF pOutBuf , PV558_JPEGBUF pInFrameBuf , UINT8 keycolor)
{
	UINT8*   pbuf;
	UINT32   len;
	
	len      =   ((gGpContext.size.cx  * gGpContext.size.cy)<<1);
	pbuf     =   malloc(len);
	if(pbuf==NULL)
		return 0;
	V558_YuvToRaw(pbuf, gGpContext.YUVBuf.pData, gGpContext.outrgbtype,
					gGpContext.inyuvtype, gGpContext.size, 1);
	pOutBuf->ActLen  = V558_GpSetFrame(pOutBuf->pData, pbuf, pInFrameBuf->pData, &gGpContext.size, keycolor);
	//memcpy(gGpContext.YUVBuf.pData, pbuf, len);
	//pOutBuf->ActLen  = V558_YuvToRaw(pOutBuf->pData ,pbuf ,	gGpContext.outrgbtype,
	//				gGpContext.inyuvtype, gGpContext.size, 1);
	free(pbuf);
	pbuf  =  NULL;
	return 1;
}

/*********************************************************************************************
Description:
	Close the gp,and free the memeoy.
Parameters:
	NULL
Returns:
	NULL
Remarks:
	If we end to do gp,we need to call this function to free the memory
*********************************************************************************************/
void V558_GpClose(void)
{
	free(gGpContext.JpgBuf.pData);
	free(gGpContext.YUVBuf.pData);
	gGpContext.YUVBuf.pData = NULL;
	gGpContext.JpgBuf.pData = NULL;
}
