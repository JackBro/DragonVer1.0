#ifndef __BASECLASS_BASEDLG_H__
#define __BASECLASS_BASEDLG_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


// CBaseDlg �Ի���

class BASECLASS_DLL CBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CBaseDlg)

public:
	CBaseDlg(UINT Idd, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBaseDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	int		OpenFileDlg(CString *str);

//private:
//	CFont m_font;
//
//public:
//	void InitBaseFont();
//	CFont* GetBaseFont();
};


#endif
