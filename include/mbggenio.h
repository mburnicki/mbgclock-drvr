
/**************************************************************************
 *
 *  $Id: mbggenio.h 1.10 2020/02/24 17:07:36 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions for generic port I/O.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbggenio.h $
 *  Revision 1.10  2020/02/24 17:07:36  martin
 *  Linux function ioremap_nocache() has been obsoleted and removed
 *  for kernel 5.6, so just use  ioremap() with newer kernels.
 *  Cleaned up and removed obsolete stuff.
 *  Revision 1.9  2018/11/22 14:52:53  martin
 *  Moved definition of FAR here.
 *  Fixed build for QNX and MBG_TGT_NO_TGT.
 *  Don't use __iomem attribute with very old Linux kernels.
 *  Moved DEBUG_RSRC stuff to pcpsdrvr.c.
 *  Revision 1.8  2018/07/05 09:08:48  martin
 *  Moved some I/O related definitions here.
 *  Renamed I/O macros and distinguish between macros
 *  that convert endianess, and macros that don't.
 *  Use system functions to implement our own memory mapped
 *  I/O macros since those usually include barriers.
 *  Revision 1.7  2017/08/10 13:53:37  martin
 *  Unified extended resource properties handling.
 *  Revision 1.6  2012/10/02 18:43:36  martin
 *  Fixed includes for NetBSD.
 *  Specify I/O range number when calling port I/O macros
 *  so they can be used for different ranges under BSD.
 *  Account for modified resource handling under *BSD.
 *  Revision 1.5  2008/12/05 13:27:33  martin
 *  Generally put macro arguments in brackets for evaluation
 *  to avoid potential side effects.
 *  There has been a problem with an improper written outp() macro
 *  in the Borland C 3.1 library's conio.h file.
 *  Support mapped I/O resources.
 *  Revision 1.4  2008/02/05 13:38:57  martin
 *  Added support for QNX.
 *  Revision 1.3  2007/03/21 14:48:56  martin
 *  Use standard inp(), outp() also under Windows since the generic
 *  Windows functions READ_PORT_UCHA(), etc., are not very
 *  compatible across DDK versions.
 *  Revision 1.2  2007/03/02 10:23:34Z  martin
 *  Renamed generic port I/O macros.
 *  Fully support Linux, *BSD, Windows, NetWare, DOS, and OS/2.
 *  Revision 1.1  2006/09/20 10:47:21  martin
 *
 **************************************************************************/

#ifndef _MBGGENIO_H
#define _MBGGENIO_H


/* Other headers to be included */

#include <mbg_arch.h>
#include <mbgddmsg.h>

#if defined( MBG_TGT_BSD )

  #include <sys/param.h>
  #include <sys/types.h>
  #include <sys/bus.h>

  #if !defined(__NetBSD_Version__) || __NetBSD_Version__ < 599005500
    #include <machine/bus.h>
  #endif

#endif


#if defined( MBG_TGT_DOS )

  #include <conio.h>
  #include <xportio.h>

#endif


#if defined( MBG_TGT_QNX )

  #if defined( MBG_TGT_QNX_NTO )   // compiling for QNX Neutrino

    // Don't know if we have to distinguish between different compilers.
    #include <hw/inout.h>

  #else    // compiling for QNX4

    #if defined( __WATCOMC__ )   // using Watcom C

      // Include prototypes of port I/O functions
      // which should match the calls used in the mbglib functions.
      #include <conio.h>
    #endif

  #endif

#endif


#if defined( MBG_TGT_NETWARE )

  #include <conio.h>

#endif


#if defined( MBG_TGT_OS2 )

  #include <conio.h>
  #include <xportio.h>

#endif



/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


#if defined( MBG_TGT_DOS )
  #define FAR far
#else
  #define FAR
#endif


// These definitions are used to provide a consistent
// resource handling across different platforms.
// This is kept completely inside the kernel drivers, so the
// structures can be modified safely, as appropriate.

