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


 Module:		Vdef.h

 Description:	VC0938 Data types definitions

 Update History:
	1.0			Basic version				

-----------------------------------------------------------------------*/

#ifndef	__VDEF_H__
#define	__VDEF_H__


#ifndef SINT8
#define SINT8 signed char
#endif

#ifndef PSINT8
#define PSINT8 signed char *
#endif

#ifndef SINT16
#define SINT16 signed short
#endif

#ifndef PSINT16
#define PSINT16 signed short *
#endif

#ifndef SINT32
#define SINT32 signed long
#endif

#ifndef PSINT32
#define PSINT32 signed long *
#endif

#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef PUINT8
#define PUINT8 unsigned char *
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef PUINT16
#define PUINT16 unsigned short *
#endif

#ifndef UINT32
#define UINT32 unsigned long
#endif

#ifndef PUINT32
#define PUINT32 unsigned long *
#endif

#ifndef NULL
#define	NULL	((void *)0)
#endif

#ifndef	TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef	VOID
#define VOID	void
#endif

#ifndef	PVOID
#define PVOID	void *
#endif


#endif //__VDEF_H__
