#include "parse_tc.h"
#include <EXTERN.h>
#include <perl.h>

static PerlInterpreter *my_perl ;
SV *sv__ ;
SV *sv_cmd ;
SV *sv_cmd_cnt ;
SV *sv_cmd_index ;
SV *sv_cmd_type ;
SV *sv_cmd_d1 ;
SV *sv_cmd_d2 ;
SV *sv_cmd_d3 ;
SV *sv_cmd_d4 ;
SV *sv_cmd_d5 ;
SV *sv_cmd_d6 ;

char *DLY_UNIT[] = { "ERR", "MS", "US", "NS", "CLK", NULL } ;
char *argv_null[] = { NULL } ;

int parse_tc::init_parse(char *tc_file)
{
	return init_parse("parse_tc.pl", tc_file, argv_null);
}

int parse_tc::init_parse()
{
	return init_parse("parse_tc.pl", "tc.cfg", argv_null) ;
}
	
int parse_tc::init_parse(char** env)
{
	return init_parse("parse_tc.pl", "tc.cfg", env) ;
}
	
int parse_tc::init_parse(char* tc_file, char** env)
{
	return init_parse("parse_tc.pl", tc_file, env) ;
}
	
int parse_tc::init_parse(char *script, char *tc_file, char **env)
{
	char *argv[] = {"parse_tc", script, 
			tc_file, "-call_by_c", NULL} ;

	file_opened = 0 ;
	if (my_perl == NULL)
	{
		my_perl = perl_alloc() ;
		perl_construct(my_perl) ;
		printf("Create Perl Interpreter ...\n");
	}

	perl_parse(my_perl, NULL, 4, argv, env) ;
	call_argv("hello_perl", G_DISCARD | G_NOARGS, argv_null) ;
	perl_run(my_perl) ;

	sv__ = get_sv("_", FALSE) ;
	sv_cmd = get_sv("cmd", FALSE) ;
	sv_cmd_cnt = get_sv("cmd_cnt", FALSE) ;
	sv_cmd_index = get_sv("cmd_index", FALSE) ;
	sv_cmd_type = get_sv("cmd_type", FALSE) ;
	sv_cmd_d1 = get_sv("cmd_d1", FALSE) ;
	sv_cmd_d2 = get_sv("cmd_d2", FALSE) ;
	sv_cmd_d3 = get_sv("cmd_d3", FALSE) ;
	sv_cmd_d4 = get_sv("cmd_d4", FALSE) ;
	sv_cmd_d5 = get_sv("cmd_d5", FALSE) ;
	sv_cmd_d6 = get_sv("cmd_d6", FALSE) ;
	return SvIV(sv_cmd_cnt) ;
}

void parse_tc::run_testcase()
{
	int i ;
	int cmd_cnt = SvIV(sv_cmd_cnt) ;

	for (i=0; i<cmd_cnt; i++)
	{
		get_cmd(i) ;
	}
}

