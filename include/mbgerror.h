
/**************************************************************************
 *
 *  $Id: mbgerror.h 1.29 2020/05/27 11:30:40 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Common error codes used with Meinberg API calls.
 *    OS-specific error codes can be translated into these codes.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgerror.h $
 *  Revision 1.29  2020/05/27 11:30:40  martin
 *  New NTP exit codes MBG_NTP_EXIT_FAIL_MEM and MBG_NTP_EXIT_FAIL_CRYPTO_API.
 *  Revision 1.28  2020/05/25 20:52:12  martin
 *  New NTP exit code MBG_NTP_EXIT_FAIL_TIMEOUT.
 *  Revision 1.27  2019/12/02 09:26:17  philipp
 *  Added MBG_ERR_NAME error code
 *  Revision 1.26  2019/11/11 10:10:10  martin
 *  Moved exit codes here that are used by NTP tools.
 *  Should be merged with other exit codes later.
 *  Revision 1.25  2019/10/09 09:31:04  philipp
 *  Added MBG_ERR_INV_USER and MBG_ERR_INV_GROUP
 *  Revision 1.24  2019/08/28 08:02:56  philipp
 *  Added error code MBG_ERR_IN_PROGRESS
 *  Revision 1.23  2019/08/26 15:14:34  martin
 *  Modified msg. text for MBG_ERR_NO_DATA.
 *  Revision 1.22  2019/08/20 08:20:04  martin
 *  Fixed build using mingw.
 *  Changed message text for MBG_ERR_ACCESS, which may also indicate that
 *  an exclusive resource is in use, not only that permissions are missing.
 *  Revision 1.21  2019/03/13 16:17:10  martin
 *  New code MBG_ERR_BAD_ADDRESS (for EFAULT).
 *  Revision 1.20  2019/03/13 09:44:30  martin
 *  Moved predefined program exit codes here.
 *  Revision 1.19  2019/02/08 10:42:56  martin
 *  Distinguish more detailed if an NTSTATUS replacement
 *  is required for a particular build environment.
 *  Revision 1.18  2018/11/22 11:25:56  martin
 *  New error code MBG_ERR_STR_SUBSTR merged from 1.15.1.x branch.
 *  Refactored surrogate definitions.
 *  Updated function prototypes.
 *  Revision 1.17  2018/09/21 09:49:57  martin
 *  Renamed MBG_ERR_FIFO to MBG_ERR_NO_DATA.
 *  Added strings for MBG_ERR_NO_DRIVER and MBG_ERR_DRV_VERSION.
 *  New code MBG_ERR_BUFFER_TOO_SMALL.
 *  MBG_ERR_OUTDATED error code added by philipp.
 *  Added some Windows stuff.
 *  Updated function prototypes.
 *  Revision 1.16  2018/06/25 14:26:38Z  martin
 *  Some new error codes were introduced.
 *  New macro mbg_rc_is_success_or_err_perm().
 *  New control macro _USE_WIN32_PRIVATE_STATUS_CODES.
 *  Split table initializers for error strings into two
 *  separate initializers to avoid overhead on target
 *  systems which don't support extended functions.
 *  Updated comments, string tables, and function prototypes.
 *  Revision 1.15  2017/07/05 09:27:25  martin
 *  New error code MBG_ERR_PARM_FMT.
 *  Changed message text for MBG_ERR_NO_ENTITY.
 *  Replaced old _mbg_err_to_os() macro by new inline functions
 *  mbg_errno_to_os() and  mbg_ret_val_to_os().
 *  Fixed build under Windows.
 *  Updated doxygen comments.
 *  Updated function prototypes.
 *  Revision 1.14  2017/05/10 15:21:39  martin
 *  Tiny cleanup.
 *  Revision 1.13  2017/02/28 15:23:14  gregoire
 *  error code MBG_ERR_INV_IDX added
 *  Revision 1.12  2017/01/10 15:54:56  philipp
 *  Fixed syntax error
 *  Revision 1.11  2017/01/10 14:26:31  philipp
 *  Added error MBG_ERR_NOT_CONFIGURED
 *  Revision 1.10  2016/12/16 12:40:33  thomas-b
 *  Added MBG_ERR_NO_SPACE
 *  Revision 1.9  2016/10/31 17:41:55  martin
 *  New error code MBG_ERR_DATA_FMT.
 *  Revision 1.8  2016/08/05 12:29:20  martin
 *  Re-enabled some symbols which have been commented out.
 *  Added new codes, and initializers for code/string conversion tables.
 *  Updated doxygen comments.
 *  Updated function prorotypes.
 *  Revision 1.7  2014/05/27 13:32:47Z  martin
 *  Defined additional common error codes which can be
 *  translated from OS specific codes.
 *  Function prototypes from new module mbgerror.c.
 *  Comments in doxygen style.
 *  Revision 1.6  2012/10/02 18:42:26Z  martin
 *  New codes MBG_ERR_N_POUT_EXCEEDS_SUPP and
 *  MBG_ERR_N_UC_MSTR_EXCEEDS_SUPP.
 *  Modified comments for doxygen.
 *  Revision 1.5  2011/03/31 10:56:17  martin
 *  Added MBG_ERR_COPY_TO_USER and MBG_ERR_COPY_FROM_USER.
 *  Revision 1.4  2008/12/05 13:28:50  martin
 *  Added new code MBG_ERR_IRQ_UNSAFE.
 *  Revision 1.3  2008/02/26 14:50:14Z  daniel
 *  Added codes:
 *  MBG_ERR_NOT_SUPP_ON_OS, MBG_ERR_LIB_NOT_COMPATIBLE,
 *  MBG_ERR_N_COM_EXCEEDS_SUPP, MBG_ERR_N_STR_EXCEEDS_SUPP
 *  Added doxygen compatible comments.
 *  Revision 1.2  2007/09/27 07:26:22Z  martin
 *  Define STATUS_SUCCESS for Windows if not in kernel mode.
 *  Revision 1.1  2007/09/26 08:08:54Z  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _MBGERROR_H
#define _MBGERROR_H


/* Other headers to be included */

