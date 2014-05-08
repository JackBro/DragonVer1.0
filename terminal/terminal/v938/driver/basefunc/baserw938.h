


#ifndef _TRM_BASERW938_H_
#define _TRM_BASERW938_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef void   BaseSetRegFunc938(UINT32 adr, UINT32 val);
typedef UINT32 BaseGetRegFunc938(UINT32 adr);

typedef struct tag_TAcceContext938
{
	UINT32                     addr;
	UINT8                      stat;
    UINT8                      resever0;
	UINT16                     resever1;
	BaseSetRegFunc938*         pMul8SetRegFunc[7];
	BaseGetRegFunc938*         pMul8GetRegFunc[7];
} TAcceContext938, *PTAcceContext938;

enum CHIPACCESSSTAenum938
{
	MUL8_REG_SET_938,
	MUL8_REG_GET_938,
	MUL8_SRAM_WRITE_938,
	MUL8_SRAM_READ_938
};

/*-------define V5 chip mapping address ---------*/
/*Seperate*/
/*used by register access*/


//Multi8, parrel

UINT32 V9_Mul8WriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V9_Mul8ReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
void   V9_Mul8SetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType);
UINT32 V9_Mul8GetRegByWType(UINT32 adr, UINT8 regWType);


void V9_StoreReg2SramCont(TAcceContext938* pAcceContext938);
void V9_RestoreReg2SramCont(TAcceContext938* pAcceContext938);


#define V9_MUL8SETREGADDR(adr) \
	    {\
		  V9_HOSTSETADDR8( V9_MULTI8_ADDR_ADDR, V9_REG_PAGE_NUM );\
		  V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)((adr) >> 8) );\
		  V9_HOSTSETADDR8( V9_MULTI8_ADDR_ADDR, (UINT8)(adr) );\
		}

#define V9_MUL8WRITESRAMSETCONT(adr) \
		{\
	      V9_HOSTSETADDR8( V9_MULTI8_ADDR_ADDR, V9_REG_PAGE_NUM );\
		  V9_HOSTSETVAL8( V9_MULTI8_ADDR_DATA, (UINT8)((adr) >> 8) );\
		  V9_HOSTSETADDR8( V9_MULTI8_ADDR_ADDR, (UINT8)(adr) );\
		}

#ifdef __cplusplus
}
#endif

#endif

