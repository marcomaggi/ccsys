/*
  Part of: CCSys
  Contents: POSIX system calls wrappers, file system functions
  Date: Wed Feb 15, 2017

  Abstract



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
#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#endif
#ifdef HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
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
 ** File system: reading directory entries.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_OPENDIR
ccsys_dir_t *
ccsys_opendir (cce_location_t * L, char const * pathname)
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
  rv = fdopendir(dirfd.data);
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
ccsys_chdir (cce_location_t * L, char const * pathname)
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
ccsys_fchdir (cce_location_t * L, ccsys_fd_t dirfd)
{
  int	rv;
  errno = 0;
  rv = fchdir(dirfd.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_STAT
void
ccsys_stat (cce_location_t * L, char const * pathname, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
  errno = 0;
  rv = stat(pathname, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FSTAT
void
ccsys_fstat (cce_location_t * L, ccsys_fd_t fd, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
  errno = 0;
  rv = fstat(fd.data, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LSTAT
void
ccsys_lstat (cce_location_t * L, char const * pathname, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
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
ccsys_mkdir (cce_location_t * L, char const * pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkdir(pathname, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_RMDIR
void
ccsys_rmdir (cce_location_t * L, char const * pathname)
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
ccsys_symlinkat (cce_location_t * L, const char * oldname, ccsys_fd_t newdirfd, const char * newname)
{
  int	rv;
  errno = 0;
  rv = symlinkat(oldname, newdirfd.data, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_READLINK
size_t
ccsys_readlink (cce_location_t * L, char const * pathname, char * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = readlink(pathname, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_READLINKAT
size_t
ccsys_readlinkat (cce_location_t * L, ccsys_fd_t dirfd, char const * pathname, char * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = readlinkat(dirfd.data, pathname, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_REALPATH
char *
ccsys_realpath (cce_location_t * L, char const * pathname, char * resolved_path)
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
ccsys_unlink (cce_location_t * L, char const * pathname)
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
ccsys_unlinkat (cce_location_t * L, ccsys_fd_t dirfd, char const * pathname, int flags)
{
  int	rv;
  errno = 0;
  rv = unlinkat(dirfd.data, pathname, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_REMOVE
void
ccsys_remove (cce_location_t * L, char const * pathname)
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
ccsys_chown (cce_location_t * L, char const * pathname, ccsys_uid_t owner, ccsys_gid_t group)
{
  int	rv;
  errno = 0;
  rv = chown(pathname, owner.data, group.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHOWN
void
ccsys_fchown (cce_location_t * L, ccsys_fd_t filedes, ccsys_uid_t owner, ccsys_gid_t group)
{
  int	rv;
  errno = 0;
  rv = fchown(filedes.data, owner.data, group.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LCHOWN
void
ccsys_lchown (cce_location_t * L, char const * pathname, ccsys_uid_t owner, ccsys_gid_t group)
{
  int	rv;
  errno = 0;
  rv = lchown(pathname, owner.data, group.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHOWNAT
void
ccsys_fchownat (cce_location_t * L, ccsys_fd_t dirfd, char const * pathname, ccsys_uid_t owner, ccsys_gid_t group, int flags)
{
  int	rv;
  errno = 0;
  rv = fchownat(dirfd.data, pathname, owner.data, group.data, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_CHMOD
void
ccsys_chmod (cce_location_t * L, char const * pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = chmod(pathname, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHMOD
void
ccsys_fchmod (cce_location_t * L, ccsys_fd_t filedes, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = fchmod(filedes.data, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHMODAT
void
ccsys_fchmodat (cce_location_t * L, ccsys_fd_t dirfd, char const * pathname, ccsys_open_mode_t mode, int flags)
{
  int	rv;
  errno = 0;
  rv = fchmodat(dirfd.data, pathname, mode.data, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_ACCESS
int
ccsys_access (cce_location_t * L, char const * pathname, int how)
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
ccsys_faccessat (cce_location_t * L, ccsys_fd_t dirfd, char const * pathname, int how, int flags)
{
  int	rv;
  errno = 0;
  rv = faccessat(dirfd.data, pathname, how, flags);
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
ccsys_utime (cce_location_t * L, char const * pathname, const ccsys_utimbuf_t * _times)
{
  struct utimbuf *	times = (struct utimbuf *)_times;
  int			rv;
  errno = 0;
  rv = utime(pathname, times);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_UTIMES
void
ccsys_utimes (cce_location_t * L, char const * pathname, const ccsys_timeval_t TVP[2])
{
  int	rv;
  errno = 0;
  rv = utimes(pathname, (const struct timeval *)TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_LUTIMES) && (! (defined CCSYS_ON_DARWIN)))
void
ccsys_lutimes (cce_location_t * L, char const * pathname, const ccsys_timeval_t TVP[2])
{
  int	rv;
  errno = 0;
  rv = lutimes(pathname, (const struct timeval *)TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_FUTIMES) && (! (defined CCSYS_ON_DARWIN)))
void
ccsys_futimes (cce_location_t * L, ccsys_fd_t filedes, const ccsys_timeval_t TVP[2])
{
  int	rv;
  errno = 0;
  rv = futimes(filedes.data, (const struct timeval *)TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_TRUNCATE
void
ccsys_truncate (cce_location_t * L, char const * pathname, ccsys_off_t length)
{
  int	rv;
  errno = 0;
  rv = truncate(pathname, (off_t)(length.data));
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FTRUNCATE
void
ccsys_ftruncate (cce_location_t * L, ccsys_fd_t filedes, ccsys_off_t length)
{
  int	rv;
  errno = 0;
  rv = ftruncate(filedes.data, (off_t)(length.data));
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
ccsys_cleanup_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
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
ccsys_error_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
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
ccsys_cleanup_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
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
ccsys_error_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
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
