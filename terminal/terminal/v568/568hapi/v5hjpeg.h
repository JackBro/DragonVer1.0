#ifndef _V5HJPEG_H_
#define _V5HJPEG_H_

#ifdef __cplusplus
extern "C" {
#endif



	typedef void V5BuffullcallBack(UINT32 bufsize);


	//typedef struct tag_FileOperations { 
	//void *(*fopen) (char *filename, char *rwmode); 
	//UINT32 (*fclose) (void *file); 
	//UINT32 (*fread) (UINT8 *buf, UINT32 length, void *file); 
	//UINT32 (*fwrite) (UINT8 *buf, UINT32 length, void *file); 
	//} TFileOperations, *PTFileOperations;
	typedef struct tag_TV5MHJpegMemList{
		UINT8 *pData;
		UINT8 *next;
	}TV5MHJpegMemList;

	typedef struct tag_TV5MHJpegCapBuf {
		TDataBuf jbuf;
		TDataBuf tbuf;
		TDataBuf ibuf;
		TV5MHJpegMemList *jbufhead;
		TV5MHJpegMemList *jbuflast;
	} TV5MHJpegCapBuf, *PTV5MHJpegCapBuf;

	typedef struct tag_TV5MHJpegEncDecBuf {
		PTDataBuf psrc;
		PTDataBuf pdst;
	}TV5MHJpegEncDecBuf;

	typedef struct tag_TV5MHJpegDispBuf {
		TDataBuf jbuf;
	}TV5MHJpegDispBuf;

	typedef union tag_TV5MHJpegFucBuf {
		TV5MHJpegCapBuf cap;
		TV5MHJpegDispBuf disp;
		TV5MHJpegEncDecBuf enc;
		TV5MHJpegEncDecBuf dec;
	} TV5MHJpegFucBuf, *PTV5MHJpegFucBuf;

	typedef struct tag_V5MTHJpegContext {
		UINT8		inttype; //0 - interrupt, 1 - Timerquery ,2 - Query
		UINT16		jpgdone;
		UINT8		stopvid;
		UINT32		videosize;  //byte
		TV5MHJpegFucBuf fucbuf;
		//V5BuffullcallBack pfullcallback;
		//V5MMalloc	*pmalloc;
		//V5MFree		*pfree;
		//TFileOperations fileop;
		//V5MF_HANDLE	pfhandle;
		char		jfilename[256];
		UINT32		curvideosize; //byte
		//PJpegCallBack pcall;
	} TV5MHJpegContext, *PTV5MHJpegContext;



	enum V5MCAPTUREMODEenum{
		V5M_CAPTURE_STILL,
		V5M_CAPTURE_MULTISHOT,
		V5M_CAPTURE_AVI,
		V5M_CAPTURE_PCCAMERA
	};

	enum V5MDECODETYPEenum{
		V5M_DECODE,
		V5M_DECODEIPP,
		V5M_DECODEFRAME
	};

	void V5M_HJpegOpen(void);
	void V5M_SaveJbufToFile(void );
	void V5M_JbufFree(void );
	void V5M_SaveIbufToFile(void );
	void V5M_SaveTbufToFile(void );
	void V5M_IndexThumbBufFree(void );
	void V5M_SetCompRatio(UINT8 value);
	void V5M_SetCompedJFileSize(UINT16 filesize);

	UINT32 V5M_SetInterruptType(UINT8 inttype) ;
	UINT32 V5M_GetInterruptType(UINT8 *inttype);
	UINT32 V5M_StartCapture(void);
	void V5M_SetVideoFileSize(UINT8 filesize);
	UINT32 V5M_GetIndexThumbBufSize(UINT8 mode, UINT32 *tsize, UINT32 *isize);
	UINT32 V5M_GetThumbActSize(void);
	UINT32 V5M_getCaptureInfoToFile(char *filename, UINT32 bufsizeOnetime );


	void V5M_HJpegDisplayFrame(PTJpegIndex pinfo, PTDataBuf psrc);
	void V5M_Display(char *filename, UINT8 bVideo );
	void V5M_HJpegDispVidNorm(TDataBuf* pVid);

	void V5M_JpegDecode(PTDataBuf psrc, PTDataBuf pdst, UINT8 type);
	void V5M_ReadWriteDecDataIppLcd(UINT16 jpgheight, UINT8 yuvtype, TSize finalImageSize, PTDataBuf pdst, UINT8 bYuv);
	void V5M_ReadWriteDecData(UINT8 YUVType, TSize jpegsize, PTDataBuf pdst);
	void V5M_DecodeEncode(char *srcfile, char *dstfile, UINT8 dectype);
	void V5M_SetDecodeIPPSizer(char *srcfile, UINT8 ratio);
	void V5M_SetDecodeMode(UINT8 dectype);
	void V5M_JpegEncode(PTDataBuf psrc, PTDataBuf pdst, UINT8 fmt, TSize size);

#ifdef __cplusplus
}
#endif

#endif
