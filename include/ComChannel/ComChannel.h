
#ifndef __CHANNELCOM_COMCHANNEL_H__
#define __CHANNELCOM_COMCHANNEL_H__


#ifndef COMCHANNEL_DLL
	#define COMCHANNEL_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"
#include "toolfile/filebin.h"
#include "UartChannel/UartChannel.h"
#include "SimRegSram/SimRegSram.h"
#include "UsbChannel/UsbChannel.h"
#include "ComChannel/ComChannel.h"
#include "ComChannel/ComChannel.h"

enum CMDCHANNELenum {
	CHANNEL_CMODEL,
	CHANNEL_UART,
	CHANNEL_SIMCMD,
	CHANNEL_USB
};

#define SAVECMD_FILE	"savecmd.lst"

#define SAVEHCMD_FILE	"highcmd.lst"

#define MAX_WAITING_TIMES	13
#define MAX_CHECKING_TIMES	10
#define CHECK_DELAY_TIME	10

typedef void DisplayContentCall(char *buf, int size);
typedef void ProgressCall(int rest, int total);

class COMCHANNEL_DLL CComChannel : public CUartChannel, public CSimRegSram, public CUsbChannel//, public CSimChannel
{
public:
	CComChannel();

public:
	virtual void	Open(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);
	virtual int		IsOpened(void);
	virtual void	Close(void);

	void	SetCallBack(ChannelCallBack *pcall);
	void	SetRWmode(int mode = CALLBACK_MODE);
	int		GetRWmode(void);

public:
	void	ChannelSelect(int sel = CHANNEL_UART);
	int		GetChannelSel(void);
	void	SetDispCall(DisplayContentCall *pcall);
	void	SetProgCall(ProgressCall *pcall);
	void	DisplayContent(char *str, int len);

protected:
	virtual int		bRead(char *buf, int len);
	virtual void 	ReadCnt(char *buf, int len);

protected:
	int		ReadRetVal(char *buf, int buflen);
	void	RemoveDummyData(void);
	void	SendChkCmd(char *str);
	int		DownBin(char *buf, int len);
	void	SendCheckCmd(char *str);
	int		ReadBinSize(char *buf, int size);

protected:
	int		SendRetCmd(char *cmd, char *buf, int buflen);
	int		SendDelayRetCmd(char *cmd, char *buf, int buflen);
	int		DownCmdBin(char *cmd, char *buf, int buflen);
	int		ReadCmdBin(char *cmd, char *buf, int buflen);

public:
	void	SendCmd(char *cmd);
	void	SetChipRegCmd(char *head, int adr, int val, int regWType);
	int		GetChipRegCmd(char *head, int adr, int regWType);
	void	SetRegCmd(char *head, int adr, int val);
	int		GetRegCmd(char *head, int adr);
	int		WriteBufCmd(char *head, int adr, char *buf, int len);
	int		ReadBufCmd(char *head, int adr, char *buf, int len);
	int		DecGetInfoCmd(char *fmt, ...);
	int		HexGetInfoCmd(char *fmt, ...);
	int		HexDelayGetInfoCmd(char *fmt, ...);
	void	Read4hcmd(char *buf, int len);
	int		V5_SendCommand_(UINT32 msg, void *param, UINT32 paramsize);

public:
	void	DownloadFile(char *src);
	void	SaveBinFile(char *src);
	void	Pc2ArmFile(char *emf, char *pcf);
	void	Arm2PcFile(char *emf, char *pcf);

public:
	void	SetRecordPath(char *path);
	
	void	StartRecordRegCmd(char *fn = SAVECMD_FILE);
	void	StartRecordCmd(char *fn = SAVEHCMD_FILE);
	void	StopRecordRegCmd(void);
	void	StopRecordCmd(void);

	void	SaveSetChipRegCmd(int adr, int val, char regWType);
	void	SaveSetRegCmd(int adr, int val);
	void	SaveCmd(char *str);

private:
	UINT32 V5_SendCommand_GETCAPTUREINFO_TOFILE(UINT32 msg, void *param, UINT32 paramsize);
	UINT32 V5_SendCommand_DECODEFRAMEENCODE(UINT32 msg, void *param, UINT32 paramsize);
	UINT32 V5_SendCommand_DECODEIPPENCODE(UINT32 msg, void *param, UINT32 paramsize);

private:
	char	m_path[0x100];
	CFileBin	m_fb;
	CFileBin	m_fbcmd;

	DisplayContentCall *m_pDispContCall;
	ProgressCall	*m_pProgCall;

	int		m_sel;
};


extern COMCHANNEL_DLL CComChannel gComChnl;


#endif
