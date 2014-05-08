

#include "internal.h"

//=======================================================================================
//			class	C702Sizer
//=======================================================================================

C702Sizer::C702Sizer(int width, int height, int pal, int interlace, int average, int compon, int fmt)
{
	this->Init(width, height, pal, interlace, average, compon, fmt);
}

C702Sizer::~C702Sizer()
{
}

void C702Sizer::Init(int width, int height, int pal, int interlace, int average, int compon, int fmt)
{
	int i;

	this->m_width = width;
	this->m_height = height;
	this->m_fmt = fmt;
	this->m_pal = pal;
	this->m_interlace = interlace;
	this->m_average = average;
	this->m_compon = compon;

	for (i = 0; i < 2; i++)
		this->m_coeff5[i] = (int)((float)(1 << VER_COEFF_FMT) * (i + 1) / 5 + 0.5);
	for (i = 0; i < 4; i++)
		this->m_coeff9[i] = (int)((float)(1 << HOR_COEFF_FMT) * (i + 1) / 9 + 0.5);
	for (i = 0; i < 22; i++)
		this->m_coeff45[i] = (int)((float)(1 << HOR_COEFF_FMT) * (i + 1) / 45 + 0.5);

	this->m_linebuf[0].SetSize(this->m_width, 0);
	this->m_linebuf[1].SetSize(this->m_width, 0);
	this->m_linebuf[0].SetFormat(this->m_fmt);
	this->m_linebuf[1].SetFormat(this->m_fmt);
	this->m_plinebuf[0] = &(this->m_linebuf[0]);
	this->m_plinebuf[1] = &(this->m_linebuf[1]);
}

int C702Sizer::Process(char *src, char *dst, char *verdst)
{
	int srcw, srch, dstw, dsth;
	int i, len, tmp0;

	srcw = this->m_width;
	srch = this->m_height;	
	dstw = (this->m_pal == 1) ? PAL_WIDTH : NTSC_WIDTH;
	dsth = (this->m_pal == 1) ? (this->m_height == 480 ? PAL_HEIGHT_EXT : PAL_HEIGHT) : NTSC_HEIGHT;

	if (srch == dsth)
		memcpy(dst, src, srcw * dsth << 1);
	else
		len = this->VerSizer(src, dst, srcw, srch, dsth);	

	if (this->m_interlace != 0)
	{
		len = this->IntelaceProc(dst, verdst, srcw, dsth);
		dsth >>= 1;
	}	
	else
		this->ProgProc(dst, verdst, srcw, dsth);

	len = this->HorSizer(verdst, dst, srcw, dsth, dstw);	

	if (this->m_interlace && (this->m_compon == 0))
	{
		for (i = 0; i < len; i += 6)
		{
			tmp0 = ((dst[i + 1] & 0xff) + (dst[i + 4] & 0xff)) >> 1;
			dst[i + 1] = (char)tmp0;
			dst[i + 4] = (char)tmp0;
			tmp0 = ((dst[i + 2] & 0xff) + (dst[i + 5] & 0xff)) >> 1;
			dst[i + 2] = (char)tmp0;
			dst[i + 5] = (char)tmp0;			
		}
	}

	return len;
}

int C702Sizer::ProgProc(char *src, char *dst, int width, int height)
{
	int i, linesize, len, pos;

	linesize = width << 1;
	len = linesize * height;	
	if ((this->m_average) && (!this->m_pal) && (this->m_height == 480))
	{		
		pos = linesize;
		memcpy(dst, src, linesize);		
		while (pos < len)
		{
			for (i = 0; i < linesize; i++)
				dst[pos + i] = ((src[pos + i - linesize] & 0xff) + (src[pos + i] & 0xff) + SIZER_RND) >> 1;
			pos += linesize;
		}
	}
	else
		memcpy(dst, src, len);

	return len;
}

