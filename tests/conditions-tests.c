/*
  Part of: CCExceptions
  Contents: tests for specific exceptional condition descriptors
  Date: Mar 24, 2017

  Abstract



  Copyright (C) 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can  redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the
  Free Software  Foundation; either version  3.0 of the License,  or (at
  your option) any later version.

  This library  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  Lesser General Public License for more details.

  You  should have  received a  copy of  the GNU  Lesser  General Public
  License along  with this library; if  not, write to  the Free Software
  Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
  USA.

*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>	// for INT_MAX
#include <assert.h>
#include <errno.h>
#include <string.h>


void
test_invalid_argument (void)
{
  cce_location_t	L[1];
  bool			error_flag = false;

  if (cce_location(L)) {
    assert(cce_condition_is_invalid_argument(cce_condition(L)));
    assert(0 == strcmp(__func__, cce_condition_invalid_argument(L)->funcname));
    cce_run_error_handlers(L);
    cce_condition_delete(cce_condition(L));
    error_flag = true;
  } else {
    cce_raise(L, cce_condition(cce_condition_new_invalid_argument(L, __func__, 1)));
    cce_run_cleanup_handlers(L);
  }
  assert(true == error_flag);
}


int
main (void)
{
  if (1) { test_invalid_argument(); }

  exit(EXIT_SUCCESS);
}

/* end of file */
