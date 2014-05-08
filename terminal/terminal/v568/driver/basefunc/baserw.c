
#include "basefunc.h"

#ifndef	V578CMD_DISABLE

#include "../../userapi/userbaserw.h"

#ifdef WIN32
#include "../../../s3c44box/s3c44box.h"
#endif

static void   V5_Mul16SetRegWType0(UINT32 adr, UINT32 val);
static void   V5_Mul16SetRegWType1(UINT32 adr, UINT32 val);
static void   V5_Mul16SetRegWType2(UINT32 adr, UINT32 val);
static void   V5_Mul16SetRegWType3(UINT32 adr, UINT32 val);
static void   V5_Mul16SetRegWType4(UINT32 adr, UINT32 val);
static void   V5_Mul16SetRegWType5(UINT32 adr, UINT32 val);
static void   V5_Mul16SetRegWType6(UINT32 adr, UINT32 val);
static UINT32 V5_Mul16GetRegWType0(UINT32 adr);
static UINT32 V5_Mul16GetRegWType1(UINT32 adr);
static UINT32 V5_Mul16GetRegWType2(UINT32 adr);
static UINT32 V5_Mul16GetRegWType3(UINT32 adr);
static UINT32 V5_Mul16GetRegWType4(UINT32 adr);
static UINT32 V5_Mul16GetRegWType5(UINT32 adr);
static UINT32 V5_Mul16GetRegWType6(UINT32 adr);

static void   V5_Mul8SetRegWType0(UINT32 adr, UINT32 val);
static void   V5_Mul8SetRegWType1(UINT32 adr, UINT32 val);
static void   V5_Mul8SetRegWType2(UINT32 adr, UINT32 val);
static void   V5_Mul8SetRegWType3(UINT32 adr, UINT32 val);
static void   V5_Mul8SetRegWType4(UINT32 adr, UINT32 val);
static void   V5_Mul8SetRegWType5(UINT32 adr, UINT32 val);
static void   V5_Mul8SetRegWType6(UINT32 adr, UINT32 val);
static UINT32 V5_Mul8GetRegWType0(UINT32 adr);
static UINT32 V5_Mul8GetRegWType1(UINT32 adr);
static UINT32 V5_Mul8GetRegWType2(UINT32 adr);
static UINT32 V5_Mul8GetRegWType3(UINT32 adr);
static UINT32 V5_Mul8GetRegWType4(UINT32 adr);
static UINT32 V5_Mul8GetRegWType5(UINT32 adr);
static UINT32 V5_Mul8GetRegWType6(UINT32 adr);

static void   V5_SepSetRegWType0(UINT32 adr, UINT32 val);
static void   V5_SepSetRegWType1(UINT32 adr, UINT32 val);
static void   V5_SepSetRegWType2(UINT32 adr, UINT32 val);
static void   V5_SepSetRegWType3(UINT32 adr, UINT32 val);
static void   V5_SepSetRegWType4(UINT32 adr, UINT32 val);
static void   V5_SepSetRegWType5(UINT32 adr, UINT32 val);
static void   V5_SepSetRegWType6(UINT32 adr, UINT32 val);
static UINT32 V5_SepGetRegWType0(UINT32 adr);
static UINT32 V5_SepGetRegWType1(UINT32 adr);
static UINT32 V5_SepGetRegWType2(UINT32 adr);
static UINT32 V5_SepGetRegWType3(UINT32 adr);
static UINT32 V5_SepGetRegWType4(UINT32 adr);
static UINT32 V5_SepGetRegWType5(UINT32 adr);
static UINT32 V5_SepGetRegWType6(UINT32 adr);

static TAcceContext gAcceCont = {
	       0,
		   0,
		   0,
		   0,
		   { 
	           V5_Mul16SetRegWType0,
	           V5_Mul16SetRegWType1,
	           V5_Mul16SetRegWType2,
	           V5_Mul16SetRegWType3,
	           V5_Mul16SetRegWType4,
	           V5_Mul16SetRegWType5,
               V5_Mul16SetRegWType6
            },
            { 
	            V5_Mul16GetRegWType0,
	            V5_Mul16GetRegWType1,
	            V5_Mul16GetRegWType2,
	            V5_Mul16GetRegWType3,
	            V5_Mul16GetRegWType4,
	            V5_Mul16GetRegWType5,
                V5_Mul16GetRegWType6
            },

			{ 
	            V5_Mul8SetRegWType0,
	            V5_Mul8SetRegWType1,
	            V5_Mul8SetRegWType2,
	            V5_Mul8SetRegWType3,
	            V5_Mul8SetRegWType4,
	            V5_Mul8SetRegWType5,
                V5_Mul8SetRegWType6
            },
            { 
	            V5_Mul8GetRegWType0,
	            V5_Mul8GetRegWType1,
	            V5_Mul8GetRegWType2,
	            V5_Mul8GetRegWType3,
	            V5_Mul8GetRegWType4,
	            V5_Mul8GetRegWType5,
                V5_Mul8GetRegWType6
             },
             
			{ 
	            V5_SepSetRegWType0,
	            V5_SepSetRegWType1,
	            V5_SepSetRegWType2,
	            V5_SepSetRegWType3,
	            V5_SepSetRegWType4,
	            V5_SepSetRegWType5,
                V5_SepSetRegWType6
            },
            { 
	            V5_SepGetRegWType0,
	            V5_SepGetRegWType1,
	            V5_SepGetRegWType2,
	            V5_SepGetRegWType3,
	            V5_SepGetRegWType4,
	            V5_SepGetRegWType5,
                V5_SepGetRegWType6
             }
};


