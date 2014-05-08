#ifndef _BAYER_5X5_RGB_H_
#define _BAYER_5X5_RGB_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif


#include "LineBayerBuf.h"
//============================
//
//		class C5x5Base
//
//===============================
#define LAST5X5LINES	BIT6

class TOOLCONVERT_DLL C5x5Base
{
public:
	C5x5Base(int width = 128, int unit = 1);
	virtual ~C5x5Base();

public:
	void	SetSize(int width = 128, int unit = 1);
	void	SetOrder(int order);

public:
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

protected:
	void	Clear(void);
	void	GetFirstGblock(void);
	void	GetNextGblock(void);
	void	LastLinePro(void);
	void	LoopLinebuf(void);
	int		process(char *buf, int len);
	int		GetGpixel(int col, int row);
	int		GetRfromBG(int pos);
	int		GetRfromGR(int pos);
	int		GetRfromGB(int pos);

private:
	CLineBayerBuf	*m_pLinebuf[9];
	CLineBayerBuf	m_Linebuf[9];
	int			m_Gblock[5][5];
	int			*m_pGblock[5];
	int			m_line, m_pos;
	int			m_unit, m_width;
	int			m_Gfirst, m_GBline;
};

#endif


