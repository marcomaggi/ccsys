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
#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#endif
#ifdef HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
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
#ifdef HAVE_UTIME_H
#  include <utime.h>
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
 ** File descriptor operations.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_DUP
int
ccsys_dup (cce_location_t * L, int old)
{
  int	rv;
  errno = 0;
  rv = dup(old);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_DUP2
int
ccsys_dup2 (cce_location_t * L, int old, int new)
{
  int	rv;
  errno = 0;
  rv = dup2(old, new);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PIPE
void
ccsys_pipe (cce_location_t * L, int pipefd[2])
{
  int	rv;
  errno = 0;
  rv = pipe(pipefd);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MKFIFO
void
ccsys_mkfifo (cce_location_t * L, const char * pathname, mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkfifo(pathname, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** File system operations.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETCWD
void
ccsys_getcwd (cce_location_t * L, char * buffer, size_t size)
{
  char *	rv;
  errno = 0;
  rv = getcwd(buffer, size);
  if (NULL == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_CHDIR
void
ccsys_chdir (cce_location_t * L, const char * pathname)
{
  int	rv;
  errno = 0;
  rv = chdir(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHDIR
void
ccsys_fchdir (cce_location_t * L, int dirfd)
{
  int	rv;
  errno = 0;
  rv = fchdir(dirfd);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_STAT
void
ccsys_stat (cce_location_t * L, const char * pathname, struct stat * buf)
{
  int	rv;
  errno = 0;
  rv = stat(pathname, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FSTAT
void
ccsys_fstat (cce_location_t * L, int fd, struct stat * buf)
{
  int	rv;
  errno = 0;
  rv = fstat(fd, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LSTAT
void
ccsys_lstat (cce_location_t * L, const char * pathname, struct stat * buf)
{
  int	rv;
  errno = 0;
  rv = lstat(pathname, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_MKDIR
void
ccsys_mkdir (cce_location_t * L, const char * pathname, mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkdir(pathname, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_RMDIR
void
ccsys_rmdir (cce_location_t * L, const char * pathname)
{
  int	rv;
  errno = 0;
  rv = rmdir(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_LINK
void
ccsys_link (cce_location_t * L, const char * oldname, const char * newname)
{
  int	rv;
  errno = 0;
  rv = link(oldname, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LINKAT
void
ccsys_linkat (cce_location_t * L,
	      int oldfd, const char * oldname,
	      int newfd, const char * newname,
	      int flags)
{
  int	rv;
  errno = 0;
  rv = linkat(oldfd, oldname, newfd, newname, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_SYMLINK
void
ccsys_symlink (cce_location_t * L, const char * oldname, const char * newname)
{
  int	rv;
  errno = 0;
  rv = symlink(oldname, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SYMLINKAT
void
ccsys_symlinkat (cce_location_t * L, const char * oldname, int newdirfd, const char * newname)
{
  int	rv;
  errno = 0;
  rv = symlinkat(oldname, newdirfd, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_READLINK
size_t
ccsys_readlink (cce_location_t * L, const char * filename, char * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = readlink(filename, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_READLINKAT
size_t
ccsys_readlinkat (cce_location_t * L, int dirfd, const char * filename, char * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = readlinkat(dirfd, filename, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_REALPATH
char *
ccsys_realpath (cce_location_t * L, const char * pathname, char * resolved_path)
{
  char *	rv;
  errno = 0;
  rv = realpath(pathname, resolved_path);
  if (rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_UNLINK
void
ccsys_unlink (cce_location_t * L, const char * pathname)
{
  int	rv;
  errno = 0;
  rv = unlink(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_UNLINKAT
void
ccsys_unlinkat (cce_location_t * L, int dirfd, const char * pathname, int flags)
{
  int	rv;
  errno = 0;
  rv = unlinkat(dirfd, pathname, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_REMOVE
void
ccsys_remove (cce_location_t * L, const char * pathname)
{
  int	rv;
  errno = 0;
  rv = remove(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_RENAME
void
ccsys_rename (cce_location_t * L, const char * oldname, const char * newname)
{
  int	rv;
  errno = 0;
  rv = rename(oldname, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_CHOWN
void
ccsys_chown (cce_location_t * L, const char * pathname, uid_t owner, gid_t group)
{
  int	rv;
  errno = 0;
  rv = chown(pathname, owner, group);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHOWN
void
ccsys_fchown (cce_location_t * L, int filedes, uid_t owner, gid_t group)
{
  int	rv;
  errno = 0;
  rv = fchown(filedes, owner, group);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LCHOWN
void
ccsys_lchown (cce_location_t * L, const char * pathname, uid_t owner, gid_t group)
{
  int	rv;
  errno = 0;
  rv = lchown(pathname, owner, group);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHOWNAT
void
ccsys_fchownat (cce_location_t * L, int dirfd, const char * pathname, uid_t owner, gid_t group, int flags)
{
  int	rv;
  errno = 0;
  rv = fchownat(dirfd, pathname, owner, group, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_CHMOD
void
ccsys_chmod (cce_location_t * L, const char * pathname, mode_t mode)
{
  int	rv;
  errno = 0;
  rv = chmod(pathname, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHMOD
void
ccsys_fchmod (cce_location_t * L, int filedes, mode_t mode)
{
  int	rv;
  errno = 0;
  rv = fchmod(filedes, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHMODAT
void
ccsys_fchmodat (cce_location_t * L, int dirfd, const char * pathname, mode_t mode, int flags)
{
  int	rv;
  errno = 0;
  rv = fchmodat(dirfd, pathname, mode, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_ACCESS
int
ccsys_access (cce_location_t * L, const char * pathname, int how)
{
  int	rv;
  errno = 0;
  rv = access(pathname, how);
  if (0 == rv) {
    return rv;
  } else if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_FACCESSAT
int
ccsys_faccessat (cce_location_t * L, int dirfd, const char * pathname, int how, int flags)
{
  int	rv;
  errno = 0;
  rv = faccessat(dirfd, pathname, how, flags);
  if (0 == rv) {
    return rv;
  } else if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_UTIME
void
ccsys_utime (cce_location_t * L, const char * pathname, const struct utimbuf * times)
{
  int	rv;
  errno = 0;
  rv = utime(pathname, times);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_UTIMES
void
ccsys_utimes (cce_location_t * L, const char * pathname, const struct timeval TVP[2])
{
  int	rv;
  errno = 0;
  rv = utimes(pathname, TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_LUTIMES) && (! (defined CCSYS_ON_DARWIN)))
void
ccsys_lutimes (cce_location_t * L, const char * pathname, const struct timeval TVP[2])
{
  int	rv;
  errno = 0;
  rv = lutimes(pathname, TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_FUTIMES) && (! (defined CCSYS_ON_DARWIN)))
void
ccsys_futimes (cce_location_t * L, int filedes, const struct timeval TVP[2])
{
  int	rv;
  errno = 0;
  rv = futimes(filedes, TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_TRUNCATE
void
ccsys_truncate (cce_location_t * L, const char * pathname, off_t length)
{
  int	rv;
  errno = 0;
  rv = truncate(pathname, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FTRUNCATE
void
ccsys_ftruncate (cce_location_t * L, int filedes, off_t length)
{
  int	rv;
  errno = 0;
  rv = ftruncate(filedes, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Temporary files and directories.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_MKSTEMP
int
ccsys_mkstemp (cce_location_t * L, char * template)
{
  int	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkstemp(template);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_MKDTEMP
char *
ccsys_mkdtemp (cce_location_t * L, char * template)
{
  char *	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkdtemp(template);
  if (NULL != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Waiting for input/output.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SELECT
int
ccsys_select (cce_location_t * L, int nfds, fd_set * read_fds, fd_set * write_fds, fd_set * except_fds, struct timeval * timeout)
{
  int	rv;
  errno = 0;
  rv = select(nfds, read_fds, write_fds, except_fds, timeout);
  if (-1 != rv) {
    return rv;
  } else {
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
