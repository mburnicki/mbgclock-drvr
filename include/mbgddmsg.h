
/**************************************************************************
 *
 *  $Id: mbgddmsg.h 1.15 2018/11/22 14:17:15 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions and function prototypes to deal with messages
 *    generated by kernel mode drivers. Some type of messages are
 *    always generated, others are only generated ibn DEBUG builds
 *    and are defined void in release builds.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgddmsg.h $
 *  Revision 1.15  2018/11/22 14:17:15  martin
 *  Support QNX target.
 *  Make sure DEBUG_MSG_SLEEP is always defined but evaluates
 *  to 'true' or 'false'.
 *  Debug logging functions now expect a flag that controls if
 *  a log message is to be generated, and a severity code
 *  (error, warning, info, ...).
 *  Revision 1.14  2018/09/21 14:55:14  martin
 *  Updated mbg_kdd_vsnprintf() for some targets.
 *  Removed obsolete debug symbols.
 *  Revision 1.13  2018/06/25 12:13:49  martin
 *  Unified implementation of kernel driver messages.
 *  Support MBG_TGT_NO_TGT.
 *  Conditionally support DEBUG_MSG_SLEEP.
 *  Improved support for most target systems.
 *  Added inline function mbg_kdd_vsnprintf() for Linux and Windows.
 *  New Meinberg-specific debug messages.
 *  mbg_kdd_msg() now also takes a log level.
 *  Moved some debug message macros here.
 *  Added _mbgddmsg_8().
 *  Revision 1.12  2017/08/10 13:52:39  martin
 *  Distinguish targets in more detail.
 *  Revision 1.11  2017/07/05 14:31:23  martin
 *  Added _mbg_kdd_msg...() macros.
 *  Code cleanup.
 *  Updated function prototypes.
 *  Revision 1.10  2012/10/02 18:33:21Z  martin
 *  Support for *BSD.
 *  Also enable debug msgs if MBG_DEBUG is defined.
 *  Revision 1.8  2009/04/22 09:54:55  martin
 *  Include mbg_tgt.h also if building without DEBUG.
 *  Revision 1.7  2009/03/19 15:22:54  martin
 *  Cleaned up debug levels.
 *  Revision 1.6  2008/12/05 13:31:47  martin
 *  Use do {} while (0) syntax to avoid potential syntax problems.
 *  Added _mbgddmsg_7().
 *  Revision 1.5  2006/06/19 15:26:19  martin
 *  Fixed compiler warnings if DEBUG or DBG not defined.
 *  Revision 1.4  2002/06/12 12:25:54  martin
 *  Bug fix: check for target MBG_TGT_WIN32 instead of MBG_TGT_W32.
 *  Revision 1.3  2002/02/19 14:50:48Z  MARTIN
 *  Added support for Win32.
 *  Revision 1.2  2002/02/19 09:28:00  MARTIN
 *  Use new header mbg_tgt.h to check the target environment.
 *  Revision 1.1  2001/03/02 13:51:23  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#ifndef _MBGDDMSG_H
#define _MBGDDMSG_H


/* Other headers to be included */

#include <mbg_tgt.h>

#if defined( MBG_TGT_NETWARE )
  #include <conio.h>
#elif defined( MBG_TGT_OS2 )
  #include <iprintf.h>
#elif defined( MBG_TGT_WIN32 )
  #include <ntddk.h>
  #include <stdio.h>
  #include <stdarg.h>
#elif defined( MBG_TGT_LINUX )
  #include <linux/module.h>
  #include <linux/version.h>
#elif defined( MBG_TGT_FREEBSD )
  #include <sys/types.h>
  #include <sys/systm.h>
  #include <machine/stdarg.h>
#elif defined( MBG_TGT_NETBSD )
  #include <sys/param.h>         // for __NetBSD_Version__, __NetBSD_Prereq__
  #if __NetBSD_Prereq__(7,0,0)   // NetBSD >= 7.0
    #include <sys/stdarg.h>      // works at least for NetBSD 7.1
  #else
    #include <machine/stdarg.h>  // works at least for NetBSD 5.1
  #endif
#elif defined( MBG_TGT_QNX )
  #include <stdio.h>
#elif defined( MBG_TGT_DOS )
  #include <stdio.h>
  #include <stdarg.h>
#endif

