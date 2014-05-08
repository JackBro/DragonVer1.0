#include "internal.h"
#ifndef	V578CMD_DISABLE

#include "../driver/basefunc/basefunc.h"
#ifdef WIN32
#include "../v568env/v5initenv.h"
#endif

static TGraphContext gGraphContext;

void V5_LoadBmp(UINT8 *pbmp, UINT8 dst)
{
	TBmpInfo info;
	TPoint pt = {0, 0};
	TBLayerProty proty;

	if(ParseBmpData(pbmp, &info))
		return;

	switch(dst)
	{
	case GE_B0:
		proty.memsize = info.size;
		proty.pos2panel.x = 0;
		proty.pos2panel.y = 0;
		proty.refwin.ltp = pt;
		proty.refwin.size = info.size;
		proty.win.ltp = pt;
		proty.win.size = info.size;

		V5_DispSetB0Proty(&proty);
		V5_DispSetB0format(info.fmt);
		V5_MarbAutoSetMem();
		V5_DispB0DrawBmp(pt, pbmp);
		break;
	case GE_B1:
		proty.memsize = info.size;
		proty.pos2panel.x = 0;
		proty.pos2panel.y = 0x50;
		proty.refwin.ltp = pt;
		proty.refwin.size = info.size;
		proty.win.ltp = pt;
		proty.win.size = info.size;

		V5_DispSetB1Proty(&proty);
		V5_DispSetB1format(info.fmt);
		V5_MarbAutoSetMem();
		V5_DispB1DrawBmp(pt, pbmp);
		break;
	default :
		return;
	}
}

void V5_DisplayB0Bmp(UINT8 *pbmp)
{
	V5_LoadBmp(pbmp, GE_B0);
	V5_DispUpdateB0Win(NULL);
}

void V5_DisplayB1Bmp(UINT8 *pbmp)
{
	V5_LoadBmp(pbmp, GE_B1);
	V5_DispUpdateB1Win(NULL);
}

void V5_FontInit(TFontInfo *pinfo)
{
	if(pinfo != NULL)
		gGraphContext.info = *pinfo;
	pinfo = &(gGraphContext.info);
	V5_MarbSetMemStart(pinfo->len);
	V5_MarbAutoSetMem();
	gGraphContext.fontadr = 0;
	V5_MarbWriteSpanBuf(pinfo->pbuf, pinfo->len, gGraphContext.fontadr, pinfo->span, pinfo->span);
	gGraphContext.status = FONT_INITED;
}

void V5_GraphInit(void)
{
	memset(&gGraphContext, 0, sizeof(TGraphContext));
	gGraphContext.info.fmt = BLAYER_RGB1;
	gGraphContext.info.len = gFontMatrixSize;

	if(gVC0568ChipID == MASS_VER)
		gGraphContext.info.pbuf = (UINT8 *)gAscFontMatrix_Mass;
	if(gVC0568ChipID == DESIGNIN_VER)
		gGraphContext.info.pbuf = (UINT8 *)gAscFontMatrix_Din;

	gGraphContext.info.span = V5_FONT_SPAN;
	gGraphContext.info.unit.cx = V5_FONT_WIDTH;
	gGraphContext.info.unit.cy = V5_FONT_HEIGHT;
	gGraphContext.info.srcfg = V5_FONT_SRC_FORE_COLOR1;
	gGraphContext.info.srcbg = V5_FONT_SRC_BACK_COLOR;
	gGraphContext.info.ptnfg = V5_FONT_PTN_FORE_COLOR;
	gGraphContext.info.ptnbg = V5_FONT_PTN_BACK_COLOR;
}

void V5_SetSrcColor(void)
{
	UINT8 fmt;

	fmt = V5_GetDstFmt();
	if(fmt == BLAYER_RGB24)
	{
		V5B_GeSetSrcForeColor(gGraphContext.info.srcfg);
		V5B_GeSetSrcBkColor(gGraphContext.info.srcbg);
	}
	else if(fmt == BLAYER_RGB565)
	{
		V5B_GeSetSrcForeColor((UINT32)Convert24To565(gGraphContext.info.srcfg));
		V5B_GeSetSrcBkColor((UINT32)Convert24To565(gGraphContext.info.srcbg));
	}
	else if(fmt == BLAYER_RGB555)
	{
		V5B_GeSetSrcForeColor((UINT32)Convert24To555(gGraphContext.info.srcfg));
		V5B_GeSetSrcBkColor((UINT32)Convert24To555(gGraphContext.info.srcbg));
	}
}

void V5_PreDispFont(UINT8 dst)		//dst must be B0 or B1
{
	if( (gGraphContext.status & FONT_INITED) == 0)
		return;

	gGraphContext.olddst = V5_GeGetDst();
	gGraphContext.oldsrc = V5_GeGetSrc();
	
	V5_GeSetDst(dst);
	V5_GeSetSrc(GE_FONT);
	V5_GeSetResSpan(GE_FONT, gGraphContext.info.span);
	V5_GeSetResfmt(GE_FONT, gGraphContext.info.fmt);

	V5_GeAutoSetProp();

	V5B_GeMonoSrcUsePtnSrc();
	V5B_GeEnableSrcMask();
}

