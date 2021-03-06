#!/usr/local/bin/perl -w
#/*!
#  @file           Makefile.PL
#  @author         MarcoP, ThomasS
#  @ingroup        dbd::MaxDB
#  @brief          
#
#\if EMIT_LICENCE
#
#    ========== licence begin  PERL_STANDARD
#    Copyright (c) 2001-2004 SAP AG
#
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of either the Artistic License, as
#    specified in the Perl README file or the GNU General Public License
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
#
#\endif
#*/
BEGIN { require 5.004 }	
use Config;
use ExtUtils::MakeMaker 5.16, qw(&WriteMakefile $Verbose);
use File::Basename;
use Getopt::Long;
use File::Spec;

use DBI 1.21;		
use DBI::DBD;
use strict;

sub getSQLDBCSDKFromIndepPath();
sub usageSQLDBCSDK;
sub checkSQLDBCSDKVersion;

my $dbi_dir      = dbd_dbi_dir();
my $dbi_arch_dir = dbd_dbi_arch_dir();

my %opts =
(
    NAME	=> 'DBD::MaxDB',
    VERSION_FROM => 'MaxDB.pm',
    clean	=> { FILES=> 'MaxDB.xsi dbdmaxdb.h' },
    dist	=> {
	DIST_DEFAULT=> 'clean distcheck readmetxt tardist',
	PREOP    => '$(MAKE) -f Makefile.old distdir',
	COMPRESS => 'gzip -v9', SUFFIX => 'gz'
    },
    OBJECT	=> '$(O_FILES)',
 );



if ($ExtUtils::MakeMaker::VERSION >= 5.43) {
    $opts{AUTHOR} = 'Marco Paskamp and Thomas Simenec mailto:maxdb@lists.mysql.com';
    $opts{ABSTRACT} = 'A MaxDB driver for the Perl5 Database Interface (DBI)';
    $opts{PREREQ_PM} = { DBI => 1.21 };
    $opts{CAPI} = 'TRUE' if $Config{archname} =~ /-object\b/i;
}

my $opt_g = 0;
my $opt_o = '';
my $opt_t = 0;
Getopt::Long::GetOptions("g!" => \$opt_g,
                         "t!"  => \$opt_t, 
                         "o=s" => \$opt_o) or die "Invalid arguments";

    print "Overriding MaxDB Directory with command line option: $opt_o\n" if $opt_o ;
    print "Working in our own tool environment (using other perl pathes)\n";

print "\nConfiguring DBD::MaxDB ...\n";


# use own tool environment with special output directories
if ($opt_t)
{
	my ($InstBin, $InstLib, $InstMan1, $InstMan3) = &get_inst_dirs(&get_tool_dir());
	$opts{INSTALLSITELIB} = $InstLib;
	$opts{INSTALLSITEARCH} = $InstLib;
	$opts{INSTALLBIN} = $InstBin;
	$opts{INSTALLSCRIPT} = $InstBin;
	$opts{INSTALLARCHLIB} = $InstLib;
   	$opts{INSTALLMAN1DIR} = $InstMan1;
	$opts{INSTALLMAN3DIR} = $InstMan3;
	$opts{INSTALLSITEMAN1DIR} = "$InstMan1/site";
	$opts{INSTALLSITEMAN3DIR} =  "$InstMan3/site";
}


my $SQLDBC_SDK= $opt_o || $ENV{SQLDBCSDK} || $ENV{INSTROOT} || getSQLDBCSDKFromIndepPath();

unless ($SQLDBC_SDK && -d $SQLDBC_SDK) {die "SQLDBCSDK environment variable ($SQLDBC_SDK) does not refer to a directory.\n";}

my $SQLDBC_lib;
my $SQLDBC_slPath;
my $SQLDBC_sl;
my $SQLDBC_incl=$SQLDBC_SDK.'/sdk/sqldbc/incl';
if ($^O ne 'MSWin32') {
  $opts{OPTIMIZE} = '-g -O0' if $opt_g;
  if (($^O eq 'aix') || ($^O eq 'solaris')) {
    $SQLDBC_slPath=$SQLDBC_SDK.'/lib/lib64';
  } else {
    $SQLDBC_slPath=$SQLDBC_SDK.'/lib';
  }
}else {
  $opts{OPTIMIZE} = '/Zi' if $opt_g;
  $SQLDBC_slPath=$SQLDBC_SDK.'/pgm';
  $SQLDBC_lib=$SQLDBC_SDK.'/sdk/sqldbc/lib/libSQLDBC_C'.$Config{_a};
}
$SQLDBC_sl=$SQLDBC_slPath.'/libSQLDBC_C.'.$Config{so};

print "gar:\$SQLDBC_incl=$SQLDBC_incl\n";

