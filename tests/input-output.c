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
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Input/output: opening a file.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Open a file with "ccsys_open()". */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_open_flags_t	flags;
    ccsys_open_mode_t	mode;
    ccsys_fd_t		fd;

    flags.data = CCSYS_O_CREAT;
    mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
    fd = ccsys_open(L, filename, flags, mode);
    ccsys_handler_filedes_init(L, H, fd);
    cctests_assert(L, 0 != fd.data);
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
      cctests_assert(L, 0 != fd.data);
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
test_3_1 (cce_destination_t upper_L)
/* Open a pipe with "ccsys_pipe()". */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_fd_t    pipefd[2];

    /* Create the pipe. */
    {
      ccsys_pipe(L, pipefd);
      ccsys_handler_pipedes_init(L, H, pipefd);
    }

    /* Write to the pipe. */
    {
      char *	buf = "0123456789";
      size_t	len = strlen(buf);
      size_t	N;
      N = ccsys_write(L, pipefd[1], buf, len);
      cctests_assert(L, N == len);
    }

    /* Read from the pipe. */
    {
      size_t	len = 256;
      char	buf[len];
      size_t	N;
      N = ccsys_read(L, pipefd[0], buf, len);
      cctests_assert(L, N == strlen("0123456789"));
      cctests_assert(L, 0 == strncmp(buf, "0123456789", N));
    }

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2 (cce_destination_t upper_L)
/* Open a pipe with "ccsys_pipe2()". */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_fd_t		pipefd[2];

    /* Create the pipe. */
    {
      ccsys_open_flags_t	flags;
      flags.data = CCSYS_O_CLOEXEC;
      ccsys_pipe2(L, pipefd, flags);
      ccsys_handler_pipedes_init(L, H, pipefd);
    }

    /* Write to the pipe. */
    {
      char *	buf = "0123456789";
      size_t	len = strlen(buf);
      size_t	N;
      N = ccsys_write(L, pipefd[1], buf, len);
      cctests_assert(L, N == len);
    }

    /* Read from the pipe. */
    {
      size_t	len = 256;
      char	buf[len];
      size_t	N;
      N = ccsys_read(L, pipefd[0], buf, len);
      cctests_assert(L, N == strlen("0123456789"));
      cctests_assert(L, 0 == strncmp(buf, "0123456789", N));
    }

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
	ccsys_waitpid_options_t	options;
	ccsys_waitpid_status_t	wstatus;

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
      if (0) { fprintf(stderr, "%s: open fifo for reading\n", __func__); }
      infd = ccsys_open(L, fifoname, flags, mode);
      if (0) { fprintf(stderr, "%s: open fifo for reading done\n", __func__); }
      ccsys_handler_filedes_init(L, infd_H, infd);
    }

    /* Read from the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      char	inbuf[len];
      if (0) { fprintf(stderr, "%s: reading from %s\n", __func__, fifoname); }
      ccsys_read (L, infd, inbuf, len);
      cctests_assert(L, 0 == strncmp(inbuf, oubuf, len));
      if (0) { fprintf(stderr, "%s: read input='%s' \n", __func__, inbuf); }
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
      if (0) { fprintf(stderr, "%s: open fifo for writing\n", __func__); }
      oufd = ccsys_open(L, fifoname, flags, mode);
      ccsys_handler_filedes_init(L, oufd_H, oufd);
    }

    /* Write to the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      if (0) { fprintf(stderr, "%s: writing to %s\n", __func__, fifoname); }
      ccsys_write(L, oufd, oubuf, len);
    }
    cce_run_cleanup_handlers(L);
  }

  /* Terminate the child process. */
  {
    ccsys_exit_status_t	status;
    status.data = CCSYS_EXIT_SUCCESS;
    ccsys__exit(status);
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

      mode.data = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
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
	ccsys_waitpid_options_t	options;
	ccsys_waitpid_status_t	wstatus;

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
      if (0) { fprintf(stderr, "%s: open fifo for reading\n", __func__); }
      infd = ccsys_openat(L, dirfd, fifoname, flags, mode);
      if (0) { fprintf(stderr, "%s: open fifo for reading done\n", __func__); }
      ccsys_handler_filedes_init(L, infd_H, infd);
    }

    /* Read from the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      char		inbuf[len];
      if (0) { fprintf(stderr, "%s: reading from %s\n", __func__, fifoname); }
      ccsys_read (L, infd, inbuf, len);
      cctests_assert(L, 0 == strncmp(inbuf, oubuf, len));
      if (0) { fprintf(stderr, "%s: read input='%s' \n", __func__, inbuf); }
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
      if (0) { fprintf(stderr, "%s: open fifo for writing\n", __func__); }
      oufd = ccsys_openat(L, dirfd, fifoname, flags, mode);
      ccsys_handler_filedes_init(L, oufd_H, oufd);
    }

    /* Write to the FIFO. */
    {
      size_t	len = 11;
      char *	oubuf = "0123456789";
      if (0) { fprintf(stderr, "%s: writing to %s\n", __func__, fifoname); }
      ccsys_write(L, oufd, oubuf, len);
    }
    cce_run_cleanup_handlers(L);
  }

  /* Terminate the child process. */
  {
    ccsys_exit_status_t	status;
    status.data = CCSYS_EXIT_SUCCESS;
    ccsys__exit(status);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: closing a file descriptor.
 ** ----------------------------------------------------------------- */

void
test_5_1 (cce_destination_t upper_L)
/* Closing a file with "ccsys_close()". */
{
  cce_location_t	  L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_open_flags_t	flags;
    ccsys_open_mode_t	mode;
    ccsys_fd_t		fd;

    flags.data = CCSYS_O_CREAT;
    mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
    fd = ccsys_open(L, filename, flags, mode);
    ccsys_close(L, fd);
    ccsys_remove(L, filename);
    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: reading and writing with read/write.
 ** ----------------------------------------------------------------- */

void
test_6_1 (cce_destination_t upper_L)
/* Open a file; write  bytes to it; read bytes from  it; close the file;
   remove the file. */
{
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filedes_H[1];
  cce_cleanup_handler_t   file_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, file_H, filename);
    }

    /* Writing. */
    {
      size_t	N;
      size_t	len = 1024;
      uint8_t	buf[len];

      for (size_t i=0; i<len; ++i) {
	buf[i] = i%256;
      }

      N = ccsys_write(L, fd, buf, len);
      cctests_assert(L, N == len);
    }

    /* Seeking. */
    {
      ccsys_off_t	offset;
      ccsys_whence_t	whence;
      offset.data = 0;
      whence.data = CCSYS_SEEK_SET;
      offset = ccsys_lseek(L, fd, offset, whence);
      cctests_assert(L, 0 == offset.data);
    }

    /* Reading. */
    {
      size_t	N;
      size_t	len = 1024;
      uint8_t	buf[len];

      N = ccsys_read(L, fd, buf, len);
      cctests_assert(L, N == len);
      for (size_t i=0; i<len; ++i) {
	if (0) { fprintf(stderr, "%s: %lu %u\n", __func__, i%256, buf[i]); }
	cctests_assert(L, (size_t)(buf[i]) == i%256);
      }
    }

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: reading and writing with pread/pwrite.
 ** ----------------------------------------------------------------- */

void
test_6_2 (cce_destination_t upper_L)
/* Open a file; write  bytes to it; read bytes from  it; close the file;
   remove the file. */
{
#if ((defined HAVE_PREAD) && (defined HAVE_PWRITE))
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filedes_H[1];
  cce_cleanup_handler_t   file_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, file_H, filename);
    }

    /* Writing. */
    {
      size_t		N;
      size_t		len = 1024;
      uint8_t		buf[len];
      ccsys_off_t	offset;

      for (size_t i=0; i<len; ++i) {
	buf[i] = i%256;
      }

      offset.data = 0;
      N = ccsys_pwrite(L, fd, buf, len, offset);
      cctests_assert(L, N == len);
    }

    /* Reading. */
    {
      size_t		N;
      size_t		len = 1024;
      uint8_t		buf[len];
      ccsys_off_t	offset;

      offset.data = 0;
      N = ccsys_pread(L, fd, buf, len, offset);
      cctests_assert(L, N == len);
      for (size_t i=0; i<len; ++i) {
	if (0) { fprintf(stderr, "%s: %lu %u\n", __func__, i%256, buf[i]); }
	cctests_assert(L, (size_t)(buf[i]) == i%256);
      }
    }

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Input/output: scatter-gather reading and writing with readv/writev.
 ** ----------------------------------------------------------------- */

