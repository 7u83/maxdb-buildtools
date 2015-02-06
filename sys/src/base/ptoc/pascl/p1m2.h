(* @(#)pascl/p1m2.h


    ========== licence begin LGPL
    Copyright (C) 2002 SAP AG

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    ========== licence end


*)

(* p1m2.h *)

procedure P1m2I;
external;

procedure ErrorMsg
   (ErrorNumber: integer;
    ColumnNumber: integer);
external;

procedure DclError
   (Name,
    Card,
    File,
    ErrorNumber: integer);
external;

procedure werror
   (message: STRING);
external;

procedure FatalErrorMsg
   (ErrorNumber: integer;
    ColumnNumber: integer);
external;

procedure WarnStrCmp
    (lno : integer);
external;
