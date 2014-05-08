


#ifndef _TRM_USERBASERW938_H_
#define _TRM_USERBASERW938_H_

#ifdef __cplusplus
extern "C" {
#endif


//Multi8, parrel
#define V9_MULTI8_ADDR_ADDR			0x6000000
#define V9_MULTI8_ADDR_DATA			0x6000001

/*the follow macros and functions should be defined by CPU*/
#ifdef WIN32

#define   Push2DisIntEnFlag() 
#define   PopIntEnFlag()

void   V9_HOSTSETADDR8(UINT32 addr, UINT8 val);
void   V9_HOSTSETVAL8(UINT32 addr, UINT8 val);
UINT8  V9_HOSTGETVAL8(UINT32 addr);
void   V9_HOSTSETADDR16(UINT32 addr, UINT16 val);
void   V9_HOSTSETVAL16(UINT32 addr, UINT16 val);
UINT16 V9_HOSTGETVAL16(UINT32 addr);

#else

#ifdef NCSC

#define   Push2DisIntEnFlag() 
#define   PopIntEnFlag()

#else

/*not use disable/enable interrupt functions now*/
//void   Push2DisIntEnFlag(void); 
//void   PopIntEnFlag(void);
#define   Push2DisIntEnFlag() 
#define   PopIntEnFlag()

#endif

#define V9_HOSTSETADDR8( addr, val ) *(volatile UINT8*)addr = val
#define V9_HOSTSETVAL8( addr, val ) *(volatile UINT8*)addr = val
#define V9_HOSTGETVAL8( addr ) *(volatile UINT8*)addr
#define V9_HOSTSETADDR16( addr, val ) *(volatile UINT16*)addr = val
#define V9_HOSTSETVAL16( addr, val ) *(volatile UINT16*)addr = val
#define V9_HOSTGETVAL16( addr ) *(volatile UINT16*)addr

#endif

void V9_HostDelay(UINT32 x);

#ifdef __cplusplus
}
#endif

#endif