#if defined( MBG_TGT_WIN32 )

  #define _MBG_IOMEM

  typedef uint64_t MBG_IOMEM_ADDR_RAW;
  typedef void _MBG_IOMEM *MBG_IOMEM_ADDR_MAPPED;

  typedef uint16_t MBG_IOPORT_ADDR_RAW;
  typedef uint16_t MBG_IOPORT_ADDR_MAPPED;

#elif defined( MBG_TGT_LINUX )

  #if ( LINUX_VERSION_CODE >= KERNEL_VERSION( 2, 6, 10 ) )
    #define _MBG_IOMEM  volatile __iomem
  #else
    // Older kernels don't know "__iomem". We could at least
    // use the "volatile" attribute, but some kernel API functions
    // which expect such an address as parameter even expect a
    // pointer that is not "volatile", so we don't use that either
    // for such old kernels.
    #define _MBG_IOMEM
  #endif

  typedef uint64_t MBG_IOMEM_ADDR_RAW;
  typedef void _MBG_IOMEM *MBG_IOMEM_ADDR_MAPPED;

  typedef ulong MBG_IOPORT_ADDR_RAW;
  typedef ulong MBG_IOPORT_ADDR_MAPPED;

#elif defined( MBG_TGT_BSD )

  #define _MBG_IOMEM  volatile

  #if defined( MBG_TGT_NETBSD ) && !defined( __LP64__ )
    #define MBG_IOMEM_ADDR_RAW uint32_t
  #else
    #define MBG_IOMEM_ADDR_RAW uint64_t
  #endif

  typedef void _MBG_IOMEM *MBG_IOMEM_ADDR_MAPPED;

  typedef ulong MBG_IOPORT_ADDR_RAW;
  typedef ulong MBG_IOPORT_ADDR_MAPPED;

#elif defined( MBG_TGT_NO_TGT )

  // Nothing to do here.

#else  // QNX, DOS, ...

  #define _MBG_IOMEM  volatile FAR

  typedef uint32_t MBG_IOMEM_ADDR_RAW;
  typedef void _MBG_IOMEM *MBG_IOMEM_ADDR_MAPPED;

  typedef uint16_t MBG_IOPORT_ADDR_RAW;
  typedef uint16_t MBG_IOPORT_ADDR_MAPPED;

#endif



#if defined( MBG_TGT_FREEBSD )

typedef struct
{
  int rid;                   ///< resource ID
  struct resource    *res;   ///< Resource information
  bus_space_tag_t    bst;    ///< Bus space tag
  bus_space_handle_t bsh;    ///< Bus space handle

} EXT_RSRC_INFO;
#define EXT_RSRC_INFO EXT_RSRC_INFO

#elif defined( MBG_TGT_NETBSD )

typedef struct
{
  int                reg;    ///< Base Address Register (BAR)
  int                type;   ///< Type
  int                valid;  ///< Valid flag
  bus_space_tag_t    bst;    ///< Bus space tag
  bus_space_handle_t bsh;    ///< Bus space handle
  bus_addr_t         base;   ///< Base address
  bus_size_t         size;   ///< Size

} EXT_RSRC_INFO;
#define EXT_RSRC_INFO EXT_RSRC_INFO

#endif



/**
 * @brief Bus memory resource information for a device
 *
 * @see ::MBG_IOPORT_RSRC
 * @see ::MBG_IRQ_RSRC
 */
typedef struct
{
  #if defined( EXT_RSRC_INFO )
    EXT_RSRC_INFO ext;                 ///< OS-specific stuff
  #endif
  MBG_IOMEM_ADDR_RAW start_raw;        ///< A raw I/O memory start address
  MBG_IOMEM_ADDR_MAPPED start_mapped;  ///< A mapped I/O memory start address
  ulong len;                           ///< Length of the address range

} MBG_IOMEM_RSRC;



/**
 * @brief I/O port resource information for a device
 *
 * @see ::MBG_IOMEM_RSRC
 * @see ::MBG_IRQ_RSRC
 */
typedef struct
{
  #if defined( EXT_RSRC_INFO )
    EXT_RSRC_INFO ext;                 ///< OS-specific stuff
  #endif
  MBG_IOPORT_ADDR_RAW base_raw;        ///< A raw port base address
  MBG_IOPORT_ADDR_MAPPED base_mapped;  ///< A mapped port base address
  ulong num;                           ///< Number of addresses in this range

} MBG_IOPORT_RSRC;



