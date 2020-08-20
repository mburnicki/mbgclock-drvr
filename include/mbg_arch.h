
/**************************************************************************
 *
 *  $Id: mbg_arch.h 1.6 2017/01/27 09:03:16 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions to support different computer hardware architectures.
 *
 *  For a good summary of predefined macros which can be used to determine
 *  the build environment, the target environment, and architecture, see:
 *  http://sourceforge.net/p/predef/wiki/Home/
 *
 * -----------------------------------------------------------------------
 *  $Log: mbg_arch.h $
 *  Revision 1.6  2017/01/27 09:03:16  martin
 *  Added macros _mbg_swab8() and _mbg_swab64().
 *  Fixed macro syntax.
 *  Modified _swab_dummy() to avoid compiler warnings due to unused variables.
 *  Revision 1.5  2014/03/11 16:01:55  martin
 *  Added a comment.
 *  Revision 1.4  2012/10/02 18:32:00  martin
 *  Include words.h and, conditionally, stdlib.h.
 *  Use generic preprocessor symbol MBG_TGT_KERNEL.
 *  Revision 1.3  2009/06/12 13:12:37Z  martin
 *  Fixed compiler warning.
 *  Revision 1.2  2009/03/19 15:14:15  martin
 *  Fixed byte swapping of doubles for SPARC architecture.
 *  Revision 1.1  2008/12/05 13:47:42  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _MBG_ARCH_H
#define _MBG_ARCH_H

#include <mbg_tgt.h>
#include <words.h>

#if !defined( MBG_TGT_KERNEL )
  #include <stdlib.h>
#endif


#if defined( MBG_ARCH_SPARC )
  #define MBG_ARCH_BIG_ENDIAN      1
#endif


#if !defined( MBG_ARCH_BIG_ENDIAN )
  #define MBG_ARCH_LITTLE_ENDIAN   1
#endif



#if defined( MBG_TGT_LINUX )

  #include <asm/byteorder.h>

  #if defined( MBG_TGT_KERNEL )
    #include <asm/unaligned.h>

    #define _mbg_put_unaligned( _v, _p )  put_unaligned( _v, _p )
    #define _mbg_get_unaligned( _p )      get_unaligned( _p )
  #endif

#endif



// If no macros required to access unaligned data have yet been defined,
// define some default macros assuming no special handling is required
// to access unaligned data.

#if !defined( _mbg_put_unaligned )
  #define _mbg_put_unaligned( _v, _p )   do { ((void)( *(_p) = (_v) )); } while ( 0 )
#endif

#if !defined( _mbg_get_unaligned )
  #define _mbg_get_unaligned( _p )       (*(_p))
#endif



// If no macros to convert endianess have yet been defined, define
// some default macros assuming endianess conversion is not required.

#if !defined( __le16_to_cpu )
  #define __le16_to_cpu( _x )   (_x)
#endif

#if !defined( __le32_to_cpu )
  #define __le32_to_cpu( _x )   (_x)
#endif

#if !defined( __le64_to_cpu )
  #define __le64_to_cpu( _x )   (_x)
#endif

#if !defined( __cpu_to_le16 )
  #define __cpu_to_le16( _x )   (_x)
#endif

#if !defined( __cpu_to_le32 )
  #define __cpu_to_le32( _x )   (_x)
#endif

#if !defined( __cpu_to_le64 )
  #define __cpu_to_le64( _x )   (_x)
#endif



// The macros below are used to convert the endianess
// of the plug-in cards to the endianess of the host CPU

#define _mbg8_to_cpu( _x )   ( _x )
#define _mbg16_to_cpu( _x )  __le16_to_cpu( _x )
#define _mbg32_to_cpu( _x )  __le32_to_cpu( _x )
#define _mbg64_to_cpu( _x )  __le64_to_cpu( _x )

#define _cpu_to_mbg8( _x )   ( _x )
#define _cpu_to_mbg16( _x )  __cpu_to_le16( _x )
#define _cpu_to_mbg32( _x )  __cpu_to_le32( _x )
#define _cpu_to_mbg64( _x )  __cpu_to_le64( _x )



/**
 * @brief Swap a 'double' type variable bytewise e.g. to convert the endianess
 */
static __mbg_inline
void mbg_swab_double( double *p )
{
#if 0  // The __swab64() may not work correctly for whatever reason ...
  __swab64p( p );
#else  // ... so we do the swapping manually
  double d = 0;
  size_t i;

  for ( i = 0; i < sizeof( double); i++ )
    BYTE_OF( d, i ) = BYTE_OF( *p, ( sizeof( double) - 1 - i ) );

  for ( i = 0; i < sizeof( double); i++ )
    BYTE_OF( *p, i ) = BYTE_OF( d, i );
#endif

}  // mbg_swab_double



#if defined( MBG_ARCH_BIG_ENDIAN )

  #define _mbg_swab8( _p )         _nop_macro_fnc()  // always a dummy, but for completeness ...
  #define _mbg_swab16( _p )        do { *(_p) = __swab16( *(_p) ); } while ( 0 )
  #define _mbg_swab32( _p )        do { *(_p) = __swab32( *(_p) ); } while ( 0 )
  #define _mbg_swab64( _p )        do { *(_p) = __swab64( *(_p) ); } while ( 0 )

  #define _mbg_swab_double( _p )   mbg_swab_double( _p )

  #define _mbg_swab_doubles( _p, _n )  \
  do                                   \
  {                                    \
    int i;                             \
    for ( i = 0; i < (_n); i++ )       \
      _mbg_swab_double( &_p[i] );      \
  } while ( 0 )

#else

  #define _mbg_swab8( _p )              _nop_macro_fnc()
  #define _mbg_swab16( _p )             _nop_macro_fnc()
  #define _mbg_swab32( _p )             _nop_macro_fnc()
  #define _mbg_swab64( _p )             _nop_macro_fnc()

  #define _mbg_swab_double( _p )        _nop_macro_fnc()

  #define _mbg_swab_doubles( _p, _n )   _nop_macro_fnc()

#endif




/**
 * @brief A placeholder for yet missing _mbg_swab_..() macros
 *
 * We don't just use the _nop_macro_fnc() macros here to avoid
 * compiler warnings 'unused variable'.
 */
#define _mbg_swab_dummy( _x )           do { (void) _x; } while ( 0 )


#endif  /* _MBG_ARCH_H */
