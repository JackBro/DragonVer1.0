
#include "internal.h"

//GET YUV ORDER BY INDEX
#define G_ODRI(i) ( (int)gImgStreamOrder[m_yuvfmt][i] )


//======================================= class CYuvBlkBufBase ====================================================
CYuvBlkBufBase::CYuvBlkBufBase(char fmt, int width, int unit)
{
	CYuvBlkBufBase::SetSize(fmt, width, unit);
}

CYuvBlkBufBase::~CYuvBlkBufBase()
{
}

void CYuvBlkBufBase::ClearPoint(void)
{
	this->m_wpoint = 0;
	this->m_rpoint = 0;
}

void CYuvBlkBufBase::SetSize(char fmt, int width, int unit)
{
	m_unit = unit;
	m_yuvfmt = fmt;
	if( !AdjustBlkFmt(m_yuvfmt) )
	{
		m_width = 0;
		m_memsize = 0;
		CSramBuf::SetSize(0, unit);
		this->ClearPoint();
		return;
	}
	AdjustWidth(width);
	m_yuvsize = CalYuvSize(m_blkfmt, m_width, 8);
	m_memsize = CalAllocMemSize(m_blkfmt, m_width);
	CSramBuf::SetSize(m_memsize, unit);
	this->ClearPoint();
}

void CYuvBlkBufBase::AdjustWidth(int width)
{
	switch(m_blkfmt)
	{
	case YUV422BLK:
		m_width = ( width >> 1) << 1;
		break;
	case YUV420BLK:
		m_width = ( width >> 1) << 1;
		break;
	case YUV411BLK:
		m_width = ( width >> 2) << 2;
		break;
	case YUV444BLK:
	case YUV400BLK:
		m_width = width;
		break;
	default:
		m_width = 0;
		break;
	}
}

int CYuvBlkBufBase::CalYuvSize(UINT8 fmt, int width, int line)
{
	int size = 0;

	size = width * line;
	switch(fmt)
	{
	case YUV422BLK:
		size <<= 1;
		break;
	case YUV420BLK:
		size *= 3;
		break;
	case YUV411BLK:
		size *= 3;
		size >>= 1;
		break;
	case YUV444BLK:
		size *= 3;
		break;
	case YUV400BLK:
		break;
	default:
		size = 0;
		break;
	}
	return size;
}

int CYuvBlkBufBase::CalAllocMemSize(UINT8 fmt, int width)
{
	switch(fmt)
	{
	case YUV422BLK:
		width = ((width + 15) >> 4) << 4;
		break;
	case YUV420BLK:
		width = ((width + 15) >> 4) << 4;
		break;
	case YUV411BLK:
		width = ((width + 31) >> 5) << 5;
		break;
	case YUV444BLK:
	case YUV400BLK:
		width = ((width + 7) >> 3) << 3;
		break;
	default:
		width = 0;
		break;
	}
	return CalYuvSize(fmt, width, 8);
}

int CYuvBlkBufBase::AdjustBlkFmt(char fmt)
{
	if( (fmt >= YUV422_UYVY) && (fmt <= YUV422_YYUV) )
	{
		m_blkfmt = YUV422BLK;
		return 1;
	}
	if( (fmt >= YUV420_UYVY) && (fmt <= YUV420_YYUV) )
	{
		m_blkfmt = YUV420BLK;
		return 1;
	}
	if( (fmt >= YUV411_YYYYUV) && (fmt <= YUV411_VUYYYY) )
	{
		m_blkfmt = YUV411BLK;
		return 1;
	}
	if( fmt == YUV400 )
	{
		m_blkfmt = YUV400BLK;
		return 1;
	}
	if( fmt >= YUV444 )
	{
		m_blkfmt = YUV444BLK;
		return 1;
	}
	return 0;
}


//======================================= class CYuv2BlkBuf ====================================================

CYuv2BlkBuf::CYuv2BlkBuf(char fmt, int width, int unit)
{
	CYuv2BlkBuf::SetSize(fmt, width, unit);
}

CYuv2BlkBuf::~CYuv2BlkBuf()
{
}

void CYuv2BlkBuf::ClearPoint(void)
{
	CYuvBlkBufBase::ClearPoint();
}

