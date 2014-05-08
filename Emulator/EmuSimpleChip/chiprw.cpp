

#include "internal.h"

static CBufferBase	gSimRegPool;

void SimRegOpen(void)
{
	memset(gSimRegPool.m_buf, 0, gSimRegPool.GetTotalSize());
}

void SimReg_SetReg(int adr, int val, int type)
{
	adr &= 0xffff;
	if(adr == 0x1003)
		return;
	switch(type)
	{
	case BIT_8:
		gSimRegPool.m_buf[adr] = (char)val;
		break;
	case BIT_16_LTOH:
		gSimRegPool.m_buf[adr] = (char)val;
		gSimRegPool.m_buf[adr+1] = (char)(val >> 8);
		break;
	case BIT_16_HTOL:
		gSimRegPool.m_buf[adr] = (char)(val >> 8);
		gSimRegPool.m_buf[adr+1] = (char)val;
		break;
	case BIT_24_LTOH:
		gSimRegPool.m_buf[adr] = (char)val;
		gSimRegPool.m_buf[adr+1] = (char)(val >> 8);
		gSimRegPool.m_buf[adr+2] = (char)(val >> 16);
		break;
	case BIT_24_HTOL:
		gSimRegPool.m_buf[adr] = (char)(val >> 16);
		gSimRegPool.m_buf[adr+1] = (char)(val >> 8);
		gSimRegPool.m_buf[adr+2] = (char)(val >> 0);
		break;
	case BIT_32_LTOH:
		gSimRegPool.m_buf[adr] = (char)val;
		gSimRegPool.m_buf[adr+1] = (char)(val >> 8);
		gSimRegPool.m_buf[adr+2] = (char)(val >> 16);
		gSimRegPool.m_buf[adr+2] = (char)(val >> 24);
		break;
	case BIT_32_HTOL:
		gSimRegPool.m_buf[adr] = (char)(val >> 24);
		gSimRegPool.m_buf[adr+1] = (char)(val >> 16);
		gSimRegPool.m_buf[adr+2] = (char)(val >> 8);
		gSimRegPool.m_buf[adr+3] = (char)(val >> 0);
		break;
	default:
		break;
	}
}

int SimReg_GetReg(int adr, int type)
{
	int val = 0;

	adr &= 0xffff;
	if(adr == 0x83f)
		return 0xf;
	if(adr == 0x1420)
		return 8;
	switch(type)
	{
	case BIT_8:
		val = gSimRegPool.m_buf[adr] & 0xff;
		break;
	case BIT_16_LTOH:
		val = gSimRegPool.m_buf[adr] & 0xff;
		val |= (gSimRegPool.m_buf[adr+1] << 8) & 0xff00;
		break;
	case BIT_16_HTOL:
		val = (gSimRegPool.m_buf[adr] << 8) & 0xff00;
		val |= gSimRegPool.m_buf[adr+1] & 0xff;
		break;
	case BIT_24_LTOH:
		val = gSimRegPool.m_buf[adr] & 0xff;
		val |= (gSimRegPool.m_buf[adr+1] << 8) & 0xff00;
		val |= (gSimRegPool.m_buf[adr+2] << 16) & 0xff0000;
		break;
	case BIT_24_HTOL:
		val = (gSimRegPool.m_buf[adr] << 16) & 0xff0000;
		val |= (gSimRegPool.m_buf[adr+1] << 8) & 0xff00;
		val |= gSimRegPool.m_buf[adr+2] & 0xff;
		break;
	case BIT_32_LTOH:
		val = gSimRegPool.m_buf[adr] & 0xff;
		val |= (gSimRegPool.m_buf[adr+1] << 8) & 0xff00;
		val |= (gSimRegPool.m_buf[adr+2] << 16) & 0xff0000;
		val |= (gSimRegPool.m_buf[adr+2] << 24) & 0xff000000;
		break;
	case BIT_32_HTOL:
		val = (gSimRegPool.m_buf[adr] << 24) & 0xff000000;
		val |= (gSimRegPool.m_buf[adr+1] << 16) & 0xff0000;
		val |= (gSimRegPool.m_buf[adr+2] << 8) & 0xff00;
		val |= gSimRegPool.m_buf[adr+3] & 0xff;
		break;
	default:
		break;
	}
	return val;
}

#define USBMAPMASK		0xff
#define USBMAPOFFSET	0x4000

void SimUsb_SetReg(int adr, int val)
{
	adr &= USBMAPMASK;
	adr += USBMAPOFFSET;
	gSimRegPool.m_buf[adr] = (char)val;
}

int SimUsb_GetReg(int adr)
{
	adr &= USBMAPMASK;
	adr += USBMAPOFFSET;
	return (int)gSimRegPool.m_buf[adr] & 0xff;
}

#define HOSTMAPOFFSET	0x5000

static int SearchAdr(int adr)
{
	int i, j, pos = 0;

	for(i = 0; i < gArm7ReglistCount; i++)
	{
		for(j = 0; j < gArm7Reglist[i].count; j++, pos++)
		{
			if(gArm7Reglist[i].parray[j].regaddr == adr)
				return pos;
		}
	}
	return pos;
}

void Simhost_SetReg(int adr, int val)
{
	int *ptr = (int *)(gSimRegPool.m_buf + HOSTMAPOFFSET);

	adr = SearchAdr(adr);
	ptr[adr] = val;
}

int Simhost_GetReg(int adr)
{
	int *ptr = (int *)(gSimRegPool.m_buf + HOSTMAPOFFSET);

	adr = SearchAdr(adr);
	return ptr[adr];
}



