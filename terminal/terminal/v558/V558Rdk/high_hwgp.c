#include "v558api.h"
#include "high_hwgp.h"
#include "yuv2bmp.h"
#include "vjpeg.h"
#include "string.h"
#include <stdlib.h>

V558_HWGP_HANDLE gHWGpHandle;

void HWGpDecNote(UINT8 msg, PV558_JPEGBUF ptr)
{
	memcpy((char *)(gHWGpHandle.hwgp.YUVBuf.pData + gHWGpHandle.hwgp.YUVBuf.ActLen), (char *)ptr->pData, ptr->ActLen);
	gHWGpHandle.hwgp.YUVBuf.ActLen  +=  ptr->ActLen;
	ptr->ActLen              =   0;
	if(msg == MSG_READ_YUV_DONE)
		gHWGpHandle.hwgp.decIsEnd      =   1;
}
/*****************************************************************************
Description:
	Create a handle of Hardware Graphic Processing(HWGP).
Parameters:
	NULL
Returns:
	Return a handle.
Remarks:
	NULL
	
*****************************************************************************/
PV558_HWGP_HANDLE  V558_HWGPCreate(void)
{
	//V558_HWGP_HANDLE  h;
	return &gHWGpHandle;
}
/******************************************************************************
Description:
	Initiation.
Parameters:
	pIn:      Input a jpegbuf,include a jpeg file to decode.
	disrect:  This parameter come from TCL's program. The meaning is dispelay rectangle.But,here,
	          It is null.
Returns:
	Return a handle.
Remarks:
	If you want to do HWGP,you need to call this function first.	
*******************************************************************************/
PV558_HWGP_HANDLE V558_HWGPInitService(PV558_JPEGBUF pIn,PV558_RECT disrect)
{
	PV558_HWGP_HANDLE   h;
	UINT8* pbuf;
	h      =   V558_HWGPCreate();
	pbuf   =   malloc(pIn->ActLen);
	memcpy(pbuf,pIn->pData,pIn->ActLen);
	h->hwgp.YUVBuf.pData  =  pbuf;
	h->hwgp.YUVBuf.ActLen =  pIn->ActLen;
	if(V558_JpegParse(&h->hwgp.YUVBuf, &h->info))
		return HWGPFAILED;
	return h;	
}
/*****************************************************************************
Description:
	Terminate the HWGP.
Parameters:
	h:   The handle.
Returns:
	NULL
Remarks:
	If we shop HWGP,we need to call this function.	
******************************************************************************/
void V558_HWGPTermService(PV558_HWGP_HANDLE h)
{
	free(h->hwgp.YUVBuf.pData);
	h->hwgp.YUVBuf.pData  =   NULL;
}
/*****************************************************************************
Description:
	Adjusting the  LCD Panel Gamma Table value to adjust the brightness.
Parameters:
	h:       The handle.
	bright:  the value of brightness.
Returns:
	NULL
Remarks:
	NULL
******************************************************************************/
void V558_HWGPAdjustBrightness(PV558_HWGP_HANDLE h, SINT8 bright)
{
	UINT8 i;
	SINT16 r,g,b;
	h->hwgp.LumOffset   =   bright;
	V558_HWGPLCDRecoverGammaTable();
	V558_LcdGetGammaTable(h->hwgp.gammar, h->hwgp.gammag, h->hwgp.gammab);
	for( i=0 ; i<17 ; i++)
	{
		r   = (*(h->hwgp.gammar+i) + bright);
		g   = (*(h->hwgp.gammag+i) + bright);
		b   = (*(h->hwgp.gammab+i) + bright);
		
		*(h->hwgp.gammar+i) =(UINT8) ((r > 255) ? 255 : ((r < 0) ? 0 : r));
   		*(h->hwgp.gammag+i) =(UINT8) ((g > 255) ? 255 : ((g < 0) ? 0 : g));
   		*(h->hwgp.gammab+i) =(UINT8) ((b > 255) ? 255 : ((b < 0) ? 0 : b));
	}
	V558_LcdSetGammaTable(h->hwgp.gammar, h->hwgp.gammag, h->hwgp.gammab);
}
/*****************************************************************************
Description:
	Adjusting the  LCD Panel Gamma Table value to adjust the contrast.
Parameters:
	h:       The handle.
	contrast:  the value of contrast.
Returns:
	NULL
Remarks:
	NULL
******************************************************************************/
void V558_HWGPAdjustContrast(PV558_HWGP_HANDLE h, SINT8 contrast)
{
	UINT8 i;
	SINT16 r,g,b;
	h->hwgp.ConstrastOffset   =   contrast;
	V558_HWGPLCDRecoverGammaTable();
	V558_LcdGetGammaTable(h->hwgp.gammar, h->hwgp.gammag, h->hwgp.gammab);
	for( i=0 ; i<17 ; i++)
	{
		r   = ((*(h->hwgp.gammar+i) - contrast)<<7)/(128 - contrast);
		g   = ((*(h->hwgp.gammag+i) - contrast)<<7)/(128 - contrast);
		b   = ((*(h->hwgp.gammab+i) - contrast)<<7)/(128 - contrast);
		
		*(h->hwgp.gammar+i) =(UINT8) ((r > 255) ? 255 : ((r < 0) ? 0 : r));
   		*(h->hwgp.gammag+i) =(UINT8) ((g > 255) ? 255 : ((g < 0) ? 0 : g));
   		*(h->hwgp.gammab+i) =(UINT8) ((b > 255) ? 255 : ((b < 0) ? 0 : b));
	}
	V558_LcdSetGammaTable(h->hwgp.gammar, h->hwgp.gammag, h->hwgp.gammab);
}
/****************************************************************************
Description:
	Recover the gamma table.
Parameters:
	NULL
Returns:
	NULL
Remarks:
	NULL	
*****************************************************************************/
void V558_HWGPLCDRecoverGammaTable(void)
{
	V558_LcdInitGammaTable();
}
/*****************************************************************************
Description:
	
******************************************************************************/
UINT8 V558_HWGPDecode(PV558_JPEGBUF pOut, PV558_JPEGBUF pIn, 
				 PV558_HWGP_HANDLE h, PV558_LAYER_PROPERTY proty)
{
	UINT8*  pbuf;
	UINT32   buflen;

	memcpy(&gHWGpHandle, h, sizeof(V558_HWGP_HANDLE));
	/////////set special effect
	V558_IppSetSpeCtrl(gHWGpHandle.hwgp.ippspectrl);
	if( (gHWGpHandle.hwgp.ippspectrl & 0x1) && ((gHWGpHandle.hwgp.ippspectrl & 0xE) == 0x0) )
	{
		V558_IppSetUoffset(gHWGpHandle.hwgp.uoffset);
		V558_IppSetVoffset(gHWGpHandle.hwgp.voffset);
	}	
	if( (gHWGpHandle.hwgp.ippspectrl & 0x1) && ((gHWGpHandle.hwgp.ippspectrl & 0x2)) )
	{
		V558_IppSetUTopThrd(gHWGpHandle.hwgp.utop);
		V558_IppSetUBottomThrd(gHWGpHandle.hwgp.ubottom);
		V558_IppSetVTopThrd(gHWGpHandle.hwgp.vtop);
		V558_IppSetVBottomThrd(gHWGpHandle.hwgp.vbottom);
	}
	V558_LcdSetGammaTable(h->hwgp.gammar, h->hwgp.gammag, h->hwgp.gammab);
	buflen   =   ((h->info.JpgSize.cx  * h->info.JpgSize.cy)<<1);
	pbuf     =    malloc(buflen);
	if(pbuf  == NULL)
		return  HWGPFAILED;
	gHWGpHandle.hwgp.decIsEnd           =  0;
	gHWGpHandle.hwgp.YUVBuf.Len         =  buflen;
	gHWGpHandle.hwgp.YUVBuf.ActLen      =  0;
	gHWGpHandle.hwgp.YUVBuf.pData	    =  pbuf;
	V558_HostDecode(pIn, &gHWGpHandle.hwgp.YUVBuf, 0, &h->info, HWGpDecNote);
	while(! gHWGpHandle.hwgp.decIsEnd)
	{
	}
	if(gHWGpHandle.hwgp.decIsEnd)
	{
		gHWGpHandle.hwgp.size.cx      =  h->info.JpgSize.cx;
		gHWGpHandle.hwgp.size.cy      =  h->info.JpgSize.cy;
		gHWGpHandle.hwgp.inyuvtype    =  V558_YUV_422;       //input data type
		gHWGpHandle.hwgp.outrgbtype   =  V558_RGB_565;       //output data type
		pOut->ActLen                  =  ((gHWGpHandle.hwgp.size.cx * gHWGpHandle.hwgp.size.cy) <<1);
		memcpy(pOut->pData, gHWGpHandle.hwgp.YUVBuf.pData, pOut->ActLen);
		//pOut->ActLen                =  V558_YuvToRaw(pOut->pData, gHWGpHandle.hwgp.YUVBuf.pData,
		//					gHWGpHandle.hwgp.outrgbtype, gHWGpHandle.hwgp.inyuvtype, gHWGpHandle.hwgp.size, 1);
		free(pbuf);
		pbuf   =   NULL;
	}
	return SUCCEED;
}
/*****************************************************************************
Description:
	Do negative.
Parameters:
	h:  The handle.
Returns:
	NULL
Remarks:
	NULL
******************************************************************************/
void V558_HWGPDoNegative(PV558_HWGP_HANDLE h)
{
	/////////set special effect
	h->hwgp.ippspectrl    =  5;
	V558_IppSetSpeCtrl(h->hwgp.ippspectrl);
	V558_DisplayStill(&h->hwgp.YUVBuf, &h->info, &h->proty);
}
/***************************************************************************
Description:
	Do horizontal mirror.
Parameters:
	h:   The handle.
Returns:
	NULL
Remarks:
	NULL
****************************************************************************/
void V558_HWGPDoHMirror(PV558_HWGP_HANDLE h)
{
	h->hwgp.displaymode     =  3;
	V558_LcdSetDispMode(h->hwgp.displaymode);
	V558_DisplayStill(&h->hwgp.YUVBuf, &h->info, &h->proty);
}
/************************************************************************
Description:
	Do vertical mirror.
Parameters:
	h:  The handle.
Returns:
	NULL
Remarks:
	NULL
*************************************************************************/
void V558_HWGPDoVMirror(PV558_HWGP_HANDLE h)
{
	h->hwgp.displaymode     =  5;
	V558_LcdSetDispMode(h->hwgp.displaymode);
	V558_DisplayStill(&h->hwgp.YUVBuf, &h->info, &h->proty);
}
/*************************************************************************
Description:
	Do rotation.
Parameters:
	h:       The handle.
	degree:  the degree of rotation.  0:90 ; 1:270 ; 2:180.
Returns:
	NULL
Remarks:
	NULL	
**************************************************************************/
void V558_HWGPDoRotation(PV558_HWGP_HANDLE h,UINT8 degree)
{
	switch(degree)
	{
		case V558_ROTATION_90:   h->hwgp.displaymode  =  0;break;
		case V558_ROTATION_180:  h->hwgp.displaymode  =  2;break;
		case V558_ROTATION_270:  h->hwgp.displaymode  =  1;break;
		default: break;
	}
	V558_LcdSetDispMode(h->hwgp.displaymode);
	V558_DisplayStill(&h->hwgp.YUVBuf, &h->info, &h->proty);
}
/*********************************************************************************
Description:
	Set sepia.
Parameters:
	h:  The handle.
Returns:
	NULL
Remarks:
	NULL
**********************************************************************************/
void V558_HWGPSetSepia(PV558_HWGP_HANDLE h)//, UINT8 r, UINT8 g, UINT8 b)
{
	h->hwgp.ippspectrl    =  1;
	V558_IppSetSpeCtrl(h->hwgp.ippspectrl);
	V558_IppSetYoffset(h->hwgp.yoffset);
	V558_IppSetUoffset(h->hwgp.uoffset);
	V558_IppSetVoffset(h->hwgp.voffset);
	V558_DisplayStill(&h->hwgp.YUVBuf, &h->info, &h->proty);
}

