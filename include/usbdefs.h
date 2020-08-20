
/**************************************************************************
 *
 *  $Id: usbdefs.h 1.48 2020/07/01 08:28:15 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions used with USB devices.
 *
 * -----------------------------------------------------------------------
 *  $Log: usbdefs.h $
 *  Revision 1.48  2020/07/01 08:28:15  martin
 *  Added definitions for GPS190 and GNS190.
 *  Revision 1.47  2020/03/17 16:45:17  martin
 *  Added models BPE2XXX, BPE2352, BPE8XXX, BPE6042.
 *  Added missing device name for GNS181_UC.
 *  Revision 1.46  2020/03/03 07:21:16  andre.hartmann
 *  Added definitions for VSG181.
 *  Revision 1.45  2019/11/15 11:39:21Z  philipp
 *  Added WiseChip OLED Display USB descriptor.
 *  Revision 1.44  2019/08/07 08:14:14  daniel
 *  Added support for GNS181_UC.
 *  Revision 1.43  2019/07/31 13:55:56  martin
 *  Added support for FCU200.
 *  Revision 1.42  2019/06/19 08:41:19  martin
 *  Added support for RSC_02.
 *  Revision 1.41  2019/03/08 10:51:15  martin
 *  Added support for GNM181.
 *  Renamed class MBG_USB_CLASS_GRC to MBG_USB_CLASS_GNS.
 *  Revision 1.40  2019/01/14 08:37:45  martin
 *  GRC181 and GRC165 were preliminary names, so changed the
 *  associated definitions to GNS181 and GNS165 which are the
 *  final device names.
 *  Removed obsolete USYNCPWR stuff.
 *  Revision 1.39  2019/01/11 11:45:42  martin
 *  Added definitions for CPC200.
 *  Revision 1.38  2018/12/18 13:08:20  gregoire.diehl
 *  Added VSI180
 *  Revision 1.37  2018/11/01 10:53:29Z  martin
 *  Added some missing device name strings.
 *  Revision 1.36  2018/10/30 12:22:26  paul.kretz
 *  Added definitions for BPE_STM
 *  Revision 1.35  2018/09/07 13:04:47Z  philipp
 *  Added USB_DEV_GRC165 which, in fact, is USB_DEV_GNS165
 *  Revision 1.34  2017/12/12 07:37:47  philipp
 *  Added definitions for SSP100
 *  Revision 1.33  2017/09/19 12:30:17  martin
 *  Added definitions for TCR180USB.
 *  Revision 1.32  2017/09/15 08:25:48  paul.kretz
 *  Added Class code and device ID for PIO180.
 *  Revision 1.31  2017/09/13 14:14:27Z  paul.kretz
 *  Added ID, name string and table entry for FCM180
 *  Revision 1.30  2017/05/17 09:52:53Z  thomas.fasse
 *  Device BITS was rennamed to LSG180.
 *  Revision 1.29  2017/05/16 07:19:40Z  thomas.fasse
 *  Added ID, name string and table entry for LIU variant BITS by Paul
 *  Revision 1.28  2017/04/04 10:43:34Z  paul.kretz
 *  Added ID, name string and table entry for FDM180M
 *  Added missing table entry for MicroSync power supply module
 *  Revision 1.27  2017/03/28 09:37:15Z  paul.kretz
 *  Added ID, name string and table entry for MicroSync power supply module
 *  Revision 1.26  2017/02/13 09:57:45Z  paul.kretz
 *  Added ID, name string and table entry for PZF180.
 *  Revision 1.25  2016/11/11 09:24:04Z  lars.meihost
 *  Added ID, name string and table entry for TCR180.
 *  Revision 1.24  2016/11/04 11:49:15Z  paul.kretz
 *  Added device ID and associated class code for MDU312
 *  Revision 1.23  2016/09/26 07:28:10Z  paul.kretz
 *  Added class code and device for N2X180
 *  Revision 1.22  2016/07/06 13:55:15Z  martin
 *  Definitions for IMS-SPT, REL1000, MDU300, and SDI added by stephan.
 *  Class code and device ID for LUE added by daniel-vdh.
 *  Class code and device ID for HPS added by daniel.
 *  Device IDs and associated class codes for SCG, FDM180, CPC180,
 *  VSG180, and GTS180 added by paul.
 *  Removed obsolete multiple definitions for FTDI serial interface chips.
 *  Added a new definition for unique FTDI chip.
 *  Added definitions for serial refclocks.
 *  Added definitions for LNE180SFP, GRC180 and GRC181.
 *  Added definitions USB_VENDOR_WILDCARD  and USB_PROD_WILDCARD.
 *  Added device name string and string table initializers.
 *  Doxygen fixes.
 *  Revision 1.21  2014/01/16 15:26:09  daniel
 *  Added class codes and devices for USB to serial adapters and ASIX network chips
 *  Revision 1.20  2013/10/08 09:13:04  daniel
 *  Added definition and class code for RSC.
 *  Revision 1.19  2013/06/04 10:45:53  daniel
 *  Added class codes and device IDs for IMS devices MRI and BPE
 *  Revision 1.18  2013/01/24 11:29:21  joerg
 *  Added class code and device ID for LNE-GB
 *  Revision 1.17  2012/08/08 07:53:29Z  daniel
 *  Added class code and device ID for LIU
 *  Revision 1.16  2012/02/13 09:29:59  paul
 *  added class code for LNO180
 *  Revision 1.15  2011/10/11 06:21:04Z  andre
 *  added class code for GPS180
 *  Revision 1.14  2011/10/07 10:13:25Z  daniel
 *  New class code and device id for CPE
 *  Revision 1.13  2011/06/29 14:11:23Z  martin
 *  Added device IDs for TCR600USB, MSF600USB, and WVB600USB.
 *  Revision 1.12  2011/05/11 07:20:37  daniel
 *  New class code and device id for fan control unit
 *  Revision 1.11  2011/04/13 07:59:11  daniel
 *  New class code and device id for external
 *  synchronization interface device.
 *  Revision 1.10  2010/11/11 09:16:33Z  martin
 *  Added device ID for DCF600USB.
 *  Revision 1.9  2009/03/13 09:02:24  martin
 *  Removed definitions for timeout intervals.
 *  Revision 1.8  2009/02/18 11:08:44  daniel
 *  Added new class code and device ID for SCU_USB
 *  Revision 1.7  2008/11/28 07:45:30Z  daniel
 *  Added new class code and device ID for WWVB51USB
 *  Revision 1.6  2008/01/09 10:39:18Z  daniel
 *  Added new class code and device ID for MSF51USB
 *  Revision 1.5  2007/10/29 08:23:26Z  daniel
 *  Added new class code and device ID for TCR51USB
 *  Revision 1.4  2007/09/25 09:59:50Z  daniel
 *  Added indices for endpoint definitions.
 *  Added timeout definitions.
 *  Revision 1.3  2006/12/20 16:11:36Z  daniel
 *  Added new device class and device_id for nCipher CMC-device.
 *  Revision 1.2  2006/12/07 09:10:57Z  daniel
 *  Added new class code and device ID for USB5131.
 *  Revision 1.1  2006/04/21 08:14:56Z  martin
 *  Initial revision
 *
 **************************************************************************/

