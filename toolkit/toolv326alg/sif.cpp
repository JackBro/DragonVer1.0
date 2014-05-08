
#include "internal.h"

//==================== class C326SifModule ===================

C326SifModule::C326SifModule()
{
}

//=============== class C326Rgb2Yiq =================

C326Rgb2Yiq::C326Rgb2Yiq(int width, int height, int unit)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_unit   = unit;

	this->m_ygstart[0] = 0x00;
	this->m_ygstart[1] = 0x15;
	this->m_ygstart[2] = 0x20;
	this->m_ygstart[3] = 0x30;
	this->m_ygstart[4] = 0x49;
	this->m_ygstart[5] = 0x5e;
	this->m_ygstart[6] = 0x6f;
	this->m_ygstart[7] = 0xa9;
	this->m_ygstart[8] = 0xd7;

	this->m_ygslope[0] = 0x30;
	this->m_ygslope[1] = 0x2b;
	this->m_ygslope[2] = 0x22;
	this->m_ygslope[3] = 0x1a;
	this->m_ygslope[4] = 0x14;
	this->m_ygslope[5] = 0x11;
	this->m_ygslope[6] = 0x0e;
	this->m_ygslope[7] = 0x0b;
	this->m_ygslope[8] = 0x0a;
	
	this->m_gamma_en = TRUE;
	this->m_rgbbuf.SetSize((3 * this->m_width), 0, unit);
}

C326Rgb2Yiq::~C326Rgb2Yiq(void)
{
}

void C326Rgb2Yiq::EnableGamma(int flg)
{
	this->m_gamma_en = flg;
}

void C326Rgb2Yiq::SetYStart(int *pval)
{
	if(pval)
		memcpy(this->m_ygstart, pval, (9 * sizeof(int)));
}

void C326Rgb2Yiq::SetYSlope(int *pval)
{
	if(pval)
		memcpy(this->m_ygslope, pval, (9 * sizeof(int)));
}

int C326Rgb2Yiq::Write(char *buf, int len)
{
	int pos = 0;

	this->m_rgbbuf.ClearPoint();
	pos = this->m_rgbbuf.Write(buf, len);
	if(pos != 0)
		this->m_vpos ++;
	return pos;
}

//	Y = (77R + 150G + 29B)/256
void C326Rgb2Yiq::Ycalc(void)
{
	int val[3], yval = 0;

	val[0] = (this->m_rgb[0] * 77) >> 8;
	val[1] = (this->m_rgb[1] * 150) >> 8;
	val[2] = (this->m_rgb[2] * 29) >> 8;
	yval = val[0] + val[1] + val[2];
	this->m_yiq[0] = (yval > 0xff) ? 0xff : (yval & 0xff);
}

void C326Rgb2Yiq::Ygamma(void)
{
	int ystart, yparam, yadapt, ymul, axy, youtw;

	int y_in   = this->m_yiq[0];
	int ysbblk = ( ((y_in >> 6) & 0x03) == 0x00 ) ? 1 : 0;
	int ybsel0 = ( ((y_in >> 2) & 0x0f) == 0x00 ) ? ysbblk : 0;
	int ybsel1 = ( ((y_in >> 2) & 0x0f) == 0x01 ) ? ysbblk : 0;
	int ybsel2 = ( ((y_in >> 3) & 0x07) == 0x01 ) ? ysbblk : 0;
	int ybsel3 = ( ((y_in >> 4) & 0x03) == 0x01 ) ? ysbblk : 0;
	int ybsel4 = ( ((y_in >> 4) & 0x03) == 0x02 ) ? ysbblk : 0;
	int ybsel5 = ( ((y_in >> 4) & 0x03) == 0x03 ) ? ysbblk : 0;
	int ybsel6 = ( ((y_in >> 6) & 0x03) == 0x01 ) ? 1 : 0;
	int ybsel7 = ( ((y_in >> 6) & 0x03) == 0x02 ) ? 1 : 0;

	ystart =	ybsel0 ? this->m_ygstart[0] :
				ybsel1 ? this->m_ygstart[1] :
				ybsel2 ? this->m_ygstart[2] :
				ybsel3 ? this->m_ygstart[3] :
				ybsel4 ? this->m_ygstart[4] :
				ybsel5 ? this->m_ygstart[5] :
				ybsel6 ? this->m_ygstart[6] :
				ybsel7 ? this->m_ygstart[7] : this->m_ygstart[8];
	ystart &= 0xff;

	yparam =	ybsel0 ? this->m_ygslope[0] :
				ybsel1 ? this->m_ygslope[1] :
				ybsel2 ? this->m_ygslope[2] :
				ybsel3 ? this->m_ygslope[3] :
				ybsel4 ? this->m_ygslope[4] :
				ybsel5 ? this->m_ygslope[5] :
				ybsel6 ? this->m_ygslope[6] :
				ybsel7 ? this->m_ygslope[7] : this->m_ygslope[8];
	yparam &= 0x7f;

	yadapt =	!ysbblk ? (y_in & 0x3f) :
				ybsel2  ? (y_in & 0x07) :
	  (ybsel1 | ybsel0) ? (y_in & 0x03) :(y_in & 0x0f);
	yadapt &= 0x3f;

	ymul = 0x1fff & (yparam * yadapt);
	axy = ybsel0 ? ((ymul >> 3) & 0x3ff) : ((ymul >> 4) & 0x1ff);
	youtw = 0x3ff & (ystart + axy);
	this->m_yiq[0] = (youtw & 0x300) ? 0xff : (youtw & 0xff);
}

