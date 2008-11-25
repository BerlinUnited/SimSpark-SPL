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
 * Implementation of stuff in cstring.h to make ron happy 
 */
#include "cstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * growth size for cstrings -- default is 8k.  use sgrowsize() to adjust. 
 */
static size_t cstring_growsize = 8192;

void sgrowsize(size_t s) {
  assert(s > 0);
  cstring_growsize = s;
}

CSTRING *snew(size_t s) {
  CSTRING *cs;

  cs = (CSTRING *)malloc(sizeof(CSTRING));
  assert(cs != NULL);
  cs->len = s;
  cs->curlen = 0;
  cs->base = (char *)calloc(sizeof(char),s);
  assert(cs->base != NULL);

  return cs;
}

CSTRING *sadd(CSTRING *s, char *a) {
  int alen;
  char *newbase;

  /* no string, so bail */
  if (s == NULL) {
    return NULL;
  }

  /* nothing to add, return s */
  if (a  == NULL) {
    return s;
  }

  alen = strlen(a);

  if (s->curlen + alen >= s->len) {
    newbase = (char *)realloc(s->base,s->len+cstring_growsize+alen);
    if (! newbase) {
	perror("realloc string");
	s->len = s->curlen = 0;
	s->base = 0;
	return 0;
    }
    s->len += cstring_growsize + alen;
    s->base = newbase;
  }

  memcpy(&s->base[s->curlen],a,alen);
  s->curlen += alen;
  s->base[s->curlen] = 0;
  return s;
}

CSTRING *saddch(CSTRING *s, char a) {
  char *newbase;

  if (s == NULL) {
    return NULL;
  }

  if (s->curlen + 1 >= s->len) {
    newbase = (char *)realloc(s->base,s->len+cstring_growsize+1);
    if (! newbase) {
	perror("realloc string");
	s->len = s->curlen = 0;
	s->base = 0;
	return 0;
    }
    s->len += cstring_growsize+1;
    s->base = newbase;
  }

  s->base[s->curlen] = a;
  s->curlen++;
  s->base[s->curlen] = 0;
  return s;
}

CSTRING *strim(CSTRING *s) {
  char *newbase;

  if (s == NULL) {
    return NULL;
  }

  /* no trimming necessary? */
  if (s->len == s->curlen+1) {
    return s;
  }

  newbase = (char *)realloc(s->base,s->curlen+1);
  if (!newbase) {
    perror("realloc string in trim");
    s->len = s->curlen = 0;
    s->base = 0;
    return NULL;
  }
  s->len = s->curlen+1;
  s->base = newbase;

  return s;
}

char *toCharPtr(CSTRING *s) {
  return s->base;
}

void sempty(CSTRING *s) {
    s->curlen = 0;
}
void sdestroy(CSTRING *s) {
  free(s->base);
  free(s);
}
