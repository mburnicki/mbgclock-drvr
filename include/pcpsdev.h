
/**************************************************************************
 *
 *  $Id: pcpsdev.h 1.61 2019/12/20 12:28:25 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions used to share information on radio clock devices
 *    between device drivers which have direct access to the hardware
 *    devices and user space programs which evaluate and present that
 *    information.
 *
 *    At the bottom of the file there are some macros defined which
 *    should be used to access the structures to extract characteristics
 *    of an individual clock.
 *
 * -----------------------------------------------------------------------
 *  $Log: pcpsdev.h $
 *  Revision 1.61  2019/12/20 12:28:25  martin
 *  New type PCPS_TYPE_NUM.
 *  Revision 1.60  2019/07/31 15:44:31  martin
 *  Doxygen changes.
 *  Revision 1.59  2019/05/08 11:27:36  martin
 *  Use new symbol PCPS_IRQ_NUM_UNDEFINED.
 *  Revision 1.58  2018/11/02 14:58:43  martin
 *  Updated a comment.
 *  Revision 1.57  2018/09/21 15:04:23  martin
 *  Added definitions for TCR180USB.
 *  Added define MBG_DEV_NAME_FMT and macro _tlv_feat_buffp();
 *  Defined a print format for firmware versions.
 *  Renamed macro _must_do_fw_workaround_20ms()
 *  to _pcps_fw_has_20ms_bug().
 *  More unified resource handling.
 *  Renamed a number of symbols.
 *  Updated naming for device feature stuff.
 *  Added doxygen comments.
 *  Revision 1.56  2017/07/04 16:31:08  martin
 *  New types PCPS_CLOCK_NAME and MBG_DEV_NAME.
 *  Definitions used with new feature check implementation.
 *  Changed some macros and definitions to clean up
 *  I/O port usage and storage.
 *  Moved some definitions used with IOCTLs to mbgioctl.h.
 *  Revision 1.55  2017/04/25 11:36:40  martin
 *  Renamed GRC181PEX to GNS181PEX.
 *  Revision 1.54  2017/01/27 09:10:57  martin
 *  Support GPS180AMC.
 *  Support GRC181PEX.
 *  Support GPIO ports.
 *  IRIG TX support for GPS180PEX and TCR180PEX only
 *  if GPS_HAS_IRIG_TX flag is set.
 *  New type PCPS_FW_REV_NUM.
 *  Moved some code to new module mbgsystm.h.
 *  Moved NANO_TIME_64 to gpsdefs.h.
 *  Moved inline function num_bits_set() to cfg_hlp.h.
 *  Added macro _ri_addr().
 *  Added macro _pcps_has_ri_xmr().
 *  Fixed macro syntax.
 *  Doxygen stuff.
 *  Removed trailing spaces.
 *  Cleanup.
 *  Revision 1.53  2013/11/08 08:46:00  martin
 *  Doxygen comment updates.
 *  Revision 1.52  2013/09/26 09:06:47Z  martin
 *  Support GLN180PEX and GNSS API.
 *  Added inline fnc num_bits_set().
 *  Revision 1.51  2013/01/25 15:44:21  martin
 *  Added inline function setup_hr_time_cycles_from_timestamp_cycles() which sets
 *  up a PCPS_HR_TIME_CYCLES structure from PCPS_TIME_STAMP_CYCLES.
 *  Revision 1.50  2012/10/02 19:00:46  martin
 *  Support GPS180PEX, TCR180PEX, and PZF180PEX.
 *  Support DCF600USB, TCR600USB, MSF600USB, and WVB600USB.
 *  Runtime support for precise time API introduced with Windows 8.
 *  This does not yet for x64 builds.
 *  There are some g++ versions which fail to compile source code using
 *  the macros provided by Linux to define IOCTL codes. If only the API
 *  functions are called by an application then the IOCTL codes aren't
 *  required anyway, so we just avoid inclusion of mbgioctl.h.
 *  However, some IOCTL related definitions are required anyway, so
 *  they have been moved to this file which is always included.
 *  Bug fix: Use negative sign for delay in KeDelayExecutionThread()
 *  Support on-board event logs.
 *  Moved macro _must_do_fw_workaround_20ms() here.
 *  New macro _pcps_has_debug_status().
 *  Added some macros to test if specific stat_info stuff is supported.
 *  Moved some definitions useful for configuration tools to new file cfg_hlp.h.
 *  Moved IA64 includes from pcpsdev.h to mbgpccyc.h.
 *  Added macros _pcps_has_corr_info() and _pcps_has_tr_distance().
 *  Extended bus flag for USB v2 and macro _pcps_is_usb_v2().
 *  New feature ..._HAS_PZF and macro _pcps_has_pzf().
 *  Moved PC cycles stuff to an new extra header.
 *  Cleaned up handling of pragma pack().
 *  Introduced generic MBG_SYS_TIME with nanosecond resolution.
 *  Support struct timespec under Linux, if available.
 *  Use MBG_TGT_KERNEL instead of _KDD_.
 *  Added PTP unicast master configuration stuff.
 *  For compatibility use cpu_counter() instead of cpu_counter_serializing() under NetBSD.
 *  Optionally support timespec for sys time (USE_TIMESPEC).
 *  Support FreeBSD and NetBSD.
 *  Moved MBG_TGT_SUPP_MEM_ACC definition here.
 *  Moved status port register definitions to pcpsdefs.h.
 *  Features are now defined using enum and bit masks.
 *  Added initializer for feature names (used for debug).
 *  Added macro _pcps_features().
 *  Implemented portable mbg_get_sys_uptime() and mbg_sleep_sec()
 *  functions and associated types.
 *  Revision 1.49  2010/06/30 13:03:48  martin
 *  Use new preprocessor symbol MBG_ARCH_X86.
 *  Use ulong port addresses for all platforms but x86.
 *  Support mbg_get_pc_cycles() for IA64, but mbg_get_pc_cycles_frequency()
 *  is not yet supported.
 *  Don't pack interface structures on Sparc and IA64 architecture.
 *  Revision 1.48  2010/04/26 14:47:42  martin
 *  Define symbol MBG_PC_CYCLES_SUPPORTED if this is the case.
 *  Revision 1.47  2010/01/12 14:03:22  daniel
 *  Added definitions to support reading the raw IRIG data bits.
 *  Revision 1.46  2009/09/29 15:10:35Z  martin
 *  Support generic system time, and retrieving time discipline info.
 *  Added _pcps_has_fast_hr_timestamp() macro and associated feature flag.
 *  Revision 1.45  2009/06/19 12:15:18  martin
 *  Added has_irig_time feature and associated macros.
 *  Revision 1.44  2009/06/08 19:30:48  daniel
 *  Account for new features PCPS_HAS_LAN_INTF and
 *  PCPS_HAS_PTP.
 *  Revision 1.43  2009/04/08 08:26:20  daniel
 *  Define firmware version at which the TCR511PCI starts
 *  to support IRIG control bits.
 *  Revision 1.42  2009/03/19 14:58:47Z  martin
 *  Tmp. workaround in mbg_delta_pc_cycles() under SPARC which might
 *  generate bus errors due to unaligned access.
 *  Revision 1.41  2009/03/16 16:01:22  martin
 *  Support reading IRIG control function bits.
 *  Revision 1.40  2009/03/13 09:13:39  martin
 *  Support new features .._has_time_scale() and .._has_utc_parm().
 *  Moved some inline functions dealing with MBG_PC_CYCLES
 *  from mbgdevio.h here.
 *  Merged the code from _pcps_get_cycles() and _pcps_get_cycles_frequency()
 *  to the mbg_get_pc_cycles...() inline functions which now replace the
 *  _pcps_get_cycles...() macros.
 *  Fixed cycles code for non-x86 architectures.
 *  Revision 1.39  2008/12/05 16:24:24Z  martin
 *  Changed MAX_PARM_STR_TYPE from 10 to 20.
 *  Added support for WWVB signal source.
 *  Support new devices PTP270PEX, FRC511PEX, TCR170PEX, and WWVB51USB.
 *  Added macros _pcps_is_ptp(), _pcps_is_frc(), and _pcps_is_wwvb().
 *  Defined firmware version numbers which fix an IRQ problem with PEX511,
 *  TCR511PEX, and GPS170PEX cards. The fix also requires specific ASIC
 *  versions specified in pci_asic.h.
 *  Defined firmware versions at which PCI511 and PEX511 start
 *  to support HR time.
 *  Support mapped I/O resources.
 *  Changed MBG_PC_CYCLES type for Windows to int64_t.
 *  Renamed MBG_VIRT_ADDR to MBG_MEM_ADDR.
 *  Added MBG_PC_CYCLES_FREQUENCY type.
 *  Added definition of PCPS_TIME_STAMP_CYCLES.
 *  Added PCPS_IRQ_STAT_INFO type and associated flags.
 *  Added macros to convert the endianess of structures.
 *  Added macros _pcps_fw_rev_num_major() and _pcps_fw_rev_num_minor().
 *  Made irq_num signed to use -1 for unassigned IRQ numbers.
 *  Revision 1.38  2008/01/17 10:12:34  daniel
 *  Added support for TCR51USB and MSF51USB.
 *  New type MBG_VIRT_ADDR to specify virtual address values.
 *  New struct PCPS_MAPPED_MEM
 *  Cleanup for PCI ASIC version and features.
 *  Added macros _pcps_is_msf(), _pcps_is_lwr(),
 *  _psps_has_asic_version(), _pcps_has_asic_features().
 *  Revision 1.37  2008/01/17 09:58:11Z  daniel
 *  Made comments compatible for doxygen parser.
 *  No sourcecode changes.
 *  Revision 1.36  2007/09/26 09:34:38Z  martin
 *  Added support for USB in general and new USB device USB5131.
 *  Added new types PCPS_DEV_ID and PCPS_REF_TYPE.
 *  Removed old PCPS_ERR_... codes. Use MBG_ERR_... codes
 *  from mbgerror.h instead. The old values haven't changed.
 *  Revision 1.35  2007/07/17 08:22:47Z  martin
 *  Added support for TCR511PEX and GPS170PEX.
 *  Revision 1.34  2007/07/16 12:50:41Z  martin
 *  Added support for PEX511.
 *  Modified/renamed some macros and symbols.
 *  Revision 1.33  2007/03/02 09:40:04Z  martin
 *  Changes due to renamed library symbols.
 *  Removed obsolete inclusion of headers.
 *  Preliminary support for *BSD.
 *  Preliminary support for USB.
 *  Revision 1.32  2006/10/23 08:47:55Z  martin
 *  Don't use abs() in _pcps_ref_offs_out_of_range() since this might
 *  not work properly for 16 bit integers and value 0x8000.
 *  Revision 1.31  2006/06/14 12:59:13Z  martin
 *  Added support for TCR511PCI.
 *  Revision 1.30  2006/04/05 14:58:41  martin
 *  Support higher baud rates for PCI511.
 *  Revision 1.29  2006/04/03 07:29:07Z  martin
 *  Added a note about the missing PCPS_ST_IRQF signal
 *  on PCI510 cards.
 *  Revision 1.28  2006/03/10 10:32:56Z  martin
 *  Added support for PCI511.
 *  Added support for programmable pulse outputs.
 *  Revision 1.27  2005/11/04 08:48:00Z  martin
 *  Added support for GPS170PCI.
 *  Revision 1.26  2005/06/02 08:34:38Z  martin
 *  New types MBG_DBG_PORT, MBG_DBG_DATA.
 *  Revision 1.25  2005/05/03 10:04:14  martin
 *  Added macro _pcps_is_pci_amcc().
 *  Revision 1.24  2005/03/29 12:58:19Z  martin
 *  Support GENERIC_IO feature.
 *  Revision 1.23  2004/12/09 11:03:37Z  martin
 *  Support configuration of on-board frequency synthesizer.
 *  Revision 1.22  2004/11/09 12:57:52Z  martin
 *  Redefined interface data types using C99 fixed-size definitions.
 *  Added support for TCR167PCI.
 *  New macro _pcps_has_gps_data().
 *  New type PCPS_STATUS_PORT.
 *  Removed obsolete inclusion of asm/timex.h for Linux.
 *  Revision 1.21  2004/09/06 15:19:49Z  martin
 *  Support a GPS_DATA interface where sizes are specified
 *  by 16 instead of the original 8 bit quantities, thus allowing
 *  to transfer data blocks which exceed 255 bytes.
 *  Modified inclusion of header files under Linux.
 *  Modified definition of MBG_PC_CYCLES for Linux.
 *  Revision 1.20  2004/04/14 09:09:11  martin
 *  Source code cleanup.
 *  Revision 1.19  2004/04/07 09:49:14Z  martin
 *  Support new feature PCPS_HAS_IRIG_TX.
 *  New macros _pcps_has_irig(), _pcps_has_irig_tx().
 *  Revision 1.18  2004/01/14 11:02:14Z  MARTIN
 *  Added formal type MBG_PC_CYCLES for OS/2,
 *  though it's not really required or used.
 *  Revision 1.17  2003/12/22 15:40:16  martin
 *  Support higher baud rates for TCR510PCI and PCI510.
 *  Supports PCPS_HR_TIME for TCR510PCI.
 *  New structures used to read device time together with associated
 *  PC CPU cycles.
 *  For Win32, differentiate between kernel mode and non-kernel mode.
 *  Moved some definitions here from mbgdevio.h.
 *  New type PCPS_ASIC_VERSION.
 *  New macro _pcps_ref_offs_out_of_range().
 *  Revision 1.16  2003/06/19 09:48:30Z  MARTIN
 *  Renamed symbols ..clr_cap_buffer to ..clr_ucap_buffer.
 *  New macro _pcps_has_ucap().
 *  New definitions to support cmds PCPS_GIVE_UCAP_ENTRIES
 *  and PCPS_GIVE_UCAP_EVENT.
 *  Revision 1.15  2003/04/15 09:57:25  martin
 *  New typedefs ALL_STR_TYPE_INFO, ALL_PORT_INFO,
 *  RECEIVER_PORT_CFG.
 *  Revision 1.14  2003/04/09 14:07:01Z  martin
 *  Supports PCI510, GPS169PCI, and TCR510PCI,
 *  and new PCI_ASIC used by those devices.
 *  Renamed macro _pcps_is_irig() to _pcps_is_irig_rx().
 *  New macros _pcps_has_ref_offs(), _pcps_has_opt_flags().
 *  Fixed macro _pcps_has_hr_time().
 *  New type PCPS_BUS_FLAGS.
 *  Preliminary support for PCPS_TZDL.
 *  Revision 1.13  2002/08/09 07:19:49  MARTIN
 *  Moved definition of ref time sources to pcpsdefs.h.
 *  New feature PCPS_CAN_CLR_CAP_BUFF and
 *  associated macro _pcps_can_clr_cap_buff().
 *  New macros _pcps_is_irig(), _pcps_has_signal(),
 *  _pcps_has_mod().
 *  Revision 1.12  2002/02/19 09:22:53  MARTIN
 *  Added definitions for the maximum number of clocks' serial ports
 *  and string types that can be handled by the configuration programs.
 *  Revision 1.11  2002/02/01 11:36:58  MARTIN
 *  Added new definitions for GPS168PCI.
 *  Inserted definitions of firmware REV_NUMs  for supported features
 *  which had previously been defined in pcpsdefs.h.
 *  Include use_pack.h.
 *  Updated comments.
 *  Source code cleanup.
 *  Revision 1.10  2001/11/30 09:52:48  martin
 *  Added support for event_time which, however, requires
 *  a custom GPS firmware.
 *  Revision 1.9  2001/10/16 10:11:14  MARTIN
 *  New Macro _pcps_has_serial_hs() which determines whether
 *  DCF77 clock supports baud rate higher than default.
 *  Re-arranged order of macro definitions.
 *  Revision 1.8  2001/09/03 07:15:05  MARTIN
 *  Added macro to access the firmware revision number.
 *  Cleaned up macro syntax.
 *  Added some comments.
 *  Revision 1.7  2001/08/30 13:20:04  MARTIN
 *  New macro to mark a PCPS_TIME variable  as unread.
 *  New macro to check if a PCPS_TIME variable  is unread.
 *  Revision 1.6  2001/03/15 15:45:01  MARTIN
 *  Added types PCPS_ERR_FLAGS, PCPS_BUS_NUM, PCPS_SLOT_NUM.
 *  Revision 1.5  2001/03/01 13:53:10  MARTIN
 *  Initial version for the new driver library.
 *
 **************************************************************************/

