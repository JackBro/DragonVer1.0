// GammaDlg.cpp : Draw Gamma Dialog
//Create by Calilon Huang 
//2005.5.30 Created
//2005.06.03 Add Apply Save Load funcs
//2005.06.06 Add Keyboard support. Up Down Left Right to help adjust points.
//&& change from saving real point to saving logic point, what enable you change
//logic value 1 by 1. If count in real pixel, step some times will be larger than 1.
//2005.06.07 Add keyboard supoort. Add gamma real line support.
//
#include "internal.h"

IMPLEMENT_DYNAMIC(CGammaDlg, CBaseDlg)
CGammaDlg::CGammaDlg(CWnd* pParent /*NULL*/)
	: CBaseDlg(CGammaDlg::IDD, pParent)
{
	m_pGloablePoints = NULL;
	m_bufGloablePoints.SetSize(GAMMA_PNUMMAX*sizeof(TGammaPoint),1);
	m_bufGammaPoints.SetSize(GAMMA_PNUMMAX*sizeof(TGammaFloatPoint)*4,1);
	m_pGloablePoints = (PTGammaPoint)(m_bufGloablePoints.m_buf);
	m_pGammaPoints	=	(PTGammaFloatPoint)(m_bufGammaPoints.m_buf);
	this->InitUserOption();
}

CGammaDlg::~CGammaDlg()
{
}

void CGammaDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GAMMA_PNUM, m_Gamma_PNum);
	DDX_Text(pDX, IDC_GAMMA_MAP_HEIGHT, m_Gamma_Map_Height);
	DDX_Text(pDX, IDC_GAMMA_MAP_WIDTH, m_Gamma_Map_Width);
	DDX_Text(pDX, IDC_GAMMA_X, m_Gamma_X);
	DDX_Text(pDX, IDC_GAMMA_Y, m_Gamma_Y);
	DDX_Text(pDX, IDC_GAMMA_COEFF,m_Gamma_Coeff);
}


BEGIN_MESSAGE_MAP(CGammaDlg, CBaseDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_GAMMA_SAVE, OnBnClickedButtonGammaSave)
	ON_BN_CLICKED(IDC_BUTTON_GAMMA_LOAD, OnBnClickedButtonGammaLoad)
	ON_BN_CLICKED(ID_APPLY, OnBnClickedApplyButton)
END_MESSAGE_MAP()

void CGammaDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPaintDC dc(this); // device context for painting
	Graphics gr(dc);
	int i;
	Point testPiont;

	testPiont.X = point.x;
	testPiont.Y = point.y;
	for(i = 0;i < m_GammaOption.PNum;i++)
	{
		if(m_pYArrowRegions[i]->IsVisible(testPiont,&gr))
		{
			m_gCurrentPoint = i;
			
			CWnd::RedrawWindow();
				
		}
	}
	CBaseDlg::OnLButtonUp(nFlags, point);
}

void CGammaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags == MK_LBUTTON )
	{
		CPaintDC dc(this); // device context for painting
		Graphics gr(dc);
		Point testPiont;

		testPiont.X = point.x;
		testPiont.Y = point.y;
		for(int i = 0; i < m_GammaOption.PNum; i++)
		{
			if(m_pYArrowRegions[i]->IsVisible(testPiont,&gr))
			{
				{
					m_pGloablePoints[i].Y = GAMMA_REAL2LOGIC_Y(testPiont.Y);
					m_gCurrentPoint = i;
					CWnd::RedrawWindow();
				}
			}
		}

	}
	CBaseDlg::OnMouseMove(nFlags, point);
}

void CGammaDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Graphics gr(dc);
	if(m_gGammaStat == GAMMA_STATE_RUN)
	{
		this->DrawXYAndFont(gr);		
		this->DrawLineAndPoints(gr);	
		this->DrawGammaLine(gr);
	}
	
	
}
void CGammaDlg::DrawGammaLine(Graphics &gr)
{
		SolidBrush  solid_brush(Color(255,0,0,0));//Brush
		Pen pen(Color(255, 0, 0, 255), 1/gr.GetDpiX());//Pen
		int i;
		//draw Gamma line
		for(i = 0; i < GAMMA_PNUMMAX*4-1; i++)
			gr.DrawLine( &pen, m_pGammaPoints[i].X, m_pGammaPoints[i].Y,
			m_pGammaPoints[i+1].X, m_pGammaPoints[i+1].Y );
}
void CGammaDlg::DrawLineAndPoints(Graphics &gr)
{
		Pen pen(Color(255, 0, 0, 0), 1/gr.GetDpiX());//Pen
		int i;
		//draw points & lines
		pen.SetStartCap(LineCapFlat);
		pen.SetLineJoin(LineJoinBevel);
		for(i = 0; i < m_GammaOption.PNum-1; i++)
			gr.DrawLine(&pen,GAMMA_LOGIC2REAL_X(m_pGloablePoints[i].X),GAMMA_LOGIC2REAL_Y(m_pGloablePoints[i].Y),
			GAMMA_LOGIC2REAL_X(m_pGloablePoints[i+1].X),GAMMA_LOGIC2REAL_Y(m_pGloablePoints[i+1].Y));
		pen.SetColor(Color(255,255,0,0));
		for( i = 0 ; i < m_GammaOption.PNum; i++)
			gr.DrawRectangle(&pen,GAMMA_LOGIC2REAL_X(m_pGloablePoints[i].X),GAMMA_LOGIC2REAL_Y(m_pGloablePoints[i].Y),2,2);
		//draw Current points
		pen.SetColor(Color(255,0,255,0));
		gr.DrawRectangle(&pen,GAMMA_LOGIC2REAL_X(m_pGloablePoints[m_gCurrentPoint].X),
			GAMMA_LOGIC2REAL_Y(m_pGloablePoints[m_gCurrentPoint].Y),3,3);
}
void CGammaDlg::DrawXYAndFont(Graphics &gr)
{
		SolidBrush  solid_brush(Color(255,0,0,0));//Brush
		Pen pen(Color(255, 0, 0, 0), 1/gr.GetDpiX());//Pen
		char str[20];
		WCHAR wstr[20];
		//Output Font
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily,8,FontStyleRegular,UnitPoint);

		//Draw X Y Axis
		pen.SetStartCap(LineCapDiamondAnchor);
		pen.SetEndCap(LineCapFlat);
		gr.DrawLine(&pen, 20, 20, 20, m_GammaOption.MapHi+20);
		gr.DrawLine(&pen, m_GammaOption.MapWid+20, m_GammaOption.MapHi+20, 20, m_GammaOption.MapHi+20);
		
		//x
		gr.DrawString(L"X Axis",-1,&font,PointF((REAL)m_GammaOption.MapWid,(REAL)m_GammaOption.MapHi),&solid_brush);
		//y
		gr.DrawString(L"Y Axis",-1,&font,PointF(23,20),&solid_brush);

		//Draw YArrowRegion Which is sensitive about Mouse
		sprintf( (char *)str,"(%d,%d)\0",
		(m_pGloablePoints[m_gCurrentPoint].X),
		(m_pGloablePoints[m_gCurrentPoint].Y));
		MultiByteToWideChar( CP_ACP, 0, str, (int)strlen(str)+1, wstr,20 );
		gr.DrawString(wstr,-1,&font,PointF((REAL)GAMMA_LOGIC2REAL_X(m_pGloablePoints[m_gCurrentPoint].X),0),&solid_brush);
}

BOOL CGammaDlg::OnInitDialog()
{
	m_gGammaStat = GAMMA_STATE_INIT;
	this->initGamma(NULL);
	this->DispOption();
	return TRUE;  // return TRUE unless you set the focus to a control
}



// set Points before Dialog start drawing
void CGammaDlg::SetPoints(int * XPoints, int * YPoints, int len)
{
	int i;
	ASSERT(XPoints != NULL && YPoints != NULL);
	ASSERT(len==m_GammaOption.PNum);
	for( i = 0; i < m_GammaOption.PNum; i++)
		{
			m_pGloablePoints[i].X = XPoints[i];
			m_pGloablePoints[i].Y = YPoints[i];
		}
}

// get Points before Dialog start drawing
void CGammaDlg::GetPoints(int * XPoints, int * YPoints, int len)
{
	int i;
	ASSERT(XPoints != NULL && YPoints != NULL);
	ASSERT(len==m_GammaOption.PNum);
	for( i = 0;i < m_GammaOption.PNum;i++)
		{
			XPoints[i] = m_pGloablePoints[i].X;
			YPoints[i] = m_pGloablePoints[i].Y;
		}
}

