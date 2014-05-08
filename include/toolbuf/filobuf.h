#ifndef _FILO_BUFFER_H_
#define _FILO_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

#include "srambuf.h"

class TOOLBUF_DLL CFiloBuf : public CSramBuf
{
public:
	CFiloBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CFiloBuf();

public:
	virtual int		Write(char *buf, int size);		// size unit = sizeof(char)
	virtual int		Read(char *buf, int size);		// size unit = sizeof(char)

public:
	int		Write(int *buf, int size);		// size unit = sizeof(int)
	int		Write(short int *buf, int size);// size unit = sizeof(short int)

	int		Read(char *buf, int size, int bw);
	int		Read(int *buf, int size);
	int		Read(short int *buf, int size);

	void	ClearPoint(void);

private:
	int		m_wpoint;
};

#endif


