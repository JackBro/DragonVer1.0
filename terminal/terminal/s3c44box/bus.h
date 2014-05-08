#ifndef _HOST_BUS_H_
#define _HOST_BUS_H_

#ifdef __cplusplus
extern "C" {
#endif


#define BUS8		0
#define BUS16		1
#define BUS32		2
#define ENWAIT		4
#define SRAMBE03	8

enum BUSINDEXenum {
	BUS_IDX_0,
	BUS_IDX_1,
	BUS_IDX_2,
	BUS_IDX_3,
	BUS_IDX_4,
	BUS_IDX_5,
	BUS_IDX_6,
	BUS_IDX_7,
	BUS_TOTAL
};


void	BusInit(int idx, int type);
void	PrintBusTiming(int idx);
void	AdjustBusTiming(int idx, int tacs, int tcos, int tacc, int toch, int tcah, int tacp);


#ifdef __cplusplus
}
#endif

#endif