/**
 * @brief IRQ resource information for a device
 *
 * @see ::MBG_IOPORT_RSRC
 * @see ::MBG_IOMEM_RSRC
 */
typedef struct
{
  #if defined( EXT_RSRC_INFO )
    EXT_RSRC_INFO ext;              ///< OS-specific stuff
  #endif
  uint16_t num;                     ///< The IRQ number

} MBG_IRQ_RSRC;



/**
 * @brief Map I/O memory resource to get a virtual address
 *
 * @param[in,out]  p  Pointer to an I/O memory resource structure
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @see ::mbg_unmap_iomem_rsrc
 */
static __mbg_inline
int mbg_map_iomem_rsrc( MBG_IOMEM_RSRC *p )
{
  #if MBG_TGT_HAS_64BIT_TYPES
    if ( p->start_mapped )
    {
      // This should never happen. Either the resource has already
      // been mapped, or the structure is uninitialized.
      _mbg_kdd_msg_2( MBG_LOG_WARN, "WARNING: mem rsrc %" PRIX64 " already mapped to %p",
                      (uint64_t) p->start_raw, p->start_mapped );
    }
  #endif

  #if defined( MBG_TGT_WIN32 )
  {
    PHYSICAL_ADDRESS phys_addr;

    phys_addr.QuadPart = p->start_raw;

    // NTKERNELAPI PVOID MmMapIoSpace( PHYSICAL_ADDRESS PhysicalAddress,
    //                                 SIZE_T NumberOfBytes,
    //                                 MEMORY_CACHING_TYPE CacheType );
    p->start_mapped = MmMapIoSpace( phys_addr, p->len, MmNonCached );
  }
  #elif defined( MBG_TGT_LINUX )

    // At least starting with kernel 5.6, ioremap_nocache() has been
    // removed from the kernel source. For earlier versions it is just
    // an alias for ioremap(), but very old kernels may distinguish
    // between the 2 variants.
    #if ( LINUX_VERSION_CODE >= KERNEL_VERSION( 5, 5, 0 ) )
      p->start_mapped = ioremap( p->start_raw, p->len );
    #else
      // void __iomem *ioremap_nocache(unsigned long phys_addr, unsigned long size);
      p->start_mapped = ioremap_nocache( p->start_raw, p->len );
    #endif

  #elif defined( MBG_TGT_FREEBSD )

    // void *rman_get_virtual(struct resource *r);
    p->start_mapped = rman_get_virtual( &p->ext.res );

  #elif defined( MBG_TGT_NETBSD )

    // void *bus_space_vaddr(bus_space_tag_t space, bus_space_handle_t handle);
    p->start_mapped = bus_space_vaddr( &p->ext.bst, &p->ext.bsh );

  #elif defined( MBG_TGT_NO_TGT )

    _mbg_map_iomem_rsrc_no_tgt( p );

  #else  // DOS, ...

    // same as the physical address
    p->start_mapped = (MBG_IOMEM_ADDR_MAPPED) p->start_raw;

  #endif  // target specific code

  if ( p->start_mapped == NULL )  // error ...
    return MBG_ERR_NO_MEM;

  return MBG_SUCCESS;

}  // mbg_map_iomem_rsrc



/**
 * @brief Unmap an I/O memory resource to release the virtual address
 *
 * @param[in,out]  p  Pointer to an I/O memory resource structure
 *
 * @see ::mbg_map_iomem_rsrc
 */
