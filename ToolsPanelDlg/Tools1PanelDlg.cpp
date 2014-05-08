// Tools1PanelDlg.cpp : 实现文件
//

#include "internal.h"


// CTools1PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools1PanelDlg, CBasePanelDlg)
CTools1PanelDlg::CTools1PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools1PanelDlg::IDD, pParent)
{
}

CTools1PanelDlg::~CTools1PanelDlg()
{
}

void CTools1PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BYTEWIDTH_COMBO, m_ByteCount);
	DDX_Control(pDX, IDC_WIDTH_EDIT, m_width);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_height);
	DDX_Control(pDX, IDC_DIVDATA_COMBO, m_div);
	DDX_Control(pDX, IDC_MAXVALUE_EDIT, m_maxval);
	DDX_Control(pDX, IDC_DSTWIDTH_EDIT, m_dstwidth);
	DDX_Control(pDX, IDC_DSTHEIGHT_EDIT, m_dstheight);
	DDX_Control(pDX, IDC_SRCFILE2_COMBO, m_srcfile2box);
	DDX_Control(pDX, IDC_DIVDATA_COMBO2, m_srcperbyte);
	DDX_Control(pDX, IDC_DIVDATA_COMBO3, m_src2perbyte);
	DDX_Control(pDX, IDC_SRCBCOUNT_COMBO, m_srcbcnt);
	DDX_Control(pDX, IDC_DESTBCOUNT_COMBO, m_dstbcnt);
	DDX_Control(pDX, IDC_OPERATE_COMBO, m_operator);
	DDX_Control(pDX, IDC_MINBOX_EDIT, m_minval);
	DDX_Control(pDX, IDC_MAXBOX_EDIT, m_capval);
	DDX_Control(pDX, IDC_POINTX_EDIT, m_pointxBox);
	DDX_Control(pDX, IDC_POINTY_EDIT, m_pointyBox);
}

BEGIN_MESSAGE_MAP(CTools1PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_CONVERT_BUTTON, OnBnClickedConvertButton)
	ON_BN_CLICKED(IDC_HEX2BIN_BUTTON, OnBnClickedHex2binButton)
	ON_BN_CLICKED(IDC_COMPARE_BUTTON, OnBnClickedCompareButton)
	ON_BN_CLICKED(IDC_BIN2HEX_BUTTON, OnBnClickedBin2hexButton)
	ON_BN_CLICKED(IDC_DROPHIGH_BUTTON, OnBnClickedDrophighButton)
	ON_BN_CLICKED(IDC_DROPLOW_BUTTON, OnBnClickedDroplowButton)
	ON_BN_CLICKED(IDC_CREATEORDERHEX_BUTTON, OnBnClickedCreateorderhexButton)
	ON_BN_CLICKED(IDC_COPY_BUTTON, OnBnClickedCopyButton)
	ON_BN_CLICKED(IDC_OPENSRCFILE2_BTN, OnBnClickedOpensrcfile2Btn)
	ON_BN_CLICKED(IDC_APPENDFILE_BUTTON, OnBnClickedAppendFileButton)
	ON_BN_CLICKED(IDC_MERGEFILE_BUTTON, OnBnClickedMergefileButton)
	ON_BN_CLICKED(IDC_MISCTOOLS_BUTTON, OnBnClickedMisctoolsButton)
	ON_BN_CLICKED(IDC_BIN2ASC_BUTTON, OnBnClickedBin2ascButton)
	ON_BN_CLICKED(IDC_SPANCVT_BUTTON, OnBnClickedSpancvtButton)
	ON_BN_CLICKED(IDC_SEQCVT_BUTTON, OnBnClickedSeqcvtButton)
END_MESSAGE_MAP()


// CTools1PanelDlg 消息处理程序

void CTools1PanelDlg::OnBnClickedConvertButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_CPP);
	tools.ConvertHeader2RegList(this->m_option.src, this->m_option.dest);
}