#ifndef _PCPSDEV_H
#define _PCPSDEV_H

#include <mbg_tgt.h>
#include <mbgtime.h>
#include <mbgsystm.h>
#include <mbgpccyc.h>
#include <pcpsdefs.h>
#include <gpsdefs.h>
#include <usbdefs.h>
#include <use_pack.h>

#if !defined( MBG_TGT_KERNEL )
  #include <string.h>
#endif

#if defined( MBG_TGT_LINUX )

  #if !defined( MBG_TGT_KERNEL )
    #include <unistd.h>
  #endif

#elif defined( MBG_TGT_FREEBSD )

  #if defined( MBG_TGT_KERNEL )
    #include <sys/sysproto.h>
    #include <sys/pcpu.h>
    #include <sys/param.h>
    #include <sys/systm.h>
    #include <sys/proc.h>
  #else
    #include <unistd.h>
    #include <sys/time.h>
  #endif

#elif defined( MBG_TGT_NETBSD )

  #if !defined( MBG_TGT_KERNEL )
    #include <unistd.h>
    #include <sys/time.h>
  #endif

#elif defined( MBG_TGT_QNX_NTO )

  #include <unistd.h>

#elif defined( MBG_TGT_DOS )

  #include <dos.h>     // for delay()

#endif


/* Start of header body */

#if defined( _USE_PACK )
  #if !defined( _NO_USE_PACK_INTF )
    #pragma pack( 1 )      // set byte alignment
    #define _USING_BYTE_ALIGNMENT
  #endif
#endif


#if defined( MBG_TGT_LINUX )

  #define MBG_TGT_SUPP_MEM_ACC  1

#elif defined( MBG_TGT_BSD )

  #define MBG_TGT_SUPP_MEM_ACC  1

#elif defined( MBG_TGT_WIN32 )

  #define MBG_TGT_SUPP_MEM_ACC  1

#elif defined( MBG_TGT_DOS )

#endif


#if !defined( MBG_TGT_SUPP_MEM_ACC )
  #define MBG_TGT_SUPP_MEM_ACC  0
#endif


typedef uint8_t MBG_DBG_DATA;
typedef uint16_t MBG_DBG_PORT;



/**
 * @brief System time plus associated cycles counter values
 *
 * The structure holds a system timestamp in a format depending on the target OS
 * plus two cycles counter values which can be taken before and after reading
 * the system time. These cycles values can be used to determine the execution
 * time required to read the system time.
 *
 * Limitations of the operating system need to be taken into account,
 * e.g. the Windows system time may increase once every ~16 ms or ~1 ms only,
 * depending on the Windows version.
 */
typedef struct
{
  MBG_PC_CYCLES cyc_before;   ///< cycles count before sys time is read
  MBG_PC_CYCLES cyc_after;    ///< cycles count after sys time has been read
  MBG_SYS_TIME sys_time;      ///< system time stamp

} MBG_SYS_TIME_CYCLES;

