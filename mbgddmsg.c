
/**************************************************************************
 *
 *  $Id: mbgddmsg.c 1.5 2018/11/22 14:12:48 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Functions called from user space to access Meinberg device drivers.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgddmsg.c $
 *  Revision 1.5  2018/11/22 14:12:48  martin
 *  Support QNX target.
 *  Assume DEBUG_MSG_SLEEP is always defined but evaluates to 'true' or 'false'.
 *  Revision 1.4  2018/09/21 14:46:12  martin
 *  More consistent naming.
 *  Revision 1.3  2018/06/25 11:05:51  martin
 *  Unified kernel driver messages.
 *  Conditionally support DEBUG_MSG_SLEEP.
 *  Added mbg_kdd_snprintf() for Linux and Windows.
 *  mbg_kdd_msg() also takes a log level.
 *  Support log levels on Windows.
 *  Evaluate log level under Linux.
 *  Includes some headers already in mbgddmsg.h.
 *  Revision 1.2  2017/07/05 14:27:43  martin
 *  Distinguish more between Windows and Linux targets.
 *  Revision 1.1  2014/04/09 15:41:33  martin
 *  Initial version.
 *
 **************************************************************************/

#define _MBGDDMSG
  #include <mbgddmsg.h>
#undef _MBGDDMSG

#if defined( MBG_TGT_KERNEL )

  #if defined( MBG_TGT_WIN32 )

    extern void KddReportEventA( int lvl, const char *info );

  #elif defined( MBG_TGT_LINUX )

    #if DEBUG_MSG_SLEEP
      #include <linux/delay.h>
    #endif

    extern const char driver_name[];

  #elif defined( MBG_TGT_BSD )

    extern const char driver_name[];

  #endif

#endif


#if USE_MBG_KDD_MSG

/*HDR*/
__attribute__( ( format( printf, 3, 4 ) ) )
int mbg_kdd_snprintf( char *buf, size_t size, const char *fmt, ... )
{
  va_list args;
  int n;

  va_start( args, fmt );
  n = mbg_kdd_vsnprintf( buf, size, fmt, args );
  va_end( args );

  return n;

}  // mbg_kdd_snprintf



/*HDR*/
__attribute__( ( format( printf, 2, 3 ) ) )
void mbg_kdd_msg( int lvl, const char *fmt, ... )
{
  va_list args;
  char s[256];

  va_start( args, fmt );
  mbg_kdd_vsnprintf( s, sizeof( s ), fmt, args );
  va_end( args );

  #if defined( MBG_TGT_WIN32 )

    KddReportEventA( lvl, s );

  #elif defined( MBG_TGT_LINUX )
  {
    const char *cp;

    switch ( lvl )  // see linux/kern_levels.h
    {
      case MBG_LOG_ERR:
        cp = KERN_ERR;
        break;

      case MBG_LOG_WARN:
        cp = KERN_WARNING;
        break;

      case MBG_LOG_INFO:
        cp = KERN_INFO;
        break;

      case MBG_LOG_DEBUG:
        cp = KERN_DEBUG;
        break;

      default:
        #if defined( KERN_DEFAULT )
          cp = KERN_DEFAULT;
        #else
          cp = KERN_WARNING;
        #endif
        break;
    }

    printk( "%s%s: %s\n", cp, driver_name, s );

    #if DEBUG_MSG_SLEEP
      msleep( debug_msg_sleep );
    #endif
  }
  #elif defined( MBG_TGT_BSD )

    printf( "%s: %s\n", driver_name, s );

  #elif defined( MBG_TGT_QNX )

    printf( "%s\n", s );

  #elif defined( MBG_TGT_DOS )

    printf( "%s\n", s );

  #else

    #error Not implemented for this target.

  #endif

}  // mbg_kdd_msg

#endif  // USE_MBG_KDD_MSG

