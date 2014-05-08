
#ifndef _FILEINPUT_H_
#define _FILEINPUT_H_


#ifndef TOOLFILE_DLL
	#define TOOLFILE_DLL  __declspec(dllimport)
#endif

#include "filebin.h"

class TOOLFILE_DLL CFileInput : public CFileBin
{
public:
	CFileInput(char *filename = NULL, int size = BUFFER_DEFAULT_SIZE);

public:
	void	Open(char *df);
	void	Open(char *path, char *df);
};

#endif


