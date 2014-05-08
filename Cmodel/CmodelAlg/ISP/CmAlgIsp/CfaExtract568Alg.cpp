
#include "internal.h"

static char gCoef[25] = {
	0,  0,  0,  0,  0, 
		0, -1, -1, -1,  0,
		0, -1,  8, -1,  0,
		0, -1, -1, -1,  0,
		0,  0,  0 , 0,  0
};

CCfaExtract568Alg::CCfaExtract568Alg()
{
	this->Init();
}

CCfaExtract568Alg::~CCfaExtract568Alg()
{
}

void CCfaExtract568Alg::Init(void)
{
	int i;
	this->m_foreRead5line = 0;
	this->m_pos = 0;

	this->SetMode(0);
	for(i = 0; i < 9; i++)
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	for(i = 0; i < 5; i++)
		this->m_pGblock[i] = this->m_Gblock[i];

	this->SetBGLine(0x1);
	this->SetGpixel(0x1);
	this->SetFemm(0x40);
	this->SetFemp(0x60);
	this->SetFemmax(0xff);
	this->SetFemmin(0x05);
	this->SetFemx1(0x05);
	this->SetFemx2(0x0f);
	this->SetCoefEdge(gCoef);
	this->SetDivEdge(0x8);

}


int CCfaExtract568Alg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CCfaExtract568Alg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->SetSize(m_width, m_height, m_unit);

	if (this->m_fmt == (RAW_GB | (VIDEOCLASS << CLASSSHIFT)) ) 
	{
		this->SetBGLine(0x1);
		this->SetGpixel(0x1);
	}
	else
	{
		this->SetBGLine(0);
		this->SetGpixel(0);
	}
}

int CCfaExtract568Alg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->unit = this->m_unit;
	switch(index) 
	{
	case 0:
		cfg->fmt = RAW_RGB | (VIDEOCLASS << CLASSSHIFT);
		break;
	case 1:
		cfg->fmt = EDGE_MAP | (VIDEOCLASS << CLASSSHIFT);
		break;
	default:
		cfg->fmt = RAW_RGB | (VIDEOCLASS << CLASSSHIFT);
	}
	return 0;
}


void CCfaExtract568Alg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_foreRead5line = 0;
	this->m_pos = 0;
	int i;
	for(i = 0; i < 9; i++)
	{
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
		this->m_pLinebuf[i]->ClearPoint();
	}
	m_LinebufEdge.ClearPoint();
}

void CCfaExtract568Alg::SetSize(int width, int height, int unit)
{
	int i;

	for(i = 0; i < 9; i++)
		this->m_Linebuf[i].SetSize(width, 0, unit);
	this->m_LinebufEdge.SetSize(width, 0, unit);
	this->m_unit = unit;
	this->m_width = width;
	this->m_height = height;
}

void CCfaExtract568Alg::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

void CCfaExtract568Alg::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int CCfaExtract568Alg::GetBGLine()
{
	return this->m_bgline;
}

int CCfaExtract568Alg::GetGpixel()
{
	return this->m_gpixel;
}


int	CCfaExtract568Alg::Write(int index, char *buf, int len)
{
	index = index;

	if(len == 0)
		return 0;

	int nLength = 0;	
	if (this->m_foreRead5line < 5) 
	{
		nLength = this->m_pLinebuf[this->m_foreRead5line+4]->Write(buf, len);
	}
	else
	{
		nLength = this->m_pLinebuf[8]->Write(buf, len);

	}	
	return nLength;
}

int	CCfaExtract568Alg::Read(int index, char *buf, int len)
{
	switch(index) 
	{
	case 0:
		if (this->m_foreRead5line < 5)
		{
			if (this->m_pLinebuf[this->m_foreRead5line+4]->IsFull()) 
			{
				m_foreRead5line ++;
			}
		}
		if (this->m_foreRead5line < 5 && this->m_height > 5) return 0;
		if (len < 1 || !this->m_pLinebuf[4]->IsFull()) return 0;

		this->LastLinePro();
		len = this->Interpolate(buf, len);
		if (len > 0) this->m_outline++;
		this->LoopLinebuf();
		break;
	case 1:
		if (this->m_foreRead5line < 5 && this->m_height > 5) return 0;
		if (len < 1 || !this->m_pLinebuf[4]->IsFull()) return 0;

		len =this->m_LinebufEdge.Read(buf, len);
		if (len  > 0) 
		{
			this->m_LinebufEdge.ClearPoint();
		}
		break;
	default:
		if (this->m_foreRead5line < 5)
		{
			if (this->m_pLinebuf[this->m_foreRead5line+4]->IsFull()) 
			{
				m_foreRead5line ++;
			}
		}
		if (this->m_foreRead5line < 5 && this->m_height > 5) return 0;
		if (len < 1 || !this->m_pLinebuf[4]->IsFull()) return 0;

		this->LastLinePro();
		len = this->Interpolate(buf, len);
		if (len > 0) this->m_outline++;
		this->LoopLinebuf();
	}
	this->m_pos %= this->m_width;
	return len;
}

