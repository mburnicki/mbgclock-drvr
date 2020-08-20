
/**************************************************************************
 *
 *  $Id: words.h 1.49 2019/10/23 08:31:48 thomas-b REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions of commonly used data types.
 *
 * -----------------------------------------------------------------------
 *  $Log: words.h $
 *  Revision 1.49  2019/10/23 08:31:48  thomas-b
 *  Added struct name for NANO_TIME_64
 *  Revision 1.48  2019/06/17 08:38:59  thomas-b
 *  Renamed structs according to Meinberg naming convention
 *  Revision 1.47  2019/06/06 12:17:14  thomas-b
 *  Added several struct names to allow forward declaration
 *  Revision 1.46  2019/02/07 14:38:56  martin
 *  Removed a duplicate definition.
 *  Revision 1.45  2018/11/22 16:38:15  martin
 *  Removed inclusion of mbg_no_tgt.h which is obsolete here.
 *  Revision 1.44  2018/06/25 09:21:02  martin
 *  Support MBG_TGT_NO_TGT.
 *  Improved STRINGIFY() macro.
 *  Revision 1.43  2018/01/29 10:30:23  martin
 *  Modified some comments.
 *  Revision 1.42  2017/07/26 14:28:50Z  martin
 *  Fixed build for NetBSD.
 *  Revision 1.41  2017/07/05 12:06:35  martin
 *  Moved macro _int_from_size_t() here.
 *  Revision 1.40  2017/06/12 11:14:25  martin
 *  Empty _DEPRECATED_BY definition for firmware targets.
 *  Revision 1.39  2017/03/15 10:01:09  martin
 *  Added comments how to represent negative numbers in NANO_TIME
 *  and NANO_TIME_64 structures.
 *  Added macros _nano_time_zero() and _nano_time_64_zero().
 *  Revision 1.38  2017/02/22 11:56:33  martin
 *  Made  MBG_CODE_NAME_TABLE_ENTRY::code signed to
 *  avoid signed/unsigned warnings with some code tables.
 *  Revision 1.37  2017/01/27 12:24:35Z  martin
 *  Moved STRINGIFY() macro here.
 *  Revision 1.36  2017/01/27 08:59:43  martin
 *  Fixed macro syntax.
 *  Revision 1.35  2016/08/05 12:17:21  martin
 *  Moved definitions for NANO_TIME and NANO_TIME_64 here.
 *  New macro _nano_time_64_negative().
 *  Conditionally define _abs64() macro.
 *  Include <inttypes.h> for Keil ARMCC target.
 *  Added some conditional debugging code.
 *  Fixed some spelling.
 *  Revision 1.34  2014/10/20 12:31:20  martin
 *  Moved macro _isdigit() here.
 *  Revision 1.33  2014/05/27 10:18:35Z  martin
 *  Finer control of which types are required for or already
 *  available on particular target systems.
 *  Added macros helpful to simplify declarations of code/name tables.
 *  Revision 1.32  2014/01/07 15:43:52  martin
 *  Define __mbg_inline for ARM firmware targets.
 *  Revision 1.31  2012/11/29 11:54:39Z  martin
 *  Removed #if sizeof() definitions which may cause build errors
 *  with some older compilers.
 *  Include stdbool.h for __ARMCC_VERSION targets.
 *  Moved _nop_macro_fnc() definition here.
 *  Revision 1.30  2012/11/02 09:12:29Z  martin
 *  Moved most feature detection code to mbg_tgt.h.
 *  Tried to define missing features most flexibly and portably.
 *  Revision 1.29  2012/07/11 16:45:45Z  martin
 *  New macros to access individual bytes of long constants.
 *  Revision 1.28  2012/04/05 14:36:18Z  martin
 *  Support CVI 2010 compiler which provides C99 types.
 *  Revision 1.27  2011/07/18 10:21:38Z  martin
 *  Added definition for MBG_CODE_NAME_TABLE_ENTRY which can
 *  be used to define tables assigning strings to numeric codes.
 *  Revision 1.26  2011/04/06 10:23:03  martin
 *  Added FBYTE_OF() and FWORD_OF() macros.
 *  Modifications required for *BSD.
 *  Revision 1.25  2010/11/17 10:23:09  martin
 *  Define _BIT_REDEFINED if bit type is redefined.
 *  Revision 1.24  2010/11/17 08:44:56Z  martin
 *  If supported, use type "bool" to implement "bit".
 *  Revision 1.23  2010/05/27 08:54:30Z  martin
 *  Support fixed size data types with Keil RealView compiler for ARM.
 *  Keil RealView ARM targets are always considered as firmware.
 *  Revision 1.22  2009/10/21 07:53:55  martin
 *  Undid changes introduced in 1.21 since they were not consistent
 *  across glibc and/or Linux kernel header versions.
 *  Revision 1.21  2009/10/01 14:00:17  martin
 *  Conditionally define ulong and friends also for Linux/glibc.
 *  Revision 1.20  2009/07/02 15:38:12  martin
 *  Added new macro _wswap32().
 *  Revision 1.19  2009/04/14 14:45:45Z  martin
 *  Added BYTE_OF_P() and WORD_OF_P() macros.
 *  Revision 1.18  2009/03/27 14:05:18  martin
 *  Cleanup for CVI.
 *  Revision 1.17  2009/03/13 09:06:03Z  martin
 *  Declared bit type for non-firmware environments.
 *  Revision 1.16  2008/12/05 12:05:41Z  martin
 *  Define dummy int64_t/uint64_t types for targets
 *  which don't support 64 bit data types.
 *  Revision 1.15  2008/07/14 14:44:00Z  martin
 *  Use fixed size C99 types which come with GCC and newer Borland compilers.
 *  Revision 1.14  2008/01/30 10:27:50Z  martin
 *  Moved some macro definitions here.
 *  Revision 1.13  2007/03/08 15:00:30Z  martin
 *  Fixed incompatibility of macro _IS_MBG_FIRMWARE.
 *  Added a workaround for _IS_MBG_FIRMWARE under CVI.
 *  Support for BSD.
 *  Revision 1.12  2006/12/15 10:45:46  martin
 *  Added macro _IS_MBG_FIRMWARE.
 *  Cleanup for Linux, QNX, and Watcom C.
 *  Include mbg_tgt.h for non-firmware targets.
 *  Revision 1.11  2004/11/10 10:45:34  martin
 *  Added C99 fixed-type handling for QNX.
 *  Revision 1.10  2004/11/09 13:12:56  martin
 *  Redefined C99 integer types with fixed sizes as standard types
 *  if required, depending on the environment.
 *  Revision 1.9  2003/02/07 11:36:54  MARTIN
 *  New macros _hilo_16() and _hilo_32() for endian conversion.
 *  Revision 1.8  2002/05/28 10:09:54  MARTIN
 *  Added new macros _var_bswap16() and _var_bswap32().
 *  Revision 1.7  2001/03/14 11:30:48  MARTIN
 *  Removed definitions for UINT8, UINT16, UINT32.
 *  Redefined preprocessor control for Win32.
 *  Revision 1.6  2001/02/28 15:43:20  MARTIN
 *  Modified preprocessor syntax.
 *  Revision 1.5  2001/02/05 10:20:53  MARTIN
 *  Include different Linux types for user space and kernel space programs.
 *  Source code cleanup.
 *  Revision 1.4  2000/09/15 08:34:11  MARTIN
 *  Exclude some definitions if compiling under Win NT.
 *  Revision 1.3  2000/08/22 15:04:28  MARTIN
 *  Added new file header.
 *  Added macros to revert endianess of 16 and 32 bit values.
 *
 **************************************************************************/