#include <mbg_tgt.h>
#include <words.h>

// We may need surrogate declarations for target-specific
// types that are used in common function prototypes.
// Without such declarations we'd get compiler errors
// on targets that don't have them.

#if defined( MBG_TGT_WIN32 )

  #if defined( MBG_TGT_KERNEL )

    #define MBG_TGT_MISSING_DWORD       1  // missing even in kernel space.
    #define MBG_TGT_MISSING_NTSTATUS    0  // available in kernel space

  #else  // Windows user space

    #define MBG_TGT_MISSING_DWORD       0  // available in user space

    // Some (but not all) Windows build environments provide a
    // bcrypt.h file which has a definition for NTSTATUS, so we
    // use.that if available to avoid duplicate / mismatching
    // definitions in case an application uses bcrypt.h anyway.
    #if !defined( MBG_TGT_HAS_BCRYPT_H )  // unless already defined elsewhere
      #if defined( _MSC_VER ) && ( _MSC_VER >= 1500 )  // at least VS2008 has it
        #define MBG_TGT_HAS_BCRYPT_H    1
      #endif

      #if defined( MBG_TGT_MINGW )  // older versions may not have it, though
        #define MBG_TGT_HAS_BCRYPT_H    1
      #endif

      #if !defined( MBG_TGT_HAS_BCRYPT_H )
        #define MBG_TGT_HAS_BCRYPT_H    0
      #endif
    #endif

    #if MBG_TGT_HAS_BCRYPT_H
      #include <bcrypt.h>
      #define MBG_TGT_MISSING_NTSTATUS  0
    #else
      #define MBG_TGT_MISSING_NTSTATUS  1
    #endif

  #endif

#else  // non-Windows targets

  #define MBG_TGT_MISSING_DWORD         1
  #define MBG_TGT_MISSING_NTSTATUS      1

#endif


#ifdef _MBGERROR
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif

#if defined( MBG_TGT_WIN32 )

  #if defined( MBG_TGT_KERNEL )
    #define MBG_SYS_RC_SUCCESS  STATUS_SUCCESS
  #else
    #define MBG_SYS_RC_SUCCESS  ERROR_SUCCESS
  #endif

#elif defined( MBG_TGT_POSIX )

  #define MBG_SYS_RC_SUCCESS  0

#else

  #define MBG_SYS_RC_SUCCESS  0

#endif


#if !defined( _USE_WIN32_PRIVATE_STATUS_CODES )  // FIXME
  #define _USE_WIN32_PRIVATE_STATUS_CODES  0
#endif


#if defined( MBG_TGT_WIN32 )

#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_ERROR            0x3

#define STATUS_SEVERITY_SHIFT_BITS       30
#define STATUS_SEVERITY_SHIFT_MASK       0x03

#define STATUS_SEVERITY_SUCCESS_MASK        ( STATUS_SEVERITY_SUCCESS       << STATUS_SEVERITY_SHIFT_BITS )
#define STATUS_SEVERITY_INFORMATIONAL_MASK  ( STATUS_SEVERITY_INFORMATIONAL << STATUS_SEVERITY_SHIFT_BITS )
#define STATUS_SEVERITY_WARNING_MASK        ( STATUS_SEVERITY_WARNING       << STATUS_SEVERITY_SHIFT_BITS )
#define STATUS_SEVERITY_ERROR_MASK          ( STATUS_SEVERITY_ERROR         << STATUS_SEVERITY_SHIFT_BITS )

#define _get_win_msg_severity( _st )  ( ( (_st) >> STATUS_SEVERITY_SHIFT_BITS ) & STATUS_SEVERITY_SHIFT_MASK )



#define STATUS_CUSTOM_FLAG               0x1

#define STATUS_CUSTOM_FLAG_SHIFT_BITS    29
#define STATUS_CUSTOM_FLAG_SHIFT_MASK    0x01

#define STATUS_CUSTOM_FLAG_MASK          ( STATUS_CUSTOM_FLAG << STATUS_CUSTOM_FLAG_SHIFT_BITS )

#define _get_win_msg_custom_flag( _st )  ( ( (_st) >> STATUS_CUSTOM_FLAG_SHIFT_BITS ) & STATUS_CUSTOM_FLAG_SHIFT_MASK )

#define _win_msg_is_custom( _st )        ( _get_win_msg_custom_flag( _st ) != 0 )


#define _mbg_msg_id_inf( num )    ((DWORD) ( STATUS_SEVERITY_INFORMATIONAL_MASK | STATUS_CUSTOM_FLAG_MASK | num ) )
#define _mbg_msg_id_wrn( num )    ((DWORD) ( STATUS_SEVERITY_WARNING_MASK       | STATUS_CUSTOM_FLAG_MASK | num ) )
#define _mbg_msg_id_err( num )    ((DWORD) ( STATUS_SEVERITY_ERROR_MASK         | STATUS_CUSTOM_FLAG_MASK | num ) )

#endif  // defined( MBG_TGT_WIN32 )



#if MBG_TGT_MISSING_DWORD
  typedef uint32_t DWORD;
  #define DWORD  DWORD
#endif

#if MBG_TGT_MISSING_NTSTATUS
  // We intentionally define an uncommon type to
  // enforce build errors in case NTSTATUS is really
  // used on targets that should not use it.
  typedef int *NTSTATUS;
  #define NTSTATUS  NTSTATUS
#endif



/**
 * @brief Error codes used with Meinberg devices and drivers
 *
 * Appropriate error strings can be retrieved via the ::mbg_strerror function.
 *
 * @see ::MBG_ERR_STR_TABLE_ENG
 *
 * @anchor MBG_RETURN_CODES @{ */

