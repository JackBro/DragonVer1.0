#ifndef _FILEBASE_H_
#define _FILEBASE_H_


#ifndef TOOLFILE_DLL
	#define TOOLFILE_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"

enum FILE_OPMODEenum
{
	FILEOP_BINMODE		= 1,
	FILEOP_TEXTMODE		= 2,

	FILEOP_PLUS			= 4,

	FILEOP_WRITE		= 8,
	FILEOP_READ			= 0x10,
	FILEOP_APPEND		= 0x20
};

//	suggestiong:
//	this class only support bin file mode, read or write not append
//
class TOOLFILE_DLL CFileBase : public CBaseObject
{
public:
	CFileBase(char *filename = NULL, char *rwmode = "rb");
	virtual ~CFileBase();

public:
	void Open(char *filename = NULL, char *rwmode = "rb");
	virtual void Close(void);
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	virtual int IsOpened(void);	//no need to call this function, because R/W is safe avoid abnormal pointer

public:
	int IsReadMode(void);
	int GetFileLength(void);

public:		//shouldn't support these function since it break the module construct
	int Putch(char ch);
	int Getch(void);
	int Printf(const char *fmt, ...);
	int Scanf(const char *fmt, ...);	//null in windows platform
	void Fseek(long pos, int start);
	int Ftell(void);
	char *Gets(char *str, int n);

protected:
	int	ParseRWmode(char *rwmode);

private:
	FILE *m_fp;
	int m_rwmode;
};

#endif


