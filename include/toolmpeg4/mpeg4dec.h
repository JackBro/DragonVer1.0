
#ifndef _TOOLS_MPEG4DEC_H_
#define _TOOLS_MPEG4DEC_H_

#ifndef TOOLMPEG4_DLL
	#define TOOLMPEG4_DLL  __declspec(dllimport)
#endif

#include "mpeg4base.h"

#define DEFAULT_MPEGGAPSIZE		0x1000

enum MPEG4DECODEMODEenum {
	MPEG4_DECNORMAL,
	MPEG4_DECFLUSH
};



class TOOLMPEG4_DLL CMpeg4Decoder : public CMpeg4DecBase
{
public:
	CMpeg4Decoder(int gap = DEFAULT_MPEGGAPSIZE);

public:
	virtual void	Open(void);
	int		Read(char *buf, int size, int mode = MPEG4_DECNORMAL);
//	int		Write(char *buf, int len);

public:
	void	decframe(char *buf);
	void	MacroBlock(char *block, int mbaddr);
	void	SkipMacroBlock(char *block, int mbaddr);
	void	SaveMB(void);
	
	void	MotionCompensation(void);

	void	decIntraBlock(char *block, int mbaddr, int blknum);
	void	decInterBlock(char *block);
	int		GetIntraDCval(int lum);
	void	GetIntraACval(int *blk, int direct, int addr);
	int		GetlumDcSize(void);
	int		GetChromDcSize(void);	

	int		GetIntraVLC(void);
	TMpeg4Event GetIntraEvent(void);

	void	iquant263(int inblk, int oublk);
	void	iquantmpeg4(int inblk, int oublk, int*QTable);


private:
	int		m_gapsize;
	int		m_mode;
};


#endif