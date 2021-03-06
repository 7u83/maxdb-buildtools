/*!*****************************************************************************

  module      : Ispc_geo36.h

  ----------------------------------------------------------------------------

  responsible : PeterG (GoertzP)

  special area: Generator of interface description code
  description : GUIDs

  ----------------------------------------------------------------------------

    ========== licence begin LGPL
    Copyright (C) 2000 SAP AG

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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
    ========== licence end

*******************************************************************************/


#ifndef ISPC_GEO36_H
#define ISPC_GEO36_H


#include "idl/xml2ispc/Ispc_vsp001.hpp"


#ifndef INITGUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        extern "C" const GUID name
#else
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        extern "C" const GUID name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif // INITGUID

DEFINE_GUID(IID_IliveCacheSink,
            0x8580E621, 0x243F, 0x11d1, 0xB5, 0xA1, 0xAC, 0x7F, 0x01, 0x06, 0x8C, 0x36);
DEFINE_GUID(IID_IliveCacheSpec,
            0x0D4BFA71, 0x5A99, 0x11D1, 0xA9, 0x43, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);

DEFINE_GUID(IID_IliveCacheDbg,
            0x1AEB0290, 0x50F2, 0x11D1, 0xA9, 0x3E, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
/* Rel 72.01.03
DEFINE_GUID(CLSID_liveCacheDbg,
            0x2fcbce35, 0x1a43, 0x11d2, 0xa9, 0x77, 0x00, 0xa0, 0xc9, 0x43, 0x11, 0xa5);
DEFINE_GUID(CLSID_LiveCacheRMT,
            0x2fcbce36, 0x1a43, 0x11d2, 0xa9, 0x77, 0x00, 0xa0, 0xc9, 0x43, 0x11, 0xa5);
*/

/* Rel 72.01.05 */
// {A435CDA1-A24E-11d3-A996-00A0C94311A5}
//DEFINE_GUID(CLSID_liveCacheDbg,
//0xa435cda1, 0xa24e, 0x11d3, 0xa9, 0x96, 0x0, 0xa0, 0xc9, 0x43, 0x11, 0xa5);
//
// {A435CDA2-A24E-11d3-A996-00A0C94311A5}
//DEFINE_GUID(CLSID_LiveCacheRMT,
//0xa435cda2, 0xa24e, 0x11d3, 0xa9, 0x96, 0x0, 0xa0, 0xc9, 0x43, 0x11, 0xa5);

/* Rel 72.2.0 */
// {E458D6A1-D54E-11d3-A99C-00A0C94311A5}

// Rel 72.4.02  PTS 1107355
// {E8254301-773C-11d4-B2E3-0050DAE0298C}
//DEFINE_GUID(CLSID_liveCacheDbg,
//0xE8254301, 0x773C, 0x11d4, 0xB2, 0xE3, 0x00, 0x50, 0xDA, 0xE0, 0x29, 0x8C);

// Rel 74  PTS 1107355
// {31EBC5F3-7DAC-11d4-B2E3-0050DAE0298C}
DEFINE_GUID(CLSID_liveCacheDbg,
0x31ebc5f3, 0x7dac, 0x11d4, 0xb2, 0xe3, 0x0, 0x50, 0xda, 0xe0, 0x29, 0x8c);

// {E458D6A2-D54E-11d3-A99C-00A0C94311A5}
DEFINE_GUID(CLSID_LiveCacheRMT,
0xe458d6a2, 0xd54e, 0x11d3, 0xa9, 0x9c, 0x0, 0xa0, 0xc9, 0x43, 0x11, 0xa5);

/**
#if !defined(_WIN32)
DEFINE_GUID(GUID_NULL,
            0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
DEFINE_GUID(IID_IUnknown,
            0x00000000, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
DEFINE_GUID(IID_IClassFactory,
            0x00000001, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
#endif
**/

DEFINE_GUID(GUID_LC_CHAR,
            0xA6DE9A83, 0x6173, 0x11d1, 0xA9, 0x43, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
DEFINE_GUID(GUID_LC_WCHAR,
            0xA6DE9A84, 0x6173, 0x11d1, 0xA9, 0x43, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
DEFINE_GUID(GUID_LC_BYTE,
            0xA6DE9A85, 0x6173, 0x11d1, 0xA9, 0x43, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
DEFINE_GUID(GUID_LC_BOOL,
            0xA6DE9A87, 0x6173, 0x11d1, 0xA9, 0x43, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
DEFINE_GUID(GUID_LC_INT1,
            0x7b0a5bd0, 0x68f1, 0x11d2, 0xa9, 0x80, 0x0, 0xa0, 0xc9, 0x43, 0x7, 0x31);
DEFINE_GUID(GUID_LC_ABAP_TAB_HANDLE,
            0x33B799A1, 0x85BD, 0x11d1, 0xA9, 0x3F, 0x00, 0xA0, 0xC9, 0x44, 0x90, 0x7B);
DEFINE_GUID(GUID_OMSTYPESTREAMHANDLE,
            0xee4b31fb, 0xcf89, 0x11d1, 0xa9, 0x55, 0x0, 0xa0, 0xc9, 0x43, 0x7, 0x31);
DEFINE_GUID(GUID_LC_PARAM_DESC,
            0x74f2c121, 0x27ec, 0x11d3, 0xa9, 0x96, 0x0, 0xa0, 0xc9, 0x43, 0x7, 0x31);
/* neue Typen mit 7.2.01.05 */
DEFINE_GUID(GUID_LC_UINT1,
            0x92F1D481, 0xA63E, 0x11d3, 0xa9, 0x97, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
DEFINE_GUID(GUID_LC_SCHEMA_HANDLE,
            0xF2AD8221, 0xA641, 0x11d3, 0xa9, 0x97, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
DEFINE_GUID(GUID_LC_CONTAINER_NO,
            0xF2AD8222, 0xA641, 0x11d3, 0xa9, 0x97, 0x00, 0xA0, 0xC9, 0x43, 0x11, 0xA5);
DEFINE_GUID(GUID_LC_ASCII7,
            0x09D9CE43, 0x88D5, 0x478A, 0x99, 0xEA, 0x00, 0xAD, 0x11, 0xA1, 0x44, 0xFF);
DEFINE_GUID(GUID_LC_WYDE,
            0x4A523F86, 0x2315, 0x4e0c, 0xBF, 0x9B, 0x95, 0xF4, 0x47, 0x74, 0x23, 0x44);
/* PTS 1131555, PG */
DEFINE_GUID(GUID_LC_NUMC,
            0x0FAF0BA8, 0xF4FD, 0x4083, 0x9B, 0xF8, 0x1C, 0x38, 0x66, 0x30, 0xB3, 0xAB);


#endif /* ISPC_GEO36_H */

