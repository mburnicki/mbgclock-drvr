
/**************************************************************************
 *
 *  $Id: mbgklist.h 1.5 2019/08/15 10:33:01 thomas-b REL_M $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Userspace implementation of Linux Kernel's list.h
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgklist.h $
 *  Revision 1.5  2019/08/15 10:33:01  thomas-b
 *  Start variable names in macros with '_'
 *  Revision 1.4  2018/09/13 05:27:28  thomas-b
 *  Added macro to get nth item of mbgklist
 *  Revision 1.3  2017/07/05 09:52:39  martin
 *  Safe loop macros added by philipp.
 *  Check if 'typeof' is supported based on the type of compiler.
 *  Reformatted code to conform to standard header file format.
 *  Updated function prototypes.
 *  Revision 1.2  2015/10/06 07:08:45  philipp
 *  Added functions to loop containers of list entries
 *  Revision 1.1  2015/09/09 10:42:27  martin
 *  Initial revision.
 *
 **************************************************************************/

#ifndef _MBGKLIST_H
#define _MBGKLIST_H

/* Other headers to be included */

#include <mbg_cof.h>


#ifdef _MBGKLIST
 #define _ext
 #define _DO_INIT
#else
 #define _ext extern
#endif


/* Start of header body */

#ifdef __cplusplus
extern "C" {
#endif


#define MBG_KLIST_INIT(name) { &(name), &(name) }

#define MBG_KLIST_DECLARE(name) \
  struct mbg_klist_head name = MBG_KLIST_INIT(name)

#define mbg_klist_for_each(head, pos) \
  for (pos = (head)->next; pos != (head); pos = pos->next)

#define mbg_klist_for_each_safe(head, pos, n) \
  for (pos = (head)->next, n = (pos)->next; \
       pos != (head); \
       pos = n, n = pos->next)

#define mbg_klist_for_each_rev(head, pos) \
  for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define mbg_klist_for_each_rev_safe(head, pos, n) \
  for (pos = (head)->prev, n = (pos)->prev; \
       pos != (head); \
       pos = n, n = pos->prev)

#define mbg_klist_nth_item(head, pos, n) \
  do { \
    unsigned _i; \
    for (pos = (head)->next, _i = 0; _i < n; pos = pos->next, ++_i); \
  } while ( 0 )

#define mbg_klist_entry(ptr, type, member) \
    mbg_container_of(ptr, type, member)

#define mbg_klist_first_entry(ptr, type, member) \
    mbg_klist_entry((ptr)->next, type, member)

#define mbg_klist_last_entry(ptr, type, member) \
    mbg_klist_entry((ptr)->prev, type, member)



#if defined( __GNUC__ ) || defined( __clang__ )  // "typeof" supported

#define mbg_klist_next_entry(pos, member) \
    mbg_klist_entry((pos)->member.next, typeof(*pos), member)

#define mbg_klist_prev_entry(pos, member) \
    mbg_klist_entry((pos)->member.prev, typeof(*pos), member)

#define mbg_klist_for_each_entry(head, pos, member) \
    for (pos = mbg_klist_first_entry(head, typeof(*pos), member); \
         &pos->member != (head); \
         pos = mbg_klist_next_entry(pos, member))

#define mbg_klist_for_each_entry_rev(head, pos, member) \
    for (pos = mbg_klist_last_entry(head, typeof(*pos), member); \
         &pos->member != (head); \
         pos = mbg_klist_prev_entry(pos, member))

#define mbg_klist_for_each_entry_safe(head, pos, n, member) \
    for (pos = mbg_klist_first_entry(head, typeof(*pos), member), \
         n = mbg_klist_next_entry(pos, member); \
         &pos->member != (head); \
         pos = n, n = mbg_klist_next_entry(pos, member))

#define mbg_klist_for_each_entry_rev_safe(head, pos, n, member) \
    for (pos = mbg_klist_last_entry(head, typeof(*pos), member), \
         n = mbg_klist_prev_entry(pos, member); \
         &pos->member != (head); \
         pos = n, n = mbg_klist_prev_entry(pos, member))

#endif



struct mbg_klist_head
{
  struct mbg_klist_head *prev;
  struct mbg_klist_head *next;
};



static __mbg_inline
void mbg_klist_init( struct mbg_klist_head *head )
{
  head->next = head;
  head->prev = head;
}


static __mbg_inline
void __mbg_klist_add_item( struct mbg_klist_head *item, struct mbg_klist_head *prev, struct mbg_klist_head *next )
{
  next->prev = item;
  item->next = next;
  item->prev = prev;
  prev->next = item;
}


static __mbg_inline
void mbg_klist_prepend_item( struct mbg_klist_head *head, struct mbg_klist_head *item )
{
  __mbg_klist_add_item( item, head, head->next );
}


static __mbg_inline
void mbg_klist_append_item( struct mbg_klist_head *head, struct mbg_klist_head *item )
{
  __mbg_klist_add_item( item, head->prev, head );
}


static __mbg_inline
void __mbg_klist_delete_item( struct mbg_klist_head *prev, struct mbg_klist_head *next )
{
  next->prev = prev;
  prev->next = next;
}


static __mbg_inline
void mbg_klist_delete_item( struct mbg_klist_head *item )
{
  __mbg_klist_delete_item( item->prev, item->next );
}


static __mbg_inline
void mbg_klist_delete_item_init( struct mbg_klist_head *item )
{
  __mbg_klist_delete_item( item->prev, item->next );
  mbg_klist_init( item );
}


static __mbg_inline
void mbg_klist_replace_item( struct mbg_klist_head *old, struct mbg_klist_head *item )
{
  item->next = old->next;
  item->next->prev = item;
  item->prev = old->prev;
  item->prev->next = item;
}


static __mbg_inline
void mbg_klist_replace_item_init( struct mbg_klist_head *old, struct mbg_klist_head *item )
{
  mbg_klist_replace_item( old, item );
  mbg_klist_init( item );
}


static __mbg_inline
void mbg_klist_move_prepend_item( struct mbg_klist_head *head, struct mbg_klist_head *item )
{
  mbg_klist_delete_item( item );
  mbg_klist_prepend_item( head, item );
}


static __mbg_inline
void mbg_klist_move_append_item( struct mbg_klist_head *head, struct mbg_klist_head *item )
{
  mbg_klist_delete_item( item );
  mbg_klist_append_item( head, item );
}


static __mbg_inline
int mbg_klist_is_first( const struct mbg_klist_head *head, const struct mbg_klist_head *item )
{
  return ( ( item->prev == head ) ? 1 : 0 );
}


static __mbg_inline
int mbg_klist_is_last( const struct mbg_klist_head *head, const struct mbg_klist_head *item )
{
  return ( ( item->next == head ) ? 1 : 0 );
}


static __mbg_inline
int mbg_klist_is_empty( const struct mbg_klist_head *head )
{
  return ( ( head->next == head ) ? 1 : 0 );
}


static __mbg_inline
void __mbg_klist_add_list( const struct mbg_klist_head *list, struct mbg_klist_head *prev, struct mbg_klist_head *next )
{
  struct mbg_klist_head *_first = list->next;
  struct mbg_klist_head *_last = list->prev;

  _first->prev = prev;
  prev->next = _first;

  _last->next = next;
  next->prev = _last;
}


static __mbg_inline
void mbg_klist_prepend_list( struct mbg_klist_head *head, const struct mbg_klist_head *list )
{
  if ( !mbg_klist_is_empty( list ) )
    __mbg_klist_add_list( list, head, head->next );
}


static __mbg_inline
void mbg_klist_append_list( struct mbg_klist_head *head, const struct mbg_klist_head *list )
{
  if ( !mbg_klist_is_empty( list ) )
    __mbg_klist_add_list( list, head->prev, head );
}


static __mbg_inline
void mbg_klist_prepend_list_init( struct mbg_klist_head *head, struct mbg_klist_head *list )
{
  if ( !mbg_klist_is_empty( list ) )
  {
    __mbg_klist_add_list( list, head, head->next );
    mbg_klist_init( list );
  }
}


static __mbg_inline
void mbg_klist_append_list_init( struct mbg_klist_head *head, struct mbg_klist_head *list )
{
  if ( !mbg_klist_is_empty( list ) )
  {
    __mbg_klist_add_list( list, head->prev, head );
    mbg_klist_init( list );
  }
}


/* ----- function prototypes begin ----- */

/* This section was generated automatically */
/* by MAKEHDR, do not remove the comments. */

 /**
 * @brief Sort a list
 *
 * @param[in]  priv  Private data, opaque to ::mbg_klist_sort, passed to @p cmp
 * @param[in]  head  The list to sort
 * @param[in]  cmp   The elements comparison function
 *
 * This function implements "merge sort", which has O(nlog(n))
 * complexity.
 *
 * The comparison function @p cmp must return a negative value if a
 * should sort before b, and a positive value if a should sort after
 * b. If a and b are equivalent, and their original relative
 * ordering is to be preserved, cmp must return 0.
 */
 void mbg_klist_sort( void *priv, struct mbg_klist_head *head, int (*cmp)( void *priv, struct mbg_klist_head *a, struct mbg_klist_head *b ) ) ;


/* ----- function prototypes end ----- */

#ifdef __cplusplus
}
#endif

/* End of header body */

#undef _ext
#undef _DO_INIT

#endif  /* _MBGKLIST_H */
