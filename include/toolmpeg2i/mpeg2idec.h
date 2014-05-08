
#ifndef _TOOLS_MPEG2IDEC_H_
#define _TOOLS_MPEG2IDEC_H_


#ifndef TOOLMPEG2I_DLL
	#define TOOLMPEG2I_DLL  __declspec(dllimport)
#endif


#include "mpeg2decbase.h"
#include "tooljpg/jpegtable.h"
#include "toolfile/dbugtools.h"

#define DEFAULT_MPEGGAPSIZE		0x1000

enum MPEGDECODEMODEenum {
	MPEG_DECNORMAL,
	MPEG_DECFLUSH
};


class TOOLMPEG2I_DLL CMpeg2Decoder : public CMpeg2DecBase
{
public:
	CMpeg2Decoder(int gap = DEFAULT_MPEGGAPSIZE);

public:
	virtual void Open(void);
	int	 Read(char *buf, int size, int mode = MPEG_DECNORMAL);
//	int	 Write(char *buf, int len);

public:
	
	void Slice(char *block, int verpos);
	void MacroBlock(char *block);
	void SkipMacroBlock(char *block);
	void decIntraBlock(char *block, int *qt, THuffValue *tbl, int *dc);
	void decInterBlock(char *block, int *qt);
	
	int	 GetHuffDcVal(PTHuffValue table, int bitlen, int tsize);
	void GetHuffAcVal(int *blk, int Intra);
	void GetIntraAcVal(int *blk, int intravlc);
	void GetInterAcVal(int *blk);
	int	 GetRunLevel(int val, int len, int *pos, int *blk, int Intra);
		
	void SaveMB(char *block, int mb_row, int mb_column);
	void ClearBlock(char *block);
	
	void MontionCompensation(char *block, int mb_row, int mb_column);
	void Y_Compensation(char *curblk, int x_pos, int y_pos, int type);
	void UV_Compensation(char *curblk, int x_pos, int y_pos, int type);

private:
	int		m_gapsize;
	int		m_mode;
	
//	CFileBin	fb;
	CDbugTools	m_dbug;
};

#endif

