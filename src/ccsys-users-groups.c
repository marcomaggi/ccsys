/*
  Part of: CCSys
  Contents: POSIX system calls wrappers, users and groups wrappers
  Date: Jan 26, 2018

  Abstract

	POSIX system calls wrappers, users and groups functions.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#include <assert.h>
#ifdef HAVE_GRP_H
#  include <grp.h>
#endif
#ifdef HAVE_PWD_H
#  include <pwd.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif


/** --------------------------------------------------------------------
 ** Users and groups: reading the persona of a process.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETUID
ccsys_uid_t
ccsys_getuid (void)
{
  ccsys_uid_t	uid;

  uid.data = getuid();
  return uid;
}
#endif

#ifdef HAVE_GETGID
ccsys_gid_t
ccsys_getgid (void)
{
  ccsys_gid_t	gid;

  gid.data = getgid();
  return gid;
}
#endif

#ifdef HAVE_GETEUID
ccsys_uid_t
ccsys_geteuid (void)
{
  ccsys_uid_t	uid;

  uid.data = geteuid();
  return uid;
}
#endif

#ifdef HAVE_GETEGID
ccsys_gid_t
ccsys_getegid (void)
{
  ccsys_gid_t	gid;

  gid.data = getegid();
  return gid;
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: setting the user ID.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SETUID
void
ccsys_setuid (cce_destination_t L, ccsys_uid_t newuid)
{
  int	rv;

  rv = setuid(newuid.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SETEUID
void
ccsys_seteuid (cce_destination_t L, ccsys_uid_t newuid)
{
  int	rv;

  rv = seteuid(newuid.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SETREUID
void
ccsys_setreuid (cce_destination_t L, ccsys_uid_t ruid, ccsys_uid_t euid)
{
  int	rv;

  rv = setreuid(ruid.data, euid.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: setting the group ID.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SETGID
void
ccsys_setgid (cce_destination_t L, ccsys_gid_t newgid)
{
  int	rv;

  rv = setgid(newgid.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SETEGID
void
ccsys_setegid (cce_destination_t L, ccsys_gid_t newgid)
{
  int	rv;

  rv = setegid(newgid.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SETREGID
void
ccsys_setregid (cce_destination_t L, ccsys_gid_t rgid, ccsys_gid_t egid)
{
  int	rv;

  rv = setregid(rgid.data, egid.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: setting and getting additional group IDs.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETGROUPS
int
ccsys_getgroups (cce_destination_t L, int count, ccsys_gid_t * groups)
{
  gid_t	gids[count];
  int	rv;
  errno = 0;
  rv    = getgroups(count, gids);
  if (0 <= rv) {
    for (int i=0; i<rv; ++i) {
      groups[i].data = gids[i];
    }
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}

int
ccsys_getgroups_acquire_groups_number (cce_destination_t L)
{
  int	rv;
  errno = 0;
  rv    = getgroups(0, NULL);
  if (0 <= rv) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_SETGROUPS
void
ccsys_setgroups (cce_destination_t L, size_t size, ccsys_gid_t const * groups)
{
  gid_t		gids[size];
  int		rv;

  for (size_t i=0; i<size; ++i) {
    gids[i] = groups[i].data;
  }
  errno = 0;
  rv    = setgroups(size, gids);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_INITGROUPS
void
ccsys_initgroups (cce_destination_t L, char const * user, ccsys_gid_t gid)
{
  int		rv;
  errno = 0;
  rv    = initgroups(user, gid.data);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETGROUPLIST
void
ccsys_getgrouplist (cce_destination_t L, char const * user, ccsys_gid_t gid, ccsys_gid_t * groups, int * ngroups)
{
  gid_t	gids[*ngroups];
  int	rv;
  errno = 0;
  rv    = getgrouplist(user, gid.data, gids, ngroups);
  if (-1 != rv) {
    for (int i=0; i<*ngroups; ++i) {
      groups[i].data = gids[i];
    }
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}

int
ccsys_getgrouplist_acquire_groups_number (cce_destination_t L, char const * username)
{
  int	ngroups = 0;
  int	rv;

  errno = 0;
  rv = getgrouplist(username, 0, NULL, &ngroups);
  assert(-1 == rv);
  if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return ngroups;
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: who has logged in.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETLOGIN_R
void
ccsys_getlogin_r (cce_destination_t L, char * buf, size_t maxlen)
{
  int	rv;
  errno = 0;
  rv    = getlogin_r(buf, maxlen);
  if (0 != rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: "struct passwd" interface.
 ** ----------------------------------------------------------------- */

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_NAME)
char const *
ccsys_ref_passwd_pw_name (ccsys_passwd_t const * const S)
{
  CCSYS_PC(struct passwd const, D, S);
  return D->pw_name;
}
void
ccsys_set_passwd_pw_name (ccsys_passwd_t * const S, char const * F)
{
  CCSYS_PC(struct passwd, D, S);
  D->pw_name = (char *)F;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_PASSWD)
