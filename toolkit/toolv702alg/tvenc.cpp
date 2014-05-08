

#include "internal.h"
/*====================================================================================================*/
//				class C702Filter
/*====================================================================================================*/
C702Filter::C702Filter(int width, int height, int pal, int format)
{
	this->Init(width, height, pal, format);
}

C702Filter::~C702Filter()
{
}

void C702Filter::Init(int width, int height, int pal, int format)
{
	int i;

	this->m_width = width;
	this->m_height = height;
	this->m_format = format;
	this->m_pal = pal;
	this->m_flg = FILTER_NONE;
	for (i = 0; i < 11; i++)
		this->m_yfilter[i] = FIRCoeff_y[this->m_pal][i];
	for (i = 0; i < 7; i++)
	{
		this->m_ufilter[i] = FIRCoeff_u[this->m_pal][i];
		this->m_vfilter[i] = FIRCoeff_v[this->m_pal][i];
	}

	this->m_linebuf.SetSize(this->m_width, 0);
	this->m_linebuf.SetFormat(this->m_format);
}

int C702Filter::Process(char *src, char *dst)
{
	int len;

	/*this->m_width = NTSC_WIDTH;
	this->m_height = (this->m_pal) ? PAL_HEIGHT : NTSC_HEIGHT;*/

	this->m_flg = FILTER_Y;
	len = this->Filter(src, dst);

	this->m_flg = FILTER_U;
	len = this->Filter(src, dst);

	this->m_flg = FILTER_V;
	len = this->Filter(src, dst);

	return len;
}

int C702Filter::Filter(char *src, char *dst)
{
	int i, j, pos = 0, pos1 = 0, width = 0;
	int size, size1;
	
	size1 = this->m_width << 1;
	size = (this->m_width * this->m_height) << 1;

	for (i = 0; i < this->m_height; i++)
	{
		if (i == 0)
		{
			for (j = 0; j < 6; j++)
			{
				this->m_firstval[j << 1] = 0;
				this->m_firstval[(j << 1) + 1] = -128;
			}
		}
		pos += this->Write(src+pos, size);
		if (i != (this->m_height - 1))
		{
			for (j = 0; j < 12; j++)
				this->m_lastval[j] = src[pos + j];
		}
		else
		{
			for (j = 0; j < 3; j++)
			{
				this->m_lastval[j << 2] = src[pos - 2];				//y    ....yuyv				
				this->m_lastval[(j << 2) + 1] = src[pos - 3];		//u    ....yuyv
				this->m_lastval[(j << 2) + 2] = src[pos - 2];		//y    ....yuyv
				this->m_lastval[(j << 2) + 3] = src[pos - 1];		//v    ....yuyv
			}
		}
		
		pos1 += this->Read(dst+pos1, size1);


		for (j = 0; j < 12; j++)
			this->m_firstval[j] = src[pos - 12 + j];
	}

	return pos1;
}

int C702Filter::Write(char *buf, int len)
{
	int pos = 0;
	
	pos = this->m_linebuf.Write(buf, len);
	this->m_linebuf.ClearPoint();
	
	return pos;
}

int C702Filter::Read(char *buf, int len)
{
	if (this->m_flg == FILTER_Y)
		len = this->YProc(buf, len);
	else if (this->m_flg == FILTER_U)
		len = this->UProc(buf, len);
	else if (this->m_flg == FILTER_V)
		len = this->VProc(buf, len);

	return len;
}

int C702Filter::YProc(char *buf, int len)
{
//	int val[NTSCPAL_WIDTH + 12];
	int *val = NULL, *Yval;
	int unit = 2;
	int i, j, result, pos = 0;
	double tmp, tmp0;
	CBaseConvert tools;

	val = new int[this->m_width + 12];
	Yval = val + 6;
	this->GetCompData(Yval);
	for (i = 0; i < this->m_width; i++, pos += 4)
	{
		tmp = 0;
		for ( j = 0; j < 5; j++)
		{			
			tmp0 = Yval[i - 5 + j] * this->m_yfilter[j] + Yval[i + 5 - j] * this->m_yfilter[10 - j];
			tmp += (double)( (int)( tmp0 / (pow(2, (double)(10 - 7))) + ( (tmp0 >= 0) ? 0 : -0.875) ) );
		}
		tmp0 = Yval[i] * this->m_yfilter[5];
		tmp += (double)( (int)( tmp0 / (pow(2, (double)(10 - 7))) + ( (tmp0 >= 0) ? 0 : -0.875) ) );
		
		result = (int)( tmp / ( pow(2, (double)(10 - 8)) ) + ( (tmp >= 0) ? 0 : -0.75 ) );
		result = (result < 0) ? 0 : ((result > 4095) ? 4095 : result);

		tools.setvalue(buf + pos, result, unit);
	}

	delete []val;
	return (len * unit);
}

