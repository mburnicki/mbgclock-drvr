
/**************************************************************************
 *
 *  $Id: pcpsdrvr.c 1.63 2020/06/30 15:59:29 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Driver functions that detect Meinberg PC plug-in devices and set up
 *    the software environment (port base address, clock features, etc.).
 *
 *    These functions should be used with programs which have direct
 *    access to the hardware (e.g. device drivers).
 *
 *    Programs which access the devices via device drivers should
 *    use the functions provided by the mbgdevio module.
 *
 *    There are several preprocessor symbols defined at the top of
 *    pcpsdrvr.h which control the default support of some features
 *    under the different operating systems. If required, each of
 *    those symbols can be overridden by compiler arguments.
 *
 *    Basically the following devices are supported:
 *      USB v2:          DCF600USB, TCR600USB, MSF600USB, WVB600USB,
 *                       TCR180USB.
 *      USB v1:          USB5131, TCR51USB, MSF51USB, WWVB51USB
 *      PCI express:     PEX511, TCR511PEX, GPS170PEX, PTP270PEX,
 *                       FRC511PEX, TCR170PEX, GPS180PEX, TCR180PEX
 *                       PZF180PEX, GLN180PEX, GPS180AMC, GNS181PEX
 *      PCI bus 5V/3.3V: PCI510, PCI511, GPS169PCI, GPS170PCI,
 *                       TCR510PCI, TCR167PCI, TCR511PCI
 *      PCI bus 5V:      PCI32, GPS167PCI, PCI509, GPS168PCI
 *      MCA bus:         PS31
 *      ISA bus:         PC31, PC32, GPS167PC
 *
 *    USB is not supported for all target environments, eventually
 *    because an operating systems doesn't provide full USB support,
 *    or USB support hasn't yet been implemented.
 *
 *    PCI support is possible in two different ways. The preferred
 *    functions are compiled in if one of the symbols _PCPS_USE_PCI_PNP
 *    or _PCPS_USE_PCI_BIOS is defined != 0.
 *
 *    If _PCPS_USE_PCI_PNP is != 0 it is assumed that the operating
 *    system's PCI layer detects a new PCI device and calls a driver's
 *    add_device()/start_device() function to initialize the device.
 *    This technique is supported with PNP operating systems
 *    (e.g. Windows versions after NT, Linux, *BSD).
 *
 *    If _PCPS_USE_PCI_BIOS is != 0 the program scans the PCI bus
 *    during startup to detect and initialize supported PCI devices.
 *    This techique is used with old non-PNP operating systems.
 *
 *    The symbol _PCPS_USE_RSRCMGR must be defined != 0 to include
 *    support of resource managers, if necessary.
 *
 *    If the symbol _PCPS_USE_MCA is defined != 0 then Micro Channel
 *    detection (and therefore auto-detection of a MCA clock) is
 *    supported.
 *
 *    MCA devices are accessed using the same low level functions as
 *    ISA devices, so if autodetection of MCA clocks is not supported
 *    then a MCA device's known port number can be passed to
 *    pcps_detect_devices() to let it be treated like an ISA device.
 *
 * -----------------------------------------------------------------------
 *  $Log: pcpsdrvr.c $
 *  Revision 1.63  2020/06/30 15:59:29  martin
 *  Fixed a bug in a debug message on Windows.
 *  Revision 1.62  2020/06/19 13:24:01Z  martin
 *  Conditionally report low level I/O errors even in release build.
 *  Revision 1.61  2020/02/27 13:43:58  martin
 *  Changed a debug message control to USB I/O.
 *  Added some conditional USB debug messages.
 *  Revision 1.60  2019/12/20 12:32:17  martin
 *  Distinguish between default and real builtin features.
 *  Adjust real builtin features when probing the device.
 *  Revision 1.59  2019/05/08 11:27:35  martin
 *  Use new symbol PCPS_IRQ_NUM_UNDEFINED.
 *  Revision 1.58  2019/03/18 16:13:38  martin
 *  Fixed a bug in the probe routine that had made it into
 *  the previous file version.
 *  Revision 1.57  2019/03/11 16:08:27  martin
 *  Conditionally use local I/O buffers. This may avoid a potential
 *  data corruption problem, but won't work with USB devices on
 *  Linux kernels 4.9 and later.
 *  Revision 1.56  2019/01/15 16:59:59  martin
 *  Use generic format specifiers for a debug message.
 *  Revision 1.55  2018/11/23 08:05:46Z  martin
 *  Improved resource handling.
 *  Refactored debug logging stuff.
 *  Return MBG_ERR_TIMEOUT in pcps_read_asic_mm() on SPARC for now.
 *  Variables force_io_access and force_mm16_access are only
 *  supported on targets where _PCPS_USE_MM_IO is not 0.
 *  New preprocessor symbol PCPS_LOG_STD_MSGS to suppress some
 *  messages on direct access targets in non-DEBUG mode.
 *  Revision 1.54  2018/09/21 15:13:41  martin
 *  Added definitions for TCR180USB.
 *  Use only DMA capable variables for device I/O.
 *  Also read ASIC features via MM I/O, if supported.
 *  Fixed do_div() in 32 bit Linux kernel build.
 *  Changed MAX_BOOT_TIME_PTP270PEX from 40 back to 27 s.
 *  Modified wait for PTP270PEX in case uptime is not supported.
 *  Set force_io_access if associated preprocessor macro is defined.
 *  Introduced access mode stuff.
 *  Unified kernel driver messages.
 *  New functions pcps_read_usb_generic() and pcps_write_usb_generic().
 *  Updated function prototypes.
 *  Let the probe routine print a success message.
 *  Cleaned up messages.
 *  Use more appropriate I/O macros.
 *  More unified resource handling.
 *  Renamed a number of symbols.
 *  Use _MBG_IOMEM.
 *  Refactored reading sernum.
 *  Support REPORT_CFG, even without DEBUG.
 *  Unified debugging in low level functions.
 *  Also log USB endpoint attributes in DEBUG.
 *  Mutexes and spinlocks with individual names.
 *  Revision 1.53  2017/08/10 14:03:03  martin
 *  String 'pcps_driver_name' is now defined in pcpsdrvr.h.
 *  Unified extended resource properties handling.
 *  Cleaned up target some dependent code.
 *  Revision 1.52  2017/07/04 16:45:36  martin
 *  Support GPS180AMC and GNS181PEX.
 *  Renamed some functions: Use _device instead of _clock,
 *  pcps_start_device() is now called pcps_probe_device(), etc.
 *  Runtime support for forcing I/O rather than MM access.
 *  Increase MAX_BOOT_TIME_PTP270PEX from 27 to 40 seconds
 *  to be safe in case a firmware update is applied at startup.
 *  Fixed type of a register address.
 *  Cleaned up I/O port usage.
 *  Older defines N_SUPP_DEV, PCPS_MAX_DDEVS, and MBG_MAX_DEVICES
 *  have been obsoleted by new defines N_SUPP_DEV_BUS, N_SUPP_DEV_EXT,
 *  and N_SUPP_DEV_TOTAL.
 *  Fixed DEBUG build under *BSD.
 *  Added DEBUG code dumping RECEIVER_INFO.
 *  Moved mbg_delta_sys_time_ms() to new module mbgsystm.c.
 *  Check for MBG_TGT_POSIX instead of MBG_TGT_UNIX.
 *  Provided a driver name string for debug build on direct-access targets.
 *  Avoid 'redundant redeclaration' warning under FreeBSD 8.2.
 *  Fixed macro definition syntax to avoid clang compiler warnings.
 *  Fixed some other warnings from clang.
 *  Attribute always_inline is now in __mbg_inline.
 *  Conditional USB debug code.
 *  Account for renamed symbols.
 *  Fixed typos, wording, and doxygen comments.
 *  Removed trailing white space.
 *  Revision 1.51  2013/10/01 14:19:03  martin
 *  Support GLN180PEX.
 *  Revision 1.50  2013/03/15 10:01:58  martin
 *  Modified/added some debug messages.
 *  Revision 1.49  2013/03/15 08:35:08  martin
 *  Account for PTP270PEX HW v2 cards which can indicate
 *  when they are ready to be accessed.
 *  Account for unified, renamed PLX symbols.
 *  Revision 1.48  2012/11/05 16:32:02Z  martin
 *  Fixed and enhanced some timing DEBUG code.
 *  Rewrote report_uptime().
 *  Revision 1.47  2012/10/15 14:12:08  martin
 *  Support GPS180PEX, TCR180PEX, and PZF180PEX.
 *  Support DCF600USB, TCR600USB, MSF600USB, and WVB600USB.
 *  Support FreeBSD and NetBSD.
 *  Support on-board event log.
 *  If required, wait until PTP270PEX has finished booting.
 *  Use USB micro frame timing conditionally only, yet disabled by default.
 *  Conditional debug code to test PCI access time and/or execution time
 *  of the low level functions.
 *  Always read the serial number directly from the device.
 *  Unified low level functions and use 16 bit types for buffer sizes.
 *  Added warnings in case a device is not handled by chip setup
 *  or device feature check.
 *  Modified low level AMCC read functions for SPARC to fix unaligned access.
 *  Added debug messages for generic I/O.
 *  Initialize device semaphores only early at device initializition
 *  if required, otherwise later, since early initialization can lead
 *  to a trap e.g. under Windows.
 *  Conditional code to test MM I/O for new PCI cards.
 *  Use common mutex primitives from mbgmutex.h.
 *  Optionally support timespec for sys time (USE_TIMESPEC).
 *  Set up basic default receiver info for devices which don't
 *  support this structure.
 *  Moved MBG_TGT_SUPP_MEM_ACC definition to pcpsdev.h.
 *  Introduced and use new function check_ri_feature().
 *  Also detect support for raw IRIG data from RECEIVER_INFO.
 *  Revision 1.46  2009/12/15 14:45:33  daniel
 *  Account for feature to read the raw IRIG bits.
 *  Revision 1.45  2009/09/29 07:24:50Z  martin
 *  Use standard feature flag to check if fast HR time is supported.
 *  Revision 1.44  2009/06/23 07:10:47  martin
 *  Fixed/modified some debug messages.
 *  Revision 1.43  2009/06/19 12:13:59  martin
 *  Check if TCR cards support raw IRIG time.
 *  Revision 1.42  2009/06/09 10:15:33  daniel
 *  Check if card has LAN interface and supports PTP.
 *  Revision 1.41  2009/04/08 08:33:20  daniel
 *  Check whether the TCR511PCI or devices with
 *  RECEIVER_INFO support IRIG control function bits.
 *  Revision 1.40  2009/03/27 09:55:13Z  martin
 *  Added some debug messages.
 *  Account for renamed library symbols.
 *  Revision 1.39  2009/03/19 12:04:31Z  martin
 *  Adjust endianess of ASIC version and ASIC features after having read.
 *  Revision 1.38  2009/03/17 15:33:53  martin
 *  Support reading IRIG control function bits.
 *  Revision 1.37  2009/03/13 09:17:00Z  martin
 *  Bug fix: Hadn't checked whether TCR170PEX card provides the
 *  programmable synthesizer.
 *  As a fix moved the code from the body of check_opt_features()
 *  into pcps_start_device() so that the check is done for every
 *  type of card.
 *  Swap receiver_info to make this work on non-x86 architectures.
 *  Support configurable time scales, and reading/writing GPS UTC
 *  parameters via the PC bus.
 *  Use mbg_get_pc_cycles() instead of _pcps_get_cycles().
 *  Revision 1.36  2009/01/13 12:03:57Z  martin
 *  Generate a separate warning message if the firmware could not
 *  be read from an ISA card.
 *  Care about "long long" in debug msg.
 *  Revision 1.35  2008/12/16 14:38:49Z  martin
 *  Account for new devices PTP270PEX, FRC270PEX, TCR170PEX, and WWVB51USB.
 *  Check the firmware / ASIC version of PEX cards and flag the device
 *  unsafe for IRQs if the versions are older than required.
 *  Check whether PEX511 and PCI511 support HR time.
 *  Moved initialization of common spinlocks and mutexes to pcps_start_device().
 *  Take access cycles count in the low level routines, with interrupts disabled.
 *  Cleanup for pcps_read_usb() which is now possible since access cycles count
 *  is now taken inside the low evel routines.
 *  Support mapped I/O resources, unaligned access and endianess conversion.
 *  Account for ASIC_FEATURES being coded as flags, and account for
 *  new symbol PCI_ASIC_HAS_MM_IO.
 *  Account for new MBG_PC_CYCLES type.
 *  Account for signed irq_num.
 *  Renamed MBG_VIRT_ADDR to MBG_MEM_ADDR.
 *  Use MBG_MEM_ADDR type for memory rather than split high/low types.
 *  Distinguish device port variables for IRQ handling.
 *  Preliminarily support USB latency compensation under Win32 PNP targets
 *  and account for USB EHCI microframe timing which requires a different
 *  latency compensation approach. This is useful if a USB 2.0 hub is connected
 *  between device and host.
 *  Also read the ASIC version at device initialization.
 *  pcps_alloc_ddev() does not take a parameter anymore.
 *  Cleaned up comments.
 *  Revision 1.34  2008/02/27 10:03:02  martin
 *  Support TCR51USB and MSF51USB.
 *  Preliminary support for mapped memory access under Windows and Linux.
 *  Enabled PCPS_IRQ_1_SEC for USB within WIN32 targets
 *  in pcps_start_device().
 *  Fixed a bug in pcps_write() where the error code
 *  that was returned from a USB device was misinterpreted
 *  due to a signed/unsigned mismatch (added typecast).
 *  Removed obsolete function pcps_cleanup_all_devices().
 *  Code cleanup.
 *  Revision 1.33  2008/01/31 08:51:30Z  martin
 *  Picked up changes from 1.31.2.1:
 *  Changed default definition of PCI_DWORD to uint32_t.
 *  Removed erraneous brace from debug code.
 *  Revision 1.32  2007/09/26 11:05:57Z  martin
 *  Added support for USB in general and new USB device USB5131.
 *  Renamed ..._USE_PCIMGR symbols to ..._USE_PCI_PNP.
 *  Renamed ..._USE_PCIBIOS symbols to ..._USE_PCI_BIOS.
 *  Added new symbol _USE_ISA_PNP to exclude non-PNP stuff.
 *  from build if ISA devices are also handled by the PNP manager.
 *  Use new MBG_... codes defined in mbgerror.h.
 *  Unified timeout handling in low level functions by using an inline function.
 *  Renamed pcps_pnp_start_device() to pcps_start_device().
 *  Renamed pcps_setup_pci_dev() to pcps_setup_and_startpci_dev().
 *  Merged code from init_ddev_cfg() and finish_ddev_cfg() into pcps_start_device().
 *  Improved and unified handling of ISA devices.
 *  Removed calling register_pnp_devices() from pcps_detect_clocks(),
 *  this is now called directly.
 *  Added missing IRIG support to pcps_rsrc_register_device().
 *  Revision 1.31  2007/07/17 08:22:47Z  martin
 *  Added support for TCR511PEX and GPS170PEX.
 *  Revision 1.30  2007/07/16 12:56:01Z  martin
 *  Added support for PEX511.
 *  Rewrote common resource handling code in order to simplify
 *  OS specific code.
 *  Revision 1.29  2007/03/02 09:40:33Z  martin
 *  Use generic port I/O macros.
 *  Pass PCPS_DDEV structure to the low level read functions.
 *  Use new _pcps_..._timeout_clk() macros.
 *  Added init code qualifier.
 *  Preliminary support for *BSD.
 *  Preliminary support for USB.
 *  Revision 1.28  2006/07/11 10:24:20  martin
 *  Use _fmemcpy() in pcps_generic_io() to support environments which
 *  require far data pointers.
 *  Revision 1.27  2006/07/07 09:41:15  martin
 *  Renamed pci_..() function calls to _mbg_pci_..() calls which are defined according to the
 *  OS requirements, in order to avoid naming conflicts.
 *  Revision 1.26  2006/06/19 15:28:52  martin
 *  Added support for TCR511PCI.
 *  Modified parameters required to detect ISA cards.
 *  The array of port addresses does no more require a 0 address
 *  as last value.
 *  Revision 1.25  2006/03/10 11:01:27  martin
 *  Added support for PCI511.
 *  Revision 1.24  2005/11/03 15:50:45Z  martin
 *  Added support for GPS170PCI.
 *  Revision 1.23  2005/09/16 08:21:08Z  martin
 *  Also flag PCI cards which have base_addr set to 0 as uninitialized.
 *  Revision 1.22  2005/06/02 10:32:07Z  martin
 *  Changed more types to C99 fixed size types.
 *  New function pcps_generic_io().
 *  Revision 1.21  2004/12/13 14:19:38Z  martin
 *  Support configuration of on-board frequency synthesizer.
 *  Revision 1.20  2004/11/09 13:02:48Z  martin
 *  Redefined fixed width data types using standard C99 types.
 *  Fixed warnings about lvalue casts.
 *  Revision 1.19  2004/10/14 15:01:24  martin
 *  Added support for TCR167PCI.
 *  Revision 1.18  2004/09/06 15:16:57Z  martin
 *  Support a GPS_DATA interface where sizes are specified
 *  by 16 instead of the original 8 bit quantities, thus allowing
 *  to transfer data blocks which exceed 255 bytes.
 *  Conditionally skip assertions under Linux.
 *  Revision 1.17  2004/04/22 14:47:54  martin
 *  Fixed conversion of firmware rev. number.
 *  Revision 1.16  2004/04/07 09:45:04Z  martin
 *  Support new feature PCPS_HAS_IRIG_TX for GPS169PCI.
 *  Revision 1.15  2003/12/22 16:15:21Z  martin
 *  Support PCPS_HR_TIME for TCR510PCI.
 *  Revision 1.14  2003/07/30 07:28:23Z  martin
 *  Moved prototype for register_pci_devices() outside to top of file.
 *  Revision 1.13  2003/07/08 15:11:55  martin
 *  Support PCI PNP interface under Linux.
 *  New function pcps_rsrc_release().
 *  Made some functions public.
 *  Renamed some public functions to start with pcps_...
 *  Revision 1.12  2003/06/19 10:08:43  MARTIN
 *  Renamed some functions to follow common naming conventions.
 *  Made a function's parameter pointer const.
 *  Changes due to renamed symbols in pcpsdev.h.
 *  Check devices for _pcps_has_ucap() support.
 *  Revision 1.11  2003/05/16 09:28:06  MARTIN
 *  Moved inclusion of some headers to pcpsdrvr.h.
 *  Revision 1.10  2003/04/09 16:35:57  martin
 *  Supports PCI510, GPS169PCI, and TCR510PCI,
 *  and new PCI_ASIC used by those devices.
 *  Revision 1.9  2003/03/20 11:42:37  martin
 *  Fixed syntax for QNX.
 *  Revision 1.8  2002/08/09 08:25:50  MARTIN
 *  Support feature PCPS_CAN_CLR_CAP_BUFF.
 *  Fixed a bug resulting in an unterminated string
 *  if SERNUM was being read.
 *  Revision 1.7  2002/02/26 09:31:57  MARTIN
 *  New function pcps_read_sernum().
 *  Revision 1.6  2002/02/19 09:46:26  MARTIN
 *  Use new header mbg_tgt.h to check the target environment.
 *  Removed function pcps_sn_str_from_ident(), use new
 *  function mbg_gps_ident_decode() from identdec.c now.
 *  If a PCI clock's interface is not properly configured don't
 *  enable the device and set the read function to the new
 *  dummy function pcps_read_null() to prevent driver from
 *  accessing random ports.
 *  Revision 1.5  2002/02/01 12:06:12  MARTIN
 *  Added support for GPS168PCI.
 *  Removed obsolete code.
 *  Revision 1.4  2001/11/30 09:52:48  martin
 *  Added support for event_time which, however, requires
 *  a custom GPS firmware.
 *  Revision 1.3  2001/09/18 06:59:18  MARTIN
 *  Account for new preprocessor symbols in the header file.
 *  Added some type casts to avoid compiler warnings under Win32.
 *  Added some debug messages to clock detection functions.
 *  Revision 1.2  2001/03/16 14:45:33  MARTIN
 *  New functions and definitions to support PNP drivers.
 *  Revision 1.1  2001/03/01 16:26:41  MARTIN
 *  Initial revision for the new library.
 *
 **************************************************************************/

#define _PCPSDRVR
  #include <pcpsdrvr.h>
#undef _PCPSDRVR

#include <identdec.h>
#include <mbgddmsg.h>
#include <plxdefs.h>
#include <pci_asic.h>
#include <amccdefs.h>
#include <pcidefs.h>

#if defined( MBG_TGT_WIN32_PNP )
  #include <usbdrv.h>
  #include <pcpsdefs.h>
  #include <ntddk.h>
  #include <stdio.h>
#elif defined( MBG_TGT_WIN32 )
  #include <pcps_ioc.h>
  #include <stdio.h>
#elif defined( MBG_TGT_DOS )
  #include <mbgplx.h>
#endif

#if defined( MBG_TGT_DOS )
  #include <assert.h>
#endif

#if defined( MBG_TGT_FREEBSD )
  #include <sys/rman.h>
#endif

#if _PCPS_USE_MCA
  #include <mca.h>
#endif

#if _PCPS_USE_PCI
  #include <pci.h>
#endif

#if _PCPS_USE_USB
  #define MBGUSB_MIN_ENDPOINTS_REQUIRED  3
#endif


#define FNC_ID_MARKER "----- "


#if !defined( REPORT_CFG )
  #if defined( DEBUG )
    #define REPORT_CFG  10
  #else
    #define REPORT_CFG  0
  #endif
#endif

#if !defined( REPORT_CFG_DETAILS )
  #if ( REPORT_CFG > 1 )
    #define REPORT_CFG_DETAILS  1
  #else
    #define REPORT_CFG_DETAILS  0
  #endif
#endif

#define REPORT_CFG_LOG_LVL  MBG_LOG_INFO

#if !defined( TEST_CFG_DETAILS )
  #define TEST_CFG_DETAILS  0
#endif

#if !defined( USE_CMD_PTR )
  #define USE_CMD_PTR  1
#endif

#if !defined( DEBUG_ACCESS_TIMING )
  // Test how much cycles it takes to read/write a register on the board
  #if ( defined( DEBUG ) && ( DEBUG >= 10 ) && !defined( MBG_TGT_DOS) )
    #define DEBUG_ACCESS_TIMING  0
  #else
    #define DEBUG_ACCESS_TIMING  0
  #endif
#endif

#if !defined( DEBUG_IO_TIMING )
  // test how much cycles it takes for a low level function to execute
  #if ( defined( DEBUG ) && ( DEBUG >= 10 ) && !defined( MBG_TGT_DOS) )
    #define DEBUG_IO_TIMING  0  // TODO
  #else
    #define DEBUG_IO_TIMING  0
  #endif
#endif

#if !defined( DEBUG_IO )
  // debug which bytes are written to or read from the board
  #if ( defined( DEBUG ) && !defined( MBG_TGT_DOS ) )
    #define DEBUG_IO  1
  #else
    #define DEBUG_IO  0
  #endif
#endif

#if !defined( REPORT_IO_ERRORS )
  // report why a device access has failed
  #define REPORT_IO_ERRORS  DEBUG_IO
#endif

#if !defined( DEBUG_PORTS )
  #if defined( DEBUG )
    #define DEBUG_PORTS  1
  #else
    #define DEBUG_PORTS  0
  #endif
#endif

#if !defined( DEBUG_SERNUM )
  #if defined( DEBUG )
    #define DEBUG_SERNUM  1
  #else
    #define DEBUG_SERNUM  0
  #endif
#endif

#if !defined( USE_USB_MICRO_FRAMES )
  #define USE_USB_MICRO_FRAMES  0
#endif


#if DEBUG_ACCESS_TIMING > 1
  #define TEST_PORT_ACCESS  1
#else
  #define TEST_PORT_ACCESS  0
#endif


#if REPORT_CFG || defined( DEBUG )
  #define MUST_REPORT_PROBE_DEVICE_DETAILS  1
#else
  #define MUST_REPORT_PROBE_DEVICE_DETAILS  0
#endif

#if REPORT_CFG || DEBUG_RSRC || DEBUG_IO
  #define MUST_REPORT_ACCESS_MODE  1
#else
  #define MUST_REPORT_ACCESS_MODE  0
#endif


#if defined( MBG_TGT_KERNEL ) || DEBUG_DEV_INIT
  #define PCPS_LOG_STD_MSGS  1
#else
  #define PCPS_LOG_STD_MSGS  0
#endif



// The do_div() macro or inline function has to be used
// for 64 bit divisions in Linux kernel mode, so we use a
// generic macro _do_div() which uses the Linux function
// if required, and just does a simple division otherwise.
#if defined ( MBG_TGT_LINUX ) && defined( MBG_TGT_KERNEL )
  #define _do_div( _x, _y )   do_div( _x, _y )
#else
  #define _do_div( _x, _y )   do { _x /= _y; } while ( 0 )
#endif


/**
 * @brief Max. time required for PTP270PEX v1 card to be ready after power-up.
 *
 * The PTP270PEX cards have an on-board Linux system which must have
 * finished booting before the card can be accessed via the PCI bus.
 * If the card is accessed earlier then the PCI bus and thus the computer
 * may hang.
 *
 * There are 2 different hardware versions of the PTP270PEX card.
 * PTP270PEX v2 hardware and firmware can flag via a bit in the PCI
 * configuration space when the card is ready, so the driver can just
 * wait until the card has flagged that it's ready.
 *
 * However, the PTP270PEX v1 hardware doesn't support this, so the driver
 * checks the system uptime and waits until it is greater or equal the
 * maximum time required for a PTP270PEX v1 card to finish booting.
 *
 * The nominal time is 27 s, but if a firmware update has been submitted
 * then the update is applied first, which takes another few seconds.
 *
 * @see ::wait_ptp270pex_ready
 * @see ::report_uptime
 */
#define MAX_BOOT_TIME_PTP270PEX    27  // [s]


#if defined( MBG_TGT_BSD )
  // Avoid compiler warnings "redundant redeclaration of ..."
  // e.g. under FreeBSD 8.2 with gcc 4.2.1.
  #define AVOID_REDUNDANT_REDECLARATION  1
#endif



// In some environments special far functions are neither
// required nor supported, so redefine calls to those functions
// to appropriate standard function calls.
#if !defined( MBG_TGT_DOS ) && !defined( MBG_TGT_OS2 )
  #define _fmemcpy( _d, _s, _n )      memcpy( _d, _s, _n )
  #define _fstrlen( _s )              strlen( _s )
  #define _fstrncmp( _s1, _s2, _n )   strncmp( (_s1), (_s2), (_n) )
#endif

#if defined( MBG_TGT_OS2 )
  #define _fstrncmp( _s1, _s2, _n )   _fmemcmp( (_s1), (_s2), (_n) )

  // Watcom C Compiler options for the OS/2 device driver result in
  // warnings if automatic stack addresses are passed to functions.
  #define static_wc static
  #define FMT_03X "%X"
  #define FMT_08X "%X"
#else
  #define static_wc
  #define FMT_03X "%03X"
  #define FMT_08X "%08lX"
#endif

#if defined( MBG_TGT_LINUX )
  typedef unsigned int PCI_DWORD;
#else
  typedef uint32_t PCI_DWORD;
#endif


#if defined( MBG_TGT_LINUX )

  #define _pcps_irq_flags \
    unsigned long irq_flags;

  #define _pcps_disb_local_irq_save() \
    local_irq_save( irq_flags )

  #define _pcps_local_irq_restore() \
    local_irq_restore( irq_flags )

#elif defined( MBG_TGT_WIN32 )

  #define _pcps_irq_flags \
    KIRQL old_irq_lvl;

  #define _pcps_disb_local_irq_save() \
     KeRaiseIrql( HIGH_LEVEL, &old_irq_lvl )

  #define _pcps_local_irq_restore() \
     KeLowerIrql( old_irq_lvl )

#else

  // Nothing to define here.

#endif

#if !defined( _pcps_irq_flags ) && \
    !defined( _pcps_disb_local_irq_save ) && \
    !defined( _pcps_local_irq_restore)
  #define _pcps_irq_flags
  #define _pcps_disb_local_irq_save();
  #define _pcps_local_irq_restore();
#endif


#if defined( MBG_TGT_LINUX ) && defined( time_after )
  #define _pcps_time_after( _curr, _tmo ) \
          time_after( (unsigned long) _curr, (unsigned long) _tmo )
#else
  #define _pcps_time_after( _curr, _tmo )   ( _curr >= _tmo )
#endif


#if defined( MBG_TGT_DOS ) || \
    defined( MBG_TGT_QNX )
  #define MBG_TGT_HAS_UPTIME    0
