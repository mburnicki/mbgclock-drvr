
/**************************************************************************
 *
 *  $Id: mbgmutex.h 1.5 2018/07/16 14:18:05 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Portable macros to deal with spinlocks, mutexes,
 *    and critical sections.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgmutex.h $
 *  Revision 1.5  2018/07/16 14:18:05  martin
 *  Support ID strings for spinlocks and mutexes.
 *  Revision 1.4  2017/07/05 10:05:02  martin
 *  windows.h is now included by mbg_tgt.h.
 *  Check for MBG_TGT_POSIX instead of MBG_TGT_UNIX.
 *  Revision 1.3  2013/04/11 13:46:58  martin
 *  Use non-specific spinlock function under Windows.
 *  Revision 1.2  2012/03/08 12:19:01Z  martin
 *  Fixes for Linux kernel and FreeBSD.
 *  Fixed build under DOS and QNX usinc dummy defines.
 *  Don't define macros for semaphore destroy functions
 *  if not required/supported on the target OS.
 *  Revision 1.1  2011/04/15 12:26:59  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _MBGMUTEX_H
#define _MBGMUTEX_H


/* Other headers to be included */

#include <mbg_tgt.h>
#include <words.h>

#ifdef _MBGMUTEX
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif

#if defined( MBG_TGT_KERNEL )        // definitions used in kernel space

  #if defined( MBG_TGT_WIN32   )     // Windows kernel space

    typedef KSPIN_LOCK MBG_SPINLOCK;
    #define _mbg_spin_lock_init( _spl, _n ) KeInitializeSpinLock( _spl )
    // _mbg_spin_lock_destroy               is not supported
    #define _mbg_spin_lock_acquire( _spl )  KeAcquireSpinLock( _spl, &OldIrql )
    #define _mbg_spin_lock_release( _spl )  KeReleaseSpinLock( _spl, OldIrql )

    #define _MBG_SPINLOCK_DEFINED  1


    typedef FAST_MUTEX MBG_MUTEX;
    #define _mbg_mutex_init( _pmtx, _n )   ExInitializeFastMutex( _pmtx )
    // _mbg_mutex_destroy( _pmtx )         is not supported
    #define _mbg_mutex_acquire( _pmtx )    ExAcquireFastMutex( _pmtx )
    #define _mbg_mutex_release( _pmtx )    ExReleaseFastMutex( _pmtx )

    #define _MBG_MUTEX_DEFINED  1

  #elif defined( MBG_TGT_LINUX )     // Linux kernel space

    #include <linux/spinlock.h>
    #include <linux/version.h>

    #if ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 26 ) )
      #include <linux/semaphore.h>
    #else
      #include <asm/semaphore.h>
    #endif

    typedef spinlock_t MBG_SPINLOCK;
    #define _mbg_spin_lock_init( _spl, _n ) spin_lock_init( _spl )
    // _mbg_spin_lock_destroy               is not supported
    #define _mbg_spin_lock_acquire( _spl )  spin_lock( _spl )
    #define _mbg_spin_lock_release( _spl )  spin_unlock( _spl )

    #define _MBG_SPINLOCK_DEFINED  1


    typedef struct semaphore MBG_MUTEX;
    #define _mbg_mutex_init( _pmtx, _n )   sema_init( _pmtx, 1 )
    // _mbg_mutex_destroy( _pmtx )         is not supported
    #define _mbg_mutex_acquire( _pmtx )    down_interruptible( _pmtx )
    #define _mbg_mutex_release( _pmtx )    up( _pmtx )

    #define _MBG_MUTEX_DEFINED  1

  #elif defined( MBG_TGT_FREEBSD )   // FreeBSD kernel space

    #include <sys/lock.h>
    #include <sys/mutex.h>

    typedef struct mtx MBG_SPINLOCK;
    #define _mbg_spin_lock_init( _spl, _n ) mtx_init( _spl, _n, NULL, MTX_SPIN )
    #define _mbg_spin_lock_destroy( _spl )  mtx_destroy( _spl )
    #define _mbg_spin_lock_acquire( _spl )  mtx_lock_spin( _spl )
    #define _mbg_spin_lock_release( _spl )  mtx_unlock_spin( _spl )

    #define _MBG_SPINLOCK_DEFINED  1


    typedef struct mtx MBG_MUTEX;
    #define _mbg_mutex_init( _pmtx, _n )    mtx_init( _pmtx, _n, NULL, MTX_DEF )
    #define _mbg_mutex_destroy( _pmtx )     mtx_destroy( _pmtx )
    #define _mbg_mutex_acquire( _pmtx )     mtx_lock( _pmtx )
    #define _mbg_mutex_release( _pmtx )     mtx_unlock( _pmtx )

    #define _MBG_MUTEX_DEFINED  1

  #elif defined( MBG_TGT_NETBSD )

    #include <sys/mutex.h>

    // The API used below has been introduced in NetBSD 5.0
    // For earlier NetBSD versions see 'man 9 lockinit'.

    typedef kmutex_t MBG_SPINLOCK;
    #define _mbg_spin_lock_init( _spl, _n ) mutex_init( _spl, MUTEX_DEFAULT, IPL_HIGH )
    #define _mbg_spin_lock_destroy( _spl )  mutex_destroy( _spl )
    #define _mbg_spin_lock_acquire( _spl )  mutex_spin_enter( _spl )
    #define _mbg_spin_lock_release( _spl )  mutex_spin_exit( _spl )

    #define _MBG_SPINLOCK_DEFINED  1


    typedef kmutex_t MBG_MUTEX;
    #define _mbg_mutex_init( _pmtx, _n )   mutex_init( _pmtx, MUTEX_DEFAULT, IPL_NONE )
    #define _mbg_mutex_destroy( _spl )     mutex_destroy( _spl )
    #define _mbg_mutex_acquire( _pmtx )    mutex_enter( _pmtx )
    #define _mbg_mutex_release( _pmtx )    mutex_exit( _pmtx )

    #define _MBG_MUTEX_DEFINED  1

  #endif

