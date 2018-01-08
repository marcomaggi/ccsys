/*
  Part of: CCSys
  Contents: memory management
  Date: Jul  1, 2017

  Abstract



  Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#ifdef HAVE_SYS_MMAN_H
#  include <sys/mman.h>
#endif


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
 ** System wrappers: locking memory pages.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_MLOCK
void
ccsys_mlock (cce_location_t * L, const void * addr, size_t len)
{
  int	rv;
  errno = 0;
  rv = mlock(addr, len);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MUNLOCK
void
ccsys_munlock (cce_location_t * L, const void * addr, size_t len)
{
  int	rv;
  errno = 0;
  rv = munlock(addr, len);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MLOCKALL
void
ccsys_mlockall (cce_location_t * L, ccsys_mlockall_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = mlockall(flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MUNLOCKALL
void
ccsys_munlockall (cce_location_t * L)
{
  int	rv;
  errno = 0;
  rv = munlockall();
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** System wrappers: memory mapping.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_MMAP
void *
ccsys_mmap (cce_location_t * L, void * address, size_t length, int protect, int flags, ccsys_fd_t filedes, ccsys_off_t offset)
{
  void *	rv;
  errno = 0;
  rv = mmap(address, length, protect, flags, filedes.data, (off_t)(offset.data));
  if (MAP_FAILED != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MUNMAP
int
ccsys_munmap (cce_location_t * L, void * addr, size_t length)
{
  int	rv;
  errno = 0;
  rv = munmap(addr, length);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MSYNC
int
ccsys_msync (cce_location_t * L, void *address, size_t length, int flags)
{
  int	rv;
  errno = 0;
  rv = msync(address, length, flags);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MPROTECT
int
ccsys_mprotect (cce_location_t * L, void * addr, size_t len, int prot)
{
  int	rv;
  errno = 0;
  rv = mprotect(addr, len, prot);
  if (-1 != 0) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_MREMAP
void *
ccsys_mremap (cce_location_t * L, void * address, size_t length, size_t new_length, int flag)
{
  void *	rv;
  errno = 0;
  rv = mremap(address, length, new_length, flag);
  if (rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MADVISE
void
ccsys_madvise (cce_location_t * L, void * address, size_t length, int advice)
{
  int	rv;
  errno = 0;
  rv = madvise(address, length, advice);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: malloc pointer.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2)))
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


/** --------------------------------------------------------------------
 ** System wrappers: guarded memory allocation.
 ** ----------------------------------------------------------------- */

void *
ccsys_malloc_guarded_cleanup (cce_location_t * L, cce_cleanup_handler_t * P_H, size_t size)
{
  void *	P = ccsys_malloc(L, size);
  cce_cleanup_handler_malloc_init(L, &(P_H->handler), P);
  return P;
}

void *
ccsys_malloc_guarded_error (cce_location_t * L, cce_error_handler_t * P_H, size_t size)
{
  void *	P = ccsys_malloc(L, size);
  cce_error_handler_malloc_init(L, &(P_H->handler), P);
  return P;
}

/* ------------------------------------------------------------------ */

void *
ccsys_realloc_guarded_cleanup (cce_location_t * L, cce_cleanup_handler_t * P_H, void * old_P, size_t newsize)
{
  if (P_H->handler.pointer == old_P) {
    void *	P = ccsys_realloc(L, old_P, newsize);
    P_H->handler.pointer = P;
    return P;
  } else {
    cce_raise(L, cce_condition_new_invalid_argument(L, __func__, 2));
  }
}

void *
ccsys_realloc_guarded_error (cce_location_t * L, cce_error_handler_t * P_H, void * old_P, size_t newsize)
{
  if (P_H->handler.pointer == old_P) {
    void *	P = ccsys_realloc(L, old_P, newsize);
    P_H->handler.pointer = P;
    return P;
  } else {
    cce_raise(L, cce_condition_new_invalid_argument(L, __func__, 2));
  }
}

/* ------------------------------------------------------------------ */

void *
ccsys_calloc_guarded_cleanup (cce_location_t * L, cce_cleanup_handler_t * P_H, size_t count, size_t eltsize)
{
  void *	P = ccsys_calloc(L, count, eltsize);
  cce_cleanup_handler_malloc_init(L, &(P_H->handler), P);
  return P;
}

void *
ccsys_calloc_guarded_error (cce_location_t * L, cce_error_handler_t * P_H, size_t count, size_t eltsize)
{
  void *	P = ccsys_calloc(L, count, eltsize);
  cce_error_handler_malloc_init(L, &(P_H->handler), P);
  return P;
}

/* end of file */
