
/**************************************************************************
 *
 *  $Id: mbgtime.h 1.42 2020/08/10 16:31:30 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions and prototypes for mbgtime.c.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgtime.h $
 *  Revision 1.42  2020/08/10 16:31:30  martin
 *  Fixed 'long long' suffix for Windows builds.
 *  Revision 1.41  2019/11/27 11:21:39Z  martin
 *  Renamed function n_days() to n_days_since_year_0()
 *  to make clearer what the function returns, and provided
 *  a backward-compatible inline function n_days().
 *  Updated some doxygen comments.
 *  Revision 1.40  2019/11/27 11:14:39  martin
 *  Added definition POSIX_1970_INITIAL_DAY.
 *  Revision 1.39  2019/11/27 11:09:58  martin
 *  Added definitions of SMPTE epoch offsets from GPS epoch.
 *  Revision 1.38  2019/09/27 14:53:20  martin
 *  New define MONTHS_PER_YEAR.
 *  New types MBG_TIME32_T MBG_TIME32U_T.
 *  New inline function mbg_exp_year().
 *  New inline functions normalize_gps_wn_dn() and de_normalize_gps_wn_dn().
 *  New definitions for different types of leap second tables.
 *  New function is_valid_leap_second_date() and variants thereof.
 *  Updated function prototypes.
 *  Revision 1.37  2019/08/28 13:19:40  martin
 *  New structures MBG_TZ_INFO and MBG_LS_INFO which
 *  are used by some new functions in new module mbgtimex.c.
 *  Revision 1.36  2019/08/02 07:51:30  martin
 *  New type MBG_TIME64_T.
 *  Revision 1.35  2019/07/23 07:27:37  martin
 *  Added definition GPS_INITIAL_DAY.
 *  Revision 1.34  2019/07/19 08:39:19  martin
 *  Don't mark frac_sec_from_bin() as deprecated anymore.
 *  Revision 1.33  2019/07/19 07:49:22Z  martin
 *  Modified dec_frac_to_bin_frac_{16,32} so that no compiler
 *  warning is emitted if a large data type is passed as an argument.
 *  Revision 1.32  2019/06/26 10:03:02Z  martin
 *  Updated function prototypes.
 *  Revision 1.31  2019/02/06 10:08:09  martin
 *  Added symbol HNS_PER_MIN.
 *  Revision 1.30  2018/11/26 12:04:44Z  martin
 *  Moved definition NTP_FRAC_PER_SEC here.
 *  Revision 1.29  2018/02/28 16:58:10  martin
 *  Removed reference to frac_sec_from_bin().
 *  Revision 1.28  2018/01/15 18:18:49  martin
 *  Renamed symbol NSECS_PER_SEC to NSEC_PER_SEC.
 *  according to namings of similar symbols.
 *  Revision 1.27  2017/11/29 11:14:57  gregoire
 *  Added Multiplier MSEC_TO_NSEC_MULTIPLIER, MSEC_TO_USEC_MULTIPLIER.
 *  Revision 1.26  2017/11/16 13:33:46  philipp
 *  Added USEC_PER_SEC define.
 *  Revision 1.25  2017/08/15 15:48:59  martin
 *  Define NSECS_PER_SEC only if it hasn't been defined before.
 *  Revision 1.24  2017/07/04 14:02:25  martin
 *  Made definitions of some constants signed to avoid
 *  signed/unsigned compiler warnings.
 *  Moved some macros and inline functions for fraction
 *  conversion here. They are excluded from build in kernel
 *  mode, though, since some kernels don't support this properly.
 *  Moved some NANO_TIME- and NANO_TIME_64-related inline
 *  functions to new module nanotime.c.
 *  Renamed PCPS_HRT_BIN_FRAC_SCALE to MBG_FRAC32_UNITS_PER_SEC.
 *  Updated function prototypes.
 *  Doxygen stuff.
 *  Revision 1.23  2017/03/16 12:26:13  martin
 *  Updated function prototypes.
 *  Revision 1.22  2017/01/25 13:10:55  gregoire.diehl
 *  nano_time_64_to_double and double_to_nano_time_64 added
 *  Revision 1.21  2016/12/15 17:44:59Z  martin
 *  Changed conditions to include time.h.
 *  Fixed spelling.
 *  Removed trailing spaces.
 *  Revision 1.20  2014/05/27 08:09:19  martin
 *  Added NTP_SEC_BIAS.
 *  Revision 1.19  2013/05/22 16:47:01  martin
 *  Added some useful macros.
 *  Revision 1.18  2012/10/02 18:51:11  martin
 *  Include <time.h> for WIN32 target and firmware only
 *  Fixed build under QNX, DOS, and FreeBSD.
 *  Revision 1.17  2010/08/06 13:03:03  martin
 *  Removed obsolete code.
 *  Revision 1.16  2010/07/16 10:22:07Z  martin
 *  Moved definitions of HNS_PER_SEC and HNS_PER_MS here.
 *  Conditionally define FILETIME_1970.
 *  Defined MASK_CLOCK_T for ARM/Cortex.
 *  Revision 1.15  2009/10/23 09:55:21  martin
 *  Added MJD numbers for commonly used epochs.
 *  Revision 1.14  2009/08/12 10:28:12  daniel
 *  Added definition NSECS_PER_SEC.
 *  Revision 1.13  2009/06/12 13:31:44Z  martin
 *  Fix build errors with arm-linux-gcc.
 *  Revision 1.12  2009/03/27 14:14:00  martin
 *  Cleanup for CVI.
 *  Revision 1.11  2009/03/13 09:30:06Z  martin
 *  Include mystd.h in mbgtime.c rather than here. The bit type used
 *  here is now defined in words.h.
 *  Updated comments for GPS_SEC_BIAS.
 *  Revision 1.10  2008/12/11 10:45:41Z  martin
 *  Added clock_t mask for gcc (GnuC).
 *  Revision 1.9  2006/08/25 09:33:46Z  martin
 *  Updated function prototypes.
 *  Revision 1.8  2004/12/28 11:29:02Z  martin
 *  Added macro _n_days.
 *  Updated function prototypes.
 *  Revision 1.7  2002/09/06 07:15:48Z  martin
 *  Added MASK_CLOCK_T for Linux.
 *  Revision 1.6  2002/02/25 08:37:44  Andre
 *  definition MASK_CLOCK_T for ARM added
 *  Revision 1.5  2001/03/02 10:18:10Z  MARTIN
 *  Added MASK_CLOCK_T for Watcom C.
 *  Revision 1.4  2000/09/15 07:57:53  MARTIN
 *  Removed outdated function prototypes.
 *  Revision 1.3  2000/07/21 14:05:18  MARTIN
 *  Defined some new constants.
 *
 **************************************************************************/

#ifndef _MBGTIME_H
#define _MBGTIME_H


/* Other headers to be included */

#include <gpsdefs.h>

#if !defined( MBG_TGT_KERNEL ) || defined( MBG_TGT_WIN32 )
  #include <time.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MBGTIME
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

/**
 * @defgroup leap_second_fncs Meinberg leap second support functions
 *
 * By default, the date and time ***at the end of a leap second***
 * is referred to as <em>leap second date</em>. This is because:
 *
 *  - At this point in time a new TAI offset comes into effect.
 *
 *  - This is precise both in case of an inserted leap second (when the
 *    numbered seconds are counted 59, 60, 0) as well as for a deleted
 *    leap second (when the numbered seconds jump from 58 to 0, and
 *    there is no second numbered 59).
 *
 *  - This complies with the leap second tables published by the
 *    International Earth Rotation and Reference Systems Service (IERS).
 *
 * So, for example, instead of 2016-12-31 23:59:59 we expect
 * 2017-01-01 00:00:00 as a valid leap second date.
 */

/**
 * @defgroup leap_date_valid_fncs Meinberg functions that check if a leap second date is valid
 * @ingroup leap_second_fncs
 *
 * For detailed hints, see @ref leap_second_fncs.
 *
 * Actually, only dates at the beginning of January or or July are
 * considered valid leap second dates. There are several functions:
 *
 * @see ::is_valid_leap_second_date_tm      A check function that expects a <em>struct tm</em> parameter.
 * @see ::is_valid_leap_second_date_tm_gps  A check function that expects a ::TM_GPS parameter.
 * @see ::is_valid_leap_second_date         A generic check function that expects day of month and month.
 *
 * @see @ref leap_second_fncs
 */

