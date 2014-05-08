#ifndef _CMODEL_BASEUNIT_H_
#define _CMODEL_BASEUNIT_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "Inport.h"
#include "Outport.h"
#include "tooltree/treebase.h"
#include "toolfile/dbugtools.h"

enum UNITRESULTEenum {
	UNIT_IDLE,
	UNIT_DONE,
	UNIT_FAILED
};

enum PINMAPREGIONenum {
	INPUT_PIN_FLAG	=	0x1000,
	OUTPUT_PIN_FLAG	=	0x2000,
	INSIDE_BOX_FLAG	=	0x4000,
	PIN_MASK		=	0xfff,
	PINPOS_MASK		=	0x3000
};

#define MAX_PINCOUNT		10

#define DEFAULT_BLOCK_DESC	"Block"

class CMODELBASE_DLL CBaseUnit
{
public:
	CBaseUnit(int inp = 1, int oup = 1, char *desc = NULL, int size = BUFFER_DEFAULT_SIZE);
	virtual ~CBaseUnit(void);

public:
	void	PostProcess(void);
	void	Open(void);
	int		Process(void);
	int		CheckConnect(void);

public:		//must be implement in dedicate algorithm
	virtual int	 GetID(void) = 0;
	virtual void ChoiceAlg(int sel);

public:
	virtual void StoreParm(CKeyTree *ptree);
	virtual void LoadParm(CKeyTree *ptree);

protected:	//must be implement in dedicate algorithm
	virtual void UpdateConfig(void);	//update configuration
protected:
	void UpdateInputCfg(void);
	int UpdateOutputCfg(void);

protected:	//only implemented in ctrl pin algorithm, 
			//such as AWB effect in digital gain
	virtual void CtrlInPinProcess(int index, char *buf, int len);

protected:
	virtual int  CheckInpins(void);	//when check connect, check image size
									//if image size not same at different pin, should 
									//inherit it
private:
	void SetInputCfg(void);					//when check connect
	int  SetOutputCfg(void);				//when check connect

protected:			//image inherit, no need inherit
	virtual int	 DummyProcess(void);
//	virtual void SaveOption(CFileBin *fb);
//	virtual void LoadOption(CFileBin *fb);

protected:
	void	SetInportFormat(int index, int count, int *format);

private:
	int  CheckInpinStatus(void);
	int  CheckPinStatus(void);
	int  WriteAlg(void);
	int  ReadAlg(void);

public:
	void	SetInPinCount(int inpcnt);	//call it when initialize
	void	SetOuPinCount(int oupcnt);	//call it when initialize
	void	SetDesc(char *str);			// call it when initialize

protected:
	void	SetAlgStr(int count, char **str);	//call it when initialize

public:
	CInportProp *GetInport(int index);
	COuportProp *GetOuport(int index);
	int		GetInPinCount();
	int		GetOuPinCount();
	void	SetInputPin(CPinBase *pin, int index);
	CPinBase *GetInputPin(int index);
	void	SetOutputPin(CPinBase *pin, int index);
	CPinBase *GetOutputPin(int index);

public:
	int		GetTotalAlg(void);
	char	*GetAlgDesc(int index);
	int		GetCurAlg(void);

	char	*GetDesc(void);
	void	SetNum(int num);
	int		GetNum(void);

protected:
	CAlgBase	*m_pAlg;

protected:
	CInport		m_inpchain;
	COutPort	m_oupchain;
	CBufferBase m_pool;		//temp buffer, child class can set temp buffer size
	CDbugTools	m_dbg;

private:
	char	**m_pAlgstr;
	int		m_AlgCount;
	int		m_algsel;
	int		m_number;
	char	m_desc[DEFAULT_FILENAME_LEN];
};

#endif
