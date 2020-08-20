
/**************************************************************************
 *
 *  $Id: macioctl.h 1.46 2020/06/19 13:26:07 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Macros used inside the IOCTL handlers of device drivers
 *    for Meinberg PCI and USB devices.
 *
 * -----------------------------------------------------------------------
 *  $Log: macioctl.h $
 *  Revision 1.46  2020/06/19 13:26:07  martin
 *  IOCTL errors in kernel space are now logged.
 *  Revision 1.45  2020/02/24 17:04:05  martin
 *  Removed obsolete stuff.
 *  Revision 1.44  2019/04/03 12:18:11  martin
 *  Copying data in and out is now also protected by the device semaphore.
 *  Made SPIN_BUFFER obsolete, use local variables for these few cases.
 *  Gave a more plausible name to a common variable.
 *  Added a couple of comments.
 *  Revision 1.43  2019/03/11 16:15:50  martin
 *  Conditionally use local I/O buffers by again. This may avoid a
 *  potential data corruption problem with PCI devices, but won't work
 *  with USB devices on Linux kernels 4.9 and later.
 *  Revision 1.42  2019/02/08 10:28:27  martin
 *  Fixed potential data corruption in IOCTL_GET_TIME_INFO_HRT which
 *  could have occured with file versions after 1.38, when we started
 *  using a DMA-capable, common buffer to meet USB requirements in
 *  Linux kernels 4.9+.
 *  Revision 1.41  2018/11/22 13:59:08  martin
 *  Refactored debug logging stuff.
 *  Fixed pointer size problems in a mixed 32/64 bit Linux environment.
 *  Revision 1.40  2018/08/20 16:06:42  martin
 *  Use current log level definitions.
 *  Account for renamed library symbols.
 *  Revision 1.39  2018/07/05 08:36:45  martin
 *  Moved some definitions to mbgioctl.h.
 *  Use a DMA capable buffer in the device data structure
 *  for I/O. Required for USB by Linux kernels 4.9+.
 *  Use safer macro syntax.
 *  Renamed some jump labels.
 *  Account for renamed library symbol NSEC_PER_SEC.
 *  Use memory access macros.
 *  Unified kernel driver messages.
 *  Separated test code that should be moved elsewhere.
 *  Revision 1.38  2017/07/05 14:20:58  martin
 *  Support GNSS, GPIO and xmulti_ref stuff.
 *  Support new way to check if a feature is supported.
 *  Attribute always_inline is now in __mbg_inline.
 *  Fixed macro definition syntax to avoid clang compiler warnings.
 *  Account for frac_sec_from_bin() obsoleted by bin_frac_32_to_dec_frac().
 *  IOCTL debugging code.
 *  Removed trailing spaces.
 *  Revision 1.37  2014/05/19 15:57:25  martin
 *  Fixed grammar in a comment.
 *  Revision 1.36  2013/09/26 07:54:22  martin
 *  Support GNSS API.
 *  Revision 1.35  2013/04/11 13:46:05  martin
 *  Account for modified spinlock handling under Windows.
 *  Revision 1.34  2012/10/02 18:29:49Z  martin
 *  Account for some renamed library symbols.
 *  Account for renamed structures.
 *  Support on-board event logs.
 *  Support debug status.
 *  Made inline functions static.
 *  Include cfg_hlp.h.
 *  Always read receiver info directly from the device.
 *  Conditionally use older IOCTL request buffer structures.
 *  Modified generic IOCTL handling such that for calls requiring variable sizes
 *  a fixed request block containing input and output buffer pointers and sizes is
 *  passed down to the kernel driver. This simplifies implementation under *BSD
 *  and also works for other target systems.
 *  Support reading CORR_INFO, and reading/writing TR_DISTANCE.
 *  Support IOCTL_DEV_HAS_PZF.
 *  Use a single union type buffer instead of a large number of local
 *  variables in ioctl_switch().
 *  The accumulated size of all local variables required much stack
 *  space which led to problems under Windows.
 *  Support PTP unicast configuration.
 *  Account for some IOCTL codes renamed to follow common naming conventions.
 *  Use common mutex primitives from mbgmutex.h.
 *  Changes by Frank Kardel: Don't require copyin/copyout under NetBSD.
 *  Support NetBSD beside FreeBSD.
 *  Account for modified _pcpc_kfree().
 *  Bug fix: Use PCPS_TIME_STAMP with
 *  IOCTL_GET_FAST_HR_TIMESTAMP as output size.
 *  Specify I/O range number when calling port I/O macros
 *  so they can be used for different ranges under BSD.
 *  Modified inline declarations for gcc.
 *  Fixed build under FreeBSD.
 *  Added _sem_inc_safe_no_irp() macro (Windows only).
 *  Simplified code and renamed some inline functions.
 *  Fixed macro.
 *  Revision 1.33  2009/12/21 16:22:55  martin
 *  Moved code reading memory mapped timestamps to inline functions.
 *  Revision 1.32  2009/12/15 15:34:57  daniel
 *  Support reading the raw IRIG data bits for firmware versions
 *  which support this feature.
 *  Revision 1.31  2009/11/04 14:58:52Z  martin
 *  Conditionally exclude port status query from build.
 *  Revision 1.30  2009/09/29 15:08:39  martin
 *  Support retrieving time discipline info.
 *  Revision 1.29  2009/08/18 08:45:16  martin
 *  Removed IOCTL switch macro, inline code used for all targets.
 *  Revision 1.28  2009/06/26 13:16:11Z  martin
 *  Fixed duplicate case in inline code (copy and paste error).
 *  Revision 1.27  2009/06/22 13:52:56  martin
 *  Fixed a bug where the size of GPS data had been truncated to 8 bits,
 *  which resulted in an IOCTL error if a buffer larger than 256 bytes had been
 *  used. This had been observed with the PC_GPS_ALL_STR_TYPE_INFO
 *  command if more than 6 string types are supported by a card.
 *  Revision 1.26  2009/06/19 12:21:12  martin
 *  Support reading raw IRIG time.
 *  Revision 1.25  2009/06/09 10:01:01  daniel
 *  Support configuration of LAN intf. and PTP.
 *  Started to support ARM / firmware.
 *  Conditionally compile ioctl_switch as inline function.
 *  Revision 1.24  2009/03/19 15:25:19  martin
 *  Support UTC parms and configurable time scales.
 *  Support IOCTL_DEV_HAS_IRIG_CTRL_BITS and IOCTL_GET_IRIG_CTRL_BITS.
 *  Support reading MM timestamps without cycles.
 *  IOCTL_GET_PCI_ASIC_VERSION now returns the ASIC
 *  version code from the device info structure which already
 *  has the correct endianess.
 *  For consistent naming renamed IOCTL_GET_FAST_HR_TIMESTAMP
 *  to IOCTL_GET_FAST_HR_TIMESTAMP_CYCLES.
 *  Use mbg_get_cycles...() instead of _pcps_get_cycles...().
 *  Revision 1.23  2008/12/11 10:30:38Z  martin
 *  _pcps_get_cycles() is now called inside the low level routines
 *  immediately when the command byte is written.
 *  Mutex for hardware access is now acquired/released in _pcps_sem_inc()
 *  and _pcps_sem_dec(), so other IOCTLs which don't access the card
 *  can be run in parallel.
 *  Moved definitions of _pcps_sem_inc(), _pcps_sem_dec(), and
 *  _pcps_get_cycles() to pcpsdrvr.h.
 *  Defined a macro which checks if access is safe (may be unsafe
 *  with certain PEX cards which have IRQs enabled).
 *  Use _pcps_sem_inc_safe() macro to check if access is safe and
 *  inhibit access if this is not the case.
 *  Consistenly use pcps_drvr_name instead of mbgclock_name
 *  for debug messages.
 *  Don't return error for unmap_mm...() under Linux.
 *  Account for ASIC_FEATURES being coded as flags, and account
 *  for new symbol PCI_ASIC_HAS_MM_IO.
 *  Handle new IOCTLs IOCTL_HAS_PCI_ASIC_FEATURES, IOCTL_HAS_PCI_ASIC_VERSION,
 *  IOCTL_DEV_IS_MSF, IOCTL_DEV_IS_LWR, IOCTL_DEV_IS_WWVB,
 *  IOCTL_GET_IRQ_STAT_INFO, IOCTL_GET_CYCLES_FREQUENCY,
 *  IOCTL_HAS_FAST_HR_TIMESTAMP, and IOCTL_GET_FAST_HR_TIMESTAMP.
 *  Support mapped I/O resources.
 *  Revision 1.22  2008/01/17 09:28:49  daniel
 *  Support for memory mapped I/O under Linux and Windows.
 *  Added macros _io_get_mapped_mem_address(),
 *  _io_unmap_mapped_mem_address().
 *  Account for IOCTL_GET_PCI_ASIC_FEATURES
 *  Cleanup for PCI ASIC version.
 *  Revision 1.21  2007/09/26 07:31:47Z  martin
 *  Support reading status port of USB devices.
 *  Use kernel malloc/free macros from pcpsdrvr.h.
 *  Modified _pcps_sem..() to take PCPS_DDEV argument.
 *  Revision 1.20  2007/05/21 15:00:00Z  martin
 *  Unified naming convention for symbols related to ref_offs.
 *  Revision 1.19  2007/03/30 13:31:42  martin
 *  Changes due to renamed library symbol.
 *  Revision 1.18  2007/03/02 10:31:21Z  martin
 *  Use generic port I/O macros.
 *  Preliminary support for *BSD.
 *  Preliminary _cmd_from_ioctl().
 *  Revision 1.17  2006/03/10 10:35:43  martin
 *  Added support for programmable pulse outputs.
 *  Revision 1.16  2005/06/02 10:16:37Z  martin
 *  Implemented IOCTL_PCPS_GENERIC_.. calls.
 *  Added support for SYNTH_STATE.
 *  Moved Debug IOCTL handling here.
 *  Revision 1.15  2005/01/14 10:26:41Z  martin
 *  Support IOCTLs which query device features.
 *  Revision 1.14  2004/12/09 11:03:36Z  martin
 *  Support configuration of on-board frequency synthesizer.
 *  Revision 1.13  2004/11/09 12:47:19Z  martin
 *  Use new macro _pcps_ddev_has_gps_data() to check whether GPS large
 *  data I/O is supported.
 *  Changes due to renamed symbols, IRIG RX/TX.
 *  Modifications were required in order to be able to configure IRIG
 *  settings of cards which provide both IRIG input and output.
 *  GPS169PCI cards with IRIG output and early firmware versions
 *  used the same codes to configure the IRIG output as the TCR
 *  cards use to configure the IRIG input. Those codes are now
 *  exclusively used to configure the IRIG input. A workaround
 *  has been included for those GPS169PCI cards, because otherwise
 *  the IRIG configuration would not work properly after a driver
 *  update, without also doing a firmware update.
 *  Show debug msg if GPS169PCI workaround for IRIG cfg in effect.
 *  Use more specific data types than generic types.
 *  Modified contents of debug messages.
 *  Added (uchar FAR *) cast.
 *  Revision 1.12  2004/06/07 09:20:52  martin
 *  Account for renamed symbols.
 *  Revision 1.11  2004/04/07 09:05:17  martin
 *  Support OS dependent IOCTLs used to trigger debug events.
 *  Revision 1.10  2004/03/16 16:25:42Z  martin
 *  Support new macro _pcps_has_irig().
 *  Revision 1.9  2004/01/08 10:57:23Z  martin
 *  Support codes to read ASIC version, and read times
 *  with associated cycle counter values.
 *  Support higher baud rates for TCR510PCI and PCI510.
 *  Support PCPS_HR_TIME for TCR510PCI.
 *  Revision 1.8  2003/09/17 12:49:57Z  martin
 *  Use PCPS_GIVE_TIME_NOCLEAR in API mbg_get_time().
 *  Revision 1.7  2003/09/09 13:33:55Z  martin
 *  Support IOCTL_GET_PCPS_TIME_SEC_CHANGE.
 *  Revision 1.6  2003/06/19 09:18:02Z  martin
 *  Supports new APIs IOCTL_PCPS_GET_UCAP_ENTRIES
 *  and IOCTL_PCPS_GET_UCAP_EVENT.
 *  Changes due to renamed symbols.
 *  Preliminary _pout_size for Linux.
 *  Revision 1.5  2003/04/15 08:50:38Z  martin
 *  Support ALL_STR_TYPE_INFO, ALL_PORT_INFO for Win32.
 *  Revision 1.4  2003/04/09 16:51:29Z  martin
 *  Use new common IOCTL codes from mbgioctl.h.
 *  Support almost all IOCTL codes.
 *  Support for Win32.
 *  Revision 1.3  2001/11/30 09:52:47Z  martin
 *  Added support for event_time which, however, requires
 *  a custom GPS firmware.
 *  Revision 1.2  2001/09/14 12:01:17  martin
 *  Decode PCPS_IOCTL_SET_GPS_CMD.
 *  Added some comments.
 *  Revision 1.1  2001/04/09 07:47:01  MARTIN
 *
 **************************************************************************/

