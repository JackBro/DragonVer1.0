


#ifndef _TRM_USERBASERW_H_
#define _TRM_USERBASERW_H_

#ifdef __cplusplus
extern "C" {
#endif


/*-------define V5 chip mapping address ---------*/
/*Seperate*/
/*used by register access*/
#define V5_INTER_ADDR_REGUIA		0x8000000


//Multi8
/*Host send address/data of VC0578 according the follow address of host*/
#define V5_MULTI8_ADDR_ADDR			0x8000200
#define V5_MULTI8_ADDR_DATA			0x8000300


//Muti16
/*Host send address/data of VC0578 according the follow address of host*/
#define V5_MULTI16_ADDR_ADDR		0x8000200
#define V5_MULTI16_ADDR_DATA		0x8000300


//Bypass
/*Host send address/data of VC0578 according the follow address of host*/
#define V5_BYPASS_ADDR_WRITE_RSLOW_CS1				0x8000000
#define V5_BYPASS_ADDR_WRITE_RSHIGH_CS1				0x8000200
#define V5_BYPASS_ADDR_WRITE_RSLOW_CS2				0xa000000
#define V5_BYPASS_ADDR_WRITE_RSHIGH_CS2				0xa000200
#define V5_BYPASS_ADDR_READ_RSLOW_CS1				0x8000100
#define V5_BYPASS_ADDR_READ_RSHIGH_CS1				0x8000300
#define V5_BYPASS_ADDR_READ_RSLOW_CS2				0xa000100
#define V5_BYPASS_ADDR_READ_RSHIGH_CS2				0xa000300


#define   V5_By8SetRSLowCs1(val)               V5_HOSTSETVAL8(V5_BYPASS_ADDR_WRITE_RSLOW_CS1, (UINT8)val)
#define   V5_By8SetRSHighCs1(val)              V5_HOSTSETVAL8(V5_BYPASS_ADDR_WRITE_RSHIGH_CS1, (UINT8)val)
#define   V5_By16SetRSLowCs1(val)              V5_HOSTSETVAL16(V5_BYPASS_ADDR_WRITE_RSLOW_CS1, val)
#define   V5_By16SetRSHighCs1(val)             V5_HOSTSETVAL16(V5_BYPASS_ADDR_WRITE_RSHIGH_CS1, val)
#define   V5_By8SetRSLowCs2(val)               V5_HOSTSETVAL8(V5_BYPASS_ADDR_WRITE_RSLOW_CS2, (UINT8)val)
#define   V5_By8SetRSHighCs2(val)              V5_HOSTSETVAL8(V5_BYPASS_ADDR_WRITE_RSHIGH_CS2, (UINT8)val)
#define   V5_By16SetRSLowCs2(val)              V5_HOSTSETVAL16(V5_BYPASS_ADDR_WRITE_RSLOW_CS2, val)
#define   V5_By16SetRSHighCs2(val)             V5_HOSTSETVAL16(V5_BYPASS_ADDR_WRITE_RSHIGH_CS2, val)
#define   V5_By8GetRSLowCs1()                  V5_HOSTGETVAL8(V5_BYPASS_ADDR_READ_RSLOW_CS1)
#define   V5_By8GetRSHighCs1()                 V5_HOSTGETVAL8(V5_BYPASS_ADDR_READ_RSHIGH_CS1)
#define   V5_By16GetRSLowCs1()                 V5_HOSTGETVAL16(V5_BYPASS_ADDR_READ_RSLOW_CS1)
#define   V5_By16GetRSHighCs1()                V5_HOSTGETVAL16(V5_BYPASS_ADDR_READ_RSHIGH_CS1)
#define   V5_By8GetRSLowCs2()                  V5_HOSTGETVAL8(V5_BYPASS_ADDR_READ_RSLOW_CS2)
#define   V5_By8GetRSHighCs2()                 V5_HOSTGETVAL8(V5_BYPASS_ADDR_READ_RSHIGH_CS2)
#define   V5_By16GetRSLowCs2()                 V5_HOSTGETVAL16(V5_BYPASS_ADDR_READ_RSLOW_CS2)
#define   V5_By16GetRSHighCs2()                V5_HOSTGETVAL16(V5_BYPASS_ADDR_READ_RSHIGH_CS2)


/*the follow macros and functions should be defined by CPU*/
#define   Push2DisIntEnFlag() 
#define   PopIntEnFlag()


#ifdef WIN32

void   V5_HOSTSETADDR8(UINT32 addr, UINT8 val);
void   V5_HOSTSETVAL8(UINT32 addr, UINT8 val);
UINT8  V5_HOSTGETVAL8(UINT32 addr);
void   V5_HOSTSETADDR16(UINT32 addr, UINT16 val);
void   V5_HOSTSETVAL16(UINT32 addr, UINT16 val);
UINT16 V5_HOSTGETVAL16(UINT32 addr);

#else

#define V5_HOSTSETADDR8( addr, val ) *(volatile UINT8*)addr = val
#define V5_HOSTSETVAL8( addr, val ) *(volatile UINT8*)addr = val
#define V5_HOSTGETVAL8( addr ) *(volatile UINT8*)addr
#define V5_HOSTSETADDR16( addr, val ) *(volatile UINT16*)addr = val
#define V5_HOSTSETVAL16( addr, val ) *(volatile UINT16*)addr = val
#define V5_HOSTGETVAL16( addr ) *(volatile UINT16*)addr

#endif


void V5_HostDelay(UINT32 x);

#ifdef __cplusplus
}
#endif

#endif

