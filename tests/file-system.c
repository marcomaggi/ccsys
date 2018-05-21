/*
  Part of: CCSys
  Contents: tests for POSIX system functions, file system
  Date: Jan 19, 2018

  Abstract

	Tests for POSIX system functions, file system.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccsys.h"
#include "cctests.h"
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <string.h>


/** --------------------------------------------------------------------
 ** File system: retrieving the current working directory.
 ** ----------------------------------------------------------------- */

void
test_1_1_1 (cce_destination_t upper_L)
/* Acquire the current working directory with "ccsys_getcwd()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    size_t	len = CCSYS_PATH_MAX;
    char	buf[len];

    ccsys_getcwd(L, buf, len);
    if (0) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
    cce_run_clean_handlers(L);
  }
}

void
test_1_1_2 (cce_destination_t upper_L)
/* Acquire the  current working directory with  "ccsys_getcwd()".  First
   with a buffer too small. */
{
  cce_location_t	L[1];
  size_t volatile	len = 1;

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: outer: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    cce_location_t	inner_L[1];

    if (cce_location(inner_L)) {
      if (cce_condition_is_errno(cce_condition(inner_L))) {
	if (CCSYS_ERANGE == cce_condition_ref_errno_errnum(cce_condition(inner_L))) {
	  len *= 2;
	  cce_retry(inner_L);
	}
      }
      cce_run_error_handlers_raise(inner_L, L);
    } else {
      char	buf[len];
      ccsys_getcwd(inner_L, buf, len);
      cce_run_clean_handlers(inner_L);
    }
    cce_run_clean_handlers(L);
  }
}

void
test_1_1_3 (cce_destination_t upper_L)
/* Acquire the current working directory with "ccsys_custom_getcwd()". */
{
  size_t	len = 1;

  for (bool done = false; false == done; len *= 2) {
    char	buf[len];
    if (0) { fprintf(stderr, "%s: trying length: %lu\n", __func__, len); }
    done = ccsys_custom_getcwd(upper_L, buf, len);
    if (0 && done) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
  }
}

void
test_1_2 (cce_destination_t upper_L)
/* Acquire the current working directory with "ccsys_get_current_dir_name()". */
{
#if (defined HAVE_GET_CURRENT_DIR_NAME)
  cce_location_t	L[1];
  cce_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char *	buf;

    buf = ccsys_get_current_dir_name(L);
    ccsys_handler_malloc_init(L, H, buf);

    if (0) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: changing the current working directory, "chdir()".
 ** ----------------------------------------------------------------- */

static void
test_1_3_child (cce_destination_t L)
{
#if (defined HAVE_CHDIR)
  ccsys_chdir(L, "..");

#if (defined HAVE_GETCWD)
  {
    size_t	len = CCSYS_PATH_MAX;
    char	buf[len];
    ccsys_getcwd(L, buf, len);
    if (0) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
  }
#endif
#endif
}

void
test_1_3 (cce_destination_t upper_L)
/* Fork a process.   In the child process: change  the current directory
   to the  parent of the current  one; if successful exit  with success,
   otherwise exit  with failure.   In the parent  process: wait  for the
   child, check its exit status. */
{
#if (defined HAVE_CHDIR)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    cctests_call_in_forked_process(L, test_1_3_child);
    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: changing the current working directory, "fchdir()".
 ** ----------------------------------------------------------------- */

static void
test_1_4_child (cce_destination_t upper_L)
{
#if (defined HAVE_FCHDIR)
  cce_location_t	L[1];
  cce_clean_handler_t	dirstream_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	dirname  = "..";
    ccsys_dir_t *	dirstream;
    ccsys_dirfd_t	dirfd;

    dirstream = ccsys_opendir(L, dirname);
    ccsys_handler_dirstream_init(L, dirstream_H, dirstream);
    dirfd = ccsys_dirfd(L, dirstream);

    ccsys_fchdir(L, dirfd);

#if (defined HAVE_GETCWD)
    {
      size_t	len = CCSYS_PATH_MAX;
      char	buf[len];
      ccsys_getcwd(L, buf, len);
      if (0) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
    }
#endif
    cce_run_clean_handlers(L);
  }
#endif
}

void
test_1_4 (cce_destination_t upper_L)
/* Fork a process.   In the child process: change  the current directory
   to the  parent of the current  one; if successful exit  with success,
   otherwise exit  with failure.   In the parent  process: wait  for the
   child, check its exit status. */
{
#if (defined HAVE_FCHDIR)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    cctests_call_in_forked_process(L, test_1_4_child);
    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: reading directory contents.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
{
  cce_location_t          L[1];
  cce_clean_handler_t   dirstream_H[1];
  char const *            pathname = "./";

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_dir_t *         dirstream;
    ccsys_dirent_t *      direntry;

    dirstream = ccsys_opendir(L, pathname);
    ccsys_handler_dirstream_init(L, dirstream_H, dirstream);
    for (direntry = ccsys_readdir(L, dirstream);
	 direntry;
	 direntry = ccsys_readdir(L, dirstream)) {
      if (0) { printf("%s\n", ccsys_ref_dirent_d_name(direntry)); }
      fflush(stdout);
    }
    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating and removing directories.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
{
  cce_location_t          L[1];
  cce_clean_handler_t   dirname_H[1];
  char const *            dirname = "name.d";

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_open_mode_t     mode;

    mode.data = CCSYS_S_IRWXU;
    ccsys_mkdir(L, dirname, mode);
    ccsys_handler_rmdir_init(L, dirname_H, dirname);
    cce_run_clean_handlers(L);
  }
}

void
test_3_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  char const *		dirname = "name.d";

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_open_mode_t	mode;

    mode.data = CCSYS_S_IRWXU;
    ccsys_mkdir(L, dirname, mode);
    ccsys_rmdir(L, dirname);
    cce_run_clean_handlers(L);
  }
}