static __mbg_inline
void mbg_unmap_iomem_rsrc( MBG_IOMEM_RSRC *p )
{
  if ( p->start_mapped )
  {
    #if defined( MBG_TGT_WIN32 )

      MmUnmapIoSpace( p->start_mapped, p->len );

    #elif defined( MBG_TGT_LINUX )

      iounmap( p->start_mapped );

    #elif defined( MBG_TGT_FREEBSD )

      #if !defined( DEBUG )
        #warning Need to implement unmapping
      #endif

    #elif defined( MBG_TGT_NETBSD )

      #if !defined( DEBUG )
        #warning Need to implement unmapping
      #endif

    #elif defined( MBG_TGT_QNX )

      #if defined( MBG_TGT_QNX_NTO )   // compiling for QNX Neutrino
        // FIXME TODO Call mmap_device_io() ?
      #endif

    #else  // DOS, ...

      // nothing to do

    #endif  // target specific code

    p->start_mapped = NULL;
  }

}  // mbg_unmap_iomem_rsrc



/**
 * @brief Map an I/O port resource to get a virtual address
 *
 * @param[in,out]  p  Pointer to an I/O port resource structure
 *
 * @return ::MBG_SUCCESS on success, else one of the @ref MBG_ERROR_CODES
 *
 * @see ::mbg_unmap_ioport_rsrc
 */
static __mbg_inline
int mbg_map_ioport_rsrc( MBG_IOPORT_RSRC *p )
{
  #if MBG_TGT_HAS_64BIT_TYPES
    if ( p->base_mapped )
    {
      // This should never happen. Either the resource has already
      // been mapped, or the structure is uninitialized.
      _mbg_kdd_msg_2( MBG_LOG_WARN, "WARNING: I/O port rsrc %" PRIX64 " already mapped to %" PRIX64,
                      (uint64_t) p->base_raw, (uint64_t) p->base_mapped );
    }
  #endif

  #if defined( MBG_TGT_BSD )
    // TODO Check if this is true:
    // On *BSD, the real base address is determined by a handle, so
    // we just specify the offset from the base address, which is 0.
    p->base_mapped = 0;
  #else
    // Other targets don't need any mapping.
    p->base_mapped = (MBG_IOPORT_ADDR_MAPPED) p->base_raw;
  #endif

  return MBG_SUCCESS;

}  // mbg_map_ioport_rsrc



/**
 * @brief Unmap an I/O port resource to release the virtual address
 *
 * @param[in,out]  p  Pointer to an I/O port resource structure
 *
 * @see ::mbg_map_ioport_rsrc
 */
static __mbg_inline
void mbg_unmap_ioport_rsrc( MBG_IOPORT_RSRC *p )
{
  if ( p->base_mapped )
  {
    // In fact there is nothing to unmap.
    // Just set the "mapped address" to 0.
    p->base_mapped = (MBG_IOPORT_ADDR_MAPPED) 0;
  }

}  // mbg_unmap_ioport_rsrc



// The I/O memory write barrier should be used whenever a write
// access to the device occurs without a subsequent read, immediately
// before a spinlock is released.
#if defined( MBG_TGT_LINUX )
  #define _mbg_mmiowb()  _mmiowb()
#else
  #define _mbg_mmiowb()  _nop_macro_fnc()
#endif

#if !defined( _mbg_rmb )
  #define _mbg_rmb()  _nop_macro_fnc()
#endif

#if !defined( _mbg_wmb )
  #define _mbg_wmb()  _nop_macro_fnc()
#endif



// Direct access to an I/O memory location, which may suffer from execution
// being reordered by the CPU, unless barriers are explicitly inserted.
// If _mbg_rmb() and _mbg_wmb() may have been defined as empty functions.

static __mbg_inline
uint8_t mbg_mmrd8_native( uint8_t _MBG_IOMEM *p )
{
  uint8_t val = *p;
  _mbg_rmb();
  return val;

}  // mbg_mmrd8_native



static __mbg_inline
uint16_t mbg_mmrd16_native( uint16_t _MBG_IOMEM *p )
{
  uint16_t val = *p;
  _mbg_rmb();
  return val;

}  // mbg_mmrd16_native


static __mbg_inline
uint32_t mbg_mmrd32_native( uint32_t _MBG_IOMEM *p )
{
  uint32_t val = *p;
  _mbg_rmb();
  return val;

}  // mbg_mmrd32_native


static __mbg_inline
void mbg_wrmm8_native( uint8_t val, uint8_t _MBG_IOMEM *p )
{
  *p = val;
  _mbg_wmb();

}  // mbg_wrmm8_native


