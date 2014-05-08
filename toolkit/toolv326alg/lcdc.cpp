
#include "internal.h"

const int gLcdcDither8_4[][4] = 
{
	{ 1, 9, 3, 11 },
	{ 13, 5, 15, 7 },
	{ 4, 12, 2, 10 },
	{ 16, 8, 14, 6 }
};

const int gLcdcDither8_5[][4] = 
{
	{ 1, 5, 2, 6 },
	{ 7, 3, 8, 4 },
	{ 2, 6, 1, 5 },
	{ 8, 4, 7, 3 }
};

const int gLcdcDither8_6[][4] = 
{
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 },
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 }
};

//=============== class CFrameBuf	=================

CFrameBuf::CFrameBuf(int width, int height, int format)
{
	this->SetSize(width, height, format);
	this->SetRotation(ROTATION_0);
	this->m_line = 0;
}

CFrameBuf::~CFrameBuf()
{

}

void CFrameBuf::SetRotation(int rot)
{
	this->m_rot = rot;
	switch(rot)
	{
	case ROTATION_0:
	case ROTATION_180:
	case ROTATION_MIRROR:
	case ROTATION_MIRROR_180:
		this->m_rotwidth = this->m_width;
		this->m_rotheight = this->m_height;
		break;
	case ROTATION_90:
	case ROTATION_270:
	case ROTATION_MIRROR_90:
	case ROTATION_MIRROR_270:
		this->m_rotwidth = this->m_height;
		this->m_rotheight = this->m_width;
		break;
	default:
		break;
	}
}

void CFrameBuf::SetFormat(int format)
{
	this->m_format = format;
	switch(format)
	{
	case YUV422_YUYV:
		this->m_unit = 2;
		break;
	case RGB24:
		this->m_unit = 3;
		break;
	default:
		this->m_unit = 1;
		break;
	}
}

void CFrameBuf::SetSize(int width, int height, int format)
{
	this->SetFormat(format);
	CBufferBase::SetSize(width*height, this->m_unit);
	this->m_width = width;
	this->m_height = height;
}

int CFrameBuf::GetRotWidth(void)
{
	return this->m_rotwidth;
}

int CFrameBuf::GetRotHeight(void)
{
	return this->m_rotheight;
}

int CFrameBuf::GetWidth(void)
{
	return this->m_width;
}

int CFrameBuf::GetHeight(void)
{
	return this->m_height;
}

int CFrameBuf::IsFull(void)
{
	if(this->m_line >= this->m_height)
		return 1;
	else
		return 0;
}

int CFrameBuf::Write(char *buf, int len)
{
	if(len <= 0)
		return 0;
	len = (len > this->m_width * this->m_unit) ? this->m_width * this->m_unit : len;
	if(this->m_line == this->m_height)
		this->m_line = 0;
	memcpy(this->m_buf + this->m_line*this->m_width*this->m_unit, buf, len);
	this->m_line ++;
	return len;
}

int CFrameBuf::Read(char *buf, int line, int width)
{
	int rotx, roty = line;
	int pos = 0;

	if(!this->IsFull())
		return 0;
	for(rotx = 0; rotx < width; rotx++)
		pos += this->ReadPoint(buf+pos, rotx, roty);

	return rotx;
}