#define _mbg_swab_mbg_sys_time_cycles( _p )      \
do                                               \
{                                                \
  _mbg_swab_mbg_pc_cycles( &(_p)->cyc_before );  \
  _mbg_swab_mbg_pc_cycles( &(_p)->cyc_after );   \
  _mbg_swab_mbg_sys_time( &(_p)->sys_time );     \
} while ( 0 )



/**
 * @defgroup group_bus_flag_masks BUS flag masks
 *
 * @anchor PCPS_BUS_FLAG_MASKS
 *
 * @{ */

// The following flags describe the bus types which are
// supported by the plugin clocks.
#define PCPS_BUS_ISA        0x0001    ///< IBM compatible PC/AT ISA bus
#define PCPS_BUS_MCA        0x0002    ///< IBM PS/2 micro channel
#define PCPS_BUS_PCI        0x0004    ///< PCI
#define PCPS_BUS_USB        0x0008    ///< USB


// The flags below are or'ed to the PC_BUS_PCI code
// in order to indicate which PCI interface chip is used
// on a PCI card. If no flag is set then the S5933 chip is
// installed which has been used for the first generation
// of Meinberg PCI cards.
#define PCPS_BUS_PCI_CHIP_S5920    0x8000    ///< S5920 PCI interface chip.
#define PCPS_BUS_PCI_CHIP_ASIC     0x4000    ///< Meinberg's own PCI interface chip.
#define PCPS_BUS_PCI_CHIP_PEX8311  0x2000    ///< PEX8311 PCI Express interface chip
#define PCPS_BUS_PCI_CHIP_MBGPEX   0x1000    ///< Meinberg's own PCI Express interface chip

// The constants below combine the PCI bus flags:
#define PCPS_BUS_PCI_S5933    ( PCPS_BUS_PCI )
#define PCPS_BUS_PCI_S5920    ( PCPS_BUS_PCI | PCPS_BUS_PCI_CHIP_S5920 )
#define PCPS_BUS_PCI_ASIC     ( PCPS_BUS_PCI | PCPS_BUS_PCI_CHIP_ASIC )
#define PCPS_BUS_PCI_PEX8311  ( PCPS_BUS_PCI | PCPS_BUS_PCI_CHIP_PEX8311 )
#define PCPS_BUS_PCI_MBGPEX   ( PCPS_BUS_PCI | PCPS_BUS_PCI_CHIP_MBGPEX )


// The flags below are or'ed to the PCPS_BUS_USB code
// in order to indicate which USB protocol version
// is supported by the device. If no additional flag is set
// then the device has a USB v1 interface.
#define PCPS_BUS_USB_FLAG_V2   0x8000

// The constant below combines the PCI bus flags:
#define PCPS_BUS_USB_V2       ( PCPS_BUS_USB | PCPS_BUS_USB_FLAG_V2 )

/** @} defgroup group_bus_flag_masks */


/**
 * @brief A data type used to store one of the ::PCPS_TYPES.
 *
 * Used e.g. for ::PCPS_DEV_TYPE::num.
 *
 * @see ::PCPS_TYPE
 */
typedef uint16_t PCPS_TYPE_NUM;


/**
 * @brief A numeric list of known devices.
 *
 * Used e.g. for ::PCPS_DEV_TYPE::num.
 *
 * @see ::PCPS_TYPE_NUM
 */
enum PCPS_TYPES
{
  PCPS_TYPE_PC31,
  PCPS_TYPE_PS31_OLD,
  PCPS_TYPE_PS31,
  PCPS_TYPE_PC32,
  PCPS_TYPE_PCI32,
  PCPS_TYPE_GPS167PC,
  PCPS_TYPE_GPS167PCI,
  PCPS_TYPE_PCI509,
  PCPS_TYPE_GPS168PCI,
  PCPS_TYPE_PCI510,
  PCPS_TYPE_GPS169PCI,
  PCPS_TYPE_TCR510PCI,
  PCPS_TYPE_TCR167PCI,
  PCPS_TYPE_GPS170PCI,
  PCPS_TYPE_PCI511,
  PCPS_TYPE_TCR511PCI,
  PCPS_TYPE_PEX511,
  PCPS_TYPE_TCR511PEX,
  PCPS_TYPE_GPS170PEX,
  PCPS_TYPE_USB5131,
  PCPS_TYPE_TCR51USB,
  PCPS_TYPE_MSF51USB,
  PCPS_TYPE_PTP270PEX,
  PCPS_TYPE_FRC511PEX,
  PCPS_TYPE_TCR170PEX,
  PCPS_TYPE_WWVB51USB,
  PCPS_TYPE_GPS180PEX,
  PCPS_TYPE_TCR180PEX,
  PCPS_TYPE_DCF600USB,
  PCPS_TYPE_PZF180PEX,
  PCPS_TYPE_TCR600USB,
  PCPS_TYPE_MSF600USB,
  PCPS_TYPE_WVB600USB,
  PCPS_TYPE_GLN180PEX,
  PCPS_TYPE_GPS180AMC,
  PCPS_TYPE_GNS181PEX,
  PCPS_TYPE_TCR180USB,
  N_PCPS_DEV_TYPE
};


#define PCPS_CLOCK_NAME_SZ   10   // including terminating 0

typedef char PCPS_CLOCK_NAME[PCPS_CLOCK_NAME_SZ];

typedef uint16_t PCPS_DEV_ID;
typedef uint16_t PCPS_REF_TYPE;
typedef uint16_t PCPS_BUS_FLAGS;

/**
 * @brief Device type specification
 *
 * Contains the characteristics of one of the ::PCPS_TYPES.
 * These specifications are always the same for a particular
 * type of device and do not change with firmware version,
 * port address, etc.
 */
typedef struct
{
  PCPS_TYPE_NUM num;                ///< see ::PCPS_TYPES
  PCPS_CLOCK_NAME name;
  PCPS_DEV_ID dev_id;               ///< see @ref MEINBERG_PCI_DEVICE_IDS and @ref MBG_USB_DEVICE_IDS
  PCPS_REF_TYPE ref_type;           ///< see ::PCPS_REF_TYPES
  PCPS_BUS_FLAGS bus_flags;         ///< see @ref PCPS_BUS_FLAG_MASKS

} PCPS_DEV_TYPE;



/**
 * @brief Legacy I/O address type, see ::PCPS_SHORT_PORT_RSRC
 */
typedef uint16_t PCPS_SHORT_PORT_ADDR;


/**
 * @brief An I/O port resource used by a device
 *
 * This structure has originally been used to store information
 * on an I/O address range.
 * However, the 16 bits provided by ::PCPS_SHORT_PORT_ADDR may
 * not be sufficient to hold an address on some target platforms,
 * so this is only kept to maintain API compatibility when
 * reporting I/O addresses to user space via ::PCPS_DEV_CFG
 * and thus ::PCPS_DEV structures.
 * A different structure is actually being used internally
 * by the kernel drivers.
 */
typedef struct
{
  PCPS_SHORT_PORT_ADDR base;
  uint16_t num;

} PCPS_SHORT_PORT_RSRC;



/**
 * @brief The max. number of I/O port resources used by a clock
 */
#define N_PCPS_PORT_RSRC 2



/**
 * @brief A structure used to retrieve an address to be mapped into user space
 *
 * @note This is currently not really supported and probably doesn't work.
 */
typedef struct
{
  void *user_virtual_address;  // TODO
  #if defined( MBG_TGT_LINUX )
    uint64_t len;
    uint64_t pfn_offset;
  #else
    ulong len;
  #endif

} PCPS_MAPPED_MEM;



typedef uint32_t PCPS_ERR_FLAGS;   ///< see @ref PCPS_ERR_FLAG_MASKS
typedef uint32_t PCPS_FEATURES;    ///< see @ref PCPS_FEATURE_MASKS
typedef uint16_t PCPS_BUS_NUM;
typedef uint16_t PCPS_SLOT_NUM;
typedef uint16_t PCPS_FW_REV_NUM;   ///< firmware revision number, MSB major, LSB minor version

#define PCPS_IRQ_NUM_UNDEFINED  -1  ///< Indicates the IRQ number is undefined.



/**
 * @brief Device information
 *
 * Contains variable device data which depends on a particular instance
 * of a device, e.g. the firmware which is currently installed, the
 * port address which has actually been assigned, etc.
 *
 * @note This structure only contains an incomplete subset of
 * information for newer devices, e.g. no memory resources,
 * and no extended device features.
 * However, adding the missing information would break
 * compatibility of some API functions.
 */
typedef struct
{
  PCPS_ERR_FLAGS err_flags;   ///< See @ref PCPS_ERR_FLAG_MASKS
  PCPS_BUS_NUM bus_num;
  PCPS_SLOT_NUM slot_num;
  PCPS_SHORT_PORT_RSRC port[N_PCPS_PORT_RSRC];
  PCPS_SHORT_PORT_ADDR short_status_port;
  int16_t irq_num;            ///< ::PCPS_IRQ_NUM_UNDEFINED if undefined
  uint32_t timeout_clk;
  PCPS_FW_REV_NUM fw_rev_num;
  PCPS_FEATURES features;     ///< See @ref PCPS_FEATURE_MASKS
  PCPS_ID_STR fw_id;
  PCPS_SN_STR sernum;

} PCPS_DEV_CFG;



/**
 * @brief Possible device initialization error flags
 *
 * Used with ::PCPS_DEV_CFG::err_flags
 *
 * @see ::PCPS_ERR_FLAGS
 *
 * @anchor PCPS_ERR_FLAG_MASKS @{ */

