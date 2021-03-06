# -*- perl -*-

use strict;

use ExtUtils::MakeMaker;
use Config;

my ($InstBin, $InstLib, $InstMan1, $InstMan3) = &get_inst_dirs(&get_tool_dir());


$| = 1;

# Check, if fork() is available. If so, we assume that syslog should
# be available too. Thankfull for any better recommendations ...
if ($Config::Config{'d_fork'} eq 'define') {
    print "Checking for Sys::Syslog ... ";
    eval { require Sys::Syslog };
    if ($@) {
	my $errmsg = $@;
	$errmsg =~ s/^/  /mg;
	if ($@ =~ /h2ph/) {
	    print STDERR <<"MSG";

While loading the Sys::Syslog module, I received the following error message:

$errmsg

Most probably this means that you did not run the h2ph script after
installing Perl. You can do this now by executing the commands

    cd /usr/include
    h2ph *.h */*.h */*/*.h


MSG
            exit 10;
        }
	my $reply = prompt(qq{

Your system looks like Unix, as you seem to have fork() available. However
I cannot load the Sys::Syslog module which should be working on any Unix
machine. Please check out the following error message:

$@

In either case, please contact the package author, joe\@ispsoft.de and
let him know your machine configuration (try "perl -V") and the above
error message.

Do you want to continue? }, "y");
	if ($reply !~ /y/i) {
	    exit 1;
	}
    } else {
	print "ok\n";
    }
}


# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
my %opts = (
    'NAME'	=> 'RPC::PlServer',
    INSTALLSITELIB      => $InstLib,
    INSTALLSITEARCH     => $InstLib,
    INSTALLBIN          => $InstBin,
    INSTALLSCRIPT       => $InstBin,
    INSTALLARCHLIB      => $InstLib,
    INSTALLMAN1DIR => $InstMan1,
	INSTALLMAN3DIR => $InstMan3,
	INSTALLSITEMAN1DIR =>  "$InstMan1/site",
	INSTALLSITEMAN3DIR =>  "$InstMan3/site",		
	'DISTNAME'  => 'PlRPC',
    'dist'      => { SUFFIX   => '.gz', DIST_DEFAULT => 'all tardist',
                     COMPRESS => 'gzip -9vf' },
    'VERSION_FROM' => 'lib/RPC/PlServer.pm', # finds $VERSION
    'LIBS'	=> [''],   # e.g., '-lm' 
    'DEFINE'	=> '',     # e.g., '-DHAVE_SOMETHING' 
    'INC'	=> '',     # e.g., '-I/usr/include/other'
    'clean'     => { 'FILES' => 'ndtest.prt t/*.cfg' }
);

if ($ExtUtils::MakeMaker::VERSION >= 5.43) {
    $opts{'AUTHOR'} = 'Jochen Wiedmann (joe@ispsoft.de)';
    $opts{'PREREQ_PM'} = { 'Storable'=> 0,
			   'Net::Daemon' => 0.13 },
    $opts{'ABSTRACT'} = 'Perl Remote Method Invocation';
}

WriteMakefile(%opts);


sub get_inst_dirs {
	my $InstDir = shift;
	my $InstBin = $InstDir."/bin";
	my $InstMan1 = $InstDir."/man/man1";
	my $InstMan3 = $InstDir."/man/man3";
	my $InstLib = $InstDir;
	if ($^O =~ /win32/i) {
		$InstLib .= "/lib/Perl";
	} else {
		$InstLib .= "/lib/perl5";
	}
	return ($InstBin, $InstLib, $InstMan1, $InstMan3 );
}


sub get_tool_dir {
    my $TOOL = $ENV{"TOOL"};
    unless (defined $TOOL && $TOOL ne "") {
        die "\$TOOL undefined, please set \$TOOL\n";
    }
    return $TOOL;
}



package MY;

sub libscan { my($self, $path) = @_; ($path =~ /\~$/) ? undef : $path }

sub postamble {
    <<"END_OF_POSTAMBLE";
pm_to_blib: README

README: lib/RPC/PlServer.pm
\tperldoc -t lib/RPC/PlServer.pm >README

END_OF_POSTAMBLE
}

