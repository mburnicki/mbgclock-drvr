
/**************************************************************************
 *
 *  $Id: pcpsdefs.h 1.65 2019/07/31 15:41:02 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    General definitions for Meinberg plug-in devices.
 *
 * -----------------------------------------------------------------------
 *  $Log: pcpsdefs.h $
 *  Revision 1.65  2019/07/31 15:41:02  martin
 *  Doxygen changes.
 *  Revision 1.64  2018/06/25 12:34:23  martin
 *  Fixed spelling inside a comment.
 *  Revision 1.63  2018/02/28 16:57:38Z  martin
 *  Replace references to frac_sec_from_bin() to bin_frac_32_to_dec_frac().
 *  Revision 1.62  2017/07/04 16:24:53  martin
 *  Moved definitions PCPS_HRT_FRAC_SCALE and PCPS_HRT_FRAC_SCALE_FMT
 *  back here.
 *  New types PCPS_SECONDS and PCPS_FRAC_32.
 *  Fixed typo, wording, and doxygen comments.
 *  Revision 1.61  2017/04/25 11:38:38  martin
 *  Renamed GRC181PEX to GNS181PEX.
 *  Revision 1.60  2017/03/17 12:00:05  martin
 *  Moved definitions of PCPS_HRT_FRAC_CONVERSION_TYPE,
 *  PCPS_HRT_BIN_FRAC_SCALE, and PCPS_HRT_FRAC_SCALE_FMT
 *  to cfg_hlp.h.
 *  Revision 1.59  2017/01/27 08:11:19  martin
 *  Fixed macro syntax.
 *  Revision 1.58  2016/11/08 16:42:52  martin
 *  New GPS cmd codes PC_GPS_XFEATURE_BUFFER and PC_GPS_TLV_INFO.
 *  Revision 1.57  2016/11/08 16:40:39  martin
 *  Doxygen cleanup.
 *  Revision 1.56  2016/10/26 13:22:41  martin
 *  Added definitions for GRC181PEX.
 *  New symbol IRIG_TIME_UNKNOWN_YEAR.
 *  Removed trailing spaces.
 *  Updated doxygen comments.
 *  Revision 1.55  2014/07/17 10:52:24  martin
 *  Increased safety of firmware builds.
 *  Revision 1.54  2014/07/17 09:54:19  martin
 *  New command codes PC_GPS_XMR_HOLDOVER_STATUS
 *  and PC_GPS_ALL_GPIO_STATUS.
 *  Huge update and cleanup on doxygen comments.
 *  Revision 1.53  2014/05/27 10:13:20  martin
 *  Support GPS180AMC.
 *  Moved some signal constant definitions to pcpsdefs.h.
 *  Simplified declaration of code/name tables.
 *  Huge rework of comments in doxygen format.
 *  Revision 1.52  2013/09/26 09:02:52Z  martin
 *  Support GNSS API.
 *  Updated doxygen comments.
 *  Revision 1.51  2013/06/25 09:51:39  martin
 *  Support GLN180PEX.
 *  Revision 1.50  2013/01/30 15:59:54  martin
 *  Updated and fixed some doxygen comments.
 *  Revision 1.49  2012/10/02 18:53:02  martin
 *  Added structure PCPS_TIME_STATUS_X_MASKS.
 *  Added initializer for command names, useful for debugging.
 *  Revision 1.48  2011/11/25 15:02:28  martin
 *  Support on-board event logs.
 *  Revision 1.47  2011/11/25 10:22:44  martin
 *  Modified handling of pragma pack().
 *  Made command group codes obsolete. They are still supported
 *  when building firmware, though.
 *  Support PTP unicast configuration.
 *  Support GPIO configuration.
 *  Support PZF180PEX.
 *  Added commands to read CORR_INFO, read/write TR_DISTANCE,
 *  PCPS_SYNC_PZF status, and associated structures.
 *  Added an initializer for a table of GPS command code/names.
 *  Added definitions MBG_PCPS_FMT_STATUS.
 *  Updated some comments.
 *  Revision 1.46  2011/01/13 11:44:29Z  martin
 *  Moved status port register definitions here.
 *  Revision 1.45  2010/09/06 07:36:24  martin
 *  Support GPS180PEX and TCR180PEX.
 *  Moved some IRIG related definitions to gpsdefs.h.
 *  Revision 1.44  2010/06/30 11:09:49  martin
 *  Added definitions for JJY longwave transmitter.
 *  Renamed MBG_RAW_IRIG_DATA::data field to data_bytes
 *  since "data" is a reserved word for C51 architecture.
 *  Revision 1.43  2010/02/09 11:20:17Z  martin
 *  Renamed yet unused CORR_INFO::flags field to signal and updated comments.
 *  Revision 1.42  2010/01/12 14:02:37  daniel
 *  Added definitions to support reading the raw IRIG data bits.
 *  Revision 1.41  2009/06/19 12:16:42Z  martin
 *  Added PCPS_GIVE_IRIG_TIME command and associated definitions.
 *  Revision 1.40  2009/06/08 19:29:11  daniel
 *  Support PTP configuration.
 *  Support LAN_IF configuration
 *  Added definition of PCPS_CMD_INFO.
 *  Revision 1.39  2009/03/19 08:58:09  martin
 *  Added PCPS_GET_IRIG_CTRL_BITS cmd and associated data type.
 *  Revision 1.38  2009/03/10 17:07:09  martin
 *  Support configurable time scales and GPS UTC parameters.
 *  Added ext. status flag for time scales, and PCPS_LS_ANN_NEG.
 *  Added bit mask PCPS_SCALE_MASK.
 *  Revision 1.37  2008/12/05 16:01:37Z  martin
 *  Added ref types PTP, FRC, and WWVB.
 *  Added ref names MSF, PTP, FRC, and WWVB.
 *  Added device codes TCR170PEX, PTP270PEX, and FRC511PEX.
 *  Added macros to convert the endianess of structures.
 *  Moved definitions of PCPS_HRT_FRAC_SCALE and
 *  PCPS_HRT_FRAC_SCALE_FMT here.
 *  Added definitions of PCPS_HRT_FRAC_CONVERSION_TYPE
 *  and PCPS_HRT_BIN_FRAC_SCALE.
 *  Escaped '<' and '>' characters for doxygen.
 *  Modified comments for PCPS_TZDL.
 *  Removed trailing spaces and obsolete comments.
 *  Revision 1.36  2008/01/17 09:20:25Z  daniel
 *  Added new REF type PCPS_REF_MSF.
 *  Revision 1.35  2008/01/17 09:18:46Z  daniel
 *  Made comments compatible for doxygen parser.
 *  No sourcecode changes.
 *  Revision 1.34  2007/07/17 08:22:47Z  martin
 *  Added support for TCR511PEX and GPS170PEX.
 *  Revision 1.33  2007/05/20 21:39:51Z  martin
 *  Added support for PEX511.
 *  Added PCPS_GET_STATUS_PORT cmd code for devices
 *  that do not support a hardware status port.
 *  Revision 1.32  2007/03/29 12:57:32Z  martin
 *  Renamed some TZCODE numbers for unique naming conventions.
 *  Added definitions of the older symbols for compatibility.
 *  Revision 1.31  2007/03/26 15:42:31Z  martin
 *  Replaced PCPS_REF_OFFS and associated definitions by MBG_REF_OFFS, etc.,
 *  which are defined in gpsdefs.h.
 *  Added PCPS_GET_DEBUG_STATUS code.
 *  Revision 1.30  2006/06/29 10:13:13  martin
 *  Added some descriptive comments.
 *  Revision 1.29  2006/06/14 12:59:12Z  martin
 *  Added support for TCR511PCI.
 *  Revision 1.28  2006/05/18 09:45:16  martin
 *  Added data types used with PZF receivers.
 *  Revision 1.27  2006/05/03 10:19:14Z  martin
 *  Added initializers for reference source names.
 *  Revision 1.26  2006/03/10 10:24:45Z  martin
 *  New definitions for PCI511.
 *  Added command codes to configure programmable pulse outputs.
 *  Revision 1.25  2005/11/03 15:05:16Z  martin
 *  New definitions for GPS170PCI.
 *  New types PCPS_TIME_STATUS and PCPS_TIME_STATUS_X.
 *  Removed obsolete enumeration of PCPS_TIME fields.
 *  Revision 1.24  2005/05/03 07:56:55Z  martin
 *  Added command PCPS_GET_SYNTH_STATE.
 *  Revision 1.23  2005/03/29 12:51:10Z  martin
 *  New cmd code PCPS_GENERIC_IO.
 *  Revision 1.22  2004/12/09 11:03:37Z  martin
 *  Support configuration of on-board frequency synthesizer.
 *  Revision 1.21  2004/11/09 12:55:32Z  martin
 *  Redefined interface data types using C99 fixed-size definitions.
 *  Added workaround macros for some structure sizes because the C166
 *  compiler always reports an even structure size even if the structure
 *  size is in fact odd, which might lead to different sizes in C166 and
 *  other environments.
 *  Modifications were required in order to be able to configure IRIG
 *  settings of cards which provide both IRIG input and output.
 *  The existing codes have been renamed with .._RX.. and are used to
 *  configure the IRIG receiver (input). New codes have been defined
 *  used to configure the IRIG transmitter.
 *  Renamed PC_GPS_STAT to PC_GPS_BVAR_STAT.
 *  Use more specific data types than generic types.
 *  Revision 1.20  2004/10/14 15:01:23  martin
 *  Added support for TCR167PCI.
 *  Revision 1.19  2004/06/16 12:46:33Z  martin
 *  Moved OPT_SETTINGS related definitions to gpsdefs.h,
 *  and renamed symbols from PCPS_.. to to MBG_...
 *  Revision 1.18  2004/04/26 14:27:08Z  martin
 *  Added union PCPS_TIME_UNION.
 *  Revision 1.17  2003/05/27 08:50:35Z  MARTIN
 *  New commands PCPS_GIVE_UCAP_ENTRIES, PCPS_GIVE_UCAP_EVENT
 *  and associated definitions which allow faster reading of
 *  user capture events and monitoring of the capture buffer
 *  fill level.
 *  Revision 1.16  2003/04/03 10:48:53  martin
 *  Support for PCI510, GPS169PCI, and TCR510PCI.
 *  New codes PCPS_GET_REF_OFFS, PCPS_SET_REF_OFFS
 *  and related structures.
 *  New codes PCPS_GET_OPT_INFO, PCPS_SET_OPT_SETTINGS
 *  and related structures.
 *  New codes PCPS_GET_IRIG_INFO, PCPS_SET_IRIG_SETTINGS.
 *  Preliminary PCPS_TZDL structure and cmd codes
 *  to read/write that structure.
 *  Revision 1.15  2002/08/08 13:24:03  MARTIN
 *  Moved definition of ref time sources here.
 *  Added new ref time source IRIG.
 *  Added new cmd to clear time capture buffer.
 *  Fixed some comments.
 *  Revision 1.14  2002/01/31 13:39:38  MARTIN
 *  Added new GPS data type codes for RECEIVER_INFO, etc.
 *  New PCPS_HR_TIME status flag PCPS_IO_BLOCKED.
 *  Moved REV_NUMs defining special features to pcpsdev.h.
 *  Removed obsolete initializer for framing string table.
 *  Updated some comments.
 *  Removed obsolete code.
 *  Revision 1.13  2001/12/03 16:15:14  martin
 *  Introduced PCPS_TIME_STAMP which allows to handle high precision
 *  time stamps.
 *  Replaced the sec/frac fields in PCPS_HR_TIME by PCPS_TIME_STAMP.
 *  This is compatible on byte level but may require source code
 *  modifications.
 *  Introduced new command PCPS_SET_EVENT_TIME which is used
 *  EXCLUSIVELY with a custom GPS firmware.
 *  Revision 1.12  2001/10/16 10:07:42  MARTIN
 *  Defined PCI509 firmware revision number which supports
 *  baud rate higher than standard.
 *  Revision 1.11  2001/03/30 13:02:39  MARTIN
 *  Control alignment of structures from new file use_pack.h.
 *  Defined initializers with valid framing parameters.
 *  Revision 1.10  2001/02/28 15:39:25  MARTIN
 *  Modified preprocessor syntax.
 *  Revision 1.9  2001/02/16 11:32:05  MARTIN
 *  Renamed "PROM" or "EPROM" in comments or and names to
 *  "FW" or firmware.
 *  This includes the cmd codes PCPS_GIVE_PROM_ID_... which have
 *  been renamed to PCPS_GIVE_FW_ID_...
 *  Renamed structure PCPS_TIME_SET to PCPS_STIME.
 *  Renamed return code PCPS_ERR_NONE to PCPS_SUCCESS.
 *  Modified some comments.
 *  Revision 1.8  2000/10/11 09:17:09  MARTIN
 *  Cleaned up comment syntax.
 *  Revision 1.7  2000/07/21 14:16:30  MARTIN
 *  Modified some comments.
 *  Added PCI definitions.
 *  Renamed PCPS_GET_GPS_DATA to PCPS_READ_GPS_DATA.
 *  Renamed PCPS_SET_GPS_DATA to PCPS_WRITE_GPS_DATA.
 *  New types PCPS_SERIAL and PCPS_TZCODE.
 *  Removed PCPS_SERIAL_BYTES and PCPS_TZCODE_BYTES, may use sizeof()
 *  the types instead.
 *  New type PCPS_TIME_SET which can be used to write date and time
 *  to the clock.
 *  Revision 1.6  2000/06/07 12:09:31  MARTIN
 *  renamed PCPS_SERIAL_GROUP to PCPS_CFG_GROUP
 *  renamed PCPS_ERR_SERIAL to PCPS_ERR_CFG
 *  modified definitions for baud rate, framing, and mode
 *  added PCPS_SN_... definitions
 *  added PCPS_GET_TZCODE and PCPS_SET_TZCODE definitions
 *  added PC_GPS_ANT_CABLE_LEN definition
 *  added RCS keywords
 *  updated some comments
 *
 * -----------------------------------------------------------------------
 *  Changes before put under RCS control:
 *
 *  Revision 1.5  2000/03/24
 *    Introduced PCPS_GIVE_SERNUM.
 *    Cleaned up for definitions for serial parameter byte.
 *    Reviewed and updated comments.
 *
 *  1998/07/22
 *    Introduced PCPS_HR_TIME.
 *    Rearranged order of definitions.
 *    Reviewed and updated comments.
 *
 *  1997/06/12
 *    Added GPS definitions.
 *
 *  1996/01/25
 *    PCPS_TIME redefined from an array of bytes to a structure.
 *
 **************************************************************************/

