/*
  Part of: CCSys
  Contents: basic program/system interface
  Date: Tue Feb  6, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can  redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the
  Free Software  Foundation; either version  3.0 of the License,  or (at
  your option) any later version.

  This library  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See the  GNU
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
#ifdef HAVE_SYS_AUXV_H
#  include <sys/auxv.h>
#endif


/** --------------------------------------------------------------------
 ** Basic system/program interface: environment variables.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETENV
char const *
ccsys_getenv (cce_destination_t L, char const * name)
{
  char const *	rv;
  errno = 0;
  rv = getenv(name);
  if (0 != errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_SECURE_GETENV
char const *
ccsys_secure_getenv (cce_destination_t L, char const * name)
{
  char const *	rv;
  errno = 0;
  rv = secure_getenv(name);
  if (0 != errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_SETENV
void
ccsys_setenv (cce_destination_t L, char const * name, char const * value, bool replace)
{
  int	rv;
  errno = 0;
  rv = setenv(name,value,(int)replace);
  if (0 != rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_UNSETENV
void
ccsys_unsetenv (cce_destination_t L, char const * name)
{
  int	rv;
  errno = 0;
  rv = unsetenv(name);
  if (0 != rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_PUTENV
void
ccsys_putenv (cce_destination_t L, char const * assignment)
{
  int	rv;
  errno = 0;
  rv = putenv((char *)assignment);
  if (0 != rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_CLEARENV
void
ccsys_clearenv (cce_destination_t L)
{
  int	rv;
  errno = 0;
  rv = clearenv();
  if (0 != rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Basic system/program interface: retrieving the auxiliary vector.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETAUXVAL
unsigned long
ccsys_getauxval (cce_destination_t L, ccsys_getauxval_type_t type)
{
  unsigned long		rv;
  errno = 0;
  rv = getauxval(type.data);
  if ((0 == rv) && (0 != errno)) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

/* end of file */
