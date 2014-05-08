


#ifndef _RDK_VJPEG_H_
#define _RDK_VJPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

	//chip state
	enum JPEG_STATEenum
	{
		JPEG_NOT_INIT	= 0X0,
		JPEG_INIT		= 0X1
	};

	//These follow founctions are used for JPEG initialization
	void	V5_JpegInitContext(void);
	void	V5_JpegSetInfo(TJpegInfo *info);
	TJpegContext * V5_JpegGetContext(void);

	void	V5_JpegOpen(void);
	void	V5_JpegClose(void);

	void	V5_JpegSetQFactor(UINT8 val);
	void	V5_JpegDisAbrc(void);

	void	V5_JpegSetImage(UINT8 fmt, TSize size);
	void	V5_JpegAutoSetImage(void);

	void	V5_JpegSetMode(UINT8 mode);
	UINT8	V5_JpegGetMode(void);

	//These follow founctions are used for JPEG main functions 
	void	V5_JpegCapture(void);
	void	V5_JpegSifCapture(void);
	void	V5_JpegReCapture(void);
	void	V5_JpegEncode(UINT8 fmt, TSize size);
	void	V5_JpegDecode(PTJpegIndex pinfo);
	void	V5_JpegDecodeVideo(UINT8 fmt, TSize size);
	void	V5_JpegDecodeAuto(UINT8 fmt, TSize size);
	void	V5_JpegYuvDecode(TSize size);
	void	V5_JpegDecodeNext(void);

	void	V5_JpegSetRudeBrcPara(PTJpegRudeBrc pRude);
	void	V5_JpegSetEnhBrcPara(PTJpegEnhBrc pEnh);

	void	_ISR_V5JpegJpegIntHandle(UINT16 intflg);
	void	V5_JpegJpegIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
	void	V5_JpegJpegIntEnable(UINT8 index);
	void	V5_JpegJpegIntDisable(UINT8 index);

	void	_ISR_V5JpegLbufIntHandle(UINT16 intflg);
	void	V5_JpegLbufIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
	void	V5_JpegLbufIntEnable(UINT8 index);
	void	V5_JpegLbufIntDisable(UINT8 index);

	void	V5_JpegSetTimerCfg(UINT32 ms);

#ifdef __cplusplus
}
#endif

#endif



