


#ifndef _HJPEG_H_
#define _HJPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define JPEG_FIFO_ACESS_SIZE	(1024*4)

	typedef void (* PJpegCallBack)(UINT8 msg, PTDataBuf pdata);
	typedef void (* PJpegDecReq)(UINT8 val);

	enum JPEGMSGenum {
		JMSG_CAP_JBUF_CNT	=	0x0,
		JMSG_DIS_JBUF_CNT	,
		JMSG_ENC_JBUF_CNT	,
		JMSG_JPEG_DONE		,
		JMSG_JPEG_RECAP		,
		JMSG_JPEG_RECAP_ERR	,
		JMSG_JPEG_JBUF_INTV ,
		JMSG_JBUF_ERROR		,
		JMSG_TBUF_CNT		,
		JMSG_THUMB_DONE		,
		JMSG_THUMB_ERROR	,
		JMSG_IBUF_CNT		,
		JMSG_ENC_DONE		,
		JMSG_DEC_DONE		,
		JMSG_DEC_ERROR		,
		JMSG_LBUF_READ		,
		JMSG_REQ_CAP_JBUF	,
		JMSG_REQ_ENC_JBUF	,
		JMSG_REQ_TBUF		,
		JMSG_REQ_IBUF		,
		JMSG_REQ_DEC_BUF	,
		JMSG_REQ_DISP_BUF

	};

	enum JPEG_CAPenum {
		JPEG_CAP_NORM,
		JPEG_CAP_SIF,
		JPEG_CAP_REP,
		JPEG_CAP_SOFT_REP,
		JPEG_CAP_TIME
	};

	struct THJpegCapBuf {
		PTDataBuf pjbuf;
		PTDataBuf ptbuf;
		PTDataBuf pibuf;
	};

	struct THJpegDispBuf {
		PTDataBuf pjbuf;
	};

	struct THJpegEncDecBuf {
		PTDataBuf psrc;
		PTDataBuf pdst;
	};

	typedef union tag_THJpegFucBuf {
		struct THJpegCapBuf cap;
		struct THJpegDispBuf disp;
		struct THJpegEncDecBuf enc;
		struct THJpegEncDecBuf dec;
	} THJpegFucBuf, *PTHJpegFucBuf;


	typedef struct tag_THJpegContext {
		UINT8	intreq; //0 - interrupt, 1 - request
		UINT16	jpgdone;
		UINT16	decdone;
		UINT8	stopvid;
		UINT8	captype; // 0 - norm, 1 - sif, 2 - repeat
		UINT8	capvidcnt;
		UINT8	recapcnt;
		UINT8	recapactcnt;
		THJpegFucBuf fucbuf;
		PJpegCallBack pcall;
	} THJpegContext, *PTHJpegContext;

	void V5_HJpegOpen(void);
	THJpegContext * V5_HJpegGetContext(void);

	void V5_HJpegSetIntReq(UINT8 intreq);
	UINT8 V5_HJpegGetIntReq(void);

	void V5_HJpegCap(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf);
	void V5_HJpegReCap(PTDataBuf pjbuf, PTDataBuf ptbuf, UINT8* cnt);
	void V5_HJpegSifCap(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf);
	void V5_HJpegCapVidTime(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf, UINT32 ms);
	void V5_HJpegEnc(PTDataBuf psrc, PTDataBuf pdst, UINT8 fmt, TSize size);
	void V5_HJpegDispFrm(PTDataBuf psrc);
	void V5_HJpegDispFrmAuto(PTDataBuf psrc);
	void V5_HJpegDispFrmDirAuto(UINT8 fmt, TSize size, UINT32 jbufsize);
	void V5_HJpegDispNextFrm(void);
	void V5_HJpegDispVidNorm(TDataBuf* pVid);
	void V5_HJpegDispVidAuto(TDataBuf* pVid, UINT32 ms);
	void V5_HJpegDispAviAuto(TDataBuf* pVid, UINT32 ms);
	void V5_HJpegDec(PTDataBuf psrc, PTDataBuf pdst);
	void V5_HJpegDecIpp(PTDataBuf psrc, PTDataBuf pdst);
	void V5_HJpegDecFrm(PTDataBuf psrc, PTDataBuf pdst);
	void V5_HJpegYuvDisp(PTDataBuf psrc, TSize size);
	void V5_HJpegYuvDecIpp(PTDataBuf psrc, PTDataBuf pdst, TSize size);
	void V5_HJpegYuvDecFrm(PTDataBuf psrc, PTDataBuf pdst, TSize size);

	void V5_HJpegSetCallBack(PJpegCallBack pcall);


#ifdef __cplusplus
}
#endif

#endif 