/* ### TODO FIXME
 * Under Windows, some message strings are provided as resources appended
 * to the mbgctrl DLL, but the codes specified here have to be translated
 * to Windows-specific message IDs before the appropriate resource string
 * can be retrieved. Actually this is done by taking the absolute number
 * of an error code and have it or'ed with 0xE0000000 afterwards, e.g.
 * ::MBG_ERR_GENERIC (-19) will yield Windows code 0xE0000013.
 * * See ::_mbg_err_to_os
 */

// NOTE: Some of these codes have to match codes which are defined in pcpsdefs.h
// and returned by the firmware of bus-level devices, so the definitions
// must *not* be renumbered.

#define MBG_SUCCESS           0  ///< No error, has to match ::PCPS_SUCCESS

 /** @anchor MBG_ERROR_CODES @{ */

// Other codes which have to match codes defined in pcpsdefs.h returned by bus-level devices
#define MBG_ERR_STIME        -1  ///< Tried to write invalid date/time/status to device, has to match ::PCPS_ERR_STIME
#define MBG_ERR_CFG          -2  ///< Tried to write invalid configuration parameters to device, has to match ::PCPS_ERR_CFG (see also ::MBG_ERR_INV_CFG)


// Codes returned by low level functions of the bus-level device driver
#define MBG_ERR_GENERIC     -19  ///< Generic error.
#define MBG_ERR_TIMEOUT     -20  ///< Timeout accessing the device.
#define MBG_ERR_FW_ID       -21  ///< Invalid firmware ID.
#define MBG_ERR_NBYTES      -22  ///< The number of parameter bytes passed to the device did not
                                 ///< match the number of bytes expected by the device.

#define MBG_ERR_INV_TIME    -23  ///< The device has no valid time.
#define MBG_ERR_NO_DATA     -24  ///< No (more) data to process, e.g. FIFO empty.
#define MBG_ERR_NOT_READY   -25  ///< Bus-level device is temp. unable to respond e.g. during init. after RESET.
#define MBG_ERR_INV_TYPE    -26  ///< Bus-level device didn't recognize data type.


// Codes returned by the high level API functions
#define MBG_ERR_NO_MEM              -27  ///< Failed to allocate memory.
#define MBG_ERR_CLAIM_RSRC          -28  ///< Failed to claim port or mem resource.
#define MBG_ERR_DEV_NOT_SUPP        -29  ///< Device type not supported by driver.
#define MBG_ERR_INV_DEV_REQUEST     -30  ///< IOCTL call not supported by driver.
#define MBG_ERR_NOT_SUPP_BY_DEV     -31  ///< Command or feature not supported by device.
// #define MBG_ERR_USB_ACCESS       -32  ///< USB access failed (FIXME TODO this is B.S., we should return *why*)
#define MBG_ERR_CYCLIC_TIMEOUT      -33  ///< Cyclic event (IRQ, etc.) didn't occur in time.
#define MBG_ERR_NOT_SUPP_ON_OS      -34  ///< Function is not supported on this operating system.
#define MBG_ERR_LIB_NOT_COMPATIBLE  -35  ///< Installed shared library version not compatible with version used at build time.
#define MBG_ERR_N_COM_EXCEEDS_SUPP  -36  ///< Num. COM ports of the device exceeds max. supp. by driver.
#define MBG_ERR_N_STR_EXCEEDS_SUPP  -37  ///< Num. string formats of the device exceeds max. supp. by driver.
#define MBG_ERR_IRQ_UNSAFE          -38  ///< Enabled IRQ of bus-level device is unsafe with this firmware/ASIC version.
#define MBG_ERR_N_POUT_EXCEEDS_SUPP -39  ///< Num. prog. outputs of the device exceeds max. supp. by driver.

// Legacy codes used with DOS TSRs only:
#define MBG_ERR_INV_INTNO           -40  ///< Invalid interrupt number.
#define MBG_ERR_NO_DRIVER           -41  ///< No driver could be found.
#define MBG_ERR_DRV_VERSION         -42  ///< The driver is too old.


#define MBG_ERR_COPY_TO_USER        -43  ///< Kernel driver failed to copy data from kernel to user space.
#define MBG_ERR_COPY_FROM_USER      -44  ///< Kernel driver failed to copy data from use to kernel space.


// More codes returned by the driver's high level functions:
#define MBG_ERR_N_UC_MSTR_EXCEEDS_SUPP -45  ///< Num. PTP unicast masters of the device exceeds max. supp. by driver.
#define MBG_ERR_N_GNSS_EXCEEDS_SUPP    -46  ///< Num. of GNSS systems supp. by device exceeds max. supp. by driver.
#define MBG_ERR_N_GPIO_EXCEEDS_SUPP    -47  ///< Num. of GPIO ports supp. by device exceeds max. supp. by driver.
#define MBG_ERR_N_XMR_EXCEEDS_SUPP     -48  ///< Num. of XMR sources supp. by device exceeds max. supp. by driver.

#define MBG_ERR_UNSPEC              -60  ///< Unspecified error.

#define MBG_ERR_HDR_CSUM            -61  ///< Binary protocol header checksum error.
#define MBG_ERR_DATA_CSUM           -62  ///< Binary protocol data checksum error.
#define MBG_ERR_RCVD_NACK           -63  ///< Binary protocol received reply msg with a NACK code.
#define MBG_ERR_RCVD_NO_ACK         -64  ///< Binary protocol received reply msg without expected ACK code. //### TODO
#define MBG_ERR_CONN_TYPE           -65  ///< Binary protocol no valid/supported connection type specified.
#define MBG_ERR_BYTES_WRITTEN       -66  ///< Binary protocol failed to write all bytes.
#define MBG_ERR_AUTH                -67  ///< Binary protocol failed authentication.

#define MBG_ERR_SOCK_INIT           -68  ///< Socket interface not initialized, or failed to initialize.
#define MBG_ERR_INV_SOCK_FD         -69  ///< Invalid socket when tried to open network socket.
#define MBG_ERR_NOT_A_SOCKET        -70  ///< Socket descriptor is not a socket.
#define MBG_ERR_NBLOCK_WAIT_SLCT    -71  ///< Select timed out when waiting for non-blocking network port to become ready.
#define MBG_ERR_NBLOCK_WAIT_WR_FD   -72  ///< Write fd not set after select when waiting for non-blocking network port to become ready.

