
/**************************************************************************
 *
 *  $Id: mbg_tgt.h 1.47 2019/11/12 15:33:59 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Check the build environment and setup control definitions
 *    for the Meinberg library modules.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbg_tgt.h $
 *  Revision 1.47  2019/11/12 15:33:59  martin
 *  Added some doxygen comments.
 *  Revision 1.46  2019/08/28 12:52:02  martin
 *  Changes for mingw.
 *  Revision 1.45  2019/02/11 09:53:02Z  martin
 *  Support the mingw build environment.
 *  Changed inclusion of winsock2.h/windows.h on Windows.
 *  Removed obsolete RCS log entries.
 *  Revision 1.44  2018/12/11 15:34:15  martin
 *  Older Visual Studio versions don't support _strtoi64().
 *  Revision 1.43  2018/11/22 11:28:01Z  martin
 *  New preprocessor symbol MBG_TGT_USE_IOCTL.
 *  Provide 'uintptr_t' for old Linux kernels.
 *  Removed obsolete Windows driver stuff.
 *  Revision 1.42  2018/09/20 09:40:21  martin
 *  Define MBG_TGT_MISSTNG_STRTOLL and
 *  MBG_TGT_MISSING_LLDDIV_T, if appropriate.
 *  Revision 1.41  2018/08/23 10:49:56Z  martin
 *  Enhanced handling of Windows DDK builds.
 *  Revision 1.40  2018/07/02 16:57:49Z  martin
 *  Removed obsolete define MBG_USE_MM_IO_FOR_PCI.
 *  Revision 1.39  2018/06/25 10:52:00  martin
 *  Support MBG_TGT_NO_TGT.
 *  Default settings for KERNEL_HAS_BOOL and KERNEL_HAS_TRUE_FALSE
 *  for Linux can be overridden by project settings.
 *  Renamed MBG_PRE64_PREFIX to MBG_PRI_64_PREFIX.
 *  MBG_PRI_64_PREFIX_L or MBG_PRI_64_PREFIX_LL can be used
 *  to override the defaults with specific project settings.
 *  MBG_TGT_HAS_DEV_FN is supported on Windows, but
 *  MBG_TGT_HAS_DEV_FN_BASE is not supported.
 *  Enhanced debug support for Windows kernel drivers.
 *  Include limits.h under Windows.
 *  Defined MBG_TGT_MISSING_STRUCT_TIMESPEC for Borland C.
 *  Defined __func__ for BC 5 and for DOS, and provide a
 *  common default declaration..
 *  Updated some comments.
 *  Revision 1.38  2017/08/08 13:07:31  martin
 *  Proper fix for PRId64 and others.
 *  Define _CRT_SECURE_NO_WARNINGS only if not already defined.
 *  Revision 1.37  2017/07/10 07:08:45  thomas-b
 *  Define PRId64 if it is not defined in inttypes.h
 *  Revision 1.36  2017/07/04 12:35:11  martin
 *  Fixed build for Windows kernel space.
 *  Don't define ssize_t if HAVE_SSIZE_T is already defined.
 *  Added definition for _NO_MBG_API.
 *  Fixed missing 'bool' type for old Linux kernels.
 *  Fixed missing 'struct timespec' for DOS.
 *  Evaluate preprocessor symbol KERNEL_HAS_BOOL to avoid
 *  compiler errors due to duplicate definitions in specific
 *  Linux kernels patched by the distro maintainers.
 *  Provide ssize_t for C++Builder 5.
 *  Improved Visual Studio version checking.
 *  New define MBG_TGT_HAS_NODE_NAME.
 *  Revision 1.35  2016/08/05 12:21:34  martin
 *  Conditionally define a macro _DEPRECATED_BY which can be used to
 *  tag functions as deprecated, so compilers can emit appropriate warnings.
 *  New symbol MBG_TGT_HAS_ABS64.
 *  Moved some compatibility definitions from gpsserio.h here.
 *  Define ssize_t for Windows, if required.
 *  Conditionally provided struct timespec for Windows.
 *  Added compatible 64 bit type print format specifiers.
 *  Include inttypes.h for all targets providing also stdint.h.
 *  Added some MSVC version code information.
 *  Fixes for FreeBSD.
 *  Fixed some spelling.
 *  Tmp workaround for 2.6.32-5-sparc64.
 *  Proper fix required.
 *  Revision 1.34  2015/03/03 13:32:49  martin
 *  Provide __func__ for MS Visual Studio.
 *  Revision 1.33  2015/03/02 11:27:59Z  martin
 *  Windows only:
 *  Define _CRT_SECURE_NO_WARNINGS to quiet compiler warnings.
 *  Define WIN32_LEAN_AND_MEAN only if it hasn't been defined before.
 *  Revision 1.32  2014/06/24 09:21:44  martin
 *  Update for newer C++Builder versions.
 *  Revision 1.31  2014/05/27 10:23:33  martin
 *  Finer control of which types are required for or already
 *  available on particular target systems.
 *  First definitions to support SunOS/Solaris.
 *  Revision 1.30  2014/04/01 12:55:58  martin
 *  Define MBG_TGT_WIN32 also for MS resource compiler.
 *  New target MBG_TGT_POSIX.
 *  Always include winsock2.h and windows.h for MBG_TGT_WIN32.
 *  Always include unistd.h for MBG_TGT_POSIX.
 *  Define empty __attribute__ macro for non-gcc environments.
 *  Revision 1.29  2013/02/01 14:50:46  martin
 *  Fixed a typo which caused an error under Borland CBuilder 5.
 *  Revision 1.28  2012/12/12 10:03:16Z  martin
 *  Fix for Borland C 3.1.
 *  Revision 1.27  2012/11/29 12:03:14Z  martin
 *  Moved definition of _no_macro_fnc() to words.h.
 *  Revision 1.26  2012/11/02 09:01:47Z  martin
 *  Merged some stuff depending on the build environment here
 *  and cleaned up.
 *  Revision 1.25  2012/04/04 07:17:18  martin
 *  Treat QNX Neutrino as Unix target.
 *  Revision 1.24  2011/08/23 10:21:23  martin
 *  New symbol _NO_MBG_API_ATTR which can be used with functions
 *  which are going to be exported by a DLL, but actually aren't, yet.
 *  Revision 1.23  2011/08/19 10:47:00  martin
 *  Don't include stddef.h.
 *  Distinguish between different gcc target platforms.
 *  Initial support for IA64 platform.
 *  Support wchar_t for BSD.
 *  Defined _NO_USE_PACK_INTF for Sparc and IA64.
 *  Fixed typo in comment.
 *  Revision 1.22  2009/10/01 08:20:50  martin
 *  Fixed inline code support with different BC versions.
 *  Revision 1.21  2009/09/01 10:34:23Z  martin
 *  Don't define __mbg_inline for CVI and undefined targets.
 *  Revision 1.20  2009/08/18 15:14:26  martin
 *  Defined default MBG_INVALID_PORT_HANDLE for non-Windows targets.
 *  Revision 1.19  2009/06/09 10:03:58  daniel
 *  Preliminary support for ARM architecture.
 *  Revision 1.18  2009/04/01 14:10:55  martin
 *  Cleanup for CVI.
 *  Revision 1.17  2009/03/19 15:21:07Z  martin
 *  Conditionally define DWORD_PTR type for old MS C compilers.
 *  Revision 1.16  2008/12/08 16:42:30  martin
 *  Defined _GNU_SOURCE for Linux.
 *  Revision 1.15  2008/11/19 15:31:49  martin
 *  Added symbol MBG_ARCH_I386.
 *  Revision 1.14  2008/09/03 15:06:04  martin
 *  Support DOS protected mode target.
 *  Support SUN SPARC architecture.
 *  Specified handle types for common host environments.
 *  Added macro MBG_USE_MM_IO_FOR_PCI.
 *  Added macro _nop_macro_fnc().
 *  Revision 1.13  2008/01/30 15:52:22  martin
 *  Modified checking for availability of wchar_t.
 *  Revision 1.13  2008/01/29 15:18:07Z  martin
 *  Recognize DOS target under Watcom compilers.
 *  Flag Watcom C always supports wchar_t.
 *  Revision 1.12  2008/01/17 09:38:50Z  daniel
 *  Added macros to determine whether C language extensions
 *  (e.g. C94, C99) are supported by the target environment.
 *  Added macro to check whether wchar_t and friends are
 *  supported, and some compatibility stuff.
 *  Revision 1.11  2007/10/31 16:58:03  martin
 *  Fixed __mbg_inline for Borland C (DOS).
 *  Revision 1.10  2007/09/25 08:10:27Z  martin
 *  Support CVI target environment.
 *  Added MBG_PORT_HANDLE type for serial ports.
 *  Added macros for unified inline code syntax.
 *  Revision 1.9  2006/12/08 12:45:54Z  martin
 *  Under Windows include ntddk.h rather than windows.h
 *  if building kernel driver .
 *  Revision 1.8  2006/10/25 12:20:45Z  martin
 *  Initial support for FreeBSD, NetBSD, and OpenBSD.
 *  Added definitions for generic handle types.
 *  Revision 1.7  2006/08/23 13:43:55  martin
 *  Added definition for MBG_TGT_UNIX.
 *  Minor syntax fixes.
 *  Revision 1.6  2006/01/25 14:37:06  martin
 *  Added definitions for 64 bit Windows environments.
 *  Revision 1.5  2003/12/17 16:11:41Z  martin
 *  Split API modifiers into _MBG_API and _MBG_API_ATTR.
 *  Revision 1.4  2003/06/19 08:20:22Z  martin
 *  Added WINAPI attribute for DLL exported functions.
 *  Revision 1.3  2003/04/09 13:37:20Z  martin
 *  Added definition for _MBG_API.
 *  Revision 1.2  2003/02/24 16:08:45Z  martin
 *  Don't setup for Win32 PNP if explicitly configured non-PNP.
 *  Revision 1.1  2002/02/19 13:46:20Z  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#ifndef _MBG_TGT_H
#define _MBG_TGT_H


/* Other headers to be included */

