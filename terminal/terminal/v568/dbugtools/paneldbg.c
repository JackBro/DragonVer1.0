
#include "../userapi/internal.h"
#include "../driver/basefunc/basefunc.h"
#ifndef	V578CMD_DISABLE

#include "vdbugtools.h"
#include "../highapi/bypass.h"

static TPanelDbgContext gPanelDbgContext;

void PanelDbgInitContext(void)
{
	memset(&gPanelDbgContext, 0, sizeof(gPanelDbgContext));

	V5B_LcdifGetBiasSpt(&gPanelDbgContext.biaspt);
	V5_DispGetPanelSize(&gPanelDbgContext.panelsize);

	gPanelDbgContext.RsLW = V5B_LcdifRsLExW;
	gPanelDbgContext.RsHW = V5B_LcdifRsHExW;
	gPanelDbgContext.RsLR = V5B_LcdifRsLExR;
	gPanelDbgContext.RsHR = V5B_LcdifRsHExR;
}

void PanelDbgSetMode(UINT8 mode)
{
	if(mode == BYPASS_MODE)
	{
		gPanelDbgContext.RsLW = V5_BySetRSLow;
		gPanelDbgContext.RsHW = V5_BySetRSHigh;
		gPanelDbgContext.RsLR = V5_ByGetRSLow;
		gPanelDbgContext.RsHR = V5_ByGetRSHigh;
	}
	else
	{
		gPanelDbgContext.RsLW = V5B_LcdifRsLExW;
		gPanelDbgContext.RsHW = V5B_LcdifRsHExW;
		gPanelDbgContext.RsLR = V5B_LcdifRsLExR;
		gPanelDbgContext.RsHR = V5B_LcdifRsHExR;
	}
}


#if V5_LCD_TYPE == V5_DEF_LCD_LTS180S3_HF1

void  PanelSetReg(UINT32 adr, UINT16 val)
{
	gPanelDbgContext.RsLW((UINT16)adr);
	gPanelDbgContext.RsHW(val);
}

UINT16  PanelGetReg(UINT32 adr)
{
	UINT16 val;

	if((gPanelDbgContext.RsLR == NULL) || (gPanelDbgContext.RsHR == NULL))
		return 0;

	gPanelDbgContext.RsLW((UINT16)adr);
	val = gPanelDbgContext.RsHR();

	return val;
}

