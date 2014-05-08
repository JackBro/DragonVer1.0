//#include "..\\stdafx.h"
//#include "..\\datadefine.h"
//#include "..\\UartProtocolExport.h"

#include "v558api.h"
#include "vipp.h"
#include "vregdef.h"
#include "string.h"




V558_IPP_CONTEXT	gIppContext;


/********************************************************************************

	top level function

*********************************************************************************/
static void IppResetSize(void);

/********************************************************************************
	Description:
		Initialize Lcd module's context
	Note:
		This function must be called when system is power on. Indict the Lcd 
		module's context is not valid.
*********************************************************************************/
void V558_IppInitContext(void)
{
	gIppContext.Valid = FALSE;
	gIppContext.State = V558_IPP_NOTINIT;
}

/********************************************************************************
	Description:
		Application layer send initialize data to IPP module, 
		the driver store the IPP info in module.
	Parameters:
		info:	point to the start address of IPP module's initialize data
	Note:
		This function must be called before use other function of this RDK,
		or the IPP module will be abnormal.
	Return Value List:
	   SUCCEED	:  if succesful
	   IPPFAILED:  otherwise
*********************************************************************************/
UINT16 V558_IppSetInfo(PV558_IPPINFO info)
{

	gIppContext.pIppInfo = info;

	gIppContext.special = 0;
	gIppContext.yoffset = info->yoffset;
	gIppContext.uoffset = info->uoffset;
	gIppContext.voffset = info->voffset;
	gIppContext.utop = info->utop;
	gIppContext.ubottom = info->ubottom;
	gIppContext.vtop = info->vtop;
	gIppContext.vbottom = info->vbottom;
	gIppContext.framedrop = info->framedrop;

	gIppContext.SizPoint = info->SizPoint; //Resizing window start point
	gIppContext.SizSize = info->SizSize; //Resizing window size
	gIppContext.CapWidth = info->CapWidth; //Capture size
	gIppContext.DisWidth = info->DisWidth; //Display size


	gIppContext.Valid = TRUE;
	gIppContext.State = V558_IPP_NOTINIT;
	return SUCCEED;
}
/************************************************************************************************
Description:
	Get the output image size when decode with ipp.
Parameters:
	srcwidth:	Input image width.
	srcheight:	Input image height.
	dstwidth:	The decode ipp width that assume.
	ptempwidth:	The pointer of real output image width.
	ptempheight:	The pointer of real output image height.
Returns:
	NULL
Notes:
	NULL
Remarks:
	NULL
*************************************************************************************************/		
void V558_GetDecodeIppSize(UINT16 srcwidth,UINT16 srcheight,UINT16 dstwidth, UINT16* ptempwidth,UINT16* ptempheight)
{
	UINT16 temp;
	temp   =   (srcwidth>>3);
	if(dstwidth <= temp)
		dstwidth  =  temp;
	if(dstwidth%2)
		*ptempwidth  = dstwidth +1;
	else 
		*ptempwidth  = dstwidth;
	*ptempheight  =  (srcheight * (*ptempwidth))/srcwidth;
}

/********************************************************************************
	Description:
		Open IPP module, reset it to default setting
	Note:
		This module must be initialized after initialize ISP module
	Return Value List:
	   SUCCEED	:  if succesful
	   IPPFAILED:  otherwise
*********************************************************************************/


#if 1
UINT8 V558_IppOpen(void)
{
	if(gIppContext.Valid)
	{

		V558_SifGetSnrInfoType(&gIppContext.SnrType);
		V558_SifGetSnrInfoPixRate(&gIppContext.PixRate);
		V558_SifGetSnrInfoImgSize(&gIppContext.ImgSize);


		if(gIppContext.SnrType)
		{
			V558_IppSetSizerCtrl(0x8);
		}
		else
			V558_IppSetSizerCtrl(0);

		V558_IppSetPixelRate(gIppContext.PixRate);
		V558_IppSetImageSize(gIppContext.ImgSize);
		
		V558_IppSetYoffset(gIppContext.yoffset);
		V558_IppSetUoffset(gIppContext.uoffset);
		V558_IppSetVoffset(gIppContext.voffset);
		V558_IppSetUTopThrd(gIppContext.utop);
		V558_IppSetUBottomThrd(gIppContext.ubottom);
		V558_IppSetVTopThrd(gIppContext.vtop);
		V558_IppSetVBottomThrd(gIppContext.vbottom);
		V558_IppSetFrameDrop(gIppContext.framedrop);

		V558_IppSetSpeCtrl(gIppContext.special);


		if(gIppContext.pIppInfo->enfilter)
		{
			V558_IppEnablePrefilter();
		}

		gIppContext.State = V558_IPP_INITED;
		return SUCCEED;
	}
	return IPPFAILED;
}

#else