#define MBG_ERR_IO                  -73  ///< Input/output error.
#define MBG_ERR_INV_PARM            -74  ///< Invalid parameter.
#define MBG_ERR_NO_DEV              -75  ///< No such device, or attempted an inappropriate function.
#define MBG_ERR_NOT_FOUND           -76  ///< Specified item not found.

#define MBG_ERR_OVERFLOW            -77  ///< range or buffer overflow
#define MBG_ERR_PIPE                -78  ///< pipe error
#define MBG_ERR_INTR                -79  ///< interrupted function call
#define MBG_ERR_ACCESS              -80  ///< Access denied, e.g. to an object that is already in use, or in case of insufficient permissions.
#define MBG_ERR_PERM                -81  ///< Operation not permitted, e.g. when trying to set the system time without sufficient permissions.
#define MBG_ERR_BUSY                -82  ///< Device or resource busy, can't be used
#define MBG_ERR_INV_HANDLE          -83  ///< invalid file/device handle specified

#define MBG_ERR_XBP_CASC_LVL        -84  ///< too many XBP cascading levels
#define MBG_ERR_ENCRYPT             -85  ///< encryption failed
#define MBG_ERR_DECRYPT             -86  ///< decryption failed

#define MBG_ERR_DISCONN             -87  ///< connection closed by remote site / host
#define MBG_ERR_INV_CFG             -88  ///< invalid/inconsistent configuration parameters read from device, see also ::MBG_ERR_CFG
#define MBG_ERR_RANGE               -89  ///< input parameter was out of range

#define MBG_ERR_INV_TLV_ANN_BYTES   -90  ///< number of announced TLV bytes doesn't match number of transferred bytes
#define MBG_ERR_INV_TLV_SIZE        -91  ///< ### TODO
#define MBG_ERR_INV_TLV_UID         -92  ///< ### TODO

#define MBG_ERR_EXIST               -93  ///< File exists
#define MBG_ERR_DATA_SIZE           -94  ///< the received data size toesn't match the expected data size
#define MBG_ERR_NO_ENTITY           -95  ///< no such file or directory
#define MBG_ERR_ALREADY_ALLOC       -96  ///< pointer already allocated when trying to allocate memory
#define MBG_ERR_HOST_NOT_FOUND      -97  ///< host not found
#define MBG_ERR_CONN_RESET          -98  ///< connection reset by peer
#define MBG_ERR_DATA_FMT            -99  ///< invalid data format

#define MBG_ERR_NO_SPACE            -100 ///< insufficient disk space left on the device
#define MBG_ERR_NOT_CONFIGURED      -101 ///< configuration option is not active/configured
#define MBG_ERR_INV_IDX             -102 ///< invalid index value used

#define MBG_ERR_PARM_FMT            -103 ///< parameter string format error
#define MBG_ERR_UNKNOWN             -104 ///< Unknown error code from external API.

#define MBG_ERR_PAM                 -105 ///< PAM error while authenticating user
#define MBG_ERR_TIMER               -106 ///< Timer expired (e.g. stream timeout on USB disconnect)

#define MBG_ERR_AGAIN               -107 ///< Try again (later). For example, a blocking operation
                                         ///< on a non-blocking socket.

#define MBG_ERR_STR_CHAR            -108 ///< Invalid character in string
#define MBG_ERR_STR_LEN             -109 ///< Wrong string length
#define MBG_ERR_SN_GCODE_LEN        -110 ///< Invalid device group code length
#define MBG_ERR_SN_GCODE_UNKN       -111 ///< Unknown device group code
#define MBG_ERR_SN_GCODE_WRONG      -112 ///< Wrong device group code in S/N
#define MBG_ERR_SN_LEN              -113 ///< Wrong serial number string length
#define MBG_ERR_SN_VRFY             -114 ///< Serial number could not be verified

#define MBG_ERR_RSRC_ITEM           -115 ///< Too many resource items
#define MBG_ERR_BUFFER_TOO_SMALL    -116 ///< Buffer is too small.

#define MBG_ERR_OUTDATED            -117

#define MBG_ERR_STR_SUBSTR          -118 ///< Invalid substring in string
#define MBG_ERR_BAD_ADDRESS         -119 ///< Bad Address (like POSIX EFAULT)

#define MBG_ERR_IN_PROGRESS         -120 ///< Long lasting operation in progress
#define MBG_ERR_INV_USER            -121 ///< Invalid user
#define MBG_ERR_INV_GROUP           -122 ///< Invalid group
#define MBG_ERR_NAME                -123 ///< Invalid name

// NOTE: New codes have to be appended to this list, and the sequence of codes must not
// be changed. Whenever new codes have been defined, appropriate entries have to be added
// to the ::MBG_ERR_STR_TABLE_ENG table initializer below, and the Windows-specific message
// texts specified in messages.mc/.h from which the resources appended to mbgsvctl.dll
// are generated have to be updated accordingly.

/** @} anchor MBG_ERROR_CODES */

/** @} anchor MBG_RETURN_CODES */



/**
 * @brief A basic subset of strings associated with @ref MBG_RETURN_CODES
 *
 * This initializer provides only the strings for basic error codes
 * that are supported on all target systems.
 *
 * See the @ref MBG_ERR_STR_TABLE_EXT_ENG initializer for error strings
 * associated with errors from network and other extended functions.
 *
 * The generic @ref MBG_ERR_STR_TABLE_ENG initializer should be used
 * preferebly since it provides only the basic subset or the full
 * set of strings depending on the target system.
 *
 * @see @ref MBG_ERR_STR_TABLE_ENG
 * @see @ref MBG_ERR_STR_TABLE_EXT_ENG
 * @see @ref MBG_RETURN_CODES
 */