#ifndef _USBDEFS_H
#define _USBDEFS_H


/* Other headers to be included */


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


/** Meinberg's USB vendor ID number (assigned by USB-IF administration) */
#define USB_VENDOR_MEINBERG     0x1938

#define USB_VENDOR_WILDCARD     0
#define USB_PROD_WILDCARD       0


/**
 * @brief USB device class codes assigned by Meinberg
 */
enum MBG_USB_CLASS_CODES
{
  MBG_USB_CLASS_NONE,         ///< (unknown or not defined)
  MBG_USB_CLASS_CPC,          ///< Control Panel Controller
  MBG_USB_CLASS_TSU,          ///< Time Stamp Unit
  MBG_USB_CLASS_DCF,          ///< DCF77 Radio Clock
  MBG_USB_CLASS_CMC,          ///< nCipher Crypto Module Carrier
  MBG_USB_CLASS_TCR,          ///< IRIG Time Code Receiver
  MBG_USB_CLASS_MSF,          ///< MSF Radio Clock
  MBG_USB_CLASS_WWVB,         ///< WWVB Radio Clock

  MBG_USB_CLASS_SCU,          ///< Meinberg Signal Changeover Unit
  MBG_USB_CLASS_ESI,          ///< External Synchronization Interface
  MBG_USB_CLASS_FCU,          ///< Fan Control Unit
  MBG_USB_CLASS_CPE,          ///< Configurable Port Expander
  MBG_USB_CLASS_GPS,          ///< GPS Receiver
  MBG_USB_CLASS_LNO,          ///< Low Phase Noise Option
  MBG_USB_CLASS_LIU,          ///< Line Interface Unit
  MBG_USB_CLASS_LNE,          ///< LNE-GB