int parse_tc::get_cmd(int index)
{
	STRLEN strlen ;
	UINT addr ;
	UINT wr_data ;
	UINT rd_data ;

	sv_setiv(sv_cmd_index, index) ;
	call_argv("get_cmd", G_DISCARD | G_NOARGS, argv_null) ;
	cmd = SvPV(sv_cmd, strlen) ;
	cmd_type = SvIV(sv_cmd_type) ;
	if (cmd_type == CFG)
	{
		cfg_data = SvIV(sv_cmd_d1) ;
		cfg_addr = SvIV(sv_cmd_d2) ;
		wr_cfg(cfg_addr, cfg_data) ;
	}
	else if (cmd_type == STR)
	{
		str_name = SvPV(sv_cmd_d1, strlen) ;
		str_value = SvPV(sv_cmd_d2, strlen) ;
		set_str_value(str_name, str_value) ;
	}
	else if (cmd_type == DLY)
	{
		dly_time = SvIV(sv_cmd_d1) ;
		dly_unit = SvIV(sv_cmd_d2) ;
		delay(dly_time, dly_unit) ;
	}
	else if (cmd_type == SYS)
	{
//		printf("%s\n", SvPV(sv_cmd_d1, strlen)) ;
	}
	else if (cmd_type == WR || cmd_type == RD || cmd_type == RDV || cmd_type==CALL || cmd_type==UNTIL)
	{
		reg_data = SvIV(sv_cmd_d1) ;
		reg_mask = SvIV(sv_cmd_d2) ;
		reg_addr1= SvIV(sv_cmd_d3) ;
		reg_addr2= SvIV(sv_cmd_d4) ;
		reg_addr_type = SvIV(sv_cmd_d5) ;
		reg_bytes = SvIV(sv_cmd_d6) ;

		if (cmd_type==CALL)
		{
			call_cmd(reg_data, reg_mask, reg_addr1, reg_addr2, reg_addr_type, reg_bytes);
		}
		else if (cmd_type==UNTIL)
		{
			UINT loop ;
			UINT ok = 0 ;
			for (loop=0;loop<reg_addr2;loop++)
			{
				rd_data = rd_reg(reg_addr1, reg_bytes) ;
				if (reg_data == (rd_data & reg_mask))
				{
					ok = 1 ;
					break ;
				}
				if (ok==0)
					printf("Err: UNTIL ~%x&%x@%x != %x\n", rd_data, reg_mask, reg_addr1, reg_data);
			}
			
		}
		else if (reg_addr_type)	//inc addr
		{
			for (addr=reg_addr1; addr<=reg_addr2; addr+=reg_bytes)
			{
				if (cmd_type == WR)
				{
					wr_data = (file_opened ? get_input_data(reg_bytes) : reg_data) & reg_mask ;
					wr_reg(addr, wr_data, reg_bytes) ;
				}
				else
				{
					rd_data = rd_reg(addr, reg_bytes) ;
					if (cmd_type == RDV && (rd_data & reg_mask) != reg_data)
					{
						printf("Err: V%x&%x@%x != %x\n", rd_data, reg_mask, addr, reg_data);
					}
					if (file_opened)
					{
						fprintf(io_file, "%x&%x@%x = %x\n", rd_data, reg_mask, addr, rd_data & reg_mask);
					}
				}
			}
		}
		else
		{
			if (cmd_type == WR && reg_addr2 == 0xffffffff)
			{
				if (file_mode == FILE_TXT_MODE)
				{
					call_argv("get_data_size", G_DISCARD | G_NOARGS, argv_null) ;
					UINT data_size = SvIV(sv__) ;
					reg_addr2 = data_size ;
				}
				else
				{
					UINT cur_pos ;
#ifdef	LINUX
					fpos_t pos ;
					fgetpos(io_file, &pos) ;
					cur_pos = pos.__pos ;
#else
					__int64 pos ;
					fgetpos(io_file, &pos) ;
					cur_pos = (UINT)pos ;
#endif
					if (file_size > cur_pos)
						reg_addr2 = (file_size - cur_pos) / reg_bytes ;
					else
						reg_addr2 = 0 ;
				}
			}
			if (reg_addr2>MAX_BUF_SIZE)
				pbuf = (UINT *)malloc(reg_addr2 * sizeof(UINT)) ;
			else
				pbuf = dbuf ;

			if (cmd_type == WR)
			{
				for (addr=0; addr<reg_addr2; addr++)
				{
					pbuf[addr] = (file_opened ? get_input_data(reg_bytes) : reg_data) & reg_mask ;
				}
				wr_fifo(reg_addr1, reg_addr2, pbuf, reg_bytes) ;
			}
			else
			{
				rd_fifo(reg_addr1, reg_addr2, pbuf, reg_bytes) ;
				for (addr=0; addr<reg_addr2; addr++)
				{
					rd_data = pbuf[addr] ;
					if (cmd_type == RDV && (rd_data & reg_mask) != reg_data)
					{
						printf("Err: V%x&%x@%x != %x\n", rd_data, reg_mask, reg_addr1, reg_data);
					}
					if (file_opened)
					{
						fprintf(io_file, "%x&%x@%x = %x\n", rd_data, reg_mask, reg_addr1, rd_data & reg_mask);
					}
				}
			}

			if (reg_addr2>MAX_BUF_SIZE)
				free(pbuf) ;
		}
	}
	else if (cmd_type == INP)
	{
		file_name = SvPV(sv_cmd_d1, strlen) ;
		file_offset = SvIV(sv_cmd_d2) ;
		file_mode = SvIV(sv_cmd_d3) ;
		reg_bytes = SvIV(sv_cmd_d4) ;
		open_input_file();
	}
	else if (cmd_type == EOI)
	{
		close_input_file();
	}
	else if (cmd_type == OUP)
	{
		file_name = SvPV(sv_cmd_d1, strlen) ;
		file_mode = SvIV(sv_cmd_d2) ;
		open_output_file(file_name, file_mode);
	}
	else if (cmd_type == APD)
	{
		file_name = SvPV(sv_cmd_d1, strlen) ;
		file_mode = SvIV(sv_cmd_d2) ;
		append_output_file(file_name, file_mode);
	}
	else if (cmd_type == EOO)
	{
		close_output_file();
	}
	else
	{
		printf("Unknown cmd: %s\n", cmd) ;
	}
	return 1 ;
}