void
test_7_1 (cce_destination_t upper_L)
/* Open a file; write  bytes to it; read bytes from  it; close the file;
   remove the file. */
{
#if ((defined HAVE_READV) && (defined HAVE_WRITEV))
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filedes_H[1];
  cce_cleanup_handler_t   file_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, file_H, filename);
    }

    /* Writing. */
    {
      size_t		buf_count	= 4;
      size_t		buf_len		= 25;
      uint8_t		bufs[buf_count][buf_len];
      ccsys_iovec_t	vec[buf_count];
      size_t		N;

      /* Fill the buffers with some known data. */
      for (size_t i=0; i<buf_count; ++i) {
	for (size_t j=0; j<buf_len; ++j) {
	  bufs[i][j] = j%256;
	  if (0) { fprintf(stderr, "%s: setting i=%lu j=%lu, val=%u\n", __func__, i, j, bufs[i][j]); }
	}
      }

      /* Initialise the vector of buffers. */
      for (size_t i=0; i<buf_count; ++i) {
	*ccsys_iovec_iov_base(&vec[i])	= bufs[i];
	*ccsys_iovec_iov_len(&vec[i])	= buf_len;
      }

      N = ccsys_writev(L, fd, vec, buf_count);
      cctests_assert(L, N == (buf_count*buf_len));
    }

    /* Seeking. */
    {
      ccsys_off_t	offset;
      ccsys_whence_t	whence;
      offset.data = 0;
      whence.data = CCSYS_SEEK_SET;
      offset = ccsys_lseek(L, fd, offset, whence);
      cctests_assert(L, 0 == offset.data);
    }

    /* Reading. */
    {
      size_t		buf_count	= 4;
      size_t		buf_len		= 25;
      uint8_t		bufs[buf_count][buf_len];
      ccsys_iovec_t	vec[buf_count];
      size_t		N;

      /* Initialise the vector of buffers. */
      for (size_t i=0; i<buf_count; ++i) {
	*ccsys_iovec_iov_base(&vec[i])	= bufs[i];
	*ccsys_iovec_iov_len(&vec[i])	= buf_len;
      }

      N = ccsys_readv(L, fd, vec, buf_count);
      cctests_assert(L, N == (buf_count*buf_len));

      for (size_t i=0; i<buf_count; ++i) {
	for (size_t j=0; j<buf_len; ++j) {
	  if (0) { fprintf(stderr, "%s: checking i=%lu j=%lu, expected=%lu, found=%u\n", __func__, i, j, j%256, bufs[i][j]); }
	  cctests_assert(L, (size_t)(bufs[i][j]) == j%256);
	}
      }
    }

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Input/output: scatter-gather reading and writing with preadv/pwritev.
 ** ----------------------------------------------------------------- */