//	I = (153R - 70G - 82B) / 256
void C326Rgb2Yiq::Icalc(void)
{
	int ival = 0, ppr = 0, png = 0, pnb = 0;

	ppr = (this->m_rgb[0] * 153) >> 7;
	png = (this->m_rgb[1] * 70) >> 7;
	pnb = (this->m_rgb[2] * 82) >> 7;

	ival = ppr - png - pnb;
	if(ival < 0)
		ival = abs(ival);
	this->m_yiq[1] = 0xff & (ival >> 1);
}

//	Q = (54R - 134G + 80B) / 256
void C326Rgb2Yiq::Qcalc(void)
{
	int qval = 0, ppr = 0, png = 0, pnb = 0;

	ppr = (this->m_rgb[0] * 54) >> 7;
	png = (this->m_rgb[1] * 134) >> 7;
	pnb = (this->m_rgb[2] * 80) >> 7;

	qval = ppr + pnb - png;
	if(qval < 0)
		qval = abs(qval);
	this->m_yiq[2] = 0xff & (qval >> 1);
}

int C326Rgb2Yiq::Read(char *buf, int len)
{
	char *dst = buf;
	int i = 0, offset = 0;
	CBaseConvert tools;

	if(len == 0)
		return 0;
	for(this->m_hpos = 0; this->m_hpos < this->m_width; this->m_hpos++, offset += 3)
	{
		for(i = 0; i < 3; i++)
			this->m_rgb[i] = 0xff & (this->m_rgbbuf.Read(offset + i) >> 2);
		this->Ycalc();
		if(this->m_gamma_en == TRUE)
			this->Ygamma();
		this->Icalc();
		this->Qcalc();
		for(i = 0; i < 3; i++, dst ++)
			tools.setvalue(dst, this->m_yiq[i]);
	}
	return offset;
}

int C326Rgb2Yiq::Process(char *src, char *dst)
{
	int len = 0, size = 0, size1 = 0, pos = 0, pos1 = 0;

	size1 = 3 * this->m_width * this->m_height;
	size = size1 << 1;
	while(size)
	{
		len = this->Write(src + pos, size);
		size -= len;
		pos += len;
		len = this->Read(dst + pos1, size1);
		size1 -= len;
		pos1 += len;
	}
	return pos1;
}

//=============== class C326HsvAdj =================

C326HsvAdj::C326HsvAdj(int width, int height)
{
	this->m_width  = width;
	this->m_height = height;

	this->m_rgbbuf.SetSize((3 * this->m_width), 0);
}

C326HsvAdj::~C326HsvAdj(void)
{
}

int C326HsvAdj::Write(char *buf, int len)
{
	int pos = 0;

	this->m_rgbbuf.ClearPoint();
	pos = this->m_rgbbuf.Write(buf, len);
	if(pos != 0)
		this->m_vpos ++;
	return pos;
}

void C326HsvAdj::Rgb2Hsv(void)
{
	this->m_vval = (this->m_rgb[0] > this->m_rgb[1]) ?	(this->m_rgb[0] > this->m_rgb[2] ? this->m_rgb[0] : this->m_rgb[2]) :
														(this->m_rgb[1] > this->m_rgb[2] ? this->m_rgb[1] : this->m_rgb[2]);
	this->m_mval = (this->m_rgb[0] < this->m_rgb[1]) ?	(this->m_rgb[0] < this->m_rgb[2] ? this->m_rgb[0] : this->m_rgb[2]) :
														(this->m_rgb[1] < this->m_rgb[2] ? this->m_rgb[1] : this->m_rgb[2]);
	this->m_vsval = this->m_vval - this->m_mval;
	if(this->m_rgb[0] == this->m_vval)
	{
		if(this->m_rgb[1] >= this->m_rgb[2])
		{
			this->m_hint = 0;
			this->m_hfract = this->m_rgb[1] - this->m_rgb[2];
		}
		else
		{
			this->m_hint = 5;
			this->m_hfract = this->m_vval - this->m_mval + this->m_rgb[1] - this->m_rgb[2];
		}
	}
	else if(this->m_rgb[1] == this->m_vval)
	{
		if(this->m_rgb[2] >= this->m_rgb[0])
		{
			this->m_hint = 2;
			this->m_hfract = this->m_rgb[2] - this->m_rgb[0];
		}
		else
		{
			this->m_hint = 1;
			this->m_hfract = this->m_vval - this->m_mval + this->m_rgb[2] - this->m_rgb[0];
		}
	}
	else
	{
		if(this->m_rgb[0] >= this->m_rgb[1])
		{
			this->m_hint = 4;
			this->m_hfract = this->m_rgb[0] - this->m_rgb[1];
		}
		else
		{
			this->m_hint = 3;
			this->m_hfract = this->m_vval - this->m_mval + this->m_rgb[0] - this->m_rgb[1];
		}
	}
}