  MBG_USB_CLASS_MRI,          ///< MRS Input card for IMS
  MBG_USB_CLASS_BPE,          ///< IMS Backplane Port Expander
  MBG_USB_CLASS_RSC,          ///< RSC Redundant Switch Control
  MBG_USB_CLASS_SERIAL,       ///< USB to Serial controller, FTDI chip connected to Meinberg serial device
  MBG_USB_CLASS_SCG,          ///< Studio Clock Generator
  MBG_USB_CLASS_SDI,          ///< SDI Input card for IMS
  MBG_USB_CLASS_FDM,          ///< Frequency Deviation Monitor
  MBG_USB_CLASS_NIC,          ///< ASIX AX88179 Network interface chips on LNE, modified by Meinberg (this ***must*** be 0x17)

  MBG_USB_CLASS_MDU,          ///< Modular Distribution Unit
  MBG_USB_CLASS_SPT,          ///< Single Path Through
  MBG_USB_CLASS_REL,          ///< Relais Error Card
  MBG_USB_CLASS_LUE,          ///< Lantime USB Expansion
  MBG_USB_CLASS_HPS,          ///< High Performance Synchronization Card (PTP/NTP)
  MBG_USB_CLASS_VSG,          ///< Video Sync Generator
  MBG_USB_CLASS_GTS,          ///< Greenwich Time Signal
  MBG_USB_CLASS_GNS,          ///< GNSS receivers (GPS, GLONASS, ... )

  MBG_USB_CLASS_N2X,          ///< NTP/PTP receiver
  MBG_USB_CLASS_PLACEHOLDER,  // TODO formerly this was MBG_USB_CLASS_USYNC, used with USYNCPWR.
                              // This is obsolete, but we need to keep an entry here to preserve
                              // the numeric values of subsequent symbols.
                              // When another new class needs to be defined, the class can be
                              // defined in place here, and MBG_USB_CLASS_PLACEHOLDER can be removed.
  MBG_USB_CLASS_FCM,          ///< Fake Clock Module
  MBG_USB_CLASS_PIO,          ///< Programmable Input/Output Module
  MBG_USB_CLASS_VSI,          ///< Video Sync Interface
  MBG_USB_CLASS_WSI,          ///< WiseChip Semiconductor

  N_MBG_USB_CLASS             ///< number of known Meinberg USB device class codes
};


/**
 * @brief USB device ID numbers assigned by Meinberg
 *
 * High byte:  USB device class, see ::MBG_USB_CLASS_CODES<br>
 * Low byte:   enumeration of devices of a class
 *
 * @see @ref MBG_USB_DEVICE_NAMES
 * @see ::DEFAULT_MBG_USB_DEVICE_NAMES
 * @see ::MBG_USB_CLASS_CODES
 *
 * @anchor MBG_USB_DEVICE_IDS @{
 */

// If new devices are defined here then appropriate definitions should also
// be added to MBG_USB_DEVICE_NAMES and DEFAULT_MBG_USB_DEVICE_NAMES.

