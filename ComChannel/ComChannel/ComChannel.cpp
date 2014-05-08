#include "internal.h"
#include "../terminal/terminal/main/main.h"


CComChannel gComChnl;

CComChannel::CComChannel(void)
{
	this->m_sel = CHANNEL_UART;
	this->m_pDispContCall = NULL;
	this->m_pProgCall = NULL;
}

//==========================================
//
//		general function
//
//===================================
void CComChannel::Open(void)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	CSimChannel::Open();
	//	break;
	case CHANNEL_UART:
		CUartChannel::Open();
		break;
	case CHANNEL_SIMCMD:
		CSimRegSram::Open();
		break;
	case CHANNEL_USB:
		CUsbChannel::Open();
		break;
	default:
		break;
	}
	this->SetRWmode();
}

int	CComChannel::IsOpened(void)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	return CSimChannel::IsOpened();
	case CHANNEL_UART:
		return CUartChannel::IsOpened();
	case CHANNEL_SIMCMD:
		return CSimRegSram::IsOpened();
	case CHANNEL_USB:
		return CUsbChannel::IsOpened();
	default:
		break;
	}
	return FALSE;
}

void CComChannel::Close(void)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	CSimChannel::Close();
	//	break;
	case CHANNEL_UART:
		CUartChannel::Close();
		break;
	case CHANNEL_SIMCMD:
		CSimRegSram::Close();
		break;
	case CHANNEL_USB:
		CUsbChannel::Close();
		break;
	default:
		break;
	}
}

int	CComChannel::Write(char *buf, int len)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	return CSimChannel::Write(buf, len);
	case CHANNEL_UART:
		return CUartChannel::Write(buf, len);
	case CHANNEL_SIMCMD:
		return CSimRegSram::Write(buf, len);
	case CHANNEL_USB:
		return CUsbChannel::Write(buf, len);
	default:
		break;
	}
	return 0;
}

int	CComChannel::bRead(char *buf, int len)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	return CSimChannel::Read(buf, len);
	case CHANNEL_UART:
		return CUartChannel::Read(buf, len);
	case CHANNEL_SIMCMD:
		return CSimRegSram::Read(buf, len);
	case CHANNEL_USB:
		return CUsbChannel::Read(buf, len);
	default:
		break;
	}
	return 0;
}

int	CComChannel::Read(char *buf, int len)
{
	int leng = 0, flag = 0, size = len, delay = 1;
	int count = MAX_WAITING_TIMES;

	if(this->IsOpened() == 0)
		return 0;
	while(count--)
	{
		leng = this->bRead(buf, len);
		buf += leng;
		len -= leng;
		if(len == 0)
			break;
		if(leng == 0)
		{
			if( (this->GetRWmode() == CALLBACK_MODE) || (this->m_sel == CHANNEL_SIMCMD) )
				break;
			if(flag == 1)
				break;
			delay <<= 1;
		}
		else if(flag == 0)
		{
			flag = 1;
			count = MAX_WAITING_TIMES;
			delay = 1;
		}
		if(this->m_sel != CHANNEL_SIMCMD)
			Sleep(delay);
	}
	return size - len;
}

void CComChannel::SetCallBack(ChannelCallBack *pcall)
{
	//CSimChannel::SetCallBack(pcall);
	CUartChannel::SetCallBack(pcall);
	CSimRegSram::SetCallBack(pcall);
	CUsbChannel::SetCallBack(pcall);

}

void CComChannel::SetRWmode(int mode)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	return CSimChannel::Read(buf, len);
	case CHANNEL_UART:
		CUartChannel::SetRWmode(mode);
		break;
	case CHANNEL_SIMCMD:
		CSimRegSram::SetRWmode(mode);
		break;
	case CHANNEL_USB:
		CUsbChannel::SetRWmode(mode);
		break;
	default:
		break;
	}
}

int	CComChannel::GetRWmode(void)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	return CSimChannel::Read(buf, len);
	case CHANNEL_UART:
		return CUartChannel::GetRWmode();
	case CHANNEL_SIMCMD:
		return CSimRegSram::GetRWmode();
	case CHANNEL_USB:
		return CUsbChannel::GetRWmode();
		break;
	default:
		break;
	}
	return CALLBACK_MODE;
}