#ifdef MBG_TGT_NO_TGT
  #include <mbg_no_tgt.h>
#else

#ifdef _MBG_TGT
 #define _ext
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif

// The prefix required in a printf() format string to print an
// int64_t type (i.e. "%li", "%lli", "I64i", etc.) may vary depending
// on the compiler type and version, and the associated run time library.
// For user space applications this is usually defined in the header
// files provided by the build environment, but for kernel code there
// are often no such defines, but there are some commonly used defaults.
// This is a hack that can be used to override these defaults in the
// project settings for kernel code, e.g. in the Makefile.
#if defined( MBG_PRI_64_PREFIX_L )
  #define MBG_PRI_64_PREFIX  "l"
#elif defined( MBG_PRI_64_PREFIX_LL )
  #define MBG_PRI_64_PREFIX  "ll"
#endif


#if defined( _CVI_ )

  #define MBG_TGT_CVI

  #if defined( _NI_mswin_ )
    #define MBG_TGT_WIN32
  #else
    #error Unsupported CVI target platform.
  #endif

#elif defined( _WIN32_WINNT )

  // MS platform SDK
  // WinNT 4.0 and above
  #define MBG_TGT_WIN32

  #if ( _WIN32_WINNT >= 0x0500 )
    // Win2k and above
    #if !defined( MBG_TGT_WIN32_NON_PNP )
      // only if not explicitly disabled
      #define MBG_TGT_WIN32_PNP
    #endif
  #endif

