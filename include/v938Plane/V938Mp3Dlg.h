#ifndef __V938PLANE_V938MP3DLG_H__
#define __V938PLANE_V938MP3DLG_H__


#ifndef V938PLANE_DLL
	#define V938PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"


// CV938SeqDlg �Ի���

class V938PLANE_DLL CV938Mp3Dlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV938Mp3Dlg)

public:
	CV938Mp3Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CV938Mp3Dlg();

// �Ի�������
	enum { IDD = IDD_V938MP3_DLG };

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
