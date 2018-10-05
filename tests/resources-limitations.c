/*
  Part of: CCSys
  Contents: tests for POSIX system functions, resources and limitations
  Date: Feb 14, 2018

  Abstract

	Tests for POSIX system functions, resources and limitations.

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
 ** System configuration parameters.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccsys_sysconf()"". */
{
#if (defined HAVE_SYSCONF)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    long	rv;

    rv = ccsys_sysconf(L, CCSYS__SC_PAGESIZE);
    if (1) { fprintf(stderr, "%s: CCSYS__SC_PAGESIZE=%lu\n", __func__, rv); }

    cce_run_body_handlers(L);
  }
#else
  cctests_skip();
#endif
}

void
test_1_2 (cce_destination_t upper_L)
/* Test for "ccsys_confstr()"". */
{
#if ((defined HAVE_CONFSTR) && (defined CCSYS__CS_PATH_MACRO))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    size_t	len = 1024;
    char	buf[len];
    size_t	rv;

    memset(buf, 0, len);

    rv = ccsys_confstr(L, CCSYS__CS_PATH, buf, len);
    if (1) { fprintf(stderr, "%s: CCSYS__CS_PATH=%s\n", __func__, buf); }

    cctests_assert(L, 0 < rv);

    cce_run_body_handlers(L);
  }
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** File configuration parameters.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
/* Test for "ccsys_pathconf()"". */
{
#if (defined HAVE_PATHCONF)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    long		rv;

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_touch(L, filename, flags, mode);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    {
      rv = ccsys_pathconf(L, filename, CCSYS__PC_PATH_MAX);
      if (1) { fprintf(stderr, "%s: CCSYS__PC_PATH_MAX=%lu\n", __func__, rv); }
    }

    cce_run_body_handlers(L);
  }
#else
  cctests_skip();
#endif
}