#ifndef _MACIOCTL_H
#define _MACIOCTL_H

#include <mbgioctl.h>
#include <cfg_hlp.h>
#include <pcpsdrvr.h>
#include <pci_asic.h>
#include <mbgddmsg.h>


// OS dependent primitives
#if defined( MBG_TGT_LINUX )

  // This is helpful for debugging, for example in a mixed
  // 32/64 bit environment where we pass 32 bit user space pointers
  // down to a 64 bit kernel.
  #if DEBUG_IOCTL
    #define _debug_iob_ptr( _p, _sz, _info )                   \
      _mbg_kdd_msg_4( MBG_LOG_INFO, "%s: 0x%llx --> %p (%lu)", \
                      _info, ( (unsigned long long) (_p) ),    \
                      (void *) (_p), (ulong) (_sz) )
  #else
    #define _debug_iob_ptr(  _p, _sz, _info )  _nop_macro_fnc()
  #endif


  #define _iob_to_pout( _piob, _pout, _size )                       \
  do                                                                \
  {                                                                 \
    _debug_iob_ptr( _pout, _size, "outp" );                         \
                                                                    \
    if ( copy_to_user( (void *) (uintptr_t) _pout, _piob, _size ) ) \
      rc = MBG_ERR_COPY_TO_USER;                                    \
  } while (  0 )


  #define _iob_from_pin( _piob, _pin, _size )                         \
  do                                                                  \
  {                                                                   \
    _debug_iob_ptr( _pin, _size, "inp" );                             \
                                                                      \
    if ( copy_from_user( _piob, (void *)  (uintptr_t) _pin, _size ) ) \
      rc = MBG_ERR_COPY_FROM_USER;                                    \
  } while (  0 )


  #define _io_wait_pcps_sec_change( _pddev, _cmd, _type, _pout ) \
    goto err_unsupp_ioctl  // not supported under Linux


  // FIXME TODO Eventually this doesn't work with PCPS_BUS_PCI_MBGPEX,
  // and it won't work with USB devices.
  // Actually we don't use this anyway.
  #define _io_get_mapped_mem_address( _pddev, _pout )                                                        \
  do                                                                                                         \
  {                                                                                                          \
    p_tmp->mapped_mem.pfn_offset = ( pddev->rsrc_info.mem[0].start_raw & ~PAGE_MASK ) + sizeof( PCI_ASIC );  \
    p_tmp->mapped_mem.len = pddev->rsrc_info.mem[0].len - sizeof( PCI_ASIC );                                \
    _iob_to_pout_var( p_tmp->mapped_mem, _pout );                                                            \
  } while (  0 )


  #define _io_unmap_mapped_mem_address( _pddev, _pin ) \
    _nop_macro_fnc()

#elif defined( MBG_TGT_BSD )

  #include <sys/malloc.h>

  #define _iob_to_pout( _piob, _pout, _size ) \
    memcpy( _pout, _piob, _size )

  #define _iob_from_pin( _piob, _pin, _size ) \
    memcpy( _piob, _pin, _size )

  #define _frc_iob_to_pout( _piob, _pout, _size ) \
    copyout( _piob, _pout, _size )

  #define _frc_iob_from_pin( _piob, _pin, _size ) \
    copyin( _pin, _piob, _size )

  #define _io_wait_pcps_sec_change( _pddev, _cmd, _type, _pout ) \
    goto err_unsupp_ioctl  // not supported under *BSD

  #define _io_get_mapped_mem_address( _pddev, _pout ) \
    goto err_unsupp_ioctl  // not supported under *BSD

  #define _io_unmap_mapped_mem_address( _pddev, _pin ) \
    goto err_unsupp_ioctl  // not supported under *BSD

#elif defined( MBG_TGT_WIN32 )

  // RtlCopyMemory() doesn't provide a return code.

  #define _iob_to_pout( _piob, _pout, _size ) \
  do                                          \
  {                                           \
    RtlCopyMemory( _pout, _piob, _size );     \
    *ret_size = _size;                        \
                                              \
  } while ( 0 )

  #define _iob_from_pin( _piob, _pin, _size ) \
    RtlCopyMemory( _piob, _pin, _size )

  // The following macros are defined in the OS dependent code:
  //
  //   _io_wait_pcps_sec_change()
  //   _io_get_mapped_mem_address()
  //   _io_unmap_mapped_mem_address()
  //   _io_set_interrupt()
  //

#endif



#if !defined( _frc_iob_to_pout )
  #define _frc_iob_to_pout   _iob_to_pout
#endif

#if !defined( _frc_iob_from_pin )
  #define _frc_iob_from_pin  _iob_from_pin
#endif


#define _iob_to_pout_var( _iob, _pout ) \
  _iob_to_pout( &(_iob), _pout, sizeof( _iob ) )

#define _iob_from_pin_var( _iob, _pin ) \
  _iob_from_pin( &(_iob), _pin, sizeof( _iob ) )



// For some cards it may be unsafe to access the card while
// interrups are enabled for the card since IRQs may during
// access may mess up the interface. The macro below checks
// whether this is the case.
#define _pcps_access_is_unsafe( _pddev ) \
  ( ( (_pddev)->irq_stat_info & PCPS_IRQ_STATE_DANGER ) == PCPS_IRQ_STATE_DANGER )



// Check whether a card can be accessed safely and set a flag
// preventing the card from being accessed from IRQ handler.
#if defined( MBG_TGT_WIN32 )

  // Under Windows we need to save a pointer to the current
  // IRP by default.
  #define _pcps_sem_inc_safe( _pddev )      \
  do                                        \
  {                                         \
    if ( _pcps_access_is_unsafe( _pddev ) ) \
      goto err_busy_irq_unsafe;             \
                                            \
    _pcps_sem_inc( _pddev );                \
    (_pddev)->p_irp = p_irp;                \
                                            \
  } while ( 0 )

  // If a function which is exported by our kernel driver
  // is called from a different kernel driver then there is
  // no IRP, so we provide a different, Windows-only macro
  // which is used by those export functions and sets the
  // IRP pointer of the device structure to NULL.
  #define _pcps_sem_inc_safe_no_irp( _pddev ) \
  do                                          \
  {                                           \
    if ( _pcps_access_is_unsafe( _pddev ) )   \
      goto err_busy_irq_unsafe;               \
                                              \
    _pcps_sem_inc( _pddev );                  \
    (_pddev)->p_irp = NULL;                   \
                                              \
  } while ( 0 )

#else

  // Other OSs don't use an IRP, so no IRP pointer
  // needs to be set up.
  #define _pcps_sem_inc_safe( _pddev )      \
  do                                        \
  {                                         \
    if ( _pcps_access_is_unsafe( _pddev ) ) \
      goto err_busy_irq_unsafe;             \
                                            \
    _pcps_sem_inc( _pddev );                \
                                            \
  } while ( 0 )

#endif



/**
 * @defgroup group_ioctl_base_macros Basic macros used to implemment IOCTL functions
 *
 * General functions that read a data structure from a device into a buffer,
 * check the return code and, if no error occurred, copy the data up to user space,
 * or copy data from user space to a buffer and write the buffer to the device.
 *
 * If device access fails, goto a label which handles the error.
 *
 * Since USB devices on Linux require a DMA-capable I/O buffer we use a common
 * buffer which is part of the device's private data. Since this buffer is shared
 * between processes the code which copies the data from or to user space has to be
 * protected by the device semaphore in the same way as the actual device access.
 *
 * @{ */

/**
 * @brief Check if a condition is true.
 *
 * If condition is false then goto an error label.
 */
#define _io_chk_cond( _cond )  \
do                             \
{                              \
  if ( !(_cond) )              \
    goto err_not_supp_by_dev;  \
                               \
} while ( 0 )



/**
 * @brief Read a standard data structure from a device.
 *
 * If access fails goto an error label.
 */
#define _io_read_var( _pddev, _cmd, _fld, _pout )        \
do                                                       \
{                                                        \
  _pcps_sem_inc_safe( _pddev );                          \
                                                         \
  rc = _pcps_read_var( _pddev, _cmd, p_dev_iob->_fld );  \
                                                         \
  if ( mbg_rc_is_success( rc ) )                         \
    _iob_to_pout_var( p_dev_iob->_fld, _pout );          \
                                                         \
  _pcps_sem_dec( _pddev );                               \
                                                         \
  if ( mbg_rc_is_error( rc ) )                           \
    goto err_dev_access;                                 \
                                                         \
} while ( 0 )



/**
 * @brief Write a standard data structure to a device.
 *
 * If access fails goto an error label.
 */
#define _io_write_var( _pddev, _cmd, _fld, _pin )         \
do                                                        \
{                                                         \
  _pcps_sem_inc_safe( _pddev );                           \
  _iob_from_pin_var( p_dev_iob->_fld, _pin );             \
  rc = _pcps_write_var( _pddev, _cmd, p_dev_iob->_fld );  \
  _pcps_sem_dec( _pddev );                                \
                                                          \
  if ( mbg_rc_is_error( rc ) )                            \
    goto err_dev_access;                                  \
                                                          \
} while ( 0 )



