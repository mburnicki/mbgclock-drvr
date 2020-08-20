
/**************************************************************************
 *
 *  $Id: mbgsystm.h 1.6 2020/03/31 08:16:38 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Generic functions and definitions to deal with the computer's
 *    system time, and prototypes for mbgsystm.c.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgsystm.h $
 *  Revision 1.6  2020/03/31 08:16:38  martin
 *  Fix for Linux kernel 5.6 which doesn't support getnstimeofday() anymore.
 *  Revision 1.5  2019/02/08 11:05:48  martin
 *  Commonly use int64_t for MBG_SYS_TIME on Windows, rather than
 *  FILETIME in user space and LARGE_INTEGER in kernel space.
 *  Also use a function pointer to read the Windows system time
 *  depending on whether the precise time API calls are supported
 *  by the running Windows system, or not.
 *  Revision 1.4  2018/07/16 12:32:58  martin
 *  Fixed do_div() for 32 bit ARM Linux kernel.
 *  Include another header file on NetBSD.
 *  Revision 1.3  2017/07/26 14:26:17  martin
 *  Fixed build for NetBSD.
 *  Revision 1.2  2017/07/04 12:26:57  martin
 *  More detailed control of inclusion of other headers.
 *  Made mbg_delta_sys_time_ms() as inline function here.
 *  Updated function prototypes.
 *  Cleanup.
 *  Revision 1.1  2015/09/15 13:21:00Z  martin
 *  Initial revision.
 *  Moved existing code from different modules here.
 *
 **************************************************************************/

#ifndef _MBGSYSTM_H
#define _MBGSYSTM_H


/* Other headers to be included */

#include <mbg_tgt.h>
#include <words.h>

#if defined( MBG_TGT_POSIX )

  #if defined( MBG_TGT_KERNEL )

    #if defined( MBG_TGT_LINUX )

      // If true, getnstimeofday() has been deprecated and removed,
      // and ktime_get_real_ts64() can be used instead.
      #define LINUX_KERNEL_HAS_KT64            ( LINUX_VERSION_CODE >= KERNEL_VERSION( 5, 6, 0 ) )

      // If true, getnstimeofday() is supported and has not yet been deprecated.
      #define LINUX_KERNEL_HAS_GETNSTIMEOFDAY  ( ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 22 ) ) && !LINUX_KERNEL_HAS_KT64 )

      #define LINUX_KERNEL_HAS_MSLEEP          ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 16 ) )
      #define LINUX_KERNEL_HAS_KTIME_H         ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 16 ) )

      #if !LINUX_KERNEL_HAS_MSLEEP
        #include <linux/wait.h>
        #include <linux/sched.h>
      #endif

      // We need the prototype for getnstimeofday(). In newer kernels
      // (e.g. 4.x) this is available via linux/ktime.h, which in turn
      // includes linux/timekeeping.h, which declares the prototype.
      #if LINUX_KERNEL_HAS_KTIME_H
        #include <linux/ktime.h>
      #endif

      // In older kernel versions the prototype for getnstimeofday()
      // is declared in linux/time.h, so we include that one anyway.
      #include <linux/time.h>

      #include <linux/delay.h>
      #include <linux/jiffies.h>

    #elif defined( MBG_TGT_BSD )

      #if defined( MBG_TGT_FREEBSD )
        #include <sys/libkern.h>
      #endif

      #if defined( MBG_TGT_NETBSD )
        #include <sys/param.h>   // mstohz
        #include <sys/kernel.h>  // hz
        #include <sys/proc.h>    // kpause
      #endif

      #include <sys/time.h>

    #endif

  #else  // POSIX user space

    #include <time.h>

    #if defined( MBG_TGT_LINUX )
      #include <sys/sysinfo.h>
    #endif

  #endif

#elif defined( MBG_TGT_WIN32 )

  #if defined( MBG_TGT_KERNEL )
    #include <mbg_w32.h>   // ke_query_system_time_fnc
  #else
    #include <timeutil.h>  // gstaft_fnc
  #endif

  #include <mbgtime.h>

#elif defined( MBG_TGT_DOS )

  #include <dos.h>

#endif


#ifdef _MBGSYSTM
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Generic types to hold system timestamps and values for the system uptime..
 */
#if defined( MBG_TGT_POSIX )

  typedef NANO_TIME_64 MBG_SYS_TIME;
  typedef int64_t MBG_SYS_UPTIME;    // [s]

#elif defined( MBG_TGT_WIN32 )

  typedef int64_t MBG_SYS_TIME;      // Number of 100ns units
  typedef int64_t MBG_SYS_UPTIME;    // [s]

#elif defined( MBG_TGT_OS2 )

  typedef uint32_t MBG_SYS_TIME;   //## dummy
  typedef long MBG_SYS_UPTIME;     //## dummy

