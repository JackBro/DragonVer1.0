
#include "internal.h"
#ifndef	V578CMD_DISABLE

#include "../driver/basefunc/basefunc.h"

static TSmpDispContext gSmpDispContext;

#ifdef NCSC

char Image$$RW$$Limit[MAX_TRANSBUF_SIZE];
char *gTransferBuf = Image$$RW$$Limit;

static const TSmpFrameProp FrameGroup_Mass[] = {
	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/15b_logo.bmp",			0   },		//logo
	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/15b_butterfly.bmp",		0   },		//bitblt

	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/15b_frame.bmp",			0xf8},		//frame 0
	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/15b_frame_1.bmp",		0xf8},		//frame 1

	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/15b_no_frame.bmp",		0	}		//no frame
};

static const TSmpFrameProp FrameGroup_Din[] = {
	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/24b_logo.bmp",			0   },		//logo
	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/24b_butterfly.bmp",		0   },		//bitblt

	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/24b_frame.bmp",			0xff},		//frame 0
	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/24b_frame_1.bmp",		0xff},		//frame 1

	{ "/disk7/home/huoxf/work/VmonitorVer1.0/flash/24b_no_frame.bmp",		0	}		//no frame
};

#else

static const TSmpFrameProp FrameGroup_Mass[] = {
	{ "/flash/15b_logo.bmp",			0   },		//logo
	{ "/flash/15b_butterfly.bmp",		0   },		//bitblt

	{ "/flash/15b_frame.bmp",			0xf8},		//frame 0
	{ "/flash/15b_frame_1.bmp",			0xf8},		//frame 1

	{ "/flash/15b_no_frame.bmp",		0	}		//no frame
};

static const TSmpFrameProp FrameGroup_Din[] = {
	{ "/flash/24b_logo.bmp",			0   },		//logo
	{ "/flash/24b_butterfly.bmp",		0   },		//bitblt

	{ "/flash/24b_frame.bmp",			0xff},		//frame 0
	{ "/flash/24b_frame_1.bmp",			0xff},		//frame 1

	{ "/flash/24b_no_frame.bmp",		0	}		//no frame
};
#endif

//	Function for each demo item
//	RM
static void SmpFuncRM(void)
{
	UINT16 tmp = 0;
	TSize size;

	gSmpDispContext.rm_cur ++;
	gSmpDispContext.rm_cur %= 0x08;

	size = gSmpDispContext.panel_size;
	if(gSmpDispContext.rm_cur % 2)
	{
		tmp = size.cx;
		size.cx = size.cy;
		size.cy = tmp;
	}

	V5_IppSetDispSizer(size);
	V5B_LcdcSetAlayerRM(gSmpDispContext.rm_cur);
}

//	Line draw
static UINT32 SmpFuncColorCnvtForDst(UINT32 color_24b)
{
	UINT8 fmt = 0;
	UINT32 color_output = 0;

	fmt = V5_GetDstFmt();
	if(fmt == BLAYER_RGB24)
		color_output = color_24b;
	else if(fmt == BLAYER_RGB565)
		color_output = (UINT32)Convert24To565(color_24b);
	else if(fmt == BLAYER_RGB555)
		color_output = (UINT32)Convert24To555(color_24b);

	return color_output;
}

static void SmpFuncDrawRect(TPoint *pt, UINT32 color)
{
	UINT8 i = 0;
	UINT32 linecolor;

	linecolor = SmpFuncColorCnvtForDst(color);
	for(i = 0; i < 3; i++)
		V5_GeDrawLine(pt[i], pt[i+1], linecolor);
	V5_GeDrawLine(pt[3], pt[0], linecolor);
}

