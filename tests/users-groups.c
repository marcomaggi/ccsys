/*
  Part of: CCSys
  Contents: tests for POSIX system functions, users and groups
  Date: Jan 26, 2018

  Abstract

	Tests for POSIX system functions, users and groups.

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
 ** Users and groups: reading the persona of a process.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L CCSYS_UNUSED)
/* Test for "ccsys_getuid()". */
{
#if (defined HAVE_GETUID)
  ccsys_uid_t	uid;

  uid = ccsys_getuid();
  if (1) { fprintf(stderr, "%s: uid=%d\n", __func__, uid.data); }
#endif
}

void
test_1_2 (cce_destination_t upper_L CCSYS_UNUSED)
/* Test for "ccsys_getgid()". */
{
#if (defined HAVE_GETGID)
  ccsys_gid_t	gid;

  gid = ccsys_getgid();
  if (1) { fprintf(stderr, "%s: gid=%d\n", __func__, gid.data); }
#endif
}

void
test_1_3 (cce_destination_t upper_L CCSYS_UNUSED)
/* Test for "ccsys_geteuid()". */
{
#if (defined HAVE_GETEUID)
  ccsys_uid_t	euid;

  euid = ccsys_geteuid();
  if (1) { fprintf(stderr, "%s: euid=%d\n", __func__, euid.data); }
#endif
}

void
test_1_4 (cce_destination_t upper_L CCSYS_UNUSED)
/* Test for "ccsys_getegid()". */
{
#if (defined HAVE_GETEGID)
  ccsys_gid_t	egid;

  egid = ccsys_getegid();
  if (1) { fprintf(stderr, "%s: egid=%d\n", __func__, egid.data); }
#endif
}


