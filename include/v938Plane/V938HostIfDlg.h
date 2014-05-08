#ifndef __V938PLANE_V938HOSTIFDLG_H__
#define __V938PLANE_V938HOSTIFDLG_H__


#ifndef V938PLANE_DLL
	#define V938PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MHexEdit.h"

typedef struct tag_THOSTIF938Option
{
	/*file name*/
	char	        srcfile[DEFAULT_FILENAME_LEN];
	char	        dstfile[DEFAULT_FILENAME_LEN];

	/*sram write/read*/
	unsigned int    startAddr;
    unsigned int    size;

} THOSTIF938Option, *PTHOSTIF938Option;

// CV938HostIfDlg 对话框

class V938PLANE_DLL CV938HostIfDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV938HostIfDlg)

public:
	CV938HostIfDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV938HostIfDlg();

// 对话框数据
	enum { IDD = IDD_V938HOSTIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void DispUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void InitUserOption(void);

private:
	THOSTIF938Option    m_option;
	CMHexEdit           m_startAddrBox;
	CMHexEdit           m_sizeBox;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedV938hostifwritesramButton();
	afx_msg void OnBnClickedV938hostifreadsramButton();
};

#endif
