
#ifndef _TESTISP_H_
#define _TESTISP_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define TEST_TIME_COUNT		10000
	typedef void (*PTestCall)(void);
	int TestTime(PTestCall pcall);

	void TestVisp(void);

	void TestComputerParm(UINT16 srcWidth, UINT16 srcHeight, UINT16 width, UINT16 *dstWidth, UINT16 *dstHeight, UINT8 *intNum, UINT16 *decNum);
	void TestSetThumb(UINT16 capWidth, UINT16 capHeight, UINT8 width, UINT8 *thumbWidth, UINT8 *thumbHeight, UINT8 * thumbRatio);
	void AdjustAe(UINT32 et, UINT8 ygain, UINT8 env);
	UINT8 TestAeInterrupt(void);
	void TestAutoExpose(void);
	void TestAutoDetectEnv(void);
	void TestAeSim(void);
	void V5_VispTestCaptureSXGA(void);
	void V5_VispTestCapture(void);

#ifdef __cplusplus
}
#endif

#endif