void CGammaDlg::OnBnClickedButtonGammaSave()
{
	CFileBin file;
	CString str;
	char *strbuf;
	char tmpstr[100] = "##########DATA###########";
	int i;
	if(CBaseDlg::OpenFileDlg(&str))
	{
		strbuf = str.GetBuffer();
		file.Open(strbuf,"w",0);
		file.Write((char *)&m_GammaOption,sizeof(TGammaOption));
		file.Write(tmpstr,(int)strlen(tmpstr));
		for(i = 0; i < m_GammaOption.PNum; i++)
		{
			file.Write((char *)&(m_pGloablePoints[i].X),sizeof(int));
			file.Write((char *)&(m_pGloablePoints[i].Y),sizeof(int));
		}

	}
	CWnd::RedrawWindow();

}

void CGammaDlg::OnBnClickedButtonGammaLoad()
{
	CFileBin file;
	CString str;
	char *strbuf;
	char tmpstr[100] = "##########DATA###########";
	int i;
	if(CBaseDlg::OpenFileDlg(&str))
	{
		strbuf = str.GetBuffer();
		file.Open(strbuf,"r",0);
		file.Read((char *)&m_GammaOption,sizeof(TGammaOption));
		this->DispOption();
		this->initGamma(&m_GammaOption);
		file.Read(tmpstr,(int)strlen(tmpstr));
		for(i = 0;i < m_GammaOption.PNum;i++)
		{
			file.Read((char *)&(m_pGloablePoints[i].X),sizeof(int));
			file.Read((char *)&(m_pGloablePoints[i].Y),sizeof(int));
		}
		
		CWnd::RedrawWindow();
	}

}

void CGammaDlg::InitUserOption(void)
{
	int i;
	memset(m_pYArrowRegions,0,sizeof(Region *)*GAMMA_PNUMMAX);
	m_GammaOption.PNum = GAMMA_PNUM;
	m_GammaOption.Arrow_Height = GAMMA_ARROW_HEIGHT;
	m_GammaOption.Arrow_Width = GAMMA_ARROW_WIDTH;
	m_GammaOption.LogicX = GAMMA_LOGIC_X;
	m_GammaOption.LogicY = GAMMA_LOGIC_Y;
	m_GammaOption.MapHi = GAMMA_MAPHI;
	m_GammaOption.MapWid = GAMMA_MAPWID;
	m_GammaOption.GammaCoeff =GAMMA_COEFF;
	for(i = 0; i <  GAMMA_PNUMMAX; i++)
	{
		m_pGloablePoints[i].X = 0;
		m_pGloablePoints[i].Y = 0;
	}
	this->ChangeUIOption(FALSE);

}

void CGammaDlg::GetUserOption(void)
{
	CWnd::UpdateData(TRUE);
	this->ChangeUIOption(TRUE);
}
void CGammaDlg::DispOption(void)
{
	this->ChangeUIOption(FALSE);
	CWnd::UpdateData(FALSE);
}


void CGammaDlg::initGamma(TGammaOption *opt)
{
	//******declare*****	
	GraphicsPath YArrowPath;
	Region tempRegion(&YArrowPath);
	int i,j;
	double k;
	int step;
	TGammaOption *myopt;
	//******************

	if(opt)
		myopt=opt;
	else
		myopt = &m_GammaOption;
			
	m_gGammaStat = GAMMA_STATE_INIT;
	for(i = 0; i <= myopt->PNum; i++)
	{
		if(m_pYArrowRegions[i] == NULL)
			m_pYArrowRegions[i] = tempRegion.Clone();
	}
	
	//INIT POINTS
	for(i = 0; i < myopt->PNum; i++)
		 {
			 m_pGloablePoints[i].X = (myopt->LogicX)*i/(myopt->PNum-1);
		 }

	//Init YArrowRegions
	step = ((myopt->MapWid-20)/(myopt->PNum-1))/4;
	for( i = 0; i < myopt->PNum; i++)
		{
			YArrowPath.Reset();
			YArrowPath.AddRectangle(Rect(GAMMA_LOGIC2REAL_X(m_pGloablePoints[i].X)-step,20,2*step,myopt->MapHi));
			m_pYArrowRegions[i]->MakeEmpty();
			m_pYArrowRegions[i]->Union(&YArrowPath);
		}
	//Init Gamma Points
	for( i = 0; i < GAMMA_PNUMMAX*4; i++)
	{
		j=i*(myopt->LogicX)/(GAMMA_PNUMMAX*4-1);
		k=pow((float)j,(float)(1/m_GammaOption.GammaCoeff));
		m_pGammaPoints[i].X = GAMMA_LOGIC2REAL_X((float)j); 
		m_pGammaPoints[i].Y = GAMMA_LOGIC2REAL_Y((float)k);
	}
	 //Init gloable vars
	 m_gCurrentPoint = 0;
	 m_gGammaStat = GAMMA_STATE_RUN;
}