#include <str_util.h>


#ifdef _MBGDDMSG
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#if defined( _USE_PACK )
  #pragma pack( 1 )      // set byte alignment
  #define _USING_BYTE_ALIGNMENT
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined( DEBUG_MSG_SLEEP )
  #define DEBUG_MSG_SLEEP  0
#endif

#if DEBUG_MSG_SLEEP
  _ext int debug_msg_sleep
  #ifdef _DO_INIT
    = DEBUG_MSG_SLEEP
  #endif
  ;
#endif


#if defined( MBG_TGT_WIN32 )

  #define USE_MBG_KDD_MSG    1

#elif defined( MBG_TGT_LINUX )

    #define USE_MBG_KDD_MSG  1

#elif defined( MBG_TGT_FREEBSD )

  #if defined( _VA_LIST_DECLARED )  // at least since FreeBSD 8.2

    #define USE_MBG_KDD_MSG  1

  #else

    #define _printf printf
    #define _hd
    #define _tl "\n"

  #endif

#elif defined( MBG_TGT_NETBSD )

  #if defined( __VA_LIST_DECLARED )  // at least NetBSD 7.1

    #define USE_MBG_KDD_MSG  1
    #define va_list __va_list

  #elif defined( _BSD_VA_LIST_ )

    #define USE_MBG_KDD_MSG  1

  #else

    #define _printf printf
    #define _hd
    #define _tl "\n"

  #endif

#elif defined( MBG_TGT_NETWARE )

  #define _printf ConsolePrintf
  #define _hd
  #define _tl "\n"

#elif defined( MBG_TGT_OS2 )

  #define _printf iprintf
  #define _hd
  #define _tl "\n"

#elif defined( MBG_TGT_QNX )  // at least QNX 6.x (Neutrino)

  #define USE_MBG_KDD_MSG  1

#elif defined( MBG_TGT_DOS )

  #define USE_MBG_KDD_MSG  1

#else  // other targets ...

  #if !defined( _printf )
    #define _printf printf
  #endif

  #if !defined( _hd )
    #define _hd
  #endif

  #if !defined( _tl )
    #define _tl "\n"
  #endif

#endif


#if !defined( USE_MBG_KDD_MSG )
  #define USE_MBG_KDD_MSG  0
#endif


enum MBG_LOG_LEVELS
{
  MBG_LOG_ERR,
  MBG_LOG_WARN,
  MBG_LOG_NOTICE,
  MBG_LOG_INFO,
  MBG_LOG_DEBUG,
  N_MBG_LOG_LVL
};


#if USE_MBG_KDD_MSG

  #define _mbg_kdd_msg_0( _lvl, _fmt ) \
    mbg_kdd_msg( _lvl, _fmt )

  #define _mbg_kdd_msg_1( _lvl, _fmt, _p1 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1) )

  #define _mbg_kdd_msg_2( _lvl, _fmt, _p1, _p2 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1), (_p2) )

  #define _mbg_kdd_msg_3( _lvl, _fmt, _p1, _p2, _p3 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1), (_p2), (_p3) )

  #define _mbg_kdd_msg_4( _lvl, _fmt, _p1, _p2, _p3, _p4 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4) )

  #define _mbg_kdd_msg_5( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5) )

  #define _mbg_kdd_msg_6( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6) )

  #define _mbg_kdd_msg_7( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6), (_p7) )

  #define _mbg_kdd_msg_8( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8 ) \
    mbg_kdd_msg( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6), (_p7), (_p8) )