static __mbg_inline
void mbg_wrmm16_native( uint16_t val, uint16_t _MBG_IOMEM *p )
{
  *p = val;
  _mbg_wmb();

}  // mbg_wrmm16_native


static __mbg_inline
void mbg_wrmm32_native( uint32_t val, uint32_t _MBG_IOMEM *p )
{
  *p = val;
  _mbg_wmb();

}  // mbg_wrmm32_native



#if defined( MBG_TGT_LINUX )

  // readX() / writeX() functions include barriers, and the 16 bit
  // and 32 bit versions also convert the endianess to CPU endianess.

  // Since the OS functions already convert to CPU endianess
  // we need to convert back if we need native endianess.
  // This may result in a a little degradation of the performance
  // on big endian targets. However, little endian targets are not
  // affected since no conversion is required at all.
  // TODO We might try to use memcpy_fromio() or something on bigendian
  // targets to prevent endian conversion. However, this may be even
  // slower since this may always do byte-wise access.

  #define _mbg_mmrd8( _iomem_addr )          readb( (_iomem_addr) )
  #define _mbg_mmrd16_native( _iomem_addr )  _cpu_to_mbg16( readw( (_iomem_addr) ) )
  #define _mbg_mmrd32_native( _iomem_addr )  _cpu_to_mbg32( readl( (_iomem_addr) ) )
  #define _mbg_mmrd16_to_cpu( _iomem_addr )  readw( (_iomem_addr) )
  #define _mbg_mmrd32_to_cpu( _iomem_addr )  readl( (_iomem_addr) )

  #define _mbg_mmwr8( _iomem_addr, _val )          writeb( (_val), (_iomem_addr) )
  #define _mbg_mmwr16_native( _iomem_addr, _val )  writew( _mbg_to_cpu16( (_val) ), (_iomem_addr) )
  #define _mbg_mmwr32_native( _iomem_addr, _val )  writel( _mbg_to_cpu32( (_val) ), (_iomem_addr) )
  #define _mbg_mmwr16_to_mbg( _iomem_addr, _val )  writew( (_val), (_iomem_addr) )
  #define _mbg_mmwr32_to_mbg( _iomem_addr, _val )  writel( (_val), (_iomem_addr) )


  // TODO implement associated _mbg_mmwrX_offs(), _mbg_inpX_offs(),
  // and _mbg_outpX_offs() macros.
  #define _mbg_mmrd8_offs( _dev, _rsrc_idx, _iomem_offs ) \
          _mbg_mmrd8( (uint8_t _MBG_IOMEM *) (_dev)->mm_asic_addr + (_iomem_offs) )

  #define _mbg_mmrd32_to_cpu_offs( _dev, _rsrc_idx, _iomem_offs ) \
          _mbg_mmrd32_to_cpu( (uint8_t _MBG_IOMEM *) (_dev)->mm_addr_asic + (_iomem_offs) )


  #if 0  // TODO
    // The macros below expect additional parameters. Eventually,
    // something like this is required for *BSD.

    #define _mbg_mmrd8_x( _dev, _rsrc_idx, _iomem_addr )          _mbg_mmrd8( (_iomem_addr) )
    #define _mbg_mmrd16_to_cpu_x( _dev, _rsrc_idx, _iomem_addr )  _mbg_mmrd16_to_cpu( (_iomem_addr) )
    #define _mbg_mmrd32_to_cpu_x( _dev, _rsrc_idx, _iomem_addr )  _mbg_mmrd32_to_cpu( (_iomem_addr) )

    #define _mbg_mmwr8_to_mbg_x( _dev, _rsrc_idx, _iomem_addr, _val )   _mbg_mmwr8_to_mbg( (_val), (_iomem_addr) )
    #define _mbg_mmwr16_to_mbg_x( _dev, _rsrc_idx, _iomem_addr, _val )  _mbg_mmwr16_to_mbg( (_val), (_iomem_addr) )
    #define _mbg_mmwr32_to_mbg_x( _dev, _rsrc_idx, _iomem_addr, _val )  _mbg_mmwr32_to_mbg( (_val), (_iomem_addr) )
  #endif

  // Use Linux functions to access the legacy I/O ports

  #define _mbg_inp8( _dev, _rsrc_idx, _port_addr )          ( (uint8_t) inb( (_port_addr) ) )
  #define _mbg_inp16_native( _dev, _rsrc_idx, _port_addr )  _cpu_to_mbg16( inw( (_port_addr) ) )
  #define _mbg_inp32_native( _dev, _rsrc_idx, _port_addr )  _cpu_to_mbg32( inl( (_port_addr) ) )
  #define _mbg_inp16_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) inw( (_port_addr) ) )
  #define _mbg_inp32_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) inl( (_port_addr) ) )

  #define _mbg_outp8( _dev, _rsrc_idx, _port_addr, _val )          outb( (_val), (_port_addr) )
  #define _mbg_outp16_native( _dev, _rsrc_idx, _port_addr, _val )  outw( _mbg_to_cpu16( (_val) ), (_port_addr) )
  #define _mbg_outp32_native( _dev, _rsrc_idx, _port_addr, _val )  outl( _mbg_to_cpu32( (_val) ), (_port_addr) )
  #define _mbg_outp16_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outw( (_val), (_port_addr) )
  #define _mbg_outp32_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outl( (_val), (_port_addr) )

