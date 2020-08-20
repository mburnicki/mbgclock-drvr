
/**************************************************************************
 *
 *  $Id: mbgioctl.h 1.34 2019/04/03 12:28:11 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions used with device driver IOCTL.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgioctl.h $
 *  Revision 1.34  2019/04/03 12:28:11  martin
 *  Removed obsolete definition PCPS_SPIN_BUFFER.
 *  Revision 1.33  2019/02/08 16:06:52  martin
 *  Moved the Windows GUID stuff elsewhere.
 *  Revision 1.32  2018/11/22 15:01:44  martin
 *  Refactored structure IOCTL_GENERIC_REQ using fixed size integer
 *  types only to avoid pointer size problems in a mixed 32/64 bit
 *  Linux environment.
 *  Revision 1.31  2018/11/06 17:20:25  martin
 *  Updated function prototypes and a comment.
 *  Revision 1.30  2018/08/24 10:03:46Z  martin
 *  Removed code that was commented out.
 *  Added a comment.
 *  Revision 1.29  2018/08/08 15:36:50Z  martin
 *  Renamed a local macro for clarity.
 *  Revision 1.28  2018/07/05 09:02:29Z  martin
 *  Always use DMA-capable buffers as required by Linux kernels 4.9+.
 *  Moved definition of PCPS_IO_BUFFER and some related
 *  types to new header file pcpsiobf.h.
 *  Common, OS-specific definitions for IOCTL return codes.
 *  Updated function prototypes.
 *  Revision 1.27  2017/07/05 09:37:18  martin
 *  Definitions to support GPIO ports and XMR.
 *  Support new way to check if specific feature supported.
 *  Moved some IOCTL-related definitions from pcpsdev.h here.
 *  Added some doxygen comments.
 *  Revision 1.26  2013/09/26 08:27:04Z  martin
 *  Support GNSS API.
 *  Revision 1.25  2012/10/02 18:45:55  martin
 *  There are some g++ versions which fail to compile source code using
 *  the macros provided by Linux to define IOCTL codes. If only the API
 *  functions are called by an application then the IOCTL codes aren't
 *  required anyway, so we just avoid inclusion of mbgioctl.h.
 *  However, some IOCTL related definitions are required anyway, so
 *  they have been moved to pcpsdev.h which is always included.
 *  Support on-board event logs.
 *  Support debug status.
 *  Conditionally use older IOCTL request buffer structures.
 *  Modified generic IOCTL handling such that for calls requiring variable sizes
 *  a fixed request block containing input and output buffer pointers and sizes is
 *  passed down to the kernel driver. This simplifies implementation under *BSD
 *  and also works for other target systems.
 *  Support reading CORR_INFO, and reading/writing TR_DISTANCE.
 *  New code IOCTL_DEV_HAS_PZF.
 *  Support PTP unicast configuration.
 *  Changed the names of a few IOCTL codes to follow general naming conventions.
 *  Added definitions to support privilege level requirements for IOCTLs.
 *  Use native alignment to avoid problems on Sparc and IA64.
 *  Added definitions to set up a table of all known
 *  IOCTL codes and names.
 *  Use MBG_TGT_KERNEL instead of _KDD_.
 *  Use IOTYPE 'Z' under *BSD since this means passthrough on NetBSD.
 *  Revision 1.24  2009/12/15 15:34:59Z  daniel
 *  Support reading the raw IRIG data bits for firmware versions
 *  which support this feature.
 *  Revision 1.23  2009/09/29 15:08:41Z  martin
 *  Support retrieving time discipline info.
 *  Revision 1.22  2009/08/17 13:48:17  martin
 *  Moved specific definition of symbol _HAVE_IOCTL_WITH_SIZE from
 *  mbgdevio.c here and renamed it to _MBG_SUPP_VAR_ACC_SIZE.
 *  Revision 1.21  2009/06/19 12:18:53  martin
 *  Added PCPS_GIVE_IRIG_TIME command and associated definitions.
 *  Fixed a declaration which might have led to syntax errors.
 *  Revision 1.20  2009/06/09 10:02:36Z  daniel
 *  Support PTP configuration and state.
 *  Support simple LAN interface configuration.
 *  Revision 1.19  2009/03/19 15:17:59  martin
 *  Support reading MM timestamps without cycles.
 *  Support UTC parms and configurable time scales.
 *  For consistent naming renamed IOCTL_GET_FAST_HR_TIMESTAMP
 *  to IOCTL_GET_FAST_HR_TIMESTAMP_CYCLES.
 *  Added IOCTL_DEV_HAS_IRIG_CTRL_BITS and IOCTL_GET_IRIG_CTRL_BITS.
 *  Revision 1.18  2008/12/11 10:32:56Z  martin
 *  Added _cmd_from_ioctl_code() macro for Linux.
 *  Added IOCTL codes for .._has_asic_version() and .._has_asic_features().
 *  Added IOCTL codes for ..._is_msf(), .._is_lwr(), .._is_wwvb().
 *  Added IOCTL codes IOCTL_GET_IRQ_STAT_INFO, IOCTL_GET_CYCLES_FREQUENCY,
 *  IOCTL_HAS_FAST_HR_TIMESTAMP, and IOCTL_GET_FAST_HR_TIMESTAMP.
 *  Revision 1.17  2008/01/17 09:35:15  daniel
 *  Added ioctl calls IOCTL_GET_MAPPED_MEM_ADDR and
 *  IOCTL_UNMAP_MAPPED_MEM.
 *  Cleanup for PCI ASIC version and features.
 *  Revision 1.16  2007/09/25 10:37:04Z  martin
 *  Added macro _cmd_from_ioctl_code() for Windows.
 *  Revision 1.15  2007/05/21 15:00:01Z  martin
 *  Unified naming convention for symbols related to ref_offs.
 *  Revision 1.14  2007/03/02 10:27:03  martin
 *  Preliminary support for *BSD.
 *  Preliminary _cmd_from_ioctl().
 *  Revision 1.13  2006/03/10 10:36:54  martin
 *  Added support for programmable pulse outputs.
 *  Revision 1.12  2005/06/02 10:22:05Z  martin
 *  Added IOCTL code IOCTL_GET_SYNTH_STATE.
 *  Added IOCTL codes IOCTL_DEV_HAS_GENERIC_IO,
 *  IOCTL_PCPS_GENERIC_IO, and IOCTL_GET_SYNTH_STATE.
 *  Revision 1.11  2005/01/14 10:21:11Z  martin
 *  Added IOCTLs which query device features.
 *  Revision 1.10  2004/12/09 11:03:36Z  martin
 *  Support configuration of on-board frequency synthesizer.
 *  Revision 1.9  2004/11/09 12:49:41Z  martin
 *  Modifications were required in order to be able to configure IRIG
 *  settings of cards which provide both IRIG input and output.
 *  The existing codes have been renamed with .._RX.. and are used to
 *  configure the IRIG receiver (input). New codes have been defined
 *  used to configure the IRIG transmitter.
 *  Renamed IOCTL_GET_GPS_STAT to IOCTL_GET_GPS_BVAR_STAT.
 *  Use more specific data types than generic types.
 *  Modified IOCTL codes used for hardware debugging.
 *  Revision 1.8  2004/09/06 15:46:04Z  martin
 *  Changed definition of IOCTL codes to support syntax used
 *  with Linux kernel 2.6.x.
 *  Account for renamed symbols.
 *  Revision 1.7  2004/04/07 10:08:11  martin
 *  Added IOCTL codes used to trigger hardware debug events.
 *  Revision 1.6  2003/12/22 15:37:18Z  martin
 *  Added codes to read ASIC version, and read times
 *  with associated cycle counter values.
 *  Revision 1.5  2003/06/19 09:02:30Z  martin
 *  New codes IOCTL_GET_PCPS_UCAP_ENTRIES and IOCTL_GET_PCPS_UCAP_EVENT.
 *  Renamed IOCTL_PCPS_CLR_CAP_BUFF to IOCTL_PCPS_CLR_UCAP_BUFF.
 *  Cleaned up IOCTL code names related to PCPS_TZDL.
 *  Reordered upper IOCTL code numbers again.
 *  Revision 1.4  2003/04/09 13:50:39Z  martin
 *  Re-organized IOCTL codes.
 *  Supports Win32.
 *  Added missing pragma pack().
 *  Revision 1.3  2003/02/14 13:20:08Z  martin
 *  Include mbggeo.h instead of mygeo.h.
 *  Revision 1.2  2001/11/30 09:52:48  martin
 *  Added support for event_time which, however, requires
 *  a custom GPS firmware.
 *  Revision 1.1  2001/03/05 16:34:22  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#ifndef _MBGIOCTL_H
#define _MBGIOCTL_H


/* Other headers to be included */

#include <mbg_tgt.h>
#include <cfg_hlp.h>
#include <mbgerror.h>
#include <mbggeo.h>
#include <pcpsdev.h>
#include <pci_asic.h>
#include <pcpsiobf.h>

#if defined( MBG_TGT_LINUX )
  #include <linux/ioctl.h>
