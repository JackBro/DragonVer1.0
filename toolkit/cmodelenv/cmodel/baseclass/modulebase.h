

#ifndef _CMODEL_MODULEBASE_H_
#define _CMODEL_MODULEBASE_H_


#include "../../../tools/global.h"
#include "../../../toolfile/dbugtools.h"
#include "../../../toolparse/batchparse.h"
#include "pinbase.h"
#include "modulemap.h"
#include "pinmap.h"
#include "register.h"

#define SENSOR_8BIT		1

enum CMODEL_CLKenum
{
	CMODEL_CLKLOW,
	CMODEL_CLKRISE,
	CMODEL_CLKHIGH,
	CMODEL_CLKFALL,

	CMODEL_CLKPAUSE,
	CMODEL_CLKSTOP,
	CMODEL_CLKRESTART
};

enum CMODELSTATEenum
{
	CMODEL_FILEEND	=	BIT0,
	CMODEL_ONEFRAME	=	BIT1,
	CMODEL_NORMAL	=	BIT2,
	CMODEL_ALLOVER	=	BIT7	//loop times = 0, and the file is achieve end
};

#define MAX_INOUTPIN	8

enum CHIPREGenum
{
	REG_MODULEEN3	=	0x1fd,
	REG_MODULEEN2	=	0x1fc,
	REG_MODULEEN1	=	0x1fb,
	REG_MODULEEN0	=	0x1fa,

	REG_INPUTPINEN	=	0x1f9,
	REG_OUTPUTPINEN	=	0x1f8,

	REG_STARTPIN	=	0x1f1,
	REG_TARGETPIN	=	0x1f0
};

#define INPUT_PIN_TYPE	0x80
#define PIN_MASK		0x7f

#define MODEL_ENBIT		0x80
#define IN_PIN_MASK		0x7
#define OUT_PIN_MASK	0x38

enum MODULEIDenum
{
	IDD_V568,
	IDD_JPEG,
	IDD_LCDIF,
	IDD_SIF,
	IDD_LBUF,

	IDD_GE,
	IDD_CPM,
	IDD_HOSTIF,
	IDD_IPP,
	IDD_ISP,

	IDD_AUDIF,
	IDD_LCDC,
	IDD_MARB,
	IDD_PLLCTRL,
	IDD_STO,

	IDD_USBC,
	IDD_SENSOR,
	IDD_PANEL,
	IDD_SDCARD,
	IDD_AUDIOADC,

	IDD_TOTAL,

	REG_LOOPTEST,
	REG_SAVEREGLIST,

};

#define TOTALREGISTER	0x4000

enum CHIPBASEADDRenum
{
	BASE_V568ADDR		=	0x3000,

	BASE_JPEGADDR		=	0x0000,
	BASE_LCDIFADDR		=	0x0400,
	BASE_SIFADDR		=	0x0800,
	BASE_LBUFADDR		=	0x0C00,

	BASE_GEADDR			=	0x1000,
	BASE_CPMADDR		=	0x1400,
	BASE_HOSTIFADDR		=	0x1800,
	BASE_IPPADDR		=	0x1C00,
	BASE_ISPADDR		=	0x2000,

	BASE_AUDIFADDR		=	0x200,
	BASE_LCDCADDR		=	0x600,
	BASE_MARBADDR		=	0xa00,
	BASE_PLLCTRLADDR	=	0xe00,
	BASE_STOADDR		=	0x1200,

	BASE_USBCADDR		=	0x1600,
	BASE_SENSORADDR		=	0x1a00,
	BASE_PANELADDR		=	0x1e00,
	BASE_SDCARDADDR		=	0x2200,
	BASE_AUDIOADCADDR	=	0x2400,

	BASE_INVALIDADDR	=	0x7000
};

enum DBUGSETenum
{
	INPUT_DIRECTORY,
	OUTPUT_DIRECTORY,
	SAVE_REGLIST,

	SENSOR_INPUT_FILE,

	SIF_INPUT_FILE,
	ISP_INPUT_FILE,
	IPP_INPUT_FILE,
	LBUF_INPUT_FILE,
	JPEG_INPUT_FILE,

	SIF_PRINT_INPUT,
	ISP_PRINT_INPUT,
	IPP_PRINT_INPUT,
	LBUF_PRINT_INPUT,
	JPEG_PRINT_INPUT,

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

	IPP_EDGE_PRINT_INPUT,
	IPP_ZOOM_PRINT_INPUT,
	IPP_SPECIAL_PRINT_INPUT,
	IPP_PREFILTER_PRINT_INPUT,
	IPP_DISPSIZER_PRINT_INPUT,
	IPP_CAPSIZER_PRINT_INPUT,