void
test_7_2 (cce_destination_t upper_L)
/* Open a file; write  bytes to it; read bytes from  it; close the file;
   remove the file. */
{
#if ((defined HAVE_PREADV) && (defined HAVE_PWRITEV))
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filedes_H[1];
  cce_cleanup_handler_t   file_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;
      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, file_H, filename);
    }

    /* Writing. */
    {
      size_t		buf_count	= 4;
      size_t		buf_len		= 25;
      uint8_t		bufs[buf_count][buf_len];
      ccsys_iovec_t	vec[buf_count];
      ccsys_off_t	offset;
      size_t		N;

      /* Fill the buffers with some known data. */
      for (size_t i=0; i<buf_count; ++i) {
	for (size_t j=0; j<buf_len; ++j) {
	  bufs[i][j] = j%256;
	  if (0) { fprintf(stderr, "%s: setting i=%lu j=%lu, val=%u\n", __func__, i, j, bufs[i][j]); }
	}
      }

      /* Initialise the vector of buffers. */
      for (size_t i=0; i<buf_count; ++i) {
	*ccsys_iovec_iov_base(&vec[i])	= bufs[i];
	*ccsys_iovec_iov_len(&vec[i])	= buf_len;
      }

      offset.data = 0;
      N = ccsys_pwritev(L, fd, vec, buf_count, offset);
      cctests_assert(L, N == (buf_count*buf_len));
    }

    /* Reading. */
    {
      size_t		buf_count	= 4;
      size_t		buf_len		= 25;
      uint8_t		bufs[buf_count][buf_len];
      ccsys_iovec_t	vec[buf_count];
      ccsys_off_t	offset;
      size_t		N;

      /* Initialise the vector of buffers. */
      for (size_t i=0; i<buf_count; ++i) {
	*ccsys_iovec_iov_base(&vec[i])	= bufs[i];
	*ccsys_iovec_iov_len(&vec[i])	= buf_len;
      }

      offset.data = 0;
      N = ccsys_preadv(L, fd, vec, buf_count, offset);
      cctests_assert(L, N == (buf_count*buf_len));

      for (size_t i=0; i<buf_count; ++i) {
	for (size_t j=0; j<buf_len; ++j) {
	  if (0) { fprintf(stderr, "%s: checking i=%lu j=%lu, expected=%lu, found=%u\n", __func__, i, j, j%256, bufs[i][j]); }
	  cctests_assert(L, (size_t)(bufs[i][j]) == j%256);
	}
      }
    }

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Input/output: file descriptor duplication with "ccsys_dup()".
 ** ----------------------------------------------------------------- */

