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
#ifdef HAVE_NETDB_H
#  include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif


/** --------------------------------------------------------------------
 ** System call wrappers: sockets.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_BIND
void
ccsys_bind (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t length)
{
  int	rv;
  errno = 0;
  rv = bind(socket, addr, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETSOCKNAME
void
ccsys_getsockname (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t * length_ptr)
{
  int	rv;
  errno = 0;
  rv = getsockname(socket, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#if ((defined HAVE_INET_ATON) && (! (defined CCSYS_ON_DARWIN)))
void
ccsys_inet_aton (cce_location_t * L, const char * name, struct in_addr * addr)
{
  int	rv;
  errno = 0;
  rv = inet_aton(name, addr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_INET_NETWORK) && (! (defined CCSYS_ON_DARWIN)))
in_addr_t
ccsys_inet_network (cce_location_t * L, const char * name)
{
  in_addr_t	rv;
  errno = 0;
  rv = inet_network(name);
  if (((in_addr_t)-1) == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_SOCKET
int
ccsys_socket (cce_location_t * L, int namespace, int style, int protocol)
{
  int	rv;
  errno = 0;
  rv = socket(namespace, style, protocol);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_SHUTDOWN
void
ccsys_shutdown (cce_location_t * L, int socket, int how)
{
  int	rv;
  errno = 0;
  rv = shutdown(socket, how);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SOCKETPAIR
void
ccsys_socketpair (cce_location_t * L, int namespace, int style, int protocol, int filedes[2])
{
  int	rv;
  errno = 0;
  rv = socketpair(namespace, style, protocol, filedes);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_CONNECT
void
ccsys_connect (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t length)
{
  int	rv;
  errno = 0;
  rv = connect(socket, addr, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LISTEN
void
ccsys_listen (cce_location_t * L, int socket, int N)
{
  int	rv;
  errno = 0;
  rv = listen(socket, N);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_ACCEPT
int
ccsys_accept (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t * length_ptr)
{
  int	rv;
  errno = 0;
  rv = accept(socket, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_GETPEERNAME
void
ccsys_getpeername (cce_location_t * L, int socket, struct sockaddr * addr, socklen_t * length_ptr)
{
  int	rv;
  errno = 0;
  rv = getpeername(socket, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SEND
size_t
ccsys_send (cce_location_t * L, int socket, const void * buffer, size_t size, int flags)
{
  ssize_t	rv;
  errno = 0;
  rv = send(socket, buffer, size, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_RECV
size_t
ccsys_recv (cce_location_t * L, int socket, void * buffer, size_t size, int flags)
{
  ssize_t	rv;
  errno = 0;
  rv = recv(socket, buffer, size, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_SENDTO
size_t
ccsys_sendto (cce_location_t * L, int socket, const void * buffer, size_t size, int flags, struct sockaddr * addr, socklen_t length)
{
  ssize_t	rv;
  errno = 0;
  rv = sendto(socket, buffer, size, flags, addr, length);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_RECVFROM
size_t
ccsys_recvfrom (cce_location_t * L, int socket, void * buffer, size_t size, int flags, struct sockaddr * addr, socklen_t * length_ptr)
{
  ssize_t	rv;
  errno = 0;
  rv = recvfrom(socket, buffer, size, flags, addr, length_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_GETSOCKOPT
void
ccsys_getsockopt (cce_location_t * L, int socket, int level, int optname, void * optval, socklen_t * optlen_ptr)
{
  int	rv;
  errno = 0;
  rv = getsockopt(socket, level, optname, optval, optlen_ptr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SETSOCKOPT
void
ccsys_setsockopt (cce_location_t * L, int socket, int level, int optname, const void * optval, socklen_t optlen)
{
  int	rv;
  errno = 0;
  rv = setsockopt(socket, level, optname, optval, optlen);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* end of file */