char const *
ccsys_ref_passwd_pw_passwd (ccsys_passwd_t const * const S)
{
  CCSYS_PC(struct passwd const, D, S);
  return D->pw_passwd;
}
void
ccsys_set_passwd_pw_passwd (ccsys_passwd_t * const S, char const * F)
{
  CCSYS_PC(struct passwd, D, S);
  D->pw_passwd = (char *)F;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_UID)
ccsys_uid_t
ccsys_ref_passwd_pw_uid (ccsys_passwd_t const * const S)
{
  CCSYS_PC(struct passwd const, D, S);
  ccsys_uid_t	rv = { .data = D->pw_uid };
  return rv;
}
void
ccsys_set_passwd_pw_uid (ccsys_passwd_t * const S, ccsys_uid_t F)
{
  CCSYS_PC(struct passwd, D, S);
  D->pw_uid = F.data;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GID)
ccsys_gid_t
ccsys_ref_passwd_pw_gid (ccsys_passwd_t const * const S)
{
  CCSYS_PC(struct passwd const, D, S);
  ccsys_gid_t	rv = { .data = D->pw_gid };
  return rv;
}
void
ccsys_set_passwd_pw_gid (ccsys_passwd_t * const S, ccsys_gid_t F)
{
  CCSYS_PC(struct passwd, D, S);
  D->pw_gid = F.data;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GECOS)
char const *
ccsys_ref_passwd_pw_gecos (ccsys_passwd_t const * const S)
{
  CCSYS_PC(struct passwd const, D, S);
  return D->pw_gecos;
}
void
ccsys_set_passwd_pw_gecos (ccsys_passwd_t * const S, char const * F)
{
  CCSYS_PC(struct passwd, D, S);
  D->pw_gecos = (char *)F;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_DIR)
char const *
ccsys_ref_passwd_pw_dir (ccsys_passwd_t const * const S)
{
  CCSYS_PC(struct passwd const, D, S);
  return D->pw_dir;
}
void
ccsys_set_passwd_pw_dir (ccsys_passwd_t * const S, char const * F)
{
  CCSYS_PC(struct passwd, D, S);
  D->pw_dir = (char *)F;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_SHELL)