/** --------------------------------------------------------------------
 ** Users and groups: setting the user identity.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
/* Test for "ccsys_setuid()". */
{
#if ((defined HAVE_GETUID) && (defined HAVE_SETUID))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_uid_t	uid;

    uid = ccsys_getuid();
    ccsys_setuid(L, uid);

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_2_2 (cce_destination_t upper_L)
/* Test for "ccsys_seteuid()". */
{
#if ((defined HAVE_GETEUID) && (defined HAVE_SETEUID))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_uid_t	uid;

    uid = ccsys_geteuid();
    ccsys_seteuid(L, uid);

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_2_3 (cce_destination_t upper_L)
/* Test for "ccsys_setreuid()". */
{
#if ((defined HAVE_GETUID) && (defined HAVE_GETEUID) && (defined HAVE_SETREUID))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_uid_t	ruid, euid;

    ruid = ccsys_getuid();
    euid = ccsys_geteuid();
    ccsys_setreuid(L, ruid, euid);

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Users and groups: setting the group identity.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
/* Test for "ccsys_setgid()". */
{
#if ((defined HAVE_GETGID) && (defined HAVE_SETGID))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_gid_t	gid;

    gid = ccsys_getgid();
    ccsys_setgid(L, gid);

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_3_2 (cce_destination_t upper_L)
/* Test for "ccsys_setegid()". */
{
#if ((defined HAVE_GETEGID) && (defined HAVE_SETEGID))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_gid_t	gid;

    gid = ccsys_getegid();
    ccsys_setegid(L, gid);

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_3_3 (cce_destination_t upper_L)
/* Test for "ccsys_setregid()". */
{
#if ((defined HAVE_GETGID) && (defined HAVE_GETEGID) && (defined HAVE_SETREGID))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_gid_t	rgid, egid;

    rgid = ccsys_getgid();
    egid = ccsys_getegid();
    ccsys_setregid(L, rgid, egid);

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Users and groups: additional group identity.
 ** ----------------------------------------------------------------- */

void
test_4_1 (cce_destination_t upper_L)
/* Test for "ccsys_getgroups()". */
{
#if (defined HAVE_GETGROUPS)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    int		count;

    count = ccsys_getgroups_acquire_groups_number(L);
    {
      ccsys_gid_t	groups[count];
      int		ngroups;

      ngroups = ccsys_getgroups(L, count, groups);
      cctests_assert(L, ngroups == count);

      for (int i=0; i<ngroups; ++i) {
	fprintf(stderr, "%s: groups[%d]=%d\n", __func__, i, groups[i].data);
      }
    }
    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_4_2 (cce_destination_t upper_L)
/* Test for "ccsys_setgroups()". */
{
#if ((defined HAVE_GETGROUPS) && (defined HAVE_SETGROUPS))
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (cce_condition_is_errno(cce_condition(L))) {
      CCSYS_PC(cce_condition_errno_t, C, cce_condition(L));

      if (EPERM == C->errnum) {
	fprintf(stderr, "%s: no permissions to set groups, but it's OK\n", __func__);
      } else {
	cce_run_error_handlers_raise(L, upper_L);
      }
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    int		count;

    count = ccsys_getgroups_acquire_groups_number(L);
    {
      ccsys_gid_t	groups[count];
      int		ngroups;

      ngroups = ccsys_getgroups(L, count, groups);
      cctests_assert(L, ngroups == count);

      ccsys_setgroups(L, ngroups, groups);
    }
    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_4_3 (cce_destination_t upper_L)
/* Test for "ccsys_initgroups()". */
{
#if ((defined HAVE_GETGID) && (defined HAVE_INITGROUPS))
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (cce_condition_is_errno(cce_condition(L))) {
      CCSYS_PC(cce_condition_errno_t, C, cce_condition(L));

      if (EPERM == C->errnum) {
	fprintf(stderr, "%s: no permissions to set groups, but it's OK\n", __func__);
      } else {
	cce_run_error_handlers_raise(L, upper_L);
      }
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    ccsys_gid_t		gid;
    char const *	username = "marco";

    gid = ccsys_getgid();
    ccsys_initgroups(L, username, gid);

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_4_4 (cce_destination_t upper_L)
/* Test for "ccsys_getgrouplist()". */
{
#if (defined HAVE_GETGROUPLIST)
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	username = "marco";
    int			count;

    count = ccsys_getgrouplist_acquire_groups_number(L, username);
    if (1) { fprintf(stderr, "%s: count of groups: %d\n", __func__, count); }
    {
      ccsys_gid_t	gid = { .data = 0 };
      int		ngroups = count;
      ccsys_gid_t	groups[ngroups];

      ccsys_getgrouplist(L, username, gid, groups, &ngroups);
      if (1) { fprintf(stderr, "%s: ngroups=%d\n", __func__, ngroups); }
      cctests_assert(L, ((ngroups == count) || (ngroups == (count-1))));

      for (int i=0; i<ngroups; ++i) {
	fprintf(stderr, "%s: groups[%d]=%d\n", __func__, i, groups[i].data);
      }
    }
    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Users and groups: getting the user login name.
 ** ----------------------------------------------------------------- */

void
test_5_1 (cce_destination_t upper_L)
{
#if (defined HAVE_GETLOGIN_R)
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    size_t	maxlen = CCSYS_L_CUSERID;
    char	username[maxlen];

    ccsys_getlogin_r(L, username, maxlen);
    if (1) { fprintf(stderr, "%s: login user=%s\n", __func__, username); }

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Users and groups: looking up in the users database.
 ** ----------------------------------------------------------------- */

void
test_6_1 (cce_destination_t upper_L)
/* Using "getpwuid()". */
{
#if (defined HAVE_GETPWUID)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_passwd_t const *	S;

    S = ccsys_getpwuid(L, ccsys_getuid());

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_NAME)
    fprintf(stderr, "%s: pw_name=%s\n",		__func__, ccsys_ref_passwd_pw_name(S));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_PASSWD)
    fprintf(stderr, "%s: pw_passwd=%s\n",	__func__, ccsys_ref_passwd_pw_passwd(S));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_UID)
    fprintf(stderr, "%s: pw_uid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_uid(S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GID)
    fprintf(stderr, "%s: pw_gid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_gid(S)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GECOS)
    fprintf(stderr, "%s: pw_gecos=%s\n",	__func__, ccsys_ref_passwd_pw_gecos(S));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_DIR)
    fprintf(stderr, "%s: pw_dir=%s\n",		__func__, ccsys_ref_passwd_pw_dir(S));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_SHELL)
    fprintf(stderr, "%s: pw_shell=%s\n",	__func__, ccsys_ref_passwd_pw_shell(S));
#endif

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_6_2 (cce_destination_t upper_L)
/* Using "getpwnam()". */
{
#if ((defined HAVE_GETPWUID) && (defined HAVE_GETPWNAM))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_passwd_t const	*A, *B;

    A = ccsys_getpwuid(L, ccsys_getuid());
    B = ccsys_getpwnam(L, ccsys_ref_passwd_pw_name(A));

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_NAME)
    fprintf(stderr, "%s: pw_name=%s\n",		__func__, ccsys_ref_passwd_pw_name(B));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_PASSWD)
    fprintf(stderr, "%s: pw_passwd=%s\n",	__func__, ccsys_ref_passwd_pw_passwd(B));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_UID)
    fprintf(stderr, "%s: pw_uid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_uid(B)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GID)
    fprintf(stderr, "%s: pw_gid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_gid(B)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GECOS)
    fprintf(stderr, "%s: pw_gecos=%s\n",	__func__, ccsys_ref_passwd_pw_gecos(B));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_DIR)
    fprintf(stderr, "%s: pw_dir=%s\n",		__func__, ccsys_ref_passwd_pw_dir(B));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_SHELL)
    fprintf(stderr, "%s: pw_shell=%s\n",	__func__, ccsys_ref_passwd_pw_shell(B));
#endif

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_6_3 (cce_destination_t upper_L)
/* Using "getpwuid_r()". */
{
#if (defined HAVE_GETPWUID_R)
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_passwd_t 	S;
    ccsys_passwd_t *	R;
    size_t		buflen = 4 * 4096;
    char		bufptr[buflen];
    bool		rv;

    rv = ccsys_getpwuid_r(L, ccsys_getuid(), &S, bufptr, buflen, &R);
    cctests_assert(L, true == rv);

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_NAME)
    fprintf(stderr, "%s: pw_name=%s\n",		__func__, ccsys_ref_passwd_pw_name(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_PASSWD)
    fprintf(stderr, "%s: pw_passwd=%s\n",	__func__, ccsys_ref_passwd_pw_passwd(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_UID)
    fprintf(stderr, "%s: pw_uid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_uid(R)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GID)
    fprintf(stderr, "%s: pw_gid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_gid(R)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GECOS)
    fprintf(stderr, "%s: pw_gecos=%s\n",	__func__, ccsys_ref_passwd_pw_gecos(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_DIR)
    fprintf(stderr, "%s: pw_dir=%s\n",		__func__, ccsys_ref_passwd_pw_dir(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_SHELL)
    fprintf(stderr, "%s: pw_shell=%s\n",	__func__, ccsys_ref_passwd_pw_shell(R));
#endif

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_6_4 (cce_destination_t upper_L)
/* Using "getpwnam_r()". */
{
#if ((defined HAVE_GETPWUID) && (defined HAVE_GETPWNAM_R))
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsys_passwd_t const *	A;
    ccsys_passwd_t		S;
    ccsys_passwd_t *		R;
    size_t			buflen = 4 * 4096;
    char			bufptr[buflen];
    bool			rv;

    A  = ccsys_getpwuid(L, ccsys_getuid());
    rv = ccsys_getpwnam_r(L, ccsys_ref_passwd_pw_name(A), &S, bufptr, buflen, &R);
    cctests_assert(L, true == rv);

#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_NAME)
    fprintf(stderr, "%s: pw_name=%s\n",		__func__, ccsys_ref_passwd_pw_name(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_PASSWD)
    fprintf(stderr, "%s: pw_passwd=%s\n",	__func__, ccsys_ref_passwd_pw_passwd(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_UID)
    fprintf(stderr, "%s: pw_uid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_uid(R)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GID)
    fprintf(stderr, "%s: pw_gid=%d\n",		__func__, ccsys_dref(ccsys_ref_passwd_pw_gid(R)));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_GECOS)
    fprintf(stderr, "%s: pw_gecos=%s\n",	__func__, ccsys_ref_passwd_pw_gecos(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_DIR)
    fprintf(stderr, "%s: pw_dir=%s\n",		__func__, ccsys_ref_passwd_pw_dir(R));
#endif
#if (1 == CCSYS_HAVE_STRUCT_PASSWD_PW_SHELL)
    fprintf(stderr, "%s: pw_shell=%s\n",	__func__, ccsys_ref_passwd_pw_shell(R));
#endif

    cce_run_cleanup_handlers(L);
  }
#endif
}


int
main (void)
{
  cctests_init("users and groups");
  {
    cctests_begin_group("reading the persona of a process");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
      cctests_run(test_1_4);
    }
    cctests_end_group();

    cctests_begin_group("setting the user identity");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
      cctests_run(test_2_3);
    }
    cctests_end_group();

    cctests_begin_group("setting the group identity");
    {
      cctests_run(test_3_1);
      cctests_run(test_3_2);
      cctests_run(test_3_3);
    }
    cctests_end_group();

    cctests_begin_group("additional group identities");
    {
      cctests_run(test_4_1);
      cctests_run(test_4_2);
      cctests_run(test_4_3);
      cctests_run(test_4_4);
    }
    cctests_end_group();

    cctests_begin_group("getting the user login name");
    {
      cctests_run(test_5_1);
    }
    cctests_end_group();

    cctests_begin_group("the users database");
    {
      cctests_run(test_6_1);
      cctests_run(test_6_2);
      cctests_run(test_6_3);
      cctests_run(test_6_4);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