#define PCPS_EF_TIMEOUT         0x00000001   ///< timeout occured
#define PCPS_EF_INV_FW_ID       0x00000002   ///< invalid firmware ID
#define PCPS_EF_IO_INIT         0x00000004   ///< I/O interface not initialized
#define PCPS_EF_IO_CFG          0x00000008   ///< I/O interface not configured
#define PCPS_EF_IO_ENB          0x00000010   ///< I/O interface not enabled
#define PCPS_EF_IO_RSRC_IO      0x00000020   ///< I/O resource not registered with resource manager
#define PCPS_EF_IO_RSRC_MEM     0x00000040   ///< Memory resource not registered with resource manager

/** @} anchor PCPS_ERR_FLAG_MASKS */



/**
 * @defgroup group_pcps_features Feature flags used with PCPS_FEATURES
 *
 * Some features of the radio clocks have been introduced with
 * specific firmware versions, so depending on the firmware version
 * a clock may support a feature or not. The clock detection function
 * checks the clock model and firmware version and updates the field
 * ::PCPS_DEV_CFG::features accordingly. There are some macros which
 * can easily be used to query whether a clock device actually
 * supports a function, or not.
 *
 * @see ::PCPS_FEATURES
 *
 * @{ */

/**
 * @brief Feature bits for bus-level devices
 *
 * @see @ref PCPS_FEATURE_MASKS
 */
enum PCPS_FEATURE_BITS
{
  PCPS_BIT_CAN_SET_TIME,
  PCPS_BIT_HAS_SERIAL,
  PCPS_BIT_HAS_SYNC_TIME,
  PCPS_BIT_HAS_TZDL,
  PCPS_BIT_HAS_IDENT,
  PCPS_BIT_HAS_UTC_OFFS,
  PCPS_BIT_HAS_HR_TIME,
  PCPS_BIT_HAS_SERNUM,

  PCPS_BIT_HAS_TZCODE,
  PCPS_BIT_HAS_CABLE_LEN,
  PCPS_BIT_HAS_EVENT_TIME,    // custom GPS firmware only
  PCPS_BIT_HAS_RECEIVER_INFO,
  PCPS_BIT_CAN_CLR_UCAP_BUFF,
  PCPS_BIT_HAS_PCPS_TZDL,
  PCPS_BIT_HAS_UCAP,
  PCPS_BIT_HAS_IRIG_TX,

  PCPS_BIT_HAS_GPS_DATA_16,   // use 16 bit size specifiers
  PCPS_BIT_HAS_SYNTH,
  PCPS_BIT_HAS_GENERIC_IO,
  PCPS_BIT_HAS_TIME_SCALE,
  PCPS_BIT_HAS_UTC_PARM,
  PCPS_BIT_HAS_IRIG_CTRL_BITS,
  PCPS_BIT_HAS_LAN_INTF,
  PCPS_BIT_HAS_PTP,

  PCPS_BIT_HAS_IRIG_TIME,
  PCPS_BIT_HAS_FAST_HR_TSTAMP,
  PCPS_BIT_HAS_RAW_IRIG_DATA,
  PCPS_BIT_HAS_PZF,           // can also demodulate DCF77 PZF
  PCPS_BIT_HAS_EVT_LOG,
  PCPS_BIT_IS_GNSS,           // supports several satellite systems and GNSS API calls

  N_PCPS_FEATURE_BITS         // must not exceed 32 !!
};



/**
 * @brief Feature bit masks for bus-level devices
 *
 * Used with ::PCPS_DEV_CFG::features
 *
 * @see ::PCPS_FEATURE_BITS
 * @see ::PCPS_FEATURES
 *
 * @anchor PCPS_FEATURE_MASKS @{ */

#define PCPS_CAN_SET_TIME       ( 1UL << PCPS_BIT_CAN_SET_TIME )        ///< see ::PCPS_BIT_CAN_SET_TIME
#define PCPS_HAS_SERIAL         ( 1UL << PCPS_BIT_HAS_SERIAL )          ///< see ::PCPS_BIT_HAS_SERIAL
#define PCPS_HAS_SYNC_TIME      ( 1UL << PCPS_BIT_HAS_SYNC_TIME )       ///< see ::PCPS_BIT_HAS_SYNC_TIME
#define PCPS_HAS_TZDL           ( 1UL << PCPS_BIT_HAS_TZDL )            ///< see ::PCPS_BIT_HAS_TZDL
#define PCPS_HAS_IDENT          ( 1UL << PCPS_BIT_HAS_IDENT )           ///< see ::PCPS_BIT_HAS_IDENT
#define PCPS_HAS_UTC_OFFS       ( 1UL << PCPS_BIT_HAS_UTC_OFFS )        ///< see ::PCPS_BIT_HAS_UTC_OFFS
#define PCPS_HAS_HR_TIME        ( 1UL << PCPS_BIT_HAS_HR_TIME )         ///< see ::PCPS_BIT_HAS_HR_TIME
#define PCPS_HAS_SERNUM         ( 1UL << PCPS_BIT_HAS_SERNUM )          ///< see ::PCPS_BIT_HAS_SERNUM

#define PCPS_HAS_TZCODE         ( 1UL << PCPS_BIT_HAS_TZCODE )          ///< see ::PCPS_BIT_HAS_TZCODE
#define PCPS_HAS_CABLE_LEN      ( 1UL << PCPS_BIT_HAS_CABLE_LEN )       ///< see ::PCPS_BIT_HAS_CABLE_LEN
#define PCPS_HAS_EVENT_TIME     ( 1UL << PCPS_BIT_HAS_EVENT_TIME )      ///< see ::PCPS_BIT_HAS_EVENT_TIME
#define PCPS_HAS_RECEIVER_INFO  ( 1UL << PCPS_BIT_HAS_RECEIVER_INFO )   ///< see ::PCPS_BIT_HAS_RECEIVER_INFO
#define PCPS_CAN_CLR_UCAP_BUFF  ( 1UL << PCPS_BIT_CAN_CLR_UCAP_BUFF )   ///< see ::PCPS_BIT_CAN_CLR_UCAP_BUFF
#define PCPS_HAS_PCPS_TZDL      ( 1UL << PCPS_BIT_HAS_PCPS_TZDL )       ///< see ::PCPS_BIT_HAS_PCPS_TZDL
#define PCPS_HAS_UCAP           ( 1UL << PCPS_BIT_HAS_UCAP )            ///< see ::PCPS_BIT_HAS_UCAP
#define PCPS_HAS_IRIG_TX        ( 1UL << PCPS_BIT_HAS_IRIG_TX )         ///< see ::PCPS_BIT_HAS_IRIG_TX

#define PCPS_HAS_GPS_DATA_16    ( 1UL << PCPS_BIT_HAS_GPS_DATA_16 )     ///< see ::PCPS_BIT_HAS_GPS_DATA_16
#define PCPS_HAS_SYNTH          ( 1UL << PCPS_BIT_HAS_SYNTH )           ///< see ::PCPS_BIT_HAS_SYNTH
#define PCPS_HAS_GENERIC_IO     ( 1UL << PCPS_BIT_HAS_GENERIC_IO )      ///< see ::PCPS_BIT_HAS_GENERIC_IO
#define PCPS_HAS_TIME_SCALE     ( 1UL << PCPS_BIT_HAS_TIME_SCALE )      ///< see ::PCPS_BIT_HAS_TIME_SCALE
#define PCPS_HAS_UTC_PARM       ( 1UL << PCPS_BIT_HAS_UTC_PARM )        ///< see ::PCPS_BIT_HAS_UTC_PARM
#define PCPS_HAS_IRIG_CTRL_BITS ( 1UL << PCPS_BIT_HAS_IRIG_CTRL_BITS )  ///< see ::PCPS_BIT_HAS_IRIG_CTRL_BITS
#define PCPS_HAS_LAN_INTF       ( 1UL << PCPS_BIT_HAS_LAN_INTF )        ///< see ::PCPS_BIT_HAS_LAN_INTF
#define PCPS_HAS_PTP            ( 1UL << PCPS_BIT_HAS_PTP )             ///< see ::PCPS_BIT_HAS_PTP

#define PCPS_HAS_IRIG_TIME      ( 1UL << PCPS_BIT_HAS_IRIG_TIME )       ///< see ::PCPS_BIT_HAS_IRIG_TIME
#define PCPS_HAS_FAST_HR_TSTAMP ( 1UL << PCPS_BIT_HAS_FAST_HR_TSTAMP )  ///< see ::PCPS_BIT_HAS_FAST_HR_TSTAMP
#define PCPS_HAS_RAW_IRIG_DATA  ( 1UL << PCPS_BIT_HAS_RAW_IRIG_DATA )   ///< see ::PCPS_BIT_HAS_RAW_IRIG_DATA
#define PCPS_HAS_PZF            ( 1UL << PCPS_BIT_HAS_PZF )             ///< see ::PCPS_BIT_HAS_PZF
#define PCPS_HAS_EVT_LOG        ( 1UL << PCPS_BIT_HAS_EVT_LOG )         ///< see ::PCPS_BIT_HAS_EVT_LOG
#define PCPS_IS_GNSS            ( 1UL << PCPS_BIT_IS_GNSS )             ///< see ::PCPS_BIT_IS_GNSS

/** @} anchor PCPS_FEATURE_MASKS */


