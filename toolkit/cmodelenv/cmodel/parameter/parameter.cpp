
#include "../baseclass/chipbase.h"
#include "../../../toolparse/batchparse.h"

TKeyStringMap gKeyMap[] =
{
	//	Index				key word				value	pin index		pin property
	{ INPUT_DIRECTORY,		"InputDirectory",		{'\0'}, IDD_TOTAL << 8, INPUTPATH },	
	{ OUTPUT_DIRECTORY,		"OutputDirectory",		{'\0'}, IDD_TOTAL << 8, OUTPUTPATH },	
	{ SAVE_REGLIST,			"SaveRegListFile",		{'\0'}, IDD_TOTAL << 8, OUTPUTFILE },

	{ SENSOR_INPUT_FILE,	"SensorInputFile",		{'\0'}, (IDD_SENSOR << 8) + (0 | INPUT_PIN_TYPE), INPUTFILE },	

	{ SIF_INPUT_FILE,		"SifInputFile",			{'\0'}, (IDD_SIF << 8) + (0 | INPUT_PIN_TYPE), INPUTFILE },	
	{ ISP_INPUT_FILE,		"IspInputFile",			{'\0'}, (IDD_ISP << 8) + (0 | INPUT_PIN_TYPE), INPUTFILE },	
	{ IPP_INPUT_FILE,		"IppInputFile",			{'\0'}, (IDD_IPP << 8) + (0 | INPUT_PIN_TYPE), INPUTFILE },	
	{ LBUF_INPUT_FILE,		"LbufInputFile",		{'\0'}, (IDD_LBUF << 8) + (0 | INPUT_PIN_TYPE), INPUTFILE },	
	{ JPEG_INPUT_FILE,		"JpegInputFile",		{'\0'}, (IDD_JPEG << 8) + (0 | INPUT_PIN_TYPE), INPUTFILE },	
	  
	{ SIF_PRINT_INPUT,		"SifPrintInput",		{'\0'}, (IDD_SIF << 8) + (0 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ ISP_PRINT_INPUT,		"IspPrintInput",		{'\0'}, (IDD_ISP << 8) + (0 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ IPP_PRINT_INPUT,		"IppPrintInput",		{'\0'}, (IDD_IPP << 8) + (0 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ LBUF_PRINT_INPUT,		"LbufPrintInput",		{'\0'}, (IDD_LBUF << 8) + (0 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ JPEG_PRINT_INPUT,		"JpegPrintInput",		{'\0'}, (IDD_JPEG << 8) + (0 | INPUT_PIN_TYPE), OUTPUTFILE },	
	  
	{ ISP_BLACKLEVEL_PRINT_INPUT,	"IspBlackPrintInput",	{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },
	{ ISP_TESTBAR_PRINT_INPUT,		"IspTestbarPrintInput",	{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ ISP_LENS_PRINT_INPUT,			"IspLensPrintInput",	{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ ISP_DPD_PRINT_INPUT,			"IspDpdPrintInput",		{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ ISP_GAIN_PRINT_INPUT,			"IspGainPrintInput",	{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ ISP_INTERPOLATE_PRINT_INPUT,	"IspInterpolatePrintInput",	{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },
	{ ISP_COLORCORRECT_PRINT_INPUT,	"IspColorCorrectPrintInput",{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },
	{ ISP_GAMMACORRECT_PRINT_INPUT,	"IspGammaCorrectPrintInput",{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },
	{ ISP_COLORCONVERT_PRINT_INPUT,	"IspColorConvertPrintInput",{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },
	{ ISP_EDGEENHANCE_PRINT_INPUT,	"IspEdgeEnhancePrintInput",{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ ISP_EDGE_PRINT_INPUT,			"IspEdgePrintInput",	{'\0'}, (IDD_ISP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	  
	{ IPP_EDGE_PRINT_INPUT,			"IppEdgePrintInput",	{'\0'}, (IDD_IPP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ IPP_ZOOM_PRINT_INPUT,			"IppZoomPrintInput",	{'\0'}, (IDD_IPP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ IPP_SPECIAL_PRINT_INPUT,		"IppSpecialPrintInput",	{'\0'}, (IDD_IPP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ IPP_PREFILTER_PRINT_INPUT,	"IppPrefilterPrintInput",{'\0'}, (IDD_IPP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },	
	{ IPP_DISPSIZER_PRINT_INPUT,	"IppDisplayPrintInput",	{'\0'}, (IDD_IPP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },		
	{ IPP_CAPSIZER_PRINT_INPUT,		"IppCapturePrintInput",	{'\0'}, (IDD_IPP << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },		
	  
	{ LBUF_ZOOM_PRINT_INPUT,		"LbufZoomPrintInput",	{'\0'}, (IDD_LBUF << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },		
	{ LBUF_2BLOCKPRINT_INPUT,		"Lbuf2BlockPrintInput",	{'\0'}, (IDD_LBUF << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE },		
	  
	{ JPEG_ENCODE_PRINT_INPUT,		"JpegEncodePrintInput",	{'\0'}, (IDD_JPEG << 8) + (1 | INPUT_PIN_TYPE), OUTPUTFILE }		
};

const int gKeyCount = sizeof(gKeyMap) / sizeof(TKeyStringMap);

//==============================================
//
//==============================================

const TConnectMap gV568Map[] =
{
	{ IDD_SENSOR,	0, IDD_SIF,		0 | INPUT_PIN_TYPE, 1 },	// sif
	{ IDD_SIF,		0, IDD_ISP,		0 | INPUT_PIN_TYPE, 1 },	//   isp
	{ IDD_ISP,		0, IDD_IPP,		0 | INPUT_PIN_TYPE, 1 },	//	   ipp
	{ IDD_ISP,		1, IDD_IPP,		1 | INPUT_PIN_TYPE, 1 },	//     ipp
	{ IDD_IPP,		0, IDD_LBUF,	0 | INPUT_PIN_TYPE, 1 },	//       lbuf
	{ IDD_IPP,		1, IDD_LCDC,	0 | INPUT_PIN_TYPE, 1 },	//         lcdc
	{ IDD_IPP,		2, IDD_MARB,	0 | INPUT_PIN_TYPE, 1 },	//	         marb
	{ IDD_LBUF,		0, IDD_JPEG,	0 | INPUT_PIN_TYPE, 1 },	//             jpeg
	{ IDD_LBUF,		1, IDD_IPP,		2 | INPUT_PIN_TYPE, 1 },	//     ipp
	{ IDD_JPEG,		0, IDD_MARB,	1 | INPUT_PIN_TYPE, 1 },	//           marb
	{ IDD_JPEG,		1, IDD_LBUF,	1 | INPUT_PIN_TYPE, 1 },	//       lbuf
	{ IDD_LCDC,		0, IDD_LBUF,	2 | INPUT_PIN_TYPE, 1 },	//       lbuf
	{ IDD_LCDC,		1, IDD_LCDIF,	0 | INPUT_PIN_TYPE, 1 },	//               lcdif
	{ IDD_LCDC,		2, IDD_MARB,	2 | INPUT_PIN_TYPE, 1 },	//           marb
	{ IDD_MARB,		0, IDD_JPEG,	1 | INPUT_PIN_TYPE, 1 },	//             jpeg
	{ IDD_MARB,		1, IDD_LCDC,	1 | INPUT_PIN_TYPE, 1 },	//         lcdc
	{ IDD_MARB,		2, IDD_GE,		1 | INPUT_PIN_TYPE, 1 },	//                 ge
	{ IDD_MARB,		3, IDD_STO,		0 | INPUT_PIN_TYPE, 1 },	//               sto
	{ IDD_MARB,		4, IDD_USBC,	0 | INPUT_PIN_TYPE, 1 },	//				     usbc
	{ IDD_GE,		0, IDD_MARB,	3 | INPUT_PIN_TYPE, 1 },	//           marb
	{ IDD_LCDIF,	0, IDD_PANEL,	0 | INPUT_PIN_TYPE, 1 },	//                     panel
	{ IDD_AUDIF,	0, IDD_MARB,	4 | INPUT_PIN_TYPE, 1 },	//           marb
	{ IDD_STO,		0, IDD_MARB,	5 | INPUT_PIN_TYPE, 1 },	//           marb
	{ IDD_STO,		0, IDD_SDCARD,	0 | INPUT_PIN_TYPE, 1 },	//                       sdcard
	{ IDD_SDCARD,	0, IDD_STO,		1 | INPUT_PIN_TYPE, 1 },	//               sto
	{ IDD_USBC,		0, IDD_MARB,	6 | INPUT_PIN_TYPE, 1 }	//           marb
};

const int gV568MapCount = sizeof(gV568Map) / sizeof(TConnectMap);

//==============================================
//
//==============================================

const TModulePara gChipPara[] =
{
	{ IDD_V568		,	BASE_V568ADDR,	V568_TOTAL_MODULES, 1, },
	{ IDD_JPEG		,	BASE_JPEGADDR,	1, 2, 2 },
	{ IDD_LCDIF		,	BASE_LCDIFADDR, 1, 1, 1 },
	{ IDD_SIF		,	BASE_SIFADDR,	1, 1, 1 },
	{ IDD_LBUF		,	BASE_LBUFADDR,	1, 3, 2 },

	{ IDD_GE		,	BASE_GEADDR,	1, 1, 1 },
	{ IDD_CPM		,	BASE_CPMADDR,	1, 1, 1 },
	{ IDD_HOSTIF	,	BASE_HOSTIFADDR,1, 1, 1 },
	{ IDD_IPP		,	BASE_IPPADDR,	1, 3, 3 },
	{ IDD_ISP		,	BASE_ISPADDR,	1, 1, 2 },

	{ IDD_AUDIF		,	BASE_AUDIFADDR, 1, 1, 1 },
	{ IDD_LCDC		,	BASE_LCDCADDR,	1, 1, 1 },
	{ IDD_MARB		,	BASE_MARBADDR,	1, 7, 5 },
	{ IDD_PLLCTRL	,	BASE_PLLCTRLADDR, 1, 1, 1 },
	{ IDD_STO		,	BASE_STOADDR,	1, 2, 2 },

	{ IDD_USBC		,	BASE_USBCADDR,	1, 1, 1 },
	{ IDD_SENSOR	,	BASE_SENSORADDR,1, 1, 1 },
	{ IDD_PANEL		,	BASE_PANELADDR, 1, 1, 1 },
	{ IDD_SDCARD	,	BASE_SDCARDADDR,1, 1, 1 },
	{ IDD_AUDIOADC	,	BASE_AUDIOADCADDR, 1, 1, 1 }
};

