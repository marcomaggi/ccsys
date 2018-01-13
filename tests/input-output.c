/*
  Part of: CCSys
  Contents: tests for POSIX system functions, input/output
  Date: Jan 10, 2018

  Abstract

	Tests for POSIX system functions, input/output.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include "cctests.h"
#include "ccsys.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/** --------------------------------------------------------------------
 ** Opening a file.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L CCSYS_UNUSED)
/* Open a file with "ccsys_open()". */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_open_flags_t	flags = { .data = CCSYS_O_CREAT };
    ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRUSR | CCSYS_S_IWUSR };
    ccsys_fd_t		fd;
    fd = ccsys_open(L, filename, flags, mode);
    ccsys_handler_filedes_init(L, H, fd);
    cctests_assert(0 != fd.data);
    ccsys_remove(L, filename);
    cce_run_cleanup_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_1 (cce_destination_t upper_L)
/* Open a file with "ccsys_openat()". */
{
  cce_location_t		L[1];
  cce_cleanup_handler_t		dir_H[1];
  cce_cleanup_handler_t		dirstream_H[1];
  cce_cleanup_handler_t		file_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const * filename = "name.ext";
    static char const *	dirname  = "name.d";
    ccsys_dirfd_t	dirfd;

    /* Create the directory. */
    {
      ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRWXU };
      ccsys_mkdir(L, dirname, mode);
      ccsys_handler_rmdir_init(L, dir_H, dirname);
    }

    /* Open  the  directory.   The  descriptor in  "dirfd"  is  released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dirstream;

      dirstream = ccsys_opendir(L, dirname);
      ccsys_handler_dirstream_init(L, dirstream_H, dirstream);
      dirfd = ccsys_dirfd(L, dirstream);
    }

    /* Open the file. */
    {
      ccsys_open_flags_t    flags = { .data = CCSYS_O_CREAT };
      ccsys_open_mode_t     mode  = { .data = CCSYS_S_IRUSR | CCSYS_S_IWUSR };
      ccsys_fd_t            fd;

      fd = ccsys_openat(L, dirfd, filename, flags, mode);
      ccsys_handler_filedes_init(L, file_H, fd);
      cctests_assert(0 != fd.data);
    }

    /* Remove the file. */
    {
      ccsys_unlinkat_flags_t      flags = { .data = 0 };

      ccsys_unlinkat(L, dirfd, filename, flags);
    }

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: pipes.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L CCSYS_UNUSED)
/* Open a pipe with "ccsys_pipe()". */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_fd_t    pipefd[2];
    ccsys_pipe(L, pipefd);
    ccsys_handler_pipedes_init(L, H, pipefd);
    cce_run_cleanup_handlers(L);
  }
}



int
main (void)
{
  cctests_init("input/output");
  {
    cctests_begin_group("file open");
    {
      cctests_run(test_1_1);
      cctests_run(test_2_1);
    }
    cctests_end_group();

    cctests_begin_group("file pipes");
    {
      cctests_run(test_3_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
