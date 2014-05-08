


#ifndef _RDK_GE_H_
#define _RDK_GE_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum BITBLTOPERenum {
		RESULT_NOTSAME_OVERWRITE,
		CURRENT_NOTSAME_OVERWRITE,
		RESULT_SAME_OVERWRITE,
		CURRENT_SAME_OVERWRITE
	};

	enum BITBLTCOLORDEPenum	{
		GE_RGB565,
		GE_RGB888
	};




//===============================
void	V5B_GeSetBitWise(UINT8 bitwise);
UINT8	V5B_GeGetBitWise(void);

//===============================
void	V5B_GeSetOpmode(UINT8 x);
UINT8	V5B_GeGetOpmode(void);
void	V5B_GeSetStartPoint(UINT8 x);
UINT8	V5B_GeGetStartPoint(void);
UINT8	V5B_GeMonoSrcUseSrc(void);
UINT8	V5B_GeMonoSrcUsePtnSrc(void);
UINT8	V5B_GeMonoSrc(void);
UINT8	V5B_GeColorSrc(void);
UINT8	V5B_GeEnableSrcMask(void);
UINT8	V5B_GeDisableSrcMask(void);
UINT8	V5B_GeEnableTransparent(void);
UINT8	V5B_GeDisableTransparent(void);
void	V5B_GeSetTransparent(UINT8 mode);
UINT8	V5B_GeGetTransparent(void);

//============================
void	V5B_GeSetOpmode1(UINT8 x);
UINT8	V5B_GeGetOpmode1(void);
UINT8	V5B_GeEnablePtnMask(void);
UINT8	V5B_GeDisablePtnMask(void);
UINT8	V5B_GeMonoPtn(void);
UINT8	V5B_GeColorPtn(void);
void	V5B_GeSetPtnVertAlign(UINT8 line);
UINT8	V5B_GeGetPtnVertAlign(void);
void	V5B_GeSetColordepth(UINT8 x);
UINT8	V5B_GeGetColordepth(void);

//==========================
void	V5B_GeStartOper(UINT8 x);
UINT8	V5B_GeGetStatus(void);
void	V5B_GeStartLineDraw(void);
void	V5B_GeStartBitblt(void);
void	V5B_GeReset(void);

//=================================
void	V5B_GeSetPtnDW(UINT8 adr, UINT32 val);
void	V5B_GeSetPattern(UINT8 cnt, UINT32 *val);

//===============
void	V5B_GeSetPtnBkColor(UINT32 x);
UINT32	V5B_GeGetPtnBkColor(void);

//=======================
void	V5B_GeSetPtnForeColor(UINT32 x);
UINT32	V5B_GeGetPtnForeColor(void);

//======================
void	V5B_GeSetSrcBkColor(UINT32 x);
UINT32	V5B_GeGetSrcBkColor(void);

//==============================
void	V5B_GeSetSrcForeColor(UINT32 x);
UINT32	V5B_GeGetSrcForeColor(void);

//============================
void	V5B_SetDstWidth(UINT16 x);
UINT16	V5B_GetDstWidth(void);
void	V5B_SetDstHeight(UINT16 x);
UINT16	V5B_GetDstHeight(void);
void	V5B_SetDstSize(TSize size);
void	V5B_GetDstSize(TSize *size);

//==============================
void	V5B_GeSetSrcStartAddr(UINT32 x);
UINT32	V5B_GeGetSrcStartAddr(void);

//================================
void	V5B_GeSetSrcLineColor(UINT32 x);
UINT32	V5B_GeGetSrcLineColor(void);

//===========================
void	V5B_GeSetDstStartAddr(UINT32 x);
UINT32	V5B_GeGetDstStartAddr(void);

//========================
void	V5B_GeSetSrcSpan(UINT16 x);
UINT16	V5B_GeGetSrcSpan(void);

//==============================
void	V5B_GeSetDstSpan(UINT16 x);
UINT16	V5B_GeGetDstSpan(void);

//===============================
void	V5B_GeSetClipStartpx(UINT16 x);
UINT16	V5B_GeGetClipStartpx(void);
void	V5B_GeSetClipStartpy(UINT16 x);
UINT16	V5B_GeGetClipStartpy(void);
void	V5B_GeSetClipStartPt(TPoint pt);
void	V5B_GeGetClipStartPt(TPoint *pt);
void	V5B_GeSetClipEndpx(UINT16 x);
UINT16	V5B_GeGetClipEndpx(void);
void	V5B_GeSetClipEndpy(UINT16 x);
UINT16	V5B_GeGetClipEndpy(void);
void	V5B_GeSetClipEndPt(TPoint pt);
void	V5B_GeGetClipEndPt(TPoint *pt);
void	V5B_GeSetClipping(TClip clip);
void	V5B_GeGetClipping(TClip *clip);

//========================
void	V5B_GeSetLineStartpx(UINT16 x);
UINT16	V5B_GeGetLineStartpx(void);
void	V5B_GeSetLineStartpy(UINT16 x);
UINT16	V5B_GeGetLineStartpy(void);
void	V5B_GeSetLineStartPt(TPoint pt);
void	V5B_GeGetLineStartPt(TPoint *pt);
void	V5B_GeSetLineEndpx(UINT16 x);
UINT16	V5B_GeGetLineEndpx(void);
void	V5B_GeSetLineEndpy(UINT16 x);
UINT16	V5B_GeGetLineEndpy(void);
void	V5B_GeSetLineEndPt(TPoint pt);
void	V5B_GeGetLineEndPt(TPoint *pt);

//======================================
void	V5B_GeDrawLine(TPoint ps, TPoint pe, UINT32 color);
void	V5B_GeBitblt(UINT32 srcstart, UINT32 dststart, TSize size, UINT8 bitwise);

#ifdef __cplusplus
}
#endif

#endif