#ifndef _WORDS_H
#define _WORDS_H


/* Other headers to be included */


#if !defined( _IS_MBG_FIRMWARE )

  #if defined( _C166 ) ||    \
      defined( _CC51 ) ||    \
      defined( __ARM ) ||    \
      defined( __ARMCC_VERSION )
    #define _IS_MBG_FIRMWARE 1
  #else
    #define _IS_MBG_FIRMWARE 0
  #endif

#endif


#if !_IS_MBG_FIRMWARE
  #include <mbg_tgt.h>
#else
  #if defined( __ARMCC_VERSION )  // Keil RealView Compiler for ARM
    #define __mbg_inline __inline
    #include <stdint.h>
    #include <inttypes.h>
    #include <stdbool.h>
    #define MBG_TGT_HAS_EXACT_SIZE_TYPES  1
  #else
    #define MBG_TGT_MISSING_64_BIT_TYPES 1
  #endif

  #if !defined( _DEPRECATED_BY )
    #define _DEPRECATED_BY( _s )  // empty definition
  #endif
#endif



#ifdef _WORDS
 #define _ext
#else
 #define _ext extern
#endif


/* Start of header body */

#if defined( _C166 ) \
 || defined( _CC51 )
  #define _BIT_DEFINED        1  // these compilers natively support the "bit" type
  #define USE_LONG_FOR_INT32  1
