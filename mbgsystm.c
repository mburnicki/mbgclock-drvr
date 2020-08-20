
/**************************************************************************
 *
 *  $Id: mbgsystm.c 1.2 2017/07/05 10:06:36 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Generic functions to deal with the computer's system time
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgsystm.c $
 *  Revision 1.2  2017/07/05 10:06:36  martin
 *  Made mbg_delta_sys_time_ms() an inline function,
 *  so this file is now effectively empty.
 *  Revision 1.1  2015/09/15 13:21:00  martin
 *  Initial revision.
 *  Moved existing code from different modules here.
 *
 **************************************************************************/

#define _MBGSYSTM
 #include <mbgsystm.h>
#undef _MBGSYSTM


#if 0

  /*HDR*/
/**
 * @brief Compute delta between two ::MBG_SYS_TIME times, in milliseconds
 *
 * @param[in] t2  the time to be subtracted from
 * @param[in] t1  The time to be subtracted
 *
 * @return The time difference in [milliseconds]
 */
long mbg_delta_sys_time_ms( const MBG_SYS_TIME *t2, const MBG_SYS_TIME *t1 )
{
  #if defined( MBG_TGT_POSIX )
    long dt = ( t2->secs - t1->secs ) * 1000;
    #if defined ( MBG_TGT_LINUX ) && defined( MBG_TGT_KERNEL )
      int64_t tmp64 = t2->nano_secs - t1->nano_secs;
      do_div( tmp64, 1000000 );
      dt += tmp64;
    #else
      dt += ( t2->nano_secs - t1->nano_secs ) / 1000000;
    #endif
    return dt;
  #elif defined( MBG_TGT_WIN32 )
    return (long) ( ( t2->QuadPart - t1->QuadPart ) / HNS_PER_MS );
  #else
    return 0;
  #endif

}  // mbg_delta_sys_time_ms

#endif

