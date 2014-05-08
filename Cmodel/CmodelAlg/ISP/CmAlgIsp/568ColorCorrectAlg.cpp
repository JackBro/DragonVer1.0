
#include "internal.h"

C568ColorCorrectAlg::C568ColorCorrectAlg()
{
	//int colorMatrix[9] = {0x40, 0xf3, 0xf3, 0xf3, 0x40, 0xf3, 0xf3, 0xf3, 0x40};
	int colorMatrix[9] = {0x60, 0xf9, 0xe5, 0xe7, 0x50, 0x05, 0xf3, 0xe6, 0x5e};
	int offset[3] = {0, 0, 0};

	this->SetColorMatrix(colorMatrix);
	this->SetColorOffset(offset);
}

C568ColorCorrectAlg::~C568ColorCorrectAlg()
{
}

int C568ColorCorrectAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C568ColorCorrectAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_Linebuf.SetSize(this->m_width*3, 0, this->m_unit);
}

int C568ColorCorrectAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->unit = this->m_unit;
	cfg->fmt = RAW_RGB | (VIDEOCLASS << CLASSSHIFT);
	return 0;
}


void C568ColorCorrectAlg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_Linebuf.ClearPoint();
}



void C568ColorCorrectAlg::SetColorMatrix(int *p)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			this->m_colormatrix[i][j] = p[i*3+j]&0xff;
}

int* C568ColorCorrectAlg::GetColorMatrix()
{
	return &this->m_colormatrix[0][0];
}


void C568ColorCorrectAlg::SetColorOffset(int *p)
{
	int i;

	for(i = 0; i < 3; i++)
		this->m_offset[i] = p[i]&0xff;
}

int* C568ColorCorrectAlg::GetColorOffset()
{
	return &this->m_offset[0];
}

int C568ColorCorrectAlg::Mul10(int p, int c)
{
	int val;

	p &= 0x3ff;
	c &= 0xff;
	if(c&0x80)	//minus
	{
		c = (~(c-1))&0xff;
		val = -p*c;
	}
	else
		val = p*c;
	return val;
}
unsigned char C568ColorCorrectAlg::GetSingleBit(unsigned char X, int whichBit)
{
	return (( X & (1<<whichBit) ) != 0 ) ;
}

int C568ColorCorrectAlg::ColMul(int P, unsigned char A)
{
	int	AxP;	// [11:0], output
	int	Neg128;
	unsigned char AN, AbsValNeg, AF;	// [7:0]
	// all of the following are [11:0]
	int	A1p01, A1p02, A1p04, A1p08, A1p16, A1p32,
		s1p01N1p02, s1p04N1p08, s1p16N1p32,
		s1p64, S01x08, S01x64, S01x64P, S01x64N,
		S16x64;

	Neg128 = (A == 0x80);
	AN = (unsigned char)(~(A - 1));

	AbsValNeg = Neg128 ? 0xff : ((GetSingleBit(A,7)<<7)+(AN&0x7f));


	AF = GetSingleBit(A,7) ? AbsValNeg : A;

	
	A1p01 = GetSingleBit(AF,6) ? (P<<1) : 0;
	A1p02 = GetSingleBit(AF,5) ? P : 0;
	s1p01N1p02 = (A1p01 + A1p02) & 0xfff;

	
	A1p04 = GetSingleBit(AF,4) ? (P>>1) : 0 ;
	A1p08 = GetSingleBit(AF,3) ? (P>>2) : 0 ;
	s1p04N1p08 = (A1p04 + A1p08) & 0xfff ;

	
	A1p16 = GetSingleBit(AF,2) ? (P>>3) : 0 ;
	A1p32 = GetSingleBit(AF,1) ? (P>>4) : 0 ;
	s1p16N1p32 = (A1p16 + A1p32) & 0xfff ;


	s1p64 = GetSingleBit(AF,0) ? (P>>5) : 0 ;

	
	S01x08 = (s1p01N1p02 + s1p04N1p08) & 0xfff; 
	S16x64 = (s1p16N1p32 + s1p64) & 0xfff;

	
	S01x64 = (S01x08 + S16x64) & 0xfff;


	S01x64P = (S01x64>>1) & 0xfff ;
	S01x64N =  (~S01x64P + 1) & 0xfff;


	AxP = ((GetSingleBit(AF,7) ? S01x64N : S01x64P) & 0xfff);

	return AxP ;
}

