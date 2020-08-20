
/**************************************************************************
 *
 *  $Id: cfg_hlp.h 1.7 2019/09/27 14:24:45 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions and prototypes for configuration programs.
 *
 *    WARNING: Changing the constants defined here affects the size of
 *    the related structures and arrays, and thus would break compatibility
 *    if used in DLLs / shared object libraries.
 *
 *    Care must be taken that the number of objects supported by
 *    any particular device (which can be only determined at runtime)
 *    does not exceed the max. number of objects specified here
 *    for the configuration programs.
 *
 * -----------------------------------------------------------------------
 *  $Log: cfg_hlp.h $
 *  Revision 1.7  2019/09/27 14:24:45  martin
 *  New structure definitions to support new API features.
 *  Struct names were added by thomas-b to support forward declarations.
 *  Some new inline functions.
 *  Updated function prototypes and some doxygen comments.
 *  Revision 1.6  2018/09/20 11:22:11  martin
 *  Renamed some global variables to more common names.
 *  Added string initializer tables and associated macros
 *  for some PTP configuration stuff.
 *  Revision 1.5  2018/07/05 10:37:05Z  martin
 *  Renamed setup_port_info_from_port_settings()
 *  to setup_port_info_from_port_parm().
 *  Initializer for OS targets added by philipp.
 *  Database feature support added by philipp.
 *  I/O helper structures and API refuctored by philipp.
 *  Firmware management wrapper structs added by philipp.
 *  Syslog option added to monitoring feature by philipp.
 *  Service feature and API added by philipp.
 *  Return random UID for TLVs under Windows.
 *  MBG_EVENT stuff integrated with common/general monitoring
 *  stuff by philipp.
 *  Monitoring events refactored and tainted config support
 *  added by philipp.
 *  Exclude some function from build in kernel mode.
 *  Fixed build for NetBSD.
 *  Supporting MBG_TGT_NO_TGT.
 *  Updated function prototypes.
 *  Revision 1.4  2017/07/05 13:22:00  martin
 *  Definitions for TLV, IMS, and GPIO provided by philipp.
 *  Definitions for ALL_XPB_INFO, ALL_NET_CFG_INFO,
 *  ALL_PTP_V2_COMMON_DATASETS, ALL_PTP_V1_COMMON_DATASETS,
 *  ALL_UCAP and associated stuff provided by thomas-b.
 *  Definitions for xmulti_ref and IO PORT, SNMP and MONITORING,
 *  and associated stuff provided by philipp and thomas-b.
 *  New definitions COMP_SIG_MODES and PCPS_TIME_EXT_FLAGS.
 *  New inline functions device_id_is_serial() and
 *  device_id_is_lan().
 *  Moved inline function num_bits_set() and a lot of
 *  global configuration variables here.
 *  Older defines N_SUPP_DEV, PCPS_MAX_DDEVS, and MBG_MAX_DEVICES
 *  have been obsoleted by new defines N_SUPP_DEV_BUS, N_SUPP_DEV_EXT,
 *  and N_SUPP_DEV_TOTAL.
 *  Updated function prototypes.
 *  Revision 1.3  2013/09/25 10:02:15  martin
 *  Added ALL_PTP_CFG_INFO, ALL_GNSS_SAT_INFO_IDX and
 *  related definitions.
 *  Added doxygen comments.
 *  Revision 1.2  2012/10/02 18:16:26  martin
 *  Modified some typedefs to be more compliant with the underlying types.
 *  Revision 1.1  2011/09/21 15:59:59  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _CFG_HLP_H
#define _CFG_HLP_H


/* Other headers to be included */

#include <gpsdefs.h>
#include <mbgklist.h>

#if !defined( MBG_TGT_KERNEL )
  #include <stdlib.h>
  #include <string.h>
#endif

#if defined( _PRELIMINARY_CODE )
  #if defined( MBG_TGT_POSIX )
    #include <sys/stat.h>
    #include <time.h>
  #endif // MBG_TGT_POSIX

  #if defined( MBG_TGT_LINUX )
    #include <sys/sysinfo.h>
  #endif // MBG_TGT_LINUX

#endif // _PRELIMINARY_CODE

#ifdef _CFG_HLP
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


#if 1  // ### TODO cleanup

#define N_SUPP_DEV_BUS   16
#define N_SUPP_DEV_EXT    1

#define N_SUPP_DEV_TOTAL   ( N_SUPP_DEV_BUS + N_SUPP_DEV_EXT )

typedef struct _DEVICE_INFO
{
  char *hw_id;
  char fw_id[100];

} DEVICE_INFO;

_ext DEVICE_INFO device_list[N_SUPP_DEV_TOTAL];

#endif



/**
 * @defgroup cfg_hlp_com_parm_cnv_fncs Legacy COM_PARM conversion functions
 */

/// @brief The max number of serial ports supported by configuration programs
#define MAX_PARM_PORT        10

/// @brief The max number of serial string types supported by configuration programs
#define MAX_PARM_STR_TYPE    20

/// @brief The max number of programmable pulse outputs supported by configuration programs
#define MAX_PARM_POUT        10

/// @brief The max number of GNSS settings supported by configuration programs
#define MAX_PARM_GNSS_SAT  N_GNSS_TYPES

/// @brief The max number of PTP unicast masters supported by configuration programs
#define MAX_PARM_PTP_UC_MASTER    3

/// @brief The max number of external NTP server associations to be handled by configuration programs
#define MAX_PARM_EXT_NTP_SRVR  20

/// @brief The max number of GPIO ports supported by configuration programs
#define MAX_PARM_GPIO    10

/// @brief The max number of XMR sources supported by configuration programs
#define MAX_PARM_XMR     10

/// @brief The max number of external NTP servers supported by configuration programs
#define MAX_EXT_NTP_SERVERS  20

/// @brief The max. number of time monitoring modules supported by configuration programs
/// Each module may support a different number of targets to be monitored.
/// @see ### TODO
#define MAX_MBG_TIME_MON_MODULES 10

/// @brief The max. number of time monitoring targets supported by configuration programs
/// This is the sum of all targets from all monitoring modules.
/// @see ### TODO
#define MAX_MBG_TIME_MON_TARGETS 100



/// @brief An array of configuration settings for all serial ports
typedef PORT_INFO_IDX ALL_PORT_INFO_IDX[MAX_PARM_PORT];

/// @brief An array of configuration settings for all serial string types
typedef STR_TYPE_INFO_IDX ALL_STR_TYPE_INFO_IDX[MAX_PARM_STR_TYPE];

/**
 * @brief All configuration parameters for all serial ports
 *
 * Used to collect all configuration parameters of a clock's serial ports
 * that can be handled by a configuration program.
 *
 * @see ::RECEIVER_INFO::n_com_ports
 * @see ::RECEIVER_INFO::n_str_type
 */
typedef struct
{
  ALL_PORT_INFO_IDX pii;       ///< all serial port configuration settings
  ALL_STR_TYPE_INFO_IDX stii;  ///< all supported serial string types
  PORT_PARM tmp_pp;            ///< used internally only, for compatibility

} RECEIVER_PORT_CFG;



/**
 * @brief All XBP information of a XBP supporting device
 *
 * This structure represents a list of connected devices
 *
 * @see ::GPS_HAS_XBP
 */
typedef struct all_xbp_info_s
{
  XBP_LIMITS limits;
  XBP_NODE_LIMITS* node_limits;
  XBP_NODE_INFO_IDX* node_infos;
} ALL_XBP_INFO;



/**
 * @brief An array of configuration settings for all programmable pulse outputs
 *
 * Used to collect all configuration parameters of a clock's programmable pulse outputs
 * that can be handled by a configuration program.
 *
 * @see ::RECEIVER_INFO::n_prg_out
 */
typedef POUT_INFO_IDX ALL_POUT_INFO_IDX[MAX_PARM_POUT];



/**
 * @brief All network configuration parameters
 *
 * Used to collect all configuration parameters for networking
 *
 * @see ::GPS_HAS_NET_CFG
 * @see ::GPS_HAS_LAN_IP4
 */
typedef struct all_net_cfg_info_s
{
  MBG_NET_GLB_CFG_INFO glb_cfg_info;
  MBG_NET_INTF_LINK_INFO_IDX *link_infos;
  MBG_NET_INTF_ADDR_INFO_IDX *addr_infos;
  MBG_IP_ADDR_IDX *dns_srvrs;
  MBG_NET_NAME_IDX *dns_srch_doms;
  MBG_NET_INTF_ROUTE_INFO_IDX *route_infos;
} ALL_NET_CFG_INFO;

typedef ALL_NET_CFG_INFO ALL_NET_STATUS_INFO;



/**
 * @brief All SNMP configuration information
 *
 * Used to collect all configuration parameters for monitoring via SNMP
 * Can be used, if ::MBG_MONITORING_TYPE_MSK_SNMP is set in ::MBG_MONITORING_LIMITS::supp_types
 *
 * @see ::MBG_XFEATURE_MONITORING
 */
typedef struct all_snmp_info_s
{
  MBG_SNMP_GLB_INFO glb_info;
  MBG_SNMP_V12_INFO_IDX *v12_infos;
  MBG_SNMP_V12_TRAP_INFO_IDX *v12_trap_infos;
  MBG_SNMP_V3_INFO_IDX *v3_infos;
  MBG_SNMP_V3_TRAP_INFO_IDX *v3_trap_infos;

} ALL_SNMP_INFO;



/**
 * @brief All monitoring event data
 *
 * Used to collect all event data and group related chunks senseful to a single
 * event so all data is in one struct.
 */
