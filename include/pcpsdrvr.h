
/**************************************************************************
 *
 *  $Id: pcpsdrvr.h 1.54 2020/02/27 13:45:44 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions and prototypes for pcpsdrvr.c.
 *
 * -----------------------------------------------------------------------
 *  $Log: pcpsdrvr.h $
 *  Revision 1.54  2020/02/27 13:45:44  martin
 *  Support builtin features that depend on the firmware version.
 *  Added some conditional USB debug messages.
 *  Removed obsolete stuff.
 *  Revision 1.53  2019/03/19 16:46:24  martin
 *  Removed some obsolete DEBUG control settings.
 *  Revision 1.52  2019/03/18 16:11:40  martin
 *  Made a warning message more detailed.
 *  Revision 1.51  2019/03/13 16:20:15  martin
 *  Don't use local I/O buffer by default.
 *  Revision 1.50  2019/03/11 16:13:46  martin
 *  Temporarily use local I/O buffers by default. This may avoid a
 *  potential data corruption problem with PCI devices, but won't work
 *  with USB devices on Linux kernels 4.9 and later.
 *  Revision 1.49  2018/11/22 16:35:34  martin
 *  Refactored debug logging stuff.
 *  Moved definition of FAR to mbggenio.h.
 *  Moved some default DEBUG_* definitions here.
 *  Revision 1.48  2018/09/21 15:19:51  martin
 *  Added definitions for TCR180USB.
 *  Always use PCPS_IO_BUFFER which is defined
 *  in new header file pcpsiobf.h.
 *  Introduced cmd union.
 *  New macro _pcps_ddev_fw_has_20ms_bug().
 *  Updated macro _pcps_ddev_read_status_port().
 *  More unified resource handling.
 *  Renamed a number of symbols.
 *  Use _MBG_IOMEM.
 *  Workaround for DEBUG builds for older NetBSD versions.
 *  Don't use volatile.
 *  Unified kernel driver messages.
 *  Conditional DEBUG_RSRC.
 *  Added _pcps_ddev_mem_rsrc().
 *  Introduced access mode stuff.
 *  Updated naming for device feature stuff.
 *  Doxygen fixes.
 *  Moved some specific macros to pcpsirq.h.
 *  Renamed some inline functions.
 *  Define PCPS_TIMEOUT_CNT only if it wasn't defined before.
 *  Updated doxygen comments.
 *  Updated function prototypes.
 *  Revision 1.47  2017/08/10 13:59:52  martin
 *  Unified extended resource properties handling.
 *  String 'pcps_driver_name' is now defined here.
 *  Revision 1.46  2017/07/04 16:50:48  martin
 *  Support GPS180AMC and GNS181PEX.
 *  Support new module parameter force_io_access to provide
 *  runtime support for forcing I/O rather than MM access.
 *  Cleaned up I/O port usage.
 *  Added  _pcps_ddev_has_xmr() macro.
 *  Older defines N_SUPP_DEV, PCPS_MAX_DDEVS, and MBG_MAX_DEVICES
 *  have been obsoleted by new defines N_SUPP_DEV_BUS, N_SUPP_DEV_EXT,
 *  and N_SUPP_DEV_TOTAL.
 *  Conditional USB debug code.
 *  Use more versatile types for function pointers.
 *  Fixed macro definition syntax to avoid clang compiler warnings.
 *  Prepare to use MBG_XDEV_FEATURES in PCPS_DDEV.
 *  Updated function prototypes.
 *  Revision 1.45  2013/09/26 09:08:09  martin
 *  Support GLN180PEX and GNSS API.
 *  Revision 1.44  2013/03/15 10:01:09  martin
 *  Optionally override setting to support memory mapped I/O.
 *  Revision 1.43  2012/11/02 09:48:04  martin
 *  Removed obsolete include.
 *  Revision 1.42  2012/10/02 19:09:20  martin
 *  Conditionally support memory mapped access for MBGPEX cards.
 *  Support on-board event logs.
 *  New macro _pcps_ddev_has_debug_status().
 *  Added some macros to test if specific stat_info stuff is supported.
 *  Updated function prototypes.
 *  Added macros _pcps_ddev_has_corr_info() and _pcps_ddev_has_tr_distance().
 *  Support GPS180PEX, TCR180PEX, and PZF180PEX.
 *  Support DCF600USB, TCR600USB, MSF600USB, and WVB600USB.
 *  New macros _pcps_ddev_is_usb_v2() and _pcps_ddev_has_pcf().
 *  Optionally support timespec for sys time (USE_TIMESPEC).
 *  Support FreeBSD and NetBSD.
 *  Specify I/O range number when calling port I/O macros
 *  so they can be used for different ranges under BSD.
 *  Added macro _pcps_ddev_features().
 *  Revision 1.41  2010/06/30 13:44:49  martin
 *  Use new preprocessor symbol MBG_ARCH_X86.
 *  Revision 1.40  2010/01/12 14:05:05  daniel
 *  Added macro to check if reading the
 *  raw IRIG data bits is supported.
 *  Revision 1.39  2009/09/29 07:24:51Z  martin
 *  Use standard feature flag to check if fast HR time is supported.
 *  Revision 1.38  2009/06/19 12:13:05  martin
 *  Added _pcps_ddev_has_irig_time() macro.
 *  Revision 1.37  2009/06/09 10:13:59  daniel
 *  Added macros _pcps_ddev_has_lan_intf( _p ) and
 *  _pcps_ddev_has_ptp_cfg( _p ).
 *  Cleaned up the low level interface and provided a
 *  possibility to override the macros for special purposes.
 *  Set USB cyclic timeout interval to 1200 ms as default.
 *  Revision 1.36  2009/03/16 16:01:24Z  martin
 *  Support reading IRIG control function bits.
 *  Revision 1.35  2009/03/13 09:23:36  martin
 *  Added _pcps_ddev_has_time_scale( _p ) and _pcps_ddev_has_utc_parm( _p ).
 *  Moved _pcps_get_cycles() and _pcps_get_cycles_frequency() to pcpsdev.h
 *  and replaced/merged them with mbg_get_pc_cycles...() functions.
 *  Under Linux use own inline function to read TSC on x86 architectures.
 *  Normally USB timeouts are short with retries in order to increase
 *  responsiveness. On some systems this may lead to problems, so
 *  optionally one long timeout can be used now by define.
 *  Revision 1.34  2008/12/16 14:40:47  martin
 *  Account for new devices PTP270PEX, FRC270PEX, TCR170PEX, and WWVB51USB.
 *  Added macros _pcps_ddev_is_ptp(), _pcps_ddev_is_frc(),
 *  and _pcps_ddev_is_wwvb().
 *  Don't use pragma pack( 1 ) but use native alignment since structures
 *  defined here are not used across system boundaries.
 *  Added fields to PCPS_DDEV to store the ASIC version, and macros
 *  _pcps_ddev_raw_asic_version() and _pcps_ddev_asic_version().
 *  Moved PC cycles types and macros here, and defined dummy _pcps_get_cycles()
 *  for targets which don't support this.
 *  Use generic spinlock/mutex macros and common device access mutex.
 *  Support getting cycles frequency from device driver.
 *  Use MBG_MEM_ADDR type for memory rather than split high/low types.
 *  Renamed MBG_VIRT_ADDR to MBG_MEM_ADDR.
 *  Additional device port variables for IRQ handling.
 *  Use new MBG_ARCH_I386 symbol.
 *  Added DEBUG_LVL_... symbols.
 *  Use PCPS_IRQ_STAT_INFO type.
 *  Account for signed irq_num.
 *  New PCPS_DDEV field acc_cycles.
 *  Added variable usb_20_mode in PCPS_DDEV.
 *  Added connected flag to PCPS_DDEV structure.
 *  Added macro _pcps_ddev_has_fast_hr_timestamp().
 *  Use macros for unaligned access and endianess conversion.
 *  Support mapped I/O resources.
 *  Use some atomic_t types under Linux.
 *  Conditionally use Linux kthread API.
 *  Updated function prototypes.
 *  Revision 1.33  2008/02/27 10:25:30  martin
 *  Added support for TCR51USB and MSF51USB.
 *  Increased N_PCPS_MEM_RSRC to 2.
 *  Modified MBG_IOMEM_RSRC to support memory mapped I/O.
 *  Added PCI_ASIC_FEATURES to PCPS_DDEV.
 *  Added new macros and modified some older macros to support
 *  cyclic reading for USB within WIN32 targets.
 *  New macros _pcps_ddev_is_lwr() (long wave receiver),
 *  _pcps_ddev_is_msf(), _pcps_ddev_has_asic_version(),
 *  _pcps_ddev_has_asic_features().
 *  Moved Linux version-specific stuff to mbg_lx.h.
 *  Don't support MCA under DOS by default.
 *  Updated function prototypes.
 *  Revision 1.32  2008/01/31 09:06:03Z  martin
 *  Don't support MCA under DOS by default.
 *  Revision 1.31  2007/09/26 09:28:03Z  martin
 *  Added support for USB in general and new USB device USB5131.
 *  Renamed ..._USE_PCIMGR symbols to ..._USE_PCI_PNP.
 *  Renamed ..._USE_PCIBIOS symbols to ..._USE_PCI_BIOS.
 *  Added definition _PCPS_USE_PNP.
 *  Added new symbol _USE_ISA_PNP to exclude non-PNP stuff.
 *  from build if ISA devices are also handled by the PNP manager.
 *  Include mbgerror.h for new MBG_... codes.
 *  Added macro _pcps_ddev_status_busy().
 *  Added kernel malloc/free macros and USB I/O macros.
 *  Use PCPS_DDEV as private device data.
 *  Use ms values for USB timeouts also under Linux. This may not be
 *  appropriate for older kernels.
 *  Limited length of some older RCS log messages.
 *  Revision 1.30  2007/07/25 14:22:23Z  martin
 *  Under Linux include param.h for definition of HZ under
 *  kernels 2.6.21 and newer.
 *  Revision 1.29  2007/07/17 08:22:48  martin
 *  Added support for TCR511PEX and GPS170PEX.
 *  Revision 1.28  2007/07/16 12:58:00Z  martin
 *  Added  support for PEX511.
 *  Added new structures used for unified resource handling.
 *  Account for renamed library symbols.
 *  Revision 1.27  2007/03/02 09:41:05Z  martin
 *  Use generic port I/O macros.
 *  Added DEVICE_OBJECT to PCPS_DDEV under Windows.
 *  Define init code qualifier.
 *  Added new _pcps_..._timeout_clk() macros.
 *  Preliminary support for *BSD.
 *  Preliminary support for USB.
 *  Revision 1.26  2006/07/07 09:44:23  martin
 *  Fixed definition of control macros for the case where
 *  _PCPS_USE_PCI_PNP is overridden from the command line.
 *  Revision 1.25  2006/06/19 15:31:09  martin
 *  Added support for TCR511PCI.
 *  Updated function prototypes.
 *  Revision 1.24  2006/03/10 11:01:51  martin
 *  Added support for PCI511.
 *  Revision 1.23  2005/11/03 15:50:45Z  martin
 *  Added support for GPS170PCI.
 *  Revision 1.22  2005/06/02 10:35:09Z  martin
 *  Added macro _pcps_ddev_is_pci_amcc().
 *  Added macro _pcps_ddev_has_generic_io().
 *  Updated function prototypes.
 *  Revision 1.21  2004/12/09 11:03:38Z  martin
 *  Support configuration of on-board frequency synthesizer.
 *  Revision 1.20  2004/11/09 13:05:12Z  martin
 *  Fixed syntax bug in macro _pcps_ddev_fw_rev_num().
 *  New macro _pcps_ddev_has_gps_data().
 *  New macro _pcps_ddev_requires_irig_workaround().
 *  Revision 1.19  2004/10/14 15:01:24Z  martin
 *  Added support for TCR167PCI.
 *  Revision 1.18  2004/09/06 15:11:04Z  martin
 *  Support a GPS_DATA interface where sizes are specified
 *  by 16 instead of the original 8 bit quantities, thus allowing
 *  to transfer data blocks which exceed 255 bytes.
 *  Revision 1.17  2004/04/14 10:29:45Z  martin
 *  Pack structures 1 byte aligned.
 *  Revision 1.16  2004/04/07 09:47:19Z  martin
 *  New macros _pcps_ddev_has_irig() and
 *  _pcps_ddev_has_irig_tx().
 *  Revision 1.15  2004/03/10 17:32:23Z  martin
 *  Use CLOCKS_PER_SEC for timeout under QNX6 (Neutrino).
 *  Revision 1.14  2003/11/17 16:15:01  martin
 *  Support clock tick timeout for QNX.
 *  Revision 1.13  2003/07/08 15:07:32Z  martin
 *  Simplified definitions of default preprocessor macros.
 *  Compile for plug'n'play for Linux kernels 2.4.0 or newer.
 *  Updated function prototypes.
 *  Revision 1.12  2003/06/19 09:56:29  MARTIN
 *  Renamed macro ..clr_cap_buffer to ..clr_ucap_buffer.
 *  New macro _pcps_ddev_has_ucap().
 *  Changes due to renamed symbols.
 *  Updated function prototypes.
 *  Revision 1.11  2003/05/16 09:31:54  MARTIN
 *  Increased timeout loop count from 0x1000 to 0x7FFFFF.
 *  Rearranged inclusion of headers depending on the target.
 *  Added array for ISA port addresses.
 *  Revision 1.10  2003/04/09 16:30:24  martin
 *  Supports PCI510, GPS169PCI, and TCR510PCI,
 *  and new PCI_ASIC used by those devices.
 *  Renamed macro _pcps_ddev_is_irig() to _pcps_ddev_is_irig_rx().
 *  New macros _pcps_ddev_has_ref_offs(), _pcps_ddev_has_opt_flags().
 *  Preliminary support for PCPS_TZDL.
 *  Revision 1.9  2002/08/09 08:53:53  MARTIN
 *  New macro _pcps_ddev_can_clr_cap_buff().
 *  New macro _pcps_ddev_is_irig().
 *  New macro _pcps_ddev_has_signal().
 *  New macro _pcps_ddev_has_mod().
 *  Revision 1.8  2002/02/26 09:34:03  MARTIN
 *  Removed macro _pcps_read_sernum() which was replaced
 *  by a function pcps_read_sernum() which reads the S/N from
 *  any clock that supports a S/N.
 *  Updated function prototypes.
 *  Revision 1.7  2002/02/19 09:28:01  MARTIN
 *  Use new header mbg_tgt.h to check the target environment.
 *  Revision 1.6  2002/02/01 12:00:10  MARTIN
 *  Added new definitions for GPS168PCI.
 *  Renamed macro _pcps_ddev_rev_num to _pcps_ddev_fw_rev_num
 *  to follow naming conventions.
 *  Source code cleanup.
 *  Revision 1.5  2001/11/30 09:52:48  martin
 *  Added support for event_time which, however, requires
 *  a custom GPS firmware.
 *  Revision 1.4  2001/10/16 10:15:44  MARTIN
 *  New Macro _pcps_ddev_has_serial_hs() which determines
 *  whether DCF77 clock supports baud rate higher than default.
 *  Added some macros and comments  corresponding to
 *  pcpsdev.h.
 *  Revision 1.3  2001/09/18 06:53:57  MARTIN
 *  Two sets of preprocessor symbols for Win9x/ME and WinNT/2k.
 *  New preprocessor symbol controls usage of clock ticks for timeout.
 *  Changed type of PCPS_RSRC.irq_num from int to ushort.
 *  Updated function prototypes.
 *  Revision 1.2  2001/03/16 14:45:34  MARTIN
 *  New functions and definitions to support PNP drivers.
 *  Revision 1.1  2001/03/01 16:29:22  MARTIN
 *  Initial version for the new library.
 *
 **************************************************************************/