#define USB_DEV_CPC_01          ( ( MBG_USB_CLASS_CPC << 8 )    | 0x01 )
#define USB_DEV_CPC180          ( ( MBG_USB_CLASS_CPC << 8 )    | 0x02 )
#define USB_DEV_CPC200          ( ( MBG_USB_CLASS_CPC << 8 )    | 0x03 )

#define USB_DEV_TSU_01          ( ( MBG_USB_CLASS_TSU << 8 )    | 0x01 )

#define USB_DEV_USB5131         ( ( MBG_USB_CLASS_DCF << 8 )    | 0x01 )
#define USB_DEV_DCF600USB       ( ( MBG_USB_CLASS_DCF << 8 )    | 0x02 )
#define USB_DEV_PZF180          ( ( MBG_USB_CLASS_DCF << 8 )    | 0x03 )

#define USB_DEV_CMC             ( ( MBG_USB_CLASS_CMC << 8 )    | 0x01 )

#define USB_DEV_TCR51USB        ( ( MBG_USB_CLASS_TCR << 8 )    | 0x01 )
#define USB_DEV_TCR600USB       ( ( MBG_USB_CLASS_TCR << 8 )    | 0x02 )
#define USB_DEV_TCR180          ( ( MBG_USB_CLASS_TCR << 8 )    | 0x03 )
#define USB_DEV_TCR180USB       ( ( MBG_USB_CLASS_TCR << 8 )    | 0x04 )

#define USB_DEV_MSF51USB        ( ( MBG_USB_CLASS_MSF << 8 )    | 0x01 )
#define USB_DEV_MSF600USB       ( ( MBG_USB_CLASS_MSF << 8 )    | 0x02 )

#define USB_DEV_WWVB51USB       ( ( MBG_USB_CLASS_WWVB << 8 )   | 0x01 )
#define USB_DEV_WVB600USB       ( ( MBG_USB_CLASS_WWVB << 8 )   | 0x02 )

#define USB_DEV_SCU_USB         ( ( MBG_USB_CLASS_SCU << 8 )    | 0x01 )

#define USB_DEV_ESI_01          ( ( MBG_USB_CLASS_ESI << 8 )    | 0x01 )

#define USB_DEV_FCU_01          ( ( MBG_USB_CLASS_FCU << 8 )    | 0x01 )   // Fan (and power supply) Control Unit, doesn't support FCU API.
#define USB_DEV_FCU200          ( ( MBG_USB_CLASS_FCU << 8 )    | 0x02 )   // Fan (and power supply) Control Unit, supports FCU API.

#define USB_DEV_CPE_01          ( ( MBG_USB_CLASS_CPE << 8 )    | 0x01 )

#define USB_DEV_GPS180          ( ( MBG_USB_CLASS_GPS << 8 )    | 0x01 )
#define USB_DEV_GPS190          ( ( MBG_USB_CLASS_GPS << 8 )    | 0x02 )

#define USB_DEV_LNO180          ( ( MBG_USB_CLASS_LNO << 8 )    | 0x01 )

#define USB_DEV_LIU_01          ( ( MBG_USB_CLASS_LIU << 8 )    | 0x01 )
#define USB_DEV_LSG180          ( ( MBG_USB_CLASS_LIU << 8 )    | 0x02 )

#define USB_DEV_LNE_01          ( ( MBG_USB_CLASS_LNE << 8 )    | 0x01 )   // LNE with standard copper
#define USB_DEV_LNE180SFP       ( ( MBG_USB_CLASS_LNE << 8 )    | 0x02 )   // LNE with SFP (fiber optic)

#define USB_DEV_MRI_01          ( ( MBG_USB_CLASS_MRI << 8 )    | 0x01 )

