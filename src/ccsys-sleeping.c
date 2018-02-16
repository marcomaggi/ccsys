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
#ifdef HAVE_TIME_H
#  include <time.h>
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

#ifdef HAVE_NANOSLEEP
ccsys_timespec_t
ccsys_nanosleep (cce_destination_t L, ccsys_timespec_t const requested)
{
  struct timespec	requested_ts = {
    .tv_sec	= (time_t) ccsys_ldref(requested.seconds),
    .tv_nsec	=          ccsys_ldref(requested.nanoseconds)
  };
  struct timespec	leftover_ts  = {
    .tv_sec	= (time_t) 0,
    .tv_nsec	= 0
  };
  int			rv;
  errno = 0;
  rv = nanosleep(&requested_ts, &leftover_ts);
  if (0 == rv) {
    /* Success. */
    ccsys_timespec_t	leftover = {
      .seconds.data	= 0,
      .nanoseconds.data	= 0
    };
    return leftover;
  } else if ((-1 == rv) && (CCSYS_EINTR == errno)) {
    /* The call was interrupted by a signal handler. */
    ccsys_timespec_t	leftover = {
      .seconds.data	= (ccsys_time_unit_t)(leftover_ts.tv_sec),
      .nanoseconds.data	= (ccsys_time_unit_t)(leftover_ts.tv_nsec)
    };
    return leftover;
  } else {
    /* Failure. */
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* end of file */
