
/**************************************************************************
 *
 *  $Id: pci_asic.h 1.31 2018/06/25 12:32:19 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions for the Meinberg PCI interface ASIC.
 *
 * -----------------------------------------------------------------------
 *  $Log: pci_asic.h $
 *  Revision 1.31  2018/06/25 12:32:19  martin
 *  Added PCPS_ASIC_STR_FMT format specifier.
 *  Revision 1.30  2018/03/27 12:39:46  martin
 *  Updated minor version for PZF180PEX to 0x01.
 *  Revision 1.29  2017/07/04 14:18:03  martin
 *  Updated minor version for PTP270PEX.
 *  Revision 1.28  2017/05/10 15:24:21  martin
 *  Tiny cleanup.
 *  Revision 1.27  2017/04/25 11:36:30  martin
 *  Renamed GRC181PEX to GNS181PEX.
 *  Revision 1.26  2016/09/15 14:55:02  martin
 *  Support GRC181PEX.
 *  Added doxgen comments.
 *  Revision 1.25  2014/10/17 11:38:39  martin
 *  Updated version info for GPS180PEX.
 *  Revision 1.24  2013/10/01 15:29:39  martin
 *  Updated version info for PTP270PEX.
 *  Revision 1.23  2013/06/26 15:57:07Z  martin
 *  Support GLN180PEX.
 *  Revision 1.22  2011/10/05 09:46:12  martin
 *  Updated version info for GPS180PEX.
 *  Revision 1.21  2011/09/13 07:36:21Z  martin
 *  Updated version info for GPS180PEX.
 *  Revision 1.20  2011/06/30 13:52:26Z  martin
 *  Updated version info for GPS180PEX.
 *  Revision 1.19  2011/06/29 08:58:32Z  martin
 *  Support PZF180PEX.
 *  Cleaned up handling of pragma pack().
 *  Revision 1.18  2011/04/06 12:31:48  martin
 *  Updated minor versions for PTP270PEX and GPS180PEX.
 *  Revision 1.17  2010/09/10 14:03:25Z  martin
 *  Support GPS180PEX and TCR180PEX.
 *  New table initializer to simplify EPLD version checking.
 *  Revision 1.16  2010/04/16 11:12:21Z  martin
 *  Updated GPS170PEX ASIC version.
 *  Revision 1.15  2009/03/27 09:39:15  martin
 *  Increased current ASIC minor number for TCR170PEX to 0x02.
 *  Renamed some symbols.
 *  Revision 1.14  2009/03/11 16:54:10Z  martin
 *  Increased current ASIC minor number for TCR511PEX to 0x04.
 *  Fixed a typo.
 *  Revision 1.13  2008/12/05 12:28:18Z  martin
 *  Modified syntax of macro _convert_asic_version_number().
 *  Added macros to deal with the ASIC version number.
 *  Added definition PCI_ASIC_HAS_PGMB_IRQ.
 *  Added ASIC revision numbers for PEX511, TCR511PEX, and GPS170PEX
 *  which fix an IRQ bug with these cards.
 *  Added definitions for PTP270PEX, FRC511PEX, and TCR170PEX.
 *  Revision 1.12  2008/07/21 10:30:00Z  martin
 *  Added macros to convert the endianess of data types.
 *  Added PCI_ASIC_CURRENT_MINOR_... symbols.
 *  Revision 1.11  2008/06/11 09:49:43  martin
 *  Added definitions and comments how to handle version numbers
 *  of the PCI and PEX interface chips and EPLDs.
 *  Revision 1.10  2008/02/29 15:21:48Z  martin
 *  Added definition PCI_ASIC_HAS_MM_IO.
 *  Revision 1.9  2008/01/17 09:51:05  daniel
 *  Added macro _convert_asic_version_number().
 *  Cleanup for PCI ASIC version and features.
 *  Revision 1.8  2006/06/14 12:59:12Z  martin
 *  Added support for TCR511PCI.
 *  Revision 1.7  2006/03/10 10:47:03  martin
 *  Added support for PCI511.
 *  Revision 1.6  2005/11/03 15:30:44Z  martin
 *  Added support for GPS170PCI.
 *  Revision 1.5  2004/11/09 12:51:56Z  martin
 *  Redefined fixed width data types using standard C99 types.
 *  Defined some constants unsigned.
 *  Revision 1.4  2004/10/14 15:01:23  martin
 *  Added support for TCR167PCI.
 *  Revision 1.3  2003/05/13 14:38:55Z  MARTIN
 *  Added ushort fields to unions PCI_ASIC_REG and
 *  PCI_ASIC_ADDON_DATA.
 *  Revision 1.2  2003/04/03 10:56:38  martin
 *  Use unions for registers.
 *  Modified BADR0 initializer due to fixed size of address decoder.
 *  Revision 1.1  2003/02/07 11:42:52  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#ifndef _PCI_ASIC_H
#define _PCI_ASIC_H


/* Other headers to be included */

