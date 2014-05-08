

#ifndef _CHIP568_H_
#define _CHIP568_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"
#include "isp.h"
#include "sensorIF.h"
#include "ipp.h"
#include "jbuf.h"
#include "jpeg.h"
#include "lbuf.h"
#include "lcdif.h"
#include "chipctrl.h"
#include "ge.h"
#include "busif.h"


enum {
	CMODEL_SUCCEED = 0,
	CMODEL_GENERATOR_CONFIGFILE,
	CMODEL_PARSE_ERROR,

	CMODEL_FAIL
};

#define BASE_JPEGADDR		0x0000
#define BASE_LCDIFADDR		0x0400
#define BASE_SENSORIFADDR	0x0800
#define BASE_LBUFADDR		0x0C00
#define BASE_GEADDR			0x1000
#define BASE_CHIPCTRLADDR	0x1400
#define BASE_BUSIFADDR		0x1800
#define BASE_IPPADDR		0x1C00
#define BASE_ISPADDR		0x2000
#define BASE_JBUFADDR		0x2400

#define BASE_SIMADDR		0x3000


#define REGFILE		"reglist.txt"


#define SIM_STREAMWIDTH		648
#define SIM_STREAMHEIGHT	488

enum {
	SIM_WHOLECHIP,
	SIM_SIF2ISP,
	SIM_ISP2IPP,
	SIM_IPP2LBUF,
	SIM_LBUF2JPEG,
	SIM_IPP2LCDIF
};

enum {
	SIM_START_SIF,
	SIM_START_ISP,
	SIM_START_IPP,
	SIM_START_LBUF,
	SIM_START_JPEG,
};

#define NOTSAVEREGLIST	0
#define SAVEREGLIST		1

//=======	simulation selection ============
#define REG_SIM_MODE			0x0
#define REG_LOOP_TEST			0x1
#define REG_SIM_SAVEREGLIST		0x2
#define REG_SIM_STREAMWIDTH_L	0x4
#define REG_SIM_STREAMWIDTH_H	0x5
#define REG_SIM_STREAMHEIGHT_L	0x6
#define REG_SIM_STREAMHEIGHT_H	0x7
#define REG_SIM_FORMAT			0x8








class OLDCMODEL_DLL CCmodelSim : public CChipBase1
{
public:
	CCmodelSim(void);
	virtual ~CCmodelSim(void);

	void	SetReg(int adr, int val);
	int		GetReg(int adr);
	void	RegInit(void);

	void	Process(void);
	int		ConfigInit(char *configfile);

protected:
	void	SimSetReg(int adr, int val);
	int		SimGetReg(int adr);
	void	SimRegInit(void);

	int		ConfigParse(char *buf, int len, int *offset);
	void	ConfigGenerator(char *configfile);
	void	ClearDataFile(void);
	void	SaveRegList(int adr, int val);
	int		FillConfig(int count, char * item[]);

	void	ClearEdge(void);
	void	InitStreamHeader(ImageStream *pIs);
	int		SendFirstLine(CChipBase1 *pModule, ImageStream *pIs);
	int		SendLine(CChipBase1 *pModule, ImageStream *pIs);
	int		SendLastLine(CChipBase1 *pModule, ImageStream *pIs);

	void	ProcessSim(void);

private:					//tempory context
	int		gInputLine;
	int		gSelect;

private:					//stream between each module
	ImageStream		gEdge;
	ImageBlock		gLbufBlock;
	ImageBlock		gJbufBlock;

private:					//module
	CSensorIF	gSif;
	CIsp		gIsp;
	CIpp		gIpp;
	CJbuf		gJbuf;
	CJpeg		gJpeg;
	CLbuf		gLbuf;
	CLcdIF		gLcdIF;
	CChipCtrl	gChipCtrl;
	CGE			gGE;
	CBusIF		gBusIF;

	CChipBase1	*gpModule;
};

#endif