#ifndef _PCPSDEFS_H
#define _PCPSDEFS_H


/* Other headers to be included */

#include <words.h>
#include <use_pack.h>

#ifndef _USE_PCPSPRIV
  #define _USE_PCPSPRIV  _IS_MBG_FIRMWARE
#endif

#if _USE_PCPSPRIV
  #include <pcpspriv.h>
#endif

/* Start of header body */

#if defined( _USE_PACK )
  #pragma pack( 1 )      // set byte alignment
  #define _USING_BYTE_ALIGNMENT
#endif


/**
 * @brief Enumeration of the ref time signal sources used by Meinberg devices
 */
enum PCPS_REF_TYPES
{
  PCPS_REF_NONE,   ///< unknown, or not defined
  PCPS_REF_DCF,    ///< DCF77 long wave signal (Germany), see http://www.meinberg.de/english/info/dcf77.htm
  PCPS_REF_GPS,    ///< GPS satellite system, see http://www.meinberg.de/english/info/gps.htm
  PCPS_REF_IRIG,   ///< IRIG or similar time code, see http://www.meinberg.de/english/info/irig.htm
  PCPS_REF_MSF,    ///< MSF long wave signal (UK)
  PCPS_REF_PTP,    ///< PTP/IEEE1588 network protocol
  PCPS_REF_FRC,    ///< Free Running Clock
  PCPS_REF_WWVB,   ///< WWVB long wave signal (U.S.)
  PCPS_REF_JJY,    ///< JJY long wave signal (Japan)
  N_PCPS_REF       ///< number of defined ref time sources
};


/**
 * @defgroup group_pcps_ref_type_names Reference type names
 *
 * @see ::PCPS_REF_TYPES
 *
 * @{ */

#define PCPS_REF_NAME_NONE_ENG  "unknown"
#define PCPS_REF_NAME_NONE_GER  "nicht bekannt"
#define PCPS_REF_NAME_DCF       "DCF77"
#define PCPS_REF_NAME_GPS       "GPS"
#define PCPS_REF_NAME_IRIG      "IRIG"
#define PCPS_REF_NAME_MSF       "MSF"
#define PCPS_REF_NAME_PTP       "PTP"
#define PCPS_REF_NAME_FRC       "FRC"
#define PCPS_REF_NAME_WWVB      "WWVB"
#define PCPS_REF_NAME_JJY       "JJY"

/** @}  @defgroup group_pcps_ref_type_names */


/**
 * @brief Initializer for an array of English reference type names
 *
 * @see ::PCPS_REF_TYPES
 */
#define PCPS_REF_NAMES_ENG \
{                          \
  PCPS_REF_NAME_NONE_ENG,  \
  PCPS_REF_NAME_DCF,       \
  PCPS_REF_NAME_GPS,       \
  PCPS_REF_NAME_IRIG,      \
  PCPS_REF_NAME_MSF,       \
  PCPS_REF_NAME_PTP,       \
  PCPS_REF_NAME_FRC,       \
  PCPS_REF_NAME_WWVB,      \
  PCPS_REF_NAME_JJY        \
}


/**
 * @brief Initializer for a multi-language array of reference type names
 *
 * @see ::PCPS_REF_TYPES
 */
#define PCPS_REF_NAMES_LSTR                            \
{                                                      \
  { PCPS_REF_NAME_NONE_ENG, PCPS_REF_NAME_NONE_GER },  \
  { PCPS_REF_NAME_DCF, NULL },                         \
  { PCPS_REF_NAME_GPS, NULL },                         \
  { PCPS_REF_NAME_IRIG, NULL },                        \
  { PCPS_REF_NAME_MSF, NULL },                         \
  { PCPS_REF_NAME_PTP, NULL },                         \
  { PCPS_REF_NAME_FRC, NULL },                         \
  { PCPS_REF_NAME_WWVB, NULL },                        \
  { PCPS_REF_NAME_JJY, NULL }                          \
}



/**
 * @brief Meinberg PCI vendor ID (assigned by the PCI SIG)
 *
 * @see @ref MEINBERG_PCI_DEVICE_IDS
 */
#define PCI_VENDOR_MEINBERG     0x1360


/**
 * @brief PCI device IDs assigned by Meinberg
 *
 * High byte:  type of ref time source, see ::PCPS_REF_TYPES
 * Low Byte:   enumeration of device types
 *
 * @see ::PCI_VENDOR_MEINBERG
 *
 * @anchor MEINBERG_PCI_DEVICE_IDS @{ */

#define PCI_DEV_PCI32           ( ( PCPS_REF_DCF << 8 )  | 0x01 )
#define PCI_DEV_PCI509          ( ( PCPS_REF_DCF << 8 )  | 0x02 )
#define PCI_DEV_PCI510          ( ( PCPS_REF_DCF << 8 )  | 0x03 )
#define PCI_DEV_PCI511          ( ( PCPS_REF_DCF << 8 )  | 0x04 )
#define PCI_DEV_PEX511          ( ( PCPS_REF_DCF << 8 )  | 0x05 )
#define PCI_DEV_PZF180PEX       ( ( PCPS_REF_DCF << 8 )  | 0x06 )

#define PCI_DEV_GPS167PCI       ( ( PCPS_REF_GPS << 8 )  | 0x01 )
#define PCI_DEV_GPS168PCI       ( ( PCPS_REF_GPS << 8 )  | 0x02 )
#define PCI_DEV_GPS169PCI       ( ( PCPS_REF_GPS << 8 )  | 0x03 )
#define PCI_DEV_GPS170PCI       ( ( PCPS_REF_GPS << 8 )  | 0x04 )
#define PCI_DEV_GPS170PEX       ( ( PCPS_REF_GPS << 8 )  | 0x05 )
#define PCI_DEV_GPS180PEX       ( ( PCPS_REF_GPS << 8 )  | 0x06 )
#define PCI_DEV_GLN180PEX       ( ( PCPS_REF_GPS << 8 )  | 0x07 )
#define PCI_DEV_GPS180AMC       ( ( PCPS_REF_GPS << 8 )  | 0x08 )
#define PCI_DEV_GNS181PEX       ( ( PCPS_REF_GPS << 8 )  | 0x09 )

#define PCI_DEV_TCR510PCI       ( ( PCPS_REF_IRIG << 8 ) | 0x01 )
#define PCI_DEV_TCR167PCI       ( ( PCPS_REF_IRIG << 8 ) | 0x02 )
#define PCI_DEV_TCR511PCI       ( ( PCPS_REF_IRIG << 8 ) | 0x03 )
#define PCI_DEV_TCR511PEX       ( ( PCPS_REF_IRIG << 8 ) | 0x04 )
#define PCI_DEV_TCR170PEX       ( ( PCPS_REF_IRIG << 8 ) | 0x05 )
#define PCI_DEV_TCR180PEX       ( ( PCPS_REF_IRIG << 8 ) | 0x06 )

#define PCI_DEV_PTP270PEX       ( ( PCPS_REF_PTP  << 8 ) | 0x01 )

#define PCI_DEV_FRC511PEX       ( ( PCPS_REF_FRC  << 8 ) | 0x01 )

/** @} anchor MEINBERG_PCI_DEVICE_IDS */



/**
 * @defgroup group_status_port Definitions used with the status port
 *
 * The status port register on bus-level cards reflects some hardware
 * signals (e.g. DCF-77 modulation), and flags used for communication
 * with the card (e.g. the BUSY flag, ::PCPS_ST_BUSY).
 *
 * @note Must not be confused with ::PCPS_TIME_STATUS which returns
 * the synchronization status and associated information
 *
 * @{ */

/**
 * @brief Type of the status register port
 */
typedef uint8_t PCPS_STATUS_PORT;  ///< see @ref PCPS_STATUS_PORT_BIT_MASKS


/**
 * @brief Bit masks used with ::PCPS_STATUS_PORT
 *
 * The flags ::PCPS_ST_SEC and ::PCPS_ST_MIN are cleared whenever the clock
 * is read, so they are not very reliable in multitasking environments
 * and thus should be considered as deprecated.
 *
 * The ::PCPS_ST_IRQF flag was used with old ISA cards to check
 * if the device has generated an IRQ.
 * Some PCI cards also support this, but in case of PCI cards the
 * associated flag of the PCI interface chip should be checked to see
 * if a particular card has generated an IRQ on the PC bus.
 *
 * The macro ::_pcps_ddev_has_gen_irq cares about this and should be used
 * to determine in a portable way whether a card has generated an IRQ.
 *
 * @anchor PCPS_STATUS_PORT_BIT_MASKS @{ */