UINT8 V558_IppOpen(void)
{
	UINT8		temp;
	V558_SIZE	size;

	if(gIppContext.Valid)
	{
		V558_SifGetSnrInfoType(&temp);
		V558_IppSetSizerCtrl(0);
		if(temp)
			V558_IppSetSizerCtrl(0x8);
		V558_SifGetSnrInfoPixRate(&temp);
		V558_IppSetPixelRate(temp);

//		temp = V558_IppGetSizerCtrl();
//		V558_IppSetSizerCtrl(temp & 0x8);
		

		if(gIppContext.pIppInfo->enfilter)
			V558_IppEnablePrefilter();

		V558_IppSetYoffset(gIppContext.yoffset);
		V558_IppSetUoffset(gIppContext.uoffset);
		V558_IppSetVoffset(gIppContext.voffset);
		V558_IppSetUTopThrd(gIppContext.utop);
		V558_IppSetUBottomThrd(gIppContext.ubottom);
		V558_IppSetVTopThrd(gIppContext.vtop);
		V558_IppSetVBottomThrd(gIppContext.vbottom);
		V558_IppSetFrameDrop(gIppContext.framedrop);
		V558_IppSetSpeCtrl(gIppContext.special);

		V558_SifGetSnrInfoImgSize(&size);
//		V558_IspGetImageSize(&size);
		V558_IppSetImageSize(size);

		gIppContext.State = V558_IPP_INITED;
		return SUCCEED;
	}
	return IPPFAILED;
}
#endif
static UINT16	MaxDivisor(UINT16 a, UINT16 b)
{
	while(a && b)
	{
		if(a > b)
			a %= b;
		else
			b %= a;
	}
	if(a)
		return a;
	return b;
}

static void ComputerParm(V558_SIZE src, UINT16 width, V558_SIZE *dst, V558_IPP_FACTOR *fct)
{
	UINT16	div, flag = 1;

	while(flag)
	{
		div = MaxDivisor(src.cx, width);
		fct->src = src.cx / div;
		fct->dst = width / div;

		while( (fct->src > 0x3f) || (fct->dst > 0x3f) )
		{
			fct->src = (fct->src + 1) >> 1;
			fct->dst = (fct->dst >> 1) + 1;
		}
		dst->cx = (src.cx * fct->dst) / fct->src;
		dst->cy = (src.cy * fct->dst) / fct->src;
		width = dst->cx;
		if(width % 2)
			width++;
		else
			flag = 0;
	}
}

/********************************************************************************
	Description:
		Set capture width
	Parameters:
		width: capture width
	Remarks:
*********************************************************************************/
void V558_IppSetCapture(UINT16 width)
{
	gIppContext.CapWidth = width;
	IppResetSize();
}

#define SELECT_DISPLAY_SIZER	0
#define SELECT_CAPTURE_SIZER	1

/********************************************************************************
	Description:
		Select prefilter type
	Parameters:
		sel: 0 display sizer, 1 capture sizer
	Remarks:
*********************************************************************************/
void	V558_IppSelectPrefilter(int sel)
{
	UINT8 x, y;

	if(sel == 1)
	{
		V558_IppGetCapFactor(&x, &y);
	}
	if(sel == 0)
	{
		V558_IppGetDispFactor(&x, &y);
	}
	V558_IppSetFilter(x, y);
}

/********************************************************************************
	Description:
		Set display width.
	Parameters:
		width: display width.
	Remarks:
*********************************************************************************/
void V558_IppSetDisplay(UINT16 width)
{
	gIppContext.DisWidth = width;
	IppResetSize();
}

/********************************************************************************
	Description:
		Set thumbnail width.
	Parameters:
		width: thumbnail width.
	Remarks:
*********************************************************************************/
void V558_IppSetThumb(UINT16 width)
{
	V558_SIZE size, image;
	V558_SIZE dst;
	V558_IPP_FACTOR fct;
	UINT8	sif;

	sif = V558_SifDisableSyncGen();
	V558_IppDisableDispSizer();
	V558_IppGetImageSize(&image);
	V558_IppGetSizerSize(&size);
	if(width > size.cx)
		width = size.cx;
	ComputerParm(size, width, &dst, &fct);
	V558_IppSetDispFactor((UINT8)(fct.src), (UINT8)(fct.dst));
	if( (dst.cx < image.cx) || (dst.cy < image.cy) )


		V558_IppEnableDispSizer();
	else
		V558_IppDisableDispSizer();
	V558_IppSetDispSize(dst);	//modify layer A propeoty

	V558_SifSetSensorOpmode(sif);
}

/********************************************************************************
	Description:
		Capture sizer setting.
	Parameters:
		pt: The point which start capture from.
		size: Source image size.
		width: Capture width
	Remarks:
*********************************************************************************/
void V558_IppSetSizerCapture(V558_POINT pt, V558_SIZE size, UINT16 width)
{
	gIppContext.SizPoint = pt;
	gIppContext.SizSize = size;
	gIppContext.CapWidth = width;
	IppResetSize();
}

/********************************************************************************
	Description:
		Display sizer setting.
	Parameters:
		pt: The point which start display from.
		size: Source image size.
		width: Display width
	Remarks:
*********************************************************************************/
void V558_IppSetSizerDisplay(V558_POINT pt, V558_SIZE size, UINT16 width)
{
	gIppContext.SizPoint = pt;
	gIppContext.SizSize = size;
	gIppContext.DisWidth = width;
	IppResetSize();
}

/********************************************************************************
	Description:
		Zoom display sizer setting.
	Parameters:
		pt: The point which start display from.
		size:  sizer  image size.
		width: Display width
	Remarks:
*********************************************************************************/
void V558_IppSetZoomDisplay(V558_POINT pt, V558_SIZE size, UINT16 width)
{
	gIppContext.SizPoint = pt;
	gIppContext.SizSize = size;
	gIppContext.DisWidth = width;
	IppResetSize();
}

/********************************************************************************
	Description:
		Display setting.
	Parameters:
		src: Source image size.
		pt: The point which start display from.
		size: Select sizer image size.
		width: Display width
	Remarks:
*********************************************************************************/
void V558_IppDisplay(V558_SIZE src, V558_POINT pt, V558_SIZE size, UINT16 width)
{
	UINT8	sif;

	sif = V558_SifDisableSyncGen();
	V558_IppSetImageSize(src);
	V558_IppSetSizerDisplay(pt, size, width);
	V558_SifSetSensorOpmode(sif);
}

