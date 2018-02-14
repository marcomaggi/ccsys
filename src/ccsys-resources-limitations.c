/*
  Part of: CCSys
  Contents: interface for resources and limitations functions
  Date: Wed Feb 14, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#include "ccsys-internals.h"
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif


/** --------------------------------------------------------------------
 ** Resources and limitations.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SYSCONF
long
ccsys_sysconf (cce_destination_t L, ccsys_sysconf_flag_enum_t name)
{
  long	rv;
  errno = 0;
  rv = sysconf(name);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

/* end of file */