#include <words.h>
#include <use_pack.h>

#ifdef _PCI_ASIC
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#if defined( _USE_PACK )
  #pragma pack( 1 )      // set byte alignment
  #define _USING_BYTE_ALIGNMENT
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Set of PCI ASIC registers which are writeable once after power-up
 **/
typedef struct
{
  uint32_t cfg_class_rev_id;
  uint16_t cfg_badr_0;
  uint16_t cfg_dev_id;

} PCI_ASIC_CFG;


/**
 * @brief A PCI ASIC register as 32, 16, or 8 bit accessible union
 */
typedef union
{
  uint32_t ul;
  uint16_t us[2];
  uint8_t b[4];

} PCI_ASIC_REG;



/**
 * @brief A data type to hold the PCI ASIC version code
 */
typedef uint32_t PCI_ASIC_VERSION;

#define _mbg_swab_asic_version( _p )   _mbg_swab32( _p )



/**
 * @brief A data type to hold the PCI ASIC feature flags mask
 *
 * @see @ref PCI_ASIC_FEATURE_MASKS
 */
typedef uint32_t PCI_ASIC_FEATURES;

#define _mbg_swab_asic_features( _p )  _mbg_swab32( _p )



/**
 * @brief Bit masks used with ::PCI_ASIC_FEATURES
 *
 * @see ::PCI_ASIC_FEATURES
 *
 * @anchor PCI_ASIC_FEATURE_MASKS @{ */

#define PCI_ASIC_HAS_MM_IO     0x0001   ///< The device supports memory mapped I/O
#define PCI_ASIC_HAS_PGMB_IRQ  0x0002   ///< The device supports programmable interrupts (yet not used)

/** @} anchor PCI_ASIC_FEATURE_MASKS */



/**
 * @brief The addon-data part of a PCI ASIC
 */
typedef union
{
  uint32_t ul[4];
  uint16_t us[8];
  uint8_t b[16];

} PCI_ASIC_ADDON_DATA;



/**
 * @brief Register layout of a PCI ASIC
 */
typedef struct
{
  PCI_ASIC_CFG cfg;                ///< Registers which are writeable from add-on once after power-up
  PCI_ASIC_VERSION raw_version;    ///< Raw version code
  PCI_ASIC_FEATURES features;      ///< PCI ASIC feature mask, see @ref PCI_ASIC_FEATURE_MASKS
  PCI_ASIC_REG status_port;        ///< The status port register
  PCI_ASIC_REG control_status;     ///< See @ref PCI_ASIC_CONTROL_STATUS_MASKS
  PCI_ASIC_REG pci_data;           ///< Register used to pass byte from PCI bus to add-on side
  PCI_ASIC_REG reserved_1;         ///< Currently not implemented / used

  PCI_ASIC_ADDON_DATA addon_data;  ///< Register set used to return data from add-on to PCI bus
  PCI_ASIC_ADDON_DATA reserved_2;  ///< Currently not implemented / used

} PCI_ASIC;



/**
 * @brief Bit masks used with ::PCI_ASIC::control_status
 *
 * @see ::PCI_ASIC
 *
 * @anchor PCI_ASIC_CONTROL_STATUS_MASKS @{ */

/**
 * @brief Add-on IRQ flag
 *
 * The IRQ flag for the add-on side is set whenever data is
 * written to the cmd register. It is cleared if the add-on
 * microcontroller writes this bit back to the control_status
 * register. If the bit is set, the add-on signals /ADD_ON_IRQ
 * and ADD_ON_BUSY are asserted.
 */
#define PCI_ASIC_ADD_ON_IRQF   0x00000001UL

/**
 * @brief PCI IRQ flag
 *
 * The IRQ flag for the PCI bus is set whenever the add-on
 * microcontroller asserts the ASIC's /PCI_IRQ line, or the
 * add-on microcontroller sets this bit to 1. It is cleared
 * if this bit is written back from the PCI side. If the bit
 * is set, an IRQ is asserted on the PCI bus.
 */
#define PCI_ASIC_PCI_IRQF      0x00010000UL

// NOTE All other bits are reserved for future use.

/** @} anchor PCI_ASIC_CONTROL_STATUS_MASKS */



/**
 * @brief PCI address range
 *
 * The ASIC's address decoder always decodes 8 bits, so
 * each device must request at least this number of
 * addresses from the PCI BIOS.
 */
