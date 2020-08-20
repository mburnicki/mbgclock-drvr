
/**************************************************************************
 *
 *  $Id: mbgerror.c 1.10 2019/11/11 09:43:41 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Meinberg Library to communicate with USB devices from user space
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgerror.c $
 *  Revision 1.10  2019/11/11 09:43:41  martin
 *  Tiny doxygen change.
 *  Revision 1.9  2019/08/26 15:15:10  martin
 *  Translate POSIX ENODATA to MBG_ERR_NO_DATA.
 *  Revision 1.8  2019/08/20 08:22:51  martin
 *  Translate win32 ERROR_FILE_NOT_FOUND to MBG_ERR_NO_ENTITY.
 *  Revision 1.7  2019/04/03 12:53:55  martin
 *  Use new code MBG_ERR_BAD_ADDR.
 *  Revision 1.6  2018/11/22 14:47:15  martin
 *  Support QNX target.
 *  New function mbg_win32_ntstatus_to_mbg().
 *  Added and renamed static code conversion tables.
 *  Added and updated doxygen comments.
 *  Revision 1.5  2018/09/21 09:44:38  martin
 *  Renamed mbg_win32_last_err_to_mbg() to
 *  mbg_win32_sys_err_to_mbg().
 *  Translate win32 error codes ERROR_BUFFER_OVERFLOW
 *  and ERROR_INSUFFICIENT_BUFFER.
 *  Updated some comments.
 *  Revision 1.4  2018/06/25 14:21:09Z  martin
 *  Cleaned up definition and usage of control macros.
 *  Distinguish between kernel mode and user mode on Windows.
 *  Support some new error codes.
 *  Revision 1.3  2017/07/05 09:20:07  martin
 *  Fixed a bug where POSIX error ENODEV wasn't mapped at all, but
 *  EXDEV was instead translated erraneously to MBG_ERR_NO_DEV.
 *  Mapped POSIX error ENOSPC to MBG_ERR_NO_SPACE, and EFAULT
 *  to MBG_ERR_INV_PARM.
 *  Mapped Windows WSA error codes WSAEFAULT and WSAEINVAL
 *  to MBG_ERR_INV_PARM.
 *  Renamed mbg_ioctl_err() to mbg_cond_err_msg().
 *  New function mbg_cond_err_msg_info() which takes an optional
 *  info string which is printed if the error code to be checked
 *  is MBG_ERR_NOT_SUPP_BY_DEV.
 *  Fixed build in Windows kernel mode.
 *  Fixed syntax error in CVI-specific code.
 *  Quieted some compiler warnings.
 *  Revision 1.2  2016/08/05 12:25:44Z  martin
 *  Added some functions.
 *  Revision 1.1  2014/03/07 12:08:14  martin
 *  Initial revision.
 *
 **************************************************************************/

#define _MBGERROR
 #include <mbgerror.h>
#undef _MBGERROR

#include <mbg_tgt.h>

#if defined( MBG_TGT_WIN32 )

  #if !defined( MBG_TGT_KERNEL )
    #include <errno.h>
    #include <stdio.h>

    #define _MBG_TGT_HAS_POSIX_ERRNO    1
    #define _MBG_TGT_HAS_WIN32_ERR_API  1
  #else
    #include <mbgddmsg.h>

    #define _MBG_TGT_OMIT_LAST_ERROR    1
    #define _MBG_TGT_OMIT_ERR_MSG       1    // Not (yet) supported in Windows kernel mode
  #endif

#elif defined( MBG_TGT_CVI )

  // Nothing to do here.

#elif defined( MBG_TGT_LINUX )

  #if !defined( MBG_TGT_KERNEL )
    #include <errno.h>
    #include <stdio.h>
    #include <string.h>
    #include <netdb.h>

    #define _MBG_TGT_HAS_POSIX_ERRNO    1
    #define _MBG_TGT_HAS_POSIX_H_ERRNO  1
  #else
    #include <asm/errno.h>

    #define _MBG_TGT_HAS_POSIX_ERRNO    1
    #define _MBG_TGT_OMIT_LAST_ERROR    1
    #define _MBG_TGT_OMIT_ERR_MSG       1
  #endif

#elif defined( MBG_TGT_BSD )

  #if !defined( MBG_TGT_KERNEL )
    #include <errno.h>
    #include <stdio.h>
    #include <string.h>
    #include <netdb.h>

    #define _MBG_TGT_HAS_POSIX_ERRNO    1
    #define _MBG_TGT_HAS_POSIX_H_ERRNO  1
  #else
    #include <sys/errno.h>
    #if defined( MBG_TGT_FREEBSD )
      #include <sys/stddef.h>   // NULL
    #elif defined( MBG_TGT_NETBSD )
      #include <sys/null.h>     // NULL
    #endif

    #define _MBG_TGT_HAS_POSIX_ERRNO    1
    #define _MBG_TGT_OMIT_LAST_ERROR    1
    #define _MBG_TGT_OMIT_ERR_MSG       1
  #endif

#elif defined( MBG_TGT_QNX_NTO )  // QNX 6.x only, but not QNX 4.x

  #include <stdio.h>
  #include <errno.h>
  #include <string.h>
  #include <netdb.h>

  #define _MBG_TGT_HAS_POSIX_ERRNO      1
  #define _MBG_TGT_HAS_POSIX_H_ERRNO    1

#elif defined( MBG_TGT_DOS )

  #include <stdio.h>
  #include <stdlib.h>
  #include <stddef.h>
  #include <errno.h>

  #define _MBG_TGT_HAS_POSIX_ERRNO      1
  #define _MBG_TGT_OMIT_SOCKET_ERRORS   1    // No network socket support by OS

#endif

#if defined( USE_MBG_ZLIB )
  #include <zlib.h>
#endif

#if !defined( DEBUG )
  // FIXME This should be done globally across projects.
  #define DEBUG   0
#endif


typedef struct
{
  int srch_errno;
  int mapped_errno;

} ERRNO_CNV_ENTRY;



#if defined( _MBG_TGT_HAS_POSIX_ERRNO )