#elif defined( WINVER )

  // MS platform SDK
  // Win95, WinNT 4.0 and above
  #define MBG_TGT_WIN32

  #if ( WINVER >= 0x0500 )
    // Win98, Win2k and above
    // #define ...
  #endif

#elif defined( __WIN32__ )

  // Borland C++ Builder
  #define MBG_TGT_WIN32

#elif defined( _WIN32 )

  // MS Visual C++
  #define MBG_TGT_WIN32

#elif defined( RC_INVOKED )

  // MS resource compiler
  #define MBG_TGT_WIN32

#elif defined( __WINDOWS_386__ )

  // Watcom C/C++ for target Win32
  #define MBG_TGT_WIN32

#elif defined( __NETWARE_386__ )

  // Watcom C/C++ for target NetWare
  #define MBG_TGT_NETWARE

#elif defined( __OS2__ )

  // Watcom C/C++ for target OS/2
  #define MBG_TGT_OS2
  #define MBG_TGT_USE_IOCTL    1

#elif defined( __linux )

  // GCC for target Linux
  #define MBG_TGT_LINUX
  #define _GNU_SOURCE    1

  #if defined( __KERNEL__ )
    #define MBG_TGT_KERNEL
  #endif

#elif defined( __FreeBSD__ )

  // GCC for target FreeBSD
  #define MBG_TGT_FREEBSD

#elif defined( __NetBSD__ )

  // GCC for target NetBSD
  #define MBG_TGT_NETBSD

#elif defined( __OpenBSD__ )

  // GCC for target OpenBSD
  #define MBG_TGT_OPENBSD

#elif defined( __sun )  // Oracle Solaris or other SunOS derived operating system

  // __SUNPRO_C    Oracle Solaris Studio C compiler, __SUNPRO_C value is the version number
  // __SUNPRO_CC   Oracle Solaris Studio C++ compiler, __SUNPRO_CC value is the version number
  // __sparc       generate code for SPARC (R) architecture (32-bit or 64-bit)
  // __sparcv9     generate code for 64-bit SPARC architecture
  // __i386        generate code for 32-bit x86 architecture
  // __amd64       generate code for 64-bit x64 architecture

  #define MBG_TGT_SUNOS

#elif defined( __QNX__ )

  // any compiler for target QNX
  #define MBG_TGT_QNX

  #if defined( __QNXNTO__ )
    // target QNX Neutrino
    #define MBG_TGT_QNX_NTO
  #endif

#elif defined( __MSDOS__ ) || defined( __DOS__ )

  // any compiler for target DOS
  #define MBG_TGT_DOS

  #if defined( __WATCOMC__ ) && defined( __386__ )

    #define MBG_TGT_DOS_PM  // protected mode DOS

  #endif

#endif



#if defined( MBG_TGT_FREEBSD ) \
 || defined( MBG_TGT_NETBSD )  \
 || defined( MBG_TGT_OPENBSD )
  #define MBG_TGT_BSD

  #if defined( _KERNEL )
    #define MBG_TGT_KERNEL
  #endif

#endif

#if defined( MBG_TGT_LINUX )   \
 || defined( MBG_TGT_BSD )     \
 || defined( MBG_TGT_QNX_NTO ) \
 || defined( MBG_TGT_SUNOS )

  #define MBG_TGT_POSIX
  #define MBG_TGT_UNIX

#endif

#if defined( MBG_TGT_WIN32 )

  #if !defined( _CRT_SECURE_NO_WARNINGS )
    #define _CRT_SECURE_NO_WARNINGS 1
  #endif

  #include <limits.h>

  #define MBG_TGT_HAS_DEV_FN       1
  #define MBG_TGT_HAS_DEV_FN_BASE  0
  #define MBG_TGT_USE_IOCTL        1

#endif


// Some definitions depending on the build environment ...

#define FUNC_UNKNOWN "func_???"