#define PCPS_ST_BUSY  0x01  ///< the clock is busy filling the output FIFO
#define PCPS_ST_IRQF  0x02  ///< the clock has generated an IRQ on the PC bus (ISA cards only)
#define PCPS_ST_MOD   0x20  ///< the raw demodulated DCF77 signal
#define PCPS_ST_SEC   0x40  ///< seconds have changed since last reading
#define PCPS_ST_MIN   0x80  ///< minutes have changed since last reading

/** @} anchor PCPS_STATUS_PORT_BIT_MASKS */

/** @} defgroup group_status_port */


/**
 * A format string to be used with snprintb() which is available on some Unix
 * systems to print information held in a bit coded variable.
 */
#define MBG_PCPS_FMT_STATUS \
  "\177\20b\0FREER\0b\1DL_ENB\0b\2SYNCD\0b\3DL_ANN\0b\4UTC\0b\5LS_ANN\0b\6IFTM\0b\7INVT" \
  "\0b\x08LS_ENB\0b\11ANT_FAIL\0b\x0aLS_ANN_NEG\0b\x0bSCALE_GPS\0b\x0cSCALE_TAI\0\0"



/**
 * @brief Command codes used to communicate with bus level devices
 *
 * These commands are used for low level access to bus-level devices
 * manufactured by Meinberg.
 *
 * Applications should instead use the API functions declared in mbgdevio.h.
 *
 * The header files pcpsdev.h and pcpsdrvr.h contain macros which can be
 * used to check if a detected device supports a certain feature or command.
 * If checking is required then the name of the macro is given in the
 * comments associated with the command codes.
 *
 * Some commands expect parameters to be passed to the board. In that
 * case, the board returns the number of parameter bytes expected when
 * the command code is passed. Every parameter byte has to be supplied
 * to the board exactly like a command byte.
 *
 * Refer to function ::pcps_write and the macro ::_pcps_write_var
 * for details.
 *
 *  - ::PCPS_GIVE_TIME<br>
 *    Return a ::PCPS_TIME structure with current date,
 *    time and status. Supported by all clocks.
 *
 *  - ::PCPS_GIVE_TIME_NOCLEAR<br>
 *    Same as ::PCPS_GIVE_TIME but the bits ::PCPS_ST_SEC
 *    and ::PCPS_ST_MIN of the status port are not cleared.
 *    Supported by all clocks except PC31/PS31 with
 *    firmware version older than v3.0.
 *    This is mainly used by the DOS TSR and should
 *    not be used in other environments.
 *
 *  - ::PCPS_GIVE_SYNC_TIME<br>
 *    Return a ::PCPS_TIME structure with date and time
 *    of last synchronization of the clock or
 *    the last time set via the interface.
 *    ::_pcps_has_sync_time checks whether supported.
 *
 *  - ::PCPS_GIVE_HR_TIME<br>
 *    Return a ::PCPS_HR_TIME structure with current
 *    date, time and status. This command should be
 *    used to read the clock with higher resolution.
 *    ::_pcps_has_hr_time checks whether supported.
 *
 *  - ::PCPS_GIVE_IRIG_TIME<br>
 *    Return a ::PCPS_IRIG_TIME structure with day-of-year,
 *    time and status as decoded from the IRIG signal.
 *    ::_pcps_has_irig_time checks whether supported.
 *
 *  - ::PCPS_SET_TIME<br>
 *    Set the board date, time and status. This command
 *    expects sizeof( ::PCPS_STIME ) parameter bytes.
 *    ::_pcps_can_set_time checks whether supported.
 *
 *  - ::PCPS_SET_EVENT_TIME<br>
 *    Send a high resolution time stamp to the clock to
 *    configure a %UTC time when the clock shall generate
 *    some event. This command expects a ::PCPS_TIME_STAMP
 *    parameter.
 *    ::_pcps_has_event_time checks whether supported.
 *    (requires custom GPS CERN firmware)
 *
 *  - ::PCPS_IRQ_NONE<br>
 *    Disable the card's hardware IRQ<br>
 *  - ::PCPS_IRQ_1_SEC<br>
 *    Enable hardware IRQs once per second<br>
 *  - ::PCPS_IRQ_1_MIN<br>
 *    Enable hardware IRQs once per minute (deprecated)<br>
 *  - ::PCPS_IRQ_10_MIN<br>
 *    Enable hardware IRQs once per 10 minutes (deprecated)<br>
 *  - ::PCPS_IRQ_30_MIN<br>
 *    Enable hardware IRQs once per 30 minutes (deprecated)<br>
 *
 *  - ::PCPS_GET_SERIAL<br>
 *    ::PCPS_SET_SERIAL<br>
 *    Deprecated. Read or write the configuration of a card's
 *    serial port COM0 in ::PCPS_SERIAL format.
 *    ::_pcps_has_serial checks whether supported.
 *    Newer cards should be configured using the structures ::RECEIVER_INFO,
 *    ::PORT_INFO, and STR_TYPE_INFO.
 *    ::_pcps_has_receiver_info checks whether these are supported.
 *
 *  - ::PCPS_GET_TZCODE<br>
 *    ::PCPS_SET_TZCODE<br>
 *    These commands read or set a DCF77 clock's
 *    time zone code and should be used preferably
 *    with older DCF77 receivers which have limited
 *    support of different time zones.
 *    ::_pcps_has_tzcode checks whether supported.
 *    Most newer devices support the ::TZDL structure which can be
 *    read or written using ::PC_GPS_TZDL.
 *
 *  - ::PCPS_GET_PCPS_TZDL<br>
 *    ::PCPS_SET_PCPS_TZDL<br>
 *    Read or write time zone / daylight saving information
 *    in ::PCPS_TZDL format.
 *    ::_pcps_has_pcps_tzdl checks whether supported.
 *
 *  - ::PCPS_GET_REF_OFFS<br>
 *    ::PCPS_SET_REF_OFFS<br>
 *    These commands can be used to configure the
 *    reference time offset from %UTC for clocks
 *    which can't determine the offset automatically,
 *    e.g. from an IRIG input signal.
 *    ::_pcps_has_ref_offs checks whether supported.
 *
 *  - ::PCPS_GET_OPT_INFO<br>
 *    ::PCPS_SET_OPT_SETTINGS<br>
 *    These commands can be used to configure some
 *    optional settings, controlled by flags.
 *    When reading, the clock returns a ::MBG_OPT_INFO
 *    structure which contains the supported values,
 *    plus the current settings.
 *    When writing, clocks accepts a ::MBG_OPT_SETTINGS
 *    structure only which contain the desired settings
 *    of the supported flags only.
 *    ::_pcps_has_opt_flags checks whether supported.
 *
 *  - ::PCPS_GET_IRIG_RX_INFO<br>
 *    ::PCPS_SET_IRIG_RX_SETTINGS<br>
 *    ::PCPS_GET_IRIG_TX_INFO<br>
 *    ::PCPS_SET_IRIG_TX_SETTINGS<br>
 *    These commands can be used to configure IRIG
 *    inputs and outputs.<br>
 *    When reading, the clock returns an ::IRIG_INFO
 *    structure which contains the supported values,
 *    plus the current settings.<br>
 *    When writing, clocks accepts an ::IRIG_SETTINGS
 *    structure only which contain the desired settings
 *    only. ::_pcps_is_irig_rx and ::_pcps_has_irig_tx
 *    check whether supported.
 *
 *  - ::PCPS_GET_IRIG_CTRL_BITS<br>
 *    This command can be used to retrieve the control function
 *    bits of the latest IRIG input frame. Those bits may carry
 *    some well-known information as in the IEEE 1344 code, but
 *    may also contain some customized information, depending on
 *    the IRIG frame type and the configuration of the IRIG generator.
 *    So these bits are returned as-is and must be interpreted
 *    by the application.
 *    ::_pcps_has_irig_ctrl_bits checks whether supported.
 *
 *  - ::PCPS_GET_SYNTH<br>
 *    ::PCPS_SET_SYNTH<br>
 *    ::PCPS_GET_SYNTH_STATE<br>
 *     These commands can be used to configure an on-board
 *     frequency synthesizer and query the synthesizer
 *     status. The commands are only supported if the board
 *     supports the ::RECEIVER_INFO structure and the flag
 *     #GPS_HAS_SYNTH is set in the ::RECEIVER_INFO::features.
 *     ::_pcps_has_synth checks whether supported.
 *     The structures ::SYNTH and ::SYNTH_STATE used with these
 *     commands are defined in gpsdefs.h.
 *
 *  - ::PCPS_GIVE_FW_ID_1<br>
 *    ::PCPS_GIVE_FW_ID_2<br>
 *    Returns the first/second block of ::PCPS_FIFO_SIZE
 *    characters of the firmware ID string. These
 *    commands can be used to check if the board
 *    responds properly. This is done by the clock
 *    detection functions.
 *
 *  - ::PCPS_GIVE_SERNUM<br>
 *    Returns ::PCPS_FIFO_SIZE characters of the
 *    clock's serial number.
 *    ::_pcps_has_sernum checks whether supported.
 *
 *  - ::PCPS_GENERIC_IO<br>
 *    Generic I/O read and write. Can be used to query
 *    specific data, e.g. a selected element of an array.
 *    ::_pcps_has_generic_io checks whether supported.
 *
 *  - ::PCPS_GET_DEBUG_STATUS<br>
 *    This command reads an ::MBG_DEBUG_STATUS structure
 *    which represents the internal status of the
 *    IRIG decoder and some additional debug info.
 *    ::_pcps_has_debug_status checks whether supported.
 *
 *  - ::PCPS_READ_GPS_DATA<br>
 *    ::PCPS_WRITE_GPS_DATA<br>
 *    These commands are used by the functions
 *    ::pcps_read_gps and ::pcps_write_gps
 *    to read or write large data structures to
 *    Meinberg GPS plug-in clocks.
 *    ::_pcps_is_gps checks whether supported.
 *
 *  - ::PCPS_CLR_UCAP_BUFF<br>
 *    Clear a clock's time capture buffer.
 *    ::_pcps_can_clr_ucap_buff checks whether
 *    supported.
 *
 *  - ::PCPS_GIVE_UCAP_ENTRIES<br>
 *    Read a ::PCPS_UCAP_ENTRIES structure which
 *    reports the max number of entries and the
 *    currently used number of entries in the
 *    user capture buffer.
 *    ::_pcps_has_ucap checks whether supported.
 *
 *  - ::PCPS_GIVE_UCAP_EVENT<br>
 *    Read capture events using a PCPS_HR_TIME
 *    structure. This is faster than reading using the
 *    GPS command ::PC_GPS_UCAP. If no capture event is
 *    available then the returned structure is all 0.
 *    ::_pcps_has_ucap checks whether supported.
 *
 *  - ::PCPS_GET_CORR_INFO<br>
 *    Read PZF correlation info using a ::CORR_INFO
 *    structure.
 *    ::_pcps_has_pzf checks whether supported.
 *
 *  - ::PCPS_GET_TR_DISTANCE<br>
 *    ::PCPS_SET_TR_DISTANCE<br>
 *    Read or write distance from the RF transmitter.
 *    This is used to compensate the RF propagation delay
 *    for PZF receivers.
 *    ::_pcps_has_tr_distance checks whether supported.
 *
 *  - ::PCPS_CLR_EVT_LOG<br>
 *    Clear on-board event log.
 *    ::_pcps_has_evt_log checks whether supported.
 *
 *  - ::PCPS_NUM_EVT_LOG_ENTRIES<br>
 *    Read max. number of num event log entries which can
 *    be saved on the board, and how many entries have currently
 *    been saved.
 *    ::_pcps_has_evt_log checks whether supported.
 *
 *  - ::PCPS_FIRST_EVT_LOG_ENTRY<br>
 *    ::PCPS_NEXT_EVT_LOG_ENTRY<br>
 *    Read first (oldest) or next event log entry.
 *    ::_pcps_has_evt_log checks whether supported.
 *
 *  - ::PCPS_FORCE_RESET<br>
 *    Resets the card's hardware. This may lock up the computer
 *    and thus should only be used by very specific applications.
 *
 * @anchor PCPS_CMD_CODES @{ */

