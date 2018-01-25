/*
  Part of: CCSys
  Contents: POSIX system calls wrappers, file system functions
  Date: Wed Feb 15, 2017

  Abstract

	POSIX system calls wrappers, file system functions.

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
 ** File system: creating files.
 ** ----------------------------------------------------------------- */

#if ((defined HAVE_OPEN) && (defined HAVE_CLOSE))
void
ccsys_touch (cce_destination_t L, const char *filename, ccsys_open_flags_t flags, ccsys_open_mode_t mode)
{
  ccsys_fd_t	fd;

  fd = ccsys_open(L, filename, flags, mode);
  ccsys_close(L, fd);
}
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
ccsys_fdopendir (cce_location_t * L, ccsys_dirfd_t dirfd)
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

#ifdef HAVE_DIRFD
ccsys_dirfd_t
ccsys_dirfd (cce_location_t * L, ccsys_dir_t * _dirstream)
{
  DIR *		dirstream = (DIR *) _dirstream;
  int		rv;
  errno = 0;
  rv = dirfd(dirstream);
  if (0 <= rv) {
    ccsys_dirfd_t	dirfd = { .data = rv };
    if (0) { fprintf(stderr, "%s: %d\n", __func__, dirfd.data); }
    return dirfd;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_CLOSEDIR
__attribute__((nonnull(1,2)))
static void
cce_handler_dirstream_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  DIR *		dirstream = H->pointer;
  closedir(dirstream);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
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
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_DIRENT_D_NAME)
char const *
ccsys_ref_dirent_d_name (ccsys_dirent_t const * S)
{
  CCSYS_PC(struct dirent const, D, S);
  return D->d_name;
}
#endif


/** --------------------------------------------------------------------
 ** File system: reading attributes.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_STAT
bool
ccsys_stat (cce_location_t * L, char const * pathname, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
  errno = 0;
  rv = stat(pathname, buf);
  if (0 == rv) {
    return true;
  } else {
    if (-1 == rv) {
      if ((ENOENT == errno) || (ENOTDIR == errno)) {
	/* The pathname does not exist. */
	return false;
      } else {
	cce_raise(L, cce_condition_new_errno_clear());
      }
    } else {
      cce_raise(L, cce_condition_new_logic_error());
    }
  }
}
#endif

#ifdef HAVE_FSTAT
bool
ccsys_rawfd_fstat (cce_location_t * L, int rawfd, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
  errno = 0;
  rv = fstat(rawfd, buf);
  if (0 == rv) {
    return true;
  } else {
    if (-1 == rv) {
      if ((ENOENT == errno) || (ENOTDIR == errno)) {
	/* The pathname does not exist. */
	return false;
      } else {
	cce_raise(L, cce_condition_new_errno_clear());
      }
    } else {
      cce_raise(L, cce_condition_new_logic_error());
    }
  }
}
#endif

#ifdef HAVE_FSTATAT
bool
ccsys_fstatat (cce_destination_t L, ccsys_dirfd_t dirfd, char const * pathname, ccsys_stat_t * _buf, ccsys_fstatat_flags_t flags)
{
  CCSYS_PC(struct stat, buf, _buf);
  int		rv;
  errno = 0;
  rv = fstatat(dirfd.data, pathname, buf, flags.data);
  if (0 == rv) {
    return true;
  } else {
    if (-1 == rv) {
      if ((ENOENT == errno) || (ENOTDIR == errno)) {
	/* The pathname does not exist. */
	return false;
      } else {
	cce_raise(L, cce_condition_new_errno_clear());
      }
    } else {
      cce_raise(L, cce_condition_new_logic_error());
    }
  }
}
#endif

#ifdef HAVE_LSTAT
bool
ccsys_lstat (cce_location_t * L, char const * pathname, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
  errno = 0;
  rv = lstat(pathname, buf);
  if (0 == rv) {
    return true;
  } else {
    if (-1 == rv) {
      if ((ENOENT == errno) || (ENOTDIR == errno)) {
	/* The pathname does not exist. */
	return false;
      } else {
	cce_raise(L, cce_condition_new_errno_clear());
      }
    } else {
      cce_raise(L, cce_condition_new_logic_error());
    }
  }
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_DEV)
ccsys_dev_t
ccsys_ref_stat_st_dev (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_dev_t	F = { .data = D->st_dev };
  return F;
}

