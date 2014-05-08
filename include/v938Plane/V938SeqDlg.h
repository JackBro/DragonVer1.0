#ifndef __V938PLANE_V938SEQDLG_H__
#define __V938PLANE_V938SEQDLG_H__


#ifndef V938PLANE_DLL
	#define V938PLANE_DLL _declspec(dllimport)
#endif


#include "BasicFunc/BaseCtrlDlg.h"


// CV938SeqDlg �Ի���

class V938PLANE_DLL CV938SeqDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV938SeqDlg)

public:
	CV938SeqDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CV938SeqDlg();

// �Ի�������
	enum { IDD = IDD_V938SEQ_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void DispUserOption(void);
	void GetUserOption(void);
	void InitUserOption(void);

private:	
    
	DECLARE_MESSAGE_MAP()

public:
	
};

#endif