#if defined( __GNUC__ ) || defined( __clang__ )

  #if defined( __clang__ )
    #define _CLANG_VERSION ( __clang_major__ * 10000 \
                           + __clang_minor__ * 100   \
                           + __clang_patchlevel__ )
  #endif  // defined( __clang__ )

  #define _GCC_VERSION ( __GNUC__ * 10000 \
                       + __GNUC_MINOR__ * 100 \
                       + __GNUC_PATCHLEVEL__ )

  #if defined( __MINGW32__ )
    #define MBG_TGT_MINGW
  #endif

  #if defined( __MINGW64__ )
    #define MBG_TGT_MINGW
  #endif

  #if defined( MBG_TGT_MINGW ) && defined( __MINGW_EXTENSION )
    // MSYS2 / MinGW defines __MINGW_EXTENSION
    // and provides _abs64() and some other stuff,
    // but the standard MinGW environment does not.
    #if !defined( MBG_TGT_HAS_ABS64 )
      #define MBG_TGT_HAS_ABS64             1
    #endif
  #endif

  #if defined( __i386__ )

    #define MBG_ARCH_I386
    #define MBG_ARCH_X86

  #elif defined( __x86_64__ )

    #define MBG_ARCH_X86_64
    #define MBG_ARCH_X86

  #elif defined( __ia64__ )

    #define MBG_ARCH_IA64

    #define _NO_USE_PACK_INTF

  #elif defined( __sparc__ )

    #define MBG_ARCH_SPARC

    #define _NO_USE_PACK_INTF

  #elif defined( __arm__ )

    #define MBG_ARCH_ARM

  #endif

  #if defined( MBG_TGT_LINUX )

    #if defined( MBG_TGT_KERNEL )

      #include <linux/types.h>
      #include <linux/version.h>

      #if ( LINUX_VERSION_CODE <= KERNEL_VERSION( 2, 6, 4 ) ) || \
          ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 4 ) )   // must be true for 2.6.32-5-sparc64
        #define _ULONG_DEFINED   1
        #define _USHORT_DEFINED  1
        #define _UINT_DEFINED    1
      #endif

      // The 'bool' type is supported by the vanilla Linux kernel 2.6.19 and later.
      #if ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 19 ) )

        // However, looks like at least the RedHat folks have backported this
        // to 2.6.18, so KERNEL_HAS_BOOL can be defined to avoid a compiler error
        // due to a duplicate definition.
        #if !defined( KERNEL_HAS_BOOL )
          typedef _Bool bool;
          #define bool bool
        #endif

        // Similar for 'true' and 'false'.
        #if !defined( KERNEL_HAS_TRUE_FALSE )
          enum
          {
            false,
            true
          };
          #define falso false
          #define true true
        #endif

      #endif

      // The 'uintptr_t' type has been introduced
      // after v2.6.24-rc1 and before v2.6.24-rc2.
      #if ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 24 ) )
        typedef unsigned long uintptr_t;
        #define uintptr_t uintptr_t
      #endif

      // String formatter codes like "PRIi64" for int64_t types
      // don't seem to be defined in Linux kernel mode, so we
      // define the required "ll" or "l" modifier here as a hack.
      // Code below uses this to define appropriate "PRIi64"-like
      // definitions as "lli" or "li".
      #if !defined( MBG_PRI_64_PREFIX )
        // Please note that e.g. Linux 2.6.16 on Itanium (ia64)
        // with gcc 4.1.2 expects "%li" to print an int64_t.
        // This may be a bug in the specific gcc version, though.
        #define MBG_PRI_64_PREFIX  "ll"
      #endif

    #else

      #include <stdint.h>
      #include <inttypes.h>
      #include <stdbool.h>

      #if defined( __u_char_defined )
        #define _ULONG_DEFINED   1
        #define _USHORT_DEFINED  1
        #define _UINT_DEFINED    1
      #endif

    #endif

    #define MBG_TGT_HAS_DEV_FN       1
    #define MBG_TGT_HAS_DEV_FN_BASE  1
    #define MBG_TGT_USE_IOCTL        1

  #elif defined( MBG_TGT_BSD )

    #if defined( MBG_TGT_KERNEL )
      #include <sys/types.h>
    #else
      #include <stdint.h>
      #include <inttypes.h>
      #include <stdbool.h>
    #endif

    #define MBG_TGT_HAS_DEV_FN       1
    #define MBG_TGT_HAS_DEV_FN_BASE  1
    #define MBG_TGT_USE_IOCTL        1

    #if defined( MBG_TGT_FREEBSD ) && defined( MBG_TGT_KERNEL )
      #if !defined( MBG_PRI_64_PREFIX )
        // String formatter codes like "PRIi64" for int64_t types
        // don't seem to be defined in FreeBSD kernel mode, so we
        // define the required "ll" or "l" modifier here as a hack.
        // Code below uses this to define appropriate "PRIi64"-like
        // definitions as "lli" or "li".
        #define MBG_PRI_64_PREFIX  "l"
      #endif
    #endif

  #elif defined( MBG_TGT_QNX_NTO )  // QNX 6.x (Neutrino)

    #include <unistd.h>
    #include <stdint.h>
    #include <inttypes.h>
    #include <stdbool.h>

  #else

    #include <stdint.h>
    #include <inttypes.h>
    #include <stdbool.h>

  #endif

  #define MBG_TGT_HAS_EXACT_SIZE_TYPES     1

  #define MBG_TGT_HAS_WCHAR_T              1


  #if defined( __clang__ )
    #define _DEPRECATED_BY( _s ) __attribute__((deprecated("use \"" _s "\" instead")))  // works with clang 3.4.1
  #elif ( _GCC_VERSION > 40500 )    // gcc 4.5.0 and newer
    #define _DEPRECATED_BY( _s ) __attribute__((deprecated("use \"" _s "\" instead")))
  #elif ( _GCC_VERSION > 30100 )  // gcc 3.1 and newer
    #define _DEPRECATED_BY( _s ) __attribute__((deprecated))
  #else
    // Not supported at all, use empty default definiton below.
  #endif

  #if ( _GCC_VERSION > 30100 )  // gcc 3.1 and newer
    #define __mbg_inline __inline__ __attribute__((always_inline))
  #else
    // Not supported at all, use empty default definiton below.
    #define __mbg_inline __inline__
  #endif