void
ccsys_set_stat_st_dev (ccsys_stat_t * S, ccsys_dev_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_dev = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_INO)
ccsys_ino_t
ccsys_ref_stat_st_ino (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_ino_t	F = { .data = D->st_ino };
  return F;
}

void
ccsys_set_stat_st_ino (ccsys_stat_t * S, ccsys_ino_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_ino = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MODE)
ccsys_stat_mode_t
ccsys_ref_stat_st_mode (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_stat_mode_t	F = { .data = D->st_mode };
  return F;
}

void
ccsys_set_stat_st_mode (ccsys_stat_t * S, ccsys_stat_mode_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_mode = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_NLINK)
ccsys_nlink_t
ccsys_ref_stat_st_nlink (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_nlink_t	F = { .data = D->st_nlink };
  return F;
}

void
ccsys_set_stat_st_nlink (ccsys_stat_t * S, ccsys_nlink_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_nlink = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_UID)
ccsys_uid_t
ccsys_ref_stat_st_uid (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_uid_t	F = { .data = D->st_uid };
  return F;
}

void
ccsys_set_stat_st_uid (ccsys_stat_t * S, ccsys_uid_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_uid = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_GID)
ccsys_gid_t
ccsys_ref_stat_st_gid (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_gid_t	F = { .data = D->st_gid };
  return F;
}

void
ccsys_set_stat_st_gid (ccsys_stat_t * S, ccsys_gid_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_gid = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_RDEV)
ccsys_dev_t
ccsys_ref_stat_st_rdev (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_dev_t	F = { .data = D->st_rdev };
  return F;
}

void
ccsys_set_stat_st_rdev (ccsys_stat_t * S, ccsys_dev_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_rdev = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_SIZE)
ccsys_off_t
ccsys_ref_stat_st_size (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_off_t	F = { .data = D->st_size };
  return F;
}

void
ccsys_set_stat_st_size (ccsys_stat_t * S, ccsys_off_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_size = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLKSIZE)
ccsys_blksize_t
ccsys_ref_stat_st_blksize (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_blksize_t	F = { .data = D->st_blksize };
  return F;
}

void
ccsys_set_stat_st_blksize (ccsys_stat_t * S, ccsys_blksize_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_blksize = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLOCKS)
ccsys_blkcnt_t
ccsys_ref_stat_st_blocks (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_blkcnt_t	F = { .data = D->st_blocks };
  return F;
}

void
ccsys_set_stat_st_blocks (ccsys_stat_t * S, ccsys_blkcnt_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_blocks = F.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME)
ccsys_timeval_t
ccsys_ref_stat_st_atime (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_timeval_t	F = {
    .seconds.data	= (ccsys_time_unit_t) D->st_atime,
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME_USEC)
    .microseconds.data	= (ccsys_time_unit_t) D->st_atime_usec
#else
    .microseconds.data	= 0
#endif
  };
  return F;
}

void
ccsys_set_stat_st_atime (ccsys_stat_t * S, ccsys_timeval_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_atime		= (time_t) F.seconds.data;
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME_USEC)
  D->st_atime_usec	= F.microseconds.data;
#endif
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME)
ccsys_timeval_t
ccsys_ref_stat_st_mtime (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_timeval_t	F = {
    .seconds.data	= (ccsys_time_unit_t) D->st_mtime,
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME_USEC)
    .microseconds.data	= (ccsys_time_unit_t) D->st_mtime_usec
#else
    .microseconds.data	= 0
#endif
  };
  return F;
}

void
ccsys_set_stat_st_mtime (ccsys_stat_t * S, ccsys_timeval_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_mtime		= (time_t) F.seconds.data;
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME_USEC)
  D->st_mtime_usec	= F.microseconds.data;
#endif
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME)
ccsys_timeval_t
ccsys_ref_stat_st_ctime (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_timeval_t	F = {
    .seconds.data	= (ccsys_time_unit_t) D->st_ctime,
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME_USEC)
    .microseconds.data	= (ccsys_time_unit_t) D->st_ctime_usec
#else
    .microseconds.data	= 0
#endif
  };
  return F;
}