#ifndef _PCPSDRVR_H
#define _PCPSDRVR_H

/* Other headers to be included */
#include <mbg_tgt.h>
#include <xdevfeat.h>
#include <pcpsiobf.h>

#if !defined( USE_LOCAL_IO_BUFFER )
  #define USE_LOCAL_IO_BUFFER  0
#endif

#if defined( MBG_TGT_NETWARE )
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  1
  #define _DEFAULT_PCPS_USE_ISA         1
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_ISA_PNP     0
  #define _DEFAULT_PCPS_USE_PCI_PNP     0
  #define _DEFAULT_PCPS_USE_USB         0
  #define _DEFAULT_PCPS_USE_RSRCMGR     0
#elif defined( MBG_TGT_OS2 )
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  0
  #define _DEFAULT_PCPS_USE_ISA         1
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_ISA_PNP     0
  #define _DEFAULT_PCPS_USE_PCI_PNP     0
  #define _DEFAULT_PCPS_USE_USB         0
  #define _DEFAULT_PCPS_USE_RSRCMGR     1
#elif defined( MBG_TGT_WIN32_PNP )
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  1
  #define _DEFAULT_PCPS_USE_ISA         1
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_ISA_PNP     1
  #define _DEFAULT_PCPS_USE_PCI_PNP     1
  #define _DEFAULT_PCPS_USE_USB         1
  #define _DEFAULT_PCPS_USE_RSRCMGR     0
#elif defined( MBG_TGT_WIN32 )
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  1
  #define _DEFAULT_PCPS_USE_ISA         1
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_ISA_PNP     0
  #define _DEFAULT_PCPS_USE_PCI_PNP     0
  #define _DEFAULT_PCPS_USE_USB         0
  #define _DEFAULT_PCPS_USE_RSRCMGR     0
#elif defined( MBG_TGT_LINUX )
  #include <mbg_lx.h>
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  1
  #define _DEFAULT_PCPS_USE_ISA         defined( MBG_ARCH_X86 )
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_ISA_PNP     0
  #define _DEFAULT_PCPS_USE_PCI_PNP     _DEFAULT_MBG_TGT_LINUX_USE_PCI_PNP
  #define _DEFAULT_PCPS_USE_USB         _DEFAULT_MBG_TGT_LINUX_USE_USB
  #define _DEFAULT_PCPS_USE_RSRCMGR     1
#elif defined( MBG_TGT_BSD )  //##++
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  1
  #define _DEFAULT_PCPS_USE_ISA         1
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_ISA_PNP     0
  #define _DEFAULT_PCPS_USE_PCI_PNP     1
  #define _DEFAULT_PCPS_USE_USB         0
  #define _DEFAULT_PCPS_USE_RSRCMGR     1
#elif defined( MBG_TGT_QNX )
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  1
  #define _DEFAULT_PCPS_USE_ISA         1
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_ISA_PNP     0
  #define _DEFAULT_PCPS_USE_PCI_PNP     0
  #define _DEFAULT_PCPS_USE_USB         0
  #define _DEFAULT_PCPS_USE_RSRCMGR     0
#else  // DOS ...
  #define _DEFAULT_PCPS_USE_CLOCK_TICK  1
  #define _DEFAULT_PCPS_USE_ISA         1
  #define _DEFAULT_PCPS_USE_MCA         0
  #define _DEFAULT_PCPS_USE_PCI         1
  #define _DEFAULT_PCPS_USE_PCI_PNP     0
  #define _DEFAULT_PCPS_USE_ISA_PNP     0
  #define _DEFAULT_PCPS_USE_USB         0
  #define _DEFAULT_PCPS_USE_RSRCMGR     0
#endif

#ifndef _PCPS_USE_CLOCK_TICK
  #define _PCPS_USE_CLOCK_TICK  _DEFAULT_PCPS_USE_CLOCK_TICK
#endif

#ifndef _PCPS_USE_ISA
  #define _PCPS_USE_ISA         _DEFAULT_PCPS_USE_ISA
#endif

#ifndef _PCPS_USE_MCA
  #define _PCPS_USE_MCA         _DEFAULT_PCPS_USE_MCA
#endif

#ifndef _PCPS_USE_PCI
  #define _PCPS_USE_PCI         _DEFAULT_PCPS_USE_PCI
#endif

#ifndef _PCPS_USE_ISA_PNP
  #define _PCPS_USE_ISA_PNP     _DEFAULT_PCPS_USE_ISA_PNP
#endif

#ifndef _PCPS_USE_PCI_PNP
  #define _PCPS_USE_PCI_PNP     _DEFAULT_PCPS_USE_PCI_PNP
#endif

#ifndef _PCPS_USE_USB
  #define _PCPS_USE_USB         _DEFAULT_PCPS_USE_USB
#endif

#ifndef _PCPS_USE_RSRCMGR
  #define _PCPS_USE_RSRCMGR     _DEFAULT_PCPS_USE_RSRCMGR
#endif


#ifndef _PCPS_USE_PCI_BIOS
  #define _PCPS_USE_PCI_BIOS    ( _PCPS_USE_PCI && !_PCPS_USE_PCI_PNP )
#endif

#define _PCPS_USE_PNP           ( _PCPS_USE_PCI_PNP || _PCPS_USE_ISA_PNP || _PCPS_USE_USB )

#if _PCPS_USE_PCI_PNP && _PCPS_USE_PCI_BIOS
  #error "PCI PNP and non-PNP can't be used at the same time"
#endif


#if !defined( _MBG_INIT_CODE_ATTR )
  // define to empty string by default
  #define _MBG_INIT_CODE_ATTR
#endif

#if !defined( DEBUG_DRVR )
  #define DEBUG_DRVR  0
#endif

#if !defined( DEBUG_DEV_INIT )
  #define DEBUG_DEV_INIT  0
#endif

#if !defined( DEBUG_USB_IO )
  #define DEBUG_USB_IO  0
#endif

#if !defined( DEBUG_IOCTL )
  #define DEBUG_IOCTL  0
#endif


#include <pcpsdev.h>
#include <cfg_hlp.h>
#include <mbgmutex.h>
#include <pci_asic.h>
#include <mbgerror.h>
#include <use_pack.h>
#include <mbggenio.h>
#include <mbgddmsg.h>

#if defined( MBG_TGT_FREEBSD )
  #include <sys/malloc.h>
  #include <sys/_null.h>
  #include <sys/param.h>
  #include <sys/lock.h>
  #include <machine/bus.h>
#elif defined( MBG_TGT_NETBSD )
  #include <sys/kmem.h>
#else
  #include <stddef.h>
#endif

