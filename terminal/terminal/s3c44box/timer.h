
#ifndef _HOSTTIMER_H_
#define _HOSTTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TTimerCall)(void);

enum TIMERINDEXenum {
	TIMER0,
	TIMER1,
	TIMER2,
	TIMER3,
	TIMER4,
	TIMER5
};

void	TimerInit(void);
void	Delay(int time);

void	SetHostTimer(int idx, int dtime);	//0.1ms unit
int		GetHostTimer(int idx);
void	SetTimerCall(int index, TTimerCall pcall);

void	EnableTimerInt(int idx);
void	DisableTimerInt(int idx);

void	WDTInit(void);
void	EnableWdtInt(void);
void	DisableWdtInt(void);
void	WdtStart(void);
unsigned WdtGetCount(void);
void	WdtStop(void);

#ifdef __cplusplus
}
#endif

#endif