/**
 * @brief Mappings between Meinberg error codes and POSIX error codes
 *
 * Always refer to the symbolic names only. The numeric codes listed
 * in the comments below are just for a quick reference, and may vary
 * depending on the OS type and version.
 *
 * Linux:          /usr/include/asm-generic/errno.h<br>
 * FreeBSD/NetBSD: /usr/include/sys/errno.h
 *
 * @see ::mbg_posix_errno_to_mbg
 * @see POSIX spec at http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_03
 */
static ERRNO_CNV_ENTRY posix_errno_table[] =
{
  { EPERM,     MBG_ERR_PERM            },  //   1, Operation not permitted (e.g. when trying to set time without sufficient permissions).
  { ENOENT,    MBG_ERR_NO_ENTITY       },  //   2, No such file or directory.
  // { ESRCH,                          },  //   3, No such process.
  { EINTR,     MBG_ERR_INTR            },  //   4, Interrupted function call.
  { EIO,       MBG_ERR_IO              },  //   5, Input/output error (from physical device).
  { ENXIO,     MBG_ERR_NOT_FOUND       },  //   6, No such device or address (refering to a device that doesn't exist, or request beyond its capabilities) of the device.
  // { E2BIG,                          },  //   7, Argument list too long (or lack of space in an output buffer, or argument exceeds system-imposed maximum.
  // { ENOEXEC,                        },  //   8, Exec format error (executable file format error).
  // { EBADF,                          },  //   9, Bad file number/descriptor (or a read (write) tried on a file only open for writing (reading)).
  // { ECHILD,                         },  //  10, No child processes (when waiting for child process).
  { EAGAIN,    MBG_ERR_AGAIN           },  //  11, Try again, resource temporarily unavailable (later calls may complete normally, maybe same as EWOULDBLOCK).
  { ENOMEM,    MBG_ERR_NO_MEM          },  //  12, Out of memory (can't allocate memory).
  { EACCES,    MBG_ERR_ACCESS          },  //  13, Permission denied (e.g. when trying to access a device without sufficient permissions).
  { EFAULT,    MBG_ERR_BAD_ADDRESS     },  //  14, Bad address (e.g. invalid address in a function argument).
  // { ENOTBLK,                        },  //  15, Block device required (eventually no POSIX error, but supported in Linux and *BSD).
  { EBUSY,     MBG_ERR_BUSY            },  //  16, Device or resource busy.
  { EEXIST,    MBG_ERR_EXIST           },  //  17, File exists.
  // { EXDEV,                          },  //  18, Cross-device link.
  { ENODEV,    MBG_ERR_NO_DEV          },  //  19, ### No such device (operation not supported by device).
  // { ENOTDIR,                        },  //  20, Not a directory (when a directory was expected for the operation).
  // { EISDIR,                         },  //  21, Is a directory (when a directory was *not* expected for the operation).
  { EINVAL,    MBG_ERR_INV_PARM        },  //  22, Invalid argument (was supplied to a function).
  // { ENFILE,                         },  //  23, Too many files open in system (file table overflow).
  // { EMFILE,                         },  //  24, Too many open files (or file descriptor value too large).
  { ENOTTY,    MBG_ERR_NOT_SUPP_BY_DEV },  //  25, ### Not a typewriter (actually means "inappropriate I/O control for device").
  // { ETXTBSY,                        },  //  26, Text file busy ("text" actually means "binary program file").
  // { EFBIG,                          },  //  27, File too large.
  { ENOSPC,    MBG_ERR_NO_SPACE        },  //  28, No space left on device (when writing a file or extending directory).
  { ESPIPE,    MBG_ERR_PIPE            },  //  29, Illegal seek (when attempting to access the file offset associated with a pipe or FIFO).
  // { EROFS,                          },  //  30, Read-only file system.
  // { EMLINK,                         },  //  31, Too many links (link count of a single file exceed).
  // { EPIPE,                          },  //  32, Broken pipe (writing to a socket, pipe, or FIFO which has no process to read the data).
  // { EDOM,                           },  //  33, Math argument out of domain of function.
  { ERANGE,    MBG_ERR_RANGE           },  //  34, Math result too large or too small (not representable).
#if defined( ENODATA )
  { ENODATA,   MBG_ERR_NO_DATA         },  //  61, No (more) data.
#endif
#if defined( ETIME )
  { ETIME,     MBG_ERR_TIMER           },  //  62, Timer expired (e.g. stream timeout on USB disconnect).
#endif
#if defined( EOVERFLOW )
  { EOVERFLOW, MBG_ERR_OVERFLOW        },  //  75, Value too large for defined data type (can't be stored).
#endif
#if defined( ENOTSOCK )
  { ENOTSOCK, MBG_ERR_NOT_A_SOCKET     },  //  88, Socket operation on non-socket (file descriptor is not a socket).
#endif
#if defined( ECONNRESET )
  { ECONNRESET, MBG_ERR_CONN_RESET     },  //  104, Connection reset by peer
#endif
#if defined( ETIMEDOUT )
  { ETIMEDOUT, MBG_ERR_TIMEOUT         },  //  110, Connection timed out
#endif
  { 0, 0                               }   // end-of-table identifier

};  // posix_errno_table

#endif  // defined( _MBG_TGT_HAS_POSIX_ERRNO )



#if defined( _MBG_TGT_HAS_POSIX_H_ERRNO )

static ERRNO_CNV_ENTRY posix_h_errno_table[] =
{
  // POSIX codes taken from Linux netdb.h
  { HOST_NOT_FOUND, MBG_ERR_HOST_NOT_FOUND },  // The specified host is unknown
  // { NO_ADDRESS,       },  // Usually same numeric value as NO_DATA
  // { NO_DATA,          },  // The requested name is valid but does not have an IP address
  // { NO_RECOVERY,      },  // A nonrecoverable name server error occurred
  // { TRY_AGAIN,        },  // A temporary error occurred on an authoritative name server. Try again later.
  { 0, 0                 }   // end-of-table identifier

};  // posix_h_errno_table

#endif  // defined( _MBG_TGT_HAS_POSIX_H_ERRNO )



#if defined( MBG_TGT_CVI )

