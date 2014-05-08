/*--------------------------------------------------------------------

			COPYRIGHT (C) 2004, Vimicro Corporation
                        ALL RIGHTS RESERVED

 This source code has been made available to you by VIMICRO on an
 AS-IS basis. Anyone receiving this source code is licensed under VIMICRO
 copyrights to use it in any way he or she deems fit, including copying it,
 modifying it, compiling it, and redistributing it either with or without
 modifications. Any person who transfers this source code or any derivative
 work must include the VIMICRO copyright notice and this paragraph in
 the transferred software.


 Module:		Vmd.c

 Description:	VC0938 common functions definitions

 Update History:
	1.0			Basic version				

-----------------------------------------------------------------------*/
#include "vmd.h"
#include "vdef.h"

void	Vmd_MemSet(HUGE UINT8 *psrc, const UINT8	uValue, UINT32	size)
{
	UINT32	uSize = size;

	while (uSize --){
		psrc[uSize] = uValue;
	}
}

void	Vmd_MemCpy(HUGE UINT8 *pdst, const HUGE UINT8 *psrc, UINT32	size)
{
	UINT32	uSize = size;

	while (uSize --){
		pdst[uSize] = psrc[uSize];
	}
}

void	Vmd_MemCpyEx(HUGE UINT8 *pdst, HUGE UINT8 *psrc)
{
	HUGE UINT8 *sptr, *dptr;

	sptr = psrc;
	dptr = pdst;
	while(*sptr)
		*dptr++ = *sptr++;
}


void	Vmd_MemMove(HUGE UINT8 *dst, const HUGE UINT8 *src, UINT32	n)
{
	if (n == 0)
		return ;
	if (src >= dst+n || dst >= src+n)
	{
		Vmd_MemCpy(dst, src, n);
		return;
	}

	if (src > dst) {
		while ((SINT32)--n >= 0)
			*dst++ = *src++;
	}
	else if (src < dst) {
		src += n;
		dst += n;
		while ((SINT32)--n >= 0)
			*--dst = *--src;
	}
}

SINT8 Vmd_MemCmp (const HUGE UINT8 *s1, const HUGE UINT8 *s2, UINT32 n)
{
    HUGE UINT8 *p1 = (HUGE UINT8 *) s1;
    HUGE UINT8 *p2 = (HUGE UINT8 *) s2;
    HUGE UINT8 *end = p1 + n;

    while (p1 < end)
    {
        if (*p1 != *p2)
        {
            if (*p1 < *p2)
            {
                return (-1);
            }
            else
            {
                return (1);
            }
        }
        ++p1;
        ++p2;
    }

    return (0);               /* memory blocks are equal */
}