/********************************************************************************
	Description:
		Capture setting.
	Parameters:
		src: Source image size.
		pt: The point which start capture from.
		size: Select sizer image size.
		width: Capture width
	Remarks:
*********************************************************************************/
void V558_IppCapture(V558_SIZE src, V558_POINT pt, V558_SIZE size, UINT16 width)
{
	UINT8	sif;

	sif = V558_SifDisableSyncGen();
	V558_IppSetImageSize(src);
	V558_IppSetSizerCapture(pt, size, width);
	V558_SifSetSensorOpmode(sif);
}

#if 0
void V558_IppSetFilter(UINT16 x, UINT16 y)
{
	UINT16 f;
	UINT16 width;
	V558_SIZE size;
	UINT8 factor[4][4] = 
	{
		{128, 64, 0, 0},
		{96, 64, 16, 0},
		{62, 52, 32, 13},
		{160, 48, 0, 0}
	};

	if(! gIppContext.pIppInfo->enfilter)
		return;

	width = V558_IppGetImageWidth();
	V558_SifGetSnrInfoImgSize(&size);
	f = (y << 8) / x;
	if(width < size.cx)
	{
		if(f > 128)
		{
			V558_IppDisablePrefilter();
			return;
		}
	}

	if(f > 108)
	{
			V558_IppSetFirCoef(factor[3]);
	}
	else if(f > 83)
	{
		V558_IppSetFirCoef(factor[0]);
	}
	else if(f > 32)
	{
		V558_IppSetFirCoef(factor[1]);
	}
	else
	{
		V558_IppSetFirCoef(factor[2]);
	}
	if(f > 128)
		V558_IppDisablePrefilter();
	else
		V558_IppEnablePrefilter();
}
#else

/********************************************************************************
	Description:
		Prefilter setting.
	Parameters:
		x: width.
		y: heigth
	Remarks:
*********************************************************************************/
void V558_IppSetFilter(UINT16 x, UINT16 y)
{
	UINT8 f;
	UINT8 i, flag = 0;

	if(! gIppContext.pIppInfo->enfilter)
		return;

	y <<= 8;
	y /= x;
	y -= 1;
	f = (UINT8)y;
	for(i = 0; i < gIppContext.pIppInfo->filtercount; i++)
	{
		if( (f >= gIppContext.pIppInfo->pfilter[i].min) &&
			(f <= gIppContext.pIppInfo->pfilter[i].max) )
		{
			V558_IppSetFirCoef(gIppContext.pIppInfo->pfilter[i].coef);
			flag = 1;
			break;
		}
	}
	if(flag)
		V558_IppEnablePrefilter();
	else
		V558_IppDisablePrefilter();
}
#endif

void IppResetSize(void)
{
	V558_SIZE image;
	V558_SIZE size;
	V558_SIZE dst, dst1;
	UINT8 sif;
	V558_POINT pt;
	UINT16 width;
	UINT16 width1;
	V558_IPP_FACTOR fct, fct1;

	image = gIppContext.ImgSize;
	pt = gIppContext.SizPoint;
	size = gIppContext.SizSize;
	width = gIppContext.DisWidth;
	width1 = gIppContext.CapWidth;

	sif = V558_SifDisableSyncGen();

	V558_CtrlSwReset(V558_SW_RESET_IPP); //reset ipp

	V558_IppDisableDispSizer();
	V558_IppDisableCapSizer();

	if(image.cx % 2)
		image.cx++;				//Image width must be even

	if(size.cx > image.cx)
		size.cx = image.cx;
	if(size.cy > image.cy)
		size.cy = image.cy;
	if(width > size.cx)
		width = size.cx;
	if(width1 > size.cx)
		width1 = size.cx;
	if(size.cx % 2)
		size.cx--;				//sizer width must be even
	if(width % 2)
		width --;
	if(width1 % 2)
		width1--;

	ComputerParm(size, width, &dst, &fct);		//size's width must be even
	ComputerParm(size, width1, &dst1, &fct1);
	V558_IppSetDispFactor((UINT8)(fct.src), (UINT8)(fct.dst));
	V558_IppSetCapFactor((UINT8)(fct1.src), (UINT8)(fct1.dst));
	V558_IppSetSizerSize(size);
	V558_IppSetSizerStartp(pt);
	V558_IppSetFilter(fct.src, fct.dst);
	if( (dst1.cx == image.cx) && (dst1.cy == image.cy) )
		V558_IppDisableCapSizer();
	else
		V558_IppEnableCapSizer();
	if( (dst.cx < image.cx) || (dst.cy < image.cy) )
		V558_IppEnableDispSizer();
	else
		V558_IppDisableDispSizer();

	V558_IppSetCapSize(dst1);
	V558_IppSetDispSize(dst);	//modify layer A propeoty
	V558_SifSetSensorOpmode(sif);
}


//==============================================================================//
//	special effect                                                         //
//==============================================================================//

/********************************************************************************
	Description:
		Sephia effect setting
	Parameters:
		byU:	U offset value
		byV:	V offset value
	Remarks:
*********************************************************************************/
void V558_IppSpeSephia(UINT8 byU, UINT8 byV)
{
	V558_IppSetUoffset(byU);
	V558_IppSetVoffset(byV);
	V558_IppSetSpeCtrl(0x01);
}