/**
 * @brief Write a command byte to the device.
 *
 * If access fails goto an error label.
 *
 * We don't have a buffer here that needs to be protected.
 */
#define _io_write_cmd( _pddev, _cmd )     \
do                                        \
{                                         \
  _pcps_sem_inc_safe( _pddev );           \
  rc = _pcps_write_byte( _pddev, _cmd );  \
  _pcps_sem_dec( _pddev );                \
                                          \
  if ( mbg_rc_is_error( rc ) )            \
    goto err_dev_access;                  \
                                          \
} while ( 0 )



/**
 * @brief Write a large data structure with variable size to a device.
 *
 * First check if the device supports large (GPS) data structures.
 *
 * If access fails goto an error label.
 */
#define _io_read_gps( _pddev, _cmd, _fld, _pout, _size )                      \
do                                                                            \
{                                                                             \
  _io_chk_cond( _pcps_ddev_has_gps_data( _pddev ) );                          \
                                                                              \
  _pcps_sem_inc_safe( _pddev );                                               \
                                                                              \
  rc = pcps_read_gps( _pddev, _cmd, (uchar FAR *) &p_dev_iob->_fld, _size );  \
                                                                              \
  if ( mbg_rc_is_success( rc ) )                                              \
    _iob_to_pout( &p_dev_iob->_fld, _pout, _size );                           \
                                                                              \
  _pcps_sem_dec( _pddev );                                                    \
                                                                              \
  if ( mbg_rc_is_error( rc ) )                                                \
    goto err_dev_access;                                                      \
                                                                              \
} while ( 0 )



/**
 * @brief Read a large data structure with fixed size from a device.
 *
 * First check if the device supports large (GPS) data structures.
 *
 * If access fails goto an error label.
 */
#define _io_read_gps_var( _pddev, _cmd, _fld, _pout )        \
do                                                           \
{                                                            \
  _io_chk_cond( _pcps_ddev_has_gps_data( _pddev ) );         \
                                                             \
  _pcps_sem_inc_safe( _pddev );                              \
                                                             \
  rc = _pcps_read_gps_var( _pddev, _cmd, p_dev_iob->_fld );  \
                                                             \
  if ( mbg_rc_is_success( rc ) )                             \
    _iob_to_pout_var( p_dev_iob->_fld, _pout );              \
                                                             \
  _pcps_sem_dec( _pddev );                                   \
                                                             \
  if ( mbg_rc_is_error( rc ) )                               \
    goto err_dev_access;                                     \
                                                             \
} while ( 0 )



/**
 * @brief Write a large data structure with fixed size to a device.
 *
 * First check if the device supports large (GPS) data structures.
 *
 * If access fails goto an error label.
 */
#define _io_write_gps_var( _pddev, _cmd, _fld, _pin )         \
do                                                            \
{                                                             \
  _io_chk_cond( _pcps_ddev_has_gps_data( _pddev ) );          \
                                                              \
  _pcps_sem_inc_safe( _pddev );                               \
  _iob_from_pin_var( p_dev_iob->_fld, _pin );                 \
  rc = _pcps_write_gps_var( _pddev, _cmd, p_dev_iob->_fld );  \
  _pcps_sem_dec( _pddev );                                    \
                                                              \
  if ( mbg_rc_is_error( rc ) )                                \
    goto err_dev_access;                                      \
                                                              \
} while ( 0 )


/** @} defgroup group_ioctl_base_macros */



/**
 * @defgroup group_ioctl_ext_macros Extended macros used to implemment IOCTL functions
 *
 * These macros are used to implement IOCTL calls that are not supported
 * by every device. If the feature is supported they use one of the
 * @ref group_ioctl_base_macros to access the device, but goto an error label
 * if the feature is not supported.
 *
 * @{ */

#define _io_read_var_chk( _pddev, _cmd, _fld, _pout, _cond )  \
do                                                            \
{                                                             \
  _io_chk_cond( _cond );                                      \
  _io_read_var( _pddev, _cmd, _fld, _pout );                  \
                                                              \
} while ( 0 )


#define _io_write_var_chk( _pddev, _cmd, _fld, _pin, _cond )  \
do                                                            \
{                                                             \
  _io_chk_cond( _cond );                                      \
  _io_write_var( _pddev, _cmd, _fld, _pin );                  \
                                                              \
} while ( 0 )


#define _io_write_cmd_chk( _pddev, _cmd, _cond )  \
do                                                \
{                                                 \
  _io_chk_cond( _cond );                          \
  _io_write_cmd( _pddev, _cmd );                  \
                                                  \
} while ( 0 )


#define _io_read_gps_chk( _pddev, _cmd, _fld, _pout, _size, _cond )  \
do                                                                   \
{                                                                    \
  _io_chk_cond( _cond );                                             \
                                                                     \
  if ( (size_t) _size > sizeof( p_dev_iob->_fld ) )                  \
    goto err_inval_param;                                            \
                                                                     \
  _io_read_gps( _pddev, _cmd, _fld, _pout, _size );                  \
                                                                     \
} while ( 0 )


#define _io_read_gps_var_chk( _pddev, _cmd, _fld, _pout, _cond )  \
do                                                                \
{                                                                 \
  _io_chk_cond( _cond );                                          \
  _io_read_gps_var( _pddev, _cmd, _fld, _pout );                  \
                                                                  \
} while ( 0 )


#define _io_write_gps_var_chk( _pddev, _cmd, _fld, _pin, _cond )  \
do                                                                \
{                                                                 \
  _io_chk_cond( _cond );                                          \
  _io_write_gps_var( _pddev, _cmd, _fld, _pin );                  \
                                                                  \
} while ( 0 )


#define _report_cond( _cond, _pout )    \
do                                      \
{                                       \
  p_tmp->i = _cond;                     \
  _iob_to_pout_var( p_tmp->i, _pout );  \
                                        \
} while ( 0 )

/** @} defgroup group_ioctl_ext_macros */



#define _mbg_dbg_set_bit( _d, _v )                    \
do                                                    \
{                                                     \
  mbg_dbg_data |= (_v);                               \
  _mbg_outp8( (_d), 0, mbg_dbg_port, mbg_dbg_data );  \
                                                      \
} while ( 0 )


#define _mbg_dbg_clr_bit( _d, _v )                    \
do                                                    \
{                                                     \
  mbg_dbg_data &= ~(_v);                              \
  _mbg_outp8( (_d), 0, mbg_dbg_port, mbg_dbg_data );  \
                                                      \
} while ( 0 )


#define _mbg_dbg_clr_all( _d )                        \
do                                                    \
{                                                     \
  mbg_dbg_data = 0;                                   \
  _mbg_outp8( (_d), 0, mbg_dbg_port, mbg_dbg_data );  \
                                                      \
} while ( 0 )



#if ( 0 && defined( MBG_TGT_LINUX ) )
  #define TEST_MM_ACCESS_TIME  1
#else
  #define TEST_MM_ACCESS_TIME  0
#endif



#if defined( __GNUC__ )
// Avoid "no previous prototype" with some gcc versions.
static __mbg_inline
void swap_tstamp( PCPS_TIME_STAMP *p_ts );
#endif

static __mbg_inline
void swap_tstamp( PCPS_TIME_STAMP *p_ts )
{
  uint32_t tmp = p_ts->sec;
  p_ts->sec = p_ts->frac;
  p_ts->frac = tmp;

}  // swap_tstamp



#if TEST_MM_ACCESS_TIME

#if defined( __GNUC__ )
// Avoid "no previous prototype" with some gcc versions.
static __mbg_inline
void test_mm_access_time( PCPS_DDEV *pddev );
#endif

static __mbg_inline
void test_mm_access_time( PCPS_DDEV *pddev )
{
  uint64_t _MBG_IOMEM *p64;
  uint32_t _MBG_IOMEM *p32;
  PCPS_TIME_STAMP tmp;
  MBG_PC_CYCLES cyc_1;
  MBG_PC_CYCLES cyc_2;
  long delta_frac;
  unsigned delta_ns;

  if ( pddev->mm_tstamp_addr == NULL )
    return;

  p64 = (uint64_t _MBG_IOMEM *) pddev->mm_tstamp_addr;
  p32 = (uint32_t _MBG_IOMEM *) pddev->mm_tstamp_addr;

  // Read current cycles twice to check how long it takes to read a cycle.
  mbg_get_pc_cycles( &cyc_1 );
  mbg_get_pc_cycles( &cyc_2 );

  // Read a timestamp as a single 64 bit access.
  mbg_get_pc_cycles( &cyc_1 );
  *( (volatile uint64_t *) &tmp ) = *p64;
  mbg_get_pc_cycles( &cyc_2 );

  // Read a timestamp as a 2 subsequent 32 bit accesses.
  mbg_get_pc_cycles( &cyc_1 );
  tmp.frac = _mbg_mmrd32_to_cpu( p32 );
  tmp.sec = _mbg_mmrd32_to_cpu( p32 + 1 );
  mbg_get_pc_cycles( &cyc_2 );

  // Read only the 32 bit fractions part of a timestamp.
  mbg_get_pc_cycles( &cyc_1 );
  tmp.frac = _mbg_mmrd32_to_cpu( *p32 );
  mbg_get_pc_cycles( &cyc_2 );
  tmp.sec = 0;

#if 0  // TODO
  delta_frac = (long) ( tmp.frac - p_ts->frac );
  delta_ns = (unsigned) bin_frac_32_to_dec_frac( delta_frac, NSEC_PER_SEC );

  _mbg_kdd_msg_3( MBG_LOG_INFO, "MM tstamp dev %04X: %li/%li cyc (%lu kHz)"
          " %08lX.%08lX->%08lX.%08lX: %li (%u.%03u us)",
          _pcps_ddev_dev_id( pddev ),
          (long) ( cyc_2 - cyc_1 ),
          (long) ( cyc_3 - cyc_2 ),
          (ulong) cpu_khz,
          (ulong) p_ts->sec, (ulong) p_ts->frac,
          (ulong) tmp.sec, (ulong) tmp.frac,
          (long) ( tmp.frac - p_ts->frac ),
          delta_ns / 1000,
          delta_ns % 1000
        );
#endif

}  // test_mm_access_time

#endif  // TEST_MM_ACCESS_TIME



#if defined( __GNUC__ )
// Avoid "no previous prototype" with some gcc versions.
static __mbg_inline
void do_get_fast_hr_timestamp_safe( PCPS_DDEV *pddev, PCPS_TIME_STAMP *p_ts );
#endif

static __mbg_inline
void do_get_fast_hr_timestamp_safe( PCPS_DDEV *pddev, PCPS_TIME_STAMP *p_ts )
{
  if ( pddev->mm_tstamp_addr )
  {
    #if defined( MBG_TGT_WIN32 )
      KIRQL OldIrql;
    #endif
    uint32_t _MBG_IOMEM *p = (uint32_t _MBG_IOMEM *) pddev->mm_tstamp_addr;

    _mbg_spin_lock_acquire( &pddev->tstamp_lock );
    p_ts->frac = _mbg_mmrd32_to_cpu( p );
    p_ts->sec = _mbg_mmrd32_to_cpu( p + 1 );
    _mbg_spin_lock_release( &pddev->tstamp_lock );
  }
  else
    p_ts->sec = p_ts->frac = 0;

}  // do_get_fast_hr_timestamp_safe