void
ccsys_set_stat_st_ctime (ccsys_stat_t * S, ccsys_timeval_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_ctime		= (time_t) F.seconds.data;
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME_USEC)
  D->st_ctime_usec	= F.microseconds.data;
#endif
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIM)
ccsys_timespec_t
ccsys_ref_stat_st_atim (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_timespec_t	F = {
    .seconds.data	= (ccsys_time_unit_t) D->st_atim.tv_sec,
    .nanoseconds.data	= (ccsys_time_unit_t) D->st_atim.tv_nsec
  };
  return F;
}

void
ccsys_set_stat_st_atim (ccsys_stat_t * S, ccsys_timespec_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_atim.tv_sec	= (time_t) F.seconds.data;
  D->st_atim.tv_nsec	= F.nanoseconds.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIM)
ccsys_timespec_t
ccsys_ref_stat_st_mtim (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_timespec_t	F = {
    .seconds.data	= (ccsys_time_unit_t) D->st_mtim.tv_sec,
    .nanoseconds.data	= (ccsys_time_unit_t) D->st_mtim.tv_nsec
  };
  return F;
}

void
ccsys_set_stat_st_mtim (ccsys_stat_t * S, ccsys_timespec_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_mtim.tv_sec	= (time_t) F.seconds.data;
  D->st_mtim.tv_nsec	= F.nanoseconds.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIM)
ccsys_timespec_t
ccsys_ref_stat_st_ctim (ccsys_stat_t const * S)
{
  CCSYS_PC(struct stat const, D, S);
  ccsys_timespec_t	F = {
    .seconds.data	= (ccsys_time_unit_t) D->st_ctim.tv_sec,
    .nanoseconds.data	= (ccsys_time_unit_t) D->st_ctim.tv_nsec
  };
  return F;
}

void
ccsys_set_stat_st_ctim (ccsys_stat_t * S, ccsys_timespec_t F)
{
  CCSYS_PC(struct stat, D, S);
  D->st_ctim.tv_sec	= (time_t) F.seconds.data;
  D->st_ctim.tv_nsec	= F.nanoseconds.data;
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MODE)

bool
ccsys_s_isreg (ccsys_stat_mode_t F)
{
  return ((S_ISREG(F.data))? true : false);
}

bool
ccsys_s_isdir (ccsys_stat_mode_t F)
{
  return ((S_ISDIR(F.data))? true : false);
}

bool
ccsys_s_ischr (ccsys_stat_mode_t F)
{
  return ((S_ISCHR(F.data))? true : false);
}

bool
ccsys_s_isblk (ccsys_stat_mode_t F)
{
  return ((S_ISBLK(F.data))? true : false);
}

bool
ccsys_s_isfifo (ccsys_stat_mode_t F)
{
  return ((S_ISFIFO(F.data))? true : false);
}

bool
ccsys_s_islnk (ccsys_stat_mode_t F)
{
  return ((S_ISLNK(F.data))? true : false);
}

bool
ccsys_s_issock (ccsys_stat_mode_t F)
{
  return ((S_ISSOCK(F.data))? true : false);
}

#endif


/** --------------------------------------------------------------------
 ** File system: working directory.
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

#ifdef HAVE_GETCWD
bool
ccsys_custom_getcwd (cce_location_t * L, char * buffer, size_t size)
{
  char *	rv;
  errno = 0;
  rv = getcwd(buffer, size);
  if (NULL != rv) {
    return true;
  } else {
    if (ERANGE == errno) {
      return false;
    } else {
      cce_raise(L, cce_condition_new_errno_clear());
    }
  }
}
#endif

#ifdef HAVE_GET_CURRENT_DIR_NAME
char *
ccsys_get_current_dir_name (cce_destination_t L)
{
  char *	rv;
  errno = 0;
  rv = get_current_dir_name();
  if (NULL != rv) {
    return rv;
  } else {
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
ccsys_fchdir (cce_location_t * L, ccsys_dirfd_t dirfd)
{
  int	rv;
  errno = 0;
  rv = fchdir(dirfd.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** File system: creating and deleting directories.
 ** ----------------------------------------------------------------- */

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