#elif defined( MBG_TGT_BSD )

  #define _mbg_inp8( _dev, _rsrc_idx, _port_addr )   ( (uint8_t) bus_space_read_1( ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bst ), \
                                                                 ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bsh ), (_port_addr) ) )
  #define _mbg_inp16( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) bus_space_read_2( ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bst ), \
                                                                 ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bsh ), (_port_addr) ) )
  #define _mbg_inp32( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) bus_space_read_4( ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bst), \
                                                                 ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bsh ), (_port_addr) ) )

  #define _mbg_outp8( _dev, _rsrc_idx, _port_addr, _val )   bus_space_write_1( ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bst ), \
                                                                ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bsh ), (_port_addr), (_val) )
  #define _mbg_outp16( _dev, _rsrc_idx, _port_addr, _val )  bus_space_write_2( ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bst ), \
                                                                ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bsh ), (_port_addr), (_val) )
  #define _mbg_outp32( _dev, _rsrc_idx, _port_addr, _val )  bus_space_write_4( ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bst ), \
                                                                ( (_dev)->rsrc_info.port[_rsrc_idx].ext.bsh ), (_port_addr), (_val) )

#elif defined( MBG_TGT_WIN32 )

  // Only X86 / X86_64 supported, so we don't care about endianess conversion.

  #if 1
    // Use READ_REGISTER_UCHAR(), WRITE_REGISTER_UCHAR() and friends ...

    #define _mbg_mmrd8( _iomem_addr )          READ_REGISTER_UCHAR( (_iomem_addr) )
    #define _mbg_mmrd16_native( _iomem_addr )  READ_REGISTER_USHORT( (_iomem_addr) )
    #define _mbg_mmrd32_native( _iomem_addr )  READ_REGISTER_ULONG( (_iomem_addr) )
    #define _mbg_mmrd16_to_cpu( _iomem_addr )  READ_REGISTER_USHORT( (_iomem_addr) )
    #define _mbg_mmrd32_to_cpu( _iomem_addr )  READ_REGISTER_ULONG( (_iomem_addr) )

    #define _mbg_mmwr8( _iomem_addr, _val )          WRITE_REGISTER_UCHAR( (_iomem_addr), (_val) )
    #define _mbg_mmwr16_native( _iomem_addr, _val )  WRITE_REGISTER_USHORT( (_iomem_addr), (_val) )
    #define _mbg_mmwr32_native( _iomem_addr, _val )  WRITE_REGISTER_ULONG( (_iomem_addr), (_val) )
    #define _mbg_mmwr16_to_mbg( _iomem_addr, _val )  WRITE_REGISTER_USHORT( (_iomem_addr), (_val) )
    #define _mbg_mmwr32_to_mbg( _iomem_addr, _val )  WRITE_REGISTER_ULONG( (_iomem_addr), (_val) )

  #endif

  // TODO implement associated _mbg_mmwrX_offs(), _mbg_inpX_offs(),
  // and _mbg_outpX_offs() macros.
  #define _mbg_mmrd8_offs( _dev, _rsrc_idx, _iomem_offs ) \
          _mbg_mmrd8( (uint8_t _MBG_IOMEM *) (_dev)->mm_asic_addr + (_iomem_offs) )

  #if 0
    // Use READ_PORT_UCHAR(), WRITE_PORT_UCHAR() and friends ...

    #define _mbg_inp8( _dev, _rsrc_idx, _port_addr )          READ_PORT_UCHAR( (_port_addr) )
    #define _mbg_inp16_native( _dev, _rsrc_idx, _port_addr )  READ_PORT_USHORT( (_port_addr) )
    #define _mbg_inp32_native( _dev, _rsrc_idx, _port_addr )  READ_PORT_ULONG( (_port_addr) )
    #define _mbg_inp16_to_cpu( _dev, _rsrc_idx, _port_addr )  READ_PORT_USHORT( (_port_addr) )
    #define _mbg_inp32_to_cpu( _dev, _rsrc_idx, _port_addr )  READ_PORT_ULONG( (_port_addr) )

    #define _mbg_outp8( _dev, _rsrc_idx, _port_addr, _val )          WRITE_PORT_UCHAR( (_port_addr), (_val) )
    #define _mbg_outp16_native( _dev, _rsrc_idx, _port_addr, _val )  WRITE_PORT_USHORT( (_port_addr), (_val) )
    #define _mbg_outp32_native( _dev, _rsrc_idx, _port_addr, _val )  WRITE_PORT_ULONG( (_port_addr), (_val) )
    #define _mbg_outp16_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  WRITE_PORT_USHORT( (_port_addr), (_val) )
    #define _mbg_outp32_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  WRITE_PORT_ULONG( (_port_addr), (_val) )

  #else
    // Use inp() / outp() and friends.

    #define _mbg_inp8( _dev, _rsrc_idx, _port_addr )          ( (uint8_t) inp( (_port_addr) ) )
    #define _mbg_inp16_native( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) inpw( (_port_addr) ) )
    #define _mbg_inp32_native( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) inpd( (_port_addr) ) )
    #define _mbg_inp16_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) inpw( (_port_addr) ) )
    #define _mbg_inp32_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) inpd( (_port_addr) ) )

    #define _mbg_outp8( _dev, _rsrc_idx, _port_addr, _val )          outp( (_port_addr), (_val) )
    #define _mbg_outp16_native( _dev, _rsrc_idx, _port_addr, _val )  outpw( (_port_addr), (_val) )
    #define _mbg_outp32_native( _dev, _rsrc_idx, _port_addr, _val )  outpd( (_port_addr), (_val) )
    #define _mbg_outp16_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outpw( (_port_addr), (_val) )
    #define _mbg_outp32_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outpd( (_port_addr), (_val) )

  #endif