#endif

#if defined( MBG_TGT_BSD )
  #include <sys/ioccom.h>
#endif

#if defined( MBG_TGT_WIN32 )

  #if !defined( MBG_TGT_KERNEL )
    #include <winioctl.h>
  #endif

#endif



/* Start of header body */

// We have to use native alignment here!

#ifdef __cplusplus
extern "C" {
#endif


#if defined( MBG_ARCH_X86 ) && defined( DEBUG ) && DEBUG
  #define USE_DEBUG_PORT  1
#else
  #define USE_DEBUG_PORT  0
#endif


#if defined( MBG_TGT_POSIX ) && !defined( MBG_TGT_QNX_NTO )
  #define MBG_HAS_POSIX_IOCTL  1
#endif


// A kernel driver's IOCTL handler should return some system-specific
// error code in case of failure.
// We define the most appropriate return codes for specific error
// conditions here, and the IOCTL caller has to map them back to
// one of the @ref MBG_ERROR_CODES.

#if defined( MBG_TGT_WIN32 ) && defined( MBG_TGT_KERNEL )

#if _USE_WIN32_PRIVATE_STATUS_CODES

  // We let the kernel driver's IOCTL handler return private error
  // codes in NTSTATUS format, so the DeviceIoControl() function in
  // user space fails, and the error code returned by a subsequent
  // GetLastError() returns the original custom error code. This
  // seems to work well but is an undocumented feature. Also, it
  // looks like it's not possible to return a non-error custom
  // status code since in case of of success the DeviceIoControl()
  // function just returns a BOOL indicating success.
  // TODO Test this, and eventually move it to mbgerror.h.
  #define _mbg_krn_errno_to_os( _n ) \
    ( ( (_n) == MBG_SUCCESS ) ? STATUS_SUCCESS : \
      ( (NTSTATUS) ( STATUS_SEVERITY_ERROR | STATUS_CUSTOM_FLAG | ( (-(_n)) & 0xFFFF ) ) ) )

#else

  // We let the kernel driver's IOCTL handler return system error
  // codes in NTSTATUS format, so the DeviceIoControl() function
  // in user space fails, and a standard Win32 error code is
  // returned by a subsequent GetLastError() call.
  //
  // ::mbg_errno_to_os in kernel mode uses ::mbg_ioctl_to_ntstatus_table
  // to lookup a system NTSTATUS code which is passed up to user space.
  //
  // The Windows kernel maps that NTSTATUS code to some Win32 error code
  // which can be retrieved by calling GetLastError() if DeviceIoControl()
  // has failed.
  //
  // ::mbg_get_last_error in user space calls GetLastError() and
  // uses ::win32_error_table to lookup one of the @ref MBG_ERROR_CODES
  // associated with the retrieved Win32 error.
  // Ideally, this should correspond to the Meinberg error code
  // that has been used in kernel mode to lookup the NTSTATUS code.
  #define _mbg_krn_errno_to_os( _n ) \
    mbg_errno_to_os( _n )

#endif  // _USE_WIN32_PRIVATE_STATUS_CODES

#endif  // defined( MBG_TGT_WIN32 ) && defined( MBG_TGT_KERNEL )



#if defined( MBG_TGT_KERNEL )

#define IOCTL_RC_SUCCESS               MBG_SYS_RC_SUCCESS

#if defined( MBG_TGT_LINUX )

  // super user rights required
  #define IOCTL_RC_ERR_PERM              -EPERM   // MBG_ERR_PERM

  #define IOCTL_RC_ERR_UNSUPP_IOCTL      -ENODEV  // MBG_ERR_INV_DEV_REQUEST ##

  #define IOCTL_RC_ERR_INVAL_PARAM       -EINVAL  // MBG_ERR_INV_PARM

  #define IOCTL_RC_ERR_NOT_SUPP_BY_DEV   -ENOTTY  // MBG_ERR_NOT_SUPP_BY_DEV

  #define IOCTL_RC_ERR_NO_MEM            -ENOMEM  // MBG_ERR_NO_MEM

  #define IOCTL_RC_ERR_BUSY_IRQ_UNSAFE   -EBUSY   // MBG_ERR_BUSY, MBG_ERR_IRQ_UNSAFE

  #define IOCTL_RC_ERR_DEV_ACCESS        -EIO     // rc from low level routine

  // Linux-specific codes
  #define IOCTL_RC_ERR_COPY_TO_USER      -EFAULT  // MBG_ERR_COPY_TO_USER
  #define IOCTL_RC_ERR_COPY_FROM_USER    -EFAULT  // MBG_ERR_COPY_FROM_USER

#elif defined( MBG_TGT_BSD )  // FIXME specific

  // super user rights required
  #define IOCTL_RC_ERR_PERM              EPERM    // MBG_ERR_PERM

  #if defined( ENOIOCTL )                         // E.g. FreeBSD, ...
    #define IOCTL_RC_ERR_UNSUPP_IOCTL    ENOIOCTL // TODO or ENODEV? MBG_ERR_INV_DEV_REQUEST
  #else
    #define IOCTL_RC_ERR_UNSUPP_IOCTL    ENODEV   // MBG_ERR_INV_DEV_REQUEST
  #endif

  #define IOCTL_RC_ERR_INVAL_PARAM       EINVAL   // MBG_ERR_INV_PARM

  #define IOCTL_RC_ERR_NOT_SUPP_BY_DEV   ENOTTY   // MBG_ERR_NOT_SUPP_BY_DEV

  #define IOCTL_RC_ERR_NO_MEM            ENOMEM   // MBG_ERR_NO_MEM

  #define IOCTL_RC_ERR_BUSY_IRQ_UNSAFE   EBUSY    // MBG_ERR_BUSY, MBG_ERR_IRQ_UNSAFE

  #define IOCTL_RC_ERR_DEV_ACCESS        EIO      // TODO or EFAULT? rc from low level routine

#elif defined( MBG_TGT_WIN32 )

  #define IOCTL_RC_ERR_PERM              _mbg_krn_errno_to_os( MBG_ERR_PERM )

  #define IOCTL_RC_ERR_UNSUPP_IOCTL      _mbg_krn_errno_to_os( MBG_ERR_INV_DEV_REQUEST )

  #define IOCTL_RC_ERR_INVAL_PARAM       _mbg_krn_errno_to_os( MBG_ERR_INV_PARM )

  #define IOCTL_RC_ERR_NOT_SUPP_BY_DEV   _mbg_krn_errno_to_os( MBG_ERR_NOT_SUPP_BY_DEV )

  #define IOCTL_RC_ERR_NO_MEM            _mbg_krn_errno_to_os( MBG_ERR_NO_MEM )

  #define IOCTL_RC_ERR_BUSY_IRQ_UNSAFE   _mbg_krn_errno_to_os( MBG_ERR_IRQ_UNSAFE )

  #define IOCTL_RC_ERR_DEV_ACCESS        _mbg_krn_errno_to_os( rc )

#endif

#endif  // defined( MBG_TGT_KERNEL )



#if defined( MBG_TGT_LINUX )

  // a magic number used to generate IOCTL cmd codes
  #define IOTYPE 'M'

  #define _MBG_IO   _IO
  #define _MBG_IOR  _IOR
  #define _MBG_IOW  _IOW

  #define _cmd_from_ioctl_code( _ioc ) \
    _IOC_NR( _ioc )

#elif defined( MBG_TGT_BSD )

  // Under NetBSD 'Z' marks passthrough IOCTLs, under FreeBSD the code
  // does not seem to matter, so we use 'Z' anyway.
  #define IOTYPE 'Z'

  #define _MBG_IO   _IO
  #define _MBG_IOR  _IOR
  #define _MBG_IOW  _IOW

#elif defined( MBG_TGT_WIN32 )

  #if !defined( _MBG_SUPP_VAR_ACC_SIZE )
    // Windows supports IOCTL commands where the sizes of
    // input and output buffer can be specified dynamically.
    #define _MBG_SUPP_VAR_ACC_SIZE   1
  #endif

  // Device type in the "User Defined" range."
  #define PCPS_TYPE 40000

  // IOCTL function codes from 0x800 to 0xFFF are for customer use.
  #define _MBG_IOCTL_BIAS  0x930

  #define _MBG_IO( _t, _n ) \
    CTL_CODE( PCPS_TYPE, _MBG_IOCTL_BIAS + _n, METHOD_BUFFERED, FILE_READ_ACCESS )

  #define _MBG_IOR( _t, _n, _sz ) \
    _MBG_IO( _t, _n )

  #define _MBG_IOW  _MBG_IOR

  #define _cmd_from_ioctl_code( _ioc ) \
    ( ( ( (_ioc) >> 2 ) & 0x0FFF ) - _MBG_IOCTL_BIAS )

#endif


#if !defined( _MBG_SUPP_VAR_ACC_SIZE )
  // Many operating systems don't support specifying the sizes of IOCTL
  // input and output buffers dynamically, so we disable this by default.
  #define _MBG_SUPP_VAR_ACC_SIZE   0
#endif



// The structure below is used by the IOCTL_PCPS_GENERIC_... calls.

#if defined( MBG_TGT_LINUX )
  #if defined( MBG_ARCH_ARM ) || defined( MBG_ARCH_SPARC )
    #define USE_IOCTL_GENERIC_REQ   0
  #endif
#endif

#if defined( MBG_TGT_WIN32 )
  // required for 32bit/64 bit compatibility
  #define USE_IOCTL_GENERIC_REQ   0
#endif

#if !defined( USE_IOCTL_GENERIC_REQ )
  #define USE_IOCTL_GENERIC_REQ   1
#endif


#if USE_IOCTL_GENERIC_REQ

/**
 * @brief A structure used to pass generic IOCTL requests to the kernel driver
 *
 * In a mixed 32/64 bit environment we may have 32 bit pointers in user space
 * but 64 bit pointers in kernel space, so if we defined buffer addresses as
 * usual pointers, and buffer sizes as size_t the memory layout of this structure
 * would be different in 32 bit and 64 bit builds, so we use only fixed-size types
 * to avoid this and make sure the structure has always the same size.
 *
 * @note Pointers should generally be cast to 'uintptr_t' before being copied
 * into the 64 bit address fields. This makes sure 32 bit pointers are expanded
 * to 64 bit in an unsigned way, i.e. with the extended bytes always set to 0x00.
 * Without this cast a pointer might be extended with 0xFF if the MSB of the
 * original 32 bit pointer is set, which would result in an incompatible 64 bit
 * address in kernel space.
 * In an environment where pointers have 64 bit anyway an 'uintptr_t' cast wouldn't
 * be required, but it doesn't hurt, either.
 */
typedef struct
{
  uint64_t in_p;      ///< Address of the input buffer.
  uint64_t out_p;     ///< Address of the output buffer.
  uint32_t in_sz;     ///< Size of the input buffer.
  uint32_t out_sz;    ///< Size of the output buffer.
  uint32_t info;      ///< E.g. cmd code for the device.
  uint32_t reserved;  ///< Reserved, yet not used.

} IOCTL_GENERIC_REQ;

#define _MBG_IOG( _t, _n, _s )   _MBG_IOW( _t, _n, _s )

#else

/**
 * @brief Control structure used for generic IOCTL requests
 *
 * Used by the IOCTL_PCPS_GENERIC_... calls.
 *
 * @note Is slower, but avoids OS-specific problems occurring
 * with IOCTL_GENERIC_REQ.
 */
typedef struct
{
  uint32_t info;
  uint32_t data_size_in;
  uint32_t data_size_out;

} IOCTL_GENERIC_CTL;


/**
 * @brief Data buffer used for generic IOCTL requests
 *
 * Used by the IOCTL_PCPS_GENERIC_... calls.
 *
 * @note Is slower, but avoids OS-specific problems occurring
 * with IOCTL_GENERIC_REQ.
 */
typedef struct
{
  IOCTL_GENERIC_CTL ctl;
  uint8_t data[1];

} IOCTL_GENERIC_BUFFER;

#define _MBG_IOG( _t, _n, _s )  _MBG_IO( _t, _n )

#endif



/**
 * @brief Request buffer used to query a device feature
 */
typedef struct
{
  uint32_t feat_type;   ///< See ::DEV_FEAT_TYPES
  uint32_t feat_num;    ///< Number and range depending on ::IOCTL_DEV_FEAT_REQ::feat_type value

} IOCTL_DEV_FEAT_REQ;



typedef union
{
  IOCTL_DEV_FEAT_REQ dev_feat_req;

  PCPS_MAPPED_MEM mapped_mem;

  #if USE_IOCTL_GENERIC_REQ
    IOCTL_GENERIC_REQ req;
  #else
    IOCTL_GENERIC_CTL ctl;
  #endif

  int i;

} IOCTL_BUFFER;



/**
 * @defgroup group_ioctl_codes IOCTL codes used by Meinberg drivers
 *
 * @see ::IOCTL_CODES_TABLE
 *
 * @anchor IOCTL_CODES
 *
 * @{ */

// read general driver info, device info, and status port
#define IOCTL_GET_PCPS_DRVR_INFO         _MBG_IOR( IOTYPE, 0x00, PCPS_DRVR_INFO )
#define IOCTL_GET_PCPS_DEV               _MBG_IOR( IOTYPE, 0x01, PCPS_DEV )
#define IOCTL_GET_PCPS_STATUS_PORT       _MBG_IOR( IOTYPE, 0x02, PCPS_STATUS_PORT )

// Generic read/write operations. We define _MBG_IOW codes since these calls
// eventually pass a generic request structure IOCTL_GENERIC_REQ to the driver.
#define IOCTL_PCPS_GENERIC_READ          _MBG_IOG( IOTYPE, 0x03, IOCTL_GENERIC_REQ )
#define IOCTL_PCPS_GENERIC_WRITE         _MBG_IOG( IOTYPE, 0x04, IOCTL_GENERIC_REQ )
#define IOCTL_PCPS_GENERIC_READ_GPS      _MBG_IOG( IOTYPE, 0x05, IOCTL_GENERIC_REQ )
#define IOCTL_PCPS_GENERIC_WRITE_GPS     _MBG_IOG( IOTYPE, 0x06, IOCTL_GENERIC_REQ )

// normal direct read/write operations
#define IOCTL_GET_PCPS_TIME              _MBG_IOR( IOTYPE, 0x10, PCPS_TIME )
#define IOCTL_SET_PCPS_TIME              _MBG_IOW( IOTYPE, 0x11, PCPS_STIME )

#define IOCTL_GET_PCPS_SYNC_TIME         _MBG_IOR( IOTYPE, 0x12, PCPS_TIME )

#define IOCTL_GET_PCPS_TIME_SEC_CHANGE   _MBG_IOR( IOTYPE, 0x13, PCPS_TIME )

#define IOCTL_GET_PCPS_HR_TIME           _MBG_IOR( IOTYPE, 0x14, PCPS_HR_TIME )

// the next one is supported with custom GPS firmware only:
#define IOCTL_SET_PCPS_EVENT_TIME        _MBG_IOW( IOTYPE, 0x15, PCPS_TIME_STAMP )

#define IOCTL_GET_PCPS_SERIAL            _MBG_IOR( IOTYPE, 0x16, PCPS_SERIAL )
#define IOCTL_SET_PCPS_SERIAL            _MBG_IOW( IOTYPE, 0x17, PCPS_SERIAL )

#define IOCTL_GET_PCPS_TZCODE            _MBG_IOR( IOTYPE, 0x18, PCPS_TZCODE )
#define IOCTL_SET_PCPS_TZCODE            _MBG_IOW( IOTYPE, 0x19, PCPS_TZCODE )

#define IOCTL_GET_PCPS_TZDL              _MBG_IOR( IOTYPE, 0x1A, PCPS_TZDL )
#define IOCTL_SET_PCPS_TZDL              _MBG_IOW( IOTYPE, 0x1B, PCPS_TZDL )

#define IOCTL_GET_REF_OFFS               _MBG_IOR( IOTYPE, 0x1C, MBG_REF_OFFS )
#define IOCTL_SET_REF_OFFS               _MBG_IOW( IOTYPE, 0x1D, MBG_REF_OFFS )

#define IOCTL_GET_MBG_OPT_INFO           _MBG_IOR( IOTYPE, 0x1E, MBG_OPT_INFO )
#define IOCTL_SET_MBG_OPT_SETTINGS       _MBG_IOW( IOTYPE, 0x1F, MBG_OPT_SETTINGS )

#define IOCTL_GET_PCPS_IRIG_RX_INFO      _MBG_IOR( IOTYPE, 0x20, IRIG_INFO )
#define IOCTL_SET_PCPS_IRIG_RX_SETTINGS  _MBG_IOW( IOTYPE, 0x21, IRIG_SETTINGS )

#define IOCTL_PCPS_CLR_UCAP_BUFF         _MBG_IO(  IOTYPE, 0x22 )
#define IOCTL_GET_PCPS_UCAP_ENTRIES      _MBG_IOR( IOTYPE, 0x23, PCPS_UCAP_ENTRIES )
#define IOCTL_GET_PCPS_UCAP_EVENT        _MBG_IOR( IOTYPE, 0x24, PCPS_HR_TIME )


#define IOCTL_GET_GPS_TZDL               _MBG_IOR( IOTYPE, 0x25, TZDL )
#define IOCTL_SET_GPS_TZDL               _MBG_IOW( IOTYPE, 0x26, TZDL )

#define IOCTL_GET_GPS_SW_REV             _MBG_IOR( IOTYPE, 0x27, SW_REV )

#define IOCTL_GET_GPS_BVAR_STAT          _MBG_IOR( IOTYPE, 0x28, BVAR_STAT )

#define IOCTL_GET_GPS_TIME               _MBG_IOR( IOTYPE, 0x29, TTM )
#define IOCTL_SET_GPS_TIME               _MBG_IOW( IOTYPE, 0x2A, TTM )

#define IOCTL_GET_GPS_PORT_PARM          _MBG_IOR( IOTYPE, 0x2B, PORT_PARM )
#define IOCTL_SET_GPS_PORT_PARM          _MBG_IOW( IOTYPE, 0x2C, PORT_PARM )

#define IOCTL_GET_GPS_ANT_INFO           _MBG_IOR( IOTYPE, 0x2D, ANT_INFO )

#define IOCTL_GET_GPS_UCAP               _MBG_IOR( IOTYPE, 0x2E, TTM )

#define IOCTL_GET_GPS_ENABLE_FLAGS       _MBG_IOR( IOTYPE, 0x2F, ENABLE_FLAGS )
#define IOCTL_SET_GPS_ENABLE_FLAGS       _MBG_IOW( IOTYPE, 0x30, ENABLE_FLAGS )

#define IOCTL_GET_GPS_STAT_INFO          _MBG_IOR( IOTYPE, 0x31, STAT_INFO )

#define IOCTL_SET_GPS_CMD                _MBG_IOW( IOTYPE, 0x32, GPS_CMD )

#define IOCTL_GET_GPS_IDENT              _MBG_IOR( IOTYPE, 0x33, IDENT )

#define IOCTL_GET_GPS_POS                _MBG_IOR( IOTYPE, 0x34, POS )
#define IOCTL_SET_GPS_POS_XYZ            _MBG_IOW( IOTYPE, 0x35, XYZ )
#define IOCTL_SET_GPS_POS_LLA            _MBG_IOW( IOTYPE, 0x36, LLA )

#define IOCTL_GET_GPS_ANT_CABLE_LEN      _MBG_IOR( IOTYPE, 0x37, ANT_CABLE_LEN )
#define IOCTL_SET_GPS_ANT_CABLE_LEN      _MBG_IOW( IOTYPE, 0x38, ANT_CABLE_LEN )

#define IOCTL_GET_GPS_RECEIVER_INFO      _MBG_IOR( IOTYPE, 0x39, RECEIVER_INFO )
#define IOCTL_GET_GPS_ALL_STR_TYPE_INFO  _MBG_IOG( IOTYPE, 0x3A, IOCTL_GENERIC_REQ )  // variable size
#define IOCTL_GET_GPS_ALL_PORT_INFO      _MBG_IOG( IOTYPE, 0x3B, IOCTL_GENERIC_REQ )  // variable size

#define IOCTL_SET_GPS_PORT_SETTINGS_IDX  _MBG_IOW( IOTYPE, 0x3C, PORT_SETTINGS_IDX )

#define IOCTL_GET_PCI_ASIC_VERSION       _MBG_IOR( IOTYPE, 0x3D, PCI_ASIC_VERSION )

#define IOCTL_GET_PCPS_TIME_CYCLES       _MBG_IOR( IOTYPE, 0x3E, PCPS_TIME_CYCLES )
#define IOCTL_GET_PCPS_HR_TIME_CYCLES    _MBG_IOR( IOTYPE, 0x3F, PCPS_HR_TIME_CYCLES )

#define IOCTL_GET_PCPS_IRIG_TX_INFO      _MBG_IOR( IOTYPE, 0x40, IRIG_INFO )
#define IOCTL_SET_PCPS_IRIG_TX_SETTINGS  _MBG_IOW( IOTYPE, 0x41, IRIG_SETTINGS )

#define IOCTL_GET_SYNTH                  _MBG_IOR( IOTYPE, 0x42, SYNTH )
#define IOCTL_SET_SYNTH                  _MBG_IOW( IOTYPE, 0x43, SYNTH )


#define IOCTL_DEV_IS_GPS                 _MBG_IOR( IOTYPE, 0x44, int )
#define IOCTL_DEV_IS_DCF                 _MBG_IOR( IOTYPE, 0x45, int )
#define IOCTL_DEV_IS_IRIG_RX             _MBG_IOR( IOTYPE, 0x46, int )

#define IOCTL_DEV_HAS_HR_TIME            _MBG_IOR( IOTYPE, 0x47, int )
#define IOCTL_DEV_HAS_CAB_LEN            _MBG_IOR( IOTYPE, 0x48, int )
#define IOCTL_DEV_HAS_TZDL               _MBG_IOR( IOTYPE, 0x49, int )
#define IOCTL_DEV_HAS_PCPS_TZDL          _MBG_IOR( IOTYPE, 0x4A, int )
#define IOCTL_DEV_HAS_TZCODE             _MBG_IOR( IOTYPE, 0x4B, int )
#define IOCTL_DEV_HAS_TZ                 _MBG_IOR( IOTYPE, 0x4C, int )
#define IOCTL_DEV_HAS_EVENT_TIME         _MBG_IOR( IOTYPE, 0x4D, int )
#define IOCTL_DEV_HAS_RECEIVER_INFO      _MBG_IOR( IOTYPE, 0x4E, int )
#define IOCTL_DEV_CAN_CLR_UCAP_BUFF      _MBG_IOR( IOTYPE, 0x4F, int )
#define IOCTL_DEV_HAS_UCAP               _MBG_IOR( IOTYPE, 0x50, int )
#define IOCTL_DEV_HAS_IRIG_TX            _MBG_IOR( IOTYPE, 0x51, int )
#define IOCTL_DEV_HAS_SERIAL_HS          _MBG_IOR( IOTYPE, 0x52, int )
#define IOCTL_DEV_HAS_SIGNAL             _MBG_IOR( IOTYPE, 0x53, int )
#define IOCTL_DEV_HAS_MOD                _MBG_IOR( IOTYPE, 0x54, int )
#define IOCTL_DEV_HAS_IRIG               _MBG_IOR( IOTYPE, 0x55, int )
#define IOCTL_DEV_HAS_REF_OFFS           _MBG_IOR( IOTYPE, 0x56, int )
#define IOCTL_DEV_HAS_OPT_FLAGS          _MBG_IOR( IOTYPE, 0x57, int )
#define IOCTL_DEV_HAS_GPS_DATA           _MBG_IOR( IOTYPE, 0x58, int )
#define IOCTL_DEV_HAS_SYNTH              _MBG_IOR( IOTYPE, 0x59, int )
#define IOCTL_DEV_HAS_GENERIC_IO         _MBG_IOR( IOTYPE, 0x5A, int )

#define IOCTL_PCPS_GENERIC_IO            _MBG_IOG( IOTYPE, 0x5B, IOCTL_GENERIC_REQ )

#define IOCTL_GET_SYNTH_STATE            _MBG_IOR( IOTYPE, 0x5C, SYNTH_STATE )

#define IOCTL_GET_GPS_ALL_POUT_INFO      _MBG_IOG( IOTYPE, 0x5D, IOCTL_GENERIC_REQ )  // variable size
#define IOCTL_SET_GPS_POUT_SETTINGS_IDX  _MBG_IOW( IOTYPE, 0x5E, POUT_SETTINGS_IDX )

#define IOCTL_GET_MAPPED_MEM_ADDR        _MBG_IOR( IOTYPE, 0x5F, PCPS_MAPPED_MEM )
#define IOCTL_UNMAP_MAPPED_MEM           _MBG_IOR( IOTYPE, 0x60, PCPS_MAPPED_MEM )

#define IOCTL_GET_PCI_ASIC_FEATURES      _MBG_IOR( IOTYPE, 0x61, PCI_ASIC_FEATURES )

#define IOCTL_DEV_HAS_PCI_ASIC_FEATURES  _MBG_IOR( IOTYPE, 0x62, int )
#define IOCTL_DEV_HAS_PCI_ASIC_VERSION   _MBG_IOR( IOTYPE, 0x63, int )

#define IOCTL_DEV_IS_MSF                 _MBG_IOR( IOTYPE, 0x64, int )
#define IOCTL_DEV_IS_LWR                 _MBG_IOR( IOTYPE, 0x65, int )
#define IOCTL_DEV_IS_WWVB                _MBG_IOR( IOTYPE, 0x66, int )

#define IOCTL_GET_IRQ_STAT_INFO          _MBG_IOR( IOTYPE, 0x67, PCPS_IRQ_STAT_INFO )
#define IOCTL_GET_CYCLES_FREQUENCY       _MBG_IOR( IOTYPE, 0x68, MBG_PC_CYCLES_FREQUENCY )

#define IOCTL_DEV_HAS_FAST_HR_TIMESTAMP     _MBG_IOR( IOTYPE, 0x69, int )
#define IOCTL_GET_FAST_HR_TIMESTAMP_CYCLES  _MBG_IOR( IOTYPE, 0x6A, PCPS_TIME_STAMP_CYCLES )
#define IOCTL_GET_FAST_HR_TIMESTAMP         _MBG_IOR( IOTYPE, 0x6B, PCPS_TIME_STAMP )

#define IOCTL_DEV_HAS_GPS_TIME_SCALE        _MBG_IOR( IOTYPE, 0x6C, int )
#define IOCTL_GET_GPS_TIME_SCALE_INFO       _MBG_IOR( IOTYPE, 0x6D, MBG_TIME_SCALE_INFO )
#define IOCTL_SET_GPS_TIME_SCALE_SETTINGS   _MBG_IOW( IOTYPE, 0x6E, MBG_TIME_SCALE_SETTINGS )

#define IOCTL_DEV_HAS_GPS_UTC_PARM       _MBG_IOR( IOTYPE, 0x6F, int )
#define IOCTL_GET_GPS_UTC_PARM           _MBG_IOR( IOTYPE, 0x70, UTC )
#define IOCTL_SET_GPS_UTC_PARM           _MBG_IOW( IOTYPE, 0x71, UTC )

#define IOCTL_DEV_HAS_IRIG_CTRL_BITS     _MBG_IOR( IOTYPE, 0x72, int )
#define IOCTL_GET_IRIG_CTRL_BITS         _MBG_IOR( IOTYPE, 0x73, MBG_IRIG_CTRL_BITS )

#define IOCTL_DEV_HAS_LAN_INTF           _MBG_IOR( IOTYPE, 0x74, int )
#define IOCTL_GET_LAN_IF_INFO            _MBG_IOR( IOTYPE, 0x75, LAN_IF_INFO )
#define IOCTL_GET_IP4_STATE              _MBG_IOR( IOTYPE, 0x76, IP4_SETTINGS )
#define IOCTL_GET_IP4_SETTINGS           _MBG_IOR( IOTYPE, 0x77, IP4_SETTINGS )
#define IOCTL_SET_IP4_SETTINGS           _MBG_IOW( IOTYPE, 0x78, IP4_SETTINGS )

#define IOCTL_DEV_IS_PTP                 _MBG_IOR( IOTYPE, 0x79, int )
#define IOCTL_DEV_HAS_PTP                _MBG_IOR( IOTYPE, 0x7A, int )
#define IOCTL_GET_PTP_STATE              _MBG_IOR( IOTYPE, 0x7B, PTP_STATE )
#define IOCTL_GET_PTP_CFG_INFO           _MBG_IOR( IOTYPE, 0x7C, PTP_CFG_INFO )
#define IOCTL_SET_PTP_CFG_SETTINGS       _MBG_IOW( IOTYPE, 0x7D, PTP_CFG_SETTINGS )

#define IOCTL_DEV_HAS_IRIG_TIME          _MBG_IOR( IOTYPE, 0x7E, int )
#define IOCTL_GET_IRIG_TIME              _MBG_IOR( IOTYPE, 0x7F, PCPS_IRIG_TIME )

#define IOCTL_GET_TIME_INFO_HRT          _MBG_IOR( IOTYPE, 0x80, MBG_TIME_INFO_HRT )
#define IOCTL_GET_TIME_INFO_TSTAMP       _MBG_IOR( IOTYPE, 0x81, MBG_TIME_INFO_TSTAMP )

#define IOCTL_DEV_HAS_RAW_IRIG_DATA      _MBG_IOR( IOTYPE, 0x82, int )
#define IOCTL_GET_RAW_IRIG_DATA          _MBG_IOR( IOTYPE, 0x83, MBG_RAW_IRIG_DATA )

#define IOCTL_DEV_HAS_PTP_UNICAST             _MBG_IOR( IOTYPE, 0x84, int )
#define IOCTL_PTP_UC_MASTER_CFG_LIMITS        _MBG_IOR( IOTYPE, 0x85, PTP_UC_MASTER_CFG_LIMITS )
#define IOCTL_GET_ALL_PTP_UC_MASTER_INFO      _MBG_IOG( IOTYPE, 0x86, IOCTL_GENERIC_REQ )  // variable size
#define IOCTL_SET_PTP_UC_MASTER_SETTINGS_IDX  _MBG_IOW( IOTYPE, 0x87, PTP_UC_MASTER_SETTINGS_IDX )

#define IOCTL_DEV_HAS_PZF                _MBG_IOR( IOTYPE, 0x88, int )
#define IOCTL_DEV_HAS_CORR_INFO          _MBG_IOR( IOTYPE, 0x89, int )
#define IOCTL_DEV_HAS_TR_DISTANCE        _MBG_IOR( IOTYPE, 0x8A, int )
#define IOCTL_GET_CORR_INFO              _MBG_IOR( IOTYPE, 0x8B, CORR_INFO )
#define IOCTL_GET_TR_DISTANCE            _MBG_IOR( IOTYPE, 0x8C, TR_DISTANCE )
#define IOCTL_SET_TR_DISTANCE            _MBG_IOW( IOTYPE, 0x8D, TR_DISTANCE )

#define IOCTL_DEV_HAS_DEBUG_STATUS       _MBG_IOR( IOTYPE, 0x8E, int )
#define IOCTL_GET_DEBUG_STATUS           _MBG_IOR( IOTYPE, 0x8F, MBG_DEBUG_STATUS )

#define IOCTL_DEV_HAS_EVT_LOG            _MBG_IOR( IOTYPE, 0x90, int )
#define IOCTL_CLR_EVT_LOG                _MBG_IO(  IOTYPE, 0x91 )
#define IOCTL_GET_NUM_EVT_LOG_ENTRIES    _MBG_IOR( IOTYPE, 0x92, MBG_NUM_EVT_LOG_ENTRIES )
#define IOCTL_GET_FIRST_EVT_LOG_ENTRY    _MBG_IOR( IOTYPE, 0x93, MBG_EVT_LOG_ENTRY )
#define IOCTL_GET_NEXT_EVT_LOG_ENTRY     _MBG_IOR( IOTYPE, 0x94, MBG_EVT_LOG_ENTRY )

#define IOCTL_DEV_IS_GNSS                _MBG_IOR( IOTYPE, 0x95, int )
#define IOCTL_GET_GNSS_MODE_INFO         _MBG_IOR( IOTYPE, 0x96, MBG_GNSS_MODE_INFO )
#define IOCTL_SET_GNSS_MODE_SETTINGS     _MBG_IOW( IOTYPE, 0x97, MBG_GNSS_MODE_SETTINGS )
#define IOCTL_GET_ALL_GNSS_SAT_INFO      _MBG_IOG( IOTYPE, 0x98, IOCTL_GENERIC_REQ )  // variable size

#define IOCTL_DEV_HAS_GPIO               _MBG_IOR( IOTYPE, 0x99, int )
#define IOCTL_GET_GPIO_CFG_LIMITS        _MBG_IOR( IOTYPE, 0x9A, MBG_GPIO_CFG_LIMITS )
#define IOCTL_GET_ALL_GPIO_INFO          _MBG_IOG( IOTYPE, 0x9B, IOCTL_GENERIC_REQ )  // variable size
#define IOCTL_SET_GPIO_SETTINGS_IDX      _MBG_IOW( IOTYPE, 0x9C, MBG_GPIO_SETTINGS_IDX )

#define IOCTL_DEV_HAS_XMR                _MBG_IOR( IOTYPE, 0x9D, int )
#define IOCTL_GET_XMR_INSTANCES          _MBG_IOR( IOTYPE, 0x9E, XMULTI_REF_INSTANCES )
#define IOCTL_GET_ALL_XMR_INFO           _MBG_IOG( IOTYPE, 0x9F, IOCTL_GENERIC_REQ )  // variable size
#define IOCTL_SET_XMR_SETTINGS_IDX       _MBG_IOW( IOTYPE, 0xA0, XMULTI_REF_SETTINGS_IDX )
#define IOCTL_GET_ALL_XMR_STATUS         _MBG_IOG( IOTYPE, 0xA1, IOCTL_GENERIC_REQ )  // variable size
#define IOCTL_GET_XMR_HOLDOVER_STATUS    _MBG_IOR( IOTYPE, 0xA2, XMR_HOLDOVER_STATUS )

#define IOCTL_GET_ALL_GPIO_STATUS        _MBG_IOG( IOTYPE, 0xA3, IOCTL_GENERIC_REQ )  // variable size
#define IOCTL_CHK_DEV_FEAT               _MBG_IOW( IOTYPE, 0xA4, IOCTL_DEV_FEAT_REQ )

// The codes below are subject to changes without notice. They may be supported
// by some kernel drivers, but usage is restricted to Meinberg software development.
// Unrestricted usage may cause system malfunction !!
#define IOCTL_MBG_DBG_GET_PORT_ADDR      _MBG_IOR( IOTYPE, 0xF0, uint16_t )
#define IOCTL_MBG_DBG_SET_PORT_ADDR      _MBG_IOW( IOTYPE, 0xF1, uint16_t )
#define IOCTL_MBG_DBG_SET_BIT            _MBG_IOW( IOTYPE, 0xF2, uint8_t )
#define IOCTL_MBG_DBG_CLR_BIT            _MBG_IOW( IOTYPE, 0xF3, uint8_t )
#define IOCTL_MBG_DBG_CLR_ALL            _MBG_IO(  IOTYPE, 0xF4 )

/** @} defgroup group_ioctl_codes */



/**
 * @brief An initializer for a table of IOCTL codes and associated names.
 *
 * This can e.g. initialize an array of ::MBG_CODE_NAME_TABLE_ENTRY elements
 * and may be helpful when debugging.
 *
 * @see @ref IOCTL_CODES
 */
#define IOCTL_CODES_TABLE                                      \
{                                                              \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_DRVR_INFO ),             \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_DEV ),                   \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_STATUS_PORT ),           \
  _mbg_cn_table_entry( IOCTL_PCPS_GENERIC_READ ),              \
  _mbg_cn_table_entry( IOCTL_PCPS_GENERIC_WRITE ),             \
  _mbg_cn_table_entry( IOCTL_PCPS_GENERIC_READ_GPS ),          \
  _mbg_cn_table_entry( IOCTL_PCPS_GENERIC_WRITE_GPS ),         \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_TIME ),                  \
  _mbg_cn_table_entry( IOCTL_SET_PCPS_TIME ),                  \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_SYNC_TIME ),             \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_TIME_SEC_CHANGE ),       \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_HR_TIME ),               \
  _mbg_cn_table_entry( IOCTL_SET_PCPS_EVENT_TIME ),            \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_SERIAL ),                \
  _mbg_cn_table_entry( IOCTL_SET_PCPS_SERIAL ),                \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_TZCODE ),                \
  _mbg_cn_table_entry( IOCTL_SET_PCPS_TZCODE ),                \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_TZDL ),                  \
  _mbg_cn_table_entry( IOCTL_SET_PCPS_TZDL ),                  \
  _mbg_cn_table_entry( IOCTL_GET_REF_OFFS ),                   \
  _mbg_cn_table_entry( IOCTL_SET_REF_OFFS ),                   \
  _mbg_cn_table_entry( IOCTL_GET_MBG_OPT_INFO ),               \
  _mbg_cn_table_entry( IOCTL_SET_MBG_OPT_SETTINGS ),           \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_IRIG_RX_INFO ),          \
  _mbg_cn_table_entry( IOCTL_SET_PCPS_IRIG_RX_SETTINGS ),      \
  _mbg_cn_table_entry( IOCTL_PCPS_CLR_UCAP_BUFF ),             \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_UCAP_ENTRIES ),          \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_UCAP_EVENT ),            \
  _mbg_cn_table_entry( IOCTL_GET_GPS_TZDL ),                   \
  _mbg_cn_table_entry( IOCTL_SET_GPS_TZDL ),                   \
  _mbg_cn_table_entry( IOCTL_GET_GPS_SW_REV ),                 \
  _mbg_cn_table_entry( IOCTL_GET_GPS_BVAR_STAT ),              \
  _mbg_cn_table_entry( IOCTL_GET_GPS_TIME ),                   \
  _mbg_cn_table_entry( IOCTL_SET_GPS_TIME ),                   \
  _mbg_cn_table_entry( IOCTL_GET_GPS_PORT_PARM ),              \
  _mbg_cn_table_entry( IOCTL_SET_GPS_PORT_PARM ),              \
  _mbg_cn_table_entry( IOCTL_GET_GPS_ANT_INFO ),               \
  _mbg_cn_table_entry( IOCTL_GET_GPS_UCAP ),                   \
  _mbg_cn_table_entry( IOCTL_GET_GPS_ENABLE_FLAGS ),           \
  _mbg_cn_table_entry( IOCTL_SET_GPS_ENABLE_FLAGS ),           \
  _mbg_cn_table_entry( IOCTL_GET_GPS_STAT_INFO ),              \
  _mbg_cn_table_entry( IOCTL_SET_GPS_CMD ),                    \
  _mbg_cn_table_entry( IOCTL_GET_GPS_IDENT ),                  \
  _mbg_cn_table_entry( IOCTL_GET_GPS_POS ),                    \
  _mbg_cn_table_entry( IOCTL_SET_GPS_POS_XYZ ),                \
  _mbg_cn_table_entry( IOCTL_SET_GPS_POS_LLA ),                \
  _mbg_cn_table_entry( IOCTL_GET_GPS_ANT_CABLE_LEN ),          \
  _mbg_cn_table_entry( IOCTL_SET_GPS_ANT_CABLE_LEN ),          \
  _mbg_cn_table_entry( IOCTL_GET_GPS_RECEIVER_INFO ),          \
  _mbg_cn_table_entry( IOCTL_GET_GPS_ALL_STR_TYPE_INFO ),      \
  _mbg_cn_table_entry( IOCTL_GET_GPS_ALL_PORT_INFO ),          \
  _mbg_cn_table_entry( IOCTL_SET_GPS_PORT_SETTINGS_IDX ),      \
  _mbg_cn_table_entry( IOCTL_GET_PCI_ASIC_VERSION ),           \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_TIME_CYCLES ),           \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_HR_TIME_CYCLES ),        \
  _mbg_cn_table_entry( IOCTL_GET_PCPS_IRIG_TX_INFO ),          \
  _mbg_cn_table_entry( IOCTL_SET_PCPS_IRIG_TX_SETTINGS ),      \
  _mbg_cn_table_entry( IOCTL_GET_SYNTH ),                      \
  _mbg_cn_table_entry( IOCTL_SET_SYNTH ),                      \
  _mbg_cn_table_entry( IOCTL_DEV_IS_GPS ),                     \
  _mbg_cn_table_entry( IOCTL_DEV_IS_DCF ),                     \
  _mbg_cn_table_entry( IOCTL_DEV_IS_IRIG_RX ),                 \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_HR_TIME ),                \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_CAB_LEN ),                \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_TZDL ),                   \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_PCPS_TZDL ),              \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_TZCODE ),                 \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_TZ ),                     \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_EVENT_TIME ),             \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_RECEIVER_INFO ),          \
  _mbg_cn_table_entry( IOCTL_DEV_CAN_CLR_UCAP_BUFF ),          \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_UCAP ),                   \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_IRIG_TX ),                \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_SERIAL_HS ),              \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_SIGNAL ),                 \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_MOD ),                    \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_IRIG ),                   \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_REF_OFFS ),               \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_OPT_FLAGS ),              \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_GPS_DATA ),               \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_SYNTH ),                  \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_GENERIC_IO ),             \
  _mbg_cn_table_entry( IOCTL_PCPS_GENERIC_IO ),                \
  _mbg_cn_table_entry( IOCTL_GET_SYNTH_STATE ),                \
  _mbg_cn_table_entry( IOCTL_GET_GPS_ALL_POUT_INFO ),          \
  _mbg_cn_table_entry( IOCTL_SET_GPS_POUT_SETTINGS_IDX ),      \
  _mbg_cn_table_entry( IOCTL_GET_MAPPED_MEM_ADDR ),            \
  _mbg_cn_table_entry( IOCTL_UNMAP_MAPPED_MEM ),               \
  _mbg_cn_table_entry( IOCTL_GET_PCI_ASIC_FEATURES ),          \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_PCI_ASIC_FEATURES ),      \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_PCI_ASIC_VERSION ),       \
  _mbg_cn_table_entry( IOCTL_DEV_IS_MSF ),                     \
  _mbg_cn_table_entry( IOCTL_DEV_IS_LWR ),                     \
  _mbg_cn_table_entry( IOCTL_DEV_IS_WWVB ),                    \
  _mbg_cn_table_entry( IOCTL_GET_IRQ_STAT_INFO ),              \
  _mbg_cn_table_entry( IOCTL_GET_CYCLES_FREQUENCY ),           \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_FAST_HR_TIMESTAMP ),      \
  _mbg_cn_table_entry( IOCTL_GET_FAST_HR_TIMESTAMP_CYCLES ),   \
  _mbg_cn_table_entry( IOCTL_GET_FAST_HR_TIMESTAMP ),          \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_GPS_TIME_SCALE ),         \
  _mbg_cn_table_entry( IOCTL_GET_GPS_TIME_SCALE_INFO ),        \
  _mbg_cn_table_entry( IOCTL_SET_GPS_TIME_SCALE_SETTINGS ),    \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_GPS_UTC_PARM ),           \
  _mbg_cn_table_entry( IOCTL_GET_GPS_UTC_PARM ),               \
  _mbg_cn_table_entry( IOCTL_SET_GPS_UTC_PARM ),               \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_IRIG_CTRL_BITS ),         \
  _mbg_cn_table_entry( IOCTL_GET_IRIG_CTRL_BITS ),             \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_LAN_INTF ),               \
  _mbg_cn_table_entry( IOCTL_GET_LAN_IF_INFO ),                \
  _mbg_cn_table_entry( IOCTL_GET_IP4_STATE ),                  \
  _mbg_cn_table_entry( IOCTL_GET_IP4_SETTINGS ),               \
  _mbg_cn_table_entry( IOCTL_SET_IP4_SETTINGS ),               \
  _mbg_cn_table_entry( IOCTL_DEV_IS_PTP ),                     \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_PTP ),                    \
  _mbg_cn_table_entry( IOCTL_GET_PTP_STATE ),                  \
  _mbg_cn_table_entry( IOCTL_GET_PTP_CFG_INFO ),               \
  _mbg_cn_table_entry( IOCTL_SET_PTP_CFG_SETTINGS ),           \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_IRIG_TIME ),              \
  _mbg_cn_table_entry( IOCTL_GET_IRIG_TIME ),                  \
  _mbg_cn_table_entry( IOCTL_GET_TIME_INFO_HRT ),              \
  _mbg_cn_table_entry( IOCTL_GET_TIME_INFO_TSTAMP ),           \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_RAW_IRIG_DATA ),          \
  _mbg_cn_table_entry( IOCTL_GET_RAW_IRIG_DATA ),              \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_PTP_UNICAST ),            \
  _mbg_cn_table_entry( IOCTL_PTP_UC_MASTER_CFG_LIMITS ),       \
  _mbg_cn_table_entry( IOCTL_GET_ALL_PTP_UC_MASTER_INFO ),     \
  _mbg_cn_table_entry( IOCTL_SET_PTP_UC_MASTER_SETTINGS_IDX ), \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_PZF ),                    \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_CORR_INFO ),              \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_TR_DISTANCE ),            \
  _mbg_cn_table_entry( IOCTL_GET_CORR_INFO ),                  \
  _mbg_cn_table_entry( IOCTL_GET_TR_DISTANCE ),                \
  _mbg_cn_table_entry( IOCTL_SET_TR_DISTANCE ),                \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_DEBUG_STATUS ),           \
  _mbg_cn_table_entry( IOCTL_GET_DEBUG_STATUS ),               \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_EVT_LOG ),                \
  _mbg_cn_table_entry( IOCTL_CLR_EVT_LOG ),                    \
  _mbg_cn_table_entry( IOCTL_GET_NUM_EVT_LOG_ENTRIES ),        \
  _mbg_cn_table_entry( IOCTL_GET_FIRST_EVT_LOG_ENTRY ),        \
  _mbg_cn_table_entry( IOCTL_GET_NEXT_EVT_LOG_ENTRY ),         \
  _mbg_cn_table_entry( IOCTL_DEV_IS_GNSS ),                    \
  _mbg_cn_table_entry( IOCTL_GET_GNSS_MODE_INFO ),             \
  _mbg_cn_table_entry( IOCTL_SET_GNSS_MODE_SETTINGS ),         \
  _mbg_cn_table_entry( IOCTL_GET_ALL_GNSS_SAT_INFO ),          \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_GPIO ),                   \
  _mbg_cn_table_entry( IOCTL_GET_GPIO_CFG_LIMITS ),            \
  _mbg_cn_table_entry( IOCTL_GET_ALL_GPIO_INFO ),              \
  _mbg_cn_table_entry( IOCTL_SET_GPIO_SETTINGS_IDX ),          \
  _mbg_cn_table_entry( IOCTL_DEV_HAS_XMR ),                    \
  _mbg_cn_table_entry( IOCTL_GET_XMR_INSTANCES ),              \
  _mbg_cn_table_entry( IOCTL_GET_ALL_XMR_INFO ),               \
  _mbg_cn_table_entry( IOCTL_SET_XMR_SETTINGS_IDX ),           \
  _mbg_cn_table_entry( IOCTL_GET_ALL_XMR_STATUS ),             \
  _mbg_cn_table_entry( IOCTL_GET_XMR_HOLDOVER_STATUS ),        \
  _mbg_cn_table_entry( IOCTL_GET_ALL_GPIO_STATUS ),            \
  _mbg_cn_table_entry( IOCTL_CHK_DEV_FEAT ),                   \
                                                               \
  _mbg_cn_table_entry( IOCTL_MBG_DBG_GET_PORT_ADDR ),          \
  _mbg_cn_table_entry( IOCTL_MBG_DBG_SET_PORT_ADDR ),          \
  _mbg_cn_table_entry( IOCTL_MBG_DBG_SET_BIT ),                \
  _mbg_cn_table_entry( IOCTL_MBG_DBG_CLR_BIT ),                \
  _mbg_cn_table_entry( IOCTL_MBG_DBG_CLR_ALL ),                \
                                                               \
  _mbg_cn_table_end()                                          \
}


