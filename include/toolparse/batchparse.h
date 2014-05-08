#ifndef _BATCHPARSE_H_
#define _BATCHPARSE_H_


#ifndef TOOLPARSE_DLL
	#define TOOLPARSE_DLL  __declspec(dllimport)
#endif

#include "toolbuf/bufferbase.h"
#include "toolfile/dbugtools.h"


enum REGTYPEenum
{
	V568_REGTYPE,
	V558_REGTYPE,
	SENSOR_REGTYPE,
	PANEL_REGTYPE,
	PANEL_GRAMLINETYPE,
	HOST_REGTYPE,
	LCD_BY_SETRSLOW,
	LCD_BY_SETRSHIGH,
	LCD_BY_SETREG,
	LCD_BY_GETRSLOW,
	LCD_BY_GETRSHIGH,
	LCD_BY_GETREG,
	USB9604_REG,
	V938_REGTYPE
};

typedef struct tag_TRegUnit
{
	int		regtype;
	char	*regname;
	int		regaddr;
	int		regval;
	int		delay;
	char	*desc;
} TRegUnit, *PTRegUnit;

typedef struct tag_TRegList
{
	char		*regclass;
	TRegUnit	*parray;
	int		count;
} TRegList, *pTRegList;

#define KEY_MAXCHAR		100

typedef struct tag_TKeyStringMap {
	int		index;
	char	keychar[KEY_MAXCHAR];
	char	value[KEY_MAXCHAR];
	int		select;
	int		type;
} TKeyStringMap, *PTKeyStringMap;

#define BATCH_MAXLINEITEM	10

enum PARSETYPEenum
{
	BATCH_OVER,
	BATCH_REGTYPE,
	BATCH_STRINGTYPE,
	BATCH_UNKNOWTYPE,
	BATCH_WRITE_REG,
	BATCH_STRING,
	BATCH_FAILED
};

//
//	key: '#' sensor flag
//	key: '~' delay time flag
//	key: ';' comment flag
//	key: '//' comment flag
//

#define BATCH_DEFAULT_KEYREG	'@'
#define BATCH_DEFAULT_KEYSTR	'='
#define BATCH_REGTYPE_SENSOR	'#'
#define BATCH_REGTYPE_BY16_CTL	'%'
#define BATCH_REGTYPE_BY16_VAL	'&'
#define BATCH_REGTYPE_BY16_REG	'$'


class TOOLPARSE_DLL CBatchParse : public CBufferBase
{
public:
	CBatchParse(char *filename = NULL, int size = BUFFER_DEFAULT_SIZE);
	virtual ~CBatchParse();

public:
	void	OpenBatchFile(char *filename);
	int		GetNextUnit(char *unit);
	void	SetKey2Type(char keyreg = BATCH_DEFAULT_KEYREG, char keystr = BATCH_DEFAULT_KEYSTR);

protected:
	int		GetContent(void);
	int		EncountComment(void);
	int		GetLineItems(char **ptr, int total);
	int		GetItemType(char **ptr, int total);
	int		DeleteMark(char **ptr, int *total, char ch = BATCH_DEFAULT_KEYSTR, int maxv = BATCH_MAXLINEITEM);
	int		ParseRegUnitItem(char **ptr, int total, char *punit);
//	int		ParseStringItem(char **ptr, int total, TKeyStringMap *unit);
	void	CloseBatch(void);

private:
	int		m_filepos;
	int		m_pos;
	int		m_len;
	int		m_line;
	char	m_keyreg;
	char	m_keystr;
	FILE	*m_fp;
	CDbugTools m_dbug;
};

extern TOOLPARSE_DLL TKeyStringMap gKeyMap[];
extern TOOLPARSE_DLL const int gKeyCount;


#endif


