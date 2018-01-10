/*
  Part of: CCSys
  Contents: tests for POSIX system functions, input/output
  Date: Jan 10, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccexceptions.h"
#include "ccsys.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/** --------------------------------------------------------------------
 ** Opening and closing a file.
 ** ----------------------------------------------------------------- */

void
test_1_1 (void)
{
  cce_location_t  L[1];
  cce_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_final(L);
  } else {
    ccsys_open_flags_t    flags = { .data = CCSYS_O_CREAT };
    ccsys_open_mode_t     mode  = { .data = CCSYS_S_IRWXU };
    ccsys_fd_t            fd;
    fd = ccsys_open(L, "name.ext", flags, mode);
    ccsys_cleanup_handler_filedes_init(L, H, fd);
    cce_run_cleanup_handlers(L);
  }

  if (cce_location(L)) {
    cce_run_error_handlers_final(L);
  } else {
    ccsys_remove(L, "name.ext");
  }
}


int
main (void)
{
  if (1) { test_1_1(); }

  exit(EXIT_SUCCESS);
}

/* end of file */
