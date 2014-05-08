#ifndef __BASECLASS_GAMMADLG_H__
#define __BASECLASS_GAMMADLG_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


#include "BaseDlg.h"
#include "toolbuf/srambuf.h"
	
//******** Default Value *****
#define GAMMA_PNUMMAX 100

	//how many point on map
#define GAMMA_PNUM 8
	//Map Height Width
#define GAMMA_MAPHI	300
#define GAMMA_MAPWID	500
	//Axis arrow 
#define GAMMA_ARROW_WIDTH 3
#define GAMMA_ARROW_HEIGHT 3
	//Logic X Y
#define GAMMA_LOGIC_X	1024
#define GAMMA_LOGIC_Y 255
	//Gloable state of this class
#define GAMMA_STATE_RUN 200
#define	GAMMA_STATE_INIT 201
	//Gamma coefficient
#define GAMMA_COEFF 1
	//Real =>Logic
#define GAMMA_REAL2LOGIC_X(_F) ((_F-20)*m_GammaOption.LogicX/m_GammaOption.MapWid)
#define GAMMA_REAL2LOGIC_Y(_F) ((m_GammaOption.MapHi+20-_F)*m_GammaOption.LogicY/m_GammaOption.MapHi)
	//Logic=>Real
#define GAMMA_LOGIC2REAL_X(_F) (((_F)*m_GammaOption.MapWid/m_GammaOption.LogicX)+20)
#define GAMMA_LOGIC2REAL_Y(_F) (m_GammaOption.MapHi+20-((_F)*m_GammaOption.MapHi/m_GammaOption.LogicY))


typedef struct tag_TGammaOption
{
	int PNum; //how many points on map
	int MapHi; //Map High
	int MapWid;	//Map width
	float GammaCoeff;
	//Axis's Arror shape
	int Arrow_Width; 
	int Arrow_Height;
	//X Y logic Num
	int LogicX;
	int LogicY;
 } TGammaOption,*PTGammaOption;

typedef struct tag_TGammaPoint
{
	int X;
	int Y;
}TGammaPoint,*PTGammaPoint;

typedef struct tag_TGammaFloatPoint
{
	float X;
	float Y;
}TGammaFloatPoint,*PTGammaFloatPoint;


class BASECLASS_DLL CGammaDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CGammaDlg)

public:
	CGammaDlg(CWnd* pParent = NULL); 
	virtual ~CGammaDlg();
	enum { IDD = IDD_GAMMA_DLG };

private:
	TGammaOption m_GammaOption;
	Region      *m_pYArrowRegions[GAMMA_PNUMMAX];
	PTGammaPoint		m_pGloablePoints;
	PTGammaFloatPoint		m_pGammaPoints;
	int			m_gCurrentPoint;
	int	m_gGammaStat;
	int m_Gamma_PNum;
	int m_Gamma_Map_Height;
	int m_Gamma_Map_Width;
	int m_Gamma_X;
	int m_Gamma_Y;
	float m_Gamma_Coeff;
	CWnd *m_Target;
	//save logic num of points
	CSramBuf m_bufGloablePoints;
	//save real num of points
	CSramBuf m_bufGammaPoints;

	void initGamma(TGammaOption *opt);
	void ChangeUIOption(bool direction);
	void OnGammaKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void DrawXYAndFont(Graphics &gr);
	void DrawLineAndPoints(Graphics &gr);
	void DrawGammaLine(Graphics &gr);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//init option to default 
	void InitUserOption(void);
	//get option from user input
	void GetUserOption(void);
	//Display option to dialog
	void DispOption(void);
	
public:
	// set Points before Dialog start drawing
	void SetPoints(int * XPoints, int * YPoints, int len);
	// get Points before Dialog start drawing
	void GetPoints(int * XPoints, int * YPoints, int len);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetTargetCWnd(CWnd * handle);
	void GetGammaOpt(PTGammaOption pOpt);
	void SetGammaOpt(PTGammaOption pOpt);

	DECLARE_MESSAGE_MAP()	
private:
	afx_msg void OnBnClickedApplyButton();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonGammaSave();
	afx_msg void OnBnClickedButtonGammaLoad();
	virtual BOOL OnInitDialog();

};


#endif