#else

  #define _mbg_kdd_msg_0( _lvl, _fmt ) \
  do {                           \
    _printf( _hd _fmt _tl );     \
  } while ( 0 )

  #define _mbg_kdd_msg_1( _lvl, _fmt, _p1 ) \
  do {                                \
    _printf( _hd _fmt _tl, (_p1) );   \
  } while ( 0 )

  #define _mbg_kdd_msg_2( _lvl, _fmt, _p1, _p2 ) \
  do {                                     \
    _printf( _hd _fmt _tl, (_p1), (_p2) ); \
  } while ( 0 )

  #define _mbg_kdd_msg_3( _lvl, _fmt, _p1, _p2, _p3 )   \
  do {                                            \
    _printf( _hd _fmt _tl, (_p1), (_p2), (_p3) ); \
  } while ( 0 )

  #define _mbg_kdd_msg_4( _lvl, _fmt, _p1, _p2, _p3, _p4 )     \
  do {                                                   \
    _printf( _hd _fmt _tl, (_p1), (_p2), (_p3), (_p4) ); \
  } while ( 0 )

  #define _mbg_kdd_msg_5( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5 )       \
  do {                                                          \
    _printf( _hd _fmt _tl, (_p1), (_p2), (_p3), (_p4), (_p5) ); \
  } while ( 0 )

  #define _mbg_kdd_msg_6( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6 )         \
  do {                                                                 \
    _printf( _hd _fmt _tl, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6) ); \
  } while ( 0 )

  #define _mbg_kdd_msg_7( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7 )           \
  do {                                                                        \
    _printf( _hd _fmt _tl, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6), (_p7) ); \
  } while ( 0 )

  #define _mbg_kdd_msg_8( _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8 )             \
  do {                                                                               \
    _printf( _hd _fmt _tl, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6), (_p7), (_p8) ); \
  } while ( 0 )

#endif


#if defined( DEBUG ) || defined( MBG_DEBUG ) || ( defined( DBG ) && DBG )

#define _mbgddmsg_0( _f, _lvl, _fmt ) \
do {                                   \
  if ( (_f) )                          \
    { _mbg_kdd_msg_0( _lvl, _fmt ); }  \
} while ( 0 )

#define _mbgddmsg_1( _f, _lvl, _fmt, _p1 )  \
do {                                         \
  if ( (_f) )                                \
    { _mbg_kdd_msg_1( _lvl, _fmt, (_p1) ); } \
} while ( 0 )

#define _mbgddmsg_2( _f, _lvl, _fmt, _p1, _p2 )    \
do {                                                \
  if ( (_f) )                                       \
    { _mbg_kdd_msg_2( _lvl, _fmt, (_p1), (_p2) ); } \
} while ( 0 )

#define _mbgddmsg_3( _f, _lvl, _fmt, _p1, _p2, _p3 )      \
do {                                                       \
  if ( (_f) )                                              \
    { _mbg_kdd_msg_3( _lvl, _fmt, (_p1), (_p2), (_p3) ); } \
} while ( 0 )

#define _mbgddmsg_4( _f, _lvl, _fmt, _p1, _p2, _p3, _p4 )        \
do {                                                              \
  if ( (_f) )                                                     \
    { _mbg_kdd_msg_4( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4) ); } \
} while ( 0 )

#define _mbgddmsg_5( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5 )          \
do {                                                                     \
  if ( (_f) )                                                            \
    { _mbg_kdd_msg_5( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5) ); } \
} while ( 0 )

#define _mbgddmsg_6( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6 )            \
do {                                                                            \
  if ( (_f) )                                                                   \
    { _mbg_kdd_msg_6( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6) ); } \
} while ( 0 )

#define _mbgddmsg_7( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7 )                  \
do {                                                                                       \
  if ( (_f) )                                                                              \
    { _mbg_kdd_msg_7( _f, _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6), (_p7) ); } \
} while ( 0 )

#define _mbgddmsg_8( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8 )                \
do {                                                                                          \
  if ( (_f) )                                                                                 \
    { _mbg_kdd_msg_8( _lvl, _fmt, (_p1), (_p2), (_p3), (_p4), (_p5), (_p6), (_p7), (_p8) ); } \
} while ( 0 )



#else

  #define _mbgddmsg_0( _f, _lvl, _fmt )                                          _nop_macro_fnc()
  #define _mbgddmsg_1( _f, _lvl, _fmt, _p1 )                                     _nop_macro_fnc()
  #define _mbgddmsg_2( _f, _lvl, _fmt, _p1, _p2 )                                _nop_macro_fnc()
  #define _mbgddmsg_3( _f, _lvl, _fmt, _p1, _p2, _p3 )                           _nop_macro_fnc()
  #define _mbgddmsg_4( _f, _lvl, _fmt, _p1, _p2, _p3, _p4 )                      _nop_macro_fnc()
  #define _mbgddmsg_5( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5 )                 _nop_macro_fnc()
  #define _mbgddmsg_6( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6 )            _nop_macro_fnc()
  #define _mbgddmsg_7( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7 )       _nop_macro_fnc()
  #define _mbgddmsg_8( _f, _lvl, _fmt, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8 )  _nop_macro_fnc()

