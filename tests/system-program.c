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
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	path;

    path = ccsys_getenv(L, "PATH");
    cctests_assert(L, NULL != path);
    if (1) { fprintf(stderr, "%s: PATH=%s\n", __func__, path); }
    cce_run_body_handlers(L);
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
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	var;

    var = ccsys_getenv(L, "IDONOTEXIST");
    cctests_assert(L, NULL == var);
    cce_run_body_handlers(L);
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
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	path;

    path = ccsys_secure_getenv(L, "PATH");
    cctests_assert(L, NULL != path);
    if (1) { fprintf(stderr, "%s: PATH=%s\n", __func__, path); }
    cce_run_body_handlers(L);
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
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	var;

    var = ccsys_secure_getenv(L, "IDONOTEXIST");
    cctests_assert(L, NULL == var);
    cce_run_body_handlers(L);
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
    cce_run_catch_handlers_raise(L, upper_L);
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

    cce_run_body_handlers(L);
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
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	hey;

    {
      ccsys_putenv(L, "HEY=123");

      hey = ccsys_getenv(L, "HEY");
      cctests_assert(L, NULL != hey);
      cctests_assert(L, 0 == strncmp("123", hey, strlen("123")));
      if (1) { fprintf(stderr, "%s: HEY=%s\n", __func__, hey); }
    }

    cce_run_body_handlers(L);
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
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	ciao;

    ccsys_setenv(L, "CIAO", "123", true);
    ccsys_unsetenv(L, "CIAO");
    ciao = ccsys_getenv(L, "CIAO");
    cctests_assert(L, NULL == ciao);

    cce_run_body_handlers(L);
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
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	ciao;

    ccsys_setenv(L, "CIAO", "123", true);
    ccsys_clearenv(L);
    ciao = ccsys_getenv(L, "CIAO");
    cctests_assert(L, NULL == ciao);

    cce_run_body_handlers(L);
  }
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** Accessing the auxiliary vector.
 ** ----------------------------------------------------------------- */

void
test_4_1 (cce_destination_t upper_L)
/* Test for "ccsys_getauxval()". */
{
#if (defined HAVE_GETAUXVAL)
  cce_location_t	L[1];

#ifdef CCSYS_AT_BASE_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_BASE);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_BASE=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_BASE_PLATFORM_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_BASE_PLATFORM);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_BASE_PLATFORM=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_CLKTCK_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_CLKTCK);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_CLKTCK=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_DCACHEBSIZE_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_DCACHEBSIZE);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_DCACHEBSIZE=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_EGID_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_EGID);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_EGID=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_ENTRY_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_ENTRY);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_ENTRY=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_EUID_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_EUID);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_EUID=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_EXECFD_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_EXECFD);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_EXECFD=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_EXECFN_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_EXECFN);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_EXECFN=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_FLAGS_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_FLAGS);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_FLAGS=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_FPUCW_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_FPUCW);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_FPUCW=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_GID_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_GID);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_GID=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_HWCAP_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_HWCAP);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_HWCAP=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_HWCAP2_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_HWCAP2);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_HWCAP2=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_ICACHEBSIZE_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_ICACHEBSIZE);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_ICACHEBSIZE=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_PAGESZ_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_PAGESZ);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_PAGESZ=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_PHDR_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_PHDR);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_PHDR=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_PHENT_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_PHENT);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_PHENT=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_PHNUM_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_PHNUM);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_PHNUM=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_PLATFORM_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_PLATFORM);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_PLATFORM=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_RANDOM_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_RANDOM);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_RANDOM=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_SECURE_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_SECURE);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_SECURE=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_SYSINFO_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_SYSINFO);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_SYSINFO=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_SYSINFO_EHDR_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_SYSINFO_EHDR);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_SYSINFO_EHDR=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_UCACHEBSIZE_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_UCACHEBSIZE);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_UCACHEBSIZE=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

#ifdef CCSYS_AT_UID_MACRO
  {
    if (cce_location(L)) {
      if (cce_condition_is_errno(cce_condition(L)) && (CCSYS_ENOENT == cce_condition_ref_errno_errnum(cce_condition(L)))) {
	/* The item is not in the auxiliary vector.  Fine. */
	cce_run_body_handlers(L);
      } else {
	if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
	cce_run_catch_handlers_raise(L, upper_L);
      }
    } else {
      ccsys_getauxval_type_t	type = ccsys_dnew(CCSYS_AT_UID);
      unsigned long	rv = ccsys_getauxval(L, type);
      fprintf(stderr, "%s: AT_UID=%lu\n", __func__, rv);
      cce_run_body_handlers(L);
    }
  }
#endif

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

    cctests_begin_group("accessing the auxiliary vector");
    {
      cctests_run(test_4_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
