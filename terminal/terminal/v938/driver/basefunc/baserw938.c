
#include "basefunc938.h"

#ifndef	V938CMD_DISABLE

#include "../../userapi/userbaserw938.h"

#ifdef WIN32
#include "../../../s3c44box/s3c44box.h"
#endif


static void   V9_Mul8SetRegWType0(UINT32 adr, UINT32 val);
static void   V9_Mul8SetRegWType1(UINT32 adr, UINT32 val);
static void   V9_Mul8SetRegWType2(UINT32 adr, UINT32 val);
static void   V9_Mul8SetRegWType3(UINT32 adr, UINT32 val);
static void   V9_Mul8SetRegWType4(UINT32 adr, UINT32 val);
static void   V9_Mul8SetRegWType5(UINT32 adr, UINT32 val);
static void   V9_Mul8SetRegWType6(UINT32 adr, UINT32 val);
static UINT32 V9_Mul8GetRegWType0(UINT32 adr);
static UINT32 V9_Mul8GetRegWType1(UINT32 adr);
static UINT32 V9_Mul8GetRegWType2(UINT32 adr);
static UINT32 V9_Mul8GetRegWType3(UINT32 adr);
static UINT32 V9_Mul8GetRegWType4(UINT32 adr);
static UINT32 V9_Mul8GetRegWType5(UINT32 adr);
static UINT32 V9_Mul8GetRegWType6(UINT32 adr);


static TAcceContext938 gAcceCont938 = {
	       0,
		   0,
		   0,
		   0,
		   { 
	            V9_Mul8SetRegWType0,
	            V9_Mul8SetRegWType1,
	            V9_Mul8SetRegWType2,
	            V9_Mul8SetRegWType3,
	            V9_Mul8SetRegWType4,
	            V9_Mul8SetRegWType5,
                V9_Mul8SetRegWType6
            },
            { 
	            V9_Mul8GetRegWType0,
	            V9_Mul8GetRegWType1,
	            V9_Mul8GetRegWType2,
	            V9_Mul8GetRegWType3,
	            V9_Mul8GetRegWType4,
	            V9_Mul8GetRegWType5,
                V9_Mul8GetRegWType6
             }
};

//DOM-IGNORE-BEG
/*=============================================================================*/
/*                  basic read/write sram/register functions                   */
/*=============================================================================*/
//DOM-IGNORE-END

//DOM-IGNORE-BEG
/*=============================================================================*/
/*                  Multi8 read/write sram/register functions                  */
/*=============================================================================*/
//DOM-IGNORE-END
/*Set 8 bit value to register in mul8 bus type*/
#ifndef NCSC

/*Set value to register order type 0 in mul8 bus type*/
static void V9_Mul8SetRegWType0(UINT32 adr, UINT32 val)
{
    Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_SET_938;

    V9_MUL8SETREGADDR(adr);
  	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)val);

	gAcceCont938.stat = 0;

	PopIntEnFlag();
}

/*Set value to register order type 1 in mul8 bus type*/
static void V9_Mul8SetRegWType1(UINT32 adr, UINT32 val)
{
	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_SET_938;

    V9_MUL8SETREGADDR(adr);
  	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)val);

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 8));

	gAcceCont938.stat = 0;

	PopIntEnFlag();
}

/*Set value to register order type 2 in mul8 bus type*/
static void V9_Mul8SetRegWType2(UINT32 adr, UINT32 val)
{
	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_SET_938;

    V9_MUL8SETREGADDR(adr);
  	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 8));

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)val);

	gAcceCont938.stat = 0;

	PopIntEnFlag();
}

/*Set value to register order type 3 in mul8 bus type*/
static void V9_Mul8SetRegWType3(UINT32 adr, UINT32 val)
{
	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_SET_938;

    V9_MUL8SETREGADDR(adr);
  	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)val);

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 8));

	gAcceCont938.addr = adr + 2;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 2));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 16));

	gAcceCont938.stat = 0;

	PopIntEnFlag();
}

/*Set value to register order type 4 in mul8 bus type*/
static void V9_Mul8SetRegWType4(UINT32 adr, UINT32 val)
{
	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_SET_938;

    V9_MUL8SETREGADDR(adr);
  	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 16));

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 8));

	gAcceCont938.addr = adr + 2;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 2));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)val);

	gAcceCont938.stat = 0;

	PopIntEnFlag();
}

/*Set value to register order type 5 in mul8 bus type*/
static void V9_Mul8SetRegWType5(UINT32 adr, UINT32 val)
{
	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_SET_938;

    V9_MUL8SETREGADDR(adr);
  	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)val);

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 8));

	gAcceCont938.addr = adr + 2;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 2));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 16));

	gAcceCont938.addr = adr + 3;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 3));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 24));

	gAcceCont938.stat = 0;

	PopIntEnFlag();
}

/*Set value to register order type 6 in mul8 bus type*/
static void V9_Mul8SetRegWType6(UINT32 adr, UINT32 val)
{
	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_SET_938;

    V9_MUL8SETREGADDR(adr);
  	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 24));

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 16));

	gAcceCont938.addr = adr + 2;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 2));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)(val >> 8));

	gAcceCont938.addr = adr + 3;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 3));
	V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)val);

	gAcceCont938.stat = 0;

	PopIntEnFlag();
}