#elif defined( MBG_TGT_FREEBSD )
  // Under FreeBSD (at least 8.2) the kernel calls to read uptime always
  // return 1 when this driver is loaded automatically, so the system
  // locks up if we wait util uptime has reached a certain value.
  #define MBG_TGT_HAS_UPTIME    0
#else
  #define MBG_TGT_HAS_UPTIME    1
#endif


static const char str_empty[] = "";
static const char str_unkn_braced[] = "(unknown)";

#if REPORT_CFG || REPORT_CFG_DETAILS
  static const char drvr_str_not_spc[] = "not ";
#endif

#if DEBUG_IO
  static const char str_gps_spc[] = "GPS ";
#endif



/**
 * @brief Clock frequency of the PC's cycles counter, in [Hz]
 *
 * An attempt to determine this value is made at the beginning
 * of the ::pcps_probe_device routine.
 * If the cycles frequency value is 0 then the target system
 * doesn't supported this, or the value couldn't be determined.
 * In this case it's not possible to convert a number of cycles
 * to an associatred time interval.
 */
static MBG_PC_CYCLES_FREQUENCY pc_cycles_frequency;



/**
 * @brief A table used to map ::RECEIVER_INFO::features to ::PCPS_DEV_CFG::features
 *
 * The enumerated ::GPS_FEATURE_BITS can be used as index
 * to this table, and the table entry contains a combination
 * of associated @ref PCPS_FEATURE_MASKS, if there are any.
 *
 * @note Devices which support a configurable time scale do also
 * support reading/writing the GPS %UTC parameters via the PC bus.
 * This is not explicitly coded in the ::RECEIVER_INFO::features
 * since the the ::RECEIVER_INFO structure can also be read via
 * the serial port, and reading/writing the GPS %UTC parameters
 * via the serial port is supported by all GPS devices anyway.
 *
 * @note Devices which support reading ::MBG_RAW_IRIG_DATA via the PC bus
 * interface also support reading ::PCPS_IRIG_TIME. However, there is
 * no associated flag in ::RECEIVER_INFO::features since this call
 * is not supported via the serial interface. Thus we use the
 * ::GPS_HAS_RAW_IRIG_DATA flag to check both features.
 *
 * @see ::GPS_FEATURE_BITS
 * @see @ref PCPS_FEATURE_MASKS
 */
static uint32_t ri_feat_tbl[N_GPS_FEATURE] =
{
  0,                                            ///< see ::GPS_FEAT_PPS
  0,                                            ///< see ::GPS_FEAT_PPM
  PCPS_HAS_SYNTH,                               ///< see ::GPS_FEAT_SYNTH
  0,                                            ///< see ::GPS_FEAT_DCFMARKS
  PCPS_HAS_IRIG_TX,                             ///< see ::GPS_FEAT_IRIG_TX
  0,                                            ///< see ::GPS_FEAT_IRIG_RX
  PCPS_HAS_LAN_INTF,                            ///< see ::GPS_FEAT_LAN_IP4
  0,                                            ///< see ::GPS_FEAT_MULTI_REF

  0,                                            ///< see ::GPS_FEAT_RCV_TIMEOUT
  0,                                            ///< see ::GPS_FEAT_IGNORE_LOCK
  0,                                            ///< see ::GPS_FEAT_5_MHZ
  0,                                            ///< see ::GPS_FEAT_XMULTI_REF
  0,                                            ///< see ::GPS_FEAT_OPT_SETTINGS
  PCPS_HAS_TIME_SCALE | PCPS_HAS_UTC_PARM,      ///< see ::GPS_FEAT_TIME_SCALE
  PCPS_HAS_IRIG_CTRL_BITS,                      ///< see ::GPS_FEAT_IRIG_CTRL_BITS
  PCPS_HAS_PTP,                                 ///< see ::GPS_FEAT_PTP

  0,                                            ///< see ::GPS_FEAT_NAV_ENGINE_SETTINGS
  PCPS_HAS_IRIG_TIME | PCPS_HAS_RAW_IRIG_DATA,  ///< see ::GPS_FEAT_RAW_IRIG_DATA
  0,                                            ///< see ::GPS_FEAT_RAW_IRIG_TIME
  0,                                            ///< see ::GPS_FEAT_PTP_UNICAST
  0,                                            ///< see ::GPS_FEAT_GPIO
  0,                                            ///< see ::GPS_FEAT_XMRS_MULT_INSTC
  0,                                            ///< see ::GPS_FEAT_10MHZ_DISBD
  PCPS_HAS_EVT_LOG,                             ///< see ::GPS_FEAT_EVT_LOG

  0,                                            ///< see ::GPS_FEAT_IMS
  0,                                            ///< see ::GPS_FEAT_HAVEQUICK
  0,                                            ///< see ::GPS_FEAT_NTP
  0,                                            ///< see ::GPS_FEAT_NET_CFG
  0,                                            ///< see ::GPS_FEAT_VST
  0,                                            ///< see ::GPS_FEAT_SHS
  0                                             ///< see ::GPS_FEAT_XBP
};



#if REPORT_CFG || REPORT_CFG_DETAILS

/**
 * @brief A table of name strings associated with ::GPS_FEATURE_BITS
 */
static const char *gps_ri_feature_names[N_GPS_FEATURE] = DEFAULT_GPS_FEATURE_NAMES;

#endif  // REPORT_CFG || REPORT_CFG_DETAILS



#if REPORT_CFG

/**
 * @brief A table of name strings associated with ::PCPS_FEATURE_BITS
 */
static const char *pcps_feature_names[N_PCPS_FEATURE_BITS] = PCPS_FEATURE_NAMES;

#endif  // REPORT_CFG



#if ( _PCPS_USE_USB || DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING )

static const char str_spc_cyc[] = " cyc";
static const char str_spc_ns[] = " ns";

static __mbg_inline /*HDR*/
/**
 * @brief Convert MBG_PC_CYCLES cycles to nanoseconds
 *
 * @param[in]  cyc   The number of PC cycles
 * @param[in]  freq  The clock frequency of the PC cycles counter
 *
 * @return the computed number of nanoseconds
 */
int64_t pc_cycles_to_ns( MBG_PC_CYCLES cyc, MBG_PC_CYCLES_FREQUENCY freq )
{
  uint64_t tmp = (uint64_t) cyc * 1000000000UL;

  _do_div( tmp, freq );

  return tmp;

}  // pc_cycles_to_ns

#endif  // ( _PCPS_USE_USB || DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING )



static __mbg_inline /*HDR*/
/**
 * @brief Check if a device is a PTP270PEX card
 */
int pcps_ddev_is_ptp270pex( const PCPS_DDEV *pddev )
{
  return _pcps_ddev_is_pci( pddev ) &&
       ( _pcps_ddev_dev_id( pddev ) == PCI_DEV_PTP270PEX );

}  // pcps_ddev_is_ptp270pex




#if defined( MBG_TGT_LINUX )

static /*HDR*/
/**
 * @brief Read a dword from a PLX PECS register
 *
 * @param[in]  pNode  Device structure
 * @param[in]  reg    Number of the register to read
 * @param[out] pval   Pointer to a variable to take the value read from the register
 *
 * @return ::PCI_SUCCESS on success, else one of the PCI BIOS error codes
 */
int mbg_plx_read_pecs_reg( struct pci_dev *pNode,
                           uint16_t reg, uint32_t *pval )
{
  int pci_rc;

  if ( reg & 0x03 )
    return PCI_BAD_REGISTER_NUMB;

  if ( reg < 0x1000 )
  {
    pci_rc = pci_read_config_dword( pNode, reg, pval );
  }
  else
  {
    uint32_t mcr_idx_sav = (uint32_t) -1;

    reg -= 0x1000;

    pci_rc = pci_read_config_dword( pNode, PLX_PECS_MAININDEX, &mcr_idx_sav );

    if ( pci_rc != PCI_SUCCESS )
      goto out;

    pci_rc = pci_write_config_dword( pNode, PLX_PECS_MAININDEX, reg );

    if ( pci_rc != PCI_SUCCESS )
      goto out;

    pci_rc = pci_read_config_dword( pNode, PLX_PECS_MAINDATA, pval );

    if ( pci_rc != PCI_SUCCESS )
      goto out;

    pci_rc = pci_write_config_dword( pNode, PLX_PECS_MAININDEX, mcr_idx_sav );
  }

out:
  return pci_rc;

}  // mbg_plx_read_pecs_reg

#endif



static /*HDR*/
/**
 * @brief Check if a PTP270PEX card can indicate when it's ready
 *
 * A PTP270PEX card must be accessed by the driver only
 * after it has finished booting. Otherwise the host system
 * may be locked up.
 *
 * On HW v2 cards the firmware can indicate when the card is
 * ready to be accessed, so this can be checked by the driver.
 * On these cards the on-board GPIO3 pin is hardwired to 0
 * to indicate this is supported, whereas on older v1 cards
 * the GPIO3 pin is pulled up to 1.
 *
 * @param[in]  pddev   Pointer to the device structure
 *
 * @return true if the card can flag "ready"
 *
 * @see ::ptp270pex_has_flagged_ready
 */
bool ptp270pex_can_flag_ready( const PCPS_DDEV *pddev )
{
  // The GPIO3 input level can only be read via a register
  // which is located inside the PCI configuration space
  // of the built-in PLX8111 PCIe-to-PCI bridge.
  //
  // So we must first locate the PCI bridge associated with our device,
  // read the PECS_GPIOCTL register and return 1 if the GPIO3 bit
  // has been pulled down to 0.

  uint32_t reg_val = (uint32_t) -1;
  int pci_rc = -1;

  #if defined( MBG_TGT_LINUX )

    struct pci_dev *bridge = NULL;

    // search for any PLX PCI bridge device
    while ( ( bridge = pci_get_device( PCI_VENDOR_ID_PLX,
                         PCI_DEVICE_ID_PLX_8111, bridge ) ) != NULL )
    {
      uint8_t uc;

      #if defined( DEBUG )
        _mbg_kdd_msg_1( MBG_LOG_DEBUG, "Found bridge: %s",
                       pci_name( bridge ) );
      #endif

      // Read the secondary bus number.
      pci_rc = pci_read_config_byte( bridge, 0x19, &uc );

      if ( ( pci_rc == PCI_SUCCESS ) && ( uc == pddev->dev.cfg.bus_num ) )
      {
        #if defined( DEBUG )
          _mbg_kdd_msg_1( MBG_LOG_DEBUG, "Found bridge associated with device %s",
                          _pcps_ddev_type_name( pddev ) );
        #endif
        break;
      }
    }

    if ( bridge )  // associated PCI bridge has been found
    {
      pci_rc = mbg_plx_read_pecs_reg( bridge, PLX_PECS_GPIOCTL, &reg_val );

      #if defined( DEBUG )
        _mbg_kdd_msg_3( MBG_LOG_DEBUG, "Read cfg dword %04lX: %08lX, rc: %i",
                        (ulong) PLX_PECS_GPIOCTL, (ulong) reg_val, pci_rc );
      #endif

      // The pci_get_device() calls above has increased the use count
      // for the last device which has been found, so we need to decrease
      // the use count if we don't need to access the device anymore.
      // This is done by calling the complementary function.
      pci_dev_put( bridge );
    }

  #elif defined( MBG_TGT_WIN32 )

    // We don't scan the PCI bus to find the associated bridge device
    // but just assume the card can flag when it's ready. If the card
    // actually can't then the driver just waits until the maximum
    // required uptime has been reached or exceeded, so there's no danger
    // that the system might lock up.
    pci_rc = PCI_SUCCESS;
    reg_val = 0;

  #elif defined( MBG_TGT_DOS )

    PLX_DEVICE_NODE dn_bridge = { 0 };

    pci_rc = mbg_find_plx8311_bridge( pddev->dev.cfg.bus_num, &dn_bridge );

    if ( pci_rc == PCI_SUCCESS )
      pci_rc = mbg_plx_read_pecs_reg( &dn_bridge, PLX_PECS_GPIOCTL, &reg_val );

  #else

  #endif

  // The device can indicate when it's ready if reg_val
  // has been read successfully, and the GPIO3 bit is 0.
  return ( pci_rc == PCI_SUCCESS ) && ( ( reg_val & PLX_PECS_GPIOCTL_GPIO3_DATA ) == 0 );

}  // ptp270pex_can_flag_ready



static /*HDR*/
/**
 * @brief Check if a PTP270PEX card indicates it is ready
 *
 * A PTP270PEX card must be accessed by the driver only
 * after it has finished booting. Otherwise the host system
 * may be locked up.
 *
 * On HW v2 cards the firmware can indicate when the card is
 * ready to be accessed, so the driver can call this function
 * to check this.
 *
 * @note The function ::ptp270pex_can_flag_ready should
 * have been called before to check if the card actually
 * supports this.
 *
 * @param[in]  pddev   Pointer to the device structure
 *
 * @return true if the card has flagged "ready"
 *
 * @see ::ptp270pex_can_flag_ready
 */
bool ptp270pex_has_flagged_ready( const PCPS_DDEV *pddev )
{
  // GPIO pin USERI is pulled down to 0 by the firmware
  // as soon as the card is ready to be accessed.
  // With PTP270PEX HW v1 cards this pin is always 1.

  // Read the LCS_CNTRL register and return 1 if the USERI
  // bit is set.

  MBG_IOPORT_ADDR_MAPPED cntrl_reg = _pcps_ddev_io_base_mapped( pddev, 1 )
                                   + PLX_LCS_CNTRL;
  uint32_t reg_val = _mbg_inp32_to_cpu( pddev, 0, cntrl_reg );

  return ( reg_val & PLX_LCS_CNTRL_USERI ) == 0;

}  // ptp270pex_has_flagged_ready



#if MBG_TGT_HAS_UPTIME

static /*HDR*/
/**
 * @brief Report the system uptime
 *
 * This is mainly used for debugging, and informational.
 *
 * @param[in]  p_uptime  The system uptime
 *
 * @see ::wait_ptp270pex_ready
 * @see ::MAX_BOOT_TIME_PTP270PEX
 */
void report_uptime( const MBG_SYS_UPTIME *p_uptime )
{
  #if defined( MBG_TGT_WIN32 )
    _mbg_kdd_msg_2( MBG_LOG_INFO, "System uptime: %" PRIi64 " s, required %u s",
                    (int64_t) *p_uptime, MAX_BOOT_TIME_PTP270PEX );
  #else

    char ws[120];
    int l = sizeof( ws );
    int n = 0;

    n += snprintf( &ws[n], l - n, "System uptime " );

    #if defined( MBG_TGT_LINUX )
      n += snprintf( &ws[n], l - n, "%llu jiffies -> ",
                     (unsigned long long) ( get_jiffies_64() - INITIAL_JIFFIES ) );
    #endif

    n += snprintf( &ws[n], l - n, "%llu", (unsigned long long) *p_uptime );
    n += snprintf( &ws[n], l - n, " s, required %u s", MAX_BOOT_TIME_PTP270PEX );

    if ( *p_uptime < MAX_BOOT_TIME_PTP270PEX )
    {
      int waiting = (int) ( MAX_BOOT_TIME_PTP270PEX - *p_uptime );
      n += snprintf( &ws[n], l - n, ", waiting %i s", waiting );
    }
    else
      n += snprintf( &ws[n], l - n, ", OK." );

    _mbg_kdd_msg_1( MBG_LOG_INFO, "%s", ws );

  #endif

}  // report_uptime

#endif  // MBG_TGT_HAS_UPTIME



static /*HDR*/
/**
 * @brief Wait until a PTP270PEX card is ready after power-up
 *
 * A PTP270PEX card must be accessed by the driver only
 * after it has finished booting. Otherwise the host system
 * may be locked up.
 *
 * @param[in]  pddev   Pointer to the device structure
 *
 * @see ::ptp270pex_can_flag_ready
 * @see ::ptp270pex_has_flagged_ready
 * @see ::report_uptime
 * @see ::MAX_BOOT_TIME_PTP270PEX
 */
void wait_ptp270pex_ready( const PCPS_DDEV *pddev )
{
  MBG_SYS_TIME t1;
  MBG_SYS_TIME t2;
  MBG_SYS_UPTIME uptime;
  int delayed = 0;
  bool can_flag_ready;
  int i = 0;

  // A newer PTP270PEX card (HW v2.0) can indicate if it has
  // finished booting and thus is ready to be accessed, but
  // older PTP270PEX cards (HW v1.0) don't support this.
  can_flag_ready = ptp270pex_can_flag_ready( pddev );

  mbg_get_sys_time( &t1 );

  for (;;)
  {
    mbg_get_sys_uptime( &uptime );

    #if MBG_TGT_HAS_UPTIME
      #if !defined( DEBUG )
        if ( delayed )
      #endif
          report_uptime( &uptime );
    #endif

    if ( can_flag_ready )
    {
      bool b = ptp270pex_has_flagged_ready( pddev );

      if ( b )
        break;
    }

    // If the card is unable to indicate when it is ready
    // and the target system doesn't support uptime
    // then it makes no sense to keep looping and waiting,
    // so just stop waiting and hope the card is ready.
    if ( uptime == 0 || uptime == -1 )
      break;  // assume uptime not supported

    // If the target system supports uptime then
    // keep looping until the system uptime exceeds
    // the max. boot time required by the card.

    if ( uptime >= MAX_BOOT_TIME_PTP270PEX )
      break;

    mbg_sleep_sec( 1 );
    delayed = 1;

    if ( ++i >= MAX_BOOT_TIME_PTP270PEX )
      break;
  }

  if ( delayed )
  {
    long dt;

    mbg_get_sys_time( &t2 );

    dt = mbg_delta_sys_time_ms( &t2, &t1 );

    _mbg_kdd_msg_2( MBG_LOG_INFO, "PTP270PEX startup delay: %li.%03li s",
                    dt / 1000, ( ( dt < 0 ) ? -dt : dt ) % 1000 );
  }

}  // wait_ptp270pex_ready



static /*HDR*/
/**
 * @brief Check if IRQ usage with a device is unsafe
 *
 * A few early PCI Express cards had a problem where usage
 * of IRQs was unsafe. Firmware upgrades are available to fix this.
 * See http://www.meinberg.de/english/info/pex-upgrades.htm
 *
 * @param[in]  pddev               Pointer to the device structure
 * @param[in]  req_fw_ver          Required firmware version
 * @param[in]  req_asic_ver_major  Required ASIC major version
 * @param[in]  req_asic_ver_minor  Required ASIC major version
 *
 * @return true if IRQ operation is unsafe
 */
bool pcps_check_pex_irq_unsafe( PCPS_DDEV *pddev, uint16_t req_fw_ver,
                                uint8_t req_asic_ver_major, uint8_t req_asic_ver_minor )
{
  bool b = !_pcps_pex_irq_is_safe( _pcps_ddev_fw_rev_num( pddev ), req_fw_ver,
                                   _pcps_ddev_asic_version( pddev ),
                                   req_asic_ver_major, req_asic_ver_minor );

  if ( b )
  {
    pddev->irq_stat_info |= PCPS_IRQ_STAT_UNSAFE;

    // Prevent the driver from writing IRQ ACK to the card even if IRQs
    // should be unintentionally enabled.
    pddev->irq_ack_port = 0;
    pddev->irq_ack_mask = 0;
  }

  return b;

}  // pcps_check_pex_irq_unsafe



static /*HDR*/
/**
 * @brief Setup the memory mapped addresses depending on the device type
 *
 * @param[in]  pddev  Pointer to the device structure
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
int setup_mm_addr( PCPS_DDEV *pddev )
{
  PCPS_MM_LAYOUT FAR *mm_addr = (PCPS_MM_LAYOUT FAR *) pddev->rsrc_info.mem[0].start_mapped;

  pddev->mm_asic_addr = NULL;
  pddev->mm_tstamp_addr = NULL;

  if ( _pcps_ddev_is_pci_mbgpex( pddev ) )
  {
    pddev->mm_asic_addr = &mm_addr->mbgpex.asic;
    pddev->mm_tstamp_addr = &mm_addr->mbgpex.tstamp;
  }
  else
    if ( _pcps_ddev_is_pci_pex8311( pddev ) )
    {
      pddev->mm_asic_addr = &mm_addr->pex8311.asic;
      pddev->mm_tstamp_addr = &mm_addr->pex8311.tstamp;
    }

  #if DEBUG_RSRC
    _mbg_kdd_msg_1( MBG_LOG_INFO, "MM base addr:   %p", mm_addr );
    _mbg_kdd_msg_1( MBG_LOG_INFO, "MM ASIC addr:   %p", pddev->mm_asic_addr );
    _mbg_kdd_msg_1( MBG_LOG_INFO, "MM tstamp addr: %p", pddev->mm_tstamp_addr );
    _mbg_kdd_msg_1( MBG_LOG_INFO, "MM tstamp offs: 0x%02lX",
                    (ulong) ( (uint8_t *) pddev->mm_tstamp_addr - (uint8_t *) mm_addr ) );
  #endif

  return MBG_SUCCESS;

}  // setup_mm_addr



#if defined( DEBUG )

/*HDR*/
void pcps_dump_data( const void *buffer, size_t count, const char *info )
{
  const uint8_t *p = (const uint8_t *) buffer;
  size_t i = 0;

  while ( i < count )
  {
    char ws[80];
    const char *cp = (const char *) p;
    int j;
    int n = 0;
    ws[0] = 0;

    for ( j = 0; j < 4; )
    {
      n += mbg_kdd_snprintf( &ws[n], sizeof( ws ) - n, " %02X", *p++ );

      j++;

      if ( ++i >= count )
        break;
    }

    n += mbg_kdd_snprintf( &ws[n], sizeof( ws ) - n, "  \"" );

    while ( j > 0 )
    {
      n += mbg_kdd_snprintf( &ws[n], sizeof( ws ) - n, "%c",
             ( ( *cp >= ' ' ) && ( *cp < 0x7F ) ) ? *cp : '#' );
      cp++;
      j--;
    }

    n += mbg_kdd_snprintf( &ws[n], sizeof( ws ) - n, "\"" );

    _mbg_kdd_msg_2( MBG_LOG_INFO, "%s:%s", info, ws );
  }

}  // pcps_dump_data

#endif  // defined( DEBUG )



#if DEBUG_IO

static /*HDR*/
void report_io_cmd( uint8_t cmd, uint16_t count, const char *info )
{
  _mbg_kdd_msg_4( MBG_LOG_DEBUG, "%s: cmd: 0x%02X (0x%08X), cnt: %u",
                  info, cmd, _cpu_to_mbg32( cmd ), count );

}  // report_io_cmd

#endif  // DEBUG_IO



#if DEBUG_ACCESS_TIMING

static uint32_t debug_dummy_var;


static /*HDR*/
/**
 * @brief Report access timing for a device
 *
 * This is only used for testing.
 *
 * @param[in]  pddev           Pointer to the device structure
 * @param[in]  info            Informational string to be printed
 * @param[in]  t_after_cmd     Cycles value taken after command code has been written
 * @param[in]  t_after_reread  Cycles value taken after a data word has been re-read
 */
void report_access_timing( const PCPS_DDEV *pddev, const char *info,
                           MBG_PC_CYCLES t_after_cmd, MBG_PC_CYCLES t_after_reread )
{
  #define _FMT "%s %s: %" PRIi64 "/%" PRIi64 "%s"

  int64_t delta_cyc_write = t_after_cmd - pddev->acc_cycles;
  int64_t delta_cyc_read = t_after_reread ? ( t_after_reread - t_after_cmd ) : 0;

  _mbg_kdd_msg_5( MBG_LOG_DEBUG, _FMT, _pcps_ddev_type_name( pddev ),
                  info, delta_cyc_write, delta_cyc_read, str_spc_cyc );

  // We can only report time intervals
  // if we know the cycles frequency.
  if ( pc_cycles_frequency )
  {
    int64_t delta_t_write = pc_cycles_to_ns( delta_cyc_write, pc_cycles_frequency );
    int64_t delta_t_read = pc_cycles_to_ns( delta_cyc_read, pc_cycles_frequency );

    _mbg_kdd_msg_5( MBG_LOG_DEBUG, _FMT, _pcps_ddev_type_name( pddev ),
                    info, delta_t_write, delta_t_read, str_spc_ns );
  }

  #undef _FMT

}  // report_access_timing



#if TEST_PORT_ACCESS

static /*HDR*/
/**
 * @brief Report delta cycles
 *
 * This is only used for testing.
 *
 * @param[in]  info   Informational string to be printed
 * @param[in]  t1     Cycles value taken before an action
 * @param[in]  t2     Cycles value taken after an action
 */
void report_delta_time( const char *info, MBG_PC_CYCLES t1, MBG_PC_CYCLES t2 )
{
  #define _FMT "%s: %" PRIi64 "%s"

  int64_t delta_cyc = t2 - t1;

  _mbg_kdd_msg_3( MBG_LOG_DEBUG, _FMT, info, delta_cyc, str_spc_cyc );

  // We can only report time intervals
  // if we know the cycles frequency.
  if ( pc_cycles_frequency )
  {
    int64_t delta_t = pc_cycles_to_ns( delta_cyc, pc_cycles_frequency );

    _mbg_kdd_msg_3( MBG_LOG_DEBUG, _FMT, info, delta_t, str_spc_ns );
  }

  #undef _FMT

}  // report_delta_time



volatile uint8_t dummy_u8;
volatile uint16_t dummy_u16;
volatile uint32_t dummy_u32;

static /*HDR*/
/**
 * @brief Test how long it takes to access specific ports
 *
 * @param[in]  pddev  Pointer to the device structure
 */
void test_port_access( const PCPS_DDEV *pddev )
{
  MBG_PC_CYCLES t1;
  MBG_PC_CYCLES t2;

  // FIXME TODO This specific test function may need some review.

  if ( _pcps_ddev_is_usb( pddev ) ||
       _pcps_ddev_is_pci_s5920( pddev ) )
  {
    _mbg_kdd_msg_1( MBG_LOG_DEBUG, "Skipping port access tests for %s.",
                    _pcps_ddev_type_name( pddev ) );
    return;
  }

  _mbg_kdd_msg_1( MBG_LOG_DEBUG, "Running port access tests for %s.",
                  _pcps_ddev_type_name( pddev ) );



  _mbg_kdd_msg_1( MBG_LOG_DEBUG, "I/O status port: %lX",
                  (ulong) pddev->status_port );

  mbg_get_pc_cycles( &t1 );
  dummy_u8 = _mbg_inp8( pddev, 0, pddev->status_port );
  mbg_get_pc_cycles( &t2 );


  mbg_get_pc_cycles( &t1 );
  dummy_u16 = _mbg_inp16_to_cpu( pddev, 0, pddev->status_port );
  mbg_get_pc_cycles( &t2 );

  report_delta_time( "16 bit read I/O status port", t1, t2 );

  mbg_get_pc_cycles( &t1 );
  dummy_u32 = _mbg_inp32_to_cpu( pddev, 0, pddev->status_port );
  mbg_get_pc_cycles( &t2 );

  report_delta_time( "32 bit read I/O status port", t1, t2 );


  #if MBG_TGT_SUPP_MEM_ACC
    // TODO Needs to be fixed, has_mapped_sys_virtual_address()
    // doesn't exist anymore.

    if ( has_mapped_sys_virtual_address( pddev ) )
    {
      // Explicitly write to a memory mapped address
      // pddev->mm_asic_addr->pci_data.ul = cmd;

      // Explicitly read from a memory mapped address
      // debug_dummy_var = pddev->mm_asic_addr->pci_data.ul;
      _mbg_kdd_msg_1( MBG_LOG_DEBUG, "MM status port: %p",
                      &pddev->mm_asic_addr->status_port.ul );

      mbg_get_pc_cycles( &t1 );
      dummy_u8 = pddev->mm_asic_addr->pci_data.b[0];
      mbg_get_pc_cycles( &t2 );

      report_delta_time( "8 bit read MM status port", t1, t2 );

      mbg_get_pc_cycles( &t1 );
      dummy_u16 = pddev->mm_asic_addr->pci_data.us[0];
      mbg_get_pc_cycles( &t2 );

      report_delta_time( "16 bit read MM status port", t1, t2 );

      mbg_get_pc_cycles( &t1 );
      dummy_u32 = pddev->mm_asic_addr->pci_data.ul;
      mbg_get_pc_cycles( &t2 );

      report_delta_time( "32 bit read MM status port", t1, t2 );
    }
    else
      _mbg_kdd_msg_0( MBG_LOG_DEBUG, "MM registers not mapped." );

  #else
    _mbg_kdd_msg_0( MBG_LOG_DEBUG, "Memory mapping not supported for this target." );
  #endif

}  // test_port_access

#endif  // TEST_PORT_ACCESS

#endif  // DEBUG_ACCESS_TIMING



#if DEBUG_IO_TIMING

