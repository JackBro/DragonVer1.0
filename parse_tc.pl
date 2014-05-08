#!/usr/bin/perl

$ext = "cfg" ;

$debug = 0 ;
foreach (@ARGV)
{
	$help = 1 if /^-h(elp)?$/i ;
	$debug = 1 if /^-debug$/ ;
	$call_by_c = 1 if /^-call_by_c$/ ;
#	$tc_file = $_ if /tc\S*\.$ext$/ ;
	$tc_file = $_ if /^\S*(\.)?\S*$/ && !/^-/ ;
}

$name = $0 ;
$name =~ s#.*/## ;

if ($help)
{
	print <<msg;

  This tool parse the config file of a testcase
  Created by huoxf At Wed Sep 22 11:09:39 CST 2004
  An question, please mail to huoxiaofang\@vimicro.com
  Version 1.0  All rights reserved.

Usage:
$name -h                 #this help
$name -help              #this help
$name tc_isp1.$ext        #parse tc_isp1.$ext
$name                    #parse tc.cfg

msg
exit;
}

unless ($tc_file)
{ $tc_file = "tc.cfg" if -e "tc.cfg" ; }
$help = 1 unless (@ARGV || $tc_file);

unless ($tc_file)
{
	print <<msg;
Err: must specify config file
Usage:
$name tc_isp1.$ext
msg
	exit ;
}

if ($ENV{OS} =~ /Windows/i)
{
	$unix_env = 0 ;
}
elsif ($ENV{OSTYPE} =~ /linux/i)
{
	$unix_env = 1 ;
}
else
{
	$plat_form = `uname`;
	$unix_env = 1 if $plat_form =~/Linux|SunOS/ ;
}

parse_tc($tc_file);
if ($debug)
{
	print "\n";
	for (@cmd)
	{
		@_ = split /\s+|&/, $_ ;
		for (@_)
		{
			$d = $_ ;
			$d = sprintf "%x", $_ if /^\d+$/ ;
			print "$d ";
		}
		print "\n";
	}
}

unless ($call_by_c)
{
	for (0..$cmd_cnt-1)
	{
		$cmd_index = $_ ;
		get_cmd();
	}
}

sub hello_perl()
{
	$_ = "Welcome to Perl2C interface!\n" ;
	print ;
	if (@_)
	{
		for $i(@_)
		{
			$_ = $i ;
			print "$_\n";
		}
	}
	$_ ;
}

sub is_NULL()
{
	$_ = $_ =~ /^NULL$/ ;
}

sub open_input_file()
{
	--$txt_file{$c_file_name} ;
	$_ = 1 ;
}

sub get_data_size()
{
	$_ = @$c_file_name - $index{$c_file_name} ;
}

sub get_file_offset()
{
	$_ = $index{$c_file_name} ;
	$_ = 0 unless $_ ;
}

sub set_file_offset()
{
	$index{$c_file_name} = $_ ;
}

sub get_input_data()
{
	if ($index{$c_file_name} < @$c_file_name)
	{
		$_ = $$c_file_name[$index{$c_file_name}] ;
		$index{$c_file_name} ++ ;
	}
	else
	{	$_ = 0 ; }
	$_ ;
}

sub close_input_file()
{
	@$c_file_name = () unless $txt_file{$c_file_name} ;
}

sub init()
{
	@all = ();
	@cmd = ();
	$cmd_cnt = 0 ;
	$cmd_index = 0 ;
	$cmd = "";
	%outfile = () ;
	%txt_file = () ;
}

sub parse_tc()
{
	unless (-e $tc_file && -f _)
	{
		print "Cannot find $tc_file\n";
		return 0 ;
	}
	unless (open FI, $tc_file)
	{
		print "Cannot open $tc_file\n";
		return 0 ;
	}
	print "parsing $tc_file ...\n" ;
	init();
	$notes = 0 ;
	while (<FI>)
	{
		deal_notes($_);
		last if /^\s*__END__\s*$/ ;
		push @all, $_ ;
	}
	close FI ;

	def_addr();
	def_cfg_addr();
	def_call_def();
	%outfile = ();
	@cmd_type = qw(
		DEF
		CFG
		STR
		DLY
		WR
		RD
		RDV
		INP
		OUP
		APD
		EOI
		EOO
		CALL
		SYS
		UNTIL
	);
	$i=0;
	for (@cmd_type)
	{ $cmd_type{$_} = $i++ ;}

	@dly_type = qw(
		ERR
		MS
		US
		NS
		CLK
	);
	$i=0;
	for (@dly_type)
	{ $dly_type{$_} = $i++ ;}

	$line = 0 ;
	for (@all)
	{
		$line ++ ;
		chomp ;
		s/\s*\/\/.*//;
		s/\s*;.*//;
#		s/\s*#.*//;
		s/^\s*//;
		deal_cmd() if ($_) ;
	}
	$cmd_cnt = @cmd ;
	$cmd_index = 0 ;
	$cmd_type = 0 ;
	$cmd_d1 = 0 ;
	$cmd_d2 = 0 ;
	$cmd_d3 = 0 ;
	$cmd_d4 = 0 ;
	$cmd_d5 = 0 ;
#	print "find $cmd_cnt command\n" ;
}