/*Get value from register order type 0 in mul8 bus type*/
static UINT32 V9_Mul8GetRegWType0(UINT32 adr)
{
	UINT32 val;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_GET_938;

    V9_MUL8SETREGADDR(adr);   
	val = V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );

	gAcceCont938.stat = 0;

	PopIntEnFlag();

	return val;
}

/*Get value from register order type 1 in mul8 bus type*/
static UINT32 V9_Mul8GetRegWType1(UINT32 adr)
{
	UINT32 val;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr + 1;
    gAcceCont938.stat = MUL8_REG_GET_938;

    V9_MUL8SETREGADDR(adr + 1);   
	val = V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)adr);
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );

	gAcceCont938.stat = 0;

	PopIntEnFlag();

	return val;
}

/*Get value from register order type 2 in mul8 bus type*/
static UINT32 V9_Mul8GetRegWType2(UINT32 adr)
{
	UINT32 val;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_GET_938;

    V9_MUL8SETREGADDR(adr);   
	val = V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );

	gAcceCont938.stat = 0;

	PopIntEnFlag();

	return val;
}

/*Get value from register order type 3 in mul8 bus type*/
static UINT32 V9_Mul8GetRegWType3(UINT32 adr)
{
	UINT32 val;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr + 2;
    gAcceCont938.stat = MUL8_REG_GET_938;

    V9_MUL8SETREGADDR(adr + 2);   
	val = V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)adr);
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );

	gAcceCont938.stat = 0;

	PopIntEnFlag();

	return val;
}

/*Get value from register order type 4 in mul8 bus type*/
static UINT32 V9_Mul8GetRegWType4(UINT32 adr)
{
	UINT32 val;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_GET_938;

    V9_MUL8SETREGADDR(adr);   
	val = V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 2;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 2));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );

	gAcceCont938.stat = 0;

	PopIntEnFlag();

	return val;
}

/*Get value from register order type 5 in mul8 bus type*/
static UINT32 V9_Mul8GetRegWType5(UINT32 adr)
{
	UINT32 val;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr + 3;
    gAcceCont938.stat = MUL8_REG_GET_938;

    V9_MUL8SETREGADDR(adr + 3);   
	val = V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 2;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 2));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)adr);
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );

	gAcceCont938.stat = 0;

	PopIntEnFlag();

	return val;
}

/*Get value from register order type 6 in mul8 bus type*/
static UINT32 V9_Mul8GetRegWType6(UINT32 adr)
{
	UINT32 val;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
    gAcceCont938.stat = MUL8_REG_GET_938;

    V9_MUL8SETREGADDR(adr);   
	val = V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 1;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 1));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 2;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 2));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );
	val <<= 8;

	gAcceCont938.addr = adr + 3;
	V9_HOSTSETADDR8(V9_MULTI8_ADDR_ADDR, (UINT8)(adr + 3));
	val += V9_HOSTGETVAL8( V9_MULTI8_ADDR_DATA );

	gAcceCont938.stat = 0;

	PopIntEnFlag();

	return val;
}


/*Set value to register according to order type in mul8 bus type*/
void V9_Mul8SetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType)
{
    gAcceCont938.pMul8SetRegFunc[regWType](adr, val);
}

/*Get value from register according order type in mul8 bus type*/
UINT32 V9_Mul8GetRegByWType(UINT32 adr, UINT8 regWType)
{
	UINT32 val;

    val = gAcceCont938.pMul8GetRegFunc[regWType](adr);

	return val;
}
#endif

/*Set data to sram in mul8 bus type*/
#ifndef NCSC

UINT32 V9_Mul8WriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize = size;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
	gAcceCont938.stat = MUL8_SRAM_WRITE_938;

	V9_MUL8WRITESRAMSETCONT(adr);

    gAcceCont938.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, *buf++ );
	}

	return actSize;
}

UINT32 V9_Mul8ReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize = size;

	Push2DisIntEnFlag();

	gAcceCont938.addr = adr;
	gAcceCont938.stat = MUL8_SRAM_READ_938;

	V9_MUL8WRITESRAMSETCONT(adr);

    gAcceCont938.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
        *buf++ = V9_HOSTGETVAL8(V9_MULTI8_ADDR_DATA);
	}

	return actSize;
}

#endif


/************************************************************************************/
/*register and sram access context store function if they are interrupted in process*/
/************************************************************************************/
void V9_StoreReg2SramCont(TAcceContext938* pAcceContext938)
{
	pAcceContext938->stat = gAcceCont938.stat;
    pAcceContext938->addr = gAcceCont938.addr;
}

/**************************************************************************************/
/*register and sram access context restore function if they are interrupted in process*/
/**************************************************************************************/
void V9_RestoreReg2SramCont(TAcceContext938* pAcceContext938)
{
	switch(pAcceContext938->stat)
	{
  	    case MUL8_REG_SET_938:
		case MUL8_REG_GET_938:
			V9_MUL8SETREGADDR(pAcceContext938->addr);
			break;

		case MUL8_SRAM_WRITE_938:
		case MUL8_SRAM_READ_938:
			V9_MUL8WRITESRAMSETCONT(pAcceContext938->addr);
			break;

		default:
			break;
	}
}

#endif  //V938CMD_DISABLE