#define MBG_ERR_STR_TABLE_BASE_ENG                                                                      \
  { MBG_SUCCESS,                     "Success" },                                                       \
  { MBG_ERR_STIME,                   "Invalid date/time for device" },                                  \
  { MBG_ERR_CFG,                     "Invalid configuration parameters for device" },                   \
  { MBG_ERR_GENERIC,                 "Generic error" },                                                 \
  { MBG_ERR_TIMEOUT,                 "Timeout" },                                                       \
  { MBG_ERR_FW_ID,                   "Invalid firmware ID" },                                           \
  { MBG_ERR_NBYTES,                  "Unexpected number of data bytes for this API" },                  \
  { MBG_ERR_INV_TIME,                "The device has no valid time" },                                  \
  { MBG_ERR_NO_DATA,                 "No (more) data to process" },                                     \
  { MBG_ERR_NOT_READY,               "Device not ready" },                                              \
  { MBG_ERR_INV_TYPE,                "Unsupported data type" },                                         \
  { MBG_ERR_NO_MEM,                  "Memory allocation error" },                                       \
  { MBG_ERR_CLAIM_RSRC,              "Faild to claim resources" },                                      \
  { MBG_ERR_DEV_NOT_SUPP,            "Device not supported" },                                          \
  { MBG_ERR_INV_DEV_REQUEST,         "Request not supported" },                                         \
  { MBG_ERR_NOT_SUPP_BY_DEV,         "Not supported by device" },                                       \
  { MBG_ERR_CYCLIC_TIMEOUT,          "Cyclic message timeout" },                                        \
  { MBG_ERR_NOT_SUPP_ON_OS,          "Not supported by OS" },                                           \
  { MBG_ERR_LIB_NOT_COMPATIBLE,      "Shared lib not compatible" },                                     \
  { MBG_ERR_N_COM_EXCEEDS_SUPP,      "Num. COM ports exceeds supported" },                              \
  { MBG_ERR_N_STR_EXCEEDS_SUPP,      "Num. string formats exceeds supported" },                         \
  { MBG_ERR_IRQ_UNSAFE,              "Unsafe IRQ support" },                                            \
  { MBG_ERR_N_POUT_EXCEEDS_SUPP,     "Num prog. outputs exceeds supported" },                           \
  { MBG_ERR_INV_INTNO,               "Invalid interrupt number" },                                      \
  { MBG_ERR_NO_DRIVER,               "Driver not found" },                                              \
  { MBG_ERR_DRV_VERSION,             "Driver too old" },                                                \
  { MBG_ERR_N_UC_MSTR_EXCEEDS_SUPP,  "Num. PTP Unicast masters exceeds supported" },                    \
  { MBG_ERR_N_GNSS_EXCEEDS_SUPP,     "Num. GNSS systems exceeds supported" },                           \
  { MBG_ERR_N_GPIO_EXCEEDS_SUPP,     "Num. GPIO ports exceeds supported" },                             \
  { MBG_ERR_N_XMR_EXCEEDS_SUPP,      "Num. XMR sources exceeds supported" },                            \
  { MBG_ERR_UNSPEC,                  "Unspecified error" },                                             \
  { MBG_ERR_HDR_CSUM,                "Header checksum error" },                                         \
  { MBG_ERR_DATA_CSUM,               "Data checksum error" },                                           \
  { MBG_ERR_RCVD_NACK,               "Received NACK message" },                                         \
  { MBG_ERR_RCVD_NO_ACK,             "Didn't receive ACK message" },                                    \
  { MBG_ERR_CONN_TYPE,               "Invalid I/O connection type" },                                   \
  { MBG_ERR_BYTES_WRITTEN,           "Failed to write all bytes" },                                     \
  { MBG_ERR_IO,                      "Input/output error" },                                            \
  { MBG_ERR_INV_PARM,                "Invalid parameter passed to function" },                          \
  { MBG_ERR_NO_DEV,                  "No such device, or attempted an inappropriate function." },       \
  { MBG_ERR_NOT_FOUND,               "Specified item not found" },                                      \
  { MBG_ERR_OVERFLOW,                "Buffer overflow" },                                               \
  { MBG_ERR_BUSY,                    "Device busy" },                                                   \
  { MBG_ERR_INV_HANDLE,              "Invalid handle" },                                                \
  { MBG_ERR_XBP_CASC_LVL,            "Too many XBP cascading levels" },                                 \
  { MBG_ERR_ENCRYPT,                 "Encryption failed" },                                             \
  { MBG_ERR_DECRYPT,                 "Decryption failed" },                                             \
  { MBG_ERR_DISCONN,                 "Connection closed by remote site/host" },                         \
  { MBG_ERR_INV_CFG,                 "Invalid/inconsistent configuration read from device" },           \
  { MBG_ERR_RANGE,                   "Input parameter was out of range" },                              \
  { MBG_ERR_INV_TLV_ANN_BYTES,       "TLV num of transferred bytes differs from num of announced bytes" }, \
  { MBG_ERR_INV_TLV_SIZE,            "MBG_ERR_INV_TLV_SIZE" },  /* ### TODO */                          \
  { MBG_ERR_INV_TLV_UID,             "MBG_ERR_INV_TLV_UID" },   /* ### TODO */                          \
  { MBG_ERR_DATA_SIZE,               "Received data size mismatch" },                                   \
  { MBG_ERR_ALREADY_ALLOC,           "Memory already allocated" },                                      \
  { MBG_ERR_DATA_FMT,                "Invalid data format" },                                           \
  { MBG_ERR_NOT_CONFIGURED,          "Configuration is not active and/or configured" },                 \
  { MBG_ERR_INV_IDX,                 "Invalid or unsupported index value used"},                        \
  { MBG_ERR_PARM_FMT,                "Parameter string format error" },                                 \
  { MBG_ERR_UNKNOWN,                 "Unknown error code from external API" },                          \
  { MBG_ERR_PAM,                     "PAM authentication error" },                                      \
  { MBG_ERR_TIMER,                   "Timer expired" },                                                 \
  { MBG_ERR_AGAIN,                   "Try again (later)" },                                             \
  { MBG_ERR_STR_CHAR,                "Invalid character in string" },                                   \
  { MBG_ERR_STR_LEN,                 "Wrong string length" },                                           \
  { MBG_ERR_SN_GCODE_LEN,            "Invalid device group code length" },                              \
  { MBG_ERR_SN_GCODE_UNKN,           "Unknown device group code" },                                     \
  { MBG_ERR_SN_GCODE_WRONG,          "Wrong device group code in S/N" },                                \
  { MBG_ERR_SN_VRFY,                 "Serial number could not be verified" },                           \
  { MBG_ERR_RSRC_ITEM,               "Too many resource items" },                                       \
  { MBG_ERR_BUFFER_TOO_SMALL,        "Data buffer too small" },                                         \
  { MBG_ERR_OUTDATED,                "Software/Module is too old/outdated. Please update!" },           \
  { MBG_ERR_STR_SUBSTR,              "Invalid substring in string" },                                   \
  { MBG_ERR_IN_PROGRESS,             "Long lasting operation in progress" },                            \
  { MBG_ERR_INV_USER,                "Invalid user" },                                                  \
  { MBG_ERR_INV_GROUP,               "Invalid group" },                                                 \
  { MBG_ERR_NAME,                    "Invalid name" }


