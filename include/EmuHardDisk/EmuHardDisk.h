#ifndef _EMUHARDDISK_BASE_H_
#define _EMUHARDDISK_BASE_H_


#ifndef EMUHARDDISK_DLL
	#define EMUHARDDISK_DLL  __declspec(dllimport)
#endif

#include "toolbuf/blockbuf.h"


#define		HD_DEF_CNUM				0x01
#define		HD_DEF_HNUM				0x01
#define		HD_DEF_SNUM				0x4000
#define		HD_DEF_SSIZE			0x200		// 0x4000 * 0x200 ==> 8M Hard Disk

typedef struct tag_THdInfo
{
	int	c_num;		// Total cylinder number
	int h_num;		// Total head number
	int s_num;		// Total sector number of one track
	int s_size;		// One sector size in bytes
} THdInfo, *PTHdInfo;



class EMUHARDDISK_DLL CEmuHdBase : public CBlockBuf
{
public :
	CEmuHdBase(int c_num = HD_DEF_CNUM, int h_num = HD_DEF_HNUM, int s_num = HD_DEF_SNUM, int s_size = HD_DEF_SSIZE);
	virtual ~CEmuHdBase(void);

public :
	void	SetSize(int c_num = HD_DEF_CNUM, int h_num = HD_DEF_HNUM, int s_num = HD_DEF_SNUM, int s_size = HD_DEF_SSIZE);
	void	SetSize(char *buf, int c_num = HD_DEF_CNUM, int h_num = HD_DEF_HNUM, int s_num = HD_DEF_SNUM, int s_size = HD_DEF_SSIZE);

	int		Read( char *buf, int c_idx = 0, int h_idx = 0, int s_idx = 0, int s_cnt = 1);
	int		Write(char *buf, int c_idx = 0, int h_idx = 0, int s_idx = 0, int s_cnt = 1);

protected :
	THdInfo	m_info;
};

#endif
