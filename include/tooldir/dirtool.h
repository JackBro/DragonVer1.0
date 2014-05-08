#ifndef _DIRECTORY_TOOLS_H_
#define _DIRECTORY_TOOLS_H_



#ifndef TOOLDIR_DLL
	#define TOOLDIR_DLL  __declspec(dllimport)
#endif


#define DEFINE_ROOTDIR	"output"

class TOOLDIR_DLL CDirTools
{
public:
	CDirTools(char *root = DEFINE_ROOTDIR);

public:
	void CreateDir(char *path);
	void DeleteDir(char *path);
	void GetRoot(char *root);

private:
	void SetRoot(char *root);
	int  CheckPath(char *src, char *dest = NULL);
	void RmDir(char *path);

private:
	char m_root[0x100];
};


#endif


