
#ifndef _CMODEL_702TVENC_H_
#define _CMODEL_702TVENC_H_



#ifndef TOOLV702ALG_DLL
	#define TOOLV702ALG_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"

#include "toolconvert/line422buf.h"
#include "toolconvert/line444buf.h"



#define NTSCPAL_WIDTH	720

const int FIRCoeff_y[2][11] = 
{
	{0, 0, 0, 0, 0, 272, 0, 0, 0, 0, 0},	//NTSC coeff

	{0, 0, 0, 0, 0, 289, 0, 0, 0, 0, 0}		//PAL coeff
};

const int FIRCoeff_u[2][7] = 
{
	{2, 32, 52, 64, 52, 32, 2},				//NTSC coeff

	{2, 34, 56, 67, 56, 34, 2}				//PAL coeff
};

const int FIRCoeff_v[2][7] = 
{
	{3, 46, 74, 90, 74, 46, 3},				//NTSC coeff

	{3, 49, 78, 95, 78, 49, 3}				//PAL coeff
};

enum TVencFILTERenum
{
	FILTER_NONE,
	FILTER_Y,
	FILTER_U,
	FILTER_V
};

enum TVencUPSAMPLEenum
{
	UPSAMPLE_NONE,
	UPSAMPLE_Y,
	UPSAMPLE_U,
	UPSAMPLE_V
};

typedef struct tag_TCompOption {
	int		tv_colmax;
	int		tv_rowmax;
	int		vs_pulse;
	int		ve1_pulse;
	int		ve2_pulse;
	int		vb_pulse;
	int		vs_duration;
	int		ve_duration;
	int		hs_duration;
	int		burst_start;
	int		burst_end;
	int		sync_level;
	int		black_level;
	int		burst_level;
	int		chrom_level;
	int		col_delta;
	int		row_delta;
	int		data_start;
	int		front_porch;
	int		first_field;	
	int		subfreq;
	unsigned int thta;
	
} TCompOption, PTCompOption;

enum PALOPTIONenum {
	TV_COLMAX_PAL		=	1728,
	TV_ROWMAX_PAL		=	625,
	VS_PULSE_PAL		=	5,
	VE1_PULSE_PAL		=	5,
	VE2_PULSE_PAL		=	5,
	VB_PULSE_PAL		=	45,
	VS_DURATION_PAL		=	737,
	VE_DURATION_PAL		=	63,
	HS_DURATION_PAL		=	127,
	BURST_START_PAL		=	153,	//add 2 from definition in verilog 
	BURST_END_PAL		=	214,	//add 2 from definition in verilog 
	SYNC_LEVEL_PAL		=	248,
	BLACK_LEVEL_PAL		=	0,
	BURST_LEVEL_PAL		=	87,
	CHROM_LEVEL_PAL		=	182,
	COL_DELTA_PAL		=	47,
	ROW_DELTA_PAL		=	6,
	DATA_START_PAL		=	286,	//add 2 from definition in verilog 
	FRONT_PORCH_PAL		=	39		//sub 2 from definition in verilog
};

enum NTSCOPTIONenum {
	TV_COLMAX_NTSC		=	1716,
	TV_ROWMAX_NTSC		=	525,
	VS_PULSE_NTSC		=	6,
	VE1_PULSE_NTSC		=	6,
	VE2_PULSE_NTSC		=	6,
	VB_PULSE_NTSC		=	42,		//in doc, the value is 34, maybe here is new version
	VS_DURATION_NTSC	=	732,
	VE_DURATION_NTSC	=	62,
	HS_DURATION_NTSC	=	127,
	BURST_START_NTSC	=	145,	//add 2 from definition in verilog 
	BURST_END_NTSC		=	217,	//add 2 from definition in verilog 
	SYNC_LEVEL_NTSC		=	236,
	BLACK_LEVEL_NTSC	=	44,
	BURST_LEVEL_NTSC	=	118,
	CHROM_LEVEL_NTSC	=	186,
	COL_DELTA_NTSC		=	18,
	ROW_DELTA_NTSC		=	1,
	DATA_START_NTSC		=	265,	//add 2 from definition in verilog 
	FRONT_PORCH_NTSC	=	44		//sub 2 from definition in verilog
};

/*====================================================================================================*/
//			class	C702Filter
/*====================================================================================================*/

class TOOLV702ALG_DLL C702Filter : public CBaseObject
{
public:
	C702Filter(int width = 720, int height = 600, int pal = 1, int format = YUV422_YUYV);
	virtual ~C702Filter();

public:
	int		Process(char *src, char *dst);
	void	Init(int width = 720, int height = 600, int pal = 1, int format = YUV422_YUYV);

protected:
	int		Filter(char *src, char *dst);
	int		Write(char *buf, int len);
	int		Read(char *buf, int len);
	int		YProc(char *buf, int len);
	int		UProc(char *buf, int len);
	int		VProc(char *buf, int len);

	void	GetCompData(int *pval);

private:
	int		m_width, m_height;
	int		m_flg, m_format, m_pal;
	int		m_yfilter[11];
	int		m_ufilter[7];
	int		m_vfilter[7];
	
	char	m_firstval[12];
	char	m_lastval[12];
	CLine422Buf		m_linebuf;

};

/*====================================================================================================*/
//			class	C702Upsample
/*====================================================================================================*/

class TOOLV702ALG_DLL C702Upsample : public CBaseObject
{
public:
	C702Upsample(int width = 720, int height = 600, int unit = 2, int format = YUV422_YUYV);
	virtual ~C702Upsample();

public:
	int		Process(char *src, char *dst);
	void	Init(int width = 720, int height = 600, int unit = 2, int format = YUV422_YUYV);

protected:
	int		UpSample(char *src, char *dst);
	int		YProc(char *buf, int len);	
	int		UVProc(char *buf, int len);
	int		Write(char *buf, int len);
	int		Read(char *buf, int len);

	void	GetCompData(int *buf);
	void	GetLastval(char *buf, int totalsize, int pos);

private:
	int		m_width, m_height;
	int		m_unit;
	int		m_format, m_flg;
	int		m_lastval[3];
	CLine422Buf		m_linebuf;
};


/*====================================================================================================*/
//			class	C702Composite
/*====================================================================================================*/

class TOOLV702ALG_DLL C702Composite : public CBaseObject
{
public:
	C702Composite(int width = 1440, int height = 300, int pal = 1, int interlace = 1, int unit = 2);
	virtual ~C702Composite();

public:
	void	Init(int width = 1440, int height = 300, int pal = 1, int interlace = 1, int unit = 2);
	int		Process(char *src, char *dst);

protected:
	int		NtscProc(char *src, char *dst);
	int		PalProc(char *src, char *dst);

private:
	void	GetSinCos(int *val_sin, int *val_cos);
	int		SyncProc(char *buf, int param1, int param2, int flag = 0);
	int		SubProc(char *buf, int param);

private:
	int		m_width, m_height;
	int		m_interlace, m_unit, m_pal;
	double	m_factor;
	TCompOption		m_option;
	CLine444Buf		m_linebuf;
};

#endif