void
test_8_1 (cce_destination_t upper_L)
/* Open a  file, duplicate the  descriptor with "ccsys_dup()",  write to
   file, read from file, close the file, remove the file. */
{
#if ((defined HAVE_DUP) && (defined HAVE_PREAD) && (defined HAVE_PWRITE))
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filedes_H[1];
  cce_cleanup_handler_t   file_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd, fdx;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, file_H, filename);
    }

    /* Duplicate the file descriptor. */
    if (1) {
      fdx = ccsys_dup(L, fd);
    } else {
      fdx = fd;
    }

    /* Write to the original file descriptor. */
    if (1) {
      size_t		len = 11;
      char const *	buf = "0123456789";
      ccsys_off_t	offset;

      offset.data = 0;
      ccsys_pwrite(L, fd, buf, len, offset);
    }

    /* Read from the duplicated file descriptor. */
    if (1) {
      static size_t const len = 11;
      char		inbuf[len];
      char const *	checkbuf = "0123456789";
      ccsys_off_t	offset;

      offset.data = 0;
      ccsys_pread(L, fdx, inbuf, len, offset);
      cctests_assert(L, 0 == strncmp(inbuf, checkbuf, len));
    }

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Input/output: file descriptor duplication with "ccsys_dup2()".
 ** ----------------------------------------------------------------- */

void
test_8_2 (cce_destination_t upper_L)
/* Open a file,  duplicate the descriptor with  "ccsys_dup2()", write to
   file, read from file, close the file, remove the file. */
{
#if ((defined HAVE_DUP2) && (defined HAVE_PREAD) && (defined HAVE_PWRITE))
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filedes_H[1];
  cce_cleanup_handler_t   file_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd, fdx;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, file_H, filename);
    }

    /* Duplicate the file descriptor. */
    if (1) {
      fdx.data = 123;
      fdx = ccsys_dup2(L, fd, fdx);
      cctests_assert(L, 123 == fdx.data);
    } else {
      fdx = fd;
    }

    /* Write to the original file descriptor. */
    if (1) {
      size_t		len = 11;
      char const *	buf = "0123456789";
      ccsys_off_t	offset;

      offset.data = 0;
      ccsys_pwrite(L, fd, buf, len, offset);
    }

    /* Read from the duplicated file descriptor. */
    if (1) {
      static size_t const len = 11;
      char		inbuf[len];
      char const *	checkbuf = "0123456789";
      ccsys_off_t	offset;

      offset.data = 0;
      ccsys_pread(L, fdx, inbuf, len, offset);
      cctests_assert(L, 0 == strncmp(inbuf, checkbuf, len));
    }

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Input/output: file descriptor duplication with "ccsys_dup3()".
 ** ----------------------------------------------------------------- */

