

#ifndef _DBUGTOOLS_H_
#define _DBUGTOOLS_H_



#ifndef TOOLFILE_DLL
	#define TOOLFILE_DLL  __declspec(dllimport)
#endif

#include "filebin.h"


enum DBUG_MODEenum
{
	DBUG_STOREMSG,
	DBUG_NOTSTOREMSG
};

class TOOLFILE_DLL CDbugTools : public CFileBin
{
public:
	CDbugTools(char *df = NULL, int size = 0, int mode = DBUG_NOTSTOREMSG);
	virtual ~CDbugTools(void);

public:
	void	PrintMsg(const char *Format, ...);
	void	RecodeData(const char *Format, ...);
	void	RecodeBin(int val, int len);
	void	RecodeString(char *str);
	void	Open(char *df, int size = 0, int mode = DBUG_NOTSTOREMSG);
	void	Open(char *path, char *df, int size = 0, int mode = DBUG_NOTSTOREMSG);

protected:
	

private:
	int		m_mode;
};



#endif


