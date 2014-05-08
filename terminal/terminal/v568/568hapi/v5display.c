#include "inheaders.h"
#ifndef	V578CMD_DISABLE


void V5M_LoadBmp(UINT8 *pbmp, UINT8 dst)
{
	TBmpInfo info;
	TPoint pt = {0, 0};
	TBLayerProty proty;

	if(ParseBmpData(pbmp, &info))
		return;

	proty.memsize = info.size;
	proty.pos2panel = pt;
	proty.refwin.ltp = pt;
	proty.refwin.size = info.size;
	proty.win.ltp = pt;
	proty.win.size = info.size;
	switch(dst)
	{
	case GE_B0:
		V5_DispSetB0Proty(&proty);
		V5_DispSetB0format(info.fmt);
		V5_MarbAutoSetMem();
		V5_DispB0DrawBmp(pt, pbmp);
		break;
	case GE_B1:
		V5_DispSetB1Proty(&proty);
		V5_DispSetB1format(info.fmt);
		V5_MarbAutoSetMem();
		V5_DispB1DrawBmp(pt, pbmp);
		break;
	default :
		return;
	}
}

void V5M_DisplayB0Bmp(UINT8 *pbmp)
{
	V5M_LoadBmp(pbmp, GE_B0);
	V5_DispUpdateB0Win(NULL);
}

void V5M_DispSetPanelRegList(PTRegGroup parm, UINT8 type)
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

void V5M_DispSetPanelState(UINT8 state, UINT8 flag)
{
	UINT8 type = 0;
	PTPanelAccess	paccess = NULL;
	PTRegGroup		preg = NULL;

	if(flag)
		if(gPanelCfg.pBacklightOnEx)
			gPanelCfg.pBacklightOnEx();			// backlight

	paccess = gPanelCfg.pMainCfg;
	if(PANEL_SLAVE == V5_DispGetPanelSel())
		paccess = gPanelCfg.pSubCfg;

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
	V5M_DispSetPanelRegList(preg, type);
}

#endif //V578CMD_DISABLE

