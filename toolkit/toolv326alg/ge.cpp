
#include "internal.h"

//========================== CGeDestSpanBuf ===============================

CGeDestSpanBuf::CGeDestSpanBuf(int size, int unit) : CSpanBuf(size, unit)
{
	this->SetOption(0, 1, 0);
}

CGeDestSpanBuf::~CGeDestSpanBuf(void)
{
}

int CGeDestSpanBuf::GetValue(int index)
{
	return CSpanBuf::ReadOne(index);
}

void CGeDestSpanBuf::SetValue(int value, int index)
{
	CSpanBuf::WriteOne(value, index);
}

void CGeDestSpanBuf::SetDestSize(char *buf, int size, int format)
{
	if(format == GE_RGB24)
		CSpanBuf::SetSize(buf, size/3, 3);
	else
		CSpanBuf::SetSize(buf, size/2, 2);
}

void CGeDestSpanBuf::SetDestPosition(int start, int dstWidth, int dstSpan)
{
	CSpanBuf::SetOption(start, dstWidth*this->m_unit, (dstSpan-dstWidth)*this->m_unit);
}

//========================== CGeSrcSpanBuf ===============================

CGeSrcSpanBuf::CGeSrcSpanBuf(int size, int unit) : CSpanBuf(size, unit)
{
	this->SetOption(0, 1, 0);
}

CGeSrcSpanBuf::~CGeSrcSpanBuf(void)
{
}

int	CGeSrcSpanBuf::GetValue(int index)
{
	int i, j, value;

	if(this->m_format == GE_MONOCOLOR)
	{
		i = index%8;
		j = index>>3;
		value = CSpanBuf::ReadOne(j);
		return (value>>(7-i))&0x1;
	}
	else
		return CSpanBuf::ReadOne(index);
}

void CGeSrcSpanBuf::SetFormat(int format)
{
	this->m_format = format;
}

void CGeSrcSpanBuf::SetSrcSize(char *buf, int size, int format)
{
	this->SetFormat(format);
	if(format == GE_RGB24)
		CSpanBuf::SetSize(buf, size/3, 3);
	else if(format == GE_RGB16)
		CSpanBuf::SetSize(buf, size/2, 2);
	else
		CSpanBuf::SetSize(buf, size, 1);
}

void CGeSrcSpanBuf::SetDestPosition(int start, int dstWidth, int dstSpan)
{
	if(this->m_format == GE_MONOCOLOR)
		CSpanBuf::SetOption(start, (dstWidth+7)>>3, dstSpan - ((dstWidth+7)>>3));
	else
		CSpanBuf::SetOption(start, dstWidth*this->m_unit, (dstSpan-dstWidth)*this->m_unit);
}

//========================== GE ===============================

CGe::CGe(int width, int height)
{
	this->InitGBuf();
	this->SetBitbltColorDepth(GE_RGB24);
	this->SetBitWise(0xcc);
	this->SetClipLeftTop(0, 0);
	this->SetClipRightBottom(width-1, height-1);
	this->SetColorTransSel(0);
	this->SetColorTrnasEn(0);
	this->SetLinecolor(0x0);
	this->SetLineDrawEnd(0, 0);
	this->SetLineDrawStart(0, 0);
	this->SetPatternBg(0xff);
	this->SetPatternColorDepth(GE_RGB24);
	this->SetPatternFg(0x0);
	this->SetPatternMask(0);
	this->SetSrcBg(0xff);
	this->SetSrcColorDepth(GE_RGB24);
	this->SetSrcExtendSel(0);
	this->SetSrcFg(0);
	this->SetSrcMask(0);
	this->SetStartPointSel(0);
	this->SetSpan(0);
	this->SetDestSize(width, height);
}

CGe::~CGe(void)
{
}

void CGe::InitGBuf(void)
{
	this->m_gbuf.SetSize(GE_GBUF_SIZE);	
	this->m_pattern_buf.SetSize(GE_PATTERN_BUF_SIZE);
	memset(this->m_gbuf.m_buf, 0, GE_GBUF_SIZE);
	memset(this->m_pattern_buf.m_buf, 0, GE_PATTERN_BUF_SIZE);
	this->m_dstbuf.SetDestSize(this->m_gbuf.m_buf, GE_GBUF_SIZE);
	this->m_srcbuf.SetSrcSize(this->m_gbuf.m_buf, GE_GBUF_SIZE);
}

