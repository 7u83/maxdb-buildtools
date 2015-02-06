#! /usr/bin/perl
#
#    SimpleCfg.pm - Implements reading / writing simple config files.
#
#    @(#)SimpleCfg.pm     2003-04-22
#
#    U. Jansen, SAP AG
#
#    ========== licence begin LGPL
#    Copyright (C) 2002 SAP AG
#
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public
#    License as published by the Free Software Foundation; either
#    version 2.1 of the License, or (at your option) any later version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#    ========== licence end
#

##############################################################################
# Static includes:

package IO::SimpleCfg;

require Exporter;

@ISA    = qw(Exporter);
@EXPORT = qw(LoadSimpleCfg SaveSimpleCfg);

use strict;

my $ProgramName = "SimpleCfg";
my $Version     = "1";
my $Revision    = "02";

##############################################################################
# LoadSimpleCfg() - Loads a simple config file into a hash reference.
##############################################################################

sub LoadSimpleCfg
{
    my ($fname) = @_;
    my $href    = {};
    my $line    = 0;
    my $secname = "";

    unless ($fname) { die "LoadSimpleCfg: ERROR! First parameter (file name) is not set!\n" }

    if (open FILE_IN, "<$fname")
    {
        while (<FILE_IN>)
        {
            $line ++;
            chomp $_;

            next if ((/^$/) or (/^#/));

            if (/^\[(.*)]$/)
            {
                next if ($1 =~ /^__.*/);
                $secname = $1;
                $href->{$secname} = {};
            }
            elsif (/^(.*)=(.*)$/)
            {
                next if ($1 =~ /^__.*/);
                if ($secname)
                {
                    $href->{$secname}->{$1} = $2;
                }
                else
                {
                    $href->{$1} = $2;
                }
            }
            else
            {
                print STDERR "LoadSimpleCfg: Error parsing file '$fname' line $line!\nUnexpected line format found!\n";
            }
        }
        close (FILE_IN);
    }

    return $href;
}

##############################################################################
# SaveSimpleCfg() - Saves a hash reference into a simple config file.
##############################################################################

sub SaveSimpleCfg
{
    my ($fname, $href, $sec) = @_;
    my $found = 0;

    unless ($fname) { die "SaveSimpleCfg: ERROR! First parameter (file name) is not set!\n" }
    unless ($href)  { die "SaveSimpleCfg: ERROR! Second parameter (hash ref) is not set!\n" }

    unlink ($fname) if (-e "$fname");
    if (open FILE_OUT, ">$fname")
    {
        print FILE_OUT "# File generated by $ProgramName v$Version.$Revision\n";
        print FILE_OUT "# Generation time: " . (scalar localtime) . "\n\n";

        foreach my $key (keys %{$href})
        {
            unless (ref($href->{$key}))
            {
                next if ($key =~ /^__.*/);
                print FILE_OUT "$key=$href->{$key}\n";
                #print STDERR "SaveSimpleCfg: WARNING! Data is written outside a section!\n";
                $found = 1;
            }
        }

        print FILE_OUT "\n" if ($found);

        foreach my $key (keys %{$href})
        {
            if (ref($href->{$key}))
            {
                next if ($key =~ /^__.*/);
                if (ref($href->{$key}) eq "HASH")
                {
                    next if ($key =~ /^__.*/);
                    print FILE_OUT "[$key]\n";

                    foreach my $subkey (keys %{$href->{$key}})
                    {
                        next if ($subkey =~ /^__.*/);
                        print FILE_OUT "$subkey=$href->{$key}->{$subkey}\n";
                    }

                    print FILE_OUT "\n";
                }
                else
                {
                    print STDERR "SaveSimpleCfg: ERROR! Reference type '" . ref($href->{$key}) . "' not implemented yet!\n";
                }
            }
        }

        unless(close FILE_OUT)
        {
            print STDERR "SaveSimpleCfg: Can't close file '$fname'!\n$!\n";
        }
        
        if ($sec)
        {
            if ($^O =~ /Win32/i)
            {
                require Win32::FileSecurity;
                my $login = getlogin();
                my %newhash;
                $newhash{$login} = Win32::FileSecurity::MakeMask(qw(FULL));
                Win32::FileSecurity::Set ($fname, \%newhash);
            }
            else
            {
	            chmod 0700, $fname;
            }
        }
    }
    else
    {
        print STDERR "SaveSimpleCfg: Can't open file '$fname' for writing!\n$!\n";
    }
}

1;