/********************************************************************************
	Description:
		Color range effect setting
	Parameters:
		uTop:	U top value
		uBottom:	U bottom value
		vTop:	V top value
		vBottom:	V bottom value
	Remarks:
*********************************************************************************/
void V558_IppSpeColorRange(UINT8 uTop, UINT8 uBottom, UINT8 vTop, UINT8 vBottom)
{
	V558_IppSetUTopThrd(uTop);
	V558_IppSetUBottomThrd(uBottom);
	V558_IppSetVTopThrd(vTop);
	V558_IppSetVBottomThrd(vBottom);
	V558_IppSetSpeCtrl(0x03);
}

/********************************************************************************
	Description:
		Negative effect setting
	Parameters:
	Remarks:
*********************************************************************************/
void V558_IppSpeNegative(void)
{
	V558_IppSetSpeCtrl(0x05);
}

/********************************************************************************
	Description:
		Relief effect setting
	Parameters:
	Remarks:
*********************************************************************************/
void V558_IppSpeRelief(void)
{
	V558_IppSetSpeCtrl(0x05);
}

/********************************************************************************
	Description:
		congfig special control in IPP module
	Parameters:
		sel:	special control setting
	Note:
		<TABLE>
		value		meaning
		-----		-------
		BIT0		special effect enable
		BIT1-3		special effect mode<p>
					000: sephia;  001: color range;  010: negative;
					011: sketch;  100: relief
		BIT4-7		reserve
		</TABLE>
	Remarks:
*********************************************************************************/
void V558_IppSetSpeCtrl(UINT8 sel)
{
	V558_SetReg(V558_REG_IPP_SPECTRL, sel);
	gIppContext.special = sel;
}

/********************************************************************************
	Description:
		Get special control setting in IPP module
	Returns:
		return special control setting
	Note:
		<TABLE>
		value		meaning
		-----		-------
		BIT0		special effect enable
		BIT1-3		special effect mode<p>
					000: sephia;  001: color range;  010: negative;
					011: sketch;  100: relief
		BIT4-7		reserve
		</TABLE>
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetSpeCtrl(void)
{
	UINT8	x;

	V558_GetReg(V558_REG_IPP_SPECTRL, &x);
	return x;
}

/********************************************************************************
	Description:
		Enable special effect
	Returns:
		return special control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSpeCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppEnableSpecial(void)
{
	UINT8	x;

	x = V558_IppGetSpeCtrl();
	V558_IppSetSpeCtrl(x | 1);
	return x;
}

/********************************************************************************
	Description:
		Disable special effect
	Returns:
		return special control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSpeCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppDisableSpecial(void)
{
	UINT8	x;

	x = V558_IppGetSpeCtrl();
	V558_IppSetSpeCtrl(x & 0xfe);
	return x;
}

/********************************************************************************
	Description:
		config sizer control in IPP module
	Parameters:
		x:		setting value
	Note:
		<TABLE>
		value			meaning
		-----			-------
		BIT0			reserved
		BIT1			capture sizer enable
		BIT2			prefilter enable
		BIT3			sensor with YUV output
		BIT4			display sizer enable
		BIT5-7			reserved
		</TABLE>
	Remarks:
*********************************************************************************/
void V558_IppSetSizerCtrl(UINT8 x)
{
	V558_SetReg(V558_REG_IPP_SIZCTRL, x);
}

