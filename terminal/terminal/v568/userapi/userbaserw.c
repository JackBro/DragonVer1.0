
#include "internal.h"
#ifndef	V578CMD_DISABLE

#ifdef WIN32

void V5_HOSTSETADDR8(UINT32 addr, UINT8 val)
{
	WriteMemory(addr, val);
}

void V5_HOSTSETVAL8(UINT32 addr, UINT8 val)
{
	WriteMemory(addr, val);
}

UINT8 V5_HOSTGETVAL8(UINT32 addr)
{
	return (UINT8)ReadMemory(addr);
}

void V5_HOSTSETADDR16(UINT32 addr, UINT16 val)
{
	WriteMemory(addr, val);
}

void V5_HOSTSETVAL16(UINT32 addr, UINT16 val)
{
	WriteMemory(addr, val);
}

UINT16 V5_HOSTGETVAL16(UINT32 addr)
{
	return (UINT16)ReadMemory(addr);
}

#endif

#ifndef NCSC
void V5_HostDelay(UINT32 x)
{
	Delay((int)x);
}
#endif

#endif
