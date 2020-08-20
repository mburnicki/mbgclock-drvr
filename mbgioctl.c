
/**************************************************************************
 *
 *  $Id: mbgioctl.c 1.7 2020/06/18 13:09:07 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Utility functions to be used with IOCTLs.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgioctl.c $
 *  Revision 1.7  2020/06/18 13:09:07  martin
 *  Always build with mbgioctl_get_name().
 *  Revision 1.6  2019/04/03 12:59:43  martin
 *  Fixed preprocessor conditionals using unknown symbols.
 *  Revision 1.5  2018/11/22 14:57:43  martin
 *  Check new preprocessor symbol MBG_TGT_USE_IOCTL to exclude
 *  code from build on targets that don't use IOCTL calls.
 *  Revision 1.4  2018/11/05 13:33:26  martin
 *  Avoid signed/unsigned mismatch by using appropriate function parameter.
 *  Revision 1.3  2018/07/05 08:57:05Z  martin
 *  New function mbgioctl_rc_to_mbg_errno().
 *  New function mbgioctl_get_name().
 *  Updated file header comment.
 *  Revision 1.2  2003/03/18 09:30:05  martin
 *  Changed names from ioctlw32 to mbgioctl.
 *  Revision 1.1  2002/02/19 13:51:08Z  MARTIN
 *  Initial revision
 *
 **************************************************************************/

#include <mbg_tgt.h>

// The code below is only required for target systems
// where IOCTLs are used to access a kernel driver, so
// we exclude it from build on systems where the physical
// devices are accessed directly from user space.

#if MBG_TGT_USE_IOCTL

#define _MBGIOCTL
 #include <mbgioctl.h>
#undef _MBGIOCTL

#include <mbgerror.h>


#if defined( MBG_TGT_WIN32 ) && !defined( MBG_TGT_KERNEL )

/*HDR*/
int mbgioctl_rc_to_mbg_errno( int sys_errno )
{
  // FIXME: This is a hack: Under Linux the IOCTL_RC_ERR_...
  // code definitions are all negative, but the sys_errno
  // number we expect here is positive. So just revert the
  // sign to be able to uses the original IOCTL_RC_ERR_...
  // definitions for the switch cases.
  #if defined( MBG_TGT_LINUX )
    int cmp_rc_code = -sys_errno;
  #else
    int cmp_rc_code = sys_errno;
  #endif

  switch ( cmp_rc_code )
  {
    case IOCTL_RC_ERR_PERM:             return MBG_ERR_PERM;
    case IOCTL_RC_ERR_UNSUPP_IOCTL:     return MBG_ERR_INV_DEV_REQUEST;
    case IOCTL_RC_ERR_INVAL_PARAM:      return MBG_ERR_INV_PARM;
    case IOCTL_RC_ERR_NOT_SUPP_BY_DEV:  return MBG_ERR_NOT_SUPP_BY_DEV;
    case IOCTL_RC_ERR_NO_MEM:           return MBG_ERR_NO_MEM;
    case IOCTL_RC_ERR_BUSY_IRQ_UNSAFE:  return MBG_ERR_IRQ_UNSAFE;  // or MBG_ERR_BUSY ?
    case IOCTL_RC_ERR_DEV_ACCESS:       return MBG_ERR_IO;

    #if defined( MBG_TGT_LINUX )  // Linux-specific codes
      case IOCTL_RC_ERR_COPY_TO_USER:      return MBG_ERR_COPY_TO_USER;   // FIXME
      // case IOCTL_RC_ERR_COPY_FROM_USER: return MBG_ERR_COPY_FROM_USER;
    #endif  // defined( MBG_TGT_LINUX )
  }

  // FIXME: This is another hack: Eventually the error code returned
  // in kernel space is remapped to some other code by the kernel,
  // so this is is not covered by the switch above, and the error
  // code used in kernel may not even be defined in user space.
  // E.g. ENOIOCTL for *BSD is only defined in kernel space.
  return mbg_posix_errno_to_mbg( sys_errno, NULL );

}  // mbgioctl_rc_to_mbg_errno

#endif  // defined( MBG_TGT_WIN32 ) && !defined( MBG_TGT_KERNEL )



/*HDR*/
const char *mbgioctl_get_name( long code )
{
  static const MBG_CODE_NAME_TABLE_ENTRY tbl[] = IOCTL_CODES_TABLE;

  const MBG_CODE_NAME_TABLE_ENTRY *p = tbl;

  for ( p = tbl; p->name; p++ )
  {
    if ( p->code == code )
      return p->name;
  }

  return "UNKNOWN_IOCTL";

}  // mbgioctl_get_name


#endif  // MBG_TGT_USE_IOCTL

