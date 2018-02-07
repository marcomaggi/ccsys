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
#include <string.h>


/** --------------------------------------------------------------------
 ** Environment variables: getenv.
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
#else
  cctests_skip();
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
#else
  cctests_skip();
#endif
}

/* ------------------------------------------------------------------ */

void
test_1_2_1 (cce_destination_t upper_L)
/* Test for "ccsys_secure_getenv()". */
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
#else
  cctests_skip();
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
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** Environment variables: setenv, putenv.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
/* Test for "ccsys_setenv()". */
{
#if ((defined HAVE_SETENV) && (defined HAVE_GETENV))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	ciao;

    {
      ccsys_setenv(L, "CIAO", "123", true);

      ciao = ccsys_getenv(L, "CIAO");
      cctests_assert(L, NULL != ciao);
      cctests_assert(L, 0 == strncmp("123", ciao, strlen("123")));
      if (1) { fprintf(stderr, "%s: CIAO=%s\n", __func__, ciao); }
    }

    /* Call setenv without overwriting. */
    {
      ccsys_setenv(L, "CIAO", "456", false);

      ciao = ccsys_getenv(L, "CIAO");
      cctests_assert(L, NULL != ciao);
      cctests_assert(L, 0 == strncmp("123", ciao, strlen("123")));
      if (1) { fprintf(stderr, "%s: CIAO=%s\n", __func__, ciao); }
    }

    /* Call setenv overwriting. */
    {
      ccsys_setenv(L, "CIAO", "789", true);

      ciao = ccsys_getenv(L, "CIAO");
      cctests_assert(L, NULL != ciao);
      cctests_assert(L, 0 == strncmp("789", ciao, strlen("123")));
      if (1) { fprintf(stderr, "%s: CIAO=%s\n", __func__, ciao); }
    }

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
#endif
}

void
test_2_2 (cce_destination_t upper_L)
/* Test for "ccsys_putenv()". */
{
#if ((defined HAVE_PUTENV) && (defined HAVE_GETENV))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	hey;

    {
      ccsys_putenv(L, "HEY=123");

      hey = ccsys_getenv(L, "HEY");
      cctests_assert(L, NULL != hey);
      cctests_assert(L, 0 == strncmp("123", hey, strlen("123")));
      if (1) { fprintf(stderr, "%s: HEY=%s\n", __func__, hey); }
    }

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** Environment variables: unsetenv.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
/* Test for "ccsys_unsetenv()". */
{
#if ((defined HAVE_UNSETENV) && (defined HAVE_SETENV) && (defined HAVE_GETENV))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	ciao;

    ccsys_setenv(L, "CIAO", "123", true);
    ccsys_unsetenv(L, "CIAO");
    ciao = ccsys_getenv(L, "CIAO");
    cctests_assert(L, NULL == ciao);

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
#endif
}

void
test_3_2 (cce_destination_t upper_L)
/* Test for "ccsys_clearenv()". */
{
#if ((defined HAVE_UNSETENV) && (defined HAVE_SETENV) && (defined HAVE_GETENV))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	ciao;

    ccsys_setenv(L, "CIAO", "123", true);
    ccsys_clearenv(L);
    ciao = ccsys_getenv(L, "CIAO");
    cctests_assert(L, NULL == ciao);

    cce_run_cleanup_handlers(L);
  }
#else
  cctests_skip();
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
      cctests_run(test_2_1);
      cctests_run(test_2_2);
      cctests_run(test_3_1);
      cctests_run(test_3_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
