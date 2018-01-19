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
 ** File system: current working directory.
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
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