static ERRNO_CNV_ENTRY cvi_rs232_error_table[] =
{
  // { kRS_UnknownSysError,            },  // Unknown system error.
  // { kRS_InvalidPortNum,             },  // In valid port number.
  // { kRS_PortNotOpen,                },  // Port is not open.
  // { kRS_UnknownIOError,             },  // Unknown I/O error.
  // { kRS_InternalError,              },  // Unexpected internal error.
  // { kRS_NoPortFound,                },  // No serial port found.
  // { kRS_CanNotOpenPort,             },  // Cannot open port.
  // { kRS_NullPointerPassed,          },  // A NULL pointer was passed when a non-NULL pointer was expected.
  // { kRS_OutOfMemory,                },  // Out of memory.
  // { kRS_OutOfSystemResources,       },  // Unable to allocate system resources.
  // { kRS_InvalidParameter,           },  // Invalid parameter.
  // { kRS_InvalidBaudRate,            },  // Invalid baud rate.
  // { kRS_InvalidParity,              },  // Invalid parity.
  // { kRS_InvalidDataBits,            },  // Invalid number of data bits.
  // { kRS_InvalidStopBits,            },  // Invalid number of stop bits.
  // { kRS_BadFileHandle,              },  // Bad file handle.
  // { kRS_FileIOError,                },  // File I/O error.
  // { kRS_InvalidCount,               },  // Invalid count; must be greater than or equal to 0.
  // { kRS_InvalidIntLevel,            },  // Invalid interrupt level.
  // { kRS_IOTimeOut,                  },  // I/O operation timed out.
  // { kRS_InvalidBreakTime,           },  // Break time must be a positive value.
  // { kRS_InvalidInQSize,             },  // Requested input queue size must be 0 or greater.
  // { kRS_InvalidOutQSize,            },  // Requested output queue size must be 0 or greater.
  // { kRS_GeneralIOFailure,           },  // General I/O error.
  // { kRS_InvalidBufferPointer,       },  // Buffer parameter is NULL.
  // { kRS_VISALibrariesMissing,       },  // A necessary run-time library could not be found or loaded.
  // { kRS_NoAckReceived,              },  // Packet was sent, but no acknowledgment was received.
  // { kRS_MaxRetriesBeforeSend,       },  // Packet was not sent within retry limit.
  // { kRS_MaxRetriesBeforeReceived,   },  // Packet was not received within retry limit.
  // { kRS_UnexpectedEOT,              },  // End of transmission character encountered when start of data character expected.
  // { kRS_CanNotReadPackNum,          },  // Unable to read packet number.
  // { kRS_InconsistentPackNum,        },  // Inconsistent packet number.
  // { kRS_CanNotReadPackData,         },  // Unable to read packet data.
  // { kRS_CanNotReadCheckSum,         },  // Unable to read checksum.
  // { kRS_CheckSumError,              },  // Checksum received did not match computed checksum.
  // { kRS_PackSizeGTInQ,              },  // Packet size exceeds input queue size.
  // { kRS_OpenFileError,              },  // Error opening file.
  // { kRS_ReadFileError,              },  // Error reading file.
  // { kRS_NoInitNegAck,               },  // Did not receive initial negative acknowledgment character.
  // { kRS_NoAckAfterEOT,              },  // Did not receive acknowledgment after end of transmission character was sent.
  // { kRS_WriteFileError,             },  // Error writing to file.
  // { kRS_NoSOHorEOT,                 },  // Did not receive either a start of data or end of transmission character when expected.
  // { kRS_TransferCancelled,          },  // Transfer was canceled because CAN ASCII character was received.
  // { kRS_InvalidStartDelay,          },  // Invalid start delay.
  // { kRS_InvalidMaxTries,            },  // Invalid maximum number of retries.
  // { kRS_InvalidWaitPeriod,          },  // Invalid wait period.
  // { kRS_InvalidPacketSize,          },  // Invalid packet size.
  // { kRS_CanNotReadCRC,              },  // Unable to read Cyclical Redundancy Check.
  // { kRS_CRCError,                   },  // Cyclical Redundancy Check error.
  { 0, 0                               }   // end-of-table identifier

};  // cvi_rs232_error_table

#endif  // defined( MBG_TGT_CVI )



#if defined( MBG_TGT_WIN32 )

#if defined( MBG_TGT_KERNEL )

/**
 * @brief Mappings of some NTSTATUS codes to Meinberg error codes
 *
 * Kernel space Windows APIs use NTSTATUS codes as return values,
 * which consist of several bit fields which also provide some severity
 * and facility codes, similar to the message IDs used with text resources.
 * The status codes and associated message texts are defined
 * in the ntstatus.h file shipped with the various Windows DDKs.
 *
 * Please note that WIN32 user space API function return a different
 * set of error codes on failure. See ::win32_sys_err_table.
 *
 * @see ::mbg_ioctl_to_ntstatus_table
 * @see ::win32_kernel_status_table
 * @see ::win32_sys_err_table
 * @see ::win32_wsa_err_table
 * @see @ref MBG_ERROR_CODES
 */
