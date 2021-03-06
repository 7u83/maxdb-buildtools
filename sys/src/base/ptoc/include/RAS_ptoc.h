/* @(#)RAS_ptoc.h

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

 */

#include           <string.h>

#if defined (__OS2__) || defined (_WIN32)

#include           <stdio.h>
#include           <string.h>
#include           <math.h>
#include           <time.h>

#endif


#if defined (_WIN32)

#pragma warning (disable : 4047 4133 4146 4307 4308 4756 4116)
#pragma warning (disable : 4035 4101 4018) /*RAS+*/

#if !defined(_System)
#define _System
#endif

#endif

#if defined(__OS2__) || defined(_WIN32)
#define sql__trunc(value)                           ((long)(value))
#define sql__sin(value)                             sin(value)
#define sql__srand(value)                           (long)sql__seed_v;sql__seed_v = (value)
#define sql__rd()                                   ( (double) (rand()) ) / (double) 32767;
#define sql__clock()                                ( (clock() * 1000) / CLOCKS_PER_SEC )
#define sql__ckl()                                  ( (clock() * 1000) / CLOCKS_PER_SEC )
#define sql__ucmp                                   memcmp

#ifndef NO_SED   /*RAS+ for test*/
#define vabort() _vabort( __LINE__, __FILE__)
#endif		 /*RAS+ for test*/

#endif


/*
 *  Function to use for memory-to-memory copy.
 */
#define __PTCCPY_ memcpy

/*
 *  Function to use for filling a block of memory with a byte pattern.
 */
#define _ptoc_MEMSET memset

/* T31 specific version obsolete for TOS 4.1 and higher */

/*
 *  Function to use for unsigned memory-to-memory comparison.
 *  Include all machines whose memcmp executes true unsigned comparison.
 *  All other machines use our own function sql__ucmp.
 */
#if T31 || SEQUENT || HP9 || NMP || ALPHA || SUN && SVR4 || SOLARIS || __OS2__ || _WIN32
#define __PTCCMP_ memcmp
#else
#define __PTCCMP_ sql__ucmp
#endif

/*
 *  Condition when to use __PTCCPY_ instead of inline copy.
 *  If not defined, inline copy will not be used.
 *  If BYTE_STRUCT is defined, inline copy will always be used.
 */
#if __OS2__
#define __PTCUSECPY_(x) (((x)&1) || ((x) > 1000))
#else
#if T31
#define __PTCUSECPY_(x) (((x)&1) || ((x) > 100))
#else
#if I386
#define __PTCUSECPY_(x) ((x) > 180)
#else
#if X86_64
#define __PTCUSECPY_(x) ((x) > 180)
#else
#if SUN || sun || SPARC
#define __PTCUSECPY_(x) ((x) > 10)
#else
#if HP9
#define __PTCUSECPY_(x) ((x) > 16)
#else
#if OSF1
/* #define __PTCUSECPY_(x,L,R) ((x) > 33 || ((x) > 43 && !L&7 && !R&7)) */
#define __PTCUSECPY_(x) ((x) > 33)
#else
#endif /*OSF1*/
#endif /*HP9*/
#endif /*SUN/SPARC*/
#endif /*I386*/
#endif /*T31*/
#endif /*__OS2__*/

#if T31
#define sqlabs abs
#endif

/*
 *  _ptoc_ASSIGN
 */
#if BYTE_STRUCT
#define _ptoc_ASSIGN(typ,size,left,right) { \
        struct _ptoc_TEMPSTRUCT { typ space[size]; }; \
        *(struct _ptoc_TEMPSTRUCT *)(left) = \
        *(struct _ptoc_TEMPSTRUCT *)(right); \
        }
#else
#ifdef  __PTCUSECPY_
#define _ptoc_ASSIGN(typ,size,left,right) { \
        if ( __PTCUSECPY_(sizeof(typ)*size) ) \
            __PTCCPY_(left,right,sizeof(typ)*size); \
        else { \
            struct _ptoc_TEMPSTRUCT { typ space[size]; }; \
            *(struct _ptoc_TEMPSTRUCT *)(left) = \
            *(struct _ptoc_TEMPSTRUCT *)(right); \
            } \
        }
#else
#define _ptoc_ASSIGN(typ,size,left,right) { \
            __PTCCPY_(left,right,sizeof(typ)*size); \
        }
#endif /*__PTCUSECPY_*/
#endif /*BYTE_STRUCT*/

/*
 *  SQLMOVC generated by comppc for fixed length moves.
 */
