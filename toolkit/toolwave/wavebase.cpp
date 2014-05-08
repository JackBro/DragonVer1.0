// SoundBase.cpp: implementation of the CWaveBase class.
//
//////////////////////////////////////////////////////////////////////


#include "internal.h"

CWaveBase::CWaveBase()
{
	this->Init();
}

CWaveBase::~CWaveBase()
{
}

void CWaveBase::Init(void)
{
	this->m_Format.wFormatTag	= WAVE_FORMAT_UNKNOWN;
	this->m_Format.cbSize		= 0;
	this->SetBitsPerSample(16);
	this->SetSamplesPerSecond(22050);
	this->SetNumberOfChannels(1);
}

int	CWaveBase::GetFormat()
{
	return this->m_Format.wFormatTag;
}

void CWaveBase::SetBitsPerSample(int bps)
{
	this->m_Format.wBitsPerSample = (UINT16)bps;
	this->Update();
}

int CWaveBase::GetBitsPerSample()
{
	return this->m_Format.wBitsPerSample;	
}

void CWaveBase::SetSamplesPerSecond(int sps)
{
	this->m_Format.nSamplesPerSec = sps;
	this->Update();
}

int CWaveBase::GetSamplesPerSecond()
{
	return this->m_Format.nSamplesPerSec;
}

void CWaveBase::SetNumberOfChannels(int nchan)
{
	this->m_Format.nChannels = (UINT16)nchan;
	this->Update();
}

int CWaveBase::GetNumberOfChannels()
{
	return this->m_Format.nChannels;
}

void CWaveBase::Update()
{
	this->m_Format.nBlockAlign = this->m_Format.nChannels;
	this->m_Format.nBlockAlign *= this->m_Format.wBitsPerSample >> 3;
	this->m_Format.nAvgBytesPerSec = this->m_Format.nSamplesPerSec;
//	this->m_Format.nAvgBytesPerSec *= this->m_Format.wBitsPerSample >> 3;
	this->m_Format.nAvgBytesPerSec *= this->m_Format.nBlockAlign;
}

void CWaveBase::GetFormat(WAVEFORMATEX *fmt)
{
	if(fmt == NULL)
		return;
	*fmt = this->m_Format;
}

void CWaveBase::SetFormat(WAVEFORMATEX *fmt)
{
	if(fmt == NULL)
		return;
	this->m_Format = *fmt;
	this->Update();
}