static ERRNO_CNV_ENTRY win32_kernel_status_table[] =
{
  { STATUS_INVALID_PARAMETER,       MBG_ERR_INV_PARM,        },  // 0xC000000DL, An invalid parameter was passed to a service or function.
  { STATUS_INVALID_DEVICE_REQUEST,  MBG_ERR_INV_DEV_REQUEST  },  // 0xC0000010L, The specified request is not a valid operation for the target device.
  { STATUS_DEVICE_BUSY,             MBG_ERR_IRQ_UNSAFE       },  // 0x80000011L, The device is currently busy.
  { STATUS_NO_MEMORY,               MBG_ERR_NO_MEM           },  // 0xC0000017L, Not enough memory available to complete the specified operation.
  { STATUS_NO_DATA_DETECTED,        MBG_ERR_NOT_SUPP_BY_DEV  },  // 0x80000022L, (normally used with tape access, but mis-used here)
  { STATUS_ACCESS_DENIED,           MBG_ERR_PERM             },  // 0xC0000022L, A process has requested access to an object, but has not been granted those access rights.
  { STATUS_IO_DEVICE_ERROR,         MBG_ERR_IO               },  // 0xC0000185L, The I/O device reported an I/O error.

  { STATUS_BUFFER_TOO_SMALL,        MBG_ERR_BUFFER_TOO_SMALL },  // 0xC0000023L, The buffer is too small to contain the entry. No information has been written to the buffer.
  // { STATUS_CANCELLED,                                     },  // 0xC0000120L, The I/O request was canceled.
  // { STATUS_DELETE_PENDING,                                },  // 0xC0000056L, A non close operation has been requested of a file object with a delete pending.
  // { STATUS_DEVICE_CONFIGURATION_ERROR,                    },  // 0xC0000182L, The I/O device is configured incorrectly or the configuration parameters to the driver are incorrect.
  { STATUS_DEVICE_NOT_READY,        MBG_ERR_NOT_READY        },  // 0xC00000A3L, The device (drive) is not ready for use.
  // { STATUS_INSUFFICIENT_RESOURCES,                        },  // 0xC000009AL, Insufficient system resources exist to complete the API.
  // { STATUS_MORE_PROCESSING_REQUIRED,                      },  // 0xC0000016L, The specified IRP cannot be disposed of because the I/O operation is not complete.
  // { STATUS_NOT_IMPLEMENTED,                               },  // 0xC0000002L, The requested operation is not implemented.
  // { STATUS_NOT_SUPPORTED,                                 },  // 0xC00000BBL, The network request is not supported.
  // { STATUS_NO_SUCH_DEVICE,                                },  // 0xC000000EL, A device which does not exist was specified.
  // { STATUS_OBJECT_NAME_NOT_FOUND,                         },  // 0xC0000034L, Object Name not found.
  // { STATUS_PENDING,                                       },  // 0x00000103L, The operation that was requested is pending completion.
  { STATUS_TIMEOUT,                 MBG_ERR_TIMEOUT          },  // 0x00000102L, STATUS_TIMEOUT
  // { STATUS_UNSUCCESSFUL,                                  },  // 0xC0000001L, The requested operation was unsuccessful.
  { 0, 0                                                     }   // end-of-table identifier

};  // win32_kernel_status_table



#if !_USE_WIN32_PRIVATE_STATUS_CODES

/**
 * @brief Windows IOCTL error conversion table.
 *
 * The IOCTL handler of a Meinberg kernel driver actually calls
 * some Meinberg mbglib functions to access a device, and those
 * functions normally return one of the @ref MBG_RETURN_CODES.
 *
 * However, when the Windows kernel calls the driver's IOCTL handler
 * it expects an NTSTATUS code to be returned, so this table can be
 * used in an IOCTL handler to map a Meinberg return code to a
 * predefined NTSTATUS code that can be returned.
 *
 * To make things worse, Windows then remaps the NTSTATUS code to one
 * of the WIN32 error codes when it passes the IOCTL results up to the
 * user space function that submitted the IOCTL request.
 *
 * The user space function can then convert the returned code back
 * to one of the  @ref MBG_RETURN_CODES.
 *
 * For the mapping of NTSTATUS codes to WIN32 error codes
 * done by Windows when it passes an IOCTL return code from kernel
 * space to user space, see:
 * https://support.microsoft.com/en-us/help/113996/info-mapping-nt-status-error-codes-to-win32-error-codes
 *
 * So care should be taken that the table entries here are defined
 * such that in spite of the mnumerous conversions the same error code
 * that was returned by a driver function is finally returned by the
 * Meinberg user space API call. For example:
 *
 *  - Driver function returns ::MBG_ERR_INV_PARM error.
 *
 *  - Table ::mbg_ioctl_to_ntstatus_table is used to convert this to
 *    STATUS_INVALID_PARAMETER, which is returned by the IOCTL handler
 *    in kernel space.
 *
 *  - Windows converts this to ERROR_INVALID_PARAMETER when it passes
 *    the error code up to user space.
 *
 *  - The user space function uses ::win32_sys_err_table to convert this
 *    to a Meinberg error code, which again yields ::MBG_ERR_INV_PARM.
 *
 * @note Eventually it would be possible to defined custom NTSTATUS codes
 * in kernel space, e.g. ((NTSTATUS) (0xC0000000 | 0x20000000 | 31)).
 * Apparently, such custom error status codes are passed up to user space,
 * and GetLastError() after the ioctl() call then returns a negative number,
 * i.e. -31 for the example here.
 * This had to be tested, though, and could be used to pass Meinberg error
 * codes directly up to user space, without the need for a multiple conversion.
 * However, there's no guarantee that this always works, and would still
 * work in future Windows versions.
 *
 * @see ::win32_kernel_status_table
 * @see ::win32_sys_err_table
 * @see ::win32_wsa_err_table
 * @see @ref MBG_ERROR_CODES
 */
static ERRNO_CNV_ENTRY mbg_ioctl_to_ntstatus_table[] =
{
  { MBG_ERR_INV_PARM,          STATUS_INVALID_PARAMETER       },  // ERROR_INVALID_PARAMETER
  { MBG_ERR_INV_DEV_REQUEST,   STATUS_INVALID_DEVICE_REQUEST  },  // ERROR_BAD_COMMAND
  { MBG_ERR_IRQ_UNSAFE,        STATUS_DEVICE_BUSY             },  // ERROR_BUSY
  { MBG_ERR_NO_MEM,            STATUS_NO_MEMORY               },  // ERROR_NOT_ENOUGH_MEMORY
  { MBG_ERR_NOT_SUPP_BY_DEV,   STATUS_NO_DATA_DETECTED        },  // ERROR_NO_DATA_DETECTED (mis-used here)
  { MBG_ERR_PERM,              STATUS_ACCESS_DENIED           },  // ERROR_ACCESS_DENIED
  { MBG_ERR_IO,                STATUS_IO_DEVICE_ERROR         },  // ERROR_IO_DEVICE
  { 0, 0                                                      }   // end-of-table identifier

};  // mbg_ioctl_to_ntstatus_table

#endif  // !_USE_WIN32_PRIVATE_STATUS_CODES


#else  // Windows user space

/**
 * @brief Mappings of WIN32 error codes to Meinberg error codes
 *
 * If a WIN32 user space API function encounters an error,
 * the Windows GetLastError() function has to be called in
 * most cases to retrieve an associated error code, which is
 * a positive integer number.
 *
 * Existing error codes and associated message texts
 * are defined in the WinError.h file shipped with the
 * various SDKs and IDEs.
 *
 * Please note that Windows socket functions return a different
 * set of error codes on failure, and WSAGetLastError() has to
 * be called to retrieve the associated error code.
 * See ::win32_wsa_err_table.
 *
 * @see ::win32_wsa_err_table
 * @see ::mbg_ioctl_to_ntstatus_table
 * @see ::win32_kernel_status_table
 * @see @ref MBG_ERROR_CODES
 */
