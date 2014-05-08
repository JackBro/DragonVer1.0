#ifndef _TOOLV968ALG_3DAUDIO_H_
#define _TOOLV968ALG_3DAUDIO_H_


#ifndef TOOLV968ALG_DLL
	#define TOOLV968ALG_DLL  __declspec(dllimport)
#endif


#include "toolbit/bitfifo.h"


const int gTCORDER   = 128;
const int gHRTFORDER = 128;


//=======================================================================================
//			class C9683StereoDipole
//
// Function: Stereo Expansion, write data is 16-bit pcm, read data is 16-bit pcm
// Notice :  pcm code must be 48kHz 
//=======================================================================================
class TOOLV968ALG_DLL C9683StereoDipole : public CBitFifo
{
public:
	C9683StereoDipole(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);
	virtual ~C9683StereoDipole(void);

public:
	int             GetDataSize(void);
	virtual int		Read(char *buf, int len);
	virtual int		Write(char *buf, int len);

private:
	void InitBuf(void);
private:
	short m_tcPos;
	short m_posBuf;
	short m_tcBuf[2][gTCORDER];
	short m_hrtfBuf[gHRTFORDER];
};


//=======================================================================================
//			class C9683StereoDipole
//
// Function: 3D Positioning, write data is 16-bit pcm, read data is 16-bit pcm
// Notice :  pcm code must be 48kHz 
//=======================================================================================
class TOOLV968ALG_DLL C968ThreeDPosition : public CBitFifo
{
public:
	C968ThreeDPosition(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);
	virtual ~C968ThreeDPosition(void);

public:
	void            SetHrtfData(char* buf, int len);
	int             GetDataSize(void);
	virtual int		Read(char *buf, int len);
	virtual int		Write(char *buf, int len);
private:
	void InitBuf(void);
private:
	short           m_posBuf;
	short           m_hrtfBuf[gHRTFORDER];
	int             m_bSuccess;
	CBufferBase     m_outHrtfBuf;
};



#endif