#define USB_DEV_BPE_01          ( ( MBG_USB_CLASS_BPE << 8 )    | 0x01 )  // Generic BPE with ATMEL M3
#define USB_DEV_BPE_STM         ( ( MBG_USB_CLASS_BPE << 8 )    | 0x02 )  // Generic BPE with STM M0
#define USB_DEV_BPE2XXX         ( ( MBG_USB_CLASS_BPE << 8 )    | 0x03 )  // Formerly BPE, port extender via jumper field
#define USB_DEV_BPE2352         ( ( MBG_USB_CLASS_BPE << 8 )    | 0x04 )  // Formerly BPE_STM, time code AM and DCLS, plus 1 relay output
#define USB_DEV_BPE8XXX         ( ( MBG_USB_CLASS_BPE << 8 )    | 0x05 )  // Formerly BPE, port extender via multiplexer
#define USB_DEV_BPE6042         ( ( MBG_USB_CLASS_BPE << 8 )    | 0x06 )  // Formerly BPE, isolated outputs via multiplexer

#define USB_DEV_RSC_01          ( ( MBG_USB_CLASS_RSC << 8 )    | 0x01 )  // RSC used with M1000 / M3000 / IMS
#define USB_DEV_RSC_02          ( ( MBG_USB_CLASS_RSC << 8 )    | 0x02 )  // RSC used with M2000

#define USB_DEV_SPT_01          ( ( MBG_USB_CLASS_SPT << 8 )    | 0x01 )

#define USB_DEV_REL_01          ( ( MBG_USB_CLASS_REL << 8 )    | 0x01 )

/// LANTIME CPU quad FTDI serial interface chip
#define USB_DEV_LAN_CPU_SERIAL  ( ( MBG_USB_CLASS_SERIAL << 8 ) | 0x01 )

#define USB_DEV_SCG_01          ( ( MBG_USB_CLASS_SCG << 8 )    | 0x01 )

#define USB_DEV_SDI_01          ( ( MBG_USB_CLASS_SDI << 8 )    | 0x01 )

#define USB_DEV_FDM180          ( ( MBG_USB_CLASS_FDM << 8 )    | 0x01 )  ///< FDM for IMS Systems
#define USB_DEV_FDM180M         ( ( MBG_USB_CLASS_FDM << 8 )    | 0x02 )  ///< FDM for old Lantime Systems (M300/M600/M900)

#define USB_DEV_MDU300          ( ( MBG_USB_CLASS_MDU << 8 )    | 0x01 )
#define USB_DEV_MDU312          ( ( MBG_USB_CLASS_MDU << 8 )    | 0x02 )

#define USB_DEV_LUE_01          ( ( MBG_USB_CLASS_LUE << 8 )    | 0x01 )

#define USB_DEV_HPS100          ( ( MBG_USB_CLASS_HPS << 8 )    | 0x01 )
#define USB_DEV_SSP100          ( ( MBG_USB_CLASS_HPS << 8 )    | 0x02 )

#define USB_DEV_VSG180          ( ( MBG_USB_CLASS_VSG << 8 )    | 0x01 )
#define USB_DEV_VSG181          ( ( MBG_USB_CLASS_VSG << 8 )    | 0x02 )

#define USB_DEV_VSI180          ( ( MBG_USB_CLASS_VSI << 8 )    | 0x01 )

#define USB_DEV_GTS180          ( ( MBG_USB_CLASS_GTS << 8 )    | 0x01 )

#define USB_DEV_GRC180          ( ( MBG_USB_CLASS_GNS << 8 )    | 0x01 )
#define USB_DEV_GNS181          ( ( MBG_USB_CLASS_GNS << 8 )    | 0x02 )
#define USB_DEV_GNS165          ( ( MBG_USB_CLASS_GNS << 8 )    | 0x03 )
#define USB_DEV_GNM181          ( ( MBG_USB_CLASS_GNS << 8 )    | 0x04 )
#define USB_DEV_GNS181_UC       ( ( MBG_USB_CLASS_GNS << 8 )    | 0x05 )
#define USB_DEV_GNS190          ( ( MBG_USB_CLASS_GNS << 8 )    | 0x06 )

#define USB_DEV_N2X180          ( ( MBG_USB_CLASS_N2X << 8 )    | 0x01 )

