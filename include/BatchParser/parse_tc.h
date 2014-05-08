#ifndef __PARSE_CFG_H
#define __PARSE_CFG_H

#include "tb_def.h"

#define		DEF		0
#define		CFG		1
#define		STR		2
#define		DLY		3
#define		WR		4
#define		RD		5
#define		RDV		6
#define		INP		7
#define		OUP		8
#define		APD		9
#define		EOI		10
#define		EOO		11
#define		CALL	12
#define		SYS		13
#define		UNTIL	14


#define		FILE_BIN_MODE	0
#define		FILE_TXT_MODE	1

#define		MAX_BUF_SIZE	4096

class parse_tc
{
public:
	int init_parse() ;
	int init_parse(char *tc_file);
	int init_parse(char **env) ;
	int init_parse(char *tc_file, char **env) ;
	int init_parse(char *script, char *tc_file, char **env) ;
	void run_testcase() ;

	virtual void wr_cfg(UINT cfg_addr, UINT cfg_data) = 0 ;
	virtual UINT rd_cfg(UINT cfg_addr) = 0 ;
	virtual void wr_reg(UINT reg_addr, UINT reg_data, UINT reg_bytes) = 0 ;
	virtual UINT rd_reg(UINT reg_addr, UINT reg_bytes) = 0 ;
	virtual void set_str_value(char *str_name, char *str_value) = 0 ;
	virtual void delay(UINT dly_time, UINT dly_unit) = 0 ;
	virtual void wr_fifo(UINT reg_addr, UINT buf_len, UINT *buf_addr, UINT reg_bytes) = 0 ;
	virtual void rd_fifo(UINT reg_addr, UINT buf_len, UINT *buf_addr, UINT reg_bytes) = 0 ;
	virtual void call_cmd(UINT call_type, UINT arg1, UINT arg2, UINT arg3, UINT arg4, UINT arg5) = 0 ;
	
	parse_tc() ;
	virtual ~parse_tc() ;

private:
	int get_cmd(int index) ;
	UINT get_input_data(UINT reg_bytes) ;
	void open_input_file() ;
	void close_input_file() ;
	void open_output_file(char *file_name, UINT file_mode) ;
	void append_output_file(char *file_name, UINT file_mode) ;
	void close_output_file() ;

	char *cmd ;
	int cmd_type ;

	FILE *io_file ;
	char *file_name ;
	UINT file_offset ;
	int file_opened ;
	int file_type ;
	int file_mode ;

protected:
	UINT cfg_data ;
	UINT cfg_addr ;
	char *str_name ;
	char *str_value ;

	UINT dly_time ;
	UINT dly_unit ;
	UINT reg_data ;
	UINT reg_mask ;
	UINT reg_addr1 ;
	UINT reg_addr2 ;
	UINT reg_addr_type ;
	UINT reg_bytes ;

	UINT file_size ;
	UINT *pbuf ;
	UINT dbuf[MAX_BUF_SIZE] ;

} ;

#endif
