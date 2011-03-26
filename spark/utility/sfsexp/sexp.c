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
/***
 * Matt's smaller s-expression parsing library
 *
 * Written by Matt Sottile (matt@lanl.gov), January 2002.
 ***/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sexp.h"
#include "faststack.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

sexp_mem_t *init_sexp_memory()
{
    sexp_mem_t *smem = malloc(sizeof(sexp_mem_t));
    smem->pd_cache = NULL;
    smem->sexp_t_cache = NULL;
    return smem;
}

void destroy_sexp_memory(sexp_mem_t *smem)
{
    if (smem)
        free(smem);
}

/**
 * Recursively walk an s-expression and free it.
 */
void
destroy_sexp (sexp_mem_t *smem, sexp_t * s)
{
  if (s == NULL)
    return;

  if (s->ty == SEXP_LIST)
    destroy_sexp (smem, s->list);

  if (s->ty == SEXP_VALUE && s->val != NULL)
    free(s->val);
  s->val = NULL;

  destroy_sexp (smem, s->next);

  s->next = s->list = NULL;

  sexp_t_deallocate(smem, s);
}

/**
 * Iterative method to walk sx and turn it back into the string
 * representation of the s-expression.  Fills the buffer.
 */
int
print_sexp (sexp_mem_t *smem, char *buf, int size, sexp_t * sx)
{
  int retval;
  int sz;
  char *b = buf, *tc;
  int left = size;
  int depth = 0;
  faststack_t *stack;
  stack_lvl_t *top;
  sexp_t *tdata;
  sexp_t *fakehead;

  if (sx == NULL) {
    fprintf(stderr,"print_sexp: s-expression is null.\n");
    return -1;
  }

  fakehead = sexp_t_allocate(smem);
  assert(fakehead!=NULL);

  /* duplicate the head to prevent going down a sx->next path
     that we don't really want to see. */
  fakehead->list = sx->list;
  fakehead->ty = sx->ty;
  fakehead->next = NULL; /* this is the important part of fakehead */
  fakehead->aty = sx->aty;
  fakehead->line = 0;

  if (fakehead->ty == SEXP_VALUE) {
    assert(sx->val != NULL);

    /* duplicate the head value into the fake head */
    fakehead->val = (char *)malloc(sizeof(char)*sx->val_used);
    assert(fakehead->val != NULL);

    fakehead->val_used = fakehead->val_allocated = sx->val_used;
    strcpy(fakehead->val,sx->val);
  }

  if (size < 1)
    {
      fprintf (stderr,
           "Warning: print_sexp not provided sufficient space.\n");
      return -1;
    }

  stack = make_stack ();

  push (stack, fakehead);

  while (stack->top != NULL)
    {
      top = stack->top;
      tdata = (sexp_t *) top->data;

      if (tdata == NULL)
    {
      pop (stack);

      if (depth > 0)
        {
          b[0] = ')';
          b++;
          left--;
          depth--;
          if (left == 0)
        {
          fprintf (stderr,
               "Warning: print_sexp out of buffer space.\n");
          break;
        }
        }

      if (stack->top == NULL)
        break;

      top = stack->top;
      top->data = ((sexp_t *) top->data)->next;
      if (top->data != NULL)
        {
          b[0] = ' ';
          b++;
          left--;
          if (left == 0)
        {
          fprintf (stderr,
               "Warning: print_sexp out of buffer space.\n");
          break;
        }
        }
    }
      else if (tdata->ty == SEXP_VALUE)
    {
      if (tdata->aty == SEXP_DQUOTE)
        {
          b[0] = '\"';
          b++;
          left--;
        }
      else if (tdata->aty == SEXP_SQUOTE)
        {
          b[0] = '\'';
          b++;
          left--;
        }

          if (tdata->aty != SEXP_BINARY) {
            assert(tdata->val != NULL);
            tc = tdata->val;
            /* copy value into string */
            while (tc[0] != 0 && left > 0)
              {
                /* escape characters that need escaping. */
                if ((tc[0] == '\"' || tc[0] == '\\') &&
                    tdata->aty == SEXP_DQUOTE)
                  {
                    b[0] = '\\';
                    b++;
                    left--;
                    if (left == 0) break;
                  }

                b[0] = tc[0];
                b++;
                tc++;
                left--;
                if (left == 0)
                  break;
              }
          } else {
            if (left > 3) {
              b[0] = '#'; b[1] = 'b'; b[2] = '#';
              b+=3;
              left-=3;

              if ((sz = snprintf(b,left,"%d#",tdata->binlength)) >= left) {
                left = 0;
                break;
              }
              b += sz;
              left -= sz;

              if (left < tdata->binlength) {
                left = 0;
                break;
              }
              memcpy(b,tdata->bindata,tdata->binlength);
              left -= tdata->binlength;
              b+=tdata->binlength;
              b[0] = ' ';
              left--;

            } else {
              left = 0;
              break;
            }
          }

      if (tdata->aty == SEXP_DQUOTE && left > 0)
        {
          b[0] = '\"';
          b++;
          left--;
        }

      if (left < 0)
        left = 0;
      if (left == 0)
        {
          fprintf (stderr, "Warning: print_sexp out of buffer space.\n");
          break;
        }

      top->data = ((sexp_t *) top->data)->next;

      if (top->data != NULL)
        {
          b[0] = ' ';
          b++;
          left--;
          if (left == 0)
        {
          fprintf (stderr,
               "Warning: print_sexp out of buffer space.\n");
          break;
        }
        }
    }
      else if (tdata->ty == SEXP_LIST)
    {
      depth++;
      b[0] = '(';
      b++;
      left--;
      if (left == 0)
        {
          fprintf (stderr, "Warning: print_sexp out of buffer space.\n");
          break;
        }

      push (stack, tdata->list);
    }
      else
    {
      fprintf (stderr, "ERROR: Unknown type in sexp_t.\n");
      fflush (stderr);
      return -1;
    }

    }
  while (depth != 0)
    {
      b[0] = ')';
      b++;
      left--;
      depth--;
      if (left == 0)
    {
      fprintf (stderr, "Warning: print_sexp out of buffer space.\n");
      break;
    }
    }

  if (left != 0) {
    b[0] = 0;
    retval = (size-left);
  } else {
    b--;
    b[0] = 0;
    retval = -1;
  }

  destroy_stack (stack);
  sexp_t_deallocate(smem, fakehead);

  return retval;
}