sub deal_cmd()
{
	if (/^\s*\[\s*(.*)\s*\]/)
	{
		$_ = $1 ;
		if (/(\w+)\s*\@\s*(\w+)/)
		{
			($cfg_d, $_) = ($1, $2) ;
			$cfg_d =~ s/^0x//i;
			$cfg_d =~ hex $cfg_d;
			get_cfg_addr() ;
			push @cmd, "CFG $_ $cfg_d" unless $_ eq "NULL" ;
		}
		else
		{
			s/\(|\)|,/ /;
			s/^\s*//;
			s/\s*$//;
			@call = split /\s+/, $_ ;
			for (@call)
			{
				get_call_def() ;
				$call[0] = "NULL" if $_ eq "NULL" ;
			}
			push @cmd, "CALL @call" unless $call[0] eq "NULL" ;
			do_reset() if ($call[0] eq $call_def{"RESET"}) ;
			$dump_on_flag = 1 if ($call[0] eq $call_def{"DUMP_ON"}) ;
		}
	}
	elsif (/^\s*'\s*(.*)'\s*\s*/)
	{
		$sys_cmd = $1 ;
		if ($sys_cmd =~ /^\s*\!/)
		{
 			unless ($call_by_c)
			{
				$sys_cmd =~ s/^\s*\!\s*//;
				if ($sys_cmd =~ /cp\s+(\S+)\.cfa/)
				{
					$cfa_src = $1 ;
					$cfa_src =~ s/.*\///;
					$cfa_file = "sensor.cfa" ;
					if (-e "../../dat/$cfa_src.cfa" && -f _)
					{
						system "unlink $cfa_file" if -l $cfa_file ;
						system "ln -s ../../dat/$cfa_src.cfa $cfa_file";
					}
					elsif (-e "../../tc/$cfa_src.cfa" && -f _)
					{
						system "unlink $cfa_file" if -l $cfa_file ;
						system "ln -s ../../tc/$cfa_src.cfa $cfa_file";
					}
				}
			}
		}
		else
		{
			$sys_cmd =~ s/^\s*\!//;
			push @cmd, "SYS $sys_cmd";
		}
	}
	elsif (/^\s*(\w+)\s*=\s*(\S*)/)
	{
		($str_name, $str_value) = ($1, $2) ;
		$str_value = "NULL" unless $str_value ;
		push @cmd, "STR $str_name $str_value" ;
	}
	elsif (/^\s*~\s*(\w+)\s*$/)
	{
		$dly = $1 ;
		s/\s*~.*//;
		deal_cmd() if $_ ;
		($dly, $unit) = ($1,$2) if $dly =~ /^\s*(\d+)(\w+)?/i ;
		unless ($unit)
		{
			if ($unix_env)
			{$unit = "NS" ;}
			else
			{$unit = "MS" ;}
		}
		$unit =~ tr/a-z/A-Z/;
		$unit = "ERR" unless $unit =~ /MS|US|NS|CLK/;
		if ($dly =~ /^0x/i)
		{
			$dly =~ s/^0x//i ;
			$dly = hex $dly ;
		}
		push @cmd, "DLY $dly $unit" if $dly ;
	}
	elsif (/>>\s*(\S+)/)
	{
		$file = $1 ;
		if (exists $outfile{$file})
		{
			push @cmd, "APD $file" ;
		}
		else
		{
			$outfile{$file} = $file ;
			push @cmd, "OUP $file" ;
		}
		s/\s*>>.*//;
#		s/.*\@/0&ffffffff\@/;
		$cmd_type = "RD" ;
		$file_opened = 1 ;
		post_deal();
		push @cmd, "EOO" ;
		$file_opened = 0 ;
	}
	elsif (/<<\s*(.+)\s*$/)
	{
		$file = $1 ;
		$offset = "";
		if ($file =~ /\@/)
		{
			($file, $offset) = @_ = split /\s*\@\s*/, $file ;
			if (@_ == 1)
			{
				$offset = "ffffffff" ;
			}
			elsif ($offset eq "0")
			{
				$offset = "0x0" ;
			}
		}
		$offset = "ffffffff" unless $offset ;
		$offset =~ s/^0x//i;
		$offset = hex $offset ;
		if (! -e $file)
		{
			$tem_ = $file ;
			$tem_ =~ s/.*\///;
			$temp = "../../dat/$tem_" ;
			if (-e $temp && -r _)
			{ $file = $temp ; }
			else
			{
				$temp = "../../tc/$tem_" ;
				if (-e $temp && -r _)
				{ $file = $temp ; }
				else
				{print "input file \"$file\" not exist\n" ;}
			}
		}
		if (! -r $file)
		{
			print "input file \"$file\" not readable\n" ;
		}
		if (-e $file && -r _)
		{
			$txt_format = 0 ;
			$cnt = 0 ;
			if (-T $file)
			{
				$txt_format = 1 ;
				open FT, $file || die "Cannot open $file\n" ;
				$txt_file{$file} += 1 ;
				$cnt = @$file ;
				$__ = $_ ;
				if ($txt_file{$file} ==1)
				{
					@$file = () ;
					while (<FT>)
					{
						chomp ;
						s/^0x//i ;
						next if /^\s*$/ ;
						if (/^([0-9,a-f]+)$/i)
						{
							$_ = hex $_ ;
							push @$file, $_ ;
						}
						else
						{
							print "Err AT line $line: line $. of \"$file\": \"$_\" is not hex data\n";
						}
					}
					$index{$file} = 0 ;
					if ($debug)
					{
						$cnt = @$file ;
						print "read $cnt data from \"$file\"\n";
					}
				}
				close FT ;
				$_ = $__ ;
			}
			push @cmd, "INP $file $offset $txt_format" ;
			$file_index = @cmd - 1 ;

			s/\s*<<.*//;
#			s/.*\@/0&ffffffff\@/;
			$cmd_type = "WR" ;
			$file_opened = 1 ;
			$search_en = 1 ;
			post_deal();
			$cmd[$file_index] .= " $bytes" if $file_index && $bytes ;
			$search_en = 0 ;
			push @cmd, "EOI" ;
			$file_opened = 0 ;
		}
	}
	elsif (/^\s*~\s*(.*\@.*)\s*/)
	{
		$save_dly = $1 ;
		s/\s*~.*// ;
		s/^\s*// ;
		deal_cmd() if $_ ;
		$_ = $save_dly ;
		s/\s*V\s*//;
		s/\s*\-.*//;
		s/\s*\*.*// if /\*\s*$/ ;
		$_ .= "*fffff" unless /\*\w+/ ;
		$cmd_type = "UNTIL" ;
		post_deal();
	}
	elsif (/^\s*(.*)\s*\@/ || /^\s*(.*)\s*\$/)
	{
		$d = $1 ;
		if (/^V/i)
		{$cmd_type = "RDV" ;}
		elsif ($d)
		{$cmd_type = "WR" ;}
		else
		{$cmd_type = "RD" ;}
		post_deal();
	}
	elsif ($_)
	{
		print "Err AT line $line: Unknow command \"$_\"\n" ;
#		push @cmd, $_ ;
	}
}