/********************************************************************************
  Description:
	Delay time
  Parameters:
	time : delay count(ms)
  Note:
  Returns:
  Remarks:
*********************************************************************************/

//DOM-IGNORE-BEG
/*=============================================================================*/
/*                  basic read/write sram/register functions                   */
/*=============================================================================*/
//DOM-IGNORE-END

//DOM-IGNORE-BEG
/*=============================================================================*/
/*                  Multi16 read/write sram/register functions                 */
/*=============================================================================*/
//DOM-IGNORE-END
/*Set 8 bit value to register in mul16 bus type*/
#ifndef NCSC
void V5_Mul16SetReg(UINT32 adr, UINT8 val)
{
	UINT16 uTemp;
    
	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = MUL16_REG_SET;

	V5_MUL16SETREGADDR(adr);

	uTemp = V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA );
	if((adr & 0x1) == 0)
	{
		uTemp = (uTemp&0xff00)|val;
	}
	else
	{
		uTemp = (uTemp&0xff)|(val<<8);
	}

	/*host send data*/
	V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, uTemp );

    gAcceCont.stat = 0;

	PopIntEnFlag();
}

/*Get 8 bit value from register in mul16 bus type*/
UINT8 V5_Mul16GetReg(UINT32 adr)
{
	UINT8 regVal;

	Push2DisIntEnFlag();
    
	gAcceCont.addr = adr;
	gAcceCont.stat = MUL16_REG_GET;

	V5_MUL16SETREGADDR(adr);
		
    /*host get data*/
	if((adr & 0x1) == 0)
	{
		regVal = (UINT8)V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA );
	}
	else
	{
        regVal = (UINT8)( V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA ) >> 8 );
	}

	gAcceCont.stat = 0;

	PopIntEnFlag();

	return regVal;
}

/*Set 16 bit value to register in mul16 bus type*/
void V5_Mul16SetReg16(UINT32 adr, UINT16 val)
{
	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = MUL16_REG_SET_16;

	V5_MUL16SETREGADDR(adr);

	/*host send data*/
	V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, val );

    gAcceCont.stat = 0;

	PopIntEnFlag();
}

/*Get 16 bit value from register in mul16 bus type*/
UINT16 V5_Mul16GetReg16(UINT32 adr)
{
	UINT16 regVal;

	Push2DisIntEnFlag();
    
	gAcceCont.addr = adr;
	gAcceCont.stat = MUL16_REG_GET_16;

	V5_MUL16SETREGADDR(adr);
		
    /*host get data*/
	regVal = V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA );

	gAcceCont.stat = 0;

	PopIntEnFlag();

	return regVal;
}
#endif


/*Set value to register order type 0 in mul16 bus type*/
static void V5_Mul16SetRegWType0(UINT32 adr, UINT32 val)
{
    V5_Mul16SetReg(adr, (UINT8)val);
}

/*Set value to register order type 1 in mul16 bus type*/
static void V5_Mul16SetRegWType1(UINT32 adr, UINT32 val)
{
	if(adr & 0x1)
	{
        V5_Mul16SetReg(adr, (UINT8)val);
		V5_Mul16SetReg(adr + 1, (UINT8)(val >> 8));
	}
	else
	{
        V5_Mul16SetReg16(adr, (UINT16)val);
	}
}

/*Set value to register order type 2 in mul16 bus type*/
static void V5_Mul16SetRegWType2(UINT32 adr, UINT32 val)
{
	UINT16 tmpVal = (UINT16)(((val >> 8) & 0xff) | ((val << 8) & 0xff00));

	if(adr & 0x1)
	{
        V5_Mul16SetReg(adr, (UINT8)(val >> 8));
		V5_Mul16SetReg(adr + 1, (UINT8)val);
	}
	else
	{
        V5_Mul16SetReg16(adr, (UINT16)tmpVal);
	}
}