void CGe::SetDestSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

void CGe::GetDestBuf(char *buf, int size)
{
	this->m_dstbuf.Read(buf, size);
}

void CGe::SetPattern(char *buf, int size)
{
	memcpy(this->m_pattern_buf.m_buf, buf, size);
}

void CGe::SetDestBuf(char *buf, int size)
{
	this->m_dstbuf.SetDestSize(this->m_gbuf.m_buf, GE_GBUF_RGB24_SIZE, this->m_bitblt_colordepth);
	this->m_dstbuf.SetDestPosition(this->m_dst_addr_offset, this->m_width, this->m_span);
	this->m_dstbuf.Write(buf, size);
}

void CGe::SetSrcBuf(char *buf, int size)
{
	this->m_srcbuf.SetSrcSize(this->m_gbuf.m_buf, GE_GBUF_RGB24_SIZE, this->m_src_colordepth);
	this->m_srcbuf.SetDestPosition(this->m_src_addr_offset, this->m_width, this->m_span);
	this->m_srcbuf.Write(buf, size);
}

char* CGe::GetGBufAddr(void)
{
	return this->m_gbuf.m_buf;
}

void CGe::SetDestAddr(char *addr)
{
	unsigned int offset;
	unsigned int tmp;

	if(this->m_bitblt_colordepth == GE_RGB24)
		tmp = this->m_width * this->m_height * 3;
	else
		tmp = this->m_width * this->m_height * 2;
	tmp = GE_GBUF_SIZE - tmp;
	offset = (unsigned int)(addr-this->m_gbuf.m_buf);
	this->m_dst_addr_offset = (offset < 0) ? 0 : (offset > tmp) ? tmp : offset;
}

void CGe::SetSrcAddr(char *addr)
{
	unsigned int offset;
	unsigned int tmp;

	if(this->m_bitblt_colordepth == GE_RGB24)
		tmp = this->m_width * this->m_height * 3;
	else
		tmp = this->m_width * this->m_height * 2;
	tmp = GE_GBUF_SIZE - tmp;
	offset = (unsigned int)(addr - this->m_gbuf.m_buf);
	this->m_src_addr_offset = (offset < 0) ? 0: (offset > tmp) ? tmp : offset;
}

void CGe::SetDestAddrOffset(unsigned int offset)
{
	unsigned int tmp;

	if(this->m_bitblt_colordepth == GE_RGB24)
		tmp = this->m_width * this->m_height * 3;
	else
		tmp = this->m_width * this->m_height * 2;
	tmp = GE_GBUF_SIZE - tmp;
	this->m_dst_addr_offset = (offset < 0) ? 0 : (offset > tmp) ? tmp : offset;
}

void CGe::SetSrcAddrOffset(unsigned int offset)
{
	unsigned int tmp;

	if(this->m_bitblt_colordepth == GE_RGB24)
		tmp = this->m_width * this->m_height * 3;
	else
		tmp = this->m_width * this->m_height * 2;
	tmp = GE_GBUF_SIZE - tmp;
	this->m_src_addr_offset = (offset < 0) ? 0: (offset > tmp) ? tmp : offset;
}

void CGe::SetBitbltColorDepth(int depth)
{
	this->m_bitblt_colordepth = depth;
}

void CGe::SetBitWise(int val)
{
	this->m_bitwise_sel = val&0xff;
}

void CGe::SetClipLeftTop(int x, int y)
{
	this->m_clip_lefttop_x = x&0xfff;
	this->m_clip_lefttop_y = y&0xfff;
}

void CGe::SetClipRightBottom(int x, int y)
{
	this->m_clip_rightbottom_x = x&0xfff;
	this->m_clip_rightbottom_y = y&0xfff;
}

void CGe::SetColorTransSel(int sel)
{
	this->m_color_trans_sel = sel&0x3;
}

void CGe::SetColorTrnasEn(int en)
{
	this->m_color_trans_en = en;
}

void CGe::SetLinecolor(int color)
{
	this->m_linecolor = color&0xffffff;
}

void CGe::SetLineDrawEnd(int x, int y)
{
	this->m_linedraw_end_x = x&0xfff;
	this->m_linedraw_end_y = y&0xfff;
}