#elif defined( _MSC_VER )

  // Known predifined MS compiler version codes:
  // 1910: MSVC++ 15.0 (Visual Studio 2017)
  // 1900: MSVC++ 14.0 (Visual Studio 2015)
  // 1800: MSVC++ 12.0 (Visual Studio 2013)
  // 1700: MSVC++ 11.0 (Visual Studio 2012)
  // 1600: MSVC++ 10.0 (Visual Studio 2010)
  // 1500: MSVC++ 9.0  (Visual Studio 2008)
  // 1400: MSVC++ 8.0  (Visual Studio 2005, Windows Server 2003 SP1 DDK - AMD64)
  // 1310: MSVC++ 7.1  (Visual Studio .NET 2003, Windows Server 2003 DDK)
  // 1300: MSVC++ 7.0  (Visual Studio .NET 2002, Windows XP SP1 DDK / DDK 2600)
  // 1200: MSVC++ 6.0
  // 1100: MSVC++ 5.0

  // Enable this to get compile-time messages on the compiler version
  #if 0
    #if ( _MSC_VER >= 1910 )
      #error >= 1910: MSVC++ 15.0 (Visual Studio 2017), or later
    #elif ( _MSC_VER >= 1900 )
      #error 1900: MSVC++ 14.0 (Visual Studio 2015)
    #elif ( _MSC_VER >= 1800 )
      #error 1800: MSVC++ 12.0 (Visual Studio 2013)
    #elif ( _MSC_VER >= 1700 )
      #error 1700: MSVC++ 11.0 (Visual Studio 2012)
    #elif ( _MSC_VER >= 1600 )
      #error 1600: MSVC++ 10.0 (Visual Studio 2010)
    #elif ( _MSC_VER >= 1500 )
      #error 1500: MSVC++ 9.0  (Visual Studio 2008)
    #elif ( _MSC_VER >= 1400 )
      #error STRINGIFY( _MSC_VER ) MSVC++ 8.0  (Visual Studio 2005, Windows Server 2003 SP1 DDK - AMD64)
    #elif ( _MSC_VER >= 1310 )
      #error 1310: MSVC++ 7.1  (Visual Studio .NET 2003, Windows Server 2003 DDK)
    #elif ( _MSC_VER >= 1300 )
      #error 1300: MSVC++ 7.0  (Visual Studio .NET 2002, Windows XP SP1 DDK / DDK 2600)
    #elif ( _MSC_VER >= 1200 )
      #error 1200: MSVC++ 6.0
    #elif ( _MSC_VER >= 1100 )
      #error 1100: MSVC++ 5.0
    #else
      #error <1100: Older than MSVC 4
    #endif
  #endif

  // "struct timespec" is supported only since VS2015
  // If it is then also the symbol TIME_UTC should be defined.
  // Functions to read the current time as struct timespec
  // are timespec_get() and friends, which are also only provided
  // by VS2015 and later.
  // As of VS2015, only TIME_UTC is supported to read
  // the UTC system time, there is no equivalent for
  // the POSIX CLOCK_MONOTONIC. However, QPC can be used
  // to get monotonic time stamps and intervals.
  #if ( _MSC_VER < 1900 )
    #if !defined( HAVE_STRUCT_TIMESPEC )
      #define MBG_TGT_MISSING_STRUCT_TIMESPEC  1
    #endif
  #endif

  #if ( _MSC_VER >= 1600 )  // TODO Eventually even 1600 doesn't support this.
    #include <stdint.h>
    #include <inttypes.h>
    #define MBG_TGT_HAS_EXACT_SIZE_TYPES    1

    #if !defined( PRId64 )
      #define MBG_PRI_64_PREFIX          "I64"
    #endif
  #else
    #define MBG_TGT_HAS_INT_8_16_32         1
    #define MBG_PRI_64_PREFIX            "I64"
  #endif

  #if !defined( __cplusplus )
    // no bool support anyway
    #define MBG_TGT_MISSING_BOOL_TYPE       1
  #endif

  #define MBG_TGT_HAS_WCHAR_T               1

  #define __mbg_inline __forceinline

  // At least up to VS2008 the C99 builtin symbol __func__
  // is not supported. Some VS versions support __FUNCTION__
  // instead, but at least VC6 doesn't support this, either.
  // of the current function instead.
  #if ( _MSC_VER >= 1300 )
    #define __func__  __FUNCTION__
  #else
    #define __func__  FUNC_UNKNOWN
  #endif

  // The "deprecated" attribute should be supported since Visual Studio 2005,
  // but doesn't seem to be supported e.g. by the compiler shipped with the
  // "Windows Server 2003 SP1 DDK", which is used to build kernel drivers
  // and defines the same _MSC_VER number as VS2005. For now we assume
  // that this is supported by compilers shipped with newer DDKs.
  // The _DDK_BUILD_ symbol has to be explicitly defined in the "sources"
  // file of the DDK project.
  #if ( ( _MSC_VER >= 1500 ) || \
      ( ( _MSC_VER >= 1400 ) && !defined( _DDK_BUILD_ ) ) )
    #define _DEPRECATED_BY( _s ) __declspec(deprecated("deprecated, use \"" _s "\""))
  #endif

  // availability of _abs64()
  #if ( _MSC_VER >= 1310 )
    // This is supported at least since Visual Studio 2008
    // and Windows Server 2003 SP1 DDK.

    #if !defined( MBG_TGT_HAS_ABS64 )
      #define MBG_TGT_HAS_ABS64             1
    #endif
  #endif

  #if !defined ( HAVE_SSIZE_T )

    // ssize_t support
    #if ( _MSC_VER >= 1500 )
      // ssize_t may not be defined, but SSIZE_T is
      #include <basetsd.h>
      typedef SSIZE_T ssize_t;
    #else
      // At least VC6 hasn't SIZE_T, either, but size_t
      // is typedef'ed as unsigned int, so we just typedef
      // the signed variant here.
      typedef int ssize_t;
    #endif

    #define HAVE_SSIZE_T 1

  #endif  // !defined ( HAVE_SSIZE_T )

  #if ( _MSC_VER <= 1500 )
    // At least MSVC++ 9.0 / Visual Studio 2008 and older
    // don't provide lldiv_t and lldiv().
    #define MBG_TGT_MISSING_LLDIV_T         1
  #endif

  #if ( _MSC_VER <= 1400 )
    #define MBG_TGT_MISSING_STRTOLL         1
  #elif ( _MSC_VER <= 1600 )
    // At least MSVC++ 10.0 / Visual Studio 2010 and older
    // don't provide strtoll(), but may provide _strtoi64 instead.
    #define strtoll _strtoi64
  #endif

