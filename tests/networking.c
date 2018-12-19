/*
  Part of: CCSys
  Contents: tests for POSIX system functions, networking
  Date: Oct 14, 2018

  Abstract

	Tests for POSIX system functions, networking.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccsys.h"
#include <cctests.h>


/** --------------------------------------------------------------------
 ** Networking: opening a file.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Open a file with "ccsys_open()". */
{
  cce_location_t	  L[1];
  cce_clean_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccstructs_pathname_I	filename = ccstructs_new_pathname_from_static_string("input-output-name.ext");
    ccsys_open_flags_t		flags = ccsys_new_open_flags(CCSYS_O_CREAT);
    ccsys_open_mode_t		mode  = ccsys_new_open_mode(CCSYS_S_IRUSR | CCSYS_S_IWUSR);
    ccsys_fd_t			fd;

    fd = ccsys_open(L, filename, flags, mode);
    ccsys_init_filedes_handler(L, H, fd);
    cctests_assert(L, 0 != fd.data);
    ccsys_remove(L, filename);
    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  cctests_init("networking");
  {
    cctests_begin_group("file open");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
