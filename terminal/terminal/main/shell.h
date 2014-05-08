#ifndef _SHELL_H_
#define _SHELL_H_

#ifdef __cplusplus
extern "C" {
#endif


#define UPKEY		0x1
#define DOWNKEY		0X4
#define LEFTKEY		0x8
#define RIGHTKEY	0x10

#define SEL5KEY		0x20
#define SEL6KEY		0x40

#define KEYMASK		(UPKEY | DOWNKEY | LEFTKEY | RIGHTKEY)
#define NOKEYDOWN	0x30

void	KeyScanStart(void);
void	KeyScanStop(void);


void shell(void);
int GetCmdLocation(int index, int *x, int *y);

extern volatile int gArmRun;

#ifdef __cplusplus
}
#endif

#endif