#if defined( __GNUC__ )
// Avoid "no previous prototype" with some gcc versions.
static __mbg_inline
void do_get_fast_hr_timestamp_cycles_safe( PCPS_DDEV *pddev, PCPS_TIME_STAMP_CYCLES *p_ts_cyc );
#endif

static __mbg_inline
void do_get_fast_hr_timestamp_cycles_safe( PCPS_DDEV *pddev, PCPS_TIME_STAMP_CYCLES *p_ts_cyc )
{
  if ( pddev->mm_tstamp_addr )
  {
    #if defined( MBG_TGT_WIN32 )
      KIRQL OldIrql;
    #endif

    uint32_t _MBG_IOMEM *p = (uint32_t _MBG_IOMEM *) pddev->mm_tstamp_addr;

    _mbg_spin_lock_acquire( &pddev->tstamp_lock );
    mbg_get_pc_cycles( &p_ts_cyc->cycles );
    p_ts_cyc->tstamp.frac = _mbg_mmrd32_to_cpu( p );
    p_ts_cyc->tstamp.sec  = _mbg_mmrd32_to_cpu( p + 1 );
    _mbg_spin_lock_release( &pddev->tstamp_lock );
  }
  else
  {
    mbg_get_pc_cycles( &p_ts_cyc->cycles );
    p_ts_cyc->tstamp.sec = p_ts_cyc->tstamp.frac = 0;
  }

}  // do_get_fast_hr_timestamp_cycles_safe



#if defined( __GNUC__ )
// Avoid "no previous prototype" with some gcc versions.
static __mbg_inline
int ioctl_switch( PCPS_DDEV *pddev, unsigned int ioctl_code,
                  #if defined( MBG_TGT_WIN32 )
                    IRP *p_irp, int *ret_size, uint16_t pout_size,
                  #endif
                  void *pin, void *pout );
#endif

/**
 * @brief Decode and handle IOCTL commands
 *
 * This function is called from the OS-specific IOCTL handlers, so eventually
 * the calling function needs to translate the Meinberg return codes to appropriate
 * OS-specific return codes.
 *
 * @param[in]  pddev       Pointer to the device structure
 * @param[in]  ioctl_code  The IOCTL code to be handled
 */
#if defined( MBG_TGT_WIN32 )
/**
 * @param[in]  p_irp       The IRP associated with the IOCTL call
 * @param[in]  ret_size    The number of bytes to be returned
 * @param[in]  pout_size   The size of the output buffer
 */