#define PCI_ASIC_ADDR_RANGE    0x100U


// Initializers for device configurations

#define PCPS_DEV_CLASS_CODE   0x08800000UL
#define PCI_ASIC_BADR0_INIT   ( ~( PCI_ASIC_ADDR_RANGE - 1 ) | 0x01 )


#define PCI_ASIC_CFG_PCI510          \
{                                    \
  _hilo_32( PCPS_DEV_CLASS_CODE ),   \
  _hilo_16( PCI_ASIC_BADR0_INIT ),   \
  _hilo_16( PCI_DEV_PCI510 )         \
}

#define PCI_ASIC_CFG_GPS169PCI       \
{                                    \
  _hilo_32( PCPS_DEV_CLASS_CODE ),   \
  _hilo_16( PCI_ASIC_BADR0_INIT ),   \
  _hilo_16( PCI_DEV_GPS169PCI )      \
}

#define PCI_ASIC_CFG_TCR510PCI       \
{                                    \
  _hilo_32( PCPS_DEV_CLASS_CODE ),   \
  _hilo_16( PCI_ASIC_BADR0_INIT ),   \
  _hilo_16( PCI_DEV_TCR510PCI )      \
}

#define PCI_ASIC_CFG_TCR167PCI       \
{                                    \
  _hilo_32( PCPS_DEV_CLASS_CODE ),   \
  _hilo_16( PCI_ASIC_BADR0_INIT ),   \
  _hilo_16( PCI_DEV_TCR167PCI )      \
}

#define PCI_ASIC_CFG_GPS170PCI       \
{                                    \
  _hilo_32( PCPS_DEV_CLASS_CODE ),   \
  _hilo_16( PCI_ASIC_BADR0_INIT ),   \
  _hilo_16( PCI_DEV_GPS170PCI )      \
}

#define PCI_ASIC_CFG_PCI511          \
{                                    \
  _hilo_32( PCPS_DEV_CLASS_CODE ),   \
  _hilo_16( PCI_ASIC_BADR0_INIT ),   \
  _hilo_16( PCI_DEV_PCI511 )         \
}

#define PCI_ASIC_CFG_TCR511PCI       \
{                                    \
  _hilo_32( PCPS_DEV_CLASS_CODE ),   \
  _hilo_16( PCI_ASIC_BADR0_INIT ),   \
  _hilo_16( PCI_DEV_TCR511PCI )      \
}



/**
 * @brief Version number conversion macro
 *
 * Handling of the version numbers of the PCI interface
 * chips has changed between the ASICs used for standard PCI
 * and the EPLDs used to configure the PEX8311 chip
 * for a specific device.
 *
 * This macro can be used to convert both types of
 * version number into the same format so that the
 * version numbers can be handled in the same way
 */
#define _convert_asic_version_number( _n ) \
  ( ( (_n) < 0x100 ) ? ( (_n) << 8 ) : (_n) )



#define PCPS_ASIC_STR_FMT  "%u.%02X"  // TODO Or should this be "%u.%02u"

/**
 * @brief Extract the major part of an ASIC version number
 */
#define _pcps_asic_version_major( _v ) \
  ( ( (_v) >> 8 ) & 0xFF )


/**
 * @brief Extract the minor part of an ASIC version number
 */
#define _pcps_asic_version_minor( _v ) \
  ( (_v) & 0xFF )



/**
 * @brief Check whether a version number is correct and matches a required minimum version
 */
#define _pcps_asic_version_greater_equal( _v, _v_major, _v_minor ) \
  (                                                                \
    ( _pcps_asic_version_major( _v ) == (_v_major) ) &&            \
    ( _pcps_asic_version_minor( _v ) >= (_v_minor) )               \
  )



/**
 * @brief ASIC major version numbers
 *
 * @see @ref PCI_ASIC_MINOR_VERSION_NUMBERS
 */
