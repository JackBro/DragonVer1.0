
#include "internal.h"

//============================
//
//		class C5x5Base
//
//===============================
C5x5Base::C5x5Base(int width, int unit)
{
	int i;

	this->SetSize(width, unit);
	for(i = 0; i < 9; i++)
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	for(i = 0; i < 5; i++)
		this->m_pGblock[i] = this->m_Gblock[i];
	this->m_GBline = 1;
	this->m_Gfirst = 1;
}

C5x5Base::~C5x5Base(void)
{
}

void C5x5Base::SetSize(int width, int unit)
{
	int i;

	for(i = 0; i < 9; i++)
		this->m_Linebuf[i].SetSize(width, 4, unit);
	this->m_unit = unit;
	this->m_width = width;
	this->Clear();
}

void C5x5Base::SetOrder(int order)
{
	const int setting[][2] =
	{
		{ 1, 0 },	//	RAW_BG,
		{ 1, 1 },	//	RAW_GB,
		{ 0, 0 },	//	RAW_RG,
		{ 0, 1 } 	//	RAW_GR,
	};
	order = ((order >= RAW_GR) && (order <= RAW_BG)) ? (order-RAW_BG) : 0;
	this->m_Gfirst = setting[order][1];
	this->m_GBline = setting[order][0];
	this->Clear();
}

void C5x5Base::Clear(void)
{
	this->m_line = 0;
	this->m_pos = 0;
}

int	C5x5Base::Write(char *buf, int len)
{
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST5X5LINES)
		return 0;
	if(this->m_line == 5)
		return this->m_pLinebuf[8]->Write(buf, len);
	while(this->m_line < 5)
	{
		pos1 = this->m_pLinebuf[4+this->m_line]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		if(pos1 == 0)
			this->m_line++;
		if(len == 0)
			break;
	}
	if(this->m_line == 5)
	{
		this->m_Linebuf[0] = this->m_Linebuf[4];
		this->m_Linebuf[1] = this->m_Linebuf[5];
		this->m_Linebuf[2] = this->m_Linebuf[4];
		this->m_Linebuf[3] = this->m_Linebuf[5];
	}
	return pos;
}

int	C5x5Base::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_pLinebuf[8]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	len = this->process(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len;
}

int	C5x5Base::process(char *buf, int len)
{
	int gpixel, val[3], i, pos = 0, max = 255;
	CBaseConvert tools;

	if(this->m_unit == 2)
		max = 1024;
	len /= 3*this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	gpixel = (this->m_Gfirst + this->m_pos) % 2;
	for(; this->m_pos < len; this->m_pos++)
	{
		if(this->m_pos == 0)
			this->GetFirstGblock();
		else
			this->GetNextGblock();
		if(this->m_GBline)
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
				val[2] = this->m_pLinebuf[4]->Read(this->m_pos+4);
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
				val[0] = this->m_pLinebuf[4]->Read(this->m_pos+4);
				val[1] = this->m_pGblock[2][2];
				val[2] = this->GetRfromBG(this->m_pos); 
			}
		}
		gpixel = gpixel ? 0 : 1;
		for(i = 2; i >= 0; i--, pos += this->m_unit)
		{
			val[i] = (val[i] < 0) ? 0 : ((val[i] > max) ? max : val[i]);
			tools.setvalue(buf+pos, val[i], this->m_unit);
		}
	}
	return pos;
}

int	C5x5Base::GetRfromBG(int pos)		//GB line, get R at B pixel
{
	int x1, x2;

	x1 = this->m_pLinebuf[3]->Read(3+pos) + this->m_pLinebuf[5]->Read(3+pos) + 
		this->m_pLinebuf[3]->Read(5+pos) + this->m_pLinebuf[5]->Read(5+pos);
	x2 = this->m_pGblock[1][1] + this->m_pGblock[3][3] + 
		this->m_pGblock[1][3] + this->m_pGblock[3][1];
	x1 -= x2;
	x1 >>= 2;
	x1 += this->m_pGblock[2][2];
	return x1;
}

