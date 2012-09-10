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
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sexp_ops.h"

/**
 * Given an s-expression, find the atom inside of it with the
 * value matchine name, and return a reference to it.  If the atom
 * doesn't occur inside start, return NULL.
 */
sexp_t *
find_sexp (char *name, sexp_t * start)
{
  sexp_t *temp;

  if (start == NULL)
    return NULL;

  if (start->ty == SEXP_LIST)
    {
      temp = find_sexp (name, start->list);
      if (temp == NULL)
    return find_sexp (name, start->next);
      else
    return temp;
    }
  else
    {
      assert(start->val != NULL);
      if (strcmp (start->val, name) == 0)
    return start;
      else
    return find_sexp (name, start->next);
    }

  return NULL;          /* shouldn't get here */
}

/**
 * Breadth first search - look at ->next before ->list when seeing list
 * elements of an expression.
 */
sexp_t *bfs_find_sexp(char *str, sexp_t *sx) {
  sexp_t *t = sx;
  sexp_t *rt;

  if (sx == NULL) return NULL;

  while (t != NULL) {
    if (t->ty == SEXP_VALUE) {
      assert(t->val != NULL);
      if (strcmp(t->val,str) == 0) {
    return t;
      }
    }

    t = t->next;
  }

  t = sx;
  while (t != NULL) {
    if (t->ty == SEXP_LIST) {
      rt = bfs_find_sexp(str,t->list);
      if (rt != NULL) return rt;
    }

    t = t->next;
  }

  return NULL;
}

/**
 * Give the length of a s-expression list.
 */
int sexp_list_length(sexp_t *sx) {
  int len = 0;
  sexp_t *t;

  if (sx == NULL) return 0;

  if (sx->ty == SEXP_VALUE) return 1;

  t = sx->list;

  while (t != NULL) {
    len++;
    t = t->next;
  }
  return len;
}

/**
 * Copy an s-expression.
 */
sexp_t *copy_sexp(sexp_t *s) {
  sexp_t *snew;

  if (s == NULL) return NULL;

  snew = sexp_t_allocate();
  assert(snew != NULL);

  snew->ty = s->ty;
  if (snew->ty == SEXP_VALUE) {
    snew->aty = s->aty;

    assert(s->val != NULL);

    /** allocate space **/
    snew->val = (char *)malloc(sizeof(char)*s->val_used);
    assert(snew->val != NULL);
    snew->val_used = snew->val_allocated = s->val_used;

    strcpy(snew->val,s->val);
    snew->list = NULL;
  } else {
    snew->list = copy_sexp(s->list);
  }

  snew->line = s->line;
  snew->next = copy_sexp(s->next);

  return snew;
}

/**
 * Cons: Concatenate two s-expressions together, without references to the
 * originals.
 */
sexp_t *cons_sexp(sexp_t *r, sexp_t *l) {
  sexp_t *cr, *cl, *t;

  cr = copy_sexp(r);
  if (cr->ty == SEXP_VALUE) {
    fprintf(stderr,"Cannot cons non-lists.\n");
    destroy_sexp(cr);
    return NULL;
  } else {
    t = cr->list;
    while (t != NULL && t->next != NULL) t = t->next;
  }

  cl = copy_sexp(l);

  if (cl->ty == SEXP_LIST) {
    if (t != NULL && cl != NULL) {
      t->next = cl->list;
      /* free(cl); */ /* memory leak fix: SMJ, 4/24/2002 */
      sexp_t_deallocate(cl);
    }
  } else {
    fprintf(stderr,"Cannot cons non-lists.\n");
    destroy_sexp(cr);
    destroy_sexp(cl);
    return NULL;
  }

  return cr;
}

/**
 * car: similar to head, except this is a copy and not just a reference.
 */
sexp_t *car_sexp(sexp_t *s) {
    sexp_t *cr, *ocr;

    /* really dumb - calling on null */
    if (s == NULL) {
        fprintf(stderr,"car called on null sexpr.\n");
        return NULL;
    }

    /* less dumb - calling on an atom */
    if (s->ty == SEXP_VALUE) {
        fprintf(stderr,"car called on an atom.\n");
        return NULL;
    }

        /*  ocr = (sexp_t *)malloc(sizeof(sexp_t));*/
        ocr = sexp_t_allocate();
    assert(ocr != NULL);
    ocr->ty = SEXP_LIST;
    ocr->next = NULL;

    /* allocate the new sexp_t */
    /* cr = (sexp_t *)malloc(sizeof(sexp_t)); */
        cr = sexp_t_allocate();
    assert(cr != NULL);
    ocr->list = cr;

    /* copy the head of the sexpr */
    if (s->list->ty == SEXP_VALUE) {
        cr->ty = SEXP_VALUE;
                assert(s->list->val != NULL);
        strcpy(cr->val,s->list->val);
        cr->next = cr->list = NULL;
    } else {
        cr->ty = SEXP_LIST;
        cr->next = NULL;
        cr->list = copy_sexp(s->list->list);
    }

    return ocr;
}

/**
 * cdr: similar to tail, except this is a copy and not just a reference.
 */
sexp_t *cdr_sexp(sexp_t *s) {
    sexp_t *cd;

    /* really dumb */
    if (s == NULL) {
        fprintf(stderr,"cdr called on null.\n");
        return NULL;
    }

    /* less dumb */
    if (s->ty != SEXP_LIST) {
        fprintf(stderr,"cdr called on atom.\n");
        return NULL;
    }

    /* cd = (sexp_t *)malloc(sizeof(sexp_t)); */
        cd = sexp_t_allocate();
    assert(cd != NULL);

    cd->ty = SEXP_LIST;
    cd->next = NULL;
    cd->list = copy_sexp(s->list->next);
    cd->line = s->line;
    return cd;
}

