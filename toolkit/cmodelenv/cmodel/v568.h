

#ifndef _CMODEL_V568_H_
#define _CMODEL_V568_H_

#include "./baseclass/chipbase.h"
#include "audif.h"
#include "pllctrl.h"
#include "sensor.h"
#include "sif.h"
#include "isp.h"
#include "cpm.h"
#include "ge.h"
#include "hostif.h"
#include "ipp.h"
#include "jpeg.h"
#include "lbuf.h"
#include "lcdc.h"
#include "lcdif.h"
#include "marb.h"
#include "sto.h"
#include "usbc.h"
#include "panel.h"
#include "sdcard.h"
#include "audioadc.h"


//
//	use "COM" conception later to modify cmodel
//


//=============== class CV568Chip =================

class CV568Chip : public CChipBase
{
public:
	CV568Chip();
	virtual ~CV568Chip();

public:

protected:
	virtual void	InitModuleIndex(void);
	virtual void	OnWriteReg(int adr, int val);
	virtual void	InitRegProp(void);
	virtual int		CanRun(int clk);
	virtual void	doProcess(TStream *input, TStream *output);
	virtual void	SelfDriver(TStream *input, int index);
	virtual void	ResetCounter(void);


private:

private:

	CSensorModule	m_sensor;
	CIspModule		m_isp;
	CSifModule		m_sif;
	CCpmModule		m_cpm;
	CAudioIFModule	m_audif;

	CGeModule		m_ge;
	CHostIFModule	m_hostif;
	CIppModule		m_ipp;
	CJpegModule		m_jpeg;
	CLbufModule		m_lbuf;

	CLcdcModule		m_lcdc;
	CLcdIFModule	m_lcdif;
	CMarbModule		m_marb;
	CPllCtrlModule	m_pll;
	CStoModule		m_sto;

	CUsbcModule		m_usbc;
	CPanelModule	m_panel;
	CSdcardModule	m_sdcard;
	CAudioAdcModule	m_audadc;
};


#endif


