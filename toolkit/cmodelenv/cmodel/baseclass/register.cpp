
#include "register.h"

CRegister::CRegister(int count, int unit) : CSramBuf(count, unit)
{
	this->Clear();
}

void CRegister::SetSize(int size)
{
	CSramBuf::SetSize(size, sizeof(TRegister));
	this->Clear();
}

void CRegister::Clear(void)
{
	int i;

	for(i = 0; i < this->m_size; i++ )
	{
		this->Write(i, 0, MODIFY_REG);
		this->SetMask(i, 0xff);
	}
}

int CRegister::Write(int adr, int val, int sel)
{
	TRegister reg;

	adr *= sizeof(TRegister);
	if( CSramBuf::Read( (char *)&reg, sizeof(TRegister), adr ) == 0 )
		return 0;
	if(sel == HOST_MODIFY_REG)
		reg.val = val & reg.mask;
	else if(sel == CHIP_MODIFY_REG)
		reg.value = val & reg.mask;
	else
	{
		reg.val = val & reg.mask;
		reg.value = val & reg.mask;
	}
	return CSramBuf::Write( (char *)&reg, sizeof(TRegister), adr );
}

int	CRegister::Read(int adr)
{
	TRegister reg;

	adr *= sizeof(TRegister);
	if( CSramBuf::Read( (char *)&reg, sizeof(TRegister), adr ) == 0 )
		return 0;
	return reg.value;
}

void CRegister::Update(int adr)
{
	TRegister reg;

	adr *= sizeof(TRegister);
	if( CSramBuf::Read( (char *)&reg, sizeof(TRegister), adr ) == 0 )
		return;
	reg.value = reg.val;
	CSramBuf::Write( (char *)&reg, sizeof(TRegister), adr);
}

int CRegister::SetMask(int adr, int mask)
{
	TRegister reg;

	adr *= sizeof(TRegister);
	if( CSramBuf::Read( (char *)&reg, sizeof(TRegister), adr ) == 0 )
		return 0;
	reg.mask = mask;
	return CSramBuf::Write( (char *)&reg, sizeof(TRegister), adr );
}

