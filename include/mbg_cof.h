
/**************************************************************************
 *
 *  $Id: mbg_cof.h 1.2 2017/07/05 14:25:12 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Container macros (see Linux Kernel)
 *
 * -----------------------------------------------------------------------
 *  $Log: mbg_cof.h $
 *  Revision 1.2  2017/07/05 14:25:12  martin
 *  Reformatted code to conform to standard header file format.
 *  Changes tfor improved cross-platform compatibility.
 *  Revision 1.1  2015/09/09 10:42:27  martin
 *  Initial revision by philipp.
 *
 **************************************************************************/

#ifndef _MBG_COF_H
#define _MBG_COF_H

/* Other headers to be included */

#include <mbg_tgt.h>

#if !defined( MBG_TGT_KERNEL )
  #include <stddef.h>   // for offsetof()
#endif


#ifdef _MBG_COF
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


#if defined( MBG_TGT_POSIX )

  // A special construct supported by gcc/clang and and implemented
  // e.g. in the Linux kernel, which is said to be very type-safe:
  #define mbg_container_of( _ptr, _type, _member ) ({          \
    const typeof( ((_type *)0)->_member ) *__mptr = (_ptr);    \
    (_type *)((char *)__mptr - offsetof(_type,_member));})

#else

  // A different implementation in ANSI C, which supports type-checking anyway:
  #define mbg_container_of( _ptr, _type, _member ) \
    ( (_type *)( (char *)(1 ? (_ptr) : &((_type *)0)->_member) - offsetof( _type, _member )))

#endif


/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

/* (no header definitions found) */

/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif

/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _MBG_COF_H */