#define PCPS_GIVE_TIME            0x00  ///< (r-) Read current time in ::PCPS_TIME format
#define PCPS_GIVE_TIME_NOCLEAR    0x01  ///< (r-) Read current time in ::PCPS_TIME format, don't clear sec and min flags (deprecated)
#define PCPS_GIVE_SYNC_TIME       0x02  ///< (r-) Read last sync time as ::PCPS_TIME, only if ::_pcps_has_sync_time
#define PCPS_GIVE_HR_TIME         0x03  ///< (r-) Read high res. time as ::PCPS_HR_TIME, only if ::_pcps_has_hr_time
#define PCPS_GIVE_IRIG_TIME       0x04  ///< (r-) Read raw IRIG time as ::PCPS_IRIG_TIME, only if ::_pcps_has_irig_time

#define PCPS_SET_TIME             0x10  ///< (-w) Set on-board time, see ::PCPS_STIME. Returns ::MBG_ERR_STIME on error.

#define PCPS_SET_EVENT_TIME       0x14  ///< (-w) Write event time as ::PCPS_TIME_STAMP, only if ::_pcps_has_event_time

#define PCPS_IRQ_NONE             0x20  ///< (-w) Disable IRQs
#define PCPS_IRQ_1_SEC            0x21  ///< (-w) Enable IRQ per 1 second
#define PCPS_IRQ_1_MIN            0x22  ///< (-w) Enable IRQ per 1 minute (deprecated)
#define PCPS_IRQ_10_MIN           0x24  ///< (-w) Enable IRQ per 10 minutes (deprecated)
#define PCPS_IRQ_30_MIN           0x28  ///< (-w) Enable IRQ per 10 minutes (deprecated)

#define PCPS_GET_SERIAL           0x30  ///< (r-) Read serial settings as ::PCPS_SERIAL, deprecated by ::PC_GPS_ALL_PORT_INFO
#define PCPS_SET_SERIAL           0x31  ///< (-w) Write serial settings as ::PCPS_SERIAL, deprecated by ::PC_GPS_PORT_SETTINGS_IDX, returns ::MBG_ERR_CFG on error

#define PCPS_GET_TZCODE           0x32  ///< (r-) Read ::PCPS_TZCODE, only if ::_pcps_has_tzcode
#define PCPS_SET_TZCODE           0x33  ///< (-w) Write ::PCPS_TZCODE, only if ::_pcps_has_tzcode, returns ::MBG_ERR_CFG on error

#define PCPS_GET_PCPS_TZDL        0x34  ///< (r-) Read ::PCPS_TZDL, only if ::_pcps_has_pcps_tzdl
#define PCPS_SET_PCPS_TZDL        0x35  ///< (-w) Write  ::PCPS_TZDL, only if ::_pcps_has_pcps_tzdl, returns ::MBG_ERR_CFG on error

#define PCPS_GET_REF_OFFS         0x36  ///< (r-) Read ::MBG_REF_OFFS, only if ::_pcps_has_ref_offs
#define PCPS_SET_REF_OFFS         0x37  ///< (-w) Write ::MBG_REF_OFFS, only if ::_pcps_has_ref_offs, returns ::MBG_ERR_CFG on error

#define PCPS_GET_OPT_INFO         0x38  ///< (r-) Read ::MBG_OPT_INFO, only if ::_pcps_has_opt_flags
#define PCPS_SET_OPT_SETTINGS     0x39  ///< (-w) Write ::MBG_OPT_SETTINGS, only if ::_pcps_has_opt_flags, returns ::MBG_ERR_CFG on error

#define PCPS_GET_IRIG_RX_INFO     0x3A  ///< (r-) Read ::IRIG_INFO, only if ::_pcps_is_irig_rx
#define PCPS_SET_IRIG_RX_SETTINGS 0x3B  ///< (-w) Write ::IRIG_SETTINGS, only if ::_pcps_is_irig_rx, returns ::MBG_ERR_CFG on error

#define PCPS_GET_IRIG_TX_INFO     0x3C  ///< (r-) Read ::IRIG_INFO, only if ::_pcps_has_irig_tx
#define PCPS_SET_IRIG_TX_SETTINGS 0x3D  ///< (-w) Write ::IRIG_SETTINGS, only if ::_pcps_has_irig_tx, returns ::MBG_ERR_CFG on error

#define PCPS_GET_SYNTH            0x3E  ///< (r-) Read ::SYNTH, only if ::_pcps_has_synth
#define PCPS_SET_SYNTH            0x3F  ///< (-w) Write ::SYNTH, only if ::_pcps_has_synth, returns ::MBG_ERR_CFG on error

#define PCPS_GIVE_FW_ID_1         0x40  ///< (r-) Read first ::PCPS_FIFO_SIZE chars of firmware ID
#define PCPS_GIVE_FW_ID_2         0x41  ///< (r-) Read last ::PCPS_FIFO_SIZE chars of firmware ID
#define PCPS_GIVE_SERNUM          0x42  ///< (r-) Read serial number as ::PCPS_SN_STR, only if ::_pcps_has_sernum
#define PCPS_GENERIC_IO           0x43  ///< (rw) See ::pcps_generic_io or ::_mbgdevio_gen_io
#define PCPS_GET_SYNTH_STATE      0x44  ///< (r-) Read ::SYNTH_STATE, only if ::_pcps_has_synth
#define PCPS_GET_IRIG_CTRL_BITS   0x45  ///< (r-) Read ::MBG_IRIG_CTRL_BITS, only if ::_pcps_has_irig_ctrl_bits
#define PCPS_GET_RAW_IRIG_DATA    0x46  ///< (r-) Read ::MBG_RAW_IRIG_DATA, only if ::_pcps_has_raw_irig_data

#define PCPS_GET_STATUS_PORT      0x4B  ///< (r-) Read ::PCPS_STATUS_PORT
#define PCPS_GET_DEBUG_STATUS     0x4C  ///< (r-) Read ::MBG_DEBUG_STATUS, only if ::_pcps_has_debug_status

/// @note Command codes 0x4D, 0x4E, and 0x4F are reserved.

#define PCPS_READ_GPS_DATA        0x50  ///< (r-) Read large data structure, see ::PC_GPS_CMD_CODES
#define PCPS_WRITE_GPS_DATA       0x51  ///< (-w) Write  large data structure, see ::PC_GPS_CMD_CODES

#define PCPS_CLR_UCAP_BUFF        0x60  ///< (-w) No param., clear on-board capture FIFO, only if ::_pcps_has_ucap
#define PCPS_GIVE_UCAP_ENTRIES    0x61  ///< (r-) Read ::PCPS_UCAP_ENTRIES, only if ::_pcps_has_ucap
#define PCPS_GIVE_UCAP_EVENT      0x62  ///< (r-) Return oldest event as ::PCPS_HR_TIME, only if ::_pcps_has_ucap

#define PCPS_GET_CORR_INFO        0x63  ///< (r-) Read ::CORR_INFO structure, only if ::_pcps_has_pzf
#define PCPS_GET_TR_DISTANCE      0x64  ///< (r-) Read ::TR_DISTANCE, only if ::_pcps_has_tr_distance
#define PCPS_SET_TR_DISTANCE      0x65  ///< (-w) Write ::TR_DISTANCE, only if ::_pcps_has_tr_distance

#define PCPS_CLR_EVT_LOG          0x66  ///< (-w) Write clear on-board event log, only if ::_pcps_has_evt_log
#define PCPS_NUM_EVT_LOG_ENTRIES  0x67  ///< (r-) Read ::MBG_NUM_EVT_LOG_ENTRIES, only if ::_pcps_has_evt_log
#define PCPS_FIRST_EVT_LOG_ENTRY  0x68  ///< (r-) Read first (oldest) ::MBG_EVT_LOG_ENTRY, only if ::_pcps_has_evt_log
#define PCPS_NEXT_EVT_LOG_ENTRY   0x69  ///< (r-) Read next ::MBG_EVT_LOG_ENTRY, only if ::_pcps_has_evt_log

#define PCPS_FORCE_RESET          0x80  ///< (-w) No param., reset the device (deprecated, this can lock up the computer!!)

/// @note Command codes 0xF0 through 0xFF are reserved.

/** @} anchor PCPS_CMD_CODES */



#if _IS_MBG_FIRMWARE

/**
 * @brief Deprecated command group codes
 *
 * @deprecated These group codes are deprecated.
 * They should not be used anymore but removed
 * from existing source code. The explicit command
 * codes @ref PCPS_CMD_CODES should be used instead.
 *
 * @anchor PCPS_CMD_GROUP_CODES @{ */

#define PCPS_GIVE_TIME_GROUP     0x00
#define PCPS_SET_TIME_GROUP      0x10
#define PCPS_IRQ_GROUP           0x20
#define PCPS_CFG_GROUP           0x30
#define PCPS_GIVE_DATA_GROUP     0x40
#define PCPS_GPS_DATA_GROUP      0x50
#define PCPS_CTRL_GROUP          0x60
#define PCPS_CFG2_GROUP          0x70

/** @} anchor PCPS_CMD_GROUP_CODES */

#endif  // _IS_MBG_FIRMWARE



#if !defined( MBG_CMD_TABLE_EXT )
  #define MBG_CMD_TABLE_EXT  _mbg_cn_table_end()
#endif

/**
 * @brief An initializer for a table of code/name entries of non-GPS commands.
 *
 * This can e.g. initialize an array of ::MBG_CODE_NAME_TABLE_ENTRY elements
 * and may be helpful when debugging.
 *
 * @see @ref PCPS_CMD_CODES
 */
