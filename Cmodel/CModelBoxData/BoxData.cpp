
#include "internal.h"




static const char *gIsp_BoxDesc[] =
{	
	"Auto Focus",
	"Black Level",
	"Cfa Extract",
	"Color Convert",
	"Color Correct",
	"Digital Gain",
	"Dpd Dpc",
	"Edge Enhance",
	"Gamma Correct",
	"Lens FallOff",
	"Test Pattern"	
};
//static const int gIspTotalBoxCount = sizeof(gIsp_BoxDesc) / sizeof(char *);


static const char *gLogic_BoxDesc[] =
{	
	"LogicAnd Gate",
	"LogicCom Gate",
	"LogicNot Gate",
	"LogicOr Gate",
	"LogicXor Gate",
	"Add Gate",
	"Sub Gate",
	"Add2 Gate",
	"Delay Cell",
	"Arithmetic Gate"
};


//static const int gLogicTotalBoxCount = sizeof(gLogic_BoxDesc) / sizeof(char *);

static const char *gIpp_BoxDesc[] = 
{
	"Vertical Sizer",
	"Horizontal Sizer"
};
//static const int gIppTotalBoxCount = sizeof(gIpp_BoxDesc) / sizeof(char *);

static const char *gStill_BoxDesc[] = 
{
	"Bmp2Rgb",
	"Rgb2Yuv",
	"Yuv2Yuv",
	"Yuv2Rgb",
	"YuvDiv",
	"YuvMerge",
	"FmtConvert"
};
//static const int gStillTotalBoxCount = sizeof(gStill_BoxDesc) / sizeof(char *);

const TFormatGroup g_fmtGroup[] =
{
	{ "ISPCLASS", (char **)gIsp_BoxDesc, sizeof(gIsp_BoxDesc) / sizeof(char *) },
	{ "LOGICCLASS", (char **)gLogic_BoxDesc, sizeof(gLogic_BoxDesc) / sizeof(char *) },
	{ "IPPCLASS", (char **)gIpp_BoxDesc, sizeof(gIpp_BoxDesc) / sizeof(char *) },
	{ "STILLCLASS", (char **)gStill_BoxDesc, sizeof(gStill_BoxDesc) / sizeof(char *) }
};
const int g_FmtGroupTotalCount = sizeof(g_fmtGroup) / sizeof(TFormatGroup);

const int gDefaultFmt = UNKNOWNSTREAM | (SYSTEMCLASS << CLASSSHIFT);

const char *gDefaultAlgType[] = { "Only one" };



