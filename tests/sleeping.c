/*
  Part of: CCSys
  Contents: tests for POSIX system functions, sleeping
  Date: Feb 10, 2018

  Abstract

	Tests for POSIX system functions, sleeping.

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
 ** Sleeping: high-resolution sleeping.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccsys_nanosleep()"". */
{
#if (defined HAVE_NANOSLEEP)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_timespec_t	requested = {
      .seconds.data	= 1,
      .nanoseconds.data	= 2
    };
    ccsys_timespec_t	leftover;

    leftover = ccsys_nanosleep(L, requested);
    cctests_assert(L, ccsys_timespec_is_zero(leftover));

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
    cctests_begin_group("High-resolution sleeping");
    {
      cctests_run(test_1_1);

    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