#elif defined( MBG_TGT_DOS )

  typedef uint32_t MBG_SYS_TIME;   //## dummy
  typedef long MBG_SYS_UPTIME;     //## dummy

#else // other target OSs which access the hardware directly

  typedef uint32_t MBG_SYS_TIME;   //## dummy
  typedef long MBG_SYS_UPTIME;     //## dummy

#endif

//### TODO
// MBG_SYS_TIME is always read in native machine endianess,
// so no need to convert endianess.
#define _mbg_swab_mbg_sys_time( _p ) \
  _nop_macro_fnc()



static __mbg_inline
void mbg_get_sys_time( MBG_SYS_TIME *p )
{
  #if defined( MBG_TGT_POSIX )

    #if defined( MBG_TGT_KERNEL )   // kernel space functions even differ for POSIX systems

      #if defined( MBG_TGT_LINUX )  // Linux kernel space

        #if LINUX_KERNEL_HAS_KT64
        {
          struct timespec64 ts;

          ktime_get_real_ts64( &ts );

          p->secs = ts.tv_sec;
          p->nano_secs = ts.tv_nsec;
        }
        #elif LINUX_KERNEL_HAS_GETNSTIMEOFDAY
        {
          struct timespec ts;

          getnstimeofday( &ts );

          p->secs = ts.tv_sec;
          p->nano_secs = ts.tv_nsec;
        }
        #else
        {
          // getnstimeofday() *not* supported
          struct timeval tv;

          do_gettimeofday( &tv );

          p->secs = tv.tv_sec;
          p->nano_secs = tv.tv_usec * 1000;
        }
        #endif

      #elif defined( MBG_TGT_BSD )  // BSD kernel space
      {
        struct timespec ts;

        nanotime( &ts );

        p->secs = ts.tv_sec;
        p->nano_secs = ts.tv_nsec;
      }
      #endif

    #else  // POSIX user space
    {
      struct timespec ts;

      #if defined( CLOCK_REALTIME_PRECISE )  // at least available in FreeBSD
        clock_gettime( CLOCK_REALTIME_PRECISE, &ts );
      #else
        clock_gettime( CLOCK_REALTIME, &ts );
      #endif

      p->secs = ts.tv_sec;
      p->nano_secs = ts.tv_nsec;
    }
    #endif

  #elif defined( MBG_TGT_WIN32 )

    #if defined( MBG_TGT_KERNEL )  // Windows kernel space
      ke_query_system_time_fnc( (LARGE_INTEGER *) p );
    #else                          // Windows user space
    {
      gstaft_fnc( (FILETIME *) p );
    }
    #endif

  #else

    *p = 0;  // dummy

  #endif

}  // mbg_get_sys_time



static __mbg_inline
void mbg_get_sys_uptime( MBG_SYS_UPTIME *p )
{
  #if defined( MBG_TGT_WIN32 )

    #if defined( MBG_TGT_KERNEL )  // kernel space

      ULONGLONG time_increment  = KeQueryTimeIncrement();
      LARGE_INTEGER tick_count;

      KeQueryTickCount( &tick_count );

      // multiplication by time_increment yields HNS units,
      // but we need seconds
      *p = ( tick_count.QuadPart * time_increment ) / HNS_PER_SEC;

    #else                          // user space

      DWORD tickCount;
      DWORD timeAdjustment;
      DWORD timeIncrement;
      BOOL timeAdjustmentDisabled;

      if ( !GetSystemTimeAdjustment( &timeAdjustment, &timeIncrement, &timeAdjustmentDisabled ) )
        *p = -1;  // failed

      // ATTENTION: This is compatible with older Windows versions, but
      // the returned tick count wraps around to zero after 49.7 days.
      // A new GetTickCount64() call is available under Windows Vista and newer,
      // but the function call had to be imported dynamically since otherwise
      // programs refused to start under pre-Vista versions due to undefined DLL symbol.
      tickCount = GetTickCount();

      *p = ( ( (MBG_SYS_UPTIME) tickCount ) * timeIncrement ) / HNS_PER_SEC;

    #endif

  #elif defined( MBG_TGT_LINUX )

    #if defined( MBG_TGT_KERNEL )
    // getrawmonotonic() can possibly be used for this in newer kernels
    {
      // Using a simple 64 bit division may result in a linker error
      // in kernel mode due to a missing symbol __udivdi3, so we use
      // a specific inline function do_div().
      // Also, the jiffies counter is not set to 0 at startup but to
      // a defined initialization value we need to account for.
      uint64_t tmp = get_jiffies_64() - INITIAL_JIFFIES;
      do_div( tmp, HZ );
      *p = tmp;
    }
    #else
    {
      struct sysinfo si;
      int rc = sysinfo( &si );
      *p = ( rc == 0 ) ? si.uptime : -1;
    }
    #endif

  #elif defined( MBG_TGT_BSD )

    #if defined( MBG_TGT_KERNEL )
    {
      struct timespec ts;
      #if 0  //##+++++++
      {
        struct bintime bt;

        binuptime( &bt );
        #if defined( DEBUG )
          printf( "binuptime: %lli.%09lli\n",
                  (long long) bt.sec,
                  (long long) bt.frac );
        #endif
      }
      #endif

      nanouptime( &ts );
      #if defined( DEBUG )
        printf( "nanouptime: %lli.%09lli\n",
                (long long) ts.tv_sec,
                (long long) ts.tv_nsec );
      #endif
      *p = ts.tv_sec;
    }
    #elif defined( MBG_TGT_FREEBSD )
    {
      struct timespec ts;
      // CLOCK_UPTIME_FAST is specific to FreeBSD
      int rc = clock_gettime( CLOCK_UPTIME_FAST, &ts );
      *p = ( rc == 0 ) ? ts.tv_sec : -1;
    }
    #else  // MBG_TGT_NETBSD, ...

      *p = -1;  //##++ needs to be implemented

    #endif

  #else

    *p = -1;  // not supported

  #endif

}  // mbg_get_sys_uptime



