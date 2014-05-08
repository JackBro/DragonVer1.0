


#ifndef _RDK_VDISP_H_
#define _RDK_VDISP_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define	V5_DISP_MIN_BRIGHTNESS	0
	#define	V5_DISP_MAX_BRIGHTNESS	64
	#define	V5_DISP_MIN_CONTRAST	0
	#define	V5_DISP_MAX_CONTRAST	64


	enum DISPSTATEenum {
		DISP_NOTINITED,
		DISP_INITED,
		DISP_OPENED,
		DISP_CLOSE
	};




void	V5_DispOpen(void);
void	V5_DispClose(void);
void	V5_DispInitClk(void);

void	V5_DispInitContext(void);
void	V5_DispSetPanelInfo(PTPanelInfo info);
TDispContext * V5_DispGetContext(void);

void	V5_DispSetMode(UINT8 mode);
UINT8	V5_DispGetMode(void);

void	V5_DispGetPanelSize(TSize *size);
void	V5_DispSwitchPanel(UINT8 sel);			//change cs, modify some content
UINT8	V5_DispGetPanelSel(void);
UINT8	V5_DispGetPanelType(void);

void	V5_DispSetB0format(UINT8 fmt);
void	V5_DispSetB1format(UINT8 fmt);
UINT8	V5_DispGetB0format(void);
UINT8	V5_DispGetB1format(void);

void	V5_DispSetB0Proty(PTBLayerProty proty);
void	V5_DispGetB0Proty(PTBLayerProty proty);
void	V5_DispSetB1Proty(PTBLayerProty proty);
void	V5_DispGetB1Proty(PTBLayerProty proty);

void	V5_DispGetB0MemSize(TSize *size);
void	V5_DispGetB1MemSize(TSize *size);

void	V5_DispSetAmemSize(PTALayerProty proty);
void	V5_DispGetAmemSize(PTALayerProty proty);

void	V5_DispB0Mono(TPoint pt, TSize size, UINT32 val);
void	V5_DispB0Rev(TPoint pt, TSize size, UINT8 *pImage);
void	V5_DispB0Image(TPoint point, TSize size, UINT8 *pImage);//image must be byte-align
void	V5_DispB1Image(TPoint point, TSize size, UINT8 *pImage);//image must be byte-align

void	V5_DispB0DrawBmp(TPoint pt, UINT8 *pData);	//Image must be byte-align
void	V5_DispB1DrawBmp(TPoint pt, UINT8 *pData);

void	V5_DispSetWrTiming(UINT16 x);

void	V5_DispSetB0BlendRatio(UINT8 ratio);
UINT8	V5_DispGetB0BlendRatio(void);
void	V5_DispSetB1BlendRatio(UINT8 ratio);
UINT8	V5_DispGetB1BlendRatio(void);

void	V5_DispSetB0Overlay(UINT8 mode);
UINT8	V5_DispGetB0Overlay(void);
void	V5_DispSetB1Overlay(UINT8 mode);
UINT8	V5_DispGetB1Overlay(void);

void	V5_DispSetBrightness(UINT8 value);
UINT8	V5_DispGetBrightness(void);
void	V5_DispSetContrast(UINT8 value);
UINT8	V5_DispGetContrast(void);

void	V5_DispSetALayerWin(TPoint pt, TSize size);
void	V5_DispSetB0LayerWin(TPoint pt, TSize size);
void	V5_DispSetB1LayerWin(TPoint pt, TSize size);
void	V5_DispSetB0RefreshWin(TPoint pt, TSize size);
void	V5_DispSetB1RefreshWin(TPoint pt, TSize size);
void	V5_DispUpdateB0Win(PTWinPos pwin);
void	V5_DispUpdateB1Win(PTWinPos pwin);

void	_ISR_DispLcdcIntHandle(UINT16 flag);

//=============================================
void	V5_GeSetDst(UINT8 sel);
UINT8	V5_GeGetDst(void);

void	V5_GeSetSrc(UINT8 sel);
UINT8	V5_GeGetSrc(void);

UINT8	V5_GetDstFmt(void);
UINT8	V5_GetSrcFmt(void);

void	V5_GeAutoSetProp(void);
void	V5_GeSetResSpan(UINT8 index, UINT16 value);
void	V5_GeSetResfmt(UINT8 index, UINT8 value);

void	V5_GeClip(TClip clip);
void	V5_GeDrawLine(TPoint ps, TPoint pe, UINT32 color);
void	V5_GeBitblt(TPoint srcp, TPoint dstp, TSize size, UINT8 bitwise);

#ifdef __cplusplus
}
#endif

#endif 


