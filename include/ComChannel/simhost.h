#ifndef _SIMHOST_H_
#define _SIMHOST_H_


#ifndef COMCHANNEL_DLL
	#define COMCHANNEL_DLL __declspec(dllimport)
#endif

#ifdef __cplusplus
extern  "C" {
#endif


extern COMCHANNEL_DLL void	SimHost_SetReg(unsigned long adr, unsigned long val, unsigned char regWType);
extern COMCHANNEL_DLL unsigned long SimHost_GetReg(unsigned long adr, unsigned char regWType);
extern COMCHANNEL_DLL int		SimHost_ReadMemory(int adr);
extern COMCHANNEL_DLL void	SimHost_WriteMemory(int adr, int val);

extern COMCHANNEL_DLL int		SimHost_WriteBuf(int adr, char *buf, int size);
extern COMCHANNEL_DLL int		SimHost_ReadBuf(int adr, char *buf, int size);
extern COMCHANNEL_DLL unsigned long SimHost_ReadSram(unsigned long adr, unsigned char *buf, unsigned long size);
extern COMCHANNEL_DLL unsigned long SimHost_WriteSram(unsigned long adr, unsigned char *buf, unsigned long size);
extern COMCHANNEL_DLL unsigned long SimHost_WriteSramRev(unsigned long adr, unsigned char *buf, unsigned long size);

extern COMCHANNEL_DLL unsigned long SimHost_V9ReadSram(unsigned long adr, unsigned char *buf, unsigned long size);
extern COMCHANNEL_DLL unsigned long SimHost_V9WriteSram(unsigned long adr, unsigned char *buf, unsigned long size);
       
extern COMCHANNEL_DLL void	SimHost_PrintMsg(char *str);
extern COMCHANNEL_DLL void	SimHost_Delay(unsigned long count);
       
extern COMCHANNEL_DLL int		SimHost_GetHostMclk(void);
extern COMCHANNEL_DLL int		SimHost_GetI2sClk(void);
extern COMCHANNEL_DLL int		SimHost_GetWdtCnt(void);
       
extern COMCHANNEL_DLL void	SimHost_WriteNandCmd(int cmd, int unit);
extern COMCHANNEL_DLL void	SimHost_WriteNandAddr(int adr, int unit);
extern COMCHANNEL_DLL void	SimHost_WriteNandData(int data, int unit);
extern COMCHANNEL_DLL int		SimHost_ReadNandData(int unit);
       
extern COMCHANNEL_DLL void	SmPrintf(char *fmt,...);
       
extern COMCHANNEL_DLL void	SimUsb_SetReg(unsigned char adr, unsigned char val);
extern COMCHANNEL_DLL unsigned char SimUsb_GetReg(unsigned char adr);
extern COMCHANNEL_DLL unsigned char SimUsb_ReadFifo(unsigned char adr, unsigned char *buf, unsigned char size);
extern COMCHANNEL_DLL unsigned char SimUsb_WriteFifo(unsigned char adr, unsigned char *buf, unsigned char size);
extern COMCHANNEL_DLL void	SaveCmd(char *str);
extern COMCHANNEL_DLL void	DownloadFile(char *src);
extern COMCHANNEL_DLL void	SaveArmFile(char *src);
extern COMCHANNEL_DLL void	PcFile2Arm(char *src, char *dst);
extern COMCHANNEL_DLL void	ArmFile2Pc(char *src, char *dst);
       
extern COMCHANNEL_DLL void	SendCmd(char *str);
extern COMCHANNEL_DLL int		SendRetCmd(char *str);
extern COMCHANNEL_DLL int		SendDelayRetCmd(char *str);
       
extern COMCHANNEL_DLL int		V5_SendCommand_(int msg, void *param, int paramsize);

#ifdef __cplusplus
}
#endif

#endif