int C702Sizer::IntelaceProc(char *src, char *dst, int width, int height)
{	
	int i, code, size, len, pos = 0, pos1 = 0;

	len = width * height;
	size = width << 1;	
	if ((this->m_pal) && (this->m_width == 352))
		this->m_average = 0;
	code = (this->m_average << 1) + (this->m_interlace - 1);
	switch(code) 
	{
	case 0:				//Odd field output without average
		while(pos < len)
		{
			memcpy(dst + pos, src + (pos << 1), size);
			pos += size;
		}
		break;
	case 1:				//Even field output without average
		while (pos < len)
		{
			memcpy(dst + pos, src + size + (pos << 1), size);
			pos += size;
		}
		break;
	case 2:				//Odd field output with average
		memcpy(dst, src, size);
		pos = size;
		pos1 = size;
		while (pos < len)
		{
			for (i = 0; i < size; i++)
				dst[pos + i] = ((src[pos1 + i] & 0xff) + (src[pos1 + size + i] & 0xff) + SIZER_RND) >> 1;
			pos += size;
			pos1 += size << 1;
		}
		break;
	default:			//Even field output with average
		while (pos < len)
		{
			for (i = 0; i < size; i++)
				dst[pos + i] = ((src[pos1 + i] & 0xff) + (src[pos1 + size + i] & 0xff) + SIZER_RND) >> 1;
			pos += size;
			pos1 += size << 1;
		}
		break;
	}

	return len;
}

int C702Sizer::VerSizer(char *src, char *dst, int srcw, int srch, int dsth)
{	
	int i, size, size1, pos = 0, pos1 = 0;

	size = srcw * srch << 1;
	size1 = srcw * dsth << 1;
	this->GetFactor(srch);
	this->m_plinebuf[0] = &(this->m_linebuf[0]);
	this->m_plinebuf[1] = &(this->m_linebuf[1]);
	pos += this->m_linebuf[0].Write(src + pos, size);
	this->m_linebuf[0].ClearPoint();

	for (i = 0; i < srch; i++)
	{
		pos = (pos == size) ? (pos - (srcw << 1)) : pos;
		pos += this->VerWrite(src + pos, size);

		pos1 += this->VerRead(dst + pos1, size1);

	}
	return pos1;
}

int C702Sizer::VerWrite(char *buf, int len)
{
	int pos;

	pos = this->m_plinebuf[1]->Write(buf, len);
	this->m_plinebuf[1]->ClearPoint();

	return pos;
}

int C702Sizer::VerRead(char *buf, int len)
{		
	int linesize, i, pos = 0;
	int val0, val1, tmp;

	linesize = this->m_width << 1;
	while(this->m_phase < this->m_up)
	{
		if (this->m_phase == 0)
		{
			memcpy(buf + pos, this->m_plinebuf[0]->m_buf, linesize);
		}
		else if ((this->m_phase << 1) < this->m_up)
		{
			for (i = 0; i < linesize; i++)
			{
				val0 = (int)(this->m_plinebuf[0]->m_buf[i] & 0xff);
				val1 = (int)(this->m_plinebuf[1]->m_buf[i] & 0xff);
				tmp = this->GetValue(val1, val0, this->m_pcoeff[this->m_phase - 1], VER_COEFF_FMT);
				buf[pos + i] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}
		}
		else if ((this->m_phase << 1) == this->m_up)
		{
			for (i = 0; i < linesize; i++)
			{
				val0 = (int)(this->m_plinebuf[0]->m_buf[i] & 0xff);
				val1 = (int)(this->m_plinebuf[1]->m_buf[i] & 0xff);
				buf[pos + i] = (char)((val0 + val1 + SIZER_RND) >> 1);
			}
		}
		else
		{
			for (i = 0; i < linesize; i++)
			{
				val0 = (int)(this->m_plinebuf[0]->m_buf[i] & 0xff);
				val1 = (int)(this->m_plinebuf[1]->m_buf[i] & 0xff);
				tmp = this->GetValue(val0, val1, this->m_pcoeff[this->m_up - this->m_phase - 1], VER_COEFF_FMT);
				buf[pos + i] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}

		}
		this->m_phase += this->m_down;
		pos += linesize;
	}
	this->m_phase -= this->m_up;
	this->LoopLinebuf();

	return pos;	
}