#endif



#define MBG_LOG_FMT_ENTERING         "Entering \"%s\""
#define MBG_LOG_FMT_ENTERING_STR     MBG_LOG_FMT_ENTERING " %s"

#define MBG_LOG_FMT_LEAVING          "Leaving \"%s\""
#define MBG_LOG_FMT_LEAVING_STR      MBG_LOG_FMT_LEAVING " %s"
#define MBG_LOG_FMT_LEAVING_SUCCESS  MBG_LOG_FMT_LEAVING " success"
#define MBG_LOG_FMT_LEAVING_ERR      MBG_LOG_FMT_LEAVING " err"
#define MBG_LOG_FMT_LEAVING_ERR_MBG  MBG_LOG_FMT_LEAVING " err %i: %s"
#define MBG_LOG_FMT_LEAVING_ERR_DEC  MBG_LOG_FMT_LEAVING " err %li"
#define MBG_LOG_FMT_LEAVING_ERR_HEX  MBG_LOG_FMT_LEAVING " err 0x%08lX"



#if USE_MBG_KDD_MSG && defined( DEBUG ) && DEBUG

  #define _mbgddmsg_fnc_entry()                       \
    mbg_kdd_msg( MBG_LOG_DEBUG, MBG_LOG_FMT_ENTERING, \
                 __func__ )

  #define _mbgddmsg_fnc_entry_str( _s )                   \
    mbg_kdd_msg( MBG_LOG_DEBUG, MBG_LOG_FMT_ENTERING_STR, \
                 __func__, (_s) )

  #define _mbgddmsg_fnc_exit()                       \
    mbg_kdd_msg( MBG_LOG_DEBUG, MBG_LOG_FMT_LEAVING, \
                 __func__ )

  #define _mbgddmsg_fnc_exit_str( _s )                   \
    mbg_kdd_msg( MBG_LOG_DEBUG, MBG_LOG_FMT_LEAVING_STR, \
                 __func__, (_s) )

  #define _mbgddmsg_fnc_exit_success()                       \
    mbg_kdd_msg( MBG_LOG_DEBUG, MBG_LOG_FMT_LEAVING_SUCCESS, \
                 __func__ )

  #define _mbgddmsg_fnc_exit_err()                            \
    mbg_kdd_msg( MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR,        \
                 __func__ )


  #define _mbgddmsg_fnc_exit_err_mbg( _mbg_errno )                \
    mbg_kdd_msg( MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR_MBG,        \
                 __func__, _mbg_errno, mbg_strerror( _mbg_errno ) )

  #define _mbgddmsg_fnc_exit_err_dec( _errnum )               \
    mbg_kdd_msg( MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR_DEC,    \
                 __func__, (long) (_errnum) )

  #define _mbgddmsg_fnc_exit_err_hex( _errnum )                \
    mbg_kdd_msg( MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR_HEX,     \
                 __func__, (ulong) (_errnum) )


  #define _mbgddmsg_fnc_exit_chk_mbg_rc( _rc ) \
    do                                         \
    {                                          \
      if ( mbg_rc_is_success( _rc ) )          \
        _mbgddmsg_fnc_exit_success();          \
      else                                     \
        _mbgddmsg_fnc_exit_err_mbg( _rc );     \
    } while ( 0 )

  #define _mbgddmsg_fnc_exit_chk_rc( _rc ) \
    do                                     \
    {                                      \
      if ( mbg_rc_is_success( _rc ) )      \
        _mbgddmsg_fnc_exit_success();      \
      else                                 \
        _mbgddmsg_fnc_exit_err_dec( _rc ); \
    } while ( 0 )

  #if defined( MBG_TGT_WIN32 )

    #define _mbgddmsg_fnc_exit_chk_st( _st ) \
      do                                     \
      {                                      \
        if ( NT_SUCCESS( (_st) ) )           \
          _mbgddmsg_fnc_exit_success();      \
        else                                 \
          _mbgddmsg_fnc_exit_err_hex( _st ); \
      } while ( 0 )

  #endif

