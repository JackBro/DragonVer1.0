#ifndef _FILTER_4X4_BASE_H_
#define _FILTER_4X4_BASE_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif



//==========================================================
//
//		class CFilter4X4Base
//
//=========================================================
#define LAST4X4LINES	BIT6

#define TEMPLATE_FILTER		template <class T>
#define CFILTERBASE			CFilter4X4Base <T>

template <class T>
class CFilter4X4Base
{
public:
	CFilter4X4Base(int width = 128, int unit = 1);
	virtual ~CFilter4X4Base();

public:
	virtual	void	SetSize(int width = 128, int unit = 1);

public:
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);

public:
	void	SetFilter(int *filter);
	void	Clear(void);

protected:
	void	Init(void);
	void	GetFirstBlock(void);
	void	GetNextBlock(void);
	void	LastLinePro(void);
	void	LoopLinebuf(void);
	int		process(char *buf, int len);

protected:
	T		m_Linebuf[7];

private:
	T		*m_pLinebuf[7];
	int		m_Block[7][7*3];
	int		*m_pBlock[7];
	int		m_line, m_pos;
	int		m_unit, m_width;
	int		m_filter[7];
};


TEMPLATE_FILTER
CFILTERBASE::CFilter4X4Base(int width, int unit)
{
	this->SetSize(width, unit);
}

TEMPLATE_FILTER
CFILTERBASE::~CFilter4X4Base(void)
{
}

TEMPLATE_FILTER
void CFILTERBASE::SetSize(int width, int unit)
{
	int i;

	for(i = 0; i < 7; i++)
		this->m_Linebuf[i].SetSize(width, 3, unit);
	this->m_width = width;
	this->m_unit = unit;
	this->Clear();
}

TEMPLATE_FILTER
void CFILTERBASE::Clear(void)
{
	this->m_line = 0;
	this->m_pos = 0;
	this->Init();
}

TEMPLATE_FILTER
void CFILTERBASE::Init(void)
{
	int i;

	this->m_filter[0] = 0x20;
	this->m_filter[1] = 0x20;
	this->m_filter[2] = 0x20;
	this->m_filter[3] = 0x40;
	this->m_filter[4] = 0x20;
	this->m_filter[5] = 0x20;
	this->m_filter[6] = 0x20;
	for(i = 0; i < 7; i++)
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	for(i = 0; i < 7; i++)
		this->m_pBlock[i] = this->m_Block[i];
}

TEMPLATE_FILTER
void CFILTERBASE::SetFilter(int *filter)
{
	int i, total = 0;

	for(i = 0; i < 4; i++)
	{
		if(i == 0)
			total += filter[i];
		else
			total += filter[i] << 1;
	}
	if(total == 0)
		return;
	for(i = 1; i < 4; i++)
	{
		this->m_filter[3-i] = (filter[i] << 8) / total;
		this->m_filter[3+i] = this->m_filter[3-i];
	}
	this->m_filter[3] = (filter[0] << 8) / total;
}

TEMPLATE_FILTER
int	CFILTERBASE::Write(char *buf, int len)
{
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST4X4LINES)
		return 0;
	if(this->m_line == 4)
		return this->m_pLinebuf[6]->Write(buf, len);
	while(this->m_line < 4)
	{
		pos1 = this->m_pLinebuf[3+this->m_line]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		if(pos1 == 0)
			this->m_line++;
		if(len == 0)
			break;
	}
	if(this->m_line == 4)
	{
		this->m_Linebuf[0] = this->m_Linebuf[3];
		this->m_Linebuf[1] = this->m_Linebuf[3];
		this->m_Linebuf[2] = this->m_Linebuf[3];
	}
	return pos;
}

TEMPLATE_FILTER
int	CFILTERBASE::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_pLinebuf[6]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	len = this->process(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len;
}

TEMPLATE_FILTER
int	CFILTERBASE::process(char *buf, int len)
{
	int pos = 0, max = 255, i, j, total[3], adr, val;
	CBaseConvert tools;

	if(this->m_unit == 2)
		max = 1024;
	len /= 3*this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	for(; this->m_pos < len; this->m_pos++)
	{
		if(this->m_pos == 0)
			this->GetFirstBlock();
		else
			this->GetNextBlock();
		total[0] = total[1] = total[2] = 0;
		for(i = 0; i < 7; i++)
		{
			for(j = 0, adr = 0; j < 7; j++, adr += 3)
			{
				val = this->m_filter[i] * this->m_filter[j];
				total[0] += val * this->m_pBlock[i][adr+0];
				total[1] += val * this->m_pBlock[i][adr+1];
				total[2] += val * this->m_pBlock[i][adr+2];
			}
		}
		for(i = 0; i < 3; i++, pos += this->m_unit)
		{
			total[i] >>= 16;
			total[i] = (total[i] < 0) ? 0 : ((total[i] > max) ? max : total[i]);
			tools.setvalue(buf+pos, total[i], this->m_unit);
		}
	}
	return pos;
}

TEMPLATE_FILTER
void CFILTERBASE::LastLinePro(void)
{
	int pos;

	if(this->m_pLinebuf[6]->IsFull())
		return;
	this->m_line |= LAST4X4LINES;
	this->m_line--;
	pos = 6 - (4 - (this->m_line & 0xf));
	this->m_pLinebuf[6] = this->m_pLinebuf[pos];
}

TEMPLATE_FILTER
void CFILTERBASE::LoopLinebuf(void)
{
	int i;
	T *p = this->m_pLinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	for(i = 0; i < 6; i++)
		this->m_pLinebuf[i] = this->m_pLinebuf[i+1];
	this->m_pLinebuf[6] = p;
	this->m_pLinebuf[6]->ClearPoint();
}

TEMPLATE_FILTER
void CFILTERBASE::GetFirstBlock(void)
{
	int i, j, adr;

	for(i = 0; i < 7; i++)
		for(j = 0, adr = 0; j < 7; j++, adr += 3)
			this->m_pLinebuf[j]->Read(i, this->m_pBlock[i] + adr);
}

TEMPLATE_FILTER
void CFILTERBASE::GetNextBlock(void)
{
	int i, adr;
	int *p = this->m_pBlock[0];

	for(i = 0, adr = 0; i < 7; i++, adr += 3)
		this->m_pLinebuf[i]->Read(this->m_pos, this->m_pBlock[0] + adr);
	for(i = 0; i < 6; i++)
		this->m_pBlock[i] = this->m_pBlock[i+1];
	this->m_pBlock[6] = p;
}


#endif