int	CCfaExtract568Alg::Interpolate(char *buf, int len)
{
	int gpixel, val[3], i, pos = 0, max = 255;
	CBaseConvert tools;

	if(this->m_unit == 2)
		max = 1024;
	if(!this->m_mode)		//cfa
		len /= 3*this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	gpixel = (this->m_gpixel + this->m_pos) % 2;
	for(; this->m_pos < len; this->m_pos++)
	{
		if(this->m_pos == 0)
			this->GetFirstGblock();
		else
			this->GetNextGblock();
		
		//if(this->m_mode == 1) //old edge
			val[0] = this->GetEdge(gpixel);
		//else		//new edge
		//	val[0] = this->GetEdge5x5(gpixel);
		
		this->m_LinebufEdge.Write((char*)&val[0], 2);

		if(this->m_bgline)
		{
			if(gpixel)
			{
				val[0] = this->GetRfromGB(this->m_pos);
				val[1] = this->m_pGblock[2][2];
				val[2] = this->GetRfromGR(this->m_pos);
			}
			else
			{
				val[0] = this->GetRfromBG(this->m_pos); 
				val[1] = this->m_pGblock[2][2];
				val[2] = this->m_pLinebuf[4]->Read(this->m_pos);
			}
		}
		else
		{
			if(gpixel)
			{
				val[0] = this->GetRfromGR(this->m_pos); 
				val[1] = this->m_pGblock[2][2];
				val[2] = this->GetRfromGB(this->m_pos);
			}
			else
			{
				val[0] = this->m_pLinebuf[4]->Read(this->m_pos);
				val[1] = this->m_pGblock[2][2];
				val[2] = this->GetRfromBG(this->m_pos); 
			}
		}
		gpixel = gpixel ? 0 : 1;
		for(i = 0; i <= 2; i++, pos += this->m_unit)
		{
			val[i] = (val[i] < 0) ? 0 : ((val[i] > max) ? max : val[i]);
			tools.setvalue(buf+pos, val[i], this->m_unit);
		}
		
	}
	return pos;
}

int	CCfaExtract568Alg::GetRfromBG(int pos)		//GB line, get R at B pixel
{
	int x1, x2;
	int max = 255;

	if(this->m_outline < 4)
	{
		x1 = this->m_pLinebuf[5]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos-1)) + 
			this->m_pLinebuf[5]->Read(this->EdgeCol(1+pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(1+pos));
	}
	else if(this->m_outline > this->m_height -5)
	{
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[3]->Read(this->EdgeCol(pos-1)) + 
			this->m_pLinebuf[3]->Read(this->EdgeCol(1+pos)) + this->m_pLinebuf[3]->Read(this->EdgeCol(1+pos));
	}
	else
	{
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos-1)) + 
			this->m_pLinebuf[3]->Read(this->EdgeCol(1+pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(1+pos));
	}
	if(this->m_pos < 2)		//hx: hw implement
		x2 = this->m_pGblock[3][3] + this->m_pGblock[3][3] + 
		this->m_pGblock[3][3] + this->m_pGblock[3][3];
	else if(this->m_pos < 4 && this->m_pos >=2)
		x2 = this->m_pGblock[3][3] + this->m_pGblock[3][1] + 
		this->m_pGblock[3][3] + this->m_pGblock[3][1];
	else if(this->m_pos > this->m_width -3)
		x2 = this->m_pGblock[1][3] + this->m_pGblock[1][1] + 
		this->m_pGblock[1][3] + this->m_pGblock[1][1];
	else
		x2 = this->m_pGblock[1][1] + this->m_pGblock[3][3] + 
		this->m_pGblock[1][3] + this->m_pGblock[3][1];

	x1 = (this->m_pGblock[2][2]<<2) + x1 - x2;
	x1 >>= 2;

	if(this->m_unit == 2)
		max = 1023;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

