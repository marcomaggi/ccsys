/*
  Part of: CCSys
  Contents: interface for resources and limitations functions
  Date: Wed Feb 14, 2018

  Abstract



  Copyright (C) 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#ifdef HAVE_SYS_RESOURCE_H
#  include <sys/resource.h>
#endif
#ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
#endif


/** --------------------------------------------------------------------
 ** Resources and limitations: system configuration parameters.
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

#ifdef HAVE_CONFSTR
size_t
ccsys_confstr (cce_destination_t L, ccsys_confstr_flag_enum_t name, char * bufptr, size_t buflen)
{
  size_t	rv;
  errno = 0;
  rv = confstr(name, bufptr, buflen);
  if (0 < rv) {
    /* The requested parameter "name" is valid and it has a value. */
    return rv;
  } else if ((0 == rv) && (0 == errno)) {
    /* The requested  parameter "name" is valid  but it does not  have a
       value. */
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Resources and limitations: file name configuration parameters.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_PATHCONF
long
ccsys_pathconf (cce_destination_t L, ccstructs_pathname_I ptn, ccsys_pathconf_flag_enum_t name)
{
  long	rv;
  errno = 0;
  rv = pathconf(ccstructs_pathname_asciiz(L, ptn).ptr, name);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_FPATHCONF
long
ccsys_fpathconf (cce_destination_t L, ccsys_fd_t fd, ccsys_pathconf_flag_enum_t name)
{
  long	rv;
  errno = 0;
  rv = fpathconf(fd.data, name);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif


/** --------------------------------------------------------------------
 ** Resources and limitations: resources usage.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETRUSAGE
void
ccsys_getrusage (cce_destination_t L, ccsys_getrusage_who_enum_t who, ccsys_rusage_t * S)
{
  int	rv;
  errno = 0;
  rv = getrusage((int)who, (struct rusage *) S);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_UTIME)
ccsys_timeval_t
ccsys_ref_rusage_ru_utime (ccsys_rusage_t const * const S)
{
  CCSYS_PC(struct rusage const, D, S);
  ccsys_timeval_t	T = {
    .seconds.data	= D->ru_utime.tv_sec,
    .microseconds.data	= D->ru_utime.tv_usec
  };

  return T;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_STIME)
ccsys_timeval_t
ccsys_ref_rusage_ru_stime (ccsys_rusage_t const * const S)
{
  CCSYS_PC(struct rusage const, D, S);
  ccsys_timeval_t	T = {
    .seconds.data	= D->ru_stime.tv_sec,
    .microseconds.data	= D->ru_stime.tv_usec
  };

  return T;
}
#endif

/* ------------------------------------------------------------------ */

#define CCSYS_STRUCT_RUSAGE_GETTER_LONG(FIELDNAME)			\
  long									\
  ccsys_ref_rusage_ ## FIELDNAME (ccsys_rusage_t const * const S)	\
  {									\
    CCSYS_PC(struct rusage const, D, S);				\
    return D->FIELDNAME;						\
  }

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_MAXRSS)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_maxrss)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_IXRSS)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_ixrss)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_IDRSS)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_idrss)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_ISRSS)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_isrss)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_MINFLT)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_minflt)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_MAJFLT)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_majflt)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_NSWAP)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_nswap)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_INBLOCK)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_inblock)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_OUBLOCK)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_oublock)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_MSGSND)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_msgsnd)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_MSGRCV)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_msgrcv)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_NSIGNALS)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_nsignals)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_NVCSW)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_nvcsw)
#endif

#if (1 == CCSYS_HAVE_STRUCT_RUSAGE_RU_NIVCSW)
CCSYS_STRUCT_RUSAGE_GETTER_LONG(ru_nivcsw)
#endif


/** --------------------------------------------------------------------
 ** Resources and limitations: resources limits.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETRLIMIT
void
ccsys_getrlimit (cce_destination_t L, ccsys_getrlimit_resource_enum_t resource, ccsys_rlimit_t * S)
{
  int	rv;
  errno = 0;
  rv = getrlimit((int)resource, (struct rlimit *) S);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#if (1 == CCSYS_HAVE_STRUCT_RLIMIT_RLIM_CUR)
ccsys_rlim_t
ccsys_ref_rlimit_rlim_cur (ccsys_rlimit_t const * const S)
{
  CCSYS_PC(struct rlimit const, D, S);
  ccsys_rlim_t	F = { .data = D->rlim_cur };
  return F;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_RLIMIT_RLIM_MAX)
ccsys_rlim_t
ccsys_ref_rlimit_rlim_max (ccsys_rlimit_t const * const S)
{
  CCSYS_PC(struct rlimit const, D, S);
  ccsys_rlim_t	F = { .data = D->rlim_max };
  return F;
}
#endif

/* end of file */
