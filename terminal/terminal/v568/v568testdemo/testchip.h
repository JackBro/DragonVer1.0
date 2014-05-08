
#ifndef _TESTCHIP_H_
#define _TESTCHIP_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define TESTCHIP_FILE_LEN 40

	typedef struct tag_TTestChipSize
	{
		TSize imgSize;
		TSize winSize;
		TSize capSize;
		TSize dispSize;
		TSize thumbSize;
		TPoint pt;
	} TTestChipSize, *PTTestChipSize;


	typedef struct tag_TTestChipContext {
		char FrmFileName[TESTCHIP_FILE_LEN];
		char DecFileName[TESTCHIP_FILE_LEN];
		char DecResultFileName[TESTCHIP_FILE_LEN];
		char DecCmpFileName[TESTCHIP_FILE_LEN];
		char StiFileName[TESTCHIP_FILE_LEN];
		char ThumbFileName[TESTCHIP_FILE_LEN];
		unsigned char KeyColor;
		TTestChipSize aProty;
	} TTestChipContext, *PTTestChipContext;

	PTTestChipContext TestChipGetContext(void);
	void TestChipOpen(void);
	void TestChipCapture(void);
	void TestChipSetCapture(void);
	void TestChipDecodeFrame(void);
	void TestChipDisplay(void);
	void TestChipSetMode(UINT8 mode);
	void TestChipSetVGACall(void);
	void TestChipSetSXGACall(void);
	void TestChipOverlay(void);
	void TestChip1TStart(void);
	UINT8  TestChip1TChk(void);

	void TestChipProcess(void);
	void TestChipPower(void);
	UINT8 TestChipRW(void);

#ifdef __cplusplus
}
#endif

#endif


