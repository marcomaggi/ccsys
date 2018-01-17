/*
  Part of: CCSys
  Contents: time handling
  Date: Jan 17, 2018

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
#ifdef HAVE_TIME_H
#  include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
#endif


/** --------------------------------------------------------------------
 ** Struct conversion.
 ** ----------------------------------------------------------------- */

struct timeval
ccsys_timeval_to_timeval (ccsys_timeval_t in)
{
  struct timeval	ou = {
    ou.tv_sec	= (time_t)in.seconds,
    ou.tv_usec	= in.microseconds
  };
  return ou;
}

ccsys_timeval_t
ccsys_timeval_from_timeval (struct timeval in)
{
  ccsys_timeval_t	ou = {
    ou.seconds		= (ccsys_seconds_t)      in.tv_sec,
    ou.microseconds	= (ccsys_microseconds_t) in.tv_usec
  };
  return ou;
}

/* ------------------------------------------------------------------ */

struct timespec
ccsys_timespec_to_timespec (ccsys_timespec_t in)
{
  struct timespec	ou = {
    ou.tv_sec	= (time_t)in.seconds,
    ou.tv_nsec	= in.nanoseconds
  };
  return ou;
}

ccsys_timespec_t
ccsys_timespec_from_timespec (struct timespec in)
{
  ccsys_timespec_t	ou = {
    ou.seconds		= (ccsys_seconds_t)      in.tv_sec,
    ou.nanoseconds	= (ccsys_nanoseconds_t)  in.tv_nsec
  };
  return ou;
}

/* end of file */