static void SmpFuncDrawGraph(TPoint ps, TSize size, UINT32 *color, UINT8 num)
{
	TPoint corner_pt[4];
	TPoint next_pt;
	TSize next_size;

	corner_pt[0] = ps;
	corner_pt[1].x = ps.x + size.cx;
	corner_pt[1].y = ps.y;
	corner_pt[2].x = ps.x + size.cx;
	corner_pt[2].y = ps.y + size.cy;
	corner_pt[3].x = ps.x;
	corner_pt[3].y = ps.y + size.cy;
#ifndef NCSC
	V5_Delay(500);
#endif
	SmpFuncDrawRect(corner_pt, color[0]);

	corner_pt[0].x = ps.x + (size.cx >> 1);
	corner_pt[0].y = ps.y;
	corner_pt[1].x = ps.x + size.cx;
	corner_pt[1].y = ps.y + (size.cy >> 1);
	corner_pt[2].x = ps.x + (size.cx >> 1);
	corner_pt[2].y = ps.y + size.cy;
	corner_pt[3].x = ps.x;
	corner_pt[3].y = ps.y + (size.cy >> 1);
#ifndef NCSC
	V5_Delay(500);
#endif
	SmpFuncDrawRect(corner_pt, color[1]);

	num -= 1;
	if(num == 0)
		return;

	next_pt.x = ps.x + (size.cx >> 2);
	next_pt.y = ps.y + (size.cy >> 2);
	next_size.cx = (size.cx >> 1);
	next_size.cy = (size.cy >> 1);
	SmpFuncDrawGraph(next_pt, next_size, &color[2], num);
}

static void SmpFuncDraw(void)
{
	UINT8 num = 2;
	TPoint pt, ps, pe;
	TSize size;
	const UINT32 linecolor[] = {0xff0000, 0xff00, 0xff, 0xff00ff, 0xffff00, 0xff00ff, 0xffff, 0xff00};

	size.cx = gSmpDispContext.panel_size.cx >> 1;
	size.cy = size.cx;
	pt.x = gSmpDispContext.panel_size.cx >> 2;
	pt.y = (gSmpDispContext.panel_size.cy - size.cy) >> 1;

	SmpDisappearMenu();
	SmpFuncDrawGraph(pt, size, (UINT32 *)linecolor, num);

	num = 0x04;
	while(num < 0x08)
	{
		switch(num)
		{
		case 4:
			ps.x = gSmpDispContext.panel_size.cx - 1;
			ps.y = 0;
			pe = pt;
			break;
		case 5:
			ps.x = gSmpDispContext.panel_size.cx - 1;
			ps.y = gSmpDispContext.panel_size.cy - 1;
			pe.x = pt.x + size.cx - 1;
			pe.y = pt.y;
			break;
		case 6:
			ps.x = 0;
			ps.y = gSmpDispContext.panel_size.cy - 1;
			pe.x = pt.x + size.cx - 1;
			pe.y = pt.y + size.cy - 1;
			break;
		case 7:
			ps.x = 0;
			ps.y = 0;
			pe.x = pt.x;
			pe.y = pt.y + size.cy - 1;
			break;
		default :
			ps.x = ps.y = pe.x = pe.y = 0;
			break;
		}
#ifndef NCSC
		V5_Delay(500);
#endif
		V5_GeDrawLine(ps, pe, linecolor[num]);
		num ++;
	}

#ifndef NCSC
	V5_Delay(5000);
#endif
	SmpDispRefreshB0();

	V5_PreDispFont(GE_B0);
	SmpSendKey(MENU_KEY_SELECT);	// to re-draw menu.
}

//	Bitblt
static void SmpFuncButterfly(void)
{
	UINT8 i = 0, cnt = 0;

	const TPoint srcpt[] = {
		{0,    0},
		{64,   0},
		{0,   53},
		{0,  106},
		{64, 106}
	};
	TWinPos dstwin = {{64, 53}, {64, 53}};

	V5_GeSetSrc(GE_B0);
	V5_GeSetDst(GE_B0);
	V5_GeAutoSetProp();
	V5B_GeDisableSrcMask();

	while(cnt < SMP_DISP_LOOP_NUM)
	{
		for(i = 0; i < 5; i ++)
		{
			V5_GeBitblt(srcpt[i], dstwin.ps, dstwin.size, 0xcc);
			V5_DispUpdateB0Win(&dstwin);
#ifndef NCSC
			V5_Delay(80);
#endif
		}
		for(i = 4; i > 0; i --)
		{
			V5_GeBitblt(srcpt[i], dstwin.ps, dstwin.size, 0xcc);
			V5_DispUpdateB0Win(&dstwin);
#ifndef NCSC
			V5_Delay(80);
#endif
		}
		
		cnt ++;
	}
}

