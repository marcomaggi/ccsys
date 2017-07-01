/*
  Part of: CCSys
  Contents: POSIX system calls wrappers
  Date: Wed Feb 15, 2017

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


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccsys-internals.h"
#include "ccsys-system.h"
#include "ccsys-networking.h"


/** --------------------------------------------------------------------
 ** System wrappers: locking memory pages.
 ** ----------------------------------------------------------------- */

void
ccsys_mlock (cce_location_t * L, const void * addr, size_t len)
{
#ifdef HAVE_MLOCK
  int	rv;
  errno = 0;
  rv = mlock(addr, len);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_munlock (cce_location_t * L, const void * addr, size_t len)
{
#ifdef HAVE_MUNLOCK
  int	rv;
  errno = 0;
  rv = munlock(addr, len);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_mlockall (cce_location_t * L, int flags)
{
#ifdef HAVE_MLOCKALL
  int	rv;
  errno = 0;
  rv = mlockall(flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_munlockall (cce_location_t * L)
{
#ifdef HAVE_MUNLOCKALL
  int	rv;
  errno = 0;
  rv = munlockall();
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}


/** --------------------------------------------------------------------
 ** System wrappers: file descriptor operations.
 ** ----------------------------------------------------------------- */

int
ccsys_select (cce_location_t * L, int nfds, fd_set * read_fds, fd_set * write_fds, fd_set * except_fds, struct timeval * timeout)
{
#ifdef HAVE_SELECT
  int	rv;
  errno = 0;
  rv = select(nfds, read_fds, write_fds, except_fds, timeout);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

int
ccsys_dup (cce_location_t * L, int old)
{
#ifdef HAVE_DUP
  int	rv;
  errno = 0;
  rv = dup(old);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

int
ccsys_dup2 (cce_location_t * L, int old, int new)
{
#ifdef HAVE_DUP2
  int	rv;
  errno = 0;
  rv = dup2(old, new);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_pipe (cce_location_t * L, int pipefd[2])
{
#ifdef HAVE_PIPE
  int	rv;
  errno = 0;
  rv = pipe(pipefd);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_mkfifo (cce_location_t * L, const char * pathname, mode_t mode)
{
#ifdef HAVE_MKFIFO
  int	rv;
  errno = 0;
  rv = mkfifo(pathname, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}


/** --------------------------------------------------------------------
 ** System wrappers: memory mapping.
 ** ----------------------------------------------------------------- */

void *
ccsys_mmap (cce_location_t * L, void * address, size_t length, int protect, int flags, int filedes, off_t offset)
{
#ifdef HAVE_MMAP
  void *	rv;
  errno = 0;
  rv = mmap(address, length, protect, flags, filedes, offset);
  if (MAP_FAILED != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

int
ccsys_munmap (cce_location_t * L, void * addr, size_t length)
{
#ifdef HAVE_MUNMAP
  int	rv;
  errno = 0;
  rv = munmap(addr, length);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

int
ccsys_msync (cce_location_t * L, void *address, size_t length, int flags)
{
#ifdef HAVE_MSYNC
  int	rv;
  errno = 0;
  rv = msync(address, length, flags);
  if (-1 != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

int
ccsys_mprotect (cce_location_t * L, void * addr, size_t len, int prot)
{
#ifdef HAVE_MPROTECT
  int	rv;
  errno = 0;
  rv = mprotect(addr, len, prot);
  if (-1 != 0) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void *
ccsys_mremap (cce_location_t * L, void * address, size_t length, size_t new_length, int flag)
{
#ifdef HAVE_MREMAP
  void *	rv;
  errno = 0;
  rv = mremap(address, length, new_length, flag);
  if (rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_madvise (cce_location_t * L, void * address, size_t length, int advice)
{
#ifdef HAVE_MADVISE
  int	rv;
  errno = 0;
  rv = madvise(address, length, advice);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}


/** --------------------------------------------------------------------
 ** System wrappers: file system operations.
 ** ----------------------------------------------------------------- */

void
ccsys_getcwd (cce_location_t * L, char * buffer, size_t size)
{
#ifdef HAVE_GETCWD
  char *	rv;
  errno = 0;
  rv = getcwd(buffer, size);
  if (NULL == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_chdir (cce_location_t * L, const char * pathname)
{
#ifdef HAVE_CHDIR
  int	rv;
  errno = 0;
  rv = chdir(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_fchdir (cce_location_t * L, int dirfd)
{
#ifdef HAVE_FCHDIR
  int	rv;
  errno = 0;
  rv = fchdir(dirfd);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

void
ccsys_stat (cce_location_t * L, const char * pathname, struct stat * buf)
{
#ifdef HAVE_STAT
  int	rv;
  errno = 0;
  rv = stat(pathname, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_fstat (cce_location_t * L, int fd, struct stat * buf)
{
#ifdef HAVE_FSTAT
  int	rv;
  errno = 0;
  rv = fstat(fd, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_lstat (cce_location_t * L, const char * pathname, struct stat * buf)
{
#ifdef HAVE_LSTAT
  int	rv;
  errno = 0;
  rv = lstat(pathname, buf);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_mkdir (cce_location_t * L, const char * pathname, mode_t mode)
{
#ifdef HAVE_MKDIR
  int	rv;
  errno = 0;
  rv = mkdir(pathname, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_rmdir (cce_location_t * L, const char * pathname)
{
#ifdef HAVE_RMDIR
  int	rv;
  errno = 0;
  rv = rmdir(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_link (cce_location_t * L, const char * oldname, const char * newname)
{
#ifdef HAVE_LINK
  int	rv;
  errno = 0;
  rv = link(oldname, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_linkat (cce_location_t * L,
		int oldfd, const char * oldname,
		int newfd, const char * newname,
		int flags)
{
#ifdef HAVE_LINKAT
  int	rv;
  errno = 0;
  rv = linkat(oldfd, oldname, newfd, newname, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_symlink (cce_location_t * L, const char * oldname, const char * newname)
{
#ifdef HAVE_SYMLINK
  int	rv;
  errno = 0;
  rv = symlink(oldname, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_symlinkat (cce_location_t * L, const char * oldname, int newdirfd, const char * newname)
{
#ifdef HAVE_SYMLINKAT
  int	rv;
  errno = 0;
  rv = symlinkat(oldname, newdirfd, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

size_t
ccsys_readlink (cce_location_t * L, const char * filename, char * buffer, size_t size)
{
#ifdef HAVE_READLINK
  ssize_t	rv;
  errno = 0;
  rv = readlink(filename, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

size_t
ccsys_readlinkat (cce_location_t * L, int dirfd, const char * filename, char * buffer, size_t size)
{
#ifdef HAVE_READLINKAT
  ssize_t	rv;
  errno = 0;
  rv = readlinkat(dirfd, filename, buffer, size);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

char *
ccsys_realpath (cce_location_t * L, const char * pathname, char * resolved_path)
{
#ifdef HAVE_REALPATH
  char *	rv;
  errno = 0;
  rv = realpath(pathname, resolved_path);
  if (rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_unlink (cce_location_t * L, const char * pathname)
{
#ifdef HAVE_UNLINK
  int	rv;
  errno = 0;
  rv = unlink(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_unlinkat (cce_location_t * L, int dirfd, const char * pathname, int flags)
{
#ifdef HAVE_UNLINKAT
  int	rv;
  errno = 0;
  rv = unlinkat(dirfd, pathname, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_remove (cce_location_t * L, const char * pathname)
{
#ifdef HAVE_REMOVE
  int	rv;
  errno = 0;
  rv = remove(pathname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_rename (cce_location_t * L, const char * oldname, const char * newname)
{
#ifdef HAVE_RENAME
  int	rv;
  errno = 0;
  rv = rename(oldname, newname);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_chown (cce_location_t * L, const char * pathname, uid_t owner, gid_t group)
{
#ifdef HAVE_CHOWN
  int	rv;
  errno = 0;
  rv = chown(pathname, owner, group);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_fchown (cce_location_t * L, int filedes, uid_t owner, gid_t group)
{
#ifdef HAVE_FCHOWN
  int	rv;
  errno = 0;
  rv = fchown(filedes, owner, group);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_lchown (cce_location_t * L, const char * pathname, uid_t owner, gid_t group)
{
#ifdef HAVE_LCHOWN
  int	rv;
  errno = 0;
  rv = lchown(pathname, owner, group);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_fchownat (cce_location_t * L, int dirfd, const char * pathname, uid_t owner, gid_t group, int flags)
{
#ifdef HAVE_FCHOWNAT
  int	rv;
  errno = 0;
  rv = fchownat(dirfd, pathname, owner, group, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_chmod (cce_location_t * L, const char * pathname, mode_t mode)
{
#ifdef HAVE_CHMOD
  int	rv;
  errno = 0;
  rv = chmod(pathname, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_fchmod (cce_location_t * L, int filedes, mode_t mode)
{
#ifdef HAVE_FCHMOD
  int	rv;
  errno = 0;
  rv = fchmod(filedes, mode);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_fchmodat (cce_location_t * L, int dirfd, const char * pathname, mode_t mode, int flags)
{
#ifdef HAVE_FCHMODAT
  int	rv;
  errno = 0;
  rv = fchmodat(dirfd, pathname, mode, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

int
ccsys_access (cce_location_t * L, const char * pathname, int how)
{
#ifdef HAVE_ACCESS
  int	rv;
  errno = 0;
  rv = access(pathname, how);
  if (0 == rv) {
    return rv;
  } else if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

int
ccsys_faccessat (cce_location_t * L, int dirfd, const char * pathname, int how, int flags)
{
#ifdef HAVE_FACCESSAT
  int	rv;
  errno = 0;
  rv = faccessat(dirfd, pathname, how, flags);
  if (0 == rv) {
    return rv;
  } else if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_utime (cce_location_t * L, const char * pathname, const struct utimbuf * times)
{
#ifdef HAVE_UTIME
  int	rv;
  errno = 0;
  rv = utime(pathname, times);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_utimes (cce_location_t * L, const char * pathname, const struct timeval TVP[2])
{
#ifdef HAVE_UTIMES
  int	rv;
  errno = 0;
  rv = utimes(pathname, TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_lutimes (cce_location_t * L, const char * pathname, const struct timeval TVP[2])
{
#if ((defined HAVE_LUTIMES) && (! (defined CCSYS_ON_DARWIN)))
  int	rv;
  errno = 0;
  rv = lutimes(pathname, TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_futimes (cce_location_t * L, int filedes, const struct timeval TVP[2])
{
#if ((defined HAVE_FUTIMES) && (! (defined CCSYS_ON_DARWIN)))
  int	rv;
  errno = 0;
  rv = futimes(filedes, TVP);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_truncate (cce_location_t * L, const char * pathname, off_t length)
{
#ifdef HAVE_TRUNCATE
  int	rv;
  errno = 0;
  rv = truncate(pathname, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_ftruncate (cce_location_t * L, int filedes, off_t length)
{
#ifdef HAVE_FTRUNCATE
  int	rv;
  errno = 0;
  rv = ftruncate(filedes, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}


/** --------------------------------------------------------------------
 ** System wrappers: temporary files and directories.
 ** ----------------------------------------------------------------- */

int
ccsys_mkstemp (cce_location_t * L, char * template)
{
#ifdef HAVE_MKSTEMP
  int	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkstemp(template);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

char *
ccsys_mkdtemp (cce_location_t * L, char * template)
{
#ifdef HAVE_MKDTEMP
  char *	rv;
  errno = 0;
  /* Remember that this call will mutate TEMPLATE. */
  rv = mkdtemp(template);
  if (NULL != rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}


/** --------------------------------------------------------------------
 ** System call wrappers: sockets.
 ** ----------------------------------------------------------------- */

void
ccsys_bind (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t length)
{
#ifdef HAVE_BIND
  int	rv;
  errno = 0;
  rv = bind(socket, addr, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_getsockname (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t * length_ptr)
{
#ifdef HAVE_GETSOCKNAME
  int	rv;
  errno = 0;
  rv = getsockname(socket, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_inet_aton (cce_location_t * L, const char * name, struct in_addr * addr)
{
#if ((defined HAVE_INET_ATON) && (! (defined CCSYS_ON_DARWIN)))
  int	rv;
  errno = 0;
  rv = inet_aton(name, addr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

in_addr_t
ccsys_inet_network (cce_location_t * L, const char * name)
{
#if ((defined HAVE_INET_NETWORK) && (! (defined CCSYS_ON_DARWIN)))
  in_addr_t	rv;
  errno = 0;
  rv = inet_network(name);
  if (((in_addr_t)-1) == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

int
ccsys_socket (cce_location_t * L, int namespace, int style, int protocol)
{
#ifdef HAVE_SOCKET
  int	rv;
  errno = 0;
  rv = socket(namespace, style, protocol);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_shutdown (cce_location_t * L, int socket, int how)
{
#ifdef HAVE_SHUTDOWN
  int	rv;
  errno = 0;
  rv = shutdown(socket, how);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_socketpair (cce_location_t * L, int namespace, int style, int protocol, int filedes[2])
{
#ifdef HAVE_SOCKETPAIR
  int	rv;
  errno = 0;
  rv = socketpair(namespace, style, protocol, filedes);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_connect (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t length)
{
#ifdef HAVE_CONNECT
  int	rv;
  errno = 0;
  rv = connect(socket, addr, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_listen (cce_location_t * L, int socket, int N)
{
#ifdef HAVE_LISTEN
  int	rv;
  errno = 0;
  rv = listen(socket, N);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

int
ccsys_accept (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t * length_ptr)
{
#ifdef HAVE_ACCEPT
  int	rv;
  errno = 0;
  rv = accept(socket, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_getpeername (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t * length_ptr)
{
#ifdef HAVE_GETPEERNAME
  int	rv;
  errno = 0;
  rv = getpeername(socket, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

size_t
ccsys_send (cce_location_t * L, int socket, const void * buffer, size_t size, int flags)
{
#ifdef HAVE_SEND
  ssize_t	rv;
  errno = 0;
  rv = send(socket, buffer, size, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

size_t
ccsys_recv (cce_location_t * L, int socket, void * buffer, size_t size, int flags)
{
#ifdef HAVE_RECV
  ssize_t	rv;
  errno = 0;
  rv = recv(socket, buffer, size, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

size_t
ccsys_sendto (cce_location_t * L, int socket, const void * buffer, size_t size, int flags, struct sockaddr * addr, socklen_t length)
{
#ifdef HAVE_SENDTO
  ssize_t	rv;
  errno = 0;
  rv = sendto(socket, buffer, size, flags, addr, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

size_t
ccsys_recvfrom (cce_location_t * L, int socket, void * buffer, size_t size, int flags, struct sockaddr * addr, socklen_t * length_ptr)
{
#ifdef HAVE_RECVFROM
  ssize_t	rv;
  errno = 0;
  rv = recvfrom(socket, buffer, size, flags, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_getsockopt (cce_location_t * L, int socket, int level, int optname, void * optval, socklen_t * optlen_ptr)
{
#ifdef HAVE_GETSOCKOPT
  int	rv;
  errno = 0;
  rv = getsockopt(socket, level, optname, optval, optlen_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_setsockopt (cce_location_t * L, int socket, int level, int optname, const void * optval, socklen_t optlen)
{
#ifdef HAVE_SETSOCKOPT
  int	rv;
  errno = 0;
  rv = setsockopt(socket, level, optname, optval, optlen);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}


/** --------------------------------------------------------------------
 ** System wrappers: process handling.
 ** ----------------------------------------------------------------- */

int
ccsys_system (cce_location_t * L, const char * command)
{
#ifdef HAVE_SYSTEM
  int	rv;
  errno = 0;
  rv = system(command);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

pid_t
ccsys_fork (cce_location_t * L)
{
#ifdef HAVE_FORK
  pid_t	rv;
  errno = 0;
  rv = fork();
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_execv (cce_location_t * L, const char * filename, char * const argv [])
{
#ifdef HAVE_EXECV
  int	rv;
  errno = 0;
  rv = execv(filename, argv);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_execve (cce_location_t * L, const char * filename, char * const argv [], char * const env [])
{
#ifdef HAVE_EXECVE
  int	rv;
  errno = 0;
  rv = execve(filename, argv, env);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

void
ccsys_execvp (cce_location_t * L, const char * filename, char * const argv [])
{
#ifdef HAVE_EXECVP
  int	rv;
  errno = 0;
  rv = execvp(filename, argv);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* ------------------------------------------------------------------ */

void
ccsys_waitpid (cce_location_t * L, pid_t pid, int * wstatus, int options)
{
#ifdef HAVE_WAITPID
  pid_t	rv;
  errno = 0;
  rv = waitpid(pid, wstatus, options);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
#else
  cce_raise(L, cce_condition_new_unimplemented());
#endif
}

/* end of file */
