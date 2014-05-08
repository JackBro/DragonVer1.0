#ifndef _CMODEL_ALG_DIGITAL_GAIN_PROPDLG_H_
#define _CMODEL_ALG_DIGITAL_GAIN_PROPDLG_H_



#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"


typedef struct tag_TDigitalGainOption
{
	int rgain;
	int bgain;
	int ggain;
	int globalgain;
	int gainstep;
} TDigitalGainOption, *PTDigitalGainOption;

class CMODELISPBOXPROP_DLL CDigitalGainBoxPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CDigitalGainBoxPropDlg)

public:
	CDigitalGainBoxPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDigitalGainBoxPropDlg();

// 对话框数据
	enum { IDD = IDD_GAINBOXPROP_DLG };

private:
	void InitUserOption(void);
	void GetUserOption(void);
	void DispUserOption(void);

private:
	TDigitalGainOption m_option;

	CMHexEdit m_rgainEdit;
	CMHexEdit m_ggainEdit;
	CMHexEdit m_bgainEdit;
	CMHexEdit m_globalgainEdit;
	CMHexEdit m_gainStep;
	CButton   m_globalCheck;
	CButton   m_rgbCheck;
	

protected:
	virtual void UpdateOption(void);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	afx_msg BOOL OnInitDialog();
};

#endif