#if BYTE_STRUCT
#define SQLMOVC(size1,size2,val1,p1,val2,p2,cnt) { \
        char *cval1 = (char*) val1 ; \
        char *cval2 = (char*) val2 ; \
        struct _ptoc_TEMPSTRUCT { char  space[cnt]; }; \
        if ((p1 + cnt - 1 > size1) || (p2 + cnt - 1 > size2) || \
            (p1 <= 0) || (p2 <= 0)) { \
            int *null = 0 ; \
            *null = 1 ; \
            } \
        *(struct _ptoc_TEMPSTRUCT *)(cval2+p2-1) = \
        *(struct _ptoc_TEMPSTRUCT *)(cval1+p1-1); \
        }
#else
#ifdef  __PTCUSECPY_
#define SQLMOVC(size1,size2,val1,p1,val2,p2,cnt) { \
        char *cval1 = (char*) val1 ; \
        char *cval2 = (char*) val2 ; \
        if ((p1 + cnt - 1 > size1) || (p2 + cnt - 1 > size2) || \
            (p1 <= 0) || (p2 <= 0)) { \
            int *null = 0 ; \
            *null = 1 ; \
            } \
        if ( __PTCUSECPY_(cnt) ) \
            __PTCCPY_(cval2+p2-1,cval1+p1-1,cnt); \
        else { \
            struct _ptoc_TEMPSTRUCT { char  space[cnt]; }; \
            *(struct _ptoc_TEMPSTRUCT *)(cval2+p2-1) = \
            *(struct _ptoc_TEMPSTRUCT *)(cval1+p1-1); \
            } \
        }
#else
#define SQLMOVC(size1,size2,val1,p1,val2,p2,cnt) { \
        s10mv(size1,size2,val1,p1,val2,p2,cnt); \
        }
#endif /*__PTCUSECPY_*/
#endif /*BYTE_STRUCT*/

#define _ptoc_MOVE(typ,size,left,right)   __PTCCPY_(left,right,sizeof(typ)*size)
#define _ptoc_MEMCMP(typ,size,left,right) __PTCCMP_(left,right,sizeof(typ)*size)

/*
 *  This is to prevent sign extension.   IST 29.04.92
 */
#if OSF1
#define sql__ord(x) ((unsigned long)(x))
#else
#define sql__ord(x) ((unsigned)(x))
#endif

#define PTOC_CONST(name,value) enum { name = value }

enum { l0_false = 0, l0_true = 1 };
enum { l0_minchar = '\0', l0_maxchar = '\177' };
enum { l0_minint = -2147483647, l0_maxint = 2147483647 };

typedef  struct {
    unsigned char (*_fileptr);
    int _lcount;
    int _llimit;
    int (*_fbuf);
} _iorechd ;

extern int        _argc;
extern int        *_argv;
extern int        *_envp;

#if defined (__OS2__) || defined (_WIN32)

extern _iorechd   sql__input;
extern _iorechd   sql__output;
extern _iorechd   sql__err;
extern _iorechd*  input;
extern _iorechd*  output;

#if defined (__OS2__) && defined (MFSIZE)
int        os2_argc;
char**     os2_argv;
#endif

#else

extern unsigned char sql__input[];
extern unsigned char sql__output[];
extern unsigned char sql__err[];

#define output  sql__output     /* problem in dump_hdg.P   JH*/
#define input   sql__input      /* problem in DSF.P        JB 1992-Jul-10 */

#endif


#if defined (__OS2__) || defined (_WIN32)

int            _System   sql__ic   ( int  , ... );
unsigned char* _System   sql__pcnst( void*, ... );
long*          _System   sql__scnst( void*, ... );

#else

extern long sql__clock();
extern int sql__ic();
extern double sql__rd();
extern long sql__srand();
extern double sql__sin();
extern long sql__trunc();

extern unsigned char *sql__pcnst();
extern long *sql__scnst();
#endif

extern long *sql__setadd();
extern double sql__atan();
extern long sql__ca();
extern double sql__cos();
extern void sql__perrorp();
extern int sql__catchae();
extern double sql__exp();
extern long sql__expp();
extern char *sql__nlf();
extern _iorechd *sql__gn();
extern double sql__ln();
extern long sql__max();
extern long *sql__multi();
extern char *sql__nname();
extern char *sql__nil();
extern long sql__pred();
extern _iorechd *sql__closep();
extern char sql__1r();
extern long sql__intr();
extern long sql__er();
extern double sql__lor();
extern int sql__nre();
extern int sql__erq();
extern int sql__ltl();
extern int sql__lel();
extern int sql__gtl();
extern int sql__ger();
extern long sql__round();
extern long sql__setrang();
extern double sql__sqrt();
extern long *sql__setmin();
extern long sql__succ();
extern int sql__endof();
extern int sql__endol();
extern _iorechd *sql__ut();
extern unsigned char    * sql__pstrcpy();
#if ! defined(__OS2__) && ! defined(_WIN32)
extern long               sql__ckl();
#endif
extern int                sql__compare();
extern int                sql__endof();
extern int                sql__endol();
extern unsigned char    * sql__psetadd();
extern unsigned char    * sql__psetco();
extern int                sql__psetge();
extern int                sql__psetgt();
extern int                sql__psetle();
extern int                sql__psetlt();
extern unsigned char    * sql__psetm();
extern unsigned char    * sql__pstrcat();
extern int                sql__pstrcomp();
extern unsigned char    * sql__setsub();
extern char             * sql__setco();
extern int                sql__ucmp();