int C702Filter::UProc(char *buf, int len)
{
//	int val[(NTSCPAL_WIDTH >> 1) + 6];
	int *Uval, *val =NULL;
	int i, j, result, pos = 2;
	int unit = 2;
	double tmp, tmp0;
	CBaseConvert tools;

	val = new int[(this->m_width >> 1) + 6];
	Uval = val + 3;
	this->GetCompData(Uval);

	for (i = 0; i < (this->m_width >> 1); i++, pos += 8)
	{
		tmp = 0;
		for (j = 0; j < 3; j++)
		{
			tmp0 = (Uval[i - 3 + j] - 128) * this->m_ufilter[j] + (Uval[i + 3 - j] - 128) * this->m_ufilter[6 - j];
			tmp += (double)( (int)( tmp0 / (pow(2, (double)(10 - 8))) + ( (tmp0 >= 0) ? 0 : -0.75) ) );
		}
		tmp0 = (Uval[i] - 128) * this->m_ufilter[3];
		tmp += (double)( (int)( tmp0 / (pow(2, (double)(10 - 8))) + ( (tmp0 >= 0) ? 0 : -0.75) ) );

		result = (int)( tmp / ( pow(2, (double)(10 - 7)) ) + ( (tmp >= 0) ? 0 : -0.875 ) );
		result = (result < (-2048)) ? (-2048) : ((result > 2047) ? 2047 : result);

		tools.setvalue(buf + pos, result, unit);
	}
	delete []val;
	return (len * unit);
}

int C702Filter::VProc(char *buf, int len)
{
//	int val[(NTSCPAL_WIDTH >> 1) + 6];
	int *Vval, *val = NULL;
	int unit = 2;
	int i, j, result, pos = 6;
	double tmp, tmp0;
	CBaseConvert tools;

	val = new int[(this->m_width >> 1) + 6];
	Vval = val + 3;
	this->GetCompData(Vval);

	for (i = 0; i < (this->m_width >> 1); i++, pos += 8)
	{
		tmp = 0;
		for (j = 0; j < 3; j++)
		{
			tmp0 = (Vval[i - 3 + j] - 128) * this->m_vfilter[j] + (Vval[i + 3 - j] - 128) * this->m_vfilter[6 - j];
			tmp += (double)( (int)( tmp0 / (pow(2, (double)(10 - 8))) + ( (tmp0 >= 0) ? 0 : -0.75) ) );
		}
		tmp0 = (Vval[i] - 128) * this->m_vfilter[3];
		tmp += (double)( (int)( tmp0 / (pow(2, (double)(10 - 8))) + ( (tmp0 >= 0) ? 0 : -0.75) ) );

		result = (int)( tmp / ( pow(2, (double)(10 - 7)) ) + ( (tmp >= 0) ? 0 : -0.875 ) );
		result = (result < (-2048)) ? (-2048) : ((result > 2047) ? 2047 : result);
		
		tools.setvalue(buf + pos, result, unit);
	}
	delete []val;
	return (len * unit);
}


void C702Filter::GetCompData(int *pval)
{
	int i, pos = 0, unit = 1;
	int fyuv[6], lyuv[6];
	CYuv2Rgb tools;

	for (i = 0; i < 3; i++, pos += 4 * unit)
	{
		tools.getyuv422(this->m_firstval + pos, fyuv, this->m_format, unit);
		tools.getyuv422(this->m_lastval + pos, lyuv, this->m_format, unit);
		if (this->m_flg == FILTER_Y)
		{
			pval[2 * i - 6] = fyuv[0];
			pval[2 * i - 5] = fyuv[3];
			pval[this->m_width + 2 * i] = lyuv[0];
			pval[this->m_width + 2 * i + 1] = lyuv[3];

		}
		else if (this->m_flg == FILTER_U)
		{
			pval[i - 3] = fyuv[1];
			pval[(this->m_width >> 1) + i] = lyuv[1];
		}
		else
		{
			pval[i - 3] = fyuv[2];
			pval[(this->m_width >> 1) + i] = lyuv[2];
		}		
	}

	for (i = 0, pos = 0; i < (this->m_width >> 1); i++, pos += 4 * unit)
	{
		tools.getyuv422(this->m_linebuf.m_buf + pos, fyuv, this->m_format, unit);
		if (this->m_flg == FILTER_Y)
		{
			pval[2 * i] = fyuv[0];
			pval[2 * i + 1] = fyuv[3];
		}
		else if (this->m_flg == FILTER_U)
			pval[i] = fyuv[1];
		else
			pval[i] = fyuv[2];
	}
}

/*====================================================================================================*/
//				class	C702Upsample
/*====================================================================================================*/

C702Upsample::C702Upsample(int width, int height, int unit, int format)
{
	this->Init(width, height, unit, format);
}

C702Upsample::~C702Upsample()
{
}

void C702Upsample::Init(int width, int height, int unit, int format)
{
	this->m_width = width;
	this->m_height = height;
	this->m_format = format;
	this->m_unit = unit;
	this->m_flg = UPSAMPLE_NONE;

	this->m_linebuf.SetFormat(this->m_format);
	this->m_linebuf.SetSize(this->m_width, 0, this->m_unit);
}

