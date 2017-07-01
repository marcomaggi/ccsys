/*
  Part of: CCSys
  Contents: system calls for memory management
  Date: Sun Mar 19, 2017

  Abstract



  Copyright (C) 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#ifndef CCSYS_SYSTEM_H
#define CCSYS_SYSTEM_H 1

#ifdef __cplusplus
extern "C" {
#endif


/** --------------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------------- */

#include <ccsys.h>

#include <dirent.h>
#include <utime.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/wait.h>


/** --------------------------------------------------------------------
 ** System call wrappers: locking memory pages.
 ** ----------------------------------------------------------------- */

cce_decl void ccsys_mlock (cce_location_t * L, const void * addr, size_t len)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_munlock (cce_location_t * L, const void * addr, size_t len)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_mlockall (cce_location_t * L, int flags)
  __attribute__((nonnull(1)));

cce_decl void ccsys_munlockall (cce_location_t * L)
  __attribute__((nonnull(1)));


/** --------------------------------------------------------------------
 ** System call wrappers: memory mapping.
 ** ----------------------------------------------------------------- */

cce_decl void * ccsys_mmap (cce_location_t * L, void * address, size_t length, int protect, int flags, int filedes, off_t offset)
  __attribute__((nonnull(1,2)));

cce_decl int ccsys_munmap (cce_location_t * L, void * address, size_t length)
  __attribute__((nonnull(1,2)));

cce_decl int ccsys_msync (cce_location_t * L, void * address, size_t length, int flags)
  __attribute__((nonnull(1,2)));

cce_decl int ccsys_mprotect (cce_location_t * L, void * address, size_t length, int prot)
  __attribute__((nonnull(1,2)));

/* ------------------------------------------------------------------ */

cce_decl void * ccsys_mremap (cce_location_t * L, void * address, size_t length, size_t new_length, int flag)
  __attribute__((nonnull(1,2),returns_nonnull));

cce_decl void ccsys_madvise (cce_location_t * L, void * address, size_t length, int advice)
  __attribute__((nonnull(1,2)));


/** --------------------------------------------------------------------
 ** System call wrappers: input/output and file descriptors.
 ** ----------------------------------------------------------------- */

cce_decl int ccsys_select (cce_location_t * L, int nfds, fd_set * read_fds, fd_set * write_fds, fd_set * except_fds,
			     struct timeval * timeout)
  __attribute__((nonnull(1)));

cce_decl int ccsys_dup (cce_location_t * L, int old)
  __attribute__((nonnull(1)));
cce_decl int ccsys_dup2 (cce_location_t * L, int old, int new)
  __attribute__((nonnull(1)));

cce_decl void ccsys_pipe (cce_location_t * L, int pipefd[2])
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_mkfifo (cce_location_t * L, const char * pathname, mode_t mode)
  __attribute__((nonnull(1,2)));


/** --------------------------------------------------------------------
 ** System call wrappers: file system operations.
 ** ----------------------------------------------------------------- */

cce_decl void ccsys_getcwd (cce_location_t * L, char * buffer, size_t size)
  __attribute__((nonnull(1)));

cce_decl void ccsys_chdir (cce_location_t * L, const char * pathname)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_fchdir (cce_location_t * L, int dirfd)
  __attribute__((nonnull(1)));

/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */

cce_decl void ccsys_stat (cce_location_t * L, const char * pathname, struct stat * buf)
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_fstat (cce_location_t * L, int fd, struct stat * buf)
  __attribute__((nonnull(1,3)));

