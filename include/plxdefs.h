
/**************************************************************************
 *
 *  $Id: plxdefs.h 1.5 2017/05/10 15:26:09 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions to be used with PLX PCI Express interface chips.
 *    Some Meinberg cards use the PLX8311 chip in endpoint mode.
 *
 *    The PLX8311 chip is combined of a PLX8111 PCIe-to-PCI bridge
 *    plus a PCI-to-Local bus interface combined in a single package.
 *    Thus each card using a PLX8311 implements an additional internal
 *    PCI bus with a single device connected to this bus.
 *
 *    Each of these devices are individually visible from the PC's
 *    PCI bus and thus provide their own configuration spaces,
 *    configuration EEPROM, etc.
 *
 *    Care must be taken not to confuse the registers of the PLX8311
 *    with the correspondent registers of the built-in PLX8111 bridge.
 *
 * -----------------------------------------------------------------------
 *  $Log: plxdefs.h $
 *  Revision 1.5  2017/05/10 15:26:09  martin
 *  Tiny cleanup.
 *  Revision 1.4  2013/03/15 10:24:09  martin
 *  Renamed register and bit mask definitions to match the names in the data sheet.
 *  Added doxygen comments.
 *  Revision 1.3  2012/10/15 09:21:42Z  martin
 *  Added some mailbox register addresses.
 *  Revision 1.2  2010/01/28 15:46:31Z  martin
 *  Added PLX8311_REG_CTRL.
 *  Revision 1.1  2007/06/08 07:46:56Z  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _PLXDEFS_H
#define _PLXDEFS_H

/* Other headers to be included */


#ifdef _PLXDEFS
 #define _ext
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * PCI device ID of the PCI bridge also built into the PLX8311
 */
#define PCI_DEVICE_ID_PLX_8111  0x8111


/**
 * @brief PLX PCI Express Configuration Space (PECS) registers
 *
 * These registers are located in the PCI configuration space
 * and can be accessed using standard PCI configuration register
 * access functions.
 *
 * In addition the PECS registers are mirrored into the 64k
 * memory space addressed via PCI base register 0, and thus
 * can be accessed using memory reads or writes via the PCI bus.
 *
 * The default values of these registers can be overwritten
 * by the PCI Express interface serial EEPROM.
 *
 * Care must be taken not to confuse the registers of the PLX8311
 * with the correspondent registers of the built-in PLX8111.
 * Similarly the serial EEPROM for the PLX8311 must not be confused
 * with the serial EEPROM for the built-in PEX8111 PCI bridge.
 *
 * Address Offset:
 *  0000h - 0FFFh  PCI compatible configuration registers
 *  1000h - 1FFFh  Main configuration registers
 *  2000h - 2FFFh  Memory mapped indirect access (PLX8311 only, see manual)
 *  8000h - 9FFFh  8k internal shared memory
 *
 * See chapter 19 of the PLX8311 manual.
 */
enum PLX_PECS_REGS
{
  PLX_PECS_PCICAPPTR   = 0x34,    ///< PCI capabilities pointer
  // PLX_PECS_LINKSTAT = 0x72,    ///< Link status
  PLX_PECS_MAININDEX   = 0x84,    ///< Main Control Register index
  PLX_PECS_MAINDATA    = 0x88,    ///< Main Control Register data

  PLX_PECS_EECTL       = 0x1004,  ///< Serial EEPROM control
  PLX_PECS_EECLKFREQ   = 0x1008,  ///< Serial EEPROM clock frequency control
  PLX_PECS_GPIOCTL     = 0x1020,  ///< General Purpose I/O control
  PLX_PECS_GPIOSTAT    = 0x1024,  ///< General Purpose I/O status
  PLX_PECS_TLPCFG0     = 0x1048,  ///< TLP controller configuration 0

  N_PLX_PECS_REGS                 // dummy
};


// Bit masks used with the PLX_PECS_EECTL register.
// See chap 18.10 of the manual
#define PLX_PECS_EECTL_WRITE_DATA_SHIFT   0
#define PLX_PECS_EECTL_READ_DATA_SHIFT    8
#define PLX_PECS_EECTL_WRITE_START        ( 1UL << 16 )
#define PLX_PECS_EECTL_READ_START         ( 1UL << 17 )
#define PLX_PECS_EECTL_CS_ENB             ( 1UL << 18 )
#define PLX_PECS_EECTL_BUSY               ( 1UL << 19 )
#define PLX_PECS_EECTL_VALID              ( 1UL << 20 )
#define PLX_PECS_EECTL_PRESENT            ( 1UL << 21 )
#define PLX_PECS_EECTL_CS_ACTIVE          ( 1UL << 22 )
#define PLX_PECS_EECTL_RELOAD             ( 1UL << 31 )

// Bit masks used with the PLX_PECS_EECLKFREQ register.
#define PLX_PECS_EECLKFREQ_8_3_MHZ        0x02  // 3 LSBs

// Bit masks used with the PLX_PECS_GPIOCTL register.
#define PLX_PECS_GPIOCTL_GPIO3_DATA       ( 1UL << 3 )



/**
 * @brief Local Configuration Space (LCS) registers
 *
 * These registers are accessed with different addresses from
 * local or PCI, and these addresses are to be used from PCI.
 * See chapter 20 of the PLX8311 manual.
 */
enum PLX_LCS_REGS_PCI
{
  PLX_LCS_INTCSR   = 0x68,  ///< Interrupt control / status
  PLX_LCS_CNTRL    = 0x6C,  ///< 0xEC from local

  N_PLX_LCS_REGS_PCI      // dummy
};


// Bit masks used with the PLX_LCS_CNTRL register.
#define PLX_LCS_CNTRL_USERO  ( 1UL << 16 )
#define PLX_LCS_CNTRL_USERI  ( 1UL << 17 )


// The following bits must be set in the INTCSR register
// to let the local microcontroller be able to generate
// interrupts on the PCI bus via the chip's LINTi# line:
#define PLX_LCS_INTCSR_INT_ENB      ( ( 1UL << 11 ) /* Local Interrupt Input Enable */ \
                                    | ( 1UL << 8 )  /* Internal PCI Wire Interrupt Enable */ \
                                    )

// The bit below signals if an LINTi# interrupt is active:
#define PLX_LCS_INTCSR_INT_FLAG     ( 1UL << 15 )   /* Local Interrupt Input Active */



/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

/* (no header definitions found) */

/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif

/* End of header body */

#undef _ext

#endif  /* _PLXDEFS_H */