int	CCfaExtract568Alg::GetRfromGR(int pos)		//GR line, get R at G pixel
{
	int x1, x2;
	int max = 255;

	x1 = this->m_pLinebuf[4]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[4]->Read(this->EdgeCol(1+pos));
	if(this->m_pos < 4)
		x2 = this->m_pGblock[3][2] + this->m_pGblock[3][2];
	else if(this->m_pos > this->m_width -3)
		x2 = this->m_pGblock[1][2] + this->m_pGblock[1][2];
	else
		x2 = this->m_pGblock[1][2] + this->m_pGblock[3][2];

	x1 = (this->m_pGblock[2][2]<<1) + x1 - x2;
	x1 >>= 1;

	if(this->m_unit == 2)
		max = 1023;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

int CCfaExtract568Alg::GetRfromGB(int pos)		//GB line, get R at G pixel
{
	int x1, x2;
	int max = 255;

	if(this->m_outline < 4)
		x1 = this->m_pLinebuf[5]->Read(this->EdgeCol(pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos));
	else if(this->m_outline > this->m_height -5)
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos)) + this->m_pLinebuf[3]->Read(this->EdgeCol(pos));
	else
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos));
	if(this->m_pos < 3 )			//hx: hw implement
		x2 = this->m_pGblock[2][3] + this->m_pGblock[2][3];
	else
		x2 = this->m_pGblock[2][1] + this->m_pGblock[2][3];
	//	x2 = this->m_pGblock[2][3] + this->m_pGblock[2][3];

	x1 = (this->m_pGblock[2][2]<<1) + x1 - x2;
	x1 >>= 1;

	if(this->m_unit == 2)
		max = 1023;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

void CCfaExtract568Alg::LastLinePro(void)
{
	if(this->m_pLinebuf[8]->IsFull())
		return;
	this->m_pLinebuf[8] = this->m_pLinebuf[6];
}

void CCfaExtract568Alg::LoopLinebuf(void)
{
	int i;
	CLineBayerBuf *p = this->m_pLinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	for(i = 0; i < 8; i++)
		this->m_pLinebuf[i] = this->m_pLinebuf[i+1];
	this->m_pLinebuf[8] = p;
	this->m_pLinebuf[8]->ClearPoint();
	this->m_bgline = this->m_bgline ? 0 : 1;
	this->m_gpixel = this->m_gpixel ? 0 : 1;
	//this->m_outline++;
}

int CCfaExtract568Alg::EdgeLine(int num)
{
	if((this->m_outline < 4 && num < 4) || (this->m_outline > this->m_height -5 && num > 4))
		return 8-num;
	else
		return num;
}

int CCfaExtract568Alg::EdgeCol(int num)
{
	if((this->m_pos < 4 && num < this->m_pos))
		return 2*this->m_pos - num;
	else if(this->m_pos > this->m_width -5 && this->m_pos < this->m_width -2 && num > this->m_width - 2)
		return 2*this->m_pos - num + 4;
	else if(this->m_pos > this->m_width-3)
	{
		if(num == this->m_pos +1 || num == this->m_pos +2)
			return 2*this->m_pos - num;
		else if(num == this->m_pos +3 || num == this->m_pos +4)
			return this->m_width -1;
		else
			return num;
	}
	else
		return num;
}

int CCfaExtract568Alg::GetGpixel(int col, int row)
{
	int dh, dv, x1, x2;
	int max = 255;

	if( (this->m_gpixel + col + row) % 2 )	
		return this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row));

	x1 = this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-1)) - this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+1));
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-2)) + this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+2)) - 
		(this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dh = (x1 << 1) + x2;

	x1 = this->m_pLinebuf[this->EdgeLine(col-1)]->Read(this->EdgeCol(row)) - this->m_pLinebuf[this->EdgeLine(col+1)]->Read(this->EdgeCol(row));
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[this->EdgeLine(col-2)]->Read(this->EdgeCol(row)) + this->m_pLinebuf[this->EdgeLine(col+2)]->Read(this->EdgeCol(row)) - 
		(this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dv = (x1 << 1) + x2;

	if(dh < dv)
	{
		x1 = (this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-1)) + this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+1))) << 2;
		x2 = (this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1) - this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-2)) - 
			this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+2));
	}
	else
	{
		x1 = (this->m_pLinebuf[this->EdgeLine(col-1)]->Read(this->EdgeCol(row)) + this->m_pLinebuf[this->EdgeLine(col+1)]->Read(this->EdgeCol(row))) << 2;
		x2 = (this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1) - this->m_pLinebuf[this->EdgeLine(col-2)]->Read(this->EdgeCol(row)) - 
			this->m_pLinebuf[this->EdgeLine(col+2)]->Read(this->EdgeCol(row));
	}
	x1 += x2;
	x1 >>= 3;
	if(this->m_unit == 2)
		max = 1023;
	//	return (x1 > max) ? max : x1;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

