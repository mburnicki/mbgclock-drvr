
/**************************************************************************
 *
 *  $Id: mbgclock.h 1.29 2017/05/10 15:21:32 martin REL_M $
 *
 *  Description:
 *    Global definitions and prototypes for the mbgclock module.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgclock.h $
 *  Revision 1.29  2017/05/10 15:21:32  martin
 *  Tiny cleanup.
 *  Revision 1.28  2011/07/07 12:42:08  martin
 *  Renamed this file from mbgdrvr.h to mbgclock.h.
 *  Revision 1.27  2010/05/03 15:01:34  martin
 *  Added function pointer types for exported kernel mode functions.
 *  Revision 1.26  2010/02/23 16:02:48  martin
 *  Updated function prototypes.
 *  Revision 1.25  2009/12/21 15:53:35  martin
 *  Moved some driver-internal definitions to the .c file.
 *  Updated function prototypes.
 *  Revision 1.24  2009/09/29 14:56:54  martin
 *  Updated version number to 3.4.0.
 *  Revision 1.23  2009/07/24 09:50:07  martin
 *  Updated version number to 3.3.0.
 *  Revision 1.22  2009/06/19 12:38:51  martin
 *  Updated version number to 3.2.0.
 *  Revision 1.21  2009/03/20 11:53:42  martin
 *  Updated version number to 3.1.0.
 *  Updated copyright year to include 2009.
 *  Revision 1.20  2008/12/22 13:30:32  martin
 *  Migrated former file mbginit.h to mbgdrvr.h.
 *  Updated revision number and string to 3.0.0.
 *  Moved definition of compatibility macros to mbg_lx.h.
 *  Revision 1.19  2007/07/20 11:04:53  martin
 *  New module version 2.09.
 *  Added order of header inclusion to fix build problems under RH9.
 *  Revision 1.18  2007/03/02 10:53:08  martin
 *  New module version 2.08.
 *  Account for the file_ops flush function taking the POSIX lock owner ID
 *  as additional parameter in kernel 2.6.17 and newer.
 *  Account for interrupt functions not receiving a register set around
 *  kernel 2.6.19 and newer.
 *  Source code reordered and cleaned up.
 *  Revision 1.17  2006/08/28 11:16:27  martin
 *  New version number 2.07.
 *  Added the optional module parameter pretend_sync which makes the driver
 *  pretend to NTP the device is always synchronized.
 *  Revision 1.16  2006/07/07 12:04:37  martin
 *  Use a different way to check whether kernel supports irqreturn_t.
 *  Checking for modversions requirement moved to mbg_lx.h.
 *  Revision 1.15  2006/07/03 13:21:25  martin
 *  New version number 2.06.
 *  Added copyright string macro.
 *  Define symbols used for conditional compilation only if that symbols
 *  haven't been defined before, e.g. on the command line.
 *  Account for IRQ return types also being supported in newer 2.4.x kernels.
 *  Revision 1.14  2005/06/02 15:52:33  martin
 *  New version number 2.05.
 *  New conditional USE_WAIT_EVENT.
 *  Define macros pci_set_drvdata and pci_get_drvdata for older
 *  kernels which dont provide those.
 *  Revision 1.13  2005/01/14 12:13:58  martin
 *  Fixed default kernel version for ESPIPE_BY_VFS.
 *  Revision 1.12  2004/11/09 09:40:20  martin
 *  New module revision number 2.04.
 *  Modifications to support kernel 2.6.
 *  Modified inclusion of headers.
 *  Defined preprocessor symbol ESPIPE_BY_VFS which is used to conditionally
 *  remove ESPIPE logic from from read/write functions since this is handled
 *  by the VFS layer in recent kernels.
 *  Defined __devexit_p for early hotplug kernels which haven't defined it.
 *  Revision 1.11  2003/07/30 08:45:35  martin
 *  New module revision number 2.03.
 *  Revision 1.10  2003/07/08 15:19:41  martin
 *  New module revision number 2.02.
 *  Removed obsolete preprocessor definitions.
 *  Revision 1.9  2003/04/25 10:24:03  martin
 *  New module revision number 2.01.
 *  Revision 1.8  2002/11/21 10:21:42  martin
 *  New module revision number 0.6.
 *  Revision 1.7  2002/06/03 15:09:32  martin
 *  New module revision number 0.5.
 *  Revision 1.6  2002/05/07 07:56:49  martin
 *  New module revision number 0.4.
 *  Revision 1.5  2001/12/03 15:57:01  martin
 *  New REV_NUM 0.03.
 *  Revision 1.4  2001/09/17 13:58:06  MARTIN
 *  New REV_NUM 0.02.
 *  Revision 1.3  2001/07/27 13:27:01  MARTIN
 *  New preprocessor symbols to control code generation.
 *  Include linux/init.h instead of asm/init.h.
 *  Revision 1.2  2001/03/28 07:42:03  MARTIN
 *  Defined preprocessor symbols to account for different "fasync"
 *  and "wait_queue" APIs depending on the kernel version.
 *  Revision 1.1  2001/03/05 17:19:28  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#ifndef _MBGDRVR_H
#define _MBGDRVR_H


/* Other headers to be included */