//==========================================
//
//		self function
//
//===================================
void CComChannel::ChannelSelect(int sel)
{
	if(sel > 3)
		sel = CHANNEL_UART;
	if(sel == this->m_sel)
		return;
	if(this->IsOpened())
	{
		this->Close();
		this->m_sel = sel;
		this->Open();
	}
	this->m_sel = sel;
}

int	CComChannel::GetChannelSel(void)
{
	return this->m_sel;
}

void CComChannel::SetDispCall(DisplayContentCall *pcall)
{
	this->m_pDispContCall = pcall;
}

void CComChannel::SetProgCall(ProgressCall *pcall)
{
	this->m_pProgCall = pcall;
}

void CComChannel::DisplayContent(char *str, int len)
{
	if(this->m_pDispContCall)
	{
		this->m_pDispContCall(str, len);
	}
}

//==========================================
//
//		command function
//
//===================================
void CComChannel::SendCmd(char *cmd)
{
	int len;

	len = (int)strlen(cmd);
	this->Write(cmd, len);
}

int CComChannel::ReadRetVal(char *buf, int buflen)
{
	buflen = this->Read(buf, buflen);
	this->DisplayContent(buf, buflen);
	return buflen;
}

void CComChannel::ReadCnt(char *buf, int len)
{
	int len1, flag = MAX_CHECKING_TIMES;

	while( len && flag )
	{
		len1 = this->Read(buf, len);
		if(len1 == 0)
			flag--;
		else
		{
			this->DisplayContent(buf, len1);
			len -= len1;
			buf += len1;
		}
	}
}


void CComChannel::Read4hcmd(char *buf, int len)
{
	int len1;

	while( len )
	{
		len1 = this->Read(buf, len);
		len -= len1;
		buf += len1;
	}
}


void CComChannel::SendChkCmd(char *str)
{
	char buf[100];
	int len, pos, flag = MAX_CHECKING_TIMES<<3;
	CStringTools tools;

	len = (int)strlen(str);
	len = this->Write(str, len);
	pos = len;
	do {
		this->ReadCnt(buf+(len-pos), pos);
		pos = tools.FindLocate(buf, str, len);
		if(pos == 0)
			return;
		if(len != pos)
			memcpy(buf, buf+pos, len-pos);
	} while(flag--);
}

void CComChannel::RemoveDummyData(void)
{
	char buf[0x100];
	int len;//, flag = 1;

	if(this->m_sel == CHANNEL_SIMCMD)
		return;
//	while(flag) {
	do {
		Sleep(1);
		len = this->Read(buf, 0x100);
		//if(len)
		//	flag = 1;
		//else if(flag < 4)
		//	flag++;
		//else
		//	flag = 0;
		this->DisplayContent(buf, len);
	} while(len);
}

void CComChannel::SendCheckCmd(char *str)
{
	this->RemoveDummyData();
	this->SetRWmode(WAITING_MODE);
	this->SendChkCmd(str);
	this->SetRWmode();
}

int CComChannel::SendRetCmd(char *cmd, char *buf, int buflen)
{
	this->RemoveDummyData();
	this->SetRWmode(WAITING_MODE);
	this->SendChkCmd(cmd);
	buflen = this->ReadRetVal(buf, buflen);
	this->SetRWmode();
	return buflen;
}

int CComChannel::SendDelayRetCmd(char *cmd, char *buf, int buflen)
{
	this->RemoveDummyData();
	this->SetRWmode(WAITING_MODE);
	this->SendChkCmd(cmd);
	buflen = this->ReadRetVal(buf, buflen);
	this->SetRWmode();
	return buflen;
}

int	CComChannel::DownBin(char *buf, int len)
{
	int size, len1 = len;

	while(len1)
	{
		size = this->Write(buf, len1);
		len1 -= size;
		buf += size;
		if(this->m_pProgCall)
			this->m_pProgCall(len1, len);
	}
	return len;
}