#if !defined(__OS2__) && !defined(_WIN32)
#define vbegexcl (*vbegexcl_func_ptr)
#endif

#ifndef NO_SED   /*RAS: for test under WIN95w/o SED : prevent error msgs*/

#define s35add_bufaddr(addr,pos) \
    ((tsp_bufaddr) ((unsigned long) ( ((char *) addr) + (pos) ) ))

#define s35add_moveobj_ptr(addr,pos) \
    ((tsp_moveobj_ptr) ((unsigned long) ( ((char *) addr) + (pos) ) ))

#define s35add_addr1(addr,pos) \
    ((char *) ((unsigned long) ( ((char *) addr) + (pos-1) ) ))

#define s35inc_st(addr,pos) \
    ((tgg_st_entry_addr) ((unsigned long) ( ((tgg_stack_entry *) addr) + (pos) ) ))

#define s35lt_bufaddr(addr1,addr2) \
    ((char *) addr1 < (char *) addr2)

#define s35le_bufaddr(addr1,addr2) \
    ((char *) addr1 <= (char *) addr2)

#define s35gt_bufaddr(addr1,addr2) \
    ((char *) addr1 > (char *) addr2)

#define s35ge_bufaddr(addr1,addr2) \
    ((char *) addr1 >= (char *) addr2)

#define s35op_case(A,B,C,D) \
   (*A) (B,C,D)

#endif		/*#ifndef NO_SED: RAS for test*/

/*-  stuff inserted by RAS ---------------------------------------------*/

/* macros to construct a set constant of 1/2/4 bytes as a constant integer
 * expression depending on the hardware's byte ordering. The sets must be
 * compatibel with function sql_pcnst() etc. */

/* for all byte orderings: */

#define _ptoc_ISET_X1(b1) \
    ( b1 )

/* for Intel 386/486/...: Least Signif. Byte first */
#if I386 || _X86_

#define _ptoc_ISET_X2(b1,b2) \
    ( ( (b2) << 8 ) | (b1) )

#define _ptoc_ISET_X4(b1,b2,b3,b4) \
    ( ( (b4) << 24 ) | ( (b3) << 16 ) | ( (b2) << 8 ) | (b1) )

/* for HP and ???: Most Signif. Byte first */
#else
#if HP9

#define _ptoc_ISET_X2(b1,b2) \
    ( ( (b1) << 8 ) | (b2) )

#define _ptoc_ISET_X4(b1,b2,b3,b4) \
    ( ( (b1) << 24 ) | ( (b2) << 16 ) | ( (b3) << 8 ) | (b4) )

#else

#error Unknown Machine ==> Unknown Byte Ordering!
#error Set Construction Macros _ptoc_ISET_X<n> Now Undefined!

#endif
#endif

/* macros to construct a set of 1/2/3 var. bits */

#define LG2BITSBYTE     03 /*copied from sql__...*/
#define MSKBITSBYTE     07

#define _ptoc_ISET_1B(res,lo,hi,bit) \
    ( (res) = (bit) > (hi) ? 0 : (bit) < (lo) ? 0 : \
    ( ( (char *) ( & (res) ) ) [ (int) ((bit)-(lo)) >> LG2BITSBYTE ] = \
    (char) 1 << ( (int) ((bit)-(lo)) & MSKBITSBYTE ), (res) ) )

#define _ptoc_ISET_CONS1(res,lo,hi,dmy1,dmy2,b1) \
    ( (res) = 0, _ptoc_ISET_1B(res,lo,hi,b1), &(res) )

#define _ptoc_ISET_CONS2(res,lo,hi,dmy1,dmy2,b1,b2) \
    ( (res) = 0, _ptoc_ISET_1B(res,lo,hi,b1), \
      _ptoc_ISET_1B(res,lo,hi,b2), &(res) )

