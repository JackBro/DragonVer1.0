#ifndef __CMODELVIEW_BOXSETBASE_H__
#define __CMODELVIEW_BOXSETBASE_H__


#include "tools/global.h"
#include "BaseClass/MDecEdit.h"
#include "CmodelBase/baseunit.h"

#ifndef _WINDLL
#define CMODELVIEW_DLL
#endif

#ifndef CMODELVIEW_DLL
#define CMODELVIEW_DLL  __declspec(dllimport)
#endif

// CBoxSetBase �Ի���

class CMODELVIEW_DLL CBoxSetBase : public CDialog
{
	DECLARE_DYNAMIC(CBoxSetBase)

public:
	CBoxSetBase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBoxSetBase();

// �Ի�������
	enum { IDD = IDD_BOXSET_DLG };

public:
	void SetBoxCwnd(CWnd *pbox);

private:
	void DispOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();

private:
	CWnd	*m_pbox;
	CMDecEdit m_InpcntEdit;
	CMDecEdit m_OupcntEdit;

protected:
	virtual void OnOK();
};

#endif
