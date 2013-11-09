/*********************************************************
 * Copyright (C) 1998 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 *********************************************************/

/*
 * dbllnklst.h --
 *
 *    Double linked lists
 */

#ifndef _DBLLNKLST_H_
#define _DBLLNKLST_H_

#define INCLUDE_ALLOW_MODULE
#define INCLUDE_ALLOW_USERLEVEL
#include "includeCheck.h"

#include "vm_basic_types.h"

#define DblLnkLst_OffsetOf(type, field) ((intptr_t)&((type *)0)->field)

#define DblLnkLst_Container(addr, type, field) \
   ((type *)((char *)(addr) - DblLnkLst_OffsetOf(type, field)))

#define DblLnkLst_ForEach(curr, head)                   \
      for (curr = (head)->next; curr != (head); curr = (curr)->next)

/* Safe from list element removal within loop body. */
#define DblLnkLst_ForEachSafe(curr, nextElem, head)             \
      for (curr = (head)->next, nextElem = (curr)->next;        \
           curr != (head);                                      \
           curr = nextElem, nextElem = (curr)->next)

typedef struct DblLnkLst_Links {
   struct DblLnkLst_Links *prev;
   struct DblLnkLst_Links *next;
} DblLnkLst_Links;


/*
 * Functions
 *
 * DblLnkLst_LinkFirst, DblLnkLst_LinkLast, and DblLnkLst_Swap are specific
 * to anchored lists.  The rest are for both circular and anchored lists.
 */


/*
 *----------------------------------------------------------------------
 *
 * DblLnkLst_Init --
 *
 *    Initialize a member of a doubly linked list
 *
 * Result
 *    None
 *
 * Side effects:
 *    None
 *
 *----------------------------------------------------------------------
 */

static INLINE void
DblLnkLst_Init(DblLnkLst_Links *l) // IN
{
   l->prev = l->next = l;
}


/*
 *----------------------------------------------------------------------
 *
 * DblLnkLst_Link --
 *
 *    Merge two doubly linked lists into one
 *
 *    The operation is commutative
 *    The operation is inversible (its inverse is DblLnkLst_Unlink)
 *
 * Result
 *    None
 *
 * Side effects:
 *    None
 *
 *----------------------------------------------------------------------
 */

static INLINE void
DblLnkLst_Link(DblLnkLst_Links *l1, // IN
               DblLnkLst_Links *l2) // IN
{
   DblLnkLst_Links *tmp;

   (tmp      = l1->prev)->next = l2;
   (l1->prev = l2->prev)->next = l1;
    l2->prev = tmp                 ;
}


/*
 *----------------------------------------------------------------------
 *
 * DblLnkLst_Unlink --
 *
 *    Split one doubly linked list into two
 *
 *    No check is performed: the caller must ensure that both members
 *    belong to the same doubly linked list
 *
 *    The operation is commutative
 *    The operation is inversible (its inverse is DblLnkLst_Link)
 *
 * Result
 *    None
 *
 * Side effects:
 *    None
 *
 *----------------------------------------------------------------------
 */

static INLINE void
DblLnkLst_Unlink(DblLnkLst_Links *l1, // IN
                 DblLnkLst_Links *l2) // IN
{
   DblLnkLst_Links *tmp;

   tmp       = l1->prev            ;
   (l1->prev = l2->prev)->next = l1;
   (l2->prev = tmp     )->next = l2;
}


/*
 *----------------------------------------------------------------------
 *
 * DblLnkLst_Unlink1 --
 *
 *    Unlink an element from its list.
 *
 * Result
 *    None
 *
 * Side effects:
 *    None
 *
 *----------------------------------------------------------------------
 */

static INLINE void
DblLnkLst_Unlink1(DblLnkLst_Links *l) // IN
{
   DblLnkLst_Unlink(l, l->next);
}


/*
 *----------------------------------------------------------------------------
 *
 * DblLnkLst_IsLinked --
 *
 *    Determines whether an element is linked with any other elements.
 *
 * Results:
 *    TRUE if link is linked, FALSE otherwise.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------------
 */

static INLINE Bool
DblLnkLst_IsLinked(DblLnkLst_Links const *l) // IN
{
   /*
    * A DblLnkLst_Links is either linked to itself (not linked) or linked to
    * other elements in a list (linked).
    */
   return l->prev != l;
}


/*
 *----------------------------------------------------------------------
 *
 * DblLnkLst_LinkFirst --
 *
 *    Insert 'l' at the beginning of the list anchored at 'head'
 *
 * Result
 *    None
 *
 * Side effects:
 *    None
 *
 *----------------------------------------------------------------------
 */

static INLINE void
DblLnkLst_LinkFirst(DblLnkLst_Links *head, // IN
                    DblLnkLst_Links *l)    // IN
{
   DblLnkLst_Link(head->next, l);
}


/*
 *----------------------------------------------------------------------
 *
 * DblLnkLst_LinkLast --
 *
 *    Insert 'l' at the end of the list anchored at 'head'
 *
 * Result
 *    None
 *
 * Side effects:
 *    None
 *
 *----------------------------------------------------------------------
 */

static INLINE void
DblLnkLst_LinkLast(DblLnkLst_Links *head, // IN
                   DblLnkLst_Links *l)    // IN
{
   DblLnkLst_Link(head, l);
}


/*
 *----------------------------------------------------------------------
 *
 * DblLnkLst_Swap --
 *
 *    Swap all entries between the list anchored at 'head1' and the list
 *    anchored at 'head2'.
 *
 *    The operation is commutative
 *    The operation is inversible (its inverse is itself)
 *
 * Result
 *    None
 *
 * Side effects:
 *    None
 *
 *----------------------------------------------------------------------
 */

static INLINE void
DblLnkLst_Swap(DblLnkLst_Links *head1,  // IN/OUT
               DblLnkLst_Links *head2)  // IN/OUT
{
   DblLnkLst_Links const tmp = *head1;

   if (DblLnkLst_IsLinked(head2)) {
      (head1->prev = head2->prev)->next = head1;
      (head1->next = head2->next)->prev = head1;
   } else {
      DblLnkLst_Init(head1);
   }

   if (tmp.prev != head1) {
      (head2->prev = tmp.prev)->next = head2;
      (head2->next = tmp.next)->prev = head2;
   } else {
      DblLnkLst_Init(head2);
   }
}


#endif /* _DBLLNKLST_H_ */