void C326HsvAdj::HsvAdj(void)
{
	int temp = 0;
	int vs_val = this->m_vval - this->m_mval;
	const int dlt_hue_div60 = 0x05, dlt_hue_mod60 = 0x20, saturation_rate = 0x20;

	this->m_hint += dlt_hue_div60;
	this->m_hfract = ((this->m_hfract << 6) + this->m_vsval * int((dlt_hue_mod60 << 6) / 60)) >> 6;
	if(this->m_hfract > vs_val)
	{
		this->m_hint ++;
		this->m_hfract -= vs_val;
	}
	if(this->m_hint > 5)
		this->m_hint -= 6;

	temp = saturation_rate * vs_val;
	if(temp > (this->m_vval << 4))
	{
		this->m_hfract = ((this->m_hfract * this->m_vval / vs_val) >> 2) << 2;
		this->m_vsval = this->m_vval;
	}
	else
	{
		this->m_vsval  = (vs_val * saturation_rate) >> 4;
		this->m_hfract = (this->m_hfract * saturation_rate) >> 4;
	}
	if(this->m_vsval < 0)
		this->m_vsval = 0;
}

void C326HsvAdj::Hsv2Rgb(void)
{
	int i = 0;

	if(this->m_vsval == 0)
	{
		for(i = 0; i < 3; i++)
			this->m_rgb[i] = this->m_vval;
	}
	else
	{
		switch(this->m_hint)
		{
		case 0 :
			this->m_rgb[0] = this->m_vval;
			this->m_rgb[1] = this->m_vval - this->m_vsval + this->m_hfract;
			this->m_rgb[2] = this->m_vval - this->m_vsval;
			break;
		case 1 :
			this->m_rgb[0] = this->m_vval - this->m_hfract;
			this->m_rgb[1] = this->m_vval;
			this->m_rgb[2] = this->m_vval - this->m_vsval;
			break;
		case 2 :
			this->m_rgb[0] = this->m_vval - this->m_vsval;
			this->m_rgb[1] = this->m_vval;
			this->m_rgb[2] = this->m_vval - this->m_vsval + this->m_hfract;
			break;
		case 3 :
			this->m_rgb[0] = this->m_vval - this->m_vsval;
			this->m_rgb[1] = this->m_vval - this->m_hfract;
			this->m_rgb[2] = this->m_vval;
			break;
		case 4 :
			this->m_rgb[0] = this->m_vval - this->m_vsval + this->m_hfract;
			this->m_rgb[1] = this->m_vval - this->m_vsval;
			this->m_rgb[2] = this->m_vval;
			break;
		case 5 :
			this->m_rgb[0] = this->m_vval;
			this->m_rgb[1] = this->m_vval - this->m_vsval;
			this->m_rgb[2] = this->m_vval - this->m_hfract;
			break;
		}
	}
	for(i = 0; i < 3; i++)
		this->m_rgb[i] = (this->m_rgb[i] > 0x3ff) ? 0x3ff : ((this->m_rgb[i] < 0) ? 0 : this->m_rgb[i]);
}

int C326HsvAdj::Read(char *buf, int len)
{
	char *dst = buf;
	int i = 0, offset = 0, unit = 2;
	CBaseConvert tools;

	if(len == 0)
		return 0;
	for(this->m_hpos = 0; this->m_hpos < this->m_width; this->m_hpos++, offset += 3)
	{
		for(i = 0; i < 3; i++)
			this->m_rgb[i] = 0x3ff & (this->m_rgbbuf.Read(offset + i) << 2);
		this->Rgb2Hsv();
		this->HsvAdj();
		this->Hsv2Rgb();
		for(i = 0; i < 3; i++, dst += unit)
			tools.setvalue(dst, this->m_rgb[i], unit);
	}
	return (offset * unit);
}

int C326HsvAdj::Process(char *src, char *dst)
{
	int len = 0, size = 0, size1 = 0, pos = 0, pos1 = 0;

	size = 3 * this->m_width * this->m_height;
	size1 = size << 1;
	while(size)
	{
		len = this->Write(src + pos, size);
		size -= len;
		pos += len;
		len = this->Read(dst + pos1, size1);
		size1 -= len;
		pos1 += len;
	}
	return pos1;
}

//=============== class C326Historgram =================

C326Historgram::C326Historgram(int width, int height, int unit)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_unit   = unit;

	this->Init();
}

C326Historgram::~C326Historgram(void)
{
}

