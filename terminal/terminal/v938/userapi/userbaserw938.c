
#include "internal938.h"


#ifdef WIN32
void V9_HOSTSETADDR8(UINT32 addr, UINT8 val)
{
	WriteMemory(addr, val);
}

void V9_HOSTSETVAL8(UINT32 addr, UINT8 val)
{
	WriteMemory(addr, val);
}

UINT8 V9_HOSTGETVAL8(UINT32 addr)
{
	return (UINT8)ReadMemory(addr);
}

void V9_HOSTSETADDR16(UINT32 addr, UINT16 val)
{
	WriteMemory(addr, val);
}

void V9_HOSTSETVAL16(UINT32 addr, UINT16 val)
{
	WriteMemory(addr, val);
}

UINT16 V9_HOSTGETVAL16(UINT32 addr)
{
	return (UINT16)ReadMemory(addr);
}

#endif

#ifndef NCSC
void V9_HostDelay(UINT32 x)
{
	Delay((int)x);
}
#endif

