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
#ifndef __SEXP_OPS_H__
#define __SEXP_OPS_H__

/**
 * \file sexp_ops.h
 *
 * \brief A collection of useful operations to perform on s-expressions.
 *
 * A set of routines for operating on s-expressions.  Note that cons,
 * car, and cdr do <B>not</B> currently have any guarantee to behave in a
 * identical manner as their LISP or Scheme equivalents.  This would be
 * fairly easy to fix, but not high on the priority list currently.
 */

#include "sexp.h"

#ifdef __cplusplus
extern "C" {
#endif

  /*========*/
  /* MACROS */
  /*========*/
  
  /**
   * Return the head of a list \a s by reference, not copy.
   */
#define hd_sexp(s) ((s)->list)
  
  /**
   * Return the tail of a list \a s by reference, not copy.
   */
#define tl_sexp(s) ((s)->list->next) 
  
  /**
   * Return the element following the argument \a s.
   */
#define next_sexp(s) ((s)->next)
  
  /**
   * Reset the continuation \a c by setting the \c lastPos pointer to
   * \c NULL.
   */
#define reset_pcont(c) ((c)->lastPos = NULL)

  /**
   * Find an atom in a sexpression data structure and return a pointer to
   * it.  Return NULL if the string doesn't occur anywhere as an atom. 
   * This is a depth-first search algorithm.
   */
  sexp_t *find_sexp(char *name, sexp_t *start);
    
  /**
   * Breadth first search for s-expressions.  Depth first search will find
   * the first occurance of a string in an s-expression by basically finding
   * the earliest occurance in the string representation of the expression
   * itself.  Breadth first search will find the first occurance of a string
   * in relation to the structure of the expression itself (IE: the instance
   * with the lowest depth will be found).
   */
  sexp_t *bfs_find_sexp(char *str, sexp_t *sx);

  /**
   * Given an s-expression, determine the length of the list that it encodes.
   * A null expression has length 0.  An atom has length 1.  A list has
   * length equal to the number of sexp_t elements from the list head
   * to the end of the ->next linked list from that point.
   */
  int sexp_list_length(sexp_t *sx);
  
  /**
   * Copy an s-expression.  This is a deep copy - so the resulting s-expression
   * shares no pointers with the original.  The new one can be changed without
   * damaging the contents of the original.
   */
  sexp_t *copy_sexp(sexp_t *s);
  
  /**
   * Cons: Concatenate two s-expressions together, without references to the
   * originals.
   */
  sexp_t *cons_sexp(sexp_t *r, sexp_t *l);
  
  /**
   * car: Like hd(), but returning a copy of the head, not a reference to it.
   */
  sexp_t *car_sexp(sexp_t *s);
  
  /**
   * cdr: Like tl(), but returning a copy of the tail, not a reference to it.
   */
  sexp_t *cdr_sexp(sexp_t *s);
  
#ifdef __cplusplus
}
#endif

#endif /* __SEXP_OPS_H__ */