#define PCPS_CMD_CODES_TABLE                                    \
{                                                               \
  _mbg_cn_table_entry( PCPS_GIVE_TIME ),            /* 0x00 */  \
  _mbg_cn_table_entry( PCPS_GIVE_TIME_NOCLEAR ),    /* 0x01 */  \
  _mbg_cn_table_entry( PCPS_GIVE_SYNC_TIME ),       /* 0x02 */  \
  _mbg_cn_table_entry( PCPS_GIVE_HR_TIME ),         /* 0x03 */  \
  _mbg_cn_table_entry( PCPS_GIVE_IRIG_TIME ),       /* 0x04 */  \
  _mbg_cn_table_entry( PCPS_SET_TIME ),             /* 0x10 */  \
  _mbg_cn_table_entry( PCPS_SET_EVENT_TIME ),       /* 0x14 */  \
  _mbg_cn_table_entry( PCPS_IRQ_NONE ),             /* 0x20 */  \
  _mbg_cn_table_entry( PCPS_IRQ_1_SEC ),            /* 0x21 */  \
  _mbg_cn_table_entry( PCPS_IRQ_1_MIN ),            /* 0x22 */  \
  _mbg_cn_table_entry( PCPS_IRQ_10_MIN ),           /* 0x24 */  \
  _mbg_cn_table_entry( PCPS_IRQ_30_MIN ),           /* 0x28 */  \
  _mbg_cn_table_entry( PCPS_GET_SERIAL ),           /* 0x30 */  \
  _mbg_cn_table_entry( PCPS_SET_SERIAL ),           /* 0x31 */  \
  _mbg_cn_table_entry( PCPS_GET_TZCODE ),           /* 0x32 */  \
  _mbg_cn_table_entry( PCPS_SET_TZCODE ),           /* 0x33 */  \
  _mbg_cn_table_entry( PCPS_GET_PCPS_TZDL ),        /* 0x34 */  \
  _mbg_cn_table_entry( PCPS_SET_PCPS_TZDL ),        /* 0x35 */  \
  _mbg_cn_table_entry( PCPS_GET_REF_OFFS ),         /* 0x36 */  \
  _mbg_cn_table_entry( PCPS_SET_REF_OFFS ),         /* 0x37 */  \
  _mbg_cn_table_entry( PCPS_GET_OPT_INFO ),         /* 0x38 */  \
  _mbg_cn_table_entry( PCPS_SET_OPT_SETTINGS ),     /* 0x39 */  \
  _mbg_cn_table_entry( PCPS_GET_IRIG_RX_INFO ),     /* 0x3A */  \
  _mbg_cn_table_entry( PCPS_SET_IRIG_RX_SETTINGS ), /* 0x3B */  \
  _mbg_cn_table_entry( PCPS_GET_IRIG_TX_INFO ),     /* 0x3C */  \
  _mbg_cn_table_entry( PCPS_SET_IRIG_TX_SETTINGS ), /* 0x3D */  \
  _mbg_cn_table_entry( PCPS_GET_SYNTH ),            /* 0x3E */  \
  _mbg_cn_table_entry( PCPS_SET_SYNTH ),            /* 0x3F */  \
  _mbg_cn_table_entry( PCPS_GIVE_FW_ID_1 ),         /* 0x40 */  \
  _mbg_cn_table_entry( PCPS_GIVE_FW_ID_2 ),         /* 0x41 */  \
  _mbg_cn_table_entry( PCPS_GIVE_SERNUM ),          /* 0x42 */  \
  _mbg_cn_table_entry( PCPS_GENERIC_IO ),           /* 0x43 */  \
  _mbg_cn_table_entry( PCPS_GET_SYNTH_STATE ),      /* 0x44 */  \
  _mbg_cn_table_entry( PCPS_GET_IRIG_CTRL_BITS ),   /* 0x45 */  \
  _mbg_cn_table_entry( PCPS_GET_RAW_IRIG_DATA ),    /* 0x46 */  \
  _mbg_cn_table_entry( PCPS_GET_STATUS_PORT ),      /* 0x4B */  \
  _mbg_cn_table_entry( PCPS_GET_DEBUG_STATUS ),     /* 0x4C */  \
  _mbg_cn_table_entry( PCPS_READ_GPS_DATA ),        /* 0x50 */  \
  _mbg_cn_table_entry( PCPS_WRITE_GPS_DATA ),       /* 0x51 */  \
  _mbg_cn_table_entry( PCPS_CLR_UCAP_BUFF ),        /* 0x60 */  \
  _mbg_cn_table_entry( PCPS_GIVE_UCAP_ENTRIES ),    /* 0x61 */  \
  _mbg_cn_table_entry( PCPS_GIVE_UCAP_EVENT ),      /* 0x62 */  \
  _mbg_cn_table_entry( PCPS_GET_CORR_INFO ),        /* 0x63 */  \
  _mbg_cn_table_entry( PCPS_GET_TR_DISTANCE ),      /* 0x64 */  \
  _mbg_cn_table_entry( PCPS_SET_TR_DISTANCE ),      /* 0x65 */  \
  _mbg_cn_table_entry( PCPS_CLR_EVT_LOG ),          /* 0x66 */  \
  _mbg_cn_table_entry( PCPS_NUM_EVT_LOG_ENTRIES ),  /* 0x67 */  \
  _mbg_cn_table_entry( PCPS_FIRST_EVT_LOG_ENTRY ),  /* 0x68 */  \
  _mbg_cn_table_entry( PCPS_NEXT_EVT_LOG_ENTRY ),   /* 0x69 */  \
  _mbg_cn_table_entry( PCPS_FORCE_RESET ),          /* 0x80 */  \
  MBG_CMD_TABLE_EXT,                                            \
  _mbg_cn_table_end()                                           \
}



/**
 * @brief Bus level command return codes
 *
 * @deprecated These codes are deprecated and @ref MBG_RETURN_CODES should be used
 * instead which provide corresponding symbols with same numeric values.
 *
 * @anchor PCPS_LEVEL_CMD_RETURN_CODES @{ */

#define PCPS_SUCCESS       0   ///< OK, no error (see ::MBG_SUCCESS)
#define PCPS_ERR_STIME    -1   ///< invalid date/time/status passed (see ::MBG_ERR_STIME)
#define PCPS_ERR_CFG      -2   ///< invalid parms for a cmd writing config parameters (see ::MBG_ERR_CFG)

/** @} anchor PCPS_LEVEL_CMD_RETURN_CODES */



#if !defined( BITMASK )
  #define BITMASK( b )  ( ( 1 << b ) - 1 )
#endif


/** @brief The size of a bus level device's command/data FIFO */
#define PCPS_FIFO_SIZE     16

/** @brief A data buffer for a bus level device's command/data */
typedef int8_t PCPS_BUFF[PCPS_FIFO_SIZE];


/** @brief The maximum length of an ID string, including terminating 0 */
#define PCPS_ID_SIZE   ( 2 * PCPS_FIFO_SIZE + 1 )  ///< ASCIIZ string

/** @brief A buffer for an ID string, including terminating 0 */
typedef char PCPS_ID_STR[PCPS_ID_SIZE];


/** @brief The maximum length of a serial number string, including terminating 0 */
#define PCPS_SN_SIZE   ( PCPS_FIFO_SIZE + 1 )  ///< ASCIIZ string

/** @brief A buffer for a serial number string, including terminating 0 */
typedef char PCPS_SN_STR[PCPS_SN_SIZE];


/**
 * @brief Seconds since epoch 1970-01-01, usually %UTC scale
 *
 * Used with ::PCPS_TIME_STAMP.
 *
 * @see ::PCPS_FRAC_32
 * @see ::PCPS_TIME_STAMP
 */
typedef uint32_t PCPS_SECONDS;

#define _mbg_swab_pcps_seconds( _p )  \
do                                    \
{                                     \
  _mbg_swab32( _p );                  \
} while ( 0 )



/**
 * @brief 32 bit binary fraction of a second
 *
 * Used with ::PCPS_TIME_STAMP, e.g.
 * 0x80000000 == 0.5 s, 0xFFFFFFFF == 0.9999999.. s, etc.
 * Use ::bin_frac_32_to_dec_frac for conversion.
 *
 * @see ::bin_frac_32_to_dec_frac
 * @see ::PCPS_SECONDS
 * @see ::PCPS_TIME_STAMP
 * @see ::PCPS_HRT_FRAC_SCALE
 * @see ::PCPS_HRT_FRAC_SCALE_FMT
 */
typedef uint32_t PCPS_FRAC_32;

#define _mbg_swab_pcps_frac_32( _p )  \
do                                    \
{                                     \
  _mbg_swab32( _p );                  \
} while ( 0 )



/**
 * @brief A high resolution time stamp
 */
typedef struct
{
  PCPS_SECONDS sec;   ///< seconds since 1970, usually %UTC scale
  PCPS_FRAC_32 frac;  ///< binary fractions of second, see ::PCPS_FRAC_32

} PCPS_TIME_STAMP;

#define _mbg_swab_pcps_time_stamp( _p )   \
do                                        \
{                                         \
  _mbg_swab_pcps_seconds( &(_p)->sec );   \
  _mbg_swab_pcps_frac_32( &(_p)->frac );  \
} while ( 0 )



#ifndef PCPS_HRT_FRAC_SCALE
  /**
   * @brief Scale to be used to print ::PCPS_TIME_STAMP::frac values
   *
   * The function ::bin_frac_32_to_dec_frac can be used for the conversion.
   *
   * @see ::PCPS_HRT_FRAC_SCALE_FMT
   */
  #define PCPS_HRT_FRAC_SCALE       10000000UL
#endif

#ifndef PCPS_HRT_FRAC_SCALE_FMT
  /**
   * @brief Format specifier used to print ::PCPS_TIME_STAMP::frac values
   *
   * Used to print values scaled with ::bin_frac_32_to_dec_frac called
   * with ::PCPS_HRT_FRAC_SCALE.
   *
   * @see ::PCPS_HRT_FRAC_SCALE
   */
  #define PCPS_HRT_FRAC_SCALE_FMT   "%07lu"
#endif



/**
 * @brief Extended status code
 *
 * Low byte corresponds to ::PCPS_TIME_STATUS, high byte
 * contains additional flags.
 *
 * @see ::PCPS_TIME_STATUS
 * @see @ref PCPS_TIME_STATUS_FLAGS
 */
typedef uint16_t PCPS_TIME_STATUS_X;

#define _mbg_swab_pcps_time_status_x( _p )   _mbg_swab16( _p )


typedef struct
{
  PCPS_TIME_STATUS_X set_mask;
  PCPS_TIME_STATUS_X clr_mask;

} PCPS_TIME_STATUS_X_MASKS;

#define _mbg_swab_pcps_time_status_x_masks( _p )   \
do                                                 \
{                                                  \
  _mbg_swab_pcps_time_status_x( &(_p)->set_mask ); \
  _mbg_swab_pcps_time_status_x( &(_p)->clr_mask ); \
} while ( 0 )



/**
 * @brief Definitions used to report a signal strength
 *
 * @anchor PCPS_SIG_VAL_DEFS @{ */

/**
 * @brief A data type used to report the signal value
 */
typedef uint8_t PCPS_SIG_VAL;

// The following constants are used to draw a signal bar
// depending on a DCF77 clock's signal value:
#define PCPS_SIG_BIAS   55
#define PCPS_SIG_ERR    1
#define PCPS_SIG_MIN    20
#define PCPS_SIG_MAX    68

// These constants are used by non-DCF77 devices to indicate
// if an input signal is available or not:
#define PCPS_SIG_LVL_SIG_NOT_AVAIL   0
#define PCPS_SIG_LVL_SIG_AVAIL       128

/** @} anchor PCPS_SIG_VAL_DEFS */



/**
 * @brief High resolution time including status and local time offset
 *
 * Used to read time with high resolution of fractions of seconds and
 * more detailed information on the local time offset and status.
 * Should be prefered over ::PCPS_TIME.
 *
 * ::_pcps_has_hr_time checks whether the command ::PCPS_GIVE_TIME is
 * supported to read a device's current time using this format.
 *
 * Newer devices providing time capture input may also accept the
 * ::PCPS_GIVE_UCAP_EVENT command to read user capture event times
 * using this format. In this case, the "signal" field contains
 * the number of the capture input line, e.g. 0 or 1.
 * ::_pcps_has_ucap checks whether this is supported.
 */
typedef struct
{
  PCPS_TIME_STAMP tstamp;     ///< High resolution time stamp (%UTC)
  int32_t utc_offs;           ///< %UTC offs [sec] (loc_time = tstamp + utc_offs)
  PCPS_TIME_STATUS_X status;  ///< status bits, see @ref PCPS_TIME_STATUS_FLAGS
  PCPS_SIG_VAL signal;        ///< signal strength, see @ref PCPS_SIG_VAL_DEFS, or capture input channel number

} PCPS_HR_TIME;

#define _mbg_swab_pcps_hr_time( _p )              \
do                                                \
{                                                 \
  _mbg_swab_pcps_time_stamp( &(_p)->tstamp );     \
  _mbg_swab32( &(_p)->utc_offs );                 \
  _mbg_swab_pcps_time_status_x( &(_p)->status );  \
} while ( 0 )


/**
 * @brief Time synchronization status
 *
 * Used by legacy API calls. New API calls provide
 * an extended status word ::PCPS_TIME_STATUS_X.
 *
 * @see ::PCPS_TIME_STATUS_FLAGS_COMMON
 * @see ::PCPS_TIME_STATUS_X
 */
