#include "internal.h"




IMPLEMENT_DYNCREATE(CRawVideoFrm, CImageFrame)

BEGIN_MESSAGE_MAP(CRawVideoFrm, CImageFrame)
END_MESSAGE_MAP()

// CRawVideoFrm ¹¹Ôì/Ïú»Ù
CRawVideoFrm::CRawVideoFrm()
{
	this->Init();
	this->StartThrd();
}

CRawVideoFrm::~CRawVideoFrm()
{
	this->TerminalThrd();
}

void CRawVideoFrm::Init(void)
{
	this->m_start = 0;
	this->m_end = 1000;
	this->m_status = VIDFRM_PAUSE;
	this->m_frmrate = 30;
	this->m_hthrd = NULL;
}

void CRawVideoFrm::Open(char *src, int start, int end)
{
	CImageFrame::CenterWindow();
	CImageFrame::ShowWindow(SW_SHOW);
	CImageFrame::LoadRawImg(src);
	this->m_start = start;
	this->m_end = (end <= 0) ? 1000 : end;
	this->m_status = VIDFRM_PAUSE;
	CImageFrame::LoadRawImg(this->m_start);
}

void CRawVideoFrm::Close(void)
{
	CImageFrame::ShowWindow(SW_HIDE);
}

void CRawVideoFrm::Play(void)
{
	this->m_status = VIDFRM_PLAY;
	if(this->m_index & END_OF_SEQUENCE)
		this->m_index = this->m_start;
}

void CRawVideoFrm::Pause(void)
{
	this->m_status = VIDFRM_PAUSE;
}
	
void CRawVideoFrm::SetFrameRate(int rate)
{
	this->m_frmrate = (rate <= 0) ? 30 : rate;
}

void CRawVideoFrm::TerminalThrd(void)
{
	int i = 4, ret;

	if(this->m_hthrd == NULL)
		return;

	while(i--)
	{
		this->m_status = VIDFRM_QUIT;
		ret = WaitForSingleObject(this->m_hthrd, 2000);
		if(ret == WAIT_OBJECT_0)
			break;
	}
	this->m_hthrd = NULL;
}

void CRawVideoFrm::ThrdProc(void)
{
	int flag = 1;

	while(flag)
	{
		switch(this->m_status)
		{
		case VIDFRM_PAUSE:
			Sleep(1000 / this->m_frmrate);
			break;
		case VIDFRM_PLAY:
			CImageFrame::LoadRawImg(this->m_index+1);
			this->InvalidateRect(NULL, FALSE);
			this->UpdateWindow();
			Sleep(1000 / this->m_frmrate);
			break;
		case VIDFRM_QUIT:
		default:
			flag = 0;
			break;
		}
	}
}

void CRawVideoFrm::StartThrd(void)
{
	DWORD id;

	this->m_hthrd = CreateThread( NULL, 
                          0,
                          (LPTHREAD_START_ROUTINE) RawVideoFrmThrdProc,
                          (LPVOID)this, 
                          0, 
                          &id);
}

DWORD RawVideoFrmThrdProc(LPVOID *ptr)
{
	((CRawVideoFrm *)ptr)->ThrdProc();
	return 0;
}


