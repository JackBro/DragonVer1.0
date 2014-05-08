#ifndef _RECORD_BUFFER_H_
#define _RECORD_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

#include "fifobuf.h"

#define RECODE_BUFFER_LEN	0x10000
#define	RECODE_LIST_LEN		0x1000

class TOOLBUF_DLL CRecordBuf : public CFifoBuf
{
public:
	CRecordBuf(int size = RECODE_BUFFER_LEN, int listlen = RECODE_LIST_LEN);

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);

public:
	void	SetSize(int size = RECODE_BUFFER_LEN, int listlen = RECODE_LIST_LEN);
	void	SetSize(char *buf, int size = RECODE_BUFFER_LEN, int listlen = RECODE_LIST_LEN);
	void	ClearPoint(void);

protected:

protected:
	
private:
	CFifoBuf	m_pos;
};

#endif