int CComChannel::ReadBinSize(char *buf, int size)
{
	int len, flag = MAX_CHECKING_TIMES, temp = size;

	while(size && flag)
	{
		len = this->Read(buf, size);
		size -= len;
		buf += len;
		if(len == 0)
			flag--;
		else
			flag = MAX_CHECKING_TIMES;
		if( (size) && (this->m_sel != CHANNEL_SIMCMD) )
			Sleep(1);
		if(this->m_pProgCall)
			this->m_pProgCall(size, temp);
	}
	return temp - size;
}

int	CComChannel::DownCmdBin(char *cmd, char *buf, int buflen)
{
	CStringTools tools;
	char str[0x100];

	this->RemoveDummyData();
	this->SetRWmode(WAITING_MODE);
	this->SendChkCmd(cmd);
	buflen = this->DownBin(buf, buflen);
	this->ReadRetVal(str, 0x100);
	this->SetRWmode();
	tools.Str2Hex(str, &buflen);
	return buflen;
}

int	CComChannel::ReadCmdBin(char *cmd, char *buf, int buflen)
{
	this->RemoveDummyData();
	this->SetRWmode(WAITING_MODE);
	this->SendChkCmd(cmd);
	buflen = this->ReadBinSize(buf, buflen);
	this->SetRWmode();
	return buflen;
}
	
//==========================================
//
//		high command function
//
//===================================
void CComChannel::SetChipRegCmd(char *head, int adr, int val, int regWType)
{
	char cmd[0x100];

	sprintf(cmd, "%s %x %x %x\n", head, adr, val, regWType);
	this->SendCmd(cmd);
}

int CComChannel::GetChipRegCmd(char *head, int adr, int regWType)
{
	CStringTools tools;
	char cmd[0x100];
	int val;

	sprintf(cmd, "%s %x %x\n", head, adr, regWType);
	adr = this->SendRetCmd(cmd, cmd, 0xff);
	cmd[adr] = 0;
	tools.Str2Hex(cmd, &val);
	return val;
}

void CComChannel::SetRegCmd(char *head, int adr, int val)
{
	char cmd[0x100];

	sprintf(cmd, "%s %x %x\n", head, adr, val);
	this->SendCmd(cmd);
}

int CComChannel::GetRegCmd(char *head, int adr)
{
	CStringTools tools;
	char cmd[0x100];
	int val;

	sprintf(cmd, "%s %x\n", head, adr);
	adr = this->SendRetCmd(cmd, cmd, 0xff);
	cmd[adr] = 0;
	tools.Str2Hex(cmd, &val);
	return val;
}

int	CComChannel::WriteBufCmd(char *head, int adr, char *buf, int len)
{
	char cmd[0x100];

	sprintf(cmd, "%s %d %d\n", head, adr, len);
	len = this->DownCmdBin(cmd, buf, len);
	return len;
}

int CComChannel::ReadBufCmd(char *head, int adr, char *buf, int len)
{
	char cmd[0x100];

	sprintf(cmd, "%s %d %d\n", head, adr, len);
	len = this->ReadCmdBin(cmd, buf, len);
	return len;
}

int	CComChannel::DecGetInfoCmd(char *fmt, ...)
{
	int len, val;
	CStringTools tools;
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);
	len = this->SendRetCmd(str, str, 0xff);
	str[len] = 0;
	tools.Str2Dec(str, &val);
	return val;
}

int	CComChannel::HexGetInfoCmd(char *fmt, ...)
{
	int len, val;
	CStringTools tools;
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);
	len = this->SendRetCmd(str, str, 0xff);
	str[len] = 0;
	tools.Str2Hex(str, &val);
	return val;
}

int	CComChannel::HexDelayGetInfoCmd(char *fmt, ...)
{
	int len, val;
	CStringTools tools;
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);
	len = this->SendDelayRetCmd(str, str, 0xff);
	str[len] = 0;
	tools.Str2Hex(str, &val);
	return val;
}