int C702Upsample::Process(char *src, char *dst)
{
	int len;

	this->m_flg = UPSAMPLE_Y;
	len = this->UpSample(src, dst);

	this->m_flg = UPSAMPLE_U;
	len = this->UpSample(src, dst);

	this->m_flg = UPSAMPLE_V;
	len = this->UpSample(src, dst);

	return len;
}

int C702Upsample::UpSample(char *src, char *dst)
{
	int i;
	int pos = 0, pos1 = 0;
	int size, size1;

	size = this->m_width * this->m_height * this->m_unit << 1;
	size1 =  this->m_width * this->m_unit << 1;
	for (i = 0; i < this->m_height; i++)
	{
		pos += this->Write(src+pos, size);
		this->GetLastval(src, size, pos);

		pos1 += this->Read(dst+pos1, size1);
	}
	return pos1;
}

int C702Upsample::Write(char *buf, int len)
{
	int pos;

	pos = this->m_linebuf.Write(buf, len);
	this->m_linebuf.ClearPoint();
	return pos;
}

int C702Upsample::Read(char *buf, int len)
{
	if (this->m_flg == UPSAMPLE_Y)
		len = this->YProc(buf, len);
	else if (this->m_flg == UPSAMPLE_U)
		len = this->UVProc(buf, len);
	else
		len = this->UVProc(buf, len);

	return len;
}

int C702Upsample::YProc(char *buf, int len)
{

	int i, pos = 0;
	double tmp;
	//int yval[NTSCPAL_WIDTH + 1];
	int *yval = NULL;
	CBaseConvert tools;

	yval = new int[this->m_width + 1];
	this->GetCompData(yval);
	yval[this->m_width] = this->m_lastval[this->m_flg - 1];			//yval[this->m_width - 1];

	for (i = 0; i < this->m_width; i++)
	{
		tools.setvalue(buf + pos, yval[i], this->m_unit);
		pos += 3 * this->m_unit;

		tmp = yval[i] * 0.5 + yval[i + 1] * 0.5;
		tmp = (tmp > 0) ? (tmp + 0.5) : (tmp - 0.5);
		tools.setvalue(buf + pos, (int)tmp, this->m_unit);
		pos += 3 * this->m_unit;
	}
	delete []yval;
	return len * 3;
}

int C702Upsample::UVProc(char *buf, int len)
{
	int i, j, pos, result;
	double tmp[4];
	int *uvval = NULL;
	//int uvval[(NTSCPAL_WIDTH >> 1) + 1];
	CBaseConvert tools;
	
	uvval = new int[(this->m_width >> 1) + 1];
	this->GetCompData(uvval);
	uvval[this->m_width >> 1] = this->m_lastval[this->m_flg - 1];			//uvval[(this->m_width >> 1) - 1];
	pos = (this->m_flg == UPSAMPLE_U) ? this->m_unit : (this->m_unit << 1);

	for (i = 0; i < (this->m_width >> 1); i++)
	{
		tmp[0] = (double)uvval[i];
		tmp[1] = uvval[i] * 0.75 + uvval[i + 1] * 0.25;
		tmp[2] = uvval[i] * 0.5 + uvval[i + 1] * 0.5;
		tmp[3] = uvval[i] * 0.25 + uvval[i + 1] * 0.75;		
		tmp[1] = (tmp[1] > 0) ? (tmp[1] + 0.5) : (tmp[1] - 0.5);
		tmp[2] = (tmp[2] > 0) ? (tmp[2] + 0.5) : (tmp[2] - 0.5);
		tmp[3] = (tmp[3] > 0) ? (tmp[3] + 0.5) : (tmp[3] - 0.5);

		for (j = 0; j < 4; j++, pos += 3 * this->m_unit)
		{
			result = (int)tmp[j];
			result = (result >= 0) ? result : ((result == -2048) ? 4095 : (2048 - result));
			tools.setvalue(buf + pos, result, this->m_unit);
		}
	}
	delete []uvval;
	return len * 3;
}

void C702Upsample::GetLastval(char *buf, int totalsize, int pos)
{
	if (pos != totalsize)
	{		
		this->m_lastval[0] = (buf[pos] & 0xff) + (buf[pos + 1] << 8);			//y
		this->m_lastval[1] = (buf[pos + 2] & 0xff) + (buf[pos + 3] << 8);		//u
		this->m_lastval[2] = (buf[pos + 6] & 0xff) + (buf[pos + 7] << 8);		//v
	}
	else
	{
		this->m_lastval[0] = (buf[pos - 4] & 0xff) + (buf[pos - 3] << 8);		//y
		this->m_lastval[1] = (buf[pos - 6] & 0xff) + (buf[pos - 5] << 8);		//u
		this->m_lastval[2] = (buf[pos - 2] & 0xff) + (buf[pos - 1] << 8);		//v
	}
}

