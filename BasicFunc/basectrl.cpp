// V558TestFpgaDlg.cpp : 实现文件
//
#include "internal.h"
#include "../terminal/terminal/main/main.h"

void CBaseCtrl::WriteReg(int type, int adr, int val)
{
	switch(type)
	{
#ifndef HOSTCMD_DISABLE
	case HOST_REGTYPE:
		WriteMemory(adr, val);
		break;
#endif

#ifndef V578CMD_DISABLE
	case V558_REGTYPE:
		V5_SetReg((UINT32)adr, (UINT8)val, 0);
		break;

	case V568_REGTYPE:
		V5_SetReg((UINT32)adr, (UINT8)val, 0);
		break;

	case SENSOR_REGTYPE:
		//V5B_SifI2cWriteByte((UINT8)adr, (UINT8)val);
		//V5B_SifI2cWriteWord((UINT8)adr, (UINT16)val);
		if(gVispContext.pinfo)
		{
			if(gVispContext.pinfo->snrSetRegCall)
				gVispContext.pinfo->snrSetRegCall((UINT8)adr, (UINT16)val);
		}
		break;
	
	case PANEL_REGTYPE:
		PanelSetReg((UINT32)adr, (UINT16)val);
		break;

	case PANEL_GRAMLINETYPE:
		PanelWriteLine((UINT16)adr, (UINT16)val);
		break;
	
	case LCD_BY_SETRSLOW:
		//adr = adr;
		V5_BySetRSLow( (UINT16)val );
		break;

	case LCD_BY_SETRSHIGH:
		//adr =adr;
		V5_BySetRSHigh( (UINT16)val );
		break;

	case LCD_BY_SETREG:
		break;
#endif

#ifndef USBCMD_DISABLE
	case USB9604_REG:
		Usb_SetReg((UINT8)adr, (UINT8)val);
		break;
#endif

#ifndef V938CMD_DISABLE
	case V938_REGTYPE:
        V9_SetReg((UINT32)adr, (UINT8)val, 0);
		break;
#endif

	default:
		break;
	}
}

void CBaseCtrl::WriteReg(TRegUnit *unit)
{
	this->WriteReg(unit->regtype, unit->regaddr, unit->regval);
	if(unit->delay)
		Sleep(unit->delay);
}

int	CBaseCtrl::ReadReg(int type, int adr)
{
	int val = 0;

	switch(type)
	{
#ifndef HOSTCMD_DISABLE
	case HOST_REGTYPE:
		val = ReadMemory(adr);
		break;
#endif

#ifndef V578CMD_DISABLE
	case V558_REGTYPE:
		val = V5_GetReg((UINT32)adr, BIT_8);
		break;
	case V568_REGTYPE:
		val = V5_GetReg((UINT32)adr, BIT_8);
		break;
	case SENSOR_REGTYPE:
		//V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
		//V5B_SifI2cReadWord((UINT8)uAddr, (UINT16*)uVal);
		V5_VispGetSnrReg((UINT8)adr, (UINT16*)&val);
		break;
	case PANEL_REGTYPE:
		val = PanelGetReg((UINT32)adr);
		break;
	case PANEL_GRAMLINETYPE:
		break;

	case LCD_BY_GETRSLOW:
		//adr = adr;
		val = V5_ByGetRSLow();
		break;

	case LCD_BY_GETRSHIGH:
		//adr = adr;
		val = V5_ByGetRSHigh();
		break;

	case LCD_BY_GETREG:
		break;
#endif

#ifndef USBCMD_DISABLE
	case USB9604_REG:
		val = Usb_GetReg((UINT8)adr);
		break;
#endif

#ifndef V938CMD_DISABLE
	case V938_REGTYPE:
        val = V9_GetReg((UINT32)adr, 0);
		break;
#endif

	default:
		break;
	}
	return val;
}

void CBaseCtrl::ReadReg(TRegUnit *unit)
{
	unit->regval = this->ReadReg(unit->regtype, unit->regaddr);
}