#define USB_DEV_FCM180          ( ( MBG_USB_CLASS_FCM << 8 )    | 0x01 )  ///< Fake Clock Module

#define USB_DEV_PIO180          ( ( MBG_USB_CLASS_PIO << 8 )    | 0x01 )  ///< Programmable Input/Output Module

#define USB_DEV_WSI_UG2864      ( ( MBG_USB_CLASS_WSI << 8 )    | 0x01 )

// If new devices are defined here then appropriate definitions should also
// be added to MBG_USB_DEVICE_NAMES and DEFAULT_MBG_USB_DEVICE_NAMES.

/** @} anchor MBG_USB_DEVICE_IDS */



/**
 * @brief Device name strings for Meinberg USB devices
 *
 * @see @ref MBG_USB_DEVICE_IDS
 * @see ::DEFAULT_MBG_USB_DEVICE_NAMES
 *
 * @anchor MBG_USB_DEVICE_NAMES @{
 */

#define USB_DEV_NAME_CPC_01          "CPC_01"
#define USB_DEV_NAME_CPC180          "CPC180"
#define USB_DEV_NAME_CPC200          "CPC200"

#define USB_DEV_NAME_TSU_01          "TSU_01"

#define USB_DEV_NAME_USB5131         "USB5131"
#define USB_DEV_NAME_DCF600USB       "DCF600USB"
#define USB_DEV_NAME_PZF180          "PZF180"

#define USB_DEV_NAME_CMC             "CMC"

#define USB_DEV_NAME_TCR51USB        "TCR51USB"
#define USB_DEV_NAME_TCR600USB       "TCR600USB"
#define USB_DEV_NAME_TCR180          "TCR180"
#define USB_DEV_NAME_TCR180USB       "TCR180USB"

#define USB_DEV_NAME_MSF51USB        "MSF51USB"
#define USB_DEV_NAME_MSF600USB       "MSF600USB"

#define USB_DEV_NAME_WWVB51USB       "WWVB51USB"
#define USB_DEV_NAME_WVB600USB       "WVB600USB"

#define USB_DEV_NAME_SCU_USB         "SCU_USB"

#define USB_DEV_NAME_ESI_01          "ESI_01"

#define USB_DEV_NAME_FCU_01          "FCU_01"
#define USB_DEV_NAME_FCU200          "FCU200"

#define USB_DEV_NAME_CPE_01          "CPE_01"

#define USB_DEV_NAME_GPS180          "GPS180"
#define USB_DEV_NAME_GPS190          "GPS190"

#define USB_DEV_NAME_LNO180          "LNO180"

#define USB_DEV_NAME_LIU_01          "LIU_01"
#define USB_DEV_NAME_LSG180          "LSG180"

#define USB_DEV_NAME_LNE_01          "LNE_01"
#define USB_DEV_NAME_LNE180SFP       "LNE180SFP"

#define USB_DEV_NAME_MRI_01          "MRI_01"

#define USB_DEV_NAME_BPE_01          "BPE_01"
#define USB_DEV_NAME_BPE_STM         "BPE"
#define USB_DEV_NAME_BPE2XXX         "BPE2XXX"
#define USB_DEV_NAME_BPE2352         "BPE2352"
#define USB_DEV_NAME_BPE8XXX         "BPE8XXX"
#define USB_DEV_NAME_BPE6042         "BPE6042"

#define USB_DEV_NAME_RSC_01          "RSC_01"
#define USB_DEV_NAME_RSC_02          "RSC_02"

#define USB_DEV_NAME_SPT_01          "SPT_01"

#define USB_DEV_NAME_REL_01          "REL_01"

#define USB_DEV_NAME_LAN_CPU_SERIAL  "LAN_CPU_SERIAL"

#define USB_DEV_NAME_SCG_01          "SCG_01"

#define USB_DEV_NAME_SDI_01          "SDI_01"

#define USB_DEV_NAME_FDM180          "FDM180"
#define USB_DEV_NAME_FDM180M         "FDM180M"

