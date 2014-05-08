#ifndef _FRAMEBUF_H_
#define _FRAMEBUF_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif


#include "bufferbase.h"

/*=================================================================*/

/*=================================================================*/


class TOOLBUF_DLL CFrameBaseBuf : public CBufferBase
{
public:
	CFrameBaseBuf(int width = 640, int height = 480);
	virtual ~CFrameBaseBuf();

public:
	int				Write(char *buf, int x_pos, int y_pos);
	virtual int		Read(char *buf, int x_pos, int y_pos, int xflag = 0, int yflag = 0);
	virtual int		Readfield(char *buf, int x_pos, int y_pos, int xflag, int yflag, int field);

public:
	void	SetFrameSize(int width = 640, int height = 480);
	void	ChangeBufPoint(int offset);

protected:
	void	clear(void);

private:
	int		m_width;
	int		m_height;
	int		m_flag;
};


typedef struct tag_TFrameBuf {
	CFrameBaseBuf	YBuf;
	CFrameBaseBuf	UBuf;
	CFrameBaseBuf	VBuf;

} TFrameBuf, *PTFrameBuf;


#endif