/*Set value to register order type 3 in mul16 bus type*/
static void V5_Mul16SetRegWType3(UINT32 adr, UINT32 val)
{
	if(adr & 0x1)
	{
        V5_Mul16SetReg(adr, (UINT8)val);
	    V5_Mul16SetReg16(adr + 1, (UINT16)(val >> 8));
	}
	else
	{
        V5_Mul16SetReg16(adr, (UINT16)val);
	    V5_Mul16SetReg(adr + 2, (UINT8)(val >> 16));
	}
}

/*Set value to register order type 4 in mul16 bus type*/
static void V5_Mul16SetRegWType4(UINT32 adr, UINT32 val)
{
	UINT16 tmpVal;

	if(adr & 0x1)
	{
		tmpVal = (UINT16)(((val >> 8) & 0xff) | ((val << 8) & 0xff00));
        V5_Mul16SetReg(adr, (UINT8)(val >> 16) );
	    V5_Mul16SetReg16(adr + 1, tmpVal);
	}
	else
	{
		tmpVal = (UINT16)(((val >> 16) & 0xff) | (val & 0xff00));
        V5_Mul16SetReg16(adr, (UINT16)tmpVal);
	    V5_Mul16SetReg(adr + 2, (UINT8)(val));
	}
}

/*Set value to register order type 5 in mul16 bus type*/
static void V5_Mul16SetRegWType5(UINT32 adr, UINT32 val)
{
	if(adr & 0x1)
	{
		V5_Mul16SetReg(adr, (UINT8)val);
        V5_Mul16SetReg16(adr + 1, (UINT16)(val >> 8));
	    V5_Mul16SetReg(adr + 3, (UINT8)(val >> 24));
	}
	else
	{
        V5_Mul16SetReg16(adr, (UINT16)val);
	    V5_Mul16SetReg16(adr + 2, (UINT16)(val >> 16));
	}
}

/*Set value to register order type 6 in mul16 bus type*/
static void V5_Mul16SetRegWType6(UINT32 adr, UINT32 val)
{
	UINT16 tmpVal;

	if(adr & 0x1)
	{
        V5_Mul16SetReg(adr, (UINT8)(val >> 24));
		tmpVal = (UINT16)(((val >> 16) & 0xff) | (val & 0xff00));
        V5_Mul16SetReg16(adr + 1, tmpVal);
		V5_Mul16SetReg(adr + 3, (UINT8)val);
	}
	else
	{
		tmpVal = (UINT16)(((val >> 24) & 0xff) | ((val >> 8) & 0xff00));
        V5_Mul16SetReg16(adr, tmpVal);

	    tmpVal = (UINT16)(((val >> 8) & 0xff) | ((val << 8) & 0xff00));
	    V5_Mul16SetReg16(adr + 2, tmpVal);
	}
}

/*Get value from register order type 0 in mul16 bus type*/
static UINT32 V5_Mul16GetRegWType0(UINT32 adr)
{
	UINT32 val;

    val = V5_Mul16GetReg(adr);

	return val;
}

/*Get value from register order type 1 in mul16 bus type*/
static UINT32 V5_Mul16GetRegWType1(UINT32 adr)
{
	UINT32 val1, val2, val;

	if(adr & 0x1)
	{
        val1 = V5_Mul16GetReg(adr);
		val2 = V5_Mul16GetReg(adr + 1);

		val = (val1 & 0xff) | ((val2 << 8) & 0xff00);
	}
	else
	{
		val = V5_Mul16GetReg16(adr);
	}

	return val;
}

/*Get value from register order type 2 in mul16 bus type*/
static UINT32 V5_Mul16GetRegWType2(UINT32 adr)
{
	UINT32 val1, val2, val;

	if(adr & 0x1)
	{
        val1 = V5_Mul16GetReg(adr);
		val2 = V5_Mul16GetReg(adr + 1);

		val  = ((val1 << 8) & 0xff00) | (val2 & 0xff);
	}
	else
	{
		 val1 = V5_Mul16GetReg16(adr);

		 val  = ((val1 >> 8) & 0xff) | ((val1 << 8) & 0xff00); 
	}   
    
	return val;
}

/*Get value from register order type 3 in mul16 bus type*/
static UINT32 V5_Mul16GetRegWType3(UINT32 adr)
{
	UINT32 val1, val2, val;

	if(adr & 0x1)
	{
        val1 = V5_Mul16GetReg(adr);
	    val2 = V5_Mul16GetReg16(adr + 1);

		val  = (val1 & 0xff) | ((val2 << 8) & 0xffff00);
	}
	else
	{
        val1 = V5_Mul16GetReg16(adr);
	    val2 = V5_Mul16GetReg(adr + 2);

        val  = (val1 & 0xffff) | ((val2 << 16) & 0xff0000);
	}

	return val;
}