#if !defined( _cmd_from_ioctl_code )
  #define _cmd_from_ioctl_code( _ioctl_code )   _ioctl_code
#endif



/**
 * @brief Privilege levels for IOCTL codes.
 *
 * IOCTLs can be used to do different things ranging from simply
 * reading a timestamp up to forcing a GPS receiver into boot mode
 * which may completely mess up the time keeping on the PC.
 *
 * These codes are used to determine a privilege level required
 * to execute a specific IOCTL command.
 *
 * How to determine if a calling process has sufficient privileges
 * depends strongly on the rights management features provided
 * by the underlying OS (e.g simple user/group rights, ACLs,
 * Linux capabilities, Windows privileges) so this needs to be
 * implemented in the OS-specific code of a driver.
 *
 * Implementation should be done in a way which introduces as low
 * latency as possible when reading time stamps from a device.
 */
enum MBG_REQ_PRIVL
{
  MBG_REQ_PRIVL_NONE,         //< e.g. read date/time/sync status
  MBG_REQ_PRIVL_EXT_STATUS,   //< e.g. read receiver position
  MBG_REQ_PRIVL_CFG_READ,     //< read device config data
  MBG_REQ_PRIVL_CFG_WRITE,    //< write config data to the device
  MBG_REQ_PRIVL_SYSTEM,       //< operations which may affect system operation
  N_MBG_REQ_PRIVL             //< the number of supported privilege levels
};