static /*HDR*/
/**
 * @brief Report I/O timing for a device
 *
 * This is only used for testing.
 *
 * @param[in]  pddev         Pointer to the device structure
 * @param[in]  info          Informational string to be printed
 * @param[in]  cmd           The command sent to the device
 * @param[in]  count         The number of bytes read from the device
 * @param[in]  t_after_cmd   Cycles value taken after command code has been written
 * @param[in]  t_after_busy  Cycles value taken after busy flag was cleared by the device
 * @param[in]  t_done        Cycles value taken after bytes have been read
 */
void report_io_timing( const PCPS_DDEV *pddev, const char *info,
                       uint8_t cmd, uint16_t count, MBG_PC_CYCLES t_after_cmd,
                       MBG_PC_CYCLES t_after_busy, MBG_PC_CYCLES t_done )
{
  #define _FMT_1 "%s %s cmd: 0x%02X (%u bytes)"
  #define _FMT_2 "write %" PRIi64 "%s, busy: %" PRIi64 "%s, read: %" PRIi64 "/%" PRIi64 "%s"

  int64_t cmd_cycles = t_after_cmd - pddev->acc_cycles;
  int64_t busy_cycles = t_after_busy - t_after_cmd;
  int64_t read_cycles = t_done - t_after_busy;
  uint64_t cycles_per_read = 0;

  if ( count )
  {
    cycles_per_read = read_cycles;
    _do_div( cycles_per_read, count );
  }

  _mbg_kdd_msg_4( MBG_LOG_DEBUG, _FMT_1, _pcps_ddev_type_name( pddev ),
                  info, cmd, count );

  _mbg_kdd_msg_7( MBG_LOG_DEBUG, _FMT_2, cmd_cycles, str_spc_cyc, busy_cycles,
                  str_spc_cyc, read_cycles, cycles_per_read, str_spc_cyc );

  // We can only report time intervals
  // if we know the cycles frequency.
  if ( pc_cycles_frequency )
  {
    int64_t cmd_time = pc_cycles_to_ns( cmd_cycles, pc_cycles_frequency );
    int64_t busy_time = pc_cycles_to_ns( busy_cycles, pc_cycles_frequency );
    int64_t read_time = pc_cycles_to_ns( read_cycles, pc_cycles_frequency );
    uint64_t time_per_read = 0;

    if ( count )
    {
      time_per_read = read_time;
      _do_div( time_per_read, count );
    }

    _mbg_kdd_msg_7( MBG_LOG_DEBUG, _FMT_2, cmd_time, str_spc_ns, busy_time,
                    str_spc_ns, read_time, cycles_per_read, str_spc_ns );
  }

  #if 0 && defined( MBG_TGT_LINUX )  // ###
    _mbg_kdd_msg_3( MBG_LOG_INFO, "Cycles after cmd: %lli, after busy: %lli, when done: %lli",
                    (long long) t_after_cmd,
                    (long long) t_after_busy,
                    (long long) t_done );
  #endif

  #undef _FMT_1
  #undef _FMT_2

}  // report_io_timing

#endif  // DEBUG_IO_TIMING



#if defined( DEBUG )

static /*HDR*/
/**
 * @brief Report the return code to be returned by a function
 *
 * @param[in]  rc    The return code to be reported
 * @param[in]  info  Informational string to be printed
 */
void report_ret_val( int rc, const char *info )
{
  if ( mbg_rc_is_error( rc ) )
    _mbg_kdd_msg_3( MBG_LOG_ERR, "%s: FAILED, rc %i: %s",
                    info, rc, mbg_strerror( rc ) );
  #if DEBUG_IO
    else
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, "%s: success, rc %i",
                      info, rc );
  #endif

}  // report_ret_val

#endif  // defined( DEBUG )



#if _PCPS_USE_USB

static /*HDR*/
/**
 * @brief Report USB timing for a device
 *
 * @param[in]  pddev  Pointer to the device structure
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
int check_usb_timing( PCPS_DDEV *pddev )
{
  int rc = MBG_SUCCESS;

  if ( _pcps_ddev_has_hr_time( pddev ) )
  {
    #if USE_LOCAL_IO_BUFFER
      PCPS_HR_TIME hrt;
      PCPS_HR_TIME *p_hrt = &hrt;
    #else
      PCPS_HR_TIME *p_hrt = &pddev->io_buffer.pcps_hr_time;
    #endif

    #define _FMT "USB access: %" PRIi64 "%s"

    MBG_PC_CYCLES completion_cycles;
    int64_t delta_cycles;

    #if defined( DEBUG )
      _mbg_kdd_msg_0( MBG_LOG_DEBUG, "Reading PCPS_HR_TIME as USB timing test:" );
    #endif

    // Determine USB access time. The first cycles value is taken
    // by the low level routine and saved in pddev->acc_cycles.
    rc = _pcps_read_var( pddev, PCPS_GIVE_HR_TIME, *p_hrt );
    mbg_get_pc_cycles( &completion_cycles );

    if ( mbg_rc_is_error( rc ) )
      goto out;


    delta_cycles = mbg_delta_pc_cycles( &completion_cycles, &pddev->acc_cycles );

    #if defined( DEBUG )
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, _FMT, delta_cycles, str_spc_cyc );
    #endif

    // We can only deal with time intervals
    // if we know the cycles frequency.
    if ( pc_cycles_frequency )
    {
      int64_t delta_t_ns = pc_cycles_to_ns( delta_cycles, pc_cycles_frequency );
      // If access time is below 1 ms then 125 us microframe timing
      // introduced with USB 2.0 is supported.
      pddev->usb_20_mode = delta_t_ns < 1000000UL;

    #if defined( DEBUG )
        _mbg_kdd_msg_2( MBG_LOG_DEBUG, _FMT, delta_t_ns, str_spc_ns );
      #endif
    }

    #if MUST_REPORT_PROBE_DEVICE_DETAILS
      _mbg_kdd_msg_1( MBG_LOG_INFO, "USB microframe timing%s detected.",
                      pddev->usb_20_mode ? str_empty : " NOT" );
    #endif

    #undef _FMT
  }

out:
  if ( mbg_rc_is_error( rc ) )
    _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_INIT );

  return rc;

}  // check_usb_timing



static /*HDR*/
/**
 * @brief Generic read data from an USB device.
 *
 * @param[in]  pddev        Pointer to the device structure.
 * @param[in]  cmd          The command code for the board, see @ref PCPS_CMD_CODES,
 *                          or @ref PC_GPS_CMD_CODES if @p is_gps_data is true.
 * @param[out] buffer       A buffer to take the bytes to be read.
 * @param[in]  count        The number of bytes to be read into the buffer.
 * @param[in]  is_gps_data  Indicates that @p cmd is a one of the @ref PC_GPS_CMD_CODES.
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @see @ref pcps_read_usb
 * @see @ref pcps_read_gps
 */
int pcps_read_usb_generic( PCPS_DDEV *pddev, uint8_t cmd,
                           void FAR *buffer, uint16_t count, bool is_gps_data )
{
  int rc;
  int transfer_size;

  #define FNC_ID_USB_READ_GEN "USB read"

  #if defined( MBG_TGT_WIN32_PNP )
    int this_frame_num_1;
    int this_frame_num_2;
    LARGE_INTEGER UsbPreCount;
    LARGE_INTEGER UsbPostCount;
  #endif

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_cmd = 0;
  #endif

  #if DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_busy = 0;
    MBG_PC_CYCLES t_done = 0;
  #endif

  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_entry();
  #endif

  mbg_get_pc_cycles( &pddev->acc_cycles );

  // We write the request data to our device's private data structure,
  // so we don't have to explicitly allocate a buffer here.
  if ( is_gps_data )
  {
    pddev->cmd_info.gps_cmd_info.cmd = PCPS_READ_GPS_DATA;
    pddev->cmd_info.gps_cmd_info.gps_cmd = cmd;
    transfer_size = sizeof( pddev->cmd_info.gps_cmd_info );
  }
  else
  {
    pddev->cmd_info.cmd = cmd;
    transfer_size = sizeof( pddev->cmd_info.cmd );
  }

  rc = pcps_direct_usb_write( pddev, &pddev->cmd_info, transfer_size );

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_cmd );
  #endif

  #if DEBUG_IO_TIMING
    t_after_busy = t_after_cmd;
  #endif

  if ( mbg_rc_is_error( rc ) )
  {
    #if REPORT_IO_ERRORS
      if ( is_gps_data )
        _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_USB_READ_GEN ": GPS cmd 0x%02X failed, rc: %i",  // REPORT_IO_ERRORS
                        pddev->cmd_info.gps_cmd_info.gps_cmd, rc );
      else
        _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_USB_READ_GEN ": cmd 0x%02X failed, rc: %i",  // REPORT_IO_ERRORS
                        pddev->cmd_info.cmd, rc );
    #endif

    goto out;
  }


  #if DEBUG_IO
    if ( is_gps_data )
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, FNC_ID_USB_READ_GEN ": GPS write 0x%02X succeeded, %i bytes read",
                      pddev->cmd_info.gps_cmd_info.gps_cmd, rc );
    else
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, FNC_ID_USB_READ_GEN ": write 0x%02X succeeded, %i bytes read",
                      pddev->cmd_info.cmd, rc );
  #endif  // DEBUG_IO


  if ( buffer == NULL || count == 0 )  // no data need to be read
  {
    #if DEBUG_IO
      _mbg_kdd_msg_0( MBG_LOG_DEBUG, FNC_ID_USB_READ_GEN ": no data to be read, exiting" );
    #endif  // DEBUG_IO

    goto out;
  }


  #if defined( MBG_TGT_WIN32_PNP )
    #if USE_USB_MICRO_FRAMES
      this_frame_num_1 = micro_frame_num_1 ? micro_frame_num_1 : frame_num_1;
      this_frame_num_2 = micro_frame_num_2 ? micro_frame_num_2 : frame_num_2;
    #else
      this_frame_num_1 = frame_num_1;
      this_frame_num_2 = frame_num_2;
    #endif

    UsbPreCount  = Count1;
    UsbPostCount = Count2;
  #endif

  rc = pcps_direct_usb_read( pddev, buffer, count );

  if ( mbg_rc_is_error( rc ) )
  {
    #if REPORT_IO_ERRORS
      if ( is_gps_data )
        _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_USB_READ_GEN ": rd after GPS cmd 0x%02X failed, rc: %i",  // REPORT_IO_ERRORS
                        pddev->cmd_info.gps_cmd_info.gps_cmd, rc );
      else
        _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_USB_READ_GEN ": rd after cmd 0x%02X failed, rc: %i",  // REPORT_IO_ERRORS
                        pddev->cmd_info.cmd, rc );
    #endif

    goto out;
  }

  #if DEBUG_IO
    if ( is_gps_data )
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, FNC_ID_USB_READ_GEN ": rd after GPS cmd 0x%02X succeeded, bytes read: %i",
                      pddev->cmd_info.gps_cmd_info.gps_cmd, rc );
    else
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, FNC_ID_USB_READ_GEN ": rd after cmd 0x%02X succeeded, bytes read: %i",
                      pddev->cmd_info.cmd, rc );

    pcps_dump_data( buffer, rc, FNC_ID_USB_READ_GEN );
  #endif


  // "rc" should now contain the number of bytes that have been read,
  // and this should match "count".
  if ( rc != count )
  {
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_USB_READ_GEN ": rcvd. %d != exp. %d",  // REPORT_IO_ERRORS
                      rc, count );
    #endif

    rc = MBG_ERR_NBYTES;
    goto out;
  }


  #if defined( MBG_TGT_WIN32_PNP )

    if ( cmd == PCPS_GIVE_HR_TIME )
    {
      ULONGLONG usb_latency_cycles;
      ULONGLONG cycles_diff;
      ULONGLONG time_diff;
      ULONGLONG frame_length_cycles;
      int FrameNumberDiff;

    #if !USE_USB_MICRO_FRAMES
      if ( pddev->usb_20_mode )
      {
        // Use USB 2.0 microframe timing.
        // Just add an offset to compensate constant latency.
        // This value has been determined experimentally on different hardware platforms
        usb_latency_cycles = ( (ULONGLONG) PerfFreq.QuadPart ) / 20000UL; // represents 50 us
      }
      else
    #endif
      {
        // USB 1.1 mode with millisecond timing.
        // Compensate latency to millisecond frame boundaries.

        if ( ( this_frame_num_2 - this_frame_num_1 ) < 0 )
          FrameNumberDiff = 2;
        else
          FrameNumberDiff = this_frame_num_2 - this_frame_num_1;

        cycles_diff = (ULONGLONG) ( UsbPostCount.QuadPart - UsbPreCount.QuadPart );

        #if USE_USB_MICRO_FRAMES
          if ( micro_frame_num_1 > 0 || micro_frame_num_2 > 0 )
            frame_length_cycles = (ULONGLONG) ( (ULONGLONG) PerfFreq.QuadPart ) / 8000UL;
          else
            frame_length_cycles = (ULONGLONG) ( (ULONGLONG) PerfFreq.QuadPart ) / 1000UL;
        #else
          frame_length_cycles = (ULONGLONG) ( (ULONGLONG) PerfFreq.QuadPart ) / 1000UL;
        #endif

        if ( ( this_frame_num_1 == 0 ) && ( this_frame_num_2 == 0 ) )
        {
          if ( cycles_diff > frame_length_cycles )
            usb_latency_cycles =  cycles_diff - frame_length_cycles;
          else
            usb_latency_cycles =  frame_length_cycles - cycles_diff;
        }
        else
          usb_latency_cycles =  cycles_diff - ( ( FrameNumberDiff - 1 ) * frame_length_cycles );

        #if defined( DEBUG )
          _mbg_kdd_msg_4( MBG_LOG_DEBUG, "FD %d CD %" PRIu64 " l %" PRIu64 " fl %" PRIu64, FrameNumberDiff,
                          cycles_diff, usb_latency_cycles, frame_length_cycles );
        #endif
      }

      pddev->acc_cycles += usb_latency_cycles;
    }
  #endif  // defined( MBG_TGT_WIN32_PNP )

out:
  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_done );
    report_io_timing( pddev, is_gps_data ? "USB GPS" : "USB", cmd, count, t_after_cmd, t_after_busy, t_done );
  #endif

  #if DEBUG_ACCESS_TIMING
    report_access_timing( pddev, "USB wr/rd", t_after_cmd, 0 );
  #endif

  #if defined( DEBUG )
    report_ret_val( rc, FNC_ID_USB_READ_GEN );
  #endif

  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_exit();
  #endif

  #undef FNC_ID_USB_READ_GEN

  return rc;

}  // pcps_read_usb_generic



static /*HDR*/
/**
 * @brief Generic write data to an USB device.
 *
 * @param[in]  pddev        Pointer to the device structure
 * @param[in]  cmd          The command code for the board, see @ref PCPS_CMD_CODES,
 *                          or @ref PC_GPS_CMD_CODES if @p is_gps_data is true.
 * @param[in]  buffer       A buffer with data to be written according to the cmd code
 * @param[in]  count        The number of bytes to be written according to the cmd code
 * @param[in]  is_gps_data  Indicates that @p cmd is a one of the @ref PC_GPS_CMD_CODES.
 *
 * @return ::MBG_SUCCESS on success,
 *         ::MBG_ERR_TIMEOUT if device didn't respond in time,
 *         or one of the other @ref MBG_RETURN_CODES
 *
 * @see @ref pcps_write
 * @see @ref pcps_write_gps
 */
int pcps_write_usb_generic( PCPS_DDEV *pddev, uint8_t cmd,
                            const void FAR *buffer, uint16_t count, bool is_gps_data )
{
  int rc;
  int buf_size = ( 2 * sizeof( cmd ) ) + count;  // Size of the buffer to allocate: Actually for one
                                                 // of the PCPS_CMD_CODES, plus eventually one of the
                                                 // PC_GPS_CMD_CODES, plus number of data bytes to transfer.
  uint8_t *p = _pcps_kmalloc( buf_size );
  uint8_t *pb;                                   // Will point into the allocated buffer.
  int transfer_bytes;

  #define FNC_ID_USB_WRITE_GEN "USB wr gen"

  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_entry();
  #endif

  if ( p == NULL )
  {
    rc = MBG_ERR_NO_MEM;
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_0( MBG_LOG_ERR, FNC_ID_USB_WRITE_GEN ": memory allocation failed" );  // REPORT_IO_ERRORS
    #endif
    goto out;
  }


  pb = p;  // point to start of the allocated buffer

  if ( is_gps_data )
  {
    *pb++ = PCPS_WRITE_GPS_DATA;  // first byte is a specific command code
    *pb++ = cmd;                  // next the cmd, which is actually the data type, one of the PC_GPS_CMD_CODES
    transfer_bytes = 2;
  }
  else
  {
    *pb++ = cmd;    // only the actual cmd code, one of the PCPS_CMD_CODES
    transfer_bytes = 1;
  }

  #if DEBUG_IO
    _mbg_kdd_msg_3( MBG_LOG_DEBUG, FNC_ID_USB_WRITE_GEN ": %scmd %02X, %u bytes",
                    is_gps_data ? str_gps_spc : str_empty, cmd, count );
    pcps_dump_data( buffer, count, FNC_ID_USB_WRITE_GEN );
  #endif

  // Now append the data bytes.
  memcpy( pb, buffer, count );
  transfer_bytes += count;

  rc = pcps_direct_usb_write( pddev, p, transfer_bytes );

  if ( mbg_rc_is_error( rc ) )
  {
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_USB_WRITE_GEN ": write failed: %s (rc: %i)",  // REPORT_IO_ERRORS
                      mbg_strerror( rc ), rc );
    #endif
    goto out_free;
  }

  // Read the completion code.
  rc = pcps_direct_usb_read( pddev, p, 1 );

  if ( mbg_rc_is_error( rc ) )
  {
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_USB_WRITE_GEN ": read completion code failed: %s (rc: %i)",  // REPORT_IO_ERRORS
                      mbg_strerror( rc ), rc );
    #endif

    goto out_free;
  }

  rc = (int8_t) p[0];  // return the completion code read from the device

out_free:
  _pcps_kfree( p, buf_size );

out:
  #if defined( DEBUG )
    report_ret_val( rc, FNC_ID_USB_WRITE_GEN );
  #endif

  #if DEBUG_USB_IO > 1
    _mbgddmsg_fnc_exit();
  #endif

  #undef FNC_ID_USB_WRITE_GEN

  return rc;

}  // pcps_write_usb_generic

#endif  // _PCPS_USE_USB



#if defined( __GNUC__ )
// Avoid "no previous prototype" with some gcc versions.
__mbg_inline
int pcps_wait_busy( PCPS_DDEV *pddev );
#endif

__mbg_inline /*HDR*/
/**
 * @brief Wait as long as a device is busy, or until timeout
 *
 * Used by the @ref pcps_read_fncs to wait after the command byte has been
 * written until the requested data has been made available by the device.
 *
 * @param[in]  pddev   Pointer to the device structure
 *
 * @return ::MBG_SUCCESS on success, or ::MBG_ERR_TIMEOUT
 *
 * @see @ref pcps_read_fncs
 */
int pcps_wait_busy( PCPS_DDEV *pddev )
{
  if ( _pcps_ddev_status_busy( pddev ) )
  {
    #if defined( MBG_TGT_BSD )
      struct timeval tv_start;

      getmicrouptime( &tv_start );

      while ( _pcps_ddev_status_busy( pddev ) )
      {
        struct timeval tv_now;
        long long delta_ms;

        getmicrouptime( &tv_now );
        delta_ms = ( ( tv_now.tv_sec - tv_start.tv_sec ) * 1000 )
                 + ( ( tv_now.tv_usec - tv_start.tv_usec ) / 1000 );
        if ( delta_ms > _pcps_ddev_timeout_clk( pddev ) )
          return MBG_ERR_TIMEOUT;
      }
    #elif _PCPS_USE_CLOCK_TICK
      clock_t timeout_val = clock() + _pcps_ddev_timeout_clk( pddev );

      while ( _pcps_ddev_status_busy( pddev ) )
        if ( _pcps_time_after( clock(), timeout_val ) )
          return MBG_ERR_TIMEOUT;
    #else
      long cnt = _pcps_ddev_timeout_clk( pddev );

      for ( ; _pcps_ddev_status_busy( pddev ); cnt-- )
        if ( cnt == 0 )
          return MBG_ERR_TIMEOUT;
    #endif
  }

  return MBG_SUCCESS;

}  // pcps_wait_busy



/**
 * @defgroup pcps_read_fncs Device read functions
 *
 * These group of functions is used for low level access to Meinberg
 * bus-level devices. Which of the function is actually to be used
 * depends on the device's bus type and interface chip and is determined
 * by the function ::pcps_probe_device which is called at device startup.
 *
 * @see ::pcps_read_null
 * @see ::pcps_read_std
 * @see ::pcps_read_amcc_s5933
 * @see ::pcps_read_amcc_s5920
 * @see ::pcps_read_asic
 * @see ::pcps_read_asic_mm
 * @see ::pcps_read_asic_mm16
 * @see ::pcps_read_usb
 *
 * @{ */


static /*HDR*/
/**
 * @brief Dummy read function
 *
 * Used if a clock is not properly initialized, in order to
 * avoid I/O access on unspecified ports.
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see @ref pcps_read_fncs
 */
int pcps_read_null( PCPS_DDEV *pddev, uint8_t cmd,
                    void FAR *buffer, uint16_t count )
{
  // avoid compiler warnings
  (void) pddev;
  (void) cmd;
  (void) buffer;
  (void) count;

  return MBG_ERR_TIMEOUT;

}  // pcps_read_null

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_null;
#endif


static /*HDR*/
/**
 * @brief Read function for devices with ISA or micro channel bus
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see @ref pcps_read_fncs
 */
int pcps_read_std( PCPS_DDEV *pddev, uint8_t cmd,
                   void FAR *buffer, uint16_t count )
{
  uint8_t FAR *p = (uint8_t FAR *) buffer;
  MBG_IOPORT_ADDR_MAPPED port = _pcps_ddev_io_base_mapped( pddev, 0 );
  int i;
  int rc;
  _pcps_irq_flags

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_cmd = 0;
  #endif

  #if DEBUG_ACCESS_TIMING
    MBG_PC_CYCLES t_after_reread = 0;
  #endif

  #if DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_busy = 0;
    MBG_PC_CYCLES t_done = 0;
  #endif

  #if DEBUG_IO
    report_io_cmd( cmd, count, "pcps_read_amcc_std" );
  #endif


  _pcps_disb_local_irq_save();

  // get current cycles and write the command byte
  mbg_get_pc_cycles( &pddev->acc_cycles );
  _mbg_outp8( pddev, 0, port, cmd );

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_cmd );
  #endif

  #if DEBUG_ACCESS_TIMING
    debug_dummy_var = _pcps_ddev_read_status_port( pddev );
    mbg_get_pc_cycles( &t_after_reread );
  #endif

  _pcps_local_irq_restore();


  // wait until BUSY flag goes low or timeout
  rc = pcps_wait_busy( pddev );

  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_busy );
  #endif

  if ( mbg_rc_is_error( rc ) )
    goto out;


  // success: read data, if required

  for ( i = 0; i < count; i++ )
  {
    *p = _mbg_inp8( pddev, 0, port );

    #if DEBUG_IO
      pcps_dump_data( p, sizeof( *p ), "pcps_read_std" );
    #endif

    p++;
  }


out:
  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_done );
    report_io_timing( pddev, "STD", cmd, count, t_after_cmd, t_after_busy, t_done );
  #endif

  #if DEBUG_ACCESS_TIMING
    report_access_timing( pddev, "STD wr/rd", t_after_cmd, t_after_reread );
  #endif

  #if defined( DEBUG )
    report_ret_val( rc, "pcps_read_std" );
  #endif

  return rc;

}  // pcps_read_std

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_std;
#endif



#if _PCPS_USE_PCI

static /*HDR*/
/**
 * @brief Read function for devices with AMCC S5933 PCI interface chip
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see @ref pcps_read_fncs
 */
int pcps_read_amcc_s5933( PCPS_DDEV *pddev, uint8_t cmd,
                          void FAR *buffer, uint16_t count )
{
  uint8_t FAR *p = (uint8_t FAR *) buffer;
  MBG_IOPORT_ADDR_MAPPED port = _pcps_ddev_io_base_mapped( pddev, 0 );
  int i;
  int rc;
  _pcps_irq_flags

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_cmd = 0;
  #endif

  #if DEBUG_ACCESS_TIMING
    MBG_PC_CYCLES t_after_reread = 0;
  #endif

  #if DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_busy = 0;
    MBG_PC_CYCLES t_done = 0;
  #endif

  #if DEBUG_IO
    report_io_cmd( cmd, count, "pcps_read_amcc_s5933" );
  #endif


  // reset inbound mailbox and FIFO status
  _mbg_outp8( pddev, 0, port + AMCC_OP_REG_MCSR + 3, 0x0C );

  // set FIFO
  _mbg_outp8( pddev, 0, port + AMCC_OP_REG_INTCSR + 3, 0x3C );


  _pcps_disb_local_irq_save();

  mbg_get_pc_cycles( &pddev->acc_cycles );
  // write the command byte
  _mbg_outp8( pddev, 0, port + AMCC_OP_REG_OMB1, cmd );
  #if defined( MBG_ARCH_SPARC )
    udelay( 3 );
  #endif

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_cmd );
  #endif

  #if DEBUG_ACCESS_TIMING
    debug_dummy_var = _pcps_ddev_read_status_port( pddev );
    mbg_get_pc_cycles( &t_after_reread );
  #endif

  _pcps_local_irq_restore();


  // wait until BUSY flag goes low or timeout
  rc = pcps_wait_busy( pddev );

  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_busy );
  #endif

  if ( mbg_rc_is_error( rc ) )
    goto out;


  // success: read data, if required

  for ( i = 0; i < count; i++ )
  {
    if ( _mbg_inp16_to_cpu( pddev, 0, port + AMCC_OP_REG_MCSR ) & 0x20 )
      return MBG_ERR_NO_DATA;

    p[i] = _mbg_inp8( pddev, 0, port + AMCC_OP_REG_FIFO + ( i % sizeof( uint32_t) ) );

    #if DEBUG_IO
      pcps_dump_data( &p[i], sizeof( p[i] ), "pcps_read_amcc_s5933" );
    #endif
  }


out:
  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_done );
    report_io_timing( pddev, "S5933", cmd, count, t_after_cmd, t_after_busy, t_done );
  #endif

  #if DEBUG_ACCESS_TIMING
    report_access_timing( pddev, "S5933 wr/rd", t_after_cmd, t_after_reread );
  #endif

  #if defined( DEBUG )
    report_ret_val( rc, "pcps_read_amcc_s5933" );
  #endif

  return rc;

}  // pcps_read_amcc_s5933

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_amcc_s5933;
#endif

#endif  /* _PCPS_USE_PCI */



#if _PCPS_USE_PCI

static /*HDR*/
/**
 * @brief Read function for devices with AMCC S5920 PCI interface chip
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see @ref pcps_read_fncs
 */
int pcps_read_amcc_s5920( PCPS_DDEV *pddev, uint8_t cmd,
                          void FAR *buffer, uint16_t count )
{
  uint8_t FAR *p = (uint8_t FAR *) buffer;
  MBG_IOPORT_ADDR_MAPPED data_port = _pcps_ddev_io_base_mapped( pddev, 1 );
  int i;
  int rc;
  int dt_quot;
  int dt_rem;
  _pcps_irq_flags

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_cmd = 0;
  #endif

  #if DEBUG_ACCESS_TIMING
    MBG_PC_CYCLES t_after_reread = 0;
  #endif

  #if DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_busy = 0;
    MBG_PC_CYCLES t_done = 0;
  #endif

  #if DEBUG_IO
    report_io_cmd( cmd, count, "pcps_read_amcc_s5920" );
  #endif


  _pcps_disb_local_irq_save();

  mbg_get_pc_cycles( &pddev->acc_cycles );
  // write the command byte
  _mbg_outp8( pddev, 0, _pcps_ddev_io_base_mapped( pddev, 0 ) + AMCC_OP_REG_OMB, cmd );
  #if defined( MBG_ARCH_SPARC )
    udelay( 3 );
  #endif

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_cmd );
  #endif

  #if DEBUG_ACCESS_TIMING
    debug_dummy_var = _pcps_ddev_read_status_port( pddev );
    mbg_get_pc_cycles( &t_after_reread );
  #endif

  _pcps_local_irq_restore();


  dt_quot = count / 4;
  dt_rem = count % 4;


  // wait until BUSY flag goes low or timeout
  rc = pcps_wait_busy( pddev );

  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_busy );
  #endif

  if ( mbg_rc_is_error( rc ) )
    goto out;


  // success: read data, if required

  if ( count )
  {
    // do this only if we must read data

    uint32_t ul;

    // first read full 32 bit words
    for ( i = 0; i < dt_quot; i++ )
    {
      ul = _mbg_inp32_native( pddev, 1, data_port );
      #if DEBUG_IO
        pcps_dump_data( &ul, sizeof( ul ), "pcps_read_amcc_s5920" );
      #endif
      _mbg_put_unaligned( ul, (uint32_t FAR *) p );
      p += sizeof( ul );
    }

    // then read the remaining bytes, if required
    if ( dt_rem )
    {
      ul = _mbg_inp32_native( pddev, 1, data_port );

      for ( i = 0; i < dt_rem; i++ )
      {
        #if DEBUG_IO
          pcps_dump_data( &ul, dt_rem, "pcps_read_amcc_s5920" );
        #endif

        *p++ = BYTE_OF( ul, i );
      }
    }
  }
  else
    (void) _mbg_inp32_native( pddev, 1, data_port );  // do a dummy read

