#ifndef __V568PLANE_V568LBUFDLG_H__
#define __V568PLANE_V568LBUFDLG_H__



#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif



#include "BasicFunc/BaseCtrlDlg.h"


// CV568LbufDlg �Ի���

class V568PLANE_DLL CV568LbufDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568LbufDlg)

public:
	CV568LbufDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CV568LbufDlg();

// �Ի�������
	enum { IDD = IDD_V568LBUF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