void C702Upsample::GetCompData(int *buf)
{
	int i, pos = 0;
	int yuv[6];
	CYuv2Rgb tools;
	
	for (i = 0; i < (this->m_width >> 1); i++)
	{
		tools.getyuv422(this->m_linebuf.m_buf + pos, yuv, this->m_format, this->m_unit);
		pos += 4 * this->m_unit;
		if (this->m_flg == UPSAMPLE_Y)
		{
			buf[i << 1] = (yuv[0] << 16) >> 16;
			buf[(i << 1) + 1] = (yuv[3] << 16) >> 16;
		}
		else if (this->m_flg == UPSAMPLE_U)
			buf[i] = (yuv[1] << 16) >> 16;
		else
			buf[i] = (yuv[2] << 16) >> 16;
	}
}

/*====================================================================================================*/
//			class	C702Composite
/*====================================================================================================*/

C702Composite::C702Composite(int width, int height, int pal, int interlace, int unit)
{
	this->Init(width, height, pal, interlace, unit);
}

C702Composite::~C702Composite()
{
}

void C702Composite::Init(int width, int height, int pal, int interlace, int unit)
{
	this->m_width = width;
	this->m_height = height;
	this->m_pal = pal;
	this->m_interlace = interlace;
	this->m_unit = unit;
	this->m_factor = asin(1) / 1024;
	if (this->m_pal == 1)
	{
		this->m_option.tv_colmax = TV_COLMAX_PAL;
		this->m_option.tv_rowmax = TV_ROWMAX_PAL;
		this->m_option.vs_pulse = VS_PULSE_PAL;
		this->m_option.ve1_pulse = VE1_PULSE_PAL;
		this->m_option.ve2_pulse = VE2_PULSE_PAL;
		this->m_option.vb_pulse = VB_PULSE_PAL;
		this->m_option.vs_duration = VS_DURATION_PAL;
		this->m_option.ve_duration = VE_DURATION_PAL;
		this->m_option.hs_duration = HS_DURATION_PAL;
		this->m_option.burst_start = BURST_START_PAL;
		this->m_option.burst_end = BURST_END_PAL;
		this->m_option.sync_level = SYNC_LEVEL_PAL;
		this->m_option.black_level = BLACK_LEVEL_PAL;
		this->m_option.burst_level = BURST_LEVEL_PAL;
		this->m_option.chrom_level = CHROM_LEVEL_PAL;
		this->m_option.col_delta = COL_DELTA_PAL;
		this->m_option.row_delta = ROW_DELTA_PAL;
		this->m_option.data_start = DATA_START_PAL;
		this->m_option.front_porch = FRONT_PORCH_PAL;
		this->m_option.first_field = 0;
	}
	else
	{
		this->m_option.tv_colmax = TV_COLMAX_NTSC;
		this->m_option.tv_rowmax = TV_ROWMAX_NTSC;
		this->m_option.vs_pulse = VS_PULSE_NTSC;
		this->m_option.ve1_pulse = VE1_PULSE_NTSC;
		this->m_option.ve2_pulse = VE2_PULSE_NTSC;
		this->m_option.vb_pulse = VB_PULSE_NTSC;
		this->m_option.vs_duration = VS_DURATION_NTSC;
		this->m_option.ve_duration = VE_DURATION_NTSC;
		this->m_option.hs_duration = HS_DURATION_NTSC;
		this->m_option.burst_start = BURST_START_NTSC;
		this->m_option.burst_end = BURST_END_NTSC;
		this->m_option.sync_level = SYNC_LEVEL_NTSC;
		this->m_option.black_level = BLACK_LEVEL_NTSC;
		this->m_option.burst_level = BURST_LEVEL_NTSC;
		this->m_option.chrom_level = CHROM_LEVEL_NTSC;
		this->m_option.col_delta = COL_DELTA_NTSC;
		this->m_option.row_delta = ROW_DELTA_NTSC;
		this->m_option.data_start = DATA_START_NTSC;
		this->m_option.front_porch = FRONT_PORCH_NTSC;
		this->m_option.first_field = 1;
	}

	this->m_linebuf.SetSize(this->m_width, 0, this->m_unit);
}

int C702Composite::Process(char *src, char *dst)
{
	int len;

	if (this->m_pal == 1)
		len = PalProc(src,dst);
	else
		len = NtscProc(src, dst);

	return len;
}

void C702Composite::GetSinCos(int *val_sin, int *val_cos)
{
	double xx, yy;
	int tmp;

	xx = this->m_option.thta / 1048576;
	xx *= this->m_factor;
	yy = sin(xx);			
	tmp = (int)(yy > 0 ? (1024 * yy + 0.5) : (1024 * yy - 0.5));
	tmp = (tmp > 1023) ? 1023 : (tmp < -1023 ? -1023 : tmp);
	*val_sin = tmp;
	yy = cos(xx);
	tmp = (int)(yy > 0 ? (1024 * yy + 0.5) : (1024 * yy - 0.5));
	tmp = (tmp > 1023) ? 1023 : (tmp < -1023 ? -1023 : tmp);
	*val_cos = tmp;
}