typedef uint8_t PCPS_TIME_STATUS;



/**
 * @brief Local calendar date and time, plus sync status
 *
 * This legacy structure is supported by all bus level devices but
 * has a time resultion of 10 ms only. For more accurate time stamps
 * the structures ::PCPS_HR_TIME and ::PCPS_TIME_STAMP should be
 * used preferably.
 *
 * @see ::PCPS_HR_TIME
 * @see ::PCPS_TIME_STAMP
 * @see ::PCPS_STIME
 */
typedef struct
{
  uint8_t sec100;  ///< hundredths of seconds, 0..99, 10 ms resolution
  uint8_t sec;     ///< seconds, 0..59, or 60 if leap second
  uint8_t min;     ///< minutes, 0..59
  uint8_t hour;    ///< hours, 0..23

  uint8_t mday;    ///< day of month, 0..31
  uint8_t wday;    ///< day of week, 1..7, 1 = Monday
  uint8_t month;   ///< month, 1..12
  uint8_t year;    ///< year of the century, 0..99

  PCPS_TIME_STATUS status;  ///< status bits, see ::PCPS_TIME_STATUS_FLAGS_COMMON
  PCPS_SIG_VAL signal;      ///< signal strength, see @ref PCPS_SIG_VAL_DEFS
  int8_t offs_utc;          ///< [hours], 0 if not ::_pcps_has_utc_offs

} PCPS_TIME;



/**
 * @brief Date time and status used with the ::PCPS_SET_TIME command
 *
 * Similar to ::PCPS_TIME, but missing the ::PCPS_TIME::signal
 * and ::PCPS_TIME::offs_utc fields.
 *
 * @see ::PCPS_TIME
 */
typedef struct
{
  uint8_t sec100;  ///< hundredths of seconds, 0..99, 10 ms resolution
  uint8_t sec;     ///< seconds, 0..59, or 60 if leap second
  uint8_t min;     ///< minutes, 0..59
  uint8_t hour;    ///< hours, 0..23

  uint8_t mday;    ///< day of month, 0..31
  uint8_t wday;    ///< day of week, 1..7, 1 = Monday
  uint8_t month;   ///< month, 1..12
  uint8_t year;    ///< year of the century, 0..99

  PCPS_TIME_STATUS status;  ///< status bits, see ::PCPS_TIME_STATUS_FLAGS_COMMON

} PCPS_STIME;

#ifdef _C166
  // This is a workaround to specify some structure sizes. The C166 compiler
  // always reports an even structure size although the structure size may
  // be odd due to the number of bytes. This might lead to errors between
  // the C166 and other build environments.
  #define sizeof_PCPS_TIME   ( sizeof( PCPS_TIME ) - 1 )
  #define sizeof_PCPS_STIME  ( sizeof( PCPS_STIME ) - 1 )
#else
  #define sizeof_PCPS_TIME   sizeof( PCPS_TIME )
  #define sizeof_PCPS_STIME  sizeof( PCPS_STIME )
#endif

typedef union
{
  PCPS_TIME t;
  PCPS_STIME stime;

} PCPS_TIME_UNION;



/**
 * @brief Raw IRIG time
 *
 * Used to read the raw IRIG time from an IRIG receiver card, if the card
 * supports this. See the ::PCPS_GIVE_IRIG_TIME command.
 *
 * The granularity of the value in the ::PCPS_IRIG_TIME::frac field
 * depends on the update interval at which the structure is updated
 * by the firmeware. I.e., if the raw IRIG time is updated only
 * once per second, the ::PCPS_IRIG_TIME::frac value can always be 0.
 *
 * @see @ref group_icode
 * @see ::ICODE_RX_CODES
 */
typedef struct
{
  PCPS_TIME_STATUS_X status;  ///< status bits, see @ref PCPS_TIME_STATUS_FLAGS
  int16_t offs_utc;           ///< [minutes], 0 unless supported by the code format, see ::MSK_ICODE_RX_HAS_TZI
  uint16_t yday;              ///< day of year, 1..365/366
  uint16_t frac;              ///< fractions of seconds, 0.1 ms units
  uint8_t sec;                ///< seconds, 0..59, may be 60 for leap second
  uint8_t min;                ///< minutes, 0..59
  uint8_t hour;               ///< hours, 0..23
  uint8_t year;               ///< 2 digit year number, or ::IRIG_TIME_UNKNOWN_YEAR if year not supported
                              ///< by the time code, see ::MSK_ICODE_RX_HAS_ANY_SHORT_YEAR
  PCPS_SIG_VAL signal;        ///< signal strength, see @ref PCPS_SIG_VAL_DEFS
  uint8_t reserved;           ///< currently not used, always 0

} PCPS_IRIG_TIME;

#define _mbg_swab_pcps_irig_time( _p )            \
do                                                \
{                                                 \
  _mbg_swab_pcps_time_status_x( &(_p)->status );  \
  _mbg_swab16( &(_p)->offs_utc );                 \
  _mbg_swab16( &(_p)->yday );                     \
  _mbg_swab16( &(_p)->frac );                     \
} while ( 0 )


/**
 * @brief A constant representing a 2 digit unknown IRIG year number
 *
 * Used with ::PCPS_IRIG_TIME::year
 */
#define IRIG_TIME_UNKNOWN_YEAR    0xFF



/**
 * @brief Time status flags
 *
 * @anchor PCPS_TIME_STATUS_FLAGS @{ */

/**
 * @brief Legacy time status flags
 *
 * Bit masks used with both ::PCPS_TIME_STATUS and ::PCPS_TIME_STATUS_X
 */
enum PCPS_TIME_STATUS_FLAGS_COMMON
{
  PCPS_FREER  = 0x01,  ///< long wave or time code receiver running on xtal, satellite receiver has not verified its position
  PCPS_DL_ENB = 0x02,  ///< daylight saving currently enabled
  PCPS_SYNCD  = 0x04,  ///< long wave or time code receiver has sync'ed at least once after pwr up, sat receiver is synchronized
  PCPS_DL_ANN = 0x08,  ///< a change in daylight saving status is announced
  PCPS_UTC    = 0x10,  ///< returned time is always %UTC instead of some local time
  PCPS_LS_ANN = 0x20,  ///< leap second announced, for *very* old clocks see ::REV_PCPS_LS_ANN_PC31PS31
  PCPS_IFTM   = 0x40,  ///< the current time has been set by an API call, for *very* old clocks see ::REV_PCPS_IFTM_PC31PS31
  PCPS_INVT   = 0x80   ///< invalid time because battery had been disconnected, or absolute time can't be decoded safely
};


/**
 * @brief Extended time status flags
 *
 * Bit masks used with ::PCPS_TIME_STATUS_X only
 */
enum PCPS_TIME_STATUS_FLAGS_EXT
{
  PCPS_LS_ENB     = 0x0100,  ///< current second is leap second
  PCPS_ANT_FAIL   = 0x0200,  ///< antenna failure
  PCPS_LS_ANN_NEG = 0x0400,  ///< announced leap second is negative
  PCPS_SCALE_GPS  = 0x0800,  ///< time stamp is GPS scale
  PCPS_SCALE_TAI  = 0x1000,  ///< time stamp is TAI scale

  PCPS_UCAP_OVERRUN     = 0x2000,  ///< events interval too short (capture events only)
  PCPS_UCAP_BUFFER_FULL = 0x4000,  ///< events read too slow (capture events only)

  /**
   * Bit masks used only with time stamps representing the current board time.
   * A DCF77 PZF receiver can set this bit if it is actually synchronized
   * using PZF correlation and thus provides higher accuracy than AM receivers.
   * Same numeric code as ::PCPS_UCAP_OVERRUN
   */
  PCPS_SYNC_PZF = 0x2000,

  /**
   * Immediately after a clock has been accessed, subsequent accesses
   * may be blocked for up to 1.5 msec to give the clock's microprocessor
   * some time to decode the incoming time signal.
   * The flag below is eventually set if a program tries to read ::PCPS_HR_TIME
   * during this interval. In this case the read function returns the
   * proper time stamp which is taken if the command byte is written,
   * however, the read function returns with delay.
   * This flag is not supported by all clocks.
   */
  PCPS_IO_BLOCKED = 0x8000
};



/**
 * This bit mask can be used to extract the time scale information out
 * of a PCPS_TIME_STATUS_X value.
 */
#define PCPS_SCALE_MASK ( PCPS_SCALE_TAI | PCPS_SCALE_GPS )


/** @} anchor PCPS_TIME_STATUS_FLAGS */



/**
 * @brief Legacy definitions used to configure a device's serial port
 *
 * @deprecated This structure and the associated command codes
 * are deprecated. ::PORT_SETTINGS, ::PORT_INFO and associated
 * definitions should be used instead, if supported.
 *
 * @anchor PCPS_OLD_SERIAL_CFG @{ */

/**
 * @brief Configuration information for a device's serial port
 *
 * Used with ::PCPS_GET_SERIAL and ::PCPS_SET_SERIAL
 *
 * The serial interface on some old DCF77 clocks can be configured
 * using the commands ::PCPS_SET_SERIAL and ::PCPS_GET_SERIAL.
 * Both commands use a parameter byte describing transmission speed,
 * framing and mode of operation. The parameter byte can be assembled
 * using the constants defined in ::PCPS_BD_CODES, ::PCPS_FR_CODES,
 * and ::PCPS_MOD_CODES, by or'ing one of the constants of each group,
 * shifted to the right position. ::PCPS_GET_SERIAL expects that parameter
 * byte and ::PCPS_GET_SERIAL returns the current configuration from
 * the board.
 * ::_pcps_has_serial checks whether supported.
 * For old GPS clocks refer to the comments for the ::PCPS_GET_SERIAL
 * command.
 */
typedef uint8_t PCPS_SERIAL;


/**
 * @brief Deprecated baud rate indices
 *
 * @deprecated These values are deprecated.
 * ::MBG_BAUD_RATE_CODES and associated structures
 * should be used preferably.
 *
 * The sequence of codes matches the sequence
 * defined in ::MBG_BAUD_RATE_CODES.
 */
enum PCPS_BD_CODES
{
  PCPS_BD_300,
  PCPS_BD_600,
  PCPS_BD_1200,
  PCPS_BD_2400,
  PCPS_BD_4800,
  PCPS_BD_9600,
  PCPS_BD_19200,
  N_PCPS_BD      ///< number of codes
};

#define PCPS_BD_BITS   4  ///< field with in the cfg byte
#define PCPS_BD_SHIFT  0  ///< num of bits to shift left



/**
 * @brief Deprecated framing code indices
 *
 * @deprecated These values are deprecated.
 * ::MBG_FRAMING_CODES and associated structures
 * should be used preferably.
 *
 * Unfortunately, these framing codes can ***not*** simply
 * be replaced by the newer MBG_FRAMING_... definitions
 * since the order of indices doesn't match.
 */
enum PCPS_FR_CODES
{
  PCPS_FR_8N1,
  PCPS_FR_7E2,
  PCPS_FR_8N2,
  PCPS_FR_8E1,
  N_PCPS_FR_DCF  ///< number of valid codes
};

#define PCPS_FR_BITS   2             ///< field with in the cfg byte
#define PCPS_FR_SHIFT  PCPS_BD_BITS  ///< num of bits to shift left



/**
 * @brief Deprecated codes for modes of operation
 *
 * @deprecated These values are deprecated.
 * ::STR_MODES and associated structures
 * should be used preferably.
 *
 * The sequence of codes matches the sequence
 * defined in ::STR_MODES.
 */