void CCfaExtract568Alg::GetFirstGblock(void)
{
	int i, j;

	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			this->m_pGblock[i][j] = this->GetGpixel(j+2, i-2);
}

void CCfaExtract568Alg::GetNextGblock(void)
{
	int i;
	int *p = this->m_pGblock[0];

	for(i = 0; i < 5; i++)
		this->m_pGblock[0][i] = this->GetGpixel(i+2, this->m_pos+2);
	for(i = 0; i < 4; i++)
		this->m_pGblock[i] = this->m_pGblock[i+1];
	this->m_pGblock[4] = p;
}

//edge extract
void CCfaExtract568Alg::SetMode(int mode)
{
	this->m_mode = mode&0x3;
}

int CCfaExtract568Alg::GetMode(void)
{
	return this->m_mode;
}

void CCfaExtract568Alg::SetFemm(int femm)
{
	this->m_femm = femm&0xff;
}

int CCfaExtract568Alg::GetFemm(void)
{
	return this->m_femm;
}

void CCfaExtract568Alg::SetFemp(int femp)
{
	this->m_femp = femp&0xff;
}

int CCfaExtract568Alg::GetFemp(void)
{
	return this->m_femp;
}

void CCfaExtract568Alg::SetFemmin(int femmin)
{
	this->m_femmin = femmin&0xff;
}

int CCfaExtract568Alg::GetFemmin(void)
{
	return this->m_femmin;
}

void CCfaExtract568Alg::SetFemmax(int femmax)
{
	this->m_femmax = femmax&0xff;
}

int CCfaExtract568Alg::GetFemmax(void)
{
	return this->m_femmax;
}

void CCfaExtract568Alg::SetFemx1(int femx1)
{
	this->m_femx1 = femx1&0xff;
}

int CCfaExtract568Alg::GetFemx1(void)
{
	return this->m_femx1;
}

void CCfaExtract568Alg::SetFemx2(int femx2)
{
	this->m_femx2 = femx2&0xff;
}

int CCfaExtract568Alg::GetFemx2(void)
{
	return this->m_femx2;
}

void CCfaExtract568Alg::SetCoefEdge(char *coef)
{
	char i, j;

	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			this->m_coefedge[i][j] = *(coef+i*5+j);
}

char* CCfaExtract568Alg::GetCoefEdge()
{
	return &this->m_coefedge[0][0];
}

void CCfaExtract568Alg::SetDivEdge(int div)
{
	this->m_divedge = div&0xff;
}

int CCfaExtract568Alg::GetDivEdge()
{
	return this->m_divedge;
}

int CCfaExtract568Alg::GetEdge(int gpixel)
{
	int x, temp;
	int femm, femp, femmin, femmax, femx1, femx2;
	char ret;
	int flag;

	femm = this->m_femm;
	femp = this->m_femp;
	femx1 = this->m_femx1;
	femx2 = this->m_femx2;
	femmin = this->m_femmin;
	femmax = this->m_femmax;

	x = this->m_pGblock[2][2] << 3;
	if(gpixel)
	{
		if(this->m_pos < 4)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2 || this->m_pos == this->m_width-1)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}

	}
	else
	{
		if(this->m_pos == 0)
		{
			this->m_pGblock[4][0] = this->m_pGblock[4][4];
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
		}	
		//	temp = 4*this->m_pGblock[4][4] + 2*this->m_pGblock[2][4] + 2*this->m_pGblock[4][2];
		else if(this->m_pos == 1)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
		}
		else if(this->m_pos == 2 || this->m_pos == 3)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}
		else if(this->m_pos == this->m_width-1)
			return 0;
	}
	temp = this->m_pGblock[0][0] + this->m_pGblock[0][2] + this->m_pGblock[0][4] +
		this->m_pGblock[2][0] + this->m_pGblock[2][4] + this->m_pGblock[4][0] +
		this->m_pGblock[4][2] + this->m_pGblock[4][4];
	if(x >= temp)
	{
		flag = 0;
		x -= temp;
	}
	else
	{
		flag = 1;
		x = temp - x;
	}
	x >>= 3;

	if(x > femx2)
		x *= femm;
	else if( (x > femx1) && (x <= femx2) )
	{
		x -= femx1;
		x *= femp;
	}
	else
	{
		x = 0;
	}

	x /= 16;
	if(flag)
		x = (~x+1);
	x >>= 2;

	ret = (char)( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x));
	//if(ret >= 0)
	if((!flag) || (ret == 0))
		return ((int)ret & 0xff);
	else
		return (((int)ret & 0xff) | 0x100);

}