int CFrameBuf::ReadPoint(char *buf, int rotx, int roty)
{
	int x, y;
	int i = 0;

	if(!this->IsFull())
		return 0;
	switch(this->m_rot)
	{
	case ROTATION_90:
		x = this->m_width - 1 - roty;
		y = rotx;
		break;
	case ROTATION_180:
		x = this->m_width - 1 - rotx;
		y = this->m_height - 1 - roty;
		break;
	case ROTATION_270:
		x = roty;
		y = this->m_height - 1 - rotx;
		break;
	case ROTATION_MIRROR:
		x = rotx;
		y = this->m_height - 1 - roty;
		break;
	case ROTATION_MIRROR_90:
		x = roty;
		y = rotx;
		break;
	case ROTATION_MIRROR_180:
		x = this->m_width - 1 - rotx;
		y = roty;
		break;
	case ROTATION_MIRROR_270:
		x = this->m_width - 1 - roty;
		y = this->m_height - 1 - rotx;
		break;
	default:
		x = rotx;
		y = roty;
		break;
	}
	if(this->m_format == YUV422_YUYV)
	{
		buf[i++] = this->m_buf[y*this->m_width*2+x*2];
		x = (x>>1)<<1;
		if(rotx%2)		//v
			buf[i++] = this->m_buf[y*this->m_width*2+x*2+3];
		else
			buf[i++] = this->m_buf[y*this->m_width*2+x*2+1];
	}
	else
	{
		for(i = 0; i < this->m_unit; i++)
			buf[i] = this->m_buf[y*this->m_width*this->m_unit+x*this->m_unit+i];
	}
	return i;
}

int CFrameBuf::Process(char *src, char *dst)
{
	int i;

	for(i = 0; i < this->m_height; i++)
		this->Write(src+i*this->m_width*this->m_unit, this->m_width*this->m_unit);

	for(i = 0; i < this->m_rotheight; i++)
		this->Read(dst+i*this->m_rotwidth*this->m_unit, i, this->m_rotwidth);
	
	return this->m_rotwidth*this->m_rotheight*this->m_unit;
}


//=============== class CLcdcVBufControl =================

CLcdcVBufControl::CLcdcVBufControl(int width, int height, int memwidth, int memheight, int pointx, int pointy, int lcdcx, int lcdcy)
{
	this->SetSize(width, height);
	this->SetMemSize(memwidth, memheight);
	this->SetPoint(pointx, pointy);
	this->SetLcdcPoint(lcdcx, lcdcy);
	this->m_line = 0;
}

CLcdcVBufControl::~CLcdcVBufControl(void)
{
}

void CLcdcVBufControl::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	switch(this->m_rot)
	{
	case ROTATION_0:
	case ROTATION_180:
	case ROTATION_MIRROR:
	case ROTATION_MIRROR_180:
		this->m_framebuf.SetSize(width, height, YUV422_YUYV);
		break;
	case ROTATION_90:
	case ROTATION_270:
	case ROTATION_MIRROR_90:
	case ROTATION_MIRROR_270:
		this->m_framebuf.SetSize(height, width, YUV422_YUYV);
		break;
	default:
		break;
	}
}

void CLcdcVBufControl::SetMemSize(int width, int height)
{
	this->m_memwidth = width;
	this->m_memheight = height;
}

void CLcdcVBufControl::SetPoint(int x, int y)
{
	this->m_pointx = x;
	this->m_pointy = y;
}

void CLcdcVBufControl::SetLcdcPoint(int x, int y)
{
	this->m_lcdcpointx = x;
	this->m_lcdcpointy = y;
}

void CLcdcVBufControl::SetRotation(int rot)
{
	this->m_rot = rot;
	this->m_framebuf.SetRotation(rot);
}

void CLcdcVBufControl::SetVBufMode(int mode)
{
	this->m_vbufmode = mode;
}

int CLcdcVBufControl::GetWidth(void)
{
	return this->m_width;
}

int CLcdcVBufControl::GetHeight(void)
{
	return this->m_height;
}

int CLcdcVBufControl::GetLcdcX(int x)
{
	return x+this->m_lcdcpointx;
}

int CLcdcVBufControl::GetLcdcY(int y)
{
	return y+this->m_lcdcpointy;
}

int CLcdcVBufControl::GetMemWidth(void)
{
	return this->m_memwidth;
}

int CLcdcVBufControl::GetMemHeight(void)
{
	return this->m_memheight;
}

/*	write buf with filter */
int CLcdcVBufControl::Write(char *buf, int len)
{
	int ret;

	if(len <= 0)
		return 0;
	len = (len > this->m_memwidth*2) ? this->m_memwidth*2 : len;
	if(this->m_line < this->m_pointy)
		return len;
	ret = this->m_framebuf.Write(buf + this->m_pointx*2, len);
	if(ret == 0)
		return ret;
	this->m_line ++;

	return len;
}

