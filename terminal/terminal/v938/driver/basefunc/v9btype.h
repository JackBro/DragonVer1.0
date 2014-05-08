


#ifndef _RDK_V9BTYPE_H_
#define _RDK_V9BTYPE_H_


//============== biu type =======================
//Define bus type
#define V9_BUS_MULTI8		3

//Define bus mode
#define V9_BUS_MODE0		0
#define V9_BUS_MODE1		1
#define V9_BUS_MODE2		2
#define V9_BUS_MODE3		3


enum  AESINFOenmu
{
	AES_KEY_LENGTH          = 16,
	AES_INITVEC_LENGTH      = 16
};

/* Aes information */
typedef struct tag_TAesInfo 
{
	UINT8     aesMod;
	UINT8     aesKey[AES_KEY_LENGTH];
	UINT8     aesInitVect[AES_INITVEC_LENGTH];
} TAesInfo, *PTAesInfo ;

/* Channel register definition */
typedef struct tag_TChanReg 
{
	/* Bank 0 */
	UINT16    curPtr13;
	UINT8     curSign;
	UINT8     evlpMod0_h;
	UINT8     lfoCnt8;
	UINT8     evlpCnt;

    UINT16    fratio;
	UINT16    endPtr_l;
	UINT8     evlpMod0_1;
	UINT8     evlpMod2;

	UINT16    intpCoef;
	UINT8     evlpChg0_l;
	UINT8     curEvlp_l;
	UINT8     sinVal;
	UINT8     sinDir1;
	UINT8     lfoSign1;
	UINT8     evlpSeg;

	UINT8     rVol;
	UINT8     chnVolEn;
	UINT8     eCntInit0;
	UINT8     eCntInit1;
	UINT8     eCntInit2;

	/* Bank 1*/
	UINT32    baseAddr20;
	UINT8     sinCtrl;
	UINT8     bitCtrl;
	UINT8     loopEn;
	UINT8     chnAtrb;
	UINT8     evlpMod1_h;
	UINT8     fmCtrl;
	UINT8     aStep;
	
	UINT8     fStep;
	UINT8     curEvlp_h;
	UINT16    preVal;
	UINT8     preIndex;
	UINT8     evlpMod1_l;
	
	UINT8     evlpChg0_h;
	UINT16    evlpChg1;
	UINT16    evlpChg2;

	UINT8     chnVol_l;
	UINT8     chnVol_r;
	UINT8     lfoInit8;
	UINT8     eCntInit3;

} TChanReg, *PTChanReg ;


#endif



