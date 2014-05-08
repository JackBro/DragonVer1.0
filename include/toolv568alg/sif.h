

#ifndef _CMODEL_SENSORIF_ALG_H_
#define _CMODEL_SENSORIF_ALG_H_

#ifndef _WINDLL
#define TOOLV568ALG_DLL
#endif

#ifndef TOOLV568ALG_DLL
	#define TOOLV568ALG_DLL  __declspec(dllimport)
#endif



//
//	use "COM" conception later to modify cmodel
//

#define IMAGEWIDTH 640
#define IMAGEHEIGHT 480

//=============== class CSifModule326 =================

class CSifModule326 : public CBaseObject
{
public:
	CSifModule326();
};

//=============== class CRgb2yiq326 =================
class CRgb2yiq326 : public CFifoBuf
{
public:
	CRgb2yiq326(int cap = 256, int width = IMAGEWIDTH, int height = IMAGEHEIGHT);

public:
	virtual int     Read(char *buf, int len);
	virtual int     Write(char *buf, int len);
	virtual int		GetSize(void);
	virtual int		GetTotalSize(void);
	virtual void	SetSize(int size = BUFFER_DEFAULT_SIZE);

    
	void  Process(char *rgb, char *yiq);

	void  SetCap(int cap);
	int   GetCap(void);
    void  SetImageWidth(int width);
	int   GetImageWidth(void);
	void  SetImageHight(int hight);
	int   GetImageHight(void);

	void  SetGamStartPt(int *gammaStartPoint);
	void  SetGamSlope(int *gammaSlope);

private:
	char  YGammaCorr(char yIn);
    char  Rgb2y(void);
	char  Rgb2i(void);
	char  Rgb2q(void);

private:
	int  m_cap;
	int  m_imageWidth;
	int  m_imageHight;
	int  m_gamStartPt[9];
	int  m_gamSlope[9];
};

//=============== class CYiq2rgb326 =================
class CYiq2rgb326 : public CFifoBuf
{
public:
	CYiq2rgb326(int cap = 256, int width = IMAGEWIDTH, int height = IMAGEHEIGHT);

public:
	virtual int     Read(char *buf, int len);
	virtual int     Write(char *buf, int len);
	int		GetSize(void);
	int		GetTotalSize(void);
	void	SetSize(int size = BUFFER_DEFAULT_SIZE);


	void  SetCap(int cap);
	int   GetCap(void);
    void  SetImageWidth(int width);
	int   GetImageWidth(void);
	void  SetImageHight(int hight);
	int   GetImageHight(void);

	void  Process(char *yiq ,char *rgb);

private:
	char  Yiq2r(void);
	char  Yiq2g(void);
	char  Yiq2b(void);

private:
	int  m_cap;
	int  m_imageWidth;
	int  m_imageHight;
};

//=============== class CRgb2hsv326 =================
#define triMax(x, y, z)	(x > y) ? (x > z ? x : z) : (y > z ? y : z)
#define triMin(x, y, z)	(x < y) ? (x < z ? x : z) : (y < z ? y : z)