int C568ColorCorrectAlg::ColSum(int AxR, int AxG, int AxB, int ColOfst)
{
	int	PxSum ;
	int	SUMrg, SUMbo, SUMrgbo;

	SUMrg =  (((AxR&0x800)!=0)<<12)+AxR
		+(((AxG&0x800)!=0)<<12)+AxG ;
	SUMrg &= 0x1fff ;	// SUMrg is [12:0]

	SUMbo =  (((AxB&0x800)!=0)<<12)+AxB
		+(((ColOfst&0x80)!=0)?0x70:0x00) + ColOfst ;
	SUMbo &= 0x1fff ;	// SUMbo is [10:0]

	SUMrgbo =(((SUMrg&0x1000)!=0)<<13)+SUMrg
		+(((SUMbo&0x1000)!=0)<<13)+SUMbo ;

	if (SUMrgbo & (1<<13))	// SignBit, [11]
		PxSum = 0 ;
	else if (SUMrgbo & 0x1c00)		// OvFlag, |[10:8]
		PxSum = 0x3ff ;
	else
		PxSum = SUMrgbo & 0x3ff ;

	return PxSum ;
}

int C568ColorCorrectAlg::Write(int index, char *buf, int len)
{
	index = index;

	if(len == 0)
		return 0;
	len = this->m_Linebuf.Write(buf, len);
	return len;
}

int C568ColorCorrectAlg::Read(int index, char *buf, int len)
{
	index = index;

	int max = 255, temp ,i;

	if(this->m_unit == 2)
		max = 1023;

	if (len < 1 || !m_Linebuf.IsFull()) return 0;
	len /= (3*this->m_unit);
	len = (this->m_width > len) ? len : this->m_width;

	for(i = 0; i < len; i++)
	{
		if(this->m_unit == 1)
		{
			this->m_r = m_Linebuf.Read(3*i)&0xff;
			this->m_g = m_Linebuf.Read(3*i+1)&0xff;
			this->m_b = m_Linebuf.Read(3*i+2)&0xff;
		}
		else
		{
			this->m_r = m_Linebuf.Read(3*i)&0x3ff;
			this->m_g = m_Linebuf.Read(3*i+1)&0x3ff;
			this->m_b = m_Linebuf.Read(3*i+2)&0x3ff;
		}
		//r ,output is bgr
		temp = this->ColSum(this->ColMul(this->m_r, (unsigned char)(this->m_colormatrix[0][0]&0xff)),
							this->ColMul(this->m_g, (unsigned char)(this->m_colormatrix[0][1]&0xff)),
							this->ColMul(this->m_b, (unsigned char)(this->m_colormatrix[0][2]&0xff)),
							(this->m_offset[0]&0xff));	
		temp = (temp > max) ? max : (temp > 0 ? temp : 0);
		if(this->m_unit == 1)
			*(buf++) = (char)temp;
		else
		{
			*(short*)buf = (short)temp;
			buf += 2;
		}
		//g
		temp = this->ColSum(this->ColMul(this->m_r, (unsigned char)(this->m_colormatrix[1][0]&0xff)),
							this->ColMul(this->m_g, (unsigned char)(this->m_colormatrix[1][1]&0xff)),
							this->ColMul(this->m_b, (unsigned char)(this->m_colormatrix[1][2]&0xff)),
							(this->m_offset[1]&0xff));
		temp = (temp > max) ? max : (temp > 0 ? temp : 0);
		if(this->m_unit == 1)
			*(buf++) = (char)temp;
		else
		{
			*(short*)buf = (short)temp;
			buf += 2;
		}
			
		//b
		temp = this->ColSum(this->ColMul(this->m_r, (unsigned char)(this->m_colormatrix[2][0]&0xff)),
							this->ColMul(this->m_g, (unsigned char)(this->m_colormatrix[2][1]&0xff)),
							this->ColMul(this->m_b, (unsigned char)(this->m_colormatrix[2][2]&0xff)),
							(this->m_offset[2]&0xff));
		temp = (temp > max) ? max : (temp > 0 ? temp : 0);
		if(this->m_unit == 1)
			*(buf++) = (char)temp;
		else
		{
			*(short*)buf = (short)temp;
			buf += 2;
		}
	}
	this->m_outline ++;
	this->m_Linebuf.ClearPoint();

	return len*this->m_unit*3;
}