int CLcdcVBufControl::Read(char *buf, int line, int width)
{
	return this->m_framebuf.Read(buf, line, width);
}

int CLcdcVBufControl::ReadPoint(char *buf, int x, int y)
{
	return this->m_framebuf.ReadPoint(buf, x, y);
}

int CLcdcVBufControl::Process(char *src, char *dst)
{
	int i;
	int frameWidth, frameHeight;
	
	frameWidth = this->m_framebuf.GetWidth();
	frameHeight = this->m_framebuf.GetHeight();

	for(i = 0; i < this->m_memheight; i++)
		this->Write(src+i*this->m_memwidth*2, frameWidth*2);

	for(i = 0; i < this->m_height; i++)
		this->Read(dst+i*this->m_width*2, i, this->m_width);
	
	return this->m_width * this->m_height * 2;
}

//=============== class CLcdcGBuf =================

CLcdcGBuf::CLcdcGBuf(int width, int height, int memwidth, int pointx, int pointy, int lcdcx, int lcdcy)
{
	this->InitGBuf();
	this->SetSize(width, height);
	this->SetMemWidth(memwidth);
	this->SetPoint(pointx, pointy);
	this->SetLcdcPoint(lcdcx, lcdcy);
	this->SetFormat(DIRECT_RGB888);
}

void CLcdcGBuf::InitGBuf(void)
{	
	this->m_gbuf.SetSize(GE_GBUF_SIZE);	
	memset(this->m_gbuf.m_buf, 0, GE_GBUF_SIZE);
	this->SetBAddr(0);
}

CLcdcGBuf::~CLcdcGBuf(void)
{

}

void CLcdcGBuf::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

void CLcdcGBuf::SetLcdcPoint(int x, int y)
{
	this->m_lcdcpointx = x;
	this->m_lcdcpointy = y;
}

void CLcdcGBuf::SetMemWidth(int width)
{
	this->m_memwidth = width;
}

void CLcdcGBuf::SetPoint(int x, int y)
{
	this->m_pointx = x;
	this->m_pointy = y;
}

void CLcdcGBuf::SetBRefreshPoint(int x, int y)
{
	this->m_refpointx = x;
	this->m_refpointy = y;
}

void CLcdcGBuf::SetBRefreshSize(int width, int height)
{
	this->m_refwidth = width;
	this->m_refheight = height;
}

void CLcdcGBuf::SetBAddr(int offset)
{
	this->m_baddr = offset;
}

void CLcdcGBuf::SetRotation(int rot)
{
	this->m_rot = rot;
}

void CLcdcGBuf::SetFormat(int format)
{
	this->m_format = format;
}

int CLcdcGBuf::GetWidth(void)
{
	return this->m_width;
}

int CLcdcGBuf::GetHeight(void)
{
	return this->m_height;
}

int CLcdcGBuf::SetGBuf(char *buf, int size)
{
	this->m_gbuf.SetSize(size);
	size = this->m_gbuf.Write(buf, size);
	return size;
}

void CLcdcGBuf::SetMemSize(int size)
{
	this->m_memsize = size;
}

void CLcdcGBuf::Update(void)
{
	//should be add
}

