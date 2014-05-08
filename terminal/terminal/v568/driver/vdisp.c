#include "driver.h"
#ifndef	V578CMD_DISABLE

#ifdef WIN32
TDispContext gDispContext;
#else
static TDispContext gDispContext;
#endif

const int gPixelByte[] = { 1, 2, 4, 8, 12, 16, 16, 18, 24, 32 };
const UINT8 gLcdcDelay[] = {	0x0e, 0xff,			//DEDLY  value   : 15 ~ 256
								0x01, 0x3e,			//DEINTV value   :  2 ~ 63
								0x0e, 0xff };		//LINEINTV value : 15 ~ 256

void	V5_DispSetLcdIFParm(PTLcdifParm parm);
void	V5_DispSetLcdcParm(PTLcdcParm parm);
void	V5_DispInitAlayer(void);
void	V5_DispInitBlayer(void);
void	V5_DispOpenModule(void);


/*---------------------------------------------------------------------
Function Name	:	V5_DispInitContext
Input			:	None.
Output			:	None.
Description		:	Initialize the context of VDISP module.
---------------------------------------------------------------------*/
void V5_DispInitContext(void)
{
	memset(&gDispContext, 0, sizeof(gDispContext));
	gDispContext.state = DISP_NOTINITED;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetPanelInfo
Input			:	The point to panel info struct.
Output			:	None.
Description		:	Set the panel info to VDISP context.
---------------------------------------------------------------------*/
void V5_DispSetPanelInfo(PTPanelInfo info)
{
	gDispContext.pinputinfo = info;
	gDispContext.state = DISP_INITED;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetContext
Input			:	None.
Output			:	The point to VDISP context struct.
Description		:	Get the current VDISP context.
---------------------------------------------------------------------*/
TDispContext * V5_DispGetContext(void)
{
	return &gDispContext;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispOpen
Input			:	None.
Output			:	None.
Description		:	Open VDISP module.
---------------------------------------------------------------------*/
void V5_DispOpen(void)
{
	V5B_LcdcDisableAlayer();
	V5B_LcdifResetPanel();
	V5_DispSwitchPanel(PANEL_MASTER);
	V5_DispInitBlayer();
	V5_DispInitAlayer();
	V5B_LcdcSetDe(0x3f);

	V5_CtrlIntSetCallback(INT_LCDC, _ISR_DispLcdcIntHandle);
	V5B_CpmSetIntEnSec(INT_LCDC, (0x01 << INT_LCDC_FIFO_WARNING));
	V5_CtrlIntEnable(INT_LCDC);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispClose
Input			:	None.
Output			:	None.
Description		:	.
---------------------------------------------------------------------*/
void V5_DispClose(void)
{
}

void V5_DispInitClk(void)
{
	
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetPanelSize
Input			:	The point to a TSize struct.
Output			:	None.
Description		:	Get current panel size.
---------------------------------------------------------------------*/
void V5_DispGetPanelSize(TSize *size)
{
	if(gDispContext.pcur)
		*size = gDispContext.pcur->lcdc->size;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSwitchPanel
Input			:	Main or slave panel.
Output			:	None.
Description		:	Switch panel between main and slave.
---------------------------------------------------------------------*/
void V5_DispSwitchPanel(UINT8 sel)
{
	if(gDispContext.pinputinfo == NULL)
		return;

	if((gDispContext.pinputinfo->master == NULL) && (gDispContext.pinputinfo->slave == NULL) )
		return;

	if(gDispContext.pinputinfo->master)
	{
		gDispContext.pcur = gDispContext.pinputinfo->master;
		gDispContext.panelsel = PANEL_MASTER;
	}
	else
	{
		gDispContext.pcur = gDispContext.pinputinfo->slave;
		gDispContext.panelsel = PANEL_SLAVE;
	}
	if(sel)
	{
		if(gDispContext.pinputinfo->slave)
		{
			gDispContext.pcur = gDispContext.pinputinfo->slave;
			gDispContext.panelsel = PANEL_SLAVE;
		}
	}
	V5_DispOpenModule();
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetPanelSel
Input			:	None.
Output			:	Main or slave panel.
Description		:	Get current panel select.
---------------------------------------------------------------------*/
UINT8 V5_DispGetPanelSel(void)
{
	return gDispContext.panelsel;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetPanelType
Input			:	None.
Output			:	Panel access type.
Description		:	Get current panel access type.
---------------------------------------------------------------------*/
UINT8 V5_DispGetPanelType(void)
{
	if(gDispContext.pcur == NULL)
		return 0;

	return gDispContext.pcur->type;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispOpenModule
Input			:	None.
Output			:	None.
Description		:	Initialize lcdc and lcdif module.
---------------------------------------------------------------------*/
void V5_DispOpenModule(void)
{
	if(gDispContext.pcur == NULL)
		return;
	V5_DispSetLcdIFParm(gDispContext.pcur->lcdif);
	V5_DispSetLcdcParm(gDispContext.pcur->lcdc);

	gDispContext.brightness = 32;
	gDispContext.contrast = 32;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetLcdIFParm
Input			:	The point to lcdif struct.
Output			:	None.
Description		:	Initialize lcdif module.
---------------------------------------------------------------------*/
void V5_DispSetLcdIFParm(PTLcdifParm parm)
{
	if(gDispContext.pcur == NULL)
		return;

	V5B_LcdifSetCsMode(parm->csmode);
	V5B_LcdifSetCsFlag(parm->csflag);
	V5B_LcdifSetHeadRsFlag(parm->rsflag);
	V5B_LcdifSetHeadNum(parm->headnum);
	V5B_LcdifSetHeadStxPos(parm->stxp);
	V5B_LcdifSetHeadStyPos(parm->styp);
	V5B_LcdifSetHeadEdxPos(parm->endxp);
	V5B_LcdifSetHeadEdyPos(parm->endyp);
	if(gDispContext.pcur->lcdif->config & V5B_LCDIF_CS1_SEL)
		V5B_LcdifEnableCs1();	//cs1 enable
	else
		V5B_LcdifEnableCs0();	//cs0 enable

	V5B_LcdifSetDmapfmt(parm->fmt);
	V5_DispSetWrTiming(parm->wcy);
	V5B_LcdifSetRdTiming(parm->rcy);
	V5B_LcdifSetHead(16, parm->head);

	V5B_LcdifEnableLineHead();
	V5B_LcdifUpdateHead();

	if(parm->proccfg & 0x01)
	{
		V5B_LcdifSetCommStartX(parm->cmd_sx);
		V5B_LcdifSetCommEndX(parm->cmd_ex);
		V5B_LcdifSetCommStartY(parm->cmd_sy);
		V5B_LcdifSetCommEndY(parm->cmd_ey);
		V5B_LcdifCommCodeEnable();
	}
	if(parm->proccfg & 0x02)
	{
		V5B_LcdifSetRptStartX(parm->cmd_sx);
		V5B_LcdifSetRptEndX(parm->cmd_ex);
		V5B_LcdifSetRptStartX(parm->cmd_sy);
		V5B_LcdifSetRptEndY(parm->cmd_ey);
		V5B_LcdifHeadRptEnable();
	}

	V5B_LcdifSetBiasSpt(parm->biaspt);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetLcdcParm
Input			:	The point to lcdc struct.
Output			:	None.
Description		:	Initialize lcdc module.
---------------------------------------------------------------------*/
void V5_DispSetLcdcParm(PTLcdcParm parm)
{
	TPoint pt = {0, 0};

	V5B_LcdcSetDc(0);
	V5B_LcdcSetBgStartPt(pt);
	V5B_LcdcSetBgSize(parm->size);
	V5B_LcdcSetGammaR(parm->gamma);
	V5B_LcdcSetGammaG(parm->gamma);
	V5B_LcdcSetGammaB(parm->gamma);

	V5B_LcdcSetDitherSel(parm->prop & (PANEL_DITHERMASK - 1));
	if(parm->prop & PANEL_DITHEREN)
		V5B_LcdcEnableDither();
	if(parm->prop & PANEL_GAMMAEN)
		V5B_LcdcEnableGamma();
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispInitAlayer
Input			:	None.
Output			:	None.
Description		:	Initialize layer A property.
---------------------------------------------------------------------*/
void V5_DispInitAlayer(void)
{
	TALayerProty protya;

	if(gDispContext.pcur == NULL)
		return;
	protya.memsize.cx = gDispContext.pcur->lcdc->size.cx;
	protya.memsize.cy = gDispContext.pcur->lcdc->size.cy;
	protya.pos2panel.x = protya.pos2panel.y = 0;
	protya.win.ltp.x = protya.win.ltp.y = 0;
	protya.win.size = protya.memsize;
	V5_DispSetAmemSize(&protya);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispInitBlayer
Input			:	None.
Output			:	None.
Description		:	Initialize layer B property.
---------------------------------------------------------------------*/
void V5_DispInitBlayer(void)
{
	TBLayerProty protyb;

	if(gDispContext.pcur == NULL)
		return;
	protyb.memsize.cx = gDispContext.pcur->lcdc->size.cx;
	protyb.memsize.cy = gDispContext.pcur->lcdc->size.cy / 2;
	protyb.pos2panel.x = protyb.pos2panel.y = 0;
	protyb.win.ltp.x = protyb.win.ltp.y = 0;
	protyb.win.size.cx = gDispContext.pcur->lcdc->size.cx;
	protyb.win.size.cy = gDispContext.pcur->lcdc->size.cy / 2;
	protyb.refwin.ltp.x = protyb.refwin.ltp.y = 0;
	protyb.refwin.size.cx = gDispContext.pcur->lcdc->size.cx;
	protyb.refwin.size.cy = gDispContext.pcur->lcdc->size.cy / 2;
	V5_DispSetB0format(BLAYER_RGB565);
	V5_DispSetB0Proty(&protyb);

	protyb.memsize.cx = gDispContext.pcur->lcdc->size.cx;
	protyb.memsize.cy = gDispContext.pcur->lcdc->size.cy / 2;
	protyb.pos2panel.x = 0;
	protyb.pos2panel.y = gDispContext.pcur->lcdc->size.cy / 2 ;
	protyb.win.ltp.x = protyb.win.ltp.y = 0;
	protyb.win.size.cx = gDispContext.pcur->lcdc->size.cx;
	protyb.win.size.cy = gDispContext.pcur->lcdc->size.cy / 2;
	protyb.refwin.ltp.x = protyb.refwin.ltp.y = 0;
	protyb.refwin.size.cx = gDispContext.pcur->lcdc->size.cx;
	protyb.refwin.size.cy = gDispContext.pcur->lcdc->size.cy / 2;
	V5_DispSetB1format(BLAYER_RGB565);
	V5_DispSetB1Proty(&protyb);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB0format
Input			:	Color format.
Output			:	None.
Description		:	Set color format of B0.
---------------------------------------------------------------------*/
void V5_DispSetB0format(UINT8 fmt)
{
	UINT8 flag = 0;

	gDispContext.fmt[0] = fmt & 0xf;
	flag = V5B_LcdcGetDe();
	if(flag & B0LAYER_EN)
		V5B_LcdcSetGbufFormat(fmt);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB0format
Input			:	None.
Output			:	Color format.
Description		:	Get color format of B0.
---------------------------------------------------------------------*/
UINT8 V5_DispGetB0format(void)
{
	return gDispContext.fmt[0];
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB1format
Input			:	Color format.
Output			:	None.
Description		:	Set color format of B1.
---------------------------------------------------------------------*/
void V5_DispSetB1format(UINT8 fmt)
{
	UINT8 flag = 0;

	gDispContext.fmt[1] = fmt & 0xf;
	flag = V5B_LcdcGetDe();
	if(flag & B1LAYER_EN)
		V5B_LcdcSetGbufFormat(fmt);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB1format
Input			:	None.
Output			:	Color format.
Description		:	Get color format of B1.
---------------------------------------------------------------------*/
UINT8 V5_DispGetB1format(void)
{
	return gDispContext.fmt[1];
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB0Proty
Input			:	The point to layer B property struct.
Output			:	None.
Description		:	Set the property of B0.
---------------------------------------------------------------------*/
void V5_DispSetB0Proty(PTBLayerProty proty)
{
	V5B_LcdcSetBmw0(proty->memsize.cx);
	V5B_LcdcSetB0SptPanel(proty->pos2panel);
	V5B_LcdcSetB0WinSize(proty->win.size);
	V5B_LcdcSetB0SptMem(proty->win.ltp);
	V5B_LcdcSetB0rPoint(proty->refwin.ltp);
	V5B_LcdcSetB0rSize(proty->refwin.size);
	gDispContext.bmem[0] = proty->memsize;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB0Proty
Input			:	The point to layer B property struct.
Output			:	None.
Description		:	Get the current property of B0.
---------------------------------------------------------------------*/
void V5_DispGetB0Proty(PTBLayerProty proty)
{
	V5B_LcdcGetB0SptPanel(&(proty->pos2panel));
	V5B_LcdcGetB0WinSize(&(proty->win.size));
	V5B_LcdcGetB0SptMem(&(proty->win.ltp));
	proty->memsize = gDispContext.bmem[0];
	V5B_LcdcGetB0rPoint(&(proty->refwin.ltp));
	V5B_LcdcGetB0rSize(&(proty->refwin.size));
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB1Proty
Input			:	The point to layer B1 property struct.
Output			:	None.
Description		:	Set the property of B1.
---------------------------------------------------------------------*/
void V5_DispSetB1Proty(PTBLayerProty proty)
{
	V5B_LcdcSetBmw1(proty->memsize.cx);
	V5B_LcdcSetB1SptPanel(proty->pos2panel);
	V5B_LcdcSetB1WinSize(proty->win.size);
	V5B_LcdcSetB1SptMem(proty->win.ltp);
	V5B_LcdcSetB1rPoint(proty->refwin.ltp);
	V5B_LcdcSetB1rSize(proty->refwin.size);
	gDispContext.bmem[1] = proty->memsize;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB1Proty
Input			:	The point to layer B property struct.
Output			:	None.
Description		:	Get the current property of B1.
---------------------------------------------------------------------*/
void V5_DispGetB1Proty(PTBLayerProty proty)
{
	V5B_LcdcGetB1SptPanel(&(proty->pos2panel));
	V5B_LcdcGetB1WinSize(&(proty->win.size));
	V5B_LcdcGetB1SptMem(&(proty->win.ltp));
	proty->memsize = gDispContext.bmem[1];
	V5B_LcdcGetB1rPoint(&(proty->refwin.ltp));
	V5B_LcdcGetB1rSize(&(proty->refwin.size));
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB0MemSize
Input			:	The point to TSize struct.
Output			:	None.
Description		:	Get current B0 memory size.
---------------------------------------------------------------------*/
void V5_DispGetB0MemSize(TSize *size)
{
	*size = gDispContext.bmem[0];
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB1MemSize
Input			:	The point to TSize struct.
Output			:	None.
Description		:	Get current B1 memory size.
---------------------------------------------------------------------*/
void V5_DispGetB1MemSize(TSize *size)
{
	*size = gDispContext.bmem[1];
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetAmemSize
Input			:	The point to layer A property struct.
Output			:	None.
Description		:	Set layer A property.
---------------------------------------------------------------------*/
void V5_DispSetAmemSize(PTALayerProty proty)
{
	V5B_LcdcSetASptPanel(proty->pos2panel);
	V5B_LcdcSetAWinSize(proty->win.size);
	V5B_LcdcSetASptMem(proty->win.ltp);
	V5B_LcdcSetAMSize(proty->memsize);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetAmemSize
Input			:	The point to layer A property struct.
Output			:	None.
Description		:	Get the current layer A property.
---------------------------------------------------------------------*/
void V5_DispGetAmemSize(PTALayerProty proty)
{
	V5B_LcdcGetASptPanel(&(proty->pos2panel));
	V5B_LcdcGetAWinSize(&(proty->win.size));
	V5B_LcdcGetASptMem(&(proty->win.ltp));
	V5B_LcdcGetAMSize(&(proty->memsize));
}

/*---------------------------------------------------------------------
Function Name	:	GetBoffset
Input			:	pt		-	The pixel coordinates;
					width	-	The width of a row by pixel;
					fmt		-	The color format.
Output			:	Offset value in byte.
Description		:	Caculate the start address offset of pixel data.
---------------------------------------------------------------------*/
static UINT32 GetBoffset(TPoint pt, UINT32 width, UINT8 fmt)
{
	UINT32 len;

	len = pt.y * width;
	len += pt.x;
	switch(fmt & 0xf)
	{
	case BLAYER_RGB1:
		len += 7;
		len >>= 3;
		break;
	case BLAYER_RGB2:
		len += 3;
		len >>= 2;
		break;
	case BLAYER_RGB4:
		len += 1;
		len >>= 1;
		break;
	case BLAYER_RGB444:
		len *= 3;
		len += 1;
		len >>= 1;
		break;
	case BLAYER_RGB555:
	case BLAYER_RGB565:
		len <<= 1;
		break;
	case BLAYER_RGB666:
		len *= 18;
		len += 7;
		len >>= 3;
		break;
	case BLAYER_RGB24:
		len = (len << 1) + len;
		break;
	case BLAYER_RGB32:
		len <<= 2;
		break;
	case BLAYER_RGB8:
	default:
		break;
	}
	return len;
}

/*---------------------------------------------------------------------
Function Name	:	GetBspan
Input			:	width	-	The width of a row by pixel;
					fmt		-	The color format.
Output			:	Span value in byte.
Description		:	Caculate the span value of graphic buffer.
---------------------------------------------------------------------*/
static UINT32 GetBspan(UINT32 width, UINT8 fmt)
{
	switch(fmt & 0xf)
	{
	case BLAYER_RGB1:
		width += 7;
		width >>= 3;
		break;
	case BLAYER_RGB2:
		width += 3;
		width >>= 2;
		break;
	case BLAYER_RGB4:
		width ++;
		width >>= 1;
		break;
	case BLAYER_RGB444:
		width *= 3;
		width ++;
		width >>= 1;
		break;
	case BLAYER_RGB555:
	case BLAYER_RGB565:
		width <<= 1;
		break;
	case BLAYER_RGB666:
		width *= 18;
		width >>= 3;
		break;
	case BLAYER_RGB24:
		width = (width << 1) + width;
		break;
	case BLAYER_RGB32:
		width <<= 2;
		break;
	case BLAYER_RGB8:
	default:
		break;
	}
	return width;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispB0Image
Input			:	pt		-	The start point to display;
					size	-	The size to display;
					pImage	-	Image data.
Output			:	None.
Description		:	Write image data to B0.
---------------------------------------------------------------------*/
void V5_DispB0Mono(TPoint pt, TSize size, UINT32 val)
{
	UINT32 offset, span, width, len;

	offset = GetBoffset(pt, gDispContext.bmem[0].cx, gDispContext.fmt[0]);
	span = GetBspan(gDispContext.bmem[0].cx, gDispContext.fmt[0]); 
	width = GetBspan(size.cx, gDispContext.fmt[0]); 
	len = GetBmemSize(gDispContext.fmt[0], size);
	V5_MarbB0WriteRepeat(val, len, offset, span, width);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispB0Rev
Input			:	pt		-	The start point to display;
					size	-	The size to display;
					pImage	-	Image data.
Output			:	None.
Description		:	Write image data to B0 in reverse order.
---------------------------------------------------------------------*/
void V5_DispB0Rev(TPoint pt, TSize size, UINT8 *pImage)
{
	UINT32 offset, span, width, len;

	offset = GetBoffset(pt, gDispContext.bmem[0].cx, gDispContext.fmt[0]);
	span = GetBspan(gDispContext.bmem[0].cx, gDispContext.fmt[0]); 
	width = GetBspan(size.cx, gDispContext.fmt[0]); 
	len = GetBmemSize(gDispContext.fmt[0], size);
	V5_MarbB0WriteRev(pImage, len, offset, span, width);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispB0Image
Input			:	pt		-	The start point to display;
					size	-	The size to display;
					pImage	-	Image data.
Output			:	None.
Description		:	Write image data to B0.
---------------------------------------------------------------------*/
void V5_DispB0Image(TPoint pt, TSize size, UINT8 *pImage)
{
	UINT32 offset, span, width, len;

	offset = GetBoffset(pt, gDispContext.bmem[0].cx, gDispContext.fmt[0]);
	span = GetBspan(gDispContext.bmem[0].cx, gDispContext.fmt[0]); 
	width = GetBspan(size.cx, gDispContext.fmt[0]); 
	len = GetBmemSize(gDispContext.fmt[0], size);
	V5_MarbB0Write(pImage, len, offset, span, width);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispB1Image
Input			:	pt		-	The start point to display;
					size	-	The size to display;
					pImage	-	Image data.
Output			:	None.
Description		:	Write image data to B1.
---------------------------------------------------------------------*/
void V5_DispB1Image(TPoint pt, TSize size, UINT8 *pImage)
{
	UINT32 offset, span, width, len;

	offset = GetBoffset(pt, gDispContext.bmem[1].cx, gDispContext.fmt[1]);
	span = GetBspan(gDispContext.bmem[1].cx, gDispContext.fmt[1]); 
	width = GetBspan(size.cx, gDispContext.fmt[1]); 
	len = GetBmemSize(gDispContext.fmt[1], size);
	V5_MarbB1Write(pImage, len, offset, span, width);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispB0DrawBmp
Input			:	pt		-	The start point to display;
					pData	-	Bmp file data.
Output			:	None.
Description		:	Write bmp file data to B0.
---------------------------------------------------------------------*/
void V5_DispB0DrawBmp(TPoint pt, UINT8 *pData)
{
	TBmpInfo info;

	if(ParseBmpData(pData, &info))
		return;
	if(info.fmt != (gDispContext.fmt[0] & 0xf))
		return;
	if(gDispContext.fmt[1] > BLAYER_RGB8)
		V5B_LcdcSetPalette(info.palsize, (UINT32 *)info.ppal);
	V5_DispB0Image(pt, info.size, info.pbuf);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispB1DrawBmp
Input			:	pt		-	The start point to display;
					pData	-	Bmp file data.
Output			:	None.
Description		:	Write bmp file data to B1.
---------------------------------------------------------------------*/
void V5_DispB1DrawBmp(TPoint pt, UINT8 *pData)
{
	TBmpInfo info;

	if(ParseBmpData(pData, &info))
		return;
	if(info.fmt != (gDispContext.fmt[1] & 0xf))
		return;
	if(gDispContext.fmt[0] > BLAYER_RGB8)
		V5B_LcdcSetPalette(info.palsize, (UINT32 *)info.ppal);
	V5_DispB1Image(pt, info.size, info.pbuf);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetMode
Input			:	The VDISP module mode to be set.
Output			:	None.
Description		:	Set VDISP module mode.
---------------------------------------------------------------------*/
void V5_DispSetMode(UINT8 mode)
{
	gDispContext.mode = mode;
	switch(gDispContext.mode)
	{
	case DISP_IDLE_MODE:	//LCD module is not working.
		return;

	case DISP_ABL_MODE:		//Both layer A and B are enable. 
		V5B_LcdcNormalMode();
		break;

	case DISP_DEF_MODE:		//Decode frame mode
		V5B_LcdcDecodeMode();
		break;

	default:
		V5B_LcdcNormalMode();
		break;
	}
	V5B_LcdcResetState();
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetMode
Input			:	None.
Output			:	VDISP module mode.
Description		:	Get current VDISP module mode.
---------------------------------------------------------------------*/
UINT8 V5_DispGetMode(void)
{
	return gDispContext.mode;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetWrTiming
Input			:	The timing value of lcdif module.
Output			:	None.
Description		:	Set timing registers of lcdc and lcdif module.
---------------------------------------------------------------------*/
void V5_DispSetWrTiming(UINT16 x)
{
	UINT8 i, j, delay[3];

	if(gDispContext.pcur == NULL)
		return;

	V5B_LcdifSetWrTiming(x);
	delay[1] = (UINT8)(GetDeInterVal(x) + 1);
	delay[0] = (UINT8)(delay[1] * gDispContext.pcur->lcdif->headnum);
	delay[2] = delay[1];
	if((gDispContext.pcur->type & MULTI_16) == 0)	//for 8-bit bus panel
	{
//		delay[0] <<= 1;
		delay[1] <<= 1;
		delay[2] <<= 1;
	}

	for(i = 0; i < 3; i++)
	{
		j = i << 1;
		delay[i] -= 1;
		delay[i] = delay[i] < gLcdcDelay[j] ? gLcdcDelay[j] : delay[i];
		delay[i] = delay[i] > gLcdcDelay[j+1] ? gLcdcDelay[j+1] : delay[i];
	}

	V5B_LcdcSetDEdelay(delay[0]);
	V5B_LcdcSetDEinteval(delay[1]);
	V5B_LcdcSetLineinteval(delay[2]);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB0BlendRatio
Input			:	Blend ratio. (0x00 ~ 0xfe, 0xff is disable)
Output			:	None.
Description		:	Set alpha blending ratio.
---------------------------------------------------------------------*/
void V5_DispSetB0BlendRatio(UINT8 ratio)
{
	if(ratio != 0xff)
	{
		V5B_LcdcSetB0BlendRatio(ratio);
		V5B_LcdcEnableBlending();
	}
	else
	{
		V5B_LcdcDisableBlending();
	}
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB1BlendRatio
Input			:	Blend ratio. (0x00 ~ 0xfe, 0xff is disable)
Output			:	None.
Description		:	Set alpha blending ratio.
---------------------------------------------------------------------*/
void V5_DispSetB1BlendRatio(UINT8 ratio)
{
	if(ratio != 0xff)
	{
		V5B_LcdcSetB1BlendRatio(ratio);
		V5B_LcdcEnableBlending();
	}
	else
	{
		V5B_LcdcDisableBlending();
	}
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB0BlendRatio
Input			:	None.
Output			:	Ratio value.
Description		:	Get current blending ratio, 0xff is disable.
---------------------------------------------------------------------*/
UINT8 V5_DispGetB0BlendRatio(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if( (x & BLEND_EN) != 0 )
		return V5B_LcdcGetB0BlendRatio();
	else
		return 0xff;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB1BlendRatio
Input			:	None.
Output			:	Ratio value.
Description		:	Get current blending ratio, 0xff is disable.
---------------------------------------------------------------------*/
UINT8 V5_DispGetB1BlendRatio(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if( (x & BLEND_EN) != 0 )
		return V5B_LcdcGetB1BlendRatio();
	else
		return 0xff;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB0Overlay
Input			:	Overlay mode.
Output			:	None.
Description		:	Set overlay mode.
---------------------------------------------------------------------*/
void V5_DispSetB0Overlay(UINT8 mode)
{
	if(mode < 0x04)
	{
		V5B_LcdcSetB0OverlayMode(mode);
		V5B_LcdcEnableOverlay();
	}
	else
	{
		V5B_LcdcDisableOverlay();
	}
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB1Overlay
Input			:	Overlay mode.
Output			:	None.
Description		:	Set overlay mode.
---------------------------------------------------------------------*/
void V5_DispSetB1Overlay(UINT8 mode)
{
	if(mode < 0x04)
	{
		V5B_LcdcSetB1OverlayMode(mode);
		V5B_LcdcEnableOverlay();
	}
	else
	{
		V5B_LcdcDisableOverlay();
	}
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB0Overlay
Input			:	None.
Output			:	Overlay mode.
Description		:	Get current overlay mode.
---------------------------------------------------------------------*/
UINT8 V5_DispGetB0Overlay(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if( (x & OVERMODE_EN) != 0 )
		return V5B_LcdcGetB0OverlayMode();
	else
		return 0xff;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetB1Overlay
Input			:	None.
Output			:	Overlay mode.
Description		:	Get current overlay mode.
---------------------------------------------------------------------*/
UINT8 V5_DispGetB1Overlay(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if( (x & OVERMODE_EN) != 0 )
		return V5B_LcdcGetB1OverlayMode();
	else
		return 0xff;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetBrightness
Input			:	Brightness value to be set.(0~64)
Output			:	None.
Description		:	Set brightness of lcdc module.
---------------------------------------------------------------------*/
void V5_DispSetBrightness(UINT8 value)
{
	SINT8 brightness;
	SINT8 contrast;
	UINT8 new_gamma[17];
	
	if(value > V5_DISP_MAX_BRIGHTNESS)
	{
		value = V5_DISP_MAX_BRIGHTNESS;
	}

	if(gDispContext.pcur == NULL)
		return;

	gDispContext.brightness = value;

	brightness = (SINT8)gDispContext.brightness - 32;
	contrast = (SINT8)gDispContext.contrast - 32;

	CalculateNewGammaCurve(gDispContext.pcur->lcdc->gamma, brightness, contrast, new_gamma);

	V5B_LcdcSetGammaR(new_gamma);
	V5B_LcdcSetGammaG(new_gamma);
	V5B_LcdcSetGammaB(new_gamma);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetBrightness
Input			:	None.
Output			:	Brightness value.
Description		:	Get current brightness value of lcdc module.
---------------------------------------------------------------------*/
UINT8 V5_DispGetBrightness(void)
{
	return	gDispContext.brightness;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetContrast
Input			:	Contrast value to be set.(0~64)
Output			:	None.
Description		:	Set contrast of lcdc module.
---------------------------------------------------------------------*/
void V5_DispSetContrast(UINT8 value)
{
	SINT8 brightness;
	SINT8 contrast;
	UINT8 new_gamma[17];
	
	if(value > V5_DISP_MAX_CONTRAST)
	{
		value = V5_DISP_MAX_CONTRAST;
	}

	if(gDispContext.pcur == NULL)
		return;

	gDispContext.contrast = value;

	brightness = (SINT8)gDispContext.brightness - 32;
	contrast = (SINT8)gDispContext.contrast - 32;

	CalculateNewGammaCurve(gDispContext.pcur->lcdc->gamma, contrast, brightness, new_gamma);

	V5B_LcdcSetGammaR(new_gamma);
	V5B_LcdcSetGammaG(new_gamma);
	V5B_LcdcSetGammaB(new_gamma);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispGetContrast
Input			:	None.
Output			:	Contrast value.
Description		:	Get current contrast value of lcdc module.
---------------------------------------------------------------------*/
UINT8 V5_DispGetContrast(void)
{
	return	gDispContext.contrast;
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetALayerWin
Input			:	pt		-	Window start point;
					size	-	Window size.
Output			:	None.
Description		:	Set layer A window property.
---------------------------------------------------------------------*/
void V5_DispSetALayerWin(TPoint pt, TSize size)
{
	V5B_LcdcSetASptPanel(pt);
	V5B_LcdcSetAWinSize(size);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB0LayerWin
Input			:	pt		-	Window start point;
					size	-	Window size.
Output			:	None.
Description		:	Set layer B window property.
---------------------------------------------------------------------*/
void V5_DispSetB0LayerWin(TPoint pt, TSize size)
{
	V5B_LcdcSetB0SptPanel(pt);
	V5B_LcdcSetB0WinSize(size);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB1LayerWin
Input			:	pt		-	Window start point;
					size	-	Window size.
Output			:	None.
Description		:	Set layer B window property.
---------------------------------------------------------------------*/
void V5_DispSetB1LayerWin(TPoint pt, TSize size)
{
	V5B_LcdcSetB1SptPanel(pt);
	V5B_LcdcSetB1WinSize(size);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB0RefreshWin
Input			:	pt		-	Window start point;
					size	-	Window size.
Output			:	None.
Description		:	Set layer B refresh window property.
---------------------------------------------------------------------*/
void V5_DispSetB0RefreshWin(TPoint pt, TSize size)
{
	V5B_LcdcSetB0rPoint(pt);
	V5B_LcdcSetB0rSize(size);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispSetB1RefreshWin
Input			:	pt		-	Window start point;
					size	-	Window size.
Output			:	None.
Description		:	Set layer B1 refresh window property.
---------------------------------------------------------------------*/
void V5_DispSetB1RefreshWin(TPoint pt, TSize size)
{
	V5B_LcdcSetB1rPoint(pt);
	V5B_LcdcSetB1rSize(size);
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispUpdateB0Win
Input			:	The point to a window position struct.
Output			:	None.
Description		:	Update a certain layer B area.
---------------------------------------------------------------------*/
void V5_DispUpdateB0Win(PTWinPos pwin)
{
	TWinPos winpos;

	if(pwin == NULL)
	{
		winpos.ps.x = 0;
		winpos.ps.y = 0;
		V5B_LcdcGetB0WinSize(&winpos.size);
		V5_DispSetB0RefreshWin(winpos.ps, winpos.size);
	}
	else
		V5_DispSetB0RefreshWin(pwin->ps, pwin->size);
	
	V5B_LcdcB0layerUpdate();
}

/*---------------------------------------------------------------------
Function Name	:	V5_DispUpdateB1Win
Input			:	The point to a window position struct.
Output			:	None.
Description		:	Update a certain layer B1 area.
---------------------------------------------------------------------*/
void V5_DispUpdateB1Win(PTWinPos pwin)
{
	TWinPos winpos;

	if(pwin == NULL)
	{
		winpos.ps.x = 0;
		winpos.ps.y = 0;
		V5B_LcdcGetB1WinSize(&winpos.size);
		V5_DispSetB1RefreshWin(winpos.ps, winpos.size);
	}
	else
		V5_DispSetB1RefreshWin(pwin->ps, pwin->size);
	V5B_LcdcB1layerUpdate();
}

/*---------------------------------------------------------------------
Function Name	:	_ISR_DispLcdcIntHandle
Input			:	INT flag.
Output			:	None.
Description		:	INT service for LCDC FIFO-warning.
---------------------------------------------------------------------*/
void _ISR_DispLcdcIntHandle(UINT16 flag)
{
	if(flag & (0x01 << INT_LCDC_FIFO_WARNING))
	{
		V5B_LcdcResetState();
	}
}

//=====================================
//
//		GE part
//
//====================================

/*---------------------------------------------------------------------
Function Name	:	V5_GeSetDst
Input			:	Destination buffer.
Output			:	None.
Description		:	Select destination buffer of GE operation.
---------------------------------------------------------------------*/
void V5_GeSetDst(UINT8 sel)
{
	gDispContext.dstbsel = sel;
	if(sel > GE_RES2)
		gDispContext.dstbsel = GE_RES2;
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeGetDst
Input			:	None.
Output			:	Destination.
Description		:	Get current destination of GE operation.
---------------------------------------------------------------------*/
UINT8 V5_GeGetDst(void)
{
	return gDispContext.dstbsel;
}

/*---------------------------------------------------------------------
Function Name	:	V5_GetDstFmt
Input			:	None.
Output			:	Color format.
Description		:	Get color format of destination buffer.
---------------------------------------------------------------------*/
UINT8 V5_GetDstFmt(void)
{
	return gDispContext.fmt[gDispContext.dstbsel];
}

/*---------------------------------------------------------------------
Function Name	:	V5_GetSrcFmt
Input			:	None.
Output			:	Color format.
Description		:	Get color format of source buffer.
---------------------------------------------------------------------*/
UINT8 V5_GetSrcFmt(void)
{
	return gDispContext.fmt[gDispContext.srcbsel];
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeSetSrc
Input			:	Source buffer.
Output			:	None.
Description		:	Select source buffer of GE operation.
---------------------------------------------------------------------*/
void V5_GeSetSrc(UINT8 sel)
{
	gDispContext.srcbsel = sel;
	if(sel > GE_RES2)
		gDispContext.dstbsel = GE_RES2;
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeGetSrc
Input			:	None.
Output			:	Source.
Description		:	Get current source buffer.
---------------------------------------------------------------------*/
UINT8 V5_GeGetSrc(void)
{
	return gDispContext.srcbsel;
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeSetResSpan
Input			:	index	-	Reserved buffer index;
					value	-	The span value of reserved buffer.
Output			:	None.
Description		:	Set the span value of a reserved buffer..
---------------------------------------------------------------------*/
void V5_GeSetResSpan(UINT8 index, UINT16 value)
{
	if(index > GE_RES2)
		index = GE_RES2;
	gDispContext.gespan[index] = value;
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeSetResfmt
Input			:	index	-	Reserved buffer index;
					value	-	Color format of reserved buffer.
Output			:	None.
Description		:	Set color format of a reserved buffer..
---------------------------------------------------------------------*/
void V5_GeSetResfmt(UINT8 index, UINT8 value)
{
	if(index > GE_RES2)
		index = GE_RES2;
	gDispContext.fmt[index] = value;
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeSetResStartAdr
Input			:	index	-	Reserved buffer index;
					adr		-	Start address of reserved buffer.
Output			:	None.
Description		:	Set start address of a reserved buffer..
---------------------------------------------------------------------*/
void V5_GeSetResStartAdr(UINT8 index, UINT32 adr)
{
	if(index > GE_RES2)
		index = GE_RES2;
	gDispContext.adr[index] = adr;
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeAutoSetProp
Input			:	None.
Output			:	None.
Description		:	Config GE module according to src & dst property.
---------------------------------------------------------------------*/
void V5_GeAutoSetProp(void)
{
	switch(gDispContext.fmt[gDispContext.srcbsel])
	{
		case BLAYER_RGB1:
		case BLAYER_RGB2:
		case BLAYER_RGB4:
		case BLAYER_RGB8:
		case BLAYER_RGB444:
		case BLAYER_RGB666:
		case BLAYER_RGB32:
			V5B_GeMonoSrc();
			break;
		case BLAYER_RGB555:
		case BLAYER_RGB565:
		case BLAYER_RGB24:
			V5B_GeColorSrc();
			break;
		default:
			break;
	}
	switch(gDispContext.fmt[gDispContext.dstbsel])
	{
		case BLAYER_RGB555:
		case BLAYER_RGB565:
			V5B_GeSetColordepth(GE_RGB565);
			break;
		case BLAYER_RGB24:
			V5B_GeSetColordepth(GE_RGB888);
			break;
		default:
			break;
	}
	
	gDispContext.adr[GE_B0] = V5_MarbGetB0StartAddr();
	gDispContext.adr[GE_B1] = V5_MarbGetB1StartAddr();
	gDispContext.gespan[GE_B0] = GetGeSpan(gDispContext.fmt[GE_B0], gDispContext.bmem[GE_B0].cx);
	gDispContext.gespan[GE_B1] = GetGeSpan(gDispContext.fmt[GE_B1], gDispContext.bmem[GE_B1].cx);

	V5B_GeSetSrcSpan(gDispContext.gespan[gDispContext.srcbsel]);
	V5B_GeSetDstSpan(gDispContext.gespan[gDispContext.dstbsel]);
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeClip
Input			:	Clip value.
Output			:	None.
Description		:	Set clip window.
---------------------------------------------------------------------*/
void V5_GeClip(TClip clip)
{
	V5B_GeSetDstStartAddr(gDispContext.adr[gDispContext.dstbsel]);
	V5B_GeSetClipping(clip);
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeDrawLine
Input			:	ps		-	Start point;
					pe		-	End point;
					color	-	line color;
Output			:	None.
Description		:	Draw line.
---------------------------------------------------------------------*/
void V5_GeDrawLine(TPoint ps, TPoint pe, UINT32 color)
{
	UINT32 adr;

	adr = gDispContext.adr[gDispContext.dstbsel] + ps.y * gDispContext.gespan[gDispContext.dstbsel];
	adr += (ps.x * gPixelByte[gDispContext.fmt[gDispContext.dstbsel]]) >> 3;
	V5B_GeSetDstStartAddr(adr);
	V5B_GeDrawLine(ps, pe, color);
}

/*---------------------------------------------------------------------
Function Name	:	V5_GeBitblt
Input			:	srcp	-	Start point of source buffer;
					dstp	-	Start point of destination buffer;
					size	-	The area size to process;
					bitwise	-	bitwise index;
Output			:	None.
Description		:	Start a bitblt operation.
---------------------------------------------------------------------*/
void V5_GeBitblt(TPoint srcp, TPoint dstp, TSize size, UINT8 bitwise)
{
	UINT32 srcadr, dstadr;
	UINT8 dir = UPLEFT;

	if( (size.cx == 0) || (size.cy == 0))
		return;

	srcadr = gDispContext.adr[gDispContext.srcbsel];
	dstadr = gDispContext.adr[gDispContext.dstbsel];
	if(gDispContext.srcbsel == gDispContext.dstbsel)
	{
		dir = GetBitbltDir(srcp, dstp, size);
		switch(dir)
		{
		case UPLEFT:
			break;
		case UPRIGHT:
			srcadr += (gPixelByte[gDispContext.fmt[gDispContext.srcbsel]] >> 3) - 1;
			dstadr += (gPixelByte[gDispContext.fmt[gDispContext.dstbsel]] >> 3) - 1;
			srcp.x = (UINT16)(srcp.x + size.cx - 1);
			dstp.x = (UINT16)(dstp.x + size.cx - 1);
			break;
		case BTMLEFT:
			srcp.y = (UINT16)(srcp.y + size.cy - 1);
			dstp.y = (UINT16)(dstp.y + size.cy - 1);
			break;
		case BTMRIGHT:
			srcadr += (gPixelByte[gDispContext.fmt[gDispContext.srcbsel]] >> 3) - 1;
			dstadr += (gPixelByte[gDispContext.fmt[gDispContext.dstbsel]] >> 3) - 1;
			srcp.x = (UINT16)(srcp.x + size.cx - 1);
			dstp.x = (UINT16)(dstp.x + size.cx - 1);
			srcp.y = (UINT16)(srcp.y + size.cy - 1);
			dstp.y = (UINT16)(dstp.y + size.cy - 1);
			break;
		default:
			break;
		}
	}
	srcadr += srcp.y * gDispContext.gespan[gDispContext.srcbsel];
	srcadr += (srcp.x * gPixelByte[gDispContext.fmt[gDispContext.srcbsel]]) >> 3;
	dstadr += dstp.y * gDispContext.gespan[gDispContext.dstbsel];
	dstadr += (dstp.x * gPixelByte[gDispContext.fmt[gDispContext.dstbsel]]) >> 3;

	V5B_GeSetStartPoint(dir);
	V5B_GeBitblt(srcadr, dstadr, size, bitwise);
}

#endif //V578CMD_DISABLE