#define PCPS_FEATURE_NAMES    \
{                             \
  "PCPS_CAN_SET_TIME",        \
  "PCPS_HAS_SERIAL",          \
  "PCPS_HAS_SYNC_TIME",       \
  "PCPS_HAS_TZDL",            \
  "PCPS_HAS_IDENT",           \
  "PCPS_HAS_UTC_OFFS",        \
  "PCPS_HAS_HR_TIME",         \
  "PCPS_HAS_SERNUM",          \
  "PCPS_HAS_TZCODE",          \
  "PCPS_HAS_CABLE_LEN",       \
  "PCPS_HAS_EVENT_TIME",      \
  "PCPS_HAS_RECEIVER_INFO",   \
  "PCPS_CAN_CLR_UCAP_BUFF",   \
  "PCPS_HAS_PCPS_TZDL",       \
  "PCPS_HAS_UCAP",            \
  "PCPS_HAS_IRIG_TX",         \
  "PCPS_HAS_GPS_DATA_16",     \
  "PCPS_HAS_SYNTH",           \
  "PCPS_HAS_GENERIC_IO",      \
  "PCPS_HAS_TIME_SCALE",      \
  "PCPS_HAS_UTC_PARM",        \
  "PCPS_HAS_IRIG_CTRL_BITS",  \
  "PCPS_HAS_LAN_INTF",        \
  "PCPS_HAS_PTP",             \
  "PCPS_HAS_IRIG_TIME",       \
  "PCPS_HAS_FAST_HR_TSTAMP",  \
  "PCPS_HAS_RAW_IRIG_DATA",   \
  "PCPS_HAS_PZF",             \
  "PCPS_HAS_EVT_LOG",         \
  "PCPS_IS_GNSS"              \
}

/** @} defgroup group_pcps_features */



// The constants below define those features which are available
// in ALL firmware versions which have been shipped with a
// specific clock.

#define PCPS_FEAT_PC31PS31  0

// Some of the features are available in all newer clocks,
// so these have been put together in one definition:
#define PCPS_FEAT_LVL2      ( PCPS_CAN_SET_TIME   \
                            | PCPS_HAS_SERIAL     \
                            | PCPS_HAS_SYNC_TIME  \
                            | PCPS_HAS_UTC_OFFS )

#define PCPS_FEAT_PC32      ( PCPS_FEAT_LVL2 )

#define PCPS_FEAT_PCI32     ( PCPS_FEAT_LVL2 )

#define PCPS_FEAT_PCI509    ( PCPS_FEAT_LVL2 \
                            | PCPS_HAS_SERNUM \
                            | PCPS_HAS_TZCODE )

#define PCPS_FEAT_PCI510    ( PCPS_FEAT_PCI509 )

#define PCPS_FEAT_PCI511    ( PCPS_FEAT_PCI510 )

#define PCPS_FEAT_GPS167PC  ( PCPS_FEAT_LVL2 \
                            | PCPS_HAS_TZDL \
                            | PCPS_HAS_IDENT )

#define PCPS_FEAT_GPS167PCI ( PCPS_FEAT_LVL2 \
                            | PCPS_HAS_TZDL  \
                            | PCPS_HAS_IDENT \
                            | PCPS_HAS_HR_TIME )

#define PCPS_FEAT_GPS168PCI ( PCPS_FEAT_LVL2 \
                            | PCPS_HAS_TZDL  \
                            | PCPS_HAS_IDENT \
                            | PCPS_HAS_HR_TIME \
                            | PCPS_HAS_CABLE_LEN \
                            | PCPS_HAS_RECEIVER_INFO )

#define PCPS_FEAT_GPS169PCI ( PCPS_FEAT_GPS168PCI \
                            | PCPS_CAN_CLR_UCAP_BUFF \
                            | PCPS_HAS_UCAP )

#define PCPS_FEAT_GPS170PCI ( PCPS_FEAT_GPS169PCI \
                            | PCPS_HAS_IRIG_TX \
                            | PCPS_HAS_GPS_DATA_16 \
                            | PCPS_HAS_GENERIC_IO )

#define PCPS_FEAT_TCR510PCI ( PCPS_FEAT_LVL2 \
                            | PCPS_HAS_SERNUM )

#define PCPS_FEAT_TCR167PCI ( PCPS_FEAT_LVL2 \
                            | PCPS_HAS_SERNUM \
                            | PCPS_HAS_TZDL \
                            | PCPS_HAS_HR_TIME \
                            | PCPS_HAS_RECEIVER_INFO \
                            | PCPS_CAN_CLR_UCAP_BUFF \
                            | PCPS_HAS_UCAP \
                            | PCPS_HAS_IRIG_TX \
                            | PCPS_HAS_GPS_DATA_16 \
                            | PCPS_HAS_GENERIC_IO )

#define PCPS_FEAT_TCR511PCI ( PCPS_FEAT_TCR510PCI \
                            | PCPS_HAS_HR_TIME )

#define PCPS_FEAT_PEX511    ( PCPS_FEAT_PCI511 )

#define PCPS_FEAT_TCR511PEX ( PCPS_FEAT_TCR511PCI )

#define PCPS_FEAT_GPS170PEX ( PCPS_FEAT_GPS170PCI )

#define PCPS_FEAT_USB5131   ( PCPS_HAS_UTC_OFFS  \
                            | PCPS_HAS_SERNUM    \
                            | PCPS_HAS_SYNC_TIME \
                            | PCPS_HAS_HR_TIME   \
                            | PCPS_CAN_SET_TIME  \
                            | PCPS_HAS_TZCODE )

#define PCPS_FEAT_TCR51USB  ( PCPS_HAS_UTC_OFFS  \
                            | PCPS_HAS_SERNUM    \
                            | PCPS_HAS_SYNC_TIME \
                            | PCPS_HAS_HR_TIME   \
                            | PCPS_CAN_SET_TIME )

#define PCPS_FEAT_MSF51USB  ( PCPS_HAS_UTC_OFFS  \
                            | PCPS_HAS_SERNUM    \
                            | PCPS_HAS_SYNC_TIME \
                            | PCPS_HAS_HR_TIME   \
                            | PCPS_CAN_SET_TIME )

#define PCPS_FEAT_PTP270PEX ( PCPS_HAS_SERNUM    \
                            | PCPS_HAS_SYNC_TIME \
                            | PCPS_HAS_HR_TIME   \
                            | PCPS_HAS_RECEIVER_INFO \
                            | PCPS_CAN_SET_TIME  \
                            | PCPS_CAN_CLR_UCAP_BUFF \
                            | PCPS_HAS_UCAP \
                            | PCPS_HAS_GPS_DATA_16 )

#define PCPS_FEAT_FRC511PEX ( PCPS_HAS_SERNUM    \
                            | PCPS_HAS_HR_TIME   \
                            | PCPS_HAS_RECEIVER_INFO \
                            | PCPS_CAN_SET_TIME  \
                            | PCPS_CAN_CLR_UCAP_BUFF \
                            | PCPS_HAS_UCAP \
                            | PCPS_HAS_GPS_DATA_16 )

#define PCPS_FEAT_TCR170PEX ( PCPS_FEAT_TCR167PCI )

#define PCPS_FEAT_WWVB51USB ( PCPS_FEAT_MSF51USB )

#define PCPS_FEAT_GPS180PEX ( ( PCPS_FEAT_GPS170PEX | PCPS_HAS_FAST_HR_TSTAMP ) \
                            & ~PCPS_HAS_IRIG_TX )
                            ///< IRIG TX only supp. if ::GPS_HAS_IRIG_TX

#define PCPS_FEAT_TCR180PEX ( ( PCPS_FEAT_TCR170PEX | PCPS_HAS_FAST_HR_TSTAMP ) \
                            & ~PCPS_HAS_IRIG_TX )
                            ///< IRIG TX only supp. if ::GPS_HAS_IRIG_TX

#define PCPS_FEAT_DCF600USB ( PCPS_FEAT_USB5131 )

#define PCPS_FEAT_PZF180PEX ( PCPS_FEAT_LVL2 \
                            | PCPS_HAS_TZDL \
                            | PCPS_HAS_HR_TIME \
                            | PCPS_HAS_SERNUM \
                            | PCPS_HAS_RECEIVER_INFO \
                            | PCPS_CAN_CLR_UCAP_BUFF \
                            | PCPS_HAS_UCAP \
                            | PCPS_HAS_GPS_DATA_16 \
                            | PCPS_HAS_GENERIC_IO \
                            | PCPS_HAS_UTC_PARM \
                            | PCPS_HAS_PZF )

#define PCPS_FEAT_TCR600USB ( PCPS_FEAT_TCR51USB \
                            | PCPS_HAS_IRIG_CTRL_BITS \
                            | PCPS_HAS_IRIG_TIME \
                            | PCPS_HAS_RAW_IRIG_DATA )

#define PCPS_FEAT_MSF600USB ( PCPS_FEAT_MSF51USB )

#define PCPS_FEAT_WVB600USB ( PCPS_FEAT_WWVB51USB )

#define PCPS_FEAT_GLN180PEX ( PCPS_FEAT_GPS180PEX | PCPS_IS_GNSS )

#define PCPS_FEAT_GPS180AMC ( PCPS_FEAT_GPS180PEX )

#define PCPS_FEAT_GNS181PEX ( PCPS_FEAT_GLN180PEX )

#define PCPS_FEAT_TCR180USB ( PCPS_FEAT_TCR600USB \
                            | PCPS_HAS_TZDL \
                            | PCPS_HAS_RECEIVER_INFO \
                            | PCPS_HAS_GPS_DATA_16 \
                            | PCPS_CAN_CLR_UCAP_BUFF /* TODO ? */ \
                            | PCPS_HAS_UCAP          /* TODO ? */ \
                            | PCPS_HAS_GENERIC_IO    /* TODO ? */ )
                            ///< IRIG TX only supp. if ::GPS_HAS_IRIG_TX



// Some features of the API used to access Meinberg plug-in devices
// have been implemented starting with the special firmware revision
// numbers defined below.
//
// If no number is specified for a feature/clock model then the feature
// is either always supported by that clock model, or not at all.


