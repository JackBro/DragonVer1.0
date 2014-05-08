

#ifndef _DBUGPRINT_H_
#define _DBUGPRINT_H_
#include "ex_OldCmodel.h"



#include "global.h"


enum {
	SIF_INPUT_FILE,
	ISP_INPUT_FILE,
	IPP_INPUT_FILE,
	LBUF_INPUT_FILE,
	JPEG_INPUT_FILE,
	LAYERB_INPUT_FILE,
	LAYERA_INPUT_FILE,
	JBUF_INPUT_FILE,

	SIF_PRINT_INPUT,
	ISP_PRINT_INPUT,
	IPP_PRINT_INPUT,
	LBUF_PRINT_INPUT,
	JPEG_PRINT_INPUT,
	LAYERB_PRINT_INPUT,
	LAYERA_PRINT_INPUT,
	JBUF_PRINTF_INPUT,

	ISP_BLACKLEVEL_PRINT_INPUT,
	ISP_TESTBAR_PRINT_INPUT,
	ISP_LENS_PRINT_INPUT,
	ISP_DPD_PRINT_INPUT,
	ISP_GAIN_PRINT_INPUT,
	ISP_INTERPOLATE_PRINT_INPUT,
	ISP_COLORCORRECT_PRINT_INPUT,
	ISP_GAMMACORRECT_PRINT_INPUT,
	ISP_COLORCONVERT_PRINT_INPUT,
	ISP_EDGEENHANCE_PRINT_INPUT,
	ISP_EDGE_PRINT_INPUT,

	IPP_EDGEPRINT_INPUT,
	IPP_ZOOM_PRINT_INPUT,
	IPP_SPECIAL_PRINT_INPUT,
	IPP_PREFILTER_PRINT_INPUT,
	IPP_DISPSIZER_PRINT_INPUT,
	IPP_CAPSIZER_PRINT_INPUT,

	LBUF_ZOOM_PRINT_INPUT,
	LBUF_2BLOCKPRINT_INPUT,

	JPEG_ENCODE_PRINT_INPUT,

	TOTAL_PRINT
};

typedef struct tag_DbugPrintMap {
	//int		mode;
	char	keychar[50];
	char	filename[50];
} DbugPrintMap, *PDbugPrintMap;

extern OLDCMODEL_DLL DbugPrintMap gDbugMap[];


#define DBUGDATAFILE	"dbugdata.txt"
#define INTERDATAFILE	"interdata.txt"

class OLDCMODEL_DLL CDbugTools1
{
public:
	CDbugTools1(void);
	virtual ~CDbugTools1(void);
	void	DbugPrintData(int mode, short int *buf, int len);
	void	DbugPrintData(int mode, ImageStream *pIn);
	void	DbugPrintData(int mode, ImageBlock *pIn);
	void	DbugMsg(const char *Format, ...);
	void	DbugData(const char *Format, ...);
	void	DbugInterData(const char *Format, ...);
	void	DbugBin(int val, int len);

private:
	FILE	*gfp;
};

extern OLDCMODEL_DLL CDbugTools1 Gdbug;



#endif


