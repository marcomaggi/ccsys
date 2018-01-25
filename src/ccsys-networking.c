/*
  Part of: CCSys
  Contents: POSIX system calls wrappers
  Date: Wed Feb 15, 2017

  Abstract



  Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can  redistribute it and/or modify it under
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
#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif


/** --------------------------------------------------------------------
 ** System call wrappers: sockets.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_BIND
void
ccsys_bind (cce_location_t * L, ccsys_fd_t socket, ccsys_sockaddr_t * addr, ccsys_socklen_t length)
{
  int	rv;
  errno = 0;
  rv = bind(socket.data, (struct sockaddr *)addr, length.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETSOCKNAME
void
ccsys_getsockname (cce_location_t * L, ccsys_fd_t socket, ccsys_sockaddr_t * addr, ccsys_socklen_t * length_ptr)
{
  socklen_t	len;
  int		rv;
  errno = 0;
  rv = getsockname(socket.data, (struct sockaddr *)addr, &len);
  if (0 == rv) {
    length_ptr->data = len;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#if ((defined HAVE_INET_ATON) && (0 == CCSYS_ON_DARWIN))
void
ccsys_inet_aton (cce_location_t * L, const char * name, ccsys_in_addr_t * _addr)
{
  struct in_addr *	addr = (struct in_addr *)_addr;
  int			rv;
  errno = 0;
  rv = inet_aton(name, addr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#if ((defined HAVE_INET_NETWORK) && (0 == CCSYS_ON_DARWIN))
ccsys_in_addr_t
ccsys_inet_network (cce_location_t * L, const char * name)
{
  in_addr_t		A;
  errno = 0;
  A = inet_network(name);
  if (((in_addr_t)-1) == A) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    ccsys_in_addr_t	addr;
    memcpy(addr.data, &A, sizeof(in_addr_t));
    return addr;
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_SOCKET
int
ccsys_socket (cce_location_t * L, ccsys_socket_namespace_t namespace,
	      ccsys_socket_style_t style, ccsys_socket_protocol_t protocol)
{
  int	rv;
  errno = 0;
  rv = socket(namespace.data, style.data, protocol.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_SHUTDOWN
void
ccsys_shutdown (cce_location_t * L, ccsys_fd_t socket, int how)
{
  int	rv;
  errno = 0;
  rv = shutdown(socket.data, how);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SOCKETPAIR
void
ccsys_socketpair (cce_location_t * L, ccsys_socket_namespace_t namespace,
		  ccsys_socket_style_t style, ccsys_socket_protocol_t protocol,
		  ccsys_fd_t filedes[2])
{
  int	rv;
  errno = 0;
  rv = socketpair(namespace.data, style.data, protocol.data, (int *)filedes);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* ------------------------------------------------------------------ */

#ifdef HAVE_CONNECT
void
ccsys_connect (cce_location_t * L, ccsys_fd_t socket, ccsys_sockaddr_t * addr, ccsys_socklen_t length)
{
  int	rv;
  errno = 0;
  rv = connect(socket.data, (struct sockaddr *)addr, length.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_LISTEN
void
ccsys_listen (cce_location_t * L, ccsys_fd_t socket, int N)
{
  int	rv;
  errno = 0;
  rv = listen(socket.data, N);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_ACCEPT
int
ccsys_accept (cce_location_t * L, ccsys_fd_t socket, ccsys_sockaddr_t * addr, ccsys_socklen_t * length_ptr)
{
  int	rv;
  errno = 0;
  rv = accept(socket.data, (struct sockaddr *)addr, (socklen_t *)&(length_ptr->data));
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return rv;
  }
}
#endif

#ifdef HAVE_GETPEERNAME
void
ccsys_getpeername (cce_location_t * L, ccsys_fd_t socket, ccsys_sockaddr_t * addr, ccsys_socklen_t * length_ptr)
{
  int	rv;
  errno = 0;
  rv = getpeername(socket.data, (struct sockaddr *)addr, (socklen_t *)&(length_ptr->data));
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SEND
size_t
ccsys_send (cce_location_t * L, ccsys_fd_t socket, const void * buffer, size_t size, int flags)
{
  ssize_t	rv;
  errno = 0;
  rv = send(socket.data, buffer, size, flags);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_RECV
size_t
ccsys_recv (cce_location_t * L, ccsys_fd_t socket, void * buffer, size_t size, int flags)
{
  ssize_t	rv;
  errno = 0;
  rv = recv(socket.data, buffer, size, flags);
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
ccsys_sendto (cce_location_t * L, ccsys_fd_t socket, const void * buffer, size_t size, int flags,
	      ccsys_sockaddr_t * addr, ccsys_socklen_t length)
{
  ssize_t	rv;
  errno = 0;
  rv = sendto(socket.data, buffer, size, flags, (struct sockaddr *)addr, length.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (size_t)rv;
  }
}
#endif

#ifdef HAVE_RECVFROM
size_t
ccsys_recvfrom (cce_location_t * L, ccsys_fd_t socket, void * buffer, size_t size, int flags, ccsys_sockaddr_t * addr, ccsys_socklen_t * length_ptr)
{
  ssize_t	rv;
  errno = 0;
  rv = recvfrom(socket.data, buffer, size, flags, (struct sockaddr *)addr, (socklen_t *)&(length_ptr->data));
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
ccsys_getsockopt (cce_location_t * L, ccsys_fd_t socket, int level, int optname, void * optval, ccsys_socklen_t * optlen_ptr)
{
  int	rv;
  errno = 0;
  rv = getsockopt(socket.data, level, optname, optval, (socklen_t *)&(optlen_ptr->data));
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SETSOCKOPT
void
ccsys_setsockopt (cce_location_t * L, ccsys_fd_t socket, int level, int optname, const void * optval, ccsys_socklen_t optlen)
{
  int	rv;
  errno = 0;
  rv = setsockopt(socket.data, level, optname, optval, optlen.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

/* end of file */
