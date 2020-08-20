
/**************************************************************************
 *
 *  $Id: mbgclock_main.c 1.31 2020/02/24 17:03:37 martin REL_M $
 *
 *  Description:
 *    Main file for the mbgclock driver for Linux which allows access to
 *    Meinberg radio clock devices from user space.
 *
 *    The binary is a loadable module called mbgclock which implements
 *    /dev/mbgclock* devices.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgclock_main.c $
 *  Revision 1.31  2020/02/24 17:03:37  martin
 *  Removed usage of obsolete symbols.
 *  Revision 1.30  2018/11/22 13:45:24  martin
 *  Refactored debug logging stuff.
 *  Moved default DEBUG_* definitions elsewhere.
 *  'PnP' is common, so don't explicitly mention it in startup msg.
 *  Revision 1.29  2018/09/21 14:56:50  martin
 *  Account for changed library symbols.
 *  Updated some debug stuff.
 *  Revision 1.28  2018/08/07 13:15:55  martin
 *  Cleaned up version information handling.
 *  Revision 1.27  2018/07/04 13:21:38  martin
 *  Removed some obsolete conditional code which
 *  caused duplicate names with kernel 4.17.
 *  Support TCR180USB.
 *  Support new parameter 'force_mm16_access'.
 *  Conditionally support DEBUG_MSG_SLEEP.
 *  Moved some debug message macros to mbgddmsg.h.
 *  Moved get_ioctl_name() as mbgioctl_get_name() to mbgioctl.c.
 *  Unified kernel driver messages.
 *  Use macros to check function return codes.
 *  Cleanup and improvements for cyclic USB.
 *  Use sys_rc as name for variables that hold system return codes.
 *  Use predefined string formats in log message.
 *   Account for some renamed symbols.
 *  Moved a success message to the library's probe routine.
 *  Revision 1.26  2017/07/05 19:18:04  martin
 *  Renamed this file from mbgdrvr.c to mbgclock_main.c.
 *  Account for renamed inline functions.
 *  Added definitions to support DCF600USB.
 *  Fixes to handle locked/unlocked ioctl calls depending on kernel settings.
 *  Account for modified _pcps_kfree() macro.
 *  Support privilege levels for IOCTL calls.
 *  Added device IDs for TCR600USB, MSF600USB, and WVB600USB.
 *  Enhanced IRQ timing debug code.
 *  Read jiffies at IRQ protected by a lock.
 *  Check all PCI base address registers and don't stop if one register is 0.
 *  Print IOCTL code names in debug messages.
 *  Print return code if request_irq() fails.
 *  Moved kernel version compatibility checks to mbg_lx.h.
 *  Use common code from lx-shared.h.
 *  Support new module parameter force_io_access.
 *  Obsoleted IRQF_DISABLED and SA_INTERRUPT which
 *  have been removed from kernel source 4.1.x.
 *  Let the IOCTL handler return MBG_ERROR_CODES.
 *  Use safer function scnprintf() to generate time string.
 *  New way to maintain version information.
 *  Removed obsolete definition of K26, and cleaned up.
 *  Revision 1.25  2010/05/03 15:00:42  martin
 *  Fixed exported functions mbgclock_default_get_ucap_entries()
 *  and mbgclock_default_get_ucap_event().
 *  Added some conditional debug code for exported kernel functions.
 *  Revision 1.24  2010/04/26 14:44:04  martin
 *  Generate a compiler warning if no cycles support to compute latencies.
 *  Revision 1.23  2010/03/05 14:41:31  martin
 *  If IRQ needs to be re-enabled the disable/unregister it first in order not to mess up
 *  the kernel. Moved mbgdrvr_disable_cyclic() up, in front of mbgdrvr_enable_cyclic().
 *  Revision 1.22  2010/02/23 16:02:25  martin
 *  Changed the name of some exported functions from mbgclock_get_default_...()
 *  to mbgclock_default_get_...() for a consistent nameing convention.
 *  Added and exported some new functions which provide access to the ucap FIFO.
 *  Revision 1.21  2009/12/21 15:52:46  martin
 *  Added functions which can be called from other kernel modules
 *  to read memory mapped timestamps.
 *  Moved some driver-internal definitions from the .h file here.
 *  Changed version code to 3.4.1.
 *  Revision 1.20  2009/09/29 14:55:46  martin
 *  Use the ioctl_switch() inline code instead of the macro.
 *  Revision 1.19  2009/09/21 13:53:20  martin
 *  Fixed CONFIG_COMPAT stuff for kernels 2.6.9.
 *  Revision 1.18  2009/08/12 15:25:29  martin
 *  Fixed a bug due to the device_create() parameter count in kernel 2.6.26.
 *  Revision 1.17  2009/07/22 12:36:30  martin
 *  Check rc of down_interruptible() with sem_usb_cyclic in mbgclock_read().
 *  Revision 1.16  2009/06/30 13:28:49  martin
 *  Set the UTC indicator in the time string passed to ntpd based on the
 *  UTC offset hours being 0, not based on the UTC status flag.
 *  Fixed printing of version number in kernel messages.
 *  Revision 1.15  2009/03/20 11:58:13  martin
 *  Enhanced error handling of USB devices.
 *  Revision 1.14  2008/12/22 13:26:50  martin
 *  Migrated former file mbginit.c to mbgdrvr.c and merged code
 *  from mbgclk.c and mbgntp.c which are now obsolete.
 *  Use only one common device node per HW device.
 *  Support USB devices and gracefully handle disconnect/reconnect
 *  while the device is opened.
 *  Return -EBUSY if ioctl() is called and unsafe PEX IRQ is enabled.
 *  Restrict usage to 2.6.x kernels for now.
 *  Create own device class with dynamical unique major number
 *  under kernel 2.6 instead of misc devices.
 *  Use 2.6 kernel API calls which trigger udev support.
 *  Enable device IRQ only when poll() or read() is called the first time.
 *  IRQs are disabled when open count decreases to 0 in release().
 *  A cyclic USB read thread emulates IRQs for USB devices.
 *  Init mutexes/spinlocks in pcps_start_device().
 *  Consistenly use pcps_drvr_name instead of mbgclock_name for messages.
 *  mbgclock_name should be used for device nodes/names etc.
 *  Conditionally use kthread API for USB cyclic polling threads.
 *  Protection against unsafe access is now handled in _ioctl_switch(),
 *  depending on whether HW access is required, or not.
 *  Initial debug level set from DEBUG symbol value.
 *  Account for signed irq_num.
 *  Changed debug messages to common fmt printing file pointers first.
 *  Check IRQ timeout also at blocking reads rather than only while polling.
 *  Use MBG_MEM_ADDR type for memory rather than split high/low types.
 *  Added conditional code for hardware timing debugging via LPT port.
 *  Use malloc'ed kernel memory for private PNP device data.
 *  Revision 1.13  2007/07/24 09:40:58  martin
 *  Added support for PEX511, TCR511PEX, and GPS170PEX.
 *  Account for new unified resource handling.
 *  Revision 1.12  2007/03/02 10:39:01  martin
 *  There are kernel sources around v2.6.9 which provide different
 *  versions of the "module_param_array" macro which is specified
 *  in the kernel header moduleparams.h.
 *  Newer versions of that file also define a macro "__MODULE_PARM_TYPE",
 *  so guess which version of "module_param_array" is to be used based
 *  on the presence of the "__MODULE_PARAM_TYPE" macro.
 *  Added type casts to avoid compiler warnings.
 *  Revision 1.11  2006/08/28 11:15:22  martin
 *  Added an optional module parameter pretend_sync which makes the driver
 *  pretend to NTP the device is always synchronized.
 *  Revision 1.10  2006/07/03 13:17:16  martin
 *  Added support for GPS170PCI, PCI511, and TCR511PCI.
 *  Updated copyright string and turned it into a macro.
 *  Export symbols which may be required for inter-module calls.
 *  Use module_param from 2.6.x kernels, if applicable.
 *  Revision 1.9  2005/06/02 15:48:34  martin
 *  Call pci_enable_device() which also accounts for IRQ routing in
 *  recent 2.6 kernels.
 *  Don't report the the return value of pci_register_driver() as number
 *  of devices found since this has been changed in the kernel code.
 *  Revision 1.8  2004/11/09 09:32:03  martin
 *  Modifications to support kernel 2.6.
 *  Use static table of supported devices to support "modinfo".
 *  Added support for TCR167PCI.
 *  New module parameter dev allows to select device.
 *  Modified load messages.
 *  Revision 1.7  2003/07/30 07:32:41  martin
 *  Print debug level before radio clock listing.
 *  Revision 1.6  2003/07/08 15:24:58  martin
 *  Use plug'n'play manager functions for PCI cards under
 *  kernel 2.4.0 or newer.
 *  Removed obsolete __initfunc() stuff.
 *  Revision 1.5  2003/04/25 10:22:10  martin
 *  Updated copyright string.
 *  Revision 1.4  2002/11/21 10:18:02  martin
 *  Modified startup syslog messages.
 *  Revision 1.3  2002/05/07 07:56:14  martin
 *  Added module license to prevent newer kernels from
 *  printing "module will taint the kernel" warning.
 *  Revision 1.2  2001/07/27 13:25:12  MARTIN
 *  Control usage of __initfunc().
 *  Revision 1.1  2001/03/05 17:19:28  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#define EXPORT_SYMTAB

#define _MBGCLOCK
  #include <mbgclock.h>
#undef _MBGCLOCK

#include <mbgversion.h>
#include <mbgioctl.h>
#include <pcpsirq.h>
#include <mbgddmsg.h>

#include <stddef.h>

#include <linux/termios.h>
#include <linux/pci.h>

#if _USE_LINUX_DEVFS
  #include <linux/devfs_fs_kernel.h>
#endif

#define MBG_COPYRIGHT    "(c) Meinberg 2001-" MBG_CURRENT_COPYRIGHT_YEAR_STR

#define MBG_DRVR_NAME    "mbgclock"

#if !defined( MBGCLOCK_MAX_DEVS )
  #define MBGCLOCK_MAX_DEVS  20
#endif


#if !defined( DEBUG_IRQ_LATENCY )
  #if ( defined( DEBUG ) )
    #define DEBUG_IRQ_LATENCY  0
  #else
    #define DEBUG_IRQ_LATENCY  0
  #endif
#endif

#if !defined( DEBUG_IRQ_TIMING )
  #if ( defined( DEBUG ) )
    #define DEBUG_IRQ_TIMING  0
  #else
    #define DEBUG_IRQ_TIMING  0
  #endif
#endif

#if !defined( DEBUG_SYS_IRQS )
  #if ( defined( DEBUG ) )
    #define DEBUG_SYS_IRQS  0
  #else
    #define DEBUG_SYS_IRQS  0
  #endif
#endif


#if !defined( OMIT_PRIV_CHECKING )
  #define OMIT_PRIV_CHECKING  0
#endif



PCPS_DRVR_INFO drvr_info = { MBG_MAIN_VERSION_CODE,
                             0, MBG_DRVR_NAME " v" MBG_FULL_VERSION_STR };

int pretend_sync;
// NOTE: int debug is defined in a library module

// The variable below is set to point to the first device supporting
// fast HR timestamps (via memotry mapped access).
static PCPS_DDEV *default_fast_hr_time_pddev;
static PCPS_DDEV *default_ucap_pddev;

#if USE_DEBUG_PORT
  static MBG_DBG_DATA mbg_dbg_data;
  static MBG_DBG_PORT mbg_dbg_port = 0x378 + 0;  //##++
#endif  // USE_DEBUG_PORT

#include <macioctl.h>


// the following variables may be overwritten at load time
static int io[PCPS_MAX_ISA_CARDS];
static int irq[PCPS_MAX_ISA_CARDS];


static int major = 0;
static int minor = 0;
static int max_devs = MBGCLOCK_MAX_DEVS;
static int ddev_list_alloc_size;


#ifdef MODULE

MODULE_AUTHOR( "Martin Burnicki <martin.burnicki@meinberg.de>" );
MODULE_DESCRIPTION( "Driver for Meinberg plug-in and USB radio clocks." );
#ifdef MODULE_VERSION
  MODULE_VERSION( MBG_FULL_VERSION_STR );
#endif
#ifdef MODULE_LICENSE
  MODULE_LICENSE( "GPL" );
#endif
MODULE_SUPPORTED_DEVICE( "mbgclock" );

#if defined( module_param_array )
  static int n_io;
  #if defined( __MODULE_PARM_TYPE )
    module_param_array( io, int, &n_io, 0444 );
  #else
    module_param_array( io, int, n_io, 0444 );
  #endif
#elif defined( MODULE_PARM )
  MODULE_PARM( io, "1-" __MODULE_STRING( PCPS_MAX_ISA_CARDS ) "i" );
#endif
MODULE_PARM_DESC( io, "port address(es) of ISA card(s)" );

#if defined( module_param_array )
  static int n_irq;
  #if defined( __MODULE_PARM_TYPE )
    module_param_array( irq, int, &n_irq, 0444 );
  #else
    module_param_array( irq, int, n_irq, 0444 );
  #endif
#elif defined( MODULE_PARM )
  MODULE_PARM( irq, "1-" __MODULE_STRING( PCPS_MAX_ISA_CARDS ) "i" );
#endif
MODULE_PARM_DESC( irq, "IRQ line(s) used by ISA card(s)" );

#if defined( module_param )
  module_param( major, int, S_IRUGO );
  module_param( minor, int, S_IRUGO );
  module_param( max_devs, int, S_IRUGO );
#elif defined( MODULE_PARM )
  MODULE_PARM( major, "i" );
  MODULE_PARM( minor, "i" );
  MODULE_PARM( max_devs, "i" );
#endif
MODULE_PARM_DESC( major, "major device number, dynamic by default" );
MODULE_PARM_DESC( minor, "first minor device number, dynamic by default" );
MODULE_PARM_DESC( max_devs, "max number of supported devices" );


#if defined( module_param )
  module_param( pretend_sync, int, 0444 );
#elif defined( MODULE_PARM )
  MODULE_PARM( pretend_sync, "i" );
#endif
MODULE_PARM_DESC( pretend_sync, "pretend to NTP to be always sync'ed" );

#if _PCPS_USE_MM_IO
  #if defined( module_param )
    module_param( force_io_access, int, 0444 );
    module_param( force_mm16_access, int, 0444 );
  #elif defined( MODULE_PARM )
    MODULE_PARM( force_io_access, "i" );
    MODULE_PARM( force_mm16_access, "i" );
  #endif
  MODULE_PARM_DESC( force_io_access, "force I/O port access even if a device supports memory mapped access." );
  MODULE_PARM_DESC( force_mm16_access, "force 16 bit memory mapped access for devices which support this." );
#endif

#if DEBUG_MSG_SLEEP
  #if defined( module_param )
    module_param( debug_msg_sleep, int, 0444 );
  #elif defined( MODULE_PARM )
    MODULE_PARM( debug_msg_sleep, "i" );
  #endif
  MODULE_PARM_DESC( debug_msg_sleep, "debug only, msleep by this amount of ms after each message." );
#endif

#ifdef DEBUG
  #if defined( module_param )
    module_param( debug, int, 0444 );
  #elif defined( MODULE_PARM )
    MODULE_PARM( debug, "i" );
  #endif
  MODULE_PARM_DESC( debug, "debug level, only if compiled with DEBUG" );
#endif

#ifdef EXPORT_SYMBOL
#if 0 //##++
EXPORT_SYMBOL( pcps_ddev );
EXPORT_SYMBOL( n_ddevs );
EXPORT_SYMBOL( access_in_progress );
#endif
#endif

#ifndef module_init
  #define mbgclock_init_module     init_module
  #define mbgclock_cleanup_module  cleanup_module
#endif

#endif

#define DRV_NAME    MBG_DRVR_NAME

#define MBG_SIZE_TLG    33

#define _min( _a, _b )  ( ( (_b) < (_a) ) ? (_b) : (_a) )

#define CYCLIC_TIMEOUT ( (ulong) 2 * HZ )  // 2 seconds

#if NEW_FASYNC2
  #define _kill_fasync( _fa, _sig, _band ) \
    kill_fasync( _fa, _sig, _band )
#elif NEW_FASYNC
  #define _kill_fasync( _fa, _sig, _band ) \
    kill_fasync( *(_fa), _sig, _band )
#else
  #define _kill_fasync( _fa, _sig, _band ) \
    kill_fasync( *(_fa), _sig )
#endif


#ifndef __exit
  #define __exit
#endif

static PCPS_DDEV **ddev_list;
static struct semaphore sem_fops;  // still needs to be initialized !!

// The definitions in this file are shared with other Linux drivers:
#include <lx-shared.h>


#if !defined( DEBUG_HW_LPT )
  #define DEBUG_HW_LPT  0
#endif

#if DEBUG_HW_LPT
  static spinlock_t hwdbg_lock = SPIN_LOCK_UNLOCKED;

  #define MBG_BIT_OPEN     0x03
  #define MBG_BIT_RELEASE  0x01
  #define MBG_BIT_IRQ      0x04
  #define MBG_BIT_TEST     0x08


  #define _mbg_dbg_hw_lpt_vars \
    unsigned long hwdbg_flags;

  #define _mbg_dbg_hw_lpt_set_bit( _b )                 \
  {                                                     \
    spin_lock_irqsave( &hwdbg_lock, hwdbg_flags );      \
    _mbg_dbg_set_bit( pddev, _b );                      \
    spin_unlock_irqrestore( &hwdbg_lock, hwdbg_flags ); \
  }

  #define _mbg_dbg_hw_lpt_clr_bit( _b )                 \
  {                                                     \
    spin_lock_irqsave( &hwdbg_lock, hwdbg_flags );      \
    _mbg_dbg_clr_bit( pddev, _b );                      \
    spin_unlock_irqrestore( &hwdbg_lock, hwdbg_flags ); \
  }

#else

  #define _mbg_dbg_hw_lpt_vars
  #define _mbg_dbg_hw_lpt_set_bit( _b );
  #define _mbg_dbg_hw_lpt_clr_bit( _b );

#endif



#if _PCPS_HAVE_LINUX_CLASS
  static char mbgclock_class_name[] = "mbgclock";
  static char mbg_clk_dev_node_fmt[] = "mbgclock%d";

  #if _PCPS_HAVE_LINUX_CLASS_CREATE
    static struct class *mbgclock_class;
  #elif _PCPS_HAVE_LINUX_CLASS_SIMPLE
    static struct class_simple *mbgclock_class;
  #endif
#endif


#if DEBUG_IRQ_LATENCY
  static unsigned long long tsc_irq_1;
  static unsigned long long tsc_irq_2;
  static unsigned long long tsc_usb_1;
  static unsigned long long tsc_usb_2;
#endif



#if DEBUG_SYS_IRQS

static /*HDR*/
void list_system_irqs( void )
{
  int i;
  struct irq_desc *desc;

  // This is some sample code from a forum which should
  // list some information on all IRQs supported by the kernel.
  // However, the members of struct irq_desc may vary with
  // the kernel version, so this code may need to be adapted
  // depending on the kernel version.
  for_each_irq_desc( i, desc )
  {
    if ( !desc )
      continue;

    mbg_kdd_msg( MBG_LOG_INFO, "%d: status=%08x, chip=%08x, handle_irq=%08x",
                 i, (u32) desc->status, (u32) desc->chip, (u32) desc->handle_irq );
  }

}  // list_system_irqs

