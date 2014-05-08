
#include "internal.h"
#ifndef	V578CMD_DISABLE
#include "../driver/basefunc/lcdc.h"

void TestDisp(void)
{
	V5_DispInitContext();
	V5_DispSetPanelInfo((PTPanelInfo)&gPanelInfo);
	V5_DispOpen();

	V5_DispSetPanelState(PANEL_POWERON, 1);
}

void TestPreview(void)
{
	TSize size = {640, 480};
	TSize size1 = {128, 96};
	TPoint pt = {0, 0};
	TALayerProty proty;

	proty.memsize = size1;
	proty.pos2panel = pt;
	proty.win.ltp = pt;
	proty.win.size = size1;

	V5_SetMode(PREVIEW_MODE);
	V5B_LcdcEnableAlayer();
	V5_DispSetAmemSize(&proty);
	V5_VispSetDispSizer(0, pt, size, size1);
//	V5B_SifEnableSnrSyncGen();
}

static void TestDrawFrame(UINT16 offset, TSize size)
{
	UINT16 i = 0;
	TPoint point[4], ps, pe;

	point[0].x = offset;
	point[0].y = offset;
	point[1].x = size.cx - 1 - offset;
	point[1].y = offset;
	point[2].x = size.cx - 1 - offset;
	point[2].y = size.cy - 1 - offset;
	point[3].x = offset;
	point[3].y = size.cy - 1 - offset;

	for(i = 0; i < 4; i++)
	{
		ps = point[i];
		pe = (i == 3) ? point[0] : point[i+1];
		V5_GeDrawLine(ps, pe, 0x87ff);
	}
}

void TestB0Frame(void)
{
	TSize  size;
	TPoint pt = {0, 0};
	UINT16 i = 0, offset = 16;
	UINT32 keycolor = 0, val = 0;

	V5_DispGetPanelSize(&size);
	size.cy >>= 1;
	keycolor = V5B_LcdcGetB0OverKeyColor();
	val = Convert24To565(keycolor);
	keycolor = (val << 16) | val;
	V5_DispB0Mono(pt, size, keycolor);

	V5_GeAutoSetProp();
	for(i = 0; i < 1; i++, offset += 4)
		TestDrawFrame(offset, size);
}

#endif //V578CMD_DISABLE