if ($^O eq 'MSWin32') {
  $SQLDBC_lib    = Win32::GetShortPathName($SQLDBC_lib);
  $SQLDBC_slPath = Win32::GetShortPathName($SQLDBC_slPath);
  $SQLDBC_sl     = Win32::GetShortPathName($SQLDBC_sl);
  $SQLDBC_incl   = Win32::GetShortPathName($SQLDBC_incl);
  $SQLDBC_SDK    = Win32::GetShortPathName($SQLDBC_SDK);
}

checkSQLDBCSDK($SQLDBC_sl);
checkSQLDBCSDK("$SQLDBC_incl/SQLDBC_C.h");
checkSQLDBCSDK("$SQLDBC_incl/SQLDBC_Types.h");
checkSQLDBCSDK($SQLDBC_lib) if ($^O eq 'MSWin32');

print "Using MaxDB SQLDBC sdk installed in $SQLDBC_SDK\n";

checkSQLDBCSDKVersion($SQLDBC_incl.'/SQLDBC_C.h');

if ($^O ne 'MSWin32') {
	$SQLDBC_sl     =~ s/^(.*\s+.*)/\"$1\"/;
	$SQLDBC_slPath =~ s/^(.*\s+.*)/\"$1\"/;
	$SQLDBC_incl   =~ s/^(.*\s+.*)/\"$1\"/;
}

if ($^O eq 'aix') {
  $opts{CCFLAGS} .= " -DREL30 -DAIX -D_IBMR2 -DSAG -ma -qnoinline -DBIT64 -qtbtable=full -qalias=noansi -U__STR__ -qchars=signed  -D_LARGE_FILES -D_REENTRANT -qnotempinc";
}
$opts{INC} .= ' -I'.$SQLDBC_incl." -I$dbi_arch_dir";
if ($^O eq 'aix') {
$opts{INC} .= " -q64 -qcpluscmt -qsuppress=1506-234 -DBIT64";
}
if ($^O ne 'MSWin32') {
  $opts{LIBS} .= ' -L'.$SQLDBC_slPath.' -lSQLDBC_C -ldl';
} else {
  $opts{LIBS} .= ' '.$SQLDBC_lib;
}

print "\n";
WriteMakefile(%opts);

local($^W)=0;
print qq{
The DBD::MaxDB tests will use these values for the database connection:
    DBI_DSN=$ENV{DBI_DSN}	e.g. dbi:MaxDB:<host>/<dbname>
    DBI_USER=$ENV{DBI_USER}
    DBI_PASS=$ENV{DBI_PASS}
};
print "Warning: not all required environment variables are set.\n"
	unless ($ENV{DBI_DSN} && $ENV{DBI_USER} && $ENV{DBI_PASS});
print "Warning: DBI_DSN ($ENV{DBI_DSN}) doesn't start with 'dbi:MaxDB:'\n"
	if ($ENV{DBI_DSN} && $ENV{DBI_DSN} !~ m/^dbi:MaxDB:/);
print "\n";

sub MY::post_constants {
   my ($self) = shift;
   my $sqldbc_sl_name = 'libSQLDBC_C.'.$Config{so};
   '
# copy some files
inst_libdbdsqldbc =       ' . File::Spec->catdir ($self->{INST_LIB}, 'DBD/MaxDB') . '
getinfo_pm =              ' . File::Spec->catfile($self->{INST_LIB}, 'DBD/MaxDB', 'GetInfo.pm') . '
typeinfoascii_pm =        ' . File::Spec->catfile($self->{INST_LIB}, 'DBD/MaxDB', 'TypeInfoAscii.pm') . '
typeinfounicode_pm =      ' . File::Spec->catfile($self->{INST_LIB}, 'DBD/MaxDB', 'TypeInfoUnicode.pm') . '
inst_libdbdsqldbc_c_sl =  ' . File::Spec->catdir ($self->{INST_ARCHAUTODIR}, '') . '
libdbdsqldbc_c_sl =       ' . File::Spec->catfile($self->{INST_ARCHAUTODIR}, '', $sqldbc_sl_name) . '

config :: $(getinfo_pm) $(typeinfoascii_pm) $(typeinfounicode_pm) $(libdbdsqldbc_c_sl)
	@$(NOOP)

$(getinfo_pm): GetInfo.pm
	$(NOECHO) $(MKPATH) $(inst_libdbdsqldbc)
	$(NOECHO) $(RM_F) $(getinfo_pm)
	$(CP) GetInfo.pm $(getinfo_pm)

$(typeinfoascii_pm): TypeInfoAscii.pm
	$(NOECHO) $(MKPATH) $(inst_libdbdsqldbc)
	$(NOECHO) $(RM_F) $(typeinfoascii_pm)
	$(CP) TypeInfoAscii.pm $(typeinfoascii_pm)

$(typeinfounicode_pm): TypeInfoUnicode.pm
	$(NOECHO) $(MKPATH) $(inst_libdbdsqldbc)
	$(NOECHO) $(RM_F) $(typeinfounicode_pm)
	$(CP) TypeInfoUnicode.pm $(typeinfounicode_pm)

$(libdbdsqldbc_c_sl): '.$SQLDBC_sl.'
	$(NOECHO) $(MKPATH) $(inst_libdbdsqldbc_c_sl)
	$(NOECHO) $(RM_F) $(libdbdsqldbc_c_sl)
	$(CP) '.$SQLDBC_sl.' $(libdbdsqldbc_c_sl)
';
}

