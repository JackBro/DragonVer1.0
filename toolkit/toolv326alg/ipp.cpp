
#include "internal.h"

//==================== class CIppModule ===================

C326Ipp::C326Ipp(int width, int height, int ratio, int format)
{
	this->SetSize(width, height);
	this->SetFormat(format);
	this->SetRatio(ratio);
}

C326Ipp::~C326Ipp()
{
}

void C326Ipp::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

void C326Ipp::SetRatio(int ratio)
{
	this->m_ratio = ratio;
}

void C326Ipp::SetFormat(int format)
{
	this->m_format = format;
}

void C326Ipp::SetFilter(int *pfilter)
{
	int num = 0, i = 0;
	num = (this->m_ratio == RATIO_HALF) ? 3 : 5;
	for(i = 0; i < num; i++)
		this->m_filter[i] = pfilter[i];
}

int C326Ipp::Process(char *src, char *dst)
{
	CBufferBase tmpb;
	int size = (this->m_width * this->m_height) << 1, len = 0;
	C326Filter filter(this->m_width, this->m_height, this->m_ratio, this->m_format);
	C326Sizer sizer(this->m_width, this->m_height, this->m_ratio, this->m_format);

	tmpb.SetSize(size);
	filter.SetFilter(this->m_filter);
	if((this->m_ratio == RATIO_HALF) || (this->m_ratio == RATIO_QUARTER))
		len = filter.Process(src, tmpb.m_buf);
	len = sizer.Process(tmpb.m_buf, dst);

	return len;
}


//================= class CFilterModule ===================

C326Filter::C326Filter(int width,int height, int ratio, int format)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_ratio  = ratio;
	this->m_format = format;
	this->m_cnum   = (this->m_ratio == RATIO_HALF) ? 3 : 5;
	this->Init();
}

C326Filter::~C326Filter()
{
}

void C326Filter::SetFilter(int *filter)
{
	int total = 0, i = 0;

	for(i = 0; i < this->m_cnum; i++)
		total += filter[i];
	if(total == 0)
		return;
	for(i = 0; i < this->m_cnum; i++)
		this->m_yfilter[i] = (filter[i] << 7) / total;

	if(this->m_cnum == 3)
	{
		this->m_uvfilter[0] = (this->m_yfilter[0] + this->m_yfilter[1]) >> 1;
		this->m_uvfilter[1] = (this->m_yfilter[0] + this->m_yfilter[1] + this->m_yfilter[2]) >> 1;
		this->m_uvfilter[2] = this->m_yfilter[2] >> 1;
	}
	else
	{
		this->m_uvfilter[0] = (this->m_yfilter[0] + this->m_yfilter[1]) >> 1;
		this->m_uvfilter[1] = (this->m_yfilter[2] + this->m_yfilter[3]) >> 1;
		this->m_uvfilter[2] = (this->m_yfilter[0] + this->m_yfilter[1] + this->m_yfilter[4]) >> 1;
		this->m_uvfilter[3] = (this->m_yfilter[2] + this->m_yfilter[3]) >> 1;
		this->m_uvfilter[4] = this->m_yfilter[4] >> 1;
	}
}

void C326Filter::Init(void)
{
	int i, unit = 1;

	this->m_flg = PREF_NONE;

	this->m_yfilter[0]  = 0x08;
	this->m_yfilter[1]  = 0x20;
	this->m_yfilter[2]  = 0x30;
	this->m_yfilter[3]  = 0x20;
	this->m_yfilter[4]  = 0x08;
	this->m_uvfilter[0] = 0x14;
	this->m_uvfilter[1] = 0x28;
	this->m_uvfilter[2] = 0x18;
	this->m_uvfilter[3] = 0x28;
	this->m_uvfilter[4] = 0x04;
	//this->m_yfilter[0]  = 0x20;
	//this->m_yfilter[1]  = 0x40;
	//this->m_yfilter[2]  = 0x20;
	//this->m_yfilter[3]  = 0x00;
	//this->m_yfilter[4]  = 0x00;
	//this->m_uvfilter[0] = 0x30;
	//this->m_uvfilter[1] = 0x40;
	//this->m_uvfilter[2] = 0x10;
	//this->m_uvfilter[3] = 0x00;
	//this->m_uvfilter[4] = 0x00;

	for(i = 0; i < 5; i++)
	{
		this->m_linebuf[i].SetFormat(this->m_format);
		this->m_linebuf[i].SetSize(this->m_width, 0, unit);
	}
	this->Clear();
}

void C326Filter::Clear()
{
	int i;

	this->m_line = 0;
	this->m_pos = 0;

	for(i = 0; i < 5; i++)
	{
		this->m_plinebuf[i] = &(this->m_linebuf[i]);
		this->m_plinebuf[i]->ClearPoint();
	}
	for(i = 0; i < 5; i++)
		this->m_pblock[i] = this->m_block[i];
}

void C326Filter::LoopLinebuf(void)
{
	int i;
	CLine422Buf *p = this->m_plinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	this->m_plinebuf[0] = this->m_plinebuf[this->m_cnum - 1];
	this->m_plinebuf[this->m_cnum - 1] = p;
	for(i = 1; i < this->m_cnum; i ++)
		this->m_plinebuf[i]->ClearPoint();
}

void C326Filter::GetBlock(void)
{
	int i, j, adr, val;

	val = (this->m_line < this->m_cnum) ? 1 : 0;
	if(this->m_pos > 0)
	{
		for(i = val; i < this->m_cnum; i++)
			this->m_plinebuf[i]->Read(this->m_pos - 1, this->m_pblock[i]);
	}

	if(this->m_flg == PREF_Y)
	{
		for(i = 0, adr = 3; i < this->m_cnum - 1; i++, adr += 3)
		{
			for(j = val; j < this->m_cnum; j++)
				this->m_plinebuf[j]->Read(this->m_pos + i, this->m_pblock[j] + adr);
		}
		return;
	}
	for(i = 0, adr = 3; i < this->m_cnum - 1; i++, adr += 3)
	{
		for(j = val; j < this->m_cnum; j++)
			this->m_plinebuf[j]->Read(this->m_pos + (i << 1), this->m_pblock[j] + adr);
	}
}

int C326Filter::Write(char *buf, int len)
{
	int pos = 0, pos1 = 0, i = 0;

	for(i = 1; i < this->m_cnum; i ++)
	{
		pos1 = this->m_plinebuf[i]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		this->m_line++;
		if(len == 0)
			break;
	}
	return pos;
}

int C326Filter::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_plinebuf[this->m_cnum - 1]->IsFull() == 0) )
		return 0;
	if(this->m_flg == PREF_Y)
		len = this->YProc(buf, len);
	else
		len = this->UvProc(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len;
}

int C326Filter::YProc(char *buf, int len)
{
	int pos = 0, max = 255, i, j, total = 0, adr, val;
	CBaseConvert tools;

	len >>= 1;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	for(; this->m_pos < len; this->m_pos += this->m_cnum - 1)
	{
		total = 0;
		this->GetBlock();
		i = (this->m_line < this->m_cnum) ? 1 : 0;
		for(; i < this->m_cnum; i++)
		{
			val = 0;
			j   = 0;
			adr = 0;
			if(this->m_pos < 3*(this->m_cnum - 1))
			{
				val += (this->m_yfilter[0] + this->m_yfilter[1]) * this->m_pblock[i][3];
				j = 2;
				adr = 6;
			}
			for(; j < this->m_cnum; j++, adr += 3)
				val += this->m_yfilter[j] * this->m_pblock[i][adr];
			val >>= 5;
			if((this->m_line < this->m_cnum) && (i == 1))
			{
				total += ((val * (this->m_yfilter[0] + this->m_yfilter[1])) >> 7);
				continue;
			}
			total += ((val * this->m_yfilter[i]) >> 7);
		}
		total >>= 2;
		total = (total < 0) ? 0 : ((total > max) ? max : total);
		tools.setvalue(buf+pos, total);
		pos += ((this->m_cnum - 1) << 1);
	}
	return pos;
}