static void SmpFuncBitBLT(void)
{
	V5B_LcdcDisableAlayer();
	SmpDisappearMenu();

	SmpDispLoadFrame(GE_B0, SMP_FRAME_BITBLT);
	V5_DispUpdateB0Win(NULL);
#ifndef NCSC
	V5_Delay(100);
#endif

	SmpFuncButterfly();
#ifndef NCSC
	V5_Delay(600);
#endif

	SmpDispRefreshB0();
	V5B_LcdcEnableAlayer();

	V5_PreDispFont(GE_B0);
	SmpSendKey(MENU_KEY_SELECT);	// to re-draw menu.
}

//Change layer A window
static void SmpFuncAWin(void)
{
	UINT8 x = 0xff;
	UINT16 flag = 0xffff;
	TALayerProty layera;

	if(gSmpDispContext.awin_step == AWIN_STEP_MIN)
		gSmpDispContext.awin_dir = AWIN_DIR_INC;
	if(gSmpDispContext.awin_step > AWIN_STEP_MAX - 1)
		gSmpDispContext.awin_dir = AWIN_DIR_DEC;

	if(gSmpDispContext.awin_dir)
		gSmpDispContext.awin_step --;
	else
		gSmpDispContext.awin_step ++;
	
	V5_DispGetAmemSize(&layera);
	layera.win.size.cx = gSmpDispContext.panel_size.cx >> gSmpDispContext.awin_step;
	layera.win.size.cy = gSmpDispContext.panel_size.cy >> gSmpDispContext.awin_step;

	V5B_LcdcDisableAlayer();
	x = 0xff;
	while(x && flag)
	{
		x = V5B_LcdcGetDe();
		x &= 0x12;
		flag --;
	}

	V5_DispSetAmemSize(&layera);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();
}

//	Blending
static void SmpFuncSwitchFrame(UINT8 sel)
{
	SmpDisappearMenu();

	if(sel == SMP_PRE_FRAME)
	{
		sel = gSmpDispContext.fore_id - 1;
		if(sel == SMP_FRAME_BITBLT)
			sel = SMP_FRAME_NULL;
	}
	else
	{
		sel = gSmpDispContext.fore_id + 1;
		if(sel == SMP_FRAME_TOTAL_NUM)
			sel = SMP_FRAME_0;
	}

	SmpDispLoadAndDispFrame(sel);
	SmpSendKey(MENU_KEY_SELECT);	// to re-draw menu.
}

static void SmpFuncPreFrame(void)
{
	SmpFuncSwitchFrame(SMP_PRE_FRAME);
}

static void SmpFuncNextFrame(void)
{
	SmpFuncSwitchFrame(SMP_NEXT_FRAME);
}

static void SmpFuncBlend(UINT8 sel)
{
	if(sel > SMP_BLEND_DEC)
		sel = SMP_BLEND_DEC;

	gSmpDispContext.blend_cur = V5_DispGetB0BlendRatio();
	if(sel == SMP_BLEND_INC)
	{
		if(gSmpDispContext.blend_cur != 0xfe)
		{
			if(gSmpDispContext.blend_cur < 0xff - SMP_DISP_BLEND_STEP)
				gSmpDispContext.blend_cur += SMP_DISP_BLEND_STEP;
			else if(gSmpDispContext.blend_cur < 0xfe)
				gSmpDispContext.blend_cur = 0xfe;
			else
				gSmpDispContext.blend_cur = 0;
		}
	}
	else
	{
		if(gSmpDispContext.blend_cur != 0xff)
		{
			if(gSmpDispContext.blend_cur > SMP_DISP_BLEND_STEP)
				gSmpDispContext.blend_cur -= SMP_DISP_BLEND_STEP;
			else if(gSmpDispContext.blend_cur)
				gSmpDispContext.blend_cur = 0;
			else
				gSmpDispContext.blend_cur = 0xff;
		}
	}

	V5_DispSetB0BlendRatio((UINT8)gSmpDispContext.blend_cur);
}

