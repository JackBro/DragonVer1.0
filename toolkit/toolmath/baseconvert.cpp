

#include "internal.h"


int CBaseConvert::GetMaxDivisor(int a, int b)
{
	while(a && b)
	{
		if(a > b)
			a %= b;
		else
			b %= a;
	}
	if(a)
		return a;
	return b;
}

int CBaseConvert::GetMinMultiple(int a, int b)
{
	int c;

	c = this->GetMaxDivisor(a, b);
	a /= c;
	b /= c;
	return a * b * c;
}

int CBaseConvert:: CheckResizeParm(int src, int dst, int factor)
{
	src *= factor;
	src >>= 10;
	if(src > dst)
		return 1;
	return 0;
}

void CBaseConvert::setvalue(char *dst, int val, int unit)
{
	switch(unit)
	{
	case 1:
		dst[0] = (char)val;
		break;
	case 2:
		dst[0] = (char)val;
		dst[1] = (char)(val >> 8);
		break;
	case 3:
		dst[0] = (char)val;
		dst[1] = (char)(val >> 8);
		dst[2] = (char)(val >> 16);
		break;
	case 4:
		dst[0] = (char)val;
		dst[1] = (char)(val >> 8);
		dst[2] = (char)(val >> 16);
		dst[3] = (char)(val >> 24);
		break;
	default:
		break;
	}
}

void CBaseConvert::setvalue(char *dst, int *val, int count, int unit)
{
	int i;

	for(i = 0; i < count; i++, dst += unit)
		this->setvalue(dst, val[i], unit);
}

int CBaseConvert::getvalue(char *src, int unit)
{
	int val = 0;

	switch(unit)
	{
	case 1:
		val = (int)src[0] & 0xff;
		break;
	case 2:
		val = ((int)src[0] & 0xff) + (((int)src[1] & 0xff) << 8);
		break;
	case 3:
		val = ((int)src[0] & 0xff) + (((int)src[1] & 0xff) << 8) + (((int)src[2] & 0xff) << 16);
		break;
	case 4:
		val = ((int)src[0] & 0xff) + (((int)src[1] & 0xff) << 8) + 
			(((int)src[2] & 0xff) << 16) + ((int)src[3] << 24);
		break;
	default:
		break;
	}
	return val;
}

void CBaseConvert::getvalue(char *src, int *val, int count, int unit)
{
	int i;

	for(i = 0; i < count; i++, src += unit)
		val[i] = this->getvalue(src, unit);
}

void CBaseConvert::prefilter(int *dat, int cycle, int *filter, int count)
{
	int i, k, start, end, offset;

	offset = count * cycle;
	for(i = 0; i < cycle; i++)
		dat[offset + i] *= filter[0];

	for(k = 0, start = 0, end = (count << 1) * cycle; k < count; k++, start += cycle, end -= cycle)
	{
		for(i = 0; i < cycle; i++)
		{
			dat[start + i] *= filter[count-k];
			dat[end + i] *= filter[count-k];
			dat[offset + i] += dat[start + i] + dat[end + i];
		}
	}
}

//=============	prefilter process ==============================
int CBaseConvert::HorExtent(char *src, char *dest, int width, int height, int cycle, int count, int unit)
{
	int i, j, size, step, pos;
	int buf[MAXCYCLECOUNT];
	char *psrc, *pdst;

	size = width * height * unit * cycle;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	psrc = src;
	pdst = dest;
	count --;
	size = width * unit * cycle;
	step = unit * cycle;
	for(i = 0; i < height; i++, psrc += size)
	{
		for(j = 0, pos = count * step; j < count; j++, pdst += step, pos -= step)
		{
			this->getvalue(psrc+pos, buf, cycle, unit);
			this->setvalue(pdst, buf, cycle, unit);
		}
		memcpy(pdst, psrc, size);
		pdst += size;
		for(j = 0, pos = (width - 2) * step; j < count; j++, pdst += step, pos -= step)
		{
			this->getvalue(psrc+pos, buf, cycle, unit);
			this->setvalue(pdst, buf, cycle, unit);
		}
	}
	return (width + (count << 1)) * height * unit * cycle;
}

