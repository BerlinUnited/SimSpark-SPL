/* ACL:license */
/*
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
*/
/* ACL:license */

/**
 * cstring.h : c string library to make Ron happy.  Wrapper around plain
 * C strings that handles automatically growing the string as data is
 * concattenated to the end.  (note: this is an improved version of cstring
 * from supermon.  Migrate it into that library eventually... )
 *
 * -matt sottile
 */
#ifndef __CSTRING_H__
#define __CSTRING_H__

#include <stdlib.h>

/**
 * Structure wrapping the character pointer and size counters (allocated vs.
 * actual used).
 */
typedef struct __cstring {
  /**
   * Base address of the string.
   */
  char *base; 

  /**
   * Size of the memory allocated and pointed to by the base pointer.
   */
  size_t len;

  /**
   * Current size of the string stored in the buffer.  len >= curlen
   * always, and when len < curlen would be true after a concat operation,
   * we realloc bigger space to keep len >= curlen.
   */
  size_t curlen; 
} CSTRING;

/**
 * Set the growth size
 */
void sgrowsize(size_t s);

/**
 * Allocate a new CSTRING of the given size.
 */
CSTRING *snew(size_t s);

/**
 * Concatenate the second argument to the CSTRING passed in the first.
 * The second argument must be a pointer to a null terminated string.
 */
CSTRING *sadd(CSTRING *s, char *a);

/**
 * Append a character to the end of the CSTRING
 */
CSTRING *saddch(CSTRING *s, char a);

/**
 * Trim the allocated memory to precisely the string length plus one char
 * to hold the null terminator
 */
CSTRING *strim(CSTRING *s);

/**
 * Return the base pointer of the CSTRING.
 */
char *toCharPtr(CSTRING *s);

/**
 * Set the current length to zero, effectively dumping the string without
 * deallocating it so we can use it later without reallocating any memory.
 */
void sempty(CSTRING *s);

/**
 * Destroy the CSTRING struct and the data it points at.
 */
void sdestroy(CSTRING *s);

#endif /* __CSTRING_H__ */
