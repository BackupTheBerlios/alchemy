#!/usr/bin/perl

#this script sets the maxWalksatExe_ variable in maxwalksat.cpp

$mwsdir = $ARGV[0];
$mwsexedir = $ARGV[1];

if ( (!$mwsdir) || (!$mwsexedir) )
{
  print "Please specify location of maxwalksat.cpp\n";
  exit(0);
}

$file = "$mwsdir/maxwalksat.cpp";

print " P: $file ";

open (IN, $file) || die("ERROR: Unable to open $file");
{
  undef $/;
  $target = <IN>;
}
close (IN) || die("ERROR: Unable to close $file");


$target =~ s/maxWalksatExe_ = ".\/maxwalksat";/maxWalksatExe_ = "$mwsexedir\/maxwalksat";/g;


open(MODIFIED, ">$file") || die("ERROR: Unable to write $file");
print MODIFIED $target;
close(MODIFIED) || die("ERROR: Unable to close $file");
exit(0);
