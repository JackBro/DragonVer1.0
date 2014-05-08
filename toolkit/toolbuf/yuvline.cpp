
#include "internal.h"

CYuv444Fifo::CYuv444Fifo(int width, int unit)
{
	CYuv444Fifo::SetSize(width, unit);
}

CYuv444Fifo::~CYuv444Fifo()
{
}

void CYuv444Fifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize(width * 3, unit);
}

void CYuv444Fifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, width * 3, unit);
}

int	CYuv444Fifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv444Fifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

void CYuv444Fifo::Write(int *yuv)	//per 4 points
{
	int i;

	for(i = 0; i < 12; i++)
		CLineFifo::Write(yuv[i]);
}

void CYuv444Fifo::Read(int *yuv)
{
	int i;

	for(i = 0; i < 12; i++)
		yuv[i] = CLineFifo::Read();
}

//========================================
CYuv400Fifo::CYuv400Fifo(int width, int unit) : CLineFifo(width, unit)
{
	this->m_format = YUV400;
}

CYuv400Fifo::~CYuv400Fifo()
{
}

void CYuv400Fifo::Write(int *yuv, int format)	//per 4 points
{
	this->SetFormat(format);
	this->Write(yuv);
}

void CYuv400Fifo::Read(int *yuv, int format)
{
	this->SetFormat(format);
	this->Read(yuv);
}

void CYuv400Fifo::Write(int *yuv)
{
	CLineFifo::Write(yuv[0]);
	CLineFifo::Write(yuv[3]);
	CLineFifo::Write(yuv[6]);
	CLineFifo::Write(yuv[9]);
}

void CYuv400Fifo::Read(int *yuv)
{
	int i, u;

	u = this->m_unit == 1 ? 0x80 : 0x200;

	for(i = 0; i < 12; i += 3)
	{
		yuv[i] = CLineFifo::Read();
		yuv[i+1] = u;
		yuv[i+2] = u;
	}
//	yuv[3] = CLineFifo::Read();
//	yuv[6] = CLineFifo::Read();
//	yuv[9] = CLineFifo::Read();
}

void CYuv400Fifo::SetFormat(int format)
{
	this->m_format = format & FORMATMASK;
}

int	CYuv400Fifo::GetFormat(void)
{
	return this->m_format | (1 << CLASSSHIFT);
}

int	CYuv400Fifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv400Fifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

//======================================
CYuv411Fifo::CYuv411Fifo(int width, int unit)
{
	CYuv411Fifo::SetSize(width, unit);
}

CYuv411Fifo::~CYuv411Fifo()
{
}

void CYuv411Fifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize((width * 3) >> 1, unit);
}

void CYuv411Fifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, (width * 3) >> 1, unit);
}

void CYuv411Fifo::Write(int *yuv)	//per 4 points
{
	int i;

	for(i = 0; i < 6; i ++)
		CLineFifo::Write(yuv[gImgStreamOrder[this->m_format][i]]);
}

void CYuv411Fifo::Read(int *yuv)
{
	int i;

	for(i = 0; i < 6; i++)
		yuv[gImgStreamOrder[this->m_format][i]] = CLineFifo::Read();
	yuv[4] = yuv[7] = yuv[10] = yuv[1];
	yuv[5] = yuv[8] = yuv[11] = yuv[2];
}

int	CYuv411Fifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv411Fifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

//======================================

CYuv422Fifo::CYuv422Fifo(int width, int unit)
{
	CYuv422Fifo::SetSize(width, unit);
}

CYuv422Fifo::~CYuv422Fifo()
{
}

void CYuv422Fifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize(width << 1, unit);
}

void CYuv422Fifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, width << 1, unit);
}

void CYuv422Fifo::Write(int *yuv)	//per 4 points
{
	int i;

	for(i = 0; i < 4; i++)
		CLineFifo::Write(yuv[gImgStreamOrder[this->m_format][i]]);
	for(i = 0; i < 4; i++)
		CLineFifo::Write(yuv[6+gImgStreamOrder[this->m_format][i]]);
}

void CYuv422Fifo::Read(int *yuv)
{
	int i;

	for(i = 0; i < 4; i++)
		yuv[gImgStreamOrder[this->m_format][i]] = CLineFifo::Read();
	for(i = 0; i < 4; i++)
		yuv[6+gImgStreamOrder[this->m_format][i]] = CLineFifo::Read();
	yuv[4] = yuv[1];
	yuv[5] = yuv[2];
	yuv[10] = yuv[7];
	yuv[11] = yuv[8];
}

