
/**************************************************************************
 *
 *  $Id: str_util.h 1.8 2020/02/27 13:57:12 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions and prototypes for str_util.c
 *
 * -----------------------------------------------------------------------
 *  $Log: str_util.h $
 *  Revision 1.8  2020/02/27 13:57:12  martin
 *  Updated function prototypes.
 *  Revision 1.7  2019/07/31 15:42:39  martin
 *  Doxygen changes.
 *  Revision 1.6  2018/08/23 13:10:26  martin
 *  New inline functions mbg_buffer_specs_valid() and
 *  mbg_chk_snprint_results() that can also be called
 *  from code used in kernel mode.
 *  Updated function prototypes.
 *  Revision 1.5  2018/06/25 13:24:15  martin
 *  Updated function prototypes.
 *  Revision 1.4  2017/05/10 15:26:10  martin
 *  Tiny cleanup.
 *  Revision 1.3  2016/12/14 16:22:24  martin
 *  Added macro _sn_cpy_str_safe() to simplify calls.
 *  Revision 1.2  2016/08/05 12:33:17  martin
 *  Moved string trim functions from cfg_util module here.
 *  Added variable str_not_avail.
 *  Fixed some spelling.
 *  Updated function prototypes.
 *  Revision 1.1  2015/08/25 15:57:43  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _STR_UTIL_H
#define _STR_UTIL_H

/* Other headers to be included */

#include <words.h>  // implicitly includes mbg_tgt.h for non-firmware projects

#if defined( MBG_TGT_KERNEL )
  #include <mbgddmsg.h>
#else
  #include <stdlib.h>
  #include <stdarg.h>
#endif


#ifdef _STR_UTIL
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


_ext const char *str_not_avail
#ifdef _DO_INIT
  = "N/A"
#endif
;



static __mbg_inline /*HDR*/
/**
 * @brief Check if the buffer plus size parameters passed to a function are valid.
 *
 * This function can be used to check parameters that have been
 * passed to another function to specify an output buffer to be filled.
 *
 * If no buffer has been specified, or the size of the buffer which
 * eventually remains after a previous operation is 0 or even less than 0
 * then no data can be placed in the buffer.
 *
 * @param[in]  s        The address of the specified buffer
 * @param[in]  max_len  The size of the specified buffer.
 *
 * @return true  if the buffer address is not NULL and the size is > 0, else false.
 */
bool mbg_buffer_specs_valid( char *s, size_t max_len )
{
  return s != NULL && max_len > 0;

}  // mbg_buffer_specs_valid



static __mbg_inline /*HDR*/
/**
 * @brief Check the results of an snprintf()-like function.
 *
 * Implementations of snprintf()-like functions may behave differently
 * and badly if the specified output buffer is too small.
 * The exact behavior depends on the runtime library shipped with a
 * specific build environment for a specific operating system,
 * the version of that runtime library, and may even differ depending
 * on whether kernel mode or user mode code is compiled.
 *
 * This function can be called after any snprintf()-like function
 * to make sure that a valid buffer is always 0-terminated, and the
 * number returned to indicate how many bytes have been written to the
 * buffer is never less than 0, and never exceeds the real size
 * of the buffer.
 *
 * @param[in]  n        The return code from an snprintf()-like function that has been called before.
 * @param[in]  s        The address of the buffer that had been passed to the snprintf()-like function.
 * @param[in]  max_len  The size of the specified buffer that had been passed to the snprintf()-like function.
 *
 * @return The real number of bytes that had been written to the buffer.
 *
 * @see ::vsnprintf_safe
 * @see ::mbg_kdd_vsnprintf
 * @see ::mbg_buffer_specs_valid
 */