#define USB_DEV_NAME_MDU300          "MDU300"
#define USB_DEV_NAME_MDU312          "MDU312"

#define USB_DEV_NAME_LUE_01          "LUE_01"

#define USB_DEV_NAME_HPS100          "HPS100"
#define USB_DEV_NAME_SSP100          "SSP100"

#define USB_DEV_NAME_VSG180          "VSG180"
#define USB_DEV_NAME_VSG181          "VSG181"
#define USB_DEV_NAME_VSI180          "VSI180"

#define USB_DEV_NAME_GTS180          "GTS180"

#define USB_DEV_NAME_GRC180          "GRC180"
#define USB_DEV_NAME_GNS181          "GNS181"
#define USB_DEV_NAME_GNS165          "GNS165"
#define USB_DEV_NAME_GNM181          "GNM181"
#define USB_DEV_NAME_GNS181_UC       "GNS181_UC"
#define USB_DEV_NAME_GNS190          "GNS190"

#define USB_DEV_NAME_N2X180          "N2X180"

#define USB_DEV_NAME_FCM180          "FCM180"

#define USB_DEV_NAME_PIO180          "PIO180"

#define USB_DEV_NAME_WSI_UG2864      "WSI_UG2864"

/** @} anchor MBG_USB_DEVICE_NAMES */



/**
 * @brief Initializer for a table of USB device IDs and name strings
 *
 * Can be used e.g. to initialize an array of ::MBG_CODE_NAME_TABLE_ENTRY.
 *
 * @see @ref MBG_USB_DEVICE_IDS
 * @see @ref MBG_USB_DEVICE_NAMES
 */
