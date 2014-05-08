
#ifndef _HOSTINIT_H_
#define _HOSTINIT_H_

#ifdef __cplusplus
extern "C" {
#endif

void Led_Display(int LedStatus);
void beep(int BeepStatus);

int  GetHostMclk(void);
void  SetHostMclk(int mclk);
int  GetI2sClk(void);
void SetI2sClk(int clk);

void HostInit(void);

#ifdef WIN32

#define   Push2DisIntEnFlag() 
#define   PopIntEnFlag()

#else

/*for test*/
//void   Push2DisIntEnFlag(void); 
//void   PopIntEnFlag(void);
#define   Push2DisIntEnFlag() 
#define   PopIntEnFlag()

#endif


#ifdef __cplusplus
}
#endif

#endif