#endif


#if _PCPS_USE_LINUX_KTHREAD

  #define _usb_read_thread_should_stop() \
     kthread_should_stop()

  #define _usb_read_thread_exit( _pddev, _v ) \
    return (_v);

  #define _usb_read_thread_started( _pddev ) \
    ( (_pddev)->usb_read_thread != NULL )

  #define _usb_read_thread_start( _fnc, _pddev )               \
    (_pddev)->usb_read_thread = kthread_run( (_fnc), (_pddev), \
       "%s%d", driver_name, MINOR( (_pddev)->lx_dev ) )

  #define _usb_read_thread_failed_to_start( _pddev ) \
    ( IS_ERR( (_pddev)->usb_read_thread ) )

  #define _usb_read_thread_set_unused( _pddev ) \
    (_pddev)->usb_read_thread = NULL;

  #define _usb_read_thread_stop( _pddev ) \
    kthread_stop( (_pddev)->usb_read_thread )

  // This is not required if the kthread API is being used.
  #define _usb_read_thread_daemonize( _pddev ) \
    _nop_macro_fnc()

#else

  #define _usb_read_thread_should_stop() \
    signal_pending( current )

  #define _usb_read_thread_exit( _pddev, _v ) \
    complete_and_exit( &pddev->usb_read_thread.exit, (_v) )

  #define _usb_read_thread_started( _pddev ) \
    ( (_pddev)->usb_read_thread.pid != 0 )

  #define _usb_read_thread_start( _fnc, _pddev )         \
    init_completion( &(_pddev)->usb_read_thread.exit );  \
    (_pddev)->usb_read_thread.pid = kernel_thread( (_fnc), (_pddev), CLONE_KERNEL );

  #define _usb_read_thread_failed_to_start( _pddev ) \
    ( (_pddev)->usb_read_thread.pid == 0 )

  #define _usb_read_thread_set_unused( _pddev ) \
    (_pddev)->usb_read_thread.pid = 0;

  #define _usb_read_thread_stop( _pddev )                   \
    kill_proc( (_pddev)->usb_read_thread.pid, SIGKILL, 1 ); \
    wait_for_completion( &(_pddev)->usb_read_thread.exit )

  #define _usb_read_thread_daemonize( _pddev )                                          \
    snprintf( (_pddev)->usb_read_thread.name, sizeof( (_pddev)->usb_read_thread.name ), \
              "%s%d", driver_name, MINOR( (_pddev)->lx_dev ) );                       \
    daemonize( (_pddev)->usb_read_thread.name );                                        \
    allow_signal( SIGKILL )

#endif


static void mbgdrvr_delete_device( PCPS_DDEV *pddev );


static /*HDR*/
void ddev_list_free( void )
{
  if ( ddev_list )
  {
    _pcps_kfree( ddev_list, ddev_list_alloc_size );
    ddev_list = NULL;

    _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "Freed device list" );
  }

}  // ddev_list_free



static /*HDR*/
int ddev_list_alloc( void )
{
  ddev_list_alloc_size = max_devs * sizeof( *ddev_list );

  ddev_list = _pcps_kmalloc( ddev_list_alloc_size );

  if ( ddev_list == NULL )
  {
    mbg_kdd_msg( MBG_LOG_ERR, "Failed to allocate memory for device list" );
    return -ENOMEM;
  }

  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Allocated device list, %u bytes for up to %u entries",
               ddev_list_alloc_size, max_devs );

  memset( ddev_list, 0, ddev_list_alloc_size );

  return 0;

}  // ddev_list_alloc



static /*HDR*/
PCPS_DDEV **ddev_list_locate_minor( unsigned int minor )
{
  int i;

  for ( i = 0; i < max_devs; i++ )
  {
    PCPS_DDEV **ppddev = &ddev_list[i];

    if ( *ppddev )
      if ( minor == MINOR( (*ppddev)->lx_dev ) )
        return ppddev;
  }

  mbg_kdd_msg( MBG_LOG_WARN, "Unable to locate minor %i in device list",
               minor );

  return NULL;

}  // ddev_list_locate_minor



static /*HDR*/
PCPS_DDEV **ddev_list_locate_device( PCPS_BUS_FLAGS bus_flags, PCPS_DEV_ID dev_id, PCPS_SN_STR sernum )
{
  int i;

  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Searching device list for ID %04X, S/N %s",
               dev_id, sernum  );

  for ( i = 0; i < max_devs; i++ )
  {
    PCPS_DDEV **ppddev = &ddev_list[i];

    if ( *ppddev )
    {
      _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_INFO, "  Entry %i: ID %04X, S/N %s", i,
                   _pcps_ddev_dev_id( *ppddev ), _pcps_ddev_sernum( *ppddev ) );

      if ( ( dev_id == _pcps_ddev_dev_id( *ppddev ) )
        && ( strcmp( sernum, _pcps_ddev_sernum( *ppddev ) ) == 0 ) )
      {
        // device found
        _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "New device %04X S/N %s already in device list",
                     dev_id, sernum  );

        return ppddev;
      }
    }
  }

  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Device ID %04X, S/N %s not found in device list",
               dev_id, sernum  );

  return NULL;

}  // ddev_list_locate_device



static /*HDR*/
int ddev_list_remove_entry( PCPS_DDEV *pddev )
{
  int i;

  for ( i = 0; i < max_devs; i++ )
  {
    PCPS_DDEV **ppddev = &ddev_list[i];

    if ( *ppddev == pddev )
    {
      *ppddev = NULL;
      return 0;
    }
  }

  mbg_kdd_msg( MBG_LOG_WARN, "Failed to remove device minor %i from device list",
               MINOR( pddev->lx_dev ) );

  return -1;  //TODO

}  // ddev_list_remove_entry



