
/**************************************************************************
 *
 *  $Id: xdevfeat.h 1.4 2019/09/27 15:37:57 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions and prototypes for xdevfeat.c.
 *
 * -----------------------------------------------------------------------
 *  $Log: xdevfeat.h $
 *  Revision 1.4  2019/09/27 15:37:57  martin
 *  Struct names were added by thomas-b to support forward declarations.
 *  Updated function prototypes and doxygen comments.
 *  Revision 1.3  2018/07/16 12:51:08  martin
 *  Fields user_info and user_status were added to MBG_XDEV_FEATURES.
 *  Updated function prototypes.
 *  Revision 1.2  2017/07/06 07:49:25  martin
 *  Added some macros and inline function simplifying
 *  implementation of the individual check functions.
 *  Updated function prototypes.
 *  Revision 1.1  2016/03/16 14:32:52  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _XDEVFEAT_H
#define _XDEVFEAT_H


/* Other headers to be included */

#include <gpsdefs.h>
#include <mbgerror.h>

#ifdef _XDEVFEAT
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup chk_supp_fncs Groups of functions used to check if a particular feature is supported
 *
 * Each of these functions can be used to check if a device supports a particular
 * feature. ::MBG_SUCCESS is returned if the requested feature is supported,
 * otherwise one of the @ref MBG_ERROR_CODES is returned, as appropriate.
 *
 * Some features are supported by a device if an associated bit is set in the
 * ::RECEIVER_INFO::features field, but since the number of bits in this field
 * is limited, newer feature bits are defined in a ::MBG_XFEATURE_BUFFER.
 * Finally there are some builtin features which may be implicitly supported
 * by a particular device model, or not.
 *
 * These functions provide a unified API for all feature types to make
 * application code more straightforward.
 *
 * If the mbgextio API is used then a ::MBG_XDEV_FEATURES structure embedded in
 * the ::MBG_MSG_CTL structure is set up automatically when the device is opened,
 * and there are mbgextio_... wrapper functions available which just expect the
 * MBG_MSG_CTL * associated with the device to check if a feature is supported.
 * See @ref mbgextio_chk_supp_fncs.
 *
 * Other implementations which retrieve the ::MBG_XDEV_FEATURES structure of
 * a device in a different way can use some lower level functions.
 * See @ref xdevfeat_chk_supp_fncs.
 */


/**
 * @defgroup xdevfeat_chk_supp_fncs Low level functions used to check if a particular feature is supported
 * @ingroup chk_supp_fncs
 *
 * @note Applications using the mbgextio API should use the mbgextio_ wrapper
 * functions preferably. See @ref mbgextio_chk_supp_fncs.
 */


/**
 * @brief A structure combining all device feature information
 */
typedef struct mbg_xdev_features_s
{
  uint32_t reserved;                    ///< Currently reserved, unused, always 0
  RECEIVER_INFO receiver_info;          ///< Receiver info provided by the device
  MBG_XFEATURE_BUFFER xfeature_buffer;  ///< Extended features provided by the device
  MBG_TLV_INFO tlv_info;                ///< TLV info provided by a device
  MBG_USER_INFO user_info;              ///< User info for the logged in user
  MBG_USER_STATUS user_status;          ///< User status for the logged in user

} MBG_XDEV_FEATURES;



/**
 * @brief Type of functions to check if a feature is supported
 */
typedef int _NO_MBG_API XDEVFEAT_CHK_SUPP_FNC( const MBG_XDEV_FEATURES *p_xdf );



/**
 * @brief Check if all bits of a specific mask are set in an integer bit mask
 *
 * This macros checks if specific bits are set in an integer bit mask.
 * This is implemented as macro since the macro works properly with
 * all integer sizes.
 *
 * @param[in] _supp_msk  An integer bit mask
 * @param[in] _chk_msk   The bit mask to be tested
 *
 * @return ::MBG_SUCCESS if all bits of the test mask are set, or
 *         ::MBG_ERR_NOT_SUPP_BY_DEV if not.
 */
#define _check_feat_supp_mask( _supp_msk, _chk_msk )  \
  ( ( ( (_supp_msk) & (_chk_msk) ) == (_chk_msk) ) ?  \
      MBG_SUCCESS : MBG_ERR_NOT_SUPP_BY_DEV )



/**
 * @brief Check if a bits with a specific number is set in an integer bit mask
 *
 * This macros checks if a bits with specific number is set in an integer bit mask.
 * This is implemented as macro since the macro works properly with
 * all integer sizes.
 *
 * @param[in] _supp_msk  An integer bit mask
 * @param[in] _bit_num   The bit mask to be tested
 *
 * @return ::MBG_SUCCESS if all bits of the test mask are set, or
 *         ::MBG_ERR_NOT_SUPP_BY_DEV if not.
 */
#define _check_feat_supp_bit( _supp_msk, _bit_num )  \
  ( ( (_supp_msk) & ( 1UL << (_bit_num) ) ) ?        \
      MBG_SUCCESS : MBG_ERR_NOT_SUPP_BY_DEV )



