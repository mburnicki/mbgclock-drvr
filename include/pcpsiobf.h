
/**************************************************************************
 *
 *  $Id: pcpsiobf.h 1.3 2019/04/03 12:33:22 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions an I/O buffer that can be used in kernel space
 *    and user space, on targets that support IOCTL as well as
 *    on targets that don't.
 *
 * -----------------------------------------------------------------------
 *  $Log: pcpsiobf.h $
 *  Revision 1.3  2019/04/03 12:33:22  martin
 *  Excluded some PCPS_IO_BUFFER fields that are currently not required.
 *  Revision 1.2  2018/01/18 14:52:52  martin
 *  Fix for C++ builds.
 *  Revision 1.1  2018/01/15 10:31:26Z  martin
 *  Initial revision with structures moved from mbgioctl.h here.
 *
 **************************************************************************/

#ifndef _PCPSIOBF_H
#define _PCPSIOBF_H

#include <pcpsdev.h>
#include <cfg_hlp.h>
#include <mbggeo.h>


/* Start of header body */

// We must *not* use byte-alignment here.


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief A wrapper struct for the ::LLA array type
 *
 * Simplifies usage of macros which expect ponters to structures
 * rather than the address of an array.
 *
 * @see ::LLA
 */
typedef struct
{
  LLA lla;
} LLAs;



/**
 * @brief A wrapper struct for the ::XYZ array type.
 *
 * Simplifies usage of macros which expect ponters to structures
 * rather than the address of an array.
 *
 * @see ::XYZ
 */
typedef struct
{
  XYZ xyz;
} XYZs;



/**
 * @brief An I/O buffer for bus level devices
 *
 * Can be used in kernel space and user space, on targets that
 * support IOCTL calls as well as on targets that don't.
 */
typedef union
{
  PCPS_STATUS_PORT pcps_status_port_usb;  // only used with USB devices
  PCPS_ID_STR fw_id;
  PCPS_SN_STR sernum;
  PCPS_TIME pcps_time;
  PCPS_STIME pcps_stime;
  PCPS_HR_TIME pcps_hr_time;
  PCPS_TIME_STAMP pcps_time_stamp;
  PCPS_SERIAL pcps_serial;
  PCPS_TZCODE pcps_tzcode;
  PCPS_TZDL pcps_tzdl;
  MBG_REF_OFFS mbg_ref_offs;
  MBG_OPT_INFO mbg_opt_info;
  MBG_OPT_SETTINGS mbg_opt_settings;
  IRIG_INFO irig_info;
  IRIG_SETTINGS irig_settings;
  PCPS_UCAP_ENTRIES pcps_ucap_entries;
  TZDL tzdl;
  SW_REV sw_rev;
  BVAR_STAT bvar_stat;
  TTM ttm;
  PORT_PARM port_parm;
  ANT_INFO ant_info;
  ENABLE_FLAGS enable_flags;
  STAT_INFO stat_info;
  RECEIVER_INFO receiver_info;
  GPS_CMD gps_cmd;
  IDENT ident;
  POS pos;
  XYZs xyzs;
  LLAs llas;
  ANT_CABLE_LEN ant_cable_len;
  PORT_SETTINGS_IDX port_settings_idx;
  SYNTH synth;
  SYNTH_STATE synth_state;
  ALL_POUT_INFO_IDX all_pout_info_idx;
  POUT_SETTINGS_IDX pout_settings_idx;
  ALL_STR_TYPE_INFO_IDX all_str_type_info_idx;
  ALL_PORT_INFO_IDX all_port_info_idx;
  ALL_PTP_UC_MASTER_INFO_IDX all_ptp_uc_master_info_idx;
  MBG_TIME_SCALE_INFO mbg_time_scale_info;
  MBG_TIME_SCALE_SETTINGS mbg_time_scale_settings;
  UTC utc;
  MBG_IRIG_CTRL_BITS mbg_irig_ctrl_bits;
  LAN_IF_INFO lan_if_info;
  IP4_SETTINGS ip4_settings;
  PTP_STATE ptp_state;
  PTP_CFG_INFO ptp_cfg_info;
  PTP_CFG_SETTINGS ptp_cfg_settings;
  PCPS_IRIG_TIME pcps_irig_time;
  MBG_RAW_IRIG_DATA mbg_raw_irig_data;
  PTP_UC_MASTER_CFG_LIMITS ptp_uc_master_cfg_limits;
  PTP_UC_MASTER_SETTINGS_IDX ptp_uc_master_settings_idx;
  PCPS_TIME_CYCLES pcps_time_cycles;
  PCPS_HR_TIME_CYCLES pcps_hr_time_cycles;
  MBG_DBG_PORT mbg_dbg_port;
  // MBG_DBG_DATA mbg_dbg_data;                        // currently not required
  // MBG_PC_CYCLES_FREQUENCY mbg_pc_cycles_frequency;  // currently not required
  MBG_TIME_INFO_HRT mbg_time_info_hrt;
  // MBG_TIME_INFO_TSTAMP mbg_time_info_tstamp;        // currently not required
  CORR_INFO corr_info;
  TR_DISTANCE tr_distance;
  MBG_DEBUG_STATUS debug_status;
  MBG_NUM_EVT_LOG_ENTRIES num_evt_log_entries;
  MBG_EVT_LOG_ENTRY evt_log_entry;
  MBG_GNSS_MODE_SETTINGS gnss_mode_settings;
  MBG_GNSS_MODE_INFO gnss_mode_info;
  ALL_GNSS_SAT_INFO_IDX all_gnss_sat_info_idx;
  MBG_GPIO_CFG_LIMITS mbg_gpio_cfg_limits;
  ALL_GPIO_INFO_IDX all_gpio_info_idx;
  ALL_GPIO_STATUS_IDX all_gpio_status_idx;
  MBG_GPIO_SETTINGS_IDX mbg_gpio_settings_idx;
  XMULTI_REF_INSTANCES xmulti_ref_instances;
  ALL_XMULTI_REF_STATUS_IDX all_xmulti_ref_status_idx;
  ALL_XMULTI_REF_INFO_IDX all_xmulti_ref_info_idx;
  XMULTI_REF_SETTINGS_IDX xmulti_ref_settings_idx;
  XMR_HOLDOVER_STATUS xmr_holdover_status;

} PCPS_IO_BUFFER;


#ifdef __cplusplus
}
#endif


/* End of header body */

#undef _ext

#endif  /* _PCPSIOBF_H */
