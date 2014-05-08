
#ifndef _TESTJPEG_H_
#define _TESTJPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tag_TJpegCapParm {
		UINT32		ilen; //index length
		UINT32		tlen; //tbuf length
		UINT32		jlen; //jbuf length
	} TJpegCapParm, *PTJpegCapParm;

	void TestCap(TDataBuf *pdbuf, TJpegCapParm* pRetParm);
	void TestReCap(TDataBuf *pdbuf, TJpegCapParm* pRetParm, UINT8* cnt);
	void TestSifCap(TDataBuf *pdbuf, TJpegCapParm* pRetParm);
	void TestCapVidTimer(TDataBuf *pdbuf, UINT32 ms, TJpegCapParm* pRetParm);

#ifdef HJPEG_RT_CAP
	UINT32 RtReqCapJbuf(UINT32 jbufst, UINT32 jbuflen);
#endif

#ifdef __cplusplus
}
#endif

#endif