/**
 * Iterative method to walk sx and turn it back into the string
 * representation of the s-expression.  Fills the buffer.
 */
int
print_sexp_cstr (sexp_mem_t *smem, CSTRING **s, sexp_t *sx, int ss, int gs)
{
  int retval;
  char *tc;
  int depth = 0;
  faststack_t *stack;
  stack_lvl_t *top;
  sexp_t *tdata;
  sexp_t *fakehead;
  CSTRING *_s;
  char sbuf[32];
  int i;

  if (sx == NULL) {
    fprintf(stderr,"print_sexp_cstr warning: s-expression is null.\n");
    return -1;
  }

  _s = snew(ss);
  sgrowsize(gs);

  fakehead = sexp_t_allocate(smem);
  assert(fakehead!=NULL);

  /* duplicate the head to prevent going down a sx->next path
     that we don't really want to see. */
  fakehead->list = sx->list;
  fakehead->ty = sx->ty;
  fakehead->next = NULL; /* this is the important part of fakehead */
  fakehead->aty = sx->aty;

  if (fakehead->ty == SEXP_VALUE) {
    assert(sx->val != NULL);

    /* duplicate the value of the head into the fake head */
    fakehead->val = (char *)malloc(sizeof(char)*sx->val_used);
    assert(fakehead->val != NULL);

    fakehead->val_used = fakehead->val_allocated = sx->val_used;
    strcpy(fakehead->val,sx->val);
  }

  stack = make_stack ();

  push (stack, fakehead);

  while (stack->top != NULL)
    {
      top = stack->top;
      tdata = (sexp_t *) top->data;

      if (tdata == NULL)
    {
      pop (stack);

      if (depth > 0)
        {
          _s = saddch(_s, ')');
          depth--;
        }

      if (stack->top == NULL)
        break;

      top = stack->top;
      top->data = ((sexp_t *) top->data)->next;
      if (top->data != NULL)
        {
          _s = saddch(_s, ' ');
        }
    }
      else if (tdata->ty == SEXP_VALUE)
    {
      if (tdata->aty == SEXP_DQUOTE)
        {
          _s = saddch(_s,'\"');
        }
      else if (tdata->aty == SEXP_SQUOTE)
        {
          _s = saddch(_s,'\'');
        }

          if (tdata->aty == SEXP_BINARY) {
            assert(tdata->bindata != NULL);
            assert(tdata->binlength > 0);
            sprintf(sbuf,"#b#%d#",tdata->binlength);
            _s = sadd(_s,sbuf);
            for (i=0;i<tdata->binlength;i++)
              _s = saddch(_s,tdata->bindata[i]);
            _s = saddch(_s,' ');
          } else {
            assert(tdata->val != NULL);
            tc = tdata->val;

            /* copy value into string */
            while (tc[0] != 0)
              {
                /* escape characters that need escaping. */
                if ((tc[0] == '\"' ||
                     tc[0] == '\\') && tdata->aty == SEXP_DQUOTE)
                  {
                    _s = saddch(_s,'\\');
                  }

                _s = saddch(_s,tc[0]);
                tc++;
              }
          }

      if (tdata->aty == SEXP_DQUOTE)
        {
          _s = saddch(_s,'\"');
        }

      top->data = ((sexp_t *) top->data)->next;

      if (top->data != NULL)
        {
          _s = saddch(_s,' ');
        }
    }
      else if (tdata->ty == SEXP_LIST)
    {
      depth++;
      _s = saddch(_s,'(');
      push (stack, tdata->list);
    }
      else
    {
      fprintf (stderr, "ERROR: Unknown type in sexp_t.\n");
      fflush (stderr);
      return -1;
    }

    }
  while (depth != 0)
    {
      _s = saddch(_s,')');
      depth--;
    }

  *s = _s;
  retval = _s->curlen;

  destroy_stack (stack);
  sexp_t_deallocate(smem, fakehead);

  return retval;
}

/**
 * Allocate a new sexp_t element representing a list.
 */
sexp_t *new_sexp_list(sexp_mem_t *smem, sexp_t *l) {
  sexp_t *sx = sexp_t_allocate(smem);

  sx->ty = SEXP_LIST;

  sx->list = l;
  sx->next = NULL;

  sx->val = NULL;
  sx->val_used = sx->val_allocated = 0;

  return sx;
}

/**
 * allocate a new sexp_t element representing a value
 */
sexp_t *new_sexp_atom(sexp_mem_t *smem, char *buf, int bs) {
  sexp_t *sx = sexp_t_allocate(smem);

  sx->ty = SEXP_VALUE;

  sx->val = (char *)malloc(sizeof(char)*(bs+1));
  assert(sx->val != NULL);

  sx->val_used = sx->val_allocated = bs+1;

  strcpy(sx->val,buf);

  sx->list = sx->next = NULL;

  sx->line = 0;

  return sx;
}