int	C5x5Base::GetRfromGR(int pos)		//GR line, get R at G pixel
{
	int x1, x2;

	x1 = this->m_pLinebuf[4]->Read(3+pos) + this->m_pLinebuf[4]->Read(5+pos);
	x2 = this->m_pGblock[1][2] + this->m_pGblock[3][2];
	x1 -= x2;
	x1 >>= 1;
	x1 += this->m_pGblock[2][2];
	return x1;
}

int C5x5Base::GetRfromGB(int pos)		//GB line, get R at G pixel
{
	int x1, x2;

	x1 = this->m_pLinebuf[3]->Read(4+pos) + this->m_pLinebuf[5]->Read(4+pos);
	x2 = this->m_pGblock[2][1] + this->m_pGblock[2][3];
	x1 -= x2;
	x1 >>= 1;
	x1 += this->m_pGblock[2][2];
	return x1;
}

void C5x5Base::LastLinePro(void)
{
	if(this->m_pLinebuf[8]->IsFull())
		return;
	this->m_line |= LAST5X5LINES;
	//this->m_line--;
	this->m_pLinebuf[8] = this->m_pLinebuf[6];
}

void C5x5Base::LoopLinebuf(void)
{
	int i;
	CLineBayerBuf *p = this->m_pLinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	for(i = 0; i < 8; i++)
		this->m_pLinebuf[i] = this->m_pLinebuf[i+1];
	this->m_pLinebuf[8] = p;
	this->m_pLinebuf[8]->ClearPoint();
	this->m_GBline = this->m_GBline ? 0 : 1;
	this->m_Gfirst = this->m_Gfirst ? 0 : 1;
}

int C5x5Base::GetGpixel(int col, int row)
{
	int dh, dv, x1, x2;

	if( (this->m_Gfirst + col + row) % 2 )	
		return this->m_pLinebuf[col]->Read(row);

	x1 = this->m_pLinebuf[col]->Read(row-1) - this->m_pLinebuf[col]->Read(row+1);
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[col]->Read(row-2) + this->m_pLinebuf[col]->Read(row+2) - 
		(this->m_pLinebuf[col]->Read(row) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dh = (x1 << 1) + x2;

	x1 = this->m_pLinebuf[col-1]->Read(row) - this->m_pLinebuf[col+1]->Read(row);
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[col-2]->Read(row) + this->m_pLinebuf[col+2]->Read(row) - 
		(this->m_pLinebuf[col]->Read(row) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dv = (x1 << 1) + x2;

	if(dh < dv)
	{
		x1 = (this->m_pLinebuf[col]->Read(row-1) + this->m_pLinebuf[col]->Read(row+1)) << 2;
		x2 = (this->m_pLinebuf[col]->Read(row) << 1) - this->m_pLinebuf[col]->Read(row-2) - 
			this->m_pLinebuf[col]->Read(row+2);
	}
	else
	{
		x1 = (this->m_pLinebuf[col-1]->Read(row) + this->m_pLinebuf[col+1]->Read(row)) << 2;
		x2 = (this->m_pLinebuf[col]->Read(row) << 1) - this->m_pLinebuf[col-2]->Read(row) - 
			this->m_pLinebuf[col+2]->Read(row);
	}
	x1 += x2;
	x1 >>= 3;
	return x1;
}

void C5x5Base::GetFirstGblock(void)
{
	int i, j;

	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			this->m_pGblock[i][j] = this->GetGpixel(j+2, i+2);
}

void C5x5Base::GetNextGblock(void)
{
	int i;
	int *p = this->m_pGblock[0];

	for(i = 0; i < 5; i++)
		this->m_pGblock[0][i] = this->GetGpixel(i+2, this->m_pos+6);
	for(i = 0; i < 4; i++)
		this->m_pGblock[i] = this->m_pGblock[i+1];
	this->m_pGblock[4] = p;
}