#endif



#if !defined( MBG_TGT_HAS_EXACT_SIZE_TYPES )

  #if defined( MBG_TGT_HAS_INT_8_16_32 )

    // Define C99 exact size types using non-standard exact-size types
    typedef __int8 int8_t;
    typedef unsigned __int8 uint8_t;

    typedef __int16 int16_t;
    typedef unsigned __int16 uint16_t;

    typedef __int32 int32_t;
    typedef unsigned __int32 uint32_t;

  #else

    // Assume a 16 or 32 bit compiler which doesn't
    // support exact-size types.

    typedef char int8_t;
    typedef unsigned char uint8_t;

    typedef short int16_t;
    typedef unsigned short uint16_t;

    // Using #if sizeof() to determine the size of a type may not
    // be supported by all preprocessors, and may even result in
    // build errors if used in a conditional preprocessor section,
    // so we can't use this here without compatibility problems.

    #if defined( USE_LONG_FOR_INT32 )
      typedef long int32_t;
      typedef unsigned long uint32_t;
    #elif defined( USE_INT_FOR_INT32 )
      typedef int int32_t;
      typedef unsigned int uint32_t;
    #else
      #error Need to define int32_t and uint32_t
    #endif

  #endif

  #if defined( MBG_TGT_MISSING_64_BIT_TYPES )

    // The build environment does not support 64 bit types. However,
    // 64 bit types need to be defined to avoid build errors e.g.
    // if these types are formally used in function prototypes.
    // We explicitly use abnormal data types to hopefully
    // cause compiler errors in case these types are
    // unexpectedly used to generate real code for a target
    // platform which does not support 64 bit types.
    typedef void *int64_t;
    typedef void *uint64_t;

  #else

    // Define C99 types using non-standard exact-size types
    // which are usually supported by build environments that
    // support 64 bit types but don't support C99 types.
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;

  #endif

#endif



#if defined( MBG_TGT_MISSING_64_BIT_TYPES )

  #define MBG_TGT_HAS_64BIT_TYPES    0

#else

  #define MBG_TGT_HAS_64BIT_TYPES    1

  #if !defined( MBG_TGT_HAS_ABS64 )
    #define _abs64( _i )  ( (int64_t) ( ( (_i) < 0 ) ? -(_i) : (_i) ) )
  #endif

#endif



// Some commonly used types

#if !defined( _UCHAR_DEFINED )
  typedef unsigned char uchar;
  #define uchar uchar
#endif

#if !defined( _USHORT_DEFINED )
  typedef unsigned short ushort;
  #define ushort ushort
#endif

#if !defined( _UINT_DEFINED )
  typedef unsigned int uint;
  #define uint uint
#endif

#if !defined( _ULONG_DEFINED )
  typedef unsigned long ulong;
  #define ulong ulong
#endif

#if !defined( _UDOUBLE_DEFINED )
  typedef double udouble;
  #define udouble udouble
#endif

#if !defined( _BYTE_DEFINED )
  typedef unsigned char byte;
  #define byte byte
#endif

#if !defined( _WORD_DEFINED )
  typedef unsigned short word;
  #define word word
#endif

#if !defined( _LONGWORD_DEFINED )
  typedef unsigned long longword;
  #define longword longword
#endif

#if !defined( _DWORD_DEFINED )
//  typedef unsigned long dword;
//  #define dword dword
#endif


#if defined( MBG_TGT_MISSING_BOOL_TYPE )
  //#error MBG_TGT_MISSING_BOOL_TYPE is defined
  // BDS/Borland C++ Builder 2006 (non-C++ mode)
  // Borland C++ Builder 5 (non-C++ mode)
  // BC 3.1
  // VC6
  // DDKbuild
  // VS2008
#endif

#if defined( __cplusplus )
  //#error __cplusplus is defined
#endif

