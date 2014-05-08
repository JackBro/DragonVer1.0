#ifndef _SCANFILEBASE_H_
#define _SCANFILEBASE_H_


#ifndef SCRIPTSBASE_DLL
	#define SCRIPTSBASE_DLL _declspec(dllimport)
#endif

#include "ScannerBase.h"
#include "toolfile/filebin.h"


#define MAX_FILE_NAME_LENGTH 128

class SCRIPTSBASE_DLL CScanFBase : public CScannerBase, public CFileBin
{
public:
	CScanFBase(char *filename = NULL, int scriptmode = SCRIPT_C);
	virtual ~CScanFBase();

public:
	void Process(char *outfile = NULL);
	int open(char * filename);
	void WriteLog();
	virtual TOKEN NextToken()=0;

public:
	CFileBin m_errfile;
};



#endif