static /*HDR*/
int ddev_list_add_entry( PCPS_DDEV *pddev )
{
  int i;

  for ( i = 0; i < max_devs; i++ )
  {
    PCPS_DDEV **ppddev = &ddev_list[i];

    if ( *ppddev == NULL )
    {
      *ppddev = pddev;
      return i;
    }
  }

  mbg_kdd_msg( MBG_LOG_WARN, "Failed to add device to device list: list full" );

  return -1;  //TODO

}  // ddev_list_add_entry



static /*HDR*/
void pcps_time_to_time_str( const PCPS_TIME *t, char *s, size_t max_len )
{
  PCPS_TIME_STATUS status = t->status;

  if ( status & PCPS_INVT )    // invalid time
  {
    status |= PCPS_FREER;
    status &= ~PCPS_SYNCD;
  }

  scnprintf( s, max_len, "%cD:%02i.%02i.%02i;T:%i;U:%02i.%02i.%02i;%c%c%c%c%c",
             '\02',                                                        // STX
             t->mday, t->month, t->year, t->wday,                          // date and day-of-week
             t->hour, t->min, t->sec,                                      // time
             ( pretend_sync || ( status & PCPS_SYNCD ) ) ? ' ' : '#',      // sync'd after reset
             ( !pretend_sync && ( status & PCPS_FREER ) ) ? '*' : ' ',     // free running
             ( t->offs_utc == 0 ) ? 'U' :                                  // UTC
               ( ( status & PCPS_DL_ENB ) ? 'S' : ' ' ),                   // DST enabled
             ( status & PCPS_DL_ANN ) ? '!' :                              // DST change announced
               ( ( status & PCPS_LS_ANN ) ? 'A' : ' ' ),                   // leap second announced
             '\03' );                                                      // ETX

}  // pcps_time_to_time_str



// The interrupt handler for plug-in cards. USB devices
// don't generate periodic interrupts. Instead, they can
// send a periodic message once per second.

static /*HDR*/
#if REQUEST_IRQ_WO_REGS
  irqreturn_t mbgclock_irq_handler( int hw_irq, void *arg )
#else
  irqreturn_t mbgclock_irq_handler( int hw_irq, void *arg, struct pt_regs *regs )
#endif
{
  PCPS_DDEV *pddev;
  unsigned long flags;
  int curr_access_in_progress;
  #if defined( IRQ_RETVAL )
    int retval = IRQ_NONE;
  #endif
  #if DEBUG_IRQ_TIMING
    unsigned long prv_jiffies_at_irq;
  #endif
  int rc;
  _mbg_dbg_hw_lpt_vars


  pddev = (PCPS_DDEV *) arg;

  if ( pddev == NULL )
  {
    mbg_kdd_msg( MBG_LOG_WARN, "IRQ handler called with NULL device" );
    goto out;
  }

  if ( !_pcps_ddev_has_gen_irq( pddev ) )
    goto out;


  _mbg_dbg_hw_lpt_set_bit( MBG_BIT_IRQ );

  rc = -1;
  spin_lock_irqsave( &pddev->irq_lock, flags );

  curr_access_in_progress = atomic_read( &pddev->access_in_progress );

  #if DEBUG_IRQ_TIMING
    prv_jiffies_at_irq = pddev->jiffies_at_irq;
  #endif
  pddev->jiffies_at_irq = jiffies;

  #if DEBUG_IRQ_LATENCY
    rdtscll( tsc_irq_1 );
  #endif

  if ( !curr_access_in_progress )
    rc = _pcps_read_var( pddev, PCPS_GIVE_TIME, pddev->t );

  #if DEBUG_IRQ_LATENCY
    rdtscll( tsc_irq_2 );
  #endif

  _pcps_ddev_ack_irq( pddev );


  if ( !curr_access_in_progress )
  {
    if ( mbg_rc_is_success( rc ) )
    {
      atomic_set( &pddev->data_avail, 1 );

      wake_up_interruptible( &pddev->wait_queue );

      if ( pddev->fasyncptr )
        _kill_fasync( &pddev->fasyncptr, SIGIO, POLL_IN );
    }
  }

  spin_unlock_irqrestore( &pddev->irq_lock, flags );


  #if DEBUG_IRQ_TIMING
  {
    const char *info;

    if ( !curr_access_in_progress )
    {
      if ( mbg_rc_is_success( rc ) )
        info = "data read";
      else
        info = "read error";
    }
    else
    {
      info = "** access in progress";
      rc = curr_access_in_progress;
    }

    mbg_kdd_msg( MBG_LOG_INFO, "IRQ handler %i at 0x%lX - 0x%lX -> %li, " MBG_DEV_NAME_FMT ": %s: %i",
                 _pcps_ddev_irq_num( pddev), pddev->jiffies_at_irq, prv_jiffies_at_irq,
                 (long) pddev->jiffies_at_irq - (long) prv_jiffies_at_irq,
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                 info, rc );
  }
  #endif

  #if defined( IRQ_RETVAL )
    retval = IRQ_HANDLED;
  #endif

out:
  _mbg_dbg_hw_lpt_clr_bit( MBG_BIT_IRQ );

  #if defined( IRQ_RETVAL )
    return retval;
  #endif

}  // mbgclock_irq_handler



__mbg_inline
int get_cyclic_lock( PCPS_DDEV *pddev, unsigned long *p_flags, const char *fnc_name )
{
  #if _PCPS_USE_USB
  if ( _pcps_ddev_is_usb( pddev ) )
  {
    if ( _down_interruptible_pddev( &pddev->sem_usb_cyclic, "sem_usb_cyclic", fnc_name, pddev ) < 0 )
      return -ERESTARTSYS;
  }
  else
  #endif
    spin_lock_irqsave( &pddev->irq_lock, *p_flags );

  return 0;

}  // get_cyclic_lock



__mbg_inline
void release_cyclic_lock( PCPS_DDEV *pddev, unsigned long *p_flags, const char *fnc_name )
{
  #if _PCPS_USE_USB
  if ( _pcps_ddev_is_usb( pddev ) )
    _up_pddev( &pddev->sem_usb_cyclic, "sem_usb_cyclic", fnc_name, pddev );
  else
  #endif
    spin_unlock_irqrestore( &pddev->irq_lock, *p_flags );

}  // release_cyclic_lock



#if _PCPS_USE_USB

// The function below is started as a new kernel thread in order to
// receive periodic messages from an USB device.
// This is used to emulate the hardware IRQ generated by
// plug-in cards.

static /*HDR*/
int mbgdrvr_read_usb_cyclic( void *p )
{
  PCPS_DDEV *pddev = p;

  _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_INFO, "USB xmt tmo: %u, rcv tmo: %u, cycl tmo: %u, HZ: %u",
               MBGUSB_TIMEOUT_SEND, MBGUSB_TIMEOUT_RECEIVE,
               MBGUSB_TIMEOUT_RECEIVE_CYCLIC, HZ );

  _usb_read_thread_daemonize( pddev );

  // The device sends a cyclic message in intervals of 1 s,
  // so we have to wait more than 1 s totally to detect a
  // timeout.
  // Unfortunatly the USB read function does not return if
  // we receive a signal (e.g. to terminate), so in order
  // to be able to react on a signal in a timely manner
  // we call the read function with a fraction of the total
  // timeout and simply retry unless we have received a signal,
  // or the total timeout interval has expired.
  for (;;)
  {
    int rc;

    if ( _usb_read_thread_should_stop() )  // we have been signalled to abort
    {
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Cyclic USB read " MBG_DEV_NAME_FMT " received signal to stop",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      break;
    }

    // read with fractional timeout interval
    rc = _pcps_direct_usb_read_var_cyclic( pddev, &pddev->t_cyc );

    if ( mbg_rc_is_error( rc ) )
    {
      if ( rc == MBG_ERR_TIMEOUT )
      {
        #if ( 1 && defined( DEBUG ) )  // TODO DEBUG_CYCLIC
          _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_WARN, "Cyclic USB read " MBG_DEV_NAME_FMT " timed out",
                       _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
        #endif
        continue;    // this may be a normal case
      }

      // Read has been terminated due to some other error.
      // Eventually the device has been disconnected, in which case
      // the USB stack may return -ETIME (-62), which is converted
      // to MBG_ERR_TIMER.
      _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_WARN, "Cyclic USB read " MBG_DEV_NAME_FMT " returned %i: %s",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ), rc, mbg_strerror( rc ) );
      break;
    }

    // success, so rc holds the number of bytes read
    if ( rc != sizeof( pddev->t_cyc ) )
    {
      _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_WARN, "Cyclic USB read " MBG_DEV_NAME_FMT " returned %i of %i bytes",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ), rc, (int) sizeof( pddev->t_cyc ) );
      continue;
    }

    // received a message from the device
    #if DEBUG_IRQ_LATENCY
      rdtscll( tsc_usb_1 );
    #endif

    if ( _down_interruptible_pddev( &pddev->sem_usb_cyclic, "sem_usb_cyclic", __func__, pddev ) < 0 )
      break;

    #if DEBUG_IRQ_LATENCY
      rdtscll( tsc_usb_2 );
    #endif

    pddev->jiffies_at_irq = jiffies;
    pddev->t = pddev->t_cyc;
    atomic_set( &pddev->data_avail, 1 );

    _up_pddev( &pddev->sem_usb_cyclic, "sem_usb_cyclic", __func__, pddev );

    _mbgddmsg_6( DEBUG_DRVR, MBG_LOG_INFO, "Cyclic USB read " MBG_DEV_NAME_FMT " success: %02d:%02d:%02d.%02d",
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                 pddev->t.hour, pddev->t.min, pddev->t.sec, pddev->t.sec100 );

    wake_up_interruptible( &pddev->wait_queue );

    if ( pddev->fasyncptr )
      _kill_fasync( &pddev->fasyncptr, SIGIO, POLL_IN );
  }

  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Cyclic USB read thread for " MBG_DEV_NAME_FMT " exits",
               _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

  _usb_read_thread_set_unused( pddev );
  _usb_read_thread_exit( pddev, 0 );

}  // mbgdrvr_read_usb_cyclic



static /*HDR*/
int mbgdrvr_ctrl_usb_cyclic( PCPS_DDEV *pddev, uint8_t cmd )
{
  int rc;

  if ( _down_interruptible( &pddev->dev_mutex, "dev_mutex", __func__, pddev ) < 0 )
    return -ERESTARTSYS;

  pddev->cmd_info.cmd = cmd;
  rc = _pcps_direct_usb_write_var( pddev, &pddev->cmd_info.cmd );

  _up( &pddev->dev_mutex, "dev_mutex", "ctrl_usb_cyclic", pddev );

  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Control USB cyclic cmd 0x%02X, rc: %d",
               cmd, rc );

  return rc;

}  // mbgdrvr_ctrl_usb_cyclic

#endif  // _PCPS_USE_USB



static /*HDR*/
int mbgdrvr_disable_cyclic( PCPS_DDEV *pddev )
{
  _mbg_dbg_hw_lpt_vars

  if ( !( pddev->irq_stat_info & PCPS_IRQ_STAT_ENABLED ) )
    goto done;

  #if _PCPS_USE_USB
  if ( _pcps_ddev_is_usb( pddev ) )
  {
    _mbg_dbg_hw_lpt_set_bit( MBG_BIT_TEST );
    mbgdrvr_ctrl_usb_cyclic( pddev, PCPS_IRQ_NONE );
    _mbg_dbg_hw_lpt_clr_bit( MBG_BIT_TEST );

    _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "Disabled cyclic USB msgs" );

    if ( _usb_read_thread_started( pddev ) )
    {
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Cyclic USB read thread for " MBG_DEV_NAME_FMT " going to be stopped",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

      _usb_read_thread_stop( pddev );

      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Cyclic USB read thread for " MBG_DEV_NAME_FMT " has been stopped",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    }
  }
  else
  #endif
  {
    int irq_num = _pcps_ddev_irq_num( pddev );

    _mbg_dbg_hw_lpt_set_bit( MBG_BIT_TEST );
    _pcps_ddev_disb_irq( pddev );
    _mbg_dbg_hw_lpt_clr_bit( MBG_BIT_TEST );

    free_irq( irq_num, pddev );

    _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "Disabled IRQ %i", irq_num );
  }

