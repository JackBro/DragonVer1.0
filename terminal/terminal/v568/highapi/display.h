


#ifndef _RDK_DISPLAY_H_
#define _RDK_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define	V5_FONT_WIDTH				0x08
	#define	V5_FONT_HEIGHT				0x10
	#define V5_FONT_SPAN				0x04

	#define V5_FONT_SRC_FORE_COLOR1		0x00ff00
	#define V5_FONT_SRC_FORE_COLOR2		0xff0000
	#define V5_FONT_SRC_BACK_COLOR		0
	#define V5_FONT_PTN_FORE_COLOR		0xffffff
	#define V5_FONT_PTN_BACK_COLOR		0


	enum GRAPHSTATUSenum {
		GRAPH_INITED	=	BIT0,
		FONT_INITED		=	BIT1
	};

	enum DISPLAYBITBLTenum{
		GE_FONT = 0x02,
		GE_CURSOR = 0x03
	};

	enum DISPFONTPROPenum {
		OP_S_COLOR1,
		OP_S_COLOR2
	};

	typedef struct tag_TFontInfo {
		UINT8	*pbuf;
		UINT32	len;
		UINT8	span;
		UINT8	fmt;
		TSize	unit;
		UINT32	srcfg, srcbg;
		UINT32	ptnfg, ptnbg;
	} TFontInfo, *PTFontInfo;

	typedef struct tag_TGraphContext {
		UINT8	oldsrc, olddst;
		UINT8	status;
		UINT32	fontadr;
		TFontInfo	info;
	} TGraphContext, *PTGraphContext;

	void	V5_LoadBmp(UINT8 *pbmp, UINT8 dst);
	void	V5_DisplayB0Bmp(UINT8 *pbmp);
	void	V5_DisplayB1Bmp(UINT8 *pbmp);

	void	V5_GraphInit(void);
	void	V5_FontInit(TFontInfo *pinfo);
	void	V5_PreDispFont(UINT8 dst);		//dst must be B1 or B0
	void	V5_PostDispFont(void);
	void	V5_DisplayChar(TPoint pt, UINT8 ch, UINT8 option);
	void	V5_DisplayStr(TPoint pt, UINT8 *str, UINT8 option);
	void	V5_DispGetCurFontSize(TSize *size);


	extern const UINT8 gAscFontMatrix_Mass[];
	extern const UINT8 gAscFontMatrix_Din[];
	extern const UINT32 gFontMatrixSize;

	void V5_DispSetPanelState(UINT8 state, UINT8 flag);	

#ifdef __cplusplus
}
#endif

#endif 


