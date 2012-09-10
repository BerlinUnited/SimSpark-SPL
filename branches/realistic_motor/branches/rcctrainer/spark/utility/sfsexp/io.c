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

#include <fcntl.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sexp.h"
#include <assert.h>

#ifdef WIN32
#include <io.h>
#define read _read
#endif

/**
 * initialize an io-wrapper
 */
sexp_iowrap_t *init_iowrap(int fd) {
  sexp_iowrap_t *iow;

  iow = (sexp_iowrap_t *)malloc(sizeof(sexp_iowrap_t));
  assert(iow != NULL);

  iow->cc = NULL;
  iow->fd = fd;
  iow->cnt = 0;

  return iow;
}

/**
 *
 */
void destroy_iowrap(sexp_iowrap_t *iow) {
  if (iow == NULL) return; /* idiot */

  destroy_continuation(iow->cc);
  free(iow);
}

/**
 *
 */
sexp_t *read_one_sexp(sexp_iowrap_t *iow) {
  sexp_t  *sx = NULL;

  if (iow->cnt == 0) {
    iow->cnt = read(iow->fd,iow->buf,BUFSIZ);
    if (iow->cnt == 0) return NULL;
  }

  iow->cc = cparse_sexp(iow->buf,iow->cnt,iow->cc);

  while (iow->cc->last_sexp == NULL) {
    if (iow->cc->error != 0) {
      fprintf(stderr,"ERROR\n");
      return NULL;
    }

    iow->cnt = read(iow->fd,iow->buf,BUFSIZ);

    if (iow->cnt == 0)
      return NULL;

    iow->cc = cparse_sexp(iow->buf,iow->cnt,iow->cc);
  }

  sx = iow->cc->last_sexp;
  iow->cc->last_sexp = NULL;

  return sx;
}