/**
 * @defgroup group_true_gps_wn_fncs Functions to determine a true leap second week number
 * @ingroup leap_second_fncs
 *
 * The ::UTC::WNlsf field originally transmitted by the GPS satellites
 * contains only the 8 LSBs of the full leap second week number, covering
 * only a range of +/- ~128 weeks from the current week number.
 *
 * GPS receivers try to derive a full extended week number
 * from this truncated week number.
 *
 * If the %UTC offset fields ::UTC::delta_tls and ::UTC::delta_tlsf
 * are different, this means that a leap second is currently being
 * announced, and thus the leap second week number is indeed in
 * the range of +/- ~128 week of the current week, which means
 * the week number can be unambiguously extended by the receiver.
 *
 * However, if ::UTC::delta_tls and ::UTC::delta_tlsf are the
 * same, no leap second is announced, and it's not clear in
 * which 256-week cycle the last leap second really occurred.
 * These functions can be used to try to resolve this ambiguity,
 * and, if possible, determine the true extended week number.
 *
 * The approach implemented here is based on a suggestion by
 * Tom van Baak [[tvb@leapsecond.com]] on the leap seconds mailing
 * list, and the fact that previous leap seconds were inserted only
 * at the end of June/beginning of July, or end of December/beginning
 * of January, and there are no plans to change this in the foreseeable
 * future.
 *
 * The truncated week number is expanded for subsequent 256-week
 * cycles, and if the extended week number and given day number
 * yield a date that matches one of these well-known leap second
 * dates then we have found the correct extended week number.
 *
 * A test has shown that there are unique results for 25 256-week
 * cycles from 1980, i.e. until year 2099, if only leap seconds
 * at the beginning of January or July are accepted. If the number of
 * 256-week cycles is extended beyond 25 / year 2099, or if leap
 * dates at the beginning of April or october are in addition taken
 * into account then the results can be ambiguous, i.e. there
 * can be more than one match.
 *
 * For example, both week numbers 1929 (0x0789, real week number)
 * and 2185 (0x0889, off by 256 weeks) with correct day 7 yield
 * the real leap second date 2017-01-01.
 *
 * @see ::mbg_find_true_gps_wn_lsf
 * @see ::mbg_find_true_gps_wn_lsf_ex
 * @see ::find_past_gps_wn_lsf_from_table
 */



/**
 * @defgroup group_timestamp_types Data types to store timestamps.
 *
 * Depending on the build and target environment, the original
 * POSIX @a time_t type is usually 32 bits or 64 bits wide.
 * If it is only 32 bits wide, timestamps will overflow in 2038.
 *
 * Using 64 bit types and associated time conversion functions
 * avoid a rollover in 2038.
 *
 * Using a distinct 32 bit type can be useful for API calls which
 * return 32 bit timestamps only, which may need to be mapped to
 * an extended 64 bit range.
 */

/**
 * @brief A POSIX-like timestamp, 64 bits wide, signed.
 *
 * Negative numbers can represent times before the epoch.
 *
 * @ingroup group_timestamp_types
 * @see @ref group_timestamp_types
 */
typedef int64_t MBG_TIME64_T;


/**
 * @brief A POSIX-like timestamp, 32 bits wide, signed.
 *
 * Negative numbers can represent times before the epoch,
 * but rolls over earlier than the unsigned type ::MBG_TIME32U_T.
 *
 * @ingroup group_timestamp_types
 * @see @ref group_timestamp_types
 */
typedef int32_t MBG_TIME32_T;


/**
 * @brief A POSIX-like timestamp, 32 bits wide, unsigned.
 *
 * No negative numbers, so times before the epoch
 * can't be represented, but rolls over later
 * than the signed type ::MBG_TIME32_T.
 *
 * @ingroup group_timestamp_types
 * @see @ref group_timestamp_types
 */
typedef uint32_t MBG_TIME32U_T;



/**
 * @brief The number of days from 0000-01-01 until POSIX epoch 1970-01-01.
 *
 * The number of days as computed by ::n_days_since_year_0 for the
 * POSIX time_t epoch, 1970-01-01, which was a Thursday.
 *
 * @see ::n_days_since_year_0
 */
#define POSIX_1970_INITIAL_DAY  719162L



/**
 * @brief The number of days from 0000-01-01 until GPS epoch.
 *
 * The number of days as computed by ::n_days_since_year_0 for the
 * date of the GPS epoch, 1980-01-06, which was a Sunday.
 *
 * @see ::n_days_since_year_0
 */
#define GPS_INITIAL_DAY  722819L


/**
 * @brief GPS epoch bias from ordinary POSIX time_t epoch.
 *
 * The POSIX time_t epoch is usually 1970-01-01 00:00:00, whereas
 * the GPS epoch is 1980-01-06 00:00, so the difference is 10 years,
 * plus 2 days due to leap years (1972 and 1976), plus the difference
 * of the day-of-month (6 - 1), so:<br>
 *
 * time_t t = ( gps_week * ::SECS_PER_WEEK ) + sec_of_week + ::GPS_SEC_BIAS
 */
#define GPS_SEC_BIAS   315964800UL     // ( ( ( 10UL * 365UL ) + 2 + 5 ) * SECS_PER_DAY )


/**
 * @brief NTP epoch bias from ordinary POSIX time_t epoch.
 *
 * The POSIX time_t epoch is usually 1970-01-01 00:00:00, whereas
 * the NTP epoch is 1900-01-01 00:00:00, so the difference is
 * a constant number of seconds:<br>
 *
 * time_t t = ntp_time - ::NTP_SEC_BIAS
 */
#define NTP_SEC_BIAS   2208988800UL

#if !defined( MBG_TGT_MISSING_64_BIT_TYPES )
  #define NTP_FRAC_PER_SEC         (uint64_t) 4294967296.0
#endif



/**
 * @defgroup group_smpte_epoch_offsets_gps Predefined SMPTE epoch offsets from GPS epoch
 *
 * With these constants, timestamps of the GPS time scale
 * can be converted to a particular time scale used by SMPTE,
 * e.g. to calculate a specific video frame number.
 *
 * @anchor MBG_SMPTE_EPOCH_OFFSETS
 * @see ::MBG_GPIO_VIDEO_EPOCHS
 *
 * @{ */


/**
 * @brief SMPTE epoch offset from GPS to 1970-01-01T00:00:00 TAI.
 *
 * The total offset consists of the difference between GPS epoch
 * and POSIX/UTC epoch (::GPS_SEC_BIAS), plus the constant offset
 * between GPS time scale and TAI (::GPS_TAI_OFFSET).
 *
 * @note This is the standard epoch referred to
 * by the PTP/IEEE1588 SMPTE extension.
 *
 * @see ::SMPTE_TAI_EPOCH_1970
 */
#define SMPTE_GPS_OFFSET_TAI_1970  ( GPS_SEC_BIAS + GPS_TAI_OFFSET )


/**
 * @brief SMPTE epoch offset from GPS to 1958-01-01T00:00:00 TAI.
 *
 * The total offset consists of the difference between GPS epoch
 * and POSIX/UTC epoch (::GPS_SEC_BIAS), plus 12 years since 1958,
 * 3 of which were leap years that had an additional day, plus the
 * constant offset between GPS time scale and TAI (::GPS_TAI_OFFSET).
 *
 * @note The TAI time scale didn't exist in 1958, and time scales were
 * shifted/adjusted/aligned between 1958 and 1972, so this offset is just
 * a hypothetic extrapolation into the past, which may be defined or
 * implemented in different ways by different manufacturers or projects.
 *
 * @see ::SMPTE_TAI_EPOCH_1958
 */
#define SMPTE_GPS_OFFSET_TAI_1958  ( GPS_SEC_BIAS + ( ( 12 * 365 + 3 ) * SECS_PER_DAY ) + GPS_TAI_OFFSET )