# ====================================================================
sub checkSQLDBCSDK {
  my ($filename)=@_;
print ("gar: check $filename\n");
  unless ( -f $filename){ 
    print "\n";
    print "Cannot find $filename\n\n";
    print "The DBD::MaxDB module needs to link with MaxDB's common database \n";
    print "interface SQLDBC. The DBD::MaxDB module does _not_ include the\n";
    print "MaxDB SQLDBC sdk! If it isn't already installed you can download\n";
    print "it from the MySQL homepage.\n\n";
    print "You need also to set the path to the SQLDBC sdk directory.\n";
    print "You can do this ether by setting the SQLDBCSDK environment variable\n";
    print "or by runing 'perl Makefile.PL -o <sqldbc-sdk-dir>'.\n\n";
    die "Makefile.PL aborted.\n";
  }
}

sub checkSQLDBCSDKVersion {
  my ($sqldbc_h) = @_;
  my $version = undef; 
  my $version_okay = 0; 
  if (open FILE_IN, "<$sqldbc_h") {
    while (<FILE_IN>) {
      chomp $_;
      if (/^#define SQLDBC_CHeaderVersion \"SQLDBC_C.H\s*((\d+).(\d+).(\d+)\s*BUILD\s*(\d+).*)\"$/) {
			   if ( $2<7 || ($2>=7 && $3<5) || ($2==7 && $3==5 && $4==0 && $5<12)){ 
			    $version = $1;
          last;
			  }
			  $version_okay = 1;
        print "SQLDBC sdk version is $1\n"; 
        last;
      }
    }
    close (FILE_IN);
    if (! $version_okay){ 
      print "\n";
      if ($version){
        print "Wrong version of SQLDBC sdk found $version.\n"; 
      }else {
        print "Cannot find the version of SQLDBC sdk.\n"; 
      }
	    print "The version of SQLDBC-SDK must be at least 7.5.0 Build 012.\n\n";
	    print "The DBD::MaxDB module needs to link with MaxDB's common database \n";
	    print "interface SQLDBC. The DBD::MaxDB module does _not_ include the\n";
	    print "MaxDB SQLDBC sdk! If it isn't already installed you can download\n";
	    print "it from the MySQL homepage.\n\n";
	    print "You need also to set the path to the SQLDBC sdk directory.\n";
	    print "You can do this ether by setting the SQLDBCSDK environment variable\n";
	    print "or by runing 'perl Makefile.PL -o <sqldbc-sdk-dir>'.\n\n";
	    die "Makefile.PL aborted.\n";
  	}
  }
}

sub getSQLDBCSDKFromIndepPath(){
  my $indep=undef;
  if ($^O eq 'MSWin32') {
     eval {require Win32API::Registry; }; if ($@) {return undef};
     my ($key, $type);
     Win32API::Registry::RegOpenKeyEx( Win32API::Registry::HKEY_LOCAL_MACHINE(), "SOFTWARE\\SAP\\SAP DBTech", 0, Win32API::Registry::KEY_READ(), $key )
         or  return undef;
     Win32API::Registry::RegQueryValueEx( $key, "IndepPrograms", [], $type, $indep, [] )
         or  return undef;
     Win32API::Registry::RegCloseKey( $key );
  }else {
    if (open FILE_IN, "</etc/opt/sdb") {
      while (<FILE_IN>) {
        chomp $_;
        next if ((/^$/) or (/^#/));
        if (/^IndepPrograms=(.*)$/) {
          $indep=$1;
          last;
        }
      }
      close (FILE_IN);
    }
  }
  return $indep;
} 


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

 
{
    package MY; # SUPER needs package context, $self is not sufficient

    use strict;
    use Config;

    sub postamble {
      require DBI::DBD;
      "\n" . DBI::DBD::dbd_postamble(@_) . <<"POSTAMBLE";
    
readmetxt: README.pod
\tpod2text README.pod  > README
    
POSTAMBLE
    };

#    sub postamble {
#       return main::dbd_postamble(@_);
#    }
}
__END__


