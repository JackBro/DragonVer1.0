$num=@ARGV;
if ($num>=1)
{
	$curdir = $ARGV[0];
	chomp($curdir);
}
else
{
	die "input parameter pls\n";
}
print "curdir is :$curdir \n";
chdir $curdir;
opendir DH, $curdir;
foreach $name (sort (readdir DH)) 
#foreach $name (sort <tools\* tools\*.*>) 
{ # list context, sorted
	#print "$name\n";
       if(-d $name  )
       {
	       
	       	print "$name\n";
	     
       	}
}
closedir DH;
