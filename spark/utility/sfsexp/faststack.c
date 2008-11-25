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
 * faststack.c : implementation of fast stack.
 *
 * matt sottile / matt@lanl.gov
 */
#include <stdlib.h>
#include <stdio.h>
#include "faststack.h"
#include "sexp.h"

/**
 * create an empty stack.
 */
faststack_t *
make_stack ()
{
  faststack_t *s;

  s = (faststack_t *) malloc (sizeof (faststack_t));
  s->top = NULL;
  s->bottom = NULL;
  s->height = 0;

  return s;
}

/**
 * free all levels of a stack
 */
void
destroy_stack (faststack_t * s)
{
  stack_lvl_t *sl;

  /* start at the bottom */
  sl = s->bottom;

  /* if bottom is null, no levels to free. */
  if (sl == NULL)
    return;

  /* go up to the top of the allocated stack */
  while (sl->above != NULL)
    sl = sl->above;

  /* until we get to the bottom (where below is null), free the data
     at each level and the level itself. */
  while (sl->below != NULL)
    {
      sl = sl->below;
      free (sl->above);
    }

  /* free the bottom level */
  free (sl);

  /* free the stack wrapper itself. */
  free (s);
}

/**
 * push a level onto the cur_stack.  reuse levels that have
 * been previously allocated, allocate a new one if none
 * are available.
 */
faststack_t *
push (faststack_t * cur_stack, void *data)
{
  stack_lvl_t *top = cur_stack->top;
  stack_lvl_t *tmp;

  /* if top isn't null, try to push above it. */
  if (top != NULL)
    {
      /* if above isn't null, set the stack top to it and set the
         data */
      if (top->above != NULL)
	{
	  top = cur_stack->top = cur_stack->top->above;
	  top->data = data;
	}
      else
	{
	  /* otherwise, allocate a new level. */
	  tmp = top->above = (stack_lvl_t *) malloc (sizeof (stack_lvl_t));
	  tmp->below = cur_stack->top;
	  tmp->above = NULL;
	  cur_stack->top = tmp;
	  tmp->data = data;
	}
    }
  else
    {
      if (cur_stack->bottom != NULL)
	{
	  cur_stack->top = cur_stack->bottom;
	  cur_stack->top->data = data;
	}
      else
	{
	  tmp = cur_stack->top =
	    (stack_lvl_t *) malloc (sizeof (stack_lvl_t));
	  cur_stack->bottom = tmp;
	  tmp->above = NULL;
	  tmp->below = NULL;
	  tmp->data = data;
	}
    }

  cur_stack->height++;

  return cur_stack;
}

/**
 * pop the top of the stack, return the stack level that was
 * popped of.
 */
stack_lvl_t *
pop (faststack_t * s)
{
  stack_lvl_t *top;
  top = s->top;

  /* if stack top isn't null, set the top pointer to the next
     level down and return the old top. */
  if (top != NULL && s->height > 0)
    {
      s->top = s->top->below;
      s->height--;
    }
  else
    {
      fprintf(stderr,"STACK: non-null top, but height < 0!\n");
    }

  return top;
}