static void SmpFuncBlendInc(void)
{
	SmpFuncBlend(SMP_BLEND_INC);
}

static void SmpFuncBlendDec(void)
{
	SmpFuncBlend(SMP_BLEND_DEC);
}

//	Overlay
static void SmpFuncOverlay(UINT8 sel)
{
	if(sel > SMP_OVERLAY_INVERT)
		sel = SMP_OVERLAY_INVERT;

	gSmpDispContext.overlay_cur = sel;
	V5_DispSetB0Overlay(gSmpDispContext.overlay_cur);
}

void SmpFuncOverlayTrans(void)
{
	SmpFuncOverlay(SMP_OVERLAY_TRANS);
}

void SmpFuncOverlayAnd(void)
{
	SmpFuncOverlay(SMP_OVERLAY_AND);
}

void SmpFuncOverlayOr(void)
{
	SmpFuncOverlay(SMP_OVERLAY_OR);
}

void SmpFuncOverlayInvert(void)
{
	SmpFuncOverlay(SMP_OVERLAY_INVERT);
}


//	For Disp module menu control
//	Main Menu
void SmpDisp(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA,			NULL},
		{ MENU_KEY_DOWN,	MENU_JPEG,				NULL},
		{ MENU_KEY_LEFT,	MENU_EXIT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_BLENDLING,	NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_BLENDLING,	NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

//	Disp Menu
void SmpDispBlend(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_EXIT,				NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_OVERLAY,			NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,					NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_BLENDLING_ADD,	NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_BLENDLING_ADD,	NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispOverlay(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_BLENDLING,		NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_RM,				NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,					NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_OVERLAY_TRAN,		NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_OVERLAY_TRAN,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispRM(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_OVERLAY,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_LINEDRAW,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,			NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_RM,		NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_RM,		SmpFuncRM}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispLineDraw(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_RM,		NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_BITBLT,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,			NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_LINEDRAW,	NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_LINEDRAW,	SmpFuncDraw}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispBitblt(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_LINEDRAW,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_AWIN,		NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,			NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_BITBLT,	NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_BITBLT,	SmpFuncBitBLT}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispAWin(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_BITBLT,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_RETURN,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,			NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_AWIN,		NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_AWIN,		SmpFuncAWin}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_AWIN,		NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_EXIT,		NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,			NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_RETURN,	NULL},
		{ MENU_KEY_SELECT,	MENU_DISP,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_RETURN,		NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_BLENDLING,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP,				NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_EXIT,			NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

//	Blending Menu
void SmpDispBldInc(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_BLENDLING_EXIT,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_BLENDLING_DEC,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_BLENDLING_ADD,	SmpFuncPreFrame},
		{ MENU_KEY_RIGHT,	MENU_DISP_BLENDLING_ADD,	SmpFuncNextFrame},
		{ MENU_KEY_SELECT,	MENU_DISP_BLENDLING_ADD,	SmpFuncBlendInc}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispBldDec(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_BLENDLING_ADD,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_BLENDLING_RETURN,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_BLENDLING_DEC,	SmpFuncPreFrame},
		{ MENU_KEY_RIGHT,	MENU_DISP_BLENDLING_DEC,	SmpFuncNextFrame},
		{ MENU_KEY_SELECT,	MENU_DISP_BLENDLING_DEC,	SmpFuncBlendDec}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispBldReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_BLENDLING_DEC,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_BLENDLING_EXIT,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_BLENDLING,		NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_BLENDLING_RETURN,	NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_BLENDLING,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispBldExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_BLENDLING_RETURN,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_BLENDLING_ADD,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_BLENDLING,		NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_BLENDLING_EXIT,	NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

//	Overlay Menu
void SmpDispOlyTrans(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_OVERLAY_EXIT,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_OVERLAY_AND,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_OVERLAY_TRAN,	SmpFuncPreFrame},
		{ MENU_KEY_RIGHT,	MENU_DISP_OVERLAY_TRAN,	SmpFuncNextFrame},
		{ MENU_KEY_SELECT,	MENU_DISP_OVERLAY_TRAN,	SmpFuncOverlayTrans}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispOlyAnd(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_OVERLAY_TRAN,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_OVERLAY_OR,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_OVERLAY_AND,	SmpFuncPreFrame},
		{ MENU_KEY_RIGHT,	MENU_DISP_OVERLAY_AND,	SmpFuncNextFrame},
		{ MENU_KEY_SELECT,	MENU_DISP_OVERLAY_AND,	SmpFuncOverlayAnd}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispOlyOr(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_OVERLAY_AND,		NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_OVERLAY_INVERT,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_OVERLAY_OR,		SmpFuncPreFrame},
		{ MENU_KEY_RIGHT,	MENU_DISP_OVERLAY_OR,		SmpFuncNextFrame},
		{ MENU_KEY_SELECT,	MENU_DISP_OVERLAY_OR,		SmpFuncOverlayOr}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispOlyInvert(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_OVERLAY_OR,		NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_OVERLAY_RETURN,	NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_OVERLAY_INVERT,	SmpFuncPreFrame},
		{ MENU_KEY_RIGHT,	MENU_DISP_OVERLAY_INVERT,	SmpFuncNextFrame},
		{ MENU_KEY_SELECT,	MENU_DISP_OVERLAY_INVERT,	SmpFuncOverlayInvert}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispOlyReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_OVERLAY_INVERT,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_OVERLAY_EXIT,		NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_OVERLAY,			NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_OVERLAY_RETURN,	NULL},
		{ MENU_KEY_SELECT,	MENU_DISP_OVERLAY,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispOlyExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP_OVERLAY_RETURN,	NULL},
		{ MENU_KEY_DOWN,	MENU_DISP_OVERLAY_TRAN,		NULL},
		{ MENU_KEY_LEFT,	MENU_DISP_OVERLAY,			NULL},
		{ MENU_KEY_RIGHT,	MENU_DISP_OVERLAY_EXIT,		NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}


///////////////////////////////////////////////////
void SmpDispLoadFrame(UINT8 dst, UINT8 sel)
{
	UINT32 len;

	if(dst > GE_B1)
		dst = GE_B1;

	if(sel >= SMP_FRAME_TOTAL_NUM)
		sel = SMP_FRAME_TOTAL_NUM - 1;

	if(gVC0568ChipID == MASS_VER)
		len = LoadImageFile(FrameGroup_Mass[sel].path, gTransferBuf, SMP_MAIN_MENU_IMAGE_SIZE);
	if(gVC0568ChipID == DESIGNIN_VER)
		len = LoadImageFile(FrameGroup_Din[sel].path, gTransferBuf, SMP_MAIN_MENU_IMAGE_SIZE);

	if(dst == GE_B0)
	{
		V5_LoadBmp((UINT8*)gTransferBuf, GE_B0);
		if(gVC0568ChipID == MASS_VER)
			V5B_LcdcSetB0OverKeyColor(FrameGroup_Mass[sel].keycolor);
		if(gVC0568ChipID == DESIGNIN_VER)
			V5B_LcdcSetB0OverKeyColor(FrameGroup_Din[sel].keycolor);
		gSmpDispContext.fore_id = sel;
	}
	else
	{
		V5_LoadBmp((UINT8*)gTransferBuf, GE_B1);
		gSmpDispContext.back_id = sel;
	}
}

void SmpDispGraphCopy(UINT8 src, UINT8 dst)
{
	TPoint srcpt = {0, 0}, pt = {0, 0};

	V5_GeSetSrc(src);
	V5_GeSetDst(dst);
	V5_GeAutoSetProp();
	V5B_GeDisableSrcMask();
	V5_GeBitblt(srcpt, pt, gSmpDispContext.panel_size, 0xcc);

	gSmpDispContext.fore_id = gSmpDispContext.back_id;
	if(gVC0568ChipID == MASS_VER)
		V5B_LcdcSetB0OverKeyColor(FrameGroup_Mass[gSmpDispContext.fore_id].keycolor);
	if(gVC0568ChipID == DESIGNIN_VER)
		V5B_LcdcSetB0OverKeyColor(FrameGroup_Din[gSmpDispContext.fore_id].keycolor);

	V5_PreDispFont(GE_B0);
}

void SmpDispRefreshB0(void)	// copy b1 to b0
{
	SmpDispGraphCopy(GE_B1, GE_B0);
}

void SmpDispRefreshB1(void)	// copy b0 to b1
{
	SmpDispGraphCopy(GE_B0, GE_B1);
}

void SmpDispLoadAndDispFrame(UINT8 sel)
{
	SmpDispLoadFrame(GE_B1, sel);
	SmpDispRefreshB0();
}

void SmpDispInitContext(void)
{
	memset(&gSmpDispContext, 0, sizeof(gSmpDispContext));

	V5_DispGetPanelSize(&gSmpDispContext.panel_size);
}

void SmpDispOpen(void)
{
	SmpDispInitContext();

	V5B_LcdcLayerB0First();
	V5_GraphInit();
	V5_FontInit(NULL);

	SmpDispLoadFrame(GE_B0, SMP_FRAME_LOGO);
	V5_DispUpdateB0Win(NULL);
	V5_DispSetB0Overlay(SMP_OVERLAY_TRANS);
}

///////////////////////////////////////////////////

void SmpDispThumbFrame(TPoint ps, TSize size, UINT32 color, UINT8 thick_sel)
{
	UINT8 i = 0;
	TPoint pt[4];

	if(size.cx < (thick_sel << 1))
		return;

	V5_GeSetDst(GE_B1);
	V5_GeAutoSetProp();

	for(i = 0; i < thick_sel; i ++)
	{
		pt[0].x = ps.x + i;
		pt[0].y = ps.y + i;
		pt[1].x = pt[0].x + size.cx - 1;
		pt[1].y = pt[0].y;
		pt[2].x = pt[0].x + size.cx - 1;
		pt[2].y = pt[0].y + size.cy - 1;
		pt[3].x = pt[0].x;
		pt[3].y = pt[0].y + size.cy - 1;
		SmpFuncDrawRect(pt, color);

		size.cx -= 2;
		size.cy -= 2;
	}
}

//////////////////////////////////////////////////////////
//						Disp Test						//
//////////////////////////////////////////////////////////
void SmpDispTest_Loop(void)
{
	UINT8 i = 0, j;
	UINT8 str[] = "Hello World !";
	TPoint sp;
	sp.x = 0x00;
	sp.y = 0x00;

	while(i < 100)
	{
		sp.y = 0x00;
		SmpDispRefreshB0();
		for(j = 0; j < 10; j++)
		{
			V5_DisplayStr(sp, str, OP_S_COLOR1);
			sp.y += 0x10;
		}
		i ++;
	}
}

void SmpDispTest_BSel(void)
{
}

void SmpDispTest_B0Clr(void)
{
	SmpDispLoadFrame(GE_B0, SMP_FRAME_NULL);
}

void SmpDispTest_B1Clr(void)
{
	SmpDispLoadFrame(GE_B1, SMP_FRAME_NULL);
}

void SmpDispTest12(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST11,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST13,			NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST12,			NULL},
		{ MENU_KEY_SELECT,	MENU_TEST12,			SmpDispTest_Loop}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispTest13(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST12,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST14,			NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST13,			NULL},
		{ MENU_KEY_SELECT,	MENU_TEST13,			SmpDispTest_BSel}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispTest14(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST13,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST15,			NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST14,			NULL},
		{ MENU_KEY_SELECT,	MENU_TEST14,			SmpDispTest_B0Clr}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpDispTest15(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST14,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST1,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST15,			NULL},
		{ MENU_KEY_SELECT,	MENU_TEST15,			SmpDispTest_B1Clr}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

#endif //V578CMD_DISABLE