out:
  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_done );
    report_io_timing( pddev, "S5920", cmd, count, t_after_cmd, t_after_busy, t_done );
  #endif

  #if DEBUG_ACCESS_TIMING
    report_access_timing( pddev, "S5920 wr/rd", t_after_cmd, t_after_reread );
  #endif

  #if defined( DEBUG )
    report_ret_val( rc, "pcps_read_amcc_s5920" );
  #endif

  return rc;

}  // pcps_read_amcc_s5920

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_amcc_s5920;
#endif

#endif  /* _PCPS_USE_PCI */



#if _PCPS_USE_PCI

static /*HDR*/
/**
 * @brief Read function for devices with Meinberg PCI interface ASIC
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see ::pcps_read_asic_mm
 * @see ::pcps_read_asic_mm16
 * @see @ref pcps_read_fncs
 */
int pcps_read_asic( PCPS_DDEV *pddev, uint8_t cmd,
                    void FAR *buffer, uint16_t count )
{
  uint8_t FAR *p = (uint8_t FAR *) buffer;
  MBG_IOPORT_ADDR_MAPPED data_port;
  PCI_ASIC_REG ar;
  int i;
  int rc;
  int dt_quot;
  int dt_rem;
  _pcps_irq_flags

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_cmd = 0;
  #endif

  #if DEBUG_ACCESS_TIMING
    MBG_PC_CYCLES t_after_reread = 0;
  #endif

  #if DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_busy = 0;
    MBG_PC_CYCLES t_done = 0;
  #endif

  #if DEBUG_IO
    report_io_cmd( cmd, count, "pcps_read_asic" );
  #endif


  _pcps_disb_local_irq_save();

  // get current cycles and write the command byte
  mbg_get_pc_cycles( &pddev->acc_cycles );
  _mbg_outp32_to_mbg( pddev, 0, _pcps_ddev_io_base_mapped( pddev, 0 )
                      + offsetof( PCI_ASIC, pci_data ), cmd );

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_cmd );
  #endif

  #if DEBUG_ACCESS_TIMING
    debug_dummy_var = _mbg_inp32_to_cpu( pddev, 0, _pcps_ddev_io_base_mapped( pddev, 0 )
                                         + offsetof( PCI_ASIC, addon_data ) );
    mbg_get_pc_cycles( &t_after_reread );
  #endif

  _pcps_local_irq_restore();


  data_port = _pcps_ddev_io_base_mapped( pddev, 0 )
                + offsetof( PCI_ASIC, addon_data );
  dt_quot = count / 4;
  dt_rem = count % 4;


  // wait until BUSY flag goes low or timeout
  rc = pcps_wait_busy( pddev );

  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_busy );
  #endif

  if ( mbg_rc_is_error( rc ) )
    goto out;


  // success: read data, if required

  // first read full 32 bit words
  for ( i = 0; i < dt_quot; i++ )
  {
    ar.ul = _mbg_inp32_native( pddev, 0, data_port );
    #if DEBUG_IO
      pcps_dump_data( &ar.ul, sizeof( ar.ul ), "pcps_read_asic" );
    #endif
    _mbg_put_unaligned( ar.ul, (uint32_t FAR *) p );
    p += sizeof( ar.ul );
    data_port += sizeof( ar.ul );
  }

  // then read the remaining bytes, if required
  if ( dt_rem )
  {
    ar.ul = _mbg_inp32_native( pddev, 0, data_port );

    for ( i = 0; i < dt_rem; i++ )
    {
      #if DEBUG_IO
        pcps_dump_data( &ar.b[i], sizeof( ar.b[i] ), "pcps_read_asic" );
      #endif

      *p++ = ar.b[i];
    }
  }

out:
  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_done );
    report_io_timing( pddev, "ASIC", cmd, count, t_after_cmd, t_after_busy, t_done );
  #endif

  #if DEBUG_ACCESS_TIMING
    report_access_timing( pddev, "ASIC wr/rd", t_after_cmd, t_after_reread );
  #endif

  #if defined( DEBUG )
    report_ret_val( rc, "pcps_read_asic" );
  #endif

  return rc;

}  // pcps_read_asic

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_asic;
#endif


#if _PCPS_USE_MM_IO

static /*HDR*/
/**
 * @brief Read function for devices with Meinberg PCI interface ASIC
 *
 * Unlike ::pcps_read_asic and most of the other read functions,
 * this function accesses the memory mapped registers rather than
 * the I/O ports, so this is significantly faster.
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see ::pcps_read_asic
 * @see @ref pcps_read_fncs
 */
int pcps_read_asic_mm( PCPS_DDEV *pddev, uint8_t cmd,
                       void FAR *buffer, uint16_t count )
{
  uint8_t FAR *p = (uint8_t FAR *) buffer;
  uint32_t _MBG_IOMEM *p_data_reg;
  PCI_ASIC_REG ar;
  int i;
  int rc;
  int dt_quot;
  int dt_rem;
  _pcps_irq_flags

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_cmd = 0;
  #endif

  #if DEBUG_ACCESS_TIMING
    MBG_PC_CYCLES t_after_reread = 0;
  #endif

  #if DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_busy = 0;
    MBG_PC_CYCLES t_done = 0;
  #endif

  #if DEBUG_IO
    report_io_cmd( cmd, count, "pcps_read_asic_mm" );
  #endif

  #if defined( MBG_ARCH_SPARC )
    // Actually there are problems on SPARC ...
    return MBG_ERR_TIMEOUT;  // FIXME TODO
  #endif

  _pcps_disb_local_irq_save();

  // get current cycles and write the command byte
  #if USE_CMD_PTR
  {
    uint32_t _MBG_IOMEM *p_cmd_reg = &pddev->mm_asic_addr->pci_data.ul;
    mbg_get_pc_cycles( &pddev->acc_cycles );
    _mbg_mmwr32_to_mbg( p_cmd_reg, cmd );
  }
  #else
    mbg_get_pc_cycles( &pddev->acc_cycles );
    _mbg_mmwr32( &pddev->mm_asic_addr->pci_data.ul, cmd );
  #endif

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_cmd );
  #endif

  #if DEBUG_ACCESS_TIMING
    debug_dummy_var = _mbg_mmrd32_native( &pddev->mm_asic_addr->pci_data.ul );
    mbg_get_pc_cycles( &t_after_reread );
  #endif

  _pcps_local_irq_restore();


  p_data_reg = &pddev->mm_asic_addr->addon_data.ul[0];
  dt_quot = count / 4;
  dt_rem = count % 4;


  // wait until BUSY flag goes low or timeout
  rc = pcps_wait_busy( pddev );

  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_busy );
  #endif

  if ( mbg_rc_is_error( rc ) )
    goto out;


  // success: read data, if required

  // first read full 32 bit words
  for ( i = 0; i < dt_quot; i++ )
  {
    ar.ul = _mbg_mmrd32_native( p_data_reg );
    #if DEBUG_IO
      pcps_dump_data( &ar.ul, sizeof( ar.ul ), "pcps_read_asic_mm" );
    #endif
    _mbg_put_unaligned( ar.ul, (uint32_t FAR *) p );
    p += sizeof( ar.ul );
    p_data_reg++;
  }

  // then read the remaining bytes, if required
  if ( dt_rem )
  {
    ar.ul = _mbg_mmrd32_native( p_data_reg );

    for ( i = 0; i < dt_rem; i++ )
    {
      #if DEBUG_IO
        pcps_dump_data( &ar.b[i], sizeof( ar.b[i] ), "pcps_read_asic_mm" );
      #endif

      *p++ = ar.b[i];
    }
  }

out:
  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_done );
    report_io_timing( pddev, "ASIC MM", cmd, count, t_after_cmd, t_after_busy, t_done );
  #endif

  #if DEBUG_ACCESS_TIMING
    report_access_timing( pddev, "ASIC MM wr/rd", t_after_cmd, t_after_reread );
  #endif

  #if defined( DEBUG )
    report_ret_val( rc, "pcps_read_asic_mm" );
  #endif

  return rc;

}  // pcps_read_asic_mm

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_asic_mm;
#endif



static /*HDR*/
/**
 * @brief Read function for devices with Meinberg PCI interface ASIC
 *
 * Unlike ::pcps_read_asic_mm this function does 16 bit memory
 * mapped register access.
 * this function accesses the memory mapped registers rather than
 * the I/O ports, so this is significantly faster.
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see ::pcps_read_asic
 * @see @ref pcps_read_fncs
 */
int pcps_read_asic_mm16( PCPS_DDEV *pddev, uint8_t cmd,
                         void FAR *buffer, uint16_t count )
{
  uint8_t FAR *p = (uint8_t FAR *) buffer;
  uint16_t _MBG_IOMEM *p_data_reg;
  PCI_ASIC_REG ar;
  int i;
  int rc;
  int dt_quot;
  int dt_rem;
  _pcps_irq_flags

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_cmd = 0;
  #endif

  #if DEBUG_ACCESS_TIMING
    MBG_PC_CYCLES t_after_reread = 0;
  #endif

  #if DEBUG_IO_TIMING
    MBG_PC_CYCLES t_after_busy = 0;
    MBG_PC_CYCLES t_done = 0;
  #endif

  #if DEBUG_IO
    report_io_cmd( cmd, count, "pcps_read_asic_mm16" );
  #endif


  _pcps_disb_local_irq_save();

  // get current cycles and write the command byte
  #if USE_CMD_PTR
  {
    uint32_t _MBG_IOMEM *p_cmd_reg = &pddev->mm_asic_addr->pci_data.ul;
    mbg_get_pc_cycles( &pddev->acc_cycles );
    _mbg_mmwr32_to_mbg( p_cmd_reg, cmd );
  }
  #else
    mbg_get_pc_cycles( &pddev->acc_cycles );
    _mbg_mmwr32_mbg( &pddev->mm_asic_addr->pci_data.ul, cmd );
  #endif

  #if DEBUG_ACCESS_TIMING || DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_cmd );
  #endif

  #if DEBUG_ACCESS_TIMING
    debug_dummy_var = _mbg_mmrd32_native( &pddev->mm_asic_addr->pci_data.ul );
    mbg_get_pc_cycles( &t_after_reread );
  #endif

  _pcps_local_irq_restore();


  p_data_reg = &pddev->mm_asic_addr->addon_data.us[0];
  dt_quot = count / 2;
  dt_rem = count % 2;


  // wait until BUSY flag goes low or timeout
  rc = pcps_wait_busy( pddev );

  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_after_busy );
  #endif

  if ( mbg_rc_is_error( rc ) )
    goto out;


  // success: read data, if required

  // first read full 32 bit words
  for ( i = 0; i < dt_quot; i++ )
  {
    ar.us[0] = _mbg_mmrd16_native( p_data_reg );
    #if DEBUG_IO
      pcps_dump_data( &ar.us[0], sizeof( ar.us[0] ), "pcps_read_asic_mm16" );
    #endif
    _mbg_put_unaligned( ar.us[0], (uint16_t FAR *) p );
    p += sizeof( ar.us[0] );
    p_data_reg++;
  }

  // then read the remaining bytes, if required
  if ( dt_rem )
  {
    ar.us[0] = _mbg_mmrd16_native( p_data_reg );

    for ( i = 0; i < dt_rem; i++ )
    {
      #if DEBUG_IO
        pcps_dump_data( &ar.b[i], sizeof( ar.b[i] ), "pcps_read_asic_mm16" );
      #endif

      *p++ = ar.b[i];
    }
  }

out:
  #if DEBUG_IO_TIMING
    mbg_get_pc_cycles( &t_done );
    report_io_timing( pddev, "ASIC MM ww", cmd, count, t_after_cmd, t_after_busy, t_done );
  #endif

  #if DEBUG_ACCESS_TIMING
    report_access_timing( pddev, "ASIC MM ww wr/rd", t_after_cmd, t_after_reread );
  #endif

  #if defined( DEBUG )
    report_ret_val( rc, "pcps_read_asic_mm16" );
  #endif

  return rc;

}  // pcps_read_asic_mm16

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_asic_mm16;
#endif

#endif  // _PCPS_USE_MM_IO

#endif  // _PCPS_USE_PCI



#if _PCPS_USE_USB

static /*HDR*/
/**
 * @brief Read function for USB devices
 *
 * @param[in]  pddev   Pointer to the device structure
 * @param[in]  cmd     The command code for the board, see @ref PCPS_CMD_CODES
 * @param[out] buffer  A buffer to take the bytes to be read
 * @param[in]  count   The number of bytes to be read into the buffer
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @ingroup pcps_read_fncs
 * @see @ref pcps_read_fncs
 */
int pcps_read_usb( PCPS_DDEV *pddev, uint8_t cmd,
                   void FAR *buffer, uint16_t count )
{
  int rc;

  #if DEBUG_IO
    report_io_cmd( cmd, count, __func__ );
  #endif

  rc = pcps_read_usb_generic( pddev, cmd, buffer, count, false );

  return rc;

}  // pcps_read_usb

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_usb;
#endif

#endif  // _PCPS_USE_USB

/** @} defgroup group_pcps_read_fnc */



/*HDR*/
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
int pcps_write( PCPS_DDEV *pddev, uint8_t cmd,
                const void FAR *buffer, uint16_t count )
{
  int rc;

#if _PCPS_USE_USB
  if ( _pcps_ddev_is_usb( pddev ) )
    rc = pcps_write_usb_generic( pddev, cmd, buffer, count, false );
  else
#endif // _PCPS_USE_USB
  {
    const uint8_t FAR *p = (const uint8_t FAR *) buffer;
    int i;
    uint8_t bytes_expected;
    int8_t write_rc;

    // Write the command and read one byte which will contain
    // the number of data bytes that must follow.
    rc = _pcps_read_var( pddev, cmd, bytes_expected );

    #if DEBUG_IO
      _mbg_kdd_msg_4( MBG_LOG_DEBUG, "pcps_write: cmd %02X, %u bytes, expects %u, rc: %i",
                      cmd, count, bytes_expected, rc );
    #endif

    if ( mbg_rc_is_error( rc ) )  // TODO REPORT ?
      goto out;


    // Check if the number of data bytes to be written is correct.
    if ( bytes_expected != count )
    {
      #if REPORT_IO_ERRORS
        _mbg_kdd_msg_3( MBG_LOG_ERR, "%s: rcvd. %d != exp. %d",  // REPORT_IO_ERRORS
                        __func__, rc, count );
      #endif

      rc = MBG_ERR_NBYTES;
      goto out;
    }


    // Write all bytes but the last one without reading anything.
    bytes_expected--;

    for ( i = 0; i < bytes_expected; i++ )
    {
      #if DEBUG_IO
        _mbg_kdd_msg_2( MBG_LOG_DEBUG, "pcps_write: byte %i: 0x%02X", i, *p );
      #endif

      rc = _pcps_write_byte( pddev, *p++ );

      if ( mbg_rc_is_error( rc ) )
        goto out;
    }

    // Write the last byte and read the completion code.
    #if DEBUG_IO
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, "pcps_write: last byte %i: 0x%02X", i, *p );
    #endif

    rc = _pcps_read_var( pddev, *p++, write_rc );

    if ( mbg_rc_is_success( rc ) )  // write operation was successfull
      rc = write_rc;                // return the completion code from the device
  }

out:
  #if defined( DEBUG )
    report_ret_val( rc, "pcps_write" );
  #endif

  return rc;

}  // pcps_write

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_WRITE_FNC pcps_write;
#endif



/*HDR*/
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
int pcps_generic_io( PCPS_DDEV *pddev, uint8_t type,
                     const void FAR *in_buff, uint8_t in_cnt,
                     void FAR *out_buff, uint8_t out_cnt )
{
  const uint8_t FAR *p;
  int i;
  int rc;
  uint8_t tmp_byte;
  int8_t data_read[PCPS_FIFO_SIZE];
  uint8_t bytes_to_read;

  #if DEBUG_IO
    #if defined( MBG_TGT_DOS )
      #define FP_FMT  "%Fp"
    #else
      #define FP_FMT  "%p"
    #endif
    _mbg_kdd_msg_5( MBG_LOG_DEBUG, "pcps_generic_io: type 0x%02X, in_buff: " FP_FMT
                    " (%u), out_buf " FP_FMT " (%u)",
                    type, in_buff, in_cnt, out_buff, out_cnt );
  #endif

  // Write the command and read one byte which will contain
  // the number of data bytes that must follow.
  rc = _pcps_read_var( pddev, PCPS_GENERIC_IO, tmp_byte );

  if ( mbg_rc_is_error( rc ) )  // TODO REPORT ?
    return rc;


  // Check if the number of data bytes to be written is correct.
  if ( tmp_byte != 3 )
  {
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_1( MBG_LOG_ERR, "pcps_generic_io: device expects %u bytes rather than 3",  // REPORT_IO_ERRORS
                      tmp_byte );
    #endif

    return MBG_ERR_NBYTES;
  }


  #if DEBUG_IO
    _mbg_kdd_msg_3( MBG_LOG_DEBUG, "pcps_generic_io: going to write type 0x%02X, in_sz %i, out_sz %i",
                    type, in_cnt, out_cnt );
  #endif

  // Write the 3 bytes which are expected:
  rc = _pcps_write_byte( pddev, type );

  if ( mbg_rc_is_error( rc ) )  // TODO REPORT ?
    goto out;


  rc = _pcps_write_byte( pddev, in_cnt );

  if ( mbg_rc_is_error( rc ) )  // TODO REPORT ?
    goto out;


  if ( in_cnt == 0 )
    tmp_byte = out_cnt;
  else
  {
    rc = _pcps_write_byte( pddev, out_cnt );

    if ( mbg_rc_is_error( rc ) )  // TODO REPORT ?
      goto out;


    // Write the input parameters
    #if DEBUG_IO
      _mbg_kdd_msg_0( MBG_LOG_DEBUG, "pcps_generic_io: going to write input bytes" );
    #endif

    p = (const uint8_t FAR *) in_buff;
    tmp_byte = in_cnt - 1;

    for ( i = 0; i < tmp_byte; i++ )
    {
      rc = _pcps_write_byte( pddev, *p++ );

      if ( mbg_rc_is_error( rc ) )  // TODO REPORT ?
        goto out;
    }

    tmp_byte = *p;
  }


  bytes_to_read = 2 + out_cnt;

  if ( bytes_to_read > sizeof( data_read ) )
    bytes_to_read = sizeof( data_read );


  // Write the last byte and read the completion code.
  rc = _pcps_read( pddev, tmp_byte, data_read, bytes_to_read );

  if ( out_cnt )   //### TODO should do some more plausibility checks
    if ( mbg_rc_is_success( rc ) )
    {
      _fmemcpy( out_buff, &data_read[2], out_cnt );
    }

out:
  // If an error code has been returned by the I/O function,
  // return that code, otherwise return the completion code
  // read from the board.
  if ( mbg_rc_is_success( rc ) )
    rc = data_read[0];

  #if defined( DEBUG )
    report_ret_val( rc, "pcps_generic_io" );
  #endif

  return rc;

}  // pcps_generic_io



static /*HDR*/
/**
 * @brief Determine which interface buffer size is supported
 *
 * @param[in]  pddev  Pointer to the device structure
 * @param[in]  count  The number of bytes to be read according to the data_type
 * @param[in]  info   An info string, e.g. the name of the calling function
 *
 * @return ::MBG_SUCCESS on success, or ::MBG_ERR_NBYTES if the actual buffer size exceeds the supported size
 */
int pcps_check_gps_data_size( PCPS_DDEV *pddev, uint16_t count,
                              const char *info )
{
  if ( _pcps_ddev_has_gps_data_16( pddev ) )
  {
    pddev->size_n_bytes = 2;  // 16 bit data size is supported.
    goto out_success;
  }

  // Only 8 bit data size is supported.
  pddev->size_n_bytes = 1;  // 16 bit data size is not supported.


  // This can only be used with small buffers.
  if ( count > 255 )   // buffer too large
  {
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_1( MBG_LOG_ERR,
         "%s: buffer size > 255, but gps_data_16 not supp.", info );  // REPORT_IO_ERRORS
    #else
      (void) info;
    #endif

    return MBG_ERR_NBYTES;
  }

out_success:
  return MBG_SUCCESS;

}  // pcps_check_gps_data_size



static /*HDR*/
/**
 * @brief Initialize GPS data transfer
 *
 * @param[in]  pddev      Pointer to the device structure
 * @param[in]  cmd        ::PCPS_READ_GPS_DATA or ::PCPS_WRITE_GPS_DATA
 * @param[in]  data_type  The code assigned to the data type, see @ref PC_GPS_CMD_CODES
 * @param[in]  count      The number of bytes to be read according to the data_type
 * @param[in]  info       An info string, e.g. the name of the calling function
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
int pcps_init_gps_transfer( PCPS_DDEV *pddev, uint8_t cmd, uint8_t data_type,
                            uint16_t count, const char *info )
{
  #define FNC_ID_GPS_INIT "GPS init"

  // Write the read or write cmd code, expect to read one byte.
  int rc = _pcps_read_var( pddev, cmd, pddev->uc );

  if ( mbg_rc_is_error( rc ) )
  {
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_2( MBG_LOG_ERR,
         FNC_ID_GPS_INIT " %s: failed rd 1 byte after cmd code %02X",  // REPORT_IO_ERRORS
         info, cmd );
    #else
      (void) info;
    #endif

    goto out;
  }


  if ( pddev->uc != 1 ) // The board doesn't expect exactly one more byte
  {
    if ( pddev->uc == 0 )
    {
      // The device is unable to respond now. This may happen
      // if it is still initializing after power-up.
      #if REPORT_IO_ERRORS
        _mbg_kdd_msg_1( MBG_LOG_WARN, FNC_ID_GPS_INIT " %s: dev. not init'", info );  // REPORT_IO_ERRORS
      #endif

      rc = MBG_ERR_NOT_READY;
      goto out;
    }


    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_GPS_INIT " %s: dev. exp. %u bytes, not 1",  // REPORT_IO_ERRORS
                      info, pddev->uc );
    #endif

    rc = MBG_ERR_NBYTES;
    goto out;
  }


  // Write the code corresponding to the type of data we
  // want to read, expect to read the data size expected
  // by the device.
  pddev->n_bytes = 0;
  rc = _pcps_read( pddev, data_type, &pddev->n_bytes, pddev->size_n_bytes );

  if ( mbg_rc_is_error( rc ) )
  {
    #if REPORT_IO_ERRORS
      // Device not yet initialized.
      _mbg_kdd_msg_1( MBG_LOG_ERR, FNC_ID_GPS_INIT " %s: dev. not init'd", info );  // REPORT_IO_ERRORS
    #endif

    goto out;
  }


  #if defined( MBG_ARCH_BIG_ENDIAN )
    // Swap n_bytes regardless of whether we have actually read 1 or 2 bytes.
    // If we have read only 1 byte then the other one is 0 anyway.
    pddev->n_bytes = _mbg16_to_cpu( pddev->n_bytes );
  #endif

  if ( pddev->n_bytes == 0 )
  {
    rc = MBG_ERR_INV_TYPE;

    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_2( MBG_LOG_ERR, FNC_ID_GPS_INIT " %s: data type %u not supp.",  // REPORT_IO_ERRORS
                      info, data_type);
    #endif

    goto out;
  }


  if ( pddev->n_bytes != count )  // Size of data structure does not match.
  {
    #if REPORT_IO_ERRORS
      _mbg_kdd_msg_3( MBG_LOG_ERR, FNC_ID_GPS_INIT " %s: buf. size %u != %u",  // REPORT_IO_ERRORS
                      info, pddev->n_bytes, count );
    #endif

    rc = MBG_ERR_NBYTES;
    goto out;
  }


  #if DEBUG_IO
    _mbg_kdd_msg_3( MBG_LOG_DEBUG, FNC_ID_GPS_INIT " %s: exp. size %u == buf. size %u",
                    info, pddev->n_bytes, count );
  #endif

  #undef FNC_ID_GPS_INIT

out:
  return rc;

}  // pcps_init_gps_transfer



static /*HDR*/
/**
 * @brief Get a block of data from a GPS device
 *
 * This static function is used by ::pcps_read_gps.
 *
 * @param[in]  pddev       Pointer to the device structure
 * @param[in]  data_type   The code assigned to the data type, see @ref PC_GPS_CMD_CODES
 * @param[out] buffer      A buffer with data to be read according to the data_type
 * @param[in]  count       The number of bytes to be read according to the data_type
 * @param[in]  block_num   A buffer with data to be written according to the type code
 * @param[in]  block_size  The number of bytes to be written according to the type code
 *
 * @return ::MBG_SUCCESS on success,
 *         ::MBG_ERR_TIMEOUT if device didn't respond in time,
 *         ::MBG_ERR_INV_TYPE if the device didn_t recognize the data type,
 *         ::MBG_ERR_NBYTES if the number of parameter bytes did not match
 *                          the number of data bytes expected by the device,
 *         or one of the other @ref MBG_RETURN_CODES
 */
int pcps_read_gps_block( PCPS_DDEV *pddev,
                         uint8_t data_type,
                         void FAR *buffer,
                         uint16_t count,
                         uint8_t block_num,
                         uint8_t block_size )
{
  #define FNC_ID_GPS_READ_BLK "GPS rd blk"

  int rc = pcps_check_gps_data_size( pddev, count, FNC_ID_GPS_READ_BLK );

  if ( mbg_rc_is_error( rc ) )
    goto out;

  #if DEBUG_IO
    _mbg_kdd_msg_4( MBG_LOG_DEBUG,
       FNC_ID_GPS_READ_BLK ": cmd 0x%02X, block %u (%u), size_n_bytes = %u",
       data_type, block_num, block_size, pddev->size_n_bytes );
  #endif

  rc = pcps_init_gps_transfer( pddev, PCPS_READ_GPS_DATA, data_type, count, "rd" );

  if ( mbg_rc_is_error( rc ) )
    goto out;


  // Write the block number and read n bytes of data.
  rc = _pcps_read( pddev, block_num, buffer, block_size );

out:
  #if defined( DEBUG )
    report_ret_val( rc, FNC_ID_GPS_READ_BLK );
  #endif

  #undef FNC_ID_GPS_READ_BLK

  return rc;

}  // pcps_read_gps_block



/*HDR*/
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
int pcps_read_gps( PCPS_DDEV *pddev,
                   uint8_t data_type,
                   void FAR *buffer,
                   uint16_t count )
{
  uint8_t FAR *p;
  int dt_quot;
  int dt_rem;
  int block_num;
  int rc;

  #define FNC_ID_GPS_READ "GPS rd"

  #if DEBUG_IO
    _mbg_kdd_msg_3( MBG_LOG_DEBUG, FNC_ID_MARKER FNC_ID_GPS_READ ": data type: 0x%02X, addr: %p, size: %u",
                    data_type, buffer, count );
  #endif

  #if _PCPS_USE_USB
    if ( _pcps_ddev_is_usb( pddev ) )
    {
      rc = pcps_read_usb_generic( pddev, data_type, buffer, count, true );
      goto out;
    }
  #endif  // _PCPS_USE_USB

  p = (uint8_t FAR *) buffer;
  rc = MBG_SUCCESS;

  // Split buffer size to a number of blocks of PCPS_FIFO_SIZE
  // and a number of remaining bytes (less than PCPS_FIFO_SIZE).
  dt_quot = count / PCPS_FIFO_SIZE;
  dt_rem = count % PCPS_FIFO_SIZE;

  // Read dt_quot full blocks of data.
  for ( block_num = 0; block_num < dt_quot; block_num++ )
  {
    rc = pcps_read_gps_block( pddev, data_type, p, count,
                              (uint8_t) block_num, PCPS_FIFO_SIZE );

    if ( mbg_rc_is_error( rc ) )
      goto out;

    // Move the destination pointer to the next free byte.
    p += PCPS_FIFO_SIZE;
  }


  // Read dt_rem additional bytes of data.
  if ( dt_rem )
    rc = pcps_read_gps_block( pddev, data_type, p, count,
                              (uint8_t) block_num, (uint8_t) dt_rem );

out:
  #if defined( DEBUG )
    report_ret_val( rc, FNC_ID_GPS_READ );
  #endif

  #undef FNC_ID_GPS_READ

  return rc;

}  // pcps_read_gps

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_READ_FNC pcps_read_gps;
#endif



