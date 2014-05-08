
#include "internal.h"


CBaseItem::CBaseItem()
{
}

CBaseItem::~CBaseItem()
{
}

int CBaseItem::Read(char *buf, int len)
{
	buf = buf;
	len = len;
	return 0;
}

int CBaseItem::Write(char *buf, int len)
{
	buf = buf;
	len = len;
	return 0;
}

//============================================
//============================================
CBaseObject::CBaseObject()
{
}

CBaseObject::~CBaseObject()
{
	this->Close();
}

void CBaseObject::Close(void)
{
}

void CBaseObject::Open(void)
{
}

int CBaseObject::IsOpened(void)
{
	return 0;
}

//=================================================
//================================================

CBaseChannel::CBaseChannel()
{
	this->m_rwMode = CALLBACK_MODE;
	this->m_pcall = NULL;
}

CBaseChannel::~CBaseChannel()
{
}

void CBaseChannel::SetCallBack(ChannelCallBack *pcall)
{
	this->m_pcall = pcall;
}

int	CBaseChannel::GetRWmode(void)
{
	return this->m_rwMode;
}

void CBaseChannel::SetRWmode(int mode)
{
	if(mode != WAITING_MODE)
		mode = CALLBACK_MODE;
	while(this->m_rwMode != mode)
		this->m_rwMode = mode;
	//this->m_rwMode = (mode == WAITING_MODE) ? WAITING_MODE : CALLBACK_MODE;
}