typedef struct mbg_event
{
  MBG_MSG_IDX idx;
  uint16_t reserved;  // for 32 bit alignment
  MBG_EVENT_INFO info;
  MBG_EVENT_STATUS status;
  /* Pointer for 3rd party to connect some data */
  void *backref;
  /* Pointer for the mallocer to connect some private data */
  void *priv_data;
  /*
   * Can be used to free/release priv or cookie data (if necessary)
   * or do other stuff before event gets freed. But NEVER use release
   * to free the event itself!! This is done by the API right after calling
   * release.
   */
  void (*release_priv)(struct mbg_event*);
  void (*release_backref)(struct mbg_event*);

} MBG_EVENT;

typedef struct all_events_s
{
  MBG_EVENT_GLB_INFO glb_info;
  MBG_MSG_IDX num_events;
  uint16_t reserved;  // for 32 bit alignment
  MBG_EVENT *events;

} ALL_EVENTS;


/**
 * @brief All monitoring syslog data
 *
 * Used to collect all syslog data.
 */


typedef struct mbg_syslog_server
{
  unsigned idx;
  MBG_SYSLOG_INFO info;

  /* Pointer for the mallocer to connect some private data */
  void *priv_data;
  /*
   * Can be used to free/release priv or cookie data (if necessary)
   * or do other stuff before event gets freed. But NEVER use release
   * to free the event itself!! This is done by the API right after calling
   * release.
   */
  void (*release_priv)(struct mbg_syslog_server*);

} MBG_SYSLOG_SERVER;

typedef struct all_syslog_info_s
{
  MBG_SYSLOG_GLB_INFO glb_info;
  MBG_SYSLOG_SERVER *servers;

} ALL_SYSLOG_INFO;


/**
 * @brief All monitoring information
 *
 * Used to collect all configuration parameters for monitoring of a device
 * Depending on the ::MBG_MONITORING_LIMITS::supp_types,
 * the approriate configurations can be found in the sub structures
 *
 * @see ::MBG_XFEATURE_MONITORING
 */
typedef struct all_monitoring_info_s
{
  MBG_MONITORING_LIMITS limits;
  ALL_SNMP_INFO *all_snmp_info;
  ALL_EVENTS *all_events;
  ALL_SYSLOG_INFO *all_syslog_info;

} ALL_MONITORING_INFO;



/// @brief Configuration settings for all unicast master specifications
typedef PTP_UC_MASTER_INFO_IDX ALL_PTP_UC_MASTER_INFO_IDX[MAX_PARM_PTP_UC_MASTER];

/**
 * @brief All PTP configuration parameters
 *
 * Used to collect all configuration parameters for a PTP daemon
 * that can be handled by a configuration program.
 *
 * @see ::GPS_HAS_PTP
 * @see ::PTP_UC_MASTER_CFG_LIMITS::n_supp_master
 */
typedef struct all_ptp_cfg_info_s
{
  PTP_CFG_INFO ptp_cfg_info;
  PTP_UC_MASTER_CFG_LIMITS ptp_uc_master_cfg_limits;
  ALL_PTP_UC_MASTER_INFO_IDX all_ptp_uc_master_info_idx;

} ALL_PTP_CFG_INFO;


/**
 * @brief All PTP next gen configuration parameters,
 * only supported if ::MBG_XFEATURE_PTP_NG is set
 */

typedef struct mbg_ptp_ng_tstamper
{
  unsigned idx;
  MBG_PTP_NG_TSTAMPER_INFO info;
  MBG_PTP_NG_TSTAMPER_STATUS status;
  MBG_PTP_NG_TSTAMPER_SETTINGS *settings;

  void *priv_data;
  void (*release_priv)(struct mbg_ptp_ng_tstamper*);

} MBG_PTP_NG_TSTAMPER;


typedef struct mbg_ptp_ng_instc
{
  MBG_MSG_IDX idx;
  uint16_t reserved;  // for 32 bit alignment
  MBG_PTP_NG_INSTC_INFO info;
  MBG_PTP_NG_INSTC_STATUS status;
  MBG_PTP_NG_INSTC_PKT_CNTRS cntrs;
  MBG_PTP_NG_INSTC_SETTINGS *settings;
  const MBG_PTP_NG_TSTAMPER *tstamper;

  MBG_PTP_NG_UC_SLAVE_STATUS *uc_slave_list;

  void *priv_data;
  void (*release_priv)(struct mbg_ptp_ng_instc*);

} MBG_PTP_NG_INSTC;


typedef struct mbg_ptp_ng_uc_master
{
  MBG_MSG_IDX idx;
  uint16_t reserved;  // for 32 bit alignment
  MBG_PTP_NG_UC_MASTER_INFO info;
  MBG_PTP_NG_UC_MASTER_SETTINGS *settings;
  const MBG_PTP_NG_INSTC *instc;

  void *priv_data;
  void (*release_priv)(struct mbg_ptp_ng_uc_master*);

} MBG_PTP_NG_UC_MASTER;


typedef struct all_ptp_ng_info_s
{
  MBG_PTP_NG_GLB_INFO glb_info;

  MBG_PTP_NG_TSTAMPER *timestampers;
  MBG_PTP_NG_INSTC *instances;
  MBG_PTP_NG_UC_MASTER *uc_masters;

} ALL_PTP_NG_INFO;


/**
 * @brief All PTPv1 common datasets for a PTP device
 *
 * Contains one of each common datasets plus one port dataset
 * for each port of a device. The number of port datasets is defined
 * in the ::MBG_PTP_V1_DEFAULT_DATASET::number_ports field of the
 * ::ALL_PTP_V1_COMMON_DATASETS::default_dataset member.
 *
 * @see ::MBG_PTP_V1_DEFAULT_DATASET
 * @see ::MBG_PTP_V1_CURRENT_DATASET
 * @see ::MBG_PTP_V1_PARENT_DATASET
 * @see ::MBG_PTP_V1_TIME_PROPERTIES_DATASET
 * @see ::MBG_PTP_V1_PORT_DATASET_IDX
 */
typedef struct
{
  MBG_PTP_V1_DEFAULT_DATASET default_dataset;
  MBG_PTP_V1_CURRENT_DATASET current_dataset;
  MBG_PTP_V1_PARENT_DATASET parent_dataset;
  MBG_PTP_V1_TIME_PROPERTIES_DATASET time_properties_dataset;
  MBG_PTP_V1_PORT_DATASET_IDX *port_datasets;

} ALL_PTP_V1_COMMON_DATASETS;



/**
 * @brief All PTPv2 common datasets for a PTP device
 *
 * Contains one of each common datasets plus one port dataset
 * for each port of a device. The number of port datasets is defined
 * in the ::MBG_PTP_V1_DEFAULT_DATASET::number_ports field of the
 * ::ALL_PTP_V1_COMMON_DATASETS::default_dataset member.
 *
 * @see ::MBG_PTP_V2_DEFAULT_DATASET
 * @see ::MBG_PTP_V2_CURRENT_DATASET
 * @see ::MBG_PTP_V2_PARENT_DATASET
 * @see ::MBG_PTP_V2_TIME_PROPERTIES_DATASET
 * @see ::MBG_PTP_V2_PORT_DATASET_IDX
 */
typedef struct
{
  MBG_PTP_V2_DEFAULT_DATASET default_dataset;
  MBG_PTP_V2_CURRENT_DATASET current_dataset;
  MBG_PTP_V2_PARENT_DATASET parent_dataset;
  MBG_PTP_V2_TIME_PROPERTIES_DATASET time_properties_dataset;
  MBG_PTP_V2_PORT_DATASET_IDX *port_datasets;

} ALL_PTP_V2_COMMON_DATASETS;



typedef struct mbg_sys_ref_src
{
  unsigned idx;
  struct mbg_klist_head head;
  MBG_SYS_REF_SRC_INFO info;
  MBG_SYS_REF_SRC_STATUS status;
  MBG_SYS_REF_SRC_SETTINGS *settings;

  void *priv_data;
  void (*release_priv)(struct mbg_sys_ref_src*);

} MBG_SYS_REF_SRC;


typedef struct all_sys_ref_info_s
{
  MBG_SYS_REF_LIMITS limits;
  MBG_SYS_REF_GLB_STATUS glb_status;
  struct mbg_klist_head sources;
  const MBG_SYS_REF_SRC *cur_master;
  void (*free_source)(MBG_SYS_REF_SRC*);

} ALL_SYS_REF_INFO;



/**
 * @brief An array of configuration settings for all programmable pulse outputs
 *
 * Used to collect all configuration parameters of a clock's programmable pulse outputs
 * that can be handled by a configuration program.
 */
typedef GNSS_SAT_INFO_IDX  ALL_GNSS_SAT_INFO_IDX[MAX_PARM_GNSS_SAT];



/**
 * @brief Summary information on all supported GNSS systems
 *
 * Used to collect all configuration parameters even for devices
 * that can receive the signals from multiple satellite systems.
 */
typedef struct all_gnss_info_s
{
  STAT_INFO stat_info;
  int n_gnss_supp;
  MBG_GNSS_MODE_INFO gnss_mode_info;
  ALL_GNSS_SAT_INFO_IDX gnss_sat_info_idx;

} ALL_GNSS_INFO;



/// @brief Configuration settings for all NTP server associations
typedef NTP_PEER_SETTINGS ALL_NTP_PEER_SETTINGS[MAX_EXT_NTP_SERVERS];

/**
 * @brief All NTP configuration parameters
 *
 * Used to collect all configuration parameters for an NTP daemon
 * that can be handled by a configuration program.
 *
 * @see ::GPS_HAS_NTP
 */