/*Get value from register order type 4 in mul16 bus type*/
static UINT32 V5_Mul16GetRegWType4(UINT32 adr)
{
	UINT32 val1, val2, val;

	if(adr & 0x1)
	{
        val1 = V5_Mul16GetReg(adr);
	    val2 = V5_Mul16GetReg16(adr + 1);

		val  = ((val1 << 16) & 0xff0000) | ((val2 << 8) & 0xff00) | ((val2 >> 8) & 0xff);
	}
	else
	{
        val1 = V5_Mul16GetReg16(adr);
	    val2 = V5_Mul16GetReg(adr + 2);

		val  = ((val1 << 16) & 0xff0000) | (val1 & 0xff00) | (val2 & 0xff);
	}
    
	return val;
}

/*Get value from register order type 5 in mul16 bus type*/
static UINT32 V5_Mul16GetRegWType5(UINT32 adr)
{
	UINT32 val1, val2, val3, val;

	if(adr & 0x1)
	{
        val1 = V5_Mul16GetReg(adr);
		val2 = V5_Mul16GetReg16(adr +1);
	    val3 = V5_Mul16GetReg(adr + 3);

		val = (val1 & 0xff) | ((val2 << 8) & 0xffff00) | ((val3 << 24) & 0xff000000);
	}
	else
	{
        val1 = V5_Mul16GetReg16(adr);
	    val2 = V5_Mul16GetReg16(adr + 2);

		val  = (val1 & 0xffff) | ((val2 << 16) & 0xffff0000);
	}

	return val;
}

/*Get value from register order type 6 in mul16 bus type*/
static UINT32 V5_Mul16GetRegWType6(UINT32 adr)
{
	UINT32 val1, val2, val3, val;

	if(adr & 0x1)
	{
        val1 = V5_Mul16GetReg(adr);
		val2 = V5_Mul16GetReg16(adr +1);
	    val3 = V5_Mul16GetReg(adr + 3);

		val = ( (val1 << 24) & 0xff000000) | ((val2 << 16) & 0xff0000) | (val2 & 0xff00) | (val3 & 0xff);
	}
	else
	{
        val1 = V5_Mul16GetReg16(adr);
	    val2 = V5_Mul16GetReg16(adr + 2);

		val  = ((val1 << 24) & 0xff000000) | ((val1 << 8) & 0xff0000) | ((val2 << 8) & 0xff00) | ((val2 >> 8) & 0xff);
	}
    
	return val;
}


/*Set value to register according to order type in mul16 bus type*/
void V5_Mul16SetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType)
{
    gAcceCont.pMul16SetRegFunc[regWType](adr, val);
}

/*Get value from register according to order type in mul16 bus type*/
UINT32 V5_Mul16GetRegByWType(UINT32 adr, UINT8 regWType)
{
	UINT32 val;

    val = gAcceCont.pMul16GetRegFunc[regWType](adr);

	return val;
}

/*Write data to sram in mul16 bus type*/
#ifndef NCSC
UINT32 V5_Mul16WriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT16 uTemp;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;	
	actSize = size << 1;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();
    
	gAcceCont.addr = adr;
	gAcceCont.stat = MUL16_SRAM_WRITE;

	V5_MUL16WRITESRAMSETCONT(adr);

	gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		uTemp = *buf++;
		uTemp += ((UINT16)(*buf++))<<8;
		V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, uTemp );
	}

	return actSize;
}

/* Write SRAM reverse sequence between two bytes;  for example: 0, 1 ,2 ,3  ---->  1 ,0, 3, 2 */
UINT32 V5_Mul16WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size)
{
    UINT32 actSize;
	UINT16 uTemp;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;	
	actSize = size << 1;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();
    
	gAcceCont.addr = adr;
	gAcceCont.stat = MUL16_SRAM_WRITE;

	V5_MUL16WRITESRAMSETCONT(adr);

	gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		uTemp = *buf++;
		uTemp = (uTemp << 8) + (*buf++);
		V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, uTemp );
	}

	return actSize;
}

UINT32 V5_Mul16WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 2;	
	actSize = size << 2;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();
    
	gAcceCont.addr = adr;
	gAcceCont.stat = MUL16_SRAM_WRITE;

	V5_MUL16WRITESRAMSETCONT(adr);

	gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, (UINT16)dat);
		V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, (UINT16)(dat >> 16));
	}

	return actSize;
}

/*Get data from sram in mul16 bus type*/
UINT32 V5_Mul16ReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT16 uTemp;

	if( adr > V5_SRAM_TOTALSIZE )
	{
		return 0;
	}
	if( size > (V5_SRAM_TOTALSIZE - adr) )
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;	
	actSize = size << 1;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
    gAcceCont.stat = MUL16_SRAM_READ;

	V5_MUL16READSRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		uTemp = V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA );
		*buf++ = (UINT8)uTemp;
		*buf++ = (UINT8)(uTemp>>8);
	}

	return actSize;
}
#endif

