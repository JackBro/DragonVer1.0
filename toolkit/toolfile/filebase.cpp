
#include "internal.h"

CFileBase::CFileBase(char *filename, char *rwmode)
{
	this->m_fp = NULL;
	this->Open(filename, rwmode);
}

CFileBase::~CFileBase()
{
	this->Close();
}

#if 0
int	CFileBase::ParseRWmode(char *rwmode)
{
	if(rwmode == NULL)
		return 0;
	if(rwmode[0] == '\0')
		return 0;
	rwmode[1] = 'b';
	rwmode[2] = 0;
	if(rwmode[0] == 'w')
		return FILEOP_WRITE | FILEOP_BINMODE;
	if(rwmode[0] == 'r')
		return FILEOP_READ | FILEOP_BINMODE;
	return 0;
}

#else
int	CFileBase::ParseRWmode(char *rwmode)
{
	int len, mode = 0, i = 1;

	len = (int)strlen(rwmode);
	if(len == 0)
		return 0;
	switch(rwmode[0])
	{
	case 'w':
		mode = FILEOP_WRITE;
		break;
	case 'r':
		mode = FILEOP_READ;
		break;
	case 'a':
		mode = FILEOP_APPEND;
		break;
	default:
		return 0;
	}
	while(i < len)
	{
		switch(rwmode[i])
		{
		case 'b':
			mode |= FILEOP_BINMODE;
			break;
		case '+':
			mode |= FILEOP_PLUS;
			return 0;
			break;
		case 't':
			mode |= FILEOP_TEXTMODE;
		default:
			break;
		}
		i++;
	}
	return mode;
}
#endif

void CFileBase::Open(char *filename, char *rwmode)
{
	char mode[10];

	this->Close();
	if(filename == NULL)
		return;
	if(filename[0] == '\0')
		return;
	memcpy(mode, rwmode, 4);
	mode[4] = 0;
	this->m_rwmode = this->ParseRWmode(mode);
	if(this->m_rwmode)
		this->m_fp = fopen(filename, mode);
}

void CFileBase::Close(void)
{
	if(this->m_fp)
		fclose(this->m_fp);
	this->m_fp = NULL;
	this->m_rwmode = 0;
}

int CFileBase::IsReadMode(void)
{
	if( (this->m_rwmode & FILEOP_READ) || (this->m_rwmode &	FILEOP_APPEND) )
		return 1;
	return 0;
}

int CFileBase::Read(char *buf, int len)
{
	if(buf == NULL)
		return 0;
	if(len <= 0)
		return 0;
	if( (this->m_fp) && this->IsReadMode() )
		return (int)fread(buf, 1, len, this->m_fp);
	return 0;
}

int CFileBase::Write(char *buf, int len)
{
	if(buf == NULL)
		return 0;
	if(len <= 0)
		return 0;
	if( (this->m_fp) && (this->IsReadMode() == 0) )
		return (int)fwrite(buf, 1, len, this->m_fp);
	return 0;
}

int CFileBase::IsOpened(void)
{
	if(this->m_fp)
		return 1;
	return 0;
}

int CFileBase::Putch(char ch)
{
	if(this->m_fp)
		return fputc(ch, this->m_fp);
	return EOF;
}

int CFileBase::Getch(void)
{
	if(this->m_fp)
		return fgetc(this->m_fp);
	return 0;
}

char *CFileBase::Gets(char *str, int n)
{
	if (this->m_fp)
		return fgets(str, n, this->m_fp);
	return NULL;
}

void CFileBase::Fseek(long pos, int start)
{
	if(this->m_fp)
		fseek(this->m_fp, pos, start);
}

int CFileBase::Printf(const char *fmt, ...)
{
	va_list ap;
	char str[256];

	if(this->m_fp == NULL)
		return 0;
	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	fprintf(this->m_fp, "%s", str);
	return 0;
}

#ifdef WIN32
int CFileBase::Scanf(const char *fmt, ...)
{
	char c;
	
	c = fmt[0];
	return 0;
}
#else
int CFileBase::Scanf(const char *fmt, ...)
{
	va_list  argptr; 
	int cnt; 

	if(this->m_fp == NULL)
		return 0;
	va_start(argptr, fmt); 
	cnt = vfscanf(this->m_fp, fmt, argptr); 

	va_end(argptr); 

	return(cnt); 
}
#endif

int CFileBase::Ftell(void)
{
	if(this->m_fp == NULL)
		return 0;
	return ftell(this->m_fp);
}

int CFileBase::GetFileLength(void)
{
	int pos;
	int len = 0;

	if(this->m_fp == NULL)
		return 0;
	pos = this->Ftell();
	fseek(this->m_fp, 0, SEEK_END);
	len = this->Ftell();
	fseek(this->m_fp, pos, SEEK_SET);
	return len;
}


