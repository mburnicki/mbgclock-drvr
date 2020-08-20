
/**************************************************************************
 *
 *  $Id: mbg_lx.h 1.13 2018/04/12 09:46:12 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    OS dependend definitions/redefinitions for Linux.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbg_lx.h $
 *  Revision 1.13  2018/04/12 09:46:12  martin
 *  Code to check if the kernel supports testing if a buffer
 *  is DMA capable.
 *  Revision 1.12  2017/07/05 16:54:16  martin
 *  Removed some obsolete code.
 *  Revision 1.11  2013/03/04 15:43:49  martin
 *  Account for __devinit and friends removed around kernel 3.8.
 *  Revision 1.10  2013/02/19 15:56:15  martin
 *  Moved more kernel version compatibility checks here.
 *  Revision 1.9  2012/10/12 11:23:11  martin
 *  Don't include linux/autoconf.h anymore.
 *  Revision 1.8  2009/03/13 09:04:18  martin
 *  Added default definition for _PCPS_USB_FULL_CYCLIC_INTV.
 *  Revision 1.7  2008/12/19 15:22:05  martin
 *  Moved lots of code here which provides compatibility across
 *  kernel versions.
 *  Added preliminary check if kthread API can be used. However,
 *  the exact version number still needs to be verified.
 *  Added a check whether USB timeouts have to be specified
 *  in jiffies (old) or milliseconds (newer).
 *  Revision 1.6  2007/02/22 15:26:56  martin
 *  Include linux/autoconf.h rather than linux/config.h since the latter has been
 *  a stub for autoconf.h which has been removed around kernel version 2.6.19.
 *  Revision 1.5  2006/09/20 12:09:29  martin
 *  Removed port I/O function redefinitions which have been replaced
 *  by generic function definitions in mbggenio.h.
 *  Revision 1.4  2006/07/07 12:06:40  martin
 *  Include modversions.h if required by the kernel.
 *  Revision 1.3  2001/05/31 09:21:09  martin
 *  #include <asm/io.h> instead of <sys/io.h>
 *  Revision 1.2  2001/03/05 16:36:11  MARTIN
 *  Renamed file.
 *
 **************************************************************************/

#ifndef _MBG_LX_H
#define _MBG_LX_H


#include <linux/version.h>

#if !defined( KERNEL_VERSION ) || ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 2, 0 ) )
  #error "You need to use at least a v2.2.x Linux kernel."
#endif

#if ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 0 ) )
  #define __NO_VERSION__  // don't declare kernel_version in <linux/module.h>
#endif


#if defined( CONFIG_MODVERSIONS ) && !defined( MODVERSIONS )
  #define MODVERSIONS
#endif

#if defined( MODVERSIONS )
  #if ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 0 ) )
    #include <linux/modversions.h>
  #endif
#endif

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/param.h>

#include <asm/uaccess.h>
#include <asm/io.h>


// definitions for clock() support in kernel drivers
extern unsigned long volatile jiffies;
#define clock() jiffies


#define _MBG_INIT_CODE_ATTR           __init


// Starting around kernel 3.8 __devinit and friends
// are not supported anymore.
#if !defined( __devinit )
  #define __devinit
#endif

#if !defined( __devinitdata )
  #define __devinitdata
#endif

#if !defined( __devexit )
  #define __devexit
#endif


// Early hotplug systems may have defined __devexit,
// but not __devexit_p, and similarly kernel 3.8 and newer
// may not have defined __devexit_p anymore.
#if !defined( __devexit_p )
  #define __devexit_p( _x ) _x
#endif



#define _DEFAULT_MBG_TGT_LINUX_USE_PCI_PNP \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 4, 0 ) )

#define _DEFAULT_MBG_TGT_LINUX_USE_USB \
  ( ( defined( CONFIG_USB ) || defined( CONFIG_USB_MODULE ) ) && \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 0 ) ) )

#define _USE_LINUX_SYSFS \
  ( defined( CONFIG_SYSFS ) )

#define _USE_LINUX_DEVFS \
  ( defined( CONFIG_DEVFS_FS ) )

#define _USE_LINUX_DEVFS_26 \
  ( _USE_LINUX_DEVFS && \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 0 ) ) )

#define _USE_LINUX_DEVFS_24 \
  ( _USE_LINUX_DEVFS && \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 4, 0 ) ) )


// Starting with kernel 4.9 we *have to* make sure that e.g.
// USB I/O buffers are DMA capable. Otherwise the kernel will
// generate a trap-like message and prevents the driver from
// working , even if DMA is not used by the driver.
// The inline functions is_vmalloc_addr() and object_is_on_stack()
// can be used to check this, but these functions may not be
// available in older 2.6 kernels.
// is_vmalloc_addr() is supported in kernels ~2.6.25-rc1 and later,
// object_is_on_stack() is supported by ~2.6.27-rc1 and later,
// so we implement this for kernels which provide at least both
// of these functions.
#define _PCPS_CHK_BUFFER_DMA_CAPABLE \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 27 ) )

