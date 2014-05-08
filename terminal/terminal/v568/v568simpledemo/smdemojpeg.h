
#ifndef _SMDEMOJPEG_H_
#define _SMDEMOJPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define SMJPEG_FILE_LEN 40

	typedef struct tag_TSmJpegMulSet {
		UINT8 MulNum;
		UINT8 rownum;
		UINT8 colnum;
		UINT32 frmcolor;
		UINT32 selcolor;
		UINT8 frmthick_sel;
	}TSmJpegMulSet, *PTSmJpegMulSet;

	typedef struct tag_TSmJpegSet {

		TSmJpegMulSet MulSet;

		char StiFileName[SMJPEG_FILE_LEN];
		char StiDispFileName[SMJPEG_FILE_LEN];
		char VidFileName[SMJPEG_FILE_LEN];
		char VidDispFileName[SMJPEG_FILE_LEN];
		char MulFileName[SMJPEG_FILE_LEN];
		char EncFileName[SMJPEG_FILE_LEN];
		char EncResultFileName[SMJPEG_FILE_LEN];
		char DecFileName[SMJPEG_FILE_LEN];
		char DecResultFileName[SMJPEG_FILE_LEN];
	}TSmJpegSet, *PTSmJpegSet;


	typedef struct tag_TSmJpegContext {
		PTSmJpegSet set;

		TSize thumActSize;
		UINT8 curThumbSel;

	}TSmJpegContext, *PTSmJpegContext;

//Init
	void SmJpegOpen(void);
	PTSmJpegSet SmJpegGetSetPt(void);

//Menu
	void SmpJpeg(UINT8 key, UINT8* menustring);
	void SmJpegCapSti(UINT8 key, UINT8* menustring);
	void SmJpegDisSti(UINT8 key, UINT8* menustring);
	void SmJpegCapFrm(UINT8 key, UINT8* menustring);
	void SmJpegCapVid(UINT8 key, UINT8* menustring);
	void SmJpegDisVid(UINT8 key, UINT8* menustring);
	void SmJpegMulShot(UINT8 key, UINT8* menustring);
	void SmJpegEnc(UINT8 key, UINT8* menustring);
	void SmJpegDec(UINT8 key, UINT8* menustring);
	void SmJpegExit(UINT8 key, UINT8* menustring);
	void SmJpegMulSel(UINT8 key, UINT8* menustring);
	void SmJpegMulDisp(UINT8 key, UINT8* menustring);

//Functions
	void smJpegCapStill(void);
	void smJpegCapVideo(void);
	void smJpegCapFrame(void);
	void smJpegDispStill(void);
	void smJpegDispVideo(void);
	void smJpegMultishot(void);
	void smJpegEncode(void);
	void smJpegDecode(void);

	void smJpegMulSelUp(void);
	void smJpegMulSelDw(void);
	void smJpegMulSelLf(void);
	void smJpegMulSelRg(void);
	void smJpegMulSelDisp(void);

	void smJpegRetView(void);
	void smJpegClearMenuRetView(void);
	void smJpegRetMulSel(void);

#ifdef __cplusplus
}
#endif

#endif