#else

  // Building with or without debug is handled
  // via the definition of the _mbgddmsg_* macros.

  #define _mbgddmsg_fnc_entry()                              \
    _mbgddmsg_1( DEBUG, MBG_LOG_DEBUG, MBG_LOG_FMT_ENTERING, \
                 __func__ )

  #define _mbgddmsg_fnc_entry_str( _s )                          \
    _mbgddmsg_2( DEBUG, MBG_LOG_DEBUG, MBG_LOG_FMT_ENTERING_STR, \
                 __func__, (_s) )

  #define _mbgddmsg_fnc_exit()                              \
    _mbgddmsg_1( DEBUG, MBG_LOG_DEBUG, MBG_LOG_FMT_LEAVING, \
                 __func__ )

  #define _mbgddmsg_fnc_exit_str( _s )                          \
    _mbgddmsg_2( DEBUG, MBG_LOG_DEBUG, MBG_LOG_FMT_LEAVING_STR, \
                 __func__, (_s) )

  #define _mbgddmsg_fnc_exit_success()                              \
    _mbgddmsg_1( DEBUG, MBG_LOG_DEBUG, MBG_LOG_FMT_LEAVING_SUCCESS, \
                 __func__ )

  #define _mbgddmsg_fnc_exit_err()                                \
    _mbgddmsg_1( DEBUG, MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR_DEC, \
                 __func__ )

  #define _mbgddmsg_fnc_exit_err_mbg( _mbg_errno )                \
    _mbgddmsg_3( DEBUG, MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR_MBG, \
                 __func__, _mbg_errno, mbg_strerror( _mbg_errno ) )

  #define _mbgddmsg_fnc_exit_err_dec( _errnum )                   \
    _mbgddmsg_2( DEBUG, MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR_DEC, \
                 __func__, (long) (_errnum) )

  #define _mbgddmsg_fnc_exit_err_hex( _errnum )                   \
    _mbgddmsg_2( DEBUG, MBG_LOG_ERR, MBG_LOG_FMT_LEAVING_ERR_HEX, \
                 __func__, (ulong) (_errnum) )

  #define _mbgddmsg_fnc_exit_chk_mbg_rc( _rc ) \
    _nop_macro_fnc()  // TODO

  #define _mbgddmsg_fnc_exit_chk_rc( _rc ) \
    _nop_macro_fnc()  // TODO

  #define _mbgddmsg_fnc_exit_chk_st( _st ) \
    _nop_macro_fnc()  // TODO

#endif



#if USE_MBG_KDD_MSG

static __mbg_inline /*HDR*/
__attribute__( ( format( printf, 3, 0 ) ) )
int mbg_kdd_vsnprintf( char *s, size_t max_len, const char *fmt, va_list args )
{
  size_t n;

  if ( !mbg_buffer_specs_valid( s, max_len ) )
    return 0;     // nothing to do anyway

  #if defined( MBG_TGT_WIN32 )

    n = _vsnprintf( s, max_len, fmt, args );

  #elif defined( MBG_TGT_LINUX )

    n = vscnprintf( s, max_len, fmt, args );

  #elif defined( MBG_TGT_BSD )

    n = vsnprintf( s, max_len, fmt, args );

  #elif defined( MBG_TGT_QNX )

    n = vsnprintf_safe( s, max_len, fmt, args );

  #elif defined( MBG_TGT_DOS )

    n = vsnprintf_safe( s, max_len, fmt, args );

  #elif defined( MBG_TGT_NO_TGT )

    n = vsnprintf_no_tgt( s, max_len, fmt, args );

  #else

    #error mbg_kdd_vsnprintf() needs to be implemented for this target.

  #endif

  // Do some common checks to avoid subsequent buffer overflows, etc.
  return mbg_chk_snprint_results( n, s, max_len );

}  // mbg_kdd_vsnprintf

#endif  // USE_MBG_KDD_MSG



/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 __attribute__( ( format( printf, 3, 4 ) ) ) int mbg_kdd_snprintf( char *buf, size_t size, const char *fmt, ... ) ;
 __attribute__( ( format( printf, 2, 3 ) ) ) void mbg_kdd_msg( int lvl, const char *fmt, ... ) ;

/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif


#if defined( _USING_BYTE_ALIGNMENT )
  #pragma pack()      // set default alignment
  #undef _USING_BYTE_ALIGNMENT
#endif

/* End of header body */


#undef _ext
#undef _DO_INIT

#endif  /* _MBGDDMSG_H */