int C326Filter::UvProc(char *buf, int len)
{
	int pos = 0, max = 255, i, j, adr;
	int uvval[2] = {0, 0}, val[2] = {0, 0};
	CBaseConvert tools;

	len >>= 1;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	for(; this->m_pos < len; this->m_pos += ((this->m_cnum - 1) << 1))
	{
		uvval[0] = 0;
		uvval[1] = 0;
		this->GetBlock();
		i = (this->m_line < this->m_cnum) ? 1 : 0;
		for(; i < this->m_cnum; i++)
		{
			val[0] = 0;
			val[1] = 0;
			j   = 0;
			adr = 0;
			if(this->m_pos < ((this->m_cnum - 1) << 2))
			{
				val[0] += (this->m_uvfilter[0] + this->m_uvfilter[1]) * this->m_pblock[i][4];
				if(this->m_pos == 0)
					val[1] += (this->m_uvfilter[0] + this->m_uvfilter[1]) * this->m_pblock[i][5];
				else
				{
					val[1] += this->m_uvfilter[0] * this->m_pblock[i][2];
					val[1] += this->m_uvfilter[1] * this->m_pblock[i][5];
				}
				j = 2;
				adr = 6;
			}
			for(; j < this->m_cnum; j++, adr += 3)
			{
				val[0] += this->m_uvfilter[j] * this->m_pblock[i][adr+1];
				val[1] += this->m_uvfilter[j] * this->m_pblock[i][adr+2];
			}
			if((this->m_line < this->m_cnum) && (i == 1))
			{
				for(j = 0; j < 2; j++)
				{
					val[j] >>= 5;
					uvval[j] += ((val[j] * (this->m_yfilter[0] + this->m_yfilter[1])) >> 7);
				}
				continue;
			}
			for(j = 0; j < 2; j++)
			{
				val[j] >>= 5;
				uvval[j] += ((this->m_yfilter[i] * val[j]) >> 7);
			}
		}
		for(i = 0; i < 2; i++)
		{
			uvval[i] >>= 2;
			uvval[i] = (uvval[i] < 0) ? 0 : ((uvval[i] > max) ? max : uvval[i]);
			tools.setvalue(buf + pos + ((i << 1) + 1), uvval[i]);
		}
		pos += ((this->m_cnum - 1) << 2);
	}
	return pos;
}

int C326Filter::Filter(char *src, char *dst)
{
	int size = 0, pos = 0, pos1 = 0, size1 = 0, count = 0, width = 0, offset = 0;

	offset = (this->m_ratio == RATIO_HALF) ? 1 : 2;
	size  = (this->m_width * this->m_height) << 1;
	size1 = (this->m_width * (this->m_height >> offset)) << 1;
	pos1  = (this->m_width * (this->m_cnum - 2)) << 1;
	count = size;
	while(size)
	{
		width = this->Write(src+pos, size);
		pos += width;
		size -= width;
		width = this->Read(dst+pos1, size1);
		pos1 += (width << offset);
		size1 -= width;
	}
	return count;
}

int C326Filter::Process(char *src, char *dst)
{
	int size = 0, len = 0;

	size  = (this->m_width * this->m_height) << 1;
	memcpy(dst, src, size);

	this->m_flg = PREF_Y;
	len = this->Filter(src, dst);
	this->Clear();
	this->m_flg = PREF_UV;
	len = this->Filter(src, dst);
	this->m_flg = PREF_NONE;
	return len;
}

//================= class CSizerModule ===================

C326Sizer::C326Sizer(int width,int height, int ratio, int format)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_ratio  = ratio;
	this->m_format = format;
}

C326Sizer::~C326Sizer()
{
}

int C326Sizer::Process(char *src, char *dst)
{
	switch(this->m_ratio)
	{
	case RATIO_HALF:
	case RATIO_QUARTER:
		return this->DownScale(src, dst);
		break;
	case RATIO_2X:
	case RATIO_4X:
		return this->UpScale(src, dst);
		break;
	default :
		return 0;
	}
}

int C326Sizer::DownScale(char *src, char *dst)
{
	int i = 0, pos = 0, val = 0, gap = this->m_width << 1;
	int src_off = 0, dst_off = 0, src_h_end = 0, src_v_end = 0;
	int yuv[5];

	if((this->m_ratio != RATIO_HALF) && (this->m_ratio != RATIO_QUARTER))
		this->m_ratio = RATIO_HALF;
	val = (this->m_ratio == RATIO_HALF) ? 1 : 2;

	src_h_end = gap - gap % (val << 3);
	src_v_end = (this->m_width * ((this->m_height >> val) << val)) << 1;
	pos = (gap << val) - gap;
	while(pos < src_v_end)
	{
		src_off = 0;
		while(src_off < src_h_end)
		{
			yuv[0] = *(src + pos + src_off);				//	Y0
			yuv[1] = *(src + pos + src_off + 1);			//	U
			yuv[2] = *(src + pos + src_off + (val << 2));	//	Y1
			yuv[3] = *(src + pos + src_off + 3);			//	V
			for(i = 0; i < 4; i++)
				*(dst + dst_off + i) = (char)yuv[i];
			dst_off += 4;
			src_off += (val << 3);
		}
		pos += (gap << val);
	}
	return dst_off;
}

int C326Sizer::UpScale(char *src, char *dst)
{
	int dst_w = 0, dst_h = 0, unit = 1;
	CYuv422 yuv422;

	if((this->m_ratio != RATIO_2X) && (this->m_ratio != RATIO_4X))
		this->m_ratio = RATIO_2X;
	dst_w = (this->m_ratio == RATIO_2X) ? (this->m_width << 1) : (this->m_width << 2);
	dst_h = (this->m_ratio == RATIO_2X) ? (this->m_height << 1) : (this->m_height << 2);

	if(this->m_format == YUV422_YUYV)
		return	yuv422.yuv422_resizeorder(src, dst, this->m_width, this->m_height, dst_w, dst_h, YUV422_YUYV, unit);
	return	yuv422.yuv422_resizeorder(src, dst, this->m_width, this->m_height, dst_w, dst_h, YUV422_YVYU, unit);
}


//==================== class C326SpecialEffect ===================

C326SpecialEffect::C326SpecialEffect(int width, int height, int format)
{
	this->SetSize(width, height);
	this->m_format = format;
	this->m_spe = IPPSEPHIA;
	this->m_uoffset = 0x80;
	this->m_voffset = 0x80;
	this->m_uup = 0xff;
	this->m_udown = 0x0;
	this->m_vup = 0xff;
	this->m_vdown = 0x0;
}

C326SpecialEffect::~C326SpecialEffect()
{
}

void C326SpecialEffect::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

void C326SpecialEffect::SetEffect(int spe)
{
	this->m_spe = spe&0x7;
}

void C326SpecialEffect::SetUOffset(int val)
{
	this->m_uoffset = val&0xff;
}

void C326SpecialEffect::SetVOffset(int val)
{
	this->m_voffset = val&0xff;
}

void C326SpecialEffect::SetUUp(int val)
{
	if(val < this->m_udown)
		val = this->m_udown;
	this->m_uup = val&0xff;
}