void PanelWriteGram(TPoint pt, UINT16 val, UINT16 len)
{
	UINT16 i;

	gPanelDbgContext.RsLW(0x21);
	gPanelDbgContext.RsHW(((pt.y & 0Xff) << 8) | (pt.x & 0xff));
	gPanelDbgContext.RsLW(0x22);
	for(i = 0; i < len; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteLine(UINT16 line, UINT16 val)
{
	UINT16 i;

	gPanelDbgContext.RsLW(0x21);
	gPanelDbgContext.RsHW(line);
	gPanelDbgContext.RsLW(0x22);
	for(i = 0; i < gPanelDbgContext.panelsize.cx; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteImage(UINT16 *buf, UINT32 len)
{
	UINT16 i;

	for(i = 0; i < len; i++)
	{
		if( (i % gPanelDbgContext.panelsize.cx) == 0)
		{
			gPanelDbgContext.RsLW(0x21);
			gPanelDbgContext.RsHW((i / gPanelDbgContext.panelsize.cx) << 8);
			gPanelDbgContext.RsLW(0x22);
		}
		gPanelDbgContext.RsHW(buf[i]);
	}
}


#elif V5_LCD_TYPE == V5_DEF_LCD_LTS180SH_H1I_X

void  PanelSetReg(UINT32 adr, UINT16 val)
{
	gPanelDbgContext.RsLW((UINT16)adr);
	gPanelDbgContext.RsHW(val);
}

UINT16  PanelGetReg(UINT32 adr)
{
	UINT16 val;

	if((gPanelDbgContext.RsLR == NULL) || (gPanelDbgContext.RsHR == NULL))
		return 0;

	gPanelDbgContext.RsLW((UINT16)adr);
	val = gPanelDbgContext.RsHR();

	return val;
}

void PanelWriteGram(TPoint pt, UINT16 val, UINT16 len)
{
	UINT16 i;

	pt.x = pt.x + gPanelDbgContext.biaspt.x;
	pt.y = pt.y + gPanelDbgContext.biaspt.y;

	gPanelDbgContext.RsLW(0x20);
	gPanelDbgContext.RsHW(pt.x);
	gPanelDbgContext.RsLW(0x21);
	gPanelDbgContext.RsHW(pt.y);
	gPanelDbgContext.RsLW(0x22);
	for(i = 0; i < len; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteLine(UINT16 line, UINT16 val)
{
	UINT16 i;
	TPoint pt;

	pt.x = (line & 0xff) + gPanelDbgContext.biaspt.x;
	pt.y = ((line >> 8) & 0xff) + gPanelDbgContext.biaspt.y;

	gPanelDbgContext.RsLW(0x20);
	gPanelDbgContext.RsHW(pt.x);
	gPanelDbgContext.RsLW(0x21);
	gPanelDbgContext.RsHW(pt.y);
	gPanelDbgContext.RsLW(0x22);
	for(i = 0; i < gPanelDbgContext.panelsize.cx; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteImage(UINT16 *buf, UINT32 len)
{
	UINT16 i;

	for(i = 0; i < len; i++)
	{
		if( (i % gPanelDbgContext.panelsize.cx) == 0 )
		{
			gPanelDbgContext.RsLW(0x20);
			gPanelDbgContext.RsHW(gPanelDbgContext.biaspt.x);
			gPanelDbgContext.RsLW(0x21);
			gPanelDbgContext.RsHW(i / gPanelDbgContext.panelsize.cx + gPanelDbgContext.biaspt.y);
			gPanelDbgContext.RsLW(0x22);
		}
		gPanelDbgContext.RsHW(buf[i]);
	}
}

#elif V5_LCD_TYPE == V5_DEF_LCD_AMPIRE128160

void  PanelSetReg(UINT32 adr, UINT16 val)
{
	gPanelDbgContext.RsLW(0x00);
	gPanelDbgContext.RsLW((UINT16)adr & 0xff);
	gPanelDbgContext.RsHW((UINT16)(val >> 8) & 0xff);
	gPanelDbgContext.RsHW(val & 0xff);
}

UINT16  PanelGetReg(UINT32 adr)
{
	UINT16 val = 0;

	if((gPanelDbgContext.RsLR == NULL) || (gPanelDbgContext.RsHR == NULL))
		return 0;

	gPanelDbgContext.RsLW(0x00);
	gPanelDbgContext.RsLW((UINT16)adr & 0xff);
	gPanelDbgContext.RsHR();
	gPanelDbgContext.RsHR();
	val = gPanelDbgContext.RsHR();
	val = (val << 8) | (0xff & gPanelDbgContext.RsHR());
	return val;
}

void PanelWriteGram(TPoint pt, UINT16 val, UINT16 len)
{
	UINT16 i;

	gPanelDbgContext.RsLW(0x00);
	gPanelDbgContext.RsLW(0x21);
	gPanelDbgContext.RsHW(pt.y & 0xff);
	gPanelDbgContext.RsHW(pt.x & 0xff);
	gPanelDbgContext.RsLW(0x00);
	gPanelDbgContext.RsLW(0x22);
	for(i = 0; i < len; i++)
	{
		gPanelDbgContext.RsHW((val >> 8) & 0xff);
		gPanelDbgContext.RsHW(val & 0xff);
	}
}

void PanelWriteLine(UINT16 line, UINT16 val)
{
	UINT16 i;

	gPanelDbgContext.RsLW(0x00);
	gPanelDbgContext.RsLW(0x21);
	gPanelDbgContext.RsHW(line & 0xff);
	gPanelDbgContext.RsHW(0x00);
	gPanelDbgContext.RsLW(0x00);
	gPanelDbgContext.RsLW(0x22);
	for(i = 0; i < gPanelDbgContext.panelsize.cx; i++)
	{
		gPanelDbgContext.RsHW((val >> 8) & 0xff);
		gPanelDbgContext.RsHW(val & 0xff);
	}
}

void PanelWriteImage(UINT16 *buf, UINT32 len)
{
	UINT16 i;

	for(i = 0; i < len; i++)
	{
		if( (i % gPanelDbgContext.panelsize.cx) == 0)
		{
			gPanelDbgContext.RsLW(0x00);
			gPanelDbgContext.RsLW(0x21);
			gPanelDbgContext.RsHW((i / gPanelDbgContext.panelsize.cx) & 0xff);
			gPanelDbgContext.RsHW(0x00);
			gPanelDbgContext.RsLW(0x00);
			gPanelDbgContext.RsLW(0x22);
		}
		gPanelDbgContext.RsHW((buf[i] >> 8) & 0xff);
		gPanelDbgContext.RsHW(buf[i] & 0xff);
	}
}

#elif V5_LCD_TYPE == V5_DEF_LCD_HD66781S

void  PanelSetReg(UINT32 adr, UINT16 val)
{
	gPanelDbgContext.RsLW((UINT16)adr);
	gPanelDbgContext.RsHW(val);
}

UINT16  PanelGetReg(UINT32 adr)
{
	UINT16 val = 0;

	if((gPanelDbgContext.RsLR == NULL) || (gPanelDbgContext.RsHR == NULL))
		return 0;

	gPanelDbgContext.RsLW((UINT16)adr);
	val = gPanelDbgContext.RsHR();

	return val;
}

void PanelWriteGram(TPoint pt, UINT16 val, UINT16 len)
{
	UINT16 i = 0;

	gPanelDbgContext.RsLW(0x200);
	gPanelDbgContext.RsHW(pt.x);
	gPanelDbgContext.RsLW(0x201);
	gPanelDbgContext.RsHW(pt.y);
	gPanelDbgContext.RsLW(0x202);
	for(i = 0; i < len; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteLine(UINT16 line, UINT16 val)
{
	UINT16 i;

	gPanelDbgContext.RsLW(0x200);
	gPanelDbgContext.RsHW(0);
	gPanelDbgContext.RsLW(0x201);
	gPanelDbgContext.RsHW(line);
	gPanelDbgContext.RsLW(0x202);
	for(i = 0; i < gPanelDbgContext.panelsize.cx; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteImage(UINT16 *buf, UINT32 len)
{
	UINT32 i = 0;

	for(i = 0; i < len; i++)
	{
		if( (i % gPanelDbgContext.panelsize.cx) == 0)
		{
			gPanelDbgContext.RsLW(0x200);
			gPanelDbgContext.RsHW(0);
			gPanelDbgContext.RsLW(0x201);
			gPanelDbgContext.RsHW((UINT16)(i / gPanelDbgContext.panelsize.cx));
			gPanelDbgContext.RsLW(0x202);
		}
		gPanelDbgContext.RsHW(buf[i]);
	}
}

#elif V5_LCD_TYPE == V5_DEF_LCD_UPD161801_24

void  PanelSetReg(UINT32 adr, UINT16 val)
{
	gPanelDbgContext.RsLW((UINT16)adr);
	gPanelDbgContext.RsLW(val);
}

UINT16  PanelGetReg(UINT32 adr)
{
	UINT16 val = 0;

	if((gPanelDbgContext.RsLR == NULL) || (gPanelDbgContext.RsLR == NULL))
		return 0;

	gPanelDbgContext.RsLW((UINT16)adr);
	val = gPanelDbgContext.RsLR();
	gPanelDbgContext.RsHR();
	return val;
}

void PanelWriteGram(TPoint pt, UINT16 val, UINT16 len)
{
	UINT16 i = 0;

	gPanelDbgContext.RsLW(0x600);
	gPanelDbgContext.RsLW(pt.x);
	gPanelDbgContext.RsLW(0x700);
	gPanelDbgContext.RsLW(pt.y);
	for(i = 0; i < len; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteLine(UINT16 line, UINT16 val)
{
	UINT16 i;

	gPanelDbgContext.RsLW(0x600);
	gPanelDbgContext.RsLW(0);
	gPanelDbgContext.RsLW(0x700);
	gPanelDbgContext.RsLW(line);
	for(i = 0; i < gPanelDbgContext.panelsize.cx; i++)
		gPanelDbgContext.RsHW(val);
}

void PanelWriteImage(UINT16 *buf, UINT32 len)
{
	UINT32 i = 0;

	for(i = 0; i < len; i++)
	{
		if( (i % gPanelDbgContext.panelsize.cx) == 0)
		{
			gPanelDbgContext.RsLW(0x600);
			gPanelDbgContext.RsLW(0);
			gPanelDbgContext.RsLW(0x700);
			gPanelDbgContext.RsLW((UINT16)(i / gPanelDbgContext.panelsize.cx));
		}
		gPanelDbgContext.RsHW(buf[i]);
	}
}

#else

void  PanelSetReg(UINT32 adr, UINT16 val)
{
	adr = val;
}

UINT16  PanelGetReg(UINT32 adr)
{
	return (UINT16)adr;
}

void	PanelWriteGram(TPoint pt, UINT16 val, UINT16 len)
{
	val = pt.x;
	val = len;
}

void	PanelWriteLine(UINT16 line, UINT16 val)
{
	line = val;
}

void	PanelWriteImage(UINT16 *buf, UINT32 len)
{
	len = buf[0];
}

#endif

#endif //V578CMD_DISABLE
