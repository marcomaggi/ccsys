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

#include "ccexceptions.h"
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
  cce_location_t  L[1];
  cce_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_open_flags_t    flags = { .data = CCSYS_O_CREAT };
    ccsys_open_mode_t     mode  = { .data = CCSYS_S_IRUSR | CCSYS_S_IWUSR };
    ccsys_fd_t            fd;
    fd = ccsys_open(L, "name.ext", flags, mode);
    ccsys_cleanup_handler_filedes_init(L, H, fd);
    cctests_assert(0 != fd.data);
    cce_run_cleanup_handlers(L);
  }

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_remove(L, "name.ext");
  }
}

/* ------------------------------------------------------------------ */

void
test_2_1 (cce_destination_t upper_L)
/* Open a file with "ccsys_openat()". */
{
  cce_location_t	L[1];
  cce_handler_t		tmpdir_H[1];
  cce_handler_t		dirstream_H[1];
  cce_handler_t		file_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	dirname = "name.d";
    ccsys_dirfd_t	dirfd;

    /* Create the directory. */
    {
      ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRWXU };
      ccsys_mkdir(L, dirname, mode);
      ccsys_cleanup_handler_tmpdir_init(L, tmpdir_H, dirname);
    }

    /* Open  the  directory.   The  descriptor in  "dirfd"  is  released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dirstream;

      dirstream = ccsys_opendir(L, dirname);
      ccsys_cleanup_handler_dirstream_init(L, dirstream_H, dirstream);
      dirfd = ccsys_dirfd(L, dirstream);
    }

    /* Open the file. */
    {
      ccsys_open_flags_t    flags = { .data = CCSYS_O_CREAT };
      ccsys_open_mode_t     mode  = { .data = CCSYS_S_IRUSR | CCSYS_S_IWUSR };
      ccsys_fd_t            fd;

      fd = ccsys_openat(L, dirfd, "name.ext", flags, mode);
      ccsys_cleanup_handler_filedes_init(L, file_H, fd);
      cctests_assert(0 != fd.data);
    }

    /* Remove the file. */
    {
      ccsys_unlinkat(L, dirfd, "name.ext", 0);
    }

    cce_run_cleanup_handlers(L);
  }
}


int
main (void)
{
  cctests_init("input/output");
  {
    cctests_begin_group("file open close");
    {
      cctests_run(test_1_1);
      cctests_run(test_2_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