enum PCI_ASIC_MAJOR_VERSION_NUMBERS
{
  PCI_ASIC_MAJOR_PCI_0,      ///< PCI ASIC with CRC bug
  PCI_ASIC_MAJOR_PCI_1,      ///< fixed version of PCI ASIC
  PCI_ASIC_MAJOR_PEX511,     ///< PEX EPLD for PEX511
  PCI_ASIC_MAJOR_GPS170PEX,  ///< PEX EPLD for GPS170PEX
  PCI_ASIC_MAJOR_TCR511PEX,  ///< PEX EPLD for TCR511PEX
  PCI_ASIC_MAJOR_PTP270PEX,  ///< PEX EPLD for PTP270PEX
  PCI_ASIC_MAJOR_FRC511PEX,  ///< PEX EPLD for FRC511PEX
  PCI_ASIC_MAJOR_TCR170PEX,  ///< PEX EPLD for TCR170PEX
  PCI_ASIC_MAJOR_GPS180PEX,  ///< PEX EPLD for GPS180PEX/GPS180AMC
  PCI_ASIC_MAJOR_TCR180PEX,  ///< PEX EPLD for TCR180PEX
  PCI_ASIC_MAJOR_PZF180PEX,  ///< PEX EPLD for PZF180PEX
  PCI_ASIC_MAJOR_GLN180PEX,  ///< PEX EPLD for GLN180PEX
  PCI_ASIC_MAJOR_GNS181PEX,  ///< PEX EPLD for GNS181PEX
  N_PCI_ASIC_MAJOR           ///< the number of known codes
};



/**
 * @brief ASIC minor version definitions
 *
 * The minor number increases when a new EPLD image is released.
 * At least EPLD images with the defined "required minor" numbers
 * should be installed for proper operation. The "current minor"
 * numbers can be used to check if a newer EPLD image is available.
 *
 * @see ::PCI_ASIC_MAJOR_VERSION_NUMBERS
 *
 * @anchor PCI_ASIC_MINOR_VERSION_NUMBERS @{ */

#define PCI_ASIC_CURRENT_MINOR_PEX511      0x04
#define PCI_ASIC_REQUIRED_MINOR_PEX511     0x03
#define PCI_ASIC_FIX_HRT_MINOR_PEX511      0x04  // Increases HRT accuracy
#define PCI_ASIC_FIX_IRQ_MINOR_PEX511      0x03  // Fixes IRQ problem
#define PCI_ASIC_HR_TIME_MINOR_PEX511      0x02  // Supports HR time with PEX511

#define PCI_ASIC_CURRENT_MINOR_GPS170PEX   0x05
#define PCI_ASIC_REQUIRED_MINOR_GPS170PEX  0x03
#define PCI_ASIC_ENH_HRT_MINOR_GPS170PEX   0x05  // Enhanced MM HRT accuracy
#define PCI_ASIC_FIX_HRT_MINOR_GPS170PEX   0x04  // Increases MM HRT accuracy
#define PCI_ASIC_FIX_IRQ_MINOR_GPS170PEX   0x03  // Fixes IRQ problem

#define PCI_ASIC_CURRENT_MINOR_TCR511PEX   0x04
#define PCI_ASIC_REQUIRED_MINOR_TCR511PEX  0x03
//                                         0x04  // EPLD sources shared with PEX511 0x04
#define PCI_ASIC_FIX_IRQ_MINOR_TCR511PEX   0x03  // Fixes IRQ problem, increases HRT accuracy

#define PCI_ASIC_CURRENT_MINOR_PTP270PEX   0x06
#define PCI_ASIC_REQUIRED_MINOR_PTP270PEX  0x01
//                                         0x06  // Supports 1 PPS pulse shift
//                                         0x05  // ...
//                                         0x04  // ...
//                                         0x03  // ...
//                                         0x02  // Increased accuracy of IRIG DCLS slopes
//                                         0x01  // Supports inversion of ucap slopes

#define PCI_ASIC_CURRENT_MINOR_FRC511PEX   0x01
#define PCI_ASIC_REQUIRED_MINOR_FRC511PEX  0x01

#define PCI_ASIC_CURRENT_MINOR_TCR170PEX   0x03
#define PCI_ASIC_REQUIRED_MINOR_TCR170PEX  0x02
#define PCI_ASIC_FIX_EE_ACCESS_TCR170PEX   0x02  // Fixes EE access problem after reset
#define PCI_ASIC_FIX_FO_IN_LEVEL_TCR170PEX 0x03  // Correct polarity for fiber optic input

#define PCI_ASIC_CURRENT_MINOR_GPS180PEX   0x06
#define PCI_ASIC_REQUIRED_MINOR_GPS180PEX  0x01
//                                         0x01  // Updated VHDL compiler and associated PCI primitives
//                                         0x02  // I/O using 3.3V LVTTL
//                                         0x03  // GPS TIC pulse len now 1 sample clock
//                                         0x04  // Enabled PCI IRQ line which had unintentionally been disabled earlier
//                                         0x05  // Increased accuracy of synthesizer output
//                                         0x06  // T0 AUX Capture used by firmware v2.0x

#define PCI_ASIC_CURRENT_MINOR_TCR180PEX   0x00
#define PCI_ASIC_REQUIRED_MINOR_TCR180PEX  0x00

