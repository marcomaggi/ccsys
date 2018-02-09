/*
  Part of: CCSys
  Contents: tests for POSIX system functions, processes
  Date: Feb  9, 2018

  Abstract

	Tests for POSIX system functions, processes.

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
#include <string.h>


/** --------------------------------------------------------------------
 ** Environment variables: executing through the shell.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccsys_system()". */
{
#if (defined HAVE_GETENV)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_system(L, "whoami");

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
#endif
}


int
main (void)
{
  cctests_init("executing subprocesses");
  {
    cctests_begin_group("running through the shell");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();

  }
  cctests_final();
}

/* end of file */
