#include "Parser.h"
#include "../basicfunc/simhost.h"


CParser::CParser()
{
	this->m_regtype = DEF_BIU;
}

void CParser::call_cmd(UINT call_type, UINT arg1, UINT arg2, UINT arg3, UINT arg4, UINT arg5)
{
	if(call_type == DEF_DEVICE)
		this->m_regtype = arg1;
	arg2 = arg3 = arg4 = arg5;
}

void CParser::set_str_value(char *str_name, char *str_value)
{
	str_name = str_value;
//	printf("TB ::cmd = STR %s = %s\n", str_name, str_value) ;
}

void CParser::delay(UINT dly_time, UINT dly_unit)
{
	dly_time = dly_unit;
//	printf("TB ::cmd = DLY %x %s\n", dly_time, DLY_UNIT[dly_unit]) ;
}

void CParser::wr_cfg(UINT cfg_addr, UINT cfg_data)
{
	cfg_addr = cfg_data;
	//printf ("TB ::cmd = CFG %x %x\n",cfg_addr, cfg_data);
	//cpu::wr_cfg(cfg_addr, cfg_data);
	//lcd::wr_cfg(cfg_addr, cfg_data);
}

UINT CParser::rd_cfg(UINT cfg_addr)
{
	cfg_addr = 0;
	return 0;
}

void CParser::wr_reg(UINT reg_addr, UINT reg_data, UINT reg_bytes)
{
	switch(this->m_regtype)
	{
	case DEF_BIU:
		SimHost_SetReg(reg_addr, (unsigned char)reg_data, 0);
		break;
	case DEF_USB:
	case DEF_LCD:
	case DEF_SENSOR:
	case DEF_SDCARD:
	default:
		break;
	}
	reg_bytes = 0;
}

UINT CParser::rd_reg(UINT reg_addr, UINT reg_bytes)
{
	UINT val = 0;

	switch(this->m_regtype)
	{
	case DEF_BIU:
		val = SimHost_GetReg(reg_addr, 0);
		break;
	case DEF_USB:
	case DEF_LCD:
	case DEF_SENSOR:
	case DEF_SDCARD:
	default:
		break;
	}
	reg_bytes = 0;
	return val;
}

void CParser::wr_fifo(UINT reg_addr, UINT buf_len, UINT *pbuf, UINT reg_bytes)
{
	UINT i;

	switch(this->m_regtype)
	{
	case DEF_BIU:
		for(i = 0; i < buf_len; i++)
			SimHost_SetReg(reg_addr + i, (unsigned char)pbuf[i], 0);
		break;
	case DEF_USB:
	case DEF_LCD:
	case DEF_SENSOR:
	case DEF_SDCARD:
	default:
		break;
	}
	reg_bytes = 0;
}

void CParser::rd_fifo(UINT reg_addr, UINT buf_len, UINT *pbuf, UINT reg_bytes)
{
	UINT i;

	switch(this->m_regtype)
	{
	case DEF_BIU:
		for(i = 0; i < buf_len; i++)
			pbuf[i] = SimHost_GetReg(reg_addr + i, 0);
		break;
	case DEF_USB:
	case DEF_LCD:
	case DEF_SENSOR:
	case DEF_SDCARD:
	default:
		break;
	}
	reg_bytes = 0;
}