void C326SpecialEffect::SetVUp(int val)
{
	if(val < this->m_vdown)
		val = this->m_vdown;
	this->m_vup = val&0xff;
}

void C326SpecialEffect::SetUDown(int val)
{
	if(val > this->m_uup)
		val = this->m_uup;
	this->m_udown = val&0xff;
}

void C326SpecialEffect::SetVDown(int val)
{
	if(val > this->m_vup)
		val = this->m_vup;
	this->m_vdown = val&0xff;
}

int C326SpecialEffect::Sephia(char *src, char *dst)
{
	int i;
	int size = this->m_width * this->m_height * 2;

	for(i = 0; i < size; i += 4)
	{
		dst[i] = src[i];
		dst[i+1] = (char)this->m_uoffset;
		dst[i+2] = src[i+2];
		dst[i+3] = (char)this->m_voffset;
	}

	return size;
}

int C326SpecialEffect::SpecialColor(char *src, char *dst)
{
	int i;
	int size = this->m_width * this->m_height * 2;
	int u, v;

	for(i = 0; i < size; i += 4)
	{
		dst[i] = src[i];
		u = src[i+1]&0xff;
		dst[i+1] = (char)((u > this->m_uup) ? 0x80 : (u < this->m_udown ? 0x80 : u));
		dst[i+2] = src[i+2];
		v = src[i+3]&0xff;
		dst[i+3] = (char)((v > this->m_vup) ? 0x80 : (v < this->m_vdown ? 0x80 : v));
	}

	return size;
}

int C326SpecialEffect::Negative(char *src, char *dst)
{
	int i;
	int size = this->m_width * this->m_height * 2;
#ifdef NEGATIVE_RGB
	char buf[6];
	CYuv422 yuvtools;
#endif

#ifdef NEGATIVE_RGB
	for(i = 0; i < size; i+=4)
	{
		yuvtools.Yuyv2Rgb(src+i, buf);
		buf[0] = 255 - buf[0];
		buf[1] = 255 - buf[1];
		buf[2] = 255 - buf[2];
		buf[3] = 255 - buf[3];
		buf[4] = 255 - buf[4];
		buf[5] = 255 - buf[5];
		yuvtools.Rgb2Yuyv(buf, dst+i);
	}
#else
	for(i = 0; i < size; i++)
		dst[i] = 255 - src[i];
#endif

	return size;
}

int C326SpecialEffect::Relief(char *src, char *dst)
{
#ifdef RELIEF_RGB
	CYuv422 yuvtools;
	CBufferBase rgbBuf, rgbBuf2;
#endif
	C326Relief relief(this->m_width, this->m_height);
	int size = this->m_width * this->m_height * 2;
#ifdef RELIEF_RGB
	rgbBuf.SetSize(size*3/2);
	rgbBuf2.SetSize(size*3/2);
	yuvtools.MYuv422_Rgborder(src, rgbBuf.m_buf, this->m_width, this->m_height, YUV422_YUYV);
	relief.Process(rgbBuf.m_buf, rgbBuf2.m_buf);
	yuvtools.MRgb2Yuv422order(rgbBuf2.m_buf, dst, this->m_width, this->m_height, YUV422_YUYV);
#else
	relief.Process(src, dst);
#endif

	return size;
}

int C326SpecialEffect::Sketch(char *src, char *dst, char *edge)
{
	int i;
	int size = this->m_width * this->m_height * 2;

	src = src;
	if(edge == NULL)
		return 0;
	for(i = 0; i < size; i += 4)
	{
		dst[i] = SketchAddEdge((*((short*)edge+i/2)), (char)this->m_uup);
		dst[i+1] = (char)this->m_uoffset;
		dst[i+2] = SketchAddEdge((*((short*)edge+i/2+1)), (char)this->m_uup);
		dst[i+3] = (char)this->m_voffset;
	}

	return size;
}

char C326SpecialEffect::SketchAddEdge(short edge, char y)
{
	short flag;
	short val;
	short temp;

	flag = edge&0x100;
	edge &= 0xff;
	temp = (short)y&0xff;
	if(flag)
		val = temp - ((~edge+1)&0xff);
	else
		val = temp + edge;
	//val &= 0xff;
	return (char)((val < 0) ? 0 : val > 255 ? 255 : val);
}

int C326SpecialEffect::Process(char *src, char *dst, char *edge)
{
	int ret;

	if(this->m_format != YUV422_YUYV)
		return 0;

	switch(this->m_spe)
	{
	case IPPSEPHIA:
		ret = this->Sephia(src, dst);
		break;
	case IPPSPECIALCOLOR:
		ret = this->SpecialColor(src, dst);
		break;
	case IPPNEGATIVE:
		ret = this->Negative(src, dst);
		break;
	case IPPSKETCH:
		ret = this->Sketch(src, dst, edge);
		break;
	case IPPRELIEF:
		ret = this->Relief(src, dst);
		break;
	default:
		ret = 0;
		break;
	}

	return ret;
}

//==================== class C326Relief ===================

C326Relief::C326Relief(int width, int height)
{
	int i;

	this->SetSize(width, height);
	for(i = 0; i < 3; i++)
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	for(i = 0; i < 3; i++)
		this->m_pBlock[i] = this->m_Block[i];
}

C326Relief::~C326Relief()
{
}

void C326Relief::SetSize(int width, int height)
{
	int i;

	for(i = 0; i < 3; i++)
#ifdef RELIEF_RGB
		this->m_Linebuf[i].SetSize(width, 0, 3);
#else
		this->m_Linebuf[i].SetSize(width, 0, 2);
#endif
	this->m_width = width;
	this->m_height = height;
	this->Clear();
}

void C326Relief::Clear(void)
{
	this->m_line = 0;
	this->m_pos = 0;
	this->m_curline = 0;
}

int	C326Relief::Write(char *buf, int len)
{
#ifdef RELIEF_HW
	if(len == 0)
		return 0;
	return this->m_pLinebuf[2]->Write(buf, len);
#else
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST3X3LINES)
		return 0;
	if(this->m_line == 3)
		return this->m_pLinebuf[2]->Write(buf, len);
	while(this->m_line < 3)
	{
		pos1 = this->m_pLinebuf[this->m_line]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		if(pos1 == 0)
			this->m_line++;
		if(len == 0)
			break;
	}
	return pos;
#endif
}

int	C326Relief::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_pLinebuf[2]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	len = this->Relief(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len;
}