// There are some versions of PCI Express cards out there which do not
// safely support hardware IRQs. The following firmware versions are required
// for safe IRQ operation:
#define REV_HAS_IRQ_FIX_MINOR_PEX511     0x0106
#define REV_HAS_IRQ_FIX_MINOR_TCR511PEX  0x0105
#define REV_HAS_IRQ_FIX_MINOR_GPS170PEX  0x0104
// Additionally there are certain revisions of the bus interface logic
// required. The associated version codes are defined in pci_asic.h.

// The macro below can be used to check whether the required versions are there:
#define _pcps_pex_irq_is_safe( _curr_fw_ver, _req_fw_ver, _curr_asic_ver,    \
                               _req_asic_ver_major, _req_asic_ver_minor )    \
  ( ( (_curr_fw_ver) >= (_req_fw_ver) ) && _pcps_asic_version_greater_equal( \
    (_curr_asic_ver), (_req_asic_ver_major), (_req_asic_ver_minor ) )        \
  )

/* command PCPS_GIVE_RAW_IRIG_DATA: */
#define REV_HAS_RAW_IRIG_DATA_TCR511PEX 0x0111
#define REV_HAS_RAW_IRIG_DATA_TCR511PCI 0x0111
#define REV_HAS_RAW_IRIG_DATA_TCR51USB  0x0106

/* command PCPS_GIVE_IRIG_TIME: */
#define REV_HAS_IRIG_TIME_TCR511PEX 0x0109
#define REV_HAS_IRIG_TIME_TCR511PCI 0x0109
#define REV_HAS_IRIG_TIME_TCR51USB  0x0106

/* command PCPS_GET_IRIG_CTRL_BITS: */
#define REV_HAS_IRIG_CTRL_BITS_TCR511PEX 0x0107
#define REV_HAS_IRIG_CTRL_BITS_TCR511PCI 0x0107
#define REV_HAS_IRIG_CTRL_BITS_TCR51USB  0x0106

/* This board uses the GPS_DATA interface with 16 bit buffer sizes
   instead of the original 8 bit sizes, thus allowing to transfer
   data blocks which exceed 255 bytes (PCPS_HAS_GPS_DATA_16) */
#define REV_HAS_GPS_DATA_16_GPS169PCI    0x0202

/* the clock supports a higher baud rate than N_PCPS_BD_DCF */
#define REV_HAS_SERIAL_HS_PCI509         0x0104

/* commands PCPS_GIVE_UCAP_ENTRIES, PCPS_GIVE_UCAP_EVENT */
#define REV_HAS_UCAP_GPS167PCI           0x0421
#define REV_HAS_UCAP_GPS168PCI           0x0104

/* command PCPS_CLR_UCAP_BUFF */
#define REV_CAN_CLR_UCAP_BUFF_GPS167PCI  0x0419
#define REV_CAN_CLR_UCAP_BUFF_GPS168PCI  0x0101

/* commands PCPS_READ_GPS_DATA and PCPS_WRITE_GPS_DATA with */
/* code PC_GPS_ANT_CABLE_LEN */
#define REV_HAS_CABLE_LEN_GPS167PCI      0x0411
#define REV_HAS_CABLE_LEN_GPS167PC       0x0411

/* command PCPS_GIVE_HR_TIME, structure PCPS_HR_TIME: */
#define REV_HAS_HR_TIME_GPS167PC         0x0305
#define REV_HAS_HR_TIME_TCR510PCI        0x0200
#define REV_HAS_HR_TIME_PEX511           0x0105  // This also requires a certain ASIC version.
#define REV_HAS_HR_TIME_PCI511           0x0103

/* field offs_utc in structure PCPS_TIME: */
#define REV_HAS_UTC_OFFS_PC31PS31        0x0300

/* command PCPS_GIVE_SYNC_TIME: */
#define REV_HAS_SYNC_TIME_PC31PS31       0x0300

/* command PCPS_GET_SERIAL, PCPS_SET_SERIAL: */
#define REV_HAS_SERIAL_PC31PS31          0x0300

/* command PCPS_GIVE_TIME_NOCLEAR: */
#define REV_GIVE_TIME_NOCLEAR_PC31PS31   0x0300

/* status bit PCPS_LS_ANN: */
#define REV_PCPS_LS_ANN_PC31PS31         0x0300

/* status bit PCPS_IFTM: */
#define REV_PCPS_IFTM_PC31PS31           0x0300

/* command PCPS_SET_TIME: */
#define REV_CAN_SET_TIME_PC31PS31        0x0240

/* command PCPS_GIVE_TIME_NOCLEAR: */
// This is supported by all clocks but PC31/PS31 with
// firmware versions before v3.0. If such a card shall
// be used then the firmware should be updated to the
// last recent version.


/**
 * @brief Device info structure
 *
 * Used to pass all information on a bus-level device
 * from a device driver to a user space application.
 */
typedef struct
{
  PCPS_DEV_TYPE type;
  PCPS_DEV_CFG cfg;

} PCPS_DEV;


#if 1 || defined( MBG_TGT_KERNEL )   //### FIXME
  #define _USE_DEV_MACROS  1
#else
  #define _USE_DEV_MACROS  0
#endif


// The macros below simplify access to the data
// stored in PCPS_DEV structure and should be used
// to extract the desired information.
// If the formal parameter is called _d then a pointer
// to device structure PCPS_DEV is expected.
// If the formal parameter is called _c then a pointer
// to configuration structure PCPS_DEV_CFG is expected.

// Access device type information:
#define _pcps_type_num( _d )     ( (_d)->type.num )
#define _pcps_type_name( _d )    ( (_d)->type.name )
#define _pcps_dev_id( _d )       ( (_d)->type.dev_id )
#define _pcps_ref_type( _d )     ( (_d)->type.ref_type )
#define _pcps_bus_flags( _d )    ( (_d)->type.bus_flags )

// Query device type features:
#define _pcps_is_gps( _d )       ( _pcps_ref_type( _d ) == PCPS_REF_GPS )
#define _pcps_is_dcf( _d )       ( _pcps_ref_type( _d ) == PCPS_REF_DCF )
#define _pcps_is_msf( _d )       ( _pcps_ref_type( _d ) == PCPS_REF_MSF )
#define _pcps_is_wwvb( _d )      ( _pcps_ref_type( _d ) == PCPS_REF_WWVB )
#define _pcps_is_irig_rx( _d )   ( _pcps_ref_type( _d ) == PCPS_REF_IRIG )
#define _pcps_is_ptp( _d )       ( _pcps_ref_type( _d ) == PCPS_REF_PTP )
#define _pcps_is_frc( _d )       ( _pcps_ref_type( _d ) == PCPS_REF_FRC )
#define _pcps_is_gnss( _d )      _pcps_has_feature( (_d), PCPS_IS_GNSS )

#define _pcps_is_lwr( _d )       ( _pcps_is_dcf( _d ) || _pcps_is_msf( _d ) || _pcps_is_wwvb( _d ) )

// Generic bus types:
#define _pcps_is_isa( _d )       ( _pcps_bus_flags( _d ) & PCPS_BUS_ISA )
#define _pcps_is_mca( _d )       ( _pcps_bus_flags( _d ) & PCPS_BUS_MCA )
#define _pcps_is_pci( _d )       ( _pcps_bus_flags( _d ) & PCPS_BUS_PCI )
#define _pcps_is_usb( _d )       ( _pcps_bus_flags( _d ) & PCPS_BUS_USB )

// Special bus types:
#define _pcps_is_usb_v2( _d )       ( _pcps_bus_flags( _d ) == PCPS_BUS_USB_V2 )
#define _pcps_is_pci_s5933( _d )    ( _pcps_bus_flags( _d ) == PCPS_BUS_PCI_S5933 )
#define _pcps_is_pci_s5920( _d )    ( _pcps_bus_flags( _d ) == PCPS_BUS_PCI_S5920 )
#define _pcps_is_pci_amcc( _d )     ( _pcps_is_pci_s5920( _d ) || _pcps_is_pci_s5933( _d )  )
#define _pcps_is_pci_asic( _d )     ( _pcps_bus_flags( _d ) == PCPS_BUS_PCI_ASIC )
#define _pcps_is_pci_pex8311( _d )  ( _pcps_bus_flags( _d ) == PCPS_BUS_PCI_PEX8311 )
#define _pcps_is_pci_mbgpex( _d )   ( _pcps_bus_flags( _d ) == PCPS_BUS_PCI_MBGPEX )


// Access device configuration information:
#define _pcps_bus_num( _d )            ( (_d)->cfg.bus_num )
#define _pcps_slot_num( _d )           ( (_d)->cfg.slot_num )

#define _pcps_cfg_short_port_rsrc( _c, _n )  ( (_c)->port[_n] )
#define _pcps_short_port_rsrc( _d, _n )      _pcps_cfg_short_port_rsrc( &(_d)->cfg, (_n) )

#define _pcps_cfg_short_port_base( _c, _n )  ( _pcps_cfg_short_port_rsrc( (_c), (_n) ).base )
#define _pcps_short_port_base( _d, _n )      ( _pcps_short_port_rsrc( (_d), (_n) ).base )

#define _pcps_cfg_irq_num( _c )        ( (_c)->irq_num )
#define _pcps_irq_num( _d )            _pcps_cfg_irq_num( &(_d)->cfg )

#define _pcps_cfg_timeout_clk( _c )    ( (_c)->timeout_clk )
#define _pcps_timeout_clk( _d )        _pcps_cfg_timeout_clk( &(_d)->cfg )

#define _pcps_fw_rev_num( _d )         ( (_d)->cfg.fw_rev_num )
#define _pcps_features( _d )           ( (_d)->cfg.features )
#define _pcps_fw_id( _d )              ( (_d)->cfg.fw_id )
#define _pcps_sernum( _d )             ( (_d)->cfg.sernum )


