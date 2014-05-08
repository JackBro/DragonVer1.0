// V558TestFpgaDlg.cpp : 实现文件
//
#include "internal.h"

#ifndef	HOSTCMD_DISABLE

// CTestFpgaDlg 对话框

IMPLEMENT_DYNAMIC(CTestFpgaDlg, CBaseCtrlDlg)
CTestFpgaDlg::CTestFpgaDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CTestFpgaDlg::IDD, pParent)
	, m_BusTypeSel(0)
{
	memset(&(this->m_option), 0, sizeof(this->m_option));
}

CTestFpgaDlg::~CTestFpgaDlg()
{
}

void CTestFpgaDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGTYPE_COMBO, m_regtype);
	DDX_Control(pDX, IDC_REGADDR_EDIT, m_regaddr);
	DDX_Control(pDX, IDC_REGVAL_EDIT, m_regval);
	DDX_Control(pDX, IDC_REGVAL_EDIT2, m_regdat);
	DDX_Control(pDX, IDC_INSTANT_CHECK, m_instant);
	DDX_Control(pDX, IDC_CMDLIST_COMBO, m_cmdBox);
	DDX_Control(pDX, IDC_CMDPARA0_EDIT, m_para0Edit);
	DDX_Control(pDX, IDC_CMDPARA1_EDIT, m_para1Edit);
	DDX_Control(pDX, IDC_CMDPARA2_EDIT, m_para2Edit);
	DDX_Control(pDX, IDC_CMDPARA3_EDIT, m_para3Edit);
	DDX_Control(pDX, IDC_CMDPARA4_EDIT, m_para4Edit);
	DDX_Control(pDX, IDC_CMDPARA5_EDIT, m_para5Edit);
	DDX_Control(pDX, IDC_CMDPARA6_EDIT, m_para6Edit);
	DDX_Control(pDX, IDC_SENDCMD_BUTTON, m_sndcmdBtn);
}


BEGIN_MESSAGE_MAP(CTestFpgaDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_WRITEREG_BUTTON, OnBnClickedWriteregButton)
	ON_BN_CLICKED(IDC_REGREAD_BUTTON, OnBnClickedRegreadButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_REGADDR_SPIN, OnDeltaposRegaddrSpin)
	ON_BN_CLICKED(IDC_BATCHWRITE_BUTTON, OnBnClickedBatchwriteButton)
	ON_CBN_SELCHANGE(IDC_CMDLIST_COMBO, OnCbnSelchangeCmdlistCombo)
	ON_BN_CLICKED(IDC_SENDCMD_BUTTON, OnBnClickedSendcmdButton)
END_MESSAGE_MAP()


void CTestFpgaDlg::InitUserOption(void)
{
	int i, j;

	this->m_pParaEdit[0] = &(this->m_para0Edit);
	this->m_pParaEdit[1] = &(this->m_para1Edit);
	this->m_pParaEdit[2] = &(this->m_para2Edit);
	this->m_pParaEdit[3] = &(this->m_para3Edit);
	this->m_pParaEdit[4] = &(this->m_para4Edit);
	this->m_pParaEdit[5] = &(this->m_para5Edit);
	this->m_pParaEdit[6] = &(this->m_para6Edit);
	for(i = 0; i < gRegTotalType; i++)
		this->m_regtype.AddString((char *)gRegType[i]);
	for(i = 0; i < gCmdGroupCount; i++)
	{
		for(j = 0; j < gCmdGroup[i].count; j++)
		{
			if(gCmdGroup[i].cmdlist[j].cmdstr)
				this->m_cmdBox.AddString(gCmdGroup[i].cmdlist[j].cmdstr);
		}
	}

	this->m_regtype.SetCurSel(0);
	this->m_regaddr.SetWindowText("0");
	this->m_regval.SetWindowText("0");

	this->m_cmdBox.SetCurSel(0);
	this->OnCbnSelchangeCmdlistCombo();

}

BOOL CTestFpgaDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTestFpgaDlg::GetUserOption(char *tail)
{
	char str[20];
	int i;

	this->m_option.regtype = this->m_regtype.GetCurSel();
	this->m_cmdBox.GetWindowText(this->m_option.cmd, CMD_LENGTH);
	for(i = 0; i < 7; i++)
		this->m_pParaEdit[i]->GetWindowText(this->m_option.para[i], PARAM_LENGTH);
	this->m_regaddr.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.regaddr));
	this->m_regval.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.regval));

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CTestFpgaDlg::OnBnClickedWriteregButton()
{
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	CBaseCtrlDlg::WriteReg(this->m_option.regtype, this->m_option.regaddr, this->m_option.regval);
}

void CTestFpgaDlg::OnBnClickedRegreadButton()
{
	char str[20];
	int val = 0;
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	val = CBaseCtrlDlg::ReadReg(this->m_option.regtype, this->m_option.regaddr);
	sprintf(str, "%x", val);
	this->m_regdat.SetWindowText(str);
}

