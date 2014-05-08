#ifndef _CHARFIFO_H_
#define _CHARFIFO_H_


#ifndef TOOLCHAR_DLL
	#define TOOLCHAR_DLL  __declspec(dllimport)
#endif

#include "charnode.h"
#include "toolbuf/fifobuf.h"


//=================================

class TOOLCHAR_DLL CCharFifo : public CFifoBuf
{
public:
	CCharFifo(void);
	virtual ~CCharFifo(void);

public:
	virtual int	Read(char *str, int size);

public:
	int		GetCurrentUnitProp(void);
	int		GetLine(void);

private:
	void	Init(void);

private:
	void	SetNodeProp(void);
	void	BuildFloatNode(void);
	void	BuildCommentsNode(void);
	void	BuildStringNode(void);
	void	BuildCharNode(void);
	void	InitGraph(void);

private:
	CCharNode	m_root;
	CCharNode	m_decdata;
	CCharNode	m_octdata;
	CCharNode	m_hexdata;
	CCharNode	m_word;
	CCharNode	m_wordh;
	CCharNode	m_ohnode;

	CCharNode	m_not;
	CCharNode	m_dbquat;
	CCharNode	m_module;
	CCharNode	m_and;
	CCharNode	m_quatation;

	CCharNode	m_mul;
	CCharNode	m_plus;
	CCharNode	m_minus;
	CCharNode	m_div;

	CCharNode	m_less;
	CCharNode	m_equal;
	CCharNode	m_big;

	CCharNode	m_space;
	CCharNode	m_enter;

	CCharNode	m_slash;
	CCharNode	m_xor;
	CCharNode	m_or;

	CCharNode	m_wplus;
	CCharNode	m_wminus;

	CCharNode	m_wpluse;
	CCharNode	m_wminuse;
	CCharNode	m_wmule;
	CCharNode	m_wdive;
	CCharNode	m_wmode;
	CCharNode	m_wande;
	CCharNode	m_wore;
	CCharNode	m_wxore;
	CCharNode	m_wor;
	CCharNode	m_wequal;
	CCharNode	m_wand;
	CCharNode	m_wnote;
	CCharNode	m_wless;
	CCharNode	m_wbig;

	CCharNode	m_float;
	CCharNode	m_floatp;
	CCharNode	m_floate;

	CCharNode	m_char;
	CCharNode	m_chari;
	CCharNode	m_charis;
	CCharNode	m_charo;
	CCharNode	m_chard;
	CCharNode	m_charx;

	CCharNode	m_cmtsi;
	CCharNode	m_cmtss;
	CCharNode	m_cmtsd;
	CCharNode	m_comments;

	CCharNode	m_str;
	CCharNode	m_strh;

private:
	int		m_flag;
	int		m_line;
};

#endif

