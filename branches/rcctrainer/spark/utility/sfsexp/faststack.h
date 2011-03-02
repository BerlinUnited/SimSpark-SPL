/**
This software and ancillary information (herein called "SOFTWARE")
called Supermon is made available under the terms described
here.  The SOFTWARE has been approved for release with associated
LA-CC Number LA-CC 99-51.

Unless otherwise indicated, this SOFTWARE has been authored by an
employee or employees of the University of California, operator of the
Los Alamos National Laboratory under Contract No.  W-7405-ENG-36 with
the U.S. Department of Energy.  The U.S. Government has rights to use,
reproduce, and distribute this SOFTWARE, and to allow others to do so.
The public may copy, distribute, prepare derivative works and publicly
display this SOFTWARE without charge, provided that this Notice and
any statement of authorship are reproduced on all copies.  Neither the
Government nor the University makes any warranty, express or implied,
or assumes any liability or responsibility for the use of this
SOFTWARE.

If SOFTWARE is modified to produce derivative works, such modified
SOFTWARE should be clearly marked, so as not to confuse it with the
version available from LANL.
**/
/** NOTICE: This software is licensed under the GNU Public License, which
    is included as LICENSE_GPL in this source distribution. **/
/** NOTE: This library is part of the supermon project, hence the name
          supermon above. **/

/**
 * \file faststack.h
 *
 * \brief Implementation of a fast stack with smart memory management.
 *
 * Author: Matt Sottile (matt@lanl.gov)
 * See LICENSE for info on licensing.
 */
#ifndef __FASTSTACK_H__
#define __FASTSTACK_H__

#ifdef _DEBUG_MALLOCS_
#include "malloc_util.h"
#endif

/**
 * Structure representing a single level in the stack.  Has a pointer to the
 * level above and below itself and a pointer to a generic blob of data
 * associated with this level.
 */
typedef struct stack_level {
  /**
   * Pointer to the level above this one.  If NULL, then this level is the
   * top of the stack.  If above is non-NULL, this level *may* be the top,
   * but all that can be guaranteed is that there are other allocated
   * but potentially unused levels above this one.
   */
  struct stack_level *above;

  /**
   * Pointer to the level below this one.  If NULL, then this level is the
   * bottom.
   */
  struct stack_level *below;

  /**
   * Pointer to some data associated with this level.  User is responsible
   * for knowing what to cast the \c void \c * pointer into.
   */
  void *data;
} stack_lvl_t;

/**
 * Wrapper around the stack levels - keeps a pointer to the current top and
 * bottom of the stack and a count of the current height.  This allows the top
 * to have non-null above pointer resulting from previously allocated stack
 * levels that may be recycled later without \c malloc overhead.
 */
typedef struct stack_wrapper {
  /**
   * The top of the stack.  If this is NULL, the stack is empty.
   */
  stack_lvl_t *top;
  
  /**
   * The bottom of the stack.  If this is NULL, the stack is empty.
   */
  stack_lvl_t *bottom;

  /**
   * The current height of the stack, in terms of allocated and used levels.
   */
  int height;
} faststack_t;

/** functions **/

/* this is for C++ */
#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Return a pointer to an empty stack structure.
   */
faststack_t *make_stack();

  /**
   * Given a stack structure, destroy it and free all of the stack levels.
   * <B>Important note</B> : This function <I>does not</I> free the data
   * pointed to from each level of the stack - the user is responsible
   * for freeing this data themselves before calling this function to
   * prevent memory leakage.
   */
void destroy_stack(faststack_t *s);

  /**
   * Given a stack, push a new level on referring to the data pointer.
   */
faststack_t *push(faststack_t *cur_stack, void *data);

  /**
   * Given a stack, pop a level off and return a pointer to that level.
   * The user is responsible for extracting the data, but the stack_lvl_t
   * structures pointed to from the level (above and below) should be left
   * alone.
   */
stack_lvl_t *pop(faststack_t *s);

/* this is for C++ */
#ifdef __cplusplus
}
#endif

/**
 * Given a stack \a s, examine the data pointer at the top.
 */
#define top_data(s) (s->top->data)

/**
 * Given a stack \a s, check to see if the stack is empty or not.  Value
 * is boolean true or false.
 */
#define empty_stack(s) (s->top == NULL)

#endif /* __FASTSTACK_H__ */