void CTools1PanelDlg::OnBnClickedHex2binButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	tools.Hex2Bin(this->m_option.src, this->m_option.dest, this->m_option.bytecount);
	this->AutoSetSrcfile();
}

void CTools1PanelDlg::OnBnClickedCompareButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;
	char src[100], dest[100];

	this->m_SrcfCombo.GetWindowText(src, 100);
	this->m_DestfCombo.GetWindowText(dest, 100);
	if( (strlen(src) == 0) || (strlen(dest) == 0) )
		return;
	tools.Compare(src, dest);
}

void CTools1PanelDlg::OnBnClickedAppendFileButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;
	char src[100], dest[100];

	this->m_SrcfCombo.GetWindowText(src, 100);
	this->m_DestfCombo.GetWindowText(dest, 100);
	if( (strlen(src) == 0) || (strlen(dest) == 0) )
		return;
	tools.AppendFile(src, dest);
}

void CTools1PanelDlg::OnBnClickedBin2hexButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	tools.Bin2Hex(this->m_option.src, this->m_option.dest, this->m_option.bytecount);
	this->AutoSetSrcfile();
}

BOOL CTools1PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitUserOption();

	return TRUE;
}

void CTools1PanelDlg::OnBnClickedDrophighButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	tools.DropHighByte(this->m_option.src, this->m_option.dest, this->m_option.opercode, 
		this->m_option.div, this->m_option.srcbcnt, this->m_option.dstbcnt,
		this->m_option.minval, this->m_option.capval);
	this->AutoSetSrcfile();
}

void CTools1PanelDlg::OnBnClickedDroplowButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	tools.DropLowByte(this->m_option.src, this->m_option.dest, this->m_option.opercode,  
		this->m_option.div, this->m_option.srcbcnt, this->m_option.dstbcnt,
		this->m_option.minval, this->m_option.capval);
	this->AutoSetSrcfile();
}

void CTools1PanelDlg::AutoSetSrcfile(void)
{
	CBasePanelDlg::AutoSetFileBox(this->m_option.dest);
}

void CTools1PanelDlg::InitUserOption(void)
{
	int i;
	char str[TOOLS_STRING_LENGTH];
	const char *opstr[] = {
		"add", "sub", "multi", "div", "div by", "mod", 
		"and", "or", "xor", "not", "equal"
	};

	memset( &(this->m_option), 0, sizeof(TUserOption) );

	for(i = 0; i < sizeof(opstr) / sizeof(char *); i++)
		this->m_operator.InsertString(i, opstr[i]);


	for(i = 0; i < 4; i++)
	{
		sprintf(str, "%d", i+1);
		this->m_ByteCount.InsertString(i, str);
		this->m_src2perbyte.InsertString(i, str);
		this->m_srcperbyte.InsertString(i, str);
		this->m_div.InsertString(i, str);
		this->m_srcbcnt.InsertString(i, str);
		this->m_dstbcnt.InsertString(i, str);
	}
	
	this->m_operator.SetCurSel(0);
	this->m_srcbcnt.SetCurSel(0);
	this->m_dstbcnt.SetCurSel(0);
	this->m_ByteCount.SetCurSel(0);
	this->m_div.SetCurSel(0);
	this->m_src2perbyte.SetCurSel(0);
	this->m_srcperbyte.SetCurSel(0);

	this->m_minval.SetWindowText("0");
	this->m_capval.SetWindowText("255");

	this->m_maxval.SetWindowText("0");
	this->m_width.SetWindowText("640");
	this->m_height.SetWindowText("480");
	this->m_dstwidth.SetWindowText("320");
	this->m_dstheight.SetWindowText("240");
	this->m_pointxBox.SetWindowText("0");
	this->m_pointyBox.SetWindowText("0");
	this->m_srcbcnt.SetWindowText("2");
	this->m_dstbcnt.SetWindowText("1");
}

