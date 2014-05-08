

#ifndef _CMODEL_BOXDATA_H_
#define _CMODEL_BOXDATA_H_


#ifndef CMODELBOXDATA_DLL
	#define CMODELBOXDATA_DLL  __declspec(dllimport)
#endif


enum BOXCLASSenum
{
	ISPCLASS,
	LOGICCLASS,
	IPPCLASS,
	STILLCLASS
};


enum ISP_BLACKBOXenum {
	AUTOFOCUS_BOX,
	BLACKLEVEL_BOX,
	CFAEXTRACT_BOX,
	COLORCONVERT_BOX,
	COLORCORRECT_BOX,
	DIGITALGAIN_BOX,
	DPDDPC_BOX,
	EDGEENHANCE_BOX,
	GAMMACORRECT_BOX,
	LENSFALLOFF_BOX,
	TESTPATTERN_BOX
};

enum LOGIC_BLACKBOXenum {
	LOGICALANDGATE_BOX,
	LOGICALCOMGATE_BOX,	
	LOGICALNOTGATE_BOX,
	LOGICALORGATE_BOX,
	LOGICALXORGATE_BOX,
	ADDGATE_BOX,
	SUBGATE_BOX,
	ADD2GATE_BOX,
	DELAYCELL_BOX,
	ARITHMGATE_BOX
};

enum IPP_BLACKBOXenum {
	VERSIZER_BOX,
	HORSIZER_BOX
};

enum STILL_BLACKBOXenum {
	BMP2RGB_BOX,
	RGB2YUV_BOX,
	YUV2YUV_BOX,
	YUV2RGB_BOX,
	YUVDIV_BOX,
	YUVMERGE_BOX,
	FMTCONVERT_BOX
};

typedef struct tag_TFormatGroup {
	char *cname;
	char **format;
	int	 fmtcount;
} TFormatGroup, *PTFormatGroup;

//extern CMODELBOXDATA_DLL const char *gBoxClassType[];
//extern CMODELBOXDATA_DLL const int  gTotalBoxClassType;

//extern CMODELBOXDATA_DLL const char *gIsp_BoxDesc[];
//extern CMODELBOXDATA_DLL const int  gIspTotalBoxCount;
//
//extern CMODELBOXDATA_DLL const char *gLogic_BoxDesc[];
//extern CMODELBOXDATA_DLL const int  gLogicTotalBoxCount;
//
//extern CMODELBOXDATA_DLL const char *gIpp_BoxDesc[];
//extern CMODELBOXDATA_DLL const int  gIppTotalBoxCount;
//
//extern CMODELBOXDATA_DLL const char *gStill_BoxDesc[];
//extern CMODELBOXDATA_DLL const int gStillTotalBoxCount;

extern CMODELBOXDATA_DLL const TFormatGroup g_fmtGroup[];
extern CMODELBOXDATA_DLL const int g_FmtGroupTotalCount;

extern CMODELBOXDATA_DLL const int  gDefaultFmt;
extern CMODELBOXDATA_DLL const char *gDefaultAlgType[];

#endif

