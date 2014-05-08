


#ifndef _RDK_JPEG_H_
#define _RDK_JPEG_H_

#ifdef __cplusplus
extern "C" {
#endif



//jpeg mode
enum V5B_JPEG_WORKMODEenum
{
	//capture   mode
	V5B_JPEG_WORKMODE_CAP						= 0x0,
	//display    mode
	V5B_JPEG_WORKMODE_DISP						= 0x1,
	//jpeg encode mode
	V5B_JPEG_WORKMODE_ENC						= 0x2,
	//jpeg decode mode
	V5B_JPEG_WORKMODE_DEC						= 0x3
};

//jpeg status
enum V5B_JPEG_STATUSenum
{
	//Encode done
	V5B_JPEG_STATUS_ED						= BIT0,
	//Decode done
	V5B_JPEG_STATUS_DD						= BIT1,
	//Decode error flag
	V5B_JPEG_STATUS_ER						= BIT2,
	//Header done
	V5B_JPEG_STATUS_HD						= BIT3
};

void	V5B_JpegSetMod(UINT8 val);
UINT8	V5B_JpegGetMod(void);
void	V5B_JpegSetWorkMod(UINT8 val);
UINT8	V5B_JpegGetWorkMod(void);
UINT8	V5B_JpegSimHeadEnable(void);
UINT8	V5B_JpegSimHeadDisable(void);
UINT8	V5B_JpegCustQTEnable(void);
UINT8	V5B_JpegCustQTDisable(void);
UINT8	V5B_JpegDispVdEnable(void);
UINT8	V5B_JpegDispVdDisable(void);
UINT8	V5B_JpegCustHTEnable(void);
UINT8	V5B_JpegCustHTDisable(void);
UINT8	V5B_JpegAviFmtEnable(void);
UINT8	V5B_JpegAviFmtDisable(void);
UINT8	V5B_JpegEnhBrcEnable(void);
UINT8	V5B_JpegEnhBrcDisable(void);


void	V5B_JpegSetCtrl(UINT8 val);
UINT8	V5B_JpegGetCtrl(void);
void	V5B_JpegStartEncode(void);
void	V5B_JpegStartDecode(void);
void	V5B_JpegRestartDecode(void);
void	V5B_JpegResetState(void);
void	V5B_JpegStartWriteQT(void);
void	V5B_JpegStartWriteHT(void);
void	V5B_JpegSelectHT(UINT8 sel);

UINT8	V5B_JpegGetStatus(void);

void	V5B_JpegSetBrc(UINT8 val);
UINT8	V5B_JpegGetBrc(void);
void	V5B_JpegSetBrcQF(UINT8 val);
UINT8	V5B_JpegGetBrcQF(void);
UINT8	V5B_JpegEnAbrc(void);
UINT8	V5B_JpegDisAbrc(void);

void	V5B_JpegSetTCR(UINT8 val);
UINT8	V5B_JpegGetTCR(void);
UINT8	V5B_JpegGetQfactor(void);

void	V5B_JpegSetHeaderStep(UINT8 val);
UINT8	V5B_JpegGetHeaderStep(void);

void	V5B_JpegSetVWC(UINT32 val);
UINT32	V5B_JpegGetVWC(void);
UINT32	V5B_JpegGetHuffmanWC(void);

void	V5B_JpegSetImgSize(TSize size);
void	V5B_JpegGetImgSize(TSize* size);
void	V5B_JpegSetImageWidth(UINT16 val);
UINT16	V5B_JpegGetImageWidth(void);
void	V5B_JpegSetImageHeight(UINT16 val);
UINT16	V5B_JpegGetImageHeight(void);

void	V5B_JpegSetBrcParm(UINT16 val);
UINT16	V5B_JpegGetBrcParm(void);

UINT16	V5B_JpegGetFrameCnt(void);

void	V5B_JpegSetDctData(UINT8 val);
void	V5B_JpegSetQtable(UINT8 val);
void	V5B_JpegSetHtable(UINT8 val);
void	V5B_JpegSetComponentNum(UINT8 val);
UINT8	V5B_JpegGetComponentNum(void);
void	V5B_JpegSetComponentParm(UINT8 sel, UINT16 val);
UINT16	V5B_JpegGetComponentParm(UINT8 sel);
void	V5B_JpegSetBlockNum(UINT16 val);
UINT16	V5B_JpegGetBlockNum(void);

void	V5B_JpegSetImageParm(UINT8 count, UINT8 *val);
void	V5B_JpegGetImageParm(UINT8 count, UINT8 *val);

void	V5B_JpegSetHeaderType(UINT8 val);
UINT8	V5B_JpegGetHeaderType(void);

UINT32	V5B_JpegGetJWC(void);

UINT8	V5B_JpegGetPixelCount(void);
UINT16	V5B_JpegGetBlockCount(void);

void V5B_JpegSetDecByp(UINT8 val);
UINT8 V5B_JpegGetDecByp(void);
UINT8 V5B_JpegDecBypEnable(void);
UINT8 V5B_JpegDecBypDisable(void);
void V5B_JpegSetDecBypStep(UINT8 val);
UINT8 V5B_JpegGetDecBypSetp(void);
void V5B_JpegSetHblank(UINT16 val);
UINT16 V5B_JpegGetHblank(void);

void V5B_JpegEncErrEnable(void);
void V5B_JpegEncErrDisable(void);

void V5B_JpegSetEnhThdR1(UINT8 val);
UINT8 V5B_JpegGetEnhThdR1(void);
void V5B_JpegSetEnhThdR2(UINT8 val);
UINT8 V5B_JpegGetEnhThdR2(void);
void V5B_JpegSetEnhThdL1(UINT8 val);
UINT8 V5B_JpegGetEnhThdL1(void);
void V5B_JpegSetEnhThdL2(UINT8 val);
UINT8 V5B_JpegGetEnhThdL2(void);
void V5B_JpegSetEnhShfR1(UINT8 val);
UINT8 V5B_JpegGetEnhShfR1(void);
void V5B_JpegSetEnhShfR2(UINT8 val);
UINT8 V5B_JpegGetEnhShfR2(void);
void V5B_JpegSetEnhShfL1(UINT8 val);
UINT8 V5B_JpegGetEnhShfL1(void);
void V5B_JpegSetEnhShfL2(UINT8 val);
UINT8 V5B_JpegGetEnhShfL2(void);

void V5B_JpegSetRudeCompLevel(UINT8 index, UINT8 val);
UINT8 V5B_JpegGetRudeCompLevel(UINT8 index);
void V5B_JpegSetRudeBrc(UINT8 val);
UINT8 V5B_JpegGetRudeBrc(void);
void V5B_JpegRudeBrcEnable(void);
void V5B_JpegRudeBrcDisable(void);
void V5B_JpegRudeBrcUvOnlyEnable(void);
void V5B_JpegRudeBrcUvOnlyDisable(void);
void V5B_JpegSetRudeAc(UINT8 val);
UINT8 V5B_JpegGetRudeAc(void);
void V5B_JpegSetRudeLevel(UINT8 val);
UINT8 V5B_JpegGetRudeLevel(void);
void V5B_JpegSetRudeThdWc(UINT32 val);
UINT32 V5B_JpegGetRudeThdWc(void);
void V5B_JpegSetTimer(UINT32 val);
UINT32 V5B_JpegGetTimer(void);



#ifdef __cplusplus
}
#endif

#endif


