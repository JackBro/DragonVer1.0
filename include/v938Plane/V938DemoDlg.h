#ifndef __V938PLANE_V938DEMODLG_H__
#define __V938PLANE_V938DEMODLG_H__


#ifndef V938PLANE_DLL
	#define V938PLANE_DLL _declspec(dllimport)
#endif


#include "BasicFunc/BaseCtrlDlg.h"



typedef struct tag_TDemo938Option
{
	/*file name*/
	char	        srcfile[DEFAULT_FILENAME_LEN];
	char	        dstfile[DEFAULT_FILENAME_LEN];

} TDemo938Option, *PTDemo938Option;

// CV938SeqDlg 对话框

class V938PLANE_DLL CV938DemoDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV938DemoDlg)

public:
	CV938DemoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV938DemoDlg();

// 对话框数据
	enum { IDD = IDD_V938DEMO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void DispUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void InitUserOption(void);

private:	
    
	DECLARE_MESSAGE_MAP()

private:
	TDemo938Option m_option;

public:
	
	afx_msg void OnBnClickedOpen938Button();
};

#endif