#if defined( MBG_TGT_DOS )
  #include <string.h>
  #include <time.h>
#endif

#if defined( MBG_TGT_WIN32 )
  #include <mbg_w32.h>
  #include <mbgnames.h>
#endif

#if defined( MBG_TGT_LINUX )
  #include <linux/slab.h>   // for kmalloc()/kfree()
  #include <linux/mm.h>     // for is_vmalloc_addr()

  #if _PCPS_USE_USB
    #include <linux/usb.h>
  #endif
#endif

#if defined( MBG_TGT_QNX )
  #include <mbg_qnx.h>
  #include <string.h>
  #include <time.h>
#endif

#if defined( MBG_TGT_NETWARE )
  #include <string.h>
  #include <time.h>
  #include <conio.h>
#endif

#if defined( MBG_TGT_OS2 )
  #ifndef OS2_INCLUDED
    #define INCL_DOSSEMAPHORES
    #include <os2.h>
    #include <rmbase.h>
  #endif

  #include <string.h>
  #include <time.h>
  #include <conio.h>
  #include <xportio.h>
#endif

#if _PCPS_USE_RSRCMGR
  #include <rsrc.h>
#endif


// If DEBUG is defined to build a debug version of this driver for
// older NetBSD versions then this collides with DEBUG being defined
// for the kernel itself, or not. As a workaround we define MBG_DEBUG
// instead when calling 'make', and we redefine DEBUG locally
// if MBG_DEBUG is defined.
#if defined( MBG_TGT_NETBSD ) && defined( MBG_DEBUG )
  #undef DEBUG
  #define DEBUG MBG_DEBUG
#endif



#ifdef _PCPSDRVR
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

// We use native alignment for structures which are not accessed across system boundaries.

