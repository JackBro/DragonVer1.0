
#ifndef _TOOLS_JPEGENC_BASE_H_
#define _TOOLS_JPEGENC_BASE_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif


#include "jpegbase.h"
#include "toolbit/bitfifo.h"
#include "toolmath/basemath.h"
#include "toolfile/dbugtools.h"


#define ABRC_B			273
enum RUDEBRCMODEenum {
	RUDEBRC_ZERO,
	RUDEBRC_QUAR,
	RUDEBRC_HALF,
	RUDEBRC_3QUARS
};

typedef struct tag_TRudebrcOption {
	int		rude_en;
	int		rude_level;
	int		only_uv;
	int		rude_ac;
	int		rude_thd;
//	int		tcr;

	int		qfcomp0;
	int		qfcomp1;
	int		qfcomp2;
	int		qfcomp3;
	int		qfcomp4;
	int		qfcomp5;

	int		thdl1;
	int		thdl2;
	int		thdr1;
	int		thdr2;

	int		shiftl1;
	int		shiftl2;
	int		shiftr1;
	int		shiftr2;
}TRudebrcOption, *PTRudebrcOption;

//===================================================
//
//		CJpegEncBase class
//
//===================================================
class TOOLJPG_DLL CJpegEncBase : public CBitFifo, public CJpegBase, public CBaseMath
{
public:
	CJpegEncBase(int size = DEFAULT_JPEGDATASIZE);

public:
	virtual void	Open(void);
	virtual int		Write(char *buf, int len);
	virtual int		WriteBrc(char *buf, int len);
	virtual int		Read(char *buf, int size);

public:
	void	SetJpegSize(TSize size);
	void	SetJpegSize(int width, int height);
	void	SetYuvFmt(int fmt);
	int		calcQ(int bytesize);
	int		RudeBrcQ(int bytesize);

protected:
	void	Init(void);
	void	generateJpegHeader(void);
	void	writeJpgHeader(void);
	void	writeDqtHeader(void);
	void	writeDhtHeader(void);

protected:
	void	encodeDC(int DCvalue, int *lastDC, THuffValue *pEncTable);
	void	encodeAC(int *zzblk, THuffValue *pEncTable);
	void	eofFlush(void);
	void	rudeAC(int *blk, int num);
	int		IsApplyRude(void);

protected:
	virtual void	encblock(char *block, int *lastDc, int mode) = 0;

public:
	int		getVwc(void);

protected:
	TJpegTable	m_Table;
	int			m_qf;
	int			m_tcr;
	int			m_rudeapply;
	int			m_pos;
	int			m_vwc;
	int			m_lastYdc, m_lastUdc, m_lastVdc;
	int			m_yuv;
	TSize		m_ImgSize;
	CDbugTools	m_dbug;
	TRudebrcOption	m_rudeoption;
	CFileBin	m_fb, m_fb1;
};


#endif