/********************************************************************************
	Description:
		get sizer control setting
	Returns:
		return sizer control setting value
	Note:
		<TABLE>
		value			meaning
		-----			-------
		BIT0			reserved
		BIT1			capture sizer enable
		BIT2			prefilter enable
		BIT3			sensor with YUV output
		BIT4			display sizer enable
		BIT5-7			reserved
		</TABLE>
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetSizerCtrl(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_IPP_SIZCTRL, &x);
	return x;
}

/********************************************************************************
	Description:
		Enable display sizer
	Returns:
		return sizer control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSizerCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppEnableDispSizer(void)
{
	UINT8	x;

	x = V558_IppGetSizerCtrl();
	V558_IppSetSizerCtrl(x | 0x10);
//	V558_GetReg(V558_REG_IPP_SIZCTRL, &x);
//	V558_SetReg(V558_REG_IPP_SIZCTRL, x | 0x10);
	return x;
}

/********************************************************************************
	Description:
		Disable display sizer
	Returns:
		return sizer control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSizerCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppDisableDispSizer(void)
{
	UINT8	x;

	x = V558_IppGetSizerCtrl();
	V558_IppSetSizerCtrl(x & 0xef);
//	V558_GetReg(V558_REG_IPP_SIZCTRL, &x);
//	V558_SetReg(V558_REG_IPP_SIZCTRL, x & 0xef);
	return x;
}

/********************************************************************************
	Description:
		Enable prefilter function
	Returns:
		return sizer control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSizerCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppEnablePrefilter(void)
{
	UINT8	x;

	x = V558_IppGetSizerCtrl();
	V558_IppSetSizerCtrl(x | 0x4);
//	V558_GetReg(V558_REG_IPP_SIZCTRL, &x);
//	V558_SetReg(V558_REG_IPP_SIZCTRL, x | 0x4);
	return x;
}

/********************************************************************************
	Description:
		Disable prefilter function
	Returns:
		return sizer control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSizerCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppDisablePrefilter(void)
{
	UINT8	x;

	x = V558_IppGetSizerCtrl();
	V558_IppSetSizerCtrl(x & 0xfb);
//	V558_GetReg(V558_REG_IPP_SIZCTRL, &x);
//	V558_SetReg(V558_REG_IPP_SIZCTRL, x & 0xfb);
	return x;
}

/********************************************************************************
	Description:
		Enable capture sizer
	Returns:
		return sizer control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSizerCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppEnableCapSizer(void)
{
	UINT8	x;

	x = V558_IppGetSizerCtrl();
	V558_IppSetSizerCtrl(x | 0x2);
//	V558_GetReg(V558_REG_IPP_SIZCTRL, &x);
//	V558_SetReg(V558_REG_IPP_SIZCTRL, x | 0x2);
	return x;
}

/********************************************************************************
	Description:
		Disable capture sizer
	Returns:
		return sizer control setting before modify it
	Note:
		You can restore special control setting by void V558_IppSetSizerCtrl(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V558_IppDisableCapSizer(void)
{
	UINT8	x;

	x = V558_IppGetSizerCtrl();
	V558_IppSetSizerCtrl(x & 0xfd);
//	V558_GetReg(V558_REG_IPP_SIZCTRL, &x);
//	V558_SetReg(V558_REG_IPP_SIZCTRL, x & 0xfd);
	return x;
}

/********************************************************************************
	Description:
		config input image width in IPP module
	Parameters:
		x:		input image width to IPP module
	Remarks:
*********************************************************************************/
void V558_IppSetImageWidth(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_IMGWD, (UINT8)x);
	V558_SetReg(V558_REG_IPP_IMGWD+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get input image width setting in IPP module
	Returns:
		return input image width to IPP module
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetImageWidth(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_IMGWD, &x1);
	V558_GetReg(V558_REG_IPP_IMGWD+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config input image height in IPP module
	Parameters:
		x:		input image height to IPP module
	Remarks:
*********************************************************************************/
void V558_IppSetImageHeight(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_IMGHT, (UINT8)x);
	V558_SetReg(V558_REG_IPP_IMGHT+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get input image height setting in IPP module
	Returns:
		return input image height to IPP module
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetImageHeight(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_IMGHT, &x1);
	V558_GetReg(V558_REG_IPP_IMGHT+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config input image size in IPP module
	Parameters:
		size:		input image size to IPP module
	Remarks:
*********************************************************************************/
void V558_IppSetImageSize(V558_SIZE size)
{
	gIppContext.ImgSize = size;
	V558_IppSetImageWidth(size.cx);
	V558_IppSetImageHeight(size.cy);
//	V558_SetReg(V558_REG_IPP_IMGWD, (UINT8)size.cx);
//	V558_SetReg(V558_REG_IPP_IMGWD+1, (UINT8)(size.cx >> 8));
//	V558_SetReg(V558_REG_IPP_IMGHT, (UINT8)size.cy);
//	V558_SetReg(V558_REG_IPP_IMGHT+1, (UINT8)(size.cy >> 8));
}

/********************************************************************************
	Description:
		get input image size setting in IPP module
	Returns:
		return input image size setting to IPP module
	Remarks:
*********************************************************************************/
void V558_IppGetImageSize(V558_SIZE *size)
{
//	UINT8 x1, x2;

	size->cx = V558_IppGetImageWidth();
	size->cy = V558_IppGetImageHeight();

//	V558_GetReg(V558_REG_IPP_IMGWD, &x1);
//	V558_GetReg(V558_REG_IPP_IMGWD+1, &x2);
//	size->cx = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
//	V558_GetReg(V558_REG_IPP_IMGHT, &x1);
//	V558_GetReg(V558_REG_IPP_IMGHT+1, &x2);
//	size->cy = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}


/********************************************************************************
	Description:
		config sizer image width in IPP module
	Parameters:
		x:		input image width to IPP module
	Note:
		sizer image's size must be smaller than input image size, and relate
		to sizer image's point
	Remarks:
*********************************************************************************/
void V558_IppSetSizerWidth(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_SIZWD, (UINT8)x);
	V558_SetReg(V558_REG_IPP_SIZWD+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get sizer image width setting in IPP module
	Returns:
		return sizer image width to IPP module
	Note:
		sizer image's size must be smaller than input image size, and relate
		to sizer image's point
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetSizerWidth(void)

{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_SIZWD, &x1);
	V558_GetReg(V558_REG_IPP_SIZWD+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config sizer image height in IPP module
	Parameters:
		x:		sizer image height to IPP module
	Note:
		sizer image's size must be smaller than input image size, and relate
		to sizer image's point
	Remarks:
*********************************************************************************/
void V558_IppSetSizerHeight(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_SIZHT, (UINT8)x);
	V558_SetReg(V558_REG_IPP_SIZHT+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get sizer image height setting in IPP module
	Returns:
		return sizer image height setting to IPP module
	Note:
		sizer image's size must be smaller than input image size, and relate
		to sizer image's point
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetSizerHeight(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_SIZHT, &x1);
	V558_GetReg(V558_REG_IPP_SIZHT+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config size of sizer image in IPP module
	Parameters:
		size:	input the size of sizer image to IPP module
	Remarks:
		state: valid
*********************************************************************************/
void V558_IppSetSizerSize(V558_SIZE size)
{
	V558_IppSetSizerWidth(size.cx);
	V558_IppSetSizerHeight(size.cy);

//	V558_SetReg(V558_REG_IPP_SIZWD, (UINT8)size.cx);
//	V558_SetReg(V558_REG_IPP_SIZWD+1, (UINT8)(size.cx >> 8));
//	V558_SetReg(V558_REG_IPP_SIZHT, (UINT8)size.cy);
//	V558_SetReg(V558_REG_IPP_SIZHT+1, (UINT8)(size.cy >> 8));
}

/********************************************************************************
	Description:
		get size setting of sizer image setting in IPP module
	Returns:
		return sizer image size setting to IPP module
	Remarks:
		state: valid
*********************************************************************************/
void V558_IppGetSizerSize(V558_SIZE *size)
{
//	UINT8 x1, x2;

	size->cx = V558_IppGetSizerWidth();
	size->cy = V558_IppGetSizerHeight();
//	V558_GetReg(V558_REG_IPP_SIZWD, &x1);
//	V558_GetReg(V558_REG_IPP_SIZWD+1, &x2);
//	size->cx = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
//	V558_GetReg(V558_REG_IPP_SIZHT, &x1);
//	V558_GetReg(V558_REG_IPP_SIZHT+1, &x2);
//	size->cy = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}


/********************************************************************************
	Description:
		config the X coordinate of sizer's start pixel in IPP module
	Parameters:
		x:		X coordinate of sizer's start pixel
	Remarks:
		state: valid
*********************************************************************************/
void V558_IppSetSizerStartx(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_SIZSTX, (UINT8)x);
	V558_SetReg(V558_REG_IPP_SIZSTX+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get the X coordinate setting of sizer's start pixel
	Returns:
		return the X coordinate setting of sizer's start pixel
	Remarks:
		state: valid
*********************************************************************************/
UINT16 V558_IppGetSizerStartx(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_SIZSTX, &x1);
	V558_GetReg(V558_REG_IPP_SIZSTX+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config the Y coordinate setting of sizer's start pixel
	Parameters:
		y:		sizer image height to IPP module
	Remarks:
*********************************************************************************/
void V558_IppSetSizerStarty(UINT16 y)
{
	V558_SetReg(V558_REG_IPP_SIZSTY, (UINT8)y);
	V558_SetReg(V558_REG_IPP_SIZSTY+1, (UINT8)(y >> 8));
}

/********************************************************************************
	Description:
		get the Y coordinate setting of sizer's start pixel
	Returns:
		return the Y coordinate setting of sizer's start pixel
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetSizerStarty(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_SIZSTY, &x1);
	V558_GetReg(V558_REG_IPP_SIZSTY+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config start point of sizer image in IPP module
	Parameters:
		p:	the start point of sizer image
	Remarks:
*********************************************************************************/
void V558_IppSetSizerStartp(V558_POINT p)
{
	V558_IppSetSizerStartx(p.x);
	V558_IppSetSizerStarty(p.y);
//	V558_SetReg(V558_REG_IPP_SIZSTX, (UINT8)p.x);
//	V558_SetReg(V558_REG_IPP_SIZSTX+1, (UINT8)(p.x >> 8));
//	V558_SetReg(V558_REG_IPP_SIZSTY, (UINT8)p.y);
//	V558_SetReg(V558_REG_IPP_SIZSTY+1, (UINT8)(p.y >> 8));
}

/********************************************************************************
	Description:
		get start point setting of sizer image in IPP module
	Returns:
		return start point of sizer image
	Remarks:
*********************************************************************************/
void V558_IppGetSizerStartp(V558_POINT *p)
{
//	UINT8 x1, x2;

	p->x = V558_IppGetSizerStartx();
	p->y = V558_IppGetSizerStarty();
//	V558_GetReg(V558_REG_IPP_SIZSTX, &x1);
//	V558_GetReg(V558_REG_IPP_SIZSTX+1, &x2);
//	p->x = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
//	V558_GetReg(V558_REG_IPP_SIZSTY, &x1);
//	V558_GetReg(V558_REG_IPP_SIZSTY+1, &x2);
//	p->y = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config the coefficient of prefilter
	Parameters:
		x:		the coefficient of prefilter
	Remarks:
*********************************************************************************/
void V558_IppSetFirCoef(UINT8 *x)
{
	V558_SetReg(V558_REG_IPP_FIRCF, *(x+1));
	V558_SetReg(V558_REG_IPP_FIRCF+1, *(x+0));
	V558_SetReg(V558_REG_IPP_FIRCF+2, *(x+3));
	V558_SetReg(V558_REG_IPP_FIRCF+3, *(x+2));
}

/********************************************************************************
	Description:
		get the coefficient of prefilter setting
	Returns:
		return the coefficient of prefilter setting
	Remarks:
*********************************************************************************/
void V558_IppGetFirCoef(UINT8 *x)
{
	V558_GetReg(V558_REG_IPP_FIRCF, x);
	V558_GetReg(V558_REG_IPP_FIRCF+1, x+1);
	V558_GetReg(V558_REG_IPP_FIRCF+2, x+2);
	V558_GetReg(V558_REG_IPP_FIRCF+3, x+3);
}

/********************************************************************************
	Description:
		config the pixel rate
	Parameters:
		x:		pixel rate setting value
	Remarks:
*********************************************************************************/
void V558_IppSetPixelRate(UINT8 x)
{
	V558_SetReg(V558_REG_IPP_PIXRATE, x);
}

/********************************************************************************
	Description:
		get the pixel rate setting
	Returns:
		return the pixel rate setting
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetPixelRate(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_IPP_PIXRATE, &x);
	return x;
}

/********************************************************************************
	Description:
		config the capture sizer factor
	Parameters:
		x:		numerator
		y:		demominator
	Note:
		src : sizer = x : y
	Remarks:
*********************************************************************************/
void V558_IppSetCapFactor(UINT8 x, UINT8 y)
{
	V558_SetReg(V558_REG_IPP_CPDWFT, y);
	V558_SetReg(V558_REG_IPP_CPUPFT, x);
}

/********************************************************************************
	Description:
		get the capture sizer factor setting
	Parameters:
		x:		return numerator of capture sizer factor
		y:		return demominator of capture sizer factor
	Note:
		src : sizer = x : y
	Remarks:
*********************************************************************************/
void V558_IppGetCapFactor(UINT8 *x, UINT8 *y)
{
	V558_GetReg(V558_REG_IPP_CPDWFT, y);
	V558_GetReg(V558_REG_IPP_CPUPFT, x);
}

/********************************************************************************
	Description:
		config the display sizer factor
	Parameters:
		src:		numerator
		dst:		demominator
	Note:
		src : sizer = src : dst
	Remarks:
*********************************************************************************/
void V558_IppSetDispFactor(UINT8 src, UINT8 dst)
{
	V558_SetReg(V558_REG_IPP_DPDWFT, dst);
	V558_SetReg(V558_REG_IPP_DPUPFT, src);
}


/********************************************************************************
	Description:
		get the display sizer factor setting
	Parameters:
		src:		return numerator of display sizer factor
		dst:		return demominator of display sizer factor
	Note:
		src : sizer = src : dst
	Remarks:
*********************************************************************************/
void V558_IppGetDispFactor(UINT8 *src, UINT8 *dst)
{
	V558_GetReg(V558_REG_IPP_DPDWFT, dst);
	V558_GetReg(V558_REG_IPP_DPUPFT, src);
}


/********************************************************************************
	Description:
		config capture image width in IPP module
	Parameters:
		x:		capture image's width
	Note:

		the size must equal to capture factor's result
	Remarks:
*********************************************************************************/
void V558_IppSetCapWidth(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_CAPWD, (UINT8)x);
	V558_SetReg(V558_REG_IPP_CAPWD+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get capture image width setting in IPP module
	Returns:
		return capture image width to IPP module
	Note:
		the size must equal to capture factor's result
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetCapWidth(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_CAPWD, &x1);
	V558_GetReg(V558_REG_IPP_CAPWD+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config capture image height in IPP module
	Parameters:
		x:		capture image height to IPP module
	Note:
		the size must equal to capture factor's result
	Remarks:
*********************************************************************************/
void V558_IppSetCapHeight(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_CAPHT, (UINT8)x);
	V558_SetReg(V558_REG_IPP_CAPHT+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get capture image height setting in IPP module
	Returns:
		return capture image height setting to IPP module
	Note:
		the size must equal to capture factor's result
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetCapHeight(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_CAPHT, &x1);
	V558_GetReg(V558_REG_IPP_CAPHT+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config the size of capture image in IPP module
	Parameters:
		size:	input the size of capture image to IPP module
	Remarks:
*********************************************************************************/
void V558_IppSetCapSize(V558_SIZE size)
{
	V558_IppSetCapWidth(size.cx);
	V558_IppSetCapHeight(size.cy);
//	V558_SetReg(V558_REG_IPP_CAPWD, (UINT8)size.cx);
//	V558_SetReg(V558_REG_IPP_CAPWD+1, (UINT8)(size.cx >> 8));
//	V558_SetReg(V558_REG_IPP_CAPHT, (UINT8)size.cy);
//	V558_SetReg(V558_REG_IPP_CAPHT+1, (UINT8)(size.cy >> 8));
	V558_AdjustCaptureSize(size);
}

/********************************************************************************
	Description:
		get the size setting of capture image in IPP module
	Returns:
		return the size setting of capture image to IPP module
	Remarks:
*********************************************************************************/
void V558_IppGetCapSize(V558_SIZE *size)
{
//	UINT8 x1, x2;

	size->cx = V558_IppGetCapWidth();
	size->cy = V558_IppGetCapHeight();
//	V558_GetReg(V558_REG_IPP_CAPWD, &x1);
//	V558_GetReg(V558_REG_IPP_CAPWD+1, &x2);
//	size->cx = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
//	V558_GetReg(V558_REG_IPP_CAPHT, &x1);
//	V558_GetReg(V558_REG_IPP_CAPHT+1, &x2);
//	size->cy = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config display image width in IPP module
	Parameters:
		x:		display image's width
	Note:
		the size must equal to display factor's result
	Remarks:
*********************************************************************************/
void V558_IppSetDispWidth(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_DISWD, (UINT8)x);
	V558_SetReg(V558_REG_IPP_DISWD+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get display image width setting in IPP module
	Returns:
		return display image width to IPP module
	Note:
		the size must equal to display factor's result
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetDispWidth(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_DISWD, &x1);
	V558_GetReg(V558_REG_IPP_DISWD+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config display image height in IPP module
	Parameters:
		x:		display image height to IPP module
	Note:
		the size must equal to display factor's result
	Remarks:
*********************************************************************************/
void V558_IppSetDispHeight(UINT16 x)
{
	V558_SetReg(V558_REG_IPP_DISHT, (UINT8)x);
	V558_SetReg(V558_REG_IPP_DISHT+1, (UINT8)(x >> 8));
}

/********************************************************************************
	Description:
		get display image height setting in IPP module
	Returns:
		return display image height setting to IPP module
	Note:
		the size must equal to display factor's result
	Remarks:
*********************************************************************************/
UINT16 V558_IppGetDispHeight(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_IPP_DISHT, &x1);
	V558_GetReg(V558_REG_IPP_DISHT+1, &x2);
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config the size of display image in IPP module
	Parameters:
		size:	the size of display image setting value to IPP module
	Remarks:
*********************************************************************************/
void V558_IppSetDispSize(V558_SIZE size)
{
	V558_IppSetDispWidth(size.cx);
	V558_IppSetDispHeight(size.cy);
//	V558_SetReg(V558_REG_IPP_DISWD, (UINT8)size.cx);
//	V558_SetReg(V558_REG_IPP_DISWD+1, (UINT8)(size.cx >> 8));
//	V558_SetReg(V558_REG_IPP_DISHT, (UINT8)size.cy);
//	V558_SetReg(V558_REG_IPP_DISHT+1, (UINT8)(size.cy >> 8));
//	V558_LcdSetALSize(size);
}

/********************************************************************************
	Description:
		get the size setting of display image in IPP module
	Returns:
		return the size setting of display image to IPP module
	Remarks:
*********************************************************************************/
void V558_IppGetDispSize(V558_SIZE *size)
{
//	UINT8 x1, x2;

	size->cx = V558_IppGetDispWidth();
	size->cy = V558_IppGetDispHeight();
//	V558_GetReg(V558_REG_IPP_DISWD, &x1);
//	V558_GetReg(V558_REG_IPP_DISWD+1, &x2);
//	size->cx = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
//	V558_GetReg(V558_REG_IPP_DISHT, &x1);
//	V558_GetReg(V558_REG_IPP_DISHT+1, &x2);
//	size->cy = ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

/********************************************************************************
	Description:
		config the offset of Y
	Parameters:
		x:	the offset of Y, use it when special effet
	Remarks:
*********************************************************************************/
void V558_IppSetYoffset(UINT8	x)
{
	V558_SetReg(V558_REG_YOFFSET, x);
	gIppContext.yoffset = x;
}

/********************************************************************************
	Description:
		get the Y offset setting
	Returns:
		return the Y offset setting
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetYoffset(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_YOFFSET, &x);
	return x;
}

/********************************************************************************
	Description:
		config the offset of U
	Parameters:
		x:	the offset of U, use it when special effet
	Remarks:
*********************************************************************************/
void V558_IppSetUoffset(UINT8	x)
{
	V558_SetReg(V558_REG_UOFFSET, x);
	gIppContext.uoffset = x;
}

/********************************************************************************
	Description:
		get the U offset setting
	Returns:
		return the U offset setting
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetUoffset(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_UOFFSET, &x);
	return x;
}

/********************************************************************************
	Description:
		config the offset of V
	Parameters:
		x:	the offset of V, use it when special effet
	Remarks:
*********************************************************************************/
void V558_IppSetVoffset(UINT8	x)
{
	V558_SetReg(V558_REG_VOFFSET, x);
	gIppContext.voffset = x;
}

/********************************************************************************
	Description:
		get the V offset setting
	Returns:
		return the V offset setting
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetVoffset(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_VOFFSET, &x);
	return x;
}

/********************************************************************************
	Description:
		config the top threshold of U
	Parameters:
		x:	the top threshold of U, use it when special effet
	Remarks:
*********************************************************************************/
void V558_IppSetUTopThrd(UINT8	x)
{
	V558_SetReg(V558_REG_UUPTHRS, x);
	gIppContext.utop = x;
}

/********************************************************************************
	Description:
		get the top threshold of U
	Returns:
		return the top threshold of U
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetUTopThrd(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_UUPTHRS, &x);
	return x;
}

/********************************************************************************
	Description:
		config the bottom threshold of U
	Parameters:
		x:	the bottom threshold of U, use it when special effet
	Remarks:
*********************************************************************************/
void V558_IppSetUBottomThrd(UINT8	x)
{
	V558_SetReg(V558_REG_UDWTHRS, x);
	gIppContext.ubottom = x;
}

/********************************************************************************
	Description:
		get the bottom threshold of U
	Returns:
		return the bottom threshold of U
	Remarks:
		state: valid
*********************************************************************************/
UINT8 V558_IppGetUBottomThrd(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_UDWTHRS, &x);
	return x;
}


/********************************************************************************
	Description:
		config the top threshold of V
	Parameters:
		x:	the top threshold of V, use it when special effet
	Remarks:
*********************************************************************************/
void V558_IppSetVTopThrd(UINT8	x)
{
	V558_SetReg(V558_REG_VUPTHRS, x);
	gIppContext.vtop = x;
}

/********************************************************************************
	Description:
		get the top threshold of V
	Returns:
		return the top threshold of V
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetVTopThrd(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_VUPTHRS, &x);
	return x;
}

/********************************************************************************
	Description:
		config the bottom threshold of V
	Parameters:
		x:	the bottom threshold of V, use it when special effet
	Remarks:
*********************************************************************************/
void V558_IppSetVBottomThrd(UINT8	x)
{
	V558_SetReg(V558_REG_VDWTHRS, x);
	gIppContext.vbottom = x;
}

/********************************************************************************
	Description:
		get the bottom threshold of V
	Returns:
		return the bottom threshold of V
	Remarks:
*********************************************************************************/
UINT8 V558_IppGetVBottomThrd(void)
{
	UINT8 x;

	V558_GetReg(V558_REG_VDWTHRS, &x);
	return x;
}

/********************************************************************************
	Description:
		config frame drop value
	Parameters:
		x:	frame drop setting value
	Remarks:
*********************************************************************************/
void V558_IppSetFrameDrop(UINT16	x)
{
	V558_SetReg(V558_REG_FRMDP, (UINT8)x);
	V558_SetReg(V558_REG_FRMDP+1, (UINT8)(x >> 8));
	gIppContext.framedrop = x;
}

/********************************************************************************
	Description:
		get the frame drop setting
	Returns:
		return the frame drop setting
	Remarks:
*********************************************************************************/
UINT16 V558_IpGSetFrameDrop(void)
{
	UINT8 x1, x2;

	V558_GetReg(V558_REG_FRMDP, &x1);
	V558_GetReg(V558_REG_FRMDP+1, &x2);
	return ((UINT16)x2 << 8) | (UINT16)x1;
}