enum PCPS_MOD_CODES
{
  PCPS_MOD_REQ,     ///< time string on request '?' only
  PCPS_MOD_SEC,     ///< time string once per second
  PCPS_MOD_MIN,     ///< time string once per minute
  PCPS_MOD_RSVD,    ///< reserved
  N_PCPS_MOD_DCF    ///< number of possible codes
};

#define PCPS_MOD_BITS   2                                ///< field with in the cfg byte
#define PCPS_MOD_SHIFT  ( PCPS_BD_BITS + PCPS_FR_BITS )  ///< num of bits to shift left

/** @} anchor PCPS_OLD_SERIAL_CFG */



/**
 * @brief Type of variable to hold a TZ code
 *
 * This is used with the PCI interface but differs from ::TZCODE
 * which is used with the binary protocol.
 *
 * @see ::TZCODE
 * @see ::TZCODE_UNION
 */
typedef uint8_t PCPS_TZCODE;


/**
 * @brief Enumeration of codes used with PCPS_TZCODE
 */
enum PCPS_TZCODES
{
  PCPS_TZCODE_CET_CEST,  ///< default as broadcast by DCF77 (UTC+1h/UTC+2h)
  PCPS_TZCODE_CET,       ///< always CET (UTC+1h), discard DST
  PCPS_TZCODE_UTC,       ///< always %UTC
  PCPS_TZCODE_EET_EEST,  ///< East European Time, CET/CEST + 1h
  N_PCPS_TZCODE          ///< the number of valid codes
};

/* the definitions below are for compatibily only: */
#define PCPS_TZCODE_MEZMESZ  PCPS_TZCODE_CET_CEST
#define PCPS_TZCODE_MEZ      PCPS_TZCODE_CET
#define PCPS_TZCODE_OEZ      PCPS_TZCODE_EET_EEST


/**
 * @brief Daylight changeover specification
 *
 * Used as member field of ::PCPS_TZDL only. Most devices supporting
 * conversion to local time support the ::TZDL structure instead.
 *
 * @see ::TZDL
 */
typedef struct
{
  uint16_t year_or_wday;  ///< The full year number, or 0..6 == Sun..Sat if the ::DL_AUTO_FLAG is set
  uint8_t month;          ///< [1..12]
  uint8_t mday;           ///< [1..31]
  uint8_t hour;           ///< [0..23]
  uint8_t min;            ///< [0..59]

} PCPS_DL_ONOFF;

#define _mbg_swab_pcps_dl_onoff( _p )  \
do                                     \
{                                      \
  _mbg_swab16( &(_p)->year_or_wday );  \
} while ( 0 )

/**
 * @brief A flag indicating if DST changeovers are to be computed automatically
 *
 * If ::PCPS_DL_ONOFF::year_or_wday is or'ed with the constant ::DL_AUTO_FLAG
 * then start and end of daylight saving time shall be computed automatically
 * for each year. In this case the remaining bits represent the day-of-week
 * after the specified mday/month at which the change shall occur.
 * If that flag is not set then the field contains the full four-digit year number
 * and the ::PCPS_DL_ONOFF::mday and ::PCPS_DL_ONOFF::month values specify
 * the exact date of that year. Most devices supporting conversion to local time
 * support the ::TZDL structure instead.
 *
 * @see ::TZDL
 * @see ::PCPS_TZDL
 */
#define DL_AUTO_FLAG  0x8000  // also defined in gpsdefs.h


/**
 * @brief Specification of a local time zone
 *
 * Most devices supporting conversion to local time support
 * the ::TZDL structure instead.
 *
 * @see ::DL_AUTO_FLAG
 * @see ::TZDL
 */
typedef struct
{
  int16_t offs;          ///< offset from %UTC to local time [min] (local time = %UTC + offs)
  int16_t offs_dl;       ///< additional offset if DST enabled [min] (DST time = local time + offs_dl)
  PCPS_DL_ONOFF tm_on;   ///< date/time when daylight saving starts
  PCPS_DL_ONOFF tm_off;  ///< date/time when daylight saving ends

} PCPS_TZDL;

#define _mbg_swab_pcps_tzdl( _p )            \
do                                           \
{                                            \
  _mbg_swab16( &(_p)->offs );                \
  _mbg_swab16( &(_p)->offs_dl );             \
  _mbg_swab_pcps_dl_onoff( &(_p)->tm_on );   \
  _mbg_swab_pcps_dl_onoff( &(_p)->tm_off );  \
} while ( 0 )



/**
 * @brief Status of the time capture FIFO buffer
 *
 * Only supported if ::RECEIVER_INFO::n_ucaps > 0.
 */
typedef struct
{
  uint32_t used;   ///< the number of saved capture events
  uint32_t max;    ///< capture buffer size

} PCPS_UCAP_ENTRIES;

#define _mbg_swab_pcps_ucap_entries( _p )  \
do                                         \
{                                          \
  _mbg_swab32( &(_p)->used );              \
  _mbg_swab32( &(_p)->max );               \
} while ( 0 )



/**
 * @defgroup group_pzf_supp Definitions used with PZF receivers
 *
 * @{ */

/**
 * @brief Receiver distance from transmitter [km]
 */
typedef uint16_t TR_DISTANCE;  ///< Range may vary with receiver type

#define _mbg_swab_tr_distance( _p )  \
  _mbg_swab16( _p )


/**
 * @brief PZF correlation status info
 */
typedef struct
{
  uint8_t val;          ///< correlation value, or check count if status ==:: PZF_CORR_CHECK
  uint8_t status;       ///< status codes, see ::PZF_CORR_STATES
  char corr_dir;        ///< space, '<', or '>', just for information
  PCPS_SIG_VAL signal;  ///< signal strength, see @ref PCPS_SIG_VAL_DEFS

} CORR_INFO;

#define _mbg_swab_corr_info( _p )  \
  _nop_macro_fnc()


/**
 * @brief Codes used with ::CORR_INFO::status
 */
enum PZF_CORR_STATES
{
  PZF_CORR_RAW,    ///< trying raw correlation, combi receivers running in AM mode
  PZF_CORR_CHECK,  ///< raw correlation achieved, doing plausibility checks
  PZF_CORR_FINE,   ///< fine correlation achieved
  N_PZF_CORR_STATE
};


#define PZF_CORR_STATE_NAME_RAW_ENG     "Searching"
#define PZF_CORR_STATE_NAME_CHECK_ENG   "Correlating"
#define PZF_CORR_STATE_NAME_FINE_ENG    "Locked"

#define PZF_CORR_STATE_NAME_RAW_GER     "suchen"
#define PZF_CORR_STATE_NAME_CHECK_GER   "korrelieren"
#define PZF_CORR_STATE_NAME_FINE_GER    "eingerastet"


#define PZF_CORR_STATE_NAMES_ENG \
{                                \
  PZF_CORR_STATE_NAME_RAW_ENG,   \
  PZF_CORR_STATE_NAME_CHECK_ENG, \
  PZF_CORR_STATE_NAME_FINE_ENG   \
}


#define PZF_CORR_STATE_NAMES_LSTR                                   \
{                                                                   \
  { PZF_CORR_STATE_NAME_RAW_ENG, PZF_CORR_STATE_NAME_RAW_GER },     \
  { PZF_CORR_STATE_NAME_CHECK_ENG, PZF_CORR_STATE_NAME_CHECK_GER }, \
  { PZF_CORR_STATE_NAME_FINE_ENG, PZF_CORR_STATE_NAME_FINE_GER }    \
}

/** @} defgroup group_pzf_supp */



/**
 * @brief GPS Command codes passed via the system bus
 *
 * Codes specifying various types of data that can be read from or
 * written to Meinberg bus level devices which support this.
 * Access is done using the low level functions ::pcps_read_gps
 * and ::pcps_write_gps since the size of some of the structures
 * exceeds the size of the device's I/O buffer and must therefore be
 * accessed in several blocks.
 *
 * Applications should instead use the API functions declared in mbgdevio.h.
 *
 * The structures to be used are defined in gpsdefs.h. Not all structures
 * are supported, yet. Check the r/w indicators for details.
 *
 * @see @ref PCPS_CMD_CODES
 * @see ::PC_GPS_CMD_CODES_TABLE
 */
enum PC_GPS_CMD_CODES
{
  PC_GPS_TZDL,                       ///< (r/w) ::TZDL, time zone / daylight saving, only if ::GPS_MODEL_HAS_TZDL
  PC_GPS_SW_REV,                     ///< (r/-) ::SW_REV, software revision, deprecated by ::PC_GPS_RECEIVER_INFO
  PC_GPS_BVAR_STAT,                  ///< (r/-) ::BVAR_STAT, status of buffered variables, only if ::GPS_MODEL_HAS_BVAR_STAT
  PC_GPS_TIME,                       ///< (r/w) ::TTM, current time, deprecated by ::PCPS_GIVE_HR_TIME
  PC_GPS_POS_XYZ,                    ///< (-/w) ::XYZ, current position in ECEF coordinates, only if ::GPS_MODEL_HAS_POS_XYZ
  PC_GPS_POS_LLA,                    ///< (-/w) ::LLA, current position in geographic coordinates, only if ::GPS_MODEL_HAS_POS_LLA
  PC_GPS_PORT_PARM,                  ///< (r/w) ::PORT_PARM, param. of the serial ports, deprecated by ::PC_GPS_ALL_PORT_INFO
  PC_GPS_ANT_INFO,                   ///< (r/-) ::ANT_INFO, time diff at sync. after antenna had been disconn., only if ::GPS_MODEL_HAS_ANT_INFO

  PC_GPS_UCAP,                       ///< (r/-) ::TTM, user capture events, deprecated by ::PCPS_GIVE_UCAP_EVENT
  PC_GPS_ENABLE_FLAGS,               ///< (r/w) ::ENABLE_FLAGS, when to enable serial, pulses, and synth, only if ::GPS_MODEL_HAS_ENABLE_FLAGS
  PC_GPS_STAT_INFO,                  ///< (r/-) ::GPS_STAT_INFO, satellite info, mode of operation, and DAC info, only if ::GPS_MODEL_HAS_STAT_INFO
  PC_GPS_CMD,                        ///< (-/w) ::GPS_CMD, send one of the ::PC_GPS_COMMANDS
  PC_GPS_IDENT,                      ///< (r/-) ::IDENT, serial number, deprecated by ::PC_GPS_RECEIVER_INFO
  PC_GPS_POS,                        ///< (r/-) ::POS, position ::XYZ, ::LLA, and ::DMS combined, only if ::GPS_MODEL_HAS_POS
  PC_GPS_ANT_CABLE_LEN,              ///< (r/w) ::ANT_CABLE_LEN, length of antenna cable, only if ::GPS_MODEL_HAS_ANT_CABLE_LEN
  PC_GPS_RECEIVER_INFO,              ///< (r/-) ::RECEIVER_INFO, rcvr model info, only if ::PCPS_HAS_RECEIVER_INFO

  PC_GPS_ALL_STR_TYPE_INFO,          ///< (r/-) n * ::STR_TYPE_INFO_IDX, names and capabilities of all supp. string types, only if ::RECEIVER_INFO::n_str_type > 0
  PC_GPS_ALL_PORT_INFO,              ///< (r/-) n * ::PORT_INFO_IDX, settings and capabilities of all serial ports, only if ::RECEIVER_INFO::n_com_ports > 0
  PC_GPS_PORT_SETTINGS_IDX,          ///< (-/w) ::PORT_SETTINGS_IDX, settings for specified serial port, only if ::RECEIVER_INFO::n_com_ports > 0
  PC_GPS_ALL_POUT_INFO,              ///< (r/-)  n * ::POUT_INFO_IDX, all programmable output info
  PC_GPS_POUT_SETTINGS_IDX,          ///< (-/w)  ::POUT_SETTINGS_IDX, settings for one programmable output
  PC_GPS_TIME_SCALE,                 ///< (r/w)  ::MBG_TIME_SCALE_SETTINGS / ::MBG_TIME_SCALE_INFO, only if ::PCPS_HAS_TIME_SCALE
  PC_GPS_LAN_IF_INFO,                ///< (r/-)  ::LAN_IF_INFO, LAN interface info, only if ::PCPS_HAS_LAN_INTF
  PC_GPS_IP4_STATE,                  ///< (r/-)  ::IP4_SETTINGS, LAN interface state, only if ::PCPS_HAS_LAN_INTF