#elif defined( _CVI_ )

  // 1000 for CVI v10.0 (CVI 2010)
  // 911 for CVI v9.1.1 (CVI 2009 SP1)
  // 910 for CVI v9.1 (CVI 2009)
  // 310 for CVI v3.1
  // 301 for CVI v3.0.1
  // 1 for CVI v3.0

  #if ( _CVI_ >= 910 )
    // LabWindows/CVI 2009 is the first version providing stdint.h.
    #include <stdint.h>
    #include <inttypes.h>
    #define MBG_TGT_HAS_EXACT_SIZE_TYPES   1
  #else
    #define USE_LONG_FOR_INT32  1
  #endif

  // As of LabWindows/CVI 2010, stdbool.h is still missing.
  #define MBG_TGT_MISSING_BOOL_TYPE        1

  #define MBG_TGT_HAS_WCHAR_T              0

  // Inline code is not supported, though the inline keyword
  // is silently accepted since CVI v9.0

#elif defined( __BORLANDC__ )  // or __CODEGEARC__ in newer versions

  // 0x0200 Borland C/C++ 2.0
  // 0x0400 Borland C/C++ 3.0
  // 0x0410 Borland C/C++ 3.1
  // 0x0550 Borland C/C++ 5.5 (C++Builder 5.0)

  // Next codes are in addition defined as __CODEGEARC__
  // See http://docwiki.embarcadero.com

  // 0x0570 for Borland Developer Studio 2006 (BDS 2006)
  // 0x0590 for C++Builder 2007
  // 0x0591 for update 1 to C++Builder 2007
  // 0x0592 for RAD Studio 2007
  // 0x0593 for the December update to RAD Studio 2007
  // 0x0610 for C++Builder 2009 and for C++Builder 2009 Update 1
  // 0x0620 for C++Builder 2010 and for C++Builder 2010 Update 1
  // 0x0621 for C++Builder 2010 Update 2
  // 0x0630 for C++Builder XE
  // 0x0631 for C++Builder XE Update 1
  // 0x0640 for C++Builder XE2
  // 0x0650 for C++Builder XE3

  #if ( __BORLANDC__ >= 0x630 )
    // C++Builder XE starts to provide stdbool.h
    #include <stdint.h>
    #include <inttypes.h>
    #include <stdbool.h>
    #define MBG_TGT_HAS_EXACT_SIZE_TYPES       1
  #elif ( __BORLANDC__ >= 0x570 )
    // BDS/Borland C++ Builder 2006 starts to provide at least stdint.h
    #include <stdint.h>
    #include <inttypes.h>
    #define MBG_TGT_HAS_EXACT_SIZE_TYPES       1
    #if !defined( __cplusplus )
      #define MBG_TGT_MISSING_BOOL_TYPE        1
    #endif
  #elif ( __BORLANDC__ >= 0x0550 )
    #define MBG_TGT_HAS_INT_8_16_32            1
    #define MBG_PRI_64_PREFIX                   "I64"
    #if !defined( __cplusplus )
      #define MBG_TGT_MISSING_BOOL_TYPE        1
    #endif
    #define __func__  FUNC_UNKNOWN
  #else  // e.g. BC 3.1 or earlier
    #if ( __BORLANDC__ <= 0x410 )
      #define MBG_TGT_MISSING_64_BIT_TYPES     1
      #define MBG_TGT_MISSING_BOOL_TYPE        1
      #define USE_LONG_FOR_INT32               1
      #define MBG_TGT_MISSING_STRUCT_TIMESPEC  1
      #define __func__  __FILE__ ":" STRINGIFY(__LINE__)

      typedef int ssize_t;
    #endif
  #endif

  #if ( __BORLANDC__ <= 0x0550 )
    // At least CBuilder 5 and earlier.
    #define MBG_TGT_MISSING_STRTOLL          1
    #define MBG_TGT_MISSING_LLDIV_T          1
  #endif

  #define MBG_TGT_HAS_WCHAR_T  defined( MBG_TGT_WIN32 )

  #define MBG_TGT_MISSING_STRUCT_TIMESPEC      1

  #if defined( __cplusplus )
    #define __mbg_inline inline    // standard C++ syntax
  #elif ( __BORLANDC__ > 0x410 )   // BC3.1 defines 0x410 !
    #define __mbg_inline __inline  // newer BC versions support this for C
  #else
    #define __mbg_inline           // up to BC3.1 not supported for C
  #endif

  #if !defined ( HAVE_SSIZE_T )
    typedef int ssize_t;           // required at least for C++ Builder 5
    #define HAVE_SSIZE_T 1
  #endif