int CBaseConvert::VerExtent(char *src, char *dest, int width, int height, int cycle, int count, int unit)
{
	int i, size, step;
	char *psrc, *pdst;

	size = width * height * unit * cycle;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	count--;
	step = width * unit * cycle;
	memcpy(dest + count * step, src, size);
	psrc = src + (step * count);
	pdst = dest;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	psrc = src + size - (step << 1);
	pdst += size;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	return width * (height + (count << 1)) * unit * cycle;
}

int CBaseConvert::Addedge(char *src, char *dest, int width, int height, int cycle, int count, int unit)
{
	this->HorExtent(src, dest, width, height, cycle, count, unit);
	width += (count - 1) << 1;
	this->VerExtent(dest, src, width, height, cycle, count, unit);
	height += (count - 1) << 1;
	width *= height * unit * cycle;
	memcpy(dest, src, width);
	return width;
}

int CBaseConvert::HorPrefilter(char *src, char *dest, int width, int height, int cycle, 
								int count, int *filter, int unit)
{
	int i, j, k, size, total, m, n, step, offset;
	char *psrc, *pdst, *psrc1;
	CBufferBase pool;
	int *pbuf, *buf;

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	buf = (int *)pool.m_buf;
	size = width * height * unit * cycle;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	total = filter[0];
	for(i = 1; i < count; i++)
		total += filter[i] << 1;
	count--;
	psrc = src;
	pdst = dest;
	size = (count * unit * cycle) << 1;
	m = width - count;
	n = (count << 1) + 1;
	offset = count * cycle;
	step = unit * cycle;
	for(i = 0; i < height; i++)
	{
		for(j = count; j < m; j++, pdst += step, psrc += step)
		{
			for(k = 0, psrc1 = psrc, pbuf = buf; k < n; k++, psrc1 += step, pbuf += cycle)
				this->getvalue(psrc1, pbuf, cycle, unit);
			this->prefilter(buf, cycle, filter, count);
			for(k = 0; k < cycle; k++)
				buf[offset + k] /= total;
			this->setvalue(pdst, buf + offset, cycle, unit);
		}
		psrc += size;
	}
	width -= count << 1;
	return width * height * unit * cycle;
}

int CBaseConvert::VerPrefilter(char *src, char *dest, int width, int height, int cycle, 
								int count, int *filter, int unit)
{
	int i, j, k, size, total, m, n, step, offset;
	char *psrc, *pdst, *pdst1, *psrc1;
	CBufferBase pool;
	int *pbuf, *buf;

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	buf = (int *)pool.m_buf;
	size = width * height * unit * cycle;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	total = filter[0];
	for(i = 1; i < count; i++)
		total += filter[i] << 1;
	psrc = src;
	pdst = dest;
	size = width * unit * cycle;
	count--;
	m = height - count;
	n = (count << 1) + 1;
	step = unit * cycle;
	offset = count * cycle;
	for(i = 0; i < width; i++, psrc += step, pdst += step)
	{
		for(j = count, pdst1 = pdst, psrc1 = psrc; j < m; j++, pdst1 += size, psrc1 += size)
		{
			for(k = 0, total = 0, pbuf = buf; k < n; k++, total += size, pbuf += cycle)
				this->getvalue(psrc1 + total, pbuf, cycle, unit);
			this->prefilter(buf, cycle, filter, count);
			for(k = 0; k < cycle; k++)
				buf[offset+k] /= total;
			this->setvalue(pdst1, buf + k, cycle, unit);
		}
	}
	height -= count << 1;
	return width * height * unit * cycle;
}

int CBaseConvert::Prefilter(char *src, char *dest, int width, int height, int cycle, 
					int count, int *filter, int unit)
{
	this->Addedge(src, dest, width, height, cycle, count, unit);
	this->HorPrefilter(src, dest, width + ((count - 1) << 1), height + ((count - 1) << 1), 
			cycle, count, filter, unit);
	this->VerPrefilter(dest, src, width, height + ((count - 1) << 1), cycle, count, filter, unit);
	width *= height * unit * cycle;
	memcpy(src, dest, width);
	return width;
}