void CGammaDlg::OnBnClickedApplyButton()
{
	this->GetUserOption();
	this->initGamma(&m_GammaOption);
	this->DispOption();
	this->m_Target->SendMessage(WM_GAMMADLG_ONAPPLY,NULL,(LPARAM)this);
	CWnd::RedrawWindow();
}
//set or get option from UI
//if direction is TRUE, set option by UI
//if FALSE, set option to UI
void CGammaDlg::ChangeUIOption(bool direction)
{
	char  str[100];
	if(direction == TRUE)
	{
		m_GammaOption.PNum = m_Gamma_PNum;
		if(m_Gamma_PNum>=GAMMA_PNUMMAX)
		{
			m_Gamma_PNum = GAMMA_PNUMMAX;
			CWnd::UpdateData(FALSE);
			m_GammaOption.PNum = m_Gamma_PNum;
			sprintf(str,"too large set to %d",GAMMA_PNUMMAX);
			CWnd::MessageBox((LPCSTR)str,"NO",MB_OK);
		}
		
		m_GammaOption.MapHi = m_Gamma_Map_Height;
		m_GammaOption.MapWid = m_Gamma_Map_Width;
		m_GammaOption.LogicX = m_Gamma_X;
		m_GammaOption.LogicY = m_Gamma_Y;
		m_GammaOption.GammaCoeff = m_Gamma_Coeff;
	}
	else
	{
		m_Gamma_PNum = m_GammaOption.PNum;
		m_Gamma_Map_Height = m_GammaOption.MapHi;
		m_Gamma_Map_Width = m_GammaOption.MapWid;
		m_Gamma_X = m_GammaOption.LogicX;
		m_Gamma_Y = m_GammaOption.LogicY;
		m_Gamma_Coeff = m_GammaOption.GammaCoeff;
	}
}

void CGammaDlg::OnGammaKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	switch(nChar)
	{
		//Y add logic 1 step
		case VK_UP:
			m_pGloablePoints[m_gCurrentPoint].Y += nRepCnt;
			if(m_pGloablePoints[m_gCurrentPoint].Y > m_GammaOption.LogicY)
				m_pGloablePoints[m_gCurrentPoint].Y = m_GammaOption.LogicY;
			break;
		//Y minus logic 1 step
		case VK_DOWN:
			m_pGloablePoints[m_gCurrentPoint].Y -=nRepCnt;
			if(m_pGloablePoints[m_gCurrentPoint].Y < 0)
				m_pGloablePoints[m_gCurrentPoint].Y = 0;
			break;
		//X minus logic 1 step
		case VK_LEFT:
            m_gCurrentPoint -=nRepCnt;
			if(m_gCurrentPoint < 0)
				m_gCurrentPoint = 0;
			break;
		//Y add logic 1 step
		case VK_RIGHT:
			 m_gCurrentPoint +=nRepCnt;
			 if(m_gCurrentPoint>m_GammaOption.PNum)
				m_gCurrentPoint = m_GammaOption.PNum;
			break;
	}
	CWnd::RedrawWindow();
	nFlags = 0;
}

BOOL CGammaDlg::PreTranslateMessage(MSG* pMsg)
{
	BOOL bHandleNow = FALSE;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
			bHandleNow = TRUE;
			break;
		} 
	}
	if (bHandleNow) 
	{
		this->OnGammaKeyDown((UINT)pMsg->wParam,(UINT) LOWORD(pMsg ->lParam),(UINT) HIWORD(pMsg->lParam)); 
		return bHandleNow;
	}
	else
		return CBaseDlg::PreTranslateMessage(pMsg);
}

void CGammaDlg::SetTargetCWnd(CWnd * handle)
{
	this->m_Target=handle;
}

void CGammaDlg::GetGammaOpt(PTGammaOption pOpt)
{
	memcpy(pOpt,&m_GammaOption,sizeof(TGammaOption));
}

void CGammaDlg::SetGammaOpt(PTGammaOption pOpt)
{
	memcpy(&m_GammaOption,pOpt,sizeof(TGammaOption));
	this->initGamma(&m_GammaOption);
	this->DispOption();
}