void
test_8_3 (cce_destination_t upper_L)
/* Open a file,  duplicate the descriptor with  "ccsys_dup3()", write to
   file, read from file, close the file, remove the file. */
{
#if ((defined HAVE_DUP3) && (defined HAVE_PREAD) && (defined HAVE_PWRITE))
  cce_location_t	  L[1];
  cce_cleanup_handler_t   filedes_H[1];
  cce_cleanup_handler_t   file_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	filename = "name.ext";
    ccsys_fd_t		fd, fdx;

    /* Create and open the file. */
    {
      ccsys_open_flags_t	flags;
      ccsys_open_mode_t		mode;

      flags.data = CCSYS_O_CREAT | CCSYS_O_RDWR;
      mode.data  = CCSYS_S_IRUSR | CCSYS_S_IWUSR;
      fd = ccsys_open(L, filename, flags, mode);
      ccsys_handler_filedes_init(L, filedes_H, fd);
      ccsys_handler_remove_init(L, file_H, filename);
    }

    /* Duplicate the file descriptor. */
    if (1) {
      ccsys_open_flags_t	flags;

      flags.data	= CCSYS_O_CLOEXEC;
      fdx.data		= 123;
      fdx = ccsys_dup3(L, fd, fdx, flags);
      cctests_assert(L, 123 == fdx.data);
    } else {
      fdx = fd;
    }

    /* Write to the original file descriptor. */
    if (1) {
      size_t		len = 11;
      char const *	buf = "0123456789";
      ccsys_off_t	offset;

      offset.data = 0;
      ccsys_pwrite(L, fd, buf, len, offset);
    }

    /* Read from the duplicated file descriptor. */
    if (1) {
      static size_t const len = 11;
      char		inbuf[len];
      char const *	checkbuf = "0123456789";
      ccsys_off_t	offset;

      offset.data = 0;
      ccsys_pread(L, fdx, inbuf, len, offset);
      cctests_assert(L, 0 == strncmp(inbuf, checkbuf, len));
    }

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Input/output: waiting for input/output events with "ccsys_select()".
 ** ----------------------------------------------------------------- */

static void test_9_1_parent (cce_destination_t upper_L, ccsys_fd_t infd, ccsys_fd_t oufd);
static void test_9_1_child  (ccsys_fd_t infd, ccsys_fd_t oufd);

void
test_9_1 (cce_destination_t upper_L)
{
  ccsys_fd_t	parent_in_fd, parent_ou_fd;
  ccsys_fd_t	 child_in_fd,  child_ou_fd;

  /* Allocate the pipes. */
  {
    cce_location_t		L[1];
    ccsys_fd_t			forwards[2];
    ccsys_fd_t			backwards[2];
    cce_error_handler_t		forwards_H[1];
    cce_error_handler_t		backwards_H[1];

    if (cce_location(L)) {
      cce_run_error_handlers_raise(L, upper_L);
    } else {
      ccsys_pipe(L, forwards);
      ccsys_handler_pipedes_init(L, forwards_H, forwards);
      ccsys_pipe(L, backwards);
      ccsys_handler_pipedes_init(L, backwards_H, backwards);
      cce_run_cleanup_handlers(L);
    }

    /* Here the  pipes have been  allocated.  Now split them  into their
       role. */
    parent_in_fd = forwards[0];
    parent_ou_fd = backwards[1];
    child_in_fd  = backwards[0];
    child_ou_fd  = forwards[1];
  }

  /* Fork the process and call the appropriate functions. */
  {
    cce_location_t	L[1];
    ccsys_pid_t		pid;

    if (cce_location(L)) {
      cce_run_error_handlers_raise(L, upper_L);
    } else {
      pid = ccsys_fork(L);
      if (ccsys_in_parent_after_fork(pid)) {
	test_9_1_parent(L, parent_in_fd, parent_ou_fd);

	/* Wait for the child process. */
	{
	  ccsys_waitpid_options_t	options;
	  ccsys_waitpid_status_t	wstatus;

	  options.data = 0;
	  ccsys_waitpid(L, pid, &wstatus, options);
	}
      } else {
	test_9_1_child(child_in_fd, child_ou_fd);

	/* Terminate the child process. */
	{
	  ccsys_exit_status_t	status;
	  status.data = CCSYS_EXIT_SUCCESS;
	  ccsys__exit(status);
	}
      }
      cce_run_cleanup_handlers(L);
    }
  }
}

void
test_9_1_parent (cce_destination_t upper_L, ccsys_fd_t infd, ccsys_fd_t oufd)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	infd_H[1];
  cce_cleanup_handler_t	oufd_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_handler_filedes_init(L, infd_H, infd);
    ccsys_handler_filedes_init(L, oufd_H, oufd);

    /* Wait for the output file descriptor to be writable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	write_fds;
      ccsys_timeval_t	timeout;
      int		rv;

      ccsys_fd_zero(&write_fds);
      ccsys_fd_set(oufd, &write_fds);
      nfds			= ccsys_fd_incr(oufd);
      timeout.seconds		= 1;
      timeout.microseconds	= 0;
      rv = ccsys_select(L, nfds, NULL, &write_fds, NULL, &timeout);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(oufd, &write_fds));
    }

    /* Write to the output file descriptor. */
    {
      char *	buf = "hello child";
      size_t	len = strlen(buf);
      size_t	N;
      N = ccsys_write(L, oufd, buf, len);
      cctests_assert(L, N == len);
    }

    /* Wait for the input file descriptor to be readable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	read_fds;
      ccsys_timeval_t	timeout;
      int		rv;

      nfds			= ccsys_fd_incr(infd);
      ccsys_fd_zero(&read_fds);
      ccsys_fd_set(infd, &read_fds);
      timeout.seconds		= 1;
      timeout.microseconds	= 0;
      rv = ccsys_select(L, nfds, &read_fds, NULL, NULL, &timeout);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(infd, &read_fds));
    }

    /* Read from the input file descriptor. */
    {
      size_t	len = 256;
      char	buf[len];
      size_t	N;
      N = ccsys_read(L, infd, buf, len);
      cctests_assert(L, 0 == strncmp(buf, "hello parent", N));
    }

    cce_run_cleanup_handlers(L);
  }
}

