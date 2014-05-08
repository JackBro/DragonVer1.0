#ifndef __V568PLANE_V568TESTCHIPDLG_H__
#define __V568PLANE_V568TESTCHIPDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif


#include "BasicFunc/BaseCtrlDlg.h"


// CV568TestChipDlg 对话框

class V568PLANE_DLL CV568TestChipDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568TestChipDlg)

public:
	CV568TestChipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568TestChipDlg();

// 对话框数据
	enum { IDD = IDD_V568TESTCHIP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	void CompDecFile();
	void CompDecFile1();
	void ChangeYuv422ToBmp(char* filename, int cx, int cy);
	void ChangeYuv422ToBmp1(char* filename, int cx, int cy);
	HRESULT GetPicInterface(char* filename, VOID** pIntf);
	void GetStillInterface(char* filename);
	void GetStillInterface1(char* filename);
	void GetThumbInterface(char* filename);
	void GetThumbInterface1(char* filename);
	void PaintStill(CDC* pDC);
	void PaintThumb(CDC* pDC);



public:
	IPicture *m_pPicStill;
	IPicture *m_pPicThumb;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnPaint();

	afx_msg void OnBnClickedButtonTest();
};

#endif
