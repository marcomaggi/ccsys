/*
  Part of: CCSys
  Contents: POSIX system calls wrappers, input/output functions
  Date: Jan 11, 2018

  Abstract

	POSIX system calls wrappers, input/output functions.

  Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you  can redistribute it and/or modify it under
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
#ifdef HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#ifdef HAVE_SYS_SELECT_H
#  include <sys/select.h>
#endif
#ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#ifdef HAVE_SYS_UIO_H
#  include <sys/uio.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif


/** --------------------------------------------------------------------
 ** Input/output: opening files.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_OPEN
ccsys_fd_t
ccsys_open (cce_location_t * L, const char *filename, ccsys_open_flags_t flags, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = open(filename, flags.data, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .data = rv };
    return fd;
  }
}
#endif

#ifdef HAVE_OPENAT
ccsys_fd_t
ccsys_openat (cce_location_t * L, ccsys_dirfd_t dirfd, const char *filename, ccsys_open_flags_t flags, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = openat(dirfd.data, filename, flags.data, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .data = rv };
    return fd;
  }
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: closing file descriptors.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_CLOSE
void
ccsys_close (cce_location_t * L, ccsys_fd_t filedes)
{
  int	rv;
  errno = 0;
  rv = close(filedes.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: creating pipes.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_PIPE
void
ccsys_pipe (cce_location_t * L, ccsys_fd_t pipefd[2])
{
  int	rv;
  int	fd[2];
  errno = 0;
  rv = pipe(fd);
  if (-1 != rv) {
    pipefd[0].data = fd[0];
    pipefd[1].data = fd[1];
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PIPE2
void
ccsys_pipe2 (cce_location_t * L, ccsys_fd_t pipefd[2], ccsys_open_flags_t flags)
{
  int	rv;
  int	fd[2];
  errno = 0;
  rv = pipe2(fd, flags.data);
  if (-1 != rv) {
    pipefd[0].data = fd[0];
    pipefd[1].data = fd[1];
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: creating FIFOs.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_MKFIFO
void
ccsys_mkfifo (cce_location_t * L, const char * pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkfifo(pathname, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MKFIFOAT
void
ccsys_mkfifoat (cce_location_t * L, ccsys_dirfd_t dirfd, const char * pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkfifoat(dirfd.data, pathname, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: reading from and writing to file descriptors.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_READ
size_t
ccsys_read (cce_location_t * L, ccsys_fd_t filedes, void * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = read(filedes.data, buffer, size);
  if (rv >= 0) {
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PREAD
size_t
ccsys_pread (cce_location_t * L, ccsys_fd_t filedes, void * buffer, size_t size, ccsys_off_t offset)
{
  ssize_t	rv;
  errno = 0;
  rv = pread(filedes.data, buffer, size, offset.data);
  if (rv >= 0) {
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_WRITE
size_t
ccsys_write (cce_location_t * L, ccsys_fd_t filedes, const void *buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = write(filedes.data, buffer, size);
  if (rv >= 0) {
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PWRITE
size_t
ccsys_pwrite (cce_location_t * L, ccsys_fd_t filedes, const void *buffer, size_t size, ccsys_off_t offset)
{
  ssize_t	rv;
  errno = 0;
  rv = pwrite(filedes.data, buffer, size, offset.data);
  if (rv >= 0) {
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_LSEEK
ccsys_off_t
ccsys_lseek (cce_location_t * L, ccsys_fd_t filedes, ccsys_off_t offset, ccsys_whence_t whence)
{
  off_t		rv;
  errno = 0;
  rv = lseek (filedes.data, offset.data, whence.data);
  if (rv >= 0) {
    ccsys_off_t	off = { .data = rv };
    return off;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: scatter-gather input/output.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_READV
size_t
ccsys_readv (cce_location_t * L, ccsys_fd_t filedes, ccsys_iovec_t const * _vector, int count)
{
  struct iovec const *	vector = (struct iovec const *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = readv(filedes.data, vector, count);
  if (rv >= 0) {
    /* We  return  a  "size_t"  because   we  have  excluded  the  error
       values. */
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_WRITEV
size_t
ccsys_writev (cce_location_t * L, ccsys_fd_t filedes, ccsys_iovec_t const * _vector, int count)
{
  struct iovec const *	vector = (struct iovec const *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = writev(filedes.data, vector, count);
  if (rv >= 0) {
    /* We  return  a  "size_t"  because   we  have  excluded  the  error
       values. */
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_PREADV
size_t
ccsys_preadv (cce_location_t * L, ccsys_fd_t filedes, ccsys_iovec_t const * _vector, int count, ccsys_off_t offset)
{
  struct iovec const *	vector = (struct iovec const *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = preadv(filedes.data, vector, count, offset.data);
  if (rv >= 0) {
    /* We  return  a  "size_t"  because   we  have  excluded  the  error
       values. */
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PWRITEV
size_t
ccsys_pwritev (cce_location_t * L, ccsys_fd_t filedes, ccsys_iovec_t const * _vector, int count, ccsys_off_t offset)
{
  struct iovec const *	vector = (struct iovec const *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = pwritev(filedes.data, vector, count, offset.data);
  if (rv >= 0) {
    /* We  return  a  "size_t"  because   we  have  excluded  the  error
       values. */
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_PREADV2
size_t
ccsys_preadv2 (cce_location_t * L, ccsys_fd_t filedes, ccsys_iovec_t const * _vector, int count,
	       ccsys_off_t offset, ccsys_scatter_gather_flags_t flags)
{
  struct iovec const *	vector = (struct iovec const *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = preadv2(filedes.data, vector, count, offset.data, flags.data);
  if (rv >= 0) {
    /* We  return  a  "size_t"  because   we  have  excluded  the  error
       values. */
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PWRITEV2
size_t
ccsys_pwritev2 (cce_location_t * L, ccsys_fd_t filedes, ccsys_iovec_t const * _vector, int count,
		ccsys_off_t offset, ccsys_scatter_gather_flags_t flags)
{
  struct iovec const *	vector = (struct iovec const *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = pwritev2(filedes.data, vector, count, offset.data, flags.data);
  if (rv >= 0) {
    /* We  return  a  "size_t"  because   we  have  excluded  the  error
       values. */
    return (size_t)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: duplicating file descriptors.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_DUP
ccsys_fd_t
ccsys_dup (cce_location_t * L, ccsys_fd_t old)
{
  ccsys_fd_t	rv;
  errno = 0;
  rv.data = dup(old.data);
  if (-1 != rv.data) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_DUP2
ccsys_fd_t
ccsys_dup2 (cce_location_t * L, ccsys_fd_t old, ccsys_fd_t new)
{
  ccsys_fd_t	rv;
  errno = 0;
  rv.data = dup2(old.data, new.data);
  if (-1 != rv.data) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_DUP3
ccsys_fd_t
ccsys_dup3 (cce_location_t * L, ccsys_fd_t old, ccsys_fd_t new, ccsys_open_flags_t flags)
{
  ccsys_fd_t	rv;
  errno = 0;
  rv.data = dup3(old.data, new.data, flags.data);
  if (-1 != rv.data) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: waiting for input/output.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SELECT
int
ccsys_select (cce_location_t * L, ccsys_fd_t nfds,
	      ccsys_fd_set_t * read_fds, ccsys_fd_set_t * write_fds, ccsys_fd_set_t * except_fds,
	      ccsys_timeval_t * timeout)
{
  struct timeval	T = ccsys_timeval_to_timeval(*timeout);
  int			rv;
  errno = 0;
  rv = select(nfds.data, (fd_set *)read_fds, (fd_set *)write_fds, (fd_set *)except_fds, &T);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PSELECT
int
ccsys_pselect (cce_location_t * L, ccsys_fd_t nfds,
	       ccsys_fd_set_t * read_fds, ccsys_fd_set_t * write_fds, ccsys_fd_set_t * except_fds,
	       ccsys_timespec_t * timeout, ccsys_sigset_t const * sigmask)
{
  struct timespec	T = ccsys_timespec_to_timespec(*timeout);
  int			rv;
  errno = 0;
  rv = pselect(nfds.data, (fd_set *)read_fds, (fd_set *)write_fds, (fd_set *)except_fds,
	       &T, (sigset_t const *)sigmask);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_SELECT
void
ccsys_fd_clr (ccsys_fd_t fd, ccsys_fd_set_t * fds)
{
  FD_CLR(fd.data, (fd_set *) fds->data);
}
#endif

#ifdef HAVE_SELECT
bool
ccsys_fd_isset (ccsys_fd_t fd, ccsys_fd_set_t * fds)
{
  return (FD_ISSET(fd.data, (fd_set *) fds->data)) ? true : false;
}
#endif

#ifdef HAVE_SELECT
void
ccsys_fd_set (ccsys_fd_t fd, ccsys_fd_set_t * fds)
{
  FD_SET(fd.data, (fd_set *) fds->data);
}
#endif

#ifdef HAVE_SELECT
void
ccsys_fd_zero (ccsys_fd_set_t * fds)
{
  FD_ZERO((fd_set *) fds->data);
}
#endif


/** --------------------------------------------------------------------
 ** Input/output: file descriptor handler.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_filedes_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  close(H->filedes);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_cleanup_handler_filedes_init (cce_location_t * L, cce_handler_t * H, ccsys_fd_t filedes)
{
  H->function	= cce_handler_filedes_function;
  H->filedes	= filedes.data;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_filedes_init (cce_location_t * L, cce_handler_t * H, ccsys_fd_t filedes)
{
  H->function	= cce_handler_filedes_function;
  H->filedes	= filedes.data;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Input/output: directory descriptor handler.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_dirfd_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  close(H->filedes);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_cleanup_handler_dirfd_init (cce_location_t * L, cce_handler_t * H, ccsys_dirfd_t dirfd)
{
  H->function	= cce_handler_dirfd_function;
  H->filedes	= dirfd.data;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_dirfd_init (cce_location_t * L, cce_handler_t * H, ccsys_dirfd_t dirfd)
{
  H->function	= cce_handler_dirfd_function;
  H->filedes	= dirfd.data;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Input/output: pipe descriptors handlers.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_pipedes_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_CLOSE
  close(H->pipedes[0]);
  close(H->pipedes[1]);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, ccsys_fd_t pipedes[2])
{
  H->function	= cce_handler_pipedes_function;
  H->pipedes[0]	= pipedes[0].data;
  H->pipedes[1]	= pipedes[1].data;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, ccsys_fd_t pipedes[2])
{
  H->function	= cce_handler_pipedes_function;
  H->pipedes[0]	= pipedes[0].data;
  H->pipedes[1]	= pipedes[1].data;
  cce_register_error_handler(L, H);
}

/* end of file */
