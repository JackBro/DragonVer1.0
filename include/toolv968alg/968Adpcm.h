
#ifndef _TOOLV968ALG_ADPCM_H_
#define _TOOLV968ALG_ADPCM_H_


#ifndef TOOLV968ALG_DLL
	#define TOOLV968ALG_DLL  __declspec(dllimport)
#endif

#include "toolbit/bitfifo.h"

typedef struct tag_ADPCMSTATE{
	int valprev;
	int index;
}ADPCMSTATE , *PADPCMSTATE;  


//=======================================================================================
//			class C968DecoderAdpcm
//
// Function: ADPCM Encode
// Input data is 16-bit pcm from ADC,output is 4 adpcm data.
//=======================================================================================
class TOOLV968ALG_DLL C968EncoderAdpcm : public CBitFifo
{
public:
	C968EncoderAdpcm(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);
	virtual ~C968EncoderAdpcm(void);
public:
	virtual int		Read(char *buf, int len);
	//Get actual Length
	int GetDataSize(void);
};


//=======================================================================================
//			class C968DecoderAdpcm
//
// Function: ADPCM Decoder
// Input data is 4-bit adpcm ,output is 16 pcm data.
//=======================================================================================
class TOOLV968ALG_DLL C968DecoderAdpcm : public CBitFifo
{
public:
	C968DecoderAdpcm(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);
	virtual ~C968DecoderAdpcm(void);
public:
	virtual int		Read(char *buf, int len);
	//Get actual Length
	int GetDataSize(void);
};


//=======================================================================================
//			class C968DecoderAdpcm
//
// Function: Vimicro ADPCM Encode
// Input data is 16-bit pcm from ADC,output is 4 adpcm data.
//=======================================================================================
class TOOLV968ALG_DLL C968VimicroEncoderAdpcm : public C968EncoderAdpcm
{
public:
	C968VimicroEncoderAdpcm(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);
	virtual ~C968VimicroEncoderAdpcm(void);
public:
	// Set adpcm state before call Write function, 
	//of course, may not call,can give default value
	void            SetAdpcmState(const ADPCMSTATE& state);
	virtual int		Read(char *buf, int len);
	virtual int		Write(char *buf, int len);

	//virtual int     Process(char *src, char *dst);
private:
	void InitState(void);
private:
	ADPCMSTATE   m_state;      //adpcm state member
};


//=======================================================================================
//			class C968DecoderAdpcm
//
// Function: Vimicro ADPCM Decoder
// Input data is 4-bit adpcm ,output is 16 pcm data.
//=======================================================================================
class TOOLV968ALG_DLL C968VimicroDecoderAdpcm : public C968DecoderAdpcm
{
public:
	C968VimicroDecoderAdpcm(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);
	virtual ~C968VimicroDecoderAdpcm(void);
public:
	// Set adpcm state before call Write function, 
	//of course, may not call,can give default value
	void            SetAdpcmState(const ADPCMSTATE& state);
	virtual int		Read(char *buf, int len);
	virtual int		Write(char *buf, int len);
private:
	void InitState(void);
private:
	ADPCMSTATE   m_state;   //adpcm state member
	
};


//=======================================================================================
//			class C968YamahaEncoderAdpcm
//
// Function: Yamaha ADPCM Encode
// Input data is 16-bit pcm from ADC,output is 4 adpcm data.
//=======================================================================================
class TOOLV968ALG_DLL C968YamahaEncoderAdpcm : public C968EncoderAdpcm
{
public:
	C968YamahaEncoderAdpcm(int size = BUFFER_DEFAULT_SIZE, int bMorePrecise = TRUE, int order = SMALLBIT_ORDER);
	virtual ~C968YamahaEncoderAdpcm(void);
public:
	virtual int		Read(char *buf, int len);
	virtual int		Write(char *buf, int len);
private:
	int LimitValue(int val,int min,int max);
private:
	int m_bMorePrecise;
};

//=======================================================================================
//			class C968YamahaDecoderAdpcm
//
// Function: Yamaha ADPCM Decoder
// Input data is 4-bit adpcm ,output is 16 pcm data.
//=======================================================================================
class TOOLV968ALG_DLL C968YamahaDecoderAdpcm : public C968DecoderAdpcm
{
public:
	C968YamahaDecoderAdpcm(int size = BUFFER_DEFAULT_SIZE, int bMorePrecise = TRUE, int order = SMALLBIT_ORDER);
	virtual ~C968YamahaDecoderAdpcm(void);
public:
	virtual int		Read(char *buf, int len);
	virtual int		Write(char *buf, int len);	
private:
	int LimitValue(int val,int min,int max);
private:
	int m_bMorePrecise;
};



//=======================================================================================
//			class C968MsEncoderAdpcm
//
// Function: Ms ADPCM Encode
// Input data is 16-bit pcm from ADC,output is 4 adpcm data.
//=======================================================================================
//class C968MsEncoderAdpcm : public C968EncoderAdpcm
//{
//public:
//	C968MsEncoderAdpcm(void);
//	virtual ~C968MsEncoderAdpcm(void);
//public:
//	virtual int		Read(char *buf, int len);
//	virtual int		Write(char *buf, int len);
//private:
//	void ChoosePredictor(unsigned int channels, short *data, int *block_pred, int *idelta);
//};


//=======================================================================================
//			class C968MsDecoderAdpcm
//
// Function: Ms ADPCM Decoder
// Input data is 4-bit adpcm ,output is 16 pcm data.
//=======================================================================================
//class C968MsDecoderAdpcm : public C968DecoderAdpcm
//{
//public:
//	C968MsDecoderAdpcm(void);
//	virtual ~C968MsDecoderAdpcm(void);
//public:
//	virtual int		Read(char *buf, int len);
//	virtual int		Write(char *buf, int len);	
//
//private:
//	//void ChoosePredictor(unsigned int channels, short *data, int *block_pred, int *idelta);
//}


#endif