char const *
ccsys_ref_passwd_pw_shell (ccsys_passwd_t const * const S)
{
  CCSYS_PC(struct passwd const, D, S);
  return D->pw_shell;
}
void
ccsys_set_passwd_pw_shell (ccsys_passwd_t * const S, char const * F)
{
  CCSYS_PC(struct passwd, D, S);
  D->pw_shell = (char *)F;
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: looking up in the users database.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETPWUID
ccsys_passwd_t const *
ccsys_getpwuid (cce_destination_t L, ccsys_uid_t uid)
{
  struct passwd const *	rv;

  errno = 0;
  rv = getpwuid(uid.data);
  if (NULL != rv) {
    return (ccsys_passwd_t const *)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETPWUID_R
bool
ccsys_getpwuid_r (cce_destination_t L, ccsys_uid_t uid, ccsys_passwd_t * result_buf,
		  char * bufptr, size_t buflen, ccsys_passwd_t ** result)
{
  int	rv;

  errno = 0;
  rv = getpwuid_r(uid.data, (struct passwd *)result_buf, bufptr, buflen, (struct passwd **) result);
  if (NULL != *result) {
    return true;
  } else if (ERANGE == rv) {
    return false;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETPWNAM
ccsys_passwd_t const *
ccsys_getpwnam (cce_destination_t L, char const * name)
{
  struct passwd const *	rv;

  errno = 0;
  rv = getpwnam(name);
  if (NULL != rv) {
    return (ccsys_passwd_t const *)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETPWNAM_R
bool
ccsys_getpwnam_r (cce_destination_t L, char const * name, ccsys_passwd_t * result_buf,
		  char * bufptr, size_t buflen, ccsys_passwd_t ** result)
{
  int	rv;

  errno = 0;
  rv = getpwnam_r(name, (struct passwd *)result_buf, bufptr, buflen, (struct passwd **) result);
  if (NULL != *result) {
    return true;
  } else if (ERANGE == rv) {
    return false;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: "struct group" interface.
 ** ----------------------------------------------------------------- */

#if (1 == CCSYS_HAVE_STRUCT_GROUP_GR_NAME)
char const *
ccsys_ref_group_gr_name (ccsys_group_t const * const S)
{
  CCSYS_PC(struct group const, D, S);
  return D->gr_name;
}
void
ccsys_set_group_gr_name (ccsys_group_t * const S, char const * F)
{
  CCSYS_PC(struct group, D, S);
  D->gr_name = (char *)F;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_GROUP_GR_GID)
ccsys_gid_t
ccsys_ref_group_gr_gid (ccsys_group_t const * const S)
{
  CCSYS_PC(struct group const, D, S);
  ccsys_gid_t	rv = { .data = D->gr_gid };
  return rv;
}
void
ccsys_set_group_gr_gid (ccsys_group_t * const S, ccsys_gid_t F)
{
  CCSYS_PC(struct group, D, S);
  D->gr_gid = F.data;
}
#endif

#if (1 == CCSYS_HAVE_STRUCT_GROUP_GR_MEM)
char const * const *
ccsys_ref_group_gr_mem (ccsys_group_t const * const S)
{
  CCSYS_PC(struct group const, D, S);
  return (char const * const *)(D->gr_mem);
}
void
ccsys_set_group_gr_mem (ccsys_group_t * const S, char const * const * F)
{
  CCSYS_PC(struct group, D, S);
  D->gr_mem = (char **)F;
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: looking up in the groups database.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_GETGRGID
ccsys_group_t const *
ccsys_getgrgid (cce_destination_t L, ccsys_gid_t gid)
{
  struct group const *	rv;

  errno = 0;
  rv = getgrgid(gid.data);
  if (NULL != rv) {
    return (ccsys_group_t const *)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETGRGID_R
bool
ccsys_getgrgid_r (cce_destination_t L, ccsys_gid_t gid, ccsys_group_t * result_buf,
		  char * bufptr, size_t buflen, ccsys_group_t ** result)
{
  int	rv;

  errno = 0;
  rv = getgrgid_r(gid.data, (struct group *)result_buf, bufptr, buflen, (struct group **) result);
  if (NULL != *result) {
    return true;
  } else if (ERANGE == rv) {
    return false;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETGRNAM
ccsys_group_t const *
ccsys_getgrnam (cce_destination_t L, char const * name)
{
  struct group const *	rv;

  errno = 0;
  rv = getgrnam(name);
  if (NULL != rv) {
    return (ccsys_group_t const *)rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETGRNAM_R
bool
ccsys_getgrnam_r (cce_destination_t L, char const * name, ccsys_group_t * result_buf,
		  char * bufptr, size_t buflen, ccsys_group_t ** result)
{
  int	rv;

  errno = 0;
  rv = getgrnam_r(name, (struct group *)result_buf, bufptr, buflen, (struct group **) result);
  if (NULL != *result) {
    return true;
  } else if (ERANGE == rv) {
    return false;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: scanning the users database.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SETPWENT
void
ccsys_setpwent (cce_destination_t L)
/* The  "errno"  handling  in  this function  is  not  documented:  does
   "setpwent()"  modify   "errno"?   We   do  this,   nevertheless,  for
   safety. */
{
  errno = 0;
  setpwent();
  if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_ENDPWENT
void
ccsys_endpwent (cce_destination_t L)
/* The  "errno"  handling  in  this function  is  not  documented:  does
   "endpwent()"  modify   "errno"?   We   do  this,   nevertheless,  for
   safety. */
{
  errno = 0;
  endpwent();
  if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETPWENT
ccsys_passwd_t const *
ccsys_getpwent (cce_destination_t L)
{
  struct passwd const *	pw;

  errno = 0;
  pw = getpwent();
  if ((NULL == pw) && (0 != errno)) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (ccsys_passwd_t const *)pw;
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: scanning the users database.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_SETGRENT
void
ccsys_setgrent (cce_destination_t L)
/* The  "errno"  handling  in  this function  is  not  documented:  does
   "setgrent()"  modify   "errno"?   We   do  this,   nevertheless,  for
   safety. */
{
  errno = 0;
  setgrent();
  if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_ENDGRENT
void
ccsys_endgrent (cce_destination_t L)
/* The  "errno"  handling  in  this function  is  not  documented:  does
   "endgrent()"  modify   "errno"?   We   do  this,   nevertheless,  for
   safety. */
{
  errno = 0;
  endgrent();
  if (errno) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif

#ifdef HAVE_GETGRENT
ccsys_group_t const *
ccsys_getgrent (cce_destination_t L)
{
  struct group const *	gr;

  errno = 0;
  gr = getgrent();
  if ((NULL == gr) && (0 != errno)) {
    cce_raise(L, cce_condition_new_errno_clear());
  } else {
    return (ccsys_group_t const *)gr;
  }
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: endpwent handler.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_ENDPWENT
__attribute__((__nonnull__(1,2)))
static void
cce_handler_endpwent_function (const cce_condition_t * C CCSYS_UNUSED, cce_handler_t * H CCSYS_UNUSED)
{
  endpwent();
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_clean_handler_endpwent_init (cce_location_t * L, cce_handler_t * H)
{
  H->function	= cce_handler_endpwent_function;
  cce_register_clean_handler(L, H);
}

void
ccsys_error_handler_endpwent_init (cce_location_t * L, cce_handler_t * H)
{
  H->function	= cce_handler_endpwent_function;
  cce_register_error_handler(L, H);
}
#endif


/** --------------------------------------------------------------------
 ** Users and groups: endgrent handler.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_ENDGRENT
__attribute__((__nonnull__(1,2)))
static void
cce_handler_endgrent_function (const cce_condition_t * C CCSYS_UNUSED, cce_handler_t * H CCSYS_UNUSED)
{
  endgrent();
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_clean_handler_endgrent_init (cce_location_t * L, cce_handler_t * H)
{
  H->function	= cce_handler_endgrent_function;
  cce_register_clean_handler(L, H);
}

void
ccsys_error_handler_endgrent_init (cce_location_t * L, cce_handler_t * H)
{
  H->function	= cce_handler_endgrent_function;
  cce_register_error_handler(L, H);
}
#endif

/* end of file */