void V558_HWGPSetColorrange(PV558_HWGP_HANDLE h)
{
	h->hwgp.ippspectrl    =  3;
	V558_IppSetSpeCtrl(h->hwgp.ippspectrl);
	V558_IppSetUTopThrd(h->hwgp.utop);
	V558_IppSetUBottomThrd(h->hwgp.ubottom);
	V558_IppSetVTopThrd(h->hwgp.vtop);
	V558_IppSetVBottomThrd(h->hwgp.vbottom);
	V558_DisplayStill(&h->hwgp.YUVBuf, &h->info, &h->proty);	
}
/*
UINT8 V558_HWGpSetSepia(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty)
{
	V558_JPEG_INDEX info;	

	memcpy(&gHWGpContext,phwgp,sizeof(V558_HWGP_CONTEXT));
	/////////set special effect
	V558_IppSetSpeCtrl(gHWGpContext.ippspectrl);
	if( (gHWGpContext.ippspectrl & 0x1) && ((gHWGpContext.ippspectrl & 0xE) == 0x0) )
	{
		V558_IppSetUoffset(gHWGpContext.uoffset);
		V558_IppSetVoffset(gHWGpContext.voffset);
	}	
	if( (gHWGpContext.ippspectrl & 0x1) && ((gHWGpContext.ippspectrl & 0x2)) )
	{
		V558_IppSetUTopThrd(gHWGpContext.utop);
		V558_IppSetUBottomThrd(gHWGpContext.ubottom);
		V558_IppSetVTopThrd(gHWGpContext.vtop);
		V558_IppSetVBottomThrd(gHWGpContext.vbottom);
	}
	if(V558_JpegParse(pIn, &info))
		return HWGPFAILED;
	V558_DisplayStill(pIn, &info, proty);
	return SUCCEED;
}

UINT8 V558_HWGpSetSpecialEffect(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty)
{
	V558_JPEG_INDEX info;	

	memcpy(&gHWGpContext,phwgp,sizeof(V558_HWGP_CONTEXT));
	/////////set special effect
	V558_IppSetSpeCtrl(gHWGpContext.ippspectrl);
	if( (gHWGpContext.ippspectrl & 0x1) && ((gHWGpContext.ippspectrl & 0xE) == 0x0) )
	{
		V558_IppSetUoffset(gHWGpContext.uoffset);
		V558_IppSetVoffset(gHWGpContext.voffset);
	}	
	if( (gHWGpContext.ippspectrl & 0x1) && ((gHWGpContext.ippspectrl & 0x2)) )
	{
		V558_IppSetUTopThrd(gHWGpContext.utop);
		V558_IppSetUBottomThrd(gHWGpContext.ubottom);
		V558_IppSetVTopThrd(gHWGpContext.vtop);
		V558_IppSetVBottomThrd(gHWGpContext.vbottom);
	}
	if(V558_JpegParse(pIn, &info))
		return HWGPFAILED;
	V558_DisplayStill(pIn, &info, proty);
	return SUCCEED;
}

UINT8 V558_HWDoHMirror(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty)
{
	V558_JPEG_INDEX info;	
	memcpy(&gHWGpContext,phwgp,sizeof(V558_HWGP_CONTEXT));
	/////////set special effect
	phwgp->displaymode     =  3;
	if(V558_JpegParse(pIn, &info))
		return HWGPFAILED;
	V558_LcdSetDispMode(phwgp->displaymode);
	V558_DisplayStill(pIn, &info, proty);
	return SUCCEED;
}

UINT8 V558_HWDoVMirror(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty)
{
	V558_JPEG_INDEX info;	
	memcpy(&gHWGpContext,phwgp,sizeof(V558_HWGP_CONTEXT));
	/////////set special effect
	phwgp->displaymode     =  5;
	if(V558_JpegParse(pIn, &info))
		return HWGPFAILED;
	V558_LcdSetDispMode(phwgp->displaymode);
	V558_DisplayStill(pIn, &info, proty);
	return SUCCEED;
}

UINT8 V558_HWDoRotation(PV558_JPEGBUF pIn, PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty)
{
	V558_JPEG_INDEX info;	
	memcpy(&gHWGpContext,phwgp,sizeof(V558_HWGP_CONTEXT));
	/////////set special effect
	//phwgp->displaymode     =  0;
	if(V558_JpegParse(pIn, &info))
		return HWGPFAILED;
	V558_LcdSetDispMode(phwgp->displaymode);
	V558_DisplayStill(pIn, &info, proty);
	return SUCCEED;
}


UINT8 V558_ReserveHWGpSetSpecialEffect(PV558_JPEGBUF pOut, PV558_JPEGBUF pIn, 
				 PV558_HWGP_CONTEXT phwgp, PV558_LAYER_PROPERTY proty)
{
	V558_JPEG_INDEX info;	
	UINT8*  pbuf;
	UINT32   buflen;

	memcpy(&gHWGpContext,phwgp,sizeof(V558_HWGP_CONTEXT));
	/////////set special effect
	V558_IppSetSpeCtrl(gHWGpContext.ippspectrl);
	if( (gHWGpContext.ippspectrl & 0x1) && ((gHWGpContext.ippspectrl & 0xE) == 0x0) )
	{
		V558_IppSetUoffset(gHWGpContext.uoffset);
		V558_IppSetVoffset(gHWGpContext.voffset);
	}	
	if( (gHWGpContext.ippspectrl & 0x1) && ((gHWGpContext.ippspectrl & 0x2)) )
	{
		V558_IppSetUTopThrd(gHWGpContext.utop);
		V558_IppSetUBottomThrd(gHWGpContext.ubottom);
		V558_IppSetVTopThrd(gHWGpContext.vtop);
		V558_IppSetVBottomThrd(gHWGpContext.vbottom);
	}
	if(V558_JpegParse(pIn, &info))
		return HWGPFAILED;
	buflen   =   ((info.JpgSize.cx  * info.JpgSize.cy)<<1);
	pbuf     =    malloc(buflen);
	if(pbuf  == NULL)
		return  HWGPFAILED;
	gHWGpContext.decIsEnd           =  0;
	gHWGpContext.YUVBuf.Len         =  buflen;
	gHWGpContext.YUVBuf.ActLen      =  0;
	gHWGpContext.YUVBuf.pData	=  pbuf;
	V558_HostDecodeLcdIF(pIn, &gHWGpContext.YUVBuf, &info, HWGpDecNote, proty);
	while(! gHWGpContext.decIsEnd)
	{
	}
	if(gHWGpContext.decIsEnd)
	{
		gHWGpContext.size.cx      =  proty->Size.cx;
		gHWGpContext.size.cy      =  (proty->Size.cx*info.JpgSize.cy)/info.JpgSize.cx;   //??? get from ipp
		gHWGpContext.inyuvtype    =  V558_YUV_422;       //input data type
		gHWGpContext.outrgbtype   =  V558_RGB_565;       //output data type
		pOut->ActLen              =  V558_YuvToRaw(pOut->pData ,gHWGpContext.YUVBuf.pData ,
							gHWGpContext.outrgbtype ,gHWGpContext.inyuvtype,gHWGpContext.size,1);
		//gHWGpContext.Valid    =  1;
		//gHWGpContext.State    = V558_HWGP_INITED;
		free(pbuf);
		pbuf   =   NULL;
	}
	return SUCCEED;
}
*/