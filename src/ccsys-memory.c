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
ccsys_mmap (cce_location_t * L, void * address, size_t length,
	    ccsys_mmap_prot_t protect, ccsys_mmap_flags_t flags,
	    ccsys_fd_t filedes, ccsys_off_t offset)
{
  void *	rv;
  errno = 0;
  rv = mmap(address, length, protect.data, flags.data, filedes.data, (off_t)(offset.data));
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
ccsys_msync (cce_location_t * L, void *address, size_t length, ccsys_msync_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = msync(address, length, flags.data);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MPROTECT
int
ccsys_mprotect (cce_location_t * L, void * addr, size_t len, ccsys_mmap_prot_t prot)
{
  int	rv;
  errno = 0;
  rv = mprotect(addr, len, prot.data);
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
ccsys_mremap (cce_location_t * L, void * old_address, size_t length, size_t new_length,
	      ccsys_mremap_flags_t flags, void * new_address)
{
  void *	rv;
  errno = 0;
  rv = mremap(old_address, length, new_length, flags.data, new_address);
  if (rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MADVISE
void
ccsys_madvise (cce_location_t * L, void * address, size_t length, ccsys_madvise_advice_t advice)
{
  int	rv;
  errno = 0;
  rv = madvise(address, length, advice.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* end of file */