done:
  pddev->irq_stat_info &= ~( PCPS_IRQ_STAT_ENABLED | PCPS_IRQ_STAT_ENABLE_CALLED );
  return 0;

}  // mbgdrvr_disable_cyclic



static /*HDR*/
int mbgdrvr_enable_cyclic( PCPS_DDEV *pddev, unsigned int force )
{
  if ( ( force == 0 ) && ( pddev->irq_stat_info & PCPS_IRQ_STAT_ENABLE_CALLED ) )
    return 0;   // has already been called

  if ( force > 1 )
    mbg_kdd_msg( MBG_LOG_WARN, "Trying to re-initialize cyclic msgs for " MBG_DEV_NAME_FMT,
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

  pddev->irq_stat_info |= PCPS_IRQ_STAT_ENABLE_CALLED;

  if ( pddev->irq_stat_info & PCPS_IRQ_STAT_UNSAFE )
    mbg_kdd_msg( MBG_LOG_WARN, "Enabling IRQs for dev " MBG_DEV_NAME_FMT " though unsafe",
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

  pddev->jiffies_at_irq = jiffies;

  #if _PCPS_USE_USB
  if ( _pcps_ddev_is_usb( pddev ) )
  {
    if ( force > 1 )
      usb_reset_device( pddev->udev );

    if ( !_usb_read_thread_started( pddev ) )
    {
      _usb_read_thread_start( mbgdrvr_read_usb_cyclic, pddev );

      if ( _usb_read_thread_failed_to_start( pddev ) )
      {
        mbg_kdd_msg( MBG_LOG_WARN, "Failed to start cyclic USB read thread for " MBG_DEV_NAME_FMT,
                     _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
        _usb_read_thread_set_unused( pddev );
      }
      else
        _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Cyclic USB read thread for " MBG_DEV_NAME_FMT " started successfully",
                     _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    }
    else
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Cyclic USB read thread for " MBG_DEV_NAME_FMT " already started",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

    if ( _usb_read_thread_started( pddev ) )
    {
      pddev->irq_stat_info |= PCPS_IRQ_STAT_ENABLED;
      mbgdrvr_ctrl_usb_cyclic( pddev, PCPS_IRQ_1_SEC );
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Initialized cyclic USB msgs for " MBG_DEV_NAME_FMT,
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    }
  }
  else
  #endif
  {
    int irq_num = _pcps_ddev_irq_num( pddev );
    int rc;
    unsigned long flags = 0;

    // If the IRQ has already been enabled and registered with the kernel before
    // then we must disable and unregister it before we re-enable it and register
    // it once more. Otherwise the kernel's list of registered IRQ handlers as
    // reported by 'cat /proc/interrupts' will be messed up.
    if ( pddev->irq_stat_info & PCPS_IRQ_STAT_ENABLED )
      mbgdrvr_disable_cyclic( pddev );

    // The IRQF_DISABLED flag has been obsoleted in 2.6.35, and has completely
    // been removed in 4.1. Also the SA_INTERRUPT flag, which was used before
    // IRQF_DISABLED was introduced in 2.6.24 to distinguish between "fast"
    // and "slow" ISRs is now obsolete.
    // Usage of either of these flags is questionable anyway, see:
    //   https://lwn.net/Articles/380931/
    // We leave an outcommented code fragment here, so it can easily be
    // activated if it should unexpectedly be required.
    #if 0
      #if defined( IRQF_DISABLED )
        flags |= IRQF_DISABLED;
      #elif defined( SA_INTERRUPT )
        flags |= SA_INTERRUPT;
      #endif
    #endif

    // Shared IRQs are not supported by old ISA cards, so we flag
    // the IRQ as "shared" only in case of non-ISA, namely PCI.
    // The symbol name of the "shared IRQ" flag varies with the
    // kernel version, though.
    if ( !_pcps_ddev_is_isa( pddev ) )
    {
      #if defined( IRQF_SHARED )
        flags |= IRQF_SHARED;
      #elif defined( SA_SHIRQ )
        flags |= SA_SHIRQ;
      #endif
    }

    rc = request_irq( irq_num, mbgclock_irq_handler, flags,
                      driver_name, pddev );

    if ( rc < 0 )
    {
      mbg_kdd_msg( MBG_LOG_ERR, "Failed to request IRQ %i for " MBG_DEV_NAME_FMT ", rc: %i",
                   irq_num, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ), rc );
      return -EBUSY;
    }

    pddev->irq_stat_info |= PCPS_IRQ_STAT_ENABLED;

    _pcps_ddev_enb_irq( pddev, PCPS_IRQ_1_SEC );
    _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_INFO, "Initialized IRQ %i for " MBG_DEV_NAME_FMT " (open_count: %i)",
                 irq_num, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                 atomic_read( &pddev->open_count ) );
  }

  return 0;

}  // mbgdrvr_enable_cyclic



#if DEBUG_IRQ_TIMING

static /*HDR*/
void report_irq_timing( int lvl, const PCPS_DDEV *pddev,
                        unsigned long jiffies_now,
                        unsigned long jiffies_at_irq,
                        long delta_jiffies )
{

}  // report_irq_timing

#endif  // DEBUG_IRQ_TIMING



static /*HDR*/
unsigned int mbgclock_poll( struct file *filp, poll_table *pt )
{
  unsigned int poll_retval = 0;
  PCPS_DDEV *pddev = NULL;
  int sys_rc = mbgdrvr_get_pddev( &pddev, filp, "poll" );

  if ( sys_rc < 0 )
  {
    poll_retval = POLLERR | POLLHUP;
    goto out;
  }

  mbgdrvr_enable_cyclic( pddev, 0 );

  poll_wait( filp, &pddev->wait_queue, pt );

  if ( atomic_read( &pddev->data_avail ) )
    poll_retval = POLLIN | POLLRDNORM;
  else
  {
    unsigned long flags = 0;
    unsigned long jiffies_now = jiffies;
    unsigned long jiffies_at_irq;
    long delta_jiffies;

    if ( get_cyclic_lock( pddev, &flags, __func__ ) < 0 )
      return -ERESTARTSYS;

    jiffies_at_irq = pddev->jiffies_at_irq;

    release_cyclic_lock( pddev, &flags, __func__ );

    delta_jiffies = (long) jiffies_now - (long) jiffies_at_irq;


    if ( delta_jiffies > CYCLIC_TIMEOUT )
    {
      mbg_kdd_msg( MBG_LOG_WARN, "** Cyclic timeout polling " MBG_DEV_NAME_FMT ": 0x%08lX - 0x%08lX = %li, exceeds %li",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                   jiffies_now, jiffies_at_irq,
                   delta_jiffies, CYCLIC_TIMEOUT );

      #if DEBUG_IRQ_TIMING
        report_irq_timing( MBG_LOG_DEBUG, pddev, jiffies_now, jiffies_at_irq, delta_jiffies );
      #endif

      mbgdrvr_enable_cyclic( pddev, 2 );
    }
    else
    {
      #if DEBUG_IRQ_TIMING
        mbg_kdd_msg( MBG_LOG_DEBUG, "Poll chk cyclic timeout " MBG_DEV_NAME_FMT ": 0x%08lX - 0x%08lX = %li, timeout: %li",
                     _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ),
                     jiffies_now, jiffies_at_irq,
                     delta_jiffies, CYCLIC_TIMEOUT );
      #endif
    }
  }

out:
  return poll_retval;

}  // mbgclock_poll



/*
 * The function below is called whenever fcntl( .., F_SETFL, ..) is called
 * to change the device's FASYNC flag.
 *
 */
static /*HDR*/
int mbgclock_fasync( int fd, struct file *filp, int on )
{
  PCPS_DDEV *pddev = NULL;
  int sys_rc = mbgdrvr_get_pddev( &pddev, filp, "fasync" );

  if ( sys_rc < 0 )
    goto out;

  // mbgdrvr_enable_cyclic( pddev, 0 );    //##++ conditionally / always enable IRQ here?

  sys_rc = fasync_helper( fd, filp, on, &pddev->fasyncptr );

out:
  return sys_rc;

}  // mbgclock_fasync



static /*HDR*/
#if FLUSH_WITH_LOCK_OWNER_ID
int mbgclock_flush( struct file *filp, fl_owner_t id )
#else
int mbgclock_flush( struct file *filp )
#endif
{
  PCPS_DDEV *pddev = NULL;
  int sys_rc = mbgdrvr_get_pddev( &pddev, filp, "flush" );

  if ( sys_rc < 0 )
    goto out;

  atomic_set( &pddev->data_avail, 0 );

out:
  return sys_rc;

}  // mbgclock_flush



static /*HDR*/
int mbgclock_open( struct inode *inode, struct file *filp )
{
  PCPS_DDEV **ppddev;
  PCPS_DDEV *pddev;
  int minor = iminor( inode );
  int retval = 0;
  _mbg_dbg_hw_lpt_vars

  _mbg_dbg_hw_lpt_set_bit( MBG_BIT_OPEN );

  if ( filp == NULL )
  {
    mbg_kdd_msg( MBG_LOG_WARN, "Open minor %i called with file pointer NULL", minor );
    retval = -EBADF;
    goto out;
  }

  if ( _down_interruptible( &sem_fops, "sem_fops", __func__, filp ) < 0 )
  {
    _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "%p open minor %i: interrupted wait for sem_fops",
                 filp, minor );
    retval = -ERESTARTSYS;
    goto out;
  }

  ppddev = ddev_list_locate_minor( minor );

  if ( ppddev == NULL )
    goto out_enodev;

  pddev = *ppddev;

  if ( pddev == NULL )
    goto out_enodev;


  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "%p open minor %i",
               filp, minor );

  filp->private_data = ppddev;

  atomic_inc( &pddev->open_count );

  _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_INFO, "%p open: new open count: %i, dev: %p",
               filp, atomic_read( &pddev->open_count ), pddev );

  #if _PCPS_MUST_UPDATE_USE_COUNT
    MOD_INC_USE_COUNT;
  #endif

  goto out_up_sem_fops;


out_enodev:
   retval = -ENODEV;
  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_WARN, "%p open failed for minor %i: ENODEV",
               filp, minor );

out_up_sem_fops:
  _up( &sem_fops, "sem_fops", "open", filp );

out:
  _mbg_dbg_hw_lpt_clr_bit( MBG_BIT_OPEN );

  return retval;

}  // mbgclock_open



static /*HDR*/
int mbgclock_release( struct inode *inode, struct file *filp )
{
  PCPS_DDEV *pddev;
  int retval = 0;
  _mbg_dbg_hw_lpt_vars


  _mbg_dbg_hw_lpt_set_bit( MBG_BIT_RELEASE );

  if ( _down_interruptible( &sem_fops, "sem_fops", __func__, filp ) < 0 )
  {
    _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "%p release %i: interrupted wait for sem_fops",
                 filp, minor );
    retval = -ERESTARTSYS;
    goto out;  // don't return directly
  }

  // sem_fops must not be released before we're through with pddev !

  pddev = *( (PCPS_DDEV **) filp->private_data );

  if ( pddev == NULL )
  {
    _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_WARN, "%p release %i: closing with dev NULL",
                 filp, iminor( inode ) );
    retval = -ENODEV;
    goto out_up_sem_fops;
  }

  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "%p release %i: closing",
                filp, iminor( inode ) );

  if ( atomic_dec_and_test( &pddev->open_count ) )
  {
    if ( get_dev_connected( pddev ) )
    {
      _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_INFO, "%p release %i: closing with connected dev " MBG_DEV_NAME_FMT,
                   filp, iminor( inode ),
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      mbgdrvr_disable_cyclic( pddev );
    }
    else
    {
      _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_INFO, "%p release %i: closing with disconnected dev " MBG_DEV_NAME_FMT,
                   filp, iminor( inode ),
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      mbgdrvr_delete_device( pddev );
      pddev = NULL;
      retval = -ENODEV;
    }
  }
  else
    _mbgddmsg_5( DEBUG_DRVR, MBG_LOG_INFO, "%p release %i: new open count: %i, dev " MBG_DEV_NAME_FMT,
                 filp, iminor( inode ), atomic_read( &pddev->open_count ),
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

  #if _PCPS_MUST_UPDATE_USE_COUNT
    MOD_DEC_USE_COUNT;
  #endif

  if ( pddev )  // has not been deleted above
  {
    _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "%p release %i calling fasync_helper",
                 filp, iminor( inode ));
    fasync_helper( -1, filp, 0, &pddev->fasyncptr );
  }