int	C326Relief::Relief(char *buf, int len)
{
	int val[3], i, pos = 0, max = 255;
	CBaseConvert tools;

	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
#ifdef RELIEF_HW
	if(this->m_curline < 2)
	{
#ifdef RELIEF_RGB
		memcpy(buf, this->m_pLinebuf[2]->m_buf, 3*len);
		return 3*len;
#else
		//memcpy(buf, this->m_pLinebuf[2]->m_buf, 2*len);
		memset(buf, 0x80, 2*len);
		this->m_pos = len;
		this->m_curline++;
		return 2*len;
#endif
	}
#endif
	for(; this->m_pos < len; this->m_pos++)
	{
#ifdef RELIEF_HW
		if(this->m_pos < 2)
		{
			int tmp;
			tmp = this->m_pLinebuf[2]->Read(this->m_pos);
			val[0] = 128;
			val[1] = 128;
			//val[0] = tmp&0xff;
			//val[1] = (tmp>>8)&0xff;
#ifdef RELIEF_RGB
			//val[2] = (tmp>>16)&0xff;
			val[2] = 128;
#endif
		}
		else
		{
			if(this->m_pos == 2)
				this->GetFirstBlock();
			else
				this->GetNextBlock();

#else
		if(this->m_pos == 0)
			this->GetFirstBlock();
		else
			this->GetNextBlock();
#endif

#ifdef RELIEF_HW
			val[0] = (this->m_pBlock[2][2]&0xff) - (this->m_pBlock[0][0]&0xff) + 128;
			val[1] = ((this->m_pBlock[2][2]>>8)&0xff) - ((this->m_pBlock[0][0]>>8)&0xff) + 128;
#ifdef RELIEF_RGB
			val[2] = ((this->m_pBlock[2][2]>>16)&0xff) - ((this->m_pBlock[0][0]>>16)&0xff) + 128;
#endif
		}
#else
		val[0] = (this->m_pBlock[0][0]&0xff) - (this->m_pBlock[2][2]&0xff) + 128;
		val[1] = ((this->m_pBlock[0][0]>>8)&0xff) - ((this->m_pBlock[2][2]>>8)&0xff) + 128;
#ifdef RELIEF_RGB
		val[2] = ((this->m_pBlock[0][0]>>16)&0xff) - ((this->m_pBlock[2][2]>>16)&0xff) + 128;
#endif
#endif
#ifdef RELIEF_RGB
		for(i = 0; i <= 2; i++, pos ++)
#else
		for(i = 0; i < 2; i++, pos++)
#endif
		{
			val[i] = (val[i] < 0) ? 0 : ((val[i] > max) ? max : val[i]);
			tools.setvalue(buf+pos, val[i], 1);
		}
	}
	return pos;
}

void C326Relief::LastLinePro(void)
{
	if(this->m_pLinebuf[2]->IsFull())
		return;
	this->m_line |= LAST3X3LINES;
	this->m_line--;
	this->m_pLinebuf[2] = this->m_pLinebuf[1];
}

void C326Relief::LoopLinebuf(void)
{
	int i;
	CLineBayerBuf *p = this->m_pLinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	for(i = 0; i < 2; i++)
		this->m_pLinebuf[i] = this->m_pLinebuf[i+1];
	this->m_pLinebuf[2] = p;
	this->m_pLinebuf[2]->ClearPoint();
}

void C326Relief::GetFirstBlock(void)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			this->m_pBlock[i][j] = this->m_pLinebuf[j]->Read(i);
}

void C326Relief::GetNextBlock(void)
{
	int i;
	int *p = this->m_pBlock[0];

	for(i = 0; i < 3; i++)
	{
		//if(this->m_pos+2 >= this->m_width)
		this->m_pBlock[0][i] = this->m_pLinebuf[i]->Read(this->m_pos);
		//else
		//	this->m_pBlock[0][i] = this->m_pLinebuf[i]->Read(this->m_pos+2);
	}
	for(i = 0; i < 2; i++)
		this->m_pBlock[i] = this->m_pBlock[i+1];
	this->m_pBlock[2] = p;
}

int C326Relief::Process(char *src, char *dst)
{
	int size, pos = 0, pos1 = 0, size1, width = 0;
	int temp;

#ifdef RELIEF_RGB
	size = this->m_width * this->m_height * 3;
#else
	size = this->m_width * this->m_height * 2;
#endif
	size1 = size;
	temp = size1;
	while(size)
	{
		width = this->Write(src+pos, size);
		pos += width;
		size -= width;
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	while(width)
	{
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	return temp;
}


//==================== class C326Canvas ===================

C326Canvas::C326Canvas(int width, int height)
{
	this->SetSize(width, height);
	this->SetPatternSize(64, 64);
	this->SetPatternFormat(YUV422_YUYV);
	this->m_line = 0;
}

C326Canvas::~C326Canvas()
{
}

void C326Canvas::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	this->m_linebuf.SetSize(width*2, 0);
}

void C326Canvas::SetPatternSize(int width, int height)
{
	this->m_patternwidth = width;
	this->m_patternheight = height;
}

void C326Canvas::SetPatternFormat(int format)
{
	this->m_patternformat = format;
}

int C326Canvas::Write(char *buf, int len)
{
	this->m_linebuf.ClearPoint();
	len = this->m_linebuf.Write(buf, len);

	return len;
}

int C326Canvas::Read(char *buf, char *src2, int len)
{
	int i, j, k;
	int y, uv;
	int count;

	i = this->m_line%this->m_patternheight;
	if(this->m_patternformat == YUV422_YUYV)
		count = i * this->m_patternwidth * 2;
	else
		count = i * this->m_patternwidth * 3;
	for(k = 0; k < len; k+=2)
	{
		j = (k/2) % this->m_patternwidth;
		if(this->m_patternformat == YUV422_YUYV)
		{
            y = this->m_linebuf.Read(k) + src2[count+j*2];
			uv = this->m_linebuf.Read(k+1) + (unsigned char)src2[count+j*2+1] - 0x80;
		}
		else
		{
			y = this->m_linebuf.Read(k) + src2[count+j*3];
			uv = this->m_linebuf.Read(k+1) + (unsigned char)src2[count+j*3+((k/2)%2)+1] - 0x80;
		}
		buf[k] = (char)((y < 0) ? 0 : (y > 255 ? 255 : y));
		buf[k+1] = (char)((uv < 0) ? 0 : (uv > 255 ? 255 : uv));
	}
	this->m_line++;

	return k;
}

void C326Canvas::PatternAvg(char *buf)
{
	int size, step;
	long avg = 0;
	int i;
	char temp;
#ifdef OUTPUT_AVERAGEPATTERN
	CFileBin fp;
#endif
	if(this->m_patternformat == YUV422_YUYV)
	{
		size = this->m_patternheight * this->m_patternwidth * 2;
		step = 2;
	}
	else
	{
		size = this->m_patternheight * this->m_patternwidth * 3;
		step = 3;
	}
	for(i = 0; i < size; i += step)
		avg += buf[i];
	avg /= (this->m_patternwidth*this->m_patternheight);
	for(i = 0; i < size; i += step)
	{
		if(buf[i] >= (char)avg)
			temp = buf[i] - (char)avg;
		else
		{
			temp = (char)avg - buf[i];
			temp = (~temp) + 1;
		}
		buf[i] = temp;
	}
#ifdef OUTPUT_AVERAGEPATTERN
	fp.Open("pattern_avg.dat", "wb");
	fp.Write(buf, size);
	fp.Close();
#endif
}

int C326Canvas::Process(char *src, char *src2, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * 2;

	this->PatternAvg(src2);

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, this->m_width*2);
		pos += len;
		len = this->Read(dst+pos1, src2, this->m_width*2);
		pos1 += len;
	}

	return pos1;
}


//==================== class C326Tv ===================

//Y--0 , U--1, V--2
static const int gTvPattern[][2] = 
{
	{1, 0},
	{2, 0},
	{2, 1},
	{0, 1},
	{0, 2},
	{1, 2}
};

C326Tv::C326Tv(int width, int height)
{
	this->SetSize(width, height);
	this->SetDelta();
	this->SetDeg(20);
	this->m_line = 0;
}

C326Tv::~C326Tv()
{
}

void C326Tv::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	this->m_linebuf.SetSize(width*2, 0);
}

void C326Tv::SetDeg(int value)
{
	this->m_deg = value;
}

void C326Tv::SetDelta(void)
{
	int rgb[3];
	
	rgb[0] = 2;
	rgb[1] = -1;
	rgb[2] = -1;
	this->rgb2yuv(rgb, this->m_delta_yuv[0]);
	rgb[0] = -1;
	rgb[1] = 2;
	rgb[2] = -1;
	this->rgb2yuv(rgb, this->m_delta_yuv[1]);
	rgb[0] = -1;
	rgb[1] = -1;
	rgb[2] = 2;
	this->rgb2yuv(rgb, this->m_delta_yuv[2]);	
}