void CTestFpgaDlg::OnDeltaposRegaddrSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	char str[20];
	int adr;

	this->m_regaddr.GetWindowText(str, 20);
	sscanf(str, "%x", &adr);
	if(pNMUpDown->iDelta > 0)
	{
		adr--;
		adr &= 0xffffff;
	}
	else
	{
		adr++;
		adr &= 0xffffff;
	}
	sprintf(str, "%x", adr);
	this->m_regaddr.SetWindowText(str);
	if(this->m_instant.GetCheck())
		this->OnBnClickedRegreadButton();

	*pResult = 0;
}
#if 0
#include "../batchparser/parser.h"

void CTestFpgaDlg::OnBnClickedBatchwriteButton(void)
{
	char unit[0x100];
	CWaitCursor	cursor;
	CParser parser;

	this->m_SrcfCombo.GetWindowText(unit, 100);
	if(strlen(unit) == 0)
		return;
	if(parser.init_parse(unit))
		parser.run_testcase();
}
#endif
#if 1
void CTestFpgaDlg::OnBnClickedBatchwriteButton(void)
{
//	char srcfile[100];
	CBatchParse parse;
	char unit[0x100];
	TRegUnit *punit = (TRegUnit *)unit;
	int flag = 1, temp;
	CWaitCursor	cursor;

	this->m_SrcfCombo.GetWindowText(unit, 100);
	if(strlen(unit) == 0)
		return;
	parse.OpenBatchFile(unit);
	while(flag)
	{
		temp = parse.GetNextUnit(unit);
		switch(temp)
		{
		case BATCH_WRITE_REG:
			this->WriteReg(punit);
			break;
		case BATCH_STRING:
			break;
		case BATCH_OVER:
		case BATCH_FAILED:
		default:
			flag = 0;
			break;
		}
	}
}
#endif

#if 0
void CTestFpgaDlg::OnBnClickedBatchwriteButton()
{
	char srcfile[100];
	CBatchParse parse;
	char unit[0x100];
	TRegUnit *punit = (TRegUnit *)unit;
	TKeyStringMap *pmap = (TKeyStringMap *)unit;
	FILE *fp;
	int flag = 1, temp;

	this->m_SrcfCombo.GetWindowText(srcfile, 100);
	if(strlen(srcfile) == 0)
		return;
	parse.OpenBatchFile(srcfile);
	fp = fopen("test.txt", "wt");
	if(fp == NULL)
	{
		AfxMessageBox("cant open writing file");
		return;
	}
	while(flag)
	{
		temp = parse.GetNextUnit(unit);
		switch(temp)
		{
		case BATCH_WRITE_REG:
			//this->WriteReg(punit);
			if(punit->regtype == SENSOR_REGTYPE)
				fprintf(fp, "#");
			fprintf(fp, "%x@%x", punit->regval, punit->regaddr);
			if(punit->delay > 0)
				fprintf(fp, "~%x", punit->delay);
			fprintf(fp, "\n");
			break;
		case BATCH_STRING:
			fprintf(fp, "%s = ", gKeyMap[pmap->index].keychar);
			if(pmap->value[0] != 0)
				fprintf(fp, "%s\n", pmap->value);
			else
				fprintf(fp, "NULL\n");
			break;
		case BATCH_OVER:
		case BATCH_FAILED:
		default:
			flag = 0;
			break;
		}
	}
	fclose(fp);
}
#endif

void CTestFpgaDlg::OnCbnSelchangeCmdlistCombo()
{
	int sel, i, x, y;

	sel = this->m_cmdBox.GetCurSel();
	if(GetCmdLocation(sel, &x, &y))
	{
		for(i = 0; i < 7; i++)
			this->m_pParaEdit[i]->EnableWindow(FALSE);
		return;
	}
	if(gCmdGroup[x].cmdlist[y].type == BUFCMD_TYPE)
	{
		this->m_sndcmdBtn.EnableWindow(FALSE);
		for(i = 0; i < 7; i++)
			this->m_pParaEdit[i]->EnableWindow(FALSE);
		return;
	}
	this->m_sndcmdBtn.EnableWindow();
	for(i = 0; i < gCmdGroup[x].cmdlist[y].maxpara - 1; i++)
		this->m_pParaEdit[i]->EnableWindow();
	for(; i < 7; i++)
		this->m_pParaEdit[i]->EnableWindow(FALSE);
}

void CTestFpgaDlg::OnBnClickedSendcmdButton()
{
	char cmd[0x200];
	int i, sel, x, y;

	this->GetUserOption(FILE_TAIL_DAT);
	sel = this->m_cmdBox.GetCurSel();
	if(GetCmdLocation(sel, &x, &y))
		return;
	sprintf(cmd, "%s", this->m_option.cmd);
	for(i = 0; i < gCmdGroup[x].cmdlist[y].maxpara - 1; i++)
		sprintf(cmd, "%s %s", cmd, this->m_option.para[i]);
	sprintf(cmd, "%s\n", cmd);
	switch(gCmdGroup[x].cmdlist[y].type)
	{
	case SETCMD_TYPE:
		SendCmd(cmd);
		break;
	case READCMD_TYPE:
		i = SendRetCmd(cmd);
		sprintf(cmd, "%x", i);
		this->m_pParaEdit[0]->SetWindowText(cmd);
		break;
	default:
		break;
	}
}

#endif //V578CMD_DISABLE