/*HDR*/
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
int pcps_write_gps( PCPS_DDEV *pddev,
                    uint8_t data_type,
                    const void FAR *buffer,
                    uint16_t count )
{
  const uint8_t FAR *p;
  int i;
  int rc;

  #define FNC_ID_GPS_WRITE "GPS wr"

  #if DEBUG_IO
    _mbg_kdd_msg_3( MBG_LOG_DEBUG, FNC_ID_MARKER FNC_ID_GPS_WRITE ": data type: 0x%02X, addr: %p, size: %u",
                    data_type, buffer, count );
  #endif

  #if _PCPS_USE_USB
    if ( _pcps_ddev_is_usb( pddev ) )
    {
      rc = pcps_write_usb_generic( pddev, data_type, buffer, count, true );
      goto out;
    }
  #endif  // _PCPS_USE_USB

  p = (const uint8_t FAR *) buffer;

  rc = pcps_check_gps_data_size( pddev, count, FNC_ID_GPS_WRITE );

  if ( mbg_rc_is_error( rc ) )
    goto out;


  #if DEBUG_IO
    _mbg_kdd_msg_1( MBG_LOG_DEBUG, FNC_ID_GPS_WRITE ": size_n_bytes = %u", pddev->size_n_bytes );
  #endif

  rc = pcps_init_gps_transfer( pddev, PCPS_WRITE_GPS_DATA, data_type, count, "wr" );

  if ( mbg_rc_is_error( rc ) )
    goto out;

  if ( count == 0 )  // done
    goto out;


  // Write all bytes but the last one without reading.
  count--;

  for ( i = 0; i < count; i++ )
  {
    rc = _pcps_write_byte( pddev, *p++ );

    if ( mbg_rc_is_error( rc ) )  // TODO REPORT ?
      goto out;
  }


  // Write the last byte and read the completion code.
  rc = _pcps_read_var( pddev, *p, pddev->n_bytes );

  #if DEBUG_IO
    _mbg_kdd_msg_2( MBG_LOG_DEBUG, FNC_ID_GPS_WRITE ": wrote last byte, rc: %i, completion code: %u",
                    rc, pddev->n_bytes );
  #endif

  // If an error code has been returned by read_fnc, return that
  // code, otherwise return the completion code read from the board.
  if ( mbg_rc_is_success( rc ) )  // TODO else REPORT error?
    rc = pddev->n_bytes;

out:
  #if defined( DEBUG )
    report_ret_val( rc, FNC_ID_GPS_WRITE );
  #endif

  #undef FNC_ID_GPS_WRITE

  return rc;

}  // pcps_write_gps

#if !defined( AVOID_REDUNDANT_REDECLARATION )
  PCPS_WRITE_FNC pcps_write_gps;
#endif



static /*HDR*/
/**
 * @brief Read the firmware ID from a device
 *
 * This is usually done first when probing a device to check if the
 * device is properly installed and can be accessed without problems.
 *
 * @param[in]  pddev  Pointer to a device structure
 * @param[out] fw_id  A buffer to be filled withtthe firmware ID
 *
 * @return ::MBG_SUCCESS if the the signature was found, or signature was specified, or
 *         ::MBG_ERR_FW_ID if the firmware ID is unknown or invalid
 */
int pcps_get_fw_id( PCPS_DDEV *pddev, PCPS_ID_STR FAR fw_id )
{
  int rc;

  #if USE_LOCAL_IO_BUFFER
    PCPS_ID_STR tmp_fw_id = { 0 };
    char *cp = tmp_fw_id;
  #else
    char *cp = pddev->io_buffer.fw_id;
  #endif

  _mbgddmsg_1( DEBUG_DEV_INIT, MBG_LOG_INFO, "Going to read %s firmware ID",
               _pcps_ddev_type_name( pddev ) );

  #if !USE_LOCAL_IO_BUFFER
    memset( pddev->io_buffer.fw_id, 0, sizeof( pddev->io_buffer.fw_id ) );  // sizeof( fw_id ) would yield pointer size only
  #endif

  // read the first part of the firmware ID
  rc = _pcps_read( pddev, PCPS_GIVE_FW_ID_1, &cp[0], PCPS_FIFO_SIZE );

  if ( mbg_rc_is_error( rc ) )
    goto out;

  // read the second part of the firmware ID
  rc = _pcps_read( pddev, PCPS_GIVE_FW_ID_2, &cp[PCPS_FIFO_SIZE], PCPS_FIFO_SIZE );

  // force termination with 0
  cp[PCPS_ID_SIZE - 1] = 0;   // FIXME is this correct ?

  _mbgddmsg_2( DEBUG_DEV_INIT, MBG_LOG_INFO, "%s firmware ID: \"%s\"",
               _pcps_ddev_type_name( pddev ), cp );

out:
  // TODO Should we remove trailing spaces from the firmware ID of some old cards?
  memcpy( fw_id, cp, sizeof( PCPS_ID_STR ) );

  return rc;

}  // pcps_get_fw_id



static /*HDR*/
/**
 * @brief Check an ASCIIZ string for a valid signature.
 *
 * This function is only used to determine the type of a legacy ISA card.
 * For PCI and USB devices the type can be determined from the device ID.
 *
 * @param[in] pddev  Pointer to a device structure
 * @param[in] ref    The reference signature, i.e. the expected signature
 *                   at the beginning of the device's firmware ID
 *
 * @return ::MBG_SUCCESS if a signature was specified and the signature was found, or
 *         ::MBG_ERR_FW_ID if the firmware ID is unknown or invalid
 */
int pcps_check_id( PCPS_DDEV *pddev, const char FAR *ref )
{
  // check if the first characters of the string match the reference

  if ( ref )
    if ( _fstrncmp( _pcps_ddev_fw_id( pddev ), ref, _fstrlen( ref ) ) )
      return MBG_ERR_FW_ID;

  return MBG_SUCCESS;

}  // pcps_check_id



static /*HDR*/
/**
 * @brief Retrieve a version number from a firmware ID string
 *
 * @param[in] idstr  The ID string, e.g. a ::PCPS_ID_STR
 *
 * @return A version number in hex format, e.g. 0x0270 for version 2.7,
 *         or 0 if no version number could be found.
 */
PCPS_FW_REV_NUM pcps_get_rev_num( char FAR *idstr )
{
  int len = _fstrlen( idstr );
  int i;

  if ( len < 2 )
    goto no_rev_num;

  len -= 2;

  for ( i = 0; i < len; i++ )
  {
    if ( idstr[i + 1] == '.' )
    {
      uint8_t rev_num_hi = idstr[i] & 0x0F;
      uint8_t rev_num_lo = ( idstr[i + 2] & 0x0F ) << 4;

      // If the minor part of the version string has 2 digits
      // then evaluate the 2nd digit, too.

      char c = idstr[i + 3];

      if ( c >= '0' && c <= '9' )
        rev_num_lo |= c & 0x0F;

      return ( rev_num_hi << 8 ) | rev_num_lo;
    }
  }

no_rev_num:
  return 0;

}  // pcps_get_rev_num



static /*HDR*/
/**
 * @brief Remove unprintable characters from a string.
 *
 * @param[in,out] s        Pointer to the string.
 * @param[in]     max_len  Size of the string buffer.
 *
 * @return true if any unprintable characters have been removed, else false
 */
bool str_remove_unprintables( char *s, size_t max_len )
{
  size_t i;
  char *cp;
  bool removed = false;

  for ( i = 0, cp = s; i < max_len; i++, cp++ )
  {
    if ( ( *cp < 0x20 ) || ( *cp >= 0x7F ) )
    {
      if ( *cp )    // not 0
      {
        #if REPORT_CFG
          *cp = '#';
        #else
          *cp = 0;
        #endif

        removed = true;
      }

      if ( *cp == 0 )
        break;
    }
  }

  return removed;

}  // str_remove_unprintables



static /*HDR*/
void beautify_sernum( char *s, size_t max_len )
{
  size_t i;

  str_remove_unprintables( s, max_len );

  // Remove trailing ' ' or 'F' characters which may
  // unfortunately be returned by some devices.

  for ( i = max_len; ; )
  {
    char *cp;

    if ( i == 0 )
      break;

    --i;
    cp = &s[i];

    if ( ( *cp > ' ' ) && ( *cp != 'F' ) )
      break;  // done

    *cp = 0;
  }

#if 0  // FIXME TODO

  // Specific for old IDENT:
  // Some old devices may have non-digits appended
  // to the S/N string. Truncate such a string.
  for ( cp = _pcps_ddev_sernum( pddev ); *cp; cp++ )
    if ( ( *cp < '0' ) || ( *cp > '9' ) )
    {
      *cp = 0;
      break;
    }
#endif

}  // beautify_sernum



static /*HDR*/
void check_unknown_sernum( char *s, size_t max_len )
{
  if ( max_len && ( s[0] == 0 ) )  // string is empty
  {
    size_t unkn_len = 8;  // Number of '?' characters to write if unknown

    if ( max_len > unkn_len )
      max_len = unkn_len;

    memset( s, '?', max_len );
    s[max_len] = 0;
  }

}  // check_unknown_sernum



#if MUST_REPORT_PROBE_DEVICE_DETAILS

static /*HDR*/
const char *get_bus_type_str( const PCPS_DDEV *pddev )
{
  switch ( _pcps_ddev_bus_flags( pddev ) )
  {
    case PCPS_BUS_USB_V2:  return "USB v2";
    case PCPS_BUS_USB:     return "USB v1";

    case PCPS_BUS_PCI_MBGPEX:   return "Meinberg FPGA";
    case PCPS_BUS_PCI_PEX8311:  return "PLX PEX8311";
    case PCPS_BUS_PCI_ASIC:     return "PCI ASIC";
    case PCPS_BUS_PCI_S5920:    return "AMCC S5920";
    case PCPS_BUS_PCI_S5933:    return "AMCC S5933";

    case PCPS_BUS_MCA:  return "Microchannel";
    case PCPS_BUS_ISA:  return "ISA";
  }

  return str_unkn_braced;

}  // get_bus_type_str



static /*HDR*/
void report_probe_device( const PCPS_DDEV *pddev )
{
  char ws[40];

  if ( _pcps_ddev_is_pci( pddev ) )
    mbg_kdd_snprintf( ws, sizeof( ws ), "%02i:%02i.%02i",
                      _pcps_ddev_bus_num( pddev ),
                      _pcps_ddev_pci_slot_num( pddev ),
                      _pcps_ddev_pci_fnc_num( pddev ) );
  else
    mbg_kdd_snprintf( ws, sizeof( ws ), "%03i:%03i",
                      _pcps_ddev_bus_num( pddev ),
                      _pcps_ddev_slot_num( pddev ) );

  _mbg_kdd_msg_4( MBG_LOG_INFO, "Probing device %s, ID 0x%04X: %s, bus %s",
                  _pcps_ddev_type_name( pddev ), _pcps_ddev_dev_id( pddev ),
                  get_bus_type_str( pddev ), ws );

}  // report_probe_device

#endif  // MUST_REPORT_PROBE_DEVICE_DETAILS



#if REPORT_CFG

static /*HDR*/
/**
 * @brief Get the name assigned to one of the @ref PCPS_FEATURE_MASKS flags
 *
 * @param[in]  flag_mask  one of the @ref PCPS_FEATURE_MASKS flags
 *
 * @return The associated feature name, or "unknown"
 *
 * @see @ref PCPS_FEATURE_MASKS
 */
const char *get_pcps_feature_name( PCPS_FEATURES flag_mask )
{
  int i;

  for ( i = 0; i < N_PCPS_FEATURE_BITS; i++ )
    if ( ( 1UL << i ) == flag_mask )
      return pcps_feature_names[i];

  return str_unkn_braced;

}  // get_pcps_feature_name

#endif  // REPORT_CFG



#if REPORT_CFG_DETAILS

static /*HDR*/
void dump_receiver_info( const PCPS_DDEV *pddev )
{
  const RECEIVER_INFO *p_ri = _ri_addr( pddev );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.model_code: 0x%04X",
                  p_ri->model_code );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.model_name: \"%s\"",
                  p_ri->model_name );

  _mbg_kdd_msg_2( REPORT_CFG_LOG_LVL, "ri.sw_rev: code 0x%04X, name \"%s\"",
                  p_ri->sw_rev.code,
                  p_ri->sw_rev.name );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.sernum: \"%s\"",
                  p_ri->sernum );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.epld_name: \"%s\"",
                  p_ri->epld_name );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.n_channels: %u",
                  p_ri->n_channels );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.ticks_per_sec: %lu",
                  (ulong) p_ri->ticks_per_sec );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.features: 0x%08lX",
                  (ulong) p_ri->features );

  _mbg_kdd_msg_2( REPORT_CFG_LOG_LVL, "ri.fixed_freq: khz_val %u, range %i",
                  p_ri->fixed_freq.khz_val,
                  p_ri->fixed_freq.range );

  _mbg_kdd_msg_2( REPORT_CFG_LOG_LVL, "ri.osc type: %u, flags: 0x%02X",
                  p_ri->osc_type, p_ri->osc_flags );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.n_ucaps: %u",
                  p_ri->n_ucaps );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.n_com_ports: %u",
                  p_ri->n_com_ports );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.n_str_type: %u",
                  p_ri->n_str_type );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.n_prg_out: %u",
                  p_ri->n_prg_out );

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "ri.flags: 0x%04X",
                  p_ri->flags );

}  // dump_receiver_info



static /*HDR*/
void dump_xfeature_buffer( const PCPS_DDEV *pddev )
{
  static const char *xfeature_names[N_MBG_XFEATURE] = MBG_XFEATURE_NAMES;

  const MBG_XFEATURE_BUFFER *p = _xfeat_addr( pddev );
  int i;

  for ( i = 0; i < MAX_XFEATURE_BITS; i++ )
    if ( mbg_rc_is_success( check_feat_supp_byte_array( i, p->b, sizeof( *p ) ) ) )
      _mbg_kdd_msg_2( ( i < N_MBG_XFEATURE ) ? REPORT_CFG_LOG_LVL : MBG_LOG_WARN,
                      "xf %i: %s", i, ( i < N_MBG_XFEATURE ) ?
                      xfeature_names[i] : str_unkn_braced );

}  // dump_xfeature_buffer



static /*HDR*/
void dump_tlv_info( const PCPS_DDEV *pddev )
{
  static const char *tlv_feat_type_names[N_MBG_TLV_FEAT_TYPES] = MBG_TLV_FEAT_TYPE_NAMES;

  const MBG_TLV_INFO *p = _tlv_info_addr( pddev );
  const MBG_TLV_FEAT_BUFFER *pb = &p->supp_tlv_feat;
  int i;

  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "tlv rsvd:  0x%08lX", (ulong) p->reserved );
  _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "tlv flags: 0x%08lX", (ulong) p->flags );

  for ( i = 0; i < MAX_MBG_TLV_FEAT_TYPES; i++ )
    if ( mbg_rc_is_success( check_feat_supp_byte_array( i, pb->b, sizeof( *pb ) ) ) )
      _mbg_kdd_msg_2( ( i < N_MBG_TLV_FEAT_TYPES ) ? REPORT_CFG_LOG_LVL : MBG_LOG_WARN,
                      "tlv %i: %s", i, ( i < N_MBG_TLV_FEAT_TYPES ) ?
                      tlv_feat_type_names[i] : str_unkn_braced );

}  // dump_tlv_info

#endif  // REPORT_CFG_DETAILS



static /*HDR*/
/**
 * @brief Check the firmware to see if a specific feature is supported
 *
 * Some devices provide a specific feature only starting with a specific
 * firmware version, but the device provides no way to test this, so
 * the feature flag in the device structure is set if the firmware
 * has the required version.
 *
 * @param[in,out]  pddev        Pointer to the device structure
 * @param[in]      req_rev_num  Revision number required for the feature to be supported
 * @param[in]      flag_mask    Flag mask of the feature to be tested
 */
void check_feature( PCPS_DDEV *pddev, ushort req_rev_num,
                    PCPS_FEATURES flag_mask )
{
  int supported = _pcps_ddev_fw_rev_num( pddev ) >= req_rev_num;

  if ( supported )
    pddev->dev.cfg.features |= flag_mask;

  #if REPORT_CFG
    _mbg_kdd_msg_5( REPORT_CFG_LOG_LVL, "%s v%03X: feature 0x%08lX (%s) %ssupported",
                    _pcps_ddev_type_name( pddev ),
                    _pcps_ddev_fw_rev_num( pddev ),
                    (ulong) flag_mask, get_pcps_feature_name( flag_mask ),
                    supported ? str_empty : drvr_str_not_spc );
  #endif

}  // check_feature



static /*HDR*/
/**
 * @brief Set or clear a specific feature bit mask.
 *
 * @param[in,out]  pddev    Pointer to the device structure
 * @param[in]      msk      The feature bit mask, a combination of @ref GPS_BUILTIN_FEATURE_MASKS
 * @param[in]      is_supp  A flag indicating if the features in @a msk are supported
 */
void adjust_builtin_feature( PCPS_DDEV *pddev, BUILTIN_FEATURE_MASK msk, int is_supp )
{
  if ( is_supp )
    pddev->real_builtin_features |= msk;
  else
    pddev->real_builtin_features &= ~msk;

}  // adjust_builtin_feature



static /*HDR*/
/**
 * @brief Set or clear a specific feature bit mask for a specific device model.
 *
 * @param[in,out]  pddev    Pointer to the device structure
 * @param[in]      msk      The feature bit mask, a combination of @ref GPS_BUILTIN_FEATURE_MASKS
 * @param[in]      is_supp  A flag indicating if the features in @a msk are supported
 */
void adjust_builtin_feature_for_dev_type( PCPS_DDEV *pddev, BUILTIN_FEATURE_MASK msk, PCPS_TYPE_NUM type_num, int min_fw_rev_num )
{
  if ( type_num == _pcps_ddev_type_num( pddev ) )
  {
    bool supported = _pcps_ddev_fw_rev_num( pddev ) >= min_fw_rev_num;

    #if REPORT_CFG_DETAILS
      _mbg_kdd_msg_4( REPORT_CFG_LOG_LVL, "%s v%03X: builtin feat. 0x%08lX: %ssupported",
                      _pcps_ddev_type_name( pddev ),
                      _pcps_ddev_fw_rev_num( pddev ),
                      (ulong) msk,
                      supported ? str_empty : drvr_str_not_spc );
    #endif

    adjust_builtin_feature( pddev, msk, supported );
  }

}  // adjust_builtin_feature_for_dev_type



static /*HDR*/
/**
 * @brief Check the default builtin features are really supported.
 *
 * Some builtin features are only exposed via the binary protocol,
 * but not via the bus API, or only starting with a particular
 * firmware version. We adjust the feature mask here so that later
 * queries for supported features can be handled quicklier.
 *
 * @param[in,out]  pddev  Pointer to the device structure
 */
void setup_real_builtin_features( PCPS_DDEV *pddev )
{
  // By default, we assume all default builtin features
  // are really supported.
  pddev->real_builtin_features = pddev->default_builtin_features;


  // Check if the GPS_MODEL_HAS_TIME_TTM feature is supported.

  // ALL GPS/GNSS reveivers support this, others usually don't.
  adjust_builtin_feature( pddev, GPS_MODEL_HAS_TIME_TTM, _pcps_ddev_is_gps( pddev ) );

  // However, there are some other devices which support this since a specific firmware version.
  adjust_builtin_feature_for_dev_type( pddev, GPS_MODEL_HAS_TIME_TTM, PCPS_TYPE_PZF180PEX, 0x206 );
  adjust_builtin_feature_for_dev_type( pddev, GPS_MODEL_HAS_TIME_TTM, PCPS_TYPE_TCR180PEX, 0x113 );

  #if REPORT_CFG
  {
    BUILTIN_FEATURE_MASK changed = pddev->real_builtin_features ^ pddev->default_builtin_features;

    if ( changed )
      _mbg_kdd_msg_5( REPORT_CFG_LOG_LVL, "%s v%03X: builtin feat. 0x%08lX --> 0x%08lX, changed: 0x%08lX",
                      _pcps_ddev_type_name( pddev ), _pcps_ddev_fw_rev_num( pddev ),
                      (ulong) pddev->default_builtin_features, (ulong) pddev->real_builtin_features,
                      (ulong) changed );
    else
      _mbg_kdd_msg_3( REPORT_CFG_LOG_LVL, "%s v%03X: builtin feat. 0x%08lX (unchanged)",
                      _pcps_ddev_type_name( pddev ), _pcps_ddev_fw_rev_num( pddev ),
                      (ulong) pddev->default_builtin_features );
  }
  #endif

}  // setup_real_builtin_features



static /*HDR*/
/**
 * @brief Check the receiver info features to see if a specific feature is supported
 *
 * Some devices provide a ::RECEIVER_INFO which provides some flags that
 * can be mapped to the feature flags we use internally, so we scan these features
 * and update the internal features mask accordingly.
 *
 * @param[in,out]  pddev  Pointer to the device structure
 */
void check_ri_features( PCPS_DDEV *pddev )
{
  const RECEIVER_INFO *p_ri = _ri_addr( pddev );
  int gps_feat_bit;

  for ( gps_feat_bit = 0; gps_feat_bit < N_GPS_FEATURE; gps_feat_bit++ )
  {
    int supported = ( p_ri->features & ( 1UL << gps_feat_bit ) ) != 0;
    PCPS_FEATURES pcps_flags = ri_feat_tbl[gps_feat_bit];

    if ( supported )
    {
      if ( pcps_flags )
      {
        pddev->dev.cfg.features |= pcps_flags;

        #if REPORT_CFG
        {
          int i;

          for ( i = 0; i < 8 * sizeof( pcps_flags ); i++ )
            if ( pcps_flags & ( 1UL << i ) )
              _mbg_kdd_msg_4( REPORT_CFG_LOG_LVL, "%s v%03X: Supported RI feature \"%s\" maps to PCPS feature %s",
                              _pcps_ddev_type_name( pddev ),
                              _pcps_ddev_fw_rev_num( pddev ),
                              gps_ri_feature_names[gps_feat_bit],
                              pcps_feature_names[i] );
        }
        #endif
      }
      else
      {
        #if REPORT_CFG
          _mbg_kdd_msg_3( REPORT_CFG_LOG_LVL, "%s v%03X: RI feature \"%s\" supported, but no corresponding PCPS feature",
                          _pcps_ddev_type_name( pddev ),
                          _pcps_ddev_fw_rev_num( pddev ),
                          gps_ri_feature_names[gps_feat_bit] );
        #endif
      }
    }
    else
    {
      #if REPORT_CFG_DETAILS
        _mbg_kdd_msg_3( REPORT_CFG_LOG_LVL, "%s v%03X: RI feature \"%s\" NOT supported by device",
                        _pcps_ddev_type_name( pddev ),
                        _pcps_ddev_fw_rev_num( pddev ),
                        gps_ri_feature_names[gps_feat_bit] );
      #endif
    }
  }

}  // check_ri_features



/*HDR*/
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
int setup_sernum_and_receiver_info( PCPS_DDEV *pddev )
{
  RECEIVER_INFO *p_ri = _ri_addr( pddev );
  int rc;

  memset( _pcps_ddev_sernum( pddev ), 0, _pcps_ddev_sernum_size( pddev ) );

  // There are different ways to read the device's S/N. Check which
  // way is supported, and read the S/N from the device.
  //
  // Never just return a previous copy of the serial number which
  // has been read earlier since the S/N may just have been set
  // by a configuration API call.

  // Read the serial number directly, if the device supports this.
  if ( _pcps_ddev_has_sernum( pddev ) )
  {
    rc = _pcps_read( pddev, PCPS_GIVE_SERNUM,
                     _pcps_ddev_sernum( pddev ), _pcps_ddev_sernum_size( pddev ) - 1 );

    if ( mbg_rc_is_error( rc ) )
    {
      _mbg_kdd_msg_2( MBG_LOG_ERR, "Failed to read sernum: %s (rc: %i)",
                      mbg_strerror( rc ), rc );
      goto out;
    }

    #if REPORT_CFG
      _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "sernum directly: \"%s\"",
                      _pcps_ddev_sernum( pddev ) );
    #endif
  }


  // Setup the RECEIVER_INFO substructure.
  memset( p_ri, 0, sizeof( *p_ri ) );

  // Read the RECEIVER_INFO directly, if the device supports this.
  if ( _pcps_ddev_has_receiver_info( pddev ) )
  {
    rc = _pcps_read_gps_var( pddev, PC_GPS_RECEIVER_INFO, *p_ri );

    if ( mbg_rc_is_error( rc ) )
    {
      _mbg_kdd_msg_2( MBG_LOG_ERR, "Failed to read receiver info: %s (rc: %i)",
                      mbg_strerror( rc ), rc );
      goto out;
    }

    _mbg_swab_receiver_info( p_ri );

    _mbgddmsg_1( DEBUG_DEV_INIT, MBG_LOG_INFO, "Successfully read receiver info from dev %X",
                 _pcps_ddev_dev_id( pddev ) );

    #if REPORT_CFG
      _mbg_kdd_msg_1( REPORT_CFG_LOG_LVL, "sernum from RI: \"%s\"",
                      p_ri->sernum );
    #endif
  }


  #if REPORT_CFG
    // If the serial number is available both via a direct call
    // and via the RECEIVER_INFO then maxe sure both are the same.
    if ( _pcps_ddev_sernum( pddev )[0] && p_ri->sernum[0] )
      if ( strcmp( _pcps_ddev_sernum( pddev ), p_ri->sernum ) )
        _mbg_kdd_msg_0( MBG_LOG_WARN, "** RI sernum differs from direct sernum" );
  #endif


  // Very old devices may neither provide a RECEIVER_INFO,
  // nor support the sernum API.
  if ( p_ri->model_code == GPS_MODEL_UNKNOWN )
  {
    _mbgddmsg_1( DEBUG_DEV_INIT, MBG_LOG_INFO, "Setting up default receiver info for %s",
                 _pcps_ddev_type_name( pddev ) );

    // Setup a default RECEIVER_INFO depending on the type of device.
    if ( _pcps_ddev_is_gps( pddev ) )
      _setup_default_receiver_info_gps( p_ri );
    else
      _setup_default_receiver_info_dcf( p_ri, &pddev->dev );

    // Older GPS clocks store the S/N in an IDENT structure
    // which needs to be decoded to get the S/N.
    if ( _pcps_ddev_has_ident( pddev ) )
    {
      IDENT ident;
      char *cp;

      memset( &ident, 0, sizeof( ident ) );

      #if defined( MBG_TGT_DOS )
        assert( sizeof( ident ) < sizeof( pddev->dev.cfg.sernum ) );
      #endif

      #if DEBUG_SERNUM
        _mbg_kdd_msg_0( MBG_LOG_DEBUG, "Getting S/N from ident" );
      #endif

      rc = _pcps_read_gps_var( pddev, PC_GPS_IDENT, ident );

      if ( mbg_rc_is_error( rc ) )
      {
        _mbg_kdd_msg_2( MBG_LOG_WARN, "Failed to read GPS ident: %s (rc: %i)",
                        mbg_strerror( rc ), rc );
        goto out;
      }

      // The ident union must never be swapped due to endianess since we are
      // using it only as an array of characters.

      #if REPORT_CFG
      {
        size_t i;

        for ( i = 0; i < sizeof( ident ); i += 4 )
          _mbg_kdd_msg_5( MBG_LOG_DEBUG, "Ident[%02u]: %02X %02X %02X %02X",
                          (unsigned) i, ident.c[i], ident.c[i+1], ident.c[i+2], ident.c[i+3] );
      }
      #endif

      mbg_gps_ident_decode( _pcps_ddev_sernum( pddev ), &ident );

      #if REPORT_CFG
        _mbg_kdd_msg_1( MBG_LOG_DEBUG, "S/N decoded from ident: %s (raw)",
                        _pcps_ddev_sernum( pddev ) );
      #endif

      // Some old devices may have non-digits appended
      // to the S/N string. Truncate such a string.
      for ( cp = _pcps_ddev_sernum( pddev ); *cp; cp++ )
        if ( ( *cp < '0' ) || ( *cp > '9' ) )
        {
          *cp = 0;
          break;
        }

      #if DEBUG_SERNUM
        _mbg_kdd_msg_1( MBG_LOG_DEBUG, "S/N decoded from ident: %s",
                        _pcps_ddev_sernum( pddev ) );
      #endif
    }
  }

  // If the RECEIVER_INFO's sernum isn't empty
  // "beautify" it in any case.
  beautify_sernum( p_ri->sernum, sizeof( p_ri->sernum ) );

  // If it's now empty mark it as unknown.
  check_unknown_sernum( p_ri->sernum, sizeof( p_ri->sernum ) );

  // Now beautify the normal sernum.
  beautify_sernum( _pcps_ddev_sernum( pddev ), sizeof( p_ri->sernum ) );

  // If the "beautified" sernum is empty then we
  // copy from the RECEIVER_INFO, which may be valid
  // or unknown.
  if ( _pcps_ddev_sernum( pddev )[0] == 0 )
    strncpy( _pcps_ddev_sernum( pddev ), p_ri->sernum,
             _pcps_ddev_sernum_size( pddev ) );

  rc = MBG_SUCCESS;