/**
 * @brief Compute delta between two ::MBG_SYS_TIME times, in milliseconds
 *
 * @param[in] t2  the time to be subtracted from
 * @param[in] t1  The time to be subtracted
 *
 * @return The time difference in [milliseconds]
 */
static __mbg_inline
long mbg_delta_sys_time_ms( const MBG_SYS_TIME *t2, const MBG_SYS_TIME *t1 )
{
  #if defined( MBG_TGT_POSIX )
    long dt = ( t2->secs - t1->secs ) * 1000;
    #if defined ( MBG_TGT_LINUX ) && defined( MBG_TGT_KERNEL )
      uint64_t tmp64 = t2->nano_secs - t1->nano_secs;
      do_div( tmp64, 1000000 );
      dt += tmp64;
    #else
      dt += ( t2->nano_secs - t1->nano_secs ) / 1000000;
    #endif
    return dt;
  #elif defined( MBG_TGT_WIN32 )
    return (long) ( ( *t2 - *t1 ) / HNS_PER_MS );
  #else
    return 0;
  #endif

}  // mbg_delta_sys_time_ms



static __mbg_inline
void mbg_sleep_sec( long sec )
{
  #if defined( MBG_TGT_POSIX )

    #if defined( MBG_TGT_KERNEL )   // kernel space functions even differ for POSIX systems

      #if defined( MBG_TGT_LINUX )  // Linux kernel space

        // msleep is not defined in older kernels, so we use this
        // only if it is surely supported.
        #if LINUX_KERNEL_HAS_MSLEEP
          msleep( sec * 1000 );
        #else
        {
          DECLARE_WAIT_QUEUE_HEAD( tmp_wait );
          wait_event_interruptible_timeout( tmp_wait, 0, sec * HZ + 1 );
        }
        #endif

      #elif defined( MBG_TGT_FREEBSD )

        struct timeval tv = { 0 };
        int ticks;
        tv.tv_sec = sec;
        ticks = tvtohz( &tv );

        #if defined( DEBUG )
          printf( "pause: %lli.%06lli (%i ticks)\n",
                  (long long) tv.tv_sec,
                  (long long) tv.tv_usec,
                  ticks );
        #endif

        pause( "pause", ticks );

      #elif defined( MBG_TGT_NETBSD )

        int timeo = mstohz( sec * 1000 );

        #if defined( DEBUG )
          printf( "kpause: %i s (%i ticks)\n", sec, timeo );
        #endif

        kpause( "pause", 1, timeo, NULL );

      #endif

    #else  // POSIX user space

      sleep( sec );

    #endif

  #elif defined( MBG_TGT_WIN32 )

    #if defined( MBG_TGT_KERNEL )  // kernel space

      LARGE_INTEGER delay;

      // we need to pass a negative value to KeDelayExecutionThread()
      // since the given time is a relative time interval, not absolute
      // time. See the API docs for KeDelayExecutionThread().
      delay.QuadPart = - ((LONGLONG) sec * HNS_PER_SEC);

      KeDelayExecutionThread( KernelMode, FALSE, &delay );

    #else                          // user space

      // Sleep() expects milliseconds
      Sleep( sec * 1000 );

    #endif

  #elif defined( MBG_TGT_DOS )

    delay( (unsigned) ( sec * 1000 ) );

  #else

    // This needs to be implemented for the target OS
    // and thus will probably yield a linker error.
    do_sleep_sec( sec );

  #endif

}  // mbg_sleep_sec



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

#endif  /* _MBGSYSTM_H */