void CTools1PanelDlg::GetUserOption(char *tail)
{
	char str[TOOLS_STRING_LENGTH];
	CStringTools tools;
	//CFileFind find;

	this->m_ByteCount.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.bytecount));

	this->m_srcperbyte.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.src1perbyte));

	this->m_src2perbyte.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.src2perbyte));

	this->m_div.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Hex(str, &(this->m_option.div));

	this->m_width.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.width));
	this->m_height.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.height));

	this->m_dstwidth.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.dstwidth));
	this->m_dstheight.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.dstheight));
	this->m_pointxBox.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.pointx));
	this->m_pointyBox.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.pointy));

	this->m_maxval.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.maxval));

	this->m_srcbcnt.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.srcbcnt));
	this->m_dstbcnt.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.dstbcnt));

	this->m_minval.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.minval));
	this->m_capval.GetWindowText(str, TOOLS_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.capval));

	this->m_option.opercode = this->m_operator.GetCurSel();

	this->m_srcfile2box.GetWindowText(this->m_option.src2, DEFAULT_FILENAME_LEN);
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
}

void CTools1PanelDlg::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option.pointy);
	this->m_pointyBox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.width);
	this->m_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.height);
	this->m_height.SetWindowText(str);
}

void CTools1PanelDlg::OnBnClickedCreateorderhexButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	tools.CreateOrderHexFile(this->m_option.dest, this->m_option.maxval);
	this->AutoSetSrcfile();
}


void CTools1PanelDlg::OnBnClickedCopyButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	tools.DropHighByte(this->m_option.src, this->m_option.dest, ADD_OPCODE, 0, 1, 1); 
	this->AutoSetSrcfile();
}

void CTools1PanelDlg::OnBnClickedOpensrcfile2Btn()
{
	CString str;

	if(CBasePanelDlg::OpenFileDlg(&str))
	{
		this->m_srcfile2box.addString(str.GetBuffer());
	}
}

void CTools1PanelDlg::OnBnClickedMergefileButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	tools.MergeFile(this->m_option.src, this->m_option.src2, 
			this->m_option.dest, this->m_option.src1perbyte, this->m_option.src2perbyte);
	this->AutoSetSrcfile();
}


void CTools1PanelDlg::OnBnClickedMisctoolsButton()
{
	CWaitCursor cursor;
	CBitTools tools;
	char buf1[] = {"adsfasdf"};
	char buf2[100], buf3[100];

	tools.SetChk(0);
	tools.PutBufBits(buf1, buf2, sizeof(buf1) << 3, 0, 0);
	tools.PutBufBits(buf1, buf2, sizeof(buf1) << 3, 3, 0);
	tools.PutBufBits(buf2, buf3, sizeof(buf1) << 3, 0, 3);
}

void CTools1PanelDlg::OnBnClickedBin2ascButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	tools.Bin2Ascii(this->m_option.src, this->m_option.dest, this->m_option.bytecount);
	this->AutoSetSrcfile();
}


void CTools1PanelDlg::OnBnClickedSpancvtButton()
{
	CNormalTools tools;
	CWaitCursor cursor;
	int len, adr, span, size;

	this->GetUserOption();
	size = this->m_option.dstwidth * this->m_option.srcbcnt;
	span = this->m_option.width * this->m_option.srcbcnt;
	adr = this->m_option.pointy * span + this->m_option.pointx * this->m_option.srcbcnt;
	len = size * this->m_option.dstheight;
	tools.SpanConvert(this->m_option.src, this->m_option.dest, len, adr, span, size,
		this->m_option.opercode, this->m_option.div, this->m_option.srcbcnt);
	this->AutoSetSrcfile();
}

void CTools1PanelDlg::OnBnClickedSeqcvtButton()
{
	CNormalTools tools;
	CWaitCursor cursor;
	int len, adr;

	this->GetUserOption();
	adr = this->m_option.pointx;
	len = this->m_option.pointy;
	tools.SeqConvert(this->m_option.src, this->m_option.dest, len, adr,
		this->m_option.opercode, this->m_option.div, this->m_option.srcbcnt);
	this->AutoSetSrcfile();
}