//DOM-IGNORE-BEG
/*=============================================================================*/
/*                  Multi8 read/write sram/register functions                  */
/*=============================================================================*/
//DOM-IGNORE-END
/*Set 8 bit value to register in mul8 bus type*/
#ifndef NCSC
void V5_Mul8SetReg(UINT32 adr, UINT8 val)
{
	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
    gAcceCont.stat = MUL8_REG_SET;

    V5_MUL8SETREGADDR(adr);

	V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, val);

	gAcceCont.stat = 0;

	PopIntEnFlag();
}

/*Get 8 bit value from register in mul8 bus type*/
UINT8 V5_Mul8GetReg(UINT32 adr)
{
	UINT8 regVal;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
    gAcceCont.stat = MUL8_REG_GET;

    V5_MUL8SETREGADDR(adr);
   
	regVal = V5_HOSTGETVAL8( V5_MULTI8_ADDR_DATA );

	gAcceCont.stat = 0;

	PopIntEnFlag();

	return regVal;
}
#endif

/*Set value to register order type 0 in mul8 bus type*/
static void V5_Mul8SetRegWType0(UINT32 adr, UINT32 val)
{
    V5_Mul8SetReg(adr, (UINT8)val);
}

/*Set value to register order type 1 in mul8 bus type*/
static void V5_Mul8SetRegWType1(UINT32 adr, UINT32 val)
{
    V5_Mul8SetReg(adr, (UINT8)val);
	V5_Mul8SetReg(adr + 1, (UINT8)(val >> 8));
}

/*Set value to register order type 2 in mul8 bus type*/
static void V5_Mul8SetRegWType2(UINT32 adr, UINT32 val)
{
    V5_Mul8SetReg(adr, (UINT8)(val >> 8));
	V5_Mul8SetReg(adr + 1, (UINT8)val);
}

/*Set value to register order type 3 in mul8 bus type*/
static void V5_Mul8SetRegWType3(UINT32 adr, UINT32 val)
{
    V5_Mul8SetReg(adr, (UINT8)val);
	V5_Mul8SetReg(adr + 1, (UINT8)(val >> 8));
	V5_Mul8SetReg(adr + 2, (UINT8)(val >> 16));
}

/*Set value to register order type 4 in mul8 bus type*/
static void V5_Mul8SetRegWType4(UINT32 adr, UINT32 val)
{
    V5_Mul8SetReg(adr, (UINT8)(val >> 16));
	V5_Mul8SetReg(adr + 1, (UINT8)(val >> 8));
	V5_Mul8SetReg(adr + 2, (UINT8)val);
}

/*Set value to register order type 5 in mul8 bus type*/
static void V5_Mul8SetRegWType5(UINT32 adr, UINT32 val)
{
    V5_Mul8SetReg(adr, (UINT8)val);
	V5_Mul8SetReg(adr + 1, (UINT8)(val >> 8));
	V5_Mul8SetReg(adr + 2, (UINT8)(val >> 16));
	V5_Mul8SetReg(adr + 3, (UINT8)(val >> 24));
}

/*Set value to register order type 6 in mul8 bus type*/
static void V5_Mul8SetRegWType6(UINT32 adr, UINT32 val)
{
    V5_Mul8SetReg(adr, (UINT8)(val >> 24));
	V5_Mul8SetReg(adr + 1, (UINT8)(val >> 16));
	V5_Mul8SetReg(adr + 2, (UINT8)(val >> 8));
	V5_Mul8SetReg(adr + 3, (UINT8)val);
}

/*Get value from register order type 0 in mul8 bus type*/
static UINT32 V5_Mul8GetRegWType0(UINT32 adr)
{
	UINT32 val;

	val = V5_Mul8GetReg(adr);

	return val;
}

/*Get value from register order type 1 in mul8 bus type*/
static UINT32 V5_Mul8GetRegWType1(UINT32 adr)
{
	UINT32 val, val1;

	val = V5_Mul8GetReg(adr);
	val1 = V5_Mul8GetReg(adr + 1);

	return (val & 0xff) | ((val1 << 8) & 0xff00);
}

/*Get value from register order type 2 in mul8 bus type*/
static UINT32 V5_Mul8GetRegWType2(UINT32 adr)
{
	UINT32 val, val1;

	val = V5_Mul8GetReg(adr);
	val1 = V5_Mul8GetReg(adr + 1);

	return ((val << 8) & 0xff00) | (val1 & 0xff);
}

/*Get value from register order type 3 in mul8 bus type*/
static UINT32 V5_Mul8GetRegWType3(UINT32 adr)
{
	UINT32 val, val1, val2;

	val = V5_Mul8GetReg(adr);
	val1 = V5_Mul8GetReg(adr + 1);
	val2 = V5_Mul8GetReg(adr + 2);

	return (val & 0xff) | ((val1 << 8) & 0xff00) | ((val2 << 16) & 0xff0000);
}

