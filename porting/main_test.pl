sub ReplaceOrAddString
{
	my $instr=$_[0];
	my $outstr=$_[1];
	my $keystr=$_[2];
	my $OUTFILE=$_[3];
	if($instr=~/$keystr/)
		 {
			print $OUTFILE $outstr."\n";
		 	
				 	
		 }
	 else
		 {
		 	print $OUTFILE $outstr."\n";
		 	print $OUTFILE $instr."\n";
		 	
		}
}
sub modifyproj
{
	my $curdir=$_[0];
	my $IsDebug;
	while(defined($nextname = <$curdir/*.vcproj>)) 
	{
		my @aa=split(/[\\|\/]/,$nextname);
		my $num=@aa;
		my $myprojname;
		      	if ($num >1)
		      	{
		      		$myprojname=$aa[$num-1];
		      		
		      	}
		      	else
		      	{
		      		die "Error in modifyproj  \n";
		      	}
		chomp($myprojname);
		$myprojname=~s/(.*)[.]vcproj/$1/;     	
		open(IN, "<$nextname");
		open(OUT, ">$nextname".".bak");
		my $cnt = 0;
		while(<IN>) 
		{
			$cnt++;
			if(/Name\=\"Debug\|Win32\"/)
			{
				$IsDebug=1;
				print OUT $_;
				$_=<IN>;
				ReplaceOrAddString($_,"\t\t\tOutputDirectory=\"\$(SolutionDir)libdebug\"","OutputDirectory",OUT);
				#print OUT $_;
			}
			elsif (/Name\=\"Release\|Win32\"/)
			{
				$IsDebug=0;
				print OUT $_;
				 $_=<IN>;
				ReplaceOrAddString($_,"\t\t\tOutputDirectory=\"\$(SolutionDir)librelease\"","OutputDirectory",OUT);	 
				#print OUT $_;
				
			}
			elsif (/Name\=\"([a-zA-Z0-9]*)\|Win32\"/)
			{
				if (($1 ne "Debug") && ($1 ne "Release"))
				{
					print OUT "no use";
					while(<IN>)
					{
						if (/<\/Configuration>/)
						{
							print OUT $_;
							last;
						}
					}
				}
			}
			elsif (/\"VCCLCompilerTool\"/)
			{
				my $mydef=$myprojname;
				$mydef=~s/([a-z]*)/\U$1\E/g;
				$mydef="__".$mydef."__";
				print OUT $_;
				print $_;
								
				while(<IN>)
				{
					print "add PreprocessorDefinitions\n";
					if($_=~/PreprocessorDefinitions="(.*)"/)
					{
						$_="\t\t\tPreprocessorDefinitions\=\"$1;$mydef\"\n";
						print OUT $_;
						last;
					}
					elsif ($_=~/AdditionalIncludeDirectories/)
					{
						print "remove: $_ \n";
					}
					else
					{
						print OUT $_;
					}
				}
			}
			elsif (/ConfigurationType\=\"4\"/)
			{
				ReplaceOrAddString($_,"\t\t\tConfigurationType\=\"2\"","ConfigurationType",OUT);	 
				
			}
			elsif (/Name\=\"VCLibrarianTool\"/)
			{
				while(<IN>)
				{
					if(/>/)
					{
						ReplaceOrAddString($_,
						"\t\t\t\tName\=\"VCLinkerTool\"
						AdditionalLibraryDirectories\=\"\$(SolutionDir)libdebug\"
						GenerateMapFile\=\"FALSE\"
						MapExports\=\"FALSE\"
						ImportLibrary\=\"\$(OutDir)/\$(ProjectName).lib\"/>",
						">",OUT);	 
						last;
					}
				}
			}
			elsif (/Name\=\"VCLinkerTool\"/)
			{
				
				print OUT $_;
				$_=<IN>;
				ReplaceOrAddString($_,
				"\t\t\t\tAdditionalLibraryDirectories=\"\$(SolutionDir)libdebug\"",
				"AdditionalLibraryDirectories",OUT);
				while(<IN>)
				{
					
					if(/>/)
					{
						ReplaceOrAddString($_,
						"\t\t\t\tImportLibrary\=\"\$(OutDir)\/\$(ProjectName).lib\"/>",
						">",OUT);
						last;
					}
					
				}
				
			}
			elsif (/Name\=\"VCResourceCompilerTool\"/)
			{
				my $IsFinish;
				if(/>/)
				{
					$IsFinish=1;
				}
				if($IsDebug ==1)
				{
					ReplaceOrAddString($_,
					"\t\t\tName\=\"VCResourceCompilerTool\"
					PreprocessorDefinitions\=\"_DEBUG\"
					Culture\=\"1033\"
					AdditionalIncludeDirectories\=\"\$(SolutionDir)arch\\win32\\res\"
					ShowProgress\=\"FALSE\"
					ResourceOutputFileName\=\"\$(SolutionDir)arch\\win32\\res\\\$(InputName).res\"\/>
					",
					"Name",OUT);
					
				}
				else
				{
					ReplaceOrAddString($_,
					"\t\t\tName\=\"VCResourceCompilerTool\"
					PreprocessorDefinitions\=\"NDEBUG\"
					Culture\=\"1033\"
					AdditionalIncludeDirectories\=\"\$(SolutionDir)arch\\win32\\res\"
					ShowProgress\=\"FALSE\"
					ResourceOutputFileName\=\"\$(SolutionDir)arch\\win32\\res\\\$(InputName).res\"\/>
					",
					"Name",OUT);
				}
				if ($IsFinish !=1)
				{
					while(<IN>)
					{
						last if(/>/);
						
					}
				}
			}
			elsif (/<Files>/)
			{
				
				$myprojname="ex_".$myprojname.".h";
				print "Add RelatevePath $myprojname \n";
				ReplaceOrAddString($_,
						"\t<Files>\n\t\t<File
			RelativePath\=\"$myprojname\">
			<\/File>",
				"<Files>",OUT);
			}
				
			else
			{
				print OUT $_;
				
			}
		}
		close(IN);
		close(OUT);
		open(IN, "<$nextname".".bak");
		open(OUT, ">$nextname");
		while(<IN>) 
		{
			if(/<Configuration/)
			{
				$tnext=<IN>;
				if($tnext=~/no use/)
				{
					
				}
				else
				{
					print OUT $_;
					print OUT $tnext;
				}
			}
			else
			{
				print OUT $_;
			}
		}
		close(OUT);
		close(IN);
		unlink($nextname.".bak");
		print "modify $nextname $cnt lines\n";
	}
}

sub listdir
{
	$curdir = $_[0];
	my @retdir;
	opendir(HDIR, $curdir) || die "no $curdir?: $!";
	foreach $name (sort readdir(HDIR)) { # list context, sorted
	       if(-d $name && ($name ne ".") && ($name ne ".."))
	       {
	       	 #print "$name\n"; # prints ., .., system.ini, and so on
	       	 push(@retdir,$name);
	       	}
	}
	closedir(HDIR);
	return @retdir;
}
sub listfile
{
	$curdir = $_[0];
	$key = $_[1];
	my @retfile;
	
	foreach $name (sort <$curdir\/*.*>) 
	{ # list context, sorted
	 	 if(-f $name  )
	       {
		       	if ($name=~/$key/)
		       	{
		     # 	print "before: $name \n";
 		   	my @aa=split(/[\\|\/]/,$name);
		      	my $num=@aa;
		      	if ($num >1)
		      	{
		      		$name=$aa[$num-1];
		      		push(@retfile,$name);
		      	}
		      	else
		      	{
		      		print "Error in listfile \n";
		      	}
		      		
		       	# print "$name\n"; 
		       	}
	       	}
	}
	return @retfile;
}
sub copymyfile
{
	($srcdir,$dstdir,@myfiles)=@_;
	
	foreach $myfile (@myfiles)
	{
		print "move: $srcdir\/$myfile $dstdir\/$myfile  \n";
		rename($srcdir."\/".$myfile,$dstdir."\/".$myfile) || die "can not copy $srcdir"."\/"."$myfile $dstdir"."\/"."$myfile";
	}
}

sub generate_export
{
	
	($projname,$dstdir,@headers)=@_;
	
	my $exportfile=$dstdir."\/"."ex_".$projname.".h";
	my $exportdef=$projname;
	my $myUprojname=$projname;
	$myUprojname=~s/([a-z]*)/\U$1\E/g;
	my $myUheader="__EX_"."$myUprojname"."_H__";
	$myUprojname="$myUprojname"."_DLL";
	$exportdef=~s/([a-z]*)/\U$1\E/g;
	$exportdef="__".$exportdef."__";
	print "Generate $exportfile: header: @headers \n";
	#print "generate $exportfile \n";
	open(myOUT, ">$exportfile")|| die "can not create file $exportfile ";
	print myOUT  "#ifndef $myUheader\n#define $myUheader\n"."#ifndef $exportdef\n \t#ifdef _DEBUG \n\t#pragma comment(lib,\"$projname\") \n\t#else \n\t#pragma comment(lib,\"$projname\") \n\t#endif\n\t#define $myUprojname  __declspec(dllimport)\n#else\n\t#define $myUprojname  __declspec(dllexport)\n";
	foreach $header (@headers)
	{
		#exclude ex_xxx.h
		if(!($header=~/^ex_.*[.]h/))
		{
		#	print myOUT "#include \"$header\"\n";
		}
	}
	print myOUT "#endif\n";
	print myOUT "#endif\n";
	close(myOUT);

}

sub parse_header
{
	#make header include to clean path
	($desdir,$myprojname,@headers)=@_;
	my $myUprojname=$myprojname;
	$myUprojname=~s/([a-z]*)/\U$1\E/g;;
	foreach $header (@headers)
	{
		my $IsHeader;
		my $IsGlobal;
		my $IsReserveFile;
		my $headerdef;
		$headerdef=$header;
		my @bb=split(/[\\|\/]/,$headerdef);
		my $bbnum=@bb;
		$headerdef="$bb[$bbnum-1]";
		$headerdef=~s/[.]/_/g;
		$headerdef=~s/([a-z]*)/\U$1\E/g;
		$headerdef="__"."$myUprojname"."_"."$headerdef"."__";
		#print "$headerdef \n";
		my $IsPragma;
		
		if(!($header=~/[._\w]*[\\\/]*ex_[a-zA-Z0-9]*[.]h$/))
		{
			if ($header=~/[.]h$/)
			{
				$IsHeader =1;
			}
			if ($header=~/type[.]h$/)
			{
				$IsReserveFile=1;
			}
			if ($header=~/global[.]h$/)
			{
				$IsReserveFile=0;
			}
			
			
			open(IN, "<$desdir\/$header");
			open(OUT,">$desdir\/$header.bak");
			while(<IN>) 
			{
				
			if(($_=~/[\/][\/]/) || ($_=~/[\/][\*]/)||($_=~/[\*][\/]/))
			{
				print OUT $_;
			}
			else
			{
				
				#print "$_ \n";
				if($_=~/#include[ ]+[\"|<]{1}(.*[.]h)/)
				#if($_=~/#include[ ]+[\"|<]{1}[\d_.]*[\\|\/]*([a-zA-Z0-9_]+[.]h)[ ]*["|>]{1}/)
				{
					if(!($_=~/terminal/))
					{
					#	print "enter include $_ \n";
						#my @aa=split(/[\\|\/]/,$1);
						#my $num=@aa;
						#$_="#include \"$aa[$num-1]\"\n";
						if (!($_=~/global[.]h/)) 
						{
							print "enter parse $_ \n";
							print OUT $_;
							
						}
						else
						{
							if ($IsHeader ==1)
							{
								print OUT $_;
							}
							else
							{
								print OUT $_;
							}
						}
					}
					else
					{
						print OUT $_;
					}
				}
				elsif ($_=~/class[ ]+(DLL )*(.*)/)
				{
					my $temp=$_;
					$temp="class $myUprojname"."_DLL $2\n";
					if($_=~/template/)
					{$temp=$_;
					}
					print $temp;
					print OUT $temp;
				}
				elsif ($_=~/extern[ ]+(DLL )*(.*)/)
				{
					my $temp=$_;
					$temp="extern $myUprojname"."_DLL $2\n";
					
					print $temp;
					print OUT $temp;
				}
				elsif ($_=~/#define/)
				{	
					if ($IsGlobal !=1 && $IsReserveFile !=1 && $IsHeader==1)
					{
					print OUT $_;
					 ReplaceOrAddString($_,"#include \"ex_$myprojname\.h\"\n","define",OUT);
					$IsGlobal=1;	
					}
					else
					{
					print OUT $_;
					}
				}
				elsif($_=~/#pragma[ ]+once/)
				{
					$IsPragma=1;
					ReplaceOrAddString($_,"#ifndef $headerdef\n#define $headerdef\n#include \"ex_$myprojname\.h\"\n","pragma",OUT);
				}
				else
				{
					
					print OUT $_;
				}
			
			}
			
			}
			if($IsPragma == 1)
			{
				print OUT "\n#endif\n";
			}
			close(IN);
			close(OUT);
			rename("$desdir\/$header.bak","$desdir\/$header");
		}
	}
	
}

while (<>)
{
	
	@aa=split(/[|]/,$_);
	$num=@aa;
	print "$num parameter @aa\n";
	if ($aa[0]=~/#/)
	{
		next;
	}
	if($num ==1)
	{
		chomp ($aa[0]);
		$targetdir=$aa[0];
		$targetdir=~s/[ \\\/]*$//;
		$targetdir=~s/^[ ]*//;
		undef ($newdir);
		undef ($newprojname);
	}
	elsif ($num ==2)
	{
		chomp ($aa[0]);
		chomp ($aa[1]);
		$targetdir=$aa[0];
		$targetdir=~s/[ \\\/]*$//;
		$targetdir=~s/^[ ]*//;
		$newdir=$aa[1];
		$newdir=~s/[ \\\/]*$//;
		$newdir=~s/^[ ]*//;
		undef ($newprojname);
	}
	elsif ($num ==3)
	{
		chomp ($aa[0]);
		chomp ($aa[1]);
		chomp ($aa[2]);
		$targetdir=$aa[0];
		$targetdir=~s/[ \\\/]*$//;
		$targetdir=~s/^[ ]*//;
		$newdir=$aa[1];
		$newdir=~s/[ \\\/]*$//;
		$newdir=~s/^[ ]*//;
		$newprojname=$aa[2];
		$newprojname=~s/[ \\\/]*$//;
		$newprojname=~s/^[ ]*//;
	}
	else
	{
		die "Wrong config format \n";
	}
	print 	"$targetdir:$newdir:$newprojname:\n";
	
	my $targetbak=$targetdir."\\*.bak";
	unlink(<$targetbak>);
	undef ($targetbak);
	modifyproj("$targetdir");
	
	@myfile=listfile("$targetdir","[.]vcproj\$");
	
	$projnum=@myfile;
	if ($projnum >=1)
	{
		($projname)=@myfile;
	}
	
	$projname=~s/(.+)[.]vcproj/$1/;
	print "**************************************\n";
	print "project name: $projname \n";
	print "**************************************\n";
	@myfile=listfile("$targetdir",".cpp\$");
	print "==> \n";
	foreach $file (@myfile)
	{
		print $file."\n";
	}
	parse_header("$targetdir",$projname,@myfile);
	print "<==parse_cpp success \n";
	@myfile=listfile("$targetdir","[.]h\$");
	print "==> \n";
	foreach $file (@myfile)
	{
		print $file."\n";
	}
	parse_header("$targetdir",$projname,@myfile);
	print "<==parse_header success \n";
	generate_export($projname,"$targetdir",@myfile);
	print "==>generate_export success \n";
	@myfile=listfile("$targetdir","[.]h\$");
	
	#copymyfile("$targetdir",".\\include",@myfile);
	print "==>copymyfile success \n";
	print "**************************************\n";
	

}