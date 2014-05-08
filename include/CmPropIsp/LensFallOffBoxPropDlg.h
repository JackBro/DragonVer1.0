#ifndef _CMODEL_LENSFALLOFF_PROPDLG_H_
#define _CMODEL_LENSFALLOFF_PROPDLG_H_


#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"



typedef struct tag_TLensFallOffOption
{
	int lensCx;
	int lensCy;
	int lensFocus;
} TLensFallOffOption, *PTLensFallOffOption;



class CMODELISPBOXPROP_DLL CLensFallOffBoxPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CLensFallOffBoxPropDlg)

public:
	CLensFallOffBoxPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLensFallOffBoxPropDlg();

// 对话框数据 
	enum { IDD = IDD_LENSFALLOFFBOXPROP_DLG };

private:
	void InitUserOption(void);
	void GetUserOption(void);
	void DispUserOption(void);
private:
	TLensFallOffOption m_option;

	CMHexEdit m_lenscxEdit;
	CMHexEdit m_lenscyEdit;
	CMHexEdit m_lensfocusEdit;

protected:
	virtual void UpdateOption(void);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
public:

};

#endif