#if _PCPS_CHK_BUFFER_DMA_CAPABLE
  // Since kernel 4.11 the inline function object_is_on_stack() has
  // been moved from linux/sched.h to linux/sched/task_stack.h.
  #if ( LINUX_VERSION_CODE >= KERNEL_VERSION( 4, 11, 0 ) )
    #include <linux/sched/task_stack.h>
  #else
    #include <linux/sched.h>
  #endif

  #define _pcps_buffer_is_dma_capable( _b ) \
    ( !is_vmalloc_addr( _b ) && !object_is_on_stack( _b ) )
#endif


// Starting at some version 2.6.11 or 2.6.12 time out values for the
// USB functions need to be specified in ms, which is the default.
#define _PCPS_USB_TIMEOUT_IS_MS \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 12 ) )

// Older Linux kernels expect USB timeout intervals in jiffies, so
// we specify a conversion macro for this case.
#if !_PCPS_USB_TIMEOUT_IS_MS
  #define _pcps_ms_to_usb_timeout( _ms )  ( (_ms) * HZ / 1000 )
#endif

// Early Linux 2.6.x kernels generate a syslog msg on each USB timeout, so the syslog
// would be flooded with USB timeout msgs if we used short timeouts with retries.
#if !defined( _PCPS_USB_FULL_CYCLIC_INTV )
  #define _PCPS_USB_FULL_CYCLIC_INTV \
    ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 11 ) )
#endif

#include <linux/cdev.h>   // Requires kernel 2.6.0 or newer

#if !defined( _PCPS_USE_LINUX_KTHREAD )
  #define _PCPS_USE_LINUX_KTHREAD \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 10 ) )  //##++++
#endif

#if _PCPS_USE_LINUX_KTHREAD
  #include <linux/kthread.h>
#endif


#if !defined( NEW_FASYNC )
  // A third parameter to kill_fasync has been added in kernel 2.3.21,
  // and is _not_ present in _stock_ 2.2.14 and 2.2.15.  However, some
  // distributions patch 2.2.x kernels to add this parameter.
  #define NEW_FASYNC      ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 2, 18 ) )
#endif

#if !defined( NEW_FASYNC2 )
  // Type of first parameter of kill_fasync() changed in kernel 2.4.0-test2
  #define NEW_FASYNC2     ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 4, 0 ) )
#endif

#if !defined( NEW_WAIT_QUEUE )
  // New wait queue types and macros have been defined around kernel 2.2.18.
  #define NEW_WAIT_QUEUE  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 2, 18 ) )
#endif

#if !defined( NEW_FILE_OPS )
  // A field "owner" has been added to struct file_operations in kernel 2.4.0
  // which needs to be initialized properly.
  #define NEW_FILE_OPS    ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 4, 0 ) )
#endif

#if !defined( USE_WAIT_EVENT )
  // Function wait_event_interruptible() has been added in kernel 2.4.20
  // which replaces the older interruptible_sleep_on() function.
  #define USE_WAIT_EVENT  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 4, 20 ) )
#endif

#if !defined( ESPIPE_BY_VFS )
  // ESPIPE logic has been removed from drivers and is handled by the VFS layer now.
  #define ESPIPE_BY_VFS   ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 8 ) )
#endif

#if !defined( FLUSH_WITH_LOCK_OWNER_ID )
  // Starting in kernel 2.6.18 the prototype for the flush function in
  // struct file_operations in fs.h has been modified to take the POSIX
  // lock owner ID as additional parameter.
  // The same patch also defined FL_CLOSE in fs.h, so we check additionally
  // whether this symbol is defined.
  // At least for SUSE SLES10 this has been backported to a kernel which
  // reports to be 2.6.16.
  #define FLUSH_WITH_LOCK_OWNER_ID \
    ( ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 18 ) ) \
    || defined( FL_CLOSE ) )
#endif

#if !defined( REQUEST_IRQ_WO_REGS )
  // The prototype of the interrupt handler function has changed around 2.6.19.
  // The newer version does not take a pointer to a register set.
  // This also seems to include linux/bottom_half.h from linux/interrupt.h,
  // so a criteria for this is also if _LINUX_BH_H is defined after
  // linux/interrupt.h has been included.
  #define REQUEST_IRQ_WO_REGS \
    ( ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 19 ) \
    || defined( _LINUX_BH_H ) ) )
#endif


#if !defined( IRQ_RETVAL )
  // defines for compatibility with older kernels
  typedef void irqreturn_t;
  #define IRQ_NONE
  #define IRQ_HANDLED
//##++  #define IRQ_RETVAL(x)
#endif


#if !defined( GENERIC_GPIO_SUPPORTED )
  //##++++ Exact kernel version to be determined!
  // At least 2.6.34 provides linux/gpio.h.
  #define GENERIC_GPIO_SUPPORTED \
    ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 34 ) )
#endif


// Care about kernel API changes across kernel versions

#define _PCPS_MUST_UPDATE_USE_COUNT \
  ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 0 ) )

#define _PCPS_HAVE_LINUX_CLASS \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 0 ) )

#define _PCPS_HAVE_LINUX_CLASS_SIMPLE \
  ( _PCPS_HAVE_LINUX_CLASS && ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 13 ) ) )