int C702Composite::SubProc(char *buf, int param)
{
	int i, pos = 0, tv_y, tv_c, tv_cvbs;
	CBaseConvert tools;

	for (i = 0; i < param; i++)
	{
		tv_y = this->m_option.sync_level;
		tv_c = 0;
		tv_cvbs = tv_y + tv_c;
		tv_c += this->m_option.chrom_level << 1;
		tools.setvalue(buf + pos, tv_cvbs, this->m_unit);
		pos += this->m_unit;
		this->m_option.thta += this->m_option.subfreq;
	}
	return pos;
}

int C702Composite::SyncProc(char *buf, int param1, int param2, int flag)
{
	int i, tv_y, tv_c, tv_cvbs;
	int pos = 0;
	CBaseConvert tools;
	
	tv_y = (int)(this->m_option.sync_level * 0.75);
	tv_c = 0;
	tv_cvbs = tv_y + tv_c;
	tv_c += this->m_option.chrom_level << 1;
	tools.setvalue(buf + pos, tv_cvbs, this->m_unit);
	pos += this->m_unit;
	this->m_option.thta += (flag) ? this->m_option.subfreq : 0;

	tv_y = (int)(this->m_option.sync_level * 0.5);
	tv_c = 0;
	tv_cvbs = tv_y + tv_c;
	tv_c += this->m_option.chrom_level << 1;
	tools.setvalue(buf + pos, tv_cvbs, this->m_unit);
	pos += this->m_unit;
	this->m_option.thta += (flag) ? this->m_option.subfreq : 0;

	tv_y = (int)(this->m_option.sync_level * 0.25);
	tv_c = 0;
	tv_cvbs = tv_y + tv_c;
	tv_c += this->m_option.chrom_level << 1;
	tools.setvalue(buf + pos, tv_cvbs, this->m_unit);
	pos += this->m_unit;
	this->m_option.thta += (flag) ? this->m_option.subfreq : 0;

	for (i = 0; i < param1; i++)
	{
		tv_y = 0;
		tv_c = 0;
		tv_cvbs = tv_y + tv_c;
		tv_c += this->m_option.chrom_level << 1;
		tools.setvalue(buf  + pos, tv_cvbs, this->m_unit);
		pos += this->m_unit;
		this->m_option.thta += (flag) ? this->m_option.subfreq : 0;
	}

	tv_y = (int)(this->m_option.sync_level * 0.25);
	tv_c = 0;
	tv_cvbs = tv_y + tv_c;
	tv_c += this->m_option.chrom_level << 1;
	tools.setvalue(buf + pos, tv_cvbs, this->m_unit);
	pos += this->m_unit;
	this->m_option.thta += (flag) ? this->m_option.subfreq : 0;

	tv_y = (int)(this->m_option.sync_level * 0.5);
	tv_c = 0;
	tv_cvbs = tv_y + tv_c;
	tv_c += this->m_option.chrom_level << 1;
	tools.setvalue(buf + pos, tv_cvbs, this->m_unit);
	pos += this->m_unit;
	this->m_option.thta += (flag) ? this->m_option.subfreq : 0;

	tv_y = (int)(this->m_option.sync_level * 0.75);
	tv_c = 0;
	tv_cvbs = tv_y + tv_c;
	tv_c += this->m_option.chrom_level << 1;
	tools.setvalue(buf + pos, tv_cvbs, this->m_unit);
	pos += this->m_unit;
	this->m_option.thta += (flag) ? this->m_option.subfreq : 0;

	for (i = 0; i < param2; i++)
	{
		tv_y = this->m_option.sync_level;
		tv_c = 0;
		tv_cvbs = tv_y + tv_c;
		tv_c += this->m_option.chrom_level << 1;
		tools.setvalue(buf  + pos, tv_cvbs, this->m_unit);
		pos += this->m_unit;
		this->m_option.thta += (flag) ? this->m_option.subfreq : 0;
	}
	return pos;
}