out_up_sem_fops:
  _up( &sem_fops, "sem_fops", "release", filp );

out:
  _mbg_dbg_hw_lpt_clr_bit( MBG_BIT_RELEASE );

  return retval;

}  // mbgclock_release



#if DEBUG_IRQ_LATENCY

static /*HDR*/
long dt( int64_t delta_cyc )
{
  return (long) ( delta_cyc / cpu_khz / 1000 );

}  // dt

#endif



static /*HDR*/
ssize_t mbgclock_read( struct file *filp, char *buffer,
                       size_t count, loff_t *ppos )
{
  PCPS_DDEV *pddev;
  unsigned long flags = 0;
  PCPS_TIME t;
  char timestr[MBG_SIZE_TLG];
  int bytes_to_copy;
  ssize_t sys_rc = mbgdrvr_get_pddev( &pddev, filp, ( filp->f_flags & O_NONBLOCK ) ?
                                      "read (non-blocking)" : "read (blocking)" );

  if ( sys_rc < 0 )
    goto out;

  _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "%p read starts", filp );

  if ( atomic_read( &pddev->open_count ) == 0 )  //##++
  {
    _mbgddmsg_3( DEBUG, MBG_LOG_ERR, "%p read: dev " MBG_DEV_NAME_FMT " not opened",
                 filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    sys_rc = -EIO;
    goto out;
  }

  #if !ESPIPE_BY_VFS
    if ( ppos != &filp->f_pos )
    {
      _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_WARN, "%p read: dev " MBG_DEV_NAME_FMT " past end",
                   filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      sys_rc = -ESPIPE;
      goto out;
    }
  #endif

  if ( buffer == NULL )
  {
    _mbgddmsg_1( DEBUG, MBG_LOG_ERR, "%p read: no buffer specified", filp );
    sys_rc = -EINVAL;
    goto out;
  }

  mbgdrvr_enable_cyclic( pddev, 0 );

  if ( count < sizeof( timestr ) )
    _mbgddmsg_3( DEBUG, MBG_LOG_WARN, "%p read: buffer size (%i) less than required (%i)",
                 filp, (int) count, (int) sizeof( timestr ) );

  _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "%p read: wait for data_avail", filp );

  while ( !atomic_read( &pddev->data_avail ) )
  {
    if ( filp->f_flags & O_NONBLOCK )
    {
      _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_WARN, "%p read non-blocking returns with no data available, dev: " MBG_DEV_NAME_FMT,
                   filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      sys_rc = -EAGAIN;
      goto out;
    }

    #if USE_WAIT_EVENT
      for (;;)
      {
        int dev_connected = 0;
        int this_rc = wait_event_interruptible_timeout( pddev->wait_queue,
           atomic_read( &pddev->data_avail ) || !(dev_connected = get_dev_connected( pddev ) ), CYCLIC_TIMEOUT );

        if ( this_rc < 0 )  // interrupted
          goto out_interrupted_wait;

        if ( !dev_connected )  // device has been removed
        {
          _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_WARN, "%p read dev " MBG_DEV_NAME_FMT ": device removed while waiting",
                       filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
          sys_rc = -ERESTARTSYS;
          goto out;
        }

        if ( this_rc )  // data available, the normal case
          break;

        // timeout, cyclic reads may have been disabled, try to re-enable
        _mbgddmsg_3( DEBUG, MBG_LOG_WARN, "%p read: IRQ timeout, dev " MBG_DEV_NAME_FMT,
                     filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

        mbgdrvr_enable_cyclic( pddev, 2 );
      }
    #else
      // There may be a race condition if data has become available
      // after the while() check but before we go to sleep below.
      // However, this doesn't really matter for our application.
      interruptible_sleep_on_timeout( &pddev->wait_queue );   // used with 2.4.x and needs to be fixed

      if ( signal_pending( current ) )
        goto out_interrupted_wait;
    #endif
  }


  if ( get_cyclic_lock( pddev, &flags, __func__ ) < 0 )
    return -ERESTARTSYS;

  t = pddev->t;
  atomic_set( &pddev->data_avail, 0 );

  release_cyclic_lock( pddev, &flags, __func__ );


  pcps_time_to_time_str( &t, timestr, sizeof( timestr ) );

  #if DEBUG_IRQ_LATENCY
    if ( debug > 1 )
    {
      unsigned long long tsc;

      rdtscll( tsc );

      if ( _pcps_ddev_is_usb( pddev ) )
        mbg_kdd_msg( MBG_LOG_INFO, "%s %Lu %lu %lu, %lu", _pcps_ddev_type_name( pddev ),
                     tsc, dt( tsc - tsc_usb_2 ), dt( tsc_usb_2 - tsc_usb_1 ),
                     dt( tsc_usb_1 - tsc_irq_1 ) );
      else
        mbg_kdd_msg( MBG_LOG_INFO, "%s %Lu %lu %lu", _pcps_ddev_type_name( pddev ),
                     tsc, dt( tsc - tsc_irq_2 ), dt( tsc_irq_2 - tsc_irq_1 ) );
    }
  #endif

  bytes_to_copy = _min( count, sizeof( timestr ) - 1 );

  if ( copy_to_user( buffer, timestr, bytes_to_copy ) )
  {
    _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_ERR, "%p read: unable to copy to user space, dev " MBG_DEV_NAME_FMT,
                  filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    sys_rc = -EFAULT;
    goto out;
  }

  sys_rc = bytes_to_copy;
  _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_INFO, "%p read dev " MBG_DEV_NAME_FMT ": \"%s\"",
               filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ), timestr );
  goto out;


out_interrupted_wait:
  _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_INFO, "%p read dev " MBG_DEV_NAME_FMT ": blocking interrupted",
               filp, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
  sys_rc = -ERESTARTSYS;

out:
  return sys_rc;

}  // mbgclock_read



static /*HDR*/
ssize_t mbgclock_write( struct file *filp, const char *buffer,
                        size_t count, loff_t *ppos )
{
  _mbgddmsg_1( DEBUG, MBG_LOG_WARN, "%p write: not supported", filp );

  #if !ESPIPE_BY_VFS
    if ( ppos != &filp->f_pos )
      return -ESPIPE;
  #endif

  return -EIO;

}  // mbgclock_write



static /*HDR*/
int mbgdrvr_ioctl_emu_serial( struct file *filp, unsigned int cmd, unsigned long arg )
{
  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "%p ioctl: emu serial cmd 0x%X", filp, cmd );

  switch ( cmd )
  {
    case TCGETS:
    {
      struct termios termios = { 0 };

      /*** Setzen der termios-Werte fuer die Meinberg Funkuhr
         ueber serielle Leitung zur Abfrage. ***/
      termios.c_iflag = IGNBRK|IGNPAR|ISTRIP;   /* input mode flags */
      termios.c_oflag = 0;                      /* output mode flags */
      termios.c_cflag = B9600|CS7|PARENB|CREAD|HUPCL|CLOCAL;  /* control mode flags */
      termios.c_lflag = 0;                      /* local mode flags */
      termios.c_line = 0;                       /* line discipline */

      if ( copy_to_user( (struct termios *) arg, &termios, sizeof( termios ) ) )
      {
        _mbgddmsg_1( DEBUG, MBG_LOG_WARN, "%p ioctl: cmd TCGETS copy_to_user failed", filp );
        return -EFAULT;
      }

      break;
    }


    case TIOCGWINSZ:
    {
      struct winsize winsize = { 0, 0, 0, 0 };

      if ( copy_to_user( (struct winsize *) arg, &winsize, sizeof( winsize ) ) )
      {
        _mbgddmsg_1( DEBUG, MBG_LOG_WARN, "%p ioctl: cmd TIOCGWINSZ copy_to_user failed", filp );
        return -EFAULT;
      }

      break;
    }


    case TCSETS:
    case TCFLSH:
      break;

    default:
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_WARN, "%p ioctl: serial IOCTL cmd 0x%X not supported",
                   filp, cmd );
      return -EINVAL;

  }  // switch

  return 0;

}  // mbgdrvr_ioctl_emu_serial



#if DEBUG_IOCTL

static /*HDR*/
void decode_ioctl( const char *s, ulong cmd )
{
  _mbgddmsg_5( DEBUG_IOCTL, MBG_LOG_INFO, "          %s 0x%08lX: type: 0x%02lX, nr: 0x%02lX, size: %lu",
               s, cmd, (ulong) _IOC_TYPE( cmd ), (ulong) _IOC_NR( cmd ), (ulong) _IOC_SIZE( cmd ) );

}  // decode_ioctl

#endif