void C326Historgram::Init(void)
{
	int i = 0;

	this->m_wt[0]  = 0x01;
	this->m_wt[1]  = 0x03;
	this->m_wt[2]  = 0x04;
	this->m_coe    = 0;
	this->m_frame  = 0;

	this->m_alpha  = 0x01;
	this->m_tval   = 192;
	this->m_sval   = 16;
	this->CalcQL();
	this->GetAera();

	this->m_bufsize = this->m_width * this->m_height * this->m_unit * 3;
	for(i = 0; i < HIST_FRAME_NUM; i++)
	{
		this->m_xval[i] = 0xff;
		this->m_databuf[i].SetSize((this->m_bufsize / this->m_unit), this->m_unit);
		memset(this->m_databuf[i].m_buf, 0, this->m_bufsize);
		this->m_pbuf[i] = &(this->m_databuf[i]);
	}
}

void C326Historgram::CalcQL(void)
{
	this->m_step = (256 - this->m_tval) / this->m_sval;
	this->m_qval = this->m_width * this->m_height * this->m_alpha / 100;
}

void C326Historgram::GetAera(void)
{
	int i = 0;

	this->m_area[0][0] = 256;
	for(i = 0; i < this->m_sval - 1; i++)
	{
		this->m_area[i][1] = 256 - this->m_step * i;
		this->m_area[i+1][0] = this->m_area[i][1];
	}
	this->m_area[this->m_sval - 1][1] = this->m_tval;
}

void C326Historgram::SetAlphaValue(int alpha)
{
	this->m_alpha = alpha;
	this->CalcQL();
}

void C326Historgram::SetTValue(int tval)
{
	this->m_tval = tval;
	this->CalcQL();
	this->GetAera();
}

void C326Historgram::SetSValue(int sval)
{
	this->m_sval = sval;
	this->CalcQL();
	this->GetAera();
}

int C326Historgram::Write(char *buf, int len)
{
	int pos = 0;

	pos = this->m_pbuf[HIST_FRAME_NUM - 1]->Write(buf, len);
	if(pos)
		this->m_frame ++;
	return pos;
}

void C326Historgram::LoopBuf(void)
{
	int i = 0;
	CBufferBase *p = this->m_pbuf[0];

	for(i = 0; i < HIST_FRAME_NUM - 1; i++)
	{
		this->m_pbuf[i] = this->m_pbuf[i+1];
		this->m_xval[i] = this->m_xval[i+1];
	}
	this->m_pbuf[HIST_FRAME_NUM - 1] = p;
}

void C326Historgram::GetStat(void)
{
	int i = 0, yval = 0, pos = 0, step = 0;
	CBaseConvert tool;

	memset(this->m_qcnt, 0, (this->m_sval * sizeof(int)));
	step = 3 * this->m_unit;
	for(; pos < this->m_bufsize; pos += step)
	{
		tool.getvalue((this->m_pbuf[HIST_FRAME_NUM - 1]->m_buf + pos), &yval, 1, this->m_unit);
		for(i = 0; i < this->m_sval; i++)
		{
			if((yval < this->m_area[i][0]) && (yval >= this->m_area[i][1]))
				this->m_qcnt[i] += 1;
		}
	}
}

void C326Historgram::GetXVal(void)
{
	int sum = 0, k = 0;

	sum += HIST_Q0_VAL;
	while(k < this->m_sval)
	{
		k ++;
		sum += this->m_qcnt[k-1];
		if(sum > this->m_qval)
			break;
	}
	if((k == this->m_sval) && (sum < this->m_qval))
		this->m_xval[HIST_FRAME_NUM - 1] = this->m_tval;
	else
		this->m_xval[HIST_FRAME_NUM - 1] = this->m_area[k-1][0];
}

void C326Historgram::CalcCoe(void)
{
	int sum0 = 0, sum1 = 0, i = 0, xval = 0;

	for(i = 0; i < HIST_FRAME_NUM; i++)
	{
		sum0 += this->m_wt[i] * this->m_xval[i];
		sum1 += this->m_wt[i];
	}
	xval = sum0 / sum1;
	this->m_coe = (0xff << 7) / xval;
}

void C326Historgram::CalcNewYVal(char *buf)
{
	int yval = 0, pos = 0, step = 0, size = 0;
	CBaseConvert tool;

	step = this->m_unit << 1;
	size = (this->m_width * this->m_height) * step;
	for(; pos < size; pos += step)
	{
		tool.getvalue((buf + pos), &yval, 1, this->m_unit);
		yval = (yval * this->m_coe) >> 7;
		tool.setvalue((buf + pos), &yval, 1, this->m_unit);
	}
}