void
test_3_3 (cce_destination_t upper_L)
/* Create a diirectory with "ccsys_mkdirat()". */
{
  cce_location_t	L[1];
  cce_clean_handler_t	dirname1_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	dirname1 = "name.d";
    static char const * dirname2 = "subname.d";
    ccsys_dirfd_t	dirfd1;

    /* Create the parent directory. */
    {
      ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRWXU };
      ccsys_mkdir(L, dirname1, mode);
      ccsys_handler_rmdir_init(L, dirname1_H, dirname1);
    }

    /* Open  the  parent  directory.   The  descriptor  in  "dirfd1"  is
       released automatically when "fd" is released. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_PATH;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd     = ccsys_open(L, dirname1, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      dirfd1 = ccsys_fd_to_dirfd(fd);
    }

    /* Create the subdirectory. */
    {
      ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRWXU };

      ccsys_mkdirat(L, dirfd1, dirname2, mode);
    }

    /* Remove the subdirectory. */
    {
      ccsys_unlinkat_flags_t      flags;
      flags.data = CCSYS_AT_REMOVEDIR;
      ccsys_unlinkat(L, dirfd1, dirname2, flags);
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: getters and setters for "ccsys_stat_t".
 ** ----------------------------------------------------------------- */

void
test_4_1 (cce_destination_t upper_L)
{
  ccsys_stat_t	S;
  long		i = 0;

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_DEV)
  {
    ccsys_dev_t	F = { .data = ++i };
    ccsys_set_stat_st_dev(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_dev(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_INO)
  {
    ccsys_ino_t	F = { .data = ++i };
    ccsys_set_stat_st_ino(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_ino(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MODE)
  {
    ccsys_stat_mode_t	F = { .data = ++i };
    ccsys_set_stat_st_mode(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_mode(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_NLINK)
  {
    ccsys_nlink_t	F = { .data = ++i };
    ccsys_set_stat_st_nlink(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_nlink(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_UID)
  {
    ccsys_uid_t	F = { .data = ++i };
    ccsys_set_stat_st_uid(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_uid(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_GID)
  {
    ccsys_gid_t	F = { .data = ++i };
    ccsys_set_stat_st_gid(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_gid(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_RDEV)
  {
    ccsys_dev_t	F = { .data = ++i };
    ccsys_set_stat_st_rdev(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_rdev(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_SIZE)
  {
    ccsys_off_t	F = { .data = ++i };
    ccsys_set_stat_st_size(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_size(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLKSIZE)
  {
    ccsys_blksize_t	F = { .data = ++i };
    ccsys_set_stat_st_blksize(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_blksize(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLOCKS)
  {
    ccsys_blkcnt_t	F = { .data = ++i };
    ccsys_set_stat_st_blocks(&S, F);
    cctests_assert(upper_L, i == ccsys_ldref(ccsys_ref_stat_st_blocks(&S)));
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME)
  {
    ccsys_timeval_t	T1, T2;
    T1.seconds.data		= ++i;
    T1.microseconds.data	= 1+i;

    ccsys_set_stat_st_atime(&S, T1);
    T2 = ccsys_ref_stat_st_atime(&S);
    if (0) {
      fprintf(stderr, "%s: st_atime=%ld\n", __func__, T2.seconds.data);
      fprintf(stderr, "%s: st_atime_usec=%ld\n", __func__, T2.microseconds.data);
    }
    cctests_assert(upper_L,   i == ccsys_ldref(T2.seconds));
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME_USEC)
    cctests_assert(upper_L, 1+i == ccsys_ldref(T2.microseconds));
#endif
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME)
  {
    ccsys_timeval_t	T1, T2;
    T1.seconds.data		= ++i;
    T1.microseconds.data	= 1+i;

    ccsys_set_stat_st_mtime(&S, T1);
    T2 = ccsys_ref_stat_st_mtime(&S);
    if (0) {
      fprintf(stderr, "%s: st_mtime=%ld\n", __func__, T2.seconds.data);
      fprintf(stderr, "%s: st_mtime_usec=%ld\n", __func__, T2.microseconds.data);
    }
    cctests_assert(upper_L,   i == ccsys_ldref(T2.seconds));
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME_USEC)
    cctests_assert(upper_L, 1+i == ccsys_ldref(T2.microseconds));
#endif
  }
#endif

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME)
  {
    ccsys_timeval_t	T1, T2;
    T1.seconds.data		= ++i;
    T1.microseconds.data	= 1+i;

    ccsys_set_stat_st_ctime(&S, T1);
    T2 = ccsys_ref_stat_st_ctime(&S);
    if (0) {
      fprintf(stderr, "%s: st_ctime=%ld\n", __func__, T2.seconds.data);
      fprintf(stderr, "%s: st_ctime_usec=%ld\n", __func__, T2.microseconds.data);
    }
    cctests_assert(upper_L,   i == ccsys_ldref(T2.seconds));
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME_USEC)
    cctests_assert(upper_L, 1+i == ccsys_ldref(T2.microseconds));
#endif
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: inspecting attributes with "ccsys_stat()".
 ** ----------------------------------------------------------------- */

void
test_4_2 (cce_destination_t upper_L)
{
  cce_location_t	  L[1];
  cce_clean_handler_t   filename_H[1];
  cce_clean_handler_t   filedes_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Inspect the file by pathname. */
    {
      ccsys_stat_t	S;

      cctests_assert(L, true == ccsys_stat(L, filename, &S));
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_DEV)
      fprintf(stderr, "%s: st_dev=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_dev(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_INO)
      fprintf(stderr, "%s: st_ino=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_ino(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MODE)
      fprintf(stderr, "%s: st_mode=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_mode(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_NLINK)
      fprintf(stderr, "%s: st_nlink=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_nlink(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_UID)
      fprintf(stderr, "%s: st_uid=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_uid(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_GID)
      fprintf(stderr, "%s: st_gid=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_gid(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_RDEV)
      fprintf(stderr, "%s: st_rdev=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_rdev(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_SIZE)
      fprintf(stderr, "%s: st_size=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_size(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLKSIZE)
      fprintf(stderr, "%s: st_blksize=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_blksize(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLOCKS)
      fprintf(stderr, "%s: st_blocks=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_blocks(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_atime(&S);
	fprintf(stderr, "%s: st_atime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_atime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_mtime(&S);
	fprintf(stderr, "%s: st_mtime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_mtime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_ctime(&S);
	fprintf(stderr, "%s: st_ctime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_ctime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: inspecting attributes with "ccsys_fstat()".
 ** ----------------------------------------------------------------- */

void
test_4_3 (cce_destination_t upper_L)
{
  cce_location_t	  L[1];
  cce_clean_handler_t	  filename_H[1];
  cce_clean_handler_t	  filedes_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data	 = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Inspect the file by pathname. */
    {
      ccsys_stat_t	S;

      cctests_assert(L, true == ccsys_fstat(L, fd, &S));
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_DEV)
      fprintf(stderr, "%s: st_dev=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_dev(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_INO)
      fprintf(stderr, "%s: st_ino=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_ino(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MODE)
      fprintf(stderr, "%s: st_mode=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_mode(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_NLINK)
      fprintf(stderr, "%s: st_nlink=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_nlink(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_UID)
      fprintf(stderr, "%s: st_uid=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_uid(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_GID)
      fprintf(stderr, "%s: st_gid=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_gid(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_RDEV)
      fprintf(stderr, "%s: st_rdev=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_rdev(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_SIZE)
      fprintf(stderr, "%s: st_size=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_size(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLKSIZE)
      fprintf(stderr, "%s: st_blksize=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_blksize(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLOCKS)
      fprintf(stderr, "%s: st_blocks=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_blocks(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_atime(&S);
	fprintf(stderr, "%s: st_atime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_atime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_mtime(&S);
	fprintf(stderr, "%s: st_mtime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_mtime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_ctime(&S);
	fprintf(stderr, "%s: st_ctime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_ctime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: inspecting attributes with "ccsys_fstatat()".
 ** ----------------------------------------------------------------- */

void
test_4_4 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_handler_t		dirname_H[1];
  cce_handler_t		dir_H[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];
  ccsys_at_link_t	lnk;

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	dirname= "name.d";
    ccsys_dirfd_t	dirfd;
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create the parent directory. */
    {
      ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRWXU };
      ccsys_mkdir(L, dirname, mode);
      ccsys_clean_handler_rmdir_init(L, dirname_H, dirname);
    }

    /* Open the parent directory.  The descriptor in "dirfd" is released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dir;

      dir = ccsys_opendir(L, dirname);
      ccsys_clean_handler_dirstream_init(L, dir_H, dir);
      dirfd = ccsys_dirfd(L, dir);
    }

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_openat(L, dirfd, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      lnk.dirfd		= dirfd;
      lnk.pathname	= filename;
      lnk.flags.data	= 0;
      ccsys_handler_unlinkat_init(L, filename_H, &lnk);
    }

    /* Inspect the file by pathname. */
    {
      ccsys_stat_t		S;
      ccsys_fstatat_flags_t	flags;

      flags.data = 0;
      cctests_assert(L, true == ccsys_fstatat(L, dirfd, filename, &S, flags));

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_DEV)
      fprintf(stderr, "%s: st_dev=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_dev(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_INO)
      fprintf(stderr, "%s: st_ino=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_ino(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MODE)
      fprintf(stderr, "%s: st_mode=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_mode(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_NLINK)
      fprintf(stderr, "%s: st_nlink=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_nlink(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_UID)
      fprintf(stderr, "%s: st_uid=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_uid(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_GID)
      fprintf(stderr, "%s: st_gid=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_gid(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_RDEV)
      fprintf(stderr, "%s: st_rdev=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_rdev(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_SIZE)
      fprintf(stderr, "%s: st_size=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_size(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLKSIZE)
      fprintf(stderr, "%s: st_blksize=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_blksize(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLOCKS)
      fprintf(stderr, "%s: st_blocks=%lu\n", __func__, ccsys_ldref(ccsys_ref_stat_st_blocks(&S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_atime(&S);
	fprintf(stderr, "%s: st_atime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_atime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_mtime(&S);
	fprintf(stderr, "%s: st_mtime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_mtime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_ctime(&S);
	fprintf(stderr, "%s: st_ctime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_ctime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: inspecting attributes with "ccsys_lstat()".
 ** ----------------------------------------------------------------- */

void
test_4_5 (cce_destination_t upper_L)
{
  cce_location_t	  L[1];
  cce_clean_handler_t   filename_H[1];
  cce_clean_handler_t   filedes_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Inspect the file by pathname. */
    {
      ccsys_stat_t	S;

      cctests_assert(L, true == ccsys_lstat(L, filename, &S));

#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_DEV)
      fprintf(stderr, "%s: st_dev=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_dev(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_INO)
      fprintf(stderr, "%s: st_ino=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_ino(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MODE)
      fprintf(stderr, "%s: st_mode=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_mode(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_NLINK)
      fprintf(stderr, "%s: st_nlink=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_nlink(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_UID)
      fprintf(stderr, "%s: st_uid=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_uid(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_GID)
      fprintf(stderr, "%s: st_gid=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_gid(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_RDEV)
      fprintf(stderr, "%s: st_rdev=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_rdev(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_SIZE)
      fprintf(stderr, "%s: st_size=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_size(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLKSIZE)
      fprintf(stderr, "%s: st_blksize=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_blksize(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_BLOCKS)
      fprintf(stderr, "%s: st_blocks=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_blocks(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_ATIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_atime(&S);
	fprintf(stderr, "%s: st_atime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_atime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_MTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_mtime(&S);
	fprintf(stderr, "%s: st_mtime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_mtime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_CTIME)
      {
	ccsys_timeval_t	T = ccsys_ref_stat_st_ctime(&S);
	fprintf(stderr, "%s: st_ctime=%ld\n", __func__, T.seconds.data);
	fprintf(stderr, "%s: st_ctime_usec=%ld\n", __func__, T.microseconds.data);
      }
#endif
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating links with "ccsys_link()".
 ** ----------------------------------------------------------------- */

void
test_5_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_link(L, filename, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Verify the link existence. */
    {
      ccsys_stat_t	S[1];

      ccsys_stat(L, linkname, S);
      cctests_assert(L, ccsys_s_isreg(ccsys_ref_stat_st_mode(S)));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating links with "ccsys_symlink()".
 ** ----------------------------------------------------------------- */

void
test_5_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_symlink(L, filename, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Verify the link existence. */
    {
      ccsys_stat_t	S[1];

      ccsys_lstat(L, linkname, S);
      cctests_assert(L, ccsys_s_islnk(ccsys_ref_stat_st_mode(S)));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating links with "ccsys_linkat()".
 ** ----------------------------------------------------------------- */

void
test_5_3_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_handler_t		dir_H[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char		dirname[CCSYS_PATH_MAX];
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";
    ccsys_dirfd_t	dirfd;

    /* Acquire  the current  working directory  name.  Open  the current
       working  directory.   The  descriptor   in  "dirfd"  is  released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dir;

      ccsys_getcwd(L, dirname, CCSYS_PATH_MAX);
      dir = ccsys_opendir(L, dirname);
      ccsys_clean_handler_dirstream_init(L, dir_H, dir);
      dirfd = ccsys_dirfd(L, dir);
    }

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_openat(L, dirfd, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_linkat_flags_t	flags;

      flags.data = 0;
      ccsys_linkat(L, dirfd, filename, dirfd, linkname, flags);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Verify the link existence. */
    {
      ccsys_stat_t	S[1];

      ccsys_stat(L, linkname, S);
      cctests_assert(L, ccsys_s_isreg(ccsys_ref_stat_st_mode(S)));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating links with "ccsys_linkat()" and CCSYS_AT_FDCWD.
 ** ----------------------------------------------------------------- */

void
test_5_3_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_linkat_flags_t	flags;

      flags.data = 0;
      ccsys_linkat(L, CCSYS_AT_FDCWD, filename, CCSYS_AT_FDCWD, linkname, flags);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Verify the link existence. */
    {
      ccsys_stat_t	S[1];

      ccsys_stat(L, linkname, S);
      cctests_assert(L, ccsys_s_isreg(ccsys_ref_stat_st_mode(S)));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating links with "ccsys_symlinkat()".
 ** ----------------------------------------------------------------- */

void
test_5_4_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_handler_t		dir_H[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char		dirname[CCSYS_PATH_MAX];
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";
    ccsys_dirfd_t	dirfd;

    /* Acquire  the current  working directory  name.  Open  the current
       working  directory.   The  descriptor   in  "dirfd"  is  released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dir;

      ccsys_getcwd(L, dirname, CCSYS_PATH_MAX);
      dir = ccsys_opendir(L, dirname);
      ccsys_clean_handler_dirstream_init(L, dir_H, dir);
      dirfd = ccsys_dirfd(L, dir);
    }

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_openat(L, dirfd, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_symlinkat(L, filename, dirfd, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Verify the link existence. */
    {
      ccsys_stat_t	S[1];

      ccsys_lstat(L, linkname, S);
      cctests_assert(L, ccsys_s_islnk(ccsys_ref_stat_st_mode(S)));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating links with "ccsys_symlinkat()" and CCSYS_AT_FDCWD.
 ** ----------------------------------------------------------------- */

void
test_5_4_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_symlinkat(L, filename, CCSYS_AT_FDCWD, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Verify the link existence. */
    {
      ccsys_stat_t	S[1];

      ccsys_lstat(L, linkname, S);
      cctests_assert(L, ccsys_s_islnk(ccsys_ref_stat_st_mode(S)));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: reading symbolic links with "ccsys_readlink()".
 ** ----------------------------------------------------------------- */

void
test_6_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_symlink(L, filename, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Read the link. */
    {
      size_t	len = 1 + CCSYS_PATH_MAX;
      char	realname[len];

      len = ccsys_readlink(L, linkname, realname, len);
      realname[len] = '\0';
      if (1) { fprintf(stderr, "%s: %s\n", __func__, realname); }
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: reading symbolic links with "ccsys_readlinkat()".
 ** ----------------------------------------------------------------- */

void
test_6_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_symlink(L, filename, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Read the link. */
    {
      size_t	len = 1 + CCSYS_PATH_MAX;
      char	realname[len];

      len = ccsys_readlinkat(L, CCSYS_AT_FDCWD, linkname, realname, len);
      realname[len] = '\0';
      if (1) { fprintf(stderr, "%s: %s\n", __func__, realname); }
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: canonicalise names with "ccsys_realpath()".
 ** ----------------------------------------------------------------- */

void
test_6_3_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Read the pathname. */
    {
      char	realname[1 + CCSYS_PATH_MAX];

      ccsys_realpath(L, filename, realname);
      if (1) { fprintf(stderr, "%s: %s\n", __func__, realname); }
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: canonicalise names with "ccsys_realpath()", alloc memory.
 ** ----------------------------------------------------------------- */

void
test_6_3_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	realname_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Read the pathname. */
    {
      char *	realname;

      realname = ccsys_realpath(L, filename, NULL);
      ccsys_handler_malloc_init(L, realname_H, realname);
      if (1) { fprintf(stderr, "%s: %s\n", __func__, realname); }
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: removing links with "ccsys_unlink()".
 ** ----------------------------------------------------------------- */

void
test_7_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Unlink the pathname. */
    {
      ccsys_stat_t	S[1];

      ccsys_unlink(L, filename);
      cctests_assert(L, false == ccsys_stat(L, filename, S));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: removing links with "ccsys_unlinkat()".
 ** ----------------------------------------------------------------- */

void
test_7_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Unlink the pathname. */
    {
      ccsys_unlinkat_flags_t	flags;
      ccsys_stat_t		S[1];

      flags.data = 0;
      ccsys_unlinkat(L, CCSYS_AT_FDCWD, filename, flags);
      cctests_assert(L, false == ccsys_stat(L, filename, S));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: renaming links with "ccsys_rename()".
 ** ----------------------------------------------------------------- */

void
test_8_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	newname_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	newname  = "blue.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Rename the pathname. */
    {
      ccsys_rename(L, filename, newname);
      ccsys_handler_remove_init(L, newname_H, newname);
      cctests_assert(L, false == ccsys_pathname_isreg(L, filename));
      cctests_assert(L, true  == ccsys_pathname_isreg(L, newname));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: renaming links with "ccsys_renameat()".
 ** ----------------------------------------------------------------- */

void
test_8_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	newname_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	newname  = "blue.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Rename the pathname. */
    {
      ccsys_renameat(L, CCSYS_AT_FDCWD, filename, CCSYS_AT_FDCWD, newname);
      ccsys_handler_remove_init(L, newname_H, newname);
      cctests_assert(L, false == ccsys_pathname_isreg(L, filename));
      cctests_assert(L, true  == ccsys_pathname_isreg(L, newname));
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: renaming links with "ccsys_renameat2()".
 ** ----------------------------------------------------------------- */

void
test_8_3 (cce_destination_t upper_L CCSYS_UNUSED)
{
#if (1 == CCSYS_ON_LINUX)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	newname_H[1];

  fprintf(stderr, "%s: running test for renameat2()\n", __func__);

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	newname  = "blue.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Rename the pathname. */
    {
      ccsys_renameat2_flags_t	flags;

      flags.data = 0;
      ccsys_renameat2(L, CCSYS_AT_FDCWD, filename, CCSYS_AT_FDCWD, newname, flags);
      ccsys_handler_remove_init(L, newname_H, newname);
      cctests_assert(L, false == ccsys_pathname_isreg(L, filename));
      cctests_assert(L, true  == ccsys_pathname_isreg(L, newname));
    }

    cce_run_clean_handlers(L);
  }
#else
  fprintf(stderr, "%s: undefined renameat2()\n", __func__);
#endif
}


/** --------------------------------------------------------------------
 ** File system: changing ownership, test for "ccsys_chown()".
 ** ----------------------------------------------------------------- */

void
test_9_1 (cce_destination_t upper_L CCSYS_UNUSED)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Change ownership. */
    {
      ccsys_uid_t	uid = ccsys_getuid();
      ccsys_gid_t	gid = ccsys_getgid();

      ccsys_chown(L, filename, uid, gid);
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: changing ownership, test for "ccsys_lchown()".
 ** ----------------------------------------------------------------- */

void
test_9_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_symlink(L, filename, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Change ownership. */
    {
      ccsys_uid_t	uid = ccsys_getuid();
      ccsys_gid_t	gid = ccsys_getgid();

      ccsys_lchown(L, linkname, uid, gid);
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: changing ownership, test for "ccsys_fchown()".
 ** ----------------------------------------------------------------- */

void
test_9_3 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filedes_H[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Change ownership. */
    {
      ccsys_uid_t	uid = ccsys_getuid();
      ccsys_gid_t	gid = ccsys_getgid();

      ccsys_fchown(L, fd, uid, gid);
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: changing ownership, test for "ccsys_fchownat()".
 ** ----------------------------------------------------------------- */

void
test_9_4 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Change ownership. */
    {
      ccsys_uid_t		uid = ccsys_getuid();
      ccsys_gid_t		gid = ccsys_getgid();
      ccsys_fchownat_flags_t	flags;

      flags.data = 0;
      ccsys_fchownat(L, CCSYS_AT_FDCWD, filename, uid, gid, flags);
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: changing file mode.
 ** ----------------------------------------------------------------- */

void
test_10_1 (cce_destination_t upper_L)
/* Testing "ccsys_chmod()".*/
{
#if (defined HAVE_CHMOD)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT;
      mode.data  = 0;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Change mode. */
    {
      ccsys_open_mode_t		mode;

      mode.data  = CCSYS_S_IRWXU;
      ccsys_chmod(L, filename, mode);
    }

    /* Validate mode. */
    {
      ccsys_stat_t	S;
      ccsys_stat_mode_t	mode;

      cctests_assert(L, true == ccsys_stat(L, filename, &S));
      mode = ccsys_ref_stat_st_mode(&S);
      if (0) { fprintf(stderr, "%s: mode=%o\n", __func__, mode.data); }
      cctests_assert(L, CCSYS_S_IRWXU == (CCSYS_S_IRWXU & mode.data));
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_10_2 (cce_destination_t upper_L)
/* Testing "ccsys_fchmod()".*/
{
#if (defined HAVE_FCHMOD)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT;
      mode.data  = 0;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Change mode. */
    {
      ccsys_open_mode_t		mode;

      mode.data  = CCSYS_S_IRWXU;
      ccsys_fchmod(L, fd, mode);
    }

    /* Validate mode. */
    {
      ccsys_stat_t	S;
      ccsys_stat_mode_t	mode;

      cctests_assert(L, true == ccsys_stat(L, filename, &S));
      mode = ccsys_ref_stat_st_mode(&S);
      if (0) { fprintf(stderr, "%s: mode=%o\n", __func__, mode.data); }
      cctests_assert(L, CCSYS_S_IRWXU == (CCSYS_S_IRWXU & mode.data));
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_10_3 (cce_destination_t upper_L)
/* Testing "ccsys_fchmodat()".*/
{
#if (defined HAVE_FCHMODAT)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT;
      mode.data  = 0;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Change mode. */
    {
      ccsys_open_mode_t		mode;
      ccsys_fchmodat_flags_t	flags;

      mode.data  = CCSYS_S_IRWXU;
      flags.data = 0;
      ccsys_fchmodat(L, CCSYS_AT_FDCWD, filename, mode, flags);
    }

    /* Validate mode. */
    {
      ccsys_stat_t	S;
      ccsys_stat_mode_t	mode;

      cctests_assert(L, true == ccsys_stat(L, filename, &S));
      mode = ccsys_ref_stat_st_mode(&S);
      if (0) { fprintf(stderr, "%s: mode=%o\n", __func__, mode.data); }
      cctests_assert(L, CCSYS_S_IRWXU == (CCSYS_S_IRWXU & mode.data));
    }

    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: testing access permissions.
 ** ----------------------------------------------------------------- */

void
test_11_1 (cce_destination_t upper_L)
/* Testing "ccsys_access()".*/
{
#if (defined HAVE_ACCESS)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT;
      mode.data  = 0;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Testing permissions. */
    {
      ccsys_access_mode_t	mode = { .data = CCSYS_R_OK };

      cctests_assert(L, false == ccsys_access(L, filename, mode));
    }

    /* Change mode. */
    {
      ccsys_open_mode_t		mode;

      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_chmod(L, filename, mode);
    }

    /* Testing read/write permissions. */
    {
      ccsys_access_mode_t	mode = { .data = CCSYS_R_OK & CCSYS_W_OK };

      cctests_assert(L, true == ccsys_access(L, filename, mode));
    }

    /* Testing execute permissions. */
    {
      ccsys_access_mode_t	mode = { .data = CCSYS_X_OK };

      cctests_assert(L, false == ccsys_access(L, filename, mode));
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_11_2 (cce_destination_t upper_L)
/* Testing "ccsys_faccessat()".*/
{
#if (defined HAVE_FACCESSAT)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT;
      mode.data  = 0;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Testing permissions. */
    {
      ccsys_access_mode_t	mode  = { .data = CCSYS_R_OK };
      ccsys_faccessat_flags_t	flags = { .data = 0 };

      cctests_assert(L, false == ccsys_faccessat(L, CCSYS_AT_FDCWD, filename, mode, flags));
    }

    /* Change mode. */
    {
      ccsys_open_mode_t		mode;

      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_chmod(L, filename, mode);
    }

    /* Testing read/write permissions. */
    {
      ccsys_access_mode_t	mode  = { .data = CCSYS_R_OK & CCSYS_W_OK };
      ccsys_faccessat_flags_t	flags = { .data = 0 };

      cctests_assert(L, true == ccsys_faccessat(L, CCSYS_AT_FDCWD, filename, mode, flags));
    }

    /* Testing execute permissions. */
    {
      ccsys_access_mode_t	mode  = { .data = CCSYS_X_OK };
      ccsys_faccessat_flags_t	flags = { .data = 0 };

      cctests_assert(L, false == ccsys_faccessat(L, CCSYS_AT_FDCWD, filename, mode, flags));
    }

    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: truncate a file to a specified length.
 ** ----------------------------------------------------------------- */

void
test_12_1 (cce_destination_t upper_L)
/* Testing "ccsys_truncate()".*/
{
#if (defined HAVE_TRUNCATE)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Write some data to the file. */
    {
      size_t		buflen = 1024;
      char const *	bufptr[buflen];

      memset(bufptr, 0, buflen);
      ccsys_write(L, fd, bufptr, buflen);
    }

    /* Check the size. */
    {
      ccsys_stat_t	S[1];

      ccsys_stat(L, filename, S);
      cctests_assert(L, 1024 == ccsys_luref(ccsys_ref_stat_st_size(S)));
    }

    /* Truncate the file. */
    {
      ccsys_off_t	len = { .data = 100 };

      ccsys_truncate(L, filename, len);
    }

    /* Check the size after truncation. */
    {
      ccsys_stat_t	S[1];

      ccsys_stat(L, filename, S);
      if (0) { fprintf(stderr, "%s: len=%lu\n", __func__, ccsys_luref(ccsys_ref_stat_st_size(S))); }
      cctests_assert(L, 100 == ccsys_luref(ccsys_ref_stat_st_size(S)));
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_12_2 (cce_destination_t upper_L)
/* Testing "ccsys_ftruncate()".*/
{
#if (defined HAVE_FTRUNCATE)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Write some data to the file. */
    {
      size_t		buflen = 1024;
      char const *	bufptr[buflen];

      memset(bufptr, 0, buflen);
      ccsys_write(L, fd, bufptr, buflen);
    }

    /* Check the size. */
    {
      ccsys_stat_t	S[1];

      ccsys_stat(L, filename, S);
      cctests_assert(L, 1024 == ccsys_luref(ccsys_ref_stat_st_size(S)));
    }

    /* Truncate the file. */
    {
      ccsys_off_t	len = { .data = 101 };

      ccsys_ftruncate(L, fd, len);
    }

    /* Check the size after truncation. */
    {
      ccsys_stat_t	S[1];

      ccsys_stat(L, filename, S);
      if (0) { fprintf(stderr, "%s: len=%lu\n", __func__, ccsys_luref(ccsys_ref_stat_st_size(S))); }
      cctests_assert(L, 101 == ccsys_luref(ccsys_ref_stat_st_size(S)));
    }

    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: setting and getting the file times.
 ** ----------------------------------------------------------------- */

void
test_13_1 (cce_destination_t upper_L)
/* Testing "ccsys_utimes()".*/
{
#if (defined HAVE_UTIMES)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Set the times. */
    {
      ccsys_timeval_t	T[2];

      T[0].seconds.data		= 123;
      T[0].microseconds.data	= 0;
      T[1].seconds.data		= 456;
      T[1].microseconds.data	= 0;

      ccsys_utimes(L, filename, T);
    }

    /* Validate the times. */
    {
      ccsys_stat_t	S[1];
      ccsys_timespec_t	atim;
      ccsys_timespec_t	mtim;

      ccsys_stat(L, filename, S);
      atim = ccsys_ref_stat_st_atim(S);
      mtim = ccsys_ref_stat_st_mtim(S);
      cctests_assert(L, 123 == ccsys_luref(atim.seconds));
      cctests_assert(L, 0   == ccsys_luref(atim.nanoseconds));
      cctests_assert(L, 456 == ccsys_luref(mtim.seconds));
      cctests_assert(L, 0   == ccsys_luref(mtim.nanoseconds));
      if (1) {
	fprintf(stderr, "%s: atim.seconds=%lu\n",	__func__, ccsys_luref(atim.seconds));
	fprintf(stderr, "%s: atim.nanoseconds=%lu\n",	__func__, ccsys_luref(atim.nanoseconds));
	fprintf(stderr, "%s: mtim.seconds=%lu\n",	__func__, ccsys_luref(mtim.seconds));
	fprintf(stderr, "%s: mtim.nanoseconds=%lu\n",	__func__, ccsys_luref(mtim.nanoseconds));
      }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_13_2 (cce_destination_t upper_L)
/* Testing "ccsys_lutimes()".*/
{
#if (defined HAVE_LUTIMES)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	linkname_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    char const *	linkname = "link.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Create the link. */
    {
      ccsys_symlink(L, filename, linkname);
      ccsys_handler_remove_init(L, linkname_H, linkname);
    }

    /* Set the times. */
    {
      ccsys_timeval_t	T[2];

      T[0].seconds.data		= 123;
      T[0].microseconds.data	= 0;
      T[1].seconds.data		= 456;
      T[1].microseconds.data	= 0;

      ccsys_lutimes(L, linkname, T);
    }

    /* Validate the times. */
    {
      ccsys_stat_t	S[1];
      ccsys_timespec_t	atim;
      ccsys_timespec_t	mtim;

      ccsys_lstat(L, linkname, S);
      atim = ccsys_ref_stat_st_atim(S);
      mtim = ccsys_ref_stat_st_mtim(S);
      cctests_assert(L, 123 == ccsys_luref(atim.seconds));
      cctests_assert(L, 0   == ccsys_luref(atim.nanoseconds));
      cctests_assert(L, 456 == ccsys_luref(mtim.seconds));
      cctests_assert(L, 0   == ccsys_luref(mtim.nanoseconds));
      if (1) {
	fprintf(stderr, "%s: atim.seconds=%lu\n",	__func__, ccsys_luref(atim.seconds));
	fprintf(stderr, "%s: atim.nanoseconds=%lu\n",	__func__, ccsys_luref(atim.nanoseconds));
	fprintf(stderr, "%s: mtim.seconds=%lu\n",	__func__, ccsys_luref(mtim.seconds));
	fprintf(stderr, "%s: mtim.nanoseconds=%lu\n",	__func__, ccsys_luref(mtim.nanoseconds));
      }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_13_3 (cce_destination_t upper_L)
/* Testing "ccsys_futimes()".*/
{
#if (defined HAVE_FUTIMES)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Set the times. */
    {
      ccsys_timeval_t	T[2];

      T[0].seconds.data		= 123;
      T[0].microseconds.data	= 0;
      T[1].seconds.data		= 456;
      T[1].microseconds.data	= 0;

      ccsys_futimes(L, fd, T);
    }

    /* Validate the times. */
    {
      ccsys_stat_t	S[1];
      ccsys_timespec_t	atim;
      ccsys_timespec_t	mtim;

      ccsys_fstat(L, fd, S);
      atim = ccsys_ref_stat_st_atim(S);
      mtim = ccsys_ref_stat_st_mtim(S);
      cctests_assert(L, 123 == ccsys_luref(atim.seconds));
      cctests_assert(L, 0   == ccsys_luref(atim.nanoseconds));
      cctests_assert(L, 456 == ccsys_luref(mtim.seconds));
      cctests_assert(L, 0   == ccsys_luref(mtim.nanoseconds));
      if (1) {
	fprintf(stderr, "%s: atim.seconds=%lu\n",	__func__, ccsys_luref(atim.seconds));
	fprintf(stderr, "%s: atim.nanoseconds=%lu\n",	__func__, ccsys_luref(atim.nanoseconds));
	fprintf(stderr, "%s: mtim.seconds=%lu\n",	__func__, ccsys_luref(mtim.seconds));
	fprintf(stderr, "%s: mtim.nanoseconds=%lu\n",	__func__, ccsys_luref(mtim.nanoseconds));
      }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_13_4 (cce_destination_t upper_L)
/* Testing "ccsys_futimesat()".*/
{
#if (defined HAVE_FUTIMESAT)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Set the times. */
    {
      ccsys_timeval_t	T[2];

      T[0].seconds.data		= 123;
      T[0].microseconds.data	= 0;
      T[1].seconds.data		= 456;
      T[1].microseconds.data	= 0;

      ccsys_futimesat(L, CCSYS_AT_FDCWD, filename, T);
    }

    /* Validate the times. */
    {
      ccsys_stat_t	S[1];
      ccsys_timespec_t	atim;
      ccsys_timespec_t	mtim;

      ccsys_stat(L, filename, S);
      atim = ccsys_ref_stat_st_atim(S);
      mtim = ccsys_ref_stat_st_mtim(S);
      cctests_assert(L, 123 == ccsys_luref(atim.seconds));
      cctests_assert(L, 0   == ccsys_luref(atim.nanoseconds));
      cctests_assert(L, 456 == ccsys_luref(mtim.seconds));
      cctests_assert(L, 0   == ccsys_luref(mtim.nanoseconds));
      if (1) {
	fprintf(stderr, "%s: atim.seconds=%lu\n",	__func__, ccsys_luref(atim.seconds));
	fprintf(stderr, "%s: atim.nanoseconds=%lu\n",	__func__, ccsys_luref(atim.nanoseconds));
	fprintf(stderr, "%s: mtim.seconds=%lu\n",	__func__, ccsys_luref(mtim.seconds));
	fprintf(stderr, "%s: mtim.nanoseconds=%lu\n",	__func__, ccsys_luref(mtim.nanoseconds));
      }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_13_5 (cce_destination_t upper_L)
/* Testing "ccsys_utimensat()".*/
{
#if (defined HAVE_UTIMENSAT)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Set the times. */
    {
      ccsys_timespec_t		T[2];
      ccsys_utimensat_flags_t	flags;

      T[0].seconds.data		= 123;
      T[0].nanoseconds.data	= 0;
      T[1].seconds.data		= 456;
      T[1].nanoseconds.data	= 0;

      flags.data = 0;
      ccsys_utimensat(L, CCSYS_AT_FDCWD, filename, T, flags);
    }

    /* Validate the times. */
    {
      ccsys_stat_t	S[1];
      ccsys_timespec_t	atim;
      ccsys_timespec_t	mtim;

      ccsys_stat(L, filename, S);
      atim = ccsys_ref_stat_st_atim(S);
      mtim = ccsys_ref_stat_st_mtim(S);
      cctests_assert(L, 123 == ccsys_luref(atim.seconds));
      cctests_assert(L, 0   == ccsys_luref(atim.nanoseconds));
      cctests_assert(L, 456 == ccsys_luref(mtim.seconds));
      cctests_assert(L, 0   == ccsys_luref(mtim.nanoseconds));
      if (1) {
	fprintf(stderr, "%s: atim.seconds=%lu\n",	__func__, ccsys_luref(atim.seconds));
	fprintf(stderr, "%s: atim.nanoseconds=%lu\n",	__func__, ccsys_luref(atim.nanoseconds));
	fprintf(stderr, "%s: mtim.seconds=%lu\n",	__func__, ccsys_luref(mtim.seconds));
	fprintf(stderr, "%s: mtim.nanoseconds=%lu\n",	__func__, ccsys_luref(mtim.nanoseconds));
      }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_13_6 (cce_destination_t upper_L)
/* Testing "ccsys_futimens()".*/
{
#if (defined HAVE_FUTIMENS)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create the file. */
    {
      ccsys_open_mode_t		mode;
      ccsys_open_flags_t	flags;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Set the times. */
    {
      ccsys_timespec_t	T[2];

      T[0].seconds.data		= 123;
      T[0].nanoseconds.data	= 0;
      T[1].seconds.data		= 456;
      T[1].nanoseconds.data	= 0;

      ccsys_futimens(L, fd, T);
    }

    /* Validate the times. */
    {
      ccsys_stat_t	S[1];
      ccsys_timespec_t	atim;
      ccsys_timespec_t	mtim;

      ccsys_stat(L, filename, S);
      atim = ccsys_ref_stat_st_atim(S);
      mtim = ccsys_ref_stat_st_mtim(S);
      cctests_assert(L, 123 == ccsys_luref(atim.seconds));
      cctests_assert(L, 0   == ccsys_luref(atim.nanoseconds));
      cctests_assert(L, 456 == ccsys_luref(mtim.seconds));
      cctests_assert(L, 0   == ccsys_luref(mtim.nanoseconds));
      if (1) {
	fprintf(stderr, "%s: atim.seconds=%lu\n",	__func__, ccsys_luref(atim.seconds));
	fprintf(stderr, "%s: atim.nanoseconds=%lu\n",	__func__, ccsys_luref(atim.nanoseconds));
	fprintf(stderr, "%s: mtim.seconds=%lu\n",	__func__, ccsys_luref(mtim.seconds));
	fprintf(stderr, "%s: mtim.nanoseconds=%lu\n",	__func__, ccsys_luref(mtim.nanoseconds));
      }
    }

    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: creating temporary files.
 ** ----------------------------------------------------------------- */

void
test_14_1 (cce_destination_t upper_L)
/* Testing "ccsys_mkstemp()".*/
{
#if (defined HAVE_MKSTEMP)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char	filename[] = "name.ext.XXXXXX";

    /* Create the file. */
    {
      ccsys_fd_t	fd;

      fd = ccsys_mkstemp(L, filename);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Validate file existence. */
    {
      cctests_assert(L, true == ccsys_pathname_isreg(L, filename));
      if (1) { fprintf(stderr, "%s: tempname=%s\n", __func__, filename); }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_14_2 (cce_destination_t upper_L)
/* Testing "ccsys_mkostemp()".*/
{
#if (defined HAVE_MKOSTEMP)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char	filename[] = "name.ext.XXXXXX";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      fd = ccsys_mkostemp(L, filename, flags);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Validate file existence. */
    {
      cctests_assert(L, true == ccsys_pathname_isreg(L, filename));
      if (1) { fprintf(stderr, "%s: tempname=%s\n", __func__, filename); }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_14_3 (cce_destination_t upper_L)
/* Testing "ccsys_mkstemps()".*/
{
#if (defined HAVE_MKSTEMPS)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char	filename[] = "nameXXXXXX.ext";

    /* Create the file. */
    {
      ccsys_fd_t	fd;

      fd = ccsys_mkstemps(L, filename, 4);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Validate file existence. */
    {
      cctests_assert(L, true == ccsys_pathname_isreg(L, filename));
      if (1) { fprintf(stderr, "%s: tempname=%s\n", __func__, filename); }
    }

    cce_run_clean_handlers(L);
  }
#endif
}

void
test_14_4 (cce_destination_t upper_L)
/* Testing "ccsys_mkostemps()".*/
{
#if (defined HAVE_MKOSTEMPS)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char	filename[] = "nameXXXXXX.ext";

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_fd_t		fd;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      fd = ccsys_mkostemps(L, filename, 4, flags);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    /* Validate file existence. */
    {
      cctests_assert(L, true == ccsys_pathname_isreg(L, filename));
      if (1) { fprintf(stderr, "%s: tempname=%s\n", __func__, filename); }
    }

    cce_run_clean_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** File system: creating temporary directories.
 ** ----------------------------------------------------------------- */

void
test_15_1 (cce_destination_t upper_L)
/* Testing "ccsys_mkdtemp()".*/
{
#if (defined HAVE_MKDTEMP)
  cce_location_t	L[1];
  cce_clean_handler_t	dirname_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char	dirname[] = "name.d.XXXXXX";

    /* Create the directory. */
    {
      ccsys_mkdtemp(L, dirname);
      ccsys_handler_rmdir_init(L, dirname_H, dirname);
    }

    /* Validate file existence. */
    {
      cctests_assert(L, true == ccsys_pathname_isdir(L, dirname));
      if (1) { fprintf(stderr, "%s: tempname=%s\n", __func__, dirname); }
    }

    cce_run_clean_handlers(L);
  }
#endif
}


int
main (void)
{
  cctests_init("file system");
  {
    cctests_begin_group("current working directory");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_1_2);
      cctests_run(test_1_1_3);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
      cctests_run(test_1_4);
    }
    cctests_end_group();

    cctests_begin_group("reading directory contents");
    {
      cctests_run(test_2_1);
    }
    cctests_end_group();

    cctests_begin_group("creating and removing directories");
    {
      cctests_run(test_3_1);
      cctests_run(test_3_2);
      cctests_run(test_3_3);
    }
    cctests_end_group();

    cctests_begin_group("inspecting attributes with stat");
    {
      cctests_run(test_4_1);
      cctests_run(test_4_2);
      cctests_run(test_4_3);
      cctests_run(test_4_4);
      cctests_run(test_4_5);
    }
    cctests_end_group();

    cctests_begin_group("creating links");
    {
      cctests_run(test_5_1);
      cctests_run(test_5_2);
      cctests_run(test_5_3_1);
      cctests_run(test_5_3_2);
      cctests_run(test_5_4_1);
      cctests_run(test_5_4_2);
    }
    cctests_end_group();

    cctests_begin_group("reading links");
    {
      cctests_run(test_6_1);
      cctests_run(test_6_2);
      cctests_run(test_6_3_1);
      cctests_run(test_6_3_2);
    }
    cctests_end_group();

    cctests_begin_group("removing links");
    {
      cctests_run(test_7_1);
      cctests_run(test_7_2);
    }
    cctests_end_group();

    cctests_begin_group("renaming links");
    {
      cctests_run(test_8_1);
      cctests_run(test_8_2);
      cctests_run(test_8_3);
    }
    cctests_end_group();

    cctests_begin_group("changing ownership");
    {
      cctests_run(test_9_1);
      cctests_run(test_9_2);
      cctests_run(test_9_3);
      cctests_run(test_9_4);
    }
    cctests_end_group();

    cctests_begin_group("changing file mode");
    {
      cctests_run(test_10_1);
      cctests_run(test_10_2);
      cctests_run(test_10_3);
    }
    cctests_end_group();

    cctests_begin_group("testing access permissions");
    {
      cctests_run(test_11_1);
      cctests_run(test_11_2);
    }
    cctests_end_group();

    cctests_begin_group("truncate a file to a specified length");
    {
      cctests_run(test_12_1);
      cctests_run(test_12_2);
    }
    cctests_end_group();

    cctests_begin_group("setting and getting the file times");
    {
      cctests_run(test_13_1);
      cctests_run(test_13_2);
      cctests_run(test_13_3);
      cctests_run(test_13_4);
      cctests_run(test_13_5);
      cctests_run(test_13_6);
    }
    cctests_end_group();

    cctests_begin_group("creating temporary files");
    {
      cctests_run(test_14_1);
      cctests_run(test_14_2);
      cctests_run(test_14_3);
      cctests_run(test_14_4);
    }
    cctests_end_group();

    cctests_begin_group("creating temporary directories");
    {
      cctests_run(test_15_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