class CRgb2hsv326 : public CFifoBuf
{
public:
	CRgb2hsv326(int cap = 256, int width = IMAGEWIDTH, int height = IMAGEHEIGHT, int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CRgb2hsv326();

public:
	void	SetSize(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	release(void);

	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	virtual int ReadF(double *buf, int len);
	virtual int WriteF(double *buf, int len);

	void  SetCap(int cap);
	int   GetCap(void);
    void  SetImageWidth(int width);
	int   GetImageWidth(void);
	void  SetImageHight(int hight);
	int   GetImageHight(void);

	void  Process(char *rgb, double *hsv);

private:
	void  Rgb2hsv(void);

private:
	int    m_cap;
	int    m_imageWidth;
	int    m_imageHight;
	double *m_bufF;
};

//=============== class CHsv2Rgb326 =================
class CHsv2rgb326 : public CFifoBuf
{
public:
	CHsv2rgb326(int cap = 256, int width = IMAGEWIDTH, int height = IMAGEHEIGHT, int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CHsv2rgb326();

public:
	void	SetSize(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	release(void);

	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);
	virtual int ReadF(double *buf, int len);
	virtual int WriteF(double *buf, int len);

	void  SetCap(int cap);
	int   GetCap(void);
    void  SetImageWidth(int width);
	int   GetImageWidth(void);
	void  SetImageHight(int hight);
	int   GetImageHight(void);

	void  Process(double *hsv, char *rgb);

private:
	void Hsv2rgb(void);

private:
	int    m_cap;
	int    m_imageWidth;
	int    m_imageHight;
	double *m_bufF;
};

//=============== class CMotionDect326 =================

class CMotionDect326 : public CBufferBase
{
public:
	CMotionDect326(int width = IMAGEWIDTH, int height = IMAGEHEIGHT, int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CMotionDect326();

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, char *buf1, int size);

	void	SetSize(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	release(void);

	void SetImageWidth(int width);
	int  GetImageWidth(void);
	void SetImageHight(int hight);
	int  GetImageHight(void);
	void SetSleekWindowWidth(char width);
	char GetSleekWindowWidth(void);
	void SetSleekWindowHight(char hight);
	char GetSleekWindowHight(void);
    void SetSumInLineThre(int sumThre);
	int  GetSumInLineThre(void);
	void SetLineNumThre(int lineNumThre);
	int  GetLineNumThre(void);

	unsigned int Process(char * pPrevFrame, char * pCurFrame);

private:
	unsigned int MotionDetection(void);

private:
	char *m_buf1;
	int  m_imageWidth;
	int  m_imageHight;
	char m_sleWinWidth;
	char m_sleWinHight;
	int  m_sumInLineThre;
	int  m_lineNumThre;	
};

//=============== class CYuvOrdTrans326 =================
#define UYVY_ORDER 0
#define VYUY_ORDER 1
#define YUYV_ORDER 2
#define YVYU_ORDER 3

class CYuvOrdTrans326 : public CFifoBuf
{
public:
	CYuvOrdTrans326(int width = IMAGEWIDTH, int height = IMAGEHEIGHT, char inOrd = VYUY_ORDER, char outOrd = UYVY_ORDER);

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

	void SetInOrder(char order);
	char GetInOrder(void);
	void SetOutOrder(char order);
	char GetOutOrder(void);

	void SetImageWidth(int width);
	int  GetImageWidth(void);
	void SetImageHight(int hight);
	int  GetImageHight(void);

	void Process(char *src, char *dst);

private:
    void Vyuy2Uyvy(void);
    void Yuyv2Uyvy(void);
    void Yvyu2Uyvy(void);

private:
	char m_inOrder;
	char m_outOrder;
	int  m_imageWidth;
	int  m_imageHight;
};

#define  SLEEKWINDOWNUM  16

//=============== class CHistogram326 =================
class CHistogram326 : public CBufferBase
{
public:
    CHistogram326(int width = IMAGEWIDTH, int height = IMAGEHEIGHT, int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CHistogram326();

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, char *buf1, int size);

	void	SetSize(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	release(void);

	void SetImageWidth(int width);
	int  GetImageWidth(void);
	void SetImageHight(int hight);
	int  GetImageHight(void);
	void SetAreaMaxVal(int areaMaxVal);
	int  GetAreaMaxVal(void);
	void SetCritPerVal(char critPerVal);
	char GetCritPerVal(void);
	void SetTotalAreaLen(int areaTotLen);
	int  GetTotalAreaLen(void);
	void SetAreaNum(int areaNum);
	int  GetAreaNum(void);
	void SetMinDelVal(int minDelVal);
	int GetMinDelVal(void);
	void SetPreFramNum(char preFramNum);
	char GetPreFramNum(void);

	void AreaPixStat(int *statRes);
	char GetAreaIdx(int *statRes);
	char GetCorCoef(void);
	char GetCorCoefMulFram(void);

	char ProcessGetCorCoef(char *imageData);
	char ProcessGetCorCoefMulFram(char *preCorCoef, char *w);
	void ProcessCalHistpgram(char *imageData, int *histSta);

private:
	int  m_imageWidth;
	int  m_imageHight;
	int  m_areaMaxVal;
	int  m_totalAreaLen;
	int  m_areaNum;
	int  m_minDelVal;
	char m_critPerVal;
	char m_preFramNum;
	char *m_buf1;
};

//=============== class CAe326 =================
class CAe326 : public CBufferBase
{
public:
    CAe326(int width = IMAGEWIDTH, int height = IMAGEHEIGHT);
	virtual ~CAe326();

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

	void SetImageWidth(int width);
	int  GetImageWidth(void);
	void SetImageHight(int hight);
	int  GetImageHight(void);
	void SetSleekWindowWidth(char width);
	char GetSleekWindowWidth(void);
	void SetSleekWindowHight(char hight);
	char GetSleekWindowHight(void);
	void SetSleekWindowWeight(char *weight);
	void GetSleekWindowWeight(char *weight);

	char  Process(char *imageData);

private:
	int   GetSumVal(char *pData, int eleNum);
	char  GetImageDatMean(void);

private:
	int  m_imageWidth;
	int  m_imageHight;
	char m_sleWinWidth;
	char m_sleWinHight;
	char m_sleWinWeight[SLEEKWINDOWNUM];
};

//=============== class CAwb326 =================
class CAwb326 : public CBufferBase
{
public:
    CAwb326(int width = IMAGEWIDTH, int height = IMAGEHEIGHT);
	virtual ~CAwb326();

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, int len);

	void SetImageWidth(int width);
	int  GetImageWidth(void);
	void SetImageHight(int hight);
	int  GetImageHight(void);
	void SetYTopVal(int yTop);
	int  GetYTopVal(void);
	void SetYBotVal(int yBot);
	int  GetYBotVal(void);
	void SetITopVal(int iTop);
	int  GetITopVal(void);
	void SetQTopVal(int qTop);
	int  GetQTopVal(void);
	void SetRGainVal(char rGain);
    char GetRGainVal(void);
	void SetBGainVal(char bGain);
    char GetBGainVal(void);
	void SetGainTopVal(char gainTop);
    char GetGainTopVal(void);
	void SetGainBotVal(char gainBot);
    char GetGainBotVal(void);
	
	void  Process(char *pImageDat, char *pAwbOut);

private:
	char  CheckSumConValid(char *rgb);
	void  GetRGBMean(char *rgbMean);
	void  GetRBGain(char *rgbMean);

private:
	int  m_imageWidth;
	int  m_imageHight;
	int  m_yTop;
	int  m_yBot;
	int  m_iTop;
	int  m_qTop;
	char m_rGain;
	char m_bGain;
	char m_gainTop;
	char m_gainBot;
};

//=============== class CAutoFlick326 =================
class CAutoFlick326 : public CBufferBase
{
public:
    CAutoFlick326(int width = IMAGEWIDTH, int height = IMAGEHEIGHT, int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CAutoFlick326();

public:
	virtual int Read(char *buf, int len);
	virtual int Write(char *buf, char *buf1, int size);

	void	SetSize(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	release(void);

	void SetImageWidth(int width);
	int  GetImageWidth(void);
	void SetImageHight(int hight);
	int  GetImageHight(void);
	void SetAFMode(char afMode);
	char GetAFMode(void);
	void SetAFStep(char afStep);
	char GetAFStep(void);
    void SetAFStartLine(int afStart);
	int  GetAFStartLine(void);
	void SetAFThreshold(int afThre);
	int  GetAFThreshold(void);	
	
	int Process(char *preData, char *curData);

private:
	int  GetSumVal(char *pData, int eleNum);
	int  FlickCheck(void);

private:
	int  m_imageWidth;
	int  m_imageHight;
	char m_afMode;
	char m_afStep;
	int  m_afStart;
	int  m_afThre;
	char *m_buf1;
};

#endif