typedef struct
{
  ALL_NTP_PEER_SETTINGS all_ntp_peer_settings;

} NTP_CLIENT_CFG_PEER_SETTINGS;


typedef struct all_ntp_cfg_info_s
{
  NTP_GLB_INFO glb_info;
  NTP_SYMM_KEY_LIMITS *symm_key_limits;
  NTP_SYMM_KEY_INFO_IDX *symm_key_info_idx;
  NTP_TRUSTED_KEY_INFO_IDX *trusted_key_info_idx;

  NTP_CLNT_MODE_INFO *clnt_info;
  NTP_PEER_SETTINGS_IDX *peer_settings_idx;

  NTP_SRV_MODE_INFO *srv_info;
  NTP_REFCLK_CFG_INFO_IDX *refclk_info_idx;
  NTP_MISC_LIMITS *misc_limits;
  NTP_MISC_ORPHAN_MODE_INFO *orphan_mode_info;

} ALL_NTP_CFG_INFO;


typedef struct all_ntp_status_s
{
  NTP_SYS_STATE sys_state;
  NTP_REFCLK_STATE_IDX *refclk_states;
  NTP_PEER_STATE_IDX *peer_states;
} ALL_NTP_STATUS;



/// @brief Configuration settings for all GPIO ports
typedef MBG_GPIO_INFO_IDX ALL_GPIO_INFO_IDX[MAX_PARM_GPIO];

/// @brief Status information on all GPIO ports
typedef MBG_GPIO_STATUS_IDX ALL_GPIO_STATUS_IDX[MAX_PARM_GPIO];




/// @brief Status of all XMR inputs
typedef XMULTI_REF_STATUS_IDX ALL_XMULTI_REF_STATUS_IDX[MAX_PARM_XMR];

/// @brief Configuration settings for all XMR inputs
typedef XMULTI_REF_INFO_IDX ALL_XMULTI_REF_INFO_IDX[MAX_PARM_XMR];



typedef struct all_xmulti_ref_info_s
{
  XMULTI_REF_INSTANCES instances;
  XMULTI_EXT_REF_INSTANCES ext_instances;
  XMULTI_REF_INFO_IDX *infos;
  XMR_EXT_SRC_INFO_IDX *ext_src_infos;
  XMR_QL_LIMITS ql_limits;
  XMR_QL_SETTINGS_IDX *ql_settings;
  /*
   * ALL_XMULTI_REF_INFO related flag if at least one ref type supports metrics
   * and thus QL stuff!
   */
  unsigned has_metrics;
} ALL_XMULTI_REF_INFO;


typedef struct all_xmulti_ref_status_s
{
  XMULTI_REF_STATUS_IDX *status;
  XMR_HOLDOVER_STATUS *holdover_status;
  XMR_STATS_IDX *stats_idx;
  XMR_METRICS_IDX *metrics_idx;
  /* ALL_XMULTI_REF_STATUS related flag if at least one ref type supports stats */
  unsigned char has_stats;
  /* ALL_XMULTI_REF_STATUS related flag if at least one ref type supports metrics */
  unsigned char has_metrics;
} ALL_XMULTI_REF_STATUS;



typedef struct all_ims_info_s
{
  MBG_IMS_STATE state;
  MBG_IMS_FDM_INFO *fdm_info;
  MBG_IMS_FDM_LIMITS *fdm_limits;
  MBG_IMS_FDM_OUTPUT_INFO_IDX *fdm_outinfo_idx;
} ALL_IMS_INFO;


typedef struct all_ims_state_s
{
  MBG_IMS_SENSOR_STATE_IDX *sensor_state_idx;
  MBG_IMS_FDM_STATE *fdm_state;
  MBG_IMS_FDM_OUTPUT_STATE_IDX *fdm_output_state_idx;
} ALL_IMS_STATE;



typedef struct all_gpio_info_s
{
  MBG_GPIO_CFG_LIMITS cfg_limits;
  MBG_GPIO_INFO_IDX *infos;
} ALL_GPIO_INFO;


typedef struct all_gpio_state_s
{
  MBG_GPIO_STATUS_IDX *states;
} ALL_GPIO_STATE;


typedef struct mbg_io_port
{
    unsigned idx;
    MBG_IO_PORT_INFO p_info;
    MBG_IO_PORT_STATUS status;
    MBG_IO_PORT_TYPE_INFO *pt_infos;
    /* Shortcut to settings in p_info */
    MBG_IO_PORT_SETTINGS *setts;
    void *priv_data;
    void (*free_priv_data)(const struct mbg_io_port*, void*);
} MBG_IO_PORT;


typedef struct all_mbg_io_ports_s
{
    MBG_IO_PORT_LIMITS limits;
    unsigned num_ports;
    MBG_IO_PORT *ports;
} ALL_MBG_IO_PORTS;


#ifndef MAX_UCAP_ENTRIES
/*
 * According to Andre's GPS firmware this is the maximum
 * number of user captures that are preserved.
 */
#define MAX_UCAP_ENTRIES 585
#endif

typedef struct
{
  struct mbg_klist_head head;
  TTM ttm;
} UCAP_ENTRY;

typedef struct all_ucap_info_s
{
  uint32_t num_ucaps; /// User capture counter, see ::MAX_UCAP_ENTRIES
  struct mbg_klist_head list;
} ALL_UCAP_INFO;

// User Captures via Network configuration, see ::MBG_XFEATURE_UCAP_NET
typedef struct all_ucap_net_info_s
{
  MBG_UCAP_NET_GLB_INFO glb_info;
  MBG_UCAP_NET_RECV_INFO_IDX *recv_infos;
} ALL_UCAP_NET_INFO;


typedef struct mbg_user
{
  unsigned idx;
  MBG_USER_INFO info;
  MBG_USER_STATUS status;

  char salt[9];

  /* Pointer for 3rd party to connect some data */
  void *backref;
  /* Pointer for the mallocer to connect some private data */
  void *priv_data;
  /*
   * Can be used to free/release priv or cookie data (if necessary)
   * or do other stuff before user gets freed. But NEVER use release
   * to free the user itself!! This is done by the API right after calling
   * release.
   */
  void (*release_priv)(struct mbg_user*);
  void (*release_backref)(struct mbg_user*);

} MBG_USER;

typedef struct all_user_info_s
{
  MBG_USER_MNGMNT_INFO user_mngmnt_info;
  MBG_USER *users;                          ///< system users for internal authentication
  MBG_USER *user_levels;                    ///< global user levels for external authentication (i.e. radius/tacacs+)

} ALL_USER_INFO;


typedef struct mbg_service
{
  unsigned idx;
  MBG_SERVICE_INFO info;
  MBG_SERVICE_STATUS status;

  /* Pointer for 3rd party to connect some data */
  void *backref;
  /* Pointer for the mallocer to connect some private data */
  void *priv_data;
  /*
   * Can be used to free/release priv or cookie data (if necessary)
   * or do other stuff before service gets freed. But NEVER use release
   * to free the service itself!! This is done by the API right after calling
   * release.
   */
  void (*release_priv)(struct mbg_service*);
  void (*release_backref)(struct mbg_service*);

} MBG_SERVICE;

typedef struct all_service_info_s
{
  MBG_SERVICE_MGMT_INFO mgmt_info;
  MBG_SERVICE *svcs;

} ALL_SERVICE_INFO;


struct mbg_firmware;
struct mbg_firmware_ufu;


typedef struct mbg_firmware
{
  unsigned idx;
  MBG_FW_INFO info;
  struct mbg_firmware_ufu *ufus;                ///< Array of ::MBG_FW_INFO::num_ufus

  /* Pointer for the mallocer to connect some private data */
  void *priv_data;
  /*
   * Can be used to free/release priv or cookie data (if necessary)
   * or do other stuff before firmware gets freed. But NEVER use release
   * to free the firmware itself!! This is done by the API right after calling
   * release.
   */
  void (*release_priv)(struct mbg_firmware*);

} MBG_FIRMWARE;


typedef struct mbg_firmware_ufu
{
  unsigned idx;
  MBG_FW_UFU_INFO info;
  const struct mbg_firmware *firmware;              ///< Backref to firmware

  /* Pointer for the mallocer to connect some private data */
  void *priv_data;
  /*
   * Can be used to free/release priv data (if necessary)
   * or do other stuff before ufu gets freed. But NEVER use release
   * to free the ufu itself!! This is done by the API right after calling
   * release.
   */
  void (*release_priv)(struct mbg_firmware_ufu*);

} MBG_FIRMWARE_UFU;


typedef struct all_firmware_info_s
{
  MBG_FW_GLB_INFO glb_info;
  MBG_FIRMWARE *firmwares;      /// Array of ::MBG_FW_GLB_INFO::installed_fws

} ALL_FIRMWARE_INFO;


typedef struct mbg_database
{
    unsigned idx;
    MBG_DATABASE_INFO info;

    /* Pointer for the mallocer to connect some private data */
    void *priv_data;

    /*
     * Can be used to free/release priv data (if necessary)
     * or do other stuff before ufu gets freed. But NEVER use release
     * to free the ufu itself!! This is done by the API right after calling
     * release.
     */
    void (*release_priv)(struct mbg_database*);

} MBG_DATABASE;


typedef struct all_database_info_s
{
  MBG_DATABASE_GLB_INFO glb_info;
  /// Array of ::MBG_DATABASE_GLB_INFO::num_dbs. Yes, until num_dbs!!
  /// Databases might have different features and flags set,
  /// thus query all info.
  MBG_DATABASE *dbs;

} ALL_DATABASE_INFO;


