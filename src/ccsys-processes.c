/*
  Part of: CCSys
  Contents: running external processes
  Date: Sat Jul  1, 2017

  Abstract



  Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif


/** --------------------------------------------------------------------
 ** Running processes.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SYSTEM
int
ccsys_system (cce_location_t * L, const char * command)
{
  int	rv;
  errno = 0;
  rv = system(command);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif


/** --------------------------------------------------------------------
 ** Forking processes.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_FORK
ccsys_pid_t
ccsys_fork (cce_location_t * L)
{
  ccsys_pid_t	rv;
  errno = 0;
  rv.data = fork();
  if (-1 == rv.data) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_EXIT
void
ccsys_exit (ccsys_exit_status_t status)
{
  exit(status.data);
}
#endif


/** --------------------------------------------------------------------
 ** Executing processes.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_EXECV
void
ccsys_execv (cce_location_t * L, const char * filename, char * const argv [])
{
  int	rv;
  errno = 0;
  rv = execv(filename, argv);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_EXECVE
void
ccsys_execve (cce_location_t * L, const char * filename, char * const argv [], char * const env [])
{
  int	rv;
  errno = 0;
  rv = execve(filename, argv, env);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_EXECVP
void
ccsys_execvp (cce_location_t * L, const char * filename, char * const argv [])
{
  int	rv;
  errno = 0;
  rv = execvp(filename, argv);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Waiting for processes.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_WAITPID
void
ccsys_waitpid (cce_location_t * L, ccsys_pid_t pid, ccsys_waitpid_status_t * wstatus, ccsys_waitpid_options_t options)
{
  ccsys_pid_t	rv;
  errno = 0;
  rv.data = waitpid(pid.data, &(wstatus->data), options.data);
  if (-1 == rv.data) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#if (defined HAVE_WAITPID)
bool
ccsys_wifexited	(ccsys_waitpid_status_t  wstatus)
{
  return (WIFEXITED(wstatus))? true : false;
}
#endif

#if (defined HAVE_WAITPID)
uint8_t
ccsys_wexitstatus (ccsys_waitpid_status_t wstatus)
{
  return (uint8_t) WEXITSTATUS(wstatus);
}
#endif

/* ------------------------------------------------------------------ */

#if (defined HAVE_WAITPID)
bool
ccsys_wifsignaled (ccsys_waitpid_status_t  wstatus)
{
  return (WIFSIGNALED(wstatus))? true : false;
}
#endif

#if (defined HAVE_WAITPID)
ccsys_signum_t
ccsys_wtermsig (ccsys_waitpid_status_t  wstatus)
{
  ccsys_signum_t	signum = { .data = WTERMSIG(wstatus) };
  return signum;
}
#endif

/* ------------------------------------------------------------------ */

#if (defined HAVE_WAITPID)
bool
ccsys_wcoredump (ccsys_waitpid_status_t wstatus)
{
  return (WCOREDUMP(wstatus))? true : false;
}
#endif

/* ------------------------------------------------------------------ */

#if (defined HAVE_WAITPID)
bool
ccsys_wifstopped (ccsys_waitpid_status_t wstatus)
{
  return (WIFSTOPPED(wstatus))? true : false;
}
#endif

#if (defined HAVE_WAITPID)
ccsys_signum_t
ccsys_wstopsig (ccsys_waitpid_status_t wstatus)
{
  ccsys_signum_t	signum = { .data = WSTOPSIG(wstatus) };
  return signum;
}
#endif

/* end of file */
