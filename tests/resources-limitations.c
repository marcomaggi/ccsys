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
/* Test for "ccsys_syscon()"". */
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
  }
  cctests_final();
}

/* end of file */
