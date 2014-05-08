

#include "internal.h"

/*===============================================================
class TOOLBUF_DLL CFrameBaseBuf

=================================================================*/

CFrameBaseBuf::CFrameBaseBuf(int width, int height)
{
	this->clear();
	this->SetFrameSize(width, height);
}

CFrameBaseBuf::~CFrameBaseBuf()
{
	
}

void CFrameBaseBuf::clear(void)
{
	
	this->m_width = 0;
	this->m_height = 0;
	this->m_flag = 0;
}
/*
void CFrameBaseBuf::release(void)
{
	if(this->m_buf)
		delete [](this->m_buf);
	this->m_buf = NULL;
	this->m_width = 0;
	this->m_height = 0;
	this->m_flag = 0;
	
}
*/
/*=================================================
 flag: video format flag
       = 0:  4:4:4,  = 1:  4:2:2,  = 2:  4:2:0  
==================================================*/
/*
void CFrameBaseBuf::SetFrameSize(int width, int height, int flag)
{
	int len;

	len = width * height >> flag;
	//len = (flag == 0) ? len : ((flag == 1) ? (len >> 1) : (len >> 2));
	if (len <= 0)
		return;
	CBufferBase::SetSize(len, 1);
	this->m_width = width;
	this->m_height = height;
	this->m_flag = flag;

}
*/
void CFrameBaseBuf::SetFrameSize(int width, int height)
{
	int len;

	len = width * height;
	if (len <= 0)
		return;
	CBufferBase::SetSize(len, 1);
	this->m_width = width;
	this->m_height = height;
}

void CFrameBaseBuf::ChangeBufPoint(int offset)
{
	if (this->m_buf != NULL)
        this->m_buf += offset;
}
/*
int CFrameBaseBuf::Write(char *buf, int x_pos, int y_pos)
{
	int i, j, offset;
	char *dstbuf;
	int stride = this->m_width;

	if ( ( x_pos > (this->m_width - 8)) || ( y_pos > (this->m_height - 8)) || (buf == NULL) )
		return 0;

	if (this->m_flag == 0)
	{
		offset = x_pos + y_pos * stride;		
	}
	else if (this->m_flag == 1)
	{
		stride >>= 1;
		offset = (x_pos >> 1) + y_pos * stride;
	}
	else
	{
		stride >>= 1;
		offset = (x_pos >> 1) + (y_pos >> 1) * stride;
	}
	dstbuf = this->m_buf + offset;
	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 8; i++)
		{
			dstbuf[i] = buf[j * 8 + i];
		}
		dstbuf += stride;
	}
	return 64;
}
*/
int CFrameBaseBuf::Write(char *buf, int x_pos, int y_pos)
{
	int i, j;
	char *dstbuf;

	if ( (x_pos < 0) || ( x_pos > (this->m_width - 8)) || (y_pos < 0) || ( y_pos > (this->m_height - 8)) || (buf == NULL) )
		return 0;

	dstbuf = this->m_buf + x_pos + y_pos * this->m_width;
	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 8; i++)
		{
			dstbuf[i] = buf[j * 8 + i];
		}
		dstbuf += this->m_width;
	}
	return 64;
}


int CFrameBaseBuf::Read(char *buf, int x_pos, int y_pos, int xflag, int yflag)
{
	int i, j;
	char *srcbuf;

	if ( (x_pos < 0) || ( x_pos > (this->m_width - 8)) || (y_pos < 0) || ( y_pos > (this->m_height - 8)) || (buf == NULL) )
		return 0;

	srcbuf = this->m_buf + x_pos + y_pos * this->m_width;
	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 8; i++)
		{
			if (!xflag && !yflag)				//neither horizontal nor vertical half-pel
				buf[j * 8 + i] = srcbuf[i];
			else if (!xflag && yflag)			//no horizontal but vertical half-pel
				buf[j * 8 + i] = ( (srcbuf[i] & 0xff) + (srcbuf[i + this->m_width] & 0xff) + 1 ) >> 1;
			else if (xflag && !yflag)			//no vertical but horizontal half-pel
				buf[j * 8 + i] = ( (srcbuf[i] & 0xff) + (srcbuf[i + 1] & 0xff) + 1 ) >> 1;
			else								//vertical and horizontal half-pel
				buf[j * 8 + i] = ( (srcbuf[i] & 0xff) + (srcbuf[i + 1] & 0xff) +
					(srcbuf[i + this->m_width] & 0xff) + (srcbuf[i + this->m_width + 1] & 0xff)+ 2 ) >> 2;
		}
		srcbuf += this->m_width;
	}
	return 64;
}