  PC_GPS_IP4_SETTINGS,               ///< (r/w)  ::IP4_SETTINGS, LAN interface configuration, only if ::PCPS_HAS_LAN_INTF
  PC_GPS_PTP_STATE,                  ///< (r/-)  ::PTP_STATE, only if ::PCPS_HAS_PTP
  PC_GPS_PTP_CFG,                    ///< (r/w)  ::PTP_CFG_SETTINGS / ::PTP_CFG_INFO, only if ::PCPS_HAS_PTP
  PC_GPS_PTP_UC_MASTER_CFG_LIMITS,   ///< (r/-)  ::PTP_UC_MASTER_CFG_LIMITS, only if ::PTP_CFG_MSK_SUPPORT_PTP_UNICAST
  PC_GPS_ALL_PTP_UC_MASTER_INFO,     ///< (r/-)  n * ::PTP_UC_MASTER_INFO_IDX, only if ::PTP_CFG_MSK_SUPPORT_PTP_UNICAST
  PC_GPS_PTP_UC_MASTER_SETTINGS_IDX, ///< (-/w)  ::PTP_UC_MASTER_SETTINGS_IDX, only if ::PTP_CFG_MSK_SUPPORT_PTP_UNICAST
  PC_GPS_GPIO_CFG_LIMITS,            ///< (r/-)  ::MBG_GPIO_CFG_LIMITS, only if ::GPS_HAS_GPIO
  PC_GPS_ALL_GPIO_INFO,              ///< (r/-)  n * ::MBG_GPIO_INFO_IDX, all GPIO info, only if ::GPS_HAS_GPIO

  PC_GPS_GPIO_SETTINGS_IDX,          ///< (-/w)  ::MBG_GPIO_SETTINGS_IDX, settings for a specific port, only if ::GPS_HAS_GPIO
  PC_GPS_GNSS_MODE,                  ///< (r/w)  ::MBG_GNSS_MODE_INFO / ::MBG_GNSS_MODE_SETTINGS, only if ::PCPS_IS_GNSS
  PC_GPS_ALL_GNSS_SAT_INFO,          ///< (r/-)  n * ::GNSS_SAT_INFO_IDX, satellite info, only if ::PCPS_IS_GNSS
  PC_GPS_XMR_INSTANCES,              ///< (r/-)  ::XMULTI_REF_INSTANCES, only if ::GPS_HAS_XMULTI_REF and ::GPS_HAS_XMRS_MULT_INSTC
  PC_GPS_XMR_SETTINGS_IDX,           ///< (-/w)  ::XMULTI_REF_SETTINGS_IDX, idx 0..::XMULTI_REF_INSTANCES::n_xmr_settings-1, only if ::GPS_HAS_XMULTI_REF
  PC_GPS_ALL_XMR_INFO,               ///< (r/-)  n * ::XMULTI_REF_INFO_IDX, where n == ::XMULTI_REF_INSTANCES::n_xmr_settings, only if ::GPS_HAS_XMULTI_REF
  PC_GPS_ALL_XMR_STATUS,             ///< (r/w)  n * ::XMULTI_REF_STATUS_IDX, where n == ::XMULTI_REF_INSTANCES::n_xmr_settings, one structure on write, only if ::GPS_HAS_XMULTI_REF
  PC_GPS_XMR_HOLDOVER_STATUS,        ///< (r/-)  ::XMR_HOLDOVER_STATUS, only if ::XMRIF_MSK_HOLDOVER_STATUS_SUPP

  PC_GPS_ALL_GPIO_STATUS,            ///< (r/-)  n * ::MBG_GPIO_STATUS_IDX, where n == ::MBG_GPIO_CFG_LIMITS::num_io, only if ::MBG_GPIO_CFG_LIMIT_FLAG_MASK_STATUS_SUPP
  PC_GPS_XFEATURE_BUFFER,            ///< (r/-)  ::MBG_XFEATURE_BUFFER, only if ::GPS_HAS_XFEATURE
  PC_GPS_TLV_INFO,                   ///< (r/-)  ::MBG_TLV_INFO, only if ::MBG_XFEATURE_TLV_API

  // GPS data
  PC_GPS_CFGH = 0x80,                ///< (-/-)  ::CFGH, SVs' config. and health codes (yet not used)
  PC_GPS_ALM,                        ///< (-/-)  ::SV_ALM, one SV's num and almanac (yet not used)
  PC_GPS_EPH,                        ///< (-/-)  ::SV_EPH, one SV's num and ephemeris (yet not used)
  PC_GPS_UTC,                        ///< (r/w)  ::UTC, %UTC corr. param., only if ::PCPS_HAS_UTC_PARM
  PC_GPS_IONO,                       ///< (-/-)  ::IONO, ionospheric corr. param. (yet not used)
  PC_GPS_ASCII_MSG                   ///< (-/-)  ::ASCII_MSG, the GPS ASCII message (yet not used)
};



/**
 * @brief An initializer for a table of code/name entries of GPS commands.
 *
 * This can e.g. be assigned to an array of ::MBG_CODE_NAME_TABLE_ENTRY elements
 * and may be helpful when debugging.
 *
 * @see ::PC_GPS_CMD_CODES
 */
#define PC_GPS_CMD_CODES_TABLE                               \
{                                                            \
  _mbg_cn_table_entry( PC_GPS_TZDL ),                        \
  _mbg_cn_table_entry( PC_GPS_SW_REV ),                      \
  _mbg_cn_table_entry( PC_GPS_BVAR_STAT ),                   \
  _mbg_cn_table_entry( PC_GPS_TIME ),                        \
  _mbg_cn_table_entry( PC_GPS_POS_XYZ ),                     \
  _mbg_cn_table_entry( PC_GPS_POS_LLA ),                     \
  _mbg_cn_table_entry( PC_GPS_PORT_PARM ),                   \
  _mbg_cn_table_entry( PC_GPS_ANT_INFO ),                    \
  _mbg_cn_table_entry( PC_GPS_UCAP ),                        \
  _mbg_cn_table_entry( PC_GPS_ENABLE_FLAGS ),                \
  _mbg_cn_table_entry( PC_GPS_STAT_INFO ),                   \
  _mbg_cn_table_entry( PC_GPS_CMD ),                         \
  _mbg_cn_table_entry( PC_GPS_IDENT ),                       \
  _mbg_cn_table_entry( PC_GPS_POS ),                         \
  _mbg_cn_table_entry( PC_GPS_ANT_CABLE_LEN ),               \
  _mbg_cn_table_entry( PC_GPS_RECEIVER_INFO ),               \
  _mbg_cn_table_entry( PC_GPS_ALL_STR_TYPE_INFO ),           \
  _mbg_cn_table_entry( PC_GPS_ALL_PORT_INFO ),               \
  _mbg_cn_table_entry( PC_GPS_PORT_SETTINGS_IDX ),           \
  _mbg_cn_table_entry( PC_GPS_ALL_POUT_INFO ),               \
  _mbg_cn_table_entry( PC_GPS_POUT_SETTINGS_IDX ),           \
  _mbg_cn_table_entry( PC_GPS_TIME_SCALE ),                  \
  _mbg_cn_table_entry( PC_GPS_LAN_IF_INFO ),                 \
  _mbg_cn_table_entry( PC_GPS_IP4_STATE ),                   \
  _mbg_cn_table_entry( PC_GPS_IP4_SETTINGS ),                \
  _mbg_cn_table_entry( PC_GPS_PTP_STATE ),                   \
  _mbg_cn_table_entry( PC_GPS_PTP_CFG ),                     \
  _mbg_cn_table_entry( PC_GPS_PTP_UC_MASTER_CFG_LIMITS ),    \
  _mbg_cn_table_entry( PC_GPS_ALL_PTP_UC_MASTER_INFO ),      \
  _mbg_cn_table_entry( PC_GPS_PTP_UC_MASTER_SETTINGS_IDX ),  \
  _mbg_cn_table_entry( PC_GPS_GPIO_CFG_LIMITS ),             \
  _mbg_cn_table_entry( PC_GPS_ALL_GPIO_INFO ),               \
  _mbg_cn_table_entry( PC_GPS_GPIO_SETTINGS_IDX ),           \
  _mbg_cn_table_entry( PC_GPS_GNSS_MODE ),                   \
  _mbg_cn_table_entry( PC_GPS_ALL_GNSS_SAT_INFO ),           \
  _mbg_cn_table_entry( PC_GPS_XMR_INSTANCES ),               \
  _mbg_cn_table_entry( PC_GPS_XMR_SETTINGS_IDX ),            \
  _mbg_cn_table_entry( PC_GPS_ALL_XMR_INFO ),                \
  _mbg_cn_table_entry( PC_GPS_ALL_XMR_STATUS ),              \
  _mbg_cn_table_entry( PC_GPS_XMR_HOLDOVER_STATUS ),         \
  _mbg_cn_table_entry( PC_GPS_ALL_GPIO_STATUS ),             \
  _mbg_cn_table_entry( PC_GPS_XFEATURE_BUFFER ),             \
  _mbg_cn_table_entry( PC_GPS_TLV_INFO ),                    \
                                                             \
  _mbg_cn_table_entry( PC_GPS_CFGH ),                        \
  _mbg_cn_table_entry( PC_GPS_ALM ),                         \
  _mbg_cn_table_entry( PC_GPS_EPH ),                         \
  _mbg_cn_table_entry( PC_GPS_UTC ),                         \
  _mbg_cn_table_entry( PC_GPS_IONO ),                        \
  _mbg_cn_table_entry( PC_GPS_ASCII_MSG ),                   \
  _mbg_cn_table_end()                                        \
}



/**
 * @brief Codes used with ::PC_GPS_CMD
 *
 * @note These commands should only used with care, in very rare cases!
 */
enum PC_GPS_COMMANDS     //##++++++++++++++
{
  PC_GPS_CMD_BOOT = 1,   ///< force a GPS receiver to boot mode
  PC_GPS_CMD_INIT_SYS,   ///< let the clock clear its system variables
  PC_GPS_CMD_INIT_USER,  ///< reset the clock's user parameters to defaults
  PC_GPS_CMD_INIT_DAC,   ///< initialize the oscillator disciplining values
  N_PC_GPS_CMD_CODES     ///< no command, just the number of known commands
};



/**
 * @brief A type used to store an unambiguous command code
 *
 * In case of the standard @ref PCPS_CMD_CODES the lower byte contains
 * the command code and the upper byte is 0.
 * In case of a GPS command the lower byte contains ::PCPS_READ_GPS_DATA
 * or ::PCPS_WRITE_GPS_DATA, as appropriate, and the upper byte contains
 * the associated type code from ::PC_GPS_CMD_CODES.
 *
 * Used internally by the firmware only.
 */
typedef uint16_t PCPS_CMD_INFO;


#if defined( _USING_BYTE_ALIGNMENT )
  #pragma pack()      // set default alignment
  #undef _USING_BYTE_ALIGNMENT
#endif

/* End of header body */

#endif  /* _PCPSDEFS_H */