int CYuv2BlkBuf::GetLineNum(void)
{
	return ( this->m_wpoint / (m_yuvsize>>3) );
}

void CYuv2BlkBuf::SetSize(char fmt, int width, int unit)
{
	CYuvBlkBufBase::SetSize(fmt, width, unit);
}

int CYuv2BlkBuf::Read(char *buf, int size)
{
	int end,start;

	if(this->GetLineNum() == 0)
		return 0;
	end = m_memsize * this->m_unit;
	start = this->m_rpoint * this->m_unit;
	size -= size % (this->m_unit);
	size = CSramBuf::Read(buf, size, start, end);
	this->m_rpoint += size;
	m_rpoint %= m_memsize;
	return size;
}

int	CYuv2BlkBuf::Write(char *buf, int size)
{
	size /= this->m_unit;
	if(size == 0)
		return 0;
	if(this->m_wpoint > m_yuvsize)
		return 0;
	switch(m_blkfmt)
	{
	case YUV422BLK://YUV422BLK
		size = WriteYuv422(buf, size);
		break;
	case YUV420BLK://YUV420BLK
		size = WriteYuv420(buf, size);
		break;
	case YUV411BLK://YUV411BLK
		size = WriteYuv411(buf, size);
		break;
	case YUV400BLK://YUV400BLK
		size = WriteYuv400(buf, size);
		break;
	case YUV444BLK://YUV444BLK
		size = WriteYuv444(buf, size);
		break;
	default:
		size = 0;
		break;
	}
	return size * this->m_unit;
}

int CYuv2BlkBuf::WriteYuv444(char *buf, int size)
{
	int	x, wsize, lineyuvsize, wlen = 0;
	char bdat = 0;

	size = (this->m_wpoint + size) > m_yuvsize ? m_yuvsize - this->m_wpoint : size;
	if(size == 0)
		return 0;
	wsize = size;
	lineyuvsize = m_yuvsize>>3;
	x = this->m_wpoint % lineyuvsize;
	wlen = WriteYuv4xxHead(buf, wsize, x, 3, &bdat);
	buf += wlen;
	wsize -= wlen;
	if(bdat)
		WriteYuv444Seg((char*)m_dat, 3);
	wlen = WriteYuv444Seg(buf, wsize);
	buf += wlen;
	wsize -= wlen;
	wlen = WriteYuv4xxTail(buf, wsize, 3);
	buf += wlen;
	wsize -= wlen;
	return size - wsize;
}

int CYuv2BlkBuf::WriteYuv422(char *buf, int size)
{
	int	x, wsize, lineyuvsize, wlen = 0;
	char bdat = 0;

	size = (this->m_wpoint + size) > m_yuvsize ? m_yuvsize - this->m_wpoint : size;
	if(size == 0)
		return 0;
	wsize = size;
	lineyuvsize = m_yuvsize>>3;
	x = this->m_wpoint % lineyuvsize;
	wlen = WriteYuv4xxHead(buf, wsize, x, 4, &bdat);
	buf += wlen;
	wsize -= wlen;
	if(bdat)
		WriteYuv422Seg((char*)m_dat, 4);
	wlen = WriteYuv422Seg(buf, wsize);
	buf += wlen;
	wsize -= wlen;
	wlen = WriteYuv4xxTail(buf, wsize, 4);
	buf += wlen;
	wsize -= wlen;
	return size - wsize;
}

int CYuv2BlkBuf::WriteYuv420(char *buf, int size)
{
	int	x, y, packnum, wsize, lineyuvsize, wlen = 0;
	char bdat = 0;

	size = (this->m_wpoint + size) > m_yuvsize ? m_yuvsize - this->m_wpoint : size;
	if(size == 0)
		return 0;
	wsize = size;
	lineyuvsize = m_yuvsize>>3;
	Get420WrPos(&x, &y);
	if(y & 0x1)
		packnum = 2;
	else
		packnum = 4;
	wlen = WriteYuv4xxHead(buf, wsize, x, packnum, &bdat);
	buf += wlen;
	wsize -= wlen;
	if(bdat)
		WriteYuv420Seg((char*)m_dat, packnum);
	wlen = WriteYuv420Seg(buf, wsize);
	buf += wlen;
	wsize -= wlen;
	Get420WrPos(&x, &y);
	if(y & 0x1)
		packnum = 2;
	else
		packnum = 4;
	wlen = WriteYuv4xxTail(buf, wsize, packnum);
	buf += wlen;
	wsize -= wlen;
	return size - wsize;
}