#elif defined( MBG_TGT_DOS ) || defined( MBG_TGT_NETWARE ) || defined( MBG_TGT_OS2 )

  // Only X86 / X86_64 supported, so we don't care about endianess conversion.

  #define _mbg_inp8( _dev, _rsrc_idx, _port_addr )          ( (uint8_t) inp( (_port_addr) ) )
  #define _mbg_inp16_native( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) inpw( (_port_addr) ) )
  #define _mbg_inp32_native( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) inpd( (_port_addr) ) )
  #define _mbg_inp16_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) inpw( (_port_addr) ) )
  #define _mbg_inp32_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) inpd( (_port_addr) ) )

  #define _mbg_outp8( _dev, _rsrc_idx, _port_addr, _val )          outp( (_port_addr), (_val) )
  #define _mbg_outp16_native( _dev, _rsrc_idx, _port_addr, _val )  outpw( (_port_addr), (_val) )
  #define _mbg_outp32_native( _dev, _rsrc_idx, _port_addr, _val )  outpd( (_port_addr), (_val) )
  #define _mbg_outp16_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outpw( (_port_addr), (_val) )
  #define _mbg_outp32_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outpd( (_port_addr), (_val) )

#elif defined( MBG_TGT_QNX )

  // Only X86 / X86_64 supported, so we don't care about endianess conversion.

  #if defined( MBG_TGT_QNX_NTO )   // compiling for QNX Neutrino

    // ATTENTION: mmap_device_io() must be called on non-x86 architectures
    // to remap the ports, otherwise a segmentation fault will occur if
    // the port I/O functions are being called.
    // FIXME This should be done in the probe routine.

    #define _mbg_inp8( _dev, _rsrc_idx, _port_addr )          ( (uint8_t) in8( (_port_addr) ) )
    #define _mbg_inp16_native( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) in16( (_port_addr) ) )
    #define _mbg_inp32_native( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) in32( (_port_addr) ) )
    #define _mbg_inp16_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) in16( (_port_addr) ) )
    #define _mbg_inp32_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) in32( (_port_addr) ) )

    #define _mbg_outp8( _dev, _rsrc_idx, _port_addr, _val )          out8( (_port_addr), (_val) )
    #define _mbg_outp16_native( _dev, _rsrc_idx, _port_addr, _val )  out16( (_port_addr), (_val) )
    #define _mbg_outp32_native( _dev, _rsrc_idx, _port_addr, _val )  out32( (_port_addr), (_val) )
    #define _mbg_outp16_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  out16( (_port_addr), (_val) )
    #define _mbg_outp32_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  out32( (_port_addr), (_val) )

  #else // compiling for QNX 4

    #if defined( __WATCOMC__ )   // using Watcom C

      #define _mbg_inp8( _dev, _rsrc_idx, _port_addr )          ( (uint8_t) inp( (_port_addr) ) )
      #define _mbg_inp16_native( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) inpw( (_port_addr) ) )
      #define _mbg_inp32_native( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) inpd( (_port_addr) ) )
      #define _mbg_inp16_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint16_t) inpw( (_port_addr) ) )
      #define _mbg_inp32_to_cpu( _dev, _rsrc_idx, _port_addr )  ( (uint32_t) inpd( (_port_addr) ) )

      #define _mbg_outp8( _dev, _rsrc_idx, _port_addr, _val )          outp( (_port_addr), (_val) )
      #define _mbg_outp16_native( _dev, _rsrc_idx, _port_addr, _val )  outpw( (_port_addr), (_val) )
      #define _mbg_outp32_native( _dev, _rsrc_idx, _port_addr, _val )  outpd( (_port_addr), (_val) )
      #define _mbg_outp16_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outpw( (_port_addr), (_val) )
      #define _mbg_outp32_to_mbg( _dev, _rsrc_idx, _port_addr, _val )  outpd( (_port_addr), (_val) )

    #endif

  #endif

