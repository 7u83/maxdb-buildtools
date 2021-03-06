/* @(#)ptoc.h		2.1.2  2004-01-26  MAXDB
 *
 * 2004-01-26  - linux use memcmp too 
               - s35add_addr1 returns with tsp00_Addr (unsigned char*) instead of char*
               - maxdb as name
 * 1.5.97 1994-Jan-10 J.Bruehe  ALPHA has correct 'memcmp' (unsigned), use it
 * 1.5.95 1993-Nov-11 Bruehe/Hofer  address arithmetic (vsp35 equivalent)
 * 1.5.95 1993-Oct-21 J.Bruehe  Omit the leading 't_' from '_iorechd'

 
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

#if defined (_WIN32)
# include          <stdio.h>
# include          <string.h>
# include          <math.h>
# include          <time.h>
#endif


#if defined (_WIN32)

#pragma warning (disable : 4047 4133 4146 4307 4308 4756 4116)

#if !defined(_System)
#define _System
#endif

#endif

#if defined (_M_IA64)
# define _ptoc_long  __int64
#else
# define _ptoc_long  long
#endif

#if defined(_WIN32)
# define         sql__trunc(value)                  ((_ptoc_long)(value))
# define         sql__sin(value)                    sin(value)
# define         sql__srand(value)                  (_ptoc_long)sql__seed_v;sql__seed_v = (value)
# define         sql__rd()                          ( (double) (rand()) ) / (double) 32767;
# define         sql__clock()                       ( (clock() * 1000) / CLOCKS_PER_SEC )
# define         sql__ckl()                         ( (clock() * 1000) / CLOCKS_PER_SEC )

# define vabort() _vabort( __LINE__, __FILE__)

#else
 long            sql__trunc( double );
 double          sql__sin( double );
 long            sql__srand( long );
 double          sql__rd();
 long            sql__clock();
 long            sql__ckl();
#endif

# define         sql__ucmp                          memcmp


/*
 *  Function to use for memory-to-memory copy.
 */
#define __PTCCPY_ memcpy

/*
 *  Function to use for filling a block of memory with a byte pattern.
 */
#define _ptoc_MEMSET memset

/*
 *  Function to use for unsigned memory-to-memory comparison.
 */
# define __PTCCMP_ memcmp

/*
 *  Condition when to use __PTCCPY_ instead of inline copy.
 *  If not defined, inline copy will not be used.
 *  If BYTE_STRUCT is defined, inline copy will always be used.
 */
#if I386 || X86_64
# define __PTCUSECPY_(x) ((x) > 180)
#else
#if SUN || sun || SPARC
# define __PTCUSECPY_(x) ((x) > 10)
#else
#if HP9 || PA11 || PA20W || HP_IA64
# define __PTCUSECPY_(x) ((x) > 16)
#else
#if OSF1
/* #define __PTCUSECPY_(x,L,R) ((x) > 33 || ((x) > 43 && !L&7 && !R&7)) */
# define __PTCUSECPY_(x) ((x) > 33)
#else
#endif /*OSF1*/
#endif /*HP9*/
#endif /*SUN/SOLARIS/SPARC*/
#endif /*I386/X86_64*/

/*
 *  _ptoc_ASSIGN
 */
#if BYTE_STRUCT
# define _ptoc_ASSIGN(typ,size,left,right) {               \
         struct _ptoc_TEMPSTRUCT { typ space[size]; };     \
         *(struct _ptoc_TEMPSTRUCT *)(left) =              \
         *(struct _ptoc_TEMPSTRUCT *)(right);              \
         }
#else
#ifdef  __PTCUSECPY_
# define _ptoc_ASSIGN(typ,size,left,right) {               \
         if ( __PTCUSECPY_(sizeof(typ)*size) )             \
             __PTCCPY_(left,right,sizeof(typ)*size);       \
         else {                                            \
             struct _ptoc_TEMPSTRUCT { typ space[size]; }; \
             *(struct _ptoc_TEMPSTRUCT *)(left) =          \
             *(struct _ptoc_TEMPSTRUCT *)(right);          \
             }                                             \
         }
#else
# define _ptoc_ASSIGN(typ,size,left,right) {               \
             __PTCCPY_(left,right,sizeof(typ)*size);       \
        }
