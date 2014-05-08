
#include "internal.h"

CFileInput::CFileInput(char *filename, int size) : CFileBin(filename, "rb", size)
{
//	this->Open(filename);
}

void CFileInput::Open(char *df)
{
	CFileBin::Open(df, "rb");
}

void CFileInput::Open(char *path, char *df)
{
	char filename[0x100] = {0};

	if(df != NULL)
		strcpy(filename, df);
	if( (path != NULL) && (df != NULL) )
		sprintf(filename, "%s%s", path, df);
	CFileBin::Open(filename, "rb");
}