int CLcdcGBuf::ReadPoint(char *buf, int lcdx, int lcdy)
{
	int x, y;
	int rotx, roty;
	int i = 0;

	rotx = lcdx - this->m_lcdcpointx;
	roty = lcdy - this->m_lcdcpointy;
	if((rotx < 0) || (roty < 0) || (rotx >= this->m_width) || (roty >= this->m_height))
		return 0;

	switch(this->m_rot)
	{
	case ROTATION_90:
		x = this->m_height - 1 - roty;
		y = rotx;
		break;
	case ROTATION_180:
		x = this->m_width - 1 - rotx;
		y = this->m_height - 1 - roty;
		break;
	case ROTATION_270:
		x = roty;
		y = this->m_width - 1 - rotx;
		break;
	case ROTATION_MIRROR:
		x = rotx;
		y = this->m_height - 1 - roty;
		break;
	case ROTATION_MIRROR_90:
		x = roty;
		y = rotx;
		break;
	case ROTATION_MIRROR_180:
		x = this->m_width - 1 - rotx;
		y = roty;
		break;
	case ROTATION_MIRROR_270:
		x = this->m_height - 1 - roty;
		y = this->m_width - 1 - rotx;
		break;
	default:
		x = rotx;
		y = roty;
		break;
	}
	x += this->m_pointx;
	y += this->m_pointy;

	switch(this->m_format)
	{
	case DIRECT_RGB888:
		buf[0] = this->m_gbuf.m_buf[y*this->m_memwidth*3+x*3];
		buf[1] = this->m_gbuf.m_buf[y*this->m_memwidth*3+x*3+1];
		buf[2] = this->m_gbuf.m_buf[y*this->m_memwidth*3+x*3+2];
		i = 3;
		break;
	default:
		break;
	}
	return i;
}

int CLcdcGBuf::Process(char *src, char *dst)
{
	int i, j;

	this->SetGBuf(src, this->m_memsize);
	for(i = 0; i < this->m_height; i++)
		for(j = 0; j < this->m_width; j++)
			this->ReadPoint(dst+i*this->m_width*3+j*3, j, i);

	return this->m_width*this->m_height*3;
}

//=============== class CLcdcGBufControl =================

CLcdcGBufControl::CLcdcGBufControl(void)
{
}

CLcdcGBufControl::~CLcdcGBufControl(void)
{
}

int CLcdcGBufControl::ReadPoint(char *buf, int lcdx, int lcdy, int *sel)
{
	int len;
	
	*sel = 0;
	len = this->m_gbuf[0].ReadPoint(buf, lcdx, lcdy);
	if(len == 0)
	{
		*sel = 1;
		len = this->m_gbuf[1].ReadPoint(buf, lcdx, lcdy);
	}
	return len;
}

void CLcdcGBufControl::Update(int sel)
{
	if(sel)
		this->m_gbuf[1].Update();
	else
		this->m_gbuf[0].Update();
}

//=============== class CLcdcModule =================

CLcdc::CLcdc(void)
{
	int rgamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};
	int ggamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};
	int bgamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};

	this->m_diteren = 0;
	this->m_dithersel = 0;
	this->m_gammaen = 0;
	this->m_overlaymode0 = 0;
	this->m_overlaymode1 = 0;
	this->m_overlayen0 = 0;
	this->m_overlayen1 = 0;
	this->m_alphablenden0 = 0;
	this->m_alphablenden1 = 0;
	this->m_priority0 = 0;
	this->m_priority1 = 0;
	this->m_overlapmode = 0;
	this->m_bgpointx = 0;
	this->m_bgpointy = 0;
	this->m_bgwidth = 0;
	this->m_bgheight = 0;
	this->m_keycolor0 = 0;
	this->m_keycolor1 = 0;
	this->m_blendratio0 = 0;
	this->m_blendratio1 = 0;
	this->m_bgdata0 = 0;
	this->m_bgdata1 = 0;
	this->m_line = 0;
	this->SetRGamma(rgamma);
	this->SetGGamma(ggamma);
	this->SetBGamma(bgamma);
}

CLcdc::~CLcdc(void)
{
}

//A layter property
void CLcdc::SetALcdcPoint(int x, int y)
{
	this->m_vbuf.SetLcdcPoint(x, y);
}

void CLcdc::SetAMemSize(int width, int height)
{
	this->m_vbuf.SetMemSize(width, height);
}

void CLcdc::SetAPoint(int x, int y)
{
	this->m_vbuf.SetPoint(x, y);
}

void CLcdc::SetARotation(int rot)
{
	this->m_vbuf.SetRotation(rot);
}