#if defined( __bool_true_false_are_defined )
  //#error __bool_true_false_are_defined is defined
  // Keil armcc
  // gcc / Linux user space
  // clang / FreeBSD user space and kernel
#endif


#if defined( MBG_TGT_MISSING_BOOL_TYPE )                             /* from mbg_tgt.h */ \
      || ( !defined( __cplusplus )                                              /* C++ */ \
        && !defined( __bool_true_false_are_defined )                            /* C99 */ \
        && !defined( _LINUX_TYPES_H ) )                                /* Linux kernel */ \
        && !( defined( MBG_TGT_NETBSD ) && defined( _SYS_TYPES_H_ ) ) /* NetBSD kernel */

  // There's no native support for a "bool" type, so we
  // need a substitute.

  #if defined( _BIT_DEFINED )
    // A native "bit" type is supported, so we use it for bool.
    //#error substituting bit for bool
    // C166
    typedef bit bool;
  #else
    // Fall back to "int". This is just a hack which
    // may yield unexpected results with code like:
    //   return (bool) ( val & 0x10 );
    // A safe way of coding would be:
    //   return (bool) ( ( val & 0x10 ) != 0 );
    //#error substituting int for bool
    // Borland C++ Builder 5
    // BC 3.1
    // VC6
    // DDKbuild
    // VS2008
    typedef int bool;
  #endif

  // Eventually provoke a build error if the build
  // environment unexpectedly supports "bool" natively.
  #define bool bool
  #define true 1
  #define false 0
#else
  //#error native bool type supported
  // Borland C++ Builder 5 and newer (C++ mode only)
  // Keil armcc
  // gcc / Linux user space
  // gcc / Linux kernel
  // clang / FreeBSD user space and kernel
#endif


#if !defined( _BIT_DEFINED )

  // There's no native support for a "bit" type, so we
  // need a substitute. The "bool" type would fit best
  // and should be fine if it's supported natively.
  //
  // However, if "bool" has been substituted above
  // by "int"then this is just a hack which may yield
  // unexpected results with code like:
  //   return (bit) ( val & 0x10 );
  // A safe way of coding would be:
  //   return (bit) ( ( val & 0x10 ) != 0 );

  //#error substituting bool for bit
  // Keil armcc
  // Borland C++ Builder 5
  // BC 3.1
  // VC6
  // DDKbuild
  // VS2008
  // gcc / Linux user space
  // gcc / Linux kernel
  // clang / FreeBSD user space and kernel
  typedef bool bit;

  // Eventually provoke a build error if the build
  // environment unexpectedly supports "bit" natively.
  #define bit bit

  #define _BIT_REDEFINED  1

#else
  //#error native bit type supported
  // C166
#endif



#define BYTE_0( _x )  ( (uint8_t ) ( (_x) & 0xFF ) )
#define BYTE_1( _x )  ( (uint8_t ) ( ( ( (uint16_t) (_x) ) >> 8 ) & 0xFF ) )
#define BYTE_2( _x )  ( (uint8_t ) ( ( ( (uint32_t) (_x) ) >> 16 ) & 0xFF ) )
#define BYTE_3( _x )  ( (uint8_t ) ( ( ( (uint32_t) (_x) ) >> 24 ) & 0xFF ) )


#define HI_BYTE( _x )      ( (uint8_t ) ( (_x) >> 8 ) )
#define LO_BYTE( _x )      ( (uint8_t ) ( (_x) & 0xFF ) )

#define HI_WORD( _x )      ( (uint16_t ) ( (_x) >> 16 ) )
#define LO_WORD( _x )      ( (uint16_t ) ( (_x) & 0xFFFF ) )

// the macros below assume little endianess
// these macros expect the name of a variable
#define BYTE_OF( _v, _n )  *( ( (uint8_t *) &(_v) ) + (_n) )
#define WORD_OF( _v, _n )  *( ( (uint16_t *) &(_v) ) + (_n) )

#define FBYTE_OF( _v, _n )  *( ( (uint8_t far *) &(_v) ) + (_n) )
#define FWORD_OF( _v, _n )  *( ( (uint16_t far *) &(_v) ) + (_n) )

// same as above, but taking pointers
#define BYTE_OF_P( _p, _n )  *( ( (uint8_t *) (_p) ) + (_n) )
#define WORD_OF_P( _p, _n )  *( ( (uint16_t *) (_p) ) + (_n) )