#endif


// If no specific memory access functions have been defined above
// then we use our inline functions by default.

#if !defined( _mbg_mmrd8 )
  #define _mbg_mmrd8( _iomem_addr )    mbg_mmrd8( (_iomem_addr) )
#endif

#if !defined( _mbg_mmrd16_native )
  #define _mbg_mmrd16_native( _iomem_addr )   mbg_mmrd16_native( (_iomem_addr) )
#endif

#if !defined( _mbg_mmrd32_native )
  #define _mbg_mmrd32_native( _iomem_addr )   mbg_mmrd32_native( (_iomem_addr) )
#endif


#if !defined( _mbg_mmwr8 )
  #define _mbg_mmwr8( _iomem_addr, _val )   mbg_wrmm8( (_val), (_iomem_addr) )
#endif

#if !defined( _mbg_mmwr16_native )
  #define _mbg_mmwr16_native( _iomem_addr, _val )  mbg_wrmm16_native( (_val), (_iomem_addr) )
#endif

#if !defined( _mbg_mmwr32_native )
  #define _mbg_mmwr32_native( _iomem_addr, _val )  mbg_wrmm32_native( (_val), (_iomem_addr) )
#endif


#ifdef __cplusplus
}
#endif

/* End of header body */

#endif  /* _MBGGENIO_H */