#elif defined( __WATCOMC__ )

  // 1050  v10.5
  // 1100  v11.0
  // 1200  Open Watcom C++ v1.0
  // 1230  Open Watcom C++ v1.3
  // 1270  Open Watcom C++ v1.7

  #if defined( MBG_TGT_QNX )    // QNX 4.x

    #include <sys/types.h>

    #define MBG_TGT_MISSING_64_BIT_TYPES   1

  #elif ( __WATCOMC__ > 1230 )  // Open Watcom C 1.3 and above

    #include <stdint.h>
    #include <inttypes.h>

  #elif !defined( __WATCOM_INT64__ )  // Watcom C 11

    #define MBG_TGT_MISSING_64_BIT_TYPES   1

  #endif

  #define MBG_TGT_HAS_WCHAR_T  defined( MBG_TGT_WIN32 )

  #define __mbg_inline _inline

#endif


#if !defined( MBG_TGT_USE_IOCTL )
  #define MBG_TGT_USE_IOCTL        0
#endif


// If the build environment doesn't provide an inttypes.h file
// with print format specifiers for 64 bit fixed size types
// then MBG_PRI_64_PREFIX should be defined, which is used
// to define our own C99 compatible format specifiers.
// Eventually, similar definitions are required for 32, 16,
// and 8 bit fixed size types.
#if defined( MBG_PRI_64_PREFIX )
  #define PRIi64  MBG_PRI_64_PREFIX "i"
  #define PRId64  MBG_PRI_64_PREFIX "d"
  #define PRIo64  MBG_PRI_64_PREFIX "o"
  #define PRIu64  MBG_PRI_64_PREFIX "u"
  #define PRIx64  MBG_PRI_64_PREFIX "x"
  #define PRIX64  MBG_PRI_64_PREFIX "X"
#endif

#if !defined( __GNUC__ ) && !defined( __attribute__ )
  #define __attribute__( _x )
#endif

#if !defined( _DEPRECATED_BY )
  #define _DEPRECATED_BY( _s )  // empty definition
#endif


#if defined( MBG_TGT_WIN32 )

  #if defined( _AMD64_ )
    // This is used for AMD64 architecture and for
    // Intel XEON CPUs with 64 bit extension.
    #define MBG_TGT_WIN32_PNP_X64
    #define WIN32_FLAVOR "x64"
  #elif defined( _IA64_ )
    #define MBG_TGT_WIN32_PNP_IA64
    #define WIN32_FLAVOR "ia64"
  #endif

  #if defined( _KDD_ )
    #define MBG_TGT_KERNEL

    #include <ntddk.h>

    #if defined( DBG ) && DBG
      #define DEBUG  1
    #endif

    #define _MBG_API
  #else
    // This must not be used for kernel drivers.

    // Prevent inclusion of obsolete winsock.h in windows.h
    #if !defined( WIN32_LEAN_AND_MEAN )
      #define WIN32_LEAN_AND_MEAN 1
    #endif

    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>

    typedef HANDLE MBG_HANDLE;

    #define MBG_INVALID_HANDLE  INVALID_HANDLE_VALUE

    #if defined( MBG_TGT_CVI )
      // CVI uses an own set of functions to support serial ports
      typedef int MBG_PORT_HANDLE;
      #define MBG_INVALID_PORT_HANDLE -1
    #else
      typedef HANDLE MBG_PORT_HANDLE;
    #endif

    // The DWORD_PTR type is not defined in the headers shipping
    // with VC6. However, if the SDK is installed then the SDK's
    // headers may declare this type. This is at least the case
    // in the Oct 2001 SDK which also defines the symbol _W64.
    #if !defined( _W64 )
      typedef DWORD DWORD_PTR;
    #endif

    // socklen_t support
    #if ( _MSC_VER < 1500 )
      // At least VS2008 has a socklen_t type
      typedef int socklen_t;
    #endif

    #define _MBG_API  WINAPI

  #endif

  #if !defined( MBG_TGT_MINGW )  // not required for MinGW
    #if defined( MBG_LIB_EXPORT )
      #define _MBG_API_ATTR __declspec( dllexport )
    #elif !defined( MBGSVCTL_STATIC_LIB )
      #define _MBG_API_ATTR __declspec( dllimport )
    #endif
  #endif