#define _PCPS_HAVE_LINUX_CLASS_CREATE \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 13 ) )

#define _PCPS_CLASS_DEVICE_CREATE_WITH_PARENT \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 14 ) )

#define _PCPS_CLASS_DEV_OBSOLETED \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 26 ) )

#define _PCPS_DEVICE_CREATE_WITH_DEV_DATA \
  ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 27 ) )


#if 0  //##+++

// 2.6.11 (2.6.0+): (prototypes copied from <linux/device.h>)
  extern struct class_simple *class_simple_create(struct module *owner, char *name);
  extern void class_simple_destroy(struct class_simple *cs);
  extern struct class_device *class_simple_device_add(struct class_simple *cs, dev_t dev,
                        struct device *device, const char *fmt, ...)
    __attribute__((format(printf,4,5)));
  extern int class_simple_set_hotplug(struct class_simple *,
    int (*hotplug)(struct class_device *dev, char **envp, int num_envp, char *buffer, int buffer_size));
  extern void class_simple_device_remove(dev_t dev);


// 2.6.13:
extern struct class_device *class_device_create(struct class *cls,
                                                dev_t devt,
                                                struct device *device,
                                                char *fmt, ...)
                                        __attribute__((format(printf,4,5)));


// 2.6.22:
extern struct class *class_create(struct module *owner, const char *name);
extern void class_destroy(struct class *cls);
extern struct class_device *class_device_create(struct class *cls,
                                                struct class_device *parent,
                                                dev_t devt,
                                                struct device *device,
                                                const char *fmt, ...)
                                        __attribute__((format(printf,5,6)));
extern void class_device_destroy(struct class *cls, dev_t devt);


// 2.6.25:
extern struct class *class_create(struct module *owner, const char *name);
extern void class_destroy(struct class *cls);
extern struct class_device *class_device_create(struct class *cls,
                                                struct class_device *parent,
                                                dev_t devt,
                                                struct device *device,
                                                const char *fmt, ...)
                                        __attribute__((format(printf, 5, 6)));
extern void class_device_destroy(struct class *cls, dev_t devt);
/*
 * Easy functions for dynamically creating devices on the fly
 */
extern struct device *device_create(struct class *cls, struct device *parent,
                                    dev_t devt, const char *fmt, ...)
                                    __attribute__((format(printf, 4, 5)));
extern void device_destroy(struct class *cls, dev_t devt);


// 2.6.27 (2.6.26+):
extern struct class * __must_check __class_create(struct module *owner,
                                                  const char *name,
                                                  struct lock_class_key *key);
/* This is a #define to keep the compiler from merging different
 * instances of the __key variable */
#define class_create(owner, name)               \
({                                              \
        static struct lock_class_key __key;     \
        __class_create(owner, name, &__key);    \
})
extern void class_destroy(struct class *cls);

extern struct device *device_create(struct class *cls, struct device *parent,
                                    dev_t devt, void *drvdata,
                                    const char *fmt, ...)
                                    __attribute__((format(printf, 5, 6)));
#define device_create_drvdata   device_create
extern void device_destroy(struct class *cls, dev_t devt);

 - class_device_create() and class_device_destroy() have been removed
 - use device_create() and device_destroy() instead

#endif


#if !defined( STRUCT_USB_DEVICE_HAS_SERIAL )
  #define STRUCT_USB_DEVICE_HAS_SERIAL \
    ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 12 ) )
#endif

#if !defined( STRUCT_USB_DEVICE_HAS_STATIC_SERIAL )
  #define STRUCT_USB_DEVICE_HAS_STATIC_SERIAL  0
  // There seems to be a patch in SuSE Linux 9.1 / 2.6.4-52 which supports this ...
#endif


#if !defined( VMA_HAS_VM_PGOFF )
  #define VMA_HAS_VM_PGOFF \
    ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 0 ) )   //##++ needs verification !!
#endif

#if !defined( _PCPS_HAS_REMAP_PFN )
  // remap_pfn_range() and io_remap_pfn_range() have been introduced around 2.6.10
  // to replace remap_page_range() and io_remap_page_range(), but in the next kernel
  // versions the old functions are still supported, so it does not seem critical
  // at which kernel version exactly we start to use the new functions.
  #define _PCPS_HAS_REMAP_PFN \
    ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 10 ) )
#endif

//##++++ Something like this could be used in the Makefile:
//  VMA_PARAM_IN_REMAP=`grep remap_page_range
//  $PATH_LINUX_INCLUDE/linux/mm.h|grep vma`
//  if [ -z "$VMA_PARAM_IN_REMAP" ]; then
//   export REMAP_PAGE_RANGE_PARAM="4"
//  else
//   export REMAP_PAGE_RANGE_PARAM="5"
//  endif


#if !defined( NEED_IMINOR )
  #define NEED_IMINOR \
    ( LINUX_VERSION_CODE < KERNEL_VERSION( 2, 4, 0 ) )   //##++ needs verification !!
#endif

#if NEED_IMINOR
  #define iminor( _d ) MINOR( (_d)->i_rdev )
#endif


#endif  /* _MBG_LX_H */