int CYuv2BlkBuf::WriteYuv411(char *buf, int size)
{
	int	x, wsize, lineyuvsize, wlen = 0;
	char bdat = 0;

	size = (this->m_wpoint + size) > m_yuvsize ? m_yuvsize - this->m_wpoint : size;
	if(size == 0)
		return 0;
	lineyuvsize = m_yuvsize>>3;
	wsize = size;
	x = this->m_wpoint % lineyuvsize;
	wlen = WriteYuv4xxHead(buf, wsize, x, 6, &bdat);
	buf += wlen;
	wsize -= wlen;
	if(bdat)
		WriteYuv420Seg((char*)m_dat, 6);
	wlen = WriteYuv411Seg(buf, wsize);
	buf += wlen;
	wsize -= wlen;
	wlen = WriteYuv4xxTail(buf, wsize, 6);
	buf += wlen;
	wsize -= wlen;
	return size - wsize;
}

int CYuv2BlkBuf::WriteYuv400(char *buf, int size)
{
	size = (this->m_wpoint + size) > m_yuvsize ? m_yuvsize - this->m_wpoint : size;
	if(size == 0)
		return 0;
	return WriteYuv400Seg(buf, size);
}

int  CYuv2BlkBuf::WriteYuv4xxHead(char *buf, int size, int x, int paknum, char* bdat)
{
	int i, pos;

	*bdat = 0;
	if(!size)
		return 0;
	pos = x % paknum;
	if(pos)
	{
		if( size < (paknum - pos) )
		{
			for(i = 0; i< size; i++, buf += m_unit)
				m_dat[pos + i] = m_basecvt.getvalue(buf, m_unit);
			m_wpoint += size;
			return size;
		}else
		{
			for(i = pos; i< paknum; i++, buf += m_unit)
				m_dat[i] = m_basecvt.getvalue(buf, m_unit);
			size = paknum;
			m_wpoint -= pos;
			*bdat = 1;
			return (paknum-pos);
		}
	}
	else
		return 0;
}

int  CYuv2BlkBuf::WriteYuv4xxTail(char *buf, int size, int paknum)
{
	int i;

	if(!size)
		return 0;
	if(size < paknum)
	{
		for(i=0; i<size; i++, buf += m_unit)
		{
			m_dat[i] = m_basecvt.getvalue(buf, m_unit);
		}
		m_wpoint += size;
		return size;
	}else
		return 0;
}

int  CYuv2BlkBuf::WriteYuv444Seg(char *buf, int size)
{
	int i, x, y, wlen, wsize, pos, lineyuvsize;

	if(!size)
		return 0;
	lineyuvsize = m_yuvsize>>3;
	wsize = size;
	while(wsize)
	{
		x = this->m_wpoint % lineyuvsize;
		y = this->m_wpoint / lineyuvsize;
		wlen = wsize > lineyuvsize ? lineyuvsize : wsize;
		wlen /= 3;
		if(wlen == 0)
			break;
		x /= 3;
		for(i=0; i< wlen; i++)
		{
			m_yuv[0] = m_basecvt.getvalue(buf, m_unit);
			m_yuv[1] = m_basecvt.getvalue(buf += m_unit, m_unit);
			m_yuv[2] = m_basecvt.getvalue(buf += m_unit, m_unit);
			buf += m_unit;
			pos = ( ( (x>>3)*192 ) + ( y<<3 ) + ( x&0x7 ) ) * m_unit;
			m_basecvt.setvalue(m_buf+pos, m_yuv[0], m_unit);
			pos += m_unit<<6;
			m_basecvt.setvalue(m_buf+pos, m_yuv[1], m_unit);
			pos += m_unit<<6;
			m_basecvt.setvalue(m_buf+pos, m_yuv[2], m_unit);
			x++;
		}
		m_wpoint += wlen*3;
		wsize -= wlen*3;
	}
	return (size - wsize);
}

