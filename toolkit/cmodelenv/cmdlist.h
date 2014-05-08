#ifndef _CMODEL_CMDLIST_H_
#define _CMODEL_CMDLIST_H_


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

extern const TCmdUnit gCmodelCmdList[];
extern const int gCmodelCmdCount;


#endif