#elif defined( MBG_TGT_POSIX )

  #if !defined( MBG_TGT_KERNEL )
    #include <unistd.h>
  #endif

  typedef int MBG_HANDLE;
  typedef int MBG_PORT_HANDLE;

  #define MBG_INVALID_HANDLE  -1

#else

  typedef int MBG_HANDLE;
  typedef int MBG_PORT_HANDLE;

  #define MBG_INVALID_HANDLE  -1

#endif


/**
 * @brief A portable socket descriptor type returned by socket().
 *
 * @see ::MBG_INVALID_SOCK_FD for the value returned in case of error.
 */
#if defined( MBG_TGT_WIN32 ) // && !defined( MBG_TGT_MINGW )
  #if !defined( MBG_TGT_KERNEL )  // we don't need this in kernel space
    /// On Windows usually evaluates to @a UINT_PTR, which in turn
    /// evaluates to <em>(unsigned int)</em>, or <em>(unsigned __int64)</em>.
    typedef SOCKET MBG_SOCK_FD;
  #endif
#elif defined( MBG_TGT_POSIX )
  typedef int MBG_SOCK_FD;      //### TODO
  //### TODO  typedef int SOCKET;
#endif



/**
 * @brief The value returned by the socket() function in case of error.
 *
 * On Windows usually evaluates to (SOCKET)(~0),
 * since the SOCKET type is unsigned.
 * For POSIX systems the value is usually -1.
 *
 * If the @a socket() call returns this value,
 * call ::mbg_get_last_socket_error to retrieve
 * one of the portable @ref MBG_ERROR_CODES.
 *
 * @see ::MBG_SOCK_FD for a portable socket descriptor type.
 */
#if defined( MBG_TGT_WIN32 )
  #define MBG_INVALID_SOCK_FD    INVALID_SOCKET  // usually evaluates to (SOCKET)(~0) since SOCKET is unsigned
#elif defined( MBG_TGT_POSIX )
  #define MBG_INVALID_SOCK_FD    -1
#endif



/**
 * @brief The return code of networking functions in case of error.
 *
 * The code is returned by functions like @a select(),
 * @a recv(), etc. in case of error.
 */
#if defined( MBG_TGT_WIN32 )
  #define MBG_SOCKET_ERR_RETVAL  SOCKET_ERROR    // usually evaluates to -1
#elif defined( MBG_TGT_POSIX )
  #define MBG_SOCKET_ERR_RETVAL  -1
#endif



#if !defined( _MBG_API )
  #define _MBG_API
#endif

#if !defined( _MBG_API_ATTR )
  #define _MBG_API_ATTR
#endif

#if !defined( _NO_MBG_API )
  #define _NO_MBG_API
#endif

#if !defined( _NO_MBG_API_ATTR )
  #define _NO_MBG_API_ATTR
#endif

#if !defined( MBG_INVALID_PORT_HANDLE )
  #define MBG_INVALID_PORT_HANDLE   MBG_INVALID_HANDLE
#endif

#if !defined( MBG_TGT_HAS_DEV_FN )
  #define MBG_TGT_HAS_DEV_FN  0
#endif

#if !defined( MBG_TGT_HAS_DEV_FN_BASE )
  #define MBG_TGT_HAS_DEV_FN_BASE  0
#endif

#if defined( MBG_TGT_MISSING_STRUCT_TIMESPEC )

  #include <time.h>

  struct timespec
  {
    time_t tv_sec;
    long tv_nsec;
  };

#endif  // defined( MBG_TGT_MISSING_STRUCT_TIMESPEC )


// The macros below are defined in order to be able to check if
// certain C language extensions are available on the target system:
#if defined( __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199409L )
  #define MBG_TGT_C94    1
#else
  #define MBG_TGT_C94    0
#endif


#if defined( __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901L )
  #define MBG_TGT_C99    1
#else
  #define MBG_TGT_C99    0
#endif

// Check if wchar_t is supported
#if !defined( MBG_TGT_HAS_WCHAR_T )
  #define MBG_TGT_HAS_WCHAR_T  ( MBG_TGT_C94 || defined( WCHAR_MAX ) )
#endif

#if !MBG_TGT_HAS_WCHAR_T
  // Even if wchar_t is not natively supported by the target platform
  // there may already be a compatibility define (e.g. BC3.1)
  // However, some functions may be missing (e.g. snwprintf()).
  #if !defined( _WCHAR_T )          /* BC3.1 */ \
   && !defined( _WCHAR_T_DEFINED_ ) /* WC11 */
    #define _WCHAR_T
    #define wchar_t char
  #endif
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

#endif  // !defined( MBG_TGT_NO_TGT )

#endif  // !defined( _MBG_TGT_H )