// The macros below handle the device's err_flags.
#define _pcps_err_flags( _d )           ( (_d)->cfg.err_flags )
#define _pcps_chk_err_flags( _d, _msk ) ( _pcps_err_flags( _d ) & (_msk) )
#define _pcps_set_err_flags( _d, _msk ) ( _pcps_err_flags( _d ) |= (_msk) )
#define _pcps_clr_err_flags( _d, _msk ) ( _pcps_err_flags( _d ) &= ~(_msk) )


#if _USE_DEV_MACROS

/// Check whether a special feature is supported
#define _pcps_has_feature( _d, _f )    ( ( (_d)->cfg.features & (_f) ) != 0 )

/// Check whether a special feature is supported according to ::RECEIVER_INFO
#define _pcps_has_ri_feature( _p_ri, _f )    ( ( (_p_ri)->features & (_f) ) != 0 )

#define _ri_addr( _p )        &(_p)->xdev_features.receiver_info
#define _xfeat_addr( _p )     &(_p)->xdev_features.xfeature_buffer
#define _tlv_info_addr( _p )  &(_p)->xdev_features.tlv_info
#define _tlv_feat_buffp( _p ) &(_p)->xdev_features.tlv_info.supp_tlv_feat


#define _pcps_can_set_time( _d )   _pcps_has_feature( (_d), PCPS_CAN_SET_TIME )
#define _pcps_has_serial( _d )     _pcps_has_feature( (_d), PCPS_HAS_SERIAL )
#define _pcps_has_sync_time( _d )  _pcps_has_feature( (_d), PCPS_HAS_SYNC_TIME )
#define _pcps_has_ident( _d )      _pcps_has_feature( (_d), PCPS_HAS_IDENT )
#define _pcps_has_utc_offs( _d )   _pcps_has_feature( (_d), PCPS_HAS_UTC_OFFS )
#define _pcps_has_hr_time( _d )    _pcps_has_feature( (_d), PCPS_HAS_HR_TIME )
#define _pcps_has_sernum( _d )     _pcps_has_feature( (_d), PCPS_HAS_SERNUM )
#define _pcps_has_cab_len( _d )    _pcps_has_feature( (_d), PCPS_HAS_CABLE_LEN )
#define _pcps_has_tzdl( _d )       _pcps_has_feature( (_d), PCPS_HAS_TZDL )
#define _pcps_has_pcps_tzdl( _d )  _pcps_has_feature( (_d), PCPS_HAS_PCPS_TZDL )
#define _pcps_has_tzcode( _d )     _pcps_has_feature( (_d), PCPS_HAS_TZCODE )
#define _pcps_has_tz( _d )         _pcps_has_feature( (_d), PCPS_HAS_TZDL \
                                                          | PCPS_HAS_PCPS_TZDL \
                                                          | PCPS_HAS_TZCODE )
// The next one is supported only with a certain GPS firmware version:
#define _pcps_has_event_time( _d ) _pcps_has_feature( (_d), PCPS_HAS_EVENT_TIME )
#define _pcps_has_receiver_info( _d ) _pcps_has_feature( (_d), PCPS_HAS_RECEIVER_INFO )
#define _pcps_can_clr_ucap_buff( _d ) _pcps_has_feature( (_d), PCPS_CAN_CLR_UCAP_BUFF )
#define _pcps_has_ucap( _d )       _pcps_has_feature( (_d), PCPS_HAS_UCAP )
#define _pcps_has_irig_tx( _d )    _pcps_has_feature( (_d), PCPS_HAS_IRIG_TX )

// The macro below determines whether a DCF77 clock
// supports a higher baud rate than standard
#define _pcps_has_serial_hs( _d ) \
  ( ( _pcps_type_num( _d ) == PCPS_TYPE_TCR511PEX ) || \
    ( _pcps_type_num( _d ) == PCPS_TYPE_PEX511 )    || \
    ( _pcps_type_num( _d ) == PCPS_TYPE_TCR511PCI ) || \
    ( _pcps_type_num( _d ) == PCPS_TYPE_TCR510PCI ) || \
    ( _pcps_type_num( _d ) == PCPS_TYPE_PCI511 )    || \
    ( _pcps_type_num( _d ) == PCPS_TYPE_PCI510 )    || \
    ( _pcps_type_num( _d ) == PCPS_TYPE_PCI509 &&      \
      _pcps_fw_rev_num( _d ) >= REV_HAS_SERIAL_HS_PCI509 ) )


#define _pcps_has_signal( _d ) \
  ( _pcps_is_dcf( _d ) || _pcps_is_msf( _d ) || _pcps_is_wwvb( _d ) || _pcps_is_irig_rx( _d )  )

#define _pcps_has_mod( _d ) \
  ( _pcps_is_dcf( _d ) || _pcps_is_msf( _d ) || _pcps_is_wwvb( _d ) )

#define _pcps_has_irig( _d ) \
  ( _pcps_is_irig_rx( _d ) || _pcps_has_irig_tx( _d ) )

#define _pcps_has_irig_ctrl_bits( _d ) \
  _pcps_has_feature( (_d), PCPS_HAS_IRIG_CTRL_BITS )

#define _pcps_has_irig_time( _d ) \
  _pcps_has_feature( (_d), PCPS_HAS_IRIG_TIME )

#define _pcps_has_raw_irig_data( _d ) \
  _pcps_has_feature( (_d), PCPS_HAS_RAW_IRIG_DATA )

#define _pcps_has_ref_offs( _d ) \
  _pcps_is_irig_rx( _d )

#define _pcps_ref_offs_out_of_range( _n ) \
  ( ( (_n) > MBG_REF_OFFS_MAX ) || ( (_n) < -MBG_REF_OFFS_MAX ) )

#define _pcps_has_opt_flags( _d ) \
  _pcps_is_irig_rx( _d )

#define _pcps_has_gps_data_16( _d )  _pcps_has_feature( (_d), PCPS_HAS_GPS_DATA_16 )

#define _pcps_has_gps_data( _d ) \
  ( _pcps_is_gps( _d ) || _pcps_has_gps_data_16( _d ) )

#define _pcps_has_synth( _d )  _pcps_has_feature( (_d), PCPS_HAS_SYNTH )

#define _pcps_has_generic_io( _d )  _pcps_has_feature( (_d), PCPS_HAS_GENERIC_IO )

#define _pcps_has_time_scale( _d )  _pcps_has_feature( (_d), PCPS_HAS_TIME_SCALE )

#define _pcps_has_utc_parm( _d )  _pcps_has_feature( (_d), PCPS_HAS_UTC_PARM )

#define _pcps_has_asic_version( _d ) ( _pcps_is_pci_asic( _d ) \
                                    || _pcps_is_pci_pex8311( _d ) \
                                    || _pcps_is_pci_mbgpex( _d ) )

#define _pcps_has_asic_features( _d ) _pcps_has_asic_version( _d )

#define _pcps_has_fast_hr_timestamp( _d )  _pcps_has_feature( (_d), PCPS_HAS_FAST_HR_TSTAMP )

#define _pcps_has_lan_intf( _d )  _pcps_has_feature( (_d), PCPS_HAS_LAN_INTF )

#define _pcps_has_ptp( _d )  _pcps_has_feature( (_d), PCPS_HAS_PTP )

#define _pcps_has_ri_ptp_unicast( _p_ri )  _pcps_has_ri_feature( (_p_ri), GPS_HAS_PTP_UNICAST )

#define _pcps_has_pzf( _d )  _pcps_has_feature( (_d), PCPS_HAS_PZF )

#define _pcps_has_corr_info( _d )  _pcps_has_pzf( _d )

#define _pcps_has_tr_distance( _d )  _pcps_has_pzf( _d )

#define _pcps_has_evt_log( _d ) _pcps_has_feature( (_d), PCPS_HAS_EVT_LOG )

#define _pcps_has_debug_status( _d ) _pcps_is_irig_rx( _d )

#define _pcps_has_stat_info( _d )  ( _pcps_is_gps( _d ) || _pcps_has_pzf( _d ) )

#define _pcps_has_stat_info_mode( _d )  _pcps_is_gps( _d )

#define _pcps_has_stat_info_svs( _d )  _pcps_is_gps( _d )

#define _pcps_has_ri_gpio( _p_ri )  _pcps_has_ri_feature( (_p_ri), GPS_HAS_GPIO )

// We only report that XMR is supported if all required features are supported.
#define _pcps_has_ri_xmr( _p_ri )  ( _pcps_has_ri_feature( (_p_ri), GPS_HAS_XMULTI_REF ) && \
                                     _pcps_has_ri_feature( (_p_ri), GPS_HAS_XMRS_MULT_INSTC ) )
//### TODO should also check GPS_MODEL_HAS_XMR_HOLDOVER_INTV, which is a builtin feature flag only

#endif  // _USE_DEV_MACROS


// There are some versions of IRIG receiver cards which ignore the TFOM code
// of an incoming IRIG signal even if an IRIG code has been configured which
// supports this. In this case these cards synchronize to the incoming IRIG
// signal even if the TFOM code reports the IRIG generator is not synchronized.
// The intended behaviour is that the IRIG receiver card changes its status
// to "freewheeling" in this case, unless it has been configured to ignore
// the TFOM code of the incoming IRIG signal (see the ::IFLAGS_DISABLE_TFOM flag
// defined in gpsdefs.h).

// The macro below can be used to check based on the device info if a specific
// card with a specific firmware always ignores the TFOM code:
#define _pcps_incoming_tfom_ignored( _d ) \
  ( ( ( _pcps_type_num( _d ) == PCPS_TYPE_TCR167PCI ) && ( _pcps_fw_rev_num( _d ) <= 0x121 ) ) \
 || ( ( _pcps_type_num( _d ) == PCPS_TYPE_TCR170PEX ) && ( _pcps_fw_rev_num( _d ) <= 0x103 ) ) )


