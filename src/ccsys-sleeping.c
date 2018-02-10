/*
  Part of: CCSys
  Contents: sleeping
  Date: Feb 10, 2018

  Abstract

	Sleeping functions.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you  can redistribute it and/or modify it under
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

#include "ccsys-internals.h"
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif


/** --------------------------------------------------------------------
 ** Sleeping functions.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SLEEP
ccsys_seconds_t
ccsys_sleep (ccsys_seconds_t seconds)
{
  ccsys_seconds_t	leftover;
  unsigned int		rv;
  rv = sleep(ccsys_dref(seconds));
  leftover.data = rv;
  return leftover;
}
#endif

/* end of file */