/*Get value from register order type 4 in mul8 bus type*/
static UINT32 V5_Mul8GetRegWType4(UINT32 adr)
{
	UINT32 val, val1, val2;

	val = V5_Mul8GetReg(adr);
	val1 = V5_Mul8GetReg(adr + 1);
	val2 = V5_Mul8GetReg(adr + 2);

	return ((val << 16) & 0xff0000) | ((val1 << 8) & 0xff00) | (val2 & 0xff);
}

/*Get value from register order type 5 in mul8 bus type*/
static UINT32 V5_Mul8GetRegWType5(UINT32 adr)
{
	UINT32 val, val1, val2, val3;

	val = V5_Mul8GetReg(adr);
	val1 = V5_Mul8GetReg(adr + 1);
	val2 = V5_Mul8GetReg(adr + 2);
	val3 = V5_Mul8GetReg(adr + 3);

	return (val & 0xff) | ((val1 << 8) & 0xff00) | ((val2 << 16) & 0xff0000) | ((val2 << 24) & 0xff000000);
}

/*Get value from register order type 6 in mul8 bus type*/
static UINT32 V5_Mul8GetRegWType6(UINT32 adr)
{
	UINT32 val, val1, val2, val3;

	val = V5_Mul8GetReg(adr);
	val1 = V5_Mul8GetReg(adr + 1);
	val2 = V5_Mul8GetReg(adr + 2);
	val3 = V5_Mul8GetReg(adr + 3);

	return ((val << 24) & 0xff000000) | ((val1 << 16) & 0xff0000) | ((val2 << 8) & 0xff00) | (val3 & 0xff);
}


/*Set value to register according to order type in mul8 bus type*/
void V5_Mul8SetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType)
{
    gAcceCont.pMul8SetRegFunc[regWType](adr, val);
}

/*Get value from register according order type in mul8 bus type*/
UINT32 V5_Mul8GetRegByWType(UINT32 adr, UINT8 regWType)
{
	UINT32 val;

    val = gAcceCont.pMul8GetRegFunc[regWType](adr);

	return val;
}

/*Set data to sram in mul8 bus type*/
#ifndef NCSC

UINT32 V5_Mul8WriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = MUL8_SRAM_WRITE;

	V5_MUL8WRITESRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, *buf++ );
	}

	return actSize;
}

/* Write SRAM reverse sequence between two bytes;  for example: 0, 1 ,2 ,3  ---->  1 ,0, 3, 2 */
UINT32 V5_Mul8WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;
	actSize = size << 1;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = MUL8_SRAM_WRITE;

	V5_MUL8WRITESRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V5_HOSTSETVAL8(V5_MULTI8_ADDR_DATA, *(buf + 2 * size +1));
		V5_HOSTSETVAL8(V5_MULTI8_ADDR_DATA, *(buf + 2 * size));
	}

	return actSize;
}

UINT32 V5_Mul8WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 2;
	actSize = size << 2;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = MUL8_SRAM_WRITE;

	V5_MUL8WRITESRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)dat);
		V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)(dat >> 8));
		V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)(dat >> 16));
		V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)(dat >> 24));
	}

	return actSize;
}

/*Get data from sram in mul8 bus type*/
UINT32 V5_Mul8ReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
    gAcceCont.stat = MUL8_SRAM_READ;

	V5_MUL8READSRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		*buf++ = V5_HOSTGETVAL8( V5_MULTI8_ADDR_DATA );
	}

	return actSize;
}
#endif

//DOM-IGNORE-BEG
/*=============================================================================*/
/*                  Seperate read/write sram/register functions                */
/*=============================================================================*/
//DOM-IGNORE-END
/*Set 8 bit value to register in sep bus type*/
#ifndef NCSC
void V5_SepSetReg(UINT32 adr, UINT8 val)
{
	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = SEP_REG_SET;

	V5_SEPSETREGADDR(adr);

	V5_HOSTSETVAL8( ( V5_INTER_ADDR_REGUIA + (UINT32)(adr&0xff) ), val );

	gAcceCont.stat = 0;

	PopIntEnFlag();
}

/*Get 8 bit value from register in sep bus type*/
UINT8 V5_SepGetReg(UINT32 adr)
{
	UINT8 regVal;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = SEP_REG_GET;

    V5_SEPSETREGADDR(adr);

	regVal = V5_HOSTGETVAL8( ( V5_INTER_ADDR_REGUIA + (UINT32)(adr&0xff) ) );

	gAcceCont.stat = 0;

	PopIntEnFlag();

	return regVal;
}
#endif