#else  // user space applications

  #if defined( MBG_TGT_WIN32 )       // Windows user space

    // definitions used with mutexes
    typedef HANDLE  MBG_MUTEX;
    #define _mbg_mutex_init( _pm, _n ) *(_pm) = CreateMutex( NULL, FALSE, NULL )
    #define _mbg_mutex_destroy( _pm )  do { CloseHandle( *(_pm) ); *(_pm) = INVALID_HANDLE_VALUE; } while ( 0 )
    #define _mbg_mutex_acquire( _pm )  WaitForSingleObject( *(_pm), INFINITE )
    #define _mbg_mutex_release( _pm )  ReleaseMutex( *(_pm) )

    #define _MBG_MUTEX_DEFINED  1

    // definitions used with critical sections
    typedef CRITICAL_SECTION MBG_CRIT_SECT;
    #define _mbg_crit_sect_init( _pcs, _n ) InitializeCriticalSection( (_pcs) )
    #define _mbg_crit_sect_destroy( _pcs )  DeleteCriticalSection( (_pcs) )
    #define _mbg_crit_sect_enter( _pcs )    EnterCriticalSection( (_pcs) )
    #define _mbg_crit_sect_leave( _pcs )    LeaveCriticalSection( (_pcs) )

    #define _MBG_CRIT_SECT_DEFINED  1

  #elif defined( MBG_TGT_POSIX )    // Unix user space use pthread library

    #include <pthread.h>

    // Mutex types:
    //   PTHREAD_MUTEX_INITIALIZER                /* Fast */
    //   PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP   /* Recursive */
    //   PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP  /* Errorcheck */
    typedef pthread_mutex_t MBG_MUTEX;
    #define _mbg_mutex_init( _pm, _n ) pthread_mutex_init( (_pm), NULL )
    #define _mbg_mutex_destroy( _pm )  pthread_mutex_destroy( (_pm) )
    #define _mbg_mutex_acquire( _pm )  pthread_mutex_lock( (_pm) )
    #define _mbg_mutex_release( _pm )  pthread_mutex_unlock( (_pm) )

    #define _MBG_MUTEX_DEFINED  1

    // For critical sections use defaults specified below

  #elif defined( MBG_TGT_DOS ) || defined( MBG_TGT_QNX )

    typedef int MBG_MUTEX;  // just a dummy declaration

    #define _MBG_MUTEX_DEFINED  1

  #endif

#endif


#if !defined( _MBG_SPINLOCK_DEFINED )

  #define _mbg_spin_lock_init( _spl, _n )  _nop_macro_fnc()
  // _mbg_spin_lock_destroy                is not supported
  #define _mbg_spin_lock_acquire( _spl )   _nop_macro_fnc()
  #define _mbg_spin_lock_release( _spl )   _nop_macro_fnc()

#endif


#if !defined( _MBG_MUTEX_DEFINED )

  #define _MBG_MUTEX_DEFINED  1

  typedef MBG_CRIT_SECT MBG_MUTEX;

  #define _mbg_mutex_init( _pm, _n ) _nop_macro_fnc()
  // _mbg_mutex_destroy( _pmtx )     is not supported
  #define _mbg_mutex_acquire( _pm )  _nop_macro_fnc()
  #define _mbg_mutex_release( _pm )  _nop_macro_fnc()

#endif


#if !defined( _MBG_CRIT_SECT_DEFINED )

  // use mutex by default, e.g. with the pthread library

  #define _MBG_CRIT_SECT_DEFINED  1

  typedef MBG_MUTEX  MBG_CRIT_SECT;
  #define _mbg_crit_sect_init       _mbg_mutex_init
  #if defined( _mbg_mutex_destroy )
    #define _mbg_crit_sect_destroy  _mbg_mutex_destroy
  #endif
  #define _mbg_crit_sect_enter      _mbg_mutex_acquire
  #define _mbg_crit_sect_leave      _mbg_mutex_release

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

#endif  /* _MBGMUTEX_H */