void
test_2_2 (cce_destination_t upper_L)
/* Test for "ccsys_fpathconf()"". */
{
#if (defined HAVE_FPATHCONF)
  cce_location_t	L[1];
  cce_clean_handler_t	filename_H[1];
  cce_clean_handler_t	filedes_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	filename = "name.ext";
    ccsys_fd_t		fd;
    long		rv;

    /* Create the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_init_filedes_handler(L, filedes_H, fd);
      ccsys_handler_remove_init(L, filename_H, filename);
    }

    {
      rv = ccsys_fpathconf(L, fd, CCSYS__PC_PATH_MAX);
      if (1) { fprintf(stderr, "%s: CCSYS__PC_PATH_MAX=%lu\n", __func__, rv); }
    }

    cce_run_body_handlers(L);
  }
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** Resources usage.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
/* Test for "ccsys_getrusage()"". */
{
#if (defined HAVE_GETRUSAGE)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsys_rusage_t	S[1];

    ccsys_getrusage(L, CCSYS_RUSAGE_SELF, S);

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_UTIME) {
      ccsys_timeval_t	F = ccsys_ref_rusage_ru_utime(S);
      if (1) {
	fprintf(stderr, "%s: ru_utime.tv_sec  = %lu\n", __func__, ccsys_luref(F.seconds));
	fprintf(stderr, "%s: ru_utime.tv_usec = %lu\n", __func__, ccsys_luref(F.microseconds));
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_STIME) {
      ccsys_timeval_t	F = ccsys_ref_rusage_ru_stime(S);
      if (1) {
	fprintf(stderr, "%s: ru_stime.tv_sec  = %lu\n", __func__, ccsys_luref(F.seconds));
	fprintf(stderr, "%s: ru_stime.tv_usec = %lu\n", __func__, ccsys_luref(F.microseconds));
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_MAXRSS) {
      long	F = ccsys_ref_rusage_ru_maxrss(S);
      if (1) {
	fprintf(stderr, "%s: ru_maxrss = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_IXRSS) {
      long	F = ccsys_ref_rusage_ru_ixrss(S);
      if (1) {
	fprintf(stderr, "%s: ru_maxrss = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_IDRSS) {
      long	F = ccsys_ref_rusage_ru_idrss(S);
      if (1) {
	fprintf(stderr, "%s: ru_maxrss = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_ISRSS) {
      long	F = ccsys_ref_rusage_ru_isrss(S);
      if (1) {
	fprintf(stderr, "%s: ru_isrss = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_MINFLT) {
      long	F = ccsys_ref_rusage_ru_minflt(S);
      if (1) {
	fprintf(stderr, "%s: ru_minflt = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_MAJFLT) {
      long	F = ccsys_ref_rusage_ru_majflt(S);
      if (1) {
	fprintf(stderr, "%s: ru_majflt = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_NSWAP) {
      long	F = ccsys_ref_rusage_ru_nswap(S);
      if (1) {
	fprintf(stderr, "%s: ru_nswap = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_INBLOCK) {
      long	F = ccsys_ref_rusage_ru_inblock(S);
      if (1) {
	fprintf(stderr, "%s: ru_inblock = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_OUBLOCK) {
      long	F = ccsys_ref_rusage_ru_oublock(S);
      if (1) {
	fprintf(stderr, "%s: ru_oublock = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_MSGSND) {
      long	F = ccsys_ref_rusage_ru_msgsnd(S);
      if (1) {
	fprintf(stderr, "%s: ru_msgsnd = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_MSGRCV) {
      long	F = ccsys_ref_rusage_ru_msgrcv(S);
      if (1) {
	fprintf(stderr, "%s: ru_msgrcv = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_NSIGNALS) {
      long	F = ccsys_ref_rusage_ru_nsignals(S);
      if (1) {
	fprintf(stderr, "%s: ru_nsignals = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_NVCSW) {
      long	F = ccsys_ref_rusage_ru_nvcsw(S);
      if (1) {
	fprintf(stderr, "%s: ru_nvcsw = %ld\n", __func__, F);
      }
    }

    if (CCSYS_HAVE_STRUCT_RUSAGE_RU_NIVCSW) {
      long	F = ccsys_ref_rusage_ru_nivcsw(S);
      if (1) {
	fprintf(stderr, "%s: ru_nivcsw = %ld\n", __func__, F);
      }
    }

    cce_run_body_handlers(L);
  }
#else
  cctests_skip();
#endif
}


/** --------------------------------------------------------------------
 ** Resources limits.
 ** ----------------------------------------------------------------- */

void
test_4_1 (cce_destination_t upper_L)
/* Test for "ccsys_getrlimit()"". */
{
#if (defined HAVE_GETRLIMIT)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsys_rlimit_t	S[1];

    if (CCSYS_RLIMIT_AS_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_AS, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_AS: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_CORE_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_CORE, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_CORE: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_CPU_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_CPU, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_CPU: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_DATA_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_DATA, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_DATA: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_FSIZE_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_FSIZE, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_FSIZE: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_LOCKS_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_LOCKS, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_LOCKS: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_MEMLOCK_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_MEMLOCK, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_MEMLOCK: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_MSGQUEUE_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_MSGQUEUE, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_MSGQUEUE: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_NICE_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_NICE, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_NICE: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_NOFILE_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_NOFILE, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_NOFILE: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_NPROC_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_NPROC, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_NPROC: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_RSS_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_RSS, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_RSS: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_RTPRIO_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_RTPRIO, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_RTPRIO: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_RTTIME_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_RTTIME, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_RTTIME: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_SIGPENDING_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_SIGPENDING, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_SIGPENDING: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    if (CCSYS_RLIMIT_STACK_MACRO) {
      ccsys_getrlimit(L, CCSYS_RLIMIT_STACK, S);
      if (1) {
	fprintf(stderr, "%s: CCSYS_RLIMIT_STACK: cur=%lu, max=%lu\n", __func__,
		ccsys_luref(ccsys_ref_rlimit_rlim_cur(S)),
		ccsys_luref(ccsys_ref_rlimit_rlim_max(S)));
      }
    }

    cce_run_body_handlers(L);
  }
#else
  cctests_skip();
#endif
}


int
main (void)
{
  cctests_init("sleeping functions");
  {
    cctests_begin_group("System configuration parameters");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
    }
    cctests_end_group();

    cctests_begin_group("file configuration parameters");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
    }
    cctests_end_group();

    cctests_begin_group("resources usage");
    {
      cctests_run(test_3_1);
    }
    cctests_end_group();

    cctests_begin_group("resources limits");
    {
      cctests_run(test_4_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