static /*HDR*/
// Unlike the other kernel functions which return POSIX errnos in case of
// an error, this fuction returns one of the MBG_ERROR_CODES, except
// for some IOCTLs for serial port emulation which are only received
// if ntpd accesses this device via the parse driver.
long mbgclock_unlocked_ioctl( struct file *filp, unsigned int cmd, unsigned long arg )
{
  PCPS_DDEV *pddev = NULL;
  long sys_rc = mbgdrvr_get_pddev( &pddev, filp, "ioctl" );
  int priv_lvl;
  #if defined( DEBUG )
    const char *ioctl_name = mbgioctl_get_name( cmd );
  #endif

  if ( sys_rc < 0 )  // usually -ENODEV, or -ERESTARTSYS
    goto out;

  // Find out which privilege level is required
  // to execute this IOCTL command.
  priv_lvl = ioctl_get_required_privilege( cmd );

  // Check if the calling process has the required privilege.
  switch ( priv_lvl )
  {
    case MBG_REQ_PRIVL_NONE:
      _mbgddmsg_5( DEBUG, MBG_LOG_INFO, "%p %s (0x%02X): no priv lvl required, dev " MBG_DEV_NAME_FMT,
                   filp, ioctl_name, cmd, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      // Always allow.
      break;

    case MBG_REQ_PRIVL_EXT_STATUS:
    case MBG_REQ_PRIVL_CFG_READ:
      _mbgddmsg_5( DEBUG, MBG_LOG_INFO, "%p %s (0x%02X): no priv lvl required to read cfg, dev " MBG_DEV_NAME_FMT,
                   filp, ioctl_name, cmd, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      // This may require some privilege for the calling process.
      // Anyway, always allow for now.
      break;

    case MBG_REQ_PRIVL_CFG_WRITE:
    case MBG_REQ_PRIVL_SYSTEM:
      #if !OMIT_PRIV_CHECKING   // this is the default case
        _mbgddmsg_5( DEBUG, MBG_LOG_INFO, "%p %s (0x%02X): admin rights required, dev " MBG_DEV_NAME_FMT,
                     filp, ioctl_name, cmd, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

        if ( !capable( CAP_SYS_ADMIN ) )  // allow only if root privileges available.
        {
          _mbgddmsg_5( DEBUG, MBG_LOG_WARN, "%p %s (0x%02X): permission denied, dev " MBG_DEV_NAME_FMT,
                       filp, ioctl_name, cmd, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
          return IOCTL_RC_ERR_PERM;
        }
      #else
        _mbgddmsg_5( DEBUG, MBG_LOG_WARN, "%p %s (0x%02X): admin rights would be required (checking omitted), dev " MBG_DEV_NAME_FMT,
                     filp, ioctl_name, cmd, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      #endif
      break;

    default:
      // maybe an unknown IOCTL code ...
      // This is OK if it is one of those codes required
      switch ( cmd )
      {
        case TCGETS:
        case TIOCGWINSZ:
        case TCSETS:
        case TCFLSH:
          sys_rc = mbgdrvr_ioctl_emu_serial( filp, cmd, arg );

          if ( sys_rc == 0 )
            goto out;  // has been handled normally

          _mbgddmsg_5( DEBUG, MBG_LOG_WARN, "%p %s (0x%02X): invalid cmd, dev " MBG_DEV_NAME_FMT,
                       filp, ioctl_name, cmd,
                       _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
          #if DEBUG_IOCTL
            decode_ioctl( "ioctl", cmd );
          #endif
          break;

        default:
          _mbgddmsg_5( DEBUG, MBG_LOG_WARN, "%p %s (0x%02X): unknown command code, dev " MBG_DEV_NAME_FMT,
                       filp, ioctl_name, cmd, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
          #if DEBUG_IOCTL
            decode_ioctl( "ioctl", cmd );
          #endif
          return IOCTL_RC_ERR_UNSUPP_IOCTL;  // TODO Is this ret val appropriate?

      }  // switch
  }

  sys_rc = ioctl_switch( pddev, cmd, (void *) arg, (void *) arg );

out:
  // If the return value is negative then this is considered as an error code.
  // The kernel converts this to a positive value which is stored in errno, and
  // lets the ioctl call return -1, so the calling user space code has to read
  // errno if -1 is returned.

  return sys_rc;

}  // mbgclock_unlocked_ioctl



#if defined( HAVE_COMPAT_IOCTL ) && defined( CONFIG_COMPAT )

static /*HDR*/
long mbgclock_compat_ioctl( struct file *filp, unsigned int cmd, unsigned long arg )
{
  long rc;

  #if DEBUG_IOCTL
    _mbg_kdd_msg_4( MBG_LOG_INFO, "%p compat_ioctl %s (0x%08X) calling ioctl (0x%08lX)",
                    filp, mbgioctl_get_name( cmd ), cmd, (ulong) IOCTL_PCPS_GENERIC_READ_GPS );

  #endif

  rc = mbgclock_unlocked_ioctl( filp, cmd, arg );

  return rc;

}  // mbgclock_compat_ioctl

#endif



#if !defined( HAVE_UNLOCKED_IOCTL )

static /*HDR*/
int mbgclock_ioctl( struct inode *not_used, struct file *filp, unsigned int cmd, unsigned long arg )
{
  long rc;

  _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "%p unlocked_ioctl calling ioctl()", filp );

  rc = mbgclock_unlocked_ioctl( filp, cmd, arg );

  return (int) rc;

}  // mbgclock_ioctl

#endif



static /*HDR*/
int mbgclock_mmap( struct file * filp, struct vm_area_struct *vma )
{
  MBG_IOMEM_ADDR_RAW addr;
  PCPS_DDEV *pddev = NULL;
  int rc = mbgdrvr_get_pddev( &pddev, filp, "mmap" );

  if ( rc < 0 )
    goto out;

  addr = pddev->rsrc_info.mem[0].start_raw;

#if VMA_HAS_VM_PGOFF
  if ( ( ( vma->vm_end - vma->vm_start ) != PAGE_SIZE ) || vma->vm_pgoff )
#else
  if ( ( ( vma->vm_end - vma->vm_start ) != PAGE_SIZE ) )
#endif
  {
    mbg_kdd_msg( MBG_LOG_ERR, "vm_end (0x%08lX) - vm_start (0x%08lX) doesn't match PAGE_SIZE (0x%08lX)",
                 vma->vm_end, vma->vm_start, (unsigned long) PAGE_SIZE );
    rc = -EINVAL;
    goto out;
  }

  #if VMA_HAS_VM_PGOFF
    _mbgddmsg_4( DEBUG_DRVR, MBG_LOG_INFO,
                 "vm_end (0x%08lX) - vm_start (0x%08lX) == PAGE_SIZE (0x%08lX), pg_off: 0x%08lX",
                 vma->vm_end, vma->vm_start, (unsigned long) PAGE_SIZE, vma->vm_pgoff );
  #else
    _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_INFO, "vm_end (0x%08lX) - vm_start (0x%08lX) == PAGE_SIZE (0x%08lX)",
                 vma->vm_end, vma->vm_start, (unsigned long) PAGE_SIZE );
  #endif

  vma->vm_flags |= VM_IO;

  #if defined( pgprot_noncached )
    vma->vm_page_prot = pgprot_noncached( vma->vm_page_prot );
  #endif

  #if _PCPS_HAS_REMAP_PFN
    rc = io_remap_pfn_range( vma, vma->vm_start, addr >> PAGE_SHIFT, PAGE_SIZE, vma->vm_page_prot );
  #else
    rc = io_remap_page_range( vma, vma->vm_start, addr, PAGE_SIZE, vma->vm_page_prot );
  #endif

  if ( rc )
  {
    rc = -EAGAIN;
    goto out;
  }

  rc = 0;

out:
  return rc;

}  // mbgclock_mmap



struct file_operations mbgclock_fops =
{
  #if NEW_FILE_OPS
    owner: THIS_MODULE,
  #endif

  read: mbgclock_read,
  write: mbgclock_write,
  poll: mbgclock_poll,
  #if defined( HAVE_UNLOCKED_IOCTL )
    unlocked_ioctl: mbgclock_unlocked_ioctl,
  #else
    ioctl: mbgclock_ioctl,
  #endif
  #if defined( HAVE_COMPAT_IOCTL ) && defined( CONFIG_COMPAT )
    compat_ioctl: mbgclock_compat_ioctl,  // called if a 32 bit app ioctls a 64 bit driver
  #endif
  open: mbgclock_open,
  flush: mbgclock_flush,
  release: mbgclock_release,
  fasync: mbgclock_fasync,
  mmap: mbgclock_mmap,
  llseek: NULL
};



#if _PCPS_USE_PCI_PNP

static /*HDR*/
int __devinit mbgdrvr_create_device( PCPS_DDEV *pddev )
{
  int rc;
  int dev_idx;
  dev_t dev;

  _mbgddmsg_fnc_entry();

  _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Going to create Linux device for " MBG_DEV_NAME_FMT,
               _pcps_ddev_type_name( pddev ),
               _pcps_ddev_sernum( pddev ) );

  dev_idx = ddev_list_add_entry( pddev );

  if ( dev_idx < 0 )
  {
    mbg_kdd_msg( MBG_LOG_WARN, "Didn't find free device list entry" );
    goto fail;
  }

  dev = MKDEV( major, minor + dev_idx );
  pddev->lx_dev = dev;

  cdev_init( &pddev->cdev, &mbgclock_fops );
  pddev->cdev.owner = THIS_MODULE;

  rc = cdev_add( &pddev->cdev, dev, 1 );

  if ( rc )
  {
    _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_ERR, "Failed to add chrdev, index %d", dev_idx );
    goto fail;
  }

  _mbgddmsg_3( DEBUG_DRVR, MBG_LOG_INFO, "Successfully registered chrdev %d:%d, index %d",
               MAJOR( pddev->cdev.dev ), MINOR( pddev->cdev.dev ), dev_idx );

  #if _PCPS_HAVE_LINUX_CLASS
    if ( !IS_ERR( mbgclock_class ) )
    {
      #if _PCPS_CLASS_DEV_OBSOLETED
        struct device *device;
        device = device_create( mbgclock_class, NULL, dev,
                                #if _PCPS_DEVICE_CREATE_WITH_DEV_DATA
                                  pddev,
                                #endif
                                mbg_clk_dev_node_fmt, minor + dev_idx );
      #elif _PCPS_HAVE_LINUX_CLASS_CREATE
        struct class_device *device;
        device = class_device_create( mbgclock_class,
                                      #if _PCPS_CLASS_DEVICE_CREATE_WITH_PARENT
                                        NULL,
                                      #endif
                                      dev, NULL, mbg_clk_dev_node_fmt, minor + dev_idx );
      #elif _PCPS_HAVE_LINUX_CLASS_SIMPLE
        struct class_device *device;
        device = class_simple_device_add( mbgclock_class, dev, NULL, mbg_clk_dev_node_fmt, minor + dev_idx );
      #endif

      if ( IS_ERR( device ) )
      {
        mbg_kdd_msg( MBG_LOG_WARN, "Failed to create device for " MBG_DEV_NAME_FMT ", errno: %li",
                     _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ), PTR_ERR( device ) );
        goto fail;
      }
    }
  #endif


  #if _USE_LINUX_DEVFS
    devfs_mk_cdev( MKDEV( pddev->major, 0 ),
                   S_IFCHR | S_IRUSR | S_IWUSR, driver_name );
  #endif

  drvr_info.n_devs++;

  if ( pddev->irq_stat_info & PCPS_IRQ_STAT_UNSAFE )
    mbg_kdd_msg( MBG_LOG_WARN, "*** Warning: %s IRQ support unsafe with FW v" PCPS_FW_STR_FMT " and ASIC v" PCPS_ASIC_STR_FMT,
                 _pcps_ddev_type_name( pddev ),
                 _pcps_fw_rev_num_major( _pcps_ddev_fw_rev_num( pddev ) ),
                 _pcps_fw_rev_num_minor( _pcps_ddev_fw_rev_num( pddev ) ),
                 _pcps_asic_version_major( _pcps_ddev_asic_version( pddev ) ),
                 _pcps_asic_version_minor( _pcps_ddev_asic_version( pddev ) ) );

  #if NEW_WAIT_QUEUE
    init_waitqueue_head( &pddev->wait_queue );
  #else
    pddev->wait_queue = NULL;
  #endif

#if 0  //##++++++++++
  pddev->remove_lock = ATOMIC_INIT( 0 );
  pddev->access_in_progress = ATOMIC_INIT( 0 );
  pddev->data_avail = ATOMIC_INIT( 0 );
#endif

  #if _PCPS_USE_USB
    _sema_init_pddev( &pddev->sem_usb_cyclic, 1, "sem_usb_cyclic", __func__, pddev );
  #endif

  if ( default_fast_hr_time_pddev == NULL )
    if ( _pcps_ddev_has_fast_hr_timestamp( pddev ) )
    {
      default_fast_hr_time_pddev = pddev;
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Made " MBG_DEV_NAME_FMT " the default device for MM access",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    }

  if ( default_ucap_pddev == NULL )
    if ( _pcps_ddev_has_ucap( pddev ) )
    {
      default_ucap_pddev = pddev;
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Made " MBG_DEV_NAME_FMT " the default device for user capture support",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    }

  _mbgddmsg_fnc_exit_success();
  return 0;

fail:
  _mbgddmsg_fnc_exit_err_dec( -1 );
  return -1;  // TODO

}  // mbgdrvr_create_device



// Applying the _devexit attribute to the function below causes a linker warning
// since the function could also be called from outside the exit section.

static /*HDR*/
void mbgdrvr_delete_device( PCPS_DDEV *pddev )
{
  _mbgddmsg_fnc_entry();

  if ( pddev )
  {
    _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Going to delete device " MBG_DEV_NAME_FMT,
                 _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

    if ( pddev == default_fast_hr_time_pddev )
    {
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Removing " MBG_DEV_NAME_FMT " as default device for MM access",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      default_fast_hr_time_pddev = NULL;
    }

    if ( pddev == default_ucap_pddev )
    {
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Removing " MBG_DEV_NAME_FMT " as default device for ucap events",
                   _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
      default_ucap_pddev = NULL;
    }

    #if _USE_LINUX_DEVFS
      //##++ devfs_mk_cdev( MKDEV( pddev->major, 0 ),
      //               S_IFCHR | S_IRUSR | S_IWUSR, driver_name );
    #endif

    cdev_del( &pddev->cdev );

    #if _PCPS_HAVE_LINUX_CLASS
      if ( !IS_ERR( mbgclock_class ) )
      {
        #if _PCPS_CLASS_DEV_OBSOLETED
          device_destroy( mbgclock_class, pddev->lx_dev );
        #elif _PCPS_HAVE_LINUX_CLASS_CREATE
          class_device_destroy( mbgclock_class, pddev->lx_dev );
        #elif _PCPS_HAVE_LINUX_CLASS_SIMPLE
          class_simple_device_remove( pddev->lx_dev );
        #endif

        _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Destroyed class device %d:%d",
                     MAJOR( pddev->lx_dev ), MINOR( pddev->lx_dev ) );
      }
    #endif

    pcps_cleanup_device( pddev );
    pcps_cleanup_ddev( pddev );

    ddev_list_remove_entry( pddev );

    if ( drvr_info.n_devs )
      drvr_info.n_devs--;
  }
  else
    _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "Unable to delete device: device pointer is NULL" );

  _mbgddmsg_fnc_exit();

}  // mbgdrvr_delete_device



#if _PCPS_USE_ISA && !_PCPS_USE_ISA_PNP  //##++ this needs cleanup

static /*HDR*/
int __devinit mbgdrvr_add_isa_device( PCPS_DDEV *pddev )
{
  int rc = mbgdrvr_create_device( pddev );

  if ( rc >= 0 )
    set_dev_connected( pddev, 1 );

  return rc;

}  // mbgdrvr_add_isa_device

#endif



static /*HDR*/
int __devinit mbgclock_probe_pci_device( struct pci_dev *pci_dev,
                                         const struct pci_device_id *ent )
{
  PCPS_DDEV *pddev = NULL;
  int rc;
  int i;

  _mbgddmsg_fnc_entry();

  rc = pci_enable_device( pci_dev );

  if ( rc )
  {
    mbg_kdd_msg( MBG_LOG_ERR, "Failed to enable PCI device %04X, I/O %04lX: rc = %i",
                 pci_dev->device, (ulong) pci_resource_start( pci_dev, 0 ), rc );
    goto fail;
  }

  // Get address of unused driver device structure.
  rc = pcps_init_ddev( &pddev );

  if ( mbg_rc_is_error( rc ) )
  {
    rc = -ENOMEM;
    goto fail;
  }

  rc = pcps_setup_ddev( pddev, PCPS_BUS_PCI, pci_dev->device );

  if ( mbg_rc_is_error( rc ) )
  {
    rc = -ENODEV;
    goto fail;
  }

  for ( i = 0; i < 5; i++ )
  {
    ulong flags = pci_resource_flags( pci_dev, i );

    if ( flags & IORESOURCE_IO )
      pcps_add_rsrc_io( pddev, pci_resource_start( pci_dev, i ), pci_resource_len( pci_dev, i ) );
    else
      if ( flags & IORESOURCE_MEM )
        pcps_add_rsrc_mem( pddev, pci_resource_start( pci_dev, i ), pci_resource_len( pci_dev, i ) );
  }

  pcps_add_rsrc_irq( pddev, pci_dev->irq );

  rc = pcps_probe_device( pddev, pci_dev->bus->number, pci_dev->devfn );

  if ( mbg_rc_is_error( rc ) )
  {
    rc = -EIO;
    goto fail;
  }

  pci_set_drvdata( pci_dev, pddev );

  rc = mbgdrvr_create_device( pddev );

  if ( rc < 0 )
    goto fail;


  set_dev_connected( pddev, 1 );

  _mbgddmsg_fnc_exit_success();

  return 0;

fail:
  if ( pddev )
    pcps_cleanup_ddev( pddev );

  _mbgddmsg_fnc_exit_err_dec( rc );
  return rc;

}  // mbgclock_probe_pci_device



static /*HDR*/
void __devexit mbgclock_remove_pci_device( struct pci_dev *pci_dev )
{
  PCPS_DDEV *pddev = (PCPS_DDEV *) pci_get_drvdata( pci_dev );

  _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "Removing PCI device %04X",
               pci_dev->device );

  set_dev_connected( pddev, 0 );

  mbgdrvr_delete_device( pddev );

  pci_set_drvdata( pci_dev, NULL );

}  // mbgclock_remove_pci_device



static struct pci_device_id mbgclock_pci_tbl[] __devinitdata =
{
  { PCI_VENDOR_MEINBERG, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID },
  { 0 }
};

MODULE_DEVICE_TABLE( pci, mbgclock_pci_tbl );

static struct pci_driver mbgclock_pci_driver =
{
  name:      MBG_DRVR_NAME,
  id_table:  mbgclock_pci_tbl,
  probe:     mbgclock_probe_pci_device,
  remove:    __devexit_p( mbgclock_remove_pci_device )
};

#endif  // _PCPS_USE_PCI_PNP



#if _PCPS_USE_USB

static /*HDR*/
int __devinit mbgclock_probe_usb_device( struct usb_interface *pintf,
                                         const struct usb_device_id *ent )
{
  PCPS_DDEV **ppddev = NULL;
  PCPS_DDEV *pddev = NULL;
  PCPS_DEV_ID dev_id;
  int rc;
  struct usb_device *usb_device;

  _mbgddmsg_fnc_entry();

  if ( _down_interruptible( &sem_fops, "sem_fops", __func__, pintf ) < 0 )
  {
    rc = -ERESTARTSYS;
    goto fail;
  }

  usb_device = interface_to_usbdev( pintf );
  dev_id = le16_to_cpu( usb_device->descriptor.idProduct );

  // search list if device already exists!

  #if STRUCT_USB_DEVICE_HAS_SERIAL
    ppddev = ddev_list_locate_device( PCPS_BUS_USB, dev_id, usb_device->serial );
  #elif STRUCT_USB_DEVICE_HAS_STATIC_SERIAL
    ppddev = ddev_list_locate_device( PCPS_BUS_USB, dev_id, usb_device->static_serial );
  #else
    if ( usb_device->descriptor.iSerialNumber )
    {
      char buf[128];

      if ( usb_string( usb_device, usb_device->descriptor.iSerialNumber, buf, sizeof( buf ) ) > 0 )
        ppddev = ddev_list_locate_device( PCPS_BUS_USB, dev_id, buf );
    }
  #endif

  if ( ppddev )
    pddev = *ppddev;
  else
  {
    rc = pcps_init_ddev( &pddev );

    if ( mbg_rc_is_error( rc ) )
    {
      rc = -ENOMEM;
      goto fail_free_up_sem_fops;
    }
  }

  pddev->udev = usb_device;

  rc = pcps_setup_ddev( pddev, PCPS_BUS_USB, dev_id );

  if ( mbg_rc_is_error( rc ) )
  {
    rc = -ENODEV;
    goto fail_free_up_sem_fops;
  }

  pddev->intf = pintf;

  usb_set_intfdata( pintf, pddev );
  _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "Called usb_set_intfdata()" );

  // no resources to add

  rc = pcps_probe_device( pddev, 0, 0 );

  if ( mbg_rc_is_error( rc ) )
  {
    rc = -EIO;
    goto fail_free_up_sem_fops;
  }

  set_dev_connected( pddev, 1 );

  if ( ppddev == NULL )  // device did not exist before
  {
    rc = mbgdrvr_create_device( pddev );

    if ( rc < 0 )
      goto fail_free_up_sem_fops;
  }

  // If the device has been "recycled" and is still opened
  // by an application then we re-enable cyclic interrupts
  if ( atomic_read( &pddev->open_count ) )
    mbgdrvr_enable_cyclic( pddev, 1 );

  _up_pddev( &sem_fops, "sem_fops", __func__, pddev );

  _mbgddmsg_fnc_exit_success();

  return 0;

fail_free_up_sem_fops:
  if ( pddev )
    pcps_cleanup_ddev( pddev );

  _mbgddmsg_fnc_exit_str( "via fail_free_up_sem_fops" );
  _up_pddev( &sem_fops, "sem_fops", __func__, NULL );

fail:
  _mbgddmsg_fnc_exit_err_dec( rc );
  return rc;

}  // mbgclock_probe_usb_device