	LBUF_ZOOM_PRINT_INPUT,
	LBUF_2BLOCKPRINT_INPUT,

	JPEG_ENCODE_PRINT_INPUT,

	TOTAL_KEYWORD
};

enum KEYSTRINGTYPEenum
{
	INPUTPATH,
	OUTPUTPATH,
	INPUTFILE,
	OUTPUTFILE
};

//=====================================
//========= chip v568 ================
enum MODULELISTenum
{
	V568_TOTAL_MODULES = 20
};	

typedef struct tag_TConnectMap
{
	UINT8	id1;
	UINT8	pin1;
	UINT8	id2;
	UINT8	pin2;

//	UINT8	pintype;
	int		delay;
} TConnectMap, *PTConnectMap;

typedef struct tag_TModulePara
{
	int id;
	int base;
	int module;
	int inputpin;
	int outputpin;
} TModulePara, *PTModulePara;

#define CHIP_CFGFILE	"config.txt"


//
//	use "COM" conception later to modify cmodel
//


//=============== class CModuleBase =================

class CModuleBase
{
public:
	CModuleBase(TModulePara para);
	virtual ~CModuleBase(void);

public:
	virtual void	SetConfigFile(char *cfgfile = CHIP_CFGFILE);
	virtual void	GenerateCfgFile(char *cfgfile = CHIP_CFGFILE);

	virtual void	SetRegister(int adr, int val);
	virtual int		GetRegister(int adr);

	virtual int		Process(int clk);
	virtual void	Run0(int clk);

public:
	virtual void	SetInputPin(CPinBase *ppin, int index);
	virtual void	SetOutputPin(CPinBase *ppin, int index);

	virtual	void	DeleteInputPin(CPinBase *ppin);
	virtual	void	DeleteOutputPin(CPinBase *ppin);
	virtual	void	DeleteInputPin(int index);
	virtual	void	DeleteOutputPin(int index);

	virtual void	SetMainChip(CModuleBase *pmodule);

protected:
	virtual	void	ConnectModule(const TConnectMap *bridge, CPinBase *pin);
	virtual void	SetModuleIndex(CModuleBase *pmodule, int index);	//module[0] is reserved for "this"
	virtual void	SetStartTarget(int id, int val);

	virtual void	SetClkModule(CModuleBase *pmodule);

protected:
	virtual void	SetRegister1(int adr, int val);

protected:
	virtual void	Run1(int clk);
	virtual void	Run2(int clk);
	virtual int		Run3(int clk);
	virtual	int		GetProcessState(void);
	virtual int		FindPin(int sel, TPinMap *pinmap);

protected:
	virtual void	SetReg(int adr, int val);
	virtual int		GetReg(int adr);
	virtual	void	UpdateReg(int adr);
	virtual void	SetDbugPara(TKeyStringMap *map);
	virtual	void	SaveRegList(int adr, int val);

protected:
	virtual void	SetRegPointer(CRegister *preg);
	virtual void	InitRegister(void);
	virtual void	InitRegProp1(void);
	virtual void	InitSimReg(void);


protected:
	virtual void	InitProcess(void);

	virtual	void	Disconnect(int id, int val);
	virtual	void	Disconnect(void);
	virtual void	ChipInit(TModulePara para);
	virtual void	SetRegMask(int adr, int mask);

	virtual void	ClearModuleState(void);
	virtual void	ResetModule(void);
	virtual void	InitConnectMap(void);

	virtual void	ReadInput(TStream *stream);
	virtual void	WriteOutput(TStream *stream);

protected:
	virtual void	InitModuleIndex(void) = 0;
	virtual void	InitRegProp(void) = 0;
	virtual void	OnWriteReg(int adr, int val) = 0;

	virtual int		CanRun(int clk) = 0;
	virtual void	ResetCounter(void) = 0;
	virtual void	doProcess(TStream *input, TStream *output) = 0;
	virtual void	SelfDriver(TStream *input, int index) = 0;

	virtual void	PrintPinMap(void);

protected:
	CDbugTools	m_reglist;

protected:
	CModuleBase	*m_pclk;
	CModuleBase	*m_pchip;
	CPinBase	*m_pin;

	CModuleMap	m_modules;
	int			m_start;
	int			m_target;

//	CPinMap		m_startpin;
//	CPinMap		m_targetpin;

	CPinMap		m_inputpin;
	CPinMap		m_outputpin;

private:
	int		m_id;
	int		m_base;
	int		m_state;

	CRegister	m_reg;
	CRegister	*m_preg;

};

extern const TModulePara gChipPara[];
extern const TConnectMap gV568Map[];
extern const int gV568MapCount;

#endif