void C326Tv::rgb2yuv(int *rgb, int *yuv)
{
	//yuv[0] = (77*rgb[0] + 150*rgb[1] + 29*rgb[2]) >> 8;
	//yuv[1] = (-38*rgb[0] - 74*rgb[1] + 112*rgb[2]) >> 8;
	//yuv[2] = (157*rgb[0] -132*rgb[1] - 25*rgb[2]) >> 8;
	yuv[0] = (77*rgb[0] + 150*rgb[1] + 29*rgb[2]);
	yuv[1] = (-38*rgb[0] - 74*rgb[1] + 112*rgb[2]);
	yuv[2] = (157*rgb[0] -132*rgb[1] - 25*rgb[2]);
	
}

int C326Tv::Write(char *buf, int len)
{
	this->m_linebuf.ClearPoint();
	len = this->m_linebuf.Write(buf, len);

	return len;
}

int C326Tv::Read(char *buf, int len)
{
	int i, k;
	int yuv;
	int y1, y2, u, v, deltaU, deltaV;

	i = this->m_line%6;
	for(k = 0; k < len; k+=4)
	{
#if 0
		j = (k/2) % 2;
		yuv = gTvPattern[i][j];
	    y = this->m_linebuf.Read(k) + (char)this->m_delta_yuv[yuv][0];
		uv = this->m_linebuf.Read(k+1) + (char)this->m_delta_yuv[yuv][((k/2)%2)+1];
		buf[k] = (char)((y < 0) ? 0 : (y > 255 ? 255 : y));
		buf[k+1] = (char)((uv < 0) ? 0 : (uv > 255 ? 255 : uv));
#else
		y1 = this->m_linebuf.Read(k);
		u = this->m_linebuf.Read(k+1);
		y2 = this->m_linebuf.Read(k+2);
		v = this->m_linebuf.Read(k+3);
		yuv = gTvPattern[i][0];
		y1 += (char)((this->m_delta_yuv[yuv][0]*this->m_deg+128)>>8);
		deltaU = this->m_delta_yuv[yuv][1];
		deltaV = this->m_delta_yuv[yuv][2];
		yuv = gTvPattern[i][1];
		y2 += (char)((this->m_delta_yuv[yuv][0]*this->m_deg+128)>>8);
		deltaU += this->m_delta_yuv[yuv][1];
		deltaV += this->m_delta_yuv[yuv][2];
		deltaU++;
		deltaV++;
		deltaU >>= 1;
		deltaV >>= 1;
		deltaU = (deltaU*this->m_deg+128)>>8;	
		deltaV = (deltaV*this->m_deg+128)>>8;
		u += deltaU;
		v += deltaV;
		buf[k] = (char)((y1 < 0) ? 0 : (y1 > 255 ? 255 : y1));
		buf[k+1] = (char)((u < 0) ? 0 : (u > 255 ? 255 : u));
		buf[k+2] = (char)((y2 < 0) ? 0 : (y2 > 255 ? 255 : y2));
		buf[k+3] = (char)((v < 0) ? 0 : (v > 255 ? 255 : v));
#endif
	}
	this->m_line++;

	return k;
}

int C326Tv::Process(char *src, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * 2;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, this->m_width*2);
		pos += len;
		len = this->Read(dst+pos1, this->m_width*2);
		pos1 += len;
	}

	return pos1;
}

//==================== class C326Shift ===================

static const int gRandSeed[] = {3, 2, 0, 7, 6, 9, 5, 4, 1, 8};
#define RAND_A	2053
#define RAND_B	13849
#define RAND_M	16		//2^16

C326Shift::C326Shift(int width, int height)
{
	this->SetSize(width, height);
	this->SetMaxMove(0x10);
	this->m_line = 0;
	this->m_currand = gRandSeed[0];
}

C326Shift::~C326Shift()
{
}

void C326Shift::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	this->m_linebuf.SetSize(width*2, 0);
}

void C326Shift::SetMaxMove(int value)
{
	this->m_maxmove = value;
}

int C326Shift::Rand(void)
{
	int val;

	this->m_currand = (RAND_A * this->m_currand + RAND_B) % (1<<RAND_M);
	val = (this->m_currand>>8) * this->m_maxmove  / ((1<<RAND_M)>>8);
//	val = gRandSeed[this->m_currand++]*this->m_maxmove/10;
	val -= val%2;
	//this->m_currand %= 10;
	return val;
}

int C326Shift::Write(char *buf, int len)
{
	this->m_linebuf.ClearPoint();
	len = this->m_linebuf.Write(buf, len);

	return len;
}

int C326Shift::Read(char *buf, int len)
{
	int i;
	int iShift;

	iShift = this->Rand();
	if(this->m_line%2)	// move left
	{
		for(i = 0; i < len - iShift*2; i+=2)
		{
			buf[i] = (char)this->m_linebuf.Read(i+iShift*2);
			buf[i+1] = (char)this->m_linebuf.Read(i+iShift*2+1);
		}
		for(; i < len; i+=2)
		{
			buf[i] = (char)this->m_linebuf.Read(len-2);
			if((i/2)%2)	//v
				buf[i+1] = (char)this->m_linebuf.Read(len-1);
			else
				buf[i+1] = (char)this->m_linebuf.Read(len-3);
		}
	}
	else	//move right
	{
		for(i = 0; i < iShift*2; i+=2)
		{
			buf[i] = (char)this->m_linebuf.Read(0);
			if((i/2)%2)		//v
				buf[i+1] = (char)this->m_linebuf.Read(3);
			else
				buf[i+1] = (char)this->m_linebuf.Read(1);
		}
		for(; i < len; i+=2)
		{
			buf[i] = (char)this->m_linebuf.Read(i-iShift*2);
			buf[i+1] = (char)this->m_linebuf.Read(i-iShift*2+1);
		}
	}
	this->m_line++;

	return i;
}

int C326Shift::Process(char *src, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * 2;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, this->m_width*2);
		pos += len;
		len = this->Read(dst+pos1, this->m_width*2);
		pos1 += len;
	}

	return pos1;
}


//==================== class C326WindBlow ===================

C326WindBlow::C326WindBlow(int width, int height)
{
	this->SetSize(width, height);
	this->SetThreshold(20);
	this->m_line = 0;
}

C326WindBlow::~C326WindBlow()
{
}

void C326WindBlow::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	this->m_linebuf.SetSize(width*2, 0);
	this->m_edgebuf.SetSize(width, 0);
}

void C326WindBlow::SetThreshold(int value)
{
	this->m_thd = value;
}

void C326WindBlow::GetEdge(void)
{
	int i;
//	char val;

	this->m_edgebuf.m_buf[0] = 0;
	for(i = 1; i < this->m_width; i++)
	{
		this->m_edgebuf.m_buf[i] = (char)abs((unsigned char)this->m_linebuf.m_buf[2*i] - (unsigned char)this->m_linebuf.m_buf[2*(i-1)]);
		//val = (char)abs((unsigned char)this->m_linebuf.m_buf[2*i] - (unsigned char)this->m_linebuf.m_buf[2*(i-1)]);
		//this->m_edgebuf.Write(val, 1);
	}
}

double C326WindBlow::Coeff(int n, int x)
{
	return (((double)n - (double)x)/(double)n) * 0.8;
}