cce_decl void ccsys_lstat (cce_location_t * L, const char * pathname, struct stat * buf)
  __attribute__((nonnull(1,2,3)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_mkdir (cce_location_t * L, const char * pathname, mode_t mode)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_rmdir (cce_location_t * L, const char * pathname)
  __attribute__((nonnull(1,2)));

/* ------------------------------------------------------------------ */

cce_decl int ccsys_mkstemp (cce_location_t * L, char * template)
  __attribute__((nonnull(1,2)));

cce_decl char * ccsys_mkdtemp (cce_location_t * L, char * template)
  __attribute__((nonnull(1,2)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_link (cce_location_t * L, const char * oldname, const char * newname)
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_linkat (cce_location_t * L,
			      int oldfd, const char * oldname,
			      int newfd, const char * newname,
			      int flags)
  __attribute__((nonnull(1,3,5)));

cce_decl void ccsys_symlink (cce_location_t * L, const char * oldname, const char * newname)
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_symlinkat (cce_location_t * L, const char * oldname, int newdirfd, const char * newname)
  __attribute__((nonnull(1,2,4)));

cce_decl size_t ccsys_readlink (cce_location_t * L, const char * filename, char * buffer, size_t size)
  __attribute__((nonnull(1,2,3)));

cce_decl size_t ccsys_readlinkat (cce_location_t * L, int dirfd, const char * filename, char * buffer, size_t size)
  __attribute__((nonnull(1,3,4)));

cce_decl char * ccsys_realpath (cce_location_t * L, const char * pathname, char * resolved_path)
  __attribute__((nonnull(1,2),returns_nonnull));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_unlink (cce_location_t * L, const char * pathname)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_unlinkat (cce_location_t * L, int dirfc, const char * pathname, int flags)
  __attribute__((nonnull(1,3)));

cce_decl void ccsys_remove (cce_location_t * L, const char * pathname)
  __attribute__((nonnull(1,2)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_rename (cce_location_t * L, const char * oldname, const char * newname)
  __attribute__((nonnull(1,2,3)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_chown (cce_location_t * L, const char * pathname, uid_t owner, gid_t group)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_fchown (cce_location_t * L, int filedes, uid_t owner, gid_t group)
  __attribute__((nonnull(1)));

cce_decl void ccsys_lchown (cce_location_t * L, const char * pathname, uid_t owner, gid_t group)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_fchownat (cce_location_t * L, int dirfd, const char * pathname, uid_t owner, gid_t group, int flags)
  __attribute__((nonnull(1,3)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_chmod (cce_location_t * L, const char * pathname, mode_t mode)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_fchmod (cce_location_t * L, int filedes, mode_t mode)
  __attribute__((nonnull(1)));

cce_decl void ccsys_fchmodat (cce_location_t * L, int dirfd, const char * pathname, mode_t mode, int flags)
  __attribute__((nonnull(1,3)));

/* ------------------------------------------------------------------ */

cce_decl int ccsys_access (cce_location_t * L, const char * pathname, int how)
  __attribute__((nonnull(1,2)));

cce_decl int ccsys_faccessat (cce_location_t * L, int dirfd, const char * pathname, int how, int flags)
  __attribute__((nonnull(1,3)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_utime (cce_location_t * L, const char * pathname, const struct utimbuf * times)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_utimes (cce_location_t * L, const char * pathname, const struct timeval TVP[2])
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_lutimes (cce_location_t * L, const char * pathname, const struct timeval TVP[2])
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_futimes (cce_location_t * L, int filedes, const struct timeval TVP[2])
  __attribute__((nonnull(1,3)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_truncate (cce_location_t * L, const char * pathname, off_t length)
  __attribute__((nonnull(1,2)));

cce_decl void ccsys_ftruncate (cce_location_t * L, int filedes, off_t length)
  __attribute__((nonnull(1)));


/** --------------------------------------------------------------------
 ** System call wrappers: process handling.
 ** ----------------------------------------------------------------- */

cce_decl int ccsys_system (cce_location_t * L, const char * command)
  __attribute__((nonnull(1,2)));

cce_decl pid_t ccsys_fork (cce_location_t * L)
  __attribute__((nonnull(1)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_execv (cce_location_t * L, const char * filename, char * const argv [])
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_execve (cce_location_t * L, const char * filename, char * const argv [], char * const env [])
  __attribute__((nonnull(1,2,3,4)));

cce_decl void ccsys_execvp (cce_location_t * L, const char * filename, char * const argv [])
  __attribute__((nonnull(1,2,3)));

/* ------------------------------------------------------------------ */

cce_decl void ccsys_waitpid (cce_location_t * L, pid_t pid, int * wstatus, int options)
  __attribute__((nonnull(1,3)));


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: pipe descriptors.
 ** ----------------------------------------------------------------- */

cce_decl void ccsys_cleanup_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, int pipedes[2])
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_error_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, int pipedes[2])
  __attribute__((nonnull(1,2,3)));


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: removal of temporary file.
 ** ----------------------------------------------------------------- */

cce_decl void ccsys_cleanup_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_error_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
  __attribute__((nonnull(1,2,3)));


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: removal of temporary directory.
 ** ----------------------------------------------------------------- */

cce_decl void ccsys_cleanup_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
  __attribute__((nonnull(1,2,3)));

cce_decl void ccsys_error_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
  __attribute__((nonnull(1,2,3)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCSYS_SYSTEM_H */

/* end of file */