static ERRNO_CNV_ENTRY win32_sys_err_table[] =
{
  // Mappings for some Windows error codes defined in WinError.h
  { ERROR_FILE_NOT_FOUND,        MBG_ERR_NO_ENTITY        },  // 2L: The system cannot find the file specified.
  { ERROR_PATH_NOT_FOUND,        MBG_ERR_NO_ENTITY        },  // 3L: The system cannot find the path specified.
  { ERROR_ACCESS_DENIED,         MBG_ERR_ACCESS           },  // 5L: Access is denied.
  { ERROR_INVALID_HANDLE,        MBG_ERR_INV_HANDLE       },  // 6L: The handle is invalid.
  { ERROR_NOT_ENOUGH_MEMORY,     MBG_ERR_NO_MEM           },  // 8L: Not enough storage is available to process this command.
  { ERROR_OUTOFMEMORY,           MBG_ERR_NO_MEM           },  // 14L: Not enough storage is available to complete this operation.
  // { ERROR_WRITE_PROTECT,                               },  // 19L: The media is write protected.
  // { ERROR_BAD_UNIT,                                    },  // 20L: The system cannot find the device specified.
  { ERROR_NOT_READY,             MBG_ERR_NOT_READY        },  // 21L: The device is not ready.
  { ERROR_WRITE_FAULT,           MBG_ERR_IO               },  // 29L: The system cannot write to the specified device.
  { ERROR_READ_FAULT,            MBG_ERR_IO               },  // 30L: The system cannot read from the specified device.
  { ERROR_GEN_FAILURE,           MBG_ERR_UNSPEC           },  // 31L: A device attached to the system is not functioning.
  // { ERROR_SHARING_VIOLATION,                           },  // 32L: The process cannot access the file because it is being used by another process.
  // { ERROR_LOCK_VIOLATION,                              },  // 33L: The process cannot access the file because another process has locked a portion of the file.
  // { ERROR_NOT_SUPPORTED,                               },  // 50L: The request is not supported.
  // { ERROR_DUP_NAME,                                    },  // 52L: A duplicate name exists on the network.
  // { ERROR_BAD_DEV_TYPE,                                },  // 66L: The network resource type is not correct.
  { ERROR_INVALID_PARAMETER,     MBG_ERR_INV_PARM         },  // 87L: The parameter is incorrect.
  { ERROR_BUFFER_OVERFLOW,       MBG_ERR_OVERFLOW         },  // 111L: The file name is too long.
  { ERROR_INSUFFICIENT_BUFFER,   MBG_ERR_BUFFER_TOO_SMALL },  // 122L: The data area passed to a system call is too small.
  { ERROR_BUSY,                  MBG_ERR_BUSY             },  // 170L: The requested resource is in use.
  // { ERROR_NOACCESS,                                    },  // 998L: Invalid access to memory location.
  { ERROR_NO_DATA_DETECTED,      MBG_ERR_NOT_SUPP_BY_DEV  },  // 1104L: No more data is on the tape. (mis-used here)
  { ERROR_IO_DEVICE,             MBG_ERR_IO               },  // 1117L: The request could not be performed because of an I/O device error.
  { ERROR_PRIVILEGE_NOT_HELD,    MBG_ERR_PERM             },  // 1314L: A required privilege is not held by the client.
  { 0, 0                                                  }   // end-of-table identifier

};  // win32_sys_err_table



/**
 * @brief Mappings of Winsock error codes to Meinberg error codes
 *
 * If a Windows socket (Winsock) function encounters an error,
 * the Windows WSAGetLastError() function has to be called in
 * most cases to retrieve an associated error code, which is
 * a positive integer number.
 *
 * Existing error codes and associated message texts are
 * defined in the Winsock2.h file shipped with the various
 * SDKs and IDEs.
 *
 * Please note that the standard WIN32 API functions return a
 * different set of error codes on failure, and GetLastError()
 * has to be called to retrieve the associated error code.
 * See ::win32_err_table.
 *
 * @see ::win32_sys_err_table
 * @see ::mbg_ioctl_to_ntstatus_table
 * @see ::win32_kernel_status_table
 * @see @ref MBG_ERROR_CODES
 */
