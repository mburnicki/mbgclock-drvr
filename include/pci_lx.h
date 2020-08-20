
/**************************************************************************
 *
 *  $Id: pci_lx.h 1.3 2017/12/12 08:23:46 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions to enable usage of Meinberg PCI function calls
 *    with Linux.
 *
 * -----------------------------------------------------------------------
 *  $Log: pci_lx.h $
 *  Revision 1.3  2017/12/12 08:23:46  martin
 *  Fixed a typo in a comment.
 *  Revision 1.2  2006/07/07 12:12:57  martin
 *  Account for _mbg_pci_..() symbols used in the updated library.
 *  Revision 1.1  2001/02/05 15:30:35  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#ifndef _PCI_LX_H
#define _PCI_LX_H


/* Other headers to be included */

#include <pcidefs.h>

#include <mbg_lx.h>
#include <linux/pci.h>


#ifdef _PCI_LX
 #define _ext
#else
 #define _ext extern
#endif


/* Start of header body */

// The pcibios_..() calls use below are not supported by
// recent 2.6.x kernels. However, those kernels should use
// the PNP PCI interface anyway, whereas the functions below
// are only used by non-PNP environments.

#define _mbg_pci_find_bios( _p1, _p2, _p3 )  \
  ( pcibios_present() ? PCI_SUCCESS : PCI_NO_SUCCESS )

#define _mbg_pci_find_device( _did, _vid, _ix, _pbn, _pdfn )  \
  pcibios_find_device( (_vid), (_did), (_ix), (_pbn), (_pdfn) )

#define _mbg_pci_read_cfg_byte    pcibios_read_config_byte
#define _mbg_pci_read_cfg_word    pcibios_read_config_word
#define _mbg_pci_read_cfg_dword   pcibios_read_config_dword

#define _mbg_pci_write_cfg_byte   pcibios_write_config_byte
#define _mbg_pci_write_cfg_word   pcibios_write_config_word
#define _mbg_pci_write_cfg_dword  pcibios_write_config_dword

/* End of header body */

#undef _ext

#endif  /* _PCI_LX_H */