#define DEFAULT_MBG_USB_DEVICE_NAMES                         \
{                                                            \
  { USB_DEV_CPC_01,          USB_DEV_NAME_CPC_01 },          \
  { USB_DEV_CPC180,          USB_DEV_NAME_CPC180 },          \
  { USB_DEV_TSU_01,          USB_DEV_NAME_TSU_01 },          \
  { USB_DEV_USB5131,         USB_DEV_NAME_USB5131 },         \
  { USB_DEV_DCF600USB,       USB_DEV_NAME_DCF600USB },       \
  { USB_DEV_CMC,             USB_DEV_NAME_CMC },             \
  { USB_DEV_TCR51USB,        USB_DEV_NAME_TCR51USB },        \
  { USB_DEV_TCR600USB,       USB_DEV_NAME_TCR600USB },       \
  { USB_DEV_TCR180,          USB_DEV_NAME_TCR180 },          \
  { USB_DEV_MSF51USB,        USB_DEV_NAME_MSF51USB },        \
  { USB_DEV_MSF600USB,       USB_DEV_NAME_MSF600USB },       \
  { USB_DEV_WWVB51USB,       USB_DEV_NAME_WWVB51USB },       \
  { USB_DEV_WVB600USB,       USB_DEV_NAME_WVB600USB },       \
  { USB_DEV_SCU_USB,         USB_DEV_NAME_SCU_USB },         \
  { USB_DEV_ESI_01,          USB_DEV_NAME_ESI_01 },          \
  { USB_DEV_FCU_01,          USB_DEV_NAME_FCU_01 },          \
  { USB_DEV_CPE_01,          USB_DEV_NAME_CPE_01 },          \
  { USB_DEV_GPS180,          USB_DEV_NAME_GPS180 },          \
  { USB_DEV_LNO180,          USB_DEV_NAME_LNO180 },          \
  { USB_DEV_LIU_01,          USB_DEV_NAME_LIU_01 },          \
  { USB_DEV_LNE_01,          USB_DEV_NAME_LNE_01 },          \
  { USB_DEV_MRI_01,          USB_DEV_NAME_MRI_01 },          \
  { USB_DEV_BPE_01,          USB_DEV_NAME_BPE_01 },          \
  { USB_DEV_RSC_01,          USB_DEV_NAME_RSC_01 },          \
  { USB_DEV_SPT_01,          USB_DEV_NAME_SPT_01 },          \
  { USB_DEV_REL_01,          USB_DEV_NAME_REL_01 },          \
  { USB_DEV_LAN_CPU_SERIAL,  USB_DEV_NAME_LAN_CPU_SERIAL },  \
  { USB_DEV_SCG_01,          USB_DEV_NAME_SCG_01 },          \
  { USB_DEV_SDI_01,          USB_DEV_NAME_SDI_01 },          \
  { USB_DEV_FDM180,          USB_DEV_NAME_FDM180 },          \
  { USB_DEV_MDU300,          USB_DEV_NAME_MDU300 },          \
  { USB_DEV_LUE_01,          USB_DEV_NAME_LUE_01 },          \
  { USB_DEV_HPS100,          USB_DEV_NAME_HPS100 },          \
  { USB_DEV_VSG180,          USB_DEV_NAME_VSG180 },          \
  { USB_DEV_LNE180SFP,       USB_DEV_NAME_LNE180SFP },       \
  { USB_DEV_GTS180,          USB_DEV_NAME_GTS180 },          \
  { USB_DEV_GRC180,          USB_DEV_NAME_GRC180 },          \
  { USB_DEV_GNS181,          USB_DEV_NAME_GNS181 },          \
  { USB_DEV_N2X180,          USB_DEV_NAME_N2X180 },          \
  { USB_DEV_MDU312,          USB_DEV_NAME_MDU312 },          \
  { USB_DEV_PZF180,          USB_DEV_NAME_PZF180 },          \
  { USB_DEV_FDM180M,         USB_DEV_NAME_FDM180M },         \
  { USB_DEV_LSG180,          USB_DEV_NAME_LSG180 },          \
  { USB_DEV_FCM180,          USB_DEV_NAME_FCM180 },          \
  { USB_DEV_PIO180,          USB_DEV_NAME_PIO180 },          \
  { USB_DEV_TCR180USB,       USB_DEV_NAME_TCR180USB },       \
  { USB_DEV_SSP100,          USB_DEV_NAME_SSP100 },          \
  { USB_DEV_GNS165,          USB_DEV_NAME_GNS165 },          \
  { USB_DEV_BPE_STM,         USB_DEV_NAME_BPE_STM },         \
  { USB_DEV_VSI180,          USB_DEV_NAME_VSI180 },          \
  { USB_DEV_CPC200,          USB_DEV_NAME_CPC200 },          \
  { USB_DEV_GNM181,          USB_DEV_NAME_GNM181 },          \
  { USB_DEV_RSC_02,          USB_DEV_NAME_RSC_02 },          \
  { USB_DEV_FCU200,          USB_DEV_NAME_FCU200 },          \
  { USB_DEV_GNS181_UC,       USB_DEV_NAME_GNS181_UC },       \
  { USB_DEV_WSI_UG2864,      USB_DEV_NAME_WSI_UG2864 },      \
  { USB_DEV_VSG181,          USB_DEV_NAME_VSG181 },          \
  { USB_DEV_BPE2XXX,         USB_DEV_NAME_BPE2XXX },         \
  { USB_DEV_BPE2352,         USB_DEV_NAME_BPE2352 },         \
  { USB_DEV_BPE8XXX,         USB_DEV_NAME_BPE8XXX },         \
  { USB_DEV_BPE6042,         USB_DEV_NAME_BPE6042 },         \
  { USB_DEV_GPS190,          USB_DEV_NAME_GPS190 },          \
  { USB_DEV_GNS190,          USB_DEV_NAME_GNS190 },          \
  { 0, /* end of table */    NULL }                          \
}



enum
{
  MBGUSB_EP_IDX_HOST_IN,          // transfers from device to host
  MBGUSB_EP_IDX_HOST_OUT,         // transfers from host to device
  MBGUSB_EP_IDX_HOST_IN_CYCLIC,   // cyclic auto-transfer to host
  MBGUSB_MAX_ENDPOINTS            // max number of supported endpoints
};


#ifdef __cplusplus
}
#endif

/* End of header body */

#endif  /* _USBDEFS_H */
