
#ifndef _CMODEL_VSIZER_PROPDLG_H_
#define _CMODEL_VSIZER_PROPDLG_H_


#ifndef CMODELIPPPROP_DLL
	#define CMODELIPPPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MComboBox.h"


typedef struct tag_THsizerOption
{
	int pal;
} THsizerOption, *PTHsizerOption;


class CMODELIPPPROP_DLL CVsizerBoxPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CVsizerBoxPropDlg)

public:
	CVsizerBoxPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVsizerBoxPropDlg();

	// 对话框数据 
	enum { IDD = IDD_VSIZERPROP_DLG };

private:
	void InitUserOption(void);
	void GetUserOption(void);
	void DispUserOption(void);

protected:
	virtual void UpdateOption(void);

private:
	THsizerOption	m_option;
	CMComboBox		m_palbox;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
public:

};












#endif