void CGe::SetLineDrawStart(int x, int y)
{
	this->m_linedraw_start_x = x&0xfff;
	this->m_linedraw_start_y = y&0xfff;
}

void CGe::SetPatternBg(int bg)
{
	this->m_pattern_bg = bg&0xffffff;
}

void CGe::SetPatternColorDepth(int depth)
{
	this->m_pattern_colordepth = depth;
}

void CGe::SetPatternFg(int fg)
{
	this->m_pattern_fg = fg&0xffffff;
}

void CGe::SetPatternMask(int mask)
{
	this->m_pattern_mask = mask;
}

void CGe::SetSrcBg(int bg)
{
	this->m_src_bg = bg&0xffffff;
}

void CGe::SetSrcColorDepth(int depth)
{
	this->m_src_colordepth = depth;
}

void CGe::SetSrcExtendSel(int sel)
{
	this->m_src_extend_sel = sel;
}

void CGe::SetSrcFg(int fg)
{
	this->m_src_fg = fg&0xffffff;
}

void CGe::SetSrcMask(int mask)
{
	this->m_src_mask = mask;
}

void CGe::SetStartPointSel(int sel)
{
	this->m_startpoint_sel = sel&0x3;
}

void CGe::SetSpan(int span)
{
	this->m_span = span;
}

int CGe::ColorExpansion(int monovalue, int flag)
{
	int ret;

	if(flag == 0)		//pattern expansion
	{
		if(monovalue)	//forground
			if(this->m_bitblt_colordepth == GE_RGB24)
				ret = this->m_pattern_fg&0xffffff;
			else
				ret = this->m_pattern_fg&0xffff;
		else
			if(this->m_bitblt_colordepth == GE_RGB24)
				ret = this->m_pattern_bg&0xffffff;
			else
				ret = this->m_pattern_bg&0xffff;
	}
	else	//source expansion
	{
		if(monovalue)	//forground
			if(this->m_bitblt_colordepth == GE_RGB24)
				if(this->m_src_extend_sel)	//use source extend para
					ret = this->m_src_fg&0xffffff;
				else
					ret = this->m_pattern_fg&0xffffff;
			else
				if(this->m_src_extend_sel)
					ret = this->m_src_fg&0xffff;
				else
					ret = this->m_pattern_fg&0xffff;
		else
			if(this->m_bitblt_colordepth == GE_RGB24)
				if(this->m_src_extend_sel)
					ret = this->m_src_bg&0xffffff;
				else
					ret = this->m_pattern_bg&0xffffff;
			else
				if(this->m_src_extend_sel)
					ret = this->m_src_bg&0xffff;
				else
					ret = this->m_pattern_bg&0xffff;
	}
	
	return ret;			
}

//=== return value:	expand value: mask = 0	or mask value:mask = 1
int CGe::GetPatternValue(int index, int mask)	
{
	int val = 0;
	int x, y, i, tmp;

	x = index%this->m_width;	//which col
	y = index/this->m_width;	//which line
	x %= 8;		//which col in pattern
	y %= 8;
	i = y*8+x;
	if(this->m_pattern_colordepth == GE_RGB24)
	{
		val |= (int)(*(this->m_pattern_buf.m_buf + i*3))&0xff;
		val |= ((int)(*(this->m_pattern_buf.m_buf + i*3 + 1))&0xff) << 8;
		val |= ((int)(*(this->m_pattern_buf.m_buf + i*3 + 2))&0xff) << 16;
		val &= 0xffffff;
	}
	else if(this->m_pattern_colordepth == GE_RGB16)
	{
		val |= (int)(*(this->m_pattern_buf.m_buf + i*2))&0xff;
		val |= ((int)(*(this->m_pattern_buf.m_buf +i*2 + 1))&0xff) << 8;
		val &= 0xffff;
	}
	else if(this->m_pattern_colordepth == GE_MONOCOLOR)
	{
		tmp = i/32;
		val = *((int*)this->m_pattern_buf.m_buf + tmp);
		i %= 32;
		val = (val>>i)&0x1;		
		if(!mask)
			val = this->ColorExpansion(val, 0);
	}
	else
		val = 0;

	return val;
}

int CGe::GetSrcValue(int index)
{
	return this->m_srcbuf.GetValue(index);
}

