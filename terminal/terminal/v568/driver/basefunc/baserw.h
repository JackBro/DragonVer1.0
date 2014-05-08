


#ifndef _TRM_BASERW_H_
#define _TRM_BASERW_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef void   BaseSetRegFunc(UINT32 adr, UINT32 val);
typedef UINT32 BaseGetRegFunc(UINT32 adr);

typedef struct tag_TAcceContext
{
	UINT32              addr;
	UINT8               stat;
    UINT8               resever0;
	UINT16              resever1;
	BaseSetRegFunc*         pMul16SetRegFunc[7];
	BaseGetRegFunc*         pMul16GetRegFunc[7];
	BaseSetRegFunc*         pMul8SetRegFunc[7];
	BaseGetRegFunc*         pMul8GetRegFunc[7];
	BaseSetRegFunc*         pSepSetRegFunc[7];
	BaseGetRegFunc*         pSepGetRegFunc[7];
} TAcceContext, *PTAcceContext;

enum CHIPACCESSSTAenum
{
	SEP_REG_SET                        = 0x1,
	SEP_REG_GET,
	SEP_SRAM_WRITE,
	SEP_SRAM_READ,
	MUL8_REG_SET,
	MUL8_REG_GET,
	MUL8_SRAM_WRITE,
	MUL8_SRAM_READ,
	MUL16_REG_SET,
	MUL16_REG_GET,
	MUL16_REG_SET_16,
	MUL16_REG_GET_16,
	MUL16_SRAM_WRITE,
	MUL16_SRAM_READ
};

#define DISRIQINT 0x80

/*-------define V5 chip mapping address ---------*/
/*Seperate*/
/*used by register access*/
#define V5_HOST_ADDR_REGUIAL		0x8000300
#define V5_HOST_ADDR_REGUIAH		0x8000310

/*used by memory access*/
#define V5_HOST_ADDR_MEMUIA			0x8000301
#define V5_HOST_ADDR_MEMLIA			0x8000302
#define V5_HOST_ADDR_MEMLIA0		0x8000303

#define V5_HOST_ADDR_MEMDATA		0x8000380


//Multi8
#define V5_MULTI8_REG_PORT			0xb0
#define V5_MULTI8_REG_WORDL			0xb2
#define V5_MULTI8_REG_WORDH			0xb4

#define V5_MULTI8_MEM_PORT			0xb6


//Muti16
#define V5_MULTI16_MEM_FLG			0x1850

void   V5_Mul16SetReg(UINT32 adr, UINT8 val);
UINT8  V5_Mul16GetReg(UINT32 adr);
void   V5_Mul16SetReg16(UINT32 adr, UINT16 val);
UINT16 V5_Mul16GetReg16(UINT32 adr);
UINT32 V5_Mul16ReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_Mul16WriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_Mul16WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_Mul16WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size);
void   V5_Mul16SetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType);
UINT32 V5_Mul16GetRegByWType(UINT32 adr, UINT8 regWType);


void   V5_Mul8SetReg(UINT32 adr, UINT8 val);
UINT8  V5_Mul8GetReg(UINT32 adr);
UINT32 V5_Mul8ReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_Mul8WriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_Mul8WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_Mul8WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size);
void   V5_Mul8SetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType);
UINT32 V5_Mul8GetRegByWType(UINT32 adr, UINT8 regWType);


void   V5_SepSetReg(UINT32 adr, UINT8 val);
UINT8  V5_SepGetReg(UINT32 adr);
UINT32 V5_SepReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_SepWriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_SepWriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_SepWriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size);
void   V5_SepSetRegByWType(UINT32 adr, UINT32 val, UINT8 regWType);
UINT32 V5_SepGetRegByWType(UINT32 adr, UINT8 regWType);


void V5_StoreReg2SramCont(TAcceContext* pAcceContext);
void V5_RestoreReg2SramCont(TAcceContext* pAcceContext);

#define V5_MUL16SETREGADDR(adr) {V5_HOSTSETADDR16( V5_MULTI16_ADDR_ADDR, (UINT16)(adr) );}

#define V5_MUL16WRITESRAMSETCONT(adr)  \
        {\
		  V5_Mul16SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x1);\
		  V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)(adr));\
		  V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)((adr) >> 8));\
		  V5_Mul16SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)((adr) >> 16));\
		  V5_HOSTSETADDR16( V5_MULTI16_ADDR_ADDR, V5_MULTI16_MEM_FLG );\
		}

#define V5_MUL16READSRAMSETCONT(adr)   \
        {\
		  V5_Mul16SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x0);\
		  V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)(adr));\
		  V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)((adr) >> 8));\
		  V5_Mul16SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)((adr) >> 16));\
		  V5_HOSTSETADDR16( V5_MULTI16_ADDR_ADDR, V5_MULTI16_MEM_FLG );\
		}

#define V5_MUL8SETREGADDR(adr) \
	    {\
		  V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_WORDH );\
 	      V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)((adr) >> 8) );\
	      V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_WORDL );\
	      V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)(adr) );\
 	      V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_PORT );\
		}

#define V5_MUL8WRITESRAMSETCONT(adr) \
		{\
		  V5_Mul8SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x1);\
	      V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)(adr));\
	      V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)((adr) >> 8));\
	      V5_Mul8SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)((adr) >> 16));\
	      V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, V5_MULTI8_MEM_PORT );\
		}

#define V5_MUL8READSRAMSETCONT(adr) \
		{\
		  V5_Mul8SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x0);\
	      V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)(adr));\
	      V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)((adr) >> 8));\
	      V5_Mul8SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)((adr) >> 16));\
	      V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, V5_MULTI8_MEM_PORT );\
		}

#define V5_SEPSETREGADDR(adr) \
		{\
		  V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAL, (UINT8)((adr) >> 8) );\
	      V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAH, (UINT8)((adr) >> 16) );\
		}

#define V5_SEPWRITESRAMSETCONT(adr) \
		{\
		  V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAL, 0x18);\
	      V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAH, 0x0);\
		  V5_SepSetReg(V5_REG_BIU_SEL_WRITE_READ, 0x1);\
	      V5_HOSTSETADDR8( V5_HOST_ADDR_MEMUIA, (UINT8)((adr) >> 16) );\
          V5_HOSTSETADDR8( V5_HOST_ADDR_MEMLIA, (UINT8)((adr) >> 8) );\
          V5_HOSTSETADDR8( V5_HOST_ADDR_MEMLIA0, (UINT8)(adr) );\
		}

#define V5_SEPREADSRAMSETCONT(adr) \
		{\
		  V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAL, 0x18);\
	      V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAH, 0x0);\
		  V5_SepSetReg(V5_REG_BIU_SEL_WRITE_READ, 0x0);\
	      V5_HOSTSETADDR8( V5_HOST_ADDR_MEMUIA, (UINT8)((adr) >> 16) );\
          V5_HOSTSETADDR8( V5_HOST_ADDR_MEMLIA, (UINT8)((adr) >>8) );\
          V5_HOSTSETADDR8( V5_HOST_ADDR_MEMLIA0, (UINT8)(adr) );\
		}


#ifdef __cplusplus
}
#endif

#endif