void C326Historgram::Convert(char *buf)
{
	int i = 0, cnt = 0, pos = 0, pos1 = 0, step = 0, step1 = 0, src[6], dst[4];
	CBaseConvert tool;

	step = (this->m_unit * 3) << 1;
	step1 = this->m_unit << 2;
	cnt = (this->m_width * this->m_height) >> 1;
	for(i = 0; i < cnt; i++, pos += step, pos1 += step1)
	{
		tool.getvalue((this->m_pbuf[HIST_FRAME_NUM - 1]->m_buf + pos), src, 6, this->m_unit);
		dst[0] = src[0];
		dst[1] = (src[2] + src[5]) >> 1;
		dst[2] = src[3];
		dst[3] = (src[1] + src[4]) >> 1;
		tool.setvalue((buf + pos1), dst, 4, this->m_unit);
	}
}

int C326Historgram::Read(char *buf, int len)
{
	this->Convert(buf);
	if(this->m_frame > 1)
		this->CalcNewYVal(buf);
	this->GetStat();
	this->GetXVal();
	this->CalcCoe();
	this->LoopBuf();
	return len;
}

int C326Historgram::Process(char *src, char *dst)
{
	int len = (this->m_width * this->m_height) << 1;

	this->Write(src, this->m_bufsize);
	this->Read(dst, this->m_bufsize);
	return len;
}

//=============== class C326Motion =================

C326Motion::C326Motion(int width, int height, int unit)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_unit   = unit;
	this->Init();
}

C326Motion::~C326Motion(void)
{
}

void C326Motion::Init(void)
{
	int i = 0;

	this->m_motion  = 0;
	this->m_motion0 = 0;
	this->m_motion1 = 0;
	this->m_curframe = 0;
	this->SetFrameCnt();
	this->SetHThrld();
	this->SetMThrld();
	this->m_bufsize = this->m_width * this->m_height * this->m_unit * 2;
	for(i = 0; i < 2; i++)
	{
		this->m_databuf[i].SetSize((this->m_bufsize / this->m_unit), this->m_unit);
		memset(this->m_databuf[i].m_buf, 0, this->m_bufsize);
		this->m_pbuf[i] = &(this->m_databuf[i]);
	}
	memset(this->m_hval, 0, (MD_H_BLOCK * MD_V_BLOCK * sizeof(int)));

	this->m_mcs  = 0;
	this->m_mce  = 0x13f;
	this->m_mrs  = 0;
	this->m_mre  = 0xef;
	this->m_h[0] = 0x14;
	this->m_h[1] = 0x28;
	this->m_h[2] = 0x3c;
	this->m_v[0] = 0x0f;
	this->m_v[1] = 0x1e;
	this->m_v[2] = 0x2d;
}

void C326Motion::SetHThrld(int val)
{
	this->m_hthrld = val;
}

void C326Motion::SetMThrld(int val)
{
	this->m_mthrld = val;
}

void C326Motion::SetFrameCnt(int cnt)
{
	this->m_framecnt = cnt;
}

int C326Motion::Write(char *buf, int len)
{
	int pos = 0;

	pos = this->m_pbuf[1]->Write(buf, len);
	if(this->m_curframe == 0)
		memcpy(this->m_pbuf[0]->m_buf, this->m_pbuf[1]->m_buf, pos);
	return pos;
}

void C326Motion::LoopBuf(void)
{
	CBufferBase *p = this->m_pbuf[0];

	this->m_pbuf[0] = this->m_pbuf[1];
	this->m_pbuf[1] = p;
}

void C326Motion::Calc(void)
{
	int i = 0, j = 0, k = 0, m = 0, h_cnt = 0, v_cnt = 0, span = 0, sum0 = 0, sum1 = 0;
	int pos = 0, val = 0, offset = 0, delta = 0;
	int h_end[4], v_end[4], h_offset[4], v_offset[4];
	CBaseConvert tool;

	this->m_motion0 = 0;
	span = (this->m_width << 1) * this->m_unit;

	h_end[0] = (this->m_h[0] << 3) - 1;
	h_end[1] = (this->m_h[1] << 3) - 1;
	h_end[2] = (this->m_h[2] << 3) - 1;
	h_end[3] = (this->m_mce << 1) - 1;
	h_offset[0] = (this->m_mcs << 1) * this->m_unit;
	h_offset[1] = ((h_end[0] + 1) << 1) * this->m_unit;
	h_offset[2] = ((h_end[1] + 1) << 1) * this->m_unit;
	h_offset[3] = ((h_end[2] + 1) << 1) * this->m_unit;

	v_end[0] = (this->m_v[0] << 2) - 1;
	v_end[1] = (this->m_v[1] << 2) - 1;
	v_end[2] = (this->m_v[2] << 2) - 1;
	v_end[3] = this->m_mre;
	v_offset[0] = (this->m_mrs << 1) * span;
	v_offset[1] = ((v_end[0] + 1) << 1) * span;
	v_offset[2] = ((v_end[1] + 1) << 1) * span;
	v_offset[3] = ((v_end[2] + 1) << 1) * span;

	for(j = 0; j < MD_V_BLOCK; j++)
	{
		v_cnt = (j == 0) ? (v_end[0] + 1) : (v_end[j] - v_end[j-1]);
		for(i = 0; i < MD_H_BLOCK; i++)
		{
			this->m_hval[j][i] = 0;
			offset = v_offset[j] + span + h_offset[i];
			h_cnt = (i == 0) ? (h_end[0] + 1) : (h_end[i] - h_end[i-1]);
			for(m = 0; m < v_cnt; m++, offset += (span << 1))
			{
				sum0 = 0;
				sum1 = 0;
				pos  = offset;
				for(k = 0; k < h_cnt; k++, pos += (this->m_unit << 1))
				{
					tool.getvalue((this->m_pbuf[0]->m_buf + pos), &val, 1, this->m_unit);
					sum0 += val;
					tool.getvalue((this->m_pbuf[1]->m_buf + pos), &val, 1, this->m_unit);
					sum1 += val;
				}
				sum0 >>= 2;
				sum1 >>= 2;
				delta = abs((sum0 - sum1));
				if(delta > this->m_hthrld)
					this->m_hval[j][i] += 1;
			}
			if(this->m_hval[j][i] > this->m_mthrld)
				this->m_motion0 |= (0x01 << (j * MD_H_BLOCK + i));
		}
	}
	this->m_motion1 = this->m_motion0 ? (this->m_motion1 + 1) : ((this->m_motion1 == 1) ? 0 : this->m_motion1);
}