int C702Composite::PalProc(char *src, char *dst)
{
	int m, n, pos = 0, pos1 = 0, size1, yuv[3];
	int sin_phai, cos_phai;	
	int	line_phase = 1;
	int tv_y, tv_c, tv_cvbs;
	CBaseConvert tools;
	int shift;
	
	//shift = this->m_interlace;
	shift = 1;
	size1 = this->m_width * this->m_height * 3 * this->m_unit;
	this->m_option.subfreq = 705268427;
	this->m_option.thta = 1383614425;		
	
	for (m = 0; m < this->m_option.vs_pulse; m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.vs_duration - 3, 
					(this->m_option.tv_colmax >> shift) - this->m_option.vs_duration - 3, 0);
	}
	for (m = 0; m < this->m_option.ve2_pulse; m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.ve_duration - 3,
					(this->m_option.tv_colmax >> shift) - this->m_option.ve_duration - 3, 0);
	}
	//full line generation
	for (m = 0; m < ((this->m_option.tv_rowmax - this->m_option.vs_pulse - this->m_option.ve1_pulse - this->m_option.ve2_pulse) >> shift); m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.hs_duration - 3,
					this->m_option.burst_start - this->m_option.hs_duration - 3, 1);

		for (n = 0; n < (this->m_option.burst_end - this->m_option.burst_start); n++)
		{
			this->GetSinCos(&sin_phai, &cos_phai);

			tv_y = this->m_option.sync_level;
			if ((m == 0) || (m == ( ((this->m_option.tv_rowmax - this->m_option.vs_pulse
										- this->m_option.ve1_pulse - this->m_option.ve2_pulse) >> shift) - 1 )))		// ?????  here is || or | ?
				tv_c = 0;
			else
				/*tv_c = ((-1) * sin_phai * this->m_option.burst_level * 4 / 512 +
						(-1) * line_phase * cos_phai * this->m_option.burst_level * 4 / 512) >> 3;*/
				//tv_c = ( (-sin_phai - cos_phai * line_phase) * this->m_option.burst_level ) >> 10;
				tv_c = (-sin_phai * this->m_option.burst_level / 128 - line_phase * cos_phai * this->m_option.burst_level / 128) >> 3;

			tv_cvbs = tv_y + tv_c;
			tv_c += this->m_option.chrom_level << 1;
			tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
			pos += this->m_unit;
			this->m_option.thta += this->m_option.subfreq;
		}
		
		pos += this->SubProc(dst + pos, this->m_option.data_start - this->m_option.burst_end);	

		if ( m < ((this->m_option.vb_pulse - this->m_option.vs_pulse - this->m_option.ve2_pulse) >> shift) )
		{			
			pos += this->SubProc(dst + pos, this->m_option.tv_colmax - this->m_option.data_start);
		}
		else
		{
			pos1 = this->m_width * ( this->m_option.row_delta + 
								(m - ((this->m_option.vb_pulse - this->m_option.vs_pulse - this->m_option.ve2_pulse) >> shift)) );
			pos1 *= 3 * this->m_unit;
			this->m_linebuf.Write(src + pos1, size1);
			this->m_linebuf.ClearPoint();

			for (n = 0; n < (this->m_option.tv_colmax - this->m_option.data_start - this->m_option.front_porch); n++)
			{
				if ( (this->m_option.vb_pulse % 2 == 1) && 
					(m == ((this->m_option.vb_pulse - this->m_option.vs_pulse - this->m_option.ve2_pulse) >> shift)) &&
					(n < (this->m_option.tv_colmax >> 1)) )
				{
					tv_y = this->m_option.sync_level;
					tv_c = 0;
					tv_cvbs = tv_y + tv_c;
					tv_c += this->m_option.chrom_level << 1;
					tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
					pos += this->m_unit;
					this->m_option.thta += this->m_option.subfreq;
				}
				else
				{
					this->GetSinCos(&sin_phai, &cos_phai);

					this->m_linebuf.Read(n + this->m_option.col_delta, yuv);
					yuv[1] = (yuv[1] << 16) >> 16;
					yuv[2] = (yuv[2] << 16) >> 16;
					yuv[1] = yuv[1] >= 2048 ? (2048 - yuv[1]) : yuv[1];
					yuv[2] = yuv[2] >= 2048 ? (2048 - yuv[2]) : yuv[2];

					tv_y = ((this->m_option.sync_level + this->m_option.black_level) << 2) + yuv[0];
					//tv_c = (sin_phai * yuv[1] - line_phase * cos_phai * yuv[2]) >> 10;
					tv_c = (sin_phai * yuv[1] /512 - line_phase * cos_phai *yuv[2] / 512) >> 1;
					tv_cvbs = (tv_y + tv_c) >> 2;

					tv_y >>= 2;
					tv_c >>= 2;
					tv_c += this->m_option.chrom_level << 1;
					tv_c = (tv_c >= 1023) ? 1023 : (tv_c < 0 ? 0 : tv_c);
					tv_cvbs = (tv_cvbs >= 1023) ? 1023 : (tv_cvbs < 0 ? 0 : tv_cvbs);
					tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
					pos += this->m_unit;
					this->m_option.thta += this->m_option.subfreq;
				}
			}			
			pos += this->SubProc(dst + pos, this->m_option.front_porch);
		}
		if ( m < (((this->m_option.tv_rowmax - this->m_option.vs_pulse - 
						this->m_option.ve1_pulse - this->m_option.ve2_pulse) >> shift) - 2) )
			line_phase = -line_phase;
	}

	if (this->m_option.first_field == 1)
	{
		pos += this->SyncProc(dst + pos, this->m_option.hs_duration - 3,
					this->m_option.burst_start - this->m_option.hs_duration - 3, 1);

		for (n = 0; n < (this->m_option.burst_end - this->m_option.burst_start); n++)
		{
			this->GetSinCos(&sin_phai, &cos_phai);

			tv_y = this->m_option.sync_level;
			//tv_c = ( (-sin_phai - cos_phai * line_phase) * this->m_option.black_level ) >> 10;
			tv_c = (-sin_phai * this->m_option.burst_level / 128 - line_phase * cos_phai * this->m_option.burst_level / 128) >> 3;
			tv_cvbs = tv_y + tv_c;
			tv_c += this->m_option.chrom_level << 1;
			tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
			pos += this->m_unit;
			this->m_option.thta += this->m_option.subfreq;
		}		
		pos += this->SubProc(dst + pos, this->m_option.data_start - this->m_option.burst_end);

		pos1 = this->m_width * ( this->m_option.row_delta + 
			(m - ((this->m_option.vb_pulse - this->m_option.vs_pulse - this->m_option.ve2_pulse) >> shift)) );
		this->m_linebuf.Write(src + pos1, size1);
		this->m_linebuf.ClearPoint();
		for (n = 0; n < ((this->m_option.tv_colmax >> shift) - this->m_option.data_start - this->m_option.front_porch); n++)
		{			
			this->GetSinCos(&sin_phai, &cos_phai);

			this->m_linebuf.Read(n + this->m_option.col_delta, yuv);
			tv_y = ((this->m_option.sync_level + this->m_option.black_level) << 2) + yuv[0];
			this->m_linebuf.Read(n + this->m_option.col_delta - 2, yuv);
			yuv[1] = (yuv[1] << 16) >> 16;
			yuv[2] = (yuv[2] << 16) >> 16;
			yuv[1] = yuv[1] >= 2048 ? (2048 - yuv[1]) : yuv[1];
			yuv[2] = yuv[2] >= 2048 ? (2048 - yuv[2]) : yuv[2];

			//tv_c = (sin_phai * yuv[1] - line_phase * cos_phai * yuv[2]) >> 10;
			tv_c = (sin_phai * yuv[1] /512 - line_phase * cos_phai *yuv[2] / 512) >> 1;
			tv_cvbs = (tv_y + tv_c) >> 2;

			tv_y >>= 2;
			tv_c >>= 2;
			tv_c += this->m_option.chrom_level << 1;
			tv_c = (tv_c >= 1023) ? 1023 : (tv_c < 0 ? 0 : tv_c);
			tv_cvbs = (tv_cvbs >= 1023) ? 1023 : (tv_cvbs < 0 ? 0 : tv_cvbs);
			tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
			pos += this->m_unit;
			this->m_option.thta += this->m_option.subfreq;
		}
		pos += this->SubProc(dst + pos, this->m_option.front_porch);
	}

	for (m = 0; m < this->m_option.ve1_pulse; m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.ve_duration - 3,
				(this->m_option.tv_colmax >> 1) - this->m_option.ve_duration - 3, 0);
	}
	return pos;
}