/*Set value to register order type 0 in sep bus type*/
static void V5_SepSetRegWType0(UINT32 adr, UINT32 val)
{
    V5_SepSetReg(adr, (UINT8)val);
}

/*Set value to register order type 1 in sep bus type*/
static void V5_SepSetRegWType1(UINT32 adr, UINT32 val)
{
    V5_SepSetReg(adr, (UINT8)val);
	V5_SepSetReg(adr + 1, (UINT8)(val >> 8));
}

/*Set value to register order type 2 in sep bus type*/
static void V5_SepSetRegWType2(UINT32 adr, UINT32 val)
{
    V5_SepSetReg(adr, (UINT8)(val >> 8));
	V5_SepSetReg(adr + 1, (UINT8)val);
}

/*Set value to register order type 3 in sep bus type*/
static void V5_SepSetRegWType3(UINT32 adr, UINT32 val)
{
    V5_SepSetReg(adr, (UINT8)val);
	V5_SepSetReg(adr + 1, (UINT8)(val >> 8));
	V5_SepSetReg(adr + 2, (UINT8)(val >> 16));
}

/*Set value to register order type 4 in sep bus type*/
static void V5_SepSetRegWType4(UINT32 adr, UINT32 val)
{
    V5_SepSetReg(adr, (UINT8)(val >> 16));
	V5_SepSetReg(adr + 1, (UINT8)(val >> 8));
	V5_SepSetReg(adr + 2, (UINT8)val);
}

/*Set value to register order type 5 in sep bus type*/
static void V5_SepSetRegWType5(UINT32 adr, UINT32 val)
{
    V5_SepSetReg(adr, (UINT8)val);
	V5_SepSetReg(adr + 1, (UINT8)(val >> 8));
	V5_SepSetReg(adr + 2, (UINT8)(val >> 16));
	V5_SepSetReg(adr + 3, (UINT8)(val >> 24));
}

/*Set value to register order type 6 in sep bus type*/
static void V5_SepSetRegWType6(UINT32 adr, UINT32 val)
{
    V5_SepSetReg(adr, (UINT8)(val >> 24));
	V5_SepSetReg(adr + 1, (UINT8)(val >> 16));
	V5_SepSetReg(adr + 2, (UINT8)(val >> 8));
	V5_SepSetReg(adr + 3, (UINT8)val);
}

/*Get value from register order type 0 in sep bus type*/
static UINT32 V5_SepGetRegWType0(UINT32 adr)
{
	UINT32 val;

	val = V5_SepGetReg(adr);

	return val;
}

/*Get value from register order type 1 in sep bus type*/
static UINT32 V5_SepGetRegWType1(UINT32 adr)
{
	UINT32 val, val1;

	val = V5_SepGetReg(adr);
	val1 = V5_SepGetReg(adr + 1);

	return (val & 0xff) | ((val1 << 8) & 0xff00);
}

/*Get value from register order type 2 in sep bus type*/
static UINT32 V5_SepGetRegWType2(UINT32 adr)
{
	UINT32 val, val1;

	val = V5_SepGetReg(adr);
	val1 = V5_SepGetReg(adr + 1);

	return ((val << 8) & 0xff00) | (val1 & 0xff);
}

/*Get value from register order type 3 in sep bus type*/
static UINT32 V5_SepGetRegWType3(UINT32 adr)
{
	UINT32 val, val1, val2;

	val = V5_SepGetReg(adr);
	val1 = V5_SepGetReg(adr + 1);
	val2 = V5_SepGetReg(adr + 2);

	return (val & 0xff) | ((val1 << 8) & 0xff00) | ((val2 << 16) & 0xff0000);
}

/*Get value from register order type 4 in sep bus type*/
static UINT32 V5_SepGetRegWType4(UINT32 adr)
{
	UINT32 val, val1, val2;

	val = V5_SepGetReg(adr);
	val1 = V5_SepGetReg(adr + 1);
	val2 = V5_SepGetReg(adr + 2);

	return ((val << 16) & 0xff0000) | ((val1 << 8) & 0xff00) | (val2 & 0xff);
}

/*Get value from register order type 5 in sep bus type*/
static UINT32 V5_SepGetRegWType5(UINT32 adr)
{
	UINT32 val, val1, val2, val3;

	val = V5_SepGetReg(adr);
	val1 = V5_SepGetReg(adr + 1);
	val2 = V5_SepGetReg(adr + 2);
	val3 = V5_SepGetReg(adr + 3);

	return (val & 0xff) | ((val1 << 8) & 0xff00) | ((val2 << 16) & 0xff0000) | ((val2 << 24) & 0xff000000);
}

