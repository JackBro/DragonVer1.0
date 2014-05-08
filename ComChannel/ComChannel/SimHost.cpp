#include "internal.h"


void SimHost_SetReg(unsigned long adr, unsigned long val, unsigned char regWType)
{
	gComChnl.SetChipRegCmd("V5_SetReg", adr, val, regWType);
	gComChnl.SaveSetChipRegCmd((int)adr, (int)val, (char)regWType);
}

unsigned long SimHost_GetReg(unsigned long adr, unsigned char regWType)
{
	return (unsigned long)(gComChnl.GetChipRegCmd("V5_GetReg", adr, regWType));
}

int SimHost_ReadMemory(int adr)
{
	return gComChnl.GetRegCmd("ReadMemory", adr);
}

void SimHost_WriteMemory(int adr, int val)
{
	gComChnl.SetRegCmd("WriteMemory", adr, val);
}

int SimHost_WriteBuf(int adr, char *buf, int size)
{
	return gComChnl.WriteBufCmd("WriteBuf", adr, buf, size);
}

int SimHost_ReadBuf(int adr, char *buf, int size)
{
	return gComChnl.ReadBufCmd("ReadBuf", adr, buf, size);
}

unsigned long SimHost_ReadSram(unsigned long adr, unsigned char *buf, unsigned long size)
{
	return gComChnl.ReadBufCmd("V5_ReadSram", adr, (char *)buf, size);
}

unsigned long SimHost_WriteSram(unsigned long adr, unsigned char *buf, unsigned long size)
{
	return gComChnl.WriteBufCmd("V5_WriteSram", adr, (char *)buf, size);
}

unsigned long SimHost_WriteSramRev(unsigned long adr, unsigned char *buf, unsigned long size)
{
	return gComChnl.WriteBufCmd("V5_WriteSramRev", adr, (char *)buf, size);
}

/******************* V938 write/read sram command ******************/
unsigned long SimHost_V9ReadSram(unsigned long adr, unsigned char *buf, unsigned long size)
{
	return gComChnl.ReadBufCmd("V9_ReadSram", adr, (char *)buf, size);
}

unsigned long SimHost_V9WriteSram(unsigned long adr, unsigned char *buf, unsigned long size)
{
	return gComChnl.WriteBufCmd("V9_WriteSram", adr, (char *)buf, size);
}

void SimHost_PrintMsg(char *str)
{
	str = str;
	//AfxMessageBox(str);
}

void SimHost_Delay(unsigned long count)
{
	Sleep(count);
}

int	SimHost_GetHostMclk(void)
{
	return gComChnl.DecGetInfoCmd("GetHostMclk\n");
}

int	SimHost_GetI2sClk(void)
{
	return gComChnl.DecGetInfoCmd("GetI2sClk\n");
}

int SimHost_GetWdtCnt(void)
{
	return gComChnl.DecGetInfoCmd("GetWdtCnt\n");
}

void SimHost_WriteNandCmd(int cmd, int unit)
{
	gComChnl.SetRegCmd("WriteNandCmd", cmd, unit);
}

void SimHost_WriteNandAddr(int adr, int unit)
{
	gComChnl.SetRegCmd("WriteNandAddr", adr, unit);
}

void SimHost_WriteNandData(int data, int unit)
{
	gComChnl.SetRegCmd("WriteNandData", data, unit);
}

int SimHost_ReadNandData(int unit)
{
	return gComChnl.GetRegCmd("ReadNandData", unit);
}

void SmPrintf(char *fmt,...)
{
	int len;
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	len = (int)strlen(str);
	gComChnl.DisplayContent(str, len);
}

//=================== simulate usb r/w

void SimUsb_SetReg(unsigned char adr, unsigned char val)
{
	gComChnl.SetRegCmd("Usb_SetReg", adr, val);
}

unsigned char SimUsb_GetReg(unsigned char adr)
{
	return (unsigned char)(gComChnl.GetRegCmd("Usb_GetReg", adr));
}

unsigned char SimUsb_ReadFifo(unsigned char adr, unsigned char *buf, unsigned char size)
{
	return (unsigned char)(gComChnl.ReadBufCmd("Usb_ReadFifo", adr, (char *)buf, size));
}

unsigned char SimUsb_WriteFifo(unsigned char adr, unsigned char *buf, unsigned char size)
{
	return (unsigned char)(gComChnl.WriteBufCmd("Usb_WriteFifo", adr, (char *)buf, size));
}

void SaveCmd(char *str)
{
	gComChnl.SaveCmd(str);
}

void DownloadFile(char *src)
{
	gComChnl.DownloadFile(src);
}

void SaveArmFile(char *src)
{
	gComChnl.SaveBinFile(src);
}

void PcFile2Arm(char *src, char *dst)
{
	gComChnl.Pc2ArmFile(dst, src);
}

void ArmFile2Pc(char *src, char *dst)
{
	gComChnl.Arm2PcFile(src, dst);
}

void SendCmd(char *str)
{
	gComChnl.SendCmd(str);
}

int SendRetCmd(char *str)
{
	return gComChnl.HexGetInfoCmd(str);
}

int SendDelayRetCmd(char *str)
{
	return gComChnl.HexDelayGetInfoCmd(str);
}

int V5_SendCommand_(int msg, void *param, int paramsize)
{
	return gComChnl.V5_SendCommand_( msg, param, paramsize);
}