out:
  return rc;

}  // setup_sernum_and_receiver_info



/*HDR*/
/**
 * @brief Check the receiver info and features.
 *
 * @param[in,out] pddev  Pointer to a device structure
 */
void check_receiver_info_and_features( PCPS_DDEV *pddev )
{
  const RECEIVER_INFO *p_ri = _ri_addr( pddev );
  int rc;

  #if REPORT_CFG
    _mbg_kdd_msg_3( REPORT_CFG_LOG_LVL, "%s v%03X RECEIVER_INFO features: 0x%08lX",
                    _pcps_ddev_type_name( pddev ), _pcps_ddev_fw_rev_num( pddev ),
                    (ulong) *_ri_addr( pddev ).features );
  #endif

  #if REPORT_CFG_DETAILS
    dump_receiver_info( pddev );
  #endif

  // Make sure this program supports at least as many ports as
  // the current device.
  if ( p_ri->n_com_ports > MAX_PARM_PORT )
    _mbg_kdd_msg_4( MBG_LOG_WARN, MBG_DEV_NAME_FMT " provides %i COM ports, but this driver only supports %i",
                    _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                    p_ri->n_com_ports, MAX_PARM_PORT );

  // Make sure this program supports at least as many string types
  // as the current device.
  if ( p_ri->n_str_type > MAX_PARM_STR_TYPE )
    _mbg_kdd_msg_4( MBG_LOG_WARN, MBG_DEV_NAME_FMT " supports %i serial string formats, but this driver only supports %i",
                    _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                    p_ri->n_str_type, MAX_PARM_STR_TYPE );

  // Make sure this program supports at least as many programmable outputs
  // as the current device.
  if ( p_ri->n_prg_out > MAX_PARM_POUT )
    _mbg_kdd_msg_4( MBG_LOG_WARN, MBG_DEV_NAME_FMT " supports %i programmable outputs, but this driver only supports %i",
                    _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                    p_ri->n_prg_out, MAX_PARM_POUT );


  // Detect the presence of some optional features at runtime.
  check_ri_features( pddev );

  #if REPORT_CFG
    _mbg_kdd_msg_3( REPORT_CFG_LOG_LVL, "%s v%03X initial PCPS features: 0x%08lX",
                    _pcps_ddev_type_name( pddev ), _pcps_ddev_fw_rev_num( pddev ),
                    (ulong) _pcps_ddev_features( pddev ) );
  #endif


  #if !defined( MBG_TGT_OS2 ) && !defined( MBG_TGT_BSD )  // TODO
    // Function strstr may not be supported at kernel level,
    // but this is not required, in most cases, anyway.
    if ( strstr( _pcps_ddev_fw_id( pddev ), "CERN" ) != NULL )
      pddev->dev.cfg.features |= PCPS_HAS_EVENT_TIME;
  #endif


  // Check if the device supports extended features

  rc = pcps_chk_dev_feat( pddev, DEV_FEAT_TYPE_RI, GPS_FEAT_XFEATURE );

  if ( mbg_rc_is_success( rc ) )
  {
    _mbgddmsg_2( DEBUG_DEV_INIT, MBG_LOG_INFO, "Going to read XFEATURE_BUFFER from " MBG_DEV_NAME_FMT,
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

    rc = _pcps_read_gps_var( pddev, PC_GPS_XFEATURE_BUFFER, *_xfeat_addr( pddev ) );

    if ( mbg_rc_is_error( rc ) )
    {
      _mbg_kdd_msg_4( MBG_LOG_WARN, "Failed to read XFEATURE_BUFFER from " MBG_DEV_NAME_FMT ": %s (rc: %i)",
                      _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                      mbg_strerror( rc ), rc );
      memset( _xfeat_addr( pddev ), 0, sizeof( *_xfeat_addr( pddev ) ) );
    }
    else
    {
      #if REPORT_CFG_DETAILS
        dump_xfeature_buffer( pddev );
      #endif
    }
  }

  #if TEST_CFG_DETAILS
  {
    _set_xfeature_bit( MBG_XFEATURE_TLV_API, _xfeat_addr( pddev ) );
    _set_xfeature_bit( MBG_XFEATURE_SAVE_CFG, _xfeat_addr( pddev ) );
    _set_xfeature_bit( N_MBG_XFEATURE - 1, _xfeat_addr( pddev ) );
    _set_xfeature_bit( N_MBG_XFEATURE, _xfeat_addr( pddev ) );
    dump_xfeature_buffer( pddev );
  }
  #endif


  // Check if the device supports the TLV API

  rc = pcps_chk_dev_feat( pddev, DEV_FEAT_TYPE_XFEAT, MBG_XFEATURE_TLV_API );

  if ( mbg_rc_is_success( rc ) )
  {
    _mbgddmsg_2( DEBUG_DEV_INIT, MBG_LOG_INFO, "Going to read TLV_INFO from " MBG_DEV_NAME_FMT,
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

    rc = _pcps_read_gps_var( pddev, PC_GPS_TLV_INFO, *_tlv_info_addr( pddev ) );

    if ( mbg_rc_is_error( rc ) )
    {
      _mbg_kdd_msg_4( MBG_LOG_WARN, "Failed to read TLV_INFO from " MBG_DEV_NAME_FMT ": %s (rc: %i)",
                      _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                      mbg_strerror( rc ), rc );
      memset( _tlv_info_addr( pddev ), 0, sizeof( *_tlv_info_addr( pddev ) ) );
    }
    else
    {
      #if REPORT_CFG_DETAILS
        dump_tlv_info( pddev );
      #endif
    }
  }

  #if TEST_CFG_DETAILS
  {
    _set_tlv_feat_bit( MBG_TLV_FEAT_TYPE_FW_UPDATE, _tlv_feat_buffp( pddev ) );
    _set_tlv_feat_bit( N_MBG_TLV_FEAT_TYPES - 1, _tlv_feat_buffp( pddev ) );
    _set_tlv_feat_bit( N_MBG_TLV_FEAT_TYPES, _tlv_feat_buffp( pddev ) );
    dump_tlv_info( pddev );
  }
  #endif

}  // check_receiver_info_and_features



/*HDR*/
/**
 * @brief Release I/O port and memory resource that have been claimed before
 *
 * @param[in,out] pddev   The device structure
 */
void pcps_release_rsrcs( PCPS_DDEV *pddev )
{
  int i;

  for ( i = 0; i < pddev->rsrc_info.num_rsrc_io; i++ )
  {
    MBG_IOPORT_RSRC *p = &_pcps_ddev_io_rsrc( pddev, i );

    if ( p->base_mapped )
    {
      #if DEBUG_RSRC
        _mbg_kdd_msg_3( MBG_LOG_INFO, "IO port rsrc %i (%" PRIX64 "): unmapping %" PRIX64,
                        i, (uint64_t) p->base_raw, (uint64_t) p->base_mapped );
      #endif

      mbg_unmap_ioport_rsrc( p );
    }
    else
    {
      #if DEBUG_RSRC
        _mbg_kdd_msg_2( MBG_LOG_INFO, "IO port rsrc %i (%" PRIX64 "): nothing to unmap",
                        i, (uint64_t) p->base_raw );
      #endif
    }

    #if _PCPS_USE_RSRCMGR
      if ( p->base_raw && p->num )
        rsrc_dealloc_ports( p->base_raw, p->num );
    #endif
  }


  for ( i = 0; i < pddev->rsrc_info.num_rsrc_mem; i++ )
  {
    MBG_IOMEM_RSRC *p = &_pcps_ddev_mem_rsrc( pddev, i );

    if ( p->start_mapped )
    {
      #if DEBUG_RSRC
        _mbg_kdd_msg_3( MBG_LOG_INFO, "IO mem rsrc %i (%" PRIX64 "): unmapping %p",
                        i, (uint64_t) p->start_raw, p->start_mapped );
      #endif

      mbg_unmap_iomem_rsrc( p );
    }
    else
    {
      #if DEBUG_RSRC
        _mbg_kdd_msg_2( MBG_LOG_INFO, "IO mem rsrc %i (%" PRIX64 "): nothing to unmap",
                        i, (uint64_t) p->start_raw );
      #endif
    }

    #if _PCPS_USE_RSRCMGR
      if ( p->start_raw && p->len )
        rsrc_dealloc_mem( p->start_raw, p->len );
    #endif  // _PCPS_USE_RSRCMGR
  }

  pddev->mm_asic_addr = NULL;
  pddev->mm_tstamp_addr = NULL;

}  // pcps_release_rsrcs



#if _PCPS_USE_RSRCMGR

#if defined( MBG_TGT_OS2 )

static /*HDR*/
/**
 * @brief Register a device under OS/2
 *
 * @param[in] pddev   The device structure
 */
void pcps_rsrc_register_device( PCPS_DDEV *pddev )
{
  #define RSRC_BASE_NAME "RADIOCLK_#  Meinberg Radio Clock "
  static const char rsrc_type_dcf77[] = RSRC_BASE_NAME "(DCF77)";
  static const char rsrc_type_gps[] = RSRC_BASE_NAME "(GPS)";
  static const char rsrc_type_irig[] = RSRC_BASE_NAME "(IRIG)";

  uchar bus_type;
  ushort rc;
  const char *cp;

  #if _PCPS_USE_USB
    #error USB not supported for this target environment!
  #endif

  if ( _pcps_ddev_is_pci( pddev ) )
    bus_type = RSRC_BUS_PCI;
  else
    if ( _pcps_ddev_is_mca( pddev ) )
      bus_type = RSRC_BUS_MCA;
    else
      bus_type = RSRC_BUS_ISA;

  if ( _pcps_ddev_is_irig_rx( pddev ) )
    cp = rsrc_type_irig;
  else
    if ( _pcps_ddev_is_gps( pddev ) )
      cp = rsrc_type_gps;
    else
      cp = rsrc_type_dcf77;

  rc = rsrc_register_device( &pddev->hDev, &pddev->rsrc, n_ddevs - 1, cp, bus_type );

}  // pcps_rsrc_register_device

#endif  // defined( MBG_TGT_OS2 )

#endif  // _PCPS_USE_RSRCMGR



#if _PCPS_USE_MCA

/*--------------------------------------------------------------
 *  PS31 only:
 *
 *  The scheme below shows the way the bits of the POS 103
 *  configuration byte are mapped to the PS31's programmable
 *  address decoder:
 *
 *   MSB            LSB
 *    |      ||      |
 *    0bbbbbb1000bxxxx  <-- 16 bit port base address (binary)
 *     ||||||    |
 *      \\\\\\   |               b:  configurable bit
 *        \\\\\\ |               x:  don't care bit
 *         |||||||
 *        1bbbbbbb      <--  8 bit configuration byte (POS 103)
 *        |
 *        |
 *    decoder enable bit, always 1 if adapter enabled
 *
 *-------------------------------------------------------------*/

static /*HDR*/
/**
 * @brief Convert the code read from PS/2 POS to the port base address
 *
 * @param[in] pos  The POS code to be converted
 *
 * @return  The decoded I/O port base address
 */
uint16_t pcps_port_from_pos( uint16_t pos )
{
  uint16_t us = ( ( pos & 0x007E ) << 8 ) | 0x0100;

  if ( pos & 0x0001 )
    us |= 0x0010;

  return us;

}  // pcps_port_from_pos



static /*HDR*/
/**
 * @brief Convert the port base address to a PS/2 POS code
 *
 * @param[in] port  The I/O port base address to be encoded
 *
 * @return  The encoded POS code
 */
uint8_t pcps_pos_from_port( uint16_t port )
{
  uint8_t uc;


  uc = *( ( (uchar *) (&port) ) + 1 ) & 0x7E;

  if ( port & 0x0010 )
    uc |= 1;

  return uc;

}  // pcps_pos_from_port



static /*HDR*/
/**
 * @brief Detect MCA devices and allocate device info structures
 *
 * @param[in]  ddev_init_fnc  Pointer to function called to allocate a device structure for each detected device.
 */
void pcps_detect_mca_devices( PCPS_DDEV_INIT_FNC *ddev_init_fnc )
{
  short rc;
  ushort type_idx;

  rc = mca_fnc_init();

  if ( rc != MCA_SUCCESS )
    return;


  // MCA is installed, now try to find a MCA clock with
  // known ID.
  for ( type_idx = 0; type_idx < N_PCPS_DEV_TYPE; type_idx++ )
  {
    static_wc MCA_POS_DATA pos_data;
    PCPS_DDEV *pddev;
    PCPS_DEV_TYPE *p = &pcps_dev_type[type_idx];

    static_wc uchar slot_num;   // the slot in which the board is installed


    if ( !( p->bus_flags & PCPS_BUS_MCA ) )
      continue;


    rc = mca_find_adapter( p->dev_id, &slot_num, &pos_data );

    if ( rc != MCA_SUCCESS )
      continue;


    // New device found, try to add to list.
    pddev = ddev_init_fnc();

    if ( pddev )  // Setup only if successful.
    {
      pddev->dev.type = *p;
      pcps_add_rsrc_io( pddev, pcps_port_from_pos( pos_data.pos_103 ),
                        PCPS_NUM_PORTS_MCA );

      //##++ Should try to read the interrupt line assigned to the clock.
      // The standard functions, however, don't use any interrupt.

      pcps_probe_device( pddev, 0, slot_num );
    }
  }

  mca_fnc_deinit();

}  // pcps_detect_mca_devices

#endif  /* _PCPS_USE_MCA */



/*HDR*/
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
PCPS_DEV_TYPE *pcps_get_dev_type_table_entry( PCPS_BUS_FLAGS bus_mask, PCPS_DEV_ID dev_id )
{
  int i;

  for ( i = 0; i < N_PCPS_DEV_TYPE; i++ )
  {
    PCPS_DEV_TYPE *p = &pcps_dev_type[i];

    if ( !( p->bus_flags & bus_mask ) )
      continue;

    if ( p->dev_id == dev_id )
      return p;
  }

  return NULL;

}  // pcps_get_dev_type_table_entry



static /*HDR*/
/**
 * @brief Free a previously allocated device info structure
 *
 * The pointer to the device structure is set to NULL
 * after the associated memory has been freed.
 *
 * @param[in,out]  ppddev  Address of a pointer to a device structure to be freed
 *
 * @see ::pcps_alloc_ddev_struc
 */
void pcps_free_ddev_struc( PCPS_DDEV **ppddev )
{
  PCPS_DDEV *pddev;

  _mbgddmsg_fnc_entry();

  pddev = *ppddev;

  if ( pddev )
  {
    #if !_PCPS_STATIC_DEV_LIST
      _pcps_kfree( pddev, sizeof( *pddev ) );
    #else
      memset( pddev, 0, sizeof( *pddev ) );

      if ( n_ddevs )
        n_ddevs--;
    #endif

    *ppddev = NULL;
  }

  _mbgddmsg_fnc_exit();

}  // pcps_free_ddev_struc



static /**
 * @brief Allocate a device info structure for a device
 *
 * On success, the pointer to the device structure is set to
 * the allocated memory for the device structure.
 *
 * @param[in,out]  ppddev  Address of a pointer to a device structure to be allocated
 *
 * @return ::MBG_SUCCESS on success, or ::MBG_ERR_NO_MEM if no memory could be allocated
 *
 * @see ::pcps_free_ddev_struc
 */
int pcps_alloc_ddev_struc( PCPS_DDEV **ppddev )
{
  PCPS_DDEV *pddev;
  int rc;

  _mbgddmsg_fnc_entry();

  #if !_PCPS_STATIC_DEV_LIST
    pddev = _pcps_kmalloc( sizeof( *pddev ) );
  #else
    if ( n_ddevs < N_SUPP_DEV_BUS )
    {
      pddev = &pcps_ddev[n_ddevs];
      n_ddevs++;
    }
    else
    {
      _mbgddmsg_0( DEBUG_DEV_INIT, MBG_LOG_ERR,
                   "Unable to add new device: max count reached" );

      pddev = NULL;
    }
  #endif

  if ( pddev )
  {
    memset( pddev, 0, sizeof( *pddev ) );
    rc = MBG_SUCCESS;
  }
  else
    rc = MBG_ERR_NO_MEM;

  *ppddev = pddev;

  _mbgddmsg_fnc_exit_chk_mbg_rc( rc );

  return rc;

}  // pcps_alloc_ddev_struc



/*HDR*/
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
int pcps_init_ddev( PCPS_DDEV **ppddev )
{
  int rc;

  _mbgddmsg_fnc_entry();

  rc = pcps_alloc_ddev_struc( ppddev );

  #if defined( _mbg_mutex_destroy ) || defined( _mbg_spin_lock_destroy )
    if ( mbg_rc_is_success( rc ) )
    {
      PCPS_DDEV *pddev = *ppddev;

      // If mutexes or spinlocks need to be destroyed on the target OS
      // when the driver shuts down then they are initialized now and
      // destroyed in the complementary function ::pcps_free_ddev_struc.
      // However, there are target OSs where those semaphores don't need
      // to be destroyed, and sometimes even *must not* be initialized
      // at this early point of driver initialization, e.g. under Windows,
      // in which case the semaphores will be initialized later.
      #if defined( _mbg_mutex_destroy )
        _mbg_mutex_init( &pddev->dev_mutex, MBG_DRVR_NAME "_dev_mutex" );
      #endif
      #if defined( _mbg_spin_lock_destroy )
        _mbg_spin_lock_init( &pddev->tstamp_lock, MBG_DRVR_NAME "_tstamp_lock" );
        _mbg_spin_lock_init( &pddev->irq_lock, MBG_DRVR_NAME "_irq_lock" );
      #endif
    }
  #endif

  _mbgddmsg_fnc_exit_chk_mbg_rc( rc );

  return rc;

}  // pcps_init_ddev



/*HDR*/
/**
 * @brief Clean up and free a previously initialized device info structure
 *
 * @param[in]  pddev  Address of a device structure to be cleaned up and released
 *
 * @see ::pcps_init_ddev
 */
void pcps_cleanup_ddev( PCPS_DDEV *pddev )
{
  _mbgddmsg_fnc_entry();

  if ( pddev )
  {
    #if defined( _mbg_mutex_destroy )
      _mbg_mutex_destroy( &pddev->dev_mutex );
    #endif

    #if defined( _mbg_spin_lock_destroy )
      _mbg_spin_lock_destroy( &pddev->tstamp_lock );
      _mbg_spin_lock_destroy( &pddev->irq_lock );
    #endif

    pcps_free_ddev_struc( &pddev );
  }

  _mbgddmsg_fnc_exit();

}  // pcps_cleanup_ddev



static /*HDR*/
/**
 * @brief Convert a raw I/O base address to a short format
 *
 * See notes for ::PCPS_SHORT_PORT_RSRC.

 * @param[out]  p_short_port_rsrc  Pointer to a variable that takes up the converted address
 * @param[in]   p_io_rsrc          Pointer to a variable with the address to be converted
 */
void rsrc_port_to_cfg_port( PCPS_SHORT_PORT_RSRC *p_short_port_rsrc, const MBG_IOPORT_RSRC *p_io_rsrc )
{
  p_short_port_rsrc->base = (PCPS_SHORT_PORT_ADDR) p_io_rsrc->base_raw;
  p_short_port_rsrc->num = (uint16_t) p_io_rsrc->num;

}  // rsrc_port_to_cfg_port



/*HDR*/
/**
 * @brief Add an I/O address range resource to the device structure
 *
 * @param[in,out]  pddev  Pointer to the device structure
 * @param[in]      base   Base address of the I/O address range
 * @param[in]      num    Number of addresses of the I/O address range
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
int pcps_add_rsrc_io( PCPS_DDEV *pddev, MBG_IOPORT_ADDR_RAW base, ulong num )
{
  PCPS_RSRC_INFO *prsrci = &pddev->rsrc_info;

  if ( prsrci->num_rsrc_io < N_PCPS_PORT_RSRC )
  {
    MBG_IOPORT_RSRC *p = &prsrci->port[prsrci->num_rsrc_io];
    int rc;

    #if _PCPS_USE_RSRCMGR
    {
      rc = rsrc_alloc_ports( base, num );

      // If the resource manager was unable to alloc the resources
      // then the selected range of ports is probably in use
      // by another hardware device and/or driver
      if ( mbg_rc_is_error( rc ) )
      {
        _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_RSRC_IO );
        return rc;
      }
    }
    #endif  // _PCPS_USE_RSRCMGR

    p->base_raw = base;
    p->num = num;

    rc = mbg_map_ioport_rsrc( p );

    if ( mbg_rc_is_error( rc ) )
    {
      _mbg_kdd_msg_2( MBG_LOG_ERR, "Failed to map I/O port rsrc %i: %s",
                      prsrci->num_rsrc_io, mbg_strerror( rc ) );
      return rc;
    }

    #if DEBUG_RSRC
      _mbg_kdd_msg_4( MBG_LOG_INFO, "Added I/O port rsrc %i: %03lX (%lu), mapped: %03lX",
                      prsrci->num_rsrc_io, (ulong) base, (ulong) num, (ulong) p->base_mapped );
    #endif

    prsrci->num_rsrc_io++;

    return MBG_SUCCESS;
  }

  return MBG_ERR_RSRC_ITEM;

}  // pcps_add_rsrc_io



/*HDR*/
/**
 * @brief Add a memory address range resource to the device structure
 *
 * @param[in,out]  pddev  Pointer to the device structure
 * @param[in]      start  Start address of the memory range
 * @param[in]      len    Size of the memory range
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
int pcps_add_rsrc_mem( PCPS_DDEV *pddev, MBG_IOMEM_ADDR_RAW start, ulong len )
{
  PCPS_RSRC_INFO *prsrci = &pddev->rsrc_info;

  if ( prsrci->num_rsrc_mem < N_PCPS_MEM_RSRC )
  {
    int rc;
    MBG_IOMEM_RSRC *p = &prsrci->mem[prsrci->num_rsrc_mem];

    #if _PCPS_USE_RSRCMGR
    {
      rc = rsrc_alloc_mem( start, len );

      // If the resource manager was unable to alloc the resources
      // then the selected range of ports is probably in use
      // by another hardware device and/or driver
      if ( mbg_rc_is_error( rc ) )
      {
        _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_RSRC_MEM );
        return rc;
      }
    }
    #endif  // _PCPS_USE_RSRCMGR

    p->start_raw = start;
    p->len = len;

    // Map this memory resource to get a virtual address
    rc = mbg_map_iomem_rsrc( p );

    if ( mbg_rc_is_error( rc ) )
    {
      _mbg_kdd_msg_2( MBG_LOG_ERR, "Failed to map I/O mem rsrc %i: %s",
                      prsrci->num_rsrc_mem, mbg_strerror( rc ) );
      return rc;
    }

    #if DEBUG_RSRC
      _mbg_kdd_msg_4( MBG_LOG_INFO, "Added I/O mem rsrc %i: %" PRIX64 " (%lu), mapped: %p",
                      prsrci->num_rsrc_mem, (uint64_t) start, (ulong) len, p->start_mapped );
    #endif

    prsrci->num_rsrc_mem++;

    return MBG_SUCCESS;
  }

  return MBG_ERR_RSRC_ITEM;

}  // pcps_add_rsrc_mem



/*HDR*/
/**
 * @brief Add an IRQ number resource to the device structure
 *
 * @param[in,out]  pddev    Pointer to the device structure
 * @param[in]      irq_num  Start address of the memory range
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
int pcps_add_rsrc_irq( PCPS_DDEV *pddev, int16_t irq_num )
{
  PCPS_RSRC_INFO *prsrci = &pddev->rsrc_info;

  if ( prsrci->num_rsrc_irq == 0 )
  {
    prsrci->irq.num = irq_num;
    prsrci->num_rsrc_irq++;

    #if DEBUG_RSRC
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, "Added IRQ rsrc #%i: %i",
                      prsrci->num_rsrc_irq, irq_num );
    #endif

    return MBG_SUCCESS;
  }

  return MBG_ERR_GENERIC;

}  // pcps_add_rsrc_irq



#if _PCPS_USE_PNP

/*HDR*/
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
int pcps_setup_ddev( PCPS_DDEV *pddev, PCPS_BUS_FLAGS bus_mask, PCPS_DEV_ID dev_id )
{
  PCPS_DEV_TYPE *pdt;

  _mbgddmsg_fnc_entry();

  // First check if we really support the device to be added.
  pdt = pcps_get_dev_type_table_entry( bus_mask, dev_id );

  if ( pdt == NULL )
  {
    _mbgddmsg_2( DEBUG_DEV_INIT, MBG_LOG_ERR, "pcps_setup_ddev: unknown %s ID 0x%04X",
                 ( bus_mask & PCPS_BUS_USB ) ? "USB" : "PCI", dev_id );

    _mbgddmsg_fnc_exit_err();

    return MBG_ERR_DEV_NOT_SUPP;
  }


  pddev->dev.type = *pdt;

  _mbgddmsg_2( DEBUG_DEV_INIT, MBG_LOG_INFO, "pcps_setup_ddev 0x%04X: found %s",
               dev_id, pdt->name );

  _mbgddmsg_fnc_exit_success();

  return MBG_SUCCESS;

}  // pcps_setup_ddev

#endif // _PCPS_USE_PNP



/*HDR*/
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
int pcps_chk_dev_feat( PCPS_DDEV *p_ddev, uint feat_type, uint feat_num )
{
  int rc;

  #if defined( DEBUG )
    const char *cp = NULL;
    int max_bit;
  #endif

  // NOTE In the code below we currently check if the bit to be tested
  // is really defined in an enumeration, and return an error if a bit
  // is tested which is not defined.
  // Alternatively we could instead just check if the bit number is valid
  // according to the buffer size.

  switch ( feat_type )
  {
    case DEV_FEAT_TYPE_BUILTIN:
      #if defined( DEBUG )
        cp = "builtin_feat";
        max_bit = N_GPS_BUILTIN_FEATURE_BITS;
      #endif
      if ( feat_num < N_GPS_BUILTIN_FEATURE_BITS )
      {
        rc = _check_feat_supp_bit( p_ddev->real_builtin_features, feat_num );
        goto out;
      }
      break;  // invalid


    case DEV_FEAT_TYPE_REF_TYPE:
      #if defined( DEBUG )
        cp = "ref_type";
        max_bit = N_PCPS_REF;
      #endif
      if ( feat_num < N_PCPS_REF )
      {
        rc = ( feat_num == _pcps_ddev_ref_type( p_ddev ) ) ? MBG_SUCCESS : MBG_ERR_NOT_SUPP_BY_DEV;
        goto out;
      }
      break;  // invalid


    case DEV_FEAT_TYPE_PCPS:
      #if defined( DEBUG )
        cp = "pcps_feat";
        max_bit = N_PCPS_FEATURE_BITS;
      #endif
      if ( feat_num < N_PCPS_FEATURE_BITS )
      {
        rc = _check_feat_supp_bit( p_ddev->dev.cfg.features, feat_num );
        goto out;
      }
      break;  // invalid


    case DEV_FEAT_TYPE_RI:
      #if defined( DEBUG )
        cp = "ri_feat";
        max_bit = N_GPS_FEATURE;
      #endif
      if ( feat_num < N_GPS_FEATURE )
      {
        rc = _check_feat_supp_bit( p_ddev->xdev_features.receiver_info.features, feat_num );
        goto out;
      }
      break;  // invalid


    case DEV_FEAT_TYPE_XFEAT:
      #if defined( DEBUG )
        cp = "x_feat";
        max_bit = N_MBG_XFEATURE;
      #endif
      if ( feat_num < N_MBG_XFEATURE )
      {
        rc = check_feat_supp_byte_array( feat_num, p_ddev->xdev_features.xfeature_buffer.b,
                                         sizeof( p_ddev->xdev_features.xfeature_buffer ) );
        goto out;
      }
      break;  // invalid


    case DEV_FEAT_TYPE_TLV_FEAT:
      #if defined( DEBUG )
        cp = "tlv_feat";
        max_bit = N_MBG_TLV_FEAT_TYPES;
      #endif
      if ( feat_num < N_MBG_TLV_FEAT_TYPES )
      {
        rc = check_feat_supp_byte_array( feat_num, p_ddev->xdev_features.tlv_info.supp_tlv_feat.b,
                                         sizeof( p_ddev->xdev_features.tlv_info.supp_tlv_feat ) );
        goto out;
      }
      break;  // invalid


    default:
      #if defined( DEBUG )
        cp = "*invalid* request";
        max_bit = 0;
      #endif
      rc = MBG_ERR_INV_PARM;
      goto out;
  }

  // Feature bit out of range
  rc = MBG_ERR_INV_PARM;

out:
  _mbgddmsg_5( DEBUG, MBG_LOG_INFO, "chk_dev_feat: %s %lu/%lu -> rc %i: %s",
               cp, (ulong) feat_num, (ulong) max_bit, rc,
               mbg_rc_is_success( rc ) ? "success" : mbg_strerror( rc ) );

  return rc;

}  // pcps_chk_dev_feat



#if _PCPS_USE_USB && !defined( MBG_TGT_WIN32 )

// The Windows-specific version is implemeted elsewhere.

static /*HDR*/
/**
 * @brief Do some USB-specific initialization
 *
 * This function is called by the probe routine if the probed device
 * is connected via USB.
 * If the device is supported then all specific information including
 * supported features is read from the device and stored in sub-structures
 * of the device structure addressed by pdev.
 *
 * @param[in,out]  pddev  Pointer to the device's device structure
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 */
int pcps_usb_init( PCPS_DDEV *pddev )
{
  int rc = MBG_ERR_NOT_SUPP_ON_OS;

  #if defined( MBG_TGT_LINUX )

    int usb_rc = usb_set_interface( pddev->udev, 0, 0 );

    if ( usb_rc < 0 )  // error returned by USB stack
    {
      rc = mbg_posix_errno_to_mbg( -usb_rc, NULL );

      _mbg_kdd_msg_3( MBG_LOG_ERR, "usb_set_interface failed with USB rc %d -> %d: %s",
                      usb_rc, rc, mbg_strerror( rc ) );
    }
    else
    {
      struct usb_host_interface *iface_desc = pddev->intf->cur_altsetting;
      int i;

      pddev->n_usb_ep = 0;

      for ( i = 0; i < iface_desc->desc.bNumEndpoints; i++ )
      {
        struct usb_endpoint_descriptor *endpoint = &iface_desc->endpoint[i].desc;
        PCPS_USB_EP *p_ep = &pddev->ep[i];
        p_ep->addr = endpoint->bEndpointAddress;
        p_ep->attrib = endpoint->bmAttributes;
        p_ep->max_packet_size = le16_to_cpu( endpoint->wMaxPacketSize );
        _mbgddmsg_4( DEBUG_USB_IO, MBG_LOG_INFO, "Endpoint %d: addr %02X, size: %d, attrib: %02X",
                     i, p_ep->addr, p_ep->max_packet_size, p_ep->attrib );
        pddev->n_usb_ep++;
      }

      rc = MBG_SUCCESS;
    }

  #else

    #error USB endpoint configuration can not be determined for this target.

  #endif

  return rc;

}  // pcps_usb_init

#endif  // _PCPS_USE_USB && !defined( MBG_TGT_WIN32 )



static /*HDR*/
void set_access_mode( PCPS_DDEV *pddev, uint mode, bool forced, PCPS_READ_FNC *read_fnc )
{
  pddev->access_mode = mode;
  pddev->access_mode_forced = forced;
  pddev->read = read_fnc;

}  // set_access_mode



static /*HDR*/
void report_access_mode( const PCPS_DDEV *pddev )
{
  static const char * const strs[N_PCPS_ACCESS_MODES] = PCPS_ACCESS_MODE_STRS;

  const char *str_access_mode;
  int log_lvl;
  bool must_report;

  #if MUST_REPORT_ACCESS_MODE
    // Always report.
    must_report = true;
  #else
    // Report only if mode forced.
    must_report = pddev->access_mode_forced;
  #endif

  if ( must_report )
  {
    if ( pddev->access_mode < N_PCPS_ACCESS_MODES )
    {
      str_access_mode = strs[pddev->access_mode];
      log_lvl = MBG_LOG_INFO;
    }
    else
    {
      str_access_mode = str_unkn_braced;
      log_lvl = MBG_LOG_WARN;
    }

    _mbg_kdd_msg_2( log_lvl, "Access mode: %s%s", str_access_mode,
                    pddev->access_mode_forced ? PCPS_ACCESS_MODE_STR_FRCD : "" );
  }

}  // report_access_mode



#if PCPS_LOG_STD_MSGS

static /*HDR*/
void report_device_info( const PCPS_DDEV *pddev )
{
  char ws[160];

  int n = mbg_kdd_snprintf( ws, sizeof( ws ), "Found %s, S/N %s",
              _pcps_ddev_fw_id( pddev ), _pcps_ddev_sernum( pddev ) );

  if ( _pcps_ddev_io_base_raw( pddev, 0 ) )
  {
    n += mbg_kdd_snprintf( &ws[n], sizeof( ws ) - n,
              ", port %03lX", (ulong) _pcps_ddev_io_base_raw( pddev, 0 ) );

    if ( _pcps_ddev_io_base_raw( pddev, 1 ) )
      n += mbg_kdd_snprintf( &ws[n], sizeof( ws ) - n,
               ",%03lX", (ulong) _pcps_ddev_io_base_raw( pddev, 1 ) );
  }

  if ( pddev->rsrc_info.num_rsrc_irq )
    n += mbg_kdd_snprintf( &ws[n], sizeof( ws ) - n,
               ", irq %i", _pcps_ddev_irq_num( pddev ) );

  _mbg_kdd_msg_1( MBG_LOG_INFO, "%s", ws );

  if ( _pcps_ddev_fw_has_20ms_bug( pddev ) )
    _mbg_kdd_msg_3( MBG_LOG_WARN, "** Warning: %s v" PCPS_FW_STR_FMT
                    " has a 20 ms timing bug. Please upgrade the firmware!",
                    _pcps_ddev_type_name( pddev ),
                    _pcps_fw_rev_num_major( _pcps_ddev_fw_rev_num( pddev ) ),
                    _pcps_fw_rev_num_minor( _pcps_ddev_fw_rev_num( pddev ) ) );

}  // report_device_info

#endif  // PCPS_LOG_STD_MSGS



/*HDR*/
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
int pcps_probe_device( PCPS_DDEV *pddev,
                       PCPS_BUS_NUM bus_num,
                       PCPS_SLOT_NUM dev_fnc_num )
{
  // The PC cycles frequency only needs to be determined once,
  // so this flag is used to remember if this has been done before.
  static bool pc_cycles_frequency_read;

  ushort port_rsrc_len[N_PCPS_PORT_RSRC] = { 0 };
  int port_ranges_expected = 0;
  int mem_ranges_expected = 0;
  const char *cp = str_empty;
  int i;
  int rc = MBG_ERR_GENERIC;

  _mbgddmsg_fnc_entry();

  #if !defined( MBG_TGT_DOS )
    // Try to read the PC's cycles frequency, but only once.
    if ( !pc_cycles_frequency_read )
    {
      mbg_get_pc_cycles_frequency( &pc_cycles_frequency );
      _mbgddmsg_1( DEBUG, MBG_LOG_DEBUG, "PC cycles frequency: %" PRIi64 " Hz",
                   (int64_t) pc_cycles_frequency );

      pc_cycles_frequency_read = true;
    }
  #endif

  #if _PCPS_USE_MM_IO
    #if defined( FORCE_IO_ACCESS )
      force_io_access = FORCE_IO_ACCESS;
    #endif

    #if defined( FORCE_MM16_ACCESS )
      force_mm16_access = FORCE_MM16_ACCESS;
    #endif
  #endif

  pddev->dev.cfg.bus_num = bus_num;
  pddev->dev.cfg.slot_num = dev_fnc_num;

  #if MUST_REPORT_PROBE_DEVICE_DETAILS
    report_probe_device( pddev );
  #endif

  set_access_mode( pddev, PCPS_ACC_MODE_NULL, 0, pcps_read_null );

  if ( _pcps_ddev_chk_err_flags( pddev, PCPS_EF_IO_INIT | PCPS_EF_IO_ENB ) )
  {
    _mbgddmsg_3( DEBUG_DEV_INIT, MBG_LOG_ERR, "Probe %s (ID 0x%X) fails, err flags 0x%X",
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_dev_id( pddev ),
                 _pcps_ddev_err_flags( pddev ) );

    goto out_fail_generic;
  }


  // If mutexes or spinlocks need to be destroyed on the target OS
  // when the driver shuts down then they have already been initialized
  // in the device allocation routine and will be destroyed in the
  // complementary device deallocation routine.
  //
  // However, there are target OSs where those semaphores don't need
  // to be destroyed, and sometimes even *must not* be initialized
  // when the device structure is allocated (e.g under Windows),
  // in which case the semaphores need to be initialized here.
  #if !defined( _mbg_mutex_destroy )
    #if defined( _mbg_mutex_init )
      _mbg_mutex_init( &pddev->dev_mutex, MBG_DRVR_NAME "_dev_mutex" );
    #endif
  #endif
  #if !defined( _mbg_spin_lock_destroy )
    #if defined( _mbg_spin_lock_init )
      _mbg_spin_lock_init( &pddev->tstamp_lock, MBG_DRVR_NAME "_tstamp_lock" );
      _mbg_spin_lock_init( &pddev->irq_lock, MBG_DRVR_NAME "_irq_lock" );
    #endif
  #endif


  // Next we do some setup depending on the interface type
  // and chip.

  switch ( _pcps_ddev_bus_flags( pddev ) )
  {
    #if _PCPS_USE_USB
      case PCPS_BUS_USB:
      case PCPS_BUS_USB_V2:
        #if defined( MBG_TGT_LINUX ) && USE_LOCAL_IO_BUFFER
          _mbg_kdd_msg_0( MBG_LOG_WARN, "Driver built using local I/O buffers, which can cause problems with newer kernels." );
        #endif

        // No direct port I/O possible.
        set_access_mode( pddev, PCPS_ACC_MODE_USB, 0, pcps_read_usb );

        // Do some USB-specific initializiation.
        rc = pcps_usb_init( pddev );

        if ( mbg_rc_is_error( rc ) )
        {
          _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_INIT );
          goto out;
        }

        if ( pddev->n_usb_ep < MBGUSB_MIN_ENDPOINTS_REQUIRED )
        {
          _mbg_kdd_msg_3( MBG_LOG_ERR, "Device %s supports only %d endpoints while %d are required",
                          _pcps_ddev_type_name( pddev ), pddev->n_usb_ep,
                          MBGUSB_MIN_ENDPOINTS_REQUIRED );
          _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_INIT );
          goto out_fail_generic;
        }
        break;

    #endif  // _PCPS_USE_USB


    case PCPS_BUS_PCI_MBGPEX:
      port_rsrc_len[0] = sizeof( PCI_ASIC );
      port_ranges_expected = 1;
      mem_ranges_expected = 1;
      pddev->status_port_offs = offsetof( PCI_ASIC, status_port );
      #if _PCPS_USE_MM_IO
        // If memory mapped access is supported then we use that
        // by default, unless I/O access is explicitly requested.
        if ( force_io_access )
          set_access_mode( pddev, PCPS_ACC_MODE_IO, 1, pcps_read_asic );
        else
          set_access_mode( pddev, PCPS_ACC_MODE_MM, 0, pcps_read_asic_mm );
      #else
        // If memory mapped access is not supported
        // then we do I/O access anyway.
        set_access_mode( pddev, PCPS_ACC_MODE_IO, 0, pcps_read_asic );
      #endif
      break;


    case PCPS_BUS_PCI_PEX8311:
      port_rsrc_len[0] = 0;
      port_rsrc_len[1] = sizeof( PCI_ASIC );  // same as ASIC
      port_ranges_expected = 2;  // ranges will be swapped later
      // mem_ranges_expected = 2;   // TODO Do we need this?
      pddev->status_port_offs = offsetof( PCI_ASIC, status_port ); // same as ASIC
      // We always use port I/O by default.
      set_access_mode( pddev, PCPS_ACC_MODE_IO, 0, pcps_read_asic );
      #if _PCPS_USE_MM_IO
        // If memory mapped access is supported then we use that
        // only if explicitly requested since memory mapped access
        // with this chip is only 16 bit wide.
        if ( force_mm16_access )
          set_access_mode( pddev, PCPS_ACC_MODE_MM16, 1, pcps_read_asic_mm16 );
      #endif
      break;


    case PCPS_BUS_PCI_ASIC:
      port_rsrc_len[0] = sizeof( PCI_ASIC );
      port_ranges_expected = 1;
      // Only I/O access is supported anyway.
      pddev->status_port_offs = offsetof( PCI_ASIC, status_port );
      set_access_mode( pddev, PCPS_ACC_MODE_IO, 0, pcps_read_asic );
      break;


    case PCPS_BUS_PCI_S5920:
      port_rsrc_len[0] = AMCC_OP_REG_RANGE_S5920;
      port_rsrc_len[1] = 16;  //##++
      port_ranges_expected = 2;
      pddev->status_port_offs = AMCC_OP_REG_IMB4 + 3;
      // Only I/O access is supported anyway.
      set_access_mode( pddev, PCPS_ACC_MODE_IO, 0, pcps_read_amcc_s5920 );
      break;


    case PCPS_BUS_PCI_S5933:
      port_rsrc_len[0] = AMCC_OP_REG_RANGE_S5933;
      port_ranges_expected = 1;
      pddev->status_port_offs = AMCC_OP_REG_IMB4 + 3;
      // Only I/O access is supported anyway.
      set_access_mode( pddev, PCPS_ACC_MODE_IO, 0, pcps_read_amcc_s5933 );
      break;


    case PCPS_BUS_MCA:
    case PCPS_BUS_ISA:
      // resource lengths have already been set
      port_ranges_expected = 1;
      pddev->status_port_offs = 1;
      // Only I/O access is supported anyway.
      set_access_mode( pddev, PCPS_ACC_MODE_IO, 0, pcps_read_std );
      break;


    default:
      _mbg_kdd_msg_2( MBG_LOG_ERR, "Bus flags %04X not handled for device %s",
                      _pcps_ddev_bus_flags( pddev ), _pcps_ddev_type_name( pddev ) );
      goto out_fail_generic;

  }  // switch ( _pcps_ddev_bus_flags( pddev ) )


  if ( _pcps_ddev_access_mode_io( pddev ) )
  {
    // check if all required I/O resources have been assigned
    if ( pddev->rsrc_info.num_rsrc_io < port_ranges_expected )
    {
      _mbgddmsg_3( DEBUG_DEV_INIT, MBG_LOG_ERR, "Probe %s fails: I/O port ranges (%u) less than expected (%u)",
                   _pcps_ddev_type_name( pddev ), pddev->rsrc_info.num_rsrc_io, port_ranges_expected );
      _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_INIT );
      goto out_fail_generic;
    }
  }

  if ( _pcps_ddev_access_mode_mm( pddev ) )
  {
    // check if all required mem resources have been assigned
    if ( pddev->rsrc_info.num_rsrc_mem < mem_ranges_expected )
    {
      _mbgddmsg_3( DEBUG_DEV_INIT, MBG_LOG_ERR, "Probe %s fails: I/O mem ranges (%u) less than expected (%u)",
                   _pcps_ddev_type_name( pddev ), pddev->rsrc_info.num_rsrc_mem, mem_ranges_expected );
      _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_INIT );
      goto out_fail_generic;
    }
  }

  // Setup additional properties depending on the
  // type of bus interface chip.

  if ( _pcps_ddev_is_pci_mbgpex( pddev ) )
  {
    pddev->irq_enb_disb_port = _pcps_ddev_io_base_mapped( pddev, 0 )
                             + offsetof( PCI_ASIC, control_status );
    pddev->irq_flag_port = pddev->irq_enb_disb_port;
    pddev->irq_flag_mask = PCI_ASIC_PCI_IRQF;

    pddev->irq_ack_port = pddev->irq_enb_disb_port;
    pddev->irq_ack_mask = PCI_ASIC_PCI_IRQF;
    goto chip_setup_done;
  }


  if ( _pcps_ddev_is_pci_pex8311( pddev ) )
  {
    // I/O and memory ranges have to be swapped for the
    // low level functions because otherwise the first
    // range addressed the chip configuration registers
    // while the second range addressed data registers.

    MBG_IOMEM_RSRC tmp_mem_rsrc;
    MBG_IOPORT_RSRC tmp_io_rsrc;

    tmp_mem_rsrc = pddev->rsrc_info.mem[0];
    pddev->rsrc_info.mem[0] = pddev->rsrc_info.mem[1];
    pddev->rsrc_info.mem[1] = tmp_mem_rsrc;

    // There should be only a single memory range for the PLX config registers
    // with index 0. We have now moved this to index 1, so we have to increase
    // the number of resources.
    if ( pddev->rsrc_info.num_rsrc_mem & 0x01 )  // odd
      pddev->rsrc_info.num_rsrc_mem++;           // make it even

    #if DEBUG_RSRC
      _mbg_kdd_msg_4( MBG_LOG_INFO, "Port rsrcs before swapping: %04lX (%08lX), %04lX (%08lX)",
        (ulong) pddev->rsrc_info.port[0].base_raw, (ulong) pddev->rsrc_info.port[0].base_mapped,
        (ulong) pddev->rsrc_info.port[1].base_raw, (ulong) pddev->rsrc_info.port[1].base_mapped );
    #endif

    tmp_io_rsrc = pddev->rsrc_info.port[0];
    pddev->rsrc_info.port[0] = pddev->rsrc_info.port[1];
    pddev->rsrc_info.port[1] = tmp_io_rsrc;

    #if DEBUG_RSRC
      _mbg_kdd_msg_4( MBG_LOG_INFO, "Port rsrcs after swapping:  %04lX (%08lX), %04lX (%08lX)",
        (ulong) pddev->rsrc_info.port[0].base_raw, (ulong) pddev->rsrc_info.port[0].base_mapped,
        (ulong) pddev->rsrc_info.port[1].base_raw, (ulong) pddev->rsrc_info.port[1].base_mapped );
    #endif

    // Attention: the interrupt control/status register is located in
    // the PLX configuration space which is addressed by a different
    // port address range than the normal data ports !!
    pddev->irq_enb_disb_port = _pcps_ddev_io_base_mapped( pddev, 1 ) + PLX_LCS_INTCSR;
    pddev->irq_enb_mask = PLX_LCS_INTCSR_INT_ENB;
    pddev->irq_disb_mask = PLX_LCS_INTCSR_INT_ENB;

    pddev->irq_flag_port = _pcps_ddev_io_base_mapped( pddev, 1 ) + PLX_LCS_INTCSR;
    pddev->irq_flag_mask = PLX_LCS_INTCSR_INT_FLAG;

    pddev->irq_ack_port = _pcps_ddev_io_base_mapped( pddev, 0 ) + offsetof( PCI_ASIC, control_status );
    pddev->irq_ack_mask = PCI_ASIC_PCI_IRQF;
    goto chip_setup_done;
  }

  if ( _pcps_ddev_is_pci_asic( pddev ) )
  {
    pddev->irq_enb_disb_port = _pcps_ddev_io_base_mapped( pddev, 0 )
                             + offsetof( PCI_ASIC, control_status );
    pddev->irq_flag_port = pddev->irq_enb_disb_port;
    pddev->irq_flag_mask = PCI_ASIC_PCI_IRQF;

    pddev->irq_ack_port = pddev->irq_enb_disb_port;
    pddev->irq_ack_mask = PCI_ASIC_PCI_IRQF;
    goto chip_setup_done;
  }

  if ( _pcps_ddev_is_pci_amcc( pddev ) )
  {
    pddev->irq_enb_disb_port = _pcps_ddev_io_base_mapped( pddev, 0 )
                             + AMCC_OP_REG_INTCSR;
    pddev->irq_enb_mask = AMCC_INT_ENB;
    pddev->irq_disb_mask = AMCC_INT_MASK;

    pddev->irq_flag_port = pddev->irq_enb_disb_port;
    pddev->irq_flag_mask = AMCC_INT_FLAG;

    pddev->irq_ack_port = pddev->irq_enb_disb_port;
    pddev->irq_ack_mask = AMCC_INT_ACK;
    goto chip_setup_done;
  }