int C326WindBlow::Write(char *buf, int len)
{
	this->m_linebuf.ClearPoint();
	this->m_edgebuf.ClearPoint();
	len = this->m_linebuf.Write(buf, len);

	return len;
}

int C326WindBlow::Read(char *buf, int len)
{
	int i, j, k;
	int rightpoint, n;
	int y, uv, y1, uv1;
	double coef;
	
	this->GetEdge();

	for(j = len-2; j >= 0; j -= 2)
	{
		if(((unsigned char)this->m_edgebuf.m_buf[j/2]) > this->m_thd)
		{
			i = this->m_line % 3;
			if(i == 0)
				rightpoint = j/2 + (unsigned char)this->m_edgebuf.m_buf[j/2]/15;
			else if(i == 1)
				rightpoint = j/2 + ((unsigned char)this->m_edgebuf.m_buf[j/2]/3)*2;
			else
				rightpoint = j/2 + (unsigned char)this->m_edgebuf.m_buf[j/2]/3;
			rightpoint = (rightpoint > this->m_width-1) ? this->m_width-1 : rightpoint;
			n = rightpoint - j/2 +1;
			y = (int)this->m_linebuf.m_buf[j]&0xff;
			uv = (int)this->m_linebuf.m_buf[j+1]&0xff;
			for(k = 0; k < n; k+=2)
			{
				coef = this->Coeff(n, k+1);
				y1 = (int)this->m_linebuf.m_buf[j+k*2]&0xff;
				uv1 = (int)this->m_linebuf.m_buf[j+k*2+1]&0xff;
				y1 = (int)(y*coef + y1*(1-coef));
				uv1 = (int)(128+(uv-128)*coef+(uv1-128)*(1-coef));
				this->m_linebuf.m_buf[j+k*2] = (char)((y1 > 255) ? 255 : (y1 < 0 ? 0 : y1));
				this->m_linebuf.m_buf[j+k*2+1] = (char)((uv1 > 255) ? 255 : (uv1 < 0 ? 0 : uv1));
			}
		}
	}
	memcpy(buf, this->m_linebuf.m_buf, len);
	this->m_line++;

	return len;
}

int C326WindBlow::Process(char *src, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * 2;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, this->m_width*2);
		pos += len;
		len = this->Read(dst+pos1, this->m_width*2);
		pos1 += len;
	}

	return pos1;
}

//==================== class C326Binary ===================

C326Binary::C326Binary(int width, int height)
{
	this->SetSize(width, height);
	this->SetThreshold(255);
	this->m_line = 0;
	this->m_sum = 0;
}

C326Binary::~C326Binary()
{
}

void C326Binary::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	this->m_linebuf.SetSize(width*2, 0);
}

void C326Binary::SetThreshold(int value)
{
	this->m_thd = value;
}

int C326Binary::Write(char *buf, int len)
{
	this->m_linebuf.ClearPoint();
	len = this->m_linebuf.Write(buf, len);

	return len;
}

int C326Binary::Read(char *buf, int len)
{
	int j;
	int uv = 0x80;
	int y = 0xff;

	if(this->m_line%2)	//right to left
	{
		for(j = len-2; j >= 0; j -= 2)
		{
			this->m_sum += (unsigned char)this->m_linebuf.Read(j);
			if(this->m_sum > this->m_thd)
			{
				buf[j] = (char)y;
				this->m_sum = 0;
			}
			else
				buf[j] = 0;
			buf[j+1] = (char)uv;
		}
	}
	else
	{
		for(j = 0; j < len; j += 2)
		{
			this->m_sum += (unsigned char)this->m_linebuf.Read(j);
			if(this->m_sum > this->m_thd)
			{
				buf[j] = (char)y;
				this->m_sum = 0;
			}
			else
				buf[j] = 0;
			buf[j+1] = (char)uv;
		}
	}
	this->m_line++;

	return len;
}

int C326Binary::Process(char *src, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * 2;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, this->m_width*2);
		pos += len;
		len = this->Read(dst+pos1, this->m_width*2);
		pos1 += len;
	}

	return pos1;
}


//=============== class C326Paint and Particle =================

C326PaintParticle::C326PaintParticle(int width, int height, int spe)
{
	int i;

	this->SetSize(width, height);
	for(i = 0; i < 3; i++)
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	for(i = 0; i < 3; i++)
		this->m_pBlock[i] = this->m_Block[i];
	this->m_spe = spe;
	this->m_rand = gRandSeed[0];
}

C326PaintParticle::~C326PaintParticle()
{
}

void C326PaintParticle::SetSize(int width, int height)
{
	int i;

	for(i = 0; i < 3; i++)
		this->m_Linebuf[i].SetSize(width, 0, 2);
	this->m_width = width;
	this->m_height = height;
	this->Clear();
}

void C326PaintParticle::SetSpecial(int spe)
{
	this->m_spe = spe;
}

void C326PaintParticle::Clear(void)
{
	this->m_line = 0;
	this->m_pos = 0;
	this->m_curline = 0;
}

int	C326PaintParticle::Write(char *buf, int len)
{
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST3X3LINES)
		return 0;
	if(this->m_line == 2)
		return this->m_pLinebuf[2]->Write(buf, len);
	while(this->m_line < 2)
	{
		pos1 = this->m_pLinebuf[1+this->m_line]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		if(pos1 == 0)
			this->m_line++;
		if(len == 0)
			break;
	}
	return pos;
}

int	C326PaintParticle::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_pLinebuf[2]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	len = this->PaintParticle(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len;
}

int	C326PaintParticle::PaintParticle(char *buf, int len)
{
	int val[2], i, pos = 0, max = 255, yuv;
	CBaseConvert tools;

	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	if((this->m_curline == 0) || (this->m_curline == this->m_height - 1))
	{
		memcpy(buf, this->m_pLinebuf[1]->m_buf, len*2);
		this->m_pos = this->m_width;
		return len*2;
	}
	for(; this->m_pos < len; this->m_pos++)
	{
		if((this->m_pos == 0) || (this->m_pos == this->m_width - 1))
		{
			this->m_rand = (RAND_A * this->m_rand + RAND_B) % (1<<RAND_M);		//update rand seed
			yuv = this->m_pLinebuf[1]->Read(this->m_pos);
			buf[pos++] = (char)(yuv&0xff);
			buf[pos++] = (char)((yuv>>8)&0xff);
			continue;
		}
		if(this->m_pos == 1)
			this->GetFirstBlock();
		else
			this->GetNextBlock();
		if(this->m_spe == IPPPAINT)
			yuv = this->GetMinPoint();
		else if(this->m_spe == IPPPARTICLE)
			yuv = this->GetRandPoint();
		else
			yuv = 0;
		val[0] = yuv&0xff;
		val[1] = (yuv>>8)&0xff;
		for(i = 0; i < 2; i++, pos++)
		{
			val[i] = (val[i] < 0) ? 0 : ((val[i] > max) ? max : val[i]);
			tools.setvalue(buf+pos, val[i], 1);
		}
	}
	return pos;
}

int C326PaintParticle::GetMinPoint(void)
{
	int yuv, i, j, minx;

	yuv = this->m_pBlock[0][0];
	minx = 0;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if((this->m_pBlock[i][j]&0xff) < (yuv&0xff))
			{
				yuv = this->m_pBlock[i][j];
				minx = i;
			}
		}
	}
	if(minx == 1)
		return yuv;
	else
		return (this->m_pBlock[1][1]&0xff00)|(yuv&0xff);
}

