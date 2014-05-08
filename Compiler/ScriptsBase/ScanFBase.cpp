
#include "internal.h"




CScanFBase::CScanFBase(char *filename, int scriptmode) : CFileBin(filename) , CScannerBase(scriptmode)
{
	char errfilename[MAX_FILE_NAME_LENGTH];
	if (filename != NULL)
	{
		sprintf(errfilename, "%s.err", filename);
		m_errfile.Open(errfilename, "wb");
	}
	this->open(filename);
}

CScanFBase::~CScanFBase()
{
}

void CScanFBase::Process(char *outfile)
{
	if(outfile == NULL)
		return;
	CFileBin out(outfile);
	
}

int CScanFBase::open(char *filename)
{
	int len;
	CBufferBase buf;
	struct stat statbuf;

	if (stat (filename, &statbuf) == -1) 
		return 0;
	len = statbuf.st_size;	
	if (len == -1) 
		return 0;

	buf.SetSize(len+4);
	if (len+4 > CCharFifo::m_totalsize) 
		CCharFifo::SetSize(len+4);

	CFileBin::Open(filename, "rb");
	CFileBin::Read(buf.m_buf, len+1);

	CCharFifo::Write(buf.m_buf, len+1);

	return 1;
}

void CScanFBase::WriteLog()
{
	char errstr[MAX_TOKEN_LENGTH];
	sprintf(errstr, "(Lexical Error at line:%d) %s\n", this->m_line, m_errorMsg);
	this->m_errfile.Write(errstr, (int)strlen(errstr));
}