void V5_PostDispFont(void)
{
	if( (gGraphContext.status & FONT_INITED) == 0)
		return;
	V5_GeSetDst(gGraphContext.olddst);
	V5_GeSetSrc(gGraphContext.oldsrc);
	V5_GeAutoSetProp();
}

void V5_DisplayChar(TPoint pt, UINT8 ch, UINT8 option)
{
	TPoint srcpt;
	UINT8 bitwise = 0xcc;

	if( (gGraphContext.status & FONT_INITED) == 0)
		return;

	if(option > OP_S_COLOR2)
		option = OP_S_COLOR2;

	if(gVC0568ChipID == MASS_VER)
	{
		if(option)
		{
			gGraphContext.info.srcfg = V5_FONT_SRC_FORE_COLOR2;
			gGraphContext.info.srcbg = V5_FONT_SRC_BACK_COLOR;
		}
		else
		{
			gGraphContext.info.srcfg = V5_FONT_SRC_FORE_COLOR1;
			gGraphContext.info.srcbg = V5_FONT_SRC_BACK_COLOR;
		}
	}

	if(gVC0568ChipID == DESIGNIN_VER)
	{
		if(option)
		{
			gGraphContext.info.srcfg = V5_FONT_SRC_BACK_COLOR;
			gGraphContext.info.srcbg = V5_FONT_SRC_FORE_COLOR2;
		}
		else
		{
			gGraphContext.info.srcfg = V5_FONT_SRC_BACK_COLOR;
			gGraphContext.info.srcbg = V5_FONT_SRC_FORE_COLOR1;
		}
	}

	V5_SetSrcColor();

	srcpt.x = 0;
	srcpt.y = (UINT16)ch << 4;
	V5_GeBitblt(srcpt, pt, gGraphContext.info.unit, bitwise);
}

void V5_DisplayStr(TPoint pt, UINT8 *str, UINT8 option)
{
	UINT16 i = 0, len;
	TSize panel_size;

	if( (gGraphContext.status & FONT_INITED) == 0)
		return;

	len = (UINT16)strlen((char *)str);
	V5_DispGetPanelSize(&panel_size);
	if(pt.x + len * V5_FONT_WIDTH > panel_size.cx)
		len = (panel_size.cx - pt.x) / V5_FONT_WIDTH;

	while(i < len)
	{
		V5_DisplayChar(pt, str[i], option);

		pt.x += V5_FONT_WIDTH;
		i ++;
	}
}

void V5_DispGetCurFontSize(TSize *size)
{
	size->cx = gGraphContext.info.unit.cx;
	size->cy = gGraphContext.info.unit.cy;
}


static void V5_DispSetPanelRegList(PTRegGroup parm, UINT8 type)
{
	UINT16 i;

	for(i = 0; i < parm->len; i++)
	{
		Panel_SetLcdReg(type, parm->regval[i].adr, parm->regval[i].val);
#ifndef NCSC
		V5_Delay(parm->regval[i].wait);
#endif
	}
}

void V5_DispSetPanelState(UINT8 state, UINT8 flag)
{
	UINT8 type = 0;
	PTPanelAccess	paccess = NULL;
	PTRegGroup		preg = NULL;

	if(flag)
		if(gPanelCfg.pBacklightOnEx)
			gPanelCfg.pBacklightOnEx();			// backlight

#ifdef	WIN32
	type = V5_GetCurPanel();
	paccess = gW32PanelCfg[type]->pMainCfg;
	if(PANEL_SLAVE == V5_DispGetPanelSel())
		paccess = gW32PanelCfg[type]->pSubCfg;
#else
	paccess = gPanelCfg.pMainCfg;
	if(PANEL_SLAVE == V5_DispGetPanelSel())
		paccess = gPanelCfg.pSubCfg;
#endif

	if(paccess == NULL)
		return;

	switch(state)
	{
	case PANEL_POWERON:
		preg = &(paccess->poweron);
		break;
	case PANEL_STANDBY:
		preg = &(paccess->standby);
		break;
	case PANEL_POWEROFF:
		preg = &(paccess->poweroff);
		break;
	case PANEL_SLEEP:
		preg = &(paccess->sleep);
		break;
	case PANEL_WAKEUPFROMSTANDBY:
		preg = &(paccess->wakeupstb);
		break;
	case PANEL_WAKEUPFSLEEP:
		preg = &(paccess->wakeup);
		break;
	default:
		return;
	}
	type = V5_DispGetPanelType();
	V5_DispSetPanelRegList(preg, type);
}

#endif //V578CMD_DISABLE