// a macro to swap the byte order of a 16 bit value
#define _bswap16( _x )                        \
(                                             \
  ( ( ( (uint16_t) (_x) ) & 0x00FF ) << 8 ) | \
  ( ( ( (uint16_t) (_x) ) & 0xFF00 ) >> 8 )   \
)

// a macro to swap the byte order of a 32 bit value
#define _bswap32( _x )                               \
(                                                    \
  ( ( ( (uint32_t) (_x) ) & 0x000000FFUL ) << 24 ) | \
  ( ( ( (uint32_t) (_x) ) & 0x0000FF00UL ) << 8 )  | \
  ( ( ( (uint32_t) (_x) ) & 0x00FF0000UL ) >> 8 )  | \
  ( ( ( (uint32_t) (_x) ) & 0xFF000000UL ) >> 24 )   \
)

// a macro to swap the word order of a 32 bit value
#define _wswap32( _x )                               \
(                                                    \
  ( ( ( (uint32_t) (_x) ) & 0x0000FFFFUL ) << 16 ) | \
  ( ( ( (uint32_t) (_x) ) >> 16 ) & 0x0000FFFFUL )   \
)

#define _var_bswap16( _v )   (_v) = _bswap16( _v )
#define _var_bswap32( _v )   (_v) = _bswap32( _v )


// The C51 compiler is big-endian, that means the most
// significant byte of a 16 or 32 bit value is stored in
// the lowest memory location. Most other systems are
// little-endian, so we must use macros to adjust the
// byte order if the C51 is used.

#if defined( _CC51 )
  #define _hilo_16( _x )  _bswap16( _x )
  #define _hilo_32( _x )  _bswap32( _x )
#else
  #define _hilo_16( _x )  (_x)
  #define _hilo_32( _x )  (_x)
#endif



#define _set_array_bit( _n, _byte_array, _max_bytes )   \
do                                                      \
{                                                       \
  int byte_idx = (_n) >> 3;                             \
                                                        \
  if ( byte_idx < _max_bytes )                          \
    _byte_array[byte_idx] |= ( 1 << ( (_n) & 0x07 ) );  \
                                                        \
} while ( 0 )


#define _clear_array_bit( _n, _byte_array, _max_bytes ) \
do                                                      \
{                                                       \
  int byte_idx = (_n) >> 3;                             \
                                                        \
  if ( byte_idx < _max_bytes )                          \
    _byte_array[byte_idx] &= ~( 1 << ( (_n) & 0x07 ) ); \
                                                        \
} while ( 0 )



#define _isdigit( _c )     ( (_c) >= '0' && (_c) <= '9' )


// A macro function which can safely be used without
// side effects as a macro doing nothing.
// This is useful to define debug macros away in
// release builds, etc.
#if !defined( _nop_macro_fnc )
  #define _nop_macro_fnc()     do {} while (0)
#endif


/**
 * @brief A table entry which can be used to map codes to names.
 */
typedef struct
{
  long code;
  const char *name;

} MBG_CODE_NAME_TABLE_ENTRY;

/**
 * @brief A macro defining a ::MBG_CODE_NAME_TABLE_ENTRY
 *
 * The stringified parameter is used for the name.
 *
 * @param _n  The symbolic name of the numeric code
 */
#define _mbg_cn_table_entry( _n )  { _n, #_n }

/**
 * @brief A macro defining an empty ::MBG_CODE_NAME_TABLE_ENTRY
 *
 * This is used to terminate a table.
 */
#define _mbg_cn_table_end()        { 0, NULL }



/**
 * @brief A timestamp with nanosecond resolution
 *
 * @note If the structure is to represent a negative value then both the
 * fields nano_secs and secs have to be set to the negative values.
 * Otherwise the sign of the represented number was ambiguous if either
 * of the fields was accidentally 0, and only the other field was not 0.
 * The macro ::_nano_time_negative should always be used to determine
 * if the sign of the represented value is negative, or not.
 *
 * @note The secs field will roll over on 2038-01-19 03:14:07
 * if used for the number of seconds since 1970-01-01, just like
 * 32 bit POSIX time_t.
 *
 * @see ::_nano_time_negative
 * @see ::_nano_time_zero
 * @see ::NANO_TIME_64
 */