void CLcdc::SetASize(int width, int height)
{
	this->m_vbuf.SetSize(width, height);
}

void CLcdc::SetBAddr(int offset, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetBAddr(offset);
}

void CLcdc::SetBFormat(int format, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetFormat(format);
}

void CLcdc::SetBLcdcPoint(int x, int y, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetLcdcPoint(x, y);
}

void CLcdc::SetBMemSize(int size, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetMemSize(size);
}

void CLcdc::SetBMemWidth(int width, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetMemWidth(width);
}

void CLcdc::SetBPoint(int x, int y, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetPoint(x, y);
}

void CLcdc::SetBRefreshPoint(int x, int y, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetBRefreshPoint(x, y);
}

void CLcdc::SetBRefreshSize(int width, int height, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetBRefreshSize(width, height);
}

void CLcdc::SetBRotation(int rot, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetRotation(rot);
}

void CLcdc::SetBSize(int width, int height, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetSize(width, height);
}

void CLcdc::SetGBuf(char *buf, int size, int sel)
{
	this->m_gbufctrl.m_gbuf[sel].SetGBuf(buf, size);
}

void CLcdc::UpdateB(int sel)
{
	this->m_gbufctrl.m_gbuf[sel].Update();
}

void CLcdc::SetDitherEn(int en)
{
	this->m_diteren = en;
}

void CLcdc::SetDitherSel(int sel)
{
	this->m_dithersel = sel;
}

void CLcdc::SetGammaEn(int en)
{
	this->m_gammaen = en;
}

void CLcdc::SetOverlapMode(int mode)
{
	this->m_overlapmode = mode;
}

void CLcdc::SetOverlayEn(int en, int sel)
{
	if(sel)
		this->m_overlayen1 = en;
	else
		this->m_overlayen0 = en;
}

void CLcdc::SetOverlayMode(int mode, int sel)
{
	if(sel)
		this->m_overlaymode1 = mode;
	else
		this->m_overlaymode0 = mode;
}

void CLcdc::SetAlphaBlendingEn(int en, int sel)
{
	if(sel)
		this->m_alphablenden1 = en;
	else
		this->m_alphablenden0 = en;
}

void CLcdc::SetPriority(int pri, int sel)
{
	if(sel)
		this->m_priority1 = pri;
	else
		this->m_priority0 = pri;
}

void CLcdc::SetVBufMode(int mode)
{
	this->m_vbuf.SetVBufMode(mode);
}

void CLcdc::SetBackgroundPoint(int x, int y)
{
	this->m_bgpointx = x;
	this->m_bgpointy = y;
}

void CLcdc::SetBackgroundSize(int width, int height)
{
	this->m_bgwidth = width;
	this->m_bgheight = height;
}

void CLcdc::SetKeyColor(int color, int sel)
{
	if(sel)
		this->m_keycolor1 = color;
	else
		this->m_keycolor0 = color;
}

void CLcdc::SetBlendRatio(int ratio, int sel)
{
	if(sel)
		this->m_blendratio1 = ratio;
	else
		this->m_blendratio0 = ratio;
}

void CLcdc::SetBackgroundData(int data, int sel)
{
	if(sel)
		this->m_bgdata1 = data;
	else
		this->m_bgdata0 = data;
}

void CLcdc::SetColorPalette(char *buf, int sel)
{
	if(sel)
		memcpy(this->m_colorpalette1, buf, LCDC_COLORPALETTE_LEN);
	else
		memcpy(this->m_colorpalette0, buf, LCDC_COLORPALETTE_LEN);
}

void CLcdc::SetRGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_rgamma[i] = p[i]&0xff;
}

void CLcdc::SetGGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_ggamma[i] = p[i]&0xff;
}

void CLcdc::SetBGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_bgamma[i] = p[i]&0xff;
}

int CLcdc::GetALayerSize(void)
{
	return this->m_vbuf.GetWidth() * this->m_vbuf.GetHeight() * 2;
}