/**
 * @brief A subset of extended strings associated with @ref MBG_RETURN_CODES
 *
 * This initializer provides only the strings associated with extended
 * errors from network and other extended functions.
 *
 * See the @ref MBG_ERR_STR_TABLE_BASE_ENG initializer for error strings
 * associated with basic error codes that are supported on all target systems.
 *
 * The generic @ref MBG_ERR_STR_TABLE_ENG initializer should be used
 * preferebly since it provides only the basic subset or the full
 * set of strings depending on the target system.
 *
 * @see @ref MBG_ERR_STR_TABLE_ENG
 * @see @ref MBG_ERR_STR_TABLE_BASE_ENG
 * @see @ref MBG_RETURN_CODES
 */
#if defined( MBG_TGT_DOS )

#define MBG_ERR_STR_TABLE_EXT_ENG  \
  { 0, NULL }

#else

#define MBG_ERR_STR_TABLE_EXT_ENG                                                                       \
  { MBG_ERR_COPY_TO_USER,            "Error copying to user space" },                                   \
  { MBG_ERR_COPY_FROM_USER,          "Error copying from user space" },                                 \
  { MBG_ERR_AUTH,                    "Authentication failed" },                                         \
  { MBG_ERR_SOCK_INIT,               "Failed to initialize socket" },                                   \
  { MBG_ERR_INV_SOCK_FD,             "Invalid socket descriptor" },                                     \
  { MBG_ERR_NOT_A_SOCKET,            "Not a socket descriptor" },                                       \
  { MBG_ERR_NBLOCK_WAIT_SLCT,        "Select timed out waiting for port ready" },                       \
  { MBG_ERR_NBLOCK_WAIT_WR_FD,       "Write file descriptor not ready after waiting for port ready" },  \
  { MBG_ERR_PIPE,                    "Pipe error" },                                                    \
  { MBG_ERR_INTR,                    "Interrupted function call" },                                     \
  { MBG_ERR_ACCESS,                  "Access denied" },                                                 \
  { MBG_ERR_PERM,                    "Operation not permitted, insufficient rights" },                  \
  { MBG_ERR_EXIST,                   "File exists" },                                                   \
  { MBG_ERR_NO_ENTITY,               "No such file or directory" },                                     \
  { MBG_ERR_HOST_NOT_FOUND,          "Host not found" },                                                \
  { MBG_ERR_CONN_RESET,              "Connection reset by peer" },                                      \
  { MBG_ERR_NO_SPACE,                "Insufficient disk space" },                                       \
  { MBG_ERR_PAM,                     "PAM authentication was not successful" },                         \
  { MBG_ERR_TIMER,                   "Timer expired" },                                                 \
  { MBG_ERR_BAD_ADDRESS,             "Bad Address" }

#endif



/**
 * @brief Strings associated with @ref MBG_RETURN_CODES
 *
 * @see @ref MBG_RETURN_CODES
 */
#define MBG_ERR_STR_TABLE_ENG     \
{                                 \
  MBG_ERR_STR_TABLE_BASE_ENG,     \
  MBG_ERR_STR_TABLE_EXT_ENG,      \
  { 0, NULL }  /* end of table */ \
}



#if defined( __mbg_inline )

static __mbg_inline
/**
 * @brief Check if the code returned by a function indicates an error
 *
 * @param[in] rc  One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbg_rc_is_success
 * @see @ref MBG_RETURN_CODES
 */
bool mbg_rc_is_error( int rc )
{
  // MBG_SUCCESS is 0, and all Meinberg error codes are < 0.
  return rc < MBG_SUCCESS;

}  // mbg_rc_is_error



static __mbg_inline
/**
 * @brief Check if the code returned by a function indicates success
 *
 * @param[in] rc  One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbg_rc_is_error
 * @see @ref MBG_RETURN_CODES
 */
bool mbg_rc_is_success( int rc )
{
  // There are functions which don't only return MBG_SUCCESS
  // on success but some arbitrary positive number, e.g. the
  // number of bytes sent. So success just means "not an error".
  return !mbg_rc_is_error( rc );

}  // mbg_rc_is_success



static __mbg_inline
/**
 * @brief Check if a return code indicates success, or the specific code @ref MBG_ERR_PERM
 *
 * @param[in] rc  One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbg_rc_is_success
 * @see @ref MBG_RETURN_CODES
 */
bool mbg_rc_is_success_or_err_perm( int rc )
{
  return mbg_rc_is_success( rc ) || ( rc == MBG_ERR_PERM );

}  // mbg_rc_is_success_or_err_perm