UINT parse_tc::get_input_data(UINT reg_bytes)
{
	if (file_mode == FILE_TXT_MODE)
	{
		call_argv("get_input_data", G_DISCARD | G_NOARGS, argv_null) ;
		return SvIV(sv__) ;
	}
	else
	{
		UINT i ;
		int temp = 0 ;
		UINT data = 0 ;
		for (i=0; i<reg_bytes; i++)
		{
			if ((temp = getc(io_file)) == EOF)
				return 0 ;
			else
				data += temp << (i*8) ;
		}
		return data ;
	}
}

void parse_tc::open_input_file()
{
	if (file_mode == FILE_TXT_MODE)
	{
		call_argv("open_input_file", G_DISCARD | G_NOARGS, argv_null) ;
		file_opened = SvIV(sv__) ;
		if (file_offset != 0xffffffff)
		{
			sv_setiv(sv__, file_offset) ;
			call_argv("set_file_offset", G_DISCARD | G_NOARGS, argv_null) ;
		}
	}
	else
	{
		io_file = fopen(file_name, "rb");
		fseek(io_file,0,SEEK_END) ;
#ifdef	LINUX
		fpos_t pos ;
		fgetpos(io_file, &pos) ;
		file_size = pos.__pos ;
#else
		__int64 pos ;
		fgetpos(io_file, &pos) ;
		file_size = (UINT)pos ;
#endif
		if (file_offset == 0xffffffff)
		{
			call_argv("get_file_offset", G_DISCARD | G_NOARGS, argv_null) ;
			fseek(io_file, SvIV(sv__), SEEK_SET) ;
		}
		else
			fseek(io_file, file_offset*reg_bytes, SEEK_SET) ;
		file_opened = (io_file != NULL) ;
	}
	if (!file_opened)
	{
		printf ("Fail to open input file: %s\n", file_name);
	}
}

void parse_tc::close_input_file()
{
	if (file_mode == FILE_TXT_MODE)
		call_argv("close_input_file", G_DISCARD | G_NOARGS, argv_null) ;
	else if (file_opened)
	{
#ifdef	LINUX
		fpos_t pos ;
		fgetpos(io_file, &pos) ;
		sv_setiv(sv__, pos.__pos) ;
#else
		__int64 pos ;
		fgetpos(io_file, &pos) ;
		sv_setiv(sv__, (long)pos) ;
#endif
		call_argv("set_file_offset", G_DISCARD | G_NOARGS, argv_null) ;
		fclose(io_file) ;
	}
	file_opened = 0 ;
}

void parse_tc::open_output_file(char *file_name, UINT file_mode)
{
	if (file_mode == FILE_TXT_MODE)
	{
		io_file = fopen(file_name, "wt");
	}
	else
	{
		sv_setiv(sv__, file_offset) ;
		io_file = fopen(file_name, "wb");
	}
	file_opened = (io_file != NULL) ;
}

void parse_tc::append_output_file(char *file_name, UINT file_mode)
{
	if (file_mode == FILE_TXT_MODE)
	{
		io_file = fopen(file_name, "awt");
	}
	else
	{
		io_file = fopen(file_name, "awb");
	}
	file_opened = (io_file != NULL) ;
}

void parse_tc::close_output_file()
{
	fclose(io_file) ;
	file_opened = 0 ;
}

parse_tc::parse_tc()
{
	file_opened = 0 ;
}

parse_tc::~parse_tc()
{
	if (my_perl)
	{
		printf("Destroy Perl Interpreter\n") ;
		perl_destruct(my_perl) ;
		perl_free(my_perl) ;
	}
}
