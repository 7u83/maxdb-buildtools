# LWP.com
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

&if $OS = UNIX
*.pm ->$TOOL/lib/perl5/LWP/ ascii
media.types ->$TOOL/lib/perl5/LWP/ ascii
&endif
&if $OS = WIN32
*.pm ->$TOOL/lib/Perl/LWP/ ascii
media.types ->$TOOL/lib/Perl/LWP/ ascii
&endif