int	CYuv422Fifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv422Fifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

//========================================
CYuv422_UFifo::CYuv422_UFifo(int width, int unit)
{
	CYuv422_UFifo::SetSize(width, unit);
}

CYuv422_UFifo::~CYuv422_UFifo()
{
}

void CYuv422_UFifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize(width >> 1, unit);
}

void CYuv422_UFifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, width >> 1, unit);
}

int	CYuv422_UFifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv422_UFifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

void CYuv422_UFifo::Write(int *yuv)
{
	CLineFifo::Write(yuv[1]);
	CLineFifo::Write(yuv[7]);
}

void CYuv422_UFifo::Read(int *yuv)
{
	yuv[1] = CLineFifo::Read();
	yuv[7] = CLineFifo::Read();
}

//========================================
CYuv422_VFifo::CYuv422_VFifo(int width, int unit)
{
	CYuv422_VFifo::SetSize(width, unit);
}

CYuv422_VFifo::~CYuv422_VFifo()
{
}

int	CYuv422_VFifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv422_VFifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

void CYuv422_VFifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize(width >> 1, unit);
}

void CYuv422_VFifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, width >> 1, unit);
}

void CYuv422_VFifo::Write(int *yuv)
{
	CLineFifo::Write(yuv[2]);
	CLineFifo::Write(yuv[8]);
}

void CYuv422_VFifo::Read(int *yuv)
{
	yuv[2] = CLineFifo::Read();
	yuv[8] = CLineFifo::Read();
}

//========================================
CYuv444_UFifo::CYuv444_UFifo(int width, int unit) : CYuv400Fifo(width, unit)
{
}

CYuv444_UFifo::~CYuv444_UFifo()
{
}

int	CYuv444_UFifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv444_UFifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

void CYuv444_UFifo::Write(int *yuv)
{
	CLineFifo::Write(yuv[1]);
	CLineFifo::Write(yuv[4]);
	CLineFifo::Write(yuv[7]);
	CLineFifo::Write(yuv[10]);
}

void CYuv444_UFifo::Read(int *yuv)
{
	yuv[1] = CLineFifo::Read();
	yuv[4] = CLineFifo::Read();
	yuv[7] = CLineFifo::Read();
	yuv[10] = CLineFifo::Read();
}

//========================================
CYuv444_VFifo::CYuv444_VFifo(int width, int unit) : CYuv400Fifo(width, unit)
{
}

CYuv444_VFifo::~CYuv444_VFifo()
{
}

int	CYuv444_VFifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv444_VFifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

void CYuv444_VFifo::Write(int *yuv)
{
	CLineFifo::Write(yuv[2]);
	CLineFifo::Write(yuv[5]);
	CLineFifo::Write(yuv[8]);
	CLineFifo::Write(yuv[11]);
}

void CYuv444_VFifo::Read(int *yuv)
{
	yuv[2] = CLineFifo::Read();
	yuv[5] = CLineFifo::Read();
	yuv[8] = CLineFifo::Read();
	yuv[11] = CLineFifo::Read();
}

//========================================
CYuv411_UFifo::CYuv411_UFifo(int width, int unit)
{
	CYuv411_UFifo::SetSize(width, unit);
}

CYuv411_UFifo::~CYuv411_UFifo()
{
}

void CYuv411_UFifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize(width >> 2, unit);
}

void CYuv411_UFifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, width >> 2, unit);
}

int	CYuv411_UFifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv411_UFifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

void CYuv411_UFifo::Write(int *yuv)
{
	CLineFifo::Write(yuv[1]);
}

void CYuv411_UFifo::Read(int *yuv)
{
	yuv[1] = CLineFifo::Read();
}

//========================================
CYuv411_VFifo::CYuv411_VFifo(int width, int unit)
{
	CYuv411_VFifo::SetSize(width, unit);
}

CYuv411_VFifo::~CYuv411_VFifo()
{
}

int	CYuv411_VFifo::Write(char *buf, int size)
{
	return CLineFifo::Write(buf, size);
}

int	CYuv411_VFifo::Read(char *buf, int size)
{
	return CLineFifo::Read(buf, size);
}

void CYuv411_VFifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize(width >> 2, unit);
}

void CYuv411_VFifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, width >> 2, unit);
}

void CYuv411_VFifo::Write(int *yuv)
{
	CLineFifo::Write(yuv[2]);
}

void CYuv411_VFifo::Read(int *yuv)
{
	yuv[2] = CLineFifo::Read();
}