int C702Composite::NtscProc(char *src, char *dst)
{
	int m, n, pos = 0, pos1 = 0, size1, yuv[3];
	int sin_phai, cos_phai;	
	int	line_phase = 1;
	int tv_y, tv_c, tv_cvbs;
	CBaseConvert tools;
	int shift;

	//shift = this->m_interlace;
	shift = 1;
	size1 = this->m_width * this->m_height * 3 * this->m_unit;
	this->m_option.subfreq = 569408471;
	this->m_option.thta = 3593535513;

	for (m = 0; m < this->m_option.vs_pulse; m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.vs_duration - 3, 
			(this->m_option.tv_colmax >> shift) - this->m_option.vs_duration - 3, 0);
	}
	
	for (m = 0; m < this->m_option.ve2_pulse; m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.ve_duration - 3,
			(this->m_option.tv_colmax >> shift) - this->m_option.ve_duration - 3, 0);
	}

	//full line generation
	for (m = 0; m < ((this->m_option.tv_rowmax - this->m_option.vs_pulse - this->m_option.ve1_pulse - this->m_option.ve2_pulse) >> shift); m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.hs_duration - 3,
			this->m_option.burst_start - this->m_option.hs_duration - 3, 1);
	
		for (n = 0; n < (this->m_option.burst_end - this->m_option.burst_start); n++)
		{
			this->GetSinCos(&sin_phai, &cos_phai);
			tv_y = this->m_option.sync_level;
			tv_c = (-sin_phai * this->m_option.burst_level / 128) >> 3;
			tv_cvbs = tv_y + tv_c;
			tv_c += this->m_option.chrom_level << 1;
			tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
			pos += this->m_unit;
			this->m_option.thta += this->m_option.subfreq;
		}
        
		pos += this->SubProc(dst + pos, this->m_option.data_start - this->m_option.burst_end);
		
		if ( m < ((this->m_option.vb_pulse - this->m_option.vs_pulse - this->m_option.ve2_pulse) >> shift) )
		{
			pos += this->SubProc(dst + pos, this->m_option.tv_colmax - this->m_option.data_start);
		}
		else
		{
			pos1 = this->m_width * ( this->m_option.row_delta + 
				(m - ((this->m_option.vb_pulse - this->m_option.vs_pulse - this->m_option.ve2_pulse) >> shift)) );
			pos1 *= 3 * this->m_unit;
			this->m_linebuf.Write(src + pos1, size1);
			this->m_linebuf.ClearPoint();

			for (n = 0; n < (this->m_option.tv_colmax - this->m_option.data_start - this->m_option.front_porch); n++)
			{
				this->GetSinCos(&sin_phai, &cos_phai);
				this->m_linebuf.Read(n + this->m_option.col_delta, yuv);
				yuv[1] = (yuv[1] << 16) >> 16;
				yuv[2] = (yuv[2] << 16) >> 16;
				yuv[1] = yuv[1] >= 2048 ? (2048 - yuv[1]) : yuv[1];
				yuv[2] = yuv[2] >= 2048 ? (2048 - yuv[2]) : yuv[2];

				tv_y = ((this->m_option.sync_level + this->m_option.black_level) << 2) + yuv[0];
				tv_y = tv_y > 4095 ? 4095 : tv_y;
				tv_c = (sin_phai * yuv[1] / 512 + cos_phai * yuv[2] / 512) >> 1;
				tv_c = tv_c > 2047 ? 2047 : (tv_c < -2048 ? -2048 : tv_c);

				tv_cvbs = (tv_y + tv_c) >> 2;
				tv_y >>= 2;
				tv_c >>= 2;
				tv_c += this->m_option.chrom_level << 1;
				tv_c = (tv_c >= 1023) ? 1023 : (tv_c < 0 ? 0 : tv_c);
				tv_cvbs = (tv_cvbs >= 1023) ? 1023 : (tv_cvbs < 0 ? 0 : tv_cvbs);
				tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
				pos += this->m_unit;
				this->m_option.thta += this->m_option.subfreq;
			}
			pos += this->SubProc(dst + pos, this->m_option.front_porch);
		}		
	}

	if (this->m_option.first_field == 1)
	{
		pos += this->SyncProc(dst + pos, this->m_option.hs_duration - 3,
			this->m_option.burst_start - this->m_option.hs_duration - 3, 1);

		for (n = 0; n < (this->m_option.burst_end - this->m_option.burst_start); n++)
		{
			this->GetSinCos(&sin_phai, &cos_phai);
			tv_y = this->m_option.sync_level;
			tv_c = (-sin_phai * this->m_option.burst_level / 128) >> 3;
			tv_cvbs = tv_y + tv_c;
			tv_c += this->m_option.chrom_level << 1;
			tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
			pos += this->m_unit;
			this->m_option.thta += this->m_option.subfreq;
		}
		pos += this->SubProc(dst + pos, this->m_option.data_start - this->m_option.burst_end);

		pos1 = this->m_width * ( this->m_option.row_delta + 
			(m - ((this->m_option.vb_pulse - this->m_option.vs_pulse - this->m_option.ve2_pulse) >> shift)) );
		this->m_linebuf.Write(src + pos1, size1);
		this->m_linebuf.ClearPoint();

		for (n = 0; n < ((this->m_option.tv_colmax >> shift) - this->m_option.data_start - this->m_option.front_porch); n++)
		{
			this->GetSinCos(&sin_phai, &cos_phai);
			this->m_linebuf.Read(n + this->m_option.col_delta, yuv);
			yuv[1] = (yuv[1] << 16) >> 16;
			yuv[2] = (yuv[2] << 16) >> 16;
			yuv[1] = yuv[1] >= 2048 ? (2048 - yuv[1]) : yuv[1];
			yuv[2] = yuv[2] >= 2048 ? (2048 - yuv[2]) : yuv[2];

			tv_y = ((this->m_option.sync_level + this->m_option.black_level) << 2) + yuv[0];
			tv_y = tv_y > 4095 ? 4095 : tv_y;
			tv_c = (sin_phai * yuv[1] / 512 + cos_phai * yuv[2] / 512) >> 1;
			tv_c = tv_c > 2047 ? 2047 : (tv_c < -2048 ? -2048 : tv_c);

			tv_cvbs = (tv_y + tv_c) >> 2;
			tv_y >>= 2;
			tv_c >>= 2;
			tv_c += this->m_option.chrom_level << 1;
			tv_c = (tv_c >= 1023) ? 1023 : (tv_c < 0 ? 0 : tv_c);
			tv_cvbs = (tv_cvbs >= 1023) ? 1023 : (tv_cvbs < 0 ? 0 : tv_cvbs);
			tools.setvalue(dst + pos, tv_cvbs, this->m_unit);
			pos += this->m_unit;
			this->m_option.thta += this->m_option.subfreq;
		}
		pos += this->SubProc(dst + pos, this->m_option.front_porch);
	}

	for (m = 0; m < this->m_option.ve1_pulse; m++)
	{
		pos += this->SyncProc(dst + pos, this->m_option.ve_duration - 3,
			(this->m_option.tv_colmax >> 1) - this->m_option.ve_duration - 3, 0);
	}	
	return pos;
}