#if defined( __GNUC__ )
// Avoid "no previous prototype" with some gcc versions.
static __mbg_inline
int ioctl_get_required_privilege( ulong ioctl_code ) __attribute__((always_inline));
#endif

/**
 * @brief Determine the privilege level required to execute a specific IOCTL command.
 *
 * @param ioctl_code The IOCTL code for which to return the privilege level
 *
 * @return One of the enumerated privilege levels
 * @return -1 for unknown IOCTL codes
 */
static __mbg_inline
int ioctl_get_required_privilege( ulong ioctl_code )
{
  // To provide best maintainability the sequence of cases in ioctl_switch()
  // should match the sequence of the cases here, which also makes sure
  // commands requiring lowest latency are handled first.

  switch ( ioctl_code )
  {
    // Commands requiring lowest latency:
    case IOCTL_GET_FAST_HR_TIMESTAMP:
    case IOCTL_GET_PCPS_HR_TIME:
    case IOCTL_GET_FAST_HR_TIMESTAMP_CYCLES:
    case IOCTL_GET_PCPS_HR_TIME_CYCLES:
    case IOCTL_GET_PCPS_UCAP_EVENT:
    // Other low latency commands:
    case IOCTL_GET_PCPS_TIME:
    case IOCTL_GET_PCPS_TIME_CYCLES:
    case IOCTL_GET_PCPS_STATUS_PORT:
    case IOCTL_GET_PCPS_TIME_SEC_CHANGE:
    case IOCTL_GET_GPS_TIME:
    case IOCTL_GET_GPS_UCAP:
    case IOCTL_GET_TIME_INFO_HRT:
    case IOCTL_GET_TIME_INFO_TSTAMP:
      return MBG_REQ_PRIVL_NONE;

    // Commands returning public status information:
    case IOCTL_GET_PCPS_DRVR_INFO:
    case IOCTL_GET_PCPS_DEV:
    case IOCTL_GET_PCPS_SYNC_TIME:
    case IOCTL_GET_GPS_SW_REV:
    case IOCTL_GET_GPS_BVAR_STAT:
    case IOCTL_GET_GPS_ANT_INFO:
    case IOCTL_GET_GPS_STAT_INFO:
    case IOCTL_GET_GPS_IDENT:
    case IOCTL_GET_GPS_RECEIVER_INFO:
    case IOCTL_GET_PCI_ASIC_VERSION:
    case IOCTL_GET_SYNTH_STATE:
    case IOCTL_GET_PCPS_UCAP_ENTRIES:
    case IOCTL_GET_PCI_ASIC_FEATURES:
    case IOCTL_GET_IRQ_STAT_INFO:
    case IOCTL_GET_CYCLES_FREQUENCY:
    case IOCTL_GET_IRIG_CTRL_BITS:
    case IOCTL_GET_IP4_STATE:
    case IOCTL_GET_PTP_STATE:
    case IOCTL_GET_CORR_INFO:
    case IOCTL_GET_DEBUG_STATUS:
    case IOCTL_GET_NUM_EVT_LOG_ENTRIES:
    case IOCTL_GET_FIRST_EVT_LOG_ENTRY:
    case IOCTL_GET_NEXT_EVT_LOG_ENTRY:
  #if _MBG_SUPP_VAR_ACC_SIZE
    case IOCTL_GET_ALL_GNSS_SAT_INFO:
  #endif
      return MBG_REQ_PRIVL_NONE;

    // Commands returning device capabilities and features:
    case IOCTL_DEV_IS_GPS:
    case IOCTL_DEV_IS_DCF:
    case IOCTL_DEV_IS_MSF:
    case IOCTL_DEV_IS_WWVB:
    case IOCTL_DEV_IS_LWR:
    case IOCTL_DEV_IS_GNSS:
    case IOCTL_DEV_IS_IRIG_RX:
    case IOCTL_DEV_HAS_HR_TIME:
    case IOCTL_DEV_HAS_CAB_LEN:
    case IOCTL_DEV_HAS_TZDL:
    case IOCTL_DEV_HAS_PCPS_TZDL:
    case IOCTL_DEV_HAS_TZCODE:
    case IOCTL_DEV_HAS_TZ:
    case IOCTL_DEV_HAS_EVENT_TIME:
    case IOCTL_DEV_HAS_RECEIVER_INFO:
    case IOCTL_DEV_CAN_CLR_UCAP_BUFF:
    case IOCTL_DEV_HAS_UCAP:
    case IOCTL_DEV_HAS_IRIG_TX:
    case IOCTL_DEV_HAS_SERIAL_HS:
    case IOCTL_DEV_HAS_SIGNAL:
    case IOCTL_DEV_HAS_MOD:
    case IOCTL_DEV_HAS_IRIG:
    case IOCTL_DEV_HAS_REF_OFFS:
    case IOCTL_DEV_HAS_OPT_FLAGS:
    case IOCTL_DEV_HAS_GPS_DATA:
    case IOCTL_DEV_HAS_SYNTH:
    case IOCTL_DEV_HAS_GENERIC_IO:
    case IOCTL_DEV_HAS_PCI_ASIC_FEATURES:
    case IOCTL_DEV_HAS_PCI_ASIC_VERSION:
    case IOCTL_DEV_HAS_FAST_HR_TIMESTAMP:
    case IOCTL_DEV_HAS_GPS_TIME_SCALE:
    case IOCTL_DEV_HAS_GPS_UTC_PARM:
    case IOCTL_DEV_HAS_IRIG_CTRL_BITS:
    case IOCTL_DEV_HAS_LAN_INTF:
    case IOCTL_DEV_IS_PTP:
    case IOCTL_DEV_HAS_PTP:
    case IOCTL_DEV_HAS_IRIG_TIME:
    case IOCTL_DEV_HAS_RAW_IRIG_DATA:
    case IOCTL_DEV_HAS_PTP_UNICAST:
    case IOCTL_DEV_HAS_PZF:
    case IOCTL_DEV_HAS_CORR_INFO:
    case IOCTL_DEV_HAS_TR_DISTANCE:
    case IOCTL_DEV_HAS_DEBUG_STATUS:
    case IOCTL_DEV_HAS_EVT_LOG:
    case IOCTL_DEV_HAS_GPIO:
    case IOCTL_DEV_HAS_XMR:
    case IOCTL_CHK_DEV_FEAT:
      return MBG_REQ_PRIVL_NONE;

    // The next codes are somewhat special since they change something
    // on the board but do not affect basic operation:
    case IOCTL_PCPS_CLR_UCAP_BUFF:
    case IOCTL_SET_PCPS_EVENT_TIME:  // supported by some customized firmware only
    case IOCTL_CLR_EVT_LOG:
      return MBG_REQ_PRIVL_NONE;

    // Status information which may not be available for everybody:
    case IOCTL_GET_GPS_POS:
      return MBG_REQ_PRIVL_EXT_STATUS;

    // Reading device configuration:
    case IOCTL_GET_PCPS_SERIAL:
    case IOCTL_GET_PCPS_TZCODE:
    case IOCTL_GET_PCPS_TZDL:
    case IOCTL_GET_REF_OFFS:
    case IOCTL_GET_MBG_OPT_INFO:
    case IOCTL_GET_PCPS_IRIG_RX_INFO:
    case IOCTL_GET_GPS_TZDL:
    case IOCTL_GET_GPS_PORT_PARM:
    case IOCTL_GET_GPS_ENABLE_FLAGS:
    case IOCTL_GET_GPS_ANT_CABLE_LEN:
    case IOCTL_GET_PCPS_IRIG_TX_INFO:
    case IOCTL_GET_SYNTH:
    case IOCTL_GET_GPS_TIME_SCALE_INFO:
    case IOCTL_GET_GPS_UTC_PARM:
    case IOCTL_GET_LAN_IF_INFO:
    case IOCTL_GET_IP4_SETTINGS:
    case IOCTL_GET_PTP_CFG_INFO:
    case IOCTL_GET_IRIG_TIME:
    case IOCTL_GET_RAW_IRIG_DATA:
    case IOCTL_PTP_UC_MASTER_CFG_LIMITS:
    case IOCTL_GET_TR_DISTANCE:
    // generic read functions
    case IOCTL_PCPS_GENERIC_READ:
    case IOCTL_PCPS_GENERIC_READ_GPS:
  #if _MBG_SUPP_VAR_ACC_SIZE
    // These codes are only supported on target systems where a variable size of
    // the IOCTL buffer can be specified in the IOCTL call. On other systems the
    // generic IOCTL functions are used instead.
    case IOCTL_GET_GPS_ALL_STR_TYPE_INFO:
    case IOCTL_GET_GPS_ALL_PORT_INFO:
    case IOCTL_GET_GPS_ALL_POUT_INFO:
    case IOCTL_GET_ALL_PTP_UC_MASTER_INFO:
    case IOCTL_GET_ALL_GPIO_INFO:
    case IOCTL_GET_ALL_GPIO_STATUS:
    case IOCTL_GET_ALL_XMR_STATUS:
    case IOCTL_GET_ALL_XMR_INFO:
  #endif
    case IOCTL_GET_GNSS_MODE_INFO:
    case IOCTL_GET_GPIO_CFG_LIMITS:
    case IOCTL_GET_XMR_INSTANCES:
    case IOCTL_GET_XMR_HOLDOVER_STATUS:
      return MBG_REQ_PRIVL_CFG_READ;

    // Writing device configuration:
    case IOCTL_SET_PCPS_SERIAL:
    case IOCTL_SET_PCPS_TZCODE:
    case IOCTL_SET_PCPS_TZDL:
    case IOCTL_SET_REF_OFFS:
    case IOCTL_SET_MBG_OPT_SETTINGS:
    case IOCTL_SET_PCPS_IRIG_RX_SETTINGS:
    case IOCTL_SET_GPS_TZDL:
    case IOCTL_SET_GPS_PORT_PARM:
    case IOCTL_SET_GPS_ENABLE_FLAGS:
    case IOCTL_SET_GPS_ANT_CABLE_LEN:
    case IOCTL_SET_GPS_PORT_SETTINGS_IDX:
    case IOCTL_SET_PCPS_IRIG_TX_SETTINGS:
    case IOCTL_SET_SYNTH:
    case IOCTL_SET_GPS_POUT_SETTINGS_IDX:
    case IOCTL_SET_IP4_SETTINGS:
    case IOCTL_SET_PTP_CFG_SETTINGS:
    case IOCTL_SET_PTP_UC_MASTER_SETTINGS_IDX:
    case IOCTL_SET_TR_DISTANCE:
    case IOCTL_SET_GNSS_MODE_SETTINGS:
    case IOCTL_SET_GPIO_SETTINGS_IDX:
      return MBG_REQ_PRIVL_CFG_WRITE;

    // Operations which may severely affect system operation:
    case IOCTL_SET_PCPS_TIME:
    case IOCTL_SET_GPS_TIME:
    case IOCTL_SET_GPS_POS_XYZ:
    case IOCTL_SET_GPS_POS_LLA:
    case IOCTL_SET_GPS_TIME_SCALE_SETTINGS:
    case IOCTL_SET_GPS_UTC_PARM:
    case IOCTL_SET_GPS_CMD:
    case IOCTL_SET_XMR_SETTINGS_IDX:
    // generic write operations can do anything
    case IOCTL_PCPS_GENERIC_WRITE:
    case IOCTL_PCPS_GENERIC_WRITE_GPS:
    case IOCTL_PCPS_GENERIC_IO:
      return MBG_REQ_PRIVL_SYSTEM;

    // The next codes are somewhat special and normally
    // not used by the driver software:
    case IOCTL_GET_MAPPED_MEM_ADDR:
    case IOCTL_UNMAP_MAPPED_MEM:
      return MBG_REQ_PRIVL_SYSTEM;

  #if USE_DEBUG_PORT
    // The codes below are used for debugging only.
    // Unrestricted usage may cause system malfunction !!
    case IOCTL_MBG_DBG_GET_PORT_ADDR:
    case IOCTL_MBG_DBG_SET_PORT_ADDR:
    case IOCTL_MBG_DBG_SET_BIT:
    case IOCTL_MBG_DBG_CLR_BIT:
    case IOCTL_MBG_DBG_CLR_ALL:
      return MBG_REQ_PRIVL_SYSTEM;
  #endif

  }  // switch

  return -1;   // unsupported code, should always be denied

}  // ioctl_get_required_privilege



/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 int mbgioctl_rc_to_mbg_errno( int sys_errno ) ;
 const char *mbgioctl_get_name( long code ) ;

/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif


/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _MBGIOCTL_H */