#endif /*__PTCUSECPY_*/
#endif /*BYTE_STRUCT*/

/*
 *  SQLMOVC generated by comppc for fixed length moves.
 */
#if BYTE_STRUCT
# define SQLMOVC(size1,size2,val1,p1,val2,p2,cnt) {              \
         char *cval1 = (char*) val1 ;                            \
         char *cval2 = (char*) val2 ;                            \
         struct _ptoc_TEMPSTRUCT { char  space[cnt]; };          \
         if ((p1 + cnt - 1 > size1) || (p2 + cnt - 1 > size2) || \
             (p1 <= 0) || (p2 <= 0)) {                           \
             int *null = 0 ;                                     \
             *null = 1 ;                                         \
             }                                                   \
         *(struct _ptoc_TEMPSTRUCT *)(cval2+p2-1) =              \
         *(struct _ptoc_TEMPSTRUCT *)(cval1+p1-1);               \
         }
#else
#ifdef  __PTCUSECPY_
# define SQLMOVC(size1,size2,val1,p1,val2,p2,cnt) {              \
         char *cval1 = (char*) val1 ;                            \
         char *cval2 = (char*) val2 ;                            \
         if ((p1 + cnt - 1 > size1) || (p2 + cnt - 1 > size2) || \
             (p1 <= 0) || (p2 <= 0)) {                           \
             int *null = 0 ;                                     \
             *null = 1 ;                                         \
             }                                                   \
         if ( __PTCUSECPY_(cnt) )                                \
             __PTCCPY_(cval2+p2-1,cval1+p1-1,cnt);               \
         else {                                                  \
             struct _ptoc_TEMPSTRUCT { char  space[cnt]; };      \
             *(struct _ptoc_TEMPSTRUCT *)(cval2+p2-1) =          \
             *(struct _ptoc_TEMPSTRUCT *)(cval1+p1-1);           \
             }                                                   \
         }
#else
#define SQLMOVC(size1,size2,val1,p1,val2,p2,cnt) {               \
        s10mv(size1,size2,val1,p1,val2,p2,cnt);                  \
        }
#endif /*__PTCUSECPY_*/
#endif /*BYTE_STRUCT*/

#define _ptoc_MOVE(typ,size,left,right)   __PTCCPY_(left,right,sizeof(typ)*size)
#define _ptoc_MEMCMP(typ,size,left,right) __PTCCMP_(left,right,sizeof(typ)*size)

/*
 *  This is to prevent sign extension.   IST 29.04.92
 */
#if ( OSF1 || PA20W )
# define sql__ord(x) ((unsigned _ptoc_long)(x))
#else
# define sql__ord(x) ((unsigned)(x))
#endif

#define PTOC_CONST(name,value) enum { name = value }

enum { l0_false = 0, l0_true = 1 };
enum { l0_minchar = '\0', l0_maxchar = '\177' };
enum { l0_minint = -2147483647, l0_maxint = 2147483647 };


typedef enum { _ptoc_false = 0, _ptoc_true = 1 } _ptoc_bool;


typedef  struct { unsigned char (*_fileptr);
                  int           _lcount;
                  int           _llimit;
                  int           (*_fbuf); } _iorechd;

extern int        _argc;
extern int        *_argv;
extern int        *_envp;

#if defined (_WIN32)

 extern _iorechd   sql__input;
 extern _iorechd   sql__output;
 extern _iorechd   sql__err;
 extern _iorechd*  input;
 extern _iorechd*  output;

#else /* UNIX */

 extern unsigned char sql__input[];
 extern unsigned char sql__output[];
 extern unsigned char sql__err[];

# define output  sql__output     /* problem in dump_hdg.P   JH*/
# define input   sql__input      /* problem in DSF.P        JB 1992-Jul-10 */

#endif


#if defined (_WIN32)
 _ptoc_bool      _System   sql__ic      ( int  , ... );  
 unsigned char * _System   sql__pcnst   ( void*, ... );
 long *          _System   sql__scnst   ( void*, ... );
 void                      sql__catchae ( int );

