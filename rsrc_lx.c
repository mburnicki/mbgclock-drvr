
/**************************************************************************
 *
 *  $Id: rsrc_lx.c 1.9 2018/11/22 16:39:43 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Interface functions for the Linux resource manager.
 *
 * -----------------------------------------------------------------------
 *  $Log: rsrc_lx.c $
 *  Revision 1.9  2018/11/22 16:39:43  martin
 *  Refactored debug logging stuff.
 *  Revision 1.8  2018/07/16 12:59:18  martin
 *  New functions rsrc_alloc_mem() and rsrc_dealloc_mem().
 *  rsrc_alloc_ports() doesn't expect a decode_width parameter anymore.
 *  Account for pcps_driver_name renamed to driver_name.
 *  Conditional debug code controlled by DEBUG_RSRC.
 *  Revision 1.7  2017/07/05 18:20:02  martin
 *  Use Meinberg error codes.
 *  Doxygen comments.
 *  Revision 1.6  2011/07/20 15:02:44  martin
 *  Account for modified type of symbol pcps_driver_name.
 *  Revision 1.5  2009/07/22 12:31:31  martin
 *  Cleaned up region handling across kernel versions.
 *  Revision 1.4  2008/12/05 12:02:06  martin
 *  Expect port addr as ulong.
 *  Revision 1.3  2004/11/09 14:49:56  martin
 *  Modified resource allocation code to match latest requirements.
 *  Included some macros to support calls used with older kernels.
 *  Revision 1.2  2001/03/05 16:26:59  MARTIN
 *  Removed obsolete/unused functions.
 *
 **************************************************************************/

#define _RSRC_LX
 #include <rsrc_lx.h>
#undef _RSRC_LX

#include <mbgerror.h>
#include <mbgddmsg.h>

#include <linux/ioport.h>
#include <linux/version.h>

extern const char driver_name[];



/*HDR*/
/**
 * @brief Try to allocate an I/O port resource range
 *
 * @param[in] port          First port address to allocate
 * @param[in] n             Number of port addresses to allocate
 *
 * @return ::MBG_SUCCESS if the port range could be allocated, else ::MBG_ERR_CLAIM_RSRC
 */
int rsrc_alloc_ports( ulong port, ulong n )
{
  struct resource *p = request_region( port, n, driver_name );

  if ( p == NULL )  // error
  {
    _mbg_kdd_msg_2( MBG_LOG_ERR, "Failed to request I/O region %03lX (%lu)", port, n );
    return MBG_ERR_CLAIM_RSRC;
  }

  _mbgddmsg_3( DEBUG_RSRC, MBG_LOG_INFO, "Requested I/O region: %03lX (%lu): %p", port, n, p );

  return MBG_SUCCESS;

}  // rsrc_alloc_ports



/*HDR*/
/**
 * @brief Release an I/O port resource range which has been allocated before
 *
 * @param[in] port          First port address to release
 * @param[in] n             Number of port addresses to release
 */
void rsrc_dealloc_ports( ulong port, ulong n )
{
  release_region( port, n );

  _mbgddmsg_2( DEBUG_RSRC, MBG_LOG_INFO, "Released I/O region: %03lX (%lu)", port, n );

}  // rsrc_dealloc_ports



/*HDR*/
/**
 * @brief Try to allocate a memory resource range
 *
 * @param[in] start  First port address to allocate
 * @param[in] len    Number of port addresses to allocate
 *
 * @return ::MBG_SUCCESS if the port range could be allocated, else ::MBG_ERR_CLAIM_RSRC
 */
int rsrc_alloc_mem( ulong start, ulong len )
{
  struct resource *p = request_mem_region( start, len, driver_name );

  if ( p == NULL )  // error
  {
    _mbg_kdd_msg_2( MBG_LOG_ERR, "Failed to request mem region %llX (%lu)",
                    (unsigned long long) start, len );
    return MBG_ERR_CLAIM_RSRC;
  }

  _mbgddmsg_3( DEBUG_RSRC, MBG_LOG_DEBUG, "Requested mem region: %llX (%lu): %p",
               (unsigned long long) start, len, p );

  return MBG_SUCCESS;

}  // rsrc_alloc_mem



/*HDR*/
/**
 * @brief Release a memory resource range which has been allocated before
 *
 * @param[in] start  First port address to release
 * @param[in] len    Number of port addresses to release
 */
void rsrc_dealloc_mem( ulong start, ulong len )
{
  release_mem_region( start, len );

  _mbgddmsg_2( DEBUG_RSRC, MBG_LOG_DEBUG, "Released mem region: %llX (%lu)",
               (unsigned long long) start, len );

}  // rsrc_dealloc_mem



