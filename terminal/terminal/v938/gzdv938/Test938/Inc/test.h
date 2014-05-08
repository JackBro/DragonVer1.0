#ifndef __TEST_H__
#define __TEST_H__

void TestPlay(void);
void TestPlayEvent(void);
void TestPlayStream(UINT8 uCtype);
void TestPlayPreload(UINT8 uChnNum);
void TestPlayDLS(void);
void TestPlayMP3(void);
void TestPlayCommand(void);

void TestEventInt(void);
void TestStream0Int(void);
void TestStream1Int(void);
void TestStream2Int(void);
void TestStream3Int(void);
void TestMp3Int(void);
void TestSTimerInt(void);
void TestLTimerInt(void);
void TestErrorInt(void);
void Test_MainIsr(void);

void TestPlay(void);
void TestStop(void);
void TestDownloadFile(void);
void TestSendDebugInfo(void);


#endif /*__TEST_H__*/