/**
 * @brief SMPTE epoch offset from GPS to 1972-01-01T00:00:00 UTC.
 *
 * The total offset consists of the difference between GPS epoch
 * and POSIX/UTC epoch (::GPS_SEC_BIAS), minus 2 years until 1972,
 * none of which were leap years that had an additional day.
 *
 * @see ::SMPTE_UTC_EPOCH_1972
 */
#define SMPTE_GPS_OFFSET_UTC_1972  ( GPS_SEC_BIAS - ( ( 2 * 365 + 0 ) * SECS_PER_DAY ) )


/**
 * @brief SMPTE epoch offset from GPS to 1980-06-01T00:00:00 GPS.
 *
 * In this case the SMPTE epoch is the same as the GPS epoch,
 * so the offset is 0.
 *
 * @see ::SMPTE_GPS_EPOCH_1980
 */
#define SMPTE_GPS_OFFSET_GPS_1980  ( 0 )


/** @} defgroup group_ptp_smpte_epoch_offsets_gps */



/**
 * @defgroup group_mjd_numbers Modified Julian Day (MJD) numbers for some commonly used epochs
 *
 * To compute the MJD for a given date just compute the days since epoch
 * and add the constant number of days according to the epoch, e.g. for POSIX:
 *
 *   current_unix_mjd = ( time( NULL ) / SECS_PER_DAY ) + MJD_AT_POSIX_EPOCH;
 *
 * @anchor MBG_MJD_NUMBERS
 *
 * @{ */

#define MJD_AT_GPS_EPOCH    44244UL    ///< MJD at 1980-01-06, see @ref MBG_MJD_NUMBERS
#define MJD_AT_POSIX_EPOCH  40587UL    ///< MJD at 1970-01-01, see @ref MBG_MJD_NUMBERS
#define MJD_AT_NTP_EPOCH    40587UL    ///< MJD at 1900-01-01, see @ref MBG_MJD_NUMBERS

/** @} defgroup group_mjd_numbers */



/**
 * @brief The Windows FILETIME value at the usual POSIX epoch.
 *
 * The Windows @a FILETIME counts the number of 100 ns intervals
 * since 1601-01-01, while the POSIX @a time_t epoch is usually
 * (but not necessarily) 1970-01-01.
 */
#if !defined( FILETIME_1970 )
  // FILETIME represents a 64 bit number, so we need to defined the
  // constant with an appendix depending on the compiler.
  #if MBG_TGT_C99 || defined( __GNUC__ )
    // syntax introduced by C99 standard
    #define FILETIME_1970    0x019db1ded53e8000ULL
  #elif defined( MBG_TGT_WIN32 )
    // MSC-specific syntax
    #define FILETIME_1970    0x019db1ded53e8000ui64
  #endif
#endif


#if defined( _C166 )
  #if _C166 >= 50
    #define MASK_CLOCK_T 0x7FFFFFFFL
  #else
    #define MASK_CLOCK_T 0x7FFF   /* time.h not shipped with compiler */
  #endif
#endif

#if defined( __WATCOMC__ )
  #define MASK_CLOCK_T 0x7FFFFFFFL
#endif

#if defined( _CVI ) || defined( _CVI_ )
  #define MASK_CLOCK_T 0x7FFFFFFFL
#endif

#if defined( _MSC_VER )
  #define MASK_CLOCK_T 0x7FFFFFFFL
#endif

#if defined( __NETWARE_386__ )
  #define MASK_CLOCK_T 0x7FFFFFFFL
#endif

#if defined( __ARM )
  #define MASK_CLOCK_T 0x7FFFFFFFL
#endif

#if defined( __ARMCC_VERSION )
  #define MASK_CLOCK_T ( ( (ulong) (clock_t) -1 ) >> 1 )
#endif

#if defined( __GNUC__ )
  #if defined( __linux )
    #define MASK_CLOCK_T ( ( (ulong) (clock_t) -1 ) >> 1 )
  #else  // Windows / MinGW
    #define MASK_CLOCK_T 0x7FFFFFFFL
  #endif
#endif


#if !defined( MASK_CLOCK_T )
  #if sizeof( clock_t ) == sizeof( short )
    #define MASK_CLOCK_T 0x7FFF
  #elif sizeof( clock_t ) == sizeof( long )
    #define MASK_CLOCK_T 0x7FFFFFFFL
  #endif
#endif

typedef struct
{
  clock_t start;
  clock_t stop;
  short is_set;

} TIMEOUT;

#define MONTHS_PER_YEAR   12

#define DAYS_PER_WEEK     7

#define SECS_PER_MIN      60
#define MINS_PER_HOUR     60
#define HOURS_PER_DAY     24
#define DAYS_PER_WEEK     7

#define MINS_PER_DAY      ( MINS_PER_HOUR * HOURS_PER_DAY )

#define SECS_PER_HOUR     3600
#define SECS_PER_DAY      86400L
#define SECS_PER_WEEK     604800L

#define SEC100S_PER_SEC   100L
#define SEC100S_PER_MIN   ( SEC100S_PER_SEC * SECS_PER_MIN )
#define SEC100S_PER_HOUR  ( SEC100S_PER_SEC * SECS_PER_HOUR )
#define SEC100S_PER_DAY   ( SEC100S_PER_SEC * SECS_PER_DAY )

#if !defined( MSEC_PER_SEC )
  #define MSEC_PER_SEC    1000L
#endif

#define MSEC_PER_MIN      ( MSEC_PER_SEC * SECS_PER_MIN )
#define MSEC_PER_HOUR     ( MSEC_PER_SEC * SECS_PER_HOUR )
#define MSEC_PER_DAY      ( MSEC_PER_SEC * SECS_PER_DAY )

#if !defined( USEC_PER_SEC )
  #define USEC_PER_SEC    1000000L
#endif

#if !defined( NSEC_PER_SEC )
  #define NSEC_PER_SEC   1000000000L
#endif

#if !defined( HNS_PER_SEC )
  #define HNS_PER_SEC     10000000L
#endif

#if !defined( HNS_PER_MS )
  #define HNS_PER_MS      10000L
#endif

#if !defined( HNS_PER_MIN )
  #define HNS_PER_MIN     ( HNS_PER_SEC * SECS_PER_MIN )
#endif

#define MSEC_TO_NSEC_MULTIPLIER    ( NSEC_PER_SEC / MSEC_PER_SEC )
#define MSEC_TO_USEC_MULTIPLIER    ( USEC_PER_SEC / MSEC_PER_SEC )


/**
 * @brief A table with the days of month
 *
 * First row is for standard years, second row is
 * for leap years.
 *
 * @see DAYS_OF_MONTH_TABLE_INIT
 */
typedef char DAYS_OF_MONTH_TABLE[2][12];


/**
 * @brief An initializer for a ::DAYS_OF_MONTH_TABLE
 */
#define DAYS_OF_MONTH_TABLE_INIT                      \
{                                                     \
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, \
  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }  \
}




_ext TM_GPS dhms;
_ext TM_GPS datum;


_ext const char *short_time_fmt
#ifdef _DO_INIT
 = "%2i:%02i"
#endif
;

_ext const char *time_fmt
#ifdef _DO_INIT
 = "%2i:%02i:%02i"
#endif
;

_ext const char *long_time_fmt
#ifdef _DO_INIT
 = "%2i:%02i:%02i.%02i"
#endif
;

_ext const char *date_fmt
#ifdef _DO_INIT
 = "%2i.%02i.%04i"
#endif
;

_ext const char *day_date_fmt
#ifdef _DO_INIT
 = "%s, %2i.%02i.%04i"
#endif
;

_ext const char *day_name_eng[]
#ifdef _DO_INIT
 = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" }
#endif
;