#ifdef HAVE_MKDIRAT
void
ccsys_mkdirat (cce_location_t * L, ccsys_dirfd_t fd, char const * pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkdirat(fd.data, pathname, mode.data);
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

#ifdef HAVE_RMDIR
__attribute__((nonnull(1,2)))
static void
cce_handler_rmdir_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  if (0) { fprintf(stderr, "%s: removing '%s'\n", __func__, H->pathname); }
  rmdir(H->pathname);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_cleanup_handler_rmdir_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
{
  H->function	= cce_handler_rmdir_function;
  H->pathname	= (char *)pathname;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_rmdir_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
{
  H->function	= cce_handler_rmdir_function;
  H->pathname	= (char *)pathname;
  cce_register_error_handler(L, H);
}
#endif


/** --------------------------------------------------------------------
 ** File system: temporary files and directories.
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
 ** File system: links.
 ** ----------------------------------------------------------------- */

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
	      ccsys_dirfd_t oldfd, const char * oldname,
	      ccsys_dirfd_t newfd, const char * newname,
	      ccsys_linkat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = linkat(oldfd.data, oldname, newfd.data, newname, flags.data);
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
ccsys_symlinkat (cce_location_t * L, const char * oldname, ccsys_dirfd_t newdirfd, const char * newname)
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
ccsys_readlinkat (cce_location_t * L, ccsys_dirfd_t dirfd, char const * pathname, char * buffer, size_t size)
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
ccsys_unlinkat (cce_location_t * L, ccsys_dirfd_t dirfd, char const * pathname, ccsys_unlinkat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = unlinkat(dirfd.data, pathname, flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_UNLINKAT
__attribute__((nonnull(1,2)))
static void
cce_handler_unlinkat_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  ccsys_at_link_t *	lnk	= H->pointer;
  unlinkat(lnk->dirfd.data, lnk->pathname, lnk->flags.data);
  if (0) { fprintf(stderr, "%s: done unlinking '%s'\n", __func__, lnk->pathname); }
}

void
ccsys_cleanup_handler_unlinkat_init (cce_location_t * L, cce_handler_t * H, ccsys_at_link_t * lnk)
{
  H->function	= cce_handler_unlinkat_function;
  H->pointer	= lnk;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_unlinkat_init (cce_location_t * L, cce_handler_t * H, ccsys_at_link_t * lnk)
{
  H->function	= cce_handler_unlinkat_function;
  H->pointer	= lnk;
  cce_register_error_handler(L, H);
}
#endif


/** --------------------------------------------------------------------
 ** File system: removing files.
 ** ----------------------------------------------------------------- */

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

__attribute__((nonnull(1,2)))
static void
cce_handler_remove_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_REMOVE
  remove(H->pathname);
  if (0) { fprintf(stderr, "%s: done removing '%s'\n", __func__, H->pathname); }
#endif
}

void
ccsys_cleanup_handler_remove_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
{
  H->function	= cce_handler_remove_function;
  H->pathname	= (char *)pathname;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_remove_init (cce_location_t * L, cce_handler_t * H, char const * pathname)
{
  H->function	= cce_handler_remove_function;
  H->pathname	= (char *)pathname;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** File system: renaming directory entries.
 ** ----------------------------------------------------------------- */

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


/** --------------------------------------------------------------------
 ** File system: changing owner.
 ** ----------------------------------------------------------------- */

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
ccsys_fchownat (cce_location_t * L, ccsys_dirfd_t dirfd, char const * pathname, ccsys_uid_t owner, ccsys_gid_t group, int flags)
{
  int	rv;
  errno = 0;
  rv = fchownat(dirfd.data, pathname, owner.data, group.data, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** File system: changing access permissions.
 ** ----------------------------------------------------------------- */

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
ccsys_fchmodat (cce_location_t * L, ccsys_dirfd_t dirfd, char const * pathname, ccsys_open_mode_t mode, int flags)
{
  int	rv;
  errno = 0;
  rv = fchmodat(dirfd.data, pathname, mode.data, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** File system: testing access permissions.
 ** ----------------------------------------------------------------- */

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
ccsys_faccessat (cce_location_t * L, ccsys_dirfd_t dirfd, char const * pathname, int how, int flags)
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


/** --------------------------------------------------------------------
 ** File system: truncating files.
 ** ----------------------------------------------------------------- */

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
 ** File system: file times.
 ** ----------------------------------------------------------------- */

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

/* end of file */
