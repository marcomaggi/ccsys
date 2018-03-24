/*
  Part of: CCTests
  Contents: assertion functions
  Date: Jan 20, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it  under the  terms  of  the GNU  Lesser  General  Public License  as
  published by  the Free  Software Foundation, either  version 3  of the
  License, or (at your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "cctests-internals.h"
#ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif


/** --------------------------------------------------------------------
 ** Calling a function in a subprocess.
 ** ----------------------------------------------------------------- */

void
cctests_call_in_forked_process (cce_destination_t L, cctests_child_process_function_t * child_function)
/* Fork a process:
 *
 * - In  the  child:  run  "child_function"; exit  successfully  if  the
 *   function  returns;  exit  with  failure  if  the  child  raises  an
 *   exception.
 *
 * - In  the parent:  wait for  the child  termination; assert  that the
 *   child process terminated successfully.
 */
{
#if ((defined HAVE_FORK) && (defined HAVE_WAITPID))
  pid_t	pid;

  pid = fork();
  if (0 != pid) {
    int		options = 0;
    int		wstatus = 0;

    waitpid(pid, &wstatus, options);
    if (! WIFEXITED(wstatus)) {
      cce_raise(L, cctests_condition_new_child_abnormal_termination());
    }
    if (0 != WEXITSTATUS(wstatus)) {
      cce_raise(L, cctests_condition_new_child_failure_exit_status());
    }
  } else {
    cce_location_t	inner_L[1];
    volatile int	status;

    if (cce_location(inner_L)) {
      status = EXIT_FAILURE;
      cce_run_error_handlers_final(inner_L);
    } else {
      child_function(inner_L);
      status = EXIT_SUCCESS;
      cce_run_cleanup_handlers(inner_L);
    }
    /* Terminate the child process. */
    _exit(status);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Calling functions in parent and child processes.
 ** ----------------------------------------------------------------- */

void
cctests_with_parent_and_child_process (cce_destination_t upper_L,
				       cctests_parent_process_function_t * parent_function,
				       cctests_child_process_function_t * child_function)
/* Fork a process:
 *
 * - In  the  child:  run  "child_function"; exit  successfully  if  the
 *   function  returns;  exit  with  failure  if  the  child  raises  an
 *   exception.
 *
 * - In  the   parent:  run   "parent_function";  wait  for   the  child
 *   termination; assert that the child process terminated successfully;
 *   assert that the parent function returned successfully.
 */
{
#if ((defined HAVE_FORK) && (defined HAVE_WAITPID))
  pid_t	pid;

  pid = fork();
  if (0 != pid) {
    /* Here we are in the parent.  Run the parent function. */
    {
      cce_location_t	L[1];

      if (cce_location(L)) {
	/* The parent  function raised an  exception.  Now wait  for the
	   child  process.   When  the  parent fails:  we  ignore  child
	   failures. */
	int	options = 0;
	int	wstatus = 0;

	waitpid(pid, &wstatus, options);
	cce_run_error_handlers_raise(L, upper_L);
      } else {
	parent_function(L, (int64_t)pid);
	cce_run_cleanup_handlers(L);
      }
    }

    /* The parent  function terminated  successfully.  Now wait  for the
       child process. */
    {
      int	options = 0;
      int	wstatus = 0;

      waitpid(pid, &wstatus, options);
      if (! WIFEXITED(wstatus)) {
	cce_raise(upper_L, cctests_condition_new_child_abnormal_termination());
      }
      if (0 != WEXITSTATUS(wstatus)) {
	cce_raise(upper_L, cctests_condition_new_child_failure_exit_status());
      }
    }
  } else {
    /* Here we are in the child. */
    cce_location_t	inner_L[1];
    int volatile	status;

    if (cce_location(inner_L)) {
      status = EXIT_FAILURE;
      cce_run_error_handlers_final(inner_L);
    } else {
      child_function(inner_L);
      status = EXIT_SUCCESS;
      cce_run_cleanup_handlers(inner_L);
    }
    /* Terminate the child process. */
    _exit(status);
  }
#endif
}

/* end of file */