int  CYuv2BlkBuf::WriteYuv422Seg(char *buf, int size)
{
	int i, x, y, wlen, wsize, pos, lineyuvsize;

	if(!size)
		return 0;
	lineyuvsize = m_yuvsize>>3;
	wsize = size;
	while(wsize)
	{
		x = this->m_wpoint % lineyuvsize;
		y = this->m_wpoint / lineyuvsize;
		wlen = wsize > lineyuvsize ? lineyuvsize : wsize;
		wlen >>= 2;
		if(wlen == 0)
			break;
		x >>= 2;
		for(i=0; i< wlen; i++)
		{
			m_yuv[G_ODRI(0)] = m_basecvt.getvalue(buf, m_unit);
			m_yuv[G_ODRI(1)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			m_yuv[G_ODRI(2)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			m_yuv[G_ODRI(3)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			buf += m_unit;
			pos = ( ( (x>>3)<<8 ) + ( ((x&0x7)>>2)<<6 ) + ( y<<3 ) + ( (x&0x3)<<1 ) ) * m_unit;
			m_basecvt.setvalue(m_buf+pos, m_yuv[0], m_unit);
			m_basecvt.setvalue(m_buf+pos+m_unit, m_yuv[3], m_unit);
			pos = ( ( (x>>3)<<8 ) + 128 + ( y<<3 ) + ( x&0x7 ) )*m_unit;
			m_basecvt.setvalue(m_buf+pos, m_yuv[1], m_unit);
			pos += m_unit<<6;
			m_basecvt.setvalue(m_buf+pos, m_yuv[2], m_unit);
			x++;
		}
		m_wpoint += wlen<<2;
		wsize -= wlen<<2;
	}
	return (size - wsize);
}

int CYuv2BlkBuf::WriteYuv420Seg(char *buf, int size)
{
	int i, x, y, wlen, wsize, pos, lineyuvsize;

	if(!size)
		return 0;
	lineyuvsize = m_yuvsize>>3;
	wsize = size;
	while(wsize)
	{

		Get420WrPos(&x, &y);
		if(y & 0x1)
		{
			wlen = wsize > ( lineyuvsize/3 - x ) ? ( lineyuvsize/3 - x ) : wsize;
			wlen >>= 1;
			if(wlen == 0)
				break;
			x >>= 1;
			for(i=0; i< wlen; i++)
			{
				m_yuv[6] = m_basecvt.getvalue(buf, m_unit);
				m_yuv[9] = m_basecvt.getvalue(buf += m_unit, m_unit);
				buf += m_unit;
				pos = ( (x>>3)*384 + ( (y>>3)<<7 ) + ( ((x&0x7)>>2)<<6 ) + ( (y&0x7)<<3 ) + ( (x&0x3)<<1 ) ) * m_unit;
				m_basecvt.setvalue(m_buf+pos, m_yuv[6], m_unit);
				m_basecvt.setvalue(m_buf+pos+m_unit, m_yuv[9], m_unit);
				x++;
			}
			m_wpoint += wlen<<1;
			wsize -= wlen<<1;
		}
		else
		{
			wlen = wsize > ( (lineyuvsize<<1)/3 - x ) ? ( (lineyuvsize<<1)/3 - x ) : wsize;
			wlen >>= 2;
			if(wlen == 0)
				break;
			x >>= 2;
			for(i=0; i< wlen; i++)
			{
				m_yuv[G_ODRI(0)] = m_basecvt.getvalue(buf, m_unit);
				m_yuv[G_ODRI(1)] = m_basecvt.getvalue(buf += m_unit, m_unit);
				m_yuv[G_ODRI(2)] = m_basecvt.getvalue(buf += m_unit, m_unit);
				m_yuv[G_ODRI(3)] = m_basecvt.getvalue(buf += m_unit, m_unit);
				buf += m_unit;
				pos = ( (x>>3)*384 + ( (y>>3)<<7 ) + ( ((x&0x7)>>2)<<6 ) + ( (y&0x7)<<3 ) + ( (x&0x3)<<1 ) ) * m_unit;
				m_basecvt.setvalue(m_buf+pos, m_yuv[0], m_unit);
				m_basecvt.setvalue(m_buf+pos+m_unit, m_yuv[3], m_unit);
				pos = ( (x>>3)*384 + 256 + (y<<2) + (x&0x7) )*m_unit;
				m_basecvt.setvalue(m_buf+pos, m_yuv[1], m_unit);
				pos += m_unit<<6;
				m_basecvt.setvalue(m_buf+pos, m_yuv[2], m_unit);
				x++;
			}
			m_wpoint += wlen<<2;
			wsize -= wlen<<2;
		}
	}
	return (size - wsize);
}

int  CYuv2BlkBuf::WriteYuv411Seg(char *buf, int size)
{
	int i, x, y, wlen, wsize, pos, lineyuvsize;

	if(!size)
		return 0;
	lineyuvsize = m_yuvsize>>3;
	wsize = size;
	while(wsize)
	{
		x = this->m_wpoint % lineyuvsize;
		y = this->m_wpoint / lineyuvsize;
		wlen = wsize > lineyuvsize ? lineyuvsize : wsize;
		wlen /= 6;
		if(wlen == 0)
			break;
		x /= 6;
		for(i=0; i< wlen; i++)
		{
			m_yuv[G_ODRI(0)] = m_basecvt.getvalue(buf, m_unit);
			m_yuv[G_ODRI(1)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			m_yuv[G_ODRI(2)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			m_yuv[G_ODRI(3)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			m_yuv[G_ODRI(4)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			m_yuv[G_ODRI(5)] = m_basecvt.getvalue(buf += m_unit, m_unit);
			buf += m_unit;
			pos = ( ( (x>>3)*384 ) + ( ((x&0x7)>>1)<<6 ) + ( y<<3 ) + ( (x&0x1)<<2 ) ) * m_unit;
			m_basecvt.setvalue(m_buf+pos, m_yuv[0], m_unit);
			m_basecvt.setvalue(m_buf+pos+m_unit, m_yuv[3], m_unit);
			m_basecvt.setvalue(m_buf+pos+(m_unit<<1), m_yuv[6], m_unit);
			m_basecvt.setvalue(m_buf+pos+(m_unit*3), m_yuv[9], m_unit);
			pos = ( (x>>3)*384 + 256 + ( y<<3 ) + ( x&0x7 ) )*m_unit;
			m_basecvt.setvalue(m_buf+pos, m_yuv[1], m_unit);
			pos += m_unit<<6;
			m_basecvt.setvalue(m_buf+pos, m_yuv[2], m_unit);
			x++;
		}
		m_wpoint += wlen*6;
		wsize -= wlen*6;
	}
	return (size - wsize);
}

int  CYuv2BlkBuf::WriteYuv400Seg(char *buf, int size)
{
	int i, x, y, wlen, wsize, pos, lineyuvsize;

	if(!size)
		return 0;
	lineyuvsize = m_yuvsize>>3;
	wsize = size;
	while(wsize)
	{
		x = this->m_wpoint % lineyuvsize;
		y = this->m_wpoint / lineyuvsize;
		wlen = wsize > lineyuvsize ? lineyuvsize : wsize;
		if(wlen == 0)
			break;
		for(i=0; i< wlen; i++)
		{
			m_yuv[0] = m_basecvt.getvalue(buf, m_unit);
			buf += m_unit;
			pos = ( ( (x>>3)<<6 ) + ( y<<3 ) + ( x&0x7 ) ) * m_unit;
			m_basecvt.setvalue(m_buf+pos, m_yuv[0], m_unit);
			x++;
		}
		m_wpoint += wlen;
		wsize -= wlen;
	}
	return (size - wsize);
}

void CYuv2BlkBuf::Get420WrPos(int* x, int* y)
{
	int temp, lineyuvsize;

	lineyuvsize = m_yuvsize>>3;
	*y = (this->m_wpoint / lineyuvsize)<<1;
	temp = this->m_wpoint % lineyuvsize;
	if(temp < (lineyuvsize<<1)/3)
		*x = temp;
	else
	{
		*x = temp - (lineyuvsize<<1)/3;
		*y += 1;
	}
}

//======================================= class CBlk2YuvBuf ====================================================


CBlk2YuvBuf::CBlk2YuvBuf(char fmt, int width, int unit)
{
	CBlk2YuvBuf::SetSize(fmt, width, unit);
}

CBlk2YuvBuf::~CBlk2YuvBuf()
{
}

void CBlk2YuvBuf::ClearPoint(void)
{
	CYuvBlkBufBase::ClearPoint();
}

int CBlk2YuvBuf::GetBlkNum(void)
{
	return ( this->m_wpoint );
}

void CBlk2YuvBuf::SetSize(char fmt, int width, int unit)
{
	CYuvBlkBufBase::SetSize(fmt, width, unit);

	this->ClearPoint();
}

int CBlk2YuvBuf::Read(char *buf, int size)
{
	int end, start, lineyuvsize, lineblksize, wsize, wlen;

	lineyuvsize = this->m_yuvsize>>3;
	lineblksize = this->m_memsize>>3;
	if(this->GetBlkNum() == 0)
		return 0;
	size /= this->m_unit;
	size = (m_rpoint + size) > m_yuvsize ? (m_yuvsize - m_rpoint) : size;
	wsize = size;
	while(size)
	{
		if(m_blkfmt == YUV420BLK)
		{
			if( (this->m_rpoint%lineyuvsize) < (lineyuvsize<<1)/3 )
			{
				start = ( lineblksize * (this->m_rpoint/lineyuvsize) + this->m_rpoint%lineyuvsize ) * this->m_unit;
				end = (lineblksize * (this->m_rpoint/lineyuvsize) + (lineyuvsize<<1)/3 ) * this->m_unit;
			}
			else
			{
				start = ( lineblksize * (this->m_rpoint/lineyuvsize) + ( (lineblksize-lineyuvsize)<<1 )/3 + this->m_rpoint%lineyuvsize ) * this->m_unit;
				end = (lineblksize * (this->m_rpoint/lineyuvsize) + (lineblksize<<1)/3 + lineyuvsize/3) * this->m_unit;
			}
			wlen = CSramBuf::Read(buf, size*this->m_unit, start, end);
			buf += wlen;
			this->m_rpoint += wlen/m_unit;
			size -= wlen/m_unit;
		}
		else
		{
			start = ( lineblksize * (this->m_rpoint/lineyuvsize) + this->m_rpoint%lineyuvsize ) * this->m_unit;
			end = (lineblksize * (this->m_rpoint/lineyuvsize) + lineyuvsize ) * this->m_unit;
			wlen = CSramBuf::Read(buf, size*this->m_unit, start, end);
			buf += wlen;
			this->m_rpoint += wlen/m_unit;
			size -= wlen/m_unit;
		}
	}
	m_rpoint %= m_yuvsize;
	return wsize;
}

int	CBlk2YuvBuf::Write(char *buf, int size)
{
	size /= this->m_unit;
	if(size == 0)
		return 0;
	switch(m_blkfmt)
	{
	case YUV422BLK://YUV422BLK
		size = WriteBlk422(buf, size);
		break;
	case YUV420BLK://YUV420BLK
		size = WriteBlk420(buf, size);
		break;
	case YUV411BLK://YUV411BLK
		size = WriteBlk411(buf, size);
		break;
	case YUV400BLK://YUV400BLK
		size = WriteBlk400(buf, size);
		break;
	case YUV444BLK://YUV444BLK
		size = WriteBlk444(buf, size);
		break;
	default:
		size = 0;
		break;
	}
	return size * this->m_unit;
}

int CBlk2YuvBuf::WriteBlk444(char *buf, int size)
{
	int i, totalnum, wrnum;

	totalnum = m_memsize/192;
	wrnum = size/192;
	wrnum = (this->m_wpoint + wrnum) > totalnum ? (totalnum - this->m_wpoint) : wrnum;
	if(wrnum == 0)
		return 0;
	for(i=0; i<wrnum; i++)
	{
		WriteBlk444Seg(buf);
		buf += 192 * this->m_unit;
		this->m_wpoint++;
	}
	return wrnum*192;
}

int CBlk2YuvBuf::WriteBlk422(char *buf, int size)
{
	int i, totalnum, wrnum;

	totalnum = m_memsize>>8;
	wrnum = size>>8;
	wrnum = (this->m_wpoint + wrnum) > totalnum ? (totalnum - this->m_wpoint) : wrnum;
	if(wrnum == 0)
		return 0;
	for(i=0; i<wrnum; i++)
	{
		WriteBlk422Seg(buf);
		buf += this->m_unit<<8;
		this->m_wpoint++;
	}
	return wrnum<<8;
}

int CBlk2YuvBuf::WriteBlk420(char *buf, int size)
{
	int i, totalnum, wrnum;

	totalnum = m_memsize/384;
	wrnum = size/384;
	wrnum = (this->m_wpoint + wrnum) > totalnum ? (totalnum - this->m_wpoint) : wrnum;
	if(wrnum == 0)
		return 0;
	for(i=0; i<wrnum; i++)
	{
		WriteBlk420Seg(buf);
		buf += 384 * this->m_unit;
		this->m_wpoint++;
	}
	return wrnum*384;
}

int CBlk2YuvBuf::WriteBlk411(char *buf, int size)
{
	int i, totalnum, wrnum;

	totalnum = m_memsize/384;
	wrnum = size/384;
	wrnum = (this->m_wpoint + wrnum) > totalnum ? (totalnum - this->m_wpoint) : wrnum;
	if(wrnum == 0)
		return 0;
	for(i=0; i<wrnum; i++)
	{
		WriteBlk411Seg(buf);
		buf += 384 * this->m_unit;
		this->m_wpoint++;
	}
	return wrnum*384;
}

int CBlk2YuvBuf::WriteBlk400(char *buf, int size)
{
	int i, totalnum, wrnum;

	totalnum = m_memsize>>6;
	wrnum = size>>6;
	wrnum = (this->m_wpoint + wrnum) > totalnum ? (totalnum - this->m_wpoint) : wrnum;
	if(wrnum == 0)
		return 0;
	for(i=0; i<wrnum; i++)
	{
		WriteBlk400Seg(buf);
		buf += this->m_unit<<6;
		this->m_wpoint++;
	}
	return wrnum<<6;
}

void CBlk2YuvBuf::WriteBlk444Seg(char *buf)
{
	char* pdst;
	int i, j, pos, linememsize;

	linememsize = m_memsize>>3;
	for(j=0; j< 8; j++)
	{
		pdst = m_buf + ( j*linememsize + m_wpoint*24 ) * this->m_unit;
		for(i=0; i<8; i++)
		{
			pos = ( (j<<3) + i ) * this->m_unit;
			m_yuv[0] = m_basecvt.getvalue(buf+pos, m_unit);
			m_yuv[1] = m_basecvt.getvalue(buf+pos+(m_unit << 6), m_unit);
			m_yuv[2] = m_basecvt.getvalue(buf+pos+(m_unit << 7), m_unit);
			m_basecvt.setvalue(pdst, m_yuv[0], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[1], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[2], m_unit);
			pdst += m_unit;
		}
	}
}

void CBlk2YuvBuf::WriteBlk422Seg(char *buf)
{
	char* pdst;
	int i, j, pos, linememsize;

	linememsize = m_memsize>>3;
	for(j=0; j< 8; j++)
	{
		pdst = m_buf + ( j*linememsize + (m_wpoint<<5) ) * this->m_unit;
		for(i=0; i<8; i++)
		{
			pos = ( (j<<3) + ((i>>2)<<6) + ((i&0x3)<<1) ) * this->m_unit;
			m_yuv[0] = m_basecvt.getvalue(buf+pos, m_unit);
			m_yuv[3] = m_basecvt.getvalue(buf+pos+m_unit, m_unit);
			pos = ( 128 + (j<<3) + i ) * this->m_unit;
			m_yuv[1] = m_basecvt.getvalue(buf+pos, m_unit);
			m_yuv[2] = m_basecvt.getvalue(buf+pos+(m_unit << 6), m_unit);
			m_basecvt.setvalue(pdst, m_yuv[G_ODRI(0)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(1)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(2)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(3)], m_unit);
			pdst += m_unit;
		}
	}
}

void CBlk2YuvBuf::WriteBlk420Seg(char *buf)
{
	char* pdst;
	int i, j, pos, linememsize;

	linememsize = m_memsize>>3;
	for(j=0; j< 16; j++)
	{
		if(j&0x1)
		{
			pdst = m_buf + ( (j>>1)*linememsize + (linememsize<<1)/3 + (m_wpoint<<4) ) * this->m_unit;
			for(i=0; i<8; i++)
			{
				pos = ( ((j>>3)<<7) + ((i>>2)<<6) + (((j&0x7)>>1)<<4) + 8 + ((i&0x3)<<1) ) * this->m_unit;
				m_yuv[6] = m_basecvt.getvalue(buf+pos, m_unit);
				m_yuv[9] = m_basecvt.getvalue(buf+pos+m_unit, m_unit);
				m_basecvt.setvalue(pdst, m_yuv[6], m_unit);
				m_basecvt.setvalue(pdst+=m_unit, m_yuv[9], m_unit);
				pdst += m_unit;
			}
		}
		else
		{
			pdst = m_buf + ( (j>>1)*linememsize + (m_wpoint<<5) ) * this->m_unit;
			for(i=0; i<8; i++)
			{
				pos = ( ((j>>3)<<7) + ((i>>2)<<6) + (((j&0x7)>>1)<<4) + ((i&0x3)<<1) ) * this->m_unit;
				m_yuv[0] = m_basecvt.getvalue(buf+pos, m_unit);
				m_yuv[3] = m_basecvt.getvalue(buf+pos+m_unit, m_unit);
				pos = ( 256 + ((j>>1)<<3) + i ) * this->m_unit;
				m_yuv[1] = m_basecvt.getvalue(buf+pos, m_unit);
				m_yuv[2] = m_basecvt.getvalue(buf+pos+(m_unit << 6), m_unit);
				m_basecvt.setvalue(pdst, m_yuv[G_ODRI(0)], m_unit);
				m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(1)], m_unit);
				m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(2)], m_unit);
				m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(3)], m_unit);
				pdst += m_unit;
			}
		}
	}
}

void CBlk2YuvBuf::WriteBlk411Seg(char *buf)
{
	char* pdst;
	int i, j, pos, linememsize;

	linememsize = m_memsize>>3;
	for(j=0; j< 8; j++)
	{
		pdst = m_buf + ( j*linememsize + m_wpoint*48 ) * this->m_unit;
		for(i=0; i<8; i++)
		{
			pos = ( (j<<3) + ((i>>1)<<6) + ((i&0x1)<<2) ) * this->m_unit;
			m_yuv[0] = m_basecvt.getvalue(buf+pos, m_unit);
			m_yuv[3] = m_basecvt.getvalue(buf+pos+m_unit, m_unit);
			m_yuv[6] = m_basecvt.getvalue(buf+pos+(m_unit<<1), m_unit);
			m_yuv[9] = m_basecvt.getvalue(buf+pos+(m_unit*3), m_unit);
			pos = ( 256 + (j<<3) + i ) * this->m_unit;
			m_yuv[1] = m_basecvt.getvalue(buf+pos, m_unit);
			m_yuv[2] = m_basecvt.getvalue(buf+pos+(m_unit << 6), m_unit);
			m_basecvt.setvalue(pdst, m_yuv[G_ODRI(0)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(1)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(2)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(3)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(4)], m_unit);
			m_basecvt.setvalue(pdst+=m_unit, m_yuv[G_ODRI(5)], m_unit);
			pdst += m_unit;
		}
	}
}

void CBlk2YuvBuf::WriteBlk400Seg(char *buf)
{
	char* pdst;
	int i, j, pos, linememsize;

	linememsize = m_memsize>>3;
	for(j=0; j< 8; j++)
	{
		pdst = m_buf + ( j*linememsize + (m_wpoint<<3) ) * this->m_unit;
		for(i=0; i<8; i++)
		{
			pos = ( (j<<3) + i ) * this->m_unit;
			m_yuv[0] = m_basecvt.getvalue(buf+pos, m_unit);
			m_basecvt.setvalue(pdst, m_yuv[0], m_unit);
			pdst += m_unit;
		}
	}
}