#define PCI_ASIC_CURRENT_MINOR_PZF180PEX   0x01
#define PCI_ASIC_REQUIRED_MINOR_PZF180PEX  0x00
//                                         0x01  // ...

#define PCI_ASIC_CURRENT_MINOR_GLN180PEX   0x00
#define PCI_ASIC_REQUIRED_MINOR_GLN180PEX  0x00

#define PCI_ASIC_CURRENT_MINOR_GNS181PEX   0x00
#define PCI_ASIC_REQUIRED_MINOR_GNS181PEX  0x00

/** @} anchor PCI_ASIC_MINOR_VERSION_NUMBERS */



/**
 * @brief A structure holding version information for a specific device
 *
 * @see ::DEFAULT_PCI_ASIC_VERSION_INFO_TABLE
 */
typedef struct
{
  unsigned int dev_type_num;
  unsigned int major;
  unsigned int current_minor;
  unsigned int required_minor;

} PCI_ASIC_VERSION_INFO;


/**
 * @brief An initializer for a table of ASIC version information for all known devices
 *
 * @note GPS180AMC uses the same ASIC as GPS180PEX
 *
 * @see ::PCI_ASIC_VERSION_INFO
 */
#define DEFAULT_PCI_ASIC_VERSION_INFO_TABLE                                                                               \
{                                                                                                                         \
  { PCPS_TYPE_PEX511, PCI_ASIC_MAJOR_PEX511, PCI_ASIC_CURRENT_MINOR_PEX511, PCI_ASIC_REQUIRED_MINOR_PEX511 },             \
  { PCPS_TYPE_GPS170PEX, PCI_ASIC_MAJOR_GPS170PEX, PCI_ASIC_CURRENT_MINOR_GPS170PEX, PCI_ASIC_REQUIRED_MINOR_GPS170PEX }, \
  { PCPS_TYPE_TCR511PEX, PCI_ASIC_MAJOR_TCR511PEX, PCI_ASIC_CURRENT_MINOR_TCR511PEX, PCI_ASIC_REQUIRED_MINOR_TCR511PEX }, \
  { PCPS_TYPE_PTP270PEX, PCI_ASIC_MAJOR_PTP270PEX, PCI_ASIC_CURRENT_MINOR_PTP270PEX, PCI_ASIC_REQUIRED_MINOR_PTP270PEX }, \
  { PCPS_TYPE_FRC511PEX, PCI_ASIC_MAJOR_FRC511PEX, PCI_ASIC_CURRENT_MINOR_FRC511PEX, PCI_ASIC_REQUIRED_MINOR_FRC511PEX }, \
  { PCPS_TYPE_TCR170PEX, PCI_ASIC_MAJOR_TCR170PEX, PCI_ASIC_CURRENT_MINOR_TCR170PEX, PCI_ASIC_REQUIRED_MINOR_TCR170PEX }, \
  { PCPS_TYPE_GPS180PEX, PCI_ASIC_MAJOR_GPS180PEX, PCI_ASIC_CURRENT_MINOR_GPS180PEX, PCI_ASIC_REQUIRED_MINOR_GPS180PEX }, \
  { PCPS_TYPE_TCR180PEX, PCI_ASIC_MAJOR_TCR180PEX, PCI_ASIC_CURRENT_MINOR_TCR180PEX, PCI_ASIC_REQUIRED_MINOR_TCR180PEX }, \
  { PCPS_TYPE_PZF180PEX, PCI_ASIC_MAJOR_PZF180PEX, PCI_ASIC_CURRENT_MINOR_PZF180PEX, PCI_ASIC_REQUIRED_MINOR_PZF180PEX }, \
  { PCPS_TYPE_GLN180PEX, PCI_ASIC_MAJOR_GLN180PEX, PCI_ASIC_CURRENT_MINOR_GLN180PEX, PCI_ASIC_REQUIRED_MINOR_GLN180PEX }, \
  { PCPS_TYPE_GPS180AMC, PCI_ASIC_MAJOR_GPS180PEX, PCI_ASIC_CURRENT_MINOR_GPS180PEX, PCI_ASIC_REQUIRED_MINOR_GPS180PEX }, \
  { PCPS_TYPE_GNS181PEX, PCI_ASIC_MAJOR_GNS181PEX, PCI_ASIC_CURRENT_MINOR_GNS181PEX, PCI_ASIC_REQUIRED_MINOR_GNS181PEX }, \
  { 0 }                                                                                                                   \
}


/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

/* (no header definitions found) */

/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif


#if defined( _USING_BYTE_ALIGNMENT )
  #pragma pack()      // set default alignment
  #undef _USING_BYTE_ALIGNMENT
#endif

/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _PCI_ASIC_H */