#endif
 /**
 * @param[in]  pin         The input buffer
 * @param[in]  pout        The output buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
static __mbg_inline
int ioctl_switch( PCPS_DDEV *pddev, unsigned int ioctl_code,
                  #if defined( MBG_TGT_WIN32 )
                    IRP *p_irp, int *ret_size, uint16_t pout_size,
                  #endif
                  void *pin, void *pout )
{
  #if USE_DEBUG_PORT
    MBG_PC_CYCLES cyc;
  #endif

  // A buffer used for normal I/O operations that require interaction
  // with the device's microcontroller. Linux kernel 4.9 and newer
  // require that I/O buffers used with USB devices are DMA capable.
  #if USE_LOCAL_IO_BUFFER
    // This type of buffer is located on the stack, so this is not
    // shared between different processes, and actually only the
    // real device access had to be protected by the device mutex.
    // However, this type of buffer is *not* DMA capable, and thus
    // we don't use this approach by default.
    PCPS_IO_BUFFER io_buffer;
    PCPS_IO_BUFFER *p_dev_iob = &io_buffer;
  #else
    // This type of buffer is part of the device specific data, which
    // is in fact DMA-capable, and thus we use this approach by default.
    // The disadvantage is that this buffer is shared between all processes
    // that access the same device, so not only the real device access
    // itself needs to be protected by the device semaphore, but copying
    // data in from and out to user space, too.
    PCPS_IO_BUFFER *p_dev_iob = &pddev->io_buffer;
  #endif

  // A buffer used for generic IOCTL operations that may use
  // one of the buffers above in addition. We also declare a pointer
  // to the buffer so we can use the same macro style as for the
  // I/O buffers above.
  IOCTL_BUFFER ioctl_buffer;
  IOCTL_BUFFER *p_tmp = &ioctl_buffer;

  #if USE_IOCTL_GENERIC_REQ
    void *p_buff_in;
    void *p_buff_out;
  #else
    IOCTL_GENERIC_BUFFER *p_buff;
    int buffer_size;
  #endif

  uint8_t pcps_cmd;
  int rc = MBG_SUCCESS;
  int sys_rc = 0;
  const char *log_info = NULL;
  int log_severity = 0;

  // To provide best maintainability the sequence of cases here should match
  // the sequence in ioctl_get_required_privilege(), which also makes sure
  // commands requiring lowest latency are handled first.

  switch ( ioctl_code )
  {
    // Commands requiring lowest latency

    case IOCTL_GET_FAST_HR_TIMESTAMP:
    {
      // This API call is *NOT* supported for USB devices, so the buffer doesn't
      // have to be DMA-capable. Thus we can use a local variable as buffer, and
      // we don't need to protect copying to user space.
      PCPS_TIME_STAMP pcps_time_stamp;

      _io_chk_cond( _pcps_ddev_has_fast_hr_timestamp( pddev ) );
      do_get_fast_hr_timestamp_safe( pddev, &pcps_time_stamp );
      _iob_to_pout_var( pcps_time_stamp, pout );

      rc = MBG_SUCCESS;
      break;
    }


    case IOCTL_GET_PCPS_HR_TIME:
      _io_read_var_chk( pddev, PCPS_GIVE_HR_TIME, pcps_hr_time, pout,
                        _pcps_ddev_has_hr_time( pddev ) );
      break;


    case IOCTL_GET_FAST_HR_TIMESTAMP_CYCLES:
    {
      // This API call is *NOT* supported for USB devices, so the buffer doesn't
      // have to be DMA-capable. Thus we can use a local variable as buffer, and
      // we don't need to protect copying to user space.
      PCPS_TIME_STAMP_CYCLES pcps_time_stamp_cycles;

      _io_chk_cond( _pcps_ddev_has_fast_hr_timestamp( pddev ) );
      do_get_fast_hr_timestamp_cycles_safe( pddev, &pcps_time_stamp_cycles );
      _iob_to_pout_var( pcps_time_stamp_cycles, pout );

      rc = MBG_SUCCESS;
      break;
    }


    case IOCTL_GET_PCPS_HR_TIME_CYCLES:
      _pcps_sem_inc_safe( pddev );

      rc = _pcps_read_var( pddev, PCPS_GIVE_HR_TIME, p_dev_iob->pcps_hr_time_cycles.t );

      p_dev_iob->pcps_hr_time_cycles.cycles = pddev->acc_cycles;

      if ( mbg_rc_is_success( rc ) )
        _iob_to_pout_var( p_dev_iob->pcps_hr_time_cycles, pout );

      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

      break;


    case IOCTL_GET_PCPS_UCAP_EVENT:
      _io_read_var_chk( pddev, PCPS_GIVE_UCAP_EVENT, pcps_hr_time,
                        pout, _pcps_ddev_has_ucap( pddev ) );
      break;


    // Other low latency commands

    case IOCTL_GET_PCPS_TIME:
      _io_read_var( pddev, PCPS_GIVE_TIME_NOCLEAR, pcps_time, pout );
      break;


    case IOCTL_GET_PCPS_TIME_CYCLES:
      _pcps_sem_inc_safe( pddev );

      rc = _pcps_read_var( pddev, PCPS_GIVE_TIME_NOCLEAR, p_dev_iob->pcps_time_cycles.t );

      p_dev_iob->pcps_time_cycles.cycles = pddev->acc_cycles;

      if ( mbg_rc_is_success( rc ) )
        _iob_to_pout_var( p_dev_iob->pcps_time_cycles, pout );

      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

      break;


  #if !defined( OMIT_STATUS_PORT )
    case IOCTL_GET_PCPS_STATUS_PORT:
      if ( _pcps_ddev_is_usb( pddev ) )
        _io_read_var( pddev, PCPS_GET_STATUS_PORT, pcps_status_port_usb, pout );
      else
      {
        PCPS_STATUS_PORT pcps_status_port = _pcps_ddev_read_status_port( pddev );
        _iob_to_pout_var( pcps_status_port, pout );
      }
      break;
  #endif


    case IOCTL_GET_PCPS_TIME_SEC_CHANGE:
      _io_wait_pcps_sec_change( pddev, PCPS_GIVE_TIME, PCPS_TIME, pout );
      break;


    case IOCTL_GET_GPS_TIME:
      _io_read_gps_var( pddev, PC_GPS_TIME, ttm, pout );
      break;


    case IOCTL_GET_GPS_UCAP:
      _io_read_gps_var( pddev, PC_GPS_UCAP, ttm, pout );
      break;


    case IOCTL_GET_TIME_INFO_HRT:
      _io_chk_cond( _pcps_ddev_has_hr_time( pddev ) );

      // This API call is also supported for USB devices, so we have to
      // use the DMA-capable common buffer provided by the device data,
      // and as a consequence also the code that reads the current
      // system time and cycles needs to be inside the critical section
      // protected by the semaphore, as well as the code that copies the
      // structure up to user space.
      _pcps_sem_inc_safe( pddev );

      mbg_get_pc_cycles( &p_dev_iob->mbg_time_info_hrt.sys_time_cycles.cyc_before );
      mbg_get_sys_time( &p_dev_iob->mbg_time_info_hrt.sys_time_cycles.sys_time );
      mbg_get_pc_cycles( &p_dev_iob->mbg_time_info_hrt.sys_time_cycles.cyc_after );

      rc = _pcps_read_var( pddev, PCPS_GIVE_HR_TIME, p_dev_iob->mbg_time_info_hrt.ref_hr_time_cycles.t );
      p_dev_iob->mbg_time_info_hrt.ref_hr_time_cycles.cycles = pddev->acc_cycles;

      if ( mbg_rc_is_success( rc ) )
        _iob_to_pout_var( p_dev_iob->mbg_time_info_hrt, pout );

      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

      break;


    case IOCTL_GET_TIME_INFO_TSTAMP:
    {
      // This API call is *NOT* supported for USB devices, so the buffer doesn't
      // have to be DMA-capable. Thus we can use a local variable as buffer for
      // the data to be returned, and neither the code that reads the current
      // system time and cycles nor copying to user space has to be inside
      // a critical section protected by a semaphore.
      MBG_TIME_INFO_TSTAMP mbg_time_info_tstamp;

      _io_chk_cond( _pcps_ddev_has_fast_hr_timestamp( pddev ) );

      mbg_get_pc_cycles( &mbg_time_info_tstamp.sys_time_cycles.cyc_before );
      mbg_get_sys_time( &mbg_time_info_tstamp.sys_time_cycles.sys_time );
      mbg_get_pc_cycles( &mbg_time_info_tstamp.sys_time_cycles.cyc_after );

      do_get_fast_hr_timestamp_cycles_safe( pddev, &mbg_time_info_tstamp.ref_tstamp_cycles );
      rc = MBG_SUCCESS;

      _iob_to_pout_var( mbg_time_info_tstamp, pout );
      break;
    }


    // Commands returning public status information

    case IOCTL_GET_PCPS_DRVR_INFO:
      _iob_to_pout_var( drvr_info, pout );
      break;


    case IOCTL_GET_PCPS_DEV:
      _iob_to_pout_var( pddev->dev, pout );
      break;


    case IOCTL_GET_PCPS_SYNC_TIME:
      _io_read_var_chk( pddev, PCPS_GIVE_SYNC_TIME, pcps_time,
                        pout, _pcps_ddev_has_sync_time( pddev ) );
      break;


    case IOCTL_GET_GPS_SW_REV:
      _io_read_gps_var( pddev, PC_GPS_SW_REV, sw_rev, pout );
      break;


    case IOCTL_GET_GPS_BVAR_STAT:
      _io_read_gps_var( pddev, PC_GPS_BVAR_STAT, bvar_stat, pout );
      break;


    case IOCTL_GET_GPS_ANT_INFO:
      _io_read_gps_var( pddev, PC_GPS_ANT_INFO, ant_info, pout );
      break;


    case IOCTL_GET_GPS_STAT_INFO:
      _io_read_gps_var( pddev, PC_GPS_STAT_INFO, stat_info, pout );
      break;


    case IOCTL_GET_GPS_IDENT:
      _io_read_gps_var( pddev, PC_GPS_IDENT, ident, pout );
      break;


    case IOCTL_GET_GPS_RECEIVER_INFO:
      // FIXME TODO Call common function used by probe routine.
      // Always read the receiver info directly from the device. Never
      // just return a previous copy which has been read earlier since
      // something may just have been changed by a configuration API call.
      _io_read_gps_var_chk( pddev, PC_GPS_RECEIVER_INFO,
                            receiver_info, pout,
                            _pcps_ddev_has_receiver_info( pddev ) );
      break;


    case IOCTL_GET_PCI_ASIC_VERSION:
      _io_chk_cond( _pcps_ddev_has_asic_version( pddev ) );
      _iob_to_pout_var( pddev->raw_asic_version, pout );
      rc = MBG_SUCCESS;
      break;


    case IOCTL_GET_SYNTH_STATE:
      _io_read_var_chk( pddev, PCPS_GET_SYNTH_STATE, synth_state,
                        pout, _pcps_ddev_has_synth( pddev ) );
      break;


    case IOCTL_GET_PCPS_UCAP_ENTRIES:
      _io_read_var_chk( pddev, PCPS_GIVE_UCAP_ENTRIES, pcps_ucap_entries,
                        pout, _pcps_ddev_has_ucap( pddev ) );
      break;


    case IOCTL_GET_PCI_ASIC_FEATURES:
      _io_chk_cond( _pcps_ddev_has_asic_features( pddev ) );
      _iob_to_pout_var( pddev->asic_features, pout );
      rc = MBG_SUCCESS;
      break;


    case IOCTL_GET_IRQ_STAT_INFO:
      _iob_to_pout_var( pddev->irq_stat_info, pout );
      rc = MBG_SUCCESS;
      break;


    case IOCTL_GET_CYCLES_FREQUENCY:
    {
      MBG_PC_CYCLES_FREQUENCY mbg_pc_cycles_frequency;

      mbg_get_pc_cycles_frequency( &mbg_pc_cycles_frequency );
      _iob_to_pout_var( mbg_pc_cycles_frequency, pout );
      rc = MBG_SUCCESS;
      break;
    }


    case IOCTL_GET_IRIG_CTRL_BITS:
      _io_read_var_chk( pddev, PCPS_GET_IRIG_CTRL_BITS, mbg_irig_ctrl_bits,
                        pout, _pcps_ddev_has_irig_ctrl_bits( pddev ) );
      break;


    case IOCTL_GET_IP4_STATE:
      _io_read_gps_var_chk( pddev, PC_GPS_IP4_STATE, ip4_settings,
                            pout, _pcps_ddev_has_lan_intf( pddev ) );
      break;


    case IOCTL_GET_PTP_STATE:
      _io_read_gps_var_chk( pddev, PC_GPS_PTP_STATE, ptp_state,
                            pout, _pcps_ddev_has_ptp( pddev ) );
      break;


    case IOCTL_GET_CORR_INFO:
      _io_read_var_chk( pddev, PCPS_GET_CORR_INFO, corr_info,
                        pout, _pcps_ddev_has_corr_info( pddev ) );
      break;


    case IOCTL_GET_DEBUG_STATUS:
      _io_read_var_chk( pddev, PCPS_GET_DEBUG_STATUS, debug_status,
                        pout, _pcps_ddev_has_debug_status( pddev ) );
      break;


    case IOCTL_GET_NUM_EVT_LOG_ENTRIES:
      _io_read_var_chk( pddev, PCPS_NUM_EVT_LOG_ENTRIES, num_evt_log_entries,
                        pout, _pcps_ddev_has_evt_log( pddev ) );
      break;


    case IOCTL_GET_FIRST_EVT_LOG_ENTRY:
      _io_read_var_chk( pddev, PCPS_FIRST_EVT_LOG_ENTRY, evt_log_entry,
                        pout, _pcps_ddev_has_evt_log( pddev ) );
      break;


    case IOCTL_GET_NEXT_EVT_LOG_ENTRY:
      _io_read_var_chk( pddev, PCPS_NEXT_EVT_LOG_ENTRY, evt_log_entry,
                        pout, _pcps_ddev_has_evt_log( pddev ) );
      break;


    #if _MBG_SUPP_VAR_ACC_SIZE  // otherwise generic IOCTL functions are used instead
      case IOCTL_GET_ALL_GNSS_SAT_INFO:
        _io_read_gps_chk( pddev, PC_GPS_ALL_GNSS_SAT_INFO, all_gnss_sat_info_idx, pout,
                          pout_size, _pcps_ddev_is_gnss( pddev ) );
        break;
    #endif



    // Commands returning device capabilities and features

    case IOCTL_DEV_IS_GPS:
      _report_cond( _pcps_ddev_is_gps( pddev ), pout );
      break;


    case IOCTL_DEV_IS_DCF:
      _report_cond( _pcps_ddev_is_dcf( pddev ), pout );
      break;


    case IOCTL_DEV_IS_MSF:
      _report_cond( _pcps_ddev_is_msf( pddev ), pout );
      break;


    case IOCTL_DEV_IS_WWVB:
      _report_cond( _pcps_ddev_is_wwvb( pddev ), pout );
      break;


    case IOCTL_DEV_IS_LWR:
      _report_cond( _pcps_ddev_is_lwr( pddev ), pout );
      break;


    case IOCTL_DEV_IS_GNSS:
      _report_cond( _pcps_ddev_is_gnss( pddev ), pout );
      break;


    case IOCTL_DEV_IS_IRIG_RX:
      _report_cond( _pcps_ddev_is_irig_rx( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_HR_TIME:
      _report_cond( _pcps_ddev_has_hr_time( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_CAB_LEN:
      _report_cond( _pcps_ddev_has_cab_len( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_TZDL:
      _report_cond( _pcps_ddev_has_tzdl( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_PCPS_TZDL:
      _report_cond( _pcps_ddev_has_pcps_tzdl( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_TZCODE:
      _report_cond( _pcps_ddev_has_tzcode( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_TZ:
      _report_cond( _pcps_ddev_has_tz( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_EVENT_TIME:
      _report_cond( _pcps_ddev_has_event_time( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_RECEIVER_INFO:
      _report_cond( _pcps_ddev_has_receiver_info( pddev ), pout );
      break;


    case IOCTL_DEV_CAN_CLR_UCAP_BUFF:
      _report_cond( _pcps_ddev_can_clr_ucap_buff( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_UCAP:
      _report_cond( _pcps_ddev_has_ucap( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_IRIG_TX:
      _report_cond( _pcps_ddev_has_irig_tx( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_SERIAL_HS:
      _report_cond( _pcps_ddev_has_serial_hs( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_SIGNAL:
      _report_cond( _pcps_ddev_has_signal( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_MOD:
      _report_cond( _pcps_ddev_has_mod( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_IRIG:
      _report_cond( _pcps_ddev_has_irig( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_REF_OFFS:
      _report_cond( _pcps_ddev_has_ref_offs( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_OPT_FLAGS:
      _report_cond( _pcps_ddev_has_opt_flags( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_GPS_DATA:
      _report_cond( _pcps_ddev_has_gps_data( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_SYNTH:
      _report_cond( _pcps_ddev_has_synth( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_GENERIC_IO:
      _report_cond( _pcps_ddev_has_generic_io( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_PCI_ASIC_FEATURES:
      _report_cond( _pcps_ddev_has_asic_features( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_PCI_ASIC_VERSION:
      _report_cond( _pcps_ddev_has_asic_version( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_FAST_HR_TIMESTAMP:
      _report_cond( _pcps_ddev_has_fast_hr_timestamp( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_GPS_TIME_SCALE:
      _report_cond( _pcps_ddev_has_time_scale( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_GPS_UTC_PARM:
      _report_cond( _pcps_ddev_has_utc_parm( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_IRIG_CTRL_BITS:
      _report_cond( _pcps_ddev_has_irig_ctrl_bits( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_LAN_INTF:
      _report_cond( _pcps_ddev_has_lan_intf( pddev ), pout );
      break;


    case IOCTL_DEV_IS_PTP:
      _report_cond( _pcps_ddev_is_ptp( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_PTP:
      _report_cond( _pcps_ddev_has_ptp( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_IRIG_TIME:
      _report_cond( _pcps_ddev_has_irig_time( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_RAW_IRIG_DATA:
      _report_cond( _pcps_ddev_has_raw_irig_data( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_PTP_UNICAST:
      _report_cond( _pcps_ddev_has_ptp_unicast( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_PZF:
      _report_cond( _pcps_ddev_has_pzf( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_CORR_INFO:
      _report_cond( _pcps_ddev_has_corr_info( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_TR_DISTANCE:
      _report_cond( _pcps_ddev_has_tr_distance( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_DEBUG_STATUS:
      _report_cond( _pcps_ddev_has_debug_status( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_EVT_LOG:
      _report_cond( _pcps_ddev_has_evt_log( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_GPIO:
      _report_cond( _pcps_ddev_has_gpio( pddev ), pout );
      break;


    case IOCTL_DEV_HAS_XMR:
      _report_cond( _pcps_ddev_has_xmr( pddev ), pout );
      break;


    case IOCTL_CHK_DEV_FEAT:
      _iob_from_pin_var( p_tmp->dev_feat_req, pin );
      rc = pcps_chk_dev_feat( pddev, p_tmp->dev_feat_req.feat_type, p_tmp->dev_feat_req.feat_num );
      #if DEBUG_IOCTL
        _mbgddmsg_3( DEBUG_IOCTL, MBG_LOG_INFO, "chk_dev_feat %lu:%lu returned %i",
                     (ulong) p_tmp->dev_feat_req.feat_type, (ulong) p_tmp->dev_feat_req.feat_num, rc );
      #endif
      if ( mbg_rc_is_error( rc ) )
      {
        if ( rc == MBG_ERR_NOT_SUPP_BY_DEV )
          goto err_not_supp_by_dev;

        goto err_inval_param;
      }
      break;


    // The next codes are somewhat special since they change something
    // on the board but do not affect basic operation

    case IOCTL_PCPS_CLR_UCAP_BUFF:
      _io_write_cmd_chk( pddev, PCPS_CLR_UCAP_BUFF,
                         _pcps_ddev_can_clr_ucap_buff( pddev ) );
      break;


    case IOCTL_SET_PCPS_EVENT_TIME:
      _io_write_var_chk( pddev, PCPS_SET_EVENT_TIME,
                           pcps_time_stamp, pin,
                           _pcps_ddev_has_event_time( pddev ) );
      break;


    case IOCTL_CLR_EVT_LOG:
      _io_write_cmd_chk( pddev, PCPS_CLR_EVT_LOG, _pcps_ddev_has_evt_log( pddev ) );
      break;



    // Status information which may not be available for everybody

    case IOCTL_GET_GPS_POS:
      _io_read_gps_var( pddev, PC_GPS_POS, pos, pout );
      break;



    // Codes reading device configuration

    case IOCTL_GET_PCPS_SERIAL:
      _io_read_var( pddev, PCPS_GET_SERIAL, pcps_serial, pout );
      break;


    case IOCTL_GET_PCPS_TZCODE:
      _io_read_var_chk( pddev, PCPS_GET_TZCODE, pcps_tzcode, pout,
                        _pcps_ddev_has_tzcode( pddev ) );
      break;


    case IOCTL_GET_PCPS_TZDL:
      _io_read_var_chk( pddev, PCPS_GET_PCPS_TZDL, pcps_tzdl, pout,
                        _pcps_ddev_has_pcps_tzdl( pddev ) );
      break;


    case IOCTL_GET_REF_OFFS:
      _io_read_var_chk( pddev, PCPS_GET_REF_OFFS, mbg_ref_offs, pout,
                        _pcps_ddev_has_ref_offs( pddev ) );
      break;


    case IOCTL_GET_MBG_OPT_INFO:
      _io_read_var_chk( pddev, PCPS_GET_OPT_INFO, mbg_opt_info, pout,
                        _pcps_ddev_has_opt_flags( pddev ) );
      break;


    case IOCTL_GET_PCPS_IRIG_RX_INFO:
      _io_read_var_chk( pddev, PCPS_GET_IRIG_RX_INFO, irig_info, pout,
                        _pcps_ddev_is_irig_rx( pddev ) );
      break;


    case IOCTL_GET_GPS_TZDL:
      _io_read_gps_var( pddev, PC_GPS_TZDL, tzdl, pout );
      break;


    case IOCTL_GET_GPS_PORT_PARM:
      _io_read_gps_var( pddev, PC_GPS_PORT_PARM, port_parm, pout );
      break;


    case IOCTL_GET_GPS_ENABLE_FLAGS:
      _io_read_gps_var( pddev, PC_GPS_ENABLE_FLAGS, enable_flags, pout );
      break;


    case IOCTL_GET_GPS_ANT_CABLE_LEN:
      _io_read_gps_var_chk( pddev, PC_GPS_ANT_CABLE_LEN, ant_cable_len, pout,
                            _pcps_ddev_has_cab_len( pddev ) );
      break;


    case IOCTL_GET_PCPS_IRIG_TX_INFO:
      /* This is a workaround for GPS169PCIs with early */
      /* firmware versions. See RCS log for details. */
      pcps_cmd = PCPS_GET_IRIG_TX_INFO;

      if ( _pcps_ddev_requires_irig_workaround( pddev ) )
      {
        pcps_cmd = PCPS_GET_IRIG_RX_INFO;
        _mbgddmsg_0( DEBUG, MBG_LOG_WARN, "Workaround for GPS169PCI \"get IRIG TX cfg\"" );
      }

      _io_read_var_chk( pddev, pcps_cmd, irig_info, pout,
                        _pcps_ddev_has_irig_tx( pddev ) );
      break;


    case IOCTL_GET_SYNTH:
      _io_read_var_chk( pddev, PCPS_GET_SYNTH, synth, pout,
                        _pcps_ddev_has_synth( pddev ) );
      break;


    case IOCTL_GET_GPS_TIME_SCALE_INFO:
      _io_read_gps_var_chk( pddev, PC_GPS_TIME_SCALE, mbg_time_scale_info,
                            pout, _pcps_ddev_has_time_scale( pddev ) );
      break;


    case IOCTL_GET_GPS_UTC_PARM:
      _io_read_gps_var_chk( pddev, PC_GPS_UTC, utc, pout,
                            _pcps_ddev_has_utc_parm( pddev ) );
      break;


    case IOCTL_GET_LAN_IF_INFO:
      _io_read_gps_var_chk( pddev, PC_GPS_LAN_IF_INFO, lan_if_info,
                            pout, _pcps_ddev_has_lan_intf( pddev ) );
      break;


    case IOCTL_GET_IP4_SETTINGS:
      _io_read_gps_var_chk( pddev, PC_GPS_IP4_SETTINGS, ip4_settings,
                            pout, _pcps_ddev_has_lan_intf( pddev ) );
      break;


    case IOCTL_GET_PTP_CFG_INFO:
      _io_read_gps_var_chk( pddev, PC_GPS_PTP_CFG, ptp_cfg_info,
                            pout, _pcps_ddev_has_ptp( pddev ) );
      break;


    case IOCTL_GET_IRIG_TIME:
      _io_read_var_chk( pddev, PCPS_GIVE_IRIG_TIME, pcps_irig_time,
                        pout, _pcps_ddev_has_irig_time( pddev ) );
      break;


    case IOCTL_GET_RAW_IRIG_DATA:
      _io_read_var_chk( pddev, PCPS_GET_RAW_IRIG_DATA, mbg_raw_irig_data,
                        pout, _pcps_ddev_has_raw_irig_data( pddev ) );
      break;


    case IOCTL_PTP_UC_MASTER_CFG_LIMITS:
      _io_read_gps_var_chk( pddev, PC_GPS_PTP_UC_MASTER_CFG_LIMITS, ptp_uc_master_cfg_limits,
                            pout, _pcps_ddev_has_ptp_unicast( pddev ) );
      break;


    case IOCTL_GET_TR_DISTANCE:
      _io_read_var_chk( pddev, PCPS_GET_TR_DISTANCE, tr_distance,
                        pout, _pcps_ddev_has_tr_distance( pddev ) );
      break;


    case IOCTL_PCPS_GENERIC_READ:
    #if USE_IOCTL_GENERIC_REQ
      _iob_from_pin_var( p_tmp->req, pin );

      p_buff_out = _pcps_kmalloc( p_tmp->req.out_sz );

      if ( p_buff_out == NULL )
      {
        _mbg_kdd_msg_3( MBG_LOG_WARN, "Failed to alloc %lu bytes for %s, cmd: %02lX",
                        (ulong) p_tmp->req.out_sz, "IOCTL_PCPS_GENERIC_READ",
                        (ulong) p_tmp->req.info );
        goto err_no_mem;
      }

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = _pcps_read( pddev, (uint8_t) p_tmp->req.info, p_buff_out,
                       (uint8_t) p_tmp->req.out_sz );
      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_success( rc ) )
        _frc_iob_to_pout( p_buff_out, p_tmp->req.out_p, p_tmp->req.out_sz );

      _pcps_kfree( p_buff_out, p_tmp->req.out_sz );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #else

      _iob_from_pin_var( p_tmp->ctl, pin );

      buffer_size = sizeof( p_tmp->ctl ) + p_tmp->ctl.data_size_out;

      p_buff = _pcps_kmalloc( buffer_size );

      if ( p_buff == NULL )
        goto err_no_mem;

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = _pcps_read( pddev, (uint8_t) p_tmp->ctl.info, p_buff->data,
                       (uint8_t) p_tmp->ctl.data_size_out );
      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_success( rc ) )
      {
        p_buff->ctl = p_tmp->ctl;
        _iob_to_pout( p_buff, pout, buffer_size );    // TODO need to check if correct size is used
      }

      _pcps_kfree( p_buff, buffer_size );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #endif
      break;


    case IOCTL_PCPS_GENERIC_READ_GPS:
    #if USE_IOCTL_GENERIC_REQ
      _iob_from_pin_var( p_tmp->req, pin );

      p_buff_out = _pcps_kmalloc( p_tmp->req.out_sz );

      if ( p_buff_out == NULL )
      {
        _mbg_kdd_msg_3( MBG_LOG_WARN, "Failed to alloc %lu bytes for %s, GPS cmd: %02lX",
                        (ulong) p_tmp->req.out_sz, "IOCTL_PCPS_GENERIC_READ_GPS",
                        (ulong) p_tmp->req.info );
        goto err_no_mem;
      }

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_read_gps( pddev, (uint8_t) p_tmp->req.info, p_buff_out,
                          (uint16_t) p_tmp->req.out_sz );
      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_success( rc ) )
        _frc_iob_to_pout( p_buff_out, p_tmp->req.out_p, p_tmp->req.out_sz );

      _pcps_kfree( p_buff_out, p_tmp->req.out_sz );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #else

      _iob_from_pin_var( p_tmp->ctl, pin );

      buffer_size = sizeof( p_tmp->ctl ) + p_tmp->ctl.data_size_out;

      p_buff = _pcps_kmalloc( buffer_size );

      if ( p_buff == NULL )
        goto err_no_mem;

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_read_gps( pddev, (uint8_t) p_tmp->ctl.info, p_buff->data,
                          (uint16_t) p_tmp->ctl.data_size_out );
      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_success( rc ) )
      {
        p_buff->ctl = p_tmp->ctl;
        _iob_to_pout( p_buff, pout, buffer_size );  // TODO need to check if correct size is used
      }

      _pcps_kfree( p_buff, buffer_size );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #endif
      break;


  #if _MBG_SUPP_VAR_ACC_SIZE

    // These codes are only supported on target systems where a variable size of
    // the IOCTL buffer can be specified in the IOCTL call. On other systems the
    // generic IOCTL functions are used instead.

    case IOCTL_GET_GPS_ALL_STR_TYPE_INFO:
      _io_read_gps_chk( pddev, PC_GPS_ALL_STR_TYPE_INFO, all_str_type_info_idx, pout,
                        pout_size, _pcps_ddev_has_receiver_info( pddev ) );
      break;


    case IOCTL_GET_GPS_ALL_PORT_INFO:
      _io_read_gps_chk( pddev, PC_GPS_ALL_PORT_INFO, all_port_info_idx, pout,
                        pout_size, _pcps_ddev_has_receiver_info( pddev ) );
      break;


    case IOCTL_GET_GPS_ALL_POUT_INFO:
      _io_read_gps_chk( pddev, PC_GPS_ALL_POUT_INFO, all_pout_info_idx, pout,
                        pout_size, _pcps_ddev_has_receiver_info( pddev ) );
      break;


    case IOCTL_GET_ALL_PTP_UC_MASTER_INFO:
      _io_read_gps_chk( pddev, PC_GPS_ALL_PTP_UC_MASTER_INFO, all_ptp_uc_master_info_idx,
                        pout, pout_size, _pcps_ddev_has_ptp_unicast( pddev ) );
      break;


    case IOCTL_GET_ALL_GPIO_INFO:
      _io_read_gps_chk( pddev, PC_GPS_ALL_GPIO_INFO, all_gpio_info_idx,
                        pout, pout_size, _pcps_ddev_has_gpio( pddev ) );
      break;


    case IOCTL_GET_ALL_GPIO_STATUS:
      _io_read_gps_var_chk( pddev, PC_GPS_ALL_GPIO_STATUS, all_gpio_status_idx,
                            pout, _pcps_ddev_has_gpio( pddev ) );  //##++++++++++++++++++++++++ condition??
      break;


    case IOCTL_GET_ALL_XMR_STATUS:
      _io_read_gps_chk( pddev, PC_GPS_ALL_XMR_STATUS, all_xmulti_ref_status_idx,
                        pout, pout_size, _pcps_ddev_has_xmr( pddev ) );
      break;


    case IOCTL_GET_ALL_XMR_INFO:
      _io_read_gps_chk( pddev, PC_GPS_ALL_XMR_INFO, all_xmulti_ref_info_idx,
                        pout, pout_size, _pcps_ddev_has_xmr( pddev ) );
      break;

  #endif  // _MBG_SUPP_VAR_ACC_SIZE


    case IOCTL_GET_GNSS_MODE_INFO:
      _io_read_gps_var_chk( pddev, PC_GPS_GNSS_MODE, gnss_mode_info,
                            pout, _pcps_ddev_is_gnss( pddev ) );
      break;

    case IOCTL_GET_GPIO_CFG_LIMITS:
      _io_read_gps_var_chk( pddev, PC_GPS_GPIO_CFG_LIMITS, mbg_gpio_cfg_limits,
                            pout, _pcps_ddev_has_gpio( pddev ) );
      break;

    case IOCTL_GET_XMR_INSTANCES:
      _io_read_gps_var_chk( pddev, PC_GPS_XMR_INSTANCES, xmulti_ref_instances,
                            pout, _pcps_ddev_has_xmr( pddev ) );
      break;

    case IOCTL_GET_XMR_HOLDOVER_STATUS:
      _io_read_gps_var_chk( pddev, PC_GPS_XMR_HOLDOVER_STATUS, xmr_holdover_status,
                            pout, _pcps_ddev_has_xmr( pddev ) );
      break;



    // Codes writing device configuration

    case IOCTL_SET_PCPS_SERIAL:
      _io_write_var( pddev, PCPS_SET_SERIAL, pcps_serial, pin );
      break;


    case IOCTL_SET_PCPS_TZCODE:
      _io_write_var_chk( pddev, PCPS_SET_TZCODE, pcps_tzcode, pin,
                         _pcps_ddev_has_tzcode( pddev ) );
      break;


    case IOCTL_SET_PCPS_TZDL:
      _io_write_var_chk( pddev, PCPS_SET_PCPS_TZDL, pcps_tzdl, pin,
                         _pcps_ddev_has_pcps_tzdl( pddev ) );
      break;


    case IOCTL_SET_REF_OFFS:
      _io_write_var_chk( pddev, PCPS_SET_REF_OFFS, mbg_ref_offs, pin,
                         _pcps_ddev_has_ref_offs( pddev ) );
      break;


    case IOCTL_SET_MBG_OPT_SETTINGS:
      _io_write_var_chk( pddev, PCPS_SET_OPT_SETTINGS, mbg_opt_settings,
                         pin, _pcps_ddev_has_opt_flags( pddev ) );
      break;


    case IOCTL_SET_PCPS_IRIG_RX_SETTINGS:
      _io_write_var_chk( pddev, PCPS_SET_IRIG_RX_SETTINGS,
                           irig_settings, pin,
                           _pcps_ddev_is_irig_rx( pddev ) );
      break;


    case IOCTL_SET_GPS_TZDL:
      _io_write_gps_var( pddev, PC_GPS_TZDL, tzdl, pin );
      break;


    case IOCTL_SET_GPS_PORT_PARM:
      _io_write_gps_var( pddev, PC_GPS_PORT_PARM, port_parm, pin );
      break;


    case IOCTL_SET_GPS_ENABLE_FLAGS:
      _io_write_gps_var( pddev, PC_GPS_ENABLE_FLAGS, enable_flags, pin );
      break;


    case IOCTL_SET_GPS_ANT_CABLE_LEN:
      _io_write_gps_var_chk( pddev, PC_GPS_ANT_CABLE_LEN, ant_cable_len,
                             pin, _pcps_ddev_has_cab_len( pddev ) );
      break;


    case IOCTL_SET_GPS_PORT_SETTINGS_IDX:
      _io_write_gps_var_chk( pddev, PC_GPS_PORT_SETTINGS_IDX, port_settings_idx,
                             pin, _pcps_ddev_has_receiver_info( pddev ) );
      break;


    case IOCTL_SET_PCPS_IRIG_TX_SETTINGS:
      /* This is a workaround for GPS169PCIs with early */
      /* firmware versions. See RCS log for details. */
      pcps_cmd = PCPS_SET_IRIG_TX_SETTINGS;

      if ( _pcps_ddev_requires_irig_workaround( pddev ) )
      {
        pcps_cmd = PCPS_SET_IRIG_RX_SETTINGS;
        _mbgddmsg_0( DEBUG, MBG_LOG_WARN, "Workaround for GPS169PCI \"set IRIG TX cfg\"" );
      }

      _io_write_var_chk( pddev, pcps_cmd, irig_settings, pin,
                         _pcps_ddev_has_irig_tx( pddev ) );
      break;


    case IOCTL_SET_SYNTH:
      _io_write_var_chk( pddev, PCPS_SET_SYNTH, synth, pin,
                         _pcps_ddev_has_synth( pddev ) );
      break;


    case IOCTL_SET_GPS_POUT_SETTINGS_IDX:
      _io_write_gps_var_chk( pddev, PC_GPS_POUT_SETTINGS_IDX, pout_settings_idx,
                             pin, _pcps_ddev_has_receiver_info( pddev ) );
      break;


    case IOCTL_SET_IP4_SETTINGS:
      _io_write_gps_var_chk( pddev, PC_GPS_IP4_SETTINGS, ip4_settings,
                             pin, _pcps_ddev_has_lan_intf( pddev ) );
      break;


    case IOCTL_SET_PTP_CFG_SETTINGS:
      _io_write_gps_var_chk( pddev, PC_GPS_PTP_CFG, ptp_cfg_settings,
                             pin, _pcps_ddev_has_ptp( pddev ) );
      break;


    case IOCTL_SET_PTP_UC_MASTER_SETTINGS_IDX:
      _io_write_gps_var_chk( pddev, PC_GPS_PTP_UC_MASTER_SETTINGS_IDX,
                             ptp_uc_master_settings_idx, pin,
                            _pcps_ddev_has_ptp_unicast( pddev ) );
      break;


    case IOCTL_SET_TR_DISTANCE:
      _io_write_var_chk( pddev, PCPS_SET_TR_DISTANCE, tr_distance,
                         pin, _pcps_ddev_has_tr_distance( pddev ) );
      break;


    case IOCTL_SET_GNSS_MODE_SETTINGS:
      _io_write_gps_var_chk( pddev, PC_GPS_GNSS_MODE, gnss_mode_settings, pin,
                             _pcps_ddev_is_gnss( pddev ) );
      break;


    case IOCTL_SET_GPIO_SETTINGS_IDX:
      _io_write_var_chk( pddev, PC_GPS_GPIO_SETTINGS_IDX, mbg_gpio_settings_idx,
                         pin, _pcps_ddev_has_gpio( pddev ) );
      break;



    // Operations which may severely affect system operation

    case IOCTL_SET_PCPS_TIME:
      _io_write_var_chk( pddev, PCPS_SET_TIME, pcps_stime, pin,
                         _pcps_ddev_can_set_time( pddev ) );
      break;


    case IOCTL_SET_GPS_TIME:
      _io_write_gps_var( pddev, PC_GPS_TIME, ttm, pin );
      break;


    case IOCTL_SET_GPS_POS_XYZ:
      _io_write_gps_var( pddev, PC_GPS_POS_XYZ, xyzs, pin );
      break;


    case IOCTL_SET_GPS_POS_LLA:
      _io_write_gps_var( pddev, PC_GPS_POS_LLA, llas, pin );
      break;


    case IOCTL_SET_GPS_TIME_SCALE_SETTINGS:
      _io_write_gps_var_chk( pddev, PC_GPS_TIME_SCALE, mbg_time_scale_settings,
                             pin, _pcps_ddev_has_time_scale( pddev ) );
      break;


    case IOCTL_SET_GPS_UTC_PARM:
      _io_write_gps_var_chk( pddev, PC_GPS_UTC, utc, pin,
                             _pcps_ddev_has_utc_parm( pddev ) );
      break;


    case IOCTL_SET_GPS_CMD:
      _io_write_gps_var( pddev, PC_GPS_CMD, gps_cmd, pin );
      break;


    case IOCTL_SET_XMR_SETTINGS_IDX:
      _io_write_gps_var( pddev, PC_GPS_XMR_SETTINGS_IDX, xmulti_ref_settings_idx, pin );
      break;



    // Generic read/write operations which can do anything

    case IOCTL_PCPS_GENERIC_WRITE:
    #if USE_IOCTL_GENERIC_REQ

      _iob_from_pin_var( p_tmp->req, pin );

      p_buff_in = _pcps_kmalloc( p_tmp->req.in_sz );

      if ( p_buff_in == NULL )
      {
        _mbg_kdd_msg_3( MBG_LOG_WARN, "Failed to alloc %lu bytes for %s, cmd: %02lX",
                        (ulong) p_tmp->req.in_sz, "IOCTL_PCPS_GENERIC_WRITE",
                        (ulong) p_tmp->req.info );
        goto err_no_mem;
      }

      _frc_iob_from_pin( p_buff_in, p_tmp->req.in_p, p_tmp->req.in_sz );

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_write( pddev, (uint8_t) p_tmp->req.info, p_buff_in,
                       (uint8_t) p_tmp->req.in_sz );
      _pcps_sem_dec( pddev );

      _pcps_kfree( p_buff_in, p_tmp->req.in_sz );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #else

      _iob_from_pin_var( p_tmp->ctl, pin );

      buffer_size = sizeof( p_tmp->ctl ) + p_tmp->ctl.data_size_in;

      p_buff = _pcps_kmalloc( buffer_size );

      if ( p_buff == NULL )
        goto err_no_mem;

      _iob_from_pin( p_buff, pin, buffer_size );

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_write( pddev, (uint8_t) p_tmp->ctl.info, p_buff->data,
                       (uint8_t) p_tmp->ctl.data_size_in );
      _pcps_sem_dec( pddev );

      _pcps_kfree( p_buff, buffer_size );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #endif
      break;


    case IOCTL_PCPS_GENERIC_WRITE_GPS:
    #if USE_IOCTL_GENERIC_REQ

      _iob_from_pin_var( p_tmp->req, pin );

      p_buff_in = _pcps_kmalloc( p_tmp->req.in_sz );

      if ( p_buff_in == NULL )
      {
        _mbg_kdd_msg_3( MBG_LOG_WARN, "Failed to alloc %lu bytes for %s, cmd: %02lX",
                        (ulong) p_tmp->req.in_sz, "IOCTL_PCPS_GENERIC_WRITE_GPS",
                        (ulong) p_tmp->req.info );
        goto err_no_mem;
      }

      _frc_iob_from_pin( p_buff_in, p_tmp->req.in_p, p_tmp->req.in_sz );

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_write_gps( pddev, (uint8_t) p_tmp->req.info, p_buff_in,
                           (uint16_t) p_tmp->req.in_sz );
      _pcps_sem_dec( pddev );

      _pcps_kfree( p_buff_in, p_tmp->req.in_sz );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #else

      _iob_from_pin_var( p_tmp->ctl, pin );

      buffer_size = sizeof( p_tmp->ctl ) + p_tmp->ctl.data_size_in;

      p_buff = _pcps_kmalloc( buffer_size );

      if ( p_buff == NULL )
        goto err_no_mem;

      _iob_from_pin( p_buff, pin, buffer_size );

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_write_gps( pddev, (uint8_t) p_tmp->ctl.info, p_buff->data,
                           (uint8_t) p_tmp->ctl.data_size_in );
      _pcps_sem_dec( pddev );

      _pcps_kfree( p_buff, buffer_size );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #endif
      break;


    case IOCTL_PCPS_GENERIC_IO:
    #if USE_IOCTL_GENERIC_REQ
      _io_chk_cond( _pcps_ddev_has_generic_io( pddev ) );

      _iob_from_pin_var( p_tmp->req, pin );

      if ( p_tmp->req.in_p && p_tmp->req.in_sz )
      {
        p_buff_in = _pcps_kmalloc( p_tmp->req.in_sz );

        if ( p_buff_in == NULL )
        {
          _mbg_kdd_msg_3( MBG_LOG_WARN, "Failed to alloc %lu bytes for input in %s, cmd: %02lX",
                          (ulong) p_tmp->req.in_sz, "IOCTL_PCPS_GENERIC_IO",
                          (ulong) p_tmp->req.info );
          goto err_no_mem;
        }

        _frc_iob_from_pin( p_buff_in, p_tmp->req.in_p, p_tmp->req.in_sz );
      }
      else
      {
        p_buff_in = NULL;
        p_tmp->req.in_sz = 0;  // just to be sure
      }

      if ( p_tmp->req.out_p && p_tmp->req.out_sz )
      {
        p_buff_out = _pcps_kmalloc( p_tmp->req.out_sz );

        if ( p_buff_out == NULL )
        {
          _mbg_kdd_msg_3( MBG_LOG_WARN, "Failed to alloc %lu bytes for output in %s, cmd: %02lX",
                          (ulong) p_tmp->req.in_sz, "IOCTL_PCPS_GENERIC_IO",
                          (ulong) p_tmp->req.info );

          // We can't continue this API call, but before we leave we have to free
          // the input buffer we already have allocated before.
          _pcps_kfree( p_buff_in, p_tmp->req.in_sz );
          goto err_no_mem;
        }
      }
      else
      {
        p_buff_out = NULL;
        p_tmp->req.out_sz = 0;  // just to be sure
      }

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_generic_io( pddev, (uint8_t) p_tmp->req.info,
                            p_buff_in, (uint8_t) p_tmp->req.in_sz,
                            p_buff_out, (uint8_t) p_tmp->req.out_sz );
      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_success( rc ) )
        _frc_iob_to_pout( p_buff_out, p_tmp->req.out_p, p_tmp->req.out_sz );

      if ( p_buff_in )
        _pcps_kfree( p_buff_in, p_tmp->req.in_sz );

      if ( p_buff_out )
        _pcps_kfree( p_buff_out, p_tmp->req.out_sz );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #else

      _io_chk_cond( _pcps_ddev_has_generic_io( pddev ) );

      _iob_from_pin_var( p_tmp->ctl, pin );

      buffer_size = sizeof( p_tmp->ctl ) +
                ( ( p_tmp->ctl.data_size_in > p_tmp->ctl.data_size_out ) ?
                    p_tmp->ctl.data_size_in : p_tmp->ctl.data_size_out );

      p_buff = _pcps_kmalloc( buffer_size );

      if ( p_buff == NULL )
        goto err_no_mem;

      _iob_from_pin( p_buff, pin, sizeof( p_buff->ctl ) + p_tmp->ctl.data_size_in );

      // We only use local buffers here, and the allocated buffer used
      // for device access is anyway DMA-capable, so only the direct access
      // to the device needs to be protected by the device semaphore.
      _pcps_sem_inc_safe( pddev );
      rc = pcps_generic_io( pddev, (uint8_t) p_tmp->ctl.info,
                            p_buff->data, (uint8_t) p_tmp->ctl.data_size_in,
                            p_buff->data, (uint8_t) p_tmp->ctl.data_size_out );
      _pcps_sem_dec( pddev );

      if ( mbg_rc_is_success( rc ) )
      {
        p_buff->ctl = p_tmp->ctl;
        _iob_to_pout( p_buff, pout, sizeof( p_buff->ctl ) + p_tmp->ctl.data_size_out );  // TODO need to check if correct size is used
      }

      _pcps_kfree( p_buff, buffer_size );

      if ( mbg_rc_is_error( rc ) )
        goto err_dev_access;

    #endif
      break;


    // The next codes are somewhat special and normally
    // not used by the driver software:

    case IOCTL_GET_MAPPED_MEM_ADDR:
      _io_chk_cond( ( pddev->asic_features & PCI_ASIC_HAS_MM_IO ) );
      _io_get_mapped_mem_address( pddev, pout );
      break;


    case IOCTL_UNMAP_MAPPED_MEM:
      _io_chk_cond( ( pddev->asic_features & PCI_ASIC_HAS_MM_IO ) );
      _io_unmap_mapped_mem_address( pddev, pin );
      break;


  #if USE_DEBUG_PORT
    // The codes below are used for debugging only.
    // Unrestricted usage may cause system malfunction !!

    case IOCTL_MBG_DBG_GET_PORT_ADDR:
      _iob_to_pout_var( mbg_dbg_port, pout );
      rc = MBG_SUCCESS;
      break;


    case IOCTL_MBG_DBG_SET_PORT_ADDR:
      _iob_from_pin_var( mbg_dbg_port, pin );
      rc = MBG_SUCCESS;
      break;


    case IOCTL_MBG_DBG_SET_BIT:
    {
      MBG_DBG_DATA dbg_data;
      _iob_from_pin_var( dbg_data, pin );
      _mbg_dbg_set_bit( pddev, dbg_data );
      mbg_get_pc_cycles( &cyc );
      _iob_to_pout_var( cyc, pout );
      break;
    }


    case IOCTL_MBG_DBG_CLR_BIT:
    {
      MBG_DBG_DATA dbg_data;
      _iob_from_pin_var( dbg_data, pin );
      _mbg_dbg_clr_bit( pddev, dbg_data );
      mbg_get_pc_cycles( &cyc );
      _iob_to_pout_var( cyc, pout );
      break;
    }


    case IOCTL_MBG_DBG_CLR_ALL:
      _mbg_dbg_clr_all( pddev );
      mbg_get_pc_cycles( &cyc );
      _iob_to_pout_var( cyc, pout );
      break;

  #endif  // USE_DEBUG_PORT


    default:  // Unsupported IOCTL code
      goto err_unsupp_ioctl;
  }

  // We get here in case of success.
  return IOCTL_RC_SUCCESS;