int C326Motion::Read(char *buf, int len)
{
	buf = buf;			// avoid warning

	this->Calc();
	this->LoopBuf();
	return len;
}

int C326Motion::Process(char *src, char *dst)
{
	this->Write(src, this->m_bufsize);
	this->Read(dst, this->m_bufsize);
	this->m_curframe ++;
	//if(this->m_curframe == MD_FRAME_NUM)
	if(this->m_curframe == this->m_framecnt)
	{
		this->m_motion = (this->m_motion1 > 1) ? 1 : 0;
		this->m_curframe = 0;
	}
	return this->m_motion;
}

//=============== class C326AE =================

C326AE::C326AE(int width, int height, int unit)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_unit   = unit;
	this->Init();
}

C326AE::~C326AE(void)
{
}

void C326AE::Init(void)
{
	this->m_win[0][0] = 0x10;
	this->m_win[0][1] = 0x50;
	this->m_win[0][2] = 0xf0;
	this->m_win[0][3] = 0x190;
	this->m_win[0][4] = 0x230;

	this->m_win[1][0] = 0x10;
	this->m_win[1][1] = 0x38;
	this->m_win[1][2] = 0xb0;
	this->m_win[1][3] = 0x128;
	this->m_win[1][4] = 0x1a0;

	this->m_win_wt[0]  = 0x01;
	this->m_win_wt[1]  = 0x03;
	this->m_win_wt[2]  = 0x03;
	this->m_win_wt[3]  = 0x01;
	this->m_win_wt[4]  = 0x02;
	this->m_win_wt[5]  = 0x04;
	this->m_win_wt[6]  = 0x04;
	this->m_win_wt[7]  = 0x02;
	this->m_win_wt[8]  = 0x02;
	this->m_win_wt[9]  = 0x04;
	this->m_win_wt[10] = 0x04;
	this->m_win_wt[11] = 0x02;
	this->m_win_wt[12] = 0x01;
	this->m_win_wt[13] = 0x03;
	this->m_win_wt[14] = 0x03;
	this->m_win_wt[15] = 0x01;
	this->m_win_wt[16] = 0x28;

	this->m_bufsize = this->m_width * this->m_height * this->m_unit * 3;
	this->m_databuf.SetSize((m_bufsize / this->m_unit), this->m_unit);
}

void C326AE::SetWin(int *pwin)
{
	int i = 0;

	for(i = 0; i < 5; (i += 2))
	{
		this->m_win[0][i] = pwin[i];
		this->m_win[1][i] = pwin[i+1];
	}
}

void C326AE::SetWinWt(int *wt)
{
	int i = 0;

	this->m_win_wt[16] = 0;
	for(i = 0; i < 16; i++)
	{
		this->m_win_wt[i] = wt[i];
		this->m_win_wt[16] += this->m_win_wt[i];
	}
}

int C326AE::Write(char *buf, int len)
{
	int pos = 0;

	pos = this->m_databuf.Write(buf, len);
	return pos;
}

void C326AE::Calc(void)
{
	int i = 0, j = 0, k = 0, m = 0;
	int step = 0, span = 0, sum = 0, total = 0, cnt = 0, num = 0;
	int pos = 0, val = 0, offset = 0, idx = 0, h_cnt = 0, v_cnt = 0;
	CBaseConvert tool;

	span = this->m_width * 3 * this->m_unit;
	step = 3 * this->m_unit;
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 4; i++)
		{
			h_cnt = this->m_win[0][i+1] - this->m_win[0][i];
			v_cnt = this->m_win[1][j+1] - this->m_win[1][j];
			offset = this->m_win[1][j] * span + this->m_win[0][i] * step;
			for(m = 0, sum = 0; m < v_cnt; m++, offset += span)
			{
				pos = offset;
				for(k = 0; k < h_cnt; k++, pos += step)
				{
					tool.getvalue((this->m_databuf.m_buf + pos), &val, 1, this->m_unit);
					sum += val;
				}
			}
			num = h_cnt * v_cnt;
			idx = (j << 2) + i;
			this->m_ywmean[idx] = sum / num;
			total += sum * this->m_win_wt[idx];
			cnt += num * this->m_win_wt[idx];
		}
	}
	this->m_ymean = (total / this->m_win_wt[16]) / (cnt / this->m_win_wt[16]);
}

