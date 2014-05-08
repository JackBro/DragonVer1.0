
#include "internal.h"

static const char g_CharSpace[] =
{
	' ', '\t', '\r'
};

static const char g_CharEnter[] =
{
	'\n'//, '\r'
};

static const char g_CharOct[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7'
};

static const char g_CharDec[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static const char g_CharHex[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
	'a', 'b', 'c', 'd', 'e', 'f', 
	'A', 'B', 'C', 'D', 'E', 'F'
};

static const char g_CharLetter[] =
{
	'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
	'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'_'
};

static const char g_CharStr[] =
{
	'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
	'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|',
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
	' '
};	// '\'', '\\', '\"'


CCharFifo::CCharFifo(void)
{
	this->m_flag = 0;
	this->m_line = 0;
	this->Init();
}

CCharFifo::~CCharFifo(void)
{
}

void CCharFifo::SetNodeProp(void)
{
	this->m_decdata.SetNodeProp(CHAR_DEC_DIGITAL);
	this->m_hexdata.SetNodeProp(CHAR_HEX_DIGITAL);
	this->m_octdata.SetNodeProp(CHAR_OCT_DIGITAL);
	this->m_word.SetNodeProp(CHAR_WORD);

	this->m_enter.SetNodeProp(CHAR_NEWLINE);

	this->m_wplus.SetNodeProp(WORD_PLUS);
	this->m_wminus.SetNodeProp(WORD_MINUS);

	this->m_wpluse.SetNodeProp(WORD_PLUSE);
	this->m_wminuse.SetNodeProp(WORD_MINUSE);
	this->m_wmule.SetNodeProp(WORD_MULE);
	this->m_wdive.SetNodeProp(WORD_DIVE);
	this->m_wmode.SetNodeProp(WORD_MODE);
	this->m_wande.SetNodeProp(WORD_ANDE);
	this->m_wore.SetNodeProp(WORD_ORE);
	this->m_wxore.SetNodeProp(WORD_XORE);
	this->m_wor.SetNodeProp(WORD_OR);
	this->m_wequal.SetNodeProp(WORD_EQUAL);
	this->m_wand.SetNodeProp(WORD_AND);
	this->m_wnote.SetNodeProp(WORD_NOTE);
	this->m_wless.SetNodeProp(WORD_LESSE);
	this->m_wbig.SetNodeProp(WORD_BIGE);

	this->m_float.SetNodeProp(WORD_DIGFLOAT);
	this->m_floate.SetNodeProp(WORD_DIGFLOAT);
	this->m_floatp.SetNodeProp(WORD_DIGFLOAT);

	this->m_char.SetNodeProp(CHAR_CHAR);
	this->m_chari.SetNodeProp(CHAR_MISC);

	this->m_charis.SetNodeProp(CHAR_MISC);
	this->m_charo.SetNodeProp(CHAR_MISC);
	this->m_chard.SetNodeProp(CHAR_MISC);
	this->m_charx.SetNodeProp(CHAR_MISC);
	
	this->m_str.SetNodeProp(CHAR_STRING);

	this->m_cmtsi.SetNodeProp(CHAR_COMMENTS);
	this->m_cmtss.SetNodeProp(CHAR_COMMENTS);
	this->m_cmtsd.SetNodeProp(CHAR_COMMENTS);
	this->m_comments.SetNodeProp(CHAR_COMMENTS);
}

void CCharFifo::InitGraph(void)
{
	char ch[256];

	memcpy(ch, g_CharDec, sizeof(g_CharDec));
	memcpy(ch + sizeof(g_CharDec), g_CharLetter, sizeof(g_CharLetter));
	this->m_decdata.Connect(sizeof(g_CharDec), (char *)g_CharDec, &this->m_decdata);
	this->m_hexdata.Connect(sizeof(g_CharHex), (char *)g_CharHex, &this->m_hexdata);
	this->m_octdata.Connect(sizeof(g_CharOct), (char *)g_CharOct, &this->m_octdata);
	this->m_space.Connect(sizeof(g_CharSpace), (char *)g_CharSpace, &this->m_space);
	this->m_word.Connect(sizeof(g_CharLetter) + sizeof(g_CharDec), ch, &this->m_word);
	this->m_word.ConnectH7(&this->m_wordh);
	this->m_wordh.ConnectAny(&this->m_word);

	ch[0] = '!';
	this->m_root.Connect(1, ch, &this->m_not);
	ch[0] = '\"';
	this->m_root.Connect(1, ch, &this->m_dbquat);
	ch[0] = '%';
	this->m_root.Connect(1, ch, &this->m_module);
	ch[0] = '&';
	this->m_root.Connect(1, ch, &this->m_and);
	ch[0] = '\'';
	this->m_root.Connect(1, ch, &this->m_quatation);

	ch[0] = '*';
	this->m_root.Connect(1, ch, &this->m_mul);
	ch[0] = '+';
	this->m_root.Connect(1, ch, &this->m_plus);
	ch[0] = '-';
	this->m_root.Connect(1, ch, &this->m_minus);
	ch[0] = '/';
	this->m_root.Connect(1, ch, &this->m_div);

	ch[0] = '<';
	this->m_root.Connect(1, ch, &this->m_less);
	ch[0] = '=';
	this->m_root.Connect(1, ch, &this->m_equal);
	ch[0] = '>';
	this->m_root.Connect(1, ch, &this->m_big);

	ch[0] = '^';
	this->m_root.Connect(1, ch, &this->m_xor);
	ch[0] = '|';
	this->m_root.Connect(1, ch, &this->m_or);

	this->m_space.SetNodeProp(CHAR_BLANK);

	ch[0] = '+';
	this->m_plus.Connect(1, ch, &this->m_wplus);
	ch[0] = '-';
	this->m_minus.Connect(1, ch, &this->m_wminus);
	ch[0] = '&';
	this->m_and.Connect(1, ch, &this->m_wand);
	ch[0] = '|';
	this->m_or.Connect(1, ch, &this->m_wor);

	ch[0] = '=';
	this->m_plus.Connect(1, ch, &this->m_wpluse);
	this->m_minus.Connect(1, ch, &this->m_wminuse);
	this->m_mul.Connect(1, ch, &this->m_wmule);
	this->m_div.Connect(1, ch, &this->m_wdive);
	this->m_module.Connect(1, ch, &this->m_wmode);
	this->m_or.Connect(1, ch, &this->m_wore);
	this->m_xor.Connect(1, ch, &this->m_wxore);
	this->m_equal.Connect(1, ch, &this->m_wequal);
	this->m_not.Connect(1, ch, &this->m_wnote);
	this->m_less.Connect(1, ch, &this->m_wless);
	this->m_big.Connect(1, ch, &this->m_wbig);

	ch[0] = '\n';
	this->m_enter.Connect(sizeof(g_CharEnter), (char *)g_CharEnter, &this->m_enter);
}

void CCharFifo::BuildFloatNode(void)
{
	char ch[256];

	ch[0] = '.';
	this->m_ohnode.Connect(1, ch, &this->m_floatp);
	this->m_decdata.Connect(1, ch, &this->m_floatp);
	this->m_floatp.Connect(sizeof(g_CharDec), (char *)g_CharDec, &this->m_float);
	ch[0] = 'e';
	ch[1] = 'E';
	this->m_decdata.Connect(2, ch, &this->m_floate);
	this->m_float.Connect(2, ch, &this->m_floate);
	this->m_float.Connect(sizeof(g_CharDec), (char *)g_CharDec, &this->m_float);
	this->m_floate.Connect(sizeof(g_CharDec), (char *)g_CharDec, &this->m_floate);
}

void CCharFifo::BuildStringNode(void)
{
	char ch[256];
	int i, j;

	for(i = 0, j = 0; i < 255; i++, j++)
	{
		if(j == '\"')
			j++;
		ch[i] = (char)j;
	}
	this->m_dbquat.Connect(255, ch, &this->m_dbquat);
	ch[0] = '\"';
	this->m_dbquat.Connect(1, ch, &this->m_str);
	this->m_str.ConnectH7(&this->m_strh);
	this->m_strh.ConnectAny(&this->m_str);
}

void CCharFifo::BuildCommentsNode(void)
{
	char ch[256];
	int i, j;

	ch[0] = '/';
	this->m_div.Connect(1, ch, &this->m_cmtsi);
	for(i = 0, j = 0; i < 255; i++, j++)
	{
		if(j == '\n')
			j++;
		ch[i] = (char)j;
	}
	this->m_cmtsi.Connect(255, ch, &this->m_cmtsi);
	ch[0] = '\n';
	this->m_cmtsi.Connect(1, ch, &this->m_comments);

	ch[0] = '*';
	this->m_div.Connect(1, ch, &this->m_cmtsd);
	for(i = 0, j = 0; i < 255; i++, j++)
	{
		if(j == '*')
			j++;
		ch[i] = (char)j;
	}
	this->m_cmtsd.Connect(255, ch, &this->m_cmtsd);

	ch[0] = '*';
	this->m_cmtsd.Connect(1, ch, &this->m_cmtss);
	for(i = 0, j = 0; i < 255; i++, j++)
	{
		if(j == '/')
			j++;
		ch[i] = (char)j;
	}
	this->m_cmtss.Connect(255, ch, &this->m_cmtsd);
	ch[0] = '/';
	this->m_cmtss.Connect(1, ch, &this->m_comments);
}

void CCharFifo::BuildCharNode(void)
{
	char ch[256];
	// '\'', '\\', '\"'
	
	this->m_quatation.Connect(sizeof(g_CharStr), (char *)g_CharStr, &this->m_chari);
	ch[0] = '\'';
	this->m_chari.Connect(1, ch, &this->m_char);

	ch[0] = '\\';
	this->m_quatation.Connect(1, ch, &this->m_charis);
	ch[1] = '\'';
	ch[2] = '\"';
	this->m_charis.Connect(3, ch, &this->m_chari);

	ch[0] = '0';
	this->m_charis.Connect(1, ch, &this->m_charo);
	this->m_charo.Connect(sizeof(g_CharOct), (char *)g_CharOct, &this->m_charo);
	ch[0] = '\'';
	this->m_charo.Connect(1, ch, &this->m_char);

	ch[0] = 'x';
	this->m_charis.Connect(1, ch, &this->m_charx);
	this->m_charx.Connect(sizeof(g_CharHex), (char *)g_CharHex, &this->m_charx);
	ch[0] = '\'';
	this->m_charx.Connect(1, ch, &this->m_char);

	this->m_charis.Connect(sizeof(g_CharDec)-1, (char *)g_CharDec+1, &this->m_chard);
	this->m_chard.Connect(sizeof(g_CharDec), (char *)g_CharDec, &this->m_chard);
	ch[0] = '\'';
	this->m_chard.Connect(1, ch, &this->m_char);
}

void CCharFifo::Init(void)
{
	char ch[256];

	this->m_root.Connect(sizeof(g_CharLetter), (char *)g_CharLetter, &this->m_word);
	this->m_root.ConnectH7(&this->m_wordh);
	this->m_root.Connect(sizeof(g_CharDec)-1, (char *)g_CharDec+1, &this->m_decdata);
	this->m_root.Connect(sizeof(g_CharSpace), (char *)g_CharSpace, &this->m_space);
	this->m_root.Connect(sizeof(g_CharEnter), (char *)g_CharEnter, &this->m_enter);

	ch[0] = '0';
	this->m_root.Connect(1, ch, &this->m_ohnode);
//	this->m_plus.Connect(1, ch, &this->m_ohnode);
//	this->m_minus.Connect(1, ch, &this->m_ohnode);
	ch[0] = 'x';
	ch[1] = 'X';
	this->m_ohnode.Connect(2, ch, &this->m_hexdata);
	this->m_ohnode.Connect(sizeof(g_CharOct), (char *)g_CharOct, &this->m_octdata);

//	this->m_plus.Connect(sizeof(g_CharDec)-1, (char *)g_CharDec+1, &this->m_decdata);
//	this->m_minus.Connect(sizeof(g_CharDec)-1, (char *)g_CharDec+1, &this->m_decdata);

	this->InitGraph();
	this->BuildFloatNode();
	this->BuildStringNode();
	this->BuildCommentsNode();
	this->BuildCharNode();

	this->m_root.ConnectOther();

	this->SetNodeProp();
}

int	CCharFifo::Read(char *str, int size)
{
	CCharNode *pnode = &this->m_root;
	char *ptr = str;
	int len, size1, rpoint;

	if(this->IsFifoEmpty())
		return 0;
	size1 = size;
	rpoint = CFifoBuf::GetReadPoint();
	while(size1)
	{
		CFifoBuf::PushState();
		len = CFifoBuf::Read(ptr, 1);
		if(len == 0)
		{
			CFifoBuf::SetReadPoint(rpoint);
			return 0;
		}
		this->m_flag = pnode->GetNodeProp();
		pnode = pnode->GetNode(ptr[0]);
		if(pnode == NULL)
		{
			CFifoBuf::PopState();
			if( (this->m_flag != CHAR_COMMENTS) &&
				(this->m_flag != CHAR_BLANK) &&
				(this->m_flag != CHAR_NEWLINE) )
			{
				if( (this->m_flag == CHAR_STRING) || (this->m_flag == CHAR_CHAR) )
				{
					str[size-size1-1] = 0;
					size1 = (int)strlen(str);
					memcpy(str, str+1, size1-1);
					return size1-1;
				}
				return size-size1;
			}
			if(this->m_flag == CHAR_NEWLINE)
				this->m_line++;
			pnode = &this->m_root;
			ptr = str;
			size1 = size;
			continue;
		}
		ptr++;
		size1--;
	}
	CFifoBuf::SetReadPoint(rpoint);
	return 0;
}

int	CCharFifo::GetCurrentUnitProp(void)
{
	return this->m_flag;
}

int	CCharFifo::GetLine(void)
{
	return this->m_line;
}
