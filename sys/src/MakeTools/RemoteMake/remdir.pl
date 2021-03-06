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


sub get_remoptions
{
	my $args = shift;
	my @call_options=();
	
	while ( $$args[0] =~ /^-.*/)
	{
		push @call_options, $$args[0];
		shift @$args;
	}
	return @call_options;
}

my @remcall_options = get_remoptions(\@ARGV);

$platform = $ARGV[0];
shift;

@tmp=@ARGV;
unless ($ENV{APOMAKE})
{
	@ARGV=( 'dir', 'free');
	push @ARGV, @remcall_options;
	push @ARGV, "$platform", 'xterm' ;
	push @ARGV, @tmp;
	push @ARGV, @remcall_options;
	
	if ( defined $DEBUGREMFREE )
	{
		print "$ENV{'TOOL'}\\bin\\remim$ENV{'TOOLEXT'}";
	    foreach ( @ARGV ) { print " $_"; }
		print "\n";
	}
	else
	{
		do "$ENV{'TOOL'}\\bin\\remim$ENV{'TOOLEXT'}";
	}
}
else
{
	@ARGV = ( "-action=dir", $platform );
	push @ARGV,  @tmp ;
	unshift @ARGV, @remcall_options;
	if ( defined $DEBUGREMFREE )
	{
		print "$ENV{'TOOL'}\\bin\\remlcmake$ENV{'TOOLEXT'}";
	    foreach ( @ARGV ) { print " $_"; }
		print "\n";
	}
	else
	{
		do "$ENV{'TOOL'}\\bin\\remlcmake$ENV{'TOOLEXT'}";
	}
}