#define _ptoc_ISET_CONS3(res,lo,hi,dmy1,dmy2,b1,b2,b3) \
    ( (res) = 0, _ptoc_ISET_1B(res,lo,hi,b1), \
      _ptoc_ISET_1B(res,lo,hi,b2), \
      _ptoc_ISET_1B(res,lo,hi,b3), &(res) )

/* macros to combine sets */

#define _ptoc_ISET_ADD(res,a,b,dmy) \
    ( (res) = (a) | (b), &(res) )

#define _ptoc_ISET_SUB(res,a,b,dmy) \
    ( (res) = (a) & ( ~ (b) ), &(res) ) /*???ok*/

#define _ptoc_ISET_MUL(res,a,b,dmy) \
    ( (res) = (a) & (b), &(res) )

/* macros to compare sets */

#define _ptoc_ISET_EQL(dmy,a,b) \
    ( (a) == (b) )

#define _ptoc_ISET_NEQ(dmy,a,b) \
    ( (a) != (b) )

#define _ptoc_ISET_GEQ(dmy,a,b) \
    ( ! ( (b) & ~ (a) ) )

#define _ptoc_ISET_LEQ(dmy,a,b) \
    ( ! ( (a) & ~ (b) ) )

#define _ptoc_ISET_GTR(dmy,a,b) \
    (_ptoc_ISET_GEQ(dmy,a,b) & _ptoc_ISET_NEQ(dmy,a,b))

/*---------------------------------------------------------------------------*/

/* macros to compare short strings witthout using memcmp().
 *  _ptoc_CMP2W (for EQ, NEQ) and _ptoc_CMP3W (for LT, GT...) compare
 * bytes 1..n-1, calling parm. more if equal, which is just the next macro call.
 * _CMP1 compares the last byte, */

#define _ptoc_CMP1(a,b,ix,opT,dmy)	   \
    ((unsigned char)((a)[ix]) opT (unsigned char)((b)[ix]))

#define _ptoc_CMP2W(a,b,ix,opT,neq_res,more) \
    ((unsigned char)((a)[ix]) != (unsigned char)((b)[ix]) ? neq_res : (more))

#define _ptoc_CMP3W(a,b,ix,opT,dmy,more) \
    ((unsigned char)((a)[ix]) != (unsigned char)((b)[ix]) ? _ptoc_CMP1(a,b,ix,opT,dmy) : (more))

/*--- RAS: stuff added by thomas anhaus ?! ------------------------------------*/

#ifdef G10_INL_TMP

#define G10MVC(mod_id, mod_intern_num, size1, size2, val1, p1, val2, p2, cnt, e) \
{tsp_int4 ptoc_var_p1, ptoc_var_p2, ptoc_var_cnt;ptoc_var_p1=p1-1;ptoc_var_p2=p2-1;ptoc_var_cnt=cnt;\
 if (*(tgg_basis_error*) e != (tgg_basis_error) -9111) { \
      if ((tsp_int4) (size1) < ptoc_var_p1 + ptoc_var_cnt || \
	  (tsp_int4) (size2) < ptoc_var_p2 + ptoc_var_cnt || \
	  ptoc_var_cnt < 0 || ptoc_var_p1 < 0 || ptoc_var_p2 < 0) { \
               g10mv_err_handling (mod_id, mod_intern_num, size1, size2, \
                     ptoc_var_p1, ptoc_var_p2, ptoc_var_cnt, e); \
	} \
      else \
	memcpy ((unsigned char*) val2 + (tsp_int4) ptoc_var_p2, \
	   (unsigned char*) val1 + (tsp_int4) ptoc_var_p1, (tsp_int4) ptoc_var_cnt);\
   }}

#define G10FIL(mod_id, mod_intern_num, size, m, pos, len, fillchar, e) \
if (*(tgg_basis_error*) e != -9111) {\
      tsp_int4 ptoc_var_pos, ptoc_var_len;\
      ptoc_var_pos = pos - 1;\
      ptoc_var_len = len;\
      if ((tsp_int4) (size) < ptoc_var_pos + ptoc_var_len || \
	  ptoc_var_pos < 0 || ptoc_var_len < 0) \
            g10fil_err_handling (mod_id, mod_intern_num, size, \
		ptoc_var_pos, ptoc_var_len, e);\
      else \
         memset ( (unsigned char*) m + (tsp_int4) ptoc_var_pos, \
		(unsigned char)fillchar, (tsp_int4) ptoc_var_len); }\

#else
#ifdef G10_INL_NOTMP

#error G10MVC, G10FIL w/o tmp.vars missing

#else
#ifdef G10_INL_NONE

extern int g10mv1();
#define G10MVC g10mv1

extern int g10fil1();
#define G10FIL g10fil1

#else
#error no G10_INL_... defined

#endif
#endif
#endif
#endif

