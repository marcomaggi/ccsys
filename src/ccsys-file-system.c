/*
  Part of: CCSys
  Contents: POSIX system calls wrappers, file system functions
  Date: Wed Feb 15, 2017

  Abstract

	POSIX system calls wrappers, file system functions.

  Copyright (C) 2017, 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#ifdef HAVE_LINUX_FS_H
#  include <linux/fs.h>
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
#endif
#ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif
#ifdef HAVE_SYS_SYSCALL_H
#  include <sys/syscall.h>
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
 ** File system: creating files.
 ** ----------------------------------------------------------------- */

#if ((defined HAVE_OPEN) && (defined HAVE_CLOSE))
void
ccsys_touch (cce_destination_t L, ccstructs_pathname_I filename, ccsys_open_flags_t flags, ccsys_open_mode_t mode)
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
ccsys_opendir (cce_location_t * L, ccstructs_pathname_I pathname)
{
  DIR *	rv;
  errno = 0;
  rv = opendir(ccstructs_pathname_asciiz(L, pathname).ptr);
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
cce_dirstream_handler_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  DIR *		dirstream = H->pointer;
  closedir(dirstream);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_init_dirstream_clean_handler (cce_location_t * L, cce_clean_handler_t * H, ccsys_dir_t * dirstream)
{
  H->handler.function	= cce_dirstream_handler_function;
  H->handler.pointer	= dirstream;
  cce_register_clean_handler(L, H);
}

void
ccsys_init_dirstream_error_handler (cce_location_t * L, cce_error_handler_t * H, ccsys_dir_t * dirstream)
{
  H->handler.function	= cce_dirstream_handler_function;
  H->handler.pointer	= dirstream;
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
ccsys_stat (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
  errno = 0;
  rv = stat(ccstructs_pathname_asciiz(L, pathname).ptr, buf);
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
ccsys_fstatat (cce_destination_t L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname, ccsys_stat_t * _buf, ccsys_fstatat_flags_t flags)
{
  CCSYS_PC(struct stat, buf, _buf);
  int		rv;
  errno = 0;
  rv = fstatat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, buf, flags.data);
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
ccsys_lstat (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_stat_t * _buf)
{
  struct stat *	buf = (struct stat *)_buf;
  int		rv;
  errno = 0;
  rv = lstat(ccstructs_pathname_asciiz(L, pathname).ptr, buf);
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
 ** File system: pathname existence and type.
 ** ----------------------------------------------------------------- */

bool
ccsys_pathname_exists (cce_destination_t L, ccstructs_pathname_I pathname)
{
  ccsys_stat_t	S[1];

  return ccsys_stat(L, pathname, S);
}

bool
ccsys_pathname_isreg (cce_destination_t L, ccstructs_pathname_I pathname)
{
  ccsys_stat_t	S[1];

  if (ccsys_stat(L, pathname, S)) {
    return ccsys_s_isreg(ccsys_ref_stat_st_mode(S));
  } else {
    return false;
  }
}

bool
ccsys_pathname_isdir (cce_destination_t L, ccstructs_pathname_I pathname)
{
  ccsys_stat_t	S[1];

  if (ccsys_stat(L, pathname, S)) {
    return ccsys_s_isdir(ccsys_ref_stat_st_mode(S));
  } else {
    return false;
  }
}

bool
ccsys_pathname_uid_gid (cce_destination_t L, ccstructs_pathname_I pathname, ccsys_uid_t * uidp, ccsys_gid_t * gidp)
{
  ccsys_stat_t	S[1];

  if (ccsys_stat(L, pathname, S)) {
    *uidp = ccsys_ref_stat_st_uid(S);
    *gidp = ccsys_ref_stat_st_gid(S);
    return true;
  } else {
    return false;
  }
}


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
ccsys_chdir (cce_location_t * L, ccstructs_pathname_I pathname)
{
  int	rv;
  errno = 0;
  rv = chdir(ccstructs_pathname_asciiz(L, pathname).ptr);
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
ccsys_mkdir (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkdir(ccstructs_pathname_asciiz(L, pathname).ptr, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_MKDIRAT
void
ccsys_mkdirat (cce_location_t * L, ccsys_dirfd_t fd, ccstructs_pathname_I pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = mkdirat(fd.data, ccstructs_pathname_asciiz(L, pathname).ptr, mode.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_RMDIR
void
ccsys_rmdir (cce_location_t * L, ccstructs_pathname_I pathname)
{
  int	rv;
  errno = 0;
  rv = rmdir(ccstructs_pathname_asciiz(L, pathname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_RMDIR
__attribute__((nonnull(1,2)))
static void
cce_rmdir_handler_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * _H)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_final(L);
  } else {
    CCSYS_PC(ccsys_pathname_clean_handler_t, H, _H);

    if (0) { fprintf(stderr, "%s: removing '%s'\n", __func__, ccstructs_pathname_asciiz(L, H->pathname).ptr); }
    rmdir(ccstructs_pathname_asciiz(L, H->pathname).ptr);
    if (0) { fprintf(stderr, "%s: done\n", __func__); }
    cce_run_body_handlers(L);
  }
}

void
ccsys_init_rmdir_clean_handler (cce_location_t * L, ccsys_pathname_clean_handler_t * H, ccstructs_pathname_I pathname)
{
  H->handler.handler.function	= cce_rmdir_handler_function;
  H->pathname			= pathname;
  cce_register_clean_handler(L, &(H->handler));
}

void
ccsys_init_rmdir_error_handler (cce_location_t * L, ccsys_pathname_error_handler_t * H, ccstructs_pathname_I pathname)
{
  H->handler.handler.function	= cce_rmdir_handler_function;
  H->pathname			= pathname;
  cce_register_error_handler(L, &(H->handler));
}
#endif


/** --------------------------------------------------------------------
 ** File system: temporary files and directories.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_MKSTEMP
ccsys_fd_t
ccsys_mkstemp (cce_location_t * L, char * template)
{
  int	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkstemp(template);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .data = rv };
    return fd;
  }
}
#endif

#ifdef HAVE_MKOSTEMP
ccsys_fd_t
ccsys_mkostemp (cce_location_t * L, char * template, ccsys_open_flags_t flags)
{
  int	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkostemp(template, flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .data = rv };
    return fd;
  }
}
#endif

#ifdef HAVE_MKSTEMPS
ccsys_fd_t
ccsys_mkstemps (cce_location_t * L, char * template, int suffixlen)
{
  int	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkstemps(template, suffixlen);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .data = rv };
    return fd;
  }
}
#endif

#ifdef HAVE_MKOSTEMPS
ccsys_fd_t
ccsys_mkostemps (cce_location_t * L, char * template, int suffixlen, ccsys_open_flags_t flags)
{
  int	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkostemps(template, suffixlen, flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_fd_t	fd = { .data = rv };
    return fd;
  }
}
#endif

/* ------------------------------------------------------------------ */

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
ccsys_link (cce_location_t * L, ccstructs_pathname_I oldname, ccstructs_pathname_I newname)
{
  int	rv;
  errno = 0;
  rv = link(ccstructs_pathname_asciiz(L, oldname).ptr, ccstructs_pathname_asciiz(L, newname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LINKAT
void
ccsys_linkat (cce_location_t * L,
	      ccsys_dirfd_t oldfd, ccstructs_pathname_I oldname,
	      ccsys_dirfd_t newfd, ccstructs_pathname_I newname,
	      ccsys_linkat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = linkat(oldfd.data, ccstructs_pathname_asciiz(L, oldname).ptr,
	      newfd.data, ccstructs_pathname_asciiz(L, newname).ptr,
	      flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_SYMLINK
void
ccsys_symlink (cce_location_t * L, ccstructs_pathname_I oldname, ccstructs_pathname_I newname)
{
  int	rv;
  errno = 0;
  rv = symlink(ccstructs_pathname_asciiz(L, oldname).ptr, ccstructs_pathname_asciiz(L, newname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SYMLINKAT
void
ccsys_symlinkat (cce_location_t * L, ccstructs_pathname_I oldname, ccsys_dirfd_t newdirfd, ccstructs_pathname_I newname)
{
  int	rv;
  errno = 0;
  rv = symlinkat(ccstructs_pathname_asciiz(L, oldname).ptr, newdirfd.data, ccstructs_pathname_asciiz(L, newname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_READLINK
size_t
ccsys_readlink (cce_location_t * L, ccstructs_pathname_I pathname, char * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = readlink(ccstructs_pathname_asciiz(L, pathname).ptr, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_READLINKAT
size_t
ccsys_readlinkat (cce_location_t * L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname, char * buffer, size_t size)
{
  ssize_t	rv;
  errno = 0;
  rv = readlinkat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_REALPATH
char *
ccsys_realpath (cce_location_t * L, ccstructs_pathname_I pathname, char * resolved_path)
{
  char *	rv;
  errno = 0;
  rv = realpath(ccstructs_pathname_asciiz(L, pathname).ptr, resolved_path);
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
ccsys_unlink (cce_location_t * L, ccstructs_pathname_I pathname)
{
  int	rv;
  errno = 0;
  rv = unlink(ccstructs_pathname_asciiz(L, pathname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_UNLINKAT
void
ccsys_unlinkat (cce_location_t * L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname, ccsys_unlinkat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = unlinkat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_UNLINKAT
__attribute__((nonnull(1,2)))
static void
cce_unlinkat_handler_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_final(L);
  } else {
    ccsys_at_link_t *	lnk	= H->pointer;
    unlinkat(lnk->dirfd.data, ccstructs_pathname_asciiz(L, lnk->pathname).ptr, lnk->flags.data);
    if (0) { fprintf(stderr, "%s: done unlinking '%s'\n", __func__, ccstructs_pathname_asciiz(L, lnk->pathname).ptr); }
    cce_run_body_handlers(L);
  }
}

void
ccsys_init_unlinkat_clean_handler (cce_location_t * L, cce_clean_handler_t * H, ccsys_at_link_t * lnk)
{
  H->handler.function	= cce_unlinkat_handler_function;
  H->handler.pointer	= lnk;
  cce_register_clean_handler(L, H);
}

void
ccsys_init_unlinkat_error_handler (cce_location_t * L, cce_error_handler_t * H, ccsys_at_link_t * lnk)
{
  H->handler.function	= cce_unlinkat_handler_function;
  H->handler.pointer	= lnk;
  cce_register_error_handler(L, H);
}
#endif


/** --------------------------------------------------------------------
 ** File system: removing files.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_REMOVE
void
ccsys_remove (cce_location_t * L, ccstructs_pathname_I pathname)
{
  int	rv;
  errno = 0;
  rv = remove(ccstructs_pathname_asciiz(L, pathname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_REMOVE
__attribute__((nonnull(1,2)))
static void
cce_remove_handler_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * _H)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_final(L);
  } else {
    CCSYS_PC(ccsys_pathname_clean_handler_t, H, _H);
    remove(ccstructs_pathname_asciiz(L, H->pathname).ptr);
    if (0) { fprintf(stderr, "%s: done removing '%s'\n", __func__, ccstructs_pathname_asciiz(L, H->pathname).ptr); }
    cce_run_body_handlers(L);
  }
}

void
ccsys_init_remove_clean_handler (cce_location_t * L, ccsys_pathname_clean_handler_t * H, ccstructs_pathname_I pathname)
{
  H->handler.handler.function	= cce_remove_handler_function;
  H->pathname			= pathname;
  cce_register_clean_handler(L, &(H->handler));
}

void
ccsys_init_remove_error_handler (cce_location_t * L, ccsys_pathname_error_handler_t * H, ccstructs_pathname_I pathname)
{
  H->handler.handler.function	= cce_remove_handler_function;
  H->pathname			= pathname;
  cce_register_error_handler(L, &(H->handler));
}
#endif


/** --------------------------------------------------------------------
 ** File system: renaming directory entries.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_RENAME
void
ccsys_rename (cce_location_t * L, ccstructs_pathname_I oldname, ccstructs_pathname_I newname)
{
  int	rv;
  errno = 0;
  rv = rename(ccstructs_pathname_asciiz(L, oldname).ptr,
	      ccstructs_pathname_asciiz(L, newname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_RENAMEAT
void
ccsys_renameat (cce_destination_t L,
		ccsys_dirfd_t old_dirfd, ccstructs_pathname_I oldname,
		ccsys_dirfd_t new_dirfd, ccstructs_pathname_I newname)
{
  int	rv;
  errno = 0;
  rv = renameat(old_dirfd.data, ccstructs_pathname_asciiz(L, oldname).ptr,
		new_dirfd.data, ccstructs_pathname_asciiz(L, newname).ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if (defined CCSYS_ON_LINUX)
void
ccsys_renameat2 (cce_destination_t L,
		 ccsys_dirfd_t old_dirfd, ccstructs_pathname_I oldname,
		 ccsys_dirfd_t new_dirfd, ccstructs_pathname_I newname,
		 ccsys_renameat2_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = syscall(SYS_renameat2,
	       old_dirfd.data, ccstructs_pathname_asciiz(L, oldname).ptr,
	       new_dirfd.data, ccstructs_pathname_asciiz(L, newname).ptr,
	       flags.data);
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
ccsys_chown (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_uid_t owner, ccsys_gid_t group)
{
  int	rv;
  errno = 0;
  rv = chown(ccstructs_pathname_asciiz(L, pathname).ptr, owner.data, group.data);
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
ccsys_lchown (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_uid_t owner, ccsys_gid_t group)
{
  int	rv;
  errno = 0;
  rv = lchown(ccstructs_pathname_asciiz(L, pathname).ptr, owner.data, group.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FCHOWNAT
void
ccsys_fchownat (cce_location_t * L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname, ccsys_uid_t owner, ccsys_gid_t group,
		ccsys_fchownat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = fchownat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, owner.data, group.data, flags.data);
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
ccsys_chmod (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_open_mode_t mode)
{
  int	rv;
  errno = 0;
  rv = chmod(ccstructs_pathname_asciiz(L, pathname).ptr, mode.data);
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
ccsys_fchmodat (cce_location_t * L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname, ccsys_open_mode_t mode, ccsys_fchmodat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = fchmodat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, mode.data, flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** File system: testing access permissions.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_ACCESS
bool
ccsys_access (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_access_mode_t how)
{
  int	rv;
  errno = 0;
  rv = access(ccstructs_pathname_asciiz(L, pathname).ptr, how.data);
  if (0 == rv) {
    return true;
  } else if (CCSYS_EACCES == errno) {
    return false;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FACCESSAT
bool
ccsys_faccessat (cce_location_t * L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname,
		 ccsys_access_mode_t  how, ccsys_faccessat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = faccessat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, how.data, flags.data);
  if (0 == rv) {
    return true;
  } else if (CCSYS_EACCES == errno) {
    return false;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** File system: truncating files.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_TRUNCATE
void
ccsys_truncate (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_off_t length)
{
  int	rv;
  errno = 0;
  rv = truncate(ccstructs_pathname_asciiz(L, pathname).ptr, (off_t)(length.data));
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

#ifdef HAVE_UTIMES
void
ccsys_utimes (cce_location_t * L, ccstructs_pathname_I pathname, ccsys_timeval_t const T[2])
{
  int	rv;
  errno = 0;
  rv = utimes(ccstructs_pathname_asciiz(L, pathname).ptr, (struct timeval const *)T);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_UTIMENSAT
void
ccsys_utimensat (cce_destination_t L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname,
		 ccsys_timespec_t const T[2], ccsys_utimensat_flags_t flags)
{
  int	rv;
  errno = 0;
  rv = utimensat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, (struct timespec const *)T, flags.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_FUTIMENS
void
ccsys_futimens (cce_destination_t L, ccsys_fd_t fd, ccsys_timespec_t const T[2])
{
  int	rv;
  errno = 0;
  rv = futimens(fd.data, (struct timespec const *)T);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#if ((defined HAVE_LUTIMES) && (1 == CCSYS_ON_LINUX))
void
ccsys_lutimes (cce_location_t * L, ccstructs_pathname_I pathname,  ccsys_timeval_t const T[2])
{
  int	rv;
  errno = 0;
  rv = lutimes(ccstructs_pathname_asciiz(L, pathname).ptr, (const struct timeval *)T);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_FUTIMES) && (1 == CCSYS_ON_LINUX))
void
ccsys_futimes (cce_location_t * L, ccsys_fd_t filedes,  ccsys_timeval_t const T[2])
{
  int	rv;
  errno = 0;
  rv = futimes(filedes.data, (const struct timeval *)T);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_FUTIMESAT) && (1 == CCSYS_ON_LINUX))
void
ccsys_futimesat (cce_destination_t L, ccsys_dirfd_t dirfd, ccstructs_pathname_I pathname, ccsys_timeval_t const T[2])
{
  int	rv;
  errno = 0;
  rv = futimesat(dirfd.data, ccstructs_pathname_asciiz(L, pathname).ptr, (struct timeval const *)T);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* end of file */
