/*
  Part of: CCSys
  Contents: tests for POSIX system functions, system/program interface
  Date: Feb  6, 2018

  Abstract

	Tests for POSIX system functions, basic system/program interface

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
 ** Baseic system/program interface: environment variables.
 ** ----------------------------------------------------------------- */

void
test_1_1_1 (cce_destination_t upper_L)
/* Test for "ccsys_getenv()". */
{
#if (defined HAVE_GETENV)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	path;

    path = ccsys_getenv(L, "PATH");
    cctests_assert(L, NULL != path);
    if (1) { fprintf(stderr, "%s: PATH=%s\n", __func__, path); }
    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_1_1_2 (cce_destination_t upper_L)
/* Test for "ccsys_getenv()". */
{
#if (defined HAVE_GETENV)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	var;

    var = ccsys_getenv(L, "IDONOTEXIST");
    cctests_assert(L, NULL == var);
    cce_run_cleanup_handlers(L);
  }
#endif
}

/* ------------------------------------------------------------------ */

void
test_1_2_1 (cce_destination_t upper_L)
/* Test for "ccsys_getenv()". */
{
#if (defined HAVE_SECURE_GETENV)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	path;

    path = ccsys_secure_getenv(L, "PATH");
    cctests_assert(L, NULL != path);
    if (1) { fprintf(stderr, "%s: PATH=%s\n", __func__, path); }
    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_1_2_2 (cce_destination_t upper_L)
/* Test for "ccsys_secure_getenv()". */
{
#if (defined HAVE_SECURE_GETENV)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	var;

    var = ccsys_secure_getenv(L, "IDONOTEXIST");
    cctests_assert(L, NULL == var);
    cce_run_cleanup_handlers(L);
  }
#endif
}


int
main (void)
{
  cctests_init("basic system/program interface");
  {
    cctests_begin_group("environment variables");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_1_2);
      cctests_run(test_1_2_1);
      cctests_run(test_1_2_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