chip_setup_done:


  // Chip-specific setup done. Continue with common device setup.

  report_access_mode( pddev );

  setup_mm_addr( pddev );

  pddev->status_port = _pcps_ddev_io_base_mapped( pddev, 0 ) + pddev->status_port_offs;

  // Set up the resource list in pddev->dev.cfg which
  // isn't really required anymore, but just informational.

  for ( i = 0; i < N_PCPS_PORT_RSRC; i++ )
  {
    MBG_IOPORT_RSRC *prsrc;

    if ( i >= port_ranges_expected )
      break;

    prsrc = &pddev->rsrc_info.port[i];

    // If the resource len has not yet been set
    // then use the default resource len.
    if ( prsrc->num == 0 )
      prsrc->num = port_rsrc_len[i];

    rsrc_port_to_cfg_port( &pddev->dev.cfg.port[i], &pddev->rsrc_info.port[i] );
  }

  pddev->dev.cfg.irq_num = pddev->rsrc_info.num_rsrc_irq ?
                           pddev->rsrc_info.irq.num : -1;
  pddev->dev.cfg.short_status_port = _pcps_ddev_short_port_base( pddev, 0 ) + pddev->status_port_offs;

  pddev->dev.cfg.timeout_clk = PCPS_TIMEOUT_CNT;

  #if DEBUG_PORTS
    _mbg_kdd_msg_3( MBG_LOG_DEBUG, "IRQ enb/disb port: %04lX, enb: %08lX, disb: %08lX",
                    (ulong) pddev->irq_enb_disb_port,
                    (ulong) pddev->irq_enb_mask,
                    (ulong) pddev->irq_disb_mask
                  );
    _mbg_kdd_msg_2( MBG_LOG_DEBUG, "IRQ flag port: %04lX, mask: %08lX",
                    (ulong) pddev->irq_flag_port,
                    (ulong) pddev->irq_flag_mask
                  );
    _mbg_kdd_msg_2( MBG_LOG_DEBUG, "IRQ ack port: %04lX, mask: %08lX",
                    (ulong) pddev->irq_ack_port,
                    (ulong) pddev->irq_ack_mask
                  );
    _mbg_kdd_msg_1( MBG_LOG_DEBUG, "Status port: %04lX", (ulong) pddev->status_port );
  #endif


  // There are some BIOSs out there which don't configure some PEX cards
  // with PLX chips properly, and thus the cards can not be accessed.
  // See note near the definition of ::_pcps_pci_cfg_err for details.
  if ( _pcps_ddev_pci_cfg_err( pddev ) )
  {
    _mbg_kdd_msg_2( MBG_LOG_WARN, "Duplicate base address 0x%04lX, device %s will not work properly (BIOS faulty)",
                    (ulong) _pcps_ddev_io_base_raw( pddev, 0 ), _pcps_ddev_type_name( pddev ) );
    goto fail_with_cleanup;
  }

  #if 0 && DEBUG //### TODO testing only
  {
    MBG_SYS_UPTIME uptime;
    mbg_get_sys_uptime( &uptime );
    mbg_sleep_sec( 1 );
    mbg_get_sys_uptime( &uptime );
  }
  #endif

  // A PTP270PEX card must have finished booting
  // before it can be accessed.
  if ( pcps_ddev_is_ptp270pex( pddev ) )
    wait_ptp270pex_ready( pddev );


  #if TEST_PORT_ACCESS
    test_port_access( pddev );
  #endif  // TEST_PORT_ACCESS


  // try to read firmware ID
  rc = pcps_get_fw_id( pddev, pddev->dev.cfg.fw_id );

  if ( mbg_rc_is_error( rc ) )
  {
    if ( _pcps_ddev_is_isa( pddev ) )
    {
      // ISA devices are detected by trying to read a firmware ID via
      // a given port, so if the firmware ID could not be read then this
      // just means there is no device using the given port address.
      _mbgddmsg_1( DEBUG_DEV_INIT, MBG_LOG_WARN, "No ISA card found at port %03lXh.",
                   (ulong) _pcps_ddev_io_base_raw( pddev, 0 ) );
    }
    else
    {
      // Non-ISA devices are detected by some other means, so if the
      // firmware ID could not be read this is a serious error.
      _mbg_kdd_msg_3( MBG_LOG_ERR, "Probe device %s %i/%i: failed to read firmware ID",
                      _pcps_ddev_type_name( pddev ), bus_num, dev_fnc_num );
    }

    _pcps_ddev_set_err_flags( pddev, PCPS_EF_TIMEOUT );
    goto fail_with_cleanup;
  }


  if ( _pcps_ddev_bus_flags( pddev ) == PCPS_BUS_ISA )
  {
    ushort dev_type;

    // Still need to find out which type of ISA clock we have found.
    // Check EPROM ID to find out which kind of clock is installed.
    if ( pcps_check_id( pddev, fw_id_ref_gps ) == MBG_SUCCESS )
      dev_type = PCPS_TYPE_GPS167PC;
    else
    {
      if ( mbg_rc_is_success( pcps_check_id( pddev, fw_id_ref_pcps ) ) )
      {
        // Device is a PC31, or a PC32 if it has signature code.
        // If no support for MCA has been compiled in, it may even
        // be a PS31 which is software compatible with a PC31.
        dev_type =
          ( _mbg_inp16_to_cpu( pddev, 0, _pcps_ddev_io_base_mapped( pddev, 0 ) + 2 )
            == pcps_dev_type[PCPS_TYPE_PC32].dev_id ) ?
         PCPS_TYPE_PC32 : PCPS_TYPE_PC31;
      }
      else
      {
        _pcps_ddev_set_err_flags( pddev, PCPS_EF_INV_FW_ID );
        goto fail_with_cleanup;
      }
    }

    pddev->dev.type = pcps_dev_type[dev_type];
  }

  #if defined( MBG_TGT_OS2 )
    pcps_rsrc_register_device( pddev );
  #endif

  // Extract the firmware revision number from the ID string.
  pddev->dev.cfg.fw_rev_num = pcps_get_rev_num( _pcps_ddev_fw_id( pddev ) );

  // If the device has an ASIC, EPLD or FPGA read the ASIC version number
  if ( _pcps_ddev_has_asic_version( pddev ) )
  {
  #if _PCPS_USE_MM_IO
    if ( _pcps_ddev_access_mode_mm( pddev ) )
    {
      pddev->raw_asic_version = _mbg_mmrd32_to_cpu( &pddev->mm_asic_addr->raw_version );
      cp = "MM";
    }
    else
  #endif  // _PCPS_USE_MM_IO
    {
      pddev->raw_asic_version = _mbg_inp32_to_cpu( pddev, 0, _pcps_ddev_io_base_mapped( pddev, 0 )
                                + offsetof( PCI_ASIC, raw_version ) );
      cp = "I/O";
    }

    pddev->asic_version = _convert_asic_version_number( pddev->raw_asic_version );

    #if DEBUG_IO || DEBUG_RSRC || REPORT_CFG
      _mbg_kdd_msg_3( MBG_LOG_INFO, "ASIC version 0x%04lX read via %s (raw: 0x%04lX)",
                      (ulong) pddev->asic_version,  cp,
                      (ulong) pddev->raw_asic_version );
    #endif

    if ( _pcps_ddev_is_pci_asic( pddev ) )  // TODO do we need this? Use macro!
      if ( _pcps_asic_version_major( pddev->asic_version ) == PCI_ASIC_MAJOR_PCI_0 )
        _mbg_kdd_msg_0( MBG_LOG_WARN, "Warning: This old device has a PCI interface CRC bug!" );
  }


  // Setup some feature flags which depend on the device type
  // and firmware version.
  switch ( _pcps_ddev_type_num( pddev ) )
  {
    case PCPS_TYPE_PC31:
    case PCPS_TYPE_PS31_OLD:
    case PCPS_TYPE_PS31:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_PC31PS31;
      check_feature( pddev, REV_CAN_SET_TIME_PC31PS31, PCPS_CAN_SET_TIME );
      check_feature( pddev, REV_HAS_SERIAL_PC31PS31, PCPS_HAS_SERIAL );
      check_feature( pddev, REV_HAS_SYNC_TIME_PC31PS31, PCPS_HAS_SYNC_TIME );
      check_feature( pddev, REV_HAS_UTC_OFFS_PC31PS31, PCPS_HAS_UTC_OFFS );
      break;

    case PCPS_TYPE_PC32:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_PC32;
      break;

    case PCPS_TYPE_PCI32:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_PCI32;
      break;

    case PCPS_TYPE_GPS167PC:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS167PC;
      pddev->dev.cfg.features = PCPS_FEAT_GPS167PC;
      check_feature( pddev, REV_HAS_HR_TIME_GPS167PC, PCPS_HAS_HR_TIME );
      check_feature( pddev, REV_HAS_CABLE_LEN_GPS167PC, PCPS_HAS_CABLE_LEN );
      break;

    case PCPS_TYPE_GPS167PCI:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS167PCI;
      pddev->dev.cfg.features = PCPS_FEAT_GPS167PCI;
      check_feature( pddev, REV_HAS_CABLE_LEN_GPS167PCI, PCPS_HAS_CABLE_LEN );
      check_feature( pddev, REV_CAN_CLR_UCAP_BUFF_GPS167PCI, PCPS_CAN_CLR_UCAP_BUFF );
      check_feature( pddev, REV_HAS_UCAP_GPS167PCI, PCPS_HAS_UCAP );
      break;

    case PCPS_TYPE_PCI509:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_PCI509;
      break;

    case PCPS_TYPE_GPS168PCI:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS168PCI;
      pddev->dev.cfg.features = PCPS_FEAT_GPS168PCI;
      check_feature( pddev, REV_CAN_CLR_UCAP_BUFF_GPS168PCI, PCPS_CAN_CLR_UCAP_BUFF );
      check_feature( pddev, REV_HAS_UCAP_GPS168PCI, PCPS_HAS_UCAP );
      break;

    case PCPS_TYPE_PCI510:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_PCI510;
      break;

    case PCPS_TYPE_GPS169PCI:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS169PCI;
      pddev->dev.cfg.features = PCPS_FEAT_GPS169PCI;
      check_feature( pddev, REV_HAS_GPS_DATA_16_GPS169PCI, PCPS_HAS_GPS_DATA_16 );
      break;

    case PCPS_TYPE_TCR510PCI:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_TCR510PCI;
      check_feature( pddev, REV_HAS_HR_TIME_TCR510PCI, PCPS_HAS_HR_TIME );
      break;

    case PCPS_TYPE_TCR167PCI:
      pddev->default_builtin_features = BUILTIN_FEAT_TCR167PCI;
      pddev->dev.cfg.features = PCPS_FEAT_TCR167PCI;
      break;

    case PCPS_TYPE_GPS170PCI:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS170PCI;
      pddev->dev.cfg.features = PCPS_FEAT_GPS170PCI;
      break;

    case PCPS_TYPE_PCI511:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_PCI511;
      check_feature( pddev, REV_HAS_HR_TIME_PCI511, PCPS_HAS_HR_TIME );
      break;

    case PCPS_TYPE_TCR511PCI:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_TCR511PCI;
      check_feature( pddev, REV_HAS_IRIG_CTRL_BITS_TCR511PCI, PCPS_HAS_IRIG_CTRL_BITS );
      check_feature( pddev, REV_HAS_IRIG_TIME_TCR511PCI, PCPS_HAS_IRIG_TIME );
      check_feature( pddev, REV_HAS_RAW_IRIG_DATA_TCR511PCI, PCPS_HAS_RAW_IRIG_DATA );
      break;

    case PCPS_TYPE_PEX511:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_PEX511;
      // HR time support for the PEX511 requires both a certain ASIC
      // version plus a certain firmware version.
      if ( _pcps_asic_version_greater_equal( _pcps_ddev_asic_version( pddev ),
           PCI_ASIC_MAJOR_PEX511, PCI_ASIC_HR_TIME_MINOR_PEX511 ) )
        check_feature( pddev, REV_HAS_HR_TIME_PEX511, PCPS_HAS_HR_TIME );

      pcps_check_pex_irq_unsafe( pddev, REV_HAS_IRQ_FIX_MINOR_PEX511,
           PCI_ASIC_MAJOR_PEX511, PCI_ASIC_FIX_IRQ_MINOR_PEX511 );
      break;

    case PCPS_TYPE_TCR511PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_TCR511PEX;
      check_feature( pddev, REV_HAS_IRIG_CTRL_BITS_TCR511PEX, PCPS_HAS_IRIG_CTRL_BITS );
      check_feature( pddev, REV_HAS_IRIG_TIME_TCR511PEX, PCPS_HAS_IRIG_TIME );
      check_feature( pddev, REV_HAS_RAW_IRIG_DATA_TCR511PEX, PCPS_HAS_RAW_IRIG_DATA );
      pcps_check_pex_irq_unsafe( pddev, REV_HAS_IRQ_FIX_MINOR_TCR511PEX,
           PCI_ASIC_MAJOR_TCR511PEX, PCI_ASIC_FIX_IRQ_MINOR_TCR511PEX );
      break;

    case PCPS_TYPE_GPS170PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS170PEX;
      pddev->dev.cfg.features = PCPS_FEAT_GPS170PEX;
      pcps_check_pex_irq_unsafe( pddev, REV_HAS_IRQ_FIX_MINOR_GPS170PEX,
           PCI_ASIC_MAJOR_GPS170PEX, PCI_ASIC_FIX_IRQ_MINOR_GPS170PEX );
      break;

    case PCPS_TYPE_USB5131:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_USB5131;
      break;

    case PCPS_TYPE_TCR51USB:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_TCR51USB;
      check_feature( pddev, REV_HAS_IRIG_CTRL_BITS_TCR51USB, PCPS_HAS_IRIG_CTRL_BITS );
      check_feature( pddev, REV_HAS_IRIG_TIME_TCR51USB, PCPS_HAS_IRIG_TIME );
      check_feature( pddev, REV_HAS_RAW_IRIG_DATA_TCR51USB, PCPS_HAS_RAW_IRIG_DATA );
      break;

    case PCPS_TYPE_MSF51USB:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_MSF51USB;
      break;

    case PCPS_TYPE_PTP270PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_PTP270PEX;
      pddev->dev.cfg.features = PCPS_FEAT_PTP270PEX;
      break;

    case PCPS_TYPE_FRC511PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_FRC511PEX;
      pddev->dev.cfg.features = PCPS_FEAT_FRC511PEX;
      break;

    case PCPS_TYPE_TCR170PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_TCR170PEX;
      pddev->dev.cfg.features = PCPS_FEAT_TCR170PEX;
      break;

    case PCPS_TYPE_WWVB51USB:
      pddev->default_builtin_features = BUILTIN_FEAT_WWVB511;
      pddev->dev.cfg.features = PCPS_FEAT_WWVB51USB;
      break;

    case PCPS_TYPE_GPS180PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS180PEX;
      pddev->dev.cfg.features = PCPS_FEAT_GPS180PEX;
      break;

    case PCPS_TYPE_TCR180PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_TCR180PEX;
      pddev->dev.cfg.features = PCPS_FEAT_TCR180PEX;
      break;

    case PCPS_TYPE_DCF600USB:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_DCF600USB;
      break;

    case PCPS_TYPE_PZF180PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_PZF180PEX;
      pddev->dev.cfg.features = PCPS_FEAT_PZF180PEX;
      break;

    case PCPS_TYPE_TCR600USB:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_TCR600USB;
      break;

    case PCPS_TYPE_MSF600USB:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_MSF600USB;
      break;

    case PCPS_TYPE_WVB600USB:
      pddev->default_builtin_features = BUILTIN_FEAT_UNDEFINED;
      pddev->dev.cfg.features = PCPS_FEAT_WVB600USB;
      break;

    case PCPS_TYPE_GLN180PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_GLN180PEX;
      pddev->dev.cfg.features = PCPS_FEAT_GLN180PEX;
      break;

    case PCPS_TYPE_GPS180AMC:
      pddev->default_builtin_features = BUILTIN_FEAT_GPS180AMC;
      pddev->dev.cfg.features = PCPS_FEAT_GPS180AMC;
      break;

    case PCPS_TYPE_GNS181PEX:
      pddev->default_builtin_features = BUILTIN_FEAT_GNS181PEX;
      pddev->dev.cfg.features = PCPS_FEAT_GNS181PEX;
      // pddev->dev.cfg.features |= PCPS_HAS_SERNUM;  FIXME This is *not* supported!
      break;

    case PCPS_TYPE_TCR180USB:
      pddev->default_builtin_features = BUILTIN_FEAT_TCR180USB;
      pddev->dev.cfg.features = PCPS_FEAT_TCR180USB;
      break;

    default:
      _mbg_kdd_msg_1( MBG_LOG_WARN, "No feature detection for device %s",
                      _pcps_ddev_type_name( pddev ) );
      goto fail_with_cleanup;

  }  // switch

  setup_real_builtin_features( pddev );

  rc = setup_sernum_and_receiver_info( pddev );

  #if PCPS_LOG_STD_MSGS
    report_device_info( pddev );
  #endif

  if ( mbg_rc_is_error( rc ) )
    goto fail_with_cleanup;

  check_receiver_info_and_features( pddev );

  if ( _pcps_ddev_has_asic_features( pddev ) )
  {
  #if _PCPS_USE_MM_IO
    if ( _pcps_ddev_access_mode_mm( pddev ) )
    {
      pddev->asic_features = _mbg_mmrd32_to_cpu( &pddev->mm_asic_addr->features );
      cp = "MM";
    }
    else
  #endif  // _PCPS_USE_MM_IO
    {
      pddev->asic_features = _mbg_inp32_to_cpu( pddev, 0, _pcps_ddev_io_base_mapped( pddev, 0 )
                             + offsetof( PCI_ASIC, features ) );
      cp = "I/O";
    }

    #if DEBUG_IO || DEBUG_RSRC || REPORT_CFG
      _mbg_kdd_msg_2( MBG_LOG_DEBUG, "ASIC features 0x%08lX read via %s",
                      (ulong) pddev->asic_features, cp );
    #endif

    if ( pddev->asic_features & PCI_ASIC_HAS_MM_IO )
      pddev->dev.cfg.features |= PCPS_HAS_FAST_HR_TSTAMP;
    else
      if ( pddev->dev.cfg.features & PCPS_HAS_FAST_HR_TSTAMP )
      {
        // The device supports memory mapped time stamps by default.
        // However, this is not reflected by the ASIC features.
        #if REPORT_CFG
          _mbg_kdd_msg_0( MBG_LOG_WARN, "Warning: ASIC features don't reflect memory mapped time stamp support." );
        #endif
      }
  }

  #if _PCPS_USE_USB
    if ( _pcps_ddev_is_usb( pddev ) )
    {
      rc = check_usb_timing( pddev );

      if ( mbg_rc_is_error( rc ) )
      {
        _pcps_ddev_set_err_flags( pddev, PCPS_EF_IO_INIT );
        goto fail_with_cleanup;
      }
    }
  #endif   // _PCPS_USE_USB


  #if DEBUG_IO && defined( MBG_TGT_LINUX )
  {
    #if USE_LOCAL_IO_BUFFER
      PCPS_TIME t;
      PCPS_TIME *p_t = &t;
    #else
      PCPS_TIME *p_t = &pddev->io_buffer.pcps_time;
    #endif

    int sz = sizeof( *p_t );

    memset( p_t, 0, sz );
    _mbg_kdd_msg_0( MBG_LOG_INFO, "Reading current PCPS_TIME as test:" );
    rc = _pcps_read( pddev, PCPS_GIVE_TIME, p_t, sz );
    _mbg_kdd_msg_2( MBG_LOG_INFO, "Read time, sz %lu, returned %i", (ulong) sz, rc );
    _mbg_kdd_msg_4( MBG_LOG_INFO, "  sec100 %02u, sec %02u, min %02u, hour %02u",
                    p_t->sec100, p_t->sec, p_t->min, p_t->hour );
    _mbg_kdd_msg_4( MBG_LOG_INFO, "  mday %02u, wday %02u, month %02u, year %02u",
                    p_t->mday, p_t->wday, p_t->month, p_t->year );
    _mbg_kdd_msg_3( MBG_LOG_INFO, "  status 0x%02X, sig 0x%02X, offs_utc 0x%02X",
                    p_t->status, p_t->signal, p_t->offs_utc );
  }
  #endif

  #if REPORT_CFG
    _mbg_kdd_msg_3( MBG_LOG_DEBUG, "%s v%03X actual PCPS features: 0x%08lX",
                    _pcps_ddev_type_name( pddev ), _pcps_ddev_fw_rev_num( pddev ),
                    (ulong) _pcps_ddev_features( pddev ) );
  #endif

  rc = MBG_SUCCESS;
  goto out;


