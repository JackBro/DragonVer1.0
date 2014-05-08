#ifndef __BASECLASS_RAWVIDEOFRM_H__
#define __BASECLASS_RAWVIDEOFRM_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


#include "ImageFrame.h"
#include "BaseClass/RawVideoFrm.h"

enum VIDEOFRMSTATEenum {
	VIDFRM_PAUSE,
	VIDFRM_PLAY,
	VIDFRM_QUIT
};

class BASECLASS_DLL CRawVideoFrm : public CImageFrame
{
	DECLARE_DYNCREATE(CRawVideoFrm)
public:
	CRawVideoFrm();
	virtual ~CRawVideoFrm();

public:
	void Open(char *src, int start = 0, int end = 0);
	void Play(void);
	void Pause(void);
	void Close(void);
	void SetFrameRate(int rate = 30);

private:
	void Init(void);

private:
	void StartThrd(void);
	void ThrdProc(void);
	void TerminalThrd(void);
	friend DWORD RawVideoFrmThrdProc(LPVOID *ptr);

private:
	int m_start;
	int m_end;
	volatile int m_status;
	int m_frmrate;
	HANDLE	m_hthrd;

protected:
	DECLARE_MESSAGE_MAP()
};


#endif