int	CBaseConvert::Calculate(int val, int opval, int opcode)
{
	switch(opcode)
	{
	case ADD_OPCODE:
		val += opval;
		break;
	case SUB_OPCODE:
		val -= opval;
		break;
	case MUL_OPCODE:
		val *= opval;
		break;
	case DIV_OPCODE:
		if(opval)
			val /= opval;
		break;
	case DIVBY_OPCODE:
		if(val)
			val = opval / val;
		break;
	case MOD_OPCODE:
		if(opval)
			val %= opval;
		break;

	case AND_OPCODE:
		val &= opval;
		break;
	case OR_OPCODE:
		val |= opval;
		break;
	case XOR_OPCODE:
		val ^= opval;
		break;
	case NOT_OPCODE:
		val = ~val;
		break;

	case EQUAL_OPCODE:
		val = opval;
		break;

	default:
		break;
	}
	return val;
}

int	CBaseConvert::DataCompare(char *src, char *dst, int len)
{
	int i;

	for(i = 0; i < len; i++)
	{
		if(src[i] != dst[i])
			return 1;
	}
	return 0;
}

void CBaseConvert::DataCalSpan(char *buf, int len, int adr, int span, int size, 
							 int opcode, int opval, int unit)
{
	if(size > span)
		size = span;
	if(span == size)
	{
		this->DataCal(buf, len, adr, opcode, opval, unit);
		return;
	}
	while(len >= size)
	{
		this->DataCal(buf, size, adr, opcode, opval, unit);
		len -= size;
		adr += span;
	}
}

void CBaseConvert::DataCal(char *buf, int len, int adr, int opcode, int opval, int unit)
{
	int i, val;

	for(i = 0; i < len; i += unit)
	{
		val = this->getvalue(buf + adr + i, unit);
		val = this->Calculate(val, opval, opcode);
		this->setvalue(buf + adr + i, val, unit);
	}
}

void CBaseConvert::GetRotateParm(int width, int height, int rotate, int *parm, int unit)
{
	parm[0] = 0;
	parm[1] = width * unit;
	parm[2] = 0;
	parm[3] = unit;

	if(rotate % 2)		//90
	{
		parm[0] = 0;
		parm[1] = unit;
		parm[2] = 0;
		parm[3] = height * unit;
	}
	rotate >>= 1;
	if(rotate % 2)		//H mirror
	{
		parm[0] = (height - 1) * parm[1];
		parm[1] *= -1;
	}

	rotate >>= 1;
	if(rotate % 2)		//V mirror
	{
		parm[2] = (width - 1) * parm[3];
		parm[3] *= -1;
	}
}

void CBaseConvert::MRotate(char *src, char *dest, int width, int height, int rotate, int unit)
{
	int i, j, parm[4], pos1, pos2;
	int k, val;

	this->GetRotateParm(width, height, rotate, parm, unit);
	for(i = 0, pos1 = parm[0]; i < height; i++, pos1 += parm[1])
	{
		for(j = 0, pos2 = parm[2]; j < width; j++, pos2 += parm[3])
		{
			for(k = 0; k < unit; k++, src++)
			{
				val = this->getvalue(src);
				this->setvalue(dest + pos1 + pos2 + k, val);
			}
		}
	}
}

int CBaseConvert::GetImgSize(int width, int height, int fmt)
{
	this->AdjustImgparm(&width, &height, fmt);

	switch(fmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RGB555:
	case RGB565:
	case YUV422M2BLK:
	case YUV422BLK:
	case YUV422LBUF:
	case YUV422FRM:
	case MPEG2IFRM422:
		width *= height << 1;
		break;

	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
	case RAW_RGB:
	case RAW_RBG:
	case RAW_GBR:
	case RAW_GRB:
	case RAW_BGR:
	case RAW_BRG:
	case YUV400:
	case RGB8:
	case RAW_R:
	case RAW_G:
	case RAW_B:
	case RAW_Y:
	case RAW_U444:
	case RAW_V444:
	case EDGE_MAP:
	case RAW_YIQ_Y:
	case RAW_YIQ_I:
	case RAW_YIQ_Q:
	case RAW_HSI_I:
	case RAW_HSI_H:
	case RAW_HSI_S:
	case YUV400BLK:
	case YUV400FRM:
	case YUV400LBUF:
		width *= height;
		break;

	case YUV444BLK:
	case YUV444M2BLK:
	case YUV444LBUF:
	case YUV444FRM:
	case MPEG2IFRM444:
	case YUV444:
	case RGB24:
	case YIQ:
	case HSI:
		width *= height * 3;
		break;

	case RGB1:
		width *= height;
		width >>= 3;
		break;

	case RGB2:
		width *= height;
		width >>= 2;
		break;

	case RGB4:
		width *= height;
		width >>= 1;
		break;

	case RGB444:
		width *= height * 3;
		width >>= 1;
		break;

	case RGB666:
		width *= height * 18;
		width >>= 3;
		break;

	case RGB32:
		width *= height;
		width <<= 2;
		break;

	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420BLK:
	case YUV420LBUF:
	case YUV420FRM:
	case MPEG1I_FRM:
	case MPEG2IFRM420:
	case MPEG4_FRM:
	case YUV411BLK:
	case YUV411LBUF:
	case YUV411FRM:
		width *= height * 3;
		width >>= 1;
		break;
	default:
		width = 0;
		break;
	}
	return width;
}

