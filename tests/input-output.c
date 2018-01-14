/*
  Part of: CCSys
  Contents: tests for POSIX system functions, input/output
  Date: Jan 10, 2018

  Abstract

	Tests for POSIX system functions, input/output.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include "cctests.h"
#include "ccsys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Opening a file.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L CCSYS_UNUSED)
/* Open a file with "ccsys_open()". */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_open_flags_t	flags = { .data = CCSYS_O_CREAT };
    ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRUSR | CCSYS_S_IWUSR };
    ccsys_fd_t		fd;
    fd = ccsys_open(L, filename, flags, mode);
    ccsys_handler_filedes_init(L, H, fd);
    cctests_assert(0 != fd.data);
    ccsys_remove(L, filename);
    cce_run_cleanup_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_1 (cce_destination_t upper_L)
/* Open a file with "ccsys_openat()". */
{
  cce_location_t		L[1];
  cce_cleanup_handler_t		dir_H[1];
  cce_cleanup_handler_t		dirstream_H[1];
  cce_cleanup_handler_t		file_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const * filename = "name.ext";
    static char const *	dirname  = "name.d";
    ccsys_dirfd_t	dirfd;

    /* Create the directory. */
    {
      ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRWXU };
      ccsys_mkdir(L, dirname, mode);
      ccsys_handler_rmdir_init(L, dir_H, dirname);
    }

    /* Open  the  directory.   The  descriptor in  "dirfd"  is  released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dirstream;

      dirstream = ccsys_opendir(L, dirname);
      ccsys_handler_dirstream_init(L, dirstream_H, dirstream);
      dirfd = ccsys_dirfd(L, dirstream);
    }

    /* Open the file. */
    {
      ccsys_open_flags_t    flags = { .data = CCSYS_O_CREAT };
      ccsys_open_mode_t     mode  = { .data = CCSYS_S_IRUSR | CCSYS_S_IWUSR };
      ccsys_fd_t            fd;

      fd = ccsys_openat(L, dirfd, filename, flags, mode);
      ccsys_handler_filedes_init(L, file_H, fd);
      cctests_assert(0 != fd.data);
    }

    /* Remove the file. */
    {
      ccsys_unlinkat_flags_t      flags = { .data = 0 };

      ccsys_unlinkat(L, dirfd, filename, flags);
    }

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: pipes.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L CCSYS_UNUSED)
/* Open a pipe with "ccsys_pipe()". */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_fd_t    pipefd[2];
    ccsys_pipe(L, pipefd);
    ccsys_handler_pipedes_init(L, H, pipefd);
    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: fifos with "ccsys_mkfifo()".
 ** ----------------------------------------------------------------- */

static void test_4_1_parent (cce_destination_t upper_L, char const * fifoname);
static void test_4_1_child  (char const * fifoname);

void
test_4_1 (cce_destination_t upper_L)
/* Open a FIFO with "ccsys_mkfifo()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	fifo_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	fifoname = "fifo.ext";
    ccsys_pid_t		pid;

    /* Create the FIFO.  We will remove it upon exiting this context. */
    {
      ccsys_open_mode_t	mode;

      mode.data = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      ccsys_mkfifo(L, fifoname, mode);
      ccsys_handler_remove_init(L, fifo_H, fifoname);
    }

    pid = ccsys_fork(L);
    if (pid.data) {
      /* We are in  the parent process.  Here we read  from the FIFO and
	 wait for the child process. */
      test_4_1_parent(L, fifoname);

      /* Wait for the child process. */
      {
	ccsys_wait_options_t	options;
	int			wstatus;

	options.data = 0;
	ccsys_waitpid(L, pid, &wstatus, options);
      }
    } else {
      /* We are  in the child process.   Here we write something  to the
	 FIFO then exit. */
      test_4_1_child(fifoname);
    }
    cce_run_cleanup_handlers(L);
  }
}

