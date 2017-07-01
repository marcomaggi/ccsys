/*
  Part of: CCSys
  Contents: POSIX system calls wrappers, file system functions
  Date: Wed Feb 15, 2017

  Abstract



  Copyright (C) 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#ifdef HAVE_STRING_H
#  include <string.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif
#ifdef HAVE_SYS_UIO_H
#  include <sys/uio.h>
#endif
#ifdef HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#endif


/** --------------------------------------------------------------------
 ** Accessing files.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_OPEN
ccsys_fd_t
ccsys_open (cce_location_t * L, const char *filename, int flags, ccsys_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = open(filename, flags, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .fd = rv };
    return fd;
  }
}
#endif

#ifdef HAVE_OPENAT
ccsys_fd_t
ccsys_openat (cce_location_t * L, ccsys_fd_t dirfd, const char *filename, int flags, ccsys_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = openat(dirfd.fd, filename, flags, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .fd = rv };
    return fd;
  }
}
#endif

#ifdef HAVE_CLOSE
void
ccsys_close (cce_location_t * L, ccsys_fd_t filedes)
{
  int	rv;
  errno = 0;
  rv = close(filedes.fd);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_READ
size_t
ccsys_read (cce_location_t * L, ccsys_fd_t filedes, void * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = read(filedes.fd, buffer, size);
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
  rv = pread(filedes.fd, buffer, size, offset.data);
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
  rv = write(filedes.fd, buffer, size);
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
  rv = pwrite(filedes.fd, buffer, size, offset.data);
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
ccsys_lseek (cce_location_t * L, ccsys_fd_t filedes, ccsys_off_t offset, int whence)
{
  off_t		rv;
  errno = 0;
  rv = lseek (filedes.fd, offset.data, whence);
  if (rv >= 0) {
    ccsys_off_t	off = { .data = rv };
    return off;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** File system: scatter-gather input/output.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_READV
size_t
ccsys_readv (cce_location_t * L, ccsys_fd_t filedes, const ccsys_iovec_t * _vector, int count)
{
  const struct iovec *	vector = (const struct iovec *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = readv(filedes.fd, vector, count);
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
ccsys_writev (cce_location_t * L, ccsys_fd_t filedes, const ccsys_iovec_t * _vector, int count)
{
  const struct iovec *	vector = (const struct iovec *)_vector;
  ssize_t		rv;
  errno = 0;
  rv = writev(filedes.fd, vector, count);
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
 ** Reading directory entries.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_OPENDIR
ccsys_dir_t *
ccsys_opendir (cce_location_t * L, const char * pathname)
{
  DIR *	rv;
  errno = 0;
  rv = opendir(pathname);
  if (NULL != rv) {
    return (ccsys_dir_t *)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FDOPENDIR
ccsys_dir_t *
ccsys_fdopendir (cce_location_t * L, ccsys_fd_t dirfd)
{
  DIR *	rv;
  errno = 0;
  rv = fdopendir(dirfd.fd);
  if (NULL != rv) {
    return (ccsys_dir_t *)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_READDIR
ccsys_dirent_t *
ccsys_readdir (cce_location_t * L, ccsys_dir_t * _dirstream)
{
  DIR *			dirstream = (DIR *) _dirstream;
  struct dirent *	rv;
  errno = 0;
  rv = readdir(dirstream);
  if (NULL != rv) {
    return (ccsys_dirent_t *)rv;
  } else if (0 == errno) {
    return NULL;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_CLOSEDIR
void
ccsys_closedir (cce_location_t * L, ccsys_dir_t * _dirstream)
{
  DIR *	dirstream = (DIR *) _dirstream;
  int	rv;
  errno = 0;
  rv = closedir(dirstream);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: file descriptor.
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
  H->filedes	= filedes.fd;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_filedes_init (cce_location_t * L, cce_handler_t * H, ccsys_fd_t filedes)
{
  H->function	= cce_handler_filedes_function;
  H->filedes	= filedes.fd;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: pipe descriptors.
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
ccsys_cleanup_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, int pipedes[2])
{
  H->function	= cce_handler_pipedes_function;
  H->pipedes[0]	= pipedes[0];
  H->pipedes[1]	= pipedes[1];
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, int pipedes[2])
{
  H->function	= cce_handler_pipedes_function;
  H->pipedes[0]	= pipedes[0];
  H->pipedes[1]	= pipedes[1];
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: temporary files.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_tmpfile_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_REMOVE
  remove(H->pathname);
  free(H->pathname);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpfile_function;
  H->pathname	= ptr;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpfile_function;
  H->pathname	= ptr;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: temporary directories.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_tmpdir_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_RMDIR
  rmdir(H->pathname);
  free(H->pathname);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpdir_function;
  H->pathname	= ptr;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpdir_function;
  H->pathname	= ptr;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: directory streams.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_dirstream_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_CLOSEDIR
  DIR *		dirstream = H->pointer;
  closedir(dirstream);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_dirstream_init (cce_location_t * L, cce_handler_t * H, ccsys_dir_t * dirstream)
{
  H->function	= cce_handler_dirstream_function;
  H->pointer	= dirstream;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_dirstream_init (cce_location_t * L, cce_handler_t * H, ccsys_dir_t * dirstream)
{
  H->function	= cce_handler_dirstream_function;
  H->pointer	= dirstream;
  cce_register_error_handler(L, H);
}

/* end of file */