#ifdef __cplusplus
extern "C" {
#endif

#if !defined( MBG_DRVR_NAME )
  #if defined( MBG_TGT_WIN32 )
    #define MBG_DRVR_NAME  MBG_SVC_NAME_MBGCLOCK
  #else
    #define MBG_DRVR_NAME  "mbgclock"
  #endif
#endif

_ext const char driver_name[]
#ifdef _DO_INIT
 = MBG_DRVR_NAME
#endif
;


#if !defined( _PCPS_USE_MM_IO )
  // MBG_TGT_SUPP_MEM_ACC determines if the target system
  // supports memory mapped access to I/O space.
  // _PCPS_USE_MM_IO determines if we use this (default,
  // if supported), or not.
  #define _PCPS_USE_MM_IO  MBG_TGT_SUPP_MEM_ACC
#endif


// Define some OS-specific primitives to alloc / free memory and handle
// mutexes and spinlocks in kernel space.

#if defined( MBG_TGT_LINUX )

  #define _pcps_kmalloc( _sz )      kmalloc( _sz, GFP_ATOMIC )
  #define _pcps_kfree( _p, _sz )    kfree( _p )


  // These special versions of _pcps_sem_inc() and _pcps_sem_dec()
  // are only required to prevent interference with the IRQ handler
  // under Linux which implements the serial port emulation for the
  // NTP parse driver.
  #define _pcps_sem_inc( _pddev )                          \
  {                                                        \
    ulong flags;                                           \
                                                           \
    if ( _mbg_mutex_acquire( &(_pddev)->dev_mutex ) < 0 )  \
      return -ERESTARTSYS;                                 \
                                                           \
    spin_lock_irqsave( &(_pddev)->irq_lock, flags );       \
    atomic_inc( &(_pddev)->access_in_progress );           \
    spin_unlock_irqrestore( &(_pddev)->irq_lock, flags );  \
  }

  #define _pcps_sem_dec( _pddev )                 \
    atomic_dec( &(_pddev)->access_in_progress );  \
    _mbg_mutex_release( &(_pddev)->dev_mutex )

#elif defined( MBG_TGT_FREEBSD )

  // malloc/free in kernel mode require usage of the
  // MALLOC_DECLARE() / MALLOC_DEFINE() macros.
  // See "man 9 malloc" for details.

  MALLOC_DECLARE( M_MBGCLOCK );
  #define _pcps_kmalloc( _sz )      malloc( _sz, M_MBGCLOCK, M_NOWAIT | M_ZERO )
  #define _pcps_kfree( _p, _sv )    free( _p, M_MBGCLOCK )

#elif defined( MBG_TGT_NETBSD )

  // For older NetBSD versions which do not suppport the calls
  // used below, see 'man 9 malloc'.
  #define _pcps_kmalloc( _sz )      kmem_alloc( _sz, KM_NOSLEEP )
  #define _pcps_kfree( _p, _sz )    kmem_free( _p, _sz )

#elif defined( MBG_TGT_WIN32 )

  #define _pcps_kmalloc( _sz )      ExAllocatePool( NonPagedPool, _sz )
  #define _pcps_kfree( _p, _sz )    ExFreePool( _p )

#elif defined( MBG_TGT_DOS )

  // No multitasking, no device driver,
  // so we don't need this.

  #define _pcps_sem_inc( _pddev ) \
    _nop_macro_fnc()

  #define _pcps_sem_dec( _pddev ) \
    _nop_macro_fnc()

#endif


#if !defined( _PCPS_STATIC_DEV_LIST )
  // On PNP systems buffers for device specific data are by default malloc'ed
  // whenever a device appears. However, a static array of a given maximum number
  // of devices is used on non-PNP systems.
  // This can be overridden for testing to avoid calling malloc in kernel space.
  #define _PCPS_STATIC_DEV_LIST ( !_PCPS_USE_PNP )
#endif



// If these macros have not yet been defined then define some dummies:

#if !defined( _pcps_sem_inc ) || !defined( _pcps_sem_dec )

  #define _pcps_sem_inc( _pddev ) \
    _mbg_mutex_acquire( &(_pddev)->dev_mutex )

  #define _pcps_sem_dec( _pddev ) \
    _mbg_mutex_release( &(_pddev)->dev_mutex )

#endif



// Default timeout count when accessing a device

#if !defined( PCPS_TIMEOUT_CNT )
  #if _PCPS_USE_CLOCK_TICK
    #if defined( MBG_TGT_NETWARE )
      #define PCPS_TIMEOUT_CNT ( (ulong)( 200 * __get_CLK_TCK() ) / 1000 )
    #elif defined( MBG_TGT_LINUX )
      #define PCPS_TIMEOUT_CNT ( (ulong)( 200 * HZ ) / 1000 )
    #elif defined( MBG_TGT_BSD )
      #define PCPS_TIMEOUT_CNT ( (ulong)( 200 ) )    // [ms]
    #elif defined( MBG_TGT_WIN32 )
      #define PCPS_TIMEOUT_CNT ( (ulong)( 200 * MBG_TICKS_PER_SEC ) / 1000 )
    #elif defined( MBG_TGT_QNX_NTO )
      #define PCPS_TIMEOUT_CNT ( (ulong)( 200 * CLOCKS_PER_SEC ) / 1000 )
    #else
      #define PCPS_TIMEOUT_CNT ( (ulong)( 200 * CLK_TCK ) / 1000 )
    #endif
  #else
    #define PCPS_TIMEOUT_CNT   0x7FFFFFUL
  #endif
#endif



/**
 * @brief The max number of bus memory resources used by a device.
 */
#define N_PCPS_MEM_RSRC  2


/**
 * @brief The max number of bus memory and I/O resources used by a device.
 */
#define MAX_PCPS_RSRC  ( N_PCPS_MEM_RSRC + N_PCPS_PORT_RSRC )


/**
 * @brief Resource info summary for a device
 */
typedef struct
{
  int num_rsrc_io;           ///< Number of actually assigned I/O address ranges
  int num_rsrc_mem;          ///< Number of actually assigned memory address ranges
  int num_rsrc_irq;          ///< Number of actually assigned IRQ numbers
  MBG_IOPORT_RSRC port[N_PCPS_PORT_RSRC];  ///< Info on actually assigned port ranges
  MBG_IOMEM_RSRC mem[N_PCPS_MEM_RSRC];     ///< Info on actually assigned memory ranges
  MBG_IRQ_RSRC irq;                        ///< Info on actually assigned IRQ numbers

} PCPS_RSRC_INFO;



#if _PCPS_USE_USB

/**
 * @brief Information on a USB endpoint
 */
typedef struct
{
  uint8_t addr;
  uint8_t attrib;
  uint16_t max_packet_size;

} PCPS_USB_EP;



static __mbg_inline
bool is_ep_rcv( uint8_t ep )
{
  return ( ep & 0x80 ) != 0;

}  // is_ep_rcv



#if defined( MBG_TGT_LINUX )

  // definitions used to control the cyclic USB read thread

  #if _PCPS_USE_LINUX_KTHREAD

    // Used by kthread_run() / kthread_stop()
    typedef struct task_struct *PCPS_THREAD_INFO;

  #else

    // Used by kernel_thread() / daemonize() / kill_proc()
    typedef struct
    {
      pid_t pid;
      char name[17];   // 16 chars as supported by the kernel, plus trailing 0
      struct completion exit;

    } PCPS_THREAD_INFO;

  #endif  // _PCPS_USE_LINUX_KTHREAD

#endif  // defined( MBG_TGT_LINUX )

#endif  // _PCPS_USE_USB



/**
 * @brief Memory layout of Meinberg PCI interface register
 */
typedef union
{
  struct pex8311
  {
    PCI_ASIC asic;
    PCPS_TIME_STAMP tstamp;
  } pex8311;

  struct mbgpex
  {
    PCI_ASIC asic;
    uint8_t b[256 - sizeof( PCI_ASIC ) ];
    PCPS_TIME_STAMP ucap[2];
    PCPS_TIME_STAMP tstamp;
  } mbgpex;

} PCPS_MM_LAYOUT;



struct PCPS_DDEV_s;
typedef struct PCPS_DDEV_s PCPS_DDEV;

typedef int PCPS_READ_FNC( PCPS_DDEV *pddev, uint8_t cmd, void FAR *buffer, uint16_t count );
typedef int PCPS_WRITE_FNC( PCPS_DDEV *pddev, uint8_t cmd, const void FAR *buffer, uint16_t count );
typedef int PCPS_DDEV_INIT_FNC( PCPS_DDEV **ppddev );
typedef void PCPS_DDEV_CLEANUP_FNC( PCPS_DDEV *pddev );
typedef int PCPS_DDEV_REGISTER_FNC( PCPS_DDEV *pddev );



struct PCPS_DDEV_s
{
  PCPS_DEV dev;             ///< Device info data that can be passed to user space.

  PCPS_READ_FNC *read;      ///< Pointer to the read function depending on the access mode.
  uint access_mode;         ///< Access mode used for the device, depending on interface type. See ::PCPS_ACCESS_MODES.
  bool access_mode_forced;  ///< Flag indicating that the access mode was forced.
  MBG_IOPORT_ADDR_MAPPED status_port_offs;
  MBG_IOPORT_ADDR_MAPPED status_port;        ///< Address of the status port register.
  MBG_IOPORT_ADDR_MAPPED irq_enb_disb_port;  ///< Address of the IRQ control register.
  MBG_IOPORT_ADDR_MAPPED irq_flag_port;      ///< Address of the IRQ status register.
  MBG_IOPORT_ADDR_MAPPED irq_ack_port;       ///< Address of the register to acknowledge an IRQ.
  uint32_t irq_enb_mask;   ///< Bit mask to be set to enable IRQs.
  uint32_t irq_disb_mask;  ///< Bit mask to be cleared to disable IRQs.
  uint32_t irq_flag_mask;  ///< Bit mask used to check if device has generated an IRQ.
  uint32_t irq_ack_mask;   ///< Bit mask to be set to acknowledge an IRQ.

  PCI_ASIC_VERSION raw_asic_version;   ///< Raw ASIC version.
  PCI_ASIC_VERSION asic_version;       ///< ASIC version.
  PCI_ASIC_FEATURES asic_features;     ///< ASIC feature mask.
  PCPS_RSRC_INFO rsrc_info;            ///< Summary of resources used by the device.

  MBG_PC_CYCLES acc_cycles;            ///< Cycles count taken when device was accessed last time.

  #if defined( _MBG_MUTEX_DEFINED )
    MBG_MUTEX dev_mutex;               ///< Mutex used for device access serialization.
  #endif

  /// Base address of the ASIC part of the register block
  /// if it has been mapped to memory, else NULL.
  PCI_ASIC _MBG_IOMEM *mm_asic_addr;

  /// Base address of the timestamp register if it has been
  /// mapped to memory, else NULL.
  PCPS_TIME_STAMP _MBG_IOMEM *mm_tstamp_addr;

  #if defined( _MBG_SPINLOCK_DEFINED )
    MBG_SPINLOCK tstamp_lock;  ///< Spinlock used to protect memory mapped access to the timestamp registers.
    MBG_SPINLOCK irq_lock;     ///< Spinlock used to protect access to data updated by IRQ handler.
  #endif

  /// IRQ status information, e.g. whether the device's IRQ is
  /// possibly unsafe, and whether IRQ has been enabled on the device.
  PCPS_IRQ_STAT_INFO irq_stat_info;

  /// The mask of builtin features, as constantly defined for the device.
  BUILTIN_FEATURE_MASK default_builtin_features;

  /// The real mask of builtin features, which may have been updated
  /// e.g depending on the firmware version.
  BUILTIN_FEATURE_MASK real_builtin_features;

  /// Receiver info plus extended device features.
  MBG_XDEV_FEATURES xdev_features;

  #if _PCPS_USE_USB
    int n_usb_ep;                          ///< Number of USB endpoints supported by the device.
    PCPS_USB_EP ep[MBGUSB_MAX_ENDPOINTS];  ///< Array of actual USB endpoints.
    bool usb_20_mode;                      ///< Flag indicating if USB 2.0 microframing is supported.
  #endif

  /// A tiny buffer that can be used to write a command or GPS (large data) command
  /// to a device. Such buffer may need to be DMA capable, which is the case for
  /// this private data structure, but is usually not the case for automatic variables.
  /// So if we use this we avoid having to allocate/free an extra buffer for each access.
  union
  {
    /// In case of small data we just need one of the @ref PCPS_CMD_CODES.
    uint8_t cmd;

    /// In case of large data we first need ::PCPS_READ_GPS_DATA or ::PCPS_WRITE_GPS_DATA,
    /// then one of the @ref PC_GPS_CMD_CODES.
    struct
    {
      uint8_t cmd;      ///< ::PCPS_READ_GPS_DATA or ::PCPS_WRITE_GPS_DATA.
      uint8_t gps_cmd;  ///< One of the @ref PC_GPS_CMD_CODES.
    } gps_cmd_info;

  } cmd_info;

  uint16_t n_bytes;
  uint8_t size_n_bytes;
  uint8_t uc;

  #if !USE_LOCAL_IO_BUFFER
    PCPS_IO_BUFFER io_buffer;
  #endif

  #if defined( MBG_TGT_WIN32 )
    _pcps_ddev_data_win        ///< Some Windows-specific stuff.
  #endif

  #if defined( MBG_TGT_LINUX )
    atomic_t connected;               ///< Flag indicating if the device is "connected"
    atomic_t access_in_progress;      ///< Flag indicating if device access is currently in progress
    atomic_t data_avail;              ///< Flag indicating if data has been made available by IRQ handler
    unsigned long jiffies_at_irq;     ///< Set by IRQ handler, used to check if cyclic IRQs still occur
    struct fasync_struct *fasyncptr;  ///< Used for asynchronous signalling when data is available
    PCPS_TIME t;                      ///< Date and time read by IRQ handler

    #if NEW_WAIT_QUEUE
      wait_queue_head_t wait_queue;   ///< Used for asynchronous I/O (newer kernel API)
    #else
      struct wait_queue *wait_queue;  ///< Used for asynchronous I/O (older kernel API)
    #endif

    atomic_t open_count;              ///< Number of processes that have opened this device

    struct cdev cdev;                 ///< Linux device class
    dev_t lx_dev;                     ///< Linux device associated with this device

    #if _PCPS_USE_USB
      struct usb_device *udev;           ///< Linux USB device associated with this device
      struct usb_interface *intf;        ///< Linux USB interface associated with this device
      PCPS_THREAD_INFO usb_read_thread;  ///< Kernel thread to receive cyclic USB messages
      struct semaphore sem_usb_cyclic;   ///< Semaphore used for cyclic USB messages
      PCPS_TIME t_cyc;                   ///< Buffer for the time read in cyclic USB messages
    #endif
  #endif

  #if defined( MBG_TGT_BSD )
    int connected;                    ///< BSD flag indicating if the device is "connected"
    int open_count;                   ///< BSD number of processes that have opened this device
  #endif

  #if _PCPS_USE_RSRCMGR
    #if defined( MBG_TGT_OS2 )
      PCPS_HDEV hDev;
      RSRC_LIST rsrc;
    #endif
  #endif
};



/**
 * @brief Codes used with ::PCPS_DDEV::access_mode
 *
 * @see ::PCPS_ACCESS_MODE_STRS
 * @see ::PCPS_ACCESS_MODE_STR_FRCD
 */
enum PCPS_ACCESS_MODES
{
  PCPS_ACC_MODE_NULL,      ///< No real I/O, dummy routine used
  PCPS_ACC_MODE_USB,       ///< USB I/O, no direct port access
  PCPS_ACC_MODE_IO,        ///< Standard port I/O
  PCPS_ACC_MODE_MM,        ///< 32 bit memory mapped access
  PCPS_ACC_MODE_MM16,      ///< 16 bit memory mapped access
  N_PCPS_ACCESS_MODES
};



/**
 * @brief Device access mode info strings
 *
 * @see ::PCPS_ACCESS_MODES
 * @see ::PCPS_ACCESS_MODE_STR_FRCD
 */
#define PCPS_ACCESS_MODE_STRS      \
{                                  \
  "No real port I/O (dummy fnc)",  \
  "I/O via USB",                   \
  "Standard port I/O",             \
  "Memory mapped I/O",             \
  "16 bit memory mapped I/O"       \
}

/**
 * @brief String to append if access mode has been forced
 *
 * @see ::PCPS_ACCESS_MODE_STRS
 */
#define PCPS_ACCESS_MODE_STR_FRCD   " (forced)"



/**
 * @brief The number of address lines decoded by a PCI clock
 */
#define PCPS_DECODE_WIDTH_PCI   16


/* ------ definitions used with MCA clocks -------------------------- */

/*
 * The MCA adapter ID number is used to detect a MCA clock in a
 * system and query which resources have been assigned by the
 * system's POS (programmable option select).
 */

#define MCA_ID_PS31        0x6AAC   ///< MCA adapter ID assigned by IBM
#define MCA_ID_PS31_OLD    0x6303   ///< MCA adapter ID assigned by Meinberg, used with the first series of PS31 boards

/**
 * @brief The total number of ports acquired by an MCA device
 */
#define PCPS_NUM_PORTS_MCA      16

/**
 * @brief The number of address lines decoded by an MCA device
 */
#define PCPS_DECODE_WIDTH_MCA   16


/* ------ definitions used with ISA clocks -------------------------- */

/**
 * @brief A board ID for later ISA bus devices
 *
 * The number can be read at port_base + 2 (low byte), and
 * port_base + 3 (high byte) of ISA devices. This ID number
 * matches the MCA adapter ID ::MCA_ID_PS31 and is not
 * provided by PC31 devices.
 */
#define ISA_ID_PCPS             MCA_ID_PS31


/**
 * @brief The default port base address for ISA bus devices
 *
 * Some programs assume a default port for an ISA default,
 * but others do not but require a cmd line parameter.
 */
#define PCPS_DEFAULT_PORT       0x0300


/**
 * @brief The total number of I/O ports used by an ISA bus device
 */
#define PCPS_NUM_PORTS_ISA      4


/**
 * @brief The number of address lines decoded by an ISA bus device
 */
#define PCPS_DECODE_WIDTH_ISA   10


/* ------ common definitions -------------------------- */

#if defined( DEBUG )
  _ext int debug
  #ifdef _DO_INIT
    = DEBUG
  #endif
  ;
#endif


_ext PCPS_DEV_TYPE pcps_dev_type[N_PCPS_DEV_TYPE]
#ifdef _DO_INIT
= {  // attention, the name is limited to PCPS_CLOCK_NAME_SZ, including terminating 0
  { PCPS_TYPE_PC31,      "PC31",      0,                 PCPS_REF_DCF,  PCPS_BUS_ISA },
  { PCPS_TYPE_PS31_OLD,  "PS31",      MCA_ID_PS31_OLD,   PCPS_REF_DCF,  PCPS_BUS_MCA },
  { PCPS_TYPE_PS31,      "PS31",      MCA_ID_PS31,       PCPS_REF_DCF,  PCPS_BUS_MCA },
  { PCPS_TYPE_PC32,      "PC32",      ISA_ID_PCPS,       PCPS_REF_DCF,  PCPS_BUS_ISA },
  { PCPS_TYPE_PCI32,     "PCI32",     PCI_DEV_PCI32,     PCPS_REF_DCF,  PCPS_BUS_PCI_S5933 },
  { PCPS_TYPE_GPS167PC,  "GPS167PC",  0,                 PCPS_REF_GPS,  PCPS_BUS_ISA },
  { PCPS_TYPE_GPS167PCI, "GPS167PCI", PCI_DEV_GPS167PCI, PCPS_REF_GPS,  PCPS_BUS_PCI_S5933 },
  { PCPS_TYPE_PCI509,    "PCI509",    PCI_DEV_PCI509,    PCPS_REF_DCF,  PCPS_BUS_PCI_S5920 },
  { PCPS_TYPE_GPS168PCI, "GPS168PCI", PCI_DEV_GPS168PCI, PCPS_REF_GPS,  PCPS_BUS_PCI_S5920 },
  { PCPS_TYPE_PCI510,    "PCI510",    PCI_DEV_PCI510,    PCPS_REF_DCF,  PCPS_BUS_PCI_ASIC },
  { PCPS_TYPE_GPS169PCI, "GPS169PCI", PCI_DEV_GPS169PCI, PCPS_REF_GPS,  PCPS_BUS_PCI_ASIC },
  { PCPS_TYPE_TCR510PCI, "TCR510PCI", PCI_DEV_TCR510PCI, PCPS_REF_IRIG, PCPS_BUS_PCI_ASIC },
  { PCPS_TYPE_TCR167PCI, "TCR167PCI", PCI_DEV_TCR167PCI, PCPS_REF_IRIG, PCPS_BUS_PCI_ASIC },
  { PCPS_TYPE_GPS170PCI, "GPS170PCI", PCI_DEV_GPS170PCI, PCPS_REF_GPS,  PCPS_BUS_PCI_ASIC },
  { PCPS_TYPE_PCI511,    "PCI511",    PCI_DEV_PCI511,    PCPS_REF_DCF,  PCPS_BUS_PCI_ASIC },
  { PCPS_TYPE_TCR511PCI, "TCR511PCI", PCI_DEV_TCR511PCI, PCPS_REF_IRIG, PCPS_BUS_PCI_ASIC },
  { PCPS_TYPE_PEX511,    "PEX511",    PCI_DEV_PEX511,    PCPS_REF_DCF,  PCPS_BUS_PCI_PEX8311 },
  { PCPS_TYPE_TCR511PEX, "TCR511PEX", PCI_DEV_TCR511PEX, PCPS_REF_IRIG, PCPS_BUS_PCI_PEX8311 },
  { PCPS_TYPE_GPS170PEX, "GPS170PEX", PCI_DEV_GPS170PEX, PCPS_REF_GPS,  PCPS_BUS_PCI_PEX8311 },
  { PCPS_TYPE_USB5131,   "USB5131",   USB_DEV_USB5131,   PCPS_REF_DCF,  PCPS_BUS_USB },
  { PCPS_TYPE_TCR51USB,  "TCR51USB",  USB_DEV_TCR51USB,  PCPS_REF_IRIG, PCPS_BUS_USB },
  { PCPS_TYPE_MSF51USB,  "MSF51USB",  USB_DEV_MSF51USB,  PCPS_REF_MSF,  PCPS_BUS_USB },
  { PCPS_TYPE_PTP270PEX, "PTP270PEX", PCI_DEV_PTP270PEX, PCPS_REF_PTP,  PCPS_BUS_PCI_PEX8311 },
  { PCPS_TYPE_FRC511PEX, "FRC511PEX", PCI_DEV_FRC511PEX, PCPS_REF_FRC,  PCPS_BUS_PCI_PEX8311 },
  { PCPS_TYPE_TCR170PEX, "TCR170PEX", PCI_DEV_TCR170PEX, PCPS_REF_IRIG, PCPS_BUS_PCI_PEX8311 },
  { PCPS_TYPE_WWVB51USB, "WWVB51USB", USB_DEV_WWVB51USB, PCPS_REF_WWVB, PCPS_BUS_USB },
  { PCPS_TYPE_GPS180PEX, "GPS180PEX", PCI_DEV_GPS180PEX, PCPS_REF_GPS,  PCPS_BUS_PCI_MBGPEX },
  { PCPS_TYPE_TCR180PEX, "TCR180PEX", PCI_DEV_TCR180PEX, PCPS_REF_IRIG, PCPS_BUS_PCI_MBGPEX },
  { PCPS_TYPE_DCF600USB, "DCF600USB", USB_DEV_DCF600USB, PCPS_REF_DCF,  PCPS_BUS_USB_V2 },
  { PCPS_TYPE_PZF180PEX, "PZF180PEX", PCI_DEV_PZF180PEX, PCPS_REF_DCF,  PCPS_BUS_PCI_MBGPEX },
  { PCPS_TYPE_TCR600USB, "TCR600USB", USB_DEV_TCR600USB, PCPS_REF_IRIG, PCPS_BUS_USB_V2 },
  { PCPS_TYPE_MSF600USB, "MSF600USB", USB_DEV_MSF600USB, PCPS_REF_MSF,  PCPS_BUS_USB_V2 },
  { PCPS_TYPE_WVB600USB, "WVB600USB", USB_DEV_WVB600USB, PCPS_REF_WWVB, PCPS_BUS_USB_V2 },
  { PCPS_TYPE_GLN180PEX, "GLN180PEX", PCI_DEV_GLN180PEX, PCPS_REF_GPS,  PCPS_BUS_PCI_MBGPEX },
  { PCPS_TYPE_GPS180AMC, "GPS180AMC", PCI_DEV_GPS180AMC, PCPS_REF_GPS,  PCPS_BUS_PCI_MBGPEX },
  { PCPS_TYPE_GNS181PEX, "GNS181PEX", PCI_DEV_GNS181PEX, PCPS_REF_GPS,  PCPS_BUS_PCI_MBGPEX },
  { PCPS_TYPE_TCR180USB, "TCR180USB", USB_DEV_TCR180USB, PCPS_REF_IRIG, PCPS_BUS_USB_V2 }

  // NOTE If a new device is added here, don't forget to add it also
  // to the Windows .inf file of supported PCI and USB devices,
  // and in case of USB to the table mbgclock_usb_tbl of the Linux
  // driver file mbgclock_main.c.
}
#endif
;


#if !defined( PCPS_MAX_ISA_CARDS )
  #define PCPS_MAX_ISA_CARDS  N_SUPP_DEV_BUS  // the number of ISA cards supported
#endif

_ext int pcps_isa_ports[PCPS_MAX_ISA_CARDS + 1];

#if _PCPS_STATIC_DEV_LIST
  _ext PCPS_DDEV pcps_ddev[N_SUPP_DEV_BUS];
  _ext int n_ddevs;
#endif

#if defined( MBG_TGT_DOS ) || defined( MBG_TGT_NETWARE ) //##++
  _ext int curr_ddev_num;
  _ext PCPS_DDEV *curr_ddev
  #ifdef _DO_INIT
   = &pcps_ddev[0]
  #endif
  ;
#endif


/**
 * @brief The first characters of a valid firmware ID
 */
_ext const char *fw_id_ref[]
#ifdef _DO_INIT
 = {
     "PC3",      // PC31, PS31, PC32
     "PCI",      // PCI32, PCI509, PCI510, PCI511
     "GPS",      // GPS167PC, GPS167PCI, GPS168PCI, GPS169PCI, GPS170PCI, GPS170PEX, GPS180PEX, GPS180AMC
     "TCR",      // TCR510PCI, TCR167PCI, TCR511PCI, TCR511PEX, TCR51USB, TCR170PEX, TCR180PEX
     "PEX",      // PEX511
     "USB",      // USB5131
     "MSF",      // MSF51USB, MSF600USB
     "WWVB",     // WWVB51USB, WVB600USB
     "DCF",      // DCF600USB
     "PZF",      // PZF180PEX
     "GLN",      // GLN180PEX
     "GNS",      // GNS181PEX
     NULL
   }
#endif
;


// These macros are used to distinguish ISA cards:
#define fw_id_ref_pcps    fw_id_ref[0]
#define fw_id_ref_gps     fw_id_ref[2]


#if _PCPS_USE_MM_IO
  _ext int force_io_access;
  _ext int force_mm16_access;
#endif


// These macros accept a (PCPS_DDEV *) for easy access
// to the information stored in PCPS_DDEV structures.

// Access device type information:
#define _pcps_ddev_type_num( _p )        _pcps_type_num( &(_p)->dev )
#define _pcps_ddev_type_name( _p )       _pcps_type_name( &(_p)->dev )
#define _pcps_ddev_dev_id( _p )          _pcps_dev_id( &(_p)->dev )
#define _pcps_ddev_ref_type( _p )        _pcps_ref_type( &(_p)->dev )
#define _pcps_ddev_bus_flags( _p )       _pcps_bus_flags( &(_p)->dev )

// Query device type features:

#define _pcps_ddev_is_gps( _p )          _pcps_is_gps( &(_p)->dev )
#define _pcps_ddev_is_dcf( _p )          _pcps_is_dcf( &(_p)->dev )
#define _pcps_ddev_is_msf( _p )          _pcps_is_msf( &(_p)->dev )
#define _pcps_ddev_is_wwvb( _p )         _pcps_is_wwvb( &(_p)->dev )
#define _pcps_ddev_is_irig_rx( _p )      _pcps_is_irig_rx( &(_p)->dev )
#define _pcps_ddev_is_ptp( _p )          _pcps_is_ptp( &(_p)->dev )
#define _pcps_ddev_is_frc( _p )          _pcps_is_frc( &(_p)->dev )

#define _pcps_ddev_is_lwr( _p )          _pcps_is_lwr( &(_p)->dev )
#define _pcps_ddev_is_gnss( _p )         _pcps_is_gnss( &(_p)->dev )


// Generic bus types:
#define _pcps_ddev_is_isa( _p )          _pcps_is_isa( &(_p)->dev )
#define _pcps_ddev_is_mca( _p )          _pcps_is_mca( &(_p)->dev )
#define _pcps_ddev_is_pci( _p )          _pcps_is_pci( &(_p)->dev )
#define _pcps_ddev_is_usb( _p )          _pcps_is_usb( &(_p)->dev )

// Special bus types:
#define _pcps_ddev_is_usb_v2( _p )       _pcps_is_usb_v2( &(_p)->dev )
#define _pcps_ddev_is_pci_s5933( _p )    _pcps_is_pci_s5933( &(_p)->dev )
#define _pcps_ddev_is_pci_s5920( _p )    _pcps_is_pci_s5920( &(_p)->dev )
#define _pcps_ddev_is_pci_amcc( _p )     _pcps_is_pci_amcc( &(_p)->dev )
#define _pcps_ddev_is_pci_asic( _p )     _pcps_is_pci_asic( &(_p)->dev )
#define _pcps_ddev_is_pci_pex8311( _p )  _pcps_is_pci_pex8311( &(_p)->dev )
#define _pcps_ddev_is_pci_mbgpex( _p )   _pcps_is_pci_mbgpex( &(_p)->dev )


// Access device configuration information:
#define _pcps_ddev_bus_num( _p )         _pcps_bus_num( &(_p)->dev )
#define _pcps_ddev_slot_num( _p )        _pcps_slot_num( &(_p)->dev )
#define _pcps_ddev_pci_slot_num( _p )    ( _pcps_ddev_slot_num( _p ) >> 3 )
#define _pcps_ddev_pci_fnc_num( _p )     ( _pcps_ddev_slot_num( _p ) & 0x07 )

#define _pcps_ddev_short_port_rsrc( _p, _n )   _pcps_short_port_rsrc( &(_p)->dev, _n )
#define _pcps_ddev_short_port_base( _p, _n )   _pcps_short_port_base( &(_p)->dev, _n )

#define _pcps_ddev_io_rsrc( _p, _n )         ( (_p)->rsrc_info.port[_n] )
#define _pcps_ddev_io_base_raw( _p, _n )     ( _pcps_ddev_io_rsrc( _p, _n ).base_raw )
#define _pcps_ddev_io_base_mapped( _p, _n )  ( _pcps_ddev_io_rsrc( _p, _n ).base_mapped )
#define _pcps_ddev_irq_num( _p )             _pcps_irq_num( &(_p)->dev )
#define _pcps_ddev_timeout_clk( _p )         _pcps_timeout_clk( &(_p)->dev )

#define _pcps_ddev_mem_rsrc( _p, _n )        ( (_p)->rsrc_info.mem[_n] )

#define _pcps_ddev_fw_rev_num( _p )      _pcps_fw_rev_num( &(_p)->dev )
#define _pcps_ddev_features( _p )        _pcps_features( &(_p)->dev )
#define _pcps_ddev_fw_id( _p )           _pcps_fw_id( &(_p)->dev )
#define _pcps_ddev_sernum( _p )          _pcps_sernum( &(_p)->dev )
#define _pcps_ddev_sernum_size( _p )     sizeof( (_p)->dev.cfg.sernum )

#define _pcps_ddev_raw_asic_version( _p )  ( (_p)->raw_asic_version )
#define _pcps_ddev_asic_version( _p )      ( (_p)->asic_version )

// These macros handle the device's err_flags:
#define _pcps_ddev_err_flags( _p ) \
        _pcps_err_flags( &(_p)->dev )

#define _pcps_ddev_chk_err_flags( _p, _msk ) \
        _pcps_chk_err_flags( &(_p)->dev, _msk )

#define _pcps_ddev_set_err_flags( _p, _msk ) \
        _pcps_set_err_flags( &(_p)->dev, _msk )

#define _pcps_ddev_clr_err_flags( _p, _msk ) \
        _pcps_clr_err_flags( &(_p)->dev, _msk )



// These macros test whether access is done via MM
// or via legacy port I/O:

#define _pcps_ddev_access_mode_io( _p )  \
        ( (_p)->access_mode == PCPS_ACC_MODE_IO )

#define _pcps_ddev_access_mode_mm( _p )  \
        ( ( (_p)->access_mode == PCPS_ACC_MODE_MM ) || \
          ( (_p)->access_mode == PCPS_ACC_MODE_MM16 ) )



// Query whether a specific feature is supported:
#define _pcps_ddev_can_set_time( _p )    _pcps_can_set_time( &(_p)->dev )
#define _pcps_ddev_has_serial( _p )      _pcps_has_serial( &(_p)->dev )
#define _pcps_ddev_has_sync_time( _p )   _pcps_has_sync_time( &(_p)->dev )
#define _pcps_ddev_has_ident( _p )       _pcps_has_ident( &(_p)->dev )
#define _pcps_ddev_has_utc_offs( _p )    _pcps_has_utc_offs( &(_p)->dev )
#define _pcps_ddev_has_hr_time( _p )     _pcps_has_hr_time( &(_p)->dev )
#define _pcps_ddev_has_sernum( _p )      _pcps_has_sernum( &(_p)->dev )
#define _pcps_ddev_has_cab_len( _p )     _pcps_has_cab_len( &(_p)->dev )
#define _pcps_ddev_has_tzdl( _p )        _pcps_has_tzdl( &(_p)->dev )
#define _pcps_ddev_has_pcps_tzdl( _p )   _pcps_has_pcps_tzdl( &(_p)->dev )
#define _pcps_ddev_has_tzcode( _p )      _pcps_has_tzcode( &(_p)->dev )
#define _pcps_ddev_has_tz( _p )          _pcps_has_tz( &(_p)->dev )
// The next one is supported only with a certain GPS firmware version:
#define _pcps_ddev_has_event_time( _p )  _pcps_has_event_time( &(_p)->dev )
#define _pcps_ddev_has_receiver_info( _p ) _pcps_has_receiver_info( &(_p)->dev )
#define _pcps_ddev_can_clr_ucap_buff( _p ) _pcps_can_clr_ucap_buff( &(_p)->dev )
#define _pcps_ddev_has_ucap( _p )        _pcps_has_ucap( &(_p)->dev )
#define _pcps_ddev_has_irig_tx( _p )     _pcps_has_irig_tx( &(_p)->dev )

// This macro determines whether a DCF77 clock
// supports a higher baud rate than standard
#define _pcps_ddev_has_serial_hs( _p ) \
        _pcps_has_serial_hs( &(_p)->dev )


#define _pcps_ddev_has_signal( _p ) \
        _pcps_has_signal( &(_p)->dev )

#define _pcps_ddev_has_mod( _p ) \
        _pcps_has_mod( &(_p)->dev )

#define _pcps_ddev_has_irig( _p ) \
        _pcps_has_irig( &(_p)->dev )

#define _pcps_ddev_has_irig_ctrl_bits( _p ) \
        _pcps_has_irig_ctrl_bits( &(_p)->dev )

#define _pcps_ddev_has_irig_time( _p ) \
        _pcps_has_irig_time( &(_p)->dev )

#define _pcps_ddev_has_raw_irig_data( _p ) \
        _pcps_has_raw_irig_data( &(_p)->dev )

#define _pcps_ddev_has_ref_offs( _p ) \
        _pcps_has_ref_offs( &(_p)->dev )

#define _pcps_ddev_has_opt_flags( _p ) \
        _pcps_has_opt_flags( &(_p)->dev )

#define _pcps_ddev_has_gps_data_16( _p ) \
        _pcps_has_gps_data_16( &(_p)->dev )

#define _pcps_ddev_has_gps_data( _p ) \
        _pcps_has_gps_data( &(_p)->dev )

#define _pcps_ddev_has_synth( _p ) \
        _pcps_has_synth( &(_p)->dev )

#define _pcps_ddev_has_generic_io( _p ) \
        _pcps_has_generic_io( &(_p)->dev )

#define _pcps_ddev_has_time_scale( _p ) \
        _pcps_has_time_scale( &(_p)->dev )

#define _pcps_ddev_has_utc_parm( _p ) \
        _pcps_has_utc_parm( &(_p)->dev )

#define _pcps_ddev_has_asic_version( _p ) \
        _pcps_has_asic_version( &(_p)->dev )

#define _pcps_ddev_has_asic_features( _p ) \
        _pcps_has_asic_features( &(_p)->dev )

#define _pcps_ddev_has_fast_hr_timestamp( _p ) \
        _pcps_has_fast_hr_timestamp( &(_p)->dev )

#define _pcps_ddev_has_lan_intf( _p ) \
        _pcps_has_lan_intf( &(_p)->dev )

#define _pcps_ddev_has_ptp( _p ) \
        _pcps_has_ptp( &(_p)->dev )

#define _pcps_ddev_has_ptp_unicast( _p ) \
        _pcps_has_ri_ptp_unicast( _ri_addr( _p ) )

#define _pcps_ddev_has_pzf( _p ) \
        _pcps_has_pzf( &(_p)->dev )

#define _pcps_ddev_has_corr_info( _p ) \
        _pcps_has_corr_info( &(_p)->dev )

#define _pcps_ddev_has_tr_distance( _p ) \
        _pcps_has_tr_distance( &(_p)->dev )

#define _pcps_ddev_has_evt_log( _p ) \
        _pcps_has_evt_log( &(_p)->dev )

#define _pcps_ddev_has_debug_status( _p ) \
        _pcps_has_debug_status( &(_p)->dev )

#define _pcps_ddev_has_stat_info( _p ) \
        _pcps_has_stat_info( &(_p)->dev )

#define _pcps_ddev_has_stat_info_mode( _p ) \
        _pcps_has_stat_info_mode( &(_p)->dev ) \

#define _pcps_ddev_has_stat_info_svs( _p ) \
        _pcps_has_stat_info_svs( &(_p)->dev ) \

#define _pcps_ddev_incoming_tfom_ignored( _p ) \
        _pcps_incoming_tfom_ignored( &(_p)->dev  )

#define _pcps_ddev_pci_cfg_err( _p ) \
        _pcps_pci_cfg_err( &(_p)->dev  )

#define _pcps_ddev_has_gpio( _p ) \
        _pcps_has_ri_gpio( _ri_addr( _p ) )

#define _pcps_ddev_has_xmr( _p ) \
        _pcps_has_ri_xmr( _ri_addr( _p ) )

#define _pcps_ddev_fw_has_20ms_bug( _p ) \
        _pcps_fw_has_20ms_bug( &(_p)->dev  )


// These macros simplify read/write access to the clocks.

// Call the device's read function to write the command byte _cmd
// and read _n bytes to buffer _s.
#if !defined( _pcps_read )
  #define _pcps_read( _pddev, _cmd, _p, _n )  \
    ( (_pddev)->read( _pddev, (_cmd), (uchar FAR *)(_p), (_n) ) )
#endif

// Write a byte _b to a device. This is typically done by just writing
// the command byte from within the read function, without actually
// reading any data bytes.
#if !defined( _pcps_write_byte )
  #define _pcps_write_byte( _pddev, _b )  \
    _pcps_read( (_pddev), (_b), NULL, 0 )
#endif

// write a command plus the contents of a data buffer to a device.
// This is typically implemented as a function which uses the
// _pcps_write_byte() macro repeatedly.
#if !defined( _pcps_write )
  #define _pcps_write( _pddev, _cmd, _p, _n )  \
    pcps_write( (_pddev), (_cmd), (uchar FAR *)(_p), (_n) )
#endif

// Read data structures which exceed ::PCPS_FIFO_SIZE bytes.
// This can't be handled in a single read cycle and due to
// limitations of the device's microprocessor the execution time
// can be up to 20 milliseconds, depending on the device type.
// This has been introduced with the first GPS devices but is
// now in fact also used with non-GPS devices.
#define _pcps_read_gps( _pddev, _cmd, _p, _n )  \
  pcps_read_gps( (_pddev), (_cmd), (uchar FAR *)(_p), (_n) )

// The complementary write function for the read function above.
#define _pcps_write_gps( _pddev, _cmd, _p, _n )  \
  pcps_write_gps( (_pddev), (_cmd), (uchar FAR *)(_p), (_n) )



// These macros simplify reading/writing typed variables by
// determining the size automatically from the type of the variable.

// Read data from a device to variable _s.
// The number of bytes to read is determined by the size
// of _s. The accepted type of _s depends on the _cmd code.
#define _pcps_read_var( _pddev, _cmd, _s )  \
  _pcps_read( (_pddev), (_cmd), &(_s), sizeof( (_s) ) )

// Write data from variable _s to a device.
// The number of bytes to write is determined by the size
// of _s. The accepted type of _s depends on the _cmd code.
#define _pcps_write_var( _pddev, _cmd, _s )  \
  _pcps_write( (_pddev), (_cmd), &(_s), sizeof( (_s) ) )


// Read data structures which exceed PCPS_FIFO_SIZE bytes.
// This can't be handled in a single read cycle and due to limitations
// of the clock's microprocessor these calls can up to 20 milliseconds.
// Currently these function is only used to read GPS specific data
// from GPS clocks.
#define _pcps_read_gps_var( _pddev, _cmd, _s )  \
  _pcps_read_gps( (_pddev), (_cmd), &(_s), sizeof( (_s) ) )

// The complementary write function for the read function above.
#define _pcps_write_gps_var( _pddev, _cmd, _s )  \
  _pcps_write_gps( (_pddev), (_cmd), &(_s), sizeof( (_s) ) )


// Generate a hardware reset on a device. This macro should be used
// VERY carefully and should be avoided, if possible, since resetting
// a device could lock up the PC.
#define _pcps_force_reset( _pddev ) \
  _pcps_write_byte( (_pddev), PCPS_FORCE_RESET )


// This macro reads a device's status port which includes
// the BUSY flag (::PCPS_ST_BUSY ) and the modulation signal
// of DCF77 and other long wave receivers.
// The macro takes a (PCPS_DDEV *) as argument.
#if _PCPS_USE_MM_IO

#if 1  // TODO

static __mbg_inline
uint8_t _pcps_ddev_read_status_port( const PCPS_DDEV *pddev )
{
  return _pcps_ddev_access_mode_mm( pddev ) ?
    _mbg_mmrd8_offs( pddev, 0, pddev->status_port_offs ) :
    _mbg_inp8( pddev, 0, pddev->status_port );  // TODO Use _mbg_inp8_offs()
}

#else

  #define _pcps_ddev_read_status_port( _d )            \
  ( _pcps_ddev_access_mode_mm( (_d) ) ?                \
    _mbg_mmrd8( &(_d)->mm_asic_addr->status_port.b ) : \
    _mbg_inp8( (_d), 0, (_d)->status_port )            \
  )

#endif

#else

  #define _pcps_ddev_read_status_port( _d ) \
    _mbg_inp8( (_d), 0, (_d)->status_port )

#endif


#define _pcps_ddev_status_busy( _d ) \
  ( _pcps_ddev_read_status_port( pddev ) & PCPS_ST_BUSY )


// This macro checks whether a workaround is required to get/set
// IRIG cfg from a GPS169PCI with IRIG output and early firmware version.
// This is handled in mbgdevio.c for direct access environments, and in
// macioctl.h for kernel device drivers.
#define _pcps_ddev_requires_irig_workaround( _d ) \
  ( ( _pcps_ddev_type_num( _d ) == PCPS_TYPE_GPS169PCI ) && \
    ( _pcps_ddev_fw_rev_num( _d ) < REV_HAS_GPS_DATA_16_GPS169PCI ) )


#if _PCPS_USE_USB

  #if !defined( MBGUSB_TIMEOUT_SEND_MS )
    #define MBGUSB_TIMEOUT_SEND_MS             500   // [ms]
  #endif

  #if !defined( MBGUSB_TIMEOUT_RECEIVE_MS )
    #define MBGUSB_TIMEOUT_RECEIVE_MS          500   // [ms]
  #endif

  #if !defined( MBGUSB_TIMEOUT_RECEIVE_CYCLIC_MS )
    // The USB read function may block until a packet has been received, or a
    // receive timeout has occurred. The cyclic USB read function has an overall
    // timeout of more than 1 second. In order to increase responsiveness we use
    // by default a shorter timeout interval plus some retries, if required.
    //
    // For some target environments it may be preferable to use only one
    // full timeout interval, so this setting can be overridden if required.
    #if !defined( _PCPS_USB_FULL_CYCLIC_INTV )
      #define _PCPS_USB_FULL_CYCLIC_INTV    1
    #endif

    #if _PCPS_USB_FULL_CYCLIC_INTV
      #define MBGUSB_TIMEOUT_RECEIVE_CYCLIC_MS    1200
    #else
      #define MBGUSB_TIMEOUT_RECEIVE_CYCLIC_MS    50
    #endif
  #endif


  #if !defined( _pcps_ms_to_usb_timeout )
    #define _pcps_ms_to_usb_timeout( _ms )     (_ms)
  #endif


  #if !defined( MBGUSB_TIMEOUT_SEND )
    #define MBGUSB_TIMEOUT_SEND            _pcps_ms_to_usb_timeout( MBGUSB_TIMEOUT_SEND_MS )
  #endif

  #if !defined( MBGUSB_TIMEOUT_RECEIVE )
    #define MBGUSB_TIMEOUT_RECEIVE         _pcps_ms_to_usb_timeout( MBGUSB_TIMEOUT_RECEIVE_MS )
  #endif

  #if !defined( MBGUSB_TIMEOUT_RECEIVE_CYCLIC )
    #define MBGUSB_TIMEOUT_RECEIVE_CYCLIC  _pcps_ms_to_usb_timeout( MBGUSB_TIMEOUT_RECEIVE_CYCLIC_MS )
  #endif



#if !defined( MBG_TGT_WIN32 )

static __mbg_inline /*HDR*/
int pcps_direct_usb_transfer( PCPS_DDEV *pddev, void *buffer, int len, int ep_idx,
                              int timeout, const char *fnc_name )
{
  int rc = MBG_ERR_NOT_SUPP_ON_OS;

  #if defined( MBG_TGT_LINUX )

    int usb_rc;  // rc from system USB stack
    unsigned int ep = pddev->ep[ep_idx].addr;
    bool rcv = is_ep_rcv( ep );
    int pipe = rcv ? usb_rcvbulkpipe( pddev->udev, ep )
                   : usb_sndbulkpipe( pddev->udev, ep );
    int actual_len = 0;
    void *p = NULL;

    (void) p;   // Avoid compiler warning "never used"

    #if DEBUG_USB_IO > 1
      _mbgddmsg_fnc_entry();
    #endif

    #if _PCPS_CHK_BUFFER_DMA_CAPABLE
      if ( !_pcps_buffer_is_dma_capable( buffer ) )
      {
        // Buffer is *NOT* DMA capable.
        _mbg_kdd_msg_4( MBG_LOG_WARN, "%s called to %s from " MBG_DEV_NAME_FMT " with non-DMA-capable buffer",
                        fnc_name, rcv ? "rcv" : "snd", _pcps_ddev_type_name( pddev ),
                        _pcps_ddev_sernum( pddev ) );

        p = _pcps_kmalloc( len );

        if ( p == NULL )
          return MBG_ERR_NO_MEM;

        if ( !rcv )
          memcpy( p, buffer, len );

        usb_rc = usb_bulk_msg( pddev->udev, pipe, p, len, &actual_len, timeout );

        if ( rcv && ( usb_rc == 0 ) )
          memcpy( buffer, p, actual_len );

        _pcps_kfree( p, len );
      }
      else
    #endif
        usb_rc = usb_bulk_msg( pddev->udev, pipe, buffer, len, &actual_len, timeout );


    #if DEBUG_USB_IO > 2
    {
      char ptr_str[40];

      if ( p )
        mbg_kdd_snprintf( ptr_str, sizeof( ptr_str ), "%p", p );
      else
        mbg_kdd_snprintf( ptr_str, sizeof( ptr_str ), "NULL" );

      _mbgddmsg_8( DEBUG_USB_IO, MBG_LOG_INFO, "%s: ep %02X, pipe %02X, malloc %s, len %i, actual_len %i, timeout %i, usb_rc: %i",
                   fnc_name, ep, pipe, ptr_str, len, actual_len, timeout, usb_rc );
    }
    #endif

    if ( usb_rc < 0 )
    {
      rc = mbg_posix_errno_to_mbg( -usb_rc, NULL );

      #if DEBUG_USB_IO > 2
        _mbgddmsg_3( DEBUG_USB_IO, MBG_LOG_INFO, "%s: USB rc %i -> %i", fnc_name, usb_rc, rc );
      #endif
    }
    else
      rc = actual_len;

  #else

    #error Needs to be implemented for this target.

  #endif

  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_exit();
  #endif

  return rc;

}  // pcps_direct_usb_transfer

#else

  int pcps_direct_usb_transfer( PCPS_DDEV *pddev, void *buffer, int len, int ep_idx, int timeout, const char *fnc_name );

#endif



static __mbg_inline /*HDR*/
int pcps_direct_usb_write( PCPS_DDEV *pddev, void *buffer, int len )
{
  int rc;

  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_entry();
  #endif

  rc = pcps_direct_usb_transfer( pddev, buffer, len, MBGUSB_EP_IDX_HOST_OUT,
                                 MBGUSB_TIMEOUT_SEND, __func__ );
  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_exit();
  #endif

  return rc;

}  // pcps_direct_usb_write

#define _pcps_direct_usb_write_var( _d, _p ) \
  pcps_direct_usb_write( _d, _p, sizeof( *(_p) ) )



static __mbg_inline /*HDR*/
int pcps_direct_usb_read( PCPS_DDEV *pddev, void *buffer, int len )
{
  int rc;

  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_entry();
  #endif

  rc = pcps_direct_usb_transfer( pddev, buffer, len, MBGUSB_EP_IDX_HOST_IN,
                                 MBGUSB_TIMEOUT_RECEIVE, __func__ );
  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_exit();
  #endif

  return rc;

}  // pcps_direct_usb_read

#define _pcps_direct_usb_read_var( _d, _p ) \
  pcps_direct_usb_read( _d, _p, sizeof( *(_p) ) )



static __mbg_inline /*HDR*/
int pcps_direct_usb_read_cyclic( PCPS_DDEV *pddev, void *buffer, int len )
{
  return pcps_direct_usb_transfer( pddev, buffer, len, MBGUSB_EP_IDX_HOST_IN_CYCLIC,
                                   MBGUSB_TIMEOUT_RECEIVE_CYCLIC, __func__ );

}  // pcps_direct_usb_read_cyclic

#define _pcps_direct_usb_read_var_cyclic( _d, _p ) \
  pcps_direct_usb_read_cyclic( _d, _p, sizeof( *(_p) ) )


#endif



/**
 * @defgroup pcps_io_fncs Low level functions used to access the hardware device
 */


/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 void pcps_dump_data( const void *buffer, size_t count, const char *info ) ;
 /**
 * @brief Write data to a device
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[in]  buffer  A buffer with data to be written according to the cmd code
 * @param[in]  count   The number of bytes to be written according to the cmd code
 *
 * @return ::MBG_SUCCESS on success,
 *         ::MBG_ERR_TIMEOUT if device didn't respond in time,
 *         ::MBG_ERR_NBYTES if the number of parameter bytes did not match
 *                          the number of data bytes expected by the device,
 *         or one of the other @ref MBG_RETURN_CODES
 *
 * @ingroup pcps_io_fncs
 * @see @ref pcps_io_fncs
 */
 int pcps_write( PCPS_DDEV *pddev, uint8_t cmd, const void FAR *buffer, uint16_t count ) ;

 /**
 * @brief Generic I/O function
 *
 * @param[in]  pddev     Pointer to the device structure
 * @param[in]  type      The type of data to be read/written, see @ref PCPS_CMD_CODES
 * @param[in]  in_buff   A buffer with data to be written according to the type code
 * @param[in]  in_cnt    The number of bytes to be written according to the type code
 * @param[out] out_buff  A buffer with data to be read according to the type  code
 * @param[in]  out_cnt   The number of bytes to be read according to the type code
 *
 * @return ::MBG_SUCCESS on success,
 *         ::MBG_ERR_TIMEOUT if device didn't respond in time,
 *         ::MBG_ERR_NBYTES if the number of parameter bytes did not match
 *                          the number of data bytes expected by the device,
 *         or one of the other @ref MBG_RETURN_CODES
 *
 * @ingroup pcps_io_fncs
 * @see @ref pcps_io_fncs
 */
 int pcps_generic_io( PCPS_DDEV *pddev, uint8_t type, const void FAR *in_buff, uint8_t in_cnt, void FAR *out_buff, uint8_t out_cnt ) ;

 /**
 * @brief Read a large data structure from a device
 *
 * Read data structures which exceed ::PCPS_FIFO_SIZE bytes.
 * This can't be handled in a single read cycle, and due to
 * limitations of the device's microprocessor the execution time
 * can be up to 20 milliseconds, depending on the device type.
 * This has been introduced with the first GPS devices but is
 * now in fact also used with non-GPS devices.
 *
 * @param[in]  pddev      Pointer to the device structure
 * @param[in]  data_type  The code assigned to the data type, see @ref PC_GPS_CMD_CODES
 * @param[out] buffer     A buffer with data to be read according to the data_type
 * @param[in]  count      The number of bytes to be read according to the data_type
 *
 * @return ::MBG_SUCCESS on success,
 *         ::MBG_ERR_TIMEOUT if device didn't respond in time,
 *         ::MBG_ERR_INV_TYPE if the device didn_t recognize the data type,
 *         ::MBG_ERR_NBYTES if the number of parameter bytes did not match
 *                          the number of data bytes expected by the device,
 *         or one of the other @ref MBG_RETURN_CODES
 *
 * @ingroup pcps_io_fncs
 * @see @ref pcps_io_fncs
 */
 int pcps_read_gps( PCPS_DDEV *pddev, uint8_t data_type, void FAR *buffer, uint16_t count ) ;

 /**
 * @brief Write a large data structure to a device
 *
 * This has been introduced with the first GPS devices but is
 * now in fact also used with non-GPS devices.
 *
 * @param[in]  pddev      Pointer to the device structure
 * @param[in]  data_type  The code assigned to the data type, see @ref PC_GPS_CMD_CODES
 * @param[in]  buffer     A buffer with data to be written according to the data_type
 * @param[in]  count      The number of bytes to be written according to the data_type
 *
 * @return ::MBG_SUCCESS on success,
 *         ::MBG_ERR_TIMEOUT if device didn't respond in time,
 *         ::MBG_ERR_NBYTES if the number of parameter bytes did not match
 *                          the number of data bytes expected by the device,
 *         or one of the other @ref MBG_RETURN_CODES
 *
 * @ingroup pcps_io_fncs
 * @see @ref pcps_io_fncs
 */
 int pcps_write_gps( PCPS_DDEV *pddev, uint8_t data_type, const void FAR *buffer, uint16_t count ) ;

 /**
 * @brief Read the serial number and receiver info from the device.
 *
 * The retrieved information is stored in the associated fields
 * of the sub-structures of pddev.
 *
 * @param[in,out] pddev  Pointer to a device structure
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
 int setup_sernum_and_receiver_info( PCPS_DDEV *pddev ) ;

 /**
 * @brief Check the receiver info and features.
 *
 * @param[in,out] pddev  Pointer to a device structure
 */
 void check_receiver_info_and_features( PCPS_DDEV *pddev ) ;

 /**
 * @brief Release I/O port and memory resource that have been claimed before
 *
 * @param[in,out] pddev   The device structure
 */
 void pcps_release_rsrcs( PCPS_DDEV *pddev ) ;

 /**
 * @brief Lookup a specific device in the device table
 *
 * The function below takes a bus flag and device ID to search
 * the table of known devices for a device which matches the
 * given criteria.
 *
 * @param[in]  bus_mask  Mask of the bus type to look up, see @ref PCPS_BUS_FLAG_MASKS
 * @param[in]  dev_id    The device ID to lookup, see @ref MEINBERG_PCI_DEVICE_IDS
 *                       or @ref MBG_USB_DEVICE_IDS, depending on the bus_mask
 *
 * @return  A pointer to the device table entry, or NULL if no entry found
 */
 PCPS_DEV_TYPE *pcps_get_dev_type_table_entry( PCPS_BUS_FLAGS bus_mask, PCPS_DEV_ID dev_id ) ;

 /**
 * @brief Allocate and initialize a device info structure
 *
 * On success, the pointer to the device structure is set to
 * the device structure, and the device structure has been initialized.
 *
 * @param[in,out]  ppddev  Address of a pointer to a device structure to be allocated and initialized
 *
 * @return ::MBG_SUCCESS on success, or ::MBG_ERR_NO_MEM if no memory could be allocated
 *
 * @see ::pcps_cleanup_ddev
 */
 int pcps_init_ddev( PCPS_DDEV **ppddev ) ;

 /**
 * @brief Clean up and free a previously initialized device info structure
 *
 * @param[in]  pddev  Address of a device structure to be cleaned up and released
 *
 * @see ::pcps_init_ddev
 */
 void pcps_cleanup_ddev( PCPS_DDEV *pddev ) ;

 /**
 * @brief Add an I/O address range resource to the device structure
 *
 * @param[in,out]  pddev  Pointer to the device structure
 * @param[in]      base   Base address of the I/O address range
 * @param[in]      num    Number of addresses of the I/O address range
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
 int pcps_add_rsrc_io( PCPS_DDEV *pddev, MBG_IOPORT_ADDR_RAW base, ulong num ) ;

 /**
 * @brief Add a memory address range resource to the device structure
 *
 * @param[in,out]  pddev  Pointer to the device structure
 * @param[in]      start  Start address of the memory range
 * @param[in]      len    Size of the memory range
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
 int pcps_add_rsrc_mem( PCPS_DDEV *pddev, MBG_IOMEM_ADDR_RAW start, ulong len ) ;

 /**
 * @brief Add an IRQ number resource to the device structure
 *
 * @param[in,out]  pddev    Pointer to the device structure
 * @param[in]      irq_num  Start address of the memory range
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
 int pcps_add_rsrc_irq( PCPS_DDEV *pddev, int16_t irq_num ) ;

 /**
 * @brief Initialize an allocated device structure for a specific device
 *
 * @param[in,out]  pddev     Pointer to the device structure
 * @param[in]      bus_mask  Mask of the bus type to look up, see @ref PCPS_BUS_FLAG_MASKS
 * @param[in]      dev_id    The device ID to lookup, see @ref MEINBERG_PCI_DEVICE_IDS
 *                           or @ref MBG_USB_DEVICE_IDS, depending on the bus_mask
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
 int pcps_setup_ddev( PCPS_DDEV *pddev, PCPS_BUS_FLAGS bus_mask, PCPS_DEV_ID dev_id ) ;

 /**
 * @brief Check if a specific feature of a specific feature type is supported
 *
 * There are different structures where information can be stored
 * if a specific feature is supported. All information is set up
 * when the ::pcps_probe_device function is called to probe and
 * initialize the device.
 * This generic low-level function can be called by API functions
 * to check if a specific feature is supported.
 *
 * @param[in]  p_ddev     Pointer to the device structure
 * @param[in]  feat_type  See ::DEV_FEAT_TYPES
 * @param[in]  feat_num   Number and range depending on feat_type value
 *
 * @return ::MBG_SUCCESS if the requested feature is supported, ::MBG_ERR_NOT_SUPP_BY_DEV
 *         if not supported, or one of the other @ref MBG_ERROR_CODES
 *
 * @see ::pcps_probe_device
 */
 int pcps_chk_dev_feat( PCPS_DDEV *p_ddev, uint feat_type, uint feat_num ) ;

 /**
 * @brief Probe if a device is supported, and allocate and setup the device structure
 *
 * This function should be called by the probe routine of any
 * target-specific kernel driver.
 * If the device is supported then all specific information including
 * supported features is read from the device and stored in sub-structures
 * of the device structure addressed by pdev.
 *
 * @param[in,out]  pddev        Pointer to the device structure which has been initialized and will be set up
 * @param[in]      bus_num      The bus number if supported (e.g. PCI), else 0
 * @param[in]      dev_fnc_num  The device/function number if supported (e.g. PCI), else 0
 *
 * @return ::MBG_SUCCESS if the requested feature is supported, ::MBG_ERR_NOT_SUPP_BY_DEV
 *         if not supported, or one of the other @ref MBG_ERROR_CODES
 *
 * @see ::pcps_cleanup_device
 * @see ::pcps_chk_dev_feat
 */
 int pcps_probe_device( PCPS_DDEV *pddev, PCPS_BUS_NUM bus_num, PCPS_SLOT_NUM dev_fnc_num ) ;

 /**
 * @brief Clean up function called by ::pcps_probe_device on error
 *
 * @param[in,out]  pddev  Pointer to the device structure
 *
 * @see ::pcps_probe_device
 */
 void pcps_cleanup_device( PCPS_DDEV *pddev ) ;

 /**
 * @brief Setup and start a PCI device in a non-PnP system
 *
 * @param[in,out]  pddev        Pointer to the device structure to be set up
 * @param[in]      bus_num      The PCI bus number returned by the PCI BIOS
 * @param[in]      dev_fnc_num  The PCI device/function number returned by the PCI BIOS
 */
 int pcps_setup_and_start_pci_dev( PCPS_DDEV *pddev, PCPS_BUS_NUM bus_num, PCPS_SLOT_NUM dev_fnc_num ) ;

 /**
 * @brief Detect and initialize PCI devices in a non-PnP system
 *
 * @param[in]  ddev_init_fnc     Pointer to function called to allocate a device structure for each detected device.
 * @param[in]  ddev_cleanup_fnc   Pointer to function called if the device structure needs to be de-allocated in case of error.
 * @param[in]  vendor_id     The PCI vendor ID code.
 * @param[in]  dev_type      An array with known PCI devices for the specified vendor ID
 * @param[in]  n_dev_types   The number of entries in the PCI device table
 */
 void pcps_detect_pci_devices( PCPS_DDEV_INIT_FNC *ddev_init_fnc, PCPS_DDEV_CLEANUP_FNC *ddev_cleanup_fnc, ushort vendor_id, PCPS_DEV_TYPE dev_type[], int n_dev_types ) ;

 /**
 * @brief Detect and initialize ISA devices in a non-PnP system
 *
 * @param[in]  ddev_init_fnc     Pointer to function called to allocate a device structure for each detected device.
 * @param[in]  ddev_cleanup_fnc   Pointer to function called if the device structure needs to be de-allocated in case of error.
 * @param[in]  ddev_register_fnc  Pointer to function called to register a detected device.
 * @param[in]  isa_ports     An array with potential I/O base addresses for ISA devices.
 * @param[in]  isa_irqs      An array with potential IRQ numbers assigned to ISA devices.
 */
 void pcps_detect_isa_devices( PCPS_DDEV_INIT_FNC *ddev_init_fnc, PCPS_DDEV_CLEANUP_FNC *ddev_cleanup_fnc, PCPS_DDEV_REGISTER_FNC *ddev_register_fnc, int isa_ports[PCPS_MAX_ISA_CARDS], int isa_irqs[PCPS_MAX_ISA_CARDS] ) ;

 /**
 * @brief Detect all bus-level devices in a non-PnP system
 *
 * @note If a DOS TSR is installed, be sure it is disabled (BUSY flag set)
 * when this function is called.
 *
 * @param[in]  isa_ports     An array with potential I/O base addresses for ISA devices.
 * @param[in]  isa_irqs      An array with potential IRQ numbers assigned to ISA devices.
 */
 void _MBG_INIT_CODE_ATTR pcps_detect_devices( int isa_ports[PCPS_MAX_ISA_CARDS], int isa_irqs[PCPS_MAX_ISA_CARDS] ) ;


/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif

// We have used native alignment here, so no need to undo alignment at this place.

/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _PCPSDRVR_H */
