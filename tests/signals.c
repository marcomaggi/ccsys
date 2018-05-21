/*
  Part of: CCSys
  Contents: tests for POSIX system functions, interprocess signals
  Date: Feb  9, 2018

  Abstract

	Tests for POSIX system functions, interprocess signals.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include "ccsys.h"
#include "cctests.h"
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <string.h>


/** --------------------------------------------------------------------
 ** Interprocess signals: delivering signals.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Single signal delivery". */
{
#if (defined HAVE_SIGNAL_H)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_signum_t	signum;
    bool		flag;

    signum.data = CCSYS_SIGUSR1;
    ccsys_raise(L, signum);
    ccsys_signal_bub_acquire();
    flag = ccsys_signal_bub_delivered(signum);
    cctests_assert(L, true == flag);

    cce_run_clean_handlers(L);
  }
#else
  cctests_skip();
#endif
}

void
test_1_2 (cce_destination_t upper_L)
/* Multiple signals close together are merged into one. */
{
#if (defined HAVE_SIGNAL_H)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_signum_t	signum;
    bool		flag;

    signum.data = CCSYS_SIGUSR2;
    ccsys_raise(L, signum);
    ccsys_raise(L, signum);
    ccsys_raise(L, signum);
    ccsys_raise(L, signum);
    ccsys_signal_bub_acquire();
    flag = ccsys_signal_bub_delivered(signum);
    cctests_assert(L, true == flag);

    cce_run_clean_handlers(L);
  }
#else
  cctests_skip();
#endif
}

void
test_1_3 (cce_destination_t upper_L)
/* Different signals acquired. */
{
#if (defined HAVE_SIGNAL_H)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_signum_t	signum1, signum2;
    bool		flag1, flag2;

    signum1.data = CCSYS_SIGUSR1;
    signum2.data = CCSYS_SIGUSR2;

    ccsys_raise(L, signum1);
    ccsys_raise(L, signum2);

    ccsys_signal_bub_acquire();
    flag1 = ccsys_signal_bub_delivered(signum1);
    flag2 = ccsys_signal_bub_delivered(signum2);

    cctests_assert(L, true == flag1);
    cctests_assert(L, true == flag2);

    cce_run_clean_handlers(L);
  }
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** Interprocess signals: sending signals.
 ** ----------------------------------------------------------------- */

static void
test_2_1_parent (cce_destination_t L, int64_t child_pid)
{
  ccsys_pid_t		pid = { .data = child_pid };
  ccsys_signum_t	signum;

  signum.data = CCSYS_SIGUSR1;
  ccsys_kill(L, pid, signum);
}

static void
test_2_1_child (cce_destination_t L)
{
  {
    ccsys_seconds_t	one = { .data = 1 };
    ccsys_sleep(one);
  }
  {
    ccsys_signum_t	signum;
    bool		flag;

    signum.data = CCSYS_SIGUSR1;
    ccsys_signal_bub_acquire();
    flag = ccsys_signal_bub_delivered(signum);
    cctests_assert(L, true == flag);
  }
}

void
test_2_1 (cce_destination_t upper_L)
/* Send a signal to a subprocess". */
{
#if (defined HAVE_SIGNAL_H)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    cctests_with_parent_and_child_process(L, test_2_1_parent, test_2_1_child);
    cce_run_clean_handlers(L);
  }
#else
  cctests_skip();
#endif
}


int
main (void)
{
  cctests_init("interprocess signals");
  ccsys_signal_bub_init ();
  {
    cctests_begin_group("delivering signals");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
    }
    cctests_end_group();

    cctests_begin_group("sending signals");
    {
      cctests_run(test_2_1);
    }
    cctests_end_group();
  }
  ccsys_signal_bub_final();
  cctests_final();
}

/* end of file */