void C702Sizer::LoopLinebuf(void)
{
	CLine422Buf *p;

	p = this->m_plinebuf[0];
	this->m_plinebuf[0] = this->m_plinebuf[1];
	this->m_plinebuf[1] = p;
}

int C702Sizer::HorSizer(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, size, size1, pos = 0, pos1 = 0;;

	size = srcw * srch << 1;
	size1 = dstw * srch << 1;
	this->GetFactor(srcw);	
	for (i = 0; i < srch; i++)
	{
		pos += this->Write(src + pos, size);

		pos1 += this->Read(dst + pos1, size1);
	}
	return pos1;
}

int C702Sizer::Write(char *buf, int len)
{
	int pos = 0;

	pos = this->m_linebuf[0].Write(buf, len);
	this->m_linebuf[0].ClearPoint();

	return pos;
}

int C702Sizer::Read(char *buf, int len)
{
	int i, j, dstw;
	int tmp, dat0[3], dat1[3];
	int pos = 0, index = 0;

	dstw = PAL_WIDTH;
	this->m_phase = 0;
	this->m_linebuf[0].Read(pos, dat0);
	pos++;
	this->m_linebuf[0].Read(pos, dat1);
	pos++;
	for (i = 0; i < dstw; i++, index += 3)
	{
		while (this->m_phase >= this->m_up)
		{
			this->m_phase -= this->m_up;			
			dat0[0] = dat1[0];
			dat0[1] = dat1[1];
			dat0[2] = dat1[2];
			this->m_linebuf[0].Read(pos, dat1);
			pos++;
		}
		if (this->m_phase == 0)
		{
			for (j = 0; j < 3; j++)
				buf[index + j] = (char)dat0[j];
		}
		else if ((this->m_phase << 1) < this->m_up)
		{
			for (j = 0; j < 3; j++)
			{
				tmp = this->GetValue(dat1[j], dat0[j], this->m_pcoeff[this->m_phase - 1], HOR_COEFF_FMT);
				buf[index + j] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}
		}
		else if ((this->m_phase << 1) == this->m_up)
		{
			for (j = 0; j < 3; j++)
			{				
				buf[index + j] = (char)((dat0[j] + dat1[j] + SIZER_RND) >> 1);
			}
		}
		else
		{
			for (j = 0; j < 3; j++)
			{
				tmp = this->GetValue(dat0[j], dat1[j], this->m_pcoeff[this->m_up - this->m_phase - 1], HOR_COEFF_FMT);
				buf[index + j] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}
		}
		this->m_phase += this->m_down;
	}
	return index;
}

int C702Sizer::GetFactor(int srcsize)
{	
	this->m_pcoeff = NULL;
	this->m_phase = 0;
	switch(srcsize)
	{
	case CIF_WIDTH:
		this->m_up = 45;
		this->m_down = 22;
		this->m_pcoeff = this->m_coeff45;
		break;

	case VGA_WIDTH:
		this->m_up = 9;
		this->m_down = 8;
		this->m_pcoeff = this->m_coeff9;
		break;

	case CIF_HEIGHT:
		if (this->m_pal == 1)
		{
			this->m_up = 2;
			this->m_down = 1;
		}
		else
		{
			this->m_up = 5;
			this->m_down = 3;
			this->m_pcoeff = this->m_coeff5;
		}		
		break;

	case VGA_HEIGHT:
		if (this->m_pal == 1)
		{
			this->m_up = 5; 
			this->m_down = 4;
			this->m_pcoeff = this->m_coeff5;
		}
		else
		{
			this->m_up = 1;
			this->m_down = 1;
		}
		break;

	default:
		return 0;
	}
	return 1;
}

// ret = val1 + ( ((val0 - val1) * coeff + 1 << (SIZER_COEFF_FMT - 1)) >> SIZER_COEFF_FMT )
int C702Sizer::GetValue(int val0, int val1, int coeff, int precision)
{
	int ret;

	if (val0 > val1)
	{
		ret = val1 + ( ((val0 - val1) * coeff + (SIZER_RND << (precision - 1))) >> precision );
	}
	else
	{
		ret = val1 - ( ((val1 - val0) * coeff + (SIZER_RND << (precision - 1))) >> precision );
	}
	return ret;
}