//New 3x3 edge enhance
int CCfaExtract568Alg::GetEdge5x5(int gpixel)
{
	int x, temp;
	int femm, femp, femmin, femmax, femx1, femx2;
	char ret;
	int flag;

	femm = this->m_femm;
	femp = this->m_femp;
	femx1 = this->m_femx1;
	femx2 = this->m_femx2;
	femmin = this->m_femmin;
	femmax = this->m_femmax;

	if(gpixel)
	{
		if(this->m_pos < 4)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2 || this->m_pos == this->m_width-1)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}
	}
	else
	{
		if(this->m_pos == 0)
		{
			this->m_pGblock[4][0] = this->m_pGblock[4][4];
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
		}	
		else if(this->m_pos == 1)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
		}
		else if(this->m_pos == 2 || this->m_pos == 3)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}
		else if(this->m_pos == this->m_width-1)
			return 0;
	}
	temp =  this->m_pGblock[0][0]*this->m_coefedge[0][0] + this->m_pGblock[0][1]*this->m_coefedge[0][1] +
		this->m_pGblock[0][2]*this->m_coefedge[0][2] + this->m_pGblock[0][3]*this->m_coefedge[0][3] +
		this->m_pGblock[0][4]*this->m_coefedge[0][4] + this->m_pGblock[1][0]*this->m_coefedge[1][0] +
		this->m_pGblock[1][1]*this->m_coefedge[1][1] + this->m_pGblock[1][2]*this->m_coefedge[1][2] +
		this->m_pGblock[1][3]*this->m_coefedge[1][3] + this->m_pGblock[1][4]*this->m_coefedge[1][4] +
		this->m_pGblock[2][0]*this->m_coefedge[2][0] + this->m_pGblock[2][1]*this->m_coefedge[2][1] +
		this->m_pGblock[2][2]*this->m_coefedge[2][2] + this->m_pGblock[2][3]*this->m_coefedge[2][3] +
		this->m_pGblock[2][4]*this->m_coefedge[2][4] + this->m_pGblock[3][0]*this->m_coefedge[3][0] +
		this->m_pGblock[3][1]*this->m_coefedge[3][1] + this->m_pGblock[3][2]*this->m_coefedge[3][2] +
		this->m_pGblock[3][3]*this->m_coefedge[3][3] + this->m_pGblock[3][4]*this->m_coefedge[3][4] +
		this->m_pGblock[4][0]*this->m_coefedge[4][0] + this->m_pGblock[4][1]*this->m_coefedge[4][1] +
		this->m_pGblock[4][2]*this->m_coefedge[4][2] + this->m_pGblock[4][3]*this->m_coefedge[4][3] +
		this->m_pGblock[4][4]*this->m_coefedge[4][4];
	if(temp >= 0)
	{
		flag = 0;
		x = temp;
	}
	else
	{
		flag = 1;
		x = -temp;
	}
	//x >>= 3;
	x /= this->m_divedge;

	if(x > femx2)
		x *= femm;
	else if( (x > femx1) && (x <= femx2) )
	{
		x -= femx1;
		x *= femp;
	}
	else
	{
		x = 0;
	}

	x /= 16;
	if(flag)
		x = (~x+1);
	x >>= 2;

	ret = (char)( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x));
	//if(ret >= 0)
	if((!flag) || (ret == 0))
		return ((int)ret & 0xff);
	else
		return (((int)ret & 0xff) | 0x100);

}

