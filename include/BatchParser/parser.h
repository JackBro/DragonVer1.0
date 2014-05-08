#ifndef _PARSER_H_
#define _PARSER_H_

#include "parse_tc.h"

class BATCHPARSER_DLL CParser : public parse_tc
{
public:
	CParser();

	void wr_cfg(UINT cfg_addr, UINT cfg_data) ;
	UINT rd_cfg(UINT cfg_addr) ;//?
	void wr_reg(UINT reg_addr, UINT reg_data, UINT reg_bytes) ;
	UINT rd_reg(UINT reg_addr, UINT reg_bytes) ;
	void wr_fifo(UINT reg_addr, UINT buf_len, UINT *buf_addr, UINT reg_bytes) ;
	void rd_fifo(UINT reg_addr, UINT buf_len, UINT *buf_addr, UINT reg_bytes) ;
	void set_str_value(char *str_name, char *str_value) ;
	void delay(UINT dly_time, UINT dly_unit) ;
	void call_cmd(UINT call_type, UINT arg1, UINT arg2, UINT arg3, UINT arg4, UINT arg5) ;

private:
	int	 m_regtype;
};

#endif
