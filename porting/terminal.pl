
sub terminal_header
{
	#make header include to clean path
	($header)=@_;
	$myUprojname="TERMINAL";
			
			if ($header=~/[.]h$/)
			{
				$IsHeader =1;
			}
						
			open(IN, "<$header");
			@AA=<IN>;
			my $IsDef;
			for ($i=0; $i<10; $i++)
			{
				 if ($AA[$i]=~/ifndef/)
				 {
				 	$IsDef=$i;
				 	last;
				}
			}
			#print " IsDef $IsDef \n";
			if(defined($IsDef))
			{
				for($i=0; $i <=($IsDef+2); $i++)
				{
					$temp=shift(@AA);
					if($temp =~/#define/)
					{
						#print " catch #define \n";
						last;
					}
					
				}
				if (!($temp = ~/#define/))
				{
					die "Error header \n";
				}
				for ($i=0; $i<=10;$i++)
				{
					$temp=pop(@AA);
					if($temp=~/#endif/)
					{
						
						last;
					}
				}
			}
			unshift(@AA,"\/\/ $header\n");
			my $IsCommentOut=0;
			foreach $_ (@AA) 
			{
				
				#print "old: $_ \n";
				if(($_=~/^[\/][\/]/) )
				{
					print  $_;
				}
				elsif ($IsCommentOut)
				{
					print $_;
				}
				elsif ($_=~/[\/][*]/)
				{
					
					$IsCommenOut=1;
					print $_;
				}
				elsif ($_=~/[*][\/]/)
				{
					
					$IsCommenOut=0;
					print $_;
				}
				else
				{
					
					#print "$_ \n";
					if($_=~/#include[ ]+[\"|<]{1}(.*[.]h)/)
					#if($_=~/#include[ ]+[\"|<]{1}[\d_.]*[\\|\/]*([a-zA-Z0-9_]+[.]h)[ ]*["|>]{1}/)
					{
						
					}
					elsif ($_=~/class[ ]+(DLL )*(.*)/)
					{
						
						print $_;
						
					}
					elsif ($_=~/"C"/)
					{
						print $_;
					}
					elsif ($_=~/extern[ ]+(DLL )*(.*)/)
					{
						my $temp=$_;
						$temp="extern $myUprojname"."_DLL $2\n";
						
						print $temp;
						
					}
					
					elsif ($_=~/#define/)
					{	
						
						print $_;
					}
					elsif ($_=~/typedef/)
					{
						print $_;
					}
					elsif( $_=~/[;][a-zA-Z\\]+/)
					{
						print $_;
					}
					elsif ($_=~/([a-zA-z_0-9*]*)[ |\t]+([*a-zA-z_0-9]*)[(](.*)/)
					{
						my $aa=$_;
						$aa=~s/[\/]{2}.*[\n]//;
						$aa=~/([a-zA-z_0-9*]*)[ |\t]+([*a-zA-z_0-9]*)[(](.*)/;
						if(defined($3))
						{
							print "$myUprojname"."_DLL $1 $2($3 \n";
						}
					}
					elsif($_=~/#pragma[ ]+once/)
					{
						
					}
					else
					{
						
						print $_;
					}
				
				}
			
			}
			
			close(IN);
				
	
}
my $cnt;
my @AA;
print "#ifndef _DLL_TERMINAL_H_
#define _DLL_TERMINAL_H_

#include \"type.h\"
#include \"ex_terminal.h\"\n";

while (<>)
{
	
	#$cnt++;
	#if($cnt <5)
	{
	$targetpath=$_;
	@aa=split(/[\\|\/]/,$_);
	$num=@aa;
	#print "$num parameter @aa\n";
	$targetfilename=$aa[$num-1];
	#print 	"open ==>  $targetfilename \n";
	
	terminal_header("$targetpath");
	
	#print " No. $cnt  ";
	close(IN);
	#print "<==========\n\n";
	}

}
print "#endif\n";