void
test_4_1_parent (cce_destination_t upper_L, char const * fifoname)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	infd_H[1];
  ccsys_fd_t		infd;

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    /* Open the FIFO for reading. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_RDONLY;
      mode.data  = 0;
      if (1) { fprintf(stderr, "%s: open fifo for reading\n", __func__); }
      infd = ccsys_open(L, fifoname, flags, mode);
      if (1) { fprintf(stderr, "%s: open fifo for reading done\n", __func__); }
      ccsys_handler_filedes_init(L, infd_H, infd);
    }

    /* Read from the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      char	inbuf[len];
      if (1) { fprintf(stderr, "%s: reading from %s\n", __func__, fifoname); }
      ccsys_read (L, infd, inbuf, len);
      cctests_assert(0 == strncmp(inbuf, oubuf, len));
      if (1) { fprintf(stderr, "%s: read input='%s' \n", __func__, inbuf); }
    }

    cce_run_cleanup_handlers(L);
  }
}

void
test_4_1_child  (char const * fifoname)
{
  cce_location_t	L[1];
  ccsys_fd_t		oufd;
  cce_cleanup_handler_t	oufd_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_final(L);
  } else {
    /* Open the FIFO for writing. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_WRONLY;
      mode.data  = 0;
      if (1) { fprintf(stderr, "%s: open fifo for writing\n", __func__); }
      oufd = ccsys_open(L, fifoname, flags, mode);
      ccsys_handler_filedes_init(L, oufd_H, oufd);
    }

    /* Write to the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      if (1) { fprintf(stderr, "%s: writing to %s\n", __func__, fifoname); }
      ccsys_write(L, oufd, oubuf, len);
    }
    cce_run_cleanup_handlers(L);
  }

  /* Terminate the child process. */
  {
    ccsys_exit_status_t	status;
    status.data = CCSYS_EXIT_SUCCESS;
    ccsys_exit(status);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: fifos with "ccsys_mkfifoat()".
 ** ----------------------------------------------------------------- */

static void test_4_2_parent (cce_destination_t upper_L, ccsys_dirfd_t dirfd, char const * fifoname);
static void test_4_2_child  (ccsys_dirfd_t dirfd, char const * fifoname);

void
test_4_2 (cce_destination_t upper_L)
/* Open a FIFO with "ccsys_mkfifo()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	dir_H[1];
  cce_cleanup_handler_t	dirstream_H[1];
  cce_cleanup_handler_t	fifo_H[1];
  ccsys_at_link_t	fifo_unlink_data;

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	dirname  = "name.d";
    static char const *	fifoname = "fifo.ext";
    ccsys_dirfd_t	dirfd;
    ccsys_pid_t		pid;

    /* Create the directory. */
    {
      ccsys_open_mode_t	mode  = { .data = CCSYS_S_IRWXU };
      ccsys_mkdir(L, dirname, mode);
      ccsys_handler_rmdir_init(L, dir_H, dirname);
    }

    /* Open  the  directory.   The  descriptor in  "dirfd"  is  released
       automatically when "dirstream" is released. */
    {
      ccsys_dir_t *	dirstream;

      dirstream = ccsys_opendir(L, dirname);
      ccsys_handler_dirstream_init(L, dirstream_H, dirstream);
      dirfd = ccsys_dirfd(L, dirstream);
    }

    /* Create the FIFO.  We will remove it upon exiting this context. */
    {
      ccsys_open_mode_t	mode;

      mode.data = CCSYS_S_IRUSR | CCSYS_S_IWUSR | CCSYS_S_IXUSR;
      ccsys_mkfifoat(L, dirfd, fifoname, mode);
      fifo_unlink_data.dirfd	= dirfd;
      fifo_unlink_data.pathname	= fifoname;
      ccsys_handler_unlinkat_init(L, fifo_H, &fifo_unlink_data);
    }

    pid = ccsys_fork(L);
    if (pid.data) {
      /* We are in  the parent process.  Here we read  from the FIFO and
	 wait for the child process. */
      test_4_2_parent(L, dirfd, fifoname);

      /* Wait for the child process. */
      {
	ccsys_wait_options_t	options;
	int			wstatus;

	options.data = 0;
	ccsys_waitpid(L, pid, &wstatus, options);
      }
    } else {
      /* We are  in the child process.   Here we write something  to the
	 FIFO then exit. */
      test_4_2_child(dirfd, fifoname);
    }
    cce_run_cleanup_handlers(L);
  }
}

void
test_4_2_parent (cce_destination_t upper_L, ccsys_dirfd_t dirfd, char const * fifoname)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	infd_H[1];
  ccsys_fd_t		infd;

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    /* Open the FIFO for reading. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t	mode;

      flags.data = CCSYS_O_RDONLY;
      mode.data  = 0;
      if (1) { fprintf(stderr, "%s: open fifo for reading\n", __func__); }
      infd = ccsys_openat(L, dirfd, fifoname, flags, mode);
      if (1) { fprintf(stderr, "%s: open fifo for reading done\n", __func__); }
      ccsys_handler_filedes_init(L, infd_H, infd);
    }

    /* Read from the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      char		inbuf[len];
      if (1) { fprintf(stderr, "%s: reading from %s\n", __func__, fifoname); }
      ccsys_read (L, infd, inbuf, len);
      cctests_assert(0 == strncmp(inbuf, oubuf, len));
      if (1) { fprintf(stderr, "%s: read input='%s' \n", __func__, inbuf); }
    }

    cce_run_cleanup_handlers(L);
  }
}

void
test_4_2_child (ccsys_dirfd_t dirfd, char const * fifoname)
{
  cce_location_t	L[1];
  ccsys_fd_t		oufd;
  cce_cleanup_handler_t	oufd_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_final(L);
  } else {
    /* Open the FIFO for writing. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t	mode;

      flags.data = CCSYS_O_WRONLY;
      mode.data  = 0;
      if (1) { fprintf(stderr, "%s: open fifo for writing\n", __func__); }
      oufd = ccsys_openat(L, dirfd, fifoname, flags, mode);
      ccsys_handler_filedes_init(L, oufd_H, oufd);
    }

    /* Write to the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      if (1) { fprintf(stderr, "%s: writing to %s\n", __func__, fifoname); }
      ccsys_write(L, oufd, oubuf, len);
    }
    cce_run_cleanup_handlers(L);
  }

  /* Terminate the child process. */
  {
    ccsys_exit_status_t	status;
    status.data = CCSYS_EXIT_SUCCESS;
    ccsys_exit(status);
  }
}


int
main (void)
{
  cctests_init("input/output");
  {
    cctests_begin_group("file open");
    {
      cctests_run(test_1_1);
      cctests_run(test_2_1);
    }
    cctests_end_group();

    cctests_begin_group("file pipes");
    {
      cctests_run(test_3_1);
    }
    cctests_end_group();

    cctests_begin_group("file fifos");
    {
      cctests_run(test_4_1);
      cctests_run(test_4_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