_ext const char *day_name_ger[]
#ifdef _DO_INIT
 = { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" }
#endif
;

_ext const TM_GPS init_tm
#ifdef _DO_INIT
  = { 1980, 1, 1, 0, 0, 0, 0, 0, 0, 0 }
#endif
;


_ext DAYS_OF_MONTH_TABLE days_of_month
#ifdef _DO_INIT
 = DAYS_OF_MONTH_TABLE_INIT
#endif
;


// simplify call to n_days with structures
#define _n_days( _s ) \
  n_days_since_year_0( (_s)->mday, (_s)->month, (_s)->year )


#define _is_leap_year( _y ) \
  ( ( ( ( (_y) % 4 ) == 0 ) && ( ( (_y) % 100 ) != 0 ) ) || ( ( (_y) % 400 ) == 0 ) )


#define _get_days_of_month( _y, _m ) \
  days_of_month[ _is_leap_year( _y ) ][_m]



static __mbg_inline /*HDR*/
/**
 * @brief Expand a 2-digit year-of-the-century to a full 4-digit year number.
 *
 * The resulting year number includes the century and is
 * in the range [year_lim ... ( year_lim + 99 )].
 *
 * @param[in] year      The 2-digit year-of-the-century to be converted.
 *
 * @param[in] year_lim  The smallest 4-digit year number to be returned.
 *
 * @return The resulting 4 digit year number including century.
 */
int mbg_exp_year( int year, int year_lim )
{
  int lyear = year + year_lim - ( year_lim % 100 );

  if ( lyear < year_lim )
    lyear += 100;

  return lyear;

}  // mbg_exp_year



static __mbg_inline /*HDR*/
/**
 * @brief Normalize a GPS week number / day number pair.
 *
 * GPS navigation data may contain week number / day number pairs where
 * the day number is in the range 1..7 rather than 0..6. For example,
 * a known leap second date was sent with 1929|7 instead of 1930|0.
 *
 * To resolve this ambiguity in computations, this function can be used
 * to normalize the wn|dn pair.
 *
 * The complementary function ::de_normalize_gps_wn_dn can be used
 * to convert a wn|dn pair back to the common wn|dn format where the
 * day number is in the range 1..7.
 *
 * @param[in,out]  p_wn  Address of a variable containing the week number.
 * @param[in,out]  p_dn  Address of a variable containing the day number.
 *
 * @see ::de_normalize_gps_wn_dn
 */
void normalize_gps_wn_dn( GPS_WNUM *p_wn, GPS_DNUM *p_dn )
{
  if ( *p_dn == 7 )
  {
    *p_dn = 0;
    (*p_wn)++;
  }

}  // normalize_gps_wn_dn



static __mbg_inline /*HDR*/
/**
 * @brief De-normalize a GPS week number / day number pair.
 *
 * GPS navigation data may contain week number / day number pairs where
 * the day number is in the range 1..7 rather than 0..6. For example,
 * a known leap second date was sent with 1929|7 instead of 1930|0.
 *
 * However, the function ::normalize_gps_wn_dn may have been called before
 * to normalize the wn|dn pair to simplify computations.
 *
 * This function can be used to convert a wn|dn pair back to the common
 * wn|dn format where the day number is in the range 1..7.
 *
 * @param[in,out]  p_wn  Address of a variable containing the week number.
 * @param[in,out]  p_dn  Address of a variable containing the day number.
 *
 * @see ::normalize_gps_wn_dn
 */
void de_normalize_gps_wn_dn( GPS_WNUM *p_wn, GPS_DNUM *p_dn )
{
  if ( *p_dn == 0 )
  {
    *p_dn = 7;
    (*p_wn)--;
  }

}  // de_normalize_gps_wn_dn



#if !defined( MBG_TGT_KERNEL )

/**
 * @brief Data type used for intermediate results on 32 bit multiplications
 *
 * We need 64 bits for intermediate integer results to avoid a range overflow
 * from 32 x 32 bit multiplications. On systems which don't support 64 bit types
 * we use the "double" type as a workaround.
 */
#if defined( MBG_TGT_MISSING_64_BIT_TYPES )
  #define MBG_FRAC32_CONVERSION_TYPE double
#else
  #define MBG_FRAC32_CONVERSION_TYPE int64_t
#endif

/**
 * @brief Constant used to convert e.g. ::PCPS_TIME_STAMP::frac values
 *
 * Max value of ::PCPS_TIME_STAMP::frac + 1, used for scaling
 */
#define MBG_FRAC32_UNITS_PER_SEC  ( (MBG_FRAC32_CONVERSION_TYPE) 4294967296.0  )  // == 0x100000000



/**
 * @brief DST on/off times pre-computed for a given year.
 *
 * Used like a cache to avoid redundant expensive computation
 * of DST switching times for a given year inside an application.
 * Not to be used for data exchange between devices.
 *
 * By default, switching times @a #t_on and @a #t_off are stored
 * as local standard time (i.e. %UTC + @a #offs already applied)
 * because this is most suitable for %UTC to local time conversions.
 *
 * However, for some cases (e.g. with PTP SMPTE) the switching
 * times need to be compared to TAI times, so the function
 * ::mbg_tz_info_to_tai can be used to convert the switching
 * times to TAI. Care must be taken that the TAI switching times
 * also need to be updated whenever a leap second event occurrs.
 *
 * @see ::mbg_set_tz_info_for_year
 * @see ::mbg_set_tz_info_for_utc_time64_t
 * @see ::mbg_tz_info_to_tai
 */
typedef struct
{
  MBG_TIME64_T t_on;   ///< 'DST on' time, local standard time (default), or TAI.
  MBG_TIME64_T t_off;  ///< 'DST off' time, local standard time (default), or TAI.
  int offs;            ///< Offset to be added to %UTC to yield local standard time [sec].
  int offs_dl;         ///< Additional offset to be added if daylight saving is in effect [sec].
  int year;            ///< The year number for which @a #t_on and @a #t_off have been computed.
  int auto_flag;       ///< A flag indicating that @a #t_on and @a #t_off were computed by automatic rules.
  int valid;           ///< A flag indicating that the information in this structure has been set up.

} MBG_TZ_INFO;



/**
 * @brief Current %UTC/TAI Offset And Leap Second Information.
 *
 * The stored information can be retrieved e.g. from the
 * ::UTC data set transmitted by the GPS satellites,
 * or from an NTP leap second file.
 *
 * @see ::mbg_set_ls_info_from_gps_utc
 */
typedef struct
{
  /// @brief Time of the nearest leap second, if available, in %UTC time scale.
  /// Should match %UTC midnight at the end of the last day in June or December of a given year.
  MBG_TIME64_T t64_ls_utc;

  /// @brief Time of the nearest leap second, if available, in TAI time scale.
  /// Should be ahead of @a #t64_ls_utc by a number of leap seconds (~37 s in year 2019)
  /// that have already been inserted in the past.
  MBG_TIME64_T t64_ls_tai;

  /// @brief Number of seconds to be inserted into the %UTC time scale at the leap second transition.
  /// This is 0 as long as no leap second announcement is currently available, +1 for a leap
  /// second to be inserted, and -1 for a leap second to be deleted, which has yet never happened.
  int ls_step;

  int offs_gps_utc;  ///< Number of seconds the GPS system time is ahead of %UTC after the leap second transition.

  int offs_tai_utc;  ///< Number of seconds TAI is ahead of %UTC after the leap second transition.

  int valid;         ///< Indicates that the structure has been set up.

} MBG_LS_INFO;



/**
 * @brief Entry of a leap second table providing timestamps and %UTC/TAI offsets.
 *
 * There are initializers that can be used to instantiate a table of entries.
 *
 * @see ::KNOWN_LEAP_SECOND_INFO_NTP
 * @see ::KNOWN_LEAP_SECOND_INFO_POSIX
 * @see ::LS_TABLE_ENTRY_GPS
 */
typedef struct
{
  /// Timestamp associated with ***the end*** of the leap second,
  /// e.g. 2017-01-01 00:00:00 instead of 2016-12-31 23:59:59.
  MBG_TIME64_T t_ls;

  /// Number of seconds that %UTC is ***behind*** TAI
  /// after the given time.
  int utc_tai_offs;

} LS_TABLE_ENTRY;



/**
 * @brief Entry of a leap second table providing GPS timestamps and %UTC/GPS offsets.
 *
 * There is an initializer that can be used to instantiate a table of entries.
 *
 * ***Please note*** the table contains normalized wn|dn pairs like 1930|0,
 * while the ::UTC info sent by the satellites could be 1929|7. So the wn|dn
 * pairs from a GPS receiver should be normalized before being compared.
 *
 * @see ::KNOWN_LEAP_SECOND_INFO_GPS
 * @see ::LS_TABLE_ENTRY
 */
typedef struct
{
  /// Extended GPS week number during which a leap second occurs.
  GPS_WNUM wn;

  /// A day-of-week indicating when the leap second occurs.
  /// Associated with ***the end*** of the leap second,
  /// e.g. 2017-01-01 00:00:00 instead of 2016-12-31 23:59:59.
  GPS_DNUM dn;

  /// Number of seconds that %UTC is ***behind*** GPS
  /// after the given time.
  int gps_tai_offs;

} LS_TABLE_ENTRY_GPS;


#if defined( MBG_TGT_WIN32 ) && defined( _MSC_VER )
  #define _ls_ntp_ts( _x )  ( _x ## i64 )
#else
  #define _ls_ntp_ts( _x )  ( _x ## LL )
#endif


/**
 * @brief Initializer for a table of known leap seconds, with NTP times.
 *
 * Can be used to initialize a table of ::LS_TABLE_ENTRY entries.
 * Values have been copied and can be updated from an NTP leap second file.
 *
 * The table can be incomplete after another leap second has been scheduled,
 * unless this initializer is updated.
 *
 * The first value is the number of seconds since the NTP epoch, 1900-01-01,
 * and the second value indicates how many seconds %UTC is ***behind TAI***
 * after the given date.
 *
 * There is also an initializer which uses a POSIX time_t for the date, and
 * another one where the date is specified by a GPS week number plus day number.
 *
 * @see ::LS_TABLE_ENTRY
 * @see ::KNOWN_LEAP_SECOND_INFO_POSIX
 * @see ::KNOWN_LEAP_SECOND_INFO_GPS
 */
#define KNOWN_LEAP_SECOND_INFO_NTP       \
{                                        \
  { _ls_ntp_ts( 2272060800 ), 10 },  /* 1 Jan 1972 */  \
  { _ls_ntp_ts( 2287785600 ), 11 },  /* 1 Jul 1972 */  \
  { _ls_ntp_ts( 2303683200 ), 12 },  /* 1 Jan 1973 */  \
  { _ls_ntp_ts( 2335219200 ), 13 },  /* 1 Jan 1974 */  \
  { _ls_ntp_ts( 2366755200 ), 14 },  /* 1 Jan 1975 */  \
  { _ls_ntp_ts( 2398291200 ), 15 },  /* 1 Jan 1976 */  \
  { _ls_ntp_ts( 2429913600 ), 16 },  /* 1 Jan 1977 */  \
  { _ls_ntp_ts( 2461449600 ), 17 },  /* 1 Jan 1978 */  \
  { _ls_ntp_ts( 2492985600 ), 18 },  /* 1 Jan 1979 */  \
  { _ls_ntp_ts( 2524521600 ), 19 },  /* 1 Jan 1980 */  \
  { _ls_ntp_ts( 2571782400 ), 20 },  /* 1 Jul 1981 */  \
  { _ls_ntp_ts( 2603318400 ), 21 },  /* 1 Jul 1982 */  \
  { _ls_ntp_ts( 2634854400 ), 22 },  /* 1 Jul 1983 */  \
  { _ls_ntp_ts( 2698012800 ), 23 },  /* 1 Jul 1985 */  \
  { _ls_ntp_ts( 2776982400 ), 24 },  /* 1 Jan 1988 */  \
  { _ls_ntp_ts( 2840140800 ), 25 },  /* 1 Jan 1990 */  \
  { _ls_ntp_ts( 2871676800 ), 26 },  /* 1 Jan 1991 */  \
  { _ls_ntp_ts( 2918937600 ), 27 },  /* 1 Jul 1992 */  \
  { _ls_ntp_ts( 2950473600 ), 28 },  /* 1 Jul 1993 */  \
  { _ls_ntp_ts( 2982009600 ), 29 },  /* 1 Jul 1994 */  \
  { _ls_ntp_ts( 3029443200 ), 30 },  /* 1 Jan 1996 */  \
  { _ls_ntp_ts( 3076704000 ), 31 },  /* 1 Jul 1997 */  \
  { _ls_ntp_ts( 3124137600 ), 32 },  /* 1 Jan 1999 */  \
  { _ls_ntp_ts( 3345062400 ), 33 },  /* 1 Jan 2006 */  \
  { _ls_ntp_ts( 3439756800 ), 34 },  /* 1 Jan 2009 */  \
  { _ls_ntp_ts( 3550089600 ), 35 },  /* 1 Jul 2012 */  \
  { _ls_ntp_ts( 3644697600 ), 36 },  /* 1 Jul 2015 */  \
  { _ls_ntp_ts( 3692217600 ), 37 },  /* 1 Jan 2017 */  \
  /* =============================== */  \
  /* If a new entry is added, don't  */  \
  /* forget to update the tables     */  \
  /* KNOWN_LEAP_SECOND_INFO_POSIX    */  \
  /* and KNOWN_LEAP_SECOND_INFO_GPS  */  \
  /* accordingly.                    */  \
  /* =============================== */  \
  { 0, 0  }    /* end-of-table */        \
}



#define _ls_ntp_to_posix( _x )  ( _ls_ntp_ts( _x ) - NTP_SEC_BIAS )

/**
 * @brief Initializer for a table of known leap seconds, with POSIX times.
 *
 * Can be used to initialize a table of ::LS_TABLE_ENTRY entries. Numeric
 * values have been copied and can be updated from an NTP leap second file,
 * and an appropriate macro is used to convert the NTP times to POSIX.
 *
 * The table can be incomplete after another leap second has been scheduled,
 * unless this initializer is updated.
 *
 * The resulting first value is the number of seconds since the POSIX epoch,
 * 1970-01-01, and the second value indicates how many seconds %UTC is
 * ***behind TAI*** after the given date.
 *
 * There is also an initializer which uses an NTP time for the date, and
 * another one where the date is specified by a GPS week number
 * plus day number.
 *
 * @see ::LS_TABLE_ENTRY
 * @see ::KNOWN_LEAP_SECOND_INFO_NTP
 * @see ::KNOWN_LEAP_SECOND_INFO_GPS
 */
#define KNOWN_LEAP_SECOND_INFO_POSIX                         \
{                                                            \
  { _ls_ntp_to_posix( 2272060800 ), 10 },  /* 1 Jan 1972 */  \
  { _ls_ntp_to_posix( 2287785600 ), 11 },  /* 1 Jul 1972 */  \
  { _ls_ntp_to_posix( 2303683200 ), 12 },  /* 1 Jan 1973 */  \
  { _ls_ntp_to_posix( 2335219200 ), 13 },  /* 1 Jan 1974 */  \
  { _ls_ntp_to_posix( 2366755200 ), 14 },  /* 1 Jan 1975 */  \
  { _ls_ntp_to_posix( 2398291200 ), 15 },  /* 1 Jan 1976 */  \
  { _ls_ntp_to_posix( 2429913600 ), 16 },  /* 1 Jan 1977 */  \
  { _ls_ntp_to_posix( 2461449600 ), 17 },  /* 1 Jan 1978 */  \
  { _ls_ntp_to_posix( 2492985600 ), 18 },  /* 1 Jan 1979 */  \
  { _ls_ntp_to_posix( 2524521600 ), 19 },  /* 1 Jan 1980 */  \
  { _ls_ntp_to_posix( 2571782400 ), 20 },  /* 1 Jul 1981 */  \
  { _ls_ntp_to_posix( 2603318400 ), 21 },  /* 1 Jul 1982 */  \
  { _ls_ntp_to_posix( 2634854400 ), 22 },  /* 1 Jul 1983 */  \
  { _ls_ntp_to_posix( 2698012800 ), 23 },  /* 1 Jul 1985 */  \
  { _ls_ntp_to_posix( 2776982400 ), 24 },  /* 1 Jan 1988 */  \
  { _ls_ntp_to_posix( 2840140800 ), 25 },  /* 1 Jan 1990 */  \
  { _ls_ntp_to_posix( 2871676800 ), 26 },  /* 1 Jan 1991 */  \
  { _ls_ntp_to_posix( 2918937600 ), 27 },  /* 1 Jul 1992 */  \
  { _ls_ntp_to_posix( 2950473600 ), 28 },  /* 1 Jul 1993 */  \
  { _ls_ntp_to_posix( 2982009600 ), 29 },  /* 1 Jul 1994 */  \
  { _ls_ntp_to_posix( 3029443200 ), 30 },  /* 1 Jan 1996 */  \
  { _ls_ntp_to_posix( 3076704000 ), 31 },  /* 1 Jul 1997 */  \
  { _ls_ntp_to_posix( 3124137600 ), 32 },  /* 1 Jan 1999 */  \
  { _ls_ntp_to_posix( 3345062400 ), 33 },  /* 1 Jan 2006 */  \
  { _ls_ntp_to_posix( 3439756800 ), 34 },  /* 1 Jan 2009 */  \
  { _ls_ntp_to_posix( 3550089600 ), 35 },  /* 1 Jul 2012 */  \
  { _ls_ntp_to_posix( 3644697600 ), 36 },  /* 1 Jul 2015 */  \
  { _ls_ntp_to_posix( 3692217600 ), 37 },  /* 1 Jan 2017 */  \
  /* =================================================== */  \
  /*   If a new entry is added, don't forget to update   */  \
  /*   the tables KNOWN_LEAP_SECOND_INFO_NTP and         */  \
  /*   KNOWN_LEAP_SECOND_INFO_GPS accordingly.           */  \
  /* =================================================== */  \
  { 0, 0 }           /* end-of-table */                      \
}



#define _ls_ntp_to_gps( _x )  \
  ( ( _ls_ntp_ts( _x ) - NTP_SEC_BIAS - GPS_SEC_BIAS ) / SECS_PER_WEEK ),  \
  ( ( ( _ls_ntp_ts( _x ) - NTP_SEC_BIAS - GPS_SEC_BIAS ) % SECS_PER_WEEK ) / SECS_PER_DAY )

#define _ls_utc_offs_to_gps( _x )  ( (_x) - GPS_TAI_OFFSET )

/**
 * @brief Initializer for a table of known leap seconds, with GPS times.
 *
 * Can be used to initialize a table of ::LS_TABLE_ENTRY_GPS entries. Numeric
 * values have been copied and can be updated from an NTP leap second file,
 * and appropriate macros are used to convert the NTP times to GPS time.
 *
 * The table can be incomplete after another leap second has been scheduled,
 * unless this initializer is updated.
 *
 * The resulting first value is an extended GPS week number, the second value
 * is a day-of-week number, and the third value indicates how many seconds %UTC
 * is ***behind GPS*** after the given date.
 *
 * ***Please note*** the resulting table does not include leap seconds before
 * the GPS epoch, and it contains normalized wn|dn pairs like 1930|0, while
 * the ::UTC info sent by the satellites could be 1929|7. So the dates from
 * a GPS receiver should be normalized before being compared.
 *
 * There is also an initializer which uses an NTP time for the date, and
 * another one where the date is specified by a POSIX time_t value.
 *
 * @see ::LS_TABLE_ENTRY
 * @see ::KNOWN_LEAP_SECOND_INFO_NTP
 * @see ::KNOWN_LEAP_SECOND_INFO_POSIX
 */
#define KNOWN_LEAP_SECOND_INFO_GPS                                                \
{                                                                                 \
  { _ls_ntp_to_gps( 2571782400 ), _ls_utc_offs_to_gps( 20 ) },  /* 1 Jul 1981 */  \
  { _ls_ntp_to_gps( 2603318400 ), _ls_utc_offs_to_gps( 21 ) },  /* 1 Jul 1982 */  \
  { _ls_ntp_to_gps( 2634854400 ), _ls_utc_offs_to_gps( 22 ) },  /* 1 Jul 1983 */  \
  { _ls_ntp_to_gps( 2698012800 ), _ls_utc_offs_to_gps( 23 ) },  /* 1 Jul 1985 */  \
  { _ls_ntp_to_gps( 2776982400 ), _ls_utc_offs_to_gps( 24 ) },  /* 1 Jan 1988 */  \
  { _ls_ntp_to_gps( 2840140800 ), _ls_utc_offs_to_gps( 25 ) },  /* 1 Jan 1990 */  \
  { _ls_ntp_to_gps( 2871676800 ), _ls_utc_offs_to_gps( 26 ) },  /* 1 Jan 1991 */  \
  { _ls_ntp_to_gps( 2918937600 ), _ls_utc_offs_to_gps( 27 ) },  /* 1 Jul 1992 */  \
  { _ls_ntp_to_gps( 2950473600 ), _ls_utc_offs_to_gps( 28 ) },  /* 1 Jul 1993 */  \
  { _ls_ntp_to_gps( 2982009600 ), _ls_utc_offs_to_gps( 29 ) },  /* 1 Jul 1994 */  \
  { _ls_ntp_to_gps( 3029443200 ), _ls_utc_offs_to_gps( 30 ) },  /* 1 Jan 1996 */  \
  { _ls_ntp_to_gps( 3076704000 ), _ls_utc_offs_to_gps( 31 ) },  /* 1 Jul 1997 */  \
  { _ls_ntp_to_gps( 3124137600 ), _ls_utc_offs_to_gps( 32 ) },  /* 1 Jan 1999 */  \
  { _ls_ntp_to_gps( 3345062400 ), _ls_utc_offs_to_gps( 33 ) },  /* 1 Jan 2006 */  \
  { _ls_ntp_to_gps( 3439756800 ), _ls_utc_offs_to_gps( 34 ) },  /* 1 Jan 2009 */  \
  { _ls_ntp_to_gps( 3550089600 ), _ls_utc_offs_to_gps( 35 ) },  /* 1 Jul 2012 */  \
  { _ls_ntp_to_gps( 3644697600 ), _ls_utc_offs_to_gps( 36 ) },  /* 1 Jul 2015 */  \
  { _ls_ntp_to_gps( 3692217600 ), _ls_utc_offs_to_gps( 37 ) },  /* 1 Jan 2017 */  \
  /* ======================================================================== */  \
  /*   If a new entry is added, don't forget to update the tables             */  \
  /*   KNOWN_LEAP_SECOND_INFO_NTP and KNOWN_LEAP_SECOND_INFO_POSIX            */  \
  /*   accordingly.                                                           */  \
  /* ======================================================================== */  \
  { 0, 0, 0 }                  /* end-of-tbl */                                   \
}



/**
 * @brief Number of 256-week-cycles to check for true leap second week number.
 *
 * The number of 256-week-cycles to check in ::find_true_gps_wn_lsf or
 * ::mbg_find_true_gps_wn_lsf when trying to resolve the ambiguity of
 * the 8 bit truncated week number in ::UTC::WNlsf.
 *
 * According to some tests, the results are not ambiguous for 25
 * 256-week cycles after 1980 (i.e. until year 2099), if only
 * leap second dates at the end of June / beginning of July or
 * end of December / beginning of January are taken into account.
 *
 * @see ::mbg_find_true_gps_wn_lsf (Meinberg API)
 * @see ::find_true_gps_wn_lsf (Meinberg firmware)
 * @see ::ACCEPT_LS_APR_OCT
 */
#define N_GPS_WN_EPOCH  25


#if !defined( ACCEPT_LS_APR_OCT )
  #define ACCEPT_LS_APR_OCT  0
  ///< Should be 0, otherwise ::find_true_gps_wn_lsf or
  ///< ::mbg_find_true_gps_wn_lsf may fail.
  ///< See also ::N_GPS_WN_EPOCH
#endif



/**
 * @brief Convert a 16 bit binary fraction to a scaled decimal
 *
 * @param[in] bin    The binary fraction
 * @param[in] scale  The scale factor
 *
 * @return The calculated number
 *
 * @see ::dec_frac_to_bin_frac_16
 * @see ::dec_frac_to_bin_frac_32
 * @see ::bin_frac_32_to_dec_frac
 */
static __mbg_inline /*HDR*/
uint32_t bin_frac_16_to_dec_frac( uint16_t bin, uint32_t scale )
{
  return (uint32_t) ( (MBG_FRAC32_CONVERSION_TYPE) bin * scale
                      / 0x10000UL );

} // bin_frac_16_to_dec_frac



/**
 * @brief Convert a 32 bit binary fraction to a scaled decimal
 *
 * @param[in] bin    The binary fraction
 * @param[in] scale  The scale factor
 *
 * @return The calculated number
 *
 * @see ::dec_frac_to_bin_frac_32
 * @see ::dec_frac_to_bin_frac_16
 * @see ::bin_frac_16_to_dec_frac
 */
static __mbg_inline /*HDR*/
uint32_t bin_frac_32_to_dec_frac( uint32_t bin, uint32_t scale )
{
  return (uint32_t) ( (MBG_FRAC32_CONVERSION_TYPE) bin * scale
                      / MBG_FRAC32_UNITS_PER_SEC );

} // bin_frac_32_to_dec_frac



#if !defined( MBG_TGT_MISSING_64_BIT_TYPES )

// On targets which don't provide 64 bit data types
// MBG_FRAC32_CONVERSION_TYPE is defined as double,
// in which case the ">> 1" operation in the 2 functions
// below yields an "invalid use of floating point" error.
// This could probably be fixed by a different way of
// casting, at least for a partial expression.

static __mbg_inline /*HDR*/
uint16_t dec_frac_to_bin_frac_16( MBG_FRAC32_CONVERSION_TYPE dec, uint32_t scale )
{
  return (uint16_t) ( ( ( dec * 0x20000 / scale ) + 1 ) >> 1 );

} // dec_frac_to_bin_frac_16


static __mbg_inline /*HDR*/
uint32_t dec_frac_to_bin_frac_32( MBG_FRAC32_CONVERSION_TYPE dec, uint32_t scale )
{
  return (uint32_t) ( ( ( dec * MBG_FRAC32_UNITS_PER_SEC * 2 / scale ) + 1 ) >> 1 );

} // dec_frac_to_bin_frac_32

#endif  // !defined( MBG_TGT_MISSING_64_BIT_TYPES )



#define bin_frac_32_to_msec( _bin ) bin_frac_32_to_dec_frac( (_bin), 1000L )
#define bin_frac_32_to_usec( _bin ) bin_frac_32_to_dec_frac( (_bin), 1000000L )
#define bin_frac_32_to_nsec( _bin ) bin_frac_32_to_dec_frac( (_bin), 1000000000L )
#define bin_frac_16_to_msec( _bin ) bin_frac_16_to_dec_frac( (_bin), 1000L )
#define bin_frac_16_to_usec( _bin ) bin_frac_16_to_dec_frac( (_bin), 1000000L )
#define bin_frac_16_to_nsec( _bin ) bin_frac_16_to_dec_frac( (_bin), 1000000000L )


#define msec_to_bin_frac_32( _msec ) dec_frac_to_bin_frac_32( (_msec), 1000L )
#define usec_to_bin_frac_32( _usec ) dec_frac_to_bin_frac_32( (_usec), 1000000L )
#define nsec_to_bin_frac_32( _nsec ) dec_frac_to_bin_frac_32( (_nsec), 1000000000L )
#define msec_to_bin_frac_16( _msec ) dec_frac_to_bin_frac_16( (_msec), 1000L )
#define usec_to_bin_frac_16( _usec ) dec_frac_to_bin_frac_16( (_usec), 1000000L )
#define nsec_to_bin_frac_16( _nsec ) dec_frac_to_bin_frac_16( (_nsec), 1000000000L )



/**
 * @brief Convert a binary fraction to a scaled decimal
 *
 * Convert a binary fraction (e.g. as in ::PCPS_TIME_STAMP::frac)
 * to a decimal fraction, using a specified scale factor. Depending
 * on the @p scale factor, the result can be milliseconds, microseconds,
 * nanoseconds, or whatever.
 *
 * This function is actually just an alias for ::bin_frac_32_to_dec_frac,
 * but has been introduced much erlier than the latter, and thus is kept
 * for compatibility reasons.
 *
 * @param[in] b      The binary fraction
 * @param[in] scale  The scale factor
 *
 * @return The calculated number
 *
 * @see ::bin_frac_32_to_dec_frac
 */
static __mbg_inline /*HDR*/
uint32_t frac_sec_from_bin( uint32_t b, uint32_t scale )
{
  return bin_frac_32_to_dec_frac( b, scale );

}  // frac_sec_from_bin



/**
 * @brief Convert a binary fraction to "double" fractions
 *
 * Convert a binary fraction (e.g. of a second, as in ::PCPS_TIME_STAMP::frac)
 * to a "double" with the units of seconds.<br>
 * E.g. a 0xFFFFFFFF fraction yields 0.9999999999....
 *
 * @note Excluded from build for kernel drivers which usually
 * don't support floating point operations.
 *
 * @param[in] b      The binary fraction
 *
 * @return The calculated fraction
 *
 * @see ::MBG_FRAC32_UNITS_PER_SEC
 */
static __mbg_inline /*HDR*/
double dfrac_sec_from_bin( uint32_t b )
{
  return (double) b / (double) MBG_FRAC32_UNITS_PER_SEC;

}  // dfrac_sec_from_bin



static __mbg_inline /*HDR*/
/**
 * @brief Check if a particular date is a valid leap second date.
 *
 * This generic function expects a day of month, and a month.
 *
 * @param[in]  mday   The day of month of the date to be checked, range [1..31].
 * @param[in]  month  The month of the date to be checked, range [1..12].
 *
 * @return  @a true if considered valid, else @a false.
 *
 * @ingroup leap_date_valid_fncs
 * @see @ref leap_date_valid_fncs
 * @see ::is_valid_leap_second_date_tm_gps
 * @see ::is_valid_leap_second_date_tm
 */
bool is_valid_leap_second_date( int mday, int month )
{
  if ( mday != 1 )
    return false;

  if ( ( month == 1 ) || ( month == 7 ) )     // 1st of January or July
    return true;

  #if ACCEPT_LS_APR_OCT
    if ( ( month == 4 ) || ( month == 10 ) )  // 1st of April or October
      return true;
  #endif

  return false;

}  // is_valid_leap_second_date



static __mbg_inline /*HDR*/
/**
 * @brief Check if a particular date in ::TM_GPS format is a valid leap second date.
 *
 * For detailed hints see @ref leap_date_valid_fncs and @ref leap_date_valid_fncs.
 *
 * This function expects a ::TM_GPS parameter and calls
 * a generic function to actually check the date.
 *
 * @param[in]  p_tm_gps  The date to be checked, in ::TM_GPS format.
 *
 * @return  @a true if considered valid, else @a false.
 *
 * @ingroup leap_date_valid_fncs
 * @see @ref leap_date_valid_fncs
 * @see ::is_valid_leap_second_date_tm
 * @see ::is_valid_leap_second_date
 */
bool is_valid_leap_second_date_tm_gps( const TM_GPS *p_tm_gps )
{
  return is_valid_leap_second_date( p_tm_gps->mday, p_tm_gps->month );

}  // is_valid_leap_second_date_tm_gps



static __mbg_inline /*HDR*/
/**
 * @brief Check if a particular date in <em>struct tm</em> format is a valid leap second date.
 *
 * We expect the date and time immediately ***after*** a leap second,
 * i.e. when a new TAI offset becomes valid. For example, we expect
 * 2017-01-01 00:00:00 instead of 2016-12-31 23:59:59.
 * This should work for both inserted and deleted leap seconds.
 *
 * By default, only dates at the beginning of January or or July are
 * considered valid.
 *
 * This function expects a <em>struct tm</em> parameter and calls
 * a generic function to actually check the date.
 *
 * @param[in]  p_tm  The date to be checked, in <em>struct tm</em> format.
 *
 * @return  @a true if considered valid, else @a false.
 *
 * @ingroup leap_date_valid_fncs
 * @see @ref leap_date_valid_fncs
 * @see ::is_valid_leap_second_date_tm_gps
 * @see ::is_valid_leap_second_date
 */
bool is_valid_leap_second_date_tm( const struct tm *p_tm )
{
  return is_valid_leap_second_date( p_tm->tm_mday, p_tm->tm_mon + 1 );

}  // is_valid_leap_second_date_tm


#endif  // !defined( MBG_TGT_KERNEL )



/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
 * @brief Set a timeout object to specified interval
 *
 * @param[out]  t         The timeout object
 * @param[in]   clk       The current time, in clock_t ticks
 * @param[in]   interval  The interval until expiration, in clock_t ticks
 */
 void set_timeout( TIMEOUT *t, clock_t clk, clock_t interval ) ;

 /**
 * @brief Stretch a timeout specified in given timeout object
 *
 * @param[in,out]  t         The timeout object
 * @param[in]      interval  The interval until expiration, in clock_t ticks
 */
 void stretch_timeout( TIMEOUT *t, clock_t interval ) ;

 /**
 * @brief Check if a timeout object has expired
 *
 * @param[in]  t    The timeout object
 * @param[in]  clk  The current time, in clock_t ticks
 *
 * @return 1 if timeout expired, else 0
 */
 bit check_timeout( TIMEOUT *t, clock_t clk ) ;

 /**
 * @brief Check if a ::TM_GPS structure contains a valid date and time
 *
 * @param[in]  tm   The date/time structure to be checked
 *
 * @return 0 if date/time is valid, else a negative number indicating
 *         which field was found invalid
 */
 int err_tm( const TM_GPS *tm ) ;

 /**
 * @brief Set the time in a ::TM_GPS structure to 00:00:00.000
 *
 * @param[in,out]  tm   The date/time structure to be set
 *
 * @return Pointer to the ::TM_GPS structure that has been passed
 */
 TM_GPS *clear_time( TM_GPS *tm ) ;

 /**
 * @brief Convert second-of-week to day-of-week and time-of-day
 *
 * @param[in]   wsec  The second-of-week number to be converted.
 *                    Must not be negative.
 * @param[out]  tm    Address of a ::TM_GPS structure which takes
 *                    the computed results. Updates the fields
 *                    ::TM_GPS::hour, ::TM_GPS::min, ::TM_GPS::sec,
 *                    and ::TM_GPS::wday in the range 0..6, with
 *                    0 = Sunday.
 *
 * @return  Pointer to the ::TM_GPS structure that has been passed
 *
 * @see ::tm_to_wsec
 * @see ::day_of_week_sun06
 */
 TM_GPS *wsec_to_tm( long wsec, TM_GPS *tm ) ;

 /**
 * @brief Compute second-of-week from day-of-week and time-of-day
 *
 * @todo Specify input / output ranges
 *
 * @param[in]  tm  Address of a ::TM_GPS structure providing day-of-week and time-of-day
 *
 * @return The computed second-of-week number
 *
 * @see ::wsec_to_tm
 */
 long tm_to_wsec( const TM_GPS *tm ) ;

 /**
 * @brief Check if a specific year is a leap year
 *
 * @param[in]  y  The full year number
 *
 * @return  != 0 if the year is a leap year, else 0
 */
 int is_leap_year( int y ) ;

 /**
 * @brief Compute the day-of-year from a given date
 *
 * @param[in]  day    The day-of-month
 * @param[in]  month  The month
 * @param[in]  year   The full year number
 *
 * @return The computed day-of-year
 */
 int day_of_year( int day, int month, int year ) ;

 /**
 * @brief Compute a date from a given year and day-of-year
 *
 * @param[in]   year     The full year number
 * @param[in]   day_num  Number of days from the beginning of that year, may be negative
 * @param[out]  tm       Address of a ::TM_GPS structure which takes the computed results
 */
 void date_of_year( int year, int day_num, TM_GPS *tm ) ;

 /**
 * @brief Compute day-of-week for a given date.
 *
 * ATTENTION: The computed day-of-week is in the range 0..6,
 * with 0 = Monday (!).
 *
 * In most cases the function ::day_of_week_sun06 is
 * more suitable for applications.
 *
 * @param[in]  day    The day-of-month, 0..31
 * @param[in]  month  The month, 1..12
 * @param[in]  year   The full year number
 *
 * @return The computed day-of-week, 0..6, 0 = Monday (!)
 *
 * @see ::day_of_week_sun06
 * @see ::n_days_since_year_0
 */
 int day_of_week( int day, int month, int year ) ;

 /**
 * @brief Compute day-of-week for a given date.
 *
 * The computed day-of-week is in the range 0..6,
 * with 0 = Sunday, as expected by most applications.
 *
 * @param[in]  day    The day-of-month, 0..31
 * @param[in]  month  The month, 1..12
 * @param[in]  year   The full year number
 *
 * @return The computed day-of-week, 0..6, with 0 = Sunday.
 *
 * @see ::n_days_since_year_0
 * @see ::wsec_to_tm
 */
 int day_of_week_sun06( int day, int month, int year ) ;

 /**
 * @brief Update a year number by a number of days, accounting for leap years
 *
 * @param[in]  day_num  The number of days to evaluate
 * @param[in]  year     The year number to start with
 *
 * @return The computed year number
 */
 int days_to_years( long *day_num, int year ) ;

 /**
 * @brief Compute number of days after Jan 1, 0000 for a given date
 *
 * @param[in]  mday   The day-of-month, [1..31]
 * @param[in]  month  The month, [1..12].
 * @param[in]  year   The full year number, starting from year 0.
 *
 * @return The computed number of days
 *
 * @see ::day_of_week
 */
 long n_days_since_year_0( int mday, int month, int year ) ;

 /**
 * @brief Search a table of known past leap second dates for a specific week and day number.
 *
 * Optionally we return the latest week number we
 * have found in the table, so an application can
 * start there searching there for future potential
 * leap second dates.
 *
 * @ingroup group_true_gps_wn_fncs
 */
 int find_past_gps_wn_lsf_from_table( GPS_WNUM *p_wn, GPS_DNUM dn_t, int srch_all, GPS_WNUM *p_wn_last ) ;

 /**
 * @brief Print time with hours, minutes, seconds to a string
 *
 * @param[out]  s   Address of a string buffer to be filled
 * @param[in]   tm  Address of a ::TM_GPS structure providing date and time
 */
 int sprint_time( char *s, const TM_GPS *tm ) ;

 /**
 * @brief Print time with hours, minutes to a string
 *
 * @param[out]  s   Address of a string buffer to be filled
 * @param[in]   tm  Address of a ::TM_GPS structure providing date and time
 */
 int sprint_short_time( char *s, const TM_GPS *tm ) ;

 /**
 * @brief Print date to a string
 *
 * @param[out]  s   Address of a string buffer to be filled
 * @param[in]   tm  Address of a ::TM_GPS structure providing date and time
 */
 int sprint_date( char *s, const TM_GPS *tm ) ;

 /**
 * @brief Print day-of-week and date to a string
 *
 * @param[out]  s   Address of a string buffer to be filled
 * @param[in]   tm  Address of a ::TM_GPS structure providing date and time
 */
 int sprint_day_date( char *s, const TM_GPS *tm ) ;

 /**
 * @brief Print day-of-week, date and time to a string
 *
 * @param[out]  s   Address of a string buffer to be filled
 * @param[in]   tm  Address of a ::TM_GPS structure providing date and time
 */
 int sprint_tm( char *s, const TM_GPS *tm ) ;

 /**
 * @brief Extract a time from a string
 *
 * @param[in]   s   A time string in format hh:mm:ss
 * @param[out]  tm  Address of a ::TM_GPS structure which takes the extracted time
 */
 void sscan_time( const char *s, TM_GPS *tm ) ;

 /**
 * @brief Extract a date from a string
 *
 * @param[in]   s   A date string in format dd.mm. or dd.mm.yyyy
 * @param[out]  tm  Address of a ::TM_GPS structure which takes the extracted date
 */
 void sscan_date( char *s, TM_GPS *tm ) ;


/* ----- function prototypes end ----- */


static __mbg_inline /*HDR*/
/**
 * @brief Compute number of days after Jan 1, 0000 for a given date.
 *
 * @deprecated This function is deprecated. Use ::n_days_since_year_0
 * instead, which has a more meaningful name to avoid usage problems.
 *
 * @param[in]  mday   The day-of-month, [1..31]
 * @param[in]  month  The month, [1..12].
 * @param[in]  year   The full year number, starting from year 0.
 *
 * @return The computed number of days
 *
 * @see ::day_of_week
 */
long _DEPRECATED_BY( "n_days_since_year_0" ) n_days( int mday, int month, int year )
{
  return n_days_since_year_0( mday, month, year );

}  // n_days


/* End of header body */


#undef _ext
#undef _DO_INIT

#ifdef __cplusplus
}
#endif


#endif  /* _MBGTIME_H */
