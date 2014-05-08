

#ifndef _CMODEL_LBUF_ALG_H_
#define _CMODEL_LBUF_ALG_H_

#ifndef _WINDLL
#define TOOLV568ALG_DLL
#endif

#ifndef TOOLV568ALG_DLL
	#define TOOLV568ALG_DLL  __declspec(dllimport)
#endif
//
//	use "COM" conception later to modify cmodel
//


//=============== class CLbufModule =================

class TOOLV568ALG_DLL CLbufModule : public CBaseObject
{
public:
	CLbufModule();
};


//class CYuvBlkBufBase

class TOOLV568ALG_DLL CYuvBlkBufBase : public CSramBuf
{
public:
	CYuvBlkBufBase(char fmt = YUV422_UYVY, int width = 128, int unit = 1);
	virtual ~CYuvBlkBufBase();

public:

public:
	virtual void	ClearPoint(void);
	virtual void	SetSize(char fmt = YUV422_UYVY, int width = 128, int unit = 1);

protected:
	void		AdjustWidth(int width);
	int			CalYuvSize(char fmt, int width, int line);
	int			CalAllocMemSize(char fmt, int width);
	int			AdjustBlkFmt(char fmt);

protected:
	char		m_blkfmt;
	int			m_yuvsize;
	int			m_memsize;
	int			m_yuv[12];
	CBaseConvert m_basecvt;

public:
	char		m_yuvfmt;
	int			m_width;
	int			m_unit;
	int			m_wpoint;
	int			m_rpoint;
};


//class CYuv2BlkBuf

class TOOLV568ALG_DLL CYuv2BlkBuf : public CYuvBlkBufBase
{
public:
	CYuv2BlkBuf(char fmt = YUV422_UYVY, int width = 128, int unit = 1);
	virtual ~CYuv2BlkBuf();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);

public:
	void	ClearPoint(void);
	int		GetLineNum(void);
	void	SetSize(char fmt = YUV422_UYVY, int width = 128, int unit = 1);

private:
	int			WriteYuv444(char *buf, int size);
	int			WriteYuv422(char *buf, int size);
	int			WriteYuv420(char *buf, int size);
	int			WriteYuv411(char *buf, int size);
	int			WriteYuv400(char *buf, int size);

	int			WriteYuv4xxHead(char *buf, int size, int x, int paknum, char* bdat);
	int			WriteYuv4xxTail(char *buf, int size, int paknum);
	int			WriteYuv444Seg(char *buf, int size);
	int			WriteYuv422Seg(char *buf, int size);
	int			WriteYuv420Seg(char *buf, int size);
	int			WriteYuv411Seg(char *buf, int size);
	int			WriteYuv400Seg(char *buf, int size);
	void		Get420WrPos(int* x, int* y);

private:
	int			m_dat[6];

};


//class CBlk2YuvBuf

class TOOLV568ALG_DLL CBlk2YuvBuf : public CYuvBlkBufBase
{
public:
	CBlk2YuvBuf(char fmt = YUV422_UYVY, int width = 128, int unit = 1);
	virtual ~CBlk2YuvBuf();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);

public:
	void	ClearPoint(void);
	int		GetBlkNum(void);
	void	SetSize(char fmt = YUV422_UYVY, int width = 128, int unit = 1);

private:
	int			WriteBlk444(char *buf, int size);
	int			WriteBlk422(char *buf, int size);
	int			WriteBlk420(char *buf, int size);
	int			WriteBlk411(char *buf, int size);
	int			WriteBlk400(char *buf, int size);

	void		WriteBlk444Seg(char *buf);
	void		WriteBlk422Seg(char *buf);
	void		WriteBlk420Seg(char *buf);
	void		WriteBlk411Seg(char *buf);
	void		WriteBlk400Seg(char *buf);
};


#endif


