
#ifndef _SMDEMODISP_H_
#define _SMDEMODISP_H_

#ifdef __cplusplus
extern "C" {
#endif


	#define	SMP_DISP_LOOP_NUM		10
	#define SMP_DISP_BLEND_STEP		0x3c


	typedef struct tag_TSmpFrameProp {
		char   *path;
		UINT32 keycolor;
	} TSmpFrameProp, *PTSmpFrameProp;

	typedef struct tag_TSmpDispContext {
		UINT8 rm_cur;
		UINT8 blend_cur;
		UINT8 overlay_cur;

		UINT8 awin_dir;
		UINT8 awin_step;

		TSize panel_size;
		UINT8 fore_id;
		UINT8 back_id;
	} TSmpDispContext, *PTSmpDispContext;

	enum SMPAWINenum {
		AWIN_DIR_INC,
		AWIN_DIR_DEC,
		AWIN_STEP_MIN	= 0,
		AWIN_STEP_MAX	= 0x02
	};

	enum SMPSWITCHFRAMEenum {
		SMP_PRE_FRAME,
		SMP_NEXT_FRAME
	};

	enum SMPBLENDenum {
		SMP_BLEND_INC,
		SMP_BLEND_DEC
	};

	enum SMPOVERLAYenum {
		SMP_OVERLAY_TRANS,
		SMP_OVERLAY_AND,
		SMP_OVERLAY_OR,
		SMP_OVERLAY_INVERT
	};

	enum SMPFRAMEIDenum {
		SMP_FRAME_LOGO,
		SMP_FRAME_BITBLT,

		SMP_FRAME_0,
		SMP_FRAME_1,

		SMP_FRAME_NULL,
		SMP_FRAME_TOTAL_NUM
	};

	void SmpDisp(UINT8 key, UINT8* menustring);

	void SmpDispBlend(UINT8 key, UINT8* menustring);
	void SmpDispOverlay(UINT8 key, UINT8* menustring);
	void SmpDispRM(UINT8 key, UINT8* menustring);
	void SmpDispLineDraw(UINT8 key, UINT8* menustring);
	void SmpDispBitblt(UINT8 key, UINT8* menustring);
	void SmpDispAWin(UINT8 key, UINT8* menustring);
	void SmpDispReturn(UINT8 key, UINT8* menustring);
	void SmpDispExit(UINT8 key, UINT8* menustring);

	void SmpDispBldInc(UINT8 key, UINT8* menustring);
	void SmpDispBldDec(UINT8 key, UINT8* menustring);
	void SmpDispBldReturn(UINT8 key, UINT8* menustring);
	void SmpDispBldExit(UINT8 key, UINT8* menustring);

	void SmpDispOlyTrans(UINT8 key, UINT8* menustring);
	void SmpDispOlyAnd(UINT8 key, UINT8* menustring);
	void SmpDispOlyOr(UINT8 key, UINT8* menustring);
	void SmpDispOlyInvert(UINT8 key, UINT8* menustring);
	void SmpDispOlyReturn(UINT8 key, UINT8* menustring);
	void SmpDispOlyExit(UINT8 key, UINT8* menustring);

	void SmpDispOpen(void);
	void SmpDispRefreshB0(void);
	void SmpDispRefreshB1(void);
	void SmpDispLoadFrame(UINT8 dst, UINT8 sel);
	void SmpDispLoadAndDispFrame(UINT8 sel);
	void SmpDispThumbFrame(TPoint ps, TSize size, UINT32 color, UINT8 thick_sel);

	void SmpDispTest12(UINT8 key, UINT8* menustring);
	void SmpDispTest13(UINT8 key, UINT8* menustring);
	void SmpDispTest14(UINT8 key, UINT8* menustring);
	void SmpDispTest15(UINT8 key, UINT8* menustring);


#ifdef __cplusplus
}
#endif

#endif