void
test_9_1_child (ccsys_fd_t infd, ccsys_fd_t oufd)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	infd_H[1];
  cce_cleanup_handler_t	oufd_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_final(L);
  } else {
    ccsys_handler_filedes_init(L, infd_H, infd);
    ccsys_handler_filedes_init(L, oufd_H, oufd);

    /* Wait for the input file descriptor to be readable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	read_fds;
      ccsys_timeval_t	timeout;
      int		rv;

      ccsys_fd_zero(&read_fds);
      ccsys_fd_set(infd, &read_fds);
      nfds			= ccsys_fd_incr(infd);
      timeout.seconds		= 1;
      timeout.microseconds	= 0;
      rv = ccsys_select(L, nfds, &read_fds, NULL, NULL, &timeout);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(infd, &read_fds));
    }

    /* Read from the input file descriptor. */
    {
      size_t	len = 256;
      char	buf[len];
      size_t	N;
      N = ccsys_read(L, infd, buf, len);
      cctests_assert(L, 0 == strncmp(buf, "hello child", N));
    }

    /* Wait for the output file descriptor to be writable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	write_fds;
      ccsys_timeval_t	timeout;
      int		rv;

      ccsys_fd_zero(&write_fds);
      ccsys_fd_set(oufd, &write_fds);
      nfds			= ccsys_fd_incr(oufd);
      timeout.seconds		= 1;
      timeout.microseconds	= 0;
      rv = ccsys_select(L, nfds, NULL, &write_fds, NULL, &timeout);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(oufd, &write_fds));
    }

    /* Write to the output file descriptor. */
    {
      char *	buf = "hello parent";
      size_t	len = strlen(buf);
      size_t	N;
      N = ccsys_write(L, oufd, buf, len);
      cctests_assert(L, N == len);
    }

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Input/output: waiting for input/output events with "ccsys_pselect()".
 ** ----------------------------------------------------------------- */

static void test_9_2_parent (cce_destination_t upper_L, ccsys_fd_t infd, ccsys_fd_t oufd);
static void test_9_2_child  (ccsys_fd_t infd, ccsys_fd_t oufd);

void
test_9_2 (cce_destination_t upper_L)
{
  ccsys_fd_t	parent_in_fd, parent_ou_fd;
  ccsys_fd_t	 child_in_fd,  child_ou_fd;

  /* Allocate the pipes. */
  {
    cce_location_t		L[1];
    ccsys_fd_t			forwards[2];
    ccsys_fd_t			backwards[2];
    cce_error_handler_t		forwards_H[1];
    cce_error_handler_t		backwards_H[1];

    if (cce_location(L)) {
      cce_run_error_handlers_raise(L, upper_L);
    } else {
      ccsys_pipe(L, forwards);
      ccsys_handler_pipedes_init(L, forwards_H, forwards);
      ccsys_pipe(L, backwards);
      ccsys_handler_pipedes_init(L, backwards_H, backwards);
      cce_run_cleanup_handlers(L);
    }

    /* Here the  pipes have been  allocated.  Now split them  into their
       role. */
    parent_in_fd = forwards[0];
    parent_ou_fd = backwards[1];
    child_in_fd  = backwards[0];
    child_ou_fd  = forwards[1];
  }

  /* Fork the process and call the appropriate functions. */
  {
    cce_location_t	L[1];
    ccsys_pid_t		pid;

    if (cce_location(L)) {
      cce_run_error_handlers_raise(L, upper_L);
    } else {
      pid = ccsys_fork(L);
      if (ccsys_in_parent_after_fork(pid)) {
	test_9_2_parent(L, parent_in_fd, parent_ou_fd);

	/* Wait for the child process. */
	{
	  ccsys_waitpid_options_t	options;
	  ccsys_waitpid_status_t	wstatus;

	  options.data = 0;
	  ccsys_waitpid(L, pid, &wstatus, options);
	}
      } else {
	test_9_2_child(child_in_fd, child_ou_fd);

	/* Terminate the child process. */
	{
	  ccsys_exit_status_t	status;
	  status.data = CCSYS_EXIT_SUCCESS;
	  ccsys__exit(status);
	}
      }
      cce_run_cleanup_handlers(L);
    }
  }
}

void
test_9_2_parent (cce_destination_t upper_L, ccsys_fd_t infd, ccsys_fd_t oufd)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	infd_H[1];
  cce_cleanup_handler_t	oufd_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_handler_filedes_init(L, infd_H, infd);
    ccsys_handler_filedes_init(L, oufd_H, oufd);

    /* Wait for the output file descriptor to be writable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	write_fds;
      ccsys_timespec_t	timeout;
      ccsys_sigset_t	sigmask;
      int		rv;

      ccsys_fd_zero(&write_fds);
      ccsys_fd_set(oufd, &write_fds);
      nfds			= ccsys_fd_incr(oufd);
      timeout.seconds		= 1;
      timeout.nanoseconds	= 0;
      rv = ccsys_pselect(L, nfds, NULL, &write_fds, NULL, &timeout, &sigmask);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(oufd, &write_fds));
    }

    /* Write to the output file descriptor. */
    {
      char *	buf = "hello child";
      size_t	len = strlen(buf);
      size_t	N;
      N = ccsys_write(L, oufd, buf, len);
      cctests_assert(L, N == len);
    }

    /* Wait for the input file descriptor to be readable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	read_fds;
      ccsys_timespec_t	timeout;
      ccsys_sigset_t	sigmask;
      int		rv;

      nfds			= ccsys_fd_incr(infd);
      ccsys_fd_zero(&read_fds);
      ccsys_fd_set(infd, &read_fds);
      timeout.seconds		= 1;
      timeout.nanoseconds	= 0;
      rv = ccsys_pselect(L, nfds, &read_fds, NULL, NULL, &timeout, &sigmask);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(infd, &read_fds));
    }

    /* Read from the input file descriptor. */
    {
      size_t	len = 256;
      char	buf[len];
      size_t	N;
      N = ccsys_read(L, infd, buf, len);
      cctests_assert(L, 0 == strncmp(buf, "hello parent", N));
    }

    cce_run_cleanup_handlers(L);
  }
}