static /*HDR*/
void  __devexit mbgclock_remove_usb_device( struct usb_interface *intf )
{
  PCPS_DDEV *pddev = (PCPS_DDEV *) usb_get_intfdata( intf );

  _mbgddmsg_fnc_entry();

  set_dev_connected( pddev, 0 );

  _down( &sem_fops, "sem_fops", __func__, NULL );

  if ( atomic_read( &pddev->open_count ) )
  {
    _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "USB remove: calling wake_up_interruptible" );
    wake_up_interruptible( &pddev->wait_queue );
  }

  if ( atomic_read( &pddev->open_count) == 0 )
  {
    _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "USB remove: wait for outstanding requests" );
    _down( &pddev->dev_mutex, "dev_mutex", __func__, NULL );

    _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "USB remove: delete USB device" );
    mbgdrvr_delete_device( pddev );
    usb_set_intfdata( intf, NULL );
  }

  _mbgddmsg_fnc_exit();

  _up( &sem_fops, "sem_fops", "remove_usb_device", NULL );

}  // mbgclock_remove_usb_device



static struct usb_device_id mbgclock_usb_tbl[] __devinitdata =
{
  // A wildcard product ID for USB devices does not seem to be supported
  // by the kernel, so we have to specify each supported device explicitly:
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_USB5131 ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_TCR51USB ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_MSF51USB ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_WWVB51USB ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_DCF600USB ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_TCR600USB ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_MSF600USB ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_WVB600USB ) },
  { USB_DEVICE( USB_VENDOR_MEINBERG, USB_DEV_TCR180USB ) },
  { 0 }
};

MODULE_DEVICE_TABLE( usb, mbgclock_usb_tbl );

static struct usb_driver mbgclock_usb_driver =
{
  name:        MBG_DRVR_NAME,
  id_table:    mbgclock_usb_tbl,
  probe:       mbgclock_probe_usb_device,
  disconnect:  mbgclock_remove_usb_device
};

#endif  // _PCPS_USE_USB



#ifdef MODULE

// Applying the _exit attribute to the function below causes a linker warning
// since the function could also be called from the module init function.

 /*HDR*/
void mbgclock_cleanup_module( void )
{
  int i;

  _mbgddmsg_fnc_entry();

  #if _PCPS_USE_PNP
    #if _PCPS_USE_PCI_PNP
      pci_unregister_driver( &mbgclock_pci_driver );
      _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "PCI driver has been unregistered" );
    #endif

    #if _PCPS_USE_USB
      usb_deregister( &mbgclock_usb_driver );
      _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "USB driver has been unregistered" );
    #endif
  #endif

  // Stop and remove all devices which are still active (maybe ISA cards)
  for ( i = 0; i < max_devs; i++ )
  {
    PCPS_DDEV **ppddev = &ddev_list[i];

    if ( *ppddev )
    {
      _mbgddmsg_2( DEBUG_DRVR, MBG_LOG_INFO, "Removing device %s, S/N %s",
                   _pcps_ddev_type_name( *ppddev ), _pcps_ddev_sernum( *ppddev ) );
      mbgdrvr_delete_device( *ppddev );
    }
  }

  unregister_chrdev_region( MKDEV( major, minor ), max_devs );
  _mbgddmsg_0( DEBUG_DRVR, MBG_LOG_INFO, "chrdev device numbers have been unregistered" );
  ddev_list_free();

  #if _PCPS_HAVE_LINUX_CLASS
    if ( !IS_ERR( mbgclock_class ) )
    {
      #if _PCPS_HAVE_LINUX_CLASS_CREATE
        class_destroy( mbgclock_class );
      #elif _PCPS_HAVE_LINUX_CLASS_SIMPLE
        class_simple_destroy( mbgclock_class );
      #endif

      _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "Class %s destroyed", mbgclock_class_name );
    }
  #endif

  mbg_kdd_msg( MBG_LOG_INFO, "Meinberg radio clock driver unloading." );

  _mbgddmsg_fnc_exit();

}  // mbgclock_cleanup_module

#endif



 /*HDR*/