int CFrameBaseBuf::Readfield(char *buf, int x_pos, int y_pos, int xflag, int yflag, int field)
{
	int i, j, stride;
	char *srcbuf;

	if ( (x_pos < 0) || ( x_pos > (this->m_width - 8)) || (y_pos < 0) || ( y_pos > (this->m_height - 8)) || (buf == NULL) )
		return 0;

	stride = this->m_width << 1;
	srcbuf = this->m_buf + (field ? this->m_width : 0) + x_pos + y_pos * stride;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 8; i++)
		{
			if (!xflag && !yflag)
				buf[j * 16 + i] = srcbuf[i];
			else if (!xflag && yflag)
				buf[j * 16 + i] = ( (srcbuf[i] & 0xff) + (srcbuf[i + stride] & 0xff) + 1 ) >> 1;
			else if (xflag && !yflag)
				buf[j * 16 + i] = ( (srcbuf[i] & 0xff) + (srcbuf[i + 1] & 0xff) + 1 ) >> 1;
			else
				buf[j * 16 + i] = ( (srcbuf[i] & 0xff) + (srcbuf[i + 1] & 0xff) + 
					(srcbuf[i + stride] & 0xff) + (srcbuf[i + stride + 1] & 0xff) + 2 ) >> 2;
		}
		srcbuf += stride;
	}
	return 32;
}

/*
int CFrameBaseBuf::Read(char *buf, int x_pos, int y_pos, int xflag, int yflag)
{
	int i, j, offset;
	char *srcbuf;
	int stride = this->m_width;

	if (buf == NULL)
		return 0;
	x_pos = (x_pos > (this->m_width - 8)) ? (this->m_width - 8) : ((x_pos < 0) ? 0 : x_pos);
	y_pos = (y_pos > (this->m_height - 8)) ? (this->m_height - 8) : ((y_pos < 0) ? 0 : y_pos);
	if ( (x_pos < 0) || ( x_pos > (this->m_width - 8)) || (y_pos < 0) || ( y_pos > (this->m_height - 8)) || (buf == NULL) )
		return 0;
	
	if (this->m_flag == 0)
	{
		offset = x_pos + y_pos * stride;		
	}
	else if (this->m_flag == 1)
	{
		stride >>= 1;
		offset = (x_pos >> 1) + y_pos * stride;
	}
	else
	{
		stride >>= 1;
		offset = (x_pos >> 1) + (y_pos >> 1) * stride;
	}
	srcbuf = this->m_buf + offset;
	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 8; i++)
		{
			if (!xflag && !yflag)				//neither horizontal nor vertical half-pel
                buf[j * 8 + i] = srcbuf[i];
			else if (!xflag && yflag)			//no horizontal but vertical half-pel
				buf[j * 8 + i] = (srcbuf[i] + srcbuf[i + stride] + 1) >> 1;
			else if (xflag && !yflag)			//no vertical but horizontal half-pel
				buf[j * 8 + i] = (srcbuf[i] + srcbuf[i + 1] + 1) >> 1;
			else								//vertical and horizontal half-pel
				buf[j * 8 + i] = (srcbuf[i] + srcbuf[i + 1] + srcbuf[i + stride] + srcbuf[i + stride + 1] + 2) >> 2;
		}
		srcbuf += stride;
	}
	return 64;
}

int CFrameBaseBuf::Read(char *buf, int x_pos, int y_pos, int xflag, int yflag, int field0)
{
	int i, j, offset;
	char *srcbuf;
	int stride = this->m_width << 1;

	if (buf == NULL)
		return 0;
	//x_pos = (x_pos > (this->m_width - 8)) ? (this->m_width - 8) : ((x_pos < 0) ? 0 : x_pos);
	//y_pos = (y_pos > (this->m_height - 8)) ? (this->m_height - 8) : ((y_pos < 0) ? 0 : y_pos);
	if ( (x_pos < 0) || ( x_pos > (this->m_width - 8)) || (y_pos < 0) || ( y_pos > (this->m_height - 8)) || (buf == NULL) )
		return 0;

	if (this->m_flag == 0)
	{
		offset = x_pos + y_pos * stride;		
	}
	else if (this->m_flag == 1)
	{
		stride >>= 1;
		offset = (x_pos >> 1) + y_pos * stride;
	}
	else
	{
		stride >>= 1;
		offset = (x_pos >> 1) + (y_pos >> 1) * stride;
	}
	srcbuf = this->m_buf + offset + (field0 ? stride >> 1 : 0);
	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 8; i++)
		{
			if (!xflag && !yflag)
				buf[j * 16 + i] = srcbuf[i];
			else if (!xflag && yflag)
				buf[j * 16 + i] = (srcbuf[i] + srcbuf[i + stride] + 1) >> 1;
			else if (xflag && !yflag)
				buf[j * 16 + i] = (srcbuf[i] + srcbuf[i + 1] + 1) >> 1;
			else
				buf[j * 16 + i] = (srcbuf[i] + srcbuf[i + 1] + srcbuf[i + stride] + srcbuf[i + stride + 1] + 2) >> 2;
		}
		srcbuf += stride;
	}

	return 32;
}
*/