static ERRNO_CNV_ENTRY win32_wsa_err_table[] =
{
  { WSAEINTR,               MBG_ERR_INTR             },    // 10004L A blocking operation was interrupted by a call to WSACancelBlockingCall.
  // { WSAEBADF                                            // 10009L The file handle supplied is not valid.
  // { WSAEACCES                                           // 10013L An attempt was made to access a socket in a way forbidden by its access permissions.
  { WSAEFAULT,              MBG_ERR_BAD_ADDRESS      },    // 10014L The system detected an invalid pointer address in attempting to use a pointer argument in a call.
  { WSAEINVAL,              MBG_ERR_INV_PARM         },    // 10022L An invalid argument was supplied.
  // { WSAEMFILE                                           // 10024L Too many open sockets.
  { WSAEWOULDBLOCK,         MBG_ERR_AGAIN            },    // 10035L A non-blocking socket operation could not be completed immediately.
  // { WSAEINPROGRESS                                      // 10036L A blocking operation is currently executing.
  // { WSAEALREADY                                         // 10037L An operation was attempted on a non-blocking socket that already had an operation in progress.
  { WSAENOTSOCK,            MBG_ERR_NOT_A_SOCKET     },    // 10038L An operation was attempted on something that is not a socket.
  // { WSAEDESTADDRREQ                                     // 10039L A required address was omitted from an operation on a socket.
  // { WSAEMSGSIZE                                         // 10040L A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself.
  // { WSAEPROTOTYPE                                       // 10041L A protocol was specified in the socket function call that does not support the semantics of the socket type requested.
  // { WSAENOPROTOOPT                                      // 10042L An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call.
  // { WSAEPROTONOSUPPORT                                  // 10043L The requested protocol has not been configured into the system, or no implementation for it exists.
  // { WSAESOCKTNOSUPPORT                                  // 10044L The support for the specified socket type does not exist in this address family.
  // { WSAEOPNOTSUPP                                       // 10045L The attempted operation is not supported for the type of object referenced.
  // { WSAEPFNOSUPPORT                                     // 10046L The protocol family has not been configured into the system or no implementation for it exists.
  // { WSAEAFNOSUPPORT                                     // 10047L An address incompatible with the requested protocol was used.
  // { WSAEADDRINUSE                                       // 10048L Only one usage of each socket address (protocol/network address/port) is normally permitted.
  // { WSAEADDRNOTAVAIL                                    // 10049L The requested address is not valid in its context.
  // { WSAENETDOWN                                         // 10050L A socket operation encountered a dead network.
  // { WSAENETUNREACH                                      // 10051L A socket operation was attempted to an unreachable network.
  // { WSAENETRESET                                        // 10052L The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
  // { WSAECONNABORTED                                     // 10053L An established connection was aborted by the software in your host machine.
  { WSAECONNRESET,          MBG_ERR_CONN_RESET       },    // 10054L An existing connection was forcibly closed by the remote host.
  // { WSAENOBUFS                                          // 10055L An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
  // { WSAEISCONN                                          // 10056L A connect request was made on an already connected socket.
  // { WSAENOTCONN                                         // 10057L A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.
  // { WSAESHUTDOWN                                        // 10058L A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.
  // { WSAETOOMANYREFS                                     // 10059L Too many references to some kernel object.
  // { WSAETIMEDOUT                                        // 10060L A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.
  // { WSAECONNREFUSED                                     // 10061L No connection could be made because the target machine actively refused it.
  // { WSAELOOP                                            // 10062L Cannot translate name.
  // { WSAENAMETOOLONG                                     // 10063L Name component or name was too long.
  // { WSAEHOSTDOWN                                        // 10064L A socket operation failed because the destination host was down.
  // { WSAEHOSTUNREACH                                     // 10065L A socket operation was attempted to an unreachable host.
  // { WSAENOTEMPTY                                        // 10066L Cannot remove a directory that is not empty.
  // { WSAEPROCLIM                                         // 10067L A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.
  // { WSAEUSERS                                           // 10068L Ran out of quota.
  // { WSAEDQUOT                                           // 10069L Ran out of disk quota.
  // { WSAESTALE                                           // 10070L File handle reference is no longer available.
  // { WSAEREMOTE                                          // 10071L Item is not available locally.
  // { WSASYSNOTREADY                                      // 10091L WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.
  // { WSAVERNOTSUPPORTED                                  // 10092L The Windows Sockets version requested is not supported.
  { WSANOTINITIALISED,      MBG_ERR_SOCK_INIT        },    // 10093L Either the application has not called WSAStartup, or WSAStartup failed.
  // { WSAEDISCON                                          // 10101L Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.
  // { WSAENOMORE                                          // 10102L No more results can be returned by WSALookupServiceNext.
  // { WSAECANCELLED                                       // 10103L A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.
  // { WSAEINVALIDPROCTABLE                                // 10104L The procedure call table is invalid.
  // { WSAEINVALIDPROVIDER                                 // 10105L The requested service provider is invalid.
  // { WSAEPROVIDERFAILEDINIT                              // 10106L The requested service provider could not be loaded or initialized.
  // { WSASYSCALLFAILURE                                   // 10107L A system call that should never fail has failed.
  // { WSASERVICE_NOT_FOUND                                // 10108L No such service is known. The service cannot be found in the specified name space.
  // { WSATYPE_NOT_FOUND                                   // 10109L The specified class was not found.
  // { WSA_E_NO_MORE                                       // 10110L No more results can be returned by WSALookupServiceNext.
  // { WSA_E_CANCELLED                                     // 10111L A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.
  // { WSAEREFUSED                                         // 10112L A database query failed because it was actively refused.
  { WSAHOST_NOT_FOUND,      MBG_ERR_HOST_NOT_FOUND   },    // 11001L No such host is known.
  // { WSATRY_AGAIN                                        // 11002L This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server.
  // { WSANO_RECOVERY                                      // 11003L A non-recoverable error occurred during a database lookup.
  // { WSANO_DATA                                          // 11004L The requested name is valid, but no data of the requested type was found.
  { 0, 0                                             }     // end-of-table identifier

};  // win32_wsa_err_table

#endif  // Windows user space

#endif  // defined( MBG_TGT_WIN32 )



static /*HDR*/
/**
 * @brief Lookup some error code in a conversion table
 *
 * @param[in]  srch_errno  The error number to be looked up in the conversion table
 * @param[in]  tbl         A conversion table
 * @param[in]  dflt_val    The code to be returned if no table entry was found.
 *
 * @return @ref MBG_ERROR_CODES associated with the original error code,
 *         or @p dflt_val if original code not found in table.
 */
int lookup_tbl_errno( int srch_errno, const ERRNO_CNV_ENTRY tbl[], int dflt_val )
{
  const ERRNO_CNV_ENTRY *p;

  for ( p = tbl; p->srch_errno || p->mapped_errno; p++ )
    if ( p->srch_errno == srch_errno )
      return p->mapped_errno;

  return dflt_val;

}  // lookup_tbl_errno


#if !_USE_WIN32_PRIVATE_STATUS_CODES

/*HDR*/
/**
 * @brief Convert one of the @ref MBG_ERROR_CODES to an OS-specific format
 *
 * @param[in]  err_no  One of the @ref MBG_ERROR_CODES.
 *
 * @see @ref MBG_ERROR_CODES
 *
 * @return An OS-specific error code
 */
int mbg_errno_to_os( int err_no )
{
  #if defined( MBG_TGT_KERNEL )

    #if defined( MBG_TGT_WIN32 )

      if ( err_no == MBG_SUCCESS )
        return STATUS_SUCCESS;

      return lookup_tbl_errno( err_no, mbg_ioctl_to_ntstatus_table, STATUS_UNSUCCESSFUL );

    #elif defined( MBG_TGT_LINUX )

    #elif defined( MBG_TGT_FREEBSD )

    #elif defined( MBG_TGT_NETBSD )

    #else

      #error this needs to be implemented

    #endif

  #else  // user space

  #if 0
  #if defined( MBG_TGT_WIN32 )  // FIXME TODO
    // Windows uses specially encoded numbers
    return ( -err_no | 0xE0000000L );

  #elif defined( MBG_TGT_BSD )

    return -err_no;

  #else

    return err_no;

  #endif
  #endif  // 0

  #endif

  return err_no;

}  // mbg_errno_to_os