typedef struct
{
  unsigned codec;
  const char *products;

} MBG_EXT_SYS_INFO_CPU_CODEC;

/// Do not access array via ::MBG_EXT_SYS_INFO_CPUS enum.
/// Only use it as lookup table. Initializer for ::MBG_EXT_SYS_INFO_CPU_CODEC.
#define MBG_EXT_SYS_INFO_CPU_CODECS                             \
{                                                               \
  {HPS_USB_HOST_G1_V0,      "microSync HSXXX"},                 \
  {HPS_USB_DEVICE_G1_V0,    "HPS100"},                          \
  {HPS_USB_DEVICE_G1_V1,    "SSP100"},                          \
  {MSSB_USB_HOST_G1_V0,     "microSyncHR, microSyncRX"}         \
}


/**
 * @brief A mode specifying how to interpret a ::PCPS_SIG_VAL
 *
 * Used with ::PCPS_TIME_EXT::comp_sig_mode. Depending on this mode
 * a signal value can be interpreted e.g. as signal strength (with
 * long wave or IRIG time code receivers), or as indicator whether an
 * antenna is connected (satellite receivers), or a network link is
 * available (PTP slaves) or not, and an appropriate status message
 * can be displayed.
 *
 * @see @ref PCPS_SIG_VAL_DEFS
 */
enum COMP_SIG_MODES
{
  COMP_SIG_MODE_NONE,       ///< signal value not used
  COMP_SIG_MODE_SIGNAL,     ///< input signal strength
  COMP_SIG_MODE_ANT_CONN,   ///< antenna connection state
  COMP_SIG_MODE_PORT_LINK,  ///< port link state
  N_CONN_SIG_MODES
};


/**
 * @brief Flag bits indicating if some extended status is available
 *
 * @see ::PCPS_TIME_EXT_FLAGS
 */
enum PCPS_TIME_EXT_FLAG_BITS
{
  PCPS_TIME_EXT_FLAG_BIT_UTC_VALID,   ///< ::PCPS_TIME_EXT::utc_offs field is valid
  N_PCPS_TIME_EXT_FLAG_BITS
};


/**
 * @brief Flag masks used with ::PCPS_TIME_EXT::flags
 *
 * @see ::PCPS_TIME_EXT_FLAG_BITS
 */
enum PCPS_TIME_EXT_FLAGS
{
  PCPS_TIME_EXT_FLAG_UTC_VALID = ( 1UL << PCPS_TIME_EXT_FLAG_BIT_UTC_VALID )  ///< see ::PCPS_TIME_EXT_FLAG_BIT_UTC_VALID
};



_ext BAUD_RATE mbg_baud_rates[N_MBG_BAUD_RATES]
#ifdef _DO_INIT
 = MBG_BAUD_RATES
#endif
;

_ext const char *mbg_baud_rate_strs[N_MBG_BAUD_RATES]
#ifdef _DO_INIT
 = MBG_BAUD_STRS
#endif
;

_ext const char *mbg_framing_strs[N_MBG_FRAMINGS]
#ifdef _DO_INIT
 = MBG_FRAMING_STRS
#endif
;

_ext const char *str_unknown
#ifdef _DO_INIT
 = "unknown"
#endif
;


_ext const char *str_undefined
#ifdef _DO_INIT
 = "(undefined)"
#endif
;


_ext const char *str_not_spc
#ifdef _DO_INIT
 = "not "
#endif
;



//### TODO
#define DEFAULT_MAX_STR_TYPE    2  // DEFAULT_N_STR_TYPE_GPS ?

_ext STR_TYPE_INFO default_str_type_info[DEFAULT_MAX_STR_TYPE]
#ifdef _DO_INIT
  = {
      { DEFAULT_STR_MODES,      "Default Time String", "Time", 0 },
      { DEFAULT_STR_MODES_UCAP, "Capture String",      "Cap",  0 }
    }
#endif
;



_ext const char *mbg_gpio_type_names[N_MBG_GPIO_TYPES]
#ifdef _DO_INIT
 = DEFAULT_GPIO_TYPES_SHORT_STRS
#endif
;

#define _get_gpio_type_name( _i ) \
  ( ( (_i) < N_MBG_GPIO_TYPES ) ? mbg_gpio_type_names[_i] : str_unknown )



_ext const char *mbg_gpio_port_state_names[N_MBG_GPIO_PORT_STATES]
#ifdef _DO_INIT
 = DEFAULT_GPIO_PORT_STATE_NAMES
#endif
;

#define _get_gpio_port_state_name( _i ) \
  ( ( (_i) < N_MBG_GPIO_PORT_STATES ) ? mbg_gpio_port_state_names[_i] : str_unknown )



_ext const char *mbg_gpio_signal_shape_names[N_MBG_GPIO_SIGNAL_SHAPES]
#ifdef _DO_INIT
 = DEFAULT_GPIO_SIGNAL_SHAPE_NAMES
#endif
;

#define _get_gpio_signal_shape_name( _i ) \
  ( ( (_i) < N_MBG_GPIO_SIGNAL_SHAPES ) ? mbg_gpio_signal_shape_names[_i] : str_unknown )



_ext const char *mbg_gpio_fixed_freq_strs[N_MBG_GPIO_FIXED_FREQ]
#ifdef _DO_INIT
 = MBG_GPIO_FIXED_FREQ_STRS
#endif
;

#define _get_gpio_fixed_freq_str( _i ) \
  ( ( (_i) < N_MBG_GPIO_FIXED_FREQ ) ? mbg_gpio_fixed_freq_strs[_i] : str_unknown )



_ext const char *xmr_holdover_status_mode_names[N_XMR_HOLDOVER_STATUS_MODES]
#ifdef _DO_INIT
 = XMR_HOLDOVER_STATUS_MODE_NAMES
#endif
;

#define _get_xmr_holdover_status_mode_name( _i ) \
  ( ( (_i) < N_XMR_HOLDOVER_STATUS_MODES ) ? xmr_holdover_status_mode_names[_i] : str_unknown )



_ext const char *ptp_role_strs[N_PTP_ROLES]
#ifdef _DO_INIT
 = PTP_ROLE_STRS
#endif
;

#define _get_ptp_role_str_dflt( _i, _dflt ) \
  ( ( (_i) < N_PTP_ROLES ) ? ptp_role_strs[_i] : _dflt )

#define _get_ptp_role_str( _i ) \
  _get_ptp_role_str_dflt( _i, str_unknown )



_ext const char *ptp_nw_protocol_strs[N_PTP_NW_PROT]
#ifdef _DO_INIT
  = PTP_NW_PROT_STRS
#endif
;

#define _get_ptp_nw_protocol_str_dflt( _i, _dflt ) \
  ( ( (_i) < N_PTP_NW_PROT ) ? ptp_nw_protocol_strs[_i] : _dflt )

#define _get_ptp_nw_protocol_str( _i ) \
  _get_ptp_nw_protocol_str_dflt(_i, str_unknown )



_ext const char *ptp_port_state_strs[N_PTP_PORT_STATE]
#ifdef _DO_INIT
  = PTP_PORT_STATE_STRS
#endif
;

#define _get_ptp_port_state_str_dflt( _i, _dflt ) \
  ( ( (_i) < N_PTP_PORT_STATE ) ? ptp_port_state_strs[_i] : _dflt )

#define _get_ptp_port_state_str( _i ) \
  _get_ptp_port_state_str_dflt( _i, str_unknown )



_ext const char *ptp_clock_accuracy_strs[N_PTP_CLOCK_ACCURACY - PTP_CLOCK_ACCURACY_NUM_BIAS]
#ifdef _DO_INIT
  = PTP_CLOCK_ACCURACY_STRS
#endif
;

#define _get_ptp_clock_accuracy_str_dflt( _i, _dflt ) \
  ( ( ( (_i) >= PTP_CLOCK_ACCURACY_NUM_BIAS ) && ( (_i) < N_PTP_CLOCK_ACCURACY ) ) ? \
    ptp_clock_accuracy_strs[(_i) - PTP_CLOCK_ACCURACY_NUM_BIAS] : _dflt )

#define _get_ptp_clock_accuracy_str( _i ) \
  _get_ptp_clock_accuracy_str( _i, str_unknown )



_ext const char *ptp_delay_mechanism_strs[N_PTP_DELAY_MECH]
#ifdef _DO_INIT
  = PTP_DELAY_MECH_NAMES
#endif
;

#define _get_ptp_delay_mechanism_str_dflt( _i, _dflt ) \
  ( ( (_i) < N_PTP_DELAY_MECH ) ? ptp_delay_mechanism_strs[_i] : _dflt )

#define _get_ptp_delay_mechanism_str( _i ) \
  _get_ptp_delay_mechanism_str_dflt( _i, str_unknown )




// Terminated by { 0, NULL }
_ext const PTP_TABLE ptp_time_source_table[]
#ifdef _DO_INIT
  = PTP_TIME_SOURCE_TABLE
#endif
;



/**
 * @brief A structure which holds all satellite info read from a device
 *
 * @note The structure contains pointers to allocated memory,
 * so it can ***not*** be used to exchange data between
 * different programs or devices.
 *
 * @see ::mbgextio_get_all_gnss_sv_info
 */
typedef struct ALL_GNSS_SV_INFO_s
{
  uint16_t n_gnss_sv_stati;    // Number of ::GNSS_SV_STATUS_IDX that can be read (if > 0, no ::SV_INFO can be read)
  uint16_t n_sv_infos;         // Number of ::SV_INFO that can be read

  GNSS_SV_STATUS_IDX *gnss_sv_stati;
  SV_INFO *sv_infos;

} ALL_GNSS_SV_INFO;