int C326PaintParticle::GetRandPoint(void)
{
	int i, j, val;

	this->m_rand = (RAND_A * this->m_rand + RAND_B) % (1<<RAND_M);
	val = (this->m_rand>>8) * 9 / ((1<<RAND_M)>>8);
	//val = gRandSeed[this->m_rand++]*8/10;
	i = val%3;
	j = val/3;
	val = this->m_pBlock[i][j];
	//this->m_rand %= 10;
	if(i == 1)
		return val;
	else
		return (this->m_pBlock[1][j]&0xff00)|(val&0xff);
}

void C326PaintParticle::LastLinePro(void)
{
	if(this->m_pLinebuf[2]->IsFull())
		return;
	this->m_line |= LAST3X3LINES;
	this->m_line--;
	this->m_pLinebuf[2] = this->m_pLinebuf[1];
}

void C326PaintParticle::LoopLinebuf(void)
{
	int i;
	CLineBayerBuf *p = this->m_pLinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	for(i = 0; i < 2; i++)
		this->m_pLinebuf[i] = this->m_pLinebuf[i+1];
	this->m_pLinebuf[2] = p;
	this->m_pLinebuf[2]->ClearPoint();
	this->m_curline ++;
}

void C326PaintParticle::GetFirstBlock(void)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			this->m_pBlock[i][j] = this->m_pLinebuf[j]->Read(i);
}

void C326PaintParticle::GetNextBlock(void)
{
	int i;
	int *p = this->m_pBlock[0];

	for(i = 0; i < 3; i++)
		this->m_pBlock[0][i] = this->m_pLinebuf[i]->Read(this->m_pos+1);
	for(i = 0; i < 2; i++)
		this->m_pBlock[i] = this->m_pBlock[i+1];
	this->m_pBlock[2] = p;
}