err_unsupp_ioctl:
  log_info = "unsupported IOCTL";
  log_severity = MBG_LOG_WARN;
  sys_rc = IOCTL_RC_ERR_UNSUPP_IOCTL;
  goto out;


err_inval_param:
  log_info = "invalid parameter";
  log_severity = MBG_LOG_WARN;
  sys_rc = IOCTL_RC_ERR_INVAL_PARAM;
  goto out;

err_not_supp_by_dev:
  log_info = "not supported by device";
  log_severity = MBG_LOG_INFO;
  sys_rc = IOCTL_RC_ERR_NOT_SUPP_BY_DEV;
  goto out;


err_no_mem:
  log_info = "failed to allocate buffer";
  log_severity = MBG_LOG_ERR;
  sys_rc = IOCTL_RC_ERR_NO_MEM;
  goto out;


err_busy_irq_unsafe:
  log_info = "busy because unsafe IRQ enabled";
  log_severity = MBG_LOG_ERR;
  sys_rc = IOCTL_RC_ERR_BUSY_IRQ_UNSAFE;
  goto out;


err_dev_access:
  // On Linux we also get here if copying from/to user space failed,
  // so we check rc to distinguish the cases.
  switch ( rc )
  {
    #if defined( MBG_TGT_LINUX )
      case MBG_ERR_COPY_TO_USER:
        log_info = "failed to copy data to user space";
        log_severity = MBG_LOG_ERR;
        sys_rc = IOCTL_RC_ERR_COPY_TO_USER;
        break;

      case MBG_ERR_COPY_FROM_USER:
        log_info = "failed to copy data from user space";
        log_severity = MBG_LOG_ERR;
        sys_rc = IOCTL_RC_ERR_COPY_FROM_USER;
        break;
    #endif  // defined( MBG_TGT_LINUX )

    default:
      log_info = "dev. acc. failed";
      log_severity = MBG_LOG_ERR;
      sys_rc = IOCTL_RC_ERR_DEV_ACCESS;

  }  // switch

  // fall through to "out:"

out:
  if ( log_info && ( log_severity <= MBG_LOG_WARN ) )
  {
    const char *ioctl_name = mbgioctl_get_name( ioctl_code );

    #if defined( MBG_TGT_WIN32 )
      _mbg_kdd_msg_4( log_severity, "%s %s, rc: %i, ret: 0x%08X",
                      ioctl_name, log_info, rc, sys_rc );
    #else
      _mbg_kdd_msg_7( log_severity, "%s (0x%02lX): %s, dev " MBG_DEV_NAME_FMT ", rc: %i, ret: %i",
                      ioctl_name, (ulong) ioctl_code, log_info, _pcps_ddev_type_name( pddev ),
                      _pcps_ddev_sernum( pddev ), rc, sys_rc );
    #endif
  }

  return sys_rc;

}  // ioctl_switch

#endif  /* _MACIOCTL_H */