int CGe::GetDstValue(int index)
{
	return this->m_dstbuf.GetValue(index);
}

int CGe::BitBltAlg(int p, int s, int d)
{
	int i, num, val = 0;
	int pTmp, sTmp, dTmp, tmp;

	if(this->m_bitblt_colordepth == GE_RGB24)
		num = 24;
	else
		num = 16;

	for(i = 0; i < num; i++)
	{
		pTmp = ((p>>i)<<2)&0x4;
		sTmp = ((s>>i)<<1)&0x2;
		dTmp = (d>>i)&0x1;
		tmp = ((pTmp|sTmp)|dTmp)&0x7;
		tmp = (this->m_bitwise_sel>>tmp)&0x1;
		val = val | (tmp<<i);
	}

	return val;
}

int CGe::PixelMask(int index)
{
	int flag = 0;
	int val;

	if((this->m_pattern_colordepth == GE_MONOCOLOR) && (this->m_pattern_mask != 0))	//pattern mask enable
	{
		val = this->GetPatternValue(index, 1);
		if(val)
			flag = 1;
	}
	if((this->m_src_colordepth == GE_MONOCOLOR) && (this->m_src_mask != 0))		//source mask enable
	{
		val = this->GetSrcValue(index);
		if(val)
			flag = 1;
	}
	return flag;
}

int CGe::PixelColorTrans(int index, int value)
{
	int flag = 0;
	int tColor;
	
	if(this->m_color_trans_en == 0)
		return flag;

	if(this->m_src_extend_sel)
		tColor = this->m_src_bg;
	else
		tColor = this->m_pattern_bg;
	switch(this->m_color_trans_sel)
	{
	case GE_AFTER_NOTSAME:
		if(value != tColor)
			flag = 0;
		else
			flag = 1;
		break;
	case GE_BEFORE_NOTSAME:
		value = this->GetDstValue(index);
		if(value != tColor)
			flag = 0;
		else
			flag = 1;
		break;
	case GE_AFTER_SAME:
		if(value == tColor)
			flag = 0;
		else
			flag = 1;
		break;
	case GE_BEFORE_SAME:
		value = this->GetDstValue(index);
		if(value == tColor)
			flag = 0;
		else
			flag = 1;
		break;
	default:
		break;
	}

	return flag;
}

void CGe::SetDstValue(int value, int index)
{
	this->m_dstbuf.SetValue(value, index);
}

int CGe::GetStartPointIndex(int i)
{
	int index = i;

	switch(this->m_startpoint_sel)
	{
	case GE_TOPLEFT:
		index = i;
		break;
	case GE_TOPRIGHT:
		index = i + this->m_width - ((i%this->m_width)<<1) - 1;
		break;
	case GE_BOTTOMLEFT:
		index = this->m_width * this->m_height - i - this->m_width + ((i%this->m_width)<<1);
		break;
	case GE_BOTTOMRIGHT:
		index = this->m_width * this->m_height - i - 1;
		break;
	default:
		break;
	}
	return index;
}

void CGe::StartBitblt(void)
{
	int i, index;
	int size = this->m_width * this->m_height;
	int srcValue, dstValue, patternValue;
	int flag = 0, flag1 = 0;

	this->m_dstbuf.SetDestSize(this->m_gbuf.m_buf, GE_GBUF_RGB24_SIZE, this->m_bitblt_colordepth);
	this->m_dstbuf.SetDestPosition(this->m_dst_addr_offset, this->m_width, this->m_span);
	this->m_srcbuf.SetSrcSize(this->m_gbuf.m_buf, GE_GBUF_RGB24_SIZE, this->m_src_colordepth);
	this->m_srcbuf.SetDestPosition(this->m_src_addr_offset, this->m_width, this->m_span);
	
	for(i = 0; i < size ; i++)
	{
		index = this->GetStartPointIndex(i);
		srcValue = this->GetSrcValue(index);
		if(this->m_src_colordepth == GE_MONOCOLOR)
			srcValue = this->ColorExpansion(srcValue, 1);
		dstValue = this->GetDstValue(index);
		patternValue = this->GetPatternValue(index, 0);
		dstValue = this->BitBltAlg(patternValue, srcValue, dstValue);
		flag = this->PixelMask(index);
		flag1 = this->PixelColorTrans(index, dstValue);
		if((!flag) && (!flag1))
			this->SetDstValue(dstValue, index);
	}
}

