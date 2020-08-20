
/**************************************************************************
 *
 *  $Id: rsrc_lx.h 1.6 2018/07/16 13:00:50 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Definitions and prototypes for rsrc_lx.c.
 *
 * -----------------------------------------------------------------------
 *  $Log: rsrc_lx.h $
 *  Revision 1.6  2018/07/16 13:00:50  martin
 *  Removed obsolete macros.
 *  Updated function prototypes.
 *  Revision 1.5  2017/05/10 15:26:52  martin
 *  Tiny cleanup.
 *  Revision 1.4  2008/12/05 12:02:28  martin
 *  Updated function prototypes.
 *  Revision 1.3  2006/07/07 12:14:25  martin
 *  Include mbg_lx.h to have modversions, if required.
 *  Revision 1.2  2001/03/05 16:27:57  MARTIN
 *  New macros to make function parameters match.
 *  Removed obsolete function prototypes.
 *
 **************************************************************************/

#ifndef _RSRC_LX_H
#define _RSRC_LX_H


/* Other headers to be included */

#include <mbg_lx.h>
#include <rsrc.h>
#include <words.h>

#ifdef _RSRC_LX
 #define _ext
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
 * @brief Try to allocate an I/O port resource range
 *
 * @param[in] port          First port address to allocate
 * @param[in] n             Number of port addresses to allocate
 *
 * @return ::MBG_SUCCESS if the port range could be allocated, else ::MBG_ERR_CLAIM_RSRC
 */
 int rsrc_alloc_ports( ulong port, ulong n ) ;

 /**
 * @brief Release an I/O port resource range which has been allocated before
 *
 * @param[in] port          First port address to release
 * @param[in] n             Number of port addresses to release
 */
 void rsrc_dealloc_ports( ulong port, ulong n ) ;

 /**
 * @brief Try to allocate a memory resource range
 *
 * @param[in] start  First port address to allocate
 * @param[in] len    Number of port addresses to allocate
 *
 * @return ::MBG_SUCCESS if the port range could be allocated, else ::MBG_ERR_CLAIM_RSRC
 */
 int rsrc_alloc_mem( ulong start, ulong len ) ;

 /**
 * @brief Release a memory resource range which has been allocated before
 *
 * @param[in] start  First port address to release
 * @param[in] len    Number of port addresses to release
 */
 void rsrc_dealloc_mem( ulong start, ulong len ) ;


/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif


/* End of header body */

#undef _ext

#endif  /* _RSRC_LX_H */