#else

  #define mbg_rc_is_error( _rc )                ( (_rc) < MBG_SUCCESS )
  #define mbg_rc_is_success( _rc )              ( !mbg_rc_is_error( _rc ) )

  #define mbg_rc_is_success_or_err_perm( _rc )  ( mbg_rc_is_success( _rc ) || ( (_rc) == MBG_ERR_PERM ) )

#endif



/**
 * @brief Predefined exit codes returned by some tools.
 *
 * Should be merged with ::MBG_NTP_EXIT_CODES.
 *
 * @see ::MBG_NTP_EXIT_CODES
 * @see ::MBG_NTP_EXIT_CODE_STRS
 */
enum MBG_EXIT_CODES
{
  MBG_EXIT_CODE_SUCCESS,   ///< Requested action completed successfully.
  MBG_EXIT_CODE_USAGE,     ///< Unable to handle requested action, usage printed.
  MBG_EXIT_CODE_NOT_SUPP,  ///< Requested action not supported on the running OS.
  MBG_EXIT_CODE_FAIL,      ///< Action failed for specified device.
  MBG_EXIT_CODE_INV_TIME,  ///< Device has no valid time to set the system time with.
  N_MBG_EXIT_CODES
};



/**
 * @brief Predefined exit codes returned by some NTP tools.
 *
 * Should be merged with ::MBG_EXIT_CODES.
 *
 * @see ::MBG_NTP_EXIT_CODE_STRS
 * @see ::MBG_EXIT_CODES
 */
enum MBG_NTP_EXIT_CODES
{
  MBG_NTP_EXIT_SUCCESS,          // Success.
  MBG_NTP_EXIT_FAIL_USAGE,       // Usage printed, invalid parameter?
  MBG_NTP_EXIT_FAIL_GENERIC,     // General failure, unspecified.
  MBG_NTP_EXIT_FAIL_DNS,         // Host name lookup failed.
  MBG_NTP_EXIT_FAIL_SOCKET,      // Failed to open socket.
  MBG_NTP_EXIT_FAIL_SEND,        // Failed to send packet.
  MBG_NTP_EXIT_FAIL_RECEIVE,     // Failed to receive packet.
  MBG_NTP_EXIT_FAIL_ENCRYPT,     // Failed to encrypt a packet.
  MBG_NTP_EXIT_FAIL_DECRYPT,     // Failed to decrypt an encrypted packet.
  MBG_NTP_EXIT_FAIL_CRYPT_NACK,  // Failed because received a crypto NACK.
  MBG_NTP_EXIT_FAIL_BIND,        // Failed to bind to specific address or port.
  MBG_NTP_EXIT_FAIL_TIMEOUT,     // Timeout that shouldn't occur unless running at full speed.
  MBG_NTP_EXIT_FAIL_MEM,         // Out of memory.
  MBG_NTP_EXIT_FAIL_CRYPTO_API,  // Some crypto API call failed for unspecified reason.
  N_MBG_NTP_EXIT_CODES
};


/**
 * @brief Info strings associated with predefined NTP tool exit codes.
 *
 * Should be merged with ::MBG_EXIT_CODES.
 *
 * @see ::MBG_NTP_EXIT_CODES
 */

#define MBG_NTP_EXIT_CODE_STRS                                                       \
{                                                                                    \
  "Success",                                      /* MBG_NTP_EXIT_OK              */ \
  "Usage printed, e.g. invalid parameter.",       /* MBG_NTP_EXIT_FAIL_USAGE      */ \
  "Unspecified error.",                           /* MBG_NTP_EXIT_FAIL_GENERIC    */ \
  "Host name lookup failed.",                     /* MBG_NTP_EXIT_FAIL_DNS        */ \
  "Failed to open socket.",                       /* MBG_NTP_EXIT_FAIL_SOCKET     */ \
  "Failed to send packet.",                       /* MBG_NTP_EXIT_FAIL_SEND       */ \
  "Failed to receive packet.",                    /* MBG_NTP_EXIT_FAIL_RECEIVE    */ \
  "Failed to encrypt a packet.",                  /* MBG_NTP_EXIT_FAIL_ENCRYPT    */ \
  "Failed to decrypt an encrypted packet.",       /* MBG_NTP_EXIT_FAIL_DECRYPT    */ \
  "Received a crypto NACK.",                      /* MBG_NTP_EXIT_FAIL_CRYPT_NACK */ \
  "Failed to bind to specific address or port.",  /* MBG_NTP_EXIT_FAIL_BIND       */ \
  "Timeout.",                                     /* MBG_NTP_EXIT_FAIL_TIMEOUT    */ \
  "Unspecified crypto API error."                 /* MBG_NTP_EXIT_FAIL_CRYPTO_API */ \
}



