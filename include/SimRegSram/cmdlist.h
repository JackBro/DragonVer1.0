#ifndef _SIMREG_CMDLIST_H_
#define _SIMREG_CMDLIST_H_


#ifndef CHANNELSIMREGSRAM_DLL
	#define CHANNELSIMREGSRAM_DLL  __declspec(dllimport)
#endif


#define MAX_PARAMETER	50

enum CMDTYPEenum
{
	SETCMD_TYPE,
	READCMD_TYPE,
	BUFCMD_TYPE
};

typedef void CmdCall(int argc, char **argv);

typedef struct tag_TCmdUnit
{
	char	*cmdstr;
	int		minpara;
	int		maxpara;
	char	*helpstr;
	int		type;
	CmdCall	*pcall;
} TCmdUnit, *pTCmdUnit;

extern CHANNELSIMREGSRAM_DLL const TCmdUnit gSimRegCmdList[];
extern CHANNELSIMREGSRAM_DLL const int gSimRegCmdCount;


#endif