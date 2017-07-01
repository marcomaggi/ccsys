/*
  Part of: CCSys
  Contents: memory management
  Date: Jul  1, 2017

  Abstract



  Copyright (C) 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can  redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the
  Free Software  Foundation; either version  3.0 of the License,  or (at
  your option) any later version.

  This library  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  Lesser General Public License for more details.

  You  should have  received a  copy of  the GNU  Lesser  General Public
  License along  with this library; if  not, write to  the Free Software
  Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
  USA.

*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccsys-internals.h"


/** --------------------------------------------------------------------
 ** System wrappers: memory allocation.
 ** ----------------------------------------------------------------- */

void *
ccsys_malloc (cce_location_t * L, size_t size)
{
  void *	rv;
  errno = 0;
  rv = malloc (size);
  if (NULL != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}

void *
ccsys_realloc (cce_location_t * L, void * ptr, size_t newsize)
{
  void *	rv;
  errno = 0;
  rv = realloc(ptr, newsize);
  if (NULL != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}

void *
ccsys_calloc (cce_location_t * L, size_t count, size_t eltsize)
{
  void *	rv;
  errno = 0;
  rv = calloc(count, eltsize);
  if (NULL != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: malloc pointer.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_malloc_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  free(H->pointer);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_cleanup_handler_malloc_init (cce_location_t * L, cce_handler_t * H, void * pointer)
{
  H->function	= cce_handler_malloc_function;
  H->pointer	= pointer;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_malloc_init (cce_location_t * L, cce_handler_t * H, void * pointer)
{
  H->function	= cce_handler_malloc_function;
  H->pointer	= pointer;
  cce_register_error_handler(L, H);
}

/* end of file */