fail_with_cleanup:
  pcps_cleanup_device( pddev );
  goto out;

out_fail_generic:
  rc = MBG_ERR_GENERIC;

out:
  return rc;

}  // pcps_probe_device



/*HDR*/
/**
 * @brief Clean up function called by ::pcps_probe_device on error
 *
 * @param[in,out]  pddev  Pointer to the device structure
 *
 * @see ::pcps_probe_device
 */
void pcps_cleanup_device( PCPS_DDEV *pddev )
{
  set_access_mode( pddev, PCPS_ACC_MODE_NULL, 0, pcps_read_null );

  #if _PCPS_USE_RSRCMGR
    pcps_release_rsrcs( pddev );
  #endif

}  // pcps_cleanup_device



#if _PCPS_USE_PCI_BIOS

static /*HDR*/
/**
 * @brief Read PCI recources for a PCI device on non-PnP systems
 *
 * This function should be called by the probe routines of any
 * target-specific kernel drivers.
 * If the device is supported then all specific information including
 * supported features is read from the device and stored in sub-structures
 * of the device structure addressed by pdev.
 *
 * @param[in]      bus_num      The PCI bus number
 * @param[in]      dev_fnc_num  The PCI device/function number
 * @param[in,out]  pddev        Pointer to the device structure to be set up
 *
 * @return 0 on success, or a combination of @ref PCPS_ERR_FLAG_MASKS masks on error
 */
PCPS_ERR_FLAGS pcps_read_pci_rsrc( PCPS_BUS_NUM bus_num,
                                   PCPS_SLOT_NUM dev_fnc_num,
                                   PCPS_DDEV *pddev )
{
  PCPS_ERR_FLAGS err_flags = 0;
  uchar irq;
  short pci_rc;
  PCI_DWORD dw;
  int i;

  // Clear resources
  memset( &pddev->rsrc_info, 0, sizeof( pddev->rsrc_info ) );

  for ( i = 0; i < MAX_PCPS_RSRC; i++ )
  {
    pci_rc = _mbg_pci_read_cfg_dword( bus_num, dev_fnc_num,
                PCI_CS_BASE_ADDRESS_0 + i * sizeof( uint32_t ), &dw );

    if ( pci_rc != PCI_SUCCESS )
      break;

    if ( dw == 0 )   // base address register not used
      continue;

    if ( dw & 0x0001 )   // is an I/O resource
    {
      if ( dw & 0xFFFF0000UL )
      {
        // The PCI interface chip is not initialized. This
        // should occur ONLY at the first-time installation
        // at the factory.
        err_flags |= PCPS_EF_IO_INIT;
        goto out;
      }

      pcps_add_rsrc_io( pddev, (uint16_t) ( dw & ~0x0001 ), 0 );
    }
    else
      pcps_add_rsrc_mem( pddev, dw, 0 );  //##++ range length?
  }

  // Read the interrupt line assigned to the clock.
  // The standard functions, however, don't use any
  // interrupt.
  pci_rc = _mbg_pci_read_cfg_byte( bus_num, dev_fnc_num,
                                   PCI_CS_INTERRUPT_LINE, &irq );

  if ( pci_rc == PCI_SUCCESS )
    pcps_add_rsrc_irq( pddev, irq );

out:
  return err_flags;

}  // pcps_read_pci_rsrc



static /*HDR*/
/**
 * @brief Enable a PCI device which has not yet been enabled
 *
 * In PnP systems only PCI devices that are required for booting
 * are always enabled by the PCI BIOS. Other PCI devices may only
 * be fully enabled if a PC BIOS setup option "PNP OS installed"
 * is set to "NO".
 * If this option is set to "YES" then I/O access to the board
 * may still be disabled, and a PnP operating system is expected
 * to enable remaining devices when it boots up.
 * Drivers can call this function to enable I/O and memory access,
 * if required, in case this hasn't been done by the PCI BIOS
 * or operating system.
 *
 * @param[in]  bus_num       The PCI bus number
 * @param[in]  dev_fnc_num   The PCI device/function number
 * @param[in]  num_rsrc_mem  If != 0 then memory access is also enabled
 *
 * @return 0 on success, or a combination of @ref PCPS_ERR_FLAG_MASKS masks on error
 */
PCPS_ERR_FLAGS pcps_enable_pci_dev( PCPS_BUS_NUM bus_num,
                                    PCPS_SLOT_NUM dev_fnc_num,
                                    int num_rsrc_mem )
{
  PCPS_ERR_FLAGS err_flags = 0;
  uint16_t pci_command;      // current value read from the PCI cfg "command" register
  uint16_t new_pci_command;  // new value to be written to the PCI cfg "command" register
  int pci_rc;


  pci_rc = _mbg_pci_read_cfg_word( bus_num, dev_fnc_num,
                                   PCI_CS_COMMAND, &pci_command );
  new_pci_command = pci_command | PCI_CMD_ENB_IO_ACC;

  if ( num_rsrc_mem )
    new_pci_command |= PCI_CMD_ENB_MEM_ACC;

  if ( new_pci_command != pci_command )
  {
    pci_rc = _mbg_pci_write_cfg_word( bus_num, dev_fnc_num,
                                      PCI_CS_COMMAND, pci_command );
    if ( pci_rc != PCI_SUCCESS )
    {
      err_flags |= PCPS_EF_IO_ENB;

      _mbgddmsg_1( DEBUG_DEV_INIT, MBG_LOG_ERR,
                   "PCI enable device returned %d", pci_rc );
    }
  }

  return err_flags;

}  // pcps_enable_pci_dev



/*HDR*/
/**
 * @brief Setup and start a PCI device in a non-PnP system
 *
 * @param[in,out]  pddev        Pointer to the device structure to be set up
 * @param[in]      bus_num      The PCI bus number returned by the PCI BIOS
 * @param[in]      dev_fnc_num  The PCI device/function number returned by the PCI BIOS
 */
int pcps_setup_and_start_pci_dev( PCPS_DDEV *pddev,
            PCPS_BUS_NUM bus_num, PCPS_SLOT_NUM dev_fnc_num )
{
  PCPS_ERR_FLAGS err_flags;
  int rc;

  err_flags = pcps_read_pci_rsrc( bus_num, dev_fnc_num, pddev );
  _pcps_ddev_set_err_flags( pddev, err_flags );

  if ( !( err_flags & PCPS_EF_IO_INIT ) )
  {
    err_flags = pcps_enable_pci_dev( bus_num, dev_fnc_num,
                  pddev->rsrc_info.num_rsrc_mem );
    _pcps_ddev_set_err_flags( pddev, err_flags );
  }

  rc = pcps_probe_device( pddev, bus_num, dev_fnc_num );

  return rc;

}  // pcps_setup_and_start_pci_dev



/*HDR*/
/**
 * @brief Detect and initialize PCI devices in a non-PnP system
 *
 * @param[in]  ddev_init_fnc     Pointer to function called to allocate a device structure for each detected device.
 * @param[in]  ddev_cleanup_fnc   Pointer to function called if the device structure needs to be de-allocated in case of error.
 * @param[in]  vendor_id     The PCI vendor ID code.
 * @param[in]  dev_type      An array with known PCI devices for the specified vendor ID
 * @param[in]  n_dev_types   The number of entries in the PCI device table
 */
void pcps_detect_pci_devices( PCPS_DDEV_INIT_FNC *ddev_init_fnc,
                              PCPS_DDEV_CLEANUP_FNC *ddev_cleanup_fnc,
                              ushort vendor_id, PCPS_DEV_TYPE dev_type[],
                              int n_dev_types )
{
  #if defined( MBG_TGT_QNX )
    #if defined( MBG_TGT_QNX_NTO )
      unsigned int pci_handle;  // specific to QNX Neutrino
    #endif
    unsigned int pci_hardware_mechanism;
    unsigned int pci_last_bus_number;
    unsigned int pci_interface_level_version;
  #elif defined( MBG_TGT_LINUX )
    // not yet supported/used
  #else
    uchar pci_hardware_mechanism;
    uchar pci_last_bus_number;
    ushort pci_interface_level_version;
  #endif
  ushort type_idx;
  int rc;


  #ifdef _mbg_pci_fnc_init
    rc = _mbg_pci_fnc_init();

    if ( rc != PCI_SUCCESS )
      return;
  #endif


  // See if PCI BIOS is installed on the machine.
  rc = _mbg_pci_find_bios( &pci_hardware_mechanism,
                           &pci_interface_level_version,
                           &pci_last_bus_number
                         );

  if ( rc == PCI_SUCCESS )
  {
    // PCI BIOS is installed, now try to find a PCI clock with
    // known ID (the list is terminated with a ID of 0).
    for ( type_idx = 0; type_idx < n_dev_types; type_idx++ )
    {
      ushort dev_idx;
      PCPS_DEV_TYPE *p = &dev_type[type_idx];

      if ( !( p->bus_flags & PCPS_BUS_PCI ) )
        continue;


      for ( dev_idx = 0; ; dev_idx++ )
      {
        PCPS_DDEV *pddev;
        #if defined( MBG_TGT_QNX )
          unsigned bus_num;
          unsigned dev_fnc_num;
        #else
          uchar bus_num;
          uchar dev_fnc_num;
        #endif

        rc = _mbg_pci_find_device( p->dev_id, vendor_id,
                                   dev_idx, &bus_num, &dev_fnc_num );

        if ( rc != PCI_SUCCESS )
          break;  // go to try next device ID


        _mbgddmsg_2( DEBUG_DEV_INIT, MBG_LOG_INFO, "Found PCI device %s (0x%04X)",
                     p->name, p->dev_id );

        // New device found, try to add to list.
        rc = ddev_init_fnc( &pddev );

        if ( mbg_rc_is_success( rc ) )  // Setup only if successfull.
        {
          #if _PCPS_USE_PCI_PNP  //##++
            // This can be used to test the PNP functions in a
            // non-PNP environment.
            pcps_setup_ddev( pddev, PCPS_BUS_PCI, p->dev_id );
          #else
            pddev->dev.type = *p;
          #endif

          pcps_setup_and_start_pci_dev( pddev, bus_num, dev_fnc_num );

          #if !_ACCEPT_UNINITD_CLOCKS
            if ( pddev->dev.cfg.err_flags )
            {
              _mbgddmsg_1( DEBUG_DEV_INIT, MBG_LOG_WARN,
                           "Remove PCI device: err_flags " FMT_08X "h",
                           (ulong) pddev->dev.cfg.err_flags );

              if ( ddev_cleanup_fnc )
                ddev_cleanup_fnc( pddev );
            }
          #else
            (void) ddev_cleanup_fnc;  // avoid compiler warning
          #endif
        }
      }
    }
  }

  #ifdef _mbg_pci_fnc_deinit
    _mbg_pci_fnc_deinit();
  #endif

}  // pcps_detect_pci_devices

#endif  // _PCPS_USE_PCI_BIOS



#if !_PCPS_USE_ISA_PNP

/*HDR*/
/**
 * @brief Detect and initialize ISA devices in a non-PnP system
 *
 * @param[in]  ddev_init_fnc     Pointer to function called to allocate a device structure for each detected device.
 * @param[in]  ddev_cleanup_fnc   Pointer to function called if the device structure needs to be de-allocated in case of error.
 * @param[in]  ddev_register_fnc  Pointer to function called to register a detected device.
 * @param[in]  isa_ports     An array with potential I/O base addresses for ISA devices.
 * @param[in]  isa_irqs      An array with potential IRQ numbers assigned to ISA devices.
 */
void pcps_detect_isa_devices( PCPS_DDEV_INIT_FNC *ddev_init_fnc,
                              PCPS_DDEV_CLEANUP_FNC *ddev_cleanup_fnc,
                              PCPS_DDEV_REGISTER_FNC *ddev_register_fnc,
                              int isa_ports[PCPS_MAX_ISA_CARDS],
                              int isa_irqs[PCPS_MAX_ISA_CARDS] )
{
  int *p_port = isa_ports;
  int *p_irq = isa_irqs;
  PCPS_DDEV *pddev;
  int i;
  int rc;

  if ( p_port == NULL )   // No list has been passed
    return;               // so don't try to detect ISA clocks.


  for( i = 0; i < PCPS_MAX_ISA_CARDS;
       i++, p_port++, p_irq ? ( p_irq++ ) : p_irq )
  {
    int irq_num;

    if ( *p_port == 0 )
      continue;

    irq_num = p_irq ? *p_irq : PCPS_IRQ_NUM_UNDEFINED;

    _mbgddmsg_2( DEBUG_DEV_INIT, MBG_LOG_INFO,
                 "Check ISA device at port " FMT_03X "h, irq %d",
                 *p_port, irq_num );

    // Assume ISA device is available,
    // but clock type is unknown, yet.
    rc = ddev_init_fnc( &pddev );

    if ( mbg_rc_is_success( rc ) )  // Setup only if successfull.
    {
      pddev->dev.type.bus_flags = PCPS_BUS_ISA;

      // Set up basic cfg for ISA devices.
      pcps_add_rsrc_io( pddev, (uint16_t) *p_port, PCPS_NUM_PORTS_ISA );

      if ( irq_num != PCPS_IRQ_NUM_UNDEFINED )
        pcps_add_rsrc_irq( pddev, (uint16_t) *p_irq );

      // Init the device structure. This includes registration
      // of I/O ports with the OS's resource manager (if supported),
      // and reading the firmware ID.
      pcps_probe_device( pddev, 0, 0 );

      // If an error has occurred, then remove the last
      // device from the list and try next.
      if ( pddev->dev.cfg.err_flags )
      {
        _mbgddmsg_1( DEBUG_DEV_INIT, MBG_LOG_WARN,
                     "ISA device not found: err_flags " FMT_08X "h",
                     (ulong) pddev->dev.cfg.err_flags );
        if ( ddev_cleanup_fnc )
          ddev_cleanup_fnc( pddev );

        continue;
      }

      // Register the device with the OS, if required.
      if ( ddev_register_fnc )
        ddev_register_fnc( pddev );  //##++
    }
  }

}  // pcps_detect_isa_devices

#endif  //!_PCPS_USE_ISA_PNP



#if !_PCPS_USE_PNP

static /*HDR*/
/**
 * @brief Detect all bus-level devices in a non-PnP system, use specific alloc/cleanup functions
 *
 * @note If a DOS TSR is installed, be sure it is disabled (BUSY flag set)
 * when this function is called.
 *
 * @param[in]  ddev_init_fnc     Pointer to function called to allocate a device structure for each detected device.
 * @param[in]  ddev_cleanup_fnc  Pointer to function called if the device structure needs to be de-allocated in case of error.
 * @param[in]  isa_ports         An array with potential I/O base addresses for ISA devices.
 * @param[in]  isa_irqs          An array with potential IRQ numbers assigned to ISA devices.
 */
void _MBG_INIT_CODE_ATTR pcps_detect_devices_init( PCPS_DDEV_INIT_FNC *ddev_init_fnc,
                                                   PCPS_DDEV_CLEANUP_FNC *ddev_cleanup_fnc,
                                                   int isa_ports[PCPS_MAX_ISA_CARDS],
                                                   int isa_irqs[PCPS_MAX_ISA_CARDS] )
{
  #if defined( MBG_TGT_OS2 )
    rsrc_register_driver();  // register driver and init resource manager
  #endif

  #if _PCPS_USE_PCI_BIOS
    pcps_detect_pci_devices( ddev_init_fnc, ddev_cleanup_fnc, PCI_VENDOR_MEINBERG,
                             pcps_dev_type, N_PCPS_DEV_TYPE );
  #endif

  #if _PCPS_USE_MCA
    pcps_detect_mca_devices( ddev_init_fnc );
  #endif

  #if !_PCPS_USE_ISA_PNP
    pcps_detect_isa_devices( ddev_init_fnc, ddev_cleanup_fnc, NULL, isa_ports, isa_irqs );
  #endif

}  // pcps_detect_devices_init



/*HDR*/
/**
 * @brief Detect all bus-level devices in a non-PnP system
 *
 * @note If a DOS TSR is installed, be sure it is disabled (BUSY flag set)
 * when this function is called.
 *
 * @param[in]  isa_ports     An array with potential I/O base addresses for ISA devices.
 * @param[in]  isa_irqs      An array with potential IRQ numbers assigned to ISA devices.
 */
void _MBG_INIT_CODE_ATTR pcps_detect_devices( int isa_ports[PCPS_MAX_ISA_CARDS],
                                              int isa_irqs[PCPS_MAX_ISA_CARDS] )
{
  pcps_detect_devices_init( pcps_init_ddev, pcps_cleanup_ddev, isa_ports, isa_irqs );

}  // pcps_detect_devices

#endif  // !_PCPS_USE_PNP