void
test_9_2_child (ccsys_fd_t infd, ccsys_fd_t oufd)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	infd_H[1];
  cce_cleanup_handler_t	oufd_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_final(L);
  } else {
    ccsys_handler_filedes_init(L, infd_H, infd);
    ccsys_handler_filedes_init(L, oufd_H, oufd);

    /* Wait for the input file descriptor to be readable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	read_fds;
      ccsys_timespec_t	timeout;
      ccsys_sigset_t	sigmask;
      int		rv;

      ccsys_fd_zero(&read_fds);
      ccsys_fd_set(infd, &read_fds);
      nfds			= ccsys_fd_incr(infd);
      timeout.seconds		= 1;
      timeout.nanoseconds	= 0;
      rv = ccsys_pselect(L, nfds, &read_fds, NULL, NULL, &timeout, &sigmask);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(infd, &read_fds));
    }

    /* Read from the input file descriptor. */
    {
      size_t	len = 256;
      char	buf[len];
      size_t	N;
      N = ccsys_read(L, infd, buf, len);
      cctests_assert(L, 0 == strncmp(buf, "hello child", N));
    }

    /* Wait for the output file descriptor to be writable. */
    {
      ccsys_fd_t	nfds;
      ccsys_fd_set_t	write_fds;
      ccsys_timespec_t	timeout;
      ccsys_sigset_t	sigmask;
      int		rv;

      ccsys_fd_zero(&write_fds);
      ccsys_fd_set(oufd, &write_fds);
      nfds			= ccsys_fd_incr(oufd);
      timeout.seconds		= 1;
      timeout.nanoseconds	= 0;
      rv = ccsys_pselect(L, nfds, NULL, &write_fds, NULL, &timeout, &sigmask);
      cctests_assert(L, 1 == rv);
      cctests_assert(L, ccsys_fd_isset(oufd, &write_fds));
    }

    /* Write to the output file descriptor. */
    {
      char *	buf = "hello parent";
      size_t	len = strlen(buf);
      size_t	N;
      N = ccsys_write(L, oufd, buf, len);
      cctests_assert(L, N == len);
    }

    cce_run_cleanup_handlers(L);
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
      cctests_run(test_3_2);
    }
    cctests_end_group();

    cctests_begin_group("file fifos");
    {
      cctests_run(test_4_1);
      cctests_run(test_4_2);
    }
    cctests_end_group();

    cctests_begin_group("file closing");
    {
      cctests_run(test_5_1);
    }
    cctests_end_group();

    cctests_begin_group("file read/write");
    {
      cctests_run(test_6_1);
      cctests_run(test_6_2);
    }
    cctests_end_group();

    cctests_begin_group("file scatter-gather read/write");
    {
      cctests_run(test_7_1);
      cctests_run(test_7_2);
    }
    cctests_end_group();

    cctests_begin_group("file descriptor duplication");
    {
      cctests_run(test_8_1);
      cctests_run(test_8_2);
      cctests_run(test_8_3);
    }
    cctests_end_group();

    cctests_begin_group("waiting for input/output events");
    {
      cctests_run(test_9_1);
      cctests_run(test_9_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