int C326PaintParticle::Process(char *src, char *dst)
{
	int size, pos = 0, pos1 = 0, size1, width = 0;
	int temp;

	size = this->m_width * this->m_height * 2;
	size1 = size;
	temp = size1;
	while(size)
	{
		width = this->Write(src+pos, size);
		pos += width;
		size -= width;
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	while(width)
	{
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	return temp;
}


//=============== class C578IppSizer =================

C578IppSizer::C578IppSizer(int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
	this->SetSrcSize(srcWidth, srcHeight);
	this->SetDstSize(dstWidth, dstHeight);
	this->m_read = 0;
	this->m_sum = 0;
}

C578IppSizer::~C578IppSizer()
{
}

void C578IppSizer::SetSrcSize(int width, int height)
{
	this->m_srcwidth = width;
	this->m_srcheight = height;
}

void C578IppSizer::SetDstSize(int width, int height)
{
	this->m_dstwidth = width;
	this->m_dstheight = height;
	this->m_linebuf.SetSize((width+IPP_SIZER_LINEBUF_SUP)*2, 0, 2);
	this->m_linebuf_tmp.SetSize((width+IPP_SIZER_LINEBUF_SUP)*2, 0, 2);
	this->m_linebuf.ClearPoint();
	this->m_linebuf_tmp.ClearPoint();
	memset(this->m_linebuf.m_buf, 0, (width+IPP_SIZER_LINEBUF_SUP)*2*2);
	memset(this->m_linebuf_tmp.m_buf, 0, (width+IPP_SIZER_LINEBUF_SUP)*2*2);
}

void C578IppSizer::SetSrcW(int w)
{
	this->m_srcw = w;
}

void C578IppSizer::SetDstW(int w)
{
	this->m_dstw = w;
}

void C578IppSizer::SetCeilW(int s)
{
	this->m_ceilw = s;
}

void C578IppSizer::SetFloorW(int s)
{
	this->m_floorw = s;
}

void C578IppSizer::SetSepW(int val)
{
	this->m_sepw = val;
}

void C578IppSizer::SetSrcH(int w)
{
	this->m_srch = w;
}

void C578IppSizer::SetDstH(int w)
{
	this->m_dsth = w;
}

void C578IppSizer::SetCeilH(int s)
{
	this->m_ceilh = s;
}

void C578IppSizer::SetFloorH(int s)
{
	this->m_floorh = s;
}

void C578IppSizer::SetSepH(int val)
{
	this->m_seph = val;
}

int C578IppSizer::Write(char *buf, int len)
{
//	1)	初始Sum=0，num=0，count=0
//	2)	读入第num个数据，sum=sum+w2，count = count+data[num]
//	3)	如果sum>=w1,则 
//			sum = sum-w1
//			如果sum>=w2-w1%w2
//				输出count/s2
//			否则
//				输出count/s1
//			count=0
//		否则如果sum<w1，则不输出
//	4)	循环2
	int sum = 0, num;
	int countY[2] = {0, 0}, countU[2] = {0, 0}, countV[2] = {0, 0};
	int y, uv, pos = 0;
	int	uflag = 0;
	int	div = 0;
	int k;
	
	if(this->m_read == 1)
		return 0;

	len = (this->m_srcwidth*2 > len) ? len : this->m_srcwidth*2;
	this->m_sum += this->m_dsth;
	if(this->m_sum >= this->m_srch)
	{
		this->m_read = 1;
		this->m_sum -= this->m_srch;
		if(this->m_sum >= this->m_seph)
			div = this->m_ceilh;
		else
			div = this->m_floorh;
	}
	for(num = 0; num < len; num += 2)
	{
		sum += this->m_dstw;
		countY[uflag] += (unsigned char)(buf[num]);
		if(((num>>1)%2) == 0)	//u
		{
			countU[uflag] += (unsigned char)(buf[num+1]);
			countV[uflag] += (unsigned char)(buf[num+3]);
		}
		else
		{
			countU[uflag] += (unsigned char)(buf[num-1]);
			countV[uflag] += (unsigned char)(buf[num+1]);
		}
		if(sum >= this->m_srcw)
		{
			sum -= this->m_srcw;
			if(sum >= this->m_sepw)
			{
#if 0	//hw
				countY[uflag] = (countY[uflag] * this->m_ceilw)>>16;
				countU[uflag] = (countU[uflag] * this->m_ceilw)>>16;
				countV[uflag] = (countV[uflag] * this->m_ceilw)>>16;
#else
				countY[uflag] /= this->m_ceilw;
				countU[uflag] /= this->m_ceilw;
				countV[uflag] /= this->m_ceilw;
#endif
			}
			else
			{
#if 0	//hw
				countY[uflag] = (countY[uflag] * this->m_floorw)>>16;	
				countU[uflag] = (countU[uflag] * this->m_floorw)>>16;
				countV[uflag] = (countV[uflag] * this->m_floorw)>>16;
#else
				countY[uflag] /= this->m_floorw;
				countU[uflag] /= this->m_floorw;
				countV[uflag] /= this->m_floorw;
#endif
			}
			if(uflag)
			{
				for(k = 0; k < 2; k++)
				{
					y = this->m_linebuf.Read(pos);
					uv = this->m_linebuf.Read(pos+1);
					y += countY[k];
					if(k == 0)
						uv += (countU[0]+countU[1])>>1;
					else
						uv += (countV[0]+countV[1])>>1;
					if(this->m_read == 1)
					{
#if 0	//hw
						y = (y * div)>>16;
						uv = (uv * div)>>16;
#else
						y /= div;
						uv /= div;
#endif
					}
					this->m_linebuf_tmp.Write(y);
					this->m_linebuf_tmp.Write(uv);
					pos += 2;
				}
				countY[0] = 0;
				countU[0] = 0;
				countV[0] = 0;
				countY[1] = 0;
				countU[1] = 0;
				countV[1] = 0;
				uflag = 0;
			}
			else
				uflag = 1;
		}
	}
	this->m_linebuf.ClearPoint();
	this->m_linebuf.Write(this->m_linebuf_tmp.m_buf, (this->m_dstwidth+IPP_SIZER_LINEBUF_SUP)*2*2);
	this->m_linebuf_tmp.ClearPoint();

	return num;
}

int C578IppSizer::Read(char *buf, int len)
{
	int i;
	int y, uv;

	if(this->m_read == 0)
		return 0;
	len = (this->m_dstwidth*2 > len) ? len : this->m_dstwidth*2;
	for(i = 0; i < len; i += 2)
	{
		y = this->m_linebuf.Read(i);
		uv = this->m_linebuf.Read(i+1);
		buf[i] = (char)y;
		buf[i+1] = (char)uv;
	}
	this->m_read = 0;
	memset(this->m_linebuf.m_buf, 0, (this->m_dstwidth+IPP_SIZER_LINEBUF_SUP) * 4);
	this->m_linebuf.ClearPoint();

	return i;
}

int C578IppSizer::Process(char *src, char *dst)
{
	int size, pos = 0, pos1 = 0, size1, width = 0;

	size = this->m_srcwidth * this->m_srcheight * 2;
	size1 = this->m_dstwidth * this->m_dstheight * 2;
	while(size)
	{
		width = this->Write(src+pos, size);
		pos += width;
		size -= width;
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	while(width)
	{
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	return pos1;
}


////////////////////////////////////////////////////////////////////////////////////////
//=============== class C578IppZoomIn =================

C578IppZoomIn::C578IppZoomIn(int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
	this->SetSrcSize(srcWidth, srcHeight);
	this->SetDstSize(dstWidth, dstHeight);
	this->m_sum = 0;
}

C578IppZoomIn::~C578IppZoomIn()
{
}

void C578IppZoomIn::SetSrcSize(int width, int height)
{
	this->m_srcwidth = width;
	this->m_srcheight = height;
}

void C578IppZoomIn::SetDstSize(int width, int height)
{
	int i;

	this->m_dstwidth = width;
	this->m_dstheight = height;
	for(i = 0; i < 2; i++)
	{
		this->m_linebuf[i].SetSize(width*2, 0, 1);
		this->m_plinebuf[i] = &(this->m_linebuf[i]);
	}
}

void C578IppZoomIn::SetSrcW(int w)
{
	this->m_srcw = w;
}

void C578IppZoomIn::SetDstW(int w)
{
	this->m_dstw = w;
}

void C578IppZoomIn::SetParaW(int val)
{
	this->m_paraw = val;
}

void C578IppZoomIn::SetSrcH(int w)
{
	this->m_srch = w;
}

void C578IppZoomIn::SetDstH(int w)
{
	this->m_dsth = w;
}

void C578IppZoomIn::SetParaH(int val)
{
	this->m_parah = val;
}

int C578IppZoomIn::Write(char *buf, int len)
{
	int sum = 0, num = 0;
	int y[2] = {0,0}, u[2] = {0, 0}, v[2] = {0, 0};
	int yout[2] = {0,0}, uout[2] = {0, 0}, vout[2] = {0, 0};
	int uflag = 0;
	int pos = 0;

	if(len == 0)
		return 0;
	len = (this->m_srcwidth*2 > len) ? len : this->m_srcwidth*2;
	yout[uflag] = (y[0] = buf[num++]);
	uout[uflag] = (u[0] = buf[num++]);
	y[1] = buf[num++];
	v[1] = buf[num++];
	vout[uflag] = (v[0] = v[1]);
	u[1] = u[0];
	//this->m_plinebuf[1]->Write(y[0]);
	//this->m_plinebuf[1]->Write((u[0]+u[1])>>1);
	uflag = 1;

	for(; num < len; )
	{
		sum += this->m_srcw;
		if(sum >= this->m_dstw)
		{
			sum -= this->m_dstw;
			y[0] = y[1];
			u[0] = u[1];
			v[0] = v[1];
			y[1] = buf[num++];
			if(((num>>1)%2) == 0)	//u
			{
				u[1] = buf[num++];
				v[1] = buf[num+1];
			}
			else
			{
				v[1] = buf[num++];
				u[1] = u[0];
			}
		}
		yout[uflag] = (y[0]*(this->m_dstw - sum) + y[1]*sum)/this->m_dstw;
		uout[uflag] = (u[0]*(this->m_dstw - sum) + u[1]*sum)/this->m_dstw;
		vout[uflag] = (v[0]*(this->m_dstw - sum) + v[1]*sum)/this->m_dstw;
		if(uflag)
		{
			this->m_plinebuf[1]->Write(yout[0]);
			this->m_plinebuf[1]->Write((uout[0]+uout[1])>>1);
			this->m_plinebuf[1]->Write(yout[1]);
			this->m_plinebuf[1]->Write((vout[0]+vout[1])>>1);
			pos += 4;
		}
		uflag = (uflag == 0) ? 1 : 0;
	}
	while(pos < this->m_dstwidth*2)
	{
		this->m_plinebuf[1]->Write(yout[0]);
		this->m_plinebuf[1]->Write((uout[0]+uout[1])>>1);
		this->m_plinebuf[1]->Write(yout[1]);
		this->m_plinebuf[1]->Write((vout[0]+vout[1])>>1);
		pos += 4;
	}
	return num;
}

int C578IppZoomIn::Read(char *buf, int len)
{
	if(this->m_plinebuf[1]->IsFull() == 0)
		return 0;
	if(this->m_plinebuf[0]->IsFull() == 0)
	{
		len = (this->m_dstwidth*2 > len) ? len : this->m_dstwidth*2;
		len = this->m_plinebuf[1]->Read(buf, len);
		this->m_sum += this->m_srch;
		this->LoopLinebuf();
		return len;
	}
	len = this->ZoomIn(buf, len);
	this->LoopLinebuf();
	return len;
}

int C578IppZoomIn::ZoomIn(char *buf, int len)
{
	int y[2], uv[2];
	int num;
	int pos = 0;

	len = (this->m_dstwidth*2 > len) ? len : this->m_dstwidth*2;
	
	while(this->m_sum <= this->m_dsth)
	{
		for(num = 0; num < len; num+=2)
		{
			y[0] = this->m_plinebuf[0]->Read(num);
			uv[0] = this->m_plinebuf[0]->Read(num+1);
			y[1] = this->m_plinebuf[1]->Read(num);
			uv[1] = this->m_plinebuf[1]->Read(num+1);
			y[0] = (y[0]*(this->m_dsth - this->m_sum ) + y[1]*this->m_sum)/this->m_dsth;
			uv[0] = (uv[0]*(this->m_dsth - this->m_sum ) + uv[1]*this->m_sum)/this->m_dsth;
			buf[pos++] = (char)y[0];
			buf[pos++] = (char)uv[0];
		}
		this->m_sum += this->m_srch;
	}
	this->m_sum -= this->m_dsth;
	
	return pos;
}

void C578IppZoomIn::LoopLinebuf(void)
{
	CLineBuf *p = this->m_plinebuf[0];

	this->m_plinebuf[0] = this->m_plinebuf[1];
	this->m_plinebuf[1] = p;
	this->m_plinebuf[1]->ClearPoint();
}

int C578IppZoomIn::Process(char *src, char *dst)
{
	int size, pos = 0, pos1 = 0, size1, width = 0;

	size = this->m_srcwidth * this->m_srcheight * 2;
	size1 = this->m_dstwidth * this->m_dstheight * 2;
	while(size)
	{
		width = this->Write(src+pos, size);
		pos += width;
		size -= width;
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	while(width)
	{
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	return pos1;
}