int CLcdc::GetAMemSize(void)
{
	return this->m_vbuf.GetMemWidth() * this->m_vbuf.GetMemHeight() * 2;
}

int CLcdc::Yuv2Rgb(char *yuv, char *rgb)
{
	int r, g ,b;
	int y, u, v;
//	R = Y + 359*V/256 ¨C 179
//	G = Y + 135 - (88*U + 183*V)/256
//	B = Y + 454*U/256 ¨C 227
	y = (int)yuv[0]&0xff;
	u = (int)yuv[1]&0xff;
	v = (int)yuv[3]&0xff;
	r = y + ((359*v)>>8) - 179;
	g = y + 135 - ((88*u + 183*v)>>8);
	b = y + ((454*u)>>8) - 227;
	rgb[2] = (char)((r < 0) ? 0 : (r > 255 ? 255 : r));
	rgb[1] = (char)((g < 0) ? 0 : (g > 255 ? 255 : g));
	rgb[0] = (char)((b < 0) ? 0 : (b > 255 ? 255 : b));
	y = (int)yuv[2]&0xff;
	r = y + ((359*v)>>8) - 179;
	g = y + 135 - ((88*u + 183*v)>>8);
	b = y + ((454*u)>>8) - 227;
	rgb[5] = (char)((r < 0) ? 0 : (r > 255 ? 255 : r));
	rgb[4] = (char)((g < 0) ? 0 : (g > 255 ? 255 : g));
	rgb[3] = (char)((b < 0) ? 0 : (b > 255 ? 255 : b));

	return 6;
}

int CLcdc::AlphaBlending(char *al, char *bl, int len, int sel)
{
	int i;
	int alphaRatio;

	if(sel)	//
		alphaRatio = this->m_blendratio1;
	else
		alphaRatio = this->m_blendratio0;
	for(i = 0; i < len; i++)
		al[i] = (char)((al[i]*alphaRatio + bl[i]*(256-alphaRatio))>>8);
	
	return i;
}

int CLcdc::Overlay(char *al, char *bl, int len, int sel)
{
	int i;
	int olMode, keyColor, priority ;
	char *pFront, *pBack;

	if(sel)
	{
		olMode = this->m_overlaymode1;
		keyColor = this->m_keycolor1;
		priority = this->m_priority1;
	}
	else
	{
		olMode = this->m_overlaymode0;
		keyColor = this->m_keycolor0;
		priority = this->m_priority0;
	}
	if(priority)
	{
		pFront = bl;
		pBack = al;
	}
	else
	{
		pFront = al;
		pBack = bl;
	}
	for(i = 0; i < len; i+=3)
	{
		if((pFront[i] == (char)(keyColor&0xff)) && (pFront[i+1] == (char)((keyColor>>8)&0xff)) && (pFront[i+2] == (char)((keyColor>>16)&0xff)))
		{
			switch(olMode)
			{
			case OVERLAY_TRANSPARENT:
				al[i] = pBack[i];
				al[i+1] = pBack[i+1];
				al[i+2] = pBack[i+2];
				break;
			case OVERLAY_AND:
				al[i] = pFront[i] & pBack[i];
				al[i+1] = pFront[i+1] & pBack[i+1];
				al[i+2] = pFront[i+2] & pBack[i+1];
				break;
			case OVERLAY_OR:
				al[i] = pFront[i] | pBack[i];
				al[i+1] = pFront[i+1] | pBack[i+1];
				al[i+2] = pFront[i+2] | pBack[i+1];
				break;
			case OVERLAY_INVERT:
				al[i] = !pBack[i];
				al[i+1] = !pBack[i+1];
				al[i+2] = !pBack[i+2];
				break;
			default:
				break;
			}
		}
		else
		{
			al[i] = pFront[i];
			al[i+1] = pFront[i+1];
			al[i+2] = pFront[i+2];
			break;
		}
	}

	return i;
}