#endif  // _USE_WIN32_PRIVATE_STATUS_CODES



/*HDR*/
/**
 * @brief Return an error string associated with the @ref MBG_ERROR_CODES
 *
 * @param[in] mbg_errno  One of the @ref MBG_ERROR_CODES
 *
 * @return  A constant string describing the error, or NULL for unknown error codes
 */
const char *mbg_strerror( int mbg_errno )
{
  static const MBG_CODE_NAME_TABLE_ENTRY tbl[] = MBG_ERR_STR_TABLE_ENG;

  const MBG_CODE_NAME_TABLE_ENTRY *p;

  for ( p = tbl; p->name; p++ )
  {
    if ( mbg_errno == p->code )
      return p->name;
  }

  return "Unknown error";

}  // mbg_strerror



#if !defined( _MBG_TGT_OMIT_ERR_MSG )

/*HDR*/
/**
 * @brief Check if a value is an error code and print an associated error message
 *
 * @param[in] rc    A positive number including ::MBG_SUCCESS, or one of the @ref MBG_ERROR_CODES
 * @param[in] what  A string indicated what failed
 *
 * @return  true if rc represented an error code, and a message has been printed, else false
 */
bool mbg_cond_err_msg( int rc, const char *what )
{
  return mbg_cond_err_msg_info( rc, what, NULL );

}  // mbg_cond_err_msg



/*HDR*/
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
bool mbg_cond_err_msg_info( int rc, const char *what, const char *info )
{
  if ( mbg_rc_is_error( rc ) )
  {
    if ( info && ( rc == MBG_ERR_NOT_SUPP_BY_DEV ) )
      fprintf( stderr, "This device does not %s.\n", info );
    else
      fprintf( stderr, "** %s failed: %s (rc: %i)\n", what, mbg_strerror( rc ), rc );

    return true;
  }

  return false;

}  // mbg_cond_err_msg_info

#endif  // !defined( _MBG_TGT_OMIT_ERR_MSG )



#if defined( MBG_TGT_CVI )

/*HDR*/
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
int mbg_cvi_rs232_error_to_mbg( int cvi_rc, const char *info )
{
  #if DEBUG
    if ( info )
      fprintf( stderr, "%s, CVI RS-232 rc: %i\n", info, cvi_rc );
  #else
    (void) info;  // avoid compiler warning
  #endif

  return ( cvi_rc < 0 ) ? lookup_tbl_errno( cvi_rc, cvi_rs232_error_table, MBG_ERR_UNKNOWN ) : MBG_SUCCESS;

}  // mbg_cvi_rs232_error_to_mbg

#endif



#if defined( MBG_TGT_WIN32 )

#if defined( MBG_TGT_KERNEL )

/*HDR*/
/**
 * @brief Translate a Windows NTSTATUS code to one of the @ref MBG_ERROR_CODES
 *
 * @param[in] st    One of the NTSTATUS codes defined in ntstatus.h
 * @param[in] info  An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
int mbg_win32_ntstatus_to_mbg( NTSTATUS st, const char *info )
{
  #if DEBUG
    if ( info )
      _mbgddmsg_2( DEBUG, MBG_LOG_INFO, "%s, ntstatus: 0x%08lX\n", info, (long) st );
  #else
    (void) info;  // avoid compiler warning
  #endif

  return lookup_tbl_errno( st, win32_kernel_status_table, MBG_ERR_UNKNOWN );

}  // mbg_win32_ntstatus_to_mbg


#else  // Windows user space

/*HDR*/
/**
 * @brief Translate a Windows non-socket API return code to one of the @ref MBG_RETURN_CODES
 *
 * @param[in] win32_sys_rc  A Windows non-socket API error code as returned by GetLastError(), or ERROR_SUCCESS.
 * @param[in] info          An optional informational text string, or NULL.
 *
 * @return  One of the @ref MBG_RETURN_CODES
 */
int mbg_win32_sys_err_to_mbg( DWORD win32_sys_rc, const char *info )
{
  int rc = MBG_SUCCESS;

  if ( win32_sys_rc == ERROR_SUCCESS )
    goto out;

  if ( win32_sys_rc & STATUS_CUSTOM_FLAG_MASK )
  {
    // This is a user-defined error code or message ID, e.g. returned by an IOCTL
    // call. The lower 16 bits contain the (positive) error code while the upper
    // 16 bits contain flags as specified for the Windows API in winerror.h.
    // So we assume the error code is just the absolute value of one of the
    // @ref MBG_ERROR_CODES, and we return the negated value.
    rc = - (int) ( win32_sys_rc & 0xFFFF );
    goto out;
  }

#if 0  // TODO FIXME
  rc = (int) win32_sys_rc;

  if ( rc < 0 )
  {
    // If the error code is negative then this is a user-defined
    // error code e.g. returned by an IOCTL call. In this case we
    // assume it's one of the
    goto out;
  }
#endif

  rc = lookup_tbl_errno( win32_sys_rc, win32_sys_err_table, MBG_ERR_UNKNOWN );

out:
  #if DEBUG
    if ( info )
      fprintf( stderr, "%s, win32_sys_rc: 0x%08lX (%i) --> %i (%s)\n",
               info, (long) win32_sys_rc, (int) win32_sys_rc,
               rc, mbg_strerror( rc ) );
  #else
    (void) info;  // avoid compiler warning
  #endif

  return rc;

}  // mbg_win32_sys_err_to_mbg



/*HDR*/
/**
 * @brief Translate a Windows socket API error code to one of the @ref MBG_ERROR_CODES
 *
 * @param[in] wsa_err  A Windows socket API error code as returned by WSAGetLastError()
 * @param[in] info     An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
int mbg_win32_wsa_err_to_mbg( int wsa_err, const char *info )
{
  #if DEBUG
    if ( info )
      fprintf( stderr, "%s, wsa_err: %i\n", info, wsa_err );
  #else
    (void) info;  // avoid compiler warning
  #endif

  // The WSA error code is only retrieved after an error has occurred, so
  // we don't need care for the success case here.
  return lookup_tbl_errno( wsa_err, win32_wsa_err_table, MBG_ERR_UNKNOWN );

}  // mbg_win32_wsa_err_to_mbg

#endif  // Windows user space

#endif  // defined( MBG_TGT_WIN32 )




#if defined( _MBG_TGT_HAS_POSIX_ERRNO )

/*HDR*/
/**
 * @brief Translate a POSIX errno error code to one of the @ref MBG_ERROR_CODES
 *
 * @param[in] posix_errno  A POSIX error code as usually defined in errno.h
 * @param[in] info         An optional informational text string, or NULL
 *
 * @return  One of the @ref MBG_ERROR_CODES
 */