UINT32 CComChannel::V5_SendCommand_GETCAPTUREINFO_TOFILE(UINT32 msg, void *param, UINT32 paramsize)
{
	char str[0x100];
	UINT32 ret, param1size; 
	UINT8 param1offset, poffset = 0;
	TgetCaptureInfoToFileParam *pparam;

	pparam = (TgetCaptureInfoToFileParam *) (param);
	paramsize = sizeof(TgetCaptureInfoToFileParam);
	param1size = (UINT32) strlen(pparam ->filename) + 1;
	param1offset = (UINT8)((UINT32)(&(pparam ->filename)) - (UINT32) pparam);
	sprintf(str, "V5_SendCommand %d %d %x %x %d %x %x\n", msg, PTR_OUT, paramsize, poffset, PTR_OUT, param1size, param1offset);
	this->SendChkCmd(str);

	this->Write((char *) pparam, paramsize);
	this->Write(pparam ->filename, param1size);
	this->Read4hcmd((char *) &ret, sizeof(ret));
	return ret;
}

UINT32 CComChannel::V5_SendCommand_DECODEFRAMEENCODE(UINT32 msg, void *param, UINT32 paramsize)
{
	char str[0x100];
	UINT32 ret, param1size, param2size;
	UINT8 param1offset, param2offset, poffset = 0;
	TDecodeFrameEncodeParam *pparam;

	
	pparam = (TDecodeFrameEncodeParam *) (param);
	paramsize = sizeof(TDecodeFrameEncodeParam);
	param1size = (UINT32) strlen(pparam ->srcfile) + 1;
	param2size = (UINT32) strlen(pparam ->dstfile) + 1;
	param1offset = (UINT8)((UINT32)(&(pparam ->srcfile)) - (UINT32) pparam);
	param2offset = (UINT8)((UINT32)(&(pparam ->dstfile)) - (UINT32) pparam);
	sprintf(str, "V5_SendCommand %d %d %x %x %d %x %x %d %x %x\n", msg, PTR_OUT, paramsize, poffset, PTR_OUT, param1size, param1offset, PTR_OUT, param2size, param2offset);
	this->SendChkCmd(str);

	this->Write((char *) pparam, paramsize);
	this->Write(pparam ->srcfile, param1size);
	this->Write(pparam ->dstfile, param2size);
	this->Read4hcmd((char *) &ret, sizeof(ret));
	return ret;
}

UINT32 CComChannel::V5_SendCommand_DECODEIPPENCODE(UINT32 msg, void *param, UINT32 paramsize)
{
	char str[0x100];
	UINT32 ret, param1size, param2size;
	UINT8 param1offset,param2offset,  poffset = 0;
	TDecodeIPPEncodeParam *pparam;

	pparam = (TDecodeIPPEncodeParam *) (param);
	paramsize = sizeof(TDecodeIPPEncodeParam);
	param1size = (UINT32) strlen(pparam ->srcfile) + 1;
	param2size = (UINT32) strlen(pparam ->dstfile) + 1;
	param1offset = (UINT8)((UINT32)(&(pparam ->srcfile)) - (UINT32) pparam);
	param2offset = (UINT8)((UINT32)(&(pparam ->dstfile)) - (UINT32) pparam);
	sprintf(str, "V5_SendCommand %d %d %x %x %d %x %x %d %x %x\n", msg, PTR_OUT, paramsize, poffset, PTR_OUT, param1size, param1offset, PTR_OUT, param2size, param2offset);
	this->SendChkCmd(str);

	this->Write((char *) pparam, paramsize);
	this->Write(pparam ->srcfile, param1size);
	this->Write(pparam ->dstfile, param2size);
	this->Read4hcmd((char *) &ret, sizeof(ret));
	return ret;
}