// Some Meinberg PCI Express cards have a PCIe interface chip with an extra
// PCI bridge built into the chip. Unfortunately there are some mainboards out there
// which do not handle PCI resources behind this PCI bridge correctly. The symptom is
// usually that both I/O address ranges of these cards get the same base address
// assigned by the BIOS, and the effect is that in this case a card is not accessible
// properly, since both I/O ranges try to respond to the same I/O addresses.
// As a consequence, data read from the card is usually garbage.
// The only known fix for this is a BIOS update for the mainboard which makes the
// BIOS handle the card's resources properly.

// The macro below can be used to test if both port base addresses assigned to a card
// are identical, and thus the BIOS is probably faulty::
#define _pcps_pci_cfg_err( _d ) \
  ( _pcps_is_pci( _d ) && _pcps_short_port_base( _d, 0 ) && \
    ( _pcps_short_port_base( _d, 1 ) == _pcps_short_port_base( _d, 0 ) ) )



// There are some old firmware versions of GPS PCI cards which may occasionally
// return a HR time stamp which is wrong by 20 milliseconds, if the HR time is
// read right after some GPS data. As a workaround for that bug an application
// must wait at least 1.5 ms and then just read the PCPS_TIME structure
// in order to re-initialize the software interface state.
// This has been fixed in more recent versions of the affected firmware.
// This macro can be used to let an application determine whether it has to
// account for this bug with a given card and firmware version.
#define _pcps_fw_has_20ms_bug( _d )                                              \
(                                                                                       \
  ( _pcps_type_num( _d ) == PCPS_TYPE_GPS168PCI && _pcps_fw_rev_num( _d ) < 0x0102 ) || \
  ( _pcps_type_num( _d ) == PCPS_TYPE_GPS167PCI && _pcps_fw_rev_num( _d ) < 0x0420 )    \
)



/**
 * @brief Codes used with ::IOCTL_DEV_FEAT_REQ::feat_type
 */
enum DEV_FEAT_TYPES
{
  DEV_FEAT_TYPE_BUILTIN,  ///< feat_num field contains one of the ::GPS_BUILTIN_FEATURE_BITS
  DEV_FEAT_TYPE_REF_TYPE, ///< feat_num field contains one of the ::PCPS_REF_TYPES
  DEV_FEAT_TYPE_PCPS,     ///< feat_num field contains one of the ::PCPS_FEATURE_BITS
  DEV_FEAT_TYPE_RI,       ///< feat_num field contains one of the ::GPS_FEATURE_BITS
  DEV_FEAT_TYPE_XFEAT,    ///< feat_num field contains one of the ::MBG_XFEATURE_BITS
  DEV_FEAT_TYPE_TLV_FEAT, ///< feat_num field contains one of the ::MBG_TLV_FEAT_TYPES
  N_DEV_FEAT_TYPES
};



/**
 * @brief Device driver information
 *
 * Used to pass info on the device driver to
 * a user space application.
 */
typedef struct
{
  uint16_t ver_num;    ///< the device driver's version number
  uint16_t n_devs;     ///< the number of devices handled by the driver
  PCPS_ID_STR id_str;  ///< the device driver's ID string

} PCPS_DRVR_INFO;



// The macros below can be used to mark a PCPS_TIME variable
// as unread, i.e. its contents have not been read from the clock,
// and to check if such a variable is marked as unread.
#define _pcps_time_set_unread( _t )       do { (_t)->sec = 0xFF; } while ( 0 )
#define _pcps_time_is_read( _t )          ( (uchar) (_t)->sec != 0xFF )



/**
 * @brief Time read from a device plus associated system cycles count
 *
 * Contains current time from a device, plus an associated PC
 * cycles counter value useful to compensate execution time.
 */
typedef struct
{
  MBG_PC_CYCLES cycles;
  PCPS_TIME t;

} PCPS_TIME_CYCLES;



/**
 * @brief High resolution time stamp plus associated system cycles count
 *
 * Contains current high resolution UTC time stamp from a device, plus
 * an associated PC cycles counter value useful to compensate execution time.
 */
typedef struct
{
  MBG_PC_CYCLES cycles;
  PCPS_TIME_STAMP tstamp;     ///< High resolution time stamp (%UTC)

} PCPS_TIME_STAMP_CYCLES;

#define _mbg_swab_pcps_time_stamp_cycles( _p ) \
do                                             \
{                                              \
  _mbg_swab_mbg_pc_cycles( &(_p)->cycles );    \
  _mbg_swab_pcps_time_stamp( &(_p)->tstamp );  \
} while ( 0 )



/**
 * @brief High resolution time plus associated system cycles count
 *
 * Contains current high resolution %UTC time from a device, including
 * local time offset and status, plus an associated PC cycles counter value
 * useful to compensate execution time.
 */
typedef struct
{
  MBG_PC_CYCLES cycles;
  PCPS_HR_TIME t;

} PCPS_HR_TIME_CYCLES;

#define _mbg_swab_pcps_hr_time_cycles( _p ) \
do                                          \
{                                           \
  _mbg_swab_mbg_pc_cycles( &(_p)->cycles ); \
  _mbg_swab_pcps_hr_time( &(_p)->t );       \
} while ( 0 )



/**
 * @brief High resolution device time, system time, and associated cycles counts
 *
 * Used to let the kernel driver read the current system time plus the associated
 * high resolution time from a bus-level device as close as possible, and return
 * the results to the caller which can then compute the time difference, taking
 * into account the latencies determined from the cycles counts.
 *
 * This structure also contains the card's status information (e.g. sync status).
 */
typedef struct
{
  PCPS_HR_TIME_CYCLES ref_hr_time_cycles;  ///< HR time read from the card, plus cycles
  MBG_SYS_TIME_CYCLES sys_time_cycles;     ///< system time stamp plus associated cycles

} MBG_TIME_INFO_HRT;

#define _mbg_swab_mbg_time_info_hrt( _p )                     \
do                                                            \
{                                                             \
  _mbg_swab_pcps_hr_time_cycles( &(_p)->ref_hr_time_cycles ); \
  _mbg_swab_mbg_sys_time_cycles( &(_p)->sys_time_cycles );    \
} while ( 0 )



/**
 * @brief High resolution device time stamp, system time, and associated cycles counts
 *
 * Used to let the kernel driver read the current system time plus the associated
 * high resolution time stamp from a bus-level device as close as possible, and return
 * the results to the caller which can then compute the time difference, taking
 * into account the latencies determined from the cycles counts.
 *
 * Since the card's time stamp is taken from the fast memory mapped registers
 * this structure does ***not*** contain the card's status information (e.g. sync status).
 */
typedef struct
{
  PCPS_TIME_STAMP_CYCLES ref_tstamp_cycles;  ///< HR timestamp from the card, plus cycles
  MBG_SYS_TIME_CYCLES sys_time_cycles;       ///< system timestamp plus associated cycles

} MBG_TIME_INFO_TSTAMP;

#define _mbg_swab_mbg_time_info_tstamp( _p )                    \
do                                                              \
{                                                               \
  _mbg_swab_pcps_time_stamp_cycles( &(_p)->ref_tstamp_cycles ); \
  _mbg_swab_mbg_sys_time_cycles( &(_p)->sys_time_cycles );      \
} while ( 0 )



/**
 * @defgroup group_irq_stat_info IRQ status information
 *
 * @anchor PCPS_IRQ_STAT_INFO_DEFS
 *
 * @{ */

typedef uint32_t PCPS_IRQ_STAT_INFO;

// Flags used with PCPS_IRQ_STAT_INFO:
#define PCPS_IRQ_STAT_ENABLE_CALLED  0x01
#define PCPS_IRQ_STAT_ENABLED        0x02
#define PCPS_IRQ_STAT_UNSAFE         0x04  // IRQs unsafe with this firmeware version / ASIC

#define PCPS_IRQ_STATE_DANGER        ( PCPS_IRQ_STAT_ENABLED | PCPS_IRQ_STAT_UNSAFE )

/** @} defgroup group_irq_stat_info */



#define PCPS_FW_STR_FMT  "%X.%02X"

#define _pcps_fw_rev_num_major( _v ) \
  ( (_v) >> 8 )

#define _pcps_fw_rev_num_minor( _v ) \
  ( (_v) & 0xFF )



#if defined( _USING_BYTE_ALIGNMENT )
  #pragma pack()      // set default alignment
  #undef _USING_BYTE_ALIGNMENT
#endif


#if !defined( MBG_TGT_KERNEL )

static __mbg_inline
void setup_hr_time_cycles_from_timestamp_cycles( PCPS_HR_TIME_CYCLES *p_ht_c,
                                                 const PCPS_TIME_STAMP_CYCLES *p_ts_c )
{
  memset( p_ht_c, 0, sizeof( *p_ht_c ) );

  p_ht_c->t.tstamp = p_ts_c->tstamp;
  p_ht_c->cycles = p_ts_c->cycles;

}  // setup_hr_time_cycles_from_timestamp_cycles

#endif



/**
 * @brief A string buffer for a unique device ID
 *
 * The unique ID is made up of the device model name
 * and its serial number, i.e. [model_name]_[serial_number]
 * E.g.: "GPS170PCI_028210040670"
 *
 * @see ::snprint_dev_name
 * @see ::MBG_DEV_NAME_FMT
 */
typedef char MBG_DEV_NAME[PCPS_CLOCK_NAME_SZ + PCPS_SN_SIZE + 1];



/**
 * @brief A string format specifier for ::MBG_DEV_NAME
 *
 * @see ::snprint_dev_name
 * @see ::MBG_DEV_NAME_FMT
 */
#define MBG_DEV_NAME_FMT  "%s_%s"


/* End of header body */

#undef _ext

#endif  /* _PCPSDEV_H */

