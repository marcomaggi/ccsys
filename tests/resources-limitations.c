/*
  Part of: CCSys
  Contents: tests for POSIX system functions, resources and limitations
  Date: Feb 14, 2018

  Abstract

	Tests for POSIX system functions, resources and limitations.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include "ccsys.h"
#include "cctests.h"
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


/** --------------------------------------------------------------------
 ** System configuration parameters.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccsys_sysconf()"". */
{
#if (defined HAVE_SYSCONF)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    long	rv;

    rv = ccsys_sysconf(L, CCSYS__SC_PAGESIZE);
    if (1) { fprintf(stderr, "%s: CCSYS__SC_PAGESIZE=%lu\n", __func__, rv); }

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** File configuration parameters.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
/* Test for "ccsys_pathconf()"". */
{
#if (defined HAVE_PATHCONF)
  cce_location_t	L[1];
  cce_cleanup_handler_t	filename_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    long		rv;

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    {
      rv = ccsys_pathconf(L, filename, CCSYS__PC_PATH_MAX);
      if (1) { fprintf(stderr, "%s: CCSYS__PC_PATH_MAX=%lu\n", __func__, rv); }
    }

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
#endif
}

void
test_2_2 (cce_destination_t upper_L)
/* Test for "ccsys_fpathconf()"". */
{
#if (defined HAVE_FPATHCONF)
  cce_location_t	L[1];
  cce_cleanup_handler_t	filename_H[1];
  cce_cleanup_handler_t	filedes_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;
    long		rv;

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

    {
      rv = ccsys_fpathconf(L, fd, CCSYS__PC_PATH_MAX);
      if (1) { fprintf(stderr, "%s: CCSYS__PC_PATH_MAX=%lu\n", __func__, rv); }
    }

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
#endif
}


int
main (void)
{
  cctests_init("sleeping functions");
  {
    cctests_begin_group("System configuration parameters");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();

    cctests_begin_group("file configuration parameters");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