int C326AE::Read(char *buf, int len)
{
	buf = buf;			// avoid warning

	this->Calc();
	return len;
}

int C326AE::Process(char *src, char *dst)
{
	int len = 0;

	len = this->Write(src, this->m_bufsize);
	this->Read(dst, this->m_bufsize);
	return len;
}

//=============== class C326AWB =================

C326AWB::C326AWB(int width, int height, int unit)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_unit   = unit;
	this->Init();
}

C326AWB::~C326AWB(void)
{
}

void C326AWB::Init(void)
{
	this->m_win[0][0] = 0x10;
	this->m_win[0][1] = 0x50;
	this->m_win[0][2] = 0xf0;
	this->m_win[0][3] = 0x190;
	this->m_win[0][4] = 0x230;

	this->m_win[1][0] = 0x10;
	this->m_win[1][1] = 0x38;
	this->m_win[1][2] = 0xb0;
	this->m_win[1][3] = 0x128;
	this->m_win[1][4] = 0x1a0;

	this->m_cnt  = 0;
	this->m_ybot = 0;
	this->m_ytop = 0xd0;
	this->m_itop = 0x18;
	this->m_qtop = 0x10;
	this->m_rgain = 0x40;
	this->m_bgain = 0x40;
	this->m_gainbot = 0x14;
	this->m_gaintop = 0x50;
	this->m_awbstep = 0x04;
	this->m_awbthd1 = 0x03;
	this->m_awbthd2 = 0x0f;

	this->m_rgbbufsize = this->m_width * this->m_height * this->m_unit * 3;
	this->m_yiqbufsize = this->m_width * this->m_height * 3;
	this->m_rgbbuf.SetSize((this->m_rgbbufsize / this->m_unit), this->m_unit);
	this->m_yiqbuf.SetSize(this->m_yiqbufsize);
}

void C326AWB::SetWin(int *pwin)
{
	int i = 0;

	for(i = 0; i < 5; (i += 2))
	{
		this->m_win[0][i] = pwin[i];
		this->m_win[1][i] = pwin[i+1];
	}
}

void C326AWB::SetYThrld(int bot, int top)
{
	this->m_ybot = bot;
	this->m_ytop = top;
}

void C326AWB::SetIThrld(int top)
{
	this->m_itop = top;
}

void C326AWB::SetQThrld(int top)
{
	this->m_qtop = top;
}

void C326AWB::SetGainThrld(int bot, int top)
{
	this->m_gainbot = bot;
	this->m_gaintop = top;
}

int C326AWB::Write(char *buf, int len)
{
	int pos = 0;

	if(this->m_cnt)
		pos = this->m_yiqbuf.Write(buf, len);
	else
		pos = this->m_rgbbuf.Write(buf, len);
	this->m_cnt = this->m_cnt ? 0 : 1;
	return pos;
}

void C326AWB::GetRgbMean(void)
{
	int i = 0, j = 0, k = 0;
	int step[2] = {0, 0}, span[2] = {0, 0}, pos[2] = {0, 0}, offset[2] = {0, 0};
	int h_cnt = 0, v_cnt = 0, cnt = 0, total[3], rgb[3], yiq[3];
	CBaseConvert tool;

	step[0] = 3 * this->m_unit;
	span[0] = this->m_width * 3 * this->m_unit;
	offset[0] = this->m_win[1][0] * span[0] + this->m_win[0][0] * step[0];
	step[1] = 3;
	span[1] = this->m_width * 3;
	offset[1] = this->m_win[1][0] * span[1] + this->m_win[0][0] * step[1];
	memset(total, 0, (3 * sizeof(int)));
	h_cnt = this->m_win[0][4] - this->m_win[0][0];
	v_cnt = this->m_win[1][4] - this->m_win[1][0];
	for(j = 0; j < v_cnt; j++, offset[0] += span[0], offset[1] += span[1])
	{
		memcpy(pos, offset, (2 * sizeof(int)));
		for(i = 0; i < h_cnt; i++, pos[0] += step[0], pos[1] += step[1])
		{
			tool.getvalue((this->m_yiqbuf.m_buf + pos[1]), yiq, 3);
			if(((yiq[0] >= this->m_ybot) && (yiq[0] <= this->m_ytop)) && (yiq[1] <= this->m_itop) && (yiq[2] <= this->m_qtop))
			{
				cnt ++;
				tool.getvalue((this->m_rgbbuf.m_buf + pos[0]), rgb, 3, this->m_unit);
				for(k = 0; k < 3; k++)
					total[k] += (rgb[k] >> 2);	// 10bit -> 8bit
			}
		}
	}
	for(i = 0; i < 3; i++)
		this->m_rgb_mean[i] = total[i] / cnt;
}