int CComChannel::V5_SendCommand_(UINT32 msg, void *param, UINT32 paramsize)
{
	char str[0x100];
	UINT32 ret; //param2size;
	UINT8  poffset = 0;

	this->RemoveDummyData();
	this->SetRWmode(WAITING_MODE);

	switch ( msg )
	{
	case V5M_GETCAPTUREINFO_TOFILE:
		ret = this->V5_SendCommand_GETCAPTUREINFO_TOFILE(msg, param, paramsize);
		break;

	case V5M_DECODEFRAMEENCODE:
		ret = this->V5_SendCommand_DECODEFRAMEENCODE(msg, param, paramsize);
		break;
		
	case V5M_DECODEIPPENCODE:
		ret = this->V5_SendCommand_DECODEIPPENCODE(msg, param, paramsize);
		break;

	default:	
		if (param && paramsize) 
		{
			sprintf(str, "V5_SendCommand %d %d %x %x\n", msg, PTR_INOUT, paramsize, poffset);
			this->SendChkCmd(str);
			
			this->Write((char *)param, paramsize);
			this->Read4hcmd((char *) &ret, sizeof(ret));
			this->Read4hcmd((char *)param, paramsize);
		}
		else
		{
			sprintf(str, "V5_SendCommand %d\n", msg);
			this->SendChkCmd(str);
			this->Read4hcmd((char *) &ret, sizeof(ret));
		}
		break;
	}

	this->SetRWmode();
	return (int)ret;			
}

//==========================================
//
//		file operate function
//
//===================================
void CComChannel::DownloadFile(char *src)
{
	CFileBin fb;
	CBufferBase pool;
	int size = pool.GetTotalSize();
	int length, len, len1, sum = 0;

	if(size == 0)
		return;

	fb.Open(src);
	length = fb.GetFileLength();
	len = length + 4 + 2;
	this->DownBin( (char *)&len, 4);
	while(length)
	{
		size = length > size ? size : length;
		len = fb.Read(pool.m_buf, size);
		for(len1 = 0; len1 < len; len1++)
			sum += (int)(pool.m_buf[len1]) & 0xff;
		length -= len;
		this->DownBin(pool.m_buf, len);
	}
	this->DownBin( (char *)&sum, 2);
}

void CComChannel::SaveBinFile(char *src)
{
	CFileBin fb;
	CBufferBase pool;
	int size = pool.GetTotalSize();
	int length, len;

	if(size == 0)
		return;
	len = this->ReadBinSize((char *)&length, 4);
	if(len != 4)
		return;

	fb.Open(src, "wb");
	while(length)
	{
		size = length > size ? size : length;
		len = this->ReadBinSize(pool.m_buf, size);
		if(len == 0)
			break;
		fb.Write(pool.m_buf, len);
		length -= len;
	}
}

void CComChannel::Pc2ArmFile(char *emf, char *pcf)
{
	char str[100];

	if(emf[0] == '\0')
		return;
	sprintf(str, "upload /flash/%s\n", emf);
	this->SendCmd(str);
	this->DownloadFile(pcf);
}

void CComChannel::Arm2PcFile(char *emf, char *pcf)
{
	char str[100];

	if(emf[0] == '\0')
		return;
	sprintf(str, "File2pc /flash/%s\n", emf);
	this->SetRWmode(WAITING_MODE);
	this->SendChkCmd(str);
	this->SaveBinFile(pcf);
	this->SetRWmode();
}

//==========================================
//
//		command history function
//
//===================================
void CComChannel::SaveSetChipRegCmd(int adr, int val, char regWType)
{
	//switch(regWType)
	//{
	//	case 
	//}
	this->m_fb.Printf("%x@%x@%x\n", val, adr, regWType);
}

void CComChannel::SaveSetRegCmd(int adr, int val)
{
	this->m_fb.Printf("%x@%x\n", val, adr);
}

void CComChannel::SaveCmd(char *str)
{
	this->m_fbcmd.Printf("%s", str);
}

void CComChannel::SetRecordPath(char *path)
{
	strcpy(this->m_path, path);
}

void CComChannel::StartRecordRegCmd(char *fn)
{
	char str[0x100];

	sprintf(str, "%s/%s", this->m_path, fn);
	this->m_fb.Open(str, "ab");
}

void CComChannel::StartRecordCmd(char *fn)
{
	char str[0x100];

	sprintf(str, "%s/%s", this->m_path, fn);
	this->m_fbcmd.Open(str, "ab");
}

void CComChannel::StopRecordRegCmd(void)
{
	this->m_fb.Close();
}

void CComChannel::StopRecordCmd(void)
{
	this->m_fbcmd.Close();
}


