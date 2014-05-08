#ifndef _EMUHARDDISK_H_
#define _EMUHARDDISK_H_


#ifndef EMUHARDDISK_DLL
	#define EMUHARDDISK_DLL  __declspec(dllimport)
#endif

#include "EmuHardDisk.h"


#define		HD_PATH_MAX_LENGTH		0x100

class EMUHARDDISK_DLL CEmuHd : public CEmuHdBase
{
public :
	CEmuHd(int c_num = HD_DEF_CNUM, int h_num = HD_DEF_HNUM, int s_num = HD_DEF_SNUM, int s_size = HD_DEF_SSIZE);
	virtual ~CEmuHd(void);

public :
	void		SetSize(int c_num = HD_DEF_CNUM, int h_num = HD_DEF_HNUM, int s_num = HD_DEF_SNUM, int s_size = HD_DEF_SSIZE);
	void		GetInfo(PTHdInfo pinfo);

	void		Open(char *src = NULL);
	void		Close(void);

	int			Read(char *buf, int c_idx = 0, int h_idx = 0, int s_idx = 0, int s_cnt = 1);		// Access by cylinder/head/sector
	int			Write(char *buf, int c_idx = 0, int h_idx = 0, int s_idx = 0, int s_cnt = 1);		// Access by cylinder/head/sector
	int			ReadEx(char *buf, int s_idx = 0, int s_cnt = 1);									// Access by sector
	int			WriteEx(char *buf, int s_idx = 0, int s_cnt = 1);									// Access by sector

protected :
	void		Init(void);

private :
	char		m_path[HD_PATH_MAX_LENGTH];
};

#endif

