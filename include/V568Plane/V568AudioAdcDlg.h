#ifndef __V568PLANE_V568AUDIOADCDLG_H__
#define __V568PLANE_V568AUDIOADCDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"

// CV568AudioAdcDlg �Ի���

class V568PLANE_DLL CV568AudioAdcDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568AudioAdcDlg)

public:
	CV568AudioAdcDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CV568AudioAdcDlg();

// �Ի�������
	enum { IDD = IDD_V568AUDIOADC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
};

#endif