sub do_reset()
{
	unless ($rst_flag)
	{
		$rst_save = pop @cmd ;
		$rst_flag = 1 ;
		$mul16 = "" ;
		$dly_time = "~70us" ;
		$dump_on = "" ;
		if (open FDEF, "define.v")
		{
			@DEF = <FDEF> ;
			close FDEF ;
			chomp @DEF ;
			$DEF = join " ", @DEF ;
			$dly_time = "~2us" if $DEF =~ /define\s*FPGA/;
			$mul16 = "01\@BIU_SEL_8_16_ADDR" if $DEF =~ /define\s*MUL16/ ;
			$dump_on = "[dump_on u_top]" if !$dump_on_flag && $DEF =~ /define\s*DEB|WAVE|VPD|VCD/ ;
		}
		if ($mul16)
		{
			$_ = "[0\@conf]" ;
			deal_cmd() ;
			$_ = "~10ns" ;
			deal_cmd() ;
			($cmd[@cmd-3], $cmd[@cmd-2]) = ($cmd[@cmd-2], $cmd[@cmd-3]) ;
		}
		$_ = $dly_time ;
		deal_cmd() ;
		if ($mul16)
		{
			$_ = $mul16 ;
			deal_cmd() ;
		}
		if ($dump_on)
		{
			$_ = $dump_on ;
			deal_cmd() ;
		}
#		push @cmd, $rst_save if $rst_save ;
	}
}

