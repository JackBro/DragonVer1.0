#ifndef _SIMREG_SHELL_H_
#define _SIMREG_SHELL_H_


#ifndef CHANNELSIMREGSRAM_DLL
	#define CHANNELSIMREGSRAM_DLL  __declspec(dllimport)
#endif


void	CHANNELSIMREGSRAM_DLL ShellOpen(void);
void	CHANNELSIMREGSRAM_DLL ShellClose(void);
void	CHANNELSIMREGSRAM_DLL Shell(void);
void	CHANNELSIMREGSRAM_DLL SimPrintf(char *fmt,...);
int	CHANNELSIMREGSRAM_DLL SimReadBuf(char *pt, int len);
int	CHANNELSIMREGSRAM_DLL SimWriteBuf(char *pt, int len);

#endif

