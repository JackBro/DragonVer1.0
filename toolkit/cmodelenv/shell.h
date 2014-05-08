#ifndef _CMODEL_SHELL_H_
#define _CMODEL_SHELL_H_

void	shellInit(void);
void	shell(void);
void	tmPrintf(char *fmt,...);
int		tmReadBuf(char *pt, int len);
int		tmWriteBuf(char *pt, int len);

#endif