sub post_deal()
{
	unless ($rst_flag)
	{
		$last_cmd = "" ;
		$last_cmd = pop @cmd if ($cmd[@cmd-1] =~ /INP|OUP/) ;
		$___ = $_ ;
		$_ = "[reset]" ;
		deal_cmd();
		$file_index = "" ;
		if ($last_cmd)
		{
			push @cmd, $last_cmd ;
			$file_index = @cmd - 1 ;
		}
		$_ = $___ ;
	}
	if (/\s*(.*)\s*\@\s*(.*)\s*/)
	{
		($dat_, $add_) = ($1, $2) ;
		$bytes = 1 ;
	}
	elsif (/\s*(.*)\s*\$\s*(.*)\s*/)
	{
		($dat_, $add_) = ($1, $2) ;
		$bytes = 2 ;
	}
	elsif (/\s*(.*)\s*\%\s*(.*)\s*/)
	{
		($dat_, $add_) = ($1, $2) ;
		$bytes = 4 ;
	}
	$dat_ =~ s/\s+//g;
	$dat_ = 0 unless $dat_ ;
	$dat_ .= "&ffffffff" ;
	if ($dat_ =~ /^V/i)
	{
		$cmd_type = "RDV" ;
		$dat_ =~ s/^V//i;
	}

	($d, $m) = split /&/, $dat_ ;
	$d = get_hex($d) ;
	$m = "ffffffff" unless $m ;
	$m = get_hex($m) ;
	$dat_ = "$d&$m" ;

	$add_ =~ s/\s*-\s*/-/g;
	$add_ =~ s/\s*\*\s*/\*/g;
	$add_ =~ s/\s*,\s*/,/g;
	@add_ = split /,|\s+/, $add_ ;

	foreach (@add_)
	{
		s/\s+//g;
		if (/-/)
		{
			@_ = split /-/, $_ ;
			for (@_)
			{
				s/^\s*$//;
				get_addr() if ($_) ;
			}
			$_[1] = $_[0] if ($_[1] < $_[0]) ;
			push @cmd , "$cmd_type $dat_ $_[0] $_[1] 1 $bytes" if $_[0] ne "NULL" && $_[1] ne "NULL";
		}
		elsif (/\*/)
		{
			@_ = split /\*/, $_ ;
			for (@_)
			{
				s/^\s*$//;
				if ($_)
				{
					if($search_en && $_ eq "?")
					{ $_ = hex "ffffffff" ; }
					else
					{ get_addr(); }
					$_[0] = "NULL" if $_ eq "NULL" ;
				}
			}
			push @cmd , "$cmd_type $dat_ $_[0] $_[1] 0 $bytes" if $_[0] ne "NULL" && $_[1] ;
		}
		else
		{
			get_addr() unless /^\s*$/ ;
			push @cmd , "$cmd_type $dat_ $_ $_ 1 $bytes" ;
		}
	}
}

sub get_hex()
{
	my $d = shift ;
	$d =~ s/^0x//i;
	$d =~ s/[^0-9,a-f,A-F]//g;
	$d = 0 unless $d ;
	$d = hex $d ;
	$d ;
}

sub get_addr()
{
	s/\s+//g;
	my $addr = $_ ;
	s/_ADDR$// ;
	if (exists $addr{$_})
	{
		$_ = $addr{$_} ;
	}
	s/^0x//i;
	if (/^([0-9,a-f]+)$/i)
	{
		$_ = hex $_ ;
	}
	else
	{
		$_ = "NULL" ;
		print "Err AT line $line: reg_addr \"$addr\" does exist\n";
	}
	$_ ;
}

