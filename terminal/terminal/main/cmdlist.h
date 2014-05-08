#ifndef _CMDLIST_H_
#define _CMDLIST_H_

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_PARAMETER	50

enum CMDTYPEenum
{
	SETCMD_TYPE,
	READCMD_TYPE,
	BUFCMD_TYPE
};

enum PARAMTYPEenum{
	PTR_IN = 1,
	PTR_OUT,
	PTR_INOUT
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
} TCmdUnit, *PTCmdUnit;

typedef struct tag_TCmdGroup
{
	TCmdUnit *cmdlist;
	int		 count;
} TCmdGroup, *PTCmdGroup;

extern const TCmdGroup gCmdGroup[];
extern const int gCmdGroupCount;



#ifdef __cplusplus
}
#endif

#endif