void C326AWB::CalcGain(void)
{
	int rnew = 0, bnew = 0, delta = 0, bstep = 0, rstep = 0;

	delta = abs((this->m_rgb_mean[0] - this->m_rgb_mean[1]));
	rstep = (delta > this->m_awbthd2) ? this->m_awbstep : (delta > this->m_awbthd1) ? 1 : 0;
	delta = abs((this->m_rgb_mean[2] - this->m_rgb_mean[1]));
	bstep = (delta > this->m_awbthd2) ? this->m_awbstep : (delta > this->m_awbthd1) ? 1 : 0;

	if(this->m_rgb_mean[0] > this->m_rgb_mean[1])
		rnew = this->m_rgain - rstep;
	else
		rnew = this->m_rgain + rstep;

	if(this->m_rgb_mean[2] > this->m_rgb_mean[1])
		bnew = this->m_bgain - bstep;
	else
		bnew = this->m_bgain + bstep;

	if(rnew >= this->m_gaintop)
		this->m_rgain = this->m_gaintop;
	else if(rnew <= this->m_gainbot)
		this->m_rgain = this->m_gainbot;
	else
		this->m_rgain = rnew;

	if(bnew >= this->m_gaintop)
		this->m_bgain = this->m_gaintop;
	else if(bnew <= this->m_gainbot)
		this->m_bgain = this->m_gainbot;
	else
		this->m_bgain = bnew;
}

int C326AWB::Read(char *buf, int len)
{
	buf = buf;			// avoid warning

	this->GetRgbMean();
	this->CalcGain();
	return len;
}

int C326AWB::Process(char *src, char *dst)
{
	int len = 0;

	len = this->Write(src, this->m_rgbbufsize);
	this->Write(dst, this->m_yiqbufsize);
	this->Read(NULL, this->m_rgbbufsize);
	return len;
}

//=============== class C326AFlicker =================

C326AFlicker::C326AFlicker(int width, int height, int unit)
{
	this->m_width  = width;
	this->m_height = height;
	this->m_unit   = unit;
	this->Init();
}

C326AFlicker::~C326AFlicker(void)
{
}

void C326AFlicker::Init(void)
{
	int i = 0;

	this->m_start = 0x40;
	this->m_num   = 0x40;
	this->m_sum   = 0;
	this->m_frame = 0;

	this->m_bufsize = this->m_width * this->m_height * this->m_unit * 3;
	for(i = 0; i < 2; i++)
	{
		this->m_databuf[i].SetSize((m_bufsize / this->m_unit), this->m_unit);
		memset(this->m_databuf[0].m_buf, 0, this->m_bufsize);
		this->m_pbuf[i] = &(this->m_databuf[i]);
	}
}

void C326AFlicker::SetLine(int start, int num)
{
	this->m_start = start;
	this->m_num   = num;
}

int C326AFlicker::Write(char *buf, int len)
{
	int pos = 0;

	pos = this->m_pbuf[1]->Write(buf, len);
	if(this->m_frame == 0)
		memcpy(this->m_pbuf[0]->m_buf, this->m_pbuf[1]->m_buf, pos);
	return pos;
}

void C326AFlicker::LoopBuf(void)
{
	CBufferBase *p = this->m_pbuf[0];

	this->m_pbuf[0] = this->m_pbuf[1];
	this->m_pbuf[1] = p;
}

void C326AFlicker::Calc(void)
{
	int i = 0, j = 0, step = 0, val = 0, pos = 0, sum[2];
	CBaseConvert tool;

	this->m_sum = 0;
	step = 3 * this->m_unit;
	pos = this->m_start * this->m_width * 3 * this->m_unit;
	for(j = 0; j < this->m_num; j++)
	{
		memset(sum, 0, (sizeof(int) << 1));
		for(i = 0; i < this->m_width; i++, pos += step)
		{
			tool.getvalue((this->m_databuf[0].m_buf + pos), &val, 1, this->m_unit);
			sum[0] += val;
			tool.getvalue((this->m_databuf[1].m_buf + pos), &val, 1, this->m_unit);
			sum[1] += val;
		}
		this->m_sum += abs((sum[0] - sum[1]));
	}
}

int C326AFlicker::Read(char *buf, int len)
{
	buf = buf;			// avoid warning

	this->Calc();
	this->LoopBuf();
	return len;
}

int C326AFlicker::Process(char *src, char *dst)
{
	int len = 0;

	len = this->Write(src, this->m_bufsize);
	this->m_frame ++;
	this->Read(dst, this->m_bufsize);
	return len;
}





