
/**************************************************************************
 *
 *  $Id: lx-shared.h 1.7 2018/11/22 16:39:10 martin REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    The definitions here could be shared with the standard
 *    Linux driver for Meinberg PCI cards.
 *
 * -----------------------------------------------------------------------
 *  $Log: lx-shared.h $
 *  Revision 1.7  2018/11/22 16:39:10  martin
 *  Refactored debug logging stuff.
 *  Revision 1.6  2018/09/21 15:25:19  martin
 *  Fixed preprocessor symbol definitions.
 *  Use safer string functions.
 *  Revision 1.5  2018/07/02 16:56:56  martin
 *  Use MBG_DEV_NAME_FMT format string.
 *  Unified kernel driver messages.
 *  Revision 1.4  2017/09/06 11:57:39  martin
 *  Fixed a compiler warning.
 *  Revision 1.3  2017/06/28 16:51:47  martin
 *  Avoid using sprintf().
 *  Revision 1.2  2013/01/02 14:48:15  martin
 *  Cleanup.
 *  Revision 1.1  2011/01/25 11:28:26  martin
 *
 **************************************************************************/

#if !defined( DEBUG_SEM )
  #if defined( DEBUG )
    #define DEBUG_SEM   0   // eventually change this to '1'
  #else
    #define DEBUG_SEM   0   // always leave this '0'
  #endif
#endif


#if DEBUG_SEM

static __mbg_inline
void snprintf_pddev( char *s, size_t max_len, const PCPS_DDEV *pddev )
{
  s[0] = 0;   // Empty string just in case ...

  if ( pddev )
  {
    mbg_kdd_snprintf( s, max_len, ", dev: " MBG_DEV_NAME_FMT,
                      _pcps_ddev_type_name( pddev ),
                      _pcps_ddev_sernum( pddev ) );
  }

}  // snprintf_pddev



static __mbg_inline
void _sema_init_pddev( struct semaphore *ps, int n, const char *sem_name,
                       const char *fnc_name, const PCPS_DDEV *pddev )
{
  char ws[40];

  snprintf_pddev( ws, sizeof( ws ), pddev );

  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%s initializing %s%s",
                fnc_name, sem_name, ws );

  sema_init( ps, n );

}  // _sema_init_pddev



static __mbg_inline
int _down_interruptible_pddev( struct semaphore *ps, const char *sem_name,
                               const char *fnc_name, const PCPS_DDEV *pddev )
{
  int retval;
  char ws[40];

  snprintf_pddev( ws, sizeof( ws ), pddev );

  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%s: going to get %s%s",
                fnc_name, sem_name, ws );

  retval = down_interruptible( ps );

  if ( retval < 0 )
    _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%s: interrupted waiting for %s%s",
                  fnc_name, sem_name, ws );
  else
    _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%s: got %s%s",
                  fnc_name, sem_name, ws );

  return retval;

}  // _down_interruptible_pddev



static __mbg_inline
void _up_pddev( struct semaphore *ps, const char *sem_name,
                const char *fnc_name, const PCPS_DDEV *pddev )
{
  char ws[40];

  snprintf_pddev( ws, sizeof( ws ), pddev );

  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%s: releasing %s%s",
                fnc_name, sem_name, ws );

  up( ps );

}  // _up_pddev



static __mbg_inline
void _down( struct semaphore *ps, const char *sem_name,
            const char *fnc_name, void *p_id_struc )
{
  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: going to get %s",
                p_id_struc, fnc_name, sem_name );

  down( ps );

  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: got %s",
                p_id_struc, fnc_name, sem_name );
}  // _down



static __mbg_inline
int _down_interruptible( struct semaphore *ps, const char *sem_name,
                         const char *fnc_name, void *p_id_struc )
{
  int retval;

  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: going to get %s",
                p_id_struc, fnc_name, sem_name );

  retval = down_interruptible( ps );

  if ( retval < 0 )
    _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: interrupted when waiting for %s",
                  p_id_struc, fnc_name, sem_name );
  else
    _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: got %s",
                  p_id_struc, fnc_name, sem_name );

  return retval;

}  // _down_interruptible



static __mbg_inline
int _down_trylock( struct semaphore *ps, const char *sem_name,
                   const char *fnc_name, void *p_id_struc )
{
  int retval;

  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: trying to get %s",
                p_id_struc, fnc_name, sem_name );

  retval = down_trylock( ps );

  if ( retval < 0 )
    _mbgddmsg_3( DEBUG_SEM, MBG_LOG_WARN, "%p %s: failed to get %s",
                  p_id_struc, fnc_name, sem_name );
  else
    _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: got %s",
                  p_id_struc, fnc_name, sem_name );

  return retval;

}  // _down_trylock



static __mbg_inline
void _up( struct semaphore *ps, const char *sem_name,
          const char *fnc_name,  void *p_id_struc )
{
  _mbgddmsg_3( DEBUG_SEM, MBG_LOG_INFO, "%p %s: releasing %s",
                p_id_struc, fnc_name, sem_name );

  up( ps );

}  // _up

#else

  #define _sema_init_pddev( _s, _n, _sn, _fn, _fp )       sema_init( _s, _n )
  #define _down_interruptible_pddev( _s, _sn, _fn, _fp )  down_interruptible( _s )
  #define _up_pddev( _s, _sn, _fn, _fp )                  up( _s )
  #define _down( _s, _sn, _fn, _fp )                      down( _s )
  #define _down_interruptible( _s, _sn, _fn, _fp )        down_interruptible( _s )
  #define _down_trylock( _s, _sn, _fn, _fp )              down_trylock( _s )
  #define _up( _s, _sn, _fn, _fp )                        up( _s )

#endif



static __mbg_inline
void set_dev_connected( PCPS_DDEV *pddev, int state )
{
  _mbgddmsg_3( DEBUG, MBG_LOG_INFO, "setting dev " MBG_DEV_NAME_FMT " connected state to %i",
                _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ), state );
  atomic_set( &pddev->connected, state );

}  // set_dev_connected



static __mbg_inline
int get_dev_connected( PCPS_DDEV *pddev )
{
  return atomic_read( &pddev->connected );

}  // get_dev_connected



static __mbg_inline /*HDR*/
int mbgdrvr_get_pddev( PCPS_DDEV **ppddev, struct file *filp, const char *info )
{
  PCPS_DDEV *pddev = NULL;
  int ret_val = 0;

  if ( _down_interruptible( &sem_fops, "sem_fops", info, filp ) < 0 )
  {
    ret_val = -ERESTARTSYS;
    goto out;
  }

  pddev = *( (PCPS_DDEV **) filp->private_data );

  if ( pddev == NULL )
  {
    _mbgddmsg_2( DEBUG, MBG_LOG_ERR, "%p %s called with NULL device", filp, info );
    ret_val = -ENODEV;
    goto out_up_sem_fops;
  }

  if ( !get_dev_connected( pddev ) )
  {
    _mbgddmsg_4( DEBUG, MBG_LOG_WARN, "%p %s called for disconnected dev " MBG_DEV_NAME_FMT,
                 filp, info, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );
    ret_val = -ENODEV;
    goto out_up_sem_fops;
  }

  _mbgddmsg_4( DEBUG, MBG_LOG_INFO, "%p %s called, dev: " MBG_DEV_NAME_FMT,
                filp, info, _pcps_ddev_type_name( pddev ), _pcps_ddev_sernum( pddev ) );

out_up_sem_fops:
  _up( &sem_fops, "sem_fops", info, filp );

out:
  *ppddev = pddev;

  return ret_val;

}  // mbgdrvr_get_pddev



