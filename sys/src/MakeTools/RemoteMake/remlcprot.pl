#!/usr/bin/perl
#
#
#    ========== licence begin  GPL
#    Copyright (C) 2001 SAP AG
#
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License
#    as published by the Free Software Foundation; either version 2
#    of the License, or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#    ========== licence end
#
use iprot;

my( $i, $maxi ) = ( 0, $#ARGV );
while( $_ = $ARGV[ $i ], ( $i<=$maxi)  )
{
	if (/^-\?$/)
	{
		iprot ("remlcprot <platform>", "-h");
		exit;
	}
	$i++;
}

$platform = $ARGV[0];
shift;


@tmp=@ARGV;
@ARGV = ( "-action=lcprot.pl", $platform );
push @ARGV, @tmp;
if ( defined $DEBUGREMFREE )
{
	print "$ENV{'TOOL'}\\bin\\remlcmake$ENV{'TOOLEXT'}";
    foreach ( @ARGV ) { print " $_"; }
	print "\n";
}
else
{
	my $lastarg=($ARGV[$#ARGV]);
	$lastarg=~ tr/://d;
	$lastarg=~ tr/\//_/;
	$MakeProt=convertFileString("$ENV{OWN}\\tmp\\${platform}_${lastarg}.prot");
	$MakeErr=convertFileString("$ENV{OWN}\\tmp\\${platform}_${lastarg}.err" );
		
	open(SAVEOUT, ">&STDOUT") ;
	open(SAVEERR, ">&STDERR") ;
	
	close(STDOUT) ;
	close(STDERR) ;
	
	open(STDOUT, ">$MakeProt") ;
	open(STDERR, ">$MakeErr") ;
	
	select(STDERR); $| = 1;
	select(STDOUT); $| = 2;
   
	do "$ENV{'TOOL'}\\bin\\remlcmake$ENV{'TOOLEXT'}";
	
	close ( STDOUT );
	close ( STDERR );
	
	open ( STDOUT , ">&SAVEOUT" ) ;
	open ( STDERR , ">&SAVEERR" ) ;
	
	open (ERRFILE, "<$MakeErr");
	if ($line=<ERRFILE>)
	{
		print $line;
		while(<ERRFILE>)
		{ print $_;}
		close (ERRFILE);
	}
	else
	{
		@ARGV= ("$MakeProt");
		do "$ENV{'TOOL'}\\bin\\opendoc$ENV{'TOOLEXT'}";
	}
}
