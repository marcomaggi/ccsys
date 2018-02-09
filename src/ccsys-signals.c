/*
  Part of: Ccsys
  Contents: interprocess signals
  Date: Feb  9, 2018

  Abstract

	Handle interprocess signals with a simplified interface.

	This  code  is  derived  from code  originally  in  the  project
	CCEvents.

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
#ifdef HAVE_SIGNAL_H
#  include <signal.h>
#endif


/** --------------------------------------------------------------------
 ** Signal handler.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SIGNAL_H
/* An integer flag  for every signal.  When the BUB  API is initialised:
   every time a signal arrives the corresponding flag is set. */
static volatile sig_atomic_t	arrived_signals[NSIG];

static sigset_t	all_signals_set;
#endif

static void
signal_bub_handler (int signum)
/* The signal handler registered by the BUB API. */
{
#ifdef HAVE_SIGNAL_H
  arrived_signals[signum] = 1;
  if (0) { fprintf(stderr, "%s: %d\n", __func__, signum); }
#endif
}


/** --------------------------------------------------------------------
 ** Signal BUB API initialisation and finalisation.
 ** ----------------------------------------------------------------- */

void
ccsys_signal_bub_init (void)
/* Initialise  the BUB  API, enabling  signal handling.   Block all  the
   signals and register our handler for each.
*/
{
#ifdef HAVE_SIGNAL_H

  struct sigaction	ac = {
    .sa_handler	= signal_bub_handler,
    .sa_flags	= SA_RESTART | SA_NOCLDSTOP
  };
  int	signum;
  sigfillset(&all_signals_set);
  sigprocmask(SIG_BLOCK, &all_signals_set, NULL);
  for (signum=0; signum<NSIG; ++signum) {
    arrived_signals[signum] = 0;
    sigaction(signum, &ac, NULL);
  }
#endif
}

void
ccsys_signal_bub_final (void)
/* Finalise  the  BUB  API,  disabling signal  handling.   Set  all  the
   handlers to SIG_IGN, then unblock the signals.
*/
{
#ifdef HAVE_SIGNAL_H
  struct sigaction	ac = {
    .sa_handler	= SIG_IGN,
    .sa_flags	= SA_RESTART
  };
  for (int signum=0; signum<NSIG; ++signum) {
    arrived_signals[signum] = 0;
    sigaction(signum, &ac, NULL);
  }
  sigprocmask(SIG_UNBLOCK, &all_signals_set, NULL);
#endif
}

void
ccsys_signal_bub_acquire (void)
/* Unblock then block  all the signals.  This causes  blocked signals to
   be delivered.
*/
{
#ifdef HAVE_SIGNAL_H
  sigprocmask(SIG_UNBLOCK, &all_signals_set, NULL);
  sigprocmask(SIG_BLOCK,   &all_signals_set, NULL);
#endif
}

bool
ccsys_signal_bub_delivered (ccsys_signum_t signum)
{
#ifdef HAVE_SIGNAL_H
  bool	flag = (arrived_signals[signum.data])? true : false;
  arrived_signals[signum.data] = 0;
  return flag;
#endif
}


/** --------------------------------------------------------------------
 ** Interproces signals: raising signals.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_RAISE
void
ccsys_raise (cce_destination_t L, ccsys_signum_t signum)
{
  int	rv;
  errno = 0;
  rv = raise(signum.data);
  if (0 != rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_KILL
void
ccsys_kill (cce_destination_t L, ccsys_pid_t pid, ccsys_signum_t signum)
{
  int	rv;
  errno = 0;
  rv = kill(pid.data, signum.data);
  if (0 != rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* end of file */