/**
 * @brief Count the number of bits which are not 0
 *
 * @param[in] val  Value to be tested
 *
 * @return The number of non-zero bits in val
 */
static __mbg_inline /*HDR*/
int num_bits_set( long val )
{
  int bits_set = 0;
  size_t i;

  for ( i = 0; i < ( 8 * sizeof( val ) ); i++ )
  {
    if ( val & 1 )
      bits_set++;

    val >>= 1;
  }

  return bits_set;

}  // num_bits_set



static __mbg_inline /*HDR*/
/**
 * @brief Compute the value of 2 to the power of n.
 *
 * In the special case of a base 2 and a small exponent n
 * the function pow( base, n ) can simply be replaced by
 * a shift operation which executes ***very*** much faster
 * than the floating point pow() function.
 * In addition this avoids the requirement of explicitly
 * linking against the math library.
 *
 * Please note that if the result is a 32 bit type then @p exp
 * must be less than 32.
 * @param[in]  exp  The number of which to compute pow( 2, n ).
 *
 * @return  The result as integer.
 */
uint32_t uint32_pow2( int exp  )
{
  return 1 << exp;

}  // uint32_pow2



static __mbg_inline /*HDR*/
/**
 * @brief Compute the number of values to be used for PTP lucky packet median filtering.
 *
 * This only makes sense if the sync interval is negative,
 * and below a certain limit, e.g. < -3.
 *
 * @param[in]  ln_sync_intv  The sync_intv parameter from a PTP configuration.
 *
 * @return  The result as integer.
 */
int lucky_packet_median_nvals( int8_t ln_sync_intv )
{
  // For example, if ln_sync_interval is -4 then pow( 2, 4 ) == 16, and the
  // the expected number of samples for the lucky packet median filter is 15.
  return uint32_pow2( -ln_sync_intv ) - 1;

}  // lucky_packet_median_nvals



#if !defined( MBG_TGT_KERNEL )

/**
 * @brief Check if a device ID refers to a serial port
 *
 * @param[in] dev_id  A string with the device name or port name
 *
 * @see ::DEFAULT_SERIAL_DEVICE_NAME
 *
 * @return true if the device id contains the name of a serial port, else false
 */
static __mbg_inline /*HDR*/
bool device_id_is_serial( const char *dev_id )
{
  #if defined( MBG_TGT_NO_TGT )
    return false;  //### FIXME
  #elif defined( MBG_TGT_WIN32 )
    //##++++ There may be also serial ports under Windows
    // which don't have "COM" in their name.
    return strstr( dev_id, "COM" ) != NULL;
  #elif defined( MBG_TGT_LINUX )
    return strstr( dev_id, "/dev/ttyS" ) != NULL     // standard serial device
        || strstr( dev_id, "/dev/ttyUSB" ) != NULL;  // serial-to-USB adapter
  #elif defined( MBG_TGT_FREEBSD )
    return strstr( dev_id, "/dev/ttyu" ) != NULL     // dial-in device (standard), FreeBSD 10 and newer
        || strstr( dev_id, "/dev/cuau" ) != NULL     // dial out device, FreeBSD 10 and newer
        || strstr( dev_id, "/dev/ttyd" ) != NULL     // dial-in device (standard), before FreeBSD 10
        || strstr( dev_id, "/dev/cuad" ) != NULL;    // dial-out device, before FreeBSD 10
  #elif defined( MBG_TGT_NETBSD )
    // TODO This is just an ugly hack which identifies
    // some traditional device names as serial ports.
    // There are quite a few other possible names.
    // See "man 4 com" and "man 4 tty".
    return strstr( dev_id, "/dev/tty0" ) != NULL     // traditional dial-in devices
        || strstr( dev_id, "/dev/dty0" ) != NULL;    // traditional dial-out devices
  #elif defined( MBG_TGT_QNX_NTO )
    return strstr( dev_id, "/dev/ser" ) != NULL;
  #elif defined( MBG_TGT_DOS )
    return strstr( dev_id, "COM" ) != NULL;
  #else
    #error device_id_is_serial() needs to be implemented for this platform
  #endif

}  // device_id_is_serial



/**
 * @brief Check if a device ID refers to a LAN connection
 *
 * @param[in] dev_id  A string with the device ID
 *
 * @return true if the device id specifies a LAN connection, else false
 */
static __mbg_inline /*HDR*/
bool device_id_is_lan( const char *dev_id )
{
  return strstr( dev_id, "LAN" ) != NULL;

}  // device_id_is_lan

#endif  // !defined( MBG_TGT_KERNEL )



#if defined( _PRELIMINARY_CODE )

static __mbg_inline /*HDR*/
MBG_TLV_UID mbg_tlv_create_id( void )
{
#if defined( MBG_TGT_LINUX )   ///### FIXME for FreeBSD and Windows
  struct sysinfo info;

  // Linux specific, implement Windows equivalent
  sysinfo( &info );
  return ( (MBG_TLV_UID) ( ( time( NULL ) >> 16 ) | ( info.uptime << 16 ) ) );
#else
  return rand();
#endif
} // mbg_tlv_create_id

#endif  // defined( _PRELIMINARY_CODE )



#if !defined( MBG_TGT_KERNEL )