/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
 * @brief Convert one of the @ref MBG_ERROR_CODES to an OS-specific format
 *
 * @param[in]  err_no  One of the @ref MBG_ERROR_CODES.
 *
 * @see @ref MBG_ERROR_CODES
 *
 * @return An OS-specific error code
 */
 int mbg_errno_to_os( int err_no ) ;

 /**
 * @brief Return an error string associated with the @ref MBG_ERROR_CODES
 *
 * @param[in] mbg_errno  One of the @ref MBG_ERROR_CODES
 *
 * @return  A constant string describing the error, or NULL for unknown error codes
 */
 const char *mbg_strerror( int mbg_errno ) ;

 /**
 * @brief Check if a value is an error code and print an associated error message
 *
 * @param[in] rc    A positive number including ::MBG_SUCCESS, or one of the @ref MBG_ERROR_CODES
 * @param[in] what  A string indicated what failed
 *
 * @return  true if rc represented an error code, and a message has been printed, else false
 */
 bool mbg_cond_err_msg( int rc, const char *what ) ;

 /**
 * @brief Check if a value is an general or a "not supported" error code and print an associated message
 *
 * If rc contains an error code then an error message is printed, and true is returned.
 *
 * If the optional parameter string info2 is not NULL then it should contain
 * the name of a feature which has been tested before. In this case, if the error
 * code is the specific error ::MBG_ERR_NOT_SUPP_BY_DEV then a "not supported" message
 * is printed using info2.
 *
 * If info2 is NULL, or the error code is not ::MBG_ERR_NOT_SUPP_BY_DEV then the standard
 * error message is printed anyway.
 *
 * @param[in] rc    A positive number including ::MBG_SUCCESS, or one of the @ref MBG_ERROR_CODES
 * @param[in] what  A string indicated what failed
 * @param[in] info  An optional informational string telling what is not supported (may be @a NULL).
 *
 * @return  true if rc represented an error code, and a message has been printed, else false
 */
 bool mbg_cond_err_msg_info( int rc, const char *what, const char *info ) ;

 /**
 * @brief Translate an error code from the Labwindows/CVI RS-232 library to one of the @ref MBG_ERROR_CODES
 *
 * @param[in] cvi_rc  An error code returned by a CVI RS-232 library function
 * @param[in] info    An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 *
 * @see http://zone.ni.com/reference/en-XX/help/370051V-01/cvi/libref/cvirs232_error_conditions/
 */
 int mbg_cvi_rs232_error_to_mbg( int cvi_rc, const char *info ) ;

 /**
 * @brief Translate a Windows NTSTATUS code to one of the @ref MBG_ERROR_CODES
 *
 * @param[in] st    One of the NTSTATUS codes defined in ntstatus.h
 * @param[in] info  An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_win32_ntstatus_to_mbg( NTSTATUS st, const char *info ) ;

 /**
 * @brief Translate a Windows non-socket API return code to one of the @ref MBG_RETURN_CODES
 *
 * @param[in] win32_sys_rc  A Windows non-socket API error code as returned by GetLastError(), or ERROR_SUCCESS.
 * @param[in] info          An optional informational text string, or NULL.
 *
 * @return  One of the @ref MBG_RETURN_CODES
 */
 int mbg_win32_sys_err_to_mbg( DWORD win32_sys_rc, const char *info ) ;

 /**
 * @brief Translate a Windows socket API error code to one of the @ref MBG_ERROR_CODES
 *
 * @param[in] wsa_err  A Windows socket API error code as returned by WSAGetLastError()
 * @param[in] info     An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_win32_wsa_err_to_mbg( int wsa_err, const char *info ) ;

 /**
 * @brief Translate a POSIX errno error code to one of the @ref MBG_ERROR_CODES
 *
 * @param[in] posix_errno  A POSIX error code as usually defined in errno.h
 * @param[in] info         An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_posix_errno_to_mbg( int posix_errno, const char *info ) ;

 /**
 * @brief Translate a POSIX h_errno error code to one of the @ref MBG_ERROR_CODES
 *
 * This function is specific to translate error codes returned by
 * gethostbyname() and gethostbyaddr(). In case of error these functions
 * don't set errno but h_errno to a specific value.
 *
 * The functions gethostbyname() and gethostbyaddr() are obsolete,
 * and getaddressinfo() should be used preferably.
 *
 * @param[in] posix_h_errno  An error code as usually defined in netdb.h
 * @param[in] info           An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_posix_h_errno_to_mbg( int posix_h_errno, const char *info ) ;

 /**
 * @brief Get and translate last error after non-socket function call
 *
 * Retrieve the "last error" code after a non-socket function has been called
 * and translate to one of the @ref MBG_ERROR_CODES.
 *
 * On POSIX systems the "last error" code is always stored in errno, but
 * e.g. under Windows the "last error" code after a socket function
 * has to be retrieved by calling WSAGetLastError(), whereas the "last error"
 * code from non-socket POSIX-like functions has to be retrieved
 * by calling GetLastError().
 *
 * @param[in] info  An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_get_last_error( const char *info ) ;

 /**
 * @brief Get and translate last error after socket function call
 *
 * Retrieve the "last error" code after a socket function has been called
 * and translate to one of the @ref MBG_ERROR_CODES.
 *
 * On POSIX systems the "last error" code is always stored in errno, but
 * e.g. under Windows the "last error" code after a socket function
 * has to be retrieved by calling WSAGetLastError, whereas the "last error"
 * code from non-socket POSIX-like functions is stored in errno as usual.
 *
 * @param[in] info  An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_get_last_socket_error( const char *info ) ;

 /**
 * @brief Retrieve and convert last error after gethostbyname()
 *
 * This function is specific to retrieve and translate error codes
 * returned by gethostbyname() and gethostbyaddr(). In case of error
 * these functions don't set errno but h_errno on POSIX systems, but
 * under Windows the error code can be retrieved by WSAGetLastError()
 * as usual.
 *
 * The functions gethostbyname() and gethostbyaddr() are obsolete,
 * and getaddressinfo() should be used preferably.
 *
 * @param[in] info  An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_get_gethostbyname_error( const char *info ) ;

 /**
 * @brief Retrieve and convert last zlib internal error code
 *
 * @param[in] zlib_error  zlib internal error code
 * @param[in] info        An optional informational text string, or NULL
 * @param[in] msg         An optional zlib specific error msg, or NULL.
 *                        Struct z_stream contains member msg.
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
 int mbg_zlib_error_to_mbg( int zlib_error, const char *info, const char *msg ) ;


/* ----- function prototypes end ----- */



#if _USE_WIN32_PRIVATE_STATUS_CODES

static __mbg_inline /*HDR*/
/**
 * @brief Convert one of the @ref MBG_RETURN_CODES to an OS-specific code
 *
 * @param[in]  rc  One of the @ref MBG_RETURN_CODES.
 *
 * @see @ref MBG_RETURN_CODES
 *
 * @return An OS-specific success or error code
 */
int mbg_ret_val_to_os( int rc )
{
  return mbg_rc_is_success( rc ) ? MBG_SYS_RC_SUCCESS : mbg_errno_to_os( rc );

}  // mbg_ret_val_to_os

#endif  // _USE_WIN32_PRIVATE_STATUS_CODES


#ifdef __cplusplus
}
#endif


/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _MBGERROR_H */