int CBaseConvert::AdjustImgparm(int *width, int *height, int fmt)
{
	int cx = 0, cy = 0;

	switch(fmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
	//case JPEG422:
		cx = ((*width + 1) >> 1) << 1;
		cy = *height;
		break;

	case RAW_RGB:
	case RAW_RBG:
	case RAW_GBR:
	case RAW_GRB:
	case RAW_BGR:
	case RAW_BRG:
	case YUV400:
	case YUV444:
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB555:
	case RGB565:
	case RGB666:
	case RGB24:
	case RGB32:
	case BMP:
	//case JPEG400:
	//case JPEG444:
	case RAW_R:
	case RAW_G:
	case RAW_B:
	case RAW_Y:
	case RAW_U444:
	case RAW_V444:
	case EDGE_MAP:
	case YIQ:
	case HSI:
	case RAW_YIQ_Y:
	case RAW_YIQ_I:
	case RAW_YIQ_Q:
	case RAW_HSI_I:
	case RAW_HSI_H:
	case RAW_HSI_S:
	case GIF:
	case PNG:
	case PCX:
		cx = *width;
		cy = *height;
		break;

	case CCIR601_656NTSC:
		cx = NTSC_WIDTH;
		cy = NTSC_HEIGHT;
		break;
	case CCIR601_656PAL:
		cx = PAL_WIDTH;
		cy = PAL_HEIGHT;
		break;

	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	//case JPEG420:
		cx = ((*width + 1) >> 1) << 1;
		cy = ((*height + 1) >> 1) << 1;
		break;

	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	//case JPEG411:
		cx = ((*width + 3) >> 2) << 2;
		cy = *height;
		break;

	case YUV422M2BLK:
	case YUV422BLK:
	case JPEG422:		
	case YUV422LBUF:
	case YUV422FRM:
	case MPEG2IFRM422:
		cx = ((*width + 15) >> 4) << 4; //*width; ----this is effective value
		cy = ((*height + 7) >> 3) << 3; //*height;----effective value
		break;

	case YUV420BLK:
	case JPEG420:		
	case YUV420LBUF:
	case YUV420FRM:
	case MPEG1I_FRM:
	case MPEG2IFRM420:
	case MPEG4_FRM:
		cx = ((*width + 15) >> 4) << 4;
		cy = ((*height + 15) >> 4) << 4;
		break;

	case YUV411BLK:
	case JPEG411:		
	case YUV411LBUF:
	case YUV411FRM:
		cx = ((*width + 31) >> 5) << 5;
		cy = ((*height + 7) >> 3) << 3;
		break;

	case YUV400BLK:
	case YUV444BLK:
	case JPEG400:
	case JPEG444:
	case YUV444M2BLK:
	case YUV400LBUF:
	case YUV444LBUF:
	case YUV400FRM:
	case YUV444FRM:
	case MPEG2IFRM444:
		cx = ((*width + 7) >> 3) << 3;
		cy = ((*height + 7) >> 3) << 3;
		break;
	default:
		break;
	}
	if( (*width == cx) && (*height == cy) )
		return 0;
	*width = cx;
	*height = cy;
	return 1;
}