static __mbg_inline /*HDR*/
/**
 * @brief Check if a specific bit is set in a byte array
 *
 * This function checks if a specific bit is set in an array of bytes.
 * Bits are counted starting from the least significant bit of the least
 * significant byte.
 *
 * @param[in] bit_num    Number of the bit to be tested, 0..(8*(max_bytes-1))
 * @param[in] p          Pointer to a buffer with an array of bytes
 * @param[in] max_bytes  The number of bytes in the buffer p
 *
 * @return ::MBG_SUCCESS if the bit is set, ::MBG_ERR_NOT_SUPP_BY_DEV if not,
 *         or ::MBG_ERR_RANGE if the bit number is out of the range of the array
 */
int check_feat_supp_byte_array( int bit_num, const uint8_t *p, int max_bytes )
{
  int byte_num = bit_num >> 3;

  if ( byte_num < max_bytes )  // the normal case
  {
    ulong bit_mask = 1UL << ( bit_num & 0x07 );

    return ( p[byte_num] & bit_mask ) ? MBG_SUCCESS : MBG_ERR_NOT_SUPP_BY_DEV;
  }

  return MBG_ERR_RANGE;

}  // check_feat_supp_byte_array



/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
 * @brief Check if a device can receive the GPS satellite system
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_dev_is_gps
 * @see ::mbg_chk_dev_is_gps
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_gps( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the GNSS API
 *
 * This is usually supported by devices which can receive signals
 * from different satellite systems, e.g. GPS, GLONASS, ...
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_dev_is_gnss
 * @see ::mbg_chk_dev_is_gnss
 * @see ::MBG_GNSS_TYPES
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_gnss( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device is a time code receiver (IRIG or similar)
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_dev_is_tcr
 * @see ::mbg_chk_dev_is_tcr
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_tcr( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device is a DCF77 AM receiver
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_dev_is_dcf
 * @see ::mbg_chk_dev_is_dcf
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_dcf( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device can receive DCF77 PZF
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_dev_has_pzf
 * @see ::mbg_chk_dev_has_pzf
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_pzf( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device is an MSF receiver
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_msf( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device is a JJY receiver
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_jjy( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device is a WWVB receiver
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_wwvb( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device is a bus level device
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_is_bus_lvl_dev( const MBG_XDEV_FEATURES *p_xdf ) ;

 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ims( const MBG_XDEV_FEATURES *p_xdf ) ;
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_havequick( const MBG_XDEV_FEATURES *p_xdf ) ;
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_gpio( const MBG_XDEV_FEATURES *p_xdf ) ;
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_synth( const MBG_XDEV_FEATURES *p_xdf ) ;
 /**
 * @brief Retrieve the number of supported programmable pulse outputs.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return The number of programmable pulse outputs supported by the device.
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::xdevfeat_has_pout
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_num_pout( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports programmable pulse outputs.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::xdevfeat_num_pout
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_pout( const MBG_XDEV_FEATURES *p_xdf ) ;

 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_irig_tx( const MBG_XDEV_FEATURES *p_xdf ) ;
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_irig_rx( const MBG_XDEV_FEATURES *p_xdf ) ;
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_mbg_os( const MBG_XDEV_FEATURES *p_xdf ) ;
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_serouts( const MBG_XDEV_FEATURES *p_xdf ) ;
 /**
 * @brief Check if a device supports the ::BVAR_STAT structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_bvar_stat( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports reading the position as ::XYZ array
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_pos_xyz( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports reading the position as ::LLA array
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_pos_lla( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if the device supports the builtin feature TIME
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_time_ttm( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::MBG_TIME_SCALE_INFO structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_time_scale_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_time_scale( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::TZDL structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_tzdl( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::TZCODE API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_tzcode( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::ANT_INFO structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ant_info( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::ENABLE_FLAGS structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_enable_flags( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::STAT_INFO structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_gps_stat_info( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::ANT_CABLE_LEN structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ant_cable_length( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::IGNORE_LOCK structure and API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_gps_ignore_lock( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if the device supports the SCU_STAT structures
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 * @see @ref group_scu
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_scu_stat( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if the device supports the ::SV_INFO structures
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_sv_info( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a timecode receiver provides ::MBG_RAW_IRIG_DATA
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_raw_irig_data
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_raw_irig_data( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the old LAN_IP4 API
 *
 * The LAN_IP4 API provides structures and functions to configure
 * parts of the networking of a device and is superseded by the
 * NET_CFG API. Some devices combine NET_CFG and LAN_IP4.
 * Therefore, ::mbgextio_get_all_net_cfg_info should be used
 * preferably to read the network configuration.
 * It will translate the old structures into the new ones.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_net_cfg_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_lan_ip4( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the new NET_CFG API
 *
 * The NET_CFG API provides structures and functions to configure
 * the complete networking part of a device and supersedes the
 * LAN_IP4 API. Not all devices support the whole feature set
 * of the NET_CFG API or combine NET_CFG and LAN_IP4.
 * Therefore, ::mbgextio_get_all_net_cfg_info should be used
 * preferably to read the network configuration.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_net_cfg_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_net_cfg( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the PTP API
 *
 * The PTP API consists of different calls and associated structures
 * which have evolved over time. Not all devices support every call,
 * so ::mbgextio_get_all_ptp_cfg_info takes care to check which parts are
 * supported and thus should be used preferably to read PTP information.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_ptp_cfg_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ptp( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the PTP next generation API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_ptp_ng_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ptp_ng( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the sys ref API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_sys_ref_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_sys_ref( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the NTP API
 *
 * The NTP API consists of different calls and associated structures
 * which have evolved over time. Not all devices support every call,
 * so ::mbgextio_get_all_ntp_cfg_info takes care to check which parts are
 * supported and thus should be used preferably to read NTP information.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_ntp_cfg_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ntp( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the FW management API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_firmware_info
 * @see ::mbgextio_get_fw_glb_info
 * @see ::mbgextio_get_fw_info_idx
 * @see ::mbgextio_get_fw_ufu_info_idx
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_fw_mngmnt( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the event log API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_evt_log( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the user capture API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ucap( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the user capture via network feature
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref group_ucap_net
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ucap_net( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the TLV API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref group_tlv_api
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_tlv_api( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports a file request via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_req_file
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_file_req( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports a command execution via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_xmt_cmd_line
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_exec_cmd( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports a factory reset via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_xmt_fw_rollback
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_fw_rollback( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports a firmware update via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_xmt_fw_update
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_fw_update( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports creating / sending a diagnostics file via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::TODO  //refer to get diag function
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_diag_file( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports PTPv2 license infos
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_ptpv2_license( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports NTP license infos via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_ntp_license( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports PTPv1 License Infos via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_ptpv1_license( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports Time Monitor License infos via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_time_monitor_license( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports UFU (Unified Firmware Update) via TLV
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_tlv_feat_supp
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_supp_tlv_ufu( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::GPS_SAVE_CFG command
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_cmd_save_cfg
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_cmd_save_cfg( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the extended feature monitoring
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_monitoring( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the LED API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::TODO ###
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_led_api( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the LNE API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::TODO ###
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_lne_api( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the power control API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref group_pwr_ctl_api
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_pwr_ctl_api( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device has extended system information.
 *
 * @param[in,out] p_xdf  Pointer to a valid message control structure
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref group_ext_sys_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_ext_sys_info( const MBG_XDEV_FEATURES *p_xdf ) ;

 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_io_ports( const MBG_XDEV_FEATURES *p_xdf ) ;
 /**
 * @brief Check if a device has ::MBG_XFEATURE_TRANSACTIONS
 *
 * @param[in,out] p_xdf  Pointer to a valid message control structure
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::TODO ###
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_transactions( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device has ::MBG_XFEATURE_REBOOT
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_reboot( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device has ::MBG_XFEATURE_REQ_TTM
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 * @see mbgextio_get_time
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_req_ttm( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the extended multi ref features including multi instances
 *
 * The different multi ref feature and its appropriate flags have evolved over time.
 * This function only checks the currently up-to-date GPS_HAS_XMRS_MULT_INSTC flag.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see ::mbgextio_get_all_xmulti_ref_info
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_xmulti_ref( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the extended binary protocol (XBP) feature
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_ri_feature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_xbp( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports database(s) feature
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_database( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports GNSS mode feature
 * All GNSS receivers support this feature by default, this is for GPS-only devices
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_gnss_mode( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the @ref group_tainted_cfg Tainted Config API
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_tainted_cfg( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports (un)registering for push messages
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_push_msgs( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the ::MBG_CLK_RES_INFO structure
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_clk_res_info( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports user authentification
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_user_auth( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports user management
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_user_mngmnt( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports services
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_service( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the XHE I/O commands
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_xhe( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if the device has an L1 frequency up converter
 *
 * If this GNSS receiver supports this then a Meinberg antenna/converter
 * can be used instead of a standard L1 antenna.
 *
 * @param[in] p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else ::MBG_ERR_NOT_SUPP_BY_DEV
 *         or ::MBG_ERR_DEV_NOT_SUPP (see @ref xdevfeat_chk_supp_fncs)
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_up_conv( const MBG_XDEV_FEATURES *p_xdf ) ;

 /**
 * @brief Check if a device supports the FCU API.
 *
 * @param[in]  p_xdf  Pointer to a ::MBG_XDEV_FEATURES buffer associated with a device.
 *
 * @return ::MBG_SUCCESS if supported, else error code from ::check_xfeature
 *
 * @ingroup xdevfeat_chk_supp_fncs
 * @see @ref group_fcu_api
 * @see @ref xdevfeat_chk_supp_fncs
 */
 _NO_MBG_API_ATTR int _NO_MBG_API xdevfeat_has_fcu_api( const MBG_XDEV_FEATURES *p_xdf ) ;


/* ----- function prototypes end ----- */


#ifdef __cplusplus
}
#endif

/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _XDEVFEAT_H */