/*Get value from register order type 6 in sep bus type*/
static UINT32 V5_SepGetRegWType6(UINT32 adr)
{
	UINT32 val, val1, val2, val3;

	val = V5_SepGetReg(adr);
	val1 = V5_SepGetReg(adr + 1);
	val2 = V5_SepGetReg(adr + 2);
	val3 = V5_SepGetReg(adr + 3);

	return ((val << 24) & 0xff000000) | ((val1 << 16) & 0xff0000) | ((val2 << 8) & 0xff00) | (val3 & 0xff);
}

/*Set value to register according to order type in Sep bus type*/
void V5_SepSetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType)
{
    gAcceCont.pSepSetRegFunc[regWType](adr, val);
}

/*Get value from register according order type in Sepbus type*/
UINT32 V5_SepGetRegByWType(UINT32 adr, UINT8 regWType)
{
	UINT32 val;

    val = gAcceCont.pSepGetRegFunc[regWType](adr);

	return val;
}

/*Set data to sram in sep bus type*/
#ifndef NCSC
UINT32 V5_SepWriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = SEP_SRAM_WRITE;

	V5_SEPWRITESRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V5_HOSTSETVAL8( V5_HOST_ADDR_MEMDATA, *buf++ );
	}

	return actSize;
}

/* Write SRAM reverse sequence between two bytes;  for example: 0, 1 ,2 ,3  ---->  1 ,0, 3, 2 */
UINT32 V5_SepWriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;
	actSize = size << 1;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = SEP_SRAM_WRITE;

	V5_SEPWRITESRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V5_HOSTSETVAL8(V5_HOST_ADDR_MEMDATA, *(buf + 2 * size +1));
		V5_HOSTSETVAL8(V5_HOST_ADDR_MEMDATA, *(buf + 2 * size));
	}

	return actSize;
}

UINT32 V5_SepWriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

    size >>= 2;
	actSize = size << 2;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = SEP_SRAM_WRITE;

	V5_SEPWRITESRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		V5_HOSTSETVAL8( V5_HOST_ADDR_MEMDATA, (UINT8)dat);
		V5_HOSTSETVAL8( V5_HOST_ADDR_MEMDATA, (UINT8)(dat >> 8));
		V5_HOSTSETVAL8( V5_HOST_ADDR_MEMDATA, (UINT8)(dat >> 16));
		V5_HOSTSETVAL8( V5_HOST_ADDR_MEMDATA, (UINT8)(dat >> 24));
	}

	return actSize;
}

/*Get data from sram in sep bus type*/
UINT32 V5_SepReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}

	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	/*set sram start address*/
	adr += V5_SRAM_BASE;

	Push2DisIntEnFlag();

	gAcceCont.addr = adr;
	gAcceCont.stat = SEP_SRAM_READ;

	V5_SEPREADSRAMSETCONT(adr);

    gAcceCont.stat = 0;

	PopIntEnFlag();

	while(size--)
	{
		*buf++ = V5_HOSTGETVAL8(V5_HOST_ADDR_MEMDATA);
	}

	return actSize;
} 
#endif

/******************************************************************************/
/*register and sram access context store function if they are interrupted in process*/
/******************************************************************************/
void V5_StoreReg2SramCont(TAcceContext* pAcceContext)
{
	pAcceContext->stat = gAcceCont.stat;
    pAcceContext->addr = gAcceCont.addr;
}

/******************************************************************************/
/*register and sram access context restore function if they are interrupted in process*/
/******************************************************************************/
void V5_RestoreReg2SramCont(TAcceContext* pAcceContext)
{
	switch(pAcceContext->stat)
	{
  	    case SEP_REG_SET:
		case SEP_REG_GET:
			V5_SEPSETREGADDR(pAcceContext->addr);
			break;

		case SEP_SRAM_WRITE:
			V5_SEPWRITESRAMSETCONT(pAcceContext->addr);
			break;

		case SEP_SRAM_READ:
			V5_SEPREADSRAMSETCONT(pAcceContext->addr);
			break;

		case MUL8_REG_SET:
		case MUL8_REG_GET:
			V5_MUL8SETREGADDR(pAcceContext->addr);
			break;

		case MUL8_SRAM_WRITE:
			V5_MUL8WRITESRAMSETCONT(pAcceContext->addr);
			break;
		
		case MUL8_SRAM_READ:
			V5_MUL8READSRAMSETCONT(pAcceContext->addr);
			break;

		case MUL16_REG_SET:
		case MUL16_REG_GET:
		case MUL16_REG_SET_16:
		case MUL16_REG_GET_16:
			V5_MUL16SETREGADDR(pAcceContext->addr);
			break;

		case MUL16_SRAM_WRITE:
			V5_MUL16WRITESRAMSETCONT(pAcceContext->addr);
			break;

		case MUL16_SRAM_READ:
			V5_MUL16READSRAMSETCONT(pAcceContext->addr);
			break;

		default:
			break;
	}
}

#endif  //V578CMD_DISABLE


