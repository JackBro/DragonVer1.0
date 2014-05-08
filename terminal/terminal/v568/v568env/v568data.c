#include "../driver/v5api.h"
#include "v5initenv.h"
#ifndef	V578CMD_DISABLE

const char *gV568BlayerFmtDesc[] =
{
	"BLAYER_RGB1", "BLAYER_RGB2", "BLAYER_RGB4", "BLAYER_RGB8",	"BLAYER_RGB444",
		"BLAYER_RGB555", "BLAYER_RGB565", "BLAYER_RGB666", "BLAYER_RGB24", "BLAYER_RGB32"
};

const int gV568Bfmtcount = sizeof(gV568BlayerFmtDesc) / sizeof(char *);

const char *gV568DitherSelDesc[] =
{
	"DITHER444", "DITHER565", "DITHER666"
};

const int gV568DitherCount = sizeof(gV568DitherSelDesc) / sizeof(char *);

const char *gV568WorkModeDesc[] =
{
	"PREVIEW_MODE",	
	"PREVIEWBRC_MODE", 
	"PREVIEWFRAME_MODE",
	"CAPTURESTILL_MODE", 
	"CAPTURESTILLTHUMB_MODE", 
	"CAPTUREVIDEO_MODE",
	"CAPTUREVIDEOTHUMB_MODE", 
	"CAPTUREAVI_MODE", 
	"CAPTUREAVITHUMB_MODE",
	"CAPTUREFRAME_MODE", 
	"CAPTUREFRAMEVIDEO_MODE",
	"CAPTUREFRAMEAVI_MODE",	
	"MULTISHOT_MODE",
	"MULTISHOTTHUMB_MODE",
	"DISPLAYSTILL_MODE",
	"ENCODE_MODE",
	"DECODE_MODE",
	"DECODEIPP_MODE",
	"DECODEFRAME_MODE",
	"MULTISHOTFRAME_MODE",
	"CAPTURETHUMB_MODE",
	"DISPLAYVIDEO_MODE",
	"DISPLAYAVI_MODE",
	"DIRECTDISPLAY_MODE",
	"BYPASS_MODE",
	"THROUGH_MODE",
	"DECODEIPPBYPASS_MODE",
	"DECODEFRAMEBYPASS_MODE",
	"DISPLAYBYPASS_MODE",
	"CAPTUREFRAMETHUMB_MODE",
	"CAPTUREFRAMEVIDEOTHUMB_MODE",
	"CAPTUREFRAMEAVITHUMB_MODE"
};

const int gV568WmodeCount = sizeof(gV568WorkModeDesc) / sizeof(char *);

const char *gV568PrevYuvmodeDesc[] =
{
	"V5B_LBUF_YUV_422", "V5B_LBUF_YUV_420", "V5B_LBUF_YUV_411", "V5B_LBUF_YUV_400"
};

const int gV568PrevYuvCount = sizeof(gV568PrevYuvmodeDesc) / sizeof(char *);

const char *gV568MemAllocmodeDesc[] =
{
	"LINEBUF_ONLY",
	"ONEFRAME_ONLY",
	"TWOFRAME_ONLY"
};

const int gV568AllocModeCnt = sizeof(gV568MemAllocmodeDesc) / sizeof(char *);

const char *gV568BusTypeDesc[] =
{
	"SEPERATE", "MULTI8", "MULTI16"
};

const int gV568BusTypeCount = sizeof(gV568BusTypeDesc) / sizeof(char *);

const char *gV568BusModeDesc[] =
{
	"MODE0", "MODE1", "MODE2", "MODE3"
};

const int gV568BusModeCount = sizeof(gV568BusModeDesc) / sizeof(char *);

const char *gV568CurSensorDesc[] =
{
	"OV_9640_BAYER",
	"OV_9640_YUV",
	"HYNIX7161SP_BAYER",
	"HYNIX7161SP_YUV",
	"OV_9650_BAYER",
	"OV_9650_YUV",
	"MT9M011_BAYER",
	"MT9M111_YUV",
	"HYNIX7131GP_BAYER"
};

const int gV568CurSensorCount = sizeof(gV568CurSensorDesc) / sizeof(char *);

const char *gV568CurLcdDesc[] =
{
	"LTS180S3_HF1_8",
	"LTS180S3_HF1",
	"LTS180SH_H1I_X",
	"TOSHIBA_LTM018A",
	"CSTN_BJ2432E_8",
	"MGG1256DTSW"
};

const int gV568CurLcdCount = sizeof(gV568CurLcdDesc) / sizeof(char *);

const char* gClkType[] =
{
	"MOD_CLK_CPM		\n",
	"MOD_CLK_LCDIF		\n",
	"MOD_CLK_LCDC		\n",
	"MOD_CLK_MARB_FAST	\n",
	"MOD_CLK_MARB		\n",
	"MOD_CLK_GE			\n",
	"MOD_CLK_JPEG_CORE	\n",
	"MOD_CLK_JPEG		\n",
	"MOD_CLK_LBUF		\n",
	"MOD_CLK_IPP		\n",
	"MOD_CLK_ISP		\n",
	"MOD_CLK_SIF		\n"
};

const char* gRstType[] =
{
	"SW_RESET_LCDIF		\n",
	"SW_RESET_LCDC		\n",
	"SW_RESET_MARB		\n",
	"SW_RESET_GE		\n",
	"SW_RESET_JPEG		\n",
	"SW_RESET_LBUF		\n",
	"SW_RESET_IPP		\n",
	"SW_RESET_SIF		\n",
	"SW_RESET_ISP		\n"
};

const UINT8 gClknum = sizeof(gClkType)/sizeof(char*);
const UINT8 gRstnum = sizeof(gRstType)/sizeof(char*);

#endif //V578CMD_DISABLE
