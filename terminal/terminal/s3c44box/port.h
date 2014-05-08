
#ifndef _ARM_PORT_H_
#define _ARM_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TPortCall)(void);

enum EXINTINDEXenum {
	EXINT0,
	EXINT1,
	EXINT2,
	EXINT3,
	EXINT4567
};

enum INTTRGTYPEenum {
	LOWLEVEL,
	HIGHLEVEL,
	FALLEDGE,
	RISEEDGE,
	BOTHEDGE
};

void	Led_Display(int LedStatus);
void	beep(int BeepStatus);

void	Port_Init(void);
void	ExtInt_Init(int idx);
void	EnableExInt(int idx);
void	DisableExInt(int idx);
void	SetExtIntTrgType(int idx, int mode);
int		GetExtIntTrgType(int idx);

void	SetPortCall(int index, TPortCall pcall);

#ifdef __cplusplus
}
#endif

#endif