#include <pcpsdrvr.h>

#ifdef _MBGDRVR
  #define _ext
  #define _DO_INIT
#else
  #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif

// If any functions exported by the mbgclock module are to be called from
// other modules then the types below can be used to define function pointers
// allowing a type-safe import of the exported functions using the kernel's
// symbol_put() / symbol_get() API.

typedef int (*MBGCLOCK_DEFAULT_GET_FAST_HR_TIMESTAMP_FNC)( PCPS_TIME_STAMP *p_ts ) ;
typedef int (*MBGCLOCK_DEFAULT_GET_FAST_HR_TIMESTAMP_CYCLES_FNC)( PCPS_TIME_STAMP_CYCLES *p_ts_cyc ) ;

typedef int (*MBGCLOCK_DEFAULT_CLR_UCAP_BUFF_FNC)( void );
typedef int (*MBGCLOCK_DEFAULT_GET_UCAP_ENTRIES_FNC)( PCPS_UCAP_ENTRIES *p );
typedef int (*MBGCLOCK_DEFAULT_GET_UCAP_EVENT_FNC)( PCPS_HR_TIME *p );



/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
    Read a high resolution ::PCPS_TIME_STAMP structure via memory mapped access.
    This function can be called from other kernel drivers and reads the timestamp
    from the first device registered by the driver which supports this call.

    @param *p_ts Pointer to a ::PCPS_TIME_STAMP structure to be filled up

    @return ::MBG_SUCCESS on success, or MBG_ERR_NOT_SUPP_BY_DEV if no device
            supports this call.

    @see mbgclock_default_get_fast_hr_timestamp_cycles()
*/
 int mbgclock_default_get_fast_hr_timestamp( PCPS_TIME_STAMP *p_ts ) ;

 /**
    Read a high resolution ::PCPS_TIME_STAMP_CYCLES structure via memory mapped access.
    This function can be called from other kernel drivers and reads the timestamp
    from the first device registered by the driver which supports this call, plus
    an associated cycles counter which can be used to determine the latency.

    @param *p_ts_cyc Pointer to a ::PCPS_TIME_STAMP_CYCLES structure to be filled up

    @return ::MBG_SUCCESS on success, or MBG_ERR_NOT_SUPP_BY_DEV if no device
            supports this call.

    @see mbgclock_default_get_fast_hr_timestamp()
*/
 int mbgclock_default_get_fast_hr_timestamp_cycles( PCPS_TIME_STAMP_CYCLES *p_ts_cyc ) ;

 /**
    Clear the user capture FIFO buffer on the first device registered
    by the driver which supports this call.

    @return ::MBG_SUCCESS on success, or
            MBG_ERR_NOT_SUPP_BY_DEV if no device supports this call, or
            MBG_ERR_IRQ_UNSAFE called on a device where such calls are unsafe
            if IRQs are enabled.

    @see mbgclock_default_get_ucap_entries()
    @see mbgclock_default_get_ucap_event()
*/
 int mbgclock_default_clr_ucap_buff( void ) ;

 /**
    Read a ::PCPS_UCAP_ENTRIES structure to retrieve the number of saved
    user capture events and the maximum capture buffer sizefrom the first
    device registered by the driver which supports this call.

    @return ::MBG_SUCCESS on success, or
            MBG_ERR_NOT_SUPP_BY_DEV if no device supports this call, or
            MBG_ERR_IRQ_UNSAFE called on a device where such calls are unsafe
            if IRQs are enabled.

    @see mbgclock_default_clr_ucap_buff()
    @see mbgclock_default_get_ucap_event()
*/
 int mbgclock_default_get_ucap_entries( PCPS_UCAP_ENTRIES *p ) ;

 /**
    Retrieve a single time capture event from the on-board FIFO buffer of the
    first device registered by the driver which supports this call.
    The captured event time is returned in a ::PCPS_HR_TIME structure.
    The oldest entry of the FIFO is retrieved and then removed from the FIFO.
    If no capture event is available in the FIFO buffer then both the seconds
    and the fractions of the returned timestamp are 0.

    @return ::MBG_SUCCESS on success, or
            MBG_ERR_NOT_SUPP_BY_DEV if no device supports this call, or
            MBG_ERR_IRQ_UNSAFE called on a device where such calls are unsafe
            if IRQs are enabled.

    @see mbgclock_default_clr_ucap_buff()
    @see mbgclock_default_get_ucap_entries()
*/
 int mbgclock_default_get_ucap_event( PCPS_HR_TIME *p ) ;


/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif


/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _MBGDRVR_H */