/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
 * @brief Check if a software revision name should be displayed.
 *
 * The software revision name is usually empty, except if the
 * firmware is a customized version, in which case the field
 * contains an identifier string.
 *
 * There are some standard firmware versions where this string
 * is not empty but padded with spaces, etc., so we try to
 * clean this up, so it can be displayed appropriately.
 *
 * @param[in,out]  p        The ::SW_REV name to check.
 * @param[in]      verbose  The app's verbosity level.
 *
 * @return  != 0 if SW name should be displayed.
 */
 int chk_sw_rev_name( SW_REV *p, int verbose ) ;

 /**
 * @brief Search a string table for a specific string.
 *
 * @param[in]  search     The string to be searched for in table @a str_table.
 * @param[in]  str_table  A table of strings like 'const char *strs[N_STRS]'.
 * @param[in]  n_entries  The number of strings in table @a str_table.
 *
 * @return  The index number of the string table entry matching the @a search string,
 *          or -1 if the @a search string could not be found.
 */
 int get_str_idx( const char *search, const char *str_table[], int n_entries ) ;

 /**
 * @brief Search the ::mbg_baud_rates table for a specific baud rate.
 *
 * @param[in]  baud_rate  The baud_rate to be searched for.
 *
 * @return  The index number of the specific @a baud_rate in table ::mbg_baud_rates,
 *          or -1 if the specific @a baud_rate could not be found.
 */
 int get_baud_rate_idx( BAUD_RATE baud_rate ) ;

 /**
 * @brief Search the ::mbg_framing_strs table for a specific framing string.
 *
 * @param[in]  framing  The framing string to be searched for, e.g. "8N1".
 *
 * @return  The index number of the specific @a framing in table ::mbg_framing_strs,
 *          or -1 if the specific @a framing could not be found.
 */
 int get_framing_idx( const char *framing ) ;

 /**
 * @brief Convert ::PORT_PARM::mode to ::PORT_SETTINGS::mode
 *
 * This function is used to evaluate the code from a @a mode field of a legacy
 * ::PORT_PARM structure and set up the appropriate fields in a ::PORT_SETTINGS
 * structure.
 *
 * @param[out]  p_ps         Pointer to a ::PORT_SETTINGS structure to be updated.
 * @param[in]   pp_mode      The mode code from a ::PORT_PARM structure, see ::LGCY_STR_MODES.
 * @param[in]   cap_str_idx  The capture string index number for the case that @a pp_mode is
 *                           a capture string mode. Usually 1 for legacy devices.
 *
 * @ingroup cfg_hlp_com_parm_cnv_fncs
 * @see ::port_parm_mode_from_port_settings (the complementary function)
 * @see ::port_settings_from_port_parm
 */
 void port_settings_from_port_parm_mode( PORT_SETTINGS *p_ps, uint8_t pp_mode, int cap_str_idx ) ;

 /**
 * @brief Convert a ::PORT_SETTINGS::mode to a legacy ::PORT_PARM::mode
 *
 * This function is used to derive a a legacy ::PORT_PARM::mode value
 * from a ::PORT_SETTINGS structure.
 *
 * @param[out]  pp_mode      Pointer to a ::PORT_PARM::mode variable to be updated.
 * @param[in]   p_ps         Pointer to a ::PORT_SETTINGS structure to be evaluated.
 * @param[in]   cap_str_idx  The capture string index number for legacy devices, which is usually 1.
 *
 * @ingroup cfg_hlp_com_parm_cnv_fncs
 * @see ::port_settings_from_port_parm_mode (the complementary function)
 * @see ::port_parm_from_port_settings
 */
 void port_parm_mode_from_port_settings( uint8_t *pp_mode, const PORT_SETTINGS *p_ps, int cap_str_idx ) ;

 /**
 * @brief Set up a ::PORT_SETTINGS structure from a legacy ::PORT_PARM structure.
 *
 * @param[out]  p_ps         Pointer to a ::PORT_SETTINGS structure to be updated.
 * @param[in]   port_idx     Index number of the port settings to be converted.
 * @param[in]   p_pp         The ::PORT_PARM structure to be evaluated, contains settings for 2 ports.
 * @param[in]   cap_str_idx  The capture string index number for legacy devices, which is usually 1.
 *
 * @ingroup cfg_hlp_com_parm_cnv_fncs
 * @see ::port_parm_from_port_settings (the complementary function)
 * @see ::port_settings_from_port_parm_mode
 */
 void port_settings_from_port_parm( PORT_SETTINGS *p_ps, int port_idx, const PORT_PARM *p_pp, int cap_str_idx ) ;

 /**
 * @brief Set up a a legacy ::PORT_PARM structure from a ::PORT_SETTINGS structure.
 *
 * @param[out]  p_pp         Pointer to a ::PORT_PARM structure to be updated.
 * @param[in]   port_idx     Index number of the port settings to be converted.
 * @param[out]  p_ps         Pointer to a ::PORT_SETTINGS structure to be updated.
 * @param[in]   cap_str_idx  The capture string index number for legacy devices, which is usually 1.
 *
 * @ingroup cfg_hlp_com_parm_cnv_fncs
 * @see ::port_settings_from_port_parm (the complementary function)
 * @see ::port_parm_mode_from_port_settings
 */
 void port_parm_from_port_settings( PORT_PARM *p_pp, int port_idx, const PORT_SETTINGS *p_ps, int cap_str_idx ) ;

 /**
 * @brief Check if all serial port capabilities reported by a device are supported by the current driver.
 *
 * @param[in]   p_pi               Address of a ::PORT_INFO structure to be checked.
 * @param[out]  str_type_info_idx  An array of ::STR_TYPE_INFO_IDX structures supported by the device
 * @param[in]   n_str_type         The number of entries in the @a str_type_info_idx array.
 *
 * @return  A bit mask of @ref MBG_COM_CFG_STATUS_MASKS flags indicating which type of parameter
 *          may not be fully supported. Should be 0 if everything is OK.
 *
 * @see ::is_valid_port_info
 * @see @ref MBG_COM_CFG_STATUS_MASKS
 */
 uint32_t check_valid_port_info( const PORT_INFO *p_pi, const STR_TYPE_INFO_IDX str_type_info_idx[], int n_str_type ) ;

 /**
 * @brief Check if the content of a ::PORT_INFO structure is known and valid.
 *
 * @param[in]  p_pi               Address of a ::PORT_INFO structure to be checked.
 * @param[in]  str_type_info_idx  An array of string type information.
 * @param[in]  n_str_type         The number of entries in the @a str_type_info_idx array.
 *
 * @return  @a true if the ::PORT_INFO structure contains valid data,
 *          else @a false.
 *
 * @see ::check_valid_port_info
 */
 bool is_valid_port_info( const PORT_INFO *p_pi, const STR_TYPE_INFO_IDX str_type_info_idx[], int n_str_type ) ;

 /**
 * @brief Setup an array of ::PORT_INFO_IDX structures from a ::PORT_PARM.
 *
 * Some legacy GPS receivers that don't provide a ::RECEIVER_INFO structure
 * also provide a ::PORT_PARM structure with the current serial port settings
 * only.
 *
 * This function sets up an array of ::PORT_INFO_IDX structures with the
 * associated settings, and fills up the remaining ::PORT_INFO fields with
 * the well-known supported settings, so applications can simply deal with
 * the current API structures.
 *
 * @param[out]  pii   Array with @a p_ri->n_com_ports ::PORT_INFO_IDX elements to be filled up.
 * @param[in]   p_pp  Pointer to a ::PORT_PARM structure providing the current COM port settings.
 * @param[in]   p_ri  Pointer to a ::RECEIVER_INFO structure providing the number of supported COM ports.
 *
 * @return  One of the @ref MBG_RETURN_CODES, but actually always ::MBG_SUCCESS.
 *
 * @ingroup cfg_hlp_com_parm_cnv_fncs
 * @see setup_default_str_type_info_idx
 */
 int setup_port_info_from_port_parm( PORT_INFO_IDX pii[], const PORT_PARM *p_pp, const RECEIVER_INFO *p_ri ) ;

 /**
 * @brief Setup an array of ::STR_TYPE_INFO_IDX for well-known string types.
 *
 * Some legacy GPS receivers that don't provide a ::RECEIVER_INFO structure
 * also don't support the ::STR_TYPE_INFO structure to indicate which serial
 * string formats are supported.
 *
 * This function sets up an array of ::STR_TYPE_INFO_IDX structures with the
 * well-known supported string types, so applications can simply deal with
 * the current API structures.
 *
 * @param[out]  stii  Array with at least @a p_ri->n_str_type ::STR_TYPE_INFO_IDX elements to be filled up.
 * @param[in]   p_ri  Pointer to a ::RECEIVER_INFO structure providing the number of supported COM ports.
 *
 * @return  One of the @ref MBG_RETURN_CODES, but actually always ::MBG_SUCCESS.
 *
 * @ingroup cfg_hlp_com_parm_cnv_fncs
 * @see ::setup_port_info_from_port_parm
 */
 int setup_default_str_type_info_idx( STR_TYPE_INFO_IDX stii[], const RECEIVER_INFO *p_ri ) ;

 /**
 * @brief Determine how many GNSS systems are marked to be supported.
 *
 * This function counts the number of bits that are set to indicate
 * that specific GNSS types (GPS, GLONASS, Galileo, ...) are supported
 * by the device, and sets up the ::ALL_GNSS_INFO::n_gnss_supp field
 * accordingly.
 *
 * @param[in,out]  p_agi  Address of an ::ALL_GNSS_INFO structure to be updated.
 *
 * @return  ::MBG_SUCCESS on success, or ::MBG_ERR_N_GNSS_EXCEEDS_SUPP if the
 *          number of bits that are set exceeds the number of known GNSS types.
 */
 int chk_set_n_gnss_supp( ALL_GNSS_INFO *p_agi ) ;

 /**
 * @brief Setup GNSS sat info from the legacy GPS ::STAT_INFO.
 *
 * This simplyifies further evaluation since e.g. a common
 * display routine can be used.
 *
 * @param[in,out]  p_agi  Address of an ::ALL_GNSS_INFO structure to be updated.
 */
 void setup_gps_only_gnss_sat_info_idx_from_stat_info( ALL_GNSS_INFO *p_agi ) ;

 /**
 * @brief Setup GNSS mode info from the legacy GPS ::STAT_INFO.
 *
 * This simplyifies further evaluation since e.g. a common
 * display routine can be used.
 *
 * @param[in,out]  p_agi  Address of an ::ALL_GNSS_INFO structure to be updated.
 */
 int setup_gps_only_gnss_mode_info_from_stat_info( ALL_GNSS_INFO *p_agi ) ;

 void chk_free_dev_hw_id( DEVICE_INFO *p ) ;
 int alloc_dev_hw_id( DEVICE_INFO *p, size_t len ) ;
 const char *get_fw_id_from_hw_id( const char *hw_id ) ;
 const char *get_hw_id_from_fw_id( const char *fw_id ) ;
 /**
 * @brief Return the currently used ::MBG_IO_PORT_TYPE_INFO for the appropriate ::MBG_IO_PORT.
 *
 * @param[in]  port       Pointer to the ::MBG_IO_PORT to search for port_type
 * @param[in]  port_type  Enum value of ::MBG_IO_PORT_TYPES to search for in @a port (::MBG_IO_PORT)
 *
 * @return  Pointer to the ::MBG_IO_PORT_TYPE_INFO_IDX found, or NULL.
 */
 MBG_IO_PORT_TYPE_INFO *get_io_port_type_info( const MBG_IO_PORT *port, uint16_t port_type ) ;

 /**
 * @brief Initialize a ::MBG_TLV_ANNOUNCE structure.
 *
 * @param[out]  tlv            Pointer to a ::MBG_TLV_ANNOUNCE structure.
 * @param[in]   uid            Unique sender ID used as identifier with all
 *                             subsequent messages related to this transaction.
 * @param[in]   tlv_feat_type  One of the ::MBG_TLV_FEAT_TYPES.
 * @param[in]   total_bytes    Total number of bytes of all upcoming TLVs.
 */
 void mbg_tlv_announce_init( MBG_TLV_ANNOUNCE *tlv, MBG_TLV_UID uid, MBG_TLV_TYPE tlv_feat_type, uint32_t total_bytes ) ;

 /**
 * @brief Initialize a ::MBG_TLV structure.
 *
 * @param[out]  tlv          Pointer to a valid ::MBG_TLV structure.
 * @param[in]   uid          Unique sender ID used as identifier for each further
 *                           TLV message related to this type.
 * @param[in]   tlv_type     Type identifier, see ::MBG_TLV_TYPES.
 * @param[in]   total_bytes  Total number of bytes belonging to this TLV transaction
 *                           (which is very likely split into several TLVs)
 */
 void mbg_tlv_init( MBG_TLV *tlv, MBG_TLV_UID uid, MBG_TLV_TYPE tlv_type, uint32_t total_bytes ) ;

 /**
 * @brief Initialize ::MBG_TLV_RCV_STATE structure.
 *
 * @param[out]  state        Pointer to ::MBG_TLV_RCV_STATE structure to be initialized.
 * @param[in]   uid          Unique sender ID used as identifier for each further
 *                           TLV message related to this type.
 * @param[in]   total_bytes  Total number of bytes belonging to this TLV transaction
 *                           (which is very likely split into several TLVS)
 */
 void mbg_tlv_rcv_state_init( MBG_TLV_RCV_STATE *state, MBG_TLV_UID uid, uint32_t total_bytes ) ;

 /**
 * @brief Print some Meinberg OS revision information into a string buffer.
 *
 * @param[out] s        The string buffer to be filled.
 * @param[in]  max_len  Size of the output buffer for 0-terminated string.
 * @param[in]  prefix   Pointer to an optional prefix string, or NULL.
 * @param[in]  suffix   Pointer to an optional suff string, or NULL.
 * @param[in]  rev      The revision code which will be split into major, minor, and patch version.
 *
 * @return Length of the string in the buffer.
 *
 * @see @ref group_ext_sys_info
 */
 int mbg_snprint_revision( char *s, size_t max_len, const char *prefix, const char *suffix, uint32_t rev ) ;

 /**
 * @brief Check if all ::RECEIVER_INFO capabilities reported by a device are supported by the current driver.
 *
 * @param[in]  p  Address of a ::RECEIVER_INFO structure to be checked.
 *
 * @return  ::MBG_SUCCESS if everything is OK, one of the @ref MBG_ERROR_CODES, as appropriate.
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_receiver_info( const RECEIVER_INFO *p ) ;

 _NO_MBG_API_ATTR int _MBG_API chk_dev_xbp_supp_nodes( const ALL_XBP_INFO *info ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_net_cfg_supp_stage_2( const ALL_NET_CFG_INFO *info ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_ntp_supp_client( const ALL_NTP_CFG_INFO *info ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_ntp_supp_server( const ALL_NTP_CFG_INFO *info ) ;
 /**
 * @brief Check if the ::XMULTI_EXT_REF_INSTANCES structure is supported.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 *
 * @return  ::MBG_SUCCESS or ::MBG_ERR_NOT_SUPP_BY_DEV,
 *          as returned by ::chk_mrs_instances_flags.
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_supp_ext_instances( const ALL_XMULTI_REF_INFO *info ) ;

 /**
 * @brief Check if the ::XMULTI_EXT_REF_INSTANCES structure is supported.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 *
 * @return  ::MBG_SUCCESS or ::MBG_ERR_NOT_SUPP_BY_DEV,
 *          as returned by ::chk_mrs_instances_flags.
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_not_configurable( const ALL_XMULTI_REF_INFO *info ) ;

 /**
 * @brief Check if the ::XMRIF_BIT_NO_STATUS bit is set.
 *
 * If this feature bit is set, ***no*** status, stats, etc., are provided.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 *
 * @return  ::MBG_SUCCESS if the bit is set, or ::MBG_ERR_NOT_SUPP_BY_DEV,
 *          as returned by ::chk_mrs_instances_flags.
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_no_status( const ALL_XMULTI_REF_INFO *info ) ;

 /**
 * @brief Check if the ::XMRIF_BIT_MRF_NONE_SUPP bit is set.
 *
 * If this feature bit is set, the MRS pseudo-type ::MULTI_REF_NONE is supported.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 *
 * @return  ::MBG_SUCCESS if the bit is set, or ::MBG_ERR_NOT_SUPP_BY_DEV,
 *          as returned by ::chk_mrs_instances_flags.
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_ref_supp_mrf_none( const ALL_XMULTI_REF_INFO *info ) ;

 /**
 * @brief Check if the ::XMR_EXT_SRC_INFO structure is supported.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 *
 * @return  ::MBG_SUCCESS or ::MBG_ERR_NOT_SUPP_BY_DEV,
 *          as returned by ::chk_mrs_instances_flags.
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_ref_supp_ext_src_info( const ALL_XMULTI_REF_INFO *info ) ;

 /**
 * @brief Check if the ::::XMR_EXT_SRC_INFO structure is supported.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 *
 * @return  ::MBG_SUCCESS or ::MBG_ERR_NOT_SUPP_BY_DEV,
 *          as returned by ::chk_mrs_instances_flags.
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_ref_supp_holdover_status( const ALL_XMULTI_REF_INFO *info ) ;

 /**
 * @brief Check if an XMR source provides ::XMR_STATS.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 * @param[in]  type  One of the ::MULTI_REF_TYPES, except ::MULTI_REF_NONE which is -1.
 *
 * @see ::chk_dev_supp_xmulti_ref_ext_src_info
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_ref_supp_ext_source_stats( const ALL_XMULTI_REF_INFO *info, int type ) ;

 /**
 * @brief Check if an XMR source provides ::XMR_METRICS.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 * @param[in]  type  One of the ::MULTI_REF_TYPES, except ::MULTI_REF_NONE which is -1.
 *
 * @see ::chk_dev_xmulti_ref_supp_ext_source_adv_metrics
 * @see ::chk_dev_supp_xmulti_ref_ext_src_info
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_ref_supp_ext_source_metrics( const ALL_XMULTI_REF_INFO *info, int type ) ;

 /**
 * @brief Check if an XMR source supports advanced XMR QL metrics configuration
 *
 * If this feature is not available, ::XMR_METRICS can not be configured.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 * @param[in]  type  One of the ::MULTI_REF_TYPES, except ::MULTI_REF_NONE which is -1.
 *
 * @see ::XMR_QL_LIMITS
 * @see ::chk_dev_xmulti_ref_supp_ext_source_metrics
 * @see ::chk_dev_supp_xmulti_ref_ext_src_info
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_ref_supp_ext_source_adv_metrics( const ALL_XMULTI_REF_INFO *info, int type ) ;

 /**
 * @brief Check if an XMR source XMR source supports coasting mode.
 *
 * If this feature is not available, ::XMR_METRICS can not be configured.
 *
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 * @param[in]  type  One of the ::MULTI_REF_TYPES, except ::MULTI_REF_NONE which is -1.
 *
 * @see ::XMR_QL_LIMITS
 * @see ::chk_dev_supp_xmulti_ref_ext_src_info
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_xmulti_ref_supp_ext_source_coasting( const ALL_XMULTI_REF_INFO *info, int type ) ;

 _NO_MBG_API_ATTR int _MBG_API chk_dev_ims_has_fdm( const ALL_IMS_INFO *info ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_ims_is_volt_out_enabled( const ALL_IMS_STATE *ims_state, unsigned idx ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_ims_is_volt_out_overload( const ALL_IMS_STATE *ims_state, unsigned idx ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_ims_is_pll_locked( const ALL_IMS_STATE *ims_state, unsigned idx ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_gpio_supp_ass_idx( const ALL_GPIO_INFO *gpio_info, unsigned idx ) ;
 _NO_MBG_API_ATTR int _MBG_API chk_dev_gpio_dep_on_ass_idx( const ALL_GPIO_INFO *gpio_info, unsigned idx ) ;
 /**
 * @brief Check whether GPIO provides a status.
 *
 * @param[out]  info  Pointer to a ::ALL_GPIO_INFO structure to be filled up ::FIXME
 * @param[in]  info  Address of an ::ALL_XMULTI_REF_INFO structure to be checked.
 *
 * @return One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbgextio_dev_has_gpio
 * @see ::mbg_chk_dev_has_gpio
 * @see ::free_all_gpio_info
 */
 _NO_MBG_API_ATTR int _MBG_API chk_dev_gpio_has_status( const ALL_GPIO_INFO *info ) ;

 /**
 * @brief Free an ::ALL_XBP_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_XBP_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_xbp_info
 */
 void free_all_xbp_info( ALL_XBP_INFO *p ) ;

 /**
 * @brief Free an ::ALL_NET_CFG_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_NET_CFG_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_net_cfg_info
 */
 void free_all_net_cfg_info( ALL_NET_CFG_INFO *p ) ;

 /**
 * @brief Free an ::ALL_NET_STATUS_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_NET_STATUS_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_net_status_info
 */
 void free_all_net_status_info( ALL_NET_STATUS_INFO *p ) ;

 /**
 * @brief Free an ::ALL_SNMP_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_SNMP_INFO structure to be freed
 */
 void free_all_snmp_info ( ALL_SNMP_INFO *p ) ;

 /**
 * @brief Free an ::ALL_SYSLOG_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_SYSLOG_INFO structure to be freed
 */
 void free_all_syslog_info ( ALL_SYSLOG_INFO *p ) ;

 /**
 * @brief Free an ::ALL_MONITORING_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_MONITORING_INFO structure to be freed
 */
 void free_all_monitoring_info ( ALL_MONITORING_INFO *p ) ;

 /**
 * @brief Free an ::ALL_EVENTS structure
 *
 * @param[in]  p  Pointer to the ::ALL_EVENTS structure to be freed
 */
 void free_all_events( ALL_EVENTS *p ) ;

 /**
 * @brief Free an ::ALL_XMULTI_REF_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_XMULTI_REF_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_xmulti_ref_info
 * @see ::mbg_get_all_xmulti_ref_info
 */
 void free_all_xmulti_ref_info( ALL_XMULTI_REF_INFO *p ) ;

 /**
 * @brief Free an ::ALL_XMULTI_REF_STATUS structure
 *
 * @param[in]  p  Pointer to the ::ALL_XMULTI_REF_STATUS structure to be freed
 *
 * @see ::mbgextio_get_all_xmulti_ref_status
 * @see ::mbg_get_all_xmulti_ref_status
 */
 void free_all_xmulti_ref_status( ALL_XMULTI_REF_STATUS *p ) ;

 /**
 * @brief Free an ::ALL_PTP_V1_COMMON_DATASETS structure allocated by ::mbgextio_get_all_ptp_v1_common_datasets
 *
 * @param[in]  p  Pointer to the ::ALL_PTP_V1_COMMON_DATASETS structure to be freed
 *
 * @see ::mbgextio_get_all_ptp_v1_common_datasets
 */
 void free_all_ptp_v1_common_datasets( ALL_PTP_V1_COMMON_DATASETS *p ) ;

 /**
 * @brief Free an ::ALL_PTP_V2_COMMON_DATASETS structure allocated by ::mbgextio_get_all_ptp_v2_common_datasets
 *
 * @param[in]  p  Pointer to the ::ALL_PTP_V2_COMMON_DATASETS structure to be freed
 *
 * @see ::mbgextio_get_all_ptp_v2_common_datasets
 */
 void free_all_ptp_v2_common_datasets( ALL_PTP_V2_COMMON_DATASETS *p ) ;

 /**
 * @brief Free an ::ALL_NTP_CFG_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_NTP_CFG_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_ntp_cfg_info
 */
 void free_all_ntp_cfg_info( ALL_NTP_CFG_INFO *p ) ;

 /**
 * @brief Free an ::ALL_NTP_STATUS structure
 *
 * @param[in]  p  Pointer to the ::ALL_NTP_STATUS structure to be freed
 *
 * @see ::mbgextio_get_all_ntp_status
 */
 void free_all_ntp_status( ALL_NTP_STATUS *p ) ;

 /**
 * @brief Free an ::ALL_PTP_NG_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_PTP_NG_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_ptp_ng_info
 */
 void free_all_ptp_ng_info( ALL_PTP_NG_INFO *p ) ;

 /**
 * @brief Free a list of ::MBG_SYS_REF_SRC structures
 *
 * @param[in]  p            Pointer to the ::MBG_SYS_REF_SRC list to be freed
 * @param[in]  free_source  Address of a function to be called to free resources.
 *
 * @see ::mbgextio_get_all_sys_ref_info
 * @see ::free_all_sys_ref_info
 */
 void free_all_sys_ref_srcs( struct mbg_klist_head *p, void (*free_source)( MBG_SYS_REF_SRC * ) ) ;

 /**
 * @brief Free an ::ALL_SYS_REF_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_SYS_REF_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_sys_ref_info
 * @see ::free_all_sys_ref_srcs
 */
 void free_all_sys_ref_info( ALL_SYS_REF_INFO *p ) ;

 /**
 * @brief Free memory allocated by ::mbgextio_get_all_ims_info
 *
 * @param[in]  p  Pointer to the ::ALL_IMS_INFO structure to be freed
 *
 * @see ::mbgextio_dev_has_ims
 * @see ::mbgextio_get_all_ims_info
 * @see ::mbgextio_get_all_ims_state
 */
 void free_all_ims_info( ALL_IMS_INFO *p ) ;

 /**
 * @brief Free memory allocated by ::mbgextio_get_all_ims_state
 *
 * @param[in]  p  Pointer to the ::ALL_IMS_STATE structure to be freed
 *
 * @see ::mbgextio_dev_has_ims
 * @see ::mbgextio_get_all_ims_info
 * @see ::mbgextio_get_all_ims_state
 */
 void free_all_ims_state( ALL_IMS_STATE *p ) ;

 /**
 * @brief Free memory allocated by ::mbgextio_get_all_gpio_info
 *
 * @param[in]  p  Pointer to the ::ALL_GPIO_INFO structure to be freed
 *
 * @return One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbgextio_dev_has_gpio
 * @see ::mbgextio_get_all_gpio_info
 */
 void free_all_gpio_info( ALL_GPIO_INFO *p ) ;

 /**
 * @brief Free memory allocated by ::mbgextio_get_all_io_ports
 *
 * @param[in]  p  Pointer to the ::ALL_MBG_IO_PORTS structure to be freed
 *
 * @return One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbgextio_dev_has_io_ports
 * @see ::mbgextio_get_all_io_ports
 * @see ::mbgextio_get_all_io_port_status
 */
 void free_all_io_ports( ALL_MBG_IO_PORTS *p ) ;

 /**
 * @brief Free memory allocated by ::mbgextio_get_all_gpio_state
 *
 * @param[in]  p  Pointer to the ::ALL_GPIO_STATE structure to be freed
 *
 * @return One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbgextio_dev_has_gpio
 * @see ::mbgextio_get_all_gpio_state
 */
 void free_all_gpio_state( ALL_GPIO_STATE *p ) ;

 /**
 * Allocates memory for a new ::UCAP_ENTRY structure
 *
 * @return The new allocated ::UCAP_ENTRY or NULL if the calloc call was not successful
 */
 UCAP_ENTRY *calloc_ucap_entry( void ) ;

 /**
 * @brief Free memory allocated by ::mbgextio_get_all_ucap_info
 *
 * @param[in]  p  Pointer to the ::ALL_UCAP_INFO structure to freed
 *
 * @return One of the @ref MBG_RETURN_CODES
 *
 * @see ::mbgextio_dev_has_ucap
 * @see ::mbg_chk_dev_has_ucap
 * @see ::mbgextio_get_all_ucap_info
 * @see ::mbg_get_all_ucap_info
 */
 void free_all_ucap_info( ALL_UCAP_INFO *p ) ;

 /**
 * @brief Free an ::ALL_UCAP_NET_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_UCAP_NET_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_ucap_net_info
 */
 void free_all_ucap_net_info( ALL_UCAP_NET_INFO *p ) ;

 /**
 * @brief Free an ::ALL_USER_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_USER_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_user_info
 */
 void free_all_user_info( ALL_USER_INFO *p ) ;

 /**
 * @brief Free an ::ALL_SERVICE_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_SERVICE_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_user_info
 */
 void free_all_svc_info( ALL_SERVICE_INFO *p ) ;

 /**
 * @brief Free an ::ALL_FIRMWARE_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_FIRMWARE_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_firmware_info
 */
 void free_all_firmware_info( ALL_FIRMWARE_INFO *p ) ;

 /**
 * @brief Free an ::ALL_DATABASE_INFO structure
 *
 * @param[in]  p  Pointer to the ::ALL_DATABASE_INFO structure to be freed
 *
 * @see ::mbgextio_get_all_database_info
 */
 void free_all_database_info( ALL_DATABASE_INFO *p ) ;

 /**
 * @brief Set up a ::NTP_TSTAMP structure from a hex string with a time in seconds and binary fractions
 *
 * @param[in]   s  A string with a time in seconds since NTP epoch 1900-01-01,
 *                 with binary fractions separated by decimal point,
 *                 e.g. 'dc763e43.73bd5a8f' as printed by the ntpq utility
 * @param[out]  p  Address of a ::NTP_TSTAMP structure to be set up
 *
 * @see ::str_ntp_hex_to_nano_time_64
 */
 void str_ntp_hex_to_ntp_tstamp( const char *s, NTP_TSTAMP *p ) ;

 /**
 * @brief Convert a ::NTP_TSTAMP structure to a ::NANO_TIME_64 structure
 *
 * @param[in]   p_nts   The ::NTP_TSTAMP structure to be converted
 * @param[out]  p_nt64  The ::NANO_TIME_64 structure to be filled up
 */
 void ntp_tstamp_to_nanotime_64( const NTP_TSTAMP *p_nts, NANO_TIME_64 *p_nt64 ) ;

 /**
 * @brief Set up a ::NANO_TIME_64 structure from a hex string with a time in seconds and binary fractions
 *
 * @param[in]   s  A string with a time in seconds since epoch 1900-01-01,
 *                 with binary fractions separated by decimal point,
 *                 e.g. 'dc763e43.73bd5a8f' as printed by the ntpq utility
 * @param[out]  p  Address of a ::NANO_TIME_64 structure to be set up
 *
 * @see ::str_ntp_hex_to_ntp_tstamp
 */
 void str_ntp_hex_to_nano_time_64( const char *s, NANO_TIME_64 *p ) ;

 /**
 * @brief Convert PTP binary config to Oregano config file
 *
 * @param[in] ptp_info          Complete PTP config binary structure
 * @param[in] instc             PTP instance to write config for
 * @param[in] inst_num          PTP instance number on its timestamper
 * @param[in] intf_lnk          Associated interface link settings to determine VLAN ID
 * @param[in] ip_addr           Associated interface link settings to determine VLAN ID
 * @param[in] ascii_file_name   Target file name for the oregano cfg file
 *
 */
 _NO_MBG_API_ATTR int _MBG_API mbg_cnv_ptp_cfg_to_oreg_cfg_file( const ALL_PTP_NG_INFO *ptp_info, const MBG_PTP_NG_INSTC *instc, unsigned inst_num, const MBG_NET_INTF_LINK_SETTINGS *intf_lnk, const MBG_IP_ADDR *ip_addr, const char *ascii_file_name ) ;

 /**
 * @brief cnvrt_bytes_to_long
 */
 uint64_t cnvrt_bytes_to_long( const uint8_t byte_array[], int num_bytes ) ;

 /**
 * @brief cnvrt_long_to_bytes
 */
 void cnvrt_long_to_bytes( uint8_t byte_array[], int num_bytes, uint64_t val ) ;

 size_t nw_str_mac_addr_to_str( MBG_MAC_ADDR *mac_addr, char *buf, size_t buflen ) ;
 int nw_str_to_mac_addr( MBG_MAC_ADDR *mac_addr, const char *str ) ;
 int nw_ip6_addr_bytes_to_str( const uint8_t *addr, uint8_t protocol, char *str, size_t buflen ) ;
 int nw_str_to_ip6_addr_bytes( uint8_t *addr, const uint8_t protocol, char *str, size_t buflen ) ;
 int mbg_os_version_to_str( const MBG_EXT_SYS_INFO *info, char *buf, size_t buflen, int long_version ) ;
 int mbg_fw_can_set_osv(const MBG_FW_GLB_INFO *info) ;

/* ----- function prototypes end ----- */

#endif  // !defined( MBG_TGT_KERNEL )

#ifdef __cplusplus
}
#endif


/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _CFG_HLP_H */