sub get_cfg_addr()
{
	s/\s+//g;
	my $addr = $_ ;
	tr/a-z/A-Z/ ;
	if (exists $cfg_addr{$_})
	{
		$_ = $cfg_addr{$_} ;
	}
	s/^0x//i;
	if (/^([0-9,a-f]+)$/i)
	{
		$_ = hex $_ ;
	}
	else
	{
		$_ = "NULL" ;
		print "Err AT line $line: cfg_addr \"$addr\" does exist\n";
	}
	$_ ;
}

sub get_call_def()
{
	s/\s+//g;
	my $addr = $_ ;
	tr/a-z/A-Z/;
	if (exists $call_def{$_})
	{
		$_ = $call_def{$_} ;
	}
	elsif (exists $call_def{"DEF_$_"})
	{
		$_ = $call_def{"DEF_$_"} ;
	}
	s/^0x//i;
	if (/^([0-9,a-f]+)$/i)
	{
		$_ = hex $_ ;
	}
	else
	{
		$_ = "NULL" ;
		print "Err AT line $line: call_def \"$addr\" does exist\n";
	}
	$_ ;
}

sub get_cmd()
{
	$_ = $cmd = $cmd[$cmd_index] ;
#	print "CMD = $_\n" ;
	s/^\s*//;
	s/\s*$//;
	if (/^SYS\s*(.*)/)
	{
		$cmd_type = "SYS" ;
		$cmd_type = $cmd_type{$cmd_type} ;
		$cmd_d1 = $1 ;
		system $1 ;
		return ;
	}
	($cmd_type, $cmd_d1, $cmd_d2, $cmd_d3, $cmd_d4, $cmd_d5, $cmd_d6) =
		 split /\s+/, $_ ;
	if ($cmd_d1 =~ /&/)
	{
		if ($cmd_type =~ /WR|RD|RDV/)
		{
			($d, $m) = split /&/, $cmd_d1 ;
			($cmd_d1, $cmd_d2, $cmd_d3, $cmd_d4, $cmd_d5, $cmd_d6) = 
			($d,	  $m,	   $cmd_d2, $cmd_d3, $cmd_d4, $cmd_d5) ;
		}
		else
		{
			$cmd_d1 =~ s/&.*//;
		}
	}
	$cmd_d2 = $dly_type{$cmd_d2} if $cmd_type eq "DLY" ;
	if ($cmd_type =~ /INP|OUP|APD/)
	{
		$c_file_name = $cmd_d1 ;
		$c_file_offset = $cmd_d2 ;
	}
	$cmd_d2 = 0 unless $cmd_d2 ;
	$cmd_d3 = 0 unless $cmd_d3 ;
	$cmd_d4 = 0 unless $cmd_d4 ;
	$cmd_d5 = 0 unless $cmd_d5 ;
	$cmd_d6 = 0 unless $cmd_d6 ;
#	print "\t$cmd_type, $cmd_d1, $cmd_d2, $cmd_d3, $cmd_d4, $cmd_d5, $cmd_d6\n" ;
	$cmd_type = $cmd_type{$cmd_type} ;
}

