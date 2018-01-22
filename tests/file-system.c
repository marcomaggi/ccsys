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
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** File system: retrieving the current working directory.
 ** ----------------------------------------------------------------- */

void
test_1_1_1 (cce_destination_t upper_L)
/* Acquire the current working directory with "ccsys_getcwd()". */
{
  size_t	len = CCSYS_PATH_MAX;
  char		buf[len];

  ccsys_getcwd(upper_L, buf, len);
  if (0) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
}

void
test_1_1_2 (cce_destination_t upper_L)
/* Acquire the  current working directory with  "ccsys_getcwd()".  First
   with a buffer too small. */
{
  cce_location_t	L[1];
  volatile size_t	len = 1;

  if (CCE_EXCEPT == cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    cce_location_t	inner_L[1];

    if (cce_location(inner_L)) {
      if (cce_condition_is_errno(cce_condition(inner_L))) {
	CCE_PC(cce_condition_errno_t, C, cce_condition(inner_L));
	if (CCSYS_ERANGE == C->errnum) {
	  len *= 2;
	  cce_retry(L);
	}
      }
      cce_run_error_handlers_raise(inner_L, L);
    } else {
      char	buf[len];
      if (0) { fprintf(stderr, "%s: trying length: %lu\n", __func__, len); }
      ccsys_getcwd(inner_L, buf, len);
      if (0) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
      cce_run_cleanup_handlers(inner_L);
    }
    cce_run_cleanup_handlers(L);
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
  cce_cleanup_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char *	buf;

    buf = ccsys_get_current_dir_name(L);
    ccsys_handler_malloc_init(L, H, buf);

    if (0) { fprintf(stderr, "%s: current working directory: %s\n", __func__, buf); }
    cce_run_cleanup_handlers(L);
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
    cce_run_cleanup_handlers(L);
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
  cce_cleanup_handler_t	dirstream_H[1];

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
    cce_run_cleanup_handlers(L);
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
    cce_run_cleanup_handlers(L);
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
  cce_cleanup_handler_t   dirstream_H[1];
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
    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: creating and removing directories.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
{
  cce_location_t          L[1];
  cce_cleanup_handler_t   dirname_H[1];
  char const *            dirname = "name.d";

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_open_mode_t     mode;

    mode.data = CCSYS_S_IRWXU;
    ccsys_mkdir(L, dirname, mode);
    ccsys_handler_rmdir_init(L, dirname_H, dirname);
    cce_run_cleanup_handlers(L);
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
    cce_run_cleanup_handlers(L);
  }
}

void
test_3_3 (cce_destination_t upper_L)
/* Create a diirectory with "ccsys_mkdirat()". */
{
  cce_location_t	L[1];
  cce_handler_t		dirname1_H[1];
  cce_handler_t		dir_H[1];

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
      ccsys_cleanup_handler_rmdir_init(L, dirname1_H, dirname1);
    }

    /* Open the parent directory.  The descriptor in "dirfd1" is released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dir;

      dir = ccsys_opendir(L, dirname1);
      ccsys_cleanup_handler_dirstream_init(L, dir_H, dir);
      dirfd1 = ccsys_dirfd(L, dir);
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

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** File system: inspecting attributes with stat.
 ** ----------------------------------------------------------------- */

void
test_4_1 (cce_destination_t upper_L)
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filename_H[1];
  cce_cleanup_handler_t   filedes_H[1];

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

      ccsys_stat(L, filename, &S);
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_DEV)
      fprintf(stderr, "%s: st_dev=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_dev(&S).data);
#endif
#if (1 == CCSYS_HAVE_STRUCT_STAT_ST_INO)
      fprintf(stderr, "%s: st_ino=%lu\n", __func__, (unsigned long)ccsys_ref_stat_st_ino(&S).data);
#endif
    }

    cce_run_cleanup_handlers(L);
  }
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
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