typedef struct nano_time_s
{
  // ATTENTION:
  // This structure is and has has been used in public API calls for a long time,
  // so even though the order of member fields is different than in NANO_TIME_64
  // this must *NOT* be changed, or API compatibility will get lost!
  int32_t nano_secs;    ///< [nanoseconds]
  int32_t secs;         ///< [seconds], usually since 1970-01-01 00:00:00

} NANO_TIME;

#define _mbg_swab_nano_time( _p )   \
do                                  \
{                                   \
  _mbg_swab32( &(_p)->nano_secs );  \
  _mbg_swab32( &(_p)->secs );       \
} while ( 0 )

/**
 * Check if the value of the ::NANO_TIME structure _nt is negative
 */
#define _nano_time_negative( _nt ) \
  ( ( (_nt)->secs < 0 ) || ( (_nt)->nano_secs < 0 ) )

/**
 * Check if the value of the ::NANO_TIME structure _nt is 0
 */
#define _nano_time_zero( _nt ) \
  ( ( (_nt)->secs == 0 ) && ( (_nt)->nano_secs == 0 ) )



/**
 * @brief A timestamp with nanosecond resolution, but 64 bit size
 *
 * @note If the structure is to represent a negative value then both the
 * fields nano_secs and secs have to be set to the negative values.
 * Otherwise the sign of the represented number was ambiguous if either
 * of the fields was accidentally 0, and only the other field was not 0.
 * The macro ::_nano_time_64_negative should always be used to determine
 * if the sign of the represented value is negative, or not.
 *
 * @see ::_nano_time_64_negative
 * @see ::_nano_time_64_zero
 * @see ::NANO_TIME
 */
typedef struct nano_time_64_s
{
  // ATTENTION:
  // This structure is and has been used in public API calls for a long time,
  // so even though the order of member fields is different than in NANO_TIME
  // this must *NOT* be changed, or API compatibility will get lost!
  int64_t secs;         ///< [seconds], usually since 1970-01-01 00:00:00
  int64_t nano_secs;    ///< [nanoseconds]

} NANO_TIME_64;

#define _mbg_swab_nano_time_64( _p ) \
do                                   \
{                                    \
  _mbg_swab64( &(_p)->secs );        \
  _mbg_swab64( &(_p)->nano_secs );   \
} while ( 0 )

/**
 * Check if the value of the ::NANO_TIME_64 structure _nt is negative
 */
#define _nano_time_64_negative( _nt ) \
  ( ( (_nt)->secs < 0 ) || ( (_nt)->nano_secs < 0 ) )

/**
 * Check if the value of the ::NANO_TIME_64 structure _nt is 0
 */
#define _nano_time_64_zero( _nt ) \
  ( ( (_nt)->secs == 0 ) && ( (_nt)->nano_secs == 0 ) )



// The size_t type can eventually be larger than an int type.
// However, some snprintf-like functions expect a size_t value
// to specify the buffer size, but just return an int value.
// So we take care that at least the return value is limited
// to MAXINT.
#if defined( MBG_TGT_WIN32 )
  #define _int_from_size_t( _n ) \
    ( ( (_n) > INT_MAX ) ? INT_MAX : (int) (_n) )
#else
  #define _int_from_size_t( _n )  (_n)
#endif



/**
 * @brief A helper macro to implement ::STRINGIFY correctly
 *
 * This just a helper macro which must be defined before
 * ::STRINGIFY to work correctly, and should not be used alone.
 *
 * @see ::STRINGIFY
 */
#define XSTRINGIFY(x) #x


/**
 * @brief Make a string from a constant definition
 *
 * This macro can be used e.g. to define a constant string on the
 * compiler's command line, e.g. like -DVERSION_STRING="v1.0 BETA".
 * Source code like
 * @code{.c}
    const char version_string[] = VERSION_STRING;
 * @endcode
 *
 * may not work for every compiler since the double quotes
 * in VERSION_STRING may be removed when the definition is evaluated.
 * A proper solution is to use the STRINGIFY() macro defined here:
 * @code{.c}
    const char version_string[] = STRINGIFY( VERSION_STRING );
 * @endcode
 */
#define STRINGIFY(x) XSTRINGIFY(x)


/* End of header body */

#undef _ext

#endif  /* _WORDS_H */
