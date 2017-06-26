/*
  Part of: CCExceptions
  Contents: basic test with no exceptions
  Date: Fri Dec 23, 2016

  Abstract



  Copyright (C) 2016, 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

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


#include <ccexceptions.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int
main (int argc CCE_UNUSED, const char *const argv[] CCE_UNUSED)
{
  cce_location_t	L[1];
  bool			flag = false;

  if (cce_location(L)) {
    flag = false;
    cce_run_error_handlers(L);
  } else {
    flag = true;
    cce_run_cleanup_handlers(L);
  }
  assert(flag);

  exit(EXIT_SUCCESS);
}

/* end of file */