int __init mbgclock_init_module( void )
{
  dev_t dev;
  int rc = 0;

  #if _PCPS_USE_PCI_PNP
    static const char info[] = "";  // PnP is common, so don't explicitly mention it.
  #else
    static const char info[] = " (non-PNP)";
  #endif

  #ifdef EXPORT_NO_SYMBOLS
    EXPORT_NO_SYMBOLS;
  #endif

  #ifdef DEBUG
    debug = DEBUG;
  #endif

  _mbgddmsg_fnc_entry();

  mbg_kdd_msg( MBG_LOG_INFO, "Meinberg radio clock driver v%u.%u.%u%s loading.",
               MBG_MAJOR_VERSION_CODE, MBG_MINOR_VERSION_CODE,
               MBG_MICRO_VERSION_CODE, info );

  mbg_kdd_msg( MBG_LOG_INFO, "%s", MBG_COPYRIGHT );

  #if MBG_TGT_SUPP_MEM_ACC
    #if _PCPS_USE_MM_IO
      if ( force_io_access )
        mbg_kdd_msg( MBG_LOG_WARN, "Forcing I/O access by command line parameter" );

      if ( force_mm16_access )
        mbg_kdd_msg( MBG_LOG_WARN, "Forcing 16 bit memory mapped access by command line parameter" );
    #else
      mbg_kdd_msg( MBG_LOG_WARN, "Memory mapped access disabled by build option" );
    #endif
  #endif

  #ifdef DEBUG
    mbg_kdd_msg( MBG_LOG_INFO, "Debug level = %i", debug );
  #endif

  #if DEBUG_MSG_SLEEP
    mbg_kdd_msg( MBG_LOG_WARN, "Sleeping for %i ms after each debug message",
                 debug_msg_sleep );
  #endif

  #if DEBUG_SYS_IRQS
    list_system_irqs();
  #endif

  _sema_init_pddev( &sem_fops, 1, "sem_fops", __func__, NULL );

  // Note: In early 2.6.x kernels the functions alloc_chrdev_region()
  // and register_chrdev_region() expected (char *) rather than
  // (const char *) for the device name. Thus we cast driver_name
  // to (char *) below in order to avoid compiler warnings.

  if ( major )  //##++ != DYNAMIC_MAJOR ?
  {
    dev = MKDEV( major, minor );
    rc = register_chrdev_region( dev, max_devs, (char *) driver_name );  // see note above

    if ( rc < 0 )
    {
      mbg_kdd_msg( MBG_LOG_WARN, "Failed to register major %d", major );
      goto fail;
    }
  }
  else
  {
    rc = alloc_chrdev_region( &dev, minor, max_devs, (char *) driver_name );  // see note above

    if ( rc < 0 )
    {
      mbg_kdd_msg( MBG_LOG_WARN, "Failed to allocate dynamic major %d", major );
      goto fail;
    }

    major = MAJOR( dev );
  }

  _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "Successfully registered chrdev, major %d", major );

  #if _PCPS_HAVE_LINUX_CLASS
    #if _PCPS_HAVE_LINUX_CLASS_CREATE
      mbgclock_class = class_create( THIS_MODULE, mbgclock_class_name );
    #elif _PCPS_HAVE_LINUX_CLASS_SIMPLE
      mbgclock_class = class_simple_create( THIS_MODULE, mbgclock_class_name );
    #endif

    if ( !IS_ERR( mbgclock_class ) )
    {
      _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "Class %s created successfully",
                   mbgclock_class_name );
    }
    else
      mbg_kdd_msg( MBG_LOG_WARN, "Failed to create class %s, no UDEV support",
                   mbgclock_class_name );
  #endif

  rc = ddev_list_alloc();

  if ( rc < 0 )
  {
    mbg_kdd_msg( MBG_LOG_WARN, "failed to allocate device list" );

    goto fail_with_cleanup;
  }

  #if _USE_LINUX_DEVFS
    #error devfs support needs cleanup!!
    devfs_mk_cdev( MKDEV( pddev->major, 0 ),
                  S_IFCHR | S_IRUSR | S_IWUSR, driver_name );
  #endif

  #if _PCPS_USE_PNP
    #if _PCPS_USE_ISA && !_PCPS_USE_ISA_PNP  //##++ this needs cleanup
      pcps_detect_isa_devices( pcps_init_ddev, pcps_cleanup_ddev, mbgdrvr_add_isa_device, io, irq );
    #endif

    #if _PCPS_USE_PCI_PNP
      rc = pci_register_driver( &mbgclock_pci_driver );

      _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "Done registering PCI devices, rc: %d", rc );
    #endif  // _PCPS_USE_PCI_PNP

    #if _PCPS_USE_USB  //##++
      rc = usb_register( &mbgclock_usb_driver );

      _mbgddmsg_1( DEBUG_DRVR, MBG_LOG_INFO, "Done registering USB devices, rc: %d", rc );
    #endif   // _PCPS_USE_USB

  #else
    pcps_detect_devices( io, irq );
  #endif


  if ( drvr_info.n_devs == 0 )
  {
    mbg_kdd_msg( MBG_LOG_INFO, "No supported device found." );
    rc = -ENODEV;
    goto fail_with_cleanup;
  }

  if ( pretend_sync )
    mbg_kdd_msg( MBG_LOG_INFO, "Pretending to NTP to be always sync'ed" );

  _mbgddmsg_fnc_exit_success();
  return 0;


fail_with_cleanup:
  mbgclock_cleanup_module();

fail:
  _mbgddmsg_fnc_exit_err_dec( rc );
  return rc;

}  // mbgclock_init_module



#ifdef module_init
  module_init( mbgclock_init_module );
  module_exit( mbgclock_cleanup_module );
#endif



/*HDR*/
/**
 * @brief Read a high resolution ::PCPS_TIME_STAMP structure via memory mapped access
 *
 * This function can be called from other kernel drivers to read a timestamp
 * of the current time from the first device registered by the driver which
 * supports this call.
 *
 * @param *p_ts Pointer to a ::PCPS_TIME_STAMP structure to be filled up
 *
 * @return ::MBG_SUCCESS on success, or ::MBG_ERR_NOT_SUPP_BY_DEV if no device
 *         supports this call
 *
 * @see ::mbgclock_default_get_fast_hr_timestamp_cycles
 */
int mbgclock_default_get_fast_hr_timestamp( PCPS_TIME_STAMP *p_ts )
{
  if ( default_fast_hr_time_pddev == NULL )
    return MBG_ERR_NOT_SUPP_BY_DEV;

  _mbgddmsg_fnc_entry();

  do_get_fast_hr_timestamp_safe( default_fast_hr_time_pddev, p_ts );

  _mbg_swab_pcps_time_stamp( p_ts );

  _mbgddmsg_fnc_exit();

  return MBG_SUCCESS;

}  // mbgclock_default_get_fast_hr_timestamp

EXPORT_SYMBOL( mbgclock_default_get_fast_hr_timestamp );



/*HDR*/
/**
 * @brief Read a high resolution ::PCPS_TIME_STAMP_CYCLES structure via memory mapped access
 *
 * This function can be called from other kernel drivers to read a timestamp
 * of the current time from the first device registered by the driver which
 * supports this call. The returned structure also contains an associated
 * cycles counter value which can be used to determine the latency.
 *
 * @param *p_ts_cyc Pointer to a ::PCPS_TIME_STAMP_CYCLES structure to be filled up
 *
 * @return ::MBG_SUCCESS on success, or:: MBG_ERR_NOT_SUPP_BY_DEV if no device
 *         supports this call
 *
 * @see ::mbgclock_default_get_fast_hr_timestamp
 */
int mbgclock_default_get_fast_hr_timestamp_cycles( PCPS_TIME_STAMP_CYCLES *p_ts_cyc )
{
  if ( default_fast_hr_time_pddev == NULL )
    return MBG_ERR_NOT_SUPP_BY_DEV;

  _mbgddmsg_fnc_entry();

  do_get_fast_hr_timestamp_cycles_safe( default_fast_hr_time_pddev, p_ts_cyc );

  _mbg_swab_pcps_time_stamp_cycles( p_ts_cyc );

  _mbgddmsg_fnc_exit();

  return MBG_SUCCESS;

}  // mbgclock_default_get_fast_hr_timestamp_cycles

EXPORT_SYMBOL( mbgclock_default_get_fast_hr_timestamp_cycles );



/*HDR*/
/**
 * @brief Clear the on-board user capture FIFO buffer
 *
 * This function can be called from other kernel drivers to clear the user capture
 * FIFO buffer on the first device registered by the driver which supports this call.
 *
 * @return ::MBG_SUCCESS on success, or
 *         ::MBG_ERR_NOT_SUPP_BY_DEV if no device supports this call, or
 *         ::MBG_ERR_IRQ_UNSAFE if called on a device where such calls are unsafe
 *         if IRQs are enabled.
 *
 * @see ::mbgclock_default_get_ucap_entries
 * @see ::mbgclock_default_get_ucap_event
 */
int mbgclock_default_clr_ucap_buff( void )
{
  int rc;

  if ( default_ucap_pddev == NULL )
    return MBG_ERR_NOT_SUPP_BY_DEV;

  _mbgddmsg_fnc_entry();

  _pcps_sem_inc_safe( default_ucap_pddev );
  rc = _pcps_write_byte( default_ucap_pddev, PCPS_CLR_UCAP_BUFF );
  _pcps_sem_dec( default_ucap_pddev );

  _mbgddmsg_fnc_exit();
  return rc;

err_busy_irq_unsafe:
  _mbgddmsg_fnc_exit_err_dec( MBG_ERR_IRQ_UNSAFE );
  return MBG_ERR_IRQ_UNSAFE;

}  // mbgclock_default_clr_ucap_buff

EXPORT_SYMBOL( mbgclock_default_clr_ucap_buff );



/*HDR*/
/**
 * @brief Read user capture FIFO information
 *
 * Read a ::PCPS_UCAP_ENTRIES structure to retrieve the number of saved
 * user capture events and the maximum capture buffer size from the first
 * device registered by the driver which supports this call.
 *
 * @return ::MBG_SUCCESS on success, or
 *         ::MBG_ERR_NOT_SUPP_BY_DEV if no device supports this call, or
 *         ::MBG_ERR_IRQ_UNSAFE if called on a device where such calls are unsafe
 *         if IRQs are enabled.
 *
 * @see ::mbgclock_default_clr_ucap_buff
 * @see ::mbgclock_default_get_ucap_event
 */
int mbgclock_default_get_ucap_entries( PCPS_UCAP_ENTRIES *p )
{
  int rc;

  if ( default_ucap_pddev == NULL )
    return MBG_ERR_NOT_SUPP_BY_DEV;

  _mbgddmsg_fnc_entry();

  _pcps_sem_inc_safe( default_ucap_pddev );
  rc = _pcps_read_var( default_ucap_pddev, PCPS_GIVE_UCAP_ENTRIES, *p );
  _pcps_sem_dec( default_ucap_pddev );

  _mbg_swab_pcps_ucap_entries( p );

  _mbgddmsg_4( DEBUG, MBG_LOG_INFO, MBG_LOG_FMT_LEAVING ": %u/%u, rc: %i",
               __func__, p->used, p->max, rc );
  return rc;

err_busy_irq_unsafe:
  _mbgddmsg_fnc_exit_err_dec( MBG_ERR_IRQ_UNSAFE );
  return MBG_ERR_IRQ_UNSAFE;

}  // mbgclock_default_get_ucap_entries

EXPORT_SYMBOL( mbgclock_default_get_ucap_entries );



/*HDR*/
/**
 * @brief Retrieve single time capture event
 *
 * Retrieve a single time capture event from the on-board FIFO buffer of the
 * first device registered by the driver which supports this call.
 * The captured event time is returned in a ::PCPS_HR_TIME structure.
 * The oldest entry of the FIFO is retrieved and then removed from the FIFO.
 * If no capture event is available in the FIFO buffer then both the seconds
 * and the fractions of the returned timestamp are 0.
 *
 * @return ::MBG_SUCCESS on success, or
 *         ::MBG_ERR_NOT_SUPP_BY_DEV if no device supports this call, or
 *         ::MBG_ERR_IRQ_UNSAFE called on a device where such calls are unsafe
 *         if IRQs are enabled.
 *
 * @see ::mbgclock_default_clr_ucap_buff
 * @see ::mbgclock_default_get_ucap_entries
 */
int mbgclock_default_get_ucap_event( PCPS_HR_TIME *p )
{
  int rc;

  if ( default_ucap_pddev == NULL )
    return MBG_ERR_NOT_SUPP_BY_DEV;

  _mbgddmsg_fnc_entry();

  _pcps_sem_inc_safe( default_ucap_pddev );
  rc = _pcps_read_var( default_ucap_pddev, PCPS_GIVE_UCAP_EVENT, *p );
  _pcps_sem_dec( default_ucap_pddev );

  _mbg_swab_pcps_hr_time( p );

  _mbgddmsg_4( DEBUG, MBG_LOG_INFO, MBG_LOG_FMT_LEAVING ": %08lX.%08lX, rc: %i",
               __func__, (ulong) p->tstamp.sec, (ulong) p->tstamp.frac, rc );
  return rc;

err_busy_irq_unsafe:
  _mbgddmsg_fnc_exit_err_dec( MBG_ERR_IRQ_UNSAFE );
  return MBG_ERR_IRQ_UNSAFE;

}  // mbgclock_default_get_ucap_event

EXPORT_SYMBOL( mbgclock_default_get_ucap_event );



#if defined( DEBUG )

// In a debug build we define a few function pointer variables
// just to verify the declaration of the function pointer types
// match the associated function prototypes.

MBGCLOCK_DEFAULT_GET_FAST_HR_TIMESTAMP_FNC get_fast_hr_timestamp_fnc = mbgclock_default_get_fast_hr_timestamp;
MBGCLOCK_DEFAULT_GET_FAST_HR_TIMESTAMP_CYCLES_FNC get_fast_hr_timestamp_cycles_fnc = mbgclock_default_get_fast_hr_timestamp_cycles;

MBGCLOCK_DEFAULT_CLR_UCAP_BUFF_FNC clr_ucap_buff_fnc = mbgclock_default_clr_ucap_buff;
MBGCLOCK_DEFAULT_GET_UCAP_ENTRIES_FNC get_ucap_entries_fnc = mbgclock_default_get_ucap_entries;
MBGCLOCK_DEFAULT_GET_UCAP_EVENT_FNC get_ucap_event_fnc = mbgclock_default_get_ucap_event;

#endif


#if !MBG_PC_CYCLES_SUPPORTED
  #warning No cycles support to compute latencies on this platform.
#endif

#if OMIT_PRIV_CHECKING
  #warning IOCTL privilege checking is omitted on your own risk!
#endif