#else
 _ptoc_bool                sql__ic      ( void *,... );

 unsigned char           * sql__pcnst   ( void *,... );

 long                    * sql__scnst   ( void *,... );



# if defined ( _IBMR2 ) || defined ( SVR4 ) || defined ( SOLARIS )
   void                    sql__catchae ( int );
# else
   void                    sql__catchae ( int, int );
# endif
#endif



long                     * sql__setadd  ( long *, long *, long *, long );

double                     sql__atan    ( double );
long                       sql__ca      ( unsigned char, long );
double                     sql__cos     ( double );
void                       sql__perrorp ( char*, long, long );
double                     sql__exp     ( double );
long                       sql__expp    ( double );
char                     * sql__nlf     ( _iorechd * );
_iorechd                 * sql__gn      ( _iorechd *, char*, long, long );

double                     sql__ln      ( double );
long                       sql__max     ( long, long, long );
long                     * sql__multi   ( long, long, long, long );
char                     * sql__nname   ( long, int * );
void                     * sql__nil     ( void *, char *, int );
long                       sql__pred    ( long, long, long, char*, int );
_iorechd                 * sql__closep  ( _iorechd *, _ptoc_bool );
char                       sql__1r      ( _iorechd * );
long                       sql__intr    ( _iorechd * );
long                       sql__er      ( _iorechd *, int );
double                     sql__lor     ( _iorechd * );
_ptoc_bool                 sql__nre     ( long, char *, char * );
_ptoc_bool                 sql__erq     ( long, char *, char * ); 
_ptoc_bool                 sql__ltl     ( long, long *, long * );
_ptoc_bool                 sql__lel     ( long, long *, long * );
_ptoc_bool                 sql__gtl     ( long, long *, long * );
_ptoc_bool                 sql__ger     ( long, long *, long * );
long                       sql__round   ( double );
long                       sql__setrang ( long, long, long );
double                     sql__sqrt    ( double );
long                     * sql__setmin  ( long *, long *, long *, long );
long                       sql__succ    ( long, long, long, char *, int );
_ptoc_bool                 sql__endof   ( _iorechd * );
_ptoc_bool                 sql__endol   ( _iorechd * );
_iorechd                 * sql__ut      ( _iorechd * );
unsigned char            * sql__pstrcpy ( unsigned char *, int, unsigned char *, int, int );
int                        sql__compare ( unsigned char *, unsigned char *, int, int );
unsigned char            * sql__psetadd ( unsigned char *, unsigned char *, unsigned char *, long );
unsigned char            * sql__psetco  ( unsigned char *, int, int, unsigned char *, int, int );
_ptoc_bool                 sql__psetge  ( long, unsigned char *, unsigned char * );
_ptoc_bool                 sql__psetgt  ( long, unsigned char *, unsigned char * );
_ptoc_bool                 sql__psetle  ( long, unsigned char *, unsigned char * );
_ptoc_bool                 sql__psetlt  ( long, unsigned char *, unsigned char * );
unsigned char            * sql__psetm   ( unsigned char *, unsigned char *, unsigned char *, long );
unsigned char            * sql__pstrcat ( unsigned char *, int, unsigned char *, unsigned char * );
int                        sql__pstrcomp( unsigned char *, unsigned char * );
unsigned char            * sql__setsub  ( unsigned char *, unsigned char *, unsigned char *, long );
char                     * sql__setco   ( unsigned char *, int, int, unsigned char *, int, int );

#if !defined(_WIN32)
# define vbegexcl (*vbegexcl_func_ptr)
#endif

#define s35add_bufaddr(addr,pos) \
    ((tsp_bufaddr) ((unsigned _ptoc_long) ( ((char *) addr) + (pos) ) ))

#define s35add_moveobj_ptr(addr,pos) \
    ((tsp_moveobj_ptr) ((unsigned _ptoc_long) ( ((char *) addr) + (pos) ) ))

#define s35add_addr1(addr,pos) \
    ((tsp00_Addr) ((unsigned long) ( ((char *) addr) + (pos-1) ) ))

#define s35inc_st(addr,pos) \
    ((tgg_st_entry_addr) ((unsigned _ptoc_long) ( ((tgg_stack_entry *) addr) + (pos) ) ))

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