void CGe::PixelDraw(int x, int y)
{
	if(((x <= this->m_clip_rightbottom_x) && (x >= this->m_clip_lefttop_x)) && ((y >= this->m_clip_rightbottom_y) && (y <= this->m_clip_lefttop_y)))
		this->SetDstValue(this->m_linecolor, x + y*this->m_width);
}

void CGe::LineDrawNeg(void)
{
	int x, y, dx, dy, e;
	int x0, y0, x1, y1;
	int i;
	
	x0 = (this->m_linedraw_start_x - this->m_linedraw_end_x) > 0 ? this->m_linedraw_end_x : this->m_linedraw_start_x;
	y0 = (this->m_linedraw_start_y - this->m_linedraw_end_y) < 0 ? this->m_linedraw_end_y : this->m_linedraw_start_y;
	x1 = (this->m_linedraw_start_x - this->m_linedraw_end_x) < 0 ? this->m_linedraw_end_x : this->m_linedraw_start_x;
	y1 = (this->m_linedraw_start_y - this->m_linedraw_end_y) > 0 ? this->m_linedraw_end_y : this->m_linedraw_start_y;
	dx = x1 - x0;
	dy = y1 - y0;
	x = x0;
	y = y0;
	if(dy/dx == 0)
	{
        e = dx;
		for (i = 0; i <= dx; i++) 
		{
			this->PixelDraw(x, y);
			x = x + 1;
			e = e + 2*dy;
			if (e <= 0)
			{
				y = y-1;
				e = e+2*dx;
			}
		}
	}
	else
	{
		e = dy;
		for (i = 0; i <= -dy; i++) 
		{
			this->PixelDraw(x, y);
			y = y - 1;
			e = e + 2*dx;
			if (e >= 0)
			{
				x = x+1;
				e = e+2*dy;
			}
		}
	}
}

void CGe::LineDrawPlus(void)
{
	int x, y, dx, dy, e;
	int x0, y0, x1, y1;
	int i;
	
	x0 = (this->m_linedraw_start_x - this->m_linedraw_end_x) > 0 ? this->m_linedraw_end_x : this->m_linedraw_start_x;
	y0 = (this->m_linedraw_start_y - this->m_linedraw_end_y) > 0 ? this->m_linedraw_end_y : this->m_linedraw_start_y;
	x1 = (this->m_linedraw_start_x - this->m_linedraw_end_x) < 0 ? this->m_linedraw_end_x : this->m_linedraw_start_x;
	y1 = (this->m_linedraw_start_y - this->m_linedraw_end_y) < 0 ? this->m_linedraw_end_y : this->m_linedraw_start_y;
	dx = x1 - x0;
	dy = y1 - y0;
	x = x0;
	y = y0;
	if(dy/dx == 0)
	{
        e = -dx;
		for (i = 0; i <= dx; i++) 
		{
			this->PixelDraw(x, y);
			x = x + 1;
			e = e + 2*dy;
			if (e >= 0)
			{
				y = y+1;
				e = e-2*dx;
			}
		}
	}
	else
	{
		e = -dy;
		for (i = 0; i <= dy; i++) 
		{
			this->PixelDraw(x, y);
			y = y + 1;
			e = e + 2*dx;
			if (e >= 0)
			{
				x = x+1;
				e = e-2*dy;
			}
		}
	}
}

void CGe::StartLineDraw(void)
{
	int x0, y0, x1, y1, flag;
	
	x0 = this->m_linedraw_start_x;
	y0 = this->m_linedraw_start_y;
	x1 = this->m_linedraw_end_x;
	y1 = this->m_linedraw_end_y;
	flag = x1 - x0;
	flag *= (y1 - y0);
	if(flag >= 0)
		this->LineDrawPlus();
	else
		this->LineDrawNeg();
}

void CGe::Process(char *src, char *dst)
{
	int size;

	if(this->m_bitblt_colordepth == GE_RGB24)
		size = this->m_width * this->m_height * 3;
	else
		size = this->m_width * this->m_height * 2;
	this->SetDestBuf(dst, size);
	this->SetSrcBuf(src, size);
	this->StartBitblt();
}
