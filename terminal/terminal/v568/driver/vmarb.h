


#ifndef _RDK_VMARB_H_
#define _RDK_VMARB_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum MARBSTATEenum
	{
		MARB_NOTINITED,
		MARB_INITED,
		MARB_OPENED,
		MARB_CLOSE
	};

	#define MARB_GAPINTERVAL	40

	void V5_MarbOpen(void);
	void V5_MarbClose(void);
	void V5_MarbInitContext(void);
	void V5_MarbSetInfo(TMarbInfo *info);
	TMarbContext * V5_MarbGetContext(void);

	//====================================================

	UINT32 V5_MarbGetJbufRWgap(void);
	UINT32 V5_MarbJbufRead(UINT8 *pbuf, UINT32 len);
	UINT32 V5_MarbJbufWrite(UINT8 *pbuf, UINT32 len);

	UINT32 V5_MarbGetThumbRWgap(void);
	UINT32 V5_MarbThumbRead(UINT8 *pbuf, UINT32 len);

	UINT32 V5_MarbIndexRead(UINT8 *pbuf, UINT32 len);

	UINT32 V5_MarbLbuf0Read(UINT8 *pbuf, UINT32 len);
	UINT32 V5_MarbLbuf1Read(UINT8 *pbuf, UINT32 len);

	UINT32	V5_MarbB0Write(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);
	UINT32	V5_MarbB0Read(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);
	UINT32	V5_MarbB0WriteRepeat(UINT32 val, UINT32 len, UINT32 start, UINT32 span, UINT32 size);
	UINT32	V5_MarbB0WriteRev(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);

	UINT32	V5_MarbB1Write(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);
	UINT32	V5_MarbB1Read(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);

	UINT32	V5_MarbWriteSpanBuf(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);
	UINT32	V5_MarbReadSpanBuf(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);
	UINT32	V5_MarbWriteSpanBufRepeat(UINT32 val, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);
	UINT32 V5_MarbWriteSpanBufRev(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);

	//====================================================
	void	V5_MarbStartCapture(void);
	void	V5_MarbStartDecoder(void);

	void	V5_MarbSetAllocMode(UINT8 mode);
	UINT8	V5_MarbGetAllocMode(void);

	//==============================================

	void	V5_MarbSetJbufStartAdr(UINT32 adr);
	void	V5_MarbSetIndexMem(UINT32 len);
	void	V5_MarbAutoSetMem(void);
	void	V5_MarbSetGapRatio(UINT8 ratio);

	void	V5_MarbSetMemStart(UINT32 adr);
	UINT32	V5_MarbGetMemStart(void);
	void	V5_MarbSetMemEnd(UINT32 adr);
	UINT32	V5_MarbGetMemEnd(void);

	//==============================================
	UINT32	V5_MarbGetIndexMem(void);
	UINT8	V5_MarbGetGapRatio(void);

	void	V5_MarbSetMode(UINT8 mode);
	UINT8	V5_MarbGetMode(void);

	UINT32	V5_MarbGetB0StartAddr(void);
	UINT32	V5_MarbGetB1StartAddr(void);
	void	V5_MarbGetMap(TMarbMap *map);

	//==============================================
	void	_ISR_V5MarbIntHandle(UINT16 intflg);
	void	V5_MarbIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
	void	V5_MarbIntEnable(UINT8 index);
	void	V5_MarbIntDisable(UINT8 index);

	void	_ISR_V5MarbSlvIntHandle(UINT16 intflg);
	void	V5_MarbSlvIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
	void	V5_MarbSlvIntEnable(UINT8 index);
	void	V5_MarbSlvIntDisable(UINT8 index);

	void	V5_MarbSetTimerCfg(UINT8 mode, UINT32 ms);
	UINT32	V5_MarbGetTimerVal(void);


#ifdef __cplusplus
}
#endif

#endif