int mbg_chk_snprint_results( size_t n, char *s, size_t max_len )
{
  if ( !mbg_buffer_specs_valid( s, max_len ) )
    return 0;    // Buffer parameters are not valid.


  // Force proper worst-case termination of the output string.
  s[max_len - 1] = 0;

  // If n is > 0, but less than the specified buffer size we
  // assume the value is correct.
  if ( n > 0 && n < max_len )
    goto out;

  // Determine the real string length, but don't just call strlen()
  // since that function may not be available in kernel mode.
  for ( n = 0; s[n]; n++ );

out:
  // Most snprintf()-like functions take a "size_t" to specify the buffer size,
  // but just return an "int", which may be a smaller data type than "size_t",
  // so we do a conversion here, if required, and try to do the conversion
  // in a safe way.
  return _int_from_size_t( n );

}  // mbg_chk_snprint_results



#define _sn_cpy_str_safe( _dst, _src )  sn_cpy_str_safe( _dst, sizeof( _dst ), _src )


/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
 * @brief A portable, safe implementation of vsnprintf()
 *
 * Unfortunately the behaviour of vsnprintf() and thus snprintf()
 * differs in detail across various build environments and run time
 * libraries.
 *
 * If the output exceeds the buffer size and thus is truncated then:<br>
 *
 * - Under Windows a negative value is returned and eventually ***no***
 *   terminating 0 is written to the output buffer, so the output string
 *   may not be terminated properly.
 *
 * - Some versions of glibc return the number of bytes that ***would***
 *   have been written to the buffer ***if*** the buffer would have been
 *   large enough, instead of the true number of characters that have
 *   been written to the buffer.
 *
 * So subsequent calls like
 *
 *   n = snprintf( s, max_len, ... );
 *   n += snprintf( &s[n], max_len - n, ... );
 *
 * may always work properly, or fail with buffer overruns or stack
 * corruption depending on the build environment.
 * This wrapper function takes care that strings are always terminated
 * properly, and that the returned value always matches the number of
 * characters really written to the string buffer, excluding the
 * terminating 0.
 *
 * @note The "size_t" type parameter used to specify the buffer size
 * can be larger (e.g. "unsigned long") than the "int" type returned
 * by mostly all functions of the printf() family. So if a very large
 * buffer is specified, and a large number of characters (more than
 * MAXINT) are written to that buffer then how can an "int" type
 * return the large number of characters written to the buffer?
 * We also try to workaround this here.
 *
 * @param[out] s        The string buffer to be filled
 * @param[in]  max_len  Size of the output buffer for 0-terminated string
 * @param[in]  fmt      Format string according to subsequent parameters
 * @param[in]  args     Variable argument list in va_list format
 *
 * @return Number of characters written to the output buffer, except the terminating 0
 *
 * @see ::snprintf_safe
 * @see ::strncpy_safe
 * @see ::sn_cpy_str_safe
 * @see ::sn_cpy_char_safe
 */
 __attribute__( ( format( printf, 3, 0 ) ) ) int vsnprintf_safe( char *s, size_t max_len, const char *fmt, va_list args ) ;

 /**
 * @brief A portable, safe implementation of snprintf()
 *
 * For a detailed description see ::vsnprintf_safe
 *
 * @param[out] s        The string buffer to be filled
 * @param[in]  max_len  Size of the output buffer for 0-terminated string
 * @param[in]  fmt      Format string according to subsequent parameters
 * @param[in]  ...      Variable argument list according to the format string
 *
 * @return Number of characters written to the output buffer, except the terminating 0
 *
 * @see ::vsnprintf_safe
 * @see ::strncpy_safe
 * @see ::sn_cpy_str_safe
 * @see ::sn_cpy_char_safe
 */
 __attribute__( ( format( printf, 3, 4 ) ) ) int snprintf_safe( char *s, size_t max_len, const char *fmt, ... ) ;

 /**
 * @brief A portable, safe implementation of strncpy()
 *
 * In the original implementation of strncpy(), if the length of the
 * string to be copied into the destination buffer exceeds the specified
 * buffer length then the string in the output buffer is not 0-terminated.
 *
 * Our implementation always forces a proper termination by 0, but unlike
 * the original implementation of strncpy() it does ***not*** fill the whole
 * remaining buffer space with '0' characters.
 *
 * @param[out] dst      Pointer to the output buffer
 * @param[in]  src      Pointer to the input buffer
 * @param[in]  max_len  Size of the output buffer for 0-terminated string
 *
 * @return Pointer to the destination buffer
 *
 * @see ::vsnprintf_safe
 * @see ::snprintf_safe
 * @see ::sn_cpy_str_safe
 * @see ::sn_cpy_char_safe
 */
 char *strncpy_safe( char *dst, const char *src, size_t max_len ) ;

 /**
 * @brief A function to copy a string safely, returning the number of characters copied
 *
 * This basically works like ::strncpy_safe but instead of a pointer to
 * the destination buffer it returns the number of characters copied
 * to the destination buffer.
 *
 * @param[out] dst      Pointer to the output buffer
 * @param[in]  max_len  Size of the output buffer for 0-terminated string
 * @param[in]  src      Pointer to the input buffer
 *
 * @return Number of characters copied to the destination buffer
 *
 * @see ::vsnprintf_safe
 * @see ::snprintf_safe
 * @see ::strncpy_safe
 * @see ::sn_cpy_char_safe
 */
 int sn_cpy_str_safe( char *dst, size_t max_len, const char *src ) ;

 /**
 * @brief A function to copy a character safely to a string buffer
 *
 * This basically works like ::sn_cpy_str_safe but expects a character
 * to be copied to the destination buffer. Appends a terminating 0 to
 * the string buffer and returns the number of characters copied to
 * the destination buffer, usually 0 or 1.
 *
 * @param[out] dst      Pointer to the output buffer
 * @param[in]  max_len  Size of the output buffer for 0-terminated string
 * @param[in]  c        Character to be copied to the destination buffer
 *
 * @return Number of characters copied to the destination buffer, without the terminating 0
 *
 * @see ::vsnprintf_safe
 * @see ::snprintf_safe
 * @see ::strncpy_safe
 * @see ::sn_cpy_str_safe
 */
 int sn_cpy_char_safe( char *dst, size_t max_len, char c ) ;

 /**
 * @brief Trim whitespace at the end of a string
 *
 * @param[in,out] s  The string to be trimmed
 */
 void trim_trailing_whitespace( char *s ) ;

 /**
 * @brief Trim whitespace at the beginning of a string
 *
 * @param[in,out] s  The string to be trimmed
 */
 void trim_leading_whitespace( char *s ) ;

 /**
 * @brief Trim both leading and trailing whitespace from a string
 *
 * @param[in,out] s  The string to be trimmed
 */
 void trim_whitespace( char *s ) ;

 /**
 * @brief Copy array of bytes starting at beginning of buffer
 *
 * Can be used if the destination address is in the same buffer
 * in front of the source address. Even though you would expect
 * that memcpy() would also work for this properly, we have seen
 * cases where it didn't, and only memmove() worked correctly.
 * Anyway, we try to avoid the overhead of memmove().
 *
 * @param[out] dst      Destination address behind the source address
 * @param[in]  src      Source address
 * @param[in]  n_bytes  Number of bytes to copy
 *
 * @see ::mbg_memcpy_reversed
 */
 void mbg_memcpy( void *dst, const void *src, size_t n_bytes ) ;

 /**
 * @brief Copy an array of bytes in reversed order, starting at end of buffer
 *
 * Can be used if the destination address is in the same buffer
 * behind the source address, so the source address would be
 * overwritten by a normal memcpy().
 *
 * @param[out] dst      Destination address behind the source address
 * @param[in]  src      Source address
 * @param[in]  n_bytes  Number of bytes to copy
 *
 * @see ::mbg_memcpy
 */
 void mbg_memcpy_reversed( void *dst, const void *src, size_t n_bytes ) ;


/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif


/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _STR_UTIL_H */