int CLcdc::DispLayMux(char *al, char *bl, int len, int sel)
{
	int alphaEn, overlayEn;
	int ret = 0;

	alphaEn = (sel == 0) ? this->m_alphablenden0 : this->m_alphablenden1;
	overlayEn = (sel == 0) ? this->m_overlayen0 : this->m_overlayen1;
	if(alphaEn && (bl != NULL))
		ret = this->AlphaBlending(al, bl, len, sel);
	else if(overlayEn && (bl != NULL))
		ret = this->Overlay(al, bl, len, sel);
	else
		ret = len;

	return ret;
}

int CLcdc::GetGammaValue(char *buf, int rgb)
{
	int	x1;
	int	y1, y2;
	int	flag;
	int	result;
	int val;

	val = ((int)(*buf))&0xff;
	//val = val&0xff;
	flag = val >> 4;
	//x1 = flag << 4;
	x1 = val&0xf;

	if(rgb == 0)	//r
	{
		y1 = this->m_rgamma[flag]&0xff;
		y2 = this->m_rgamma[flag+1]&0xff;
	}
	else if(rgb == 1)	//g
	{
		y1 = this->m_ggamma[flag]&0xff;
		y2 = this->m_ggamma[flag+1]&0xff;
	}
	else		//b
	{
		y1 = this->m_bgamma[flag]&0xff;
		y2 = this->m_bgamma[flag+1]&0xff;
	}

	result = y2 - y1;
	result *= x1;
	result >>= 6;
	result += y1;

	return result > 255 ? 255 : result;
}

void CLcdc::GammaCorrect(char *buf)
{
	buf[0] = (char)this->GetGammaValue(buf, 0);
	buf[1] = (char)this->GetGammaValue(buf+1, 1);
	buf[2] = (char)this->GetGammaValue(buf+2, 2);
}

void CLcdc::Dither(char *buf, int x)
{
	buf = buf;
	x = x;
}

int CLcdc::WriteGBuf(char *buf ,int len, int sel)
{
	len = this->m_gbufctrl.m_gbuf[sel].SetGBuf(buf, len);
	return len;
}

int CLcdc::Write(char *buf, int len)
{
	len = this->m_vbuf.Write(buf, len);
	return len;
}

int CLcdc::Read(char *buf, int len)
{
	int i;
	int width;
	char yuv[4], al[6], bl[3];
	int ret = 0, sel = 0;
	int lcdx, lcdy;

	if(len <= 0)
		return 0;
	width = this->m_vbuf.GetWidth();
	len = (len > width*3) ? width*3 : len;
	if(this->m_line == this->m_vbuf.GetHeight())
		this->m_line = 0;
	for(i = 0; i < width; i+=2)
	{
		ret = this->m_vbuf.ReadPoint(yuv, i, this->m_line);
		ret += this->m_vbuf.ReadPoint(yuv+2, i+1, this->m_line);
		if(ret == 0)
			break;
		this->Yuv2Rgb(yuv, al);
		lcdx = this->m_vbuf.GetLcdcX(i);
		lcdy = this->m_vbuf.GetLcdcY(this->m_line);
		ret = this->m_gbufctrl.ReadPoint(bl, lcdx, lcdy, &sel);
		//should add color palette
		if(ret != 0)
			this->DispLayMux(al, bl, 3, sel);
		if(this->m_gammaen)
			this->GammaCorrect(al);
//		if(this->m_diteren)
		ret = this->m_gbufctrl.ReadPoint(bl, lcdx+1, lcdy, &sel);
		if(ret != 0)
			this->DispLayMux(al+3, bl, 3, sel);
		if(this->m_gammaen)
			this->GammaCorrect(al+3);
		memcpy(buf+i*3, al, 6);
	}
	if(i)
		this->m_line ++;
	return i*3;
}

int CLcdc::Process(char *src, char *dst)
{
	int size, pos = 0, pos1 = 0, size1, width = 0;
	int temp;

	size = this->GetAMemSize();
//	size = this->GetALayerSize();
	size1 = this->GetALayerSize() * 3 / 2;
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
