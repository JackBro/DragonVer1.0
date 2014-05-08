
#include "internal.h"


CDbugTools::CDbugTools(char *df, int size, int mode) :
	CFileBin(df, "wb", size)
{
//	this->Open(df, size, mode);
	this->m_mode = mode;
}

CDbugTools::~CDbugTools(void)
{
}


void CDbugTools::Open(char *df, int size, int mode)
{
	CFileBin::Open(df, "wb", size);
	this->m_mode = mode;
}

void CDbugTools::Open(char *path, char *df, int size, int mode)
{
	char filename[0x100] = {0};

	if(df != NULL)
		strcpy(filename, df);
	if( (path != NULL) && (df != NULL) )
		sprintf(filename, "%s%s", path, df);
	this->Open(filename, size, mode);
}

void CDbugTools::RecodeString(char *str)
{
	int len = (int)strlen(str);
	CFileBin::Write(str, len);
}

void CDbugTools::RecodeData(const char *Format, ...)
{
	char tmsg[256];
	va_list va;

	va_start( va, Format );  
	vsprintf(tmsg, Format, va);
	va_end( va );   

	this->RecodeString(tmsg);
}

void CDbugTools::RecodeBin(int val, int len)
{
	char tmsg[256];
	int i;

	tmsg[0] = 0;
	for(i = len-1; i >= 0; i--)
	{
		if( val & (1 << i) )
			sprintf(tmsg, "%s1", tmsg);
		else
			sprintf(tmsg, "%s0", tmsg);
	}
	sprintf(tmsg, "%s ",tmsg);

	this->RecodeString(tmsg);
}

#ifdef WIN32
void CDbugTools::PrintMsg(const char *Format, ...)
{
	char tmsg[256];
	va_list va;

	va_start( va, Format );  
	vsprintf(tmsg, Format, va);
	va_end( va );   
	AfxMessageBox(tmsg);

	if(this->m_mode == DBUG_STOREMSG)
		this->RecodeString(tmsg);
}
#else
void CDbugTools::PrintMsg(const char *Format, ...)
{
	char tmsg[256];
	va_list va;

	va_start( va, Format );  
	vsprintf(tmsg, Format, va);
	va_end( va );   
	printf("%s", tmsg);

	if(this->m_mode == DBUG_STOREMSG)
		this->RecodeString(tmsg);
}

#endif