int mbg_posix_errno_to_mbg( int posix_errno, const char *info )
{
  #if DEBUG && !defined( MBG_TGT_KERNEL )
    if ( info )
      fprintf( stderr, "%s: %s (errno: %i)\n", info,
               strerror( posix_errno ), posix_errno );
  #else
    (void) info;  // avoid compiler warning
  #endif

  return lookup_tbl_errno( posix_errno, posix_errno_table, MBG_ERR_UNKNOWN );

}  // mbg_posix_errno_to_mbg

#endif  // defined( _MBG_TGT_HAS_POSIX_ERRNO )



#if defined( _MBG_TGT_HAS_POSIX_H_ERRNO )

/*HDR*/
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
int mbg_posix_h_errno_to_mbg( int posix_h_errno, const char *info )
{
  #if DEBUG && !defined( MBG_TGT_KERNEL )
    if ( info )
      fprintf( stderr, "%s: %s (h_errno: %i)\n", info,
               hstrerror( posix_h_errno ), posix_h_errno );
  #else
    (void) info;  // avoid compiler warning
  #endif

  return lookup_tbl_errno( posix_h_errno, posix_h_errno_table, MBG_ERR_UNKNOWN );

}  // mbg_posix_h_errno_to_mbg

#endif  // defined( _MBG_TGT_HAS_POSIX_H_ERRNO )



#if !defined( _MBG_TGT_OMIT_LAST_ERROR )

/*HDR*/
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
int mbg_get_last_error( const char *info )
{
  #if defined( MBG_TGT_WIN32 )

    // Under Windows the "last error" code after a non-socket function
    // ("Windows System Errors") has to be retrieved by calling GetLastError(),
    // whereas the "last error" code from POSIX-like socket functions
    // ("Windows Sockets Error Codes") has to be retrieved by calling
    // WSAGetLastError().
    return mbg_win32_sys_err_to_mbg( GetLastError(), info );

  #elif defined( MBG_TGT_POSIX )

    // On POSIX systems the "last error" code is always stored in errno.
    return mbg_posix_errno_to_mbg( errno, info );

  #else

    // ### TODO #error This function is not supported for this target.
    return mbg_posix_errno_to_mbg( errno, info );

  #endif

}  // mbg_get_last_error



#if !defined( _MBG_TGT_OMIT_SOCKET_ERRORS )

/*HDR*/
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
int mbg_get_last_socket_error( const char *info )
{
  #if defined( MBG_TGT_CVI )

    #warning This needs to be implemented for CVI
    return MBG_ERR_UNKNOWN;

  #elif defined( MBG_TGT_WIN32 )

    #if !defined( MBG_TGT_KERNEL )
      // Under Windows the "last error" code after a POSIX-like socket
      // function ("Windows Sockets Error Code") has to be retrieved
      // by calling WSAGetLastError(), whereas the "last error" code
      // after a non-socket function ("Windows System Errors") has
      // to be retrieved by calling GetLastError().
      return mbg_win32_wsa_err_to_mbg( WSAGetLastError(), info );
    #else
      return MBG_ERR_GENERIC;  // TODO should we only work with NTSTATUS codes in kernel mode?
    #endif

  #elif defined( MBG_TGT_POSIX )

    // On POSIX systems the "last error" code is always stored in errno.
    return mbg_posix_errno_to_mbg( errno, info );

  #else

    #error This function is not supported for this target.

  #endif

}  // mbg_get_last_socket_error



/*HDR*/
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
int mbg_get_gethostbyname_error( const char *info )
{
  #if defined( MBG_TGT_CVI )

    #warning This needs to be implemented for CVI
    return MBG_ERR_UNKNOWN;

  #elif defined( MBG_TGT_WIN32 )

    #if !defined( MBG_TGT_KERNEL )
      return mbg_win32_wsa_err_to_mbg( WSAGetLastError(), info );
    #else
      return MBG_ERR_GENERIC;
    #endif

  #elif defined( MBG_TGT_POSIX )

    return mbg_posix_h_errno_to_mbg( h_errno, info );

  #else

    #error This function is not supported for this target.

  #endif

}  // mbg_get_gethostbyname_error

#endif  // !defined( _MBG_TGT_OMIT_SOCKET_ERRORS )

#endif  // !defined( _MBG_TGT_OMIT_LAST_ERROR )



#if 0  // not yet finished
// error handler for getaddressinfo()
/*
 * Handle specific error returned by getaddressinfo()
 */
 /*HDR*/
int mbg_gai_error( int rc, const char *info )
{
  #if defined( MBG_TGT_CVI )

    #warning This needs to be implemented for CVI
    return MBG_ERR_UNKNOWN;

  #elif defined( MBG_TGT_WIN32 )

    return mbg_win32_wsa_err_to_mbg( WSAGetLastError(), info );

  #elif defined( MBG_TGT_POSIX )

    return mbg_posix_h_errno_to_mbg( h_errno, info );

  #else

    return MBG_ERR_UNKNOWN;

  #endif

}  // mbg_get_gai_error

#endif



#if defined( USE_MBG_ZLIB )

/*HDR*/
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
int mbg_zlib_error_to_mbg( int zlib_error, const char *info, const char *msg )
{
  #if DEBUG
    if ( info && msg )
      fprintf( stderr, "%s: %s (zlib_error: %d)\n", info,
               msg, zlib_error );
  #endif

  switch ( zlib_error )
  {
    case Z_ERRNO: return mbg_get_last_error( info );
    case Z_MEM_ERROR: return MBG_ERR_NO_MEM;
    /*
     * All other zlib error codes are not specified any further.
     * So, it's hard to guess what they mean and we return MBG_UNSPEC so far.
     */
    default:
      return MBG_ERR_UNKNOWN;
  }

}  // mbg_zlib_error_to_mbg

#endif // defined( USE_MBG_ZLIB )