sub deal_notes()
{
	return $_ if m#^\s*//# ;
	s#/\*.*\*/##;
	if ( $notes )
	{
		if ( /\*\// )
		{
#			print "  $tc_file: Multi-line comment End   at line $.\n";
			$notes = 0 ;
			s/^.*\*\/// ;
		}
		elsif ( /\/\*/ )
		{
			print "  $tc_file: Multi-line comment Error at line $.\n";
			$_ = "" ;
		}
		else { $_ = "" ; }
	}
	else
	{
		if ( /\/\*/ )
		{
#			print "  $tc_file: Multi-line comment Begin at line $.\n";
		$notes = 1 ;
		s/\/\*.*$//;
		}
		elsif ( /\*\// )
		{
			print "  $tc_file: Multi-line comment Error at line $.\n";
			$_ = "" ;
		}
	}
	$_ ;
}

sub def_call_def()
{
	$_ = sprintf <<msg;
reset
stop
finish
dump_on
	u_top
	u_biu
	u_cpm
	u_sif
	u_isp
	u_ipp
	u_lcdc
	u_lcdif
	u_marb
	u_lbuf
	u_jpeg
	u_ge
	u_sramtop_6t
	u_sramtop_1t
	u_pad_ctl
	u_test_ctl
	u_umonitor
dump_off
chip
	vc0568
	vc0558
	vc0938
device
	biu
	usb
	lcd
	sensor
	sdcard
set_clk
msg

	@_ = split /\n/, $_ ;
	$sub = 0 ;
	for (@_)
	{
		tr/a-z/A-Z/ ;
		$def = $_ ;
		if (/^\w+$/)
		{
			$sub ++ ;
			$para = 0 ;
			$i = $sub ;
			$def_type = "/*cmd type*/\n" ;
			$sub_type = "/*$def type*/\n" ;
#			print "\n" ;
		}
		elsif (/^\s+\w+$/)
		{
			s/^\s+//;
			$para++ ;
			$i = $para ;
			$def_type = $sub_type ;
		}
		$ii = sprintf "%x", $i ;
		$call_def{$_} = $ii ;
#		print "#define		DEF_${_}			$i	$def_type";
#		print "`define		DEF_${_}			$i\n";
		if (0)
		{
			print <<msg;
		case DEF_${_} :
			
			break ;

msg
		}
	}
}

sub def_cfg_addr()
{
	%cfg_addr = qw(
		BANKCON1	01C80008
		BANKCON1_ADDR	01C80008
		BANKCON2	01C8000C
		BANKCON2_ADDR	01C8000C
		BANKCON3	01C80010
		BANKCON3_ADDR	01C80010
		BANKCON4	01C80014
		BANKCON4_ADDR	01C80014
		BANKCON5	01C80018
		BANKCON5_ADDR	01C80018
		EXTINT	01d20050
		EXTINT_ADDR	01d20050
		EXTINPND	01d20054
		EXTINPND_ADDR	01d20054
		INTCON	01E00000
		INTCON_ADDR	01E00000
		INTPND	01e00004
		INTPND_ADDR	01e00004
		INTMOD	01e00008
		INTMOD_ADDR	01e00008
		INTMSK	01e0000c
		INTMSK_ADDR	01e0000c
		I_ISPR	01e00020
		I_ISPR_ADDR	01e00020
		I_ISPC	01e00024
		I_ISPC_ADDR	01e00024
		F_ISPR	01e00038
		F_ISPR_ADDR	01e00038
		F_ISPC	01e0003c
		F_ISPC_ADDR	01e0003c
		I_INUM	01e00100
		I_INUM_ADDR	01e00100
		I_IEDG	01e00104
		I_IEDG_ADDR	01e00104
		I_IAST	01e00108
		I_IAST_ADDR	01e00108
		I_PSLV	01e00010
		I_PSLV_ADDR	01e00010
		I_PMST	01e00014
		I_PMST_ADDR	01e00014
		I_CSLV	01e00018
		I_CSLV_ADDR	01e00018
		I_CMST	01e0001c
		I_CMST_ADDR	01e0001c
	);

	@_ = qw(
		debug
		BO
		TEST
		CONF
		GPIO
		Tas
		Tah
		Tcs_we
		Twe_cs
		Twds
		Twdh
		Tww
		Tcs_rd
		Trd_cs
		Tacc
		Trw
		Trdh
	);

	$i = 1 ;
	for (@_)
	{
		$_ ;
#		print "#define		${_}_ADDR			$i\n";
		if (0)
		{
			print <<msg;
		case ${_}_ADDR :
			$_ = data ;
			break ;

msg
		}
		tr/a-z/A-Z/ ;
		$ii = sprintf "%x", $i ;
		$cfg_addr{$_} = $ii ;
		$i ++ ;
	}
}

sub def_addr()
{
	my $chip = shift ;
	$chip = "vc0568" unless $chip ;
	$chip = "vc0$chip" unless $chip =~ /^vc0/i ;
	$chip =~ tr/A-Z/a-z/;
	if ($unix_env)
	{
		$addr_file = "../bfm/reg_def_$chip.h" ;
	}
	else
	{
		$addr_file = "reg_def_$chip.h" ;
	}
	$addr_file = "../$addr_file" unless -e $addr_file ;
	open FADDR, "$addr_file" || die "Cannot open \"$addr_file\"\n";
	%addr = ();
	while (<FADDR>)
	{
		chomp ;
		s#//.*## ;
		next if /^\s*$/ ;
		if (/^\s*#define\s+(\w+)\s+(\w+)/)
		{
			$key = $1 ;
			$val = $2 ;
			$key =~ s/_ADDR$//i ;
			$key =~ tr/a-z/A-Z/ ;
			$val =~ s/^0x//i;
			$addr{$key} = $val ;
		}
	}
}
