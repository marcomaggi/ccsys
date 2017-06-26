/*
  Part of: CCExceptions
  Contents: tests for predefined POSIX exception handlers
  Date: Sat Mar 11, 2017

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

#include "ccexceptions.h"
#include "ccexceptions-system.h"
#include "ccexceptions-networking.h"
#include <assert.h>


void
test_handler_malloc (void)
{
  /* No error.  Cleanup call. */
  {
    cce_location_t	L[1];
    cce_handler_t	H[1];
    void *		P;
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      P = cce_sys_malloc(L, 4096);
      cce_cleanup_handler_malloc_init(L, H, P);
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_handler_t	H[1];
    void *		P;
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      P = cce_sys_malloc(L, 4096);
      cce_cleanup_handler_malloc_init(L, H, P);
      cce_raise(L, cce_condition_new_unknown());
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


void
test_handler_filedes (void)
{
  /* No error.  Cleanup call. */
  {
    cce_location_t	L[1];
    cce_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      int   fd = cce_sys_open(L, "name.ext", O_CREAT, S_IRWXU);
      cce_cleanup_handler_filedes_init(L, H, fd);
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    remove("name.ext");
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      int   fd = cce_sys_open(L, "name.ext", O_CREAT, S_IRWXU);
      cce_cleanup_handler_filedes_init(L, H, fd);
      cce_raise(L, cce_condition_new_unknown());
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    remove("name.ext");
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


void
test_handler_pipedes (void)
{
  /* No error.  Cleanup call. */
  {
    cce_location_t	L[1];
    cce_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      int	pipedes[2];
      cce_sys_pipe(L, pipedes);
      cce_cleanup_handler_pipedes_init(L, H, pipedes);
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      int	pipedes[2];
      cce_sys_pipe(L, pipedes);
      cce_cleanup_handler_pipedes_init(L, H, pipedes);
      cce_raise(L, cce_condition_new_unknown());
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


void
test_handler_tmpfile (void)
{
  /* No error.  Cleanup call. */
  {
    cce_location_t	L[1];
    cce_handler_t	filedes_H[1];
    cce_handler_t	tmpfile_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      int   fd = cce_sys_open(L, "name.ext", O_CREAT, S_IRUSR|S_IWUSR);
      cce_cleanup_handler_filedes_init(L, filedes_H, fd);
      cce_cleanup_handler_tmpfile_init(L, tmpfile_H, "name.ext");
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_handler_t	filedes_H[1];
    cce_handler_t	tmpfile_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      int   fd = cce_sys_open(L, "name.ext", O_CREAT, S_IRUSR|S_IWUSR);
      cce_cleanup_handler_filedes_init(L, filedes_H, fd);
      cce_cleanup_handler_tmpfile_init(L, tmpfile_H, "name.ext");
      cce_raise(L, cce_condition_new_unknown());
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


void
test_handler_tmpdir (void)
{
  /* No error.  Cleanup call. */
  {
    cce_location_t	L[1];
    cce_handler_t	tmpdir_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      cce_sys_mkdir(L, "name.d", 0);
      cce_cleanup_handler_tmpdir_init(L, tmpdir_H, "name.d");
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_handler_t	tmpdir_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_error_handlers(L);
      cce_condition_delete(cce_condition(L));
      error_flag = true;
    } else {
      cce_sys_mkdir(L, "name.d", 0);
      cce_cleanup_handler_tmpdir_init(L, tmpdir_H, "name.d");
      cce_raise(L, cce_condition_new_unknown());
      cce_run_cleanup_handlers(L);
      done_flag = true;
    }
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


int
main (int argc CCE_UNUSED, const char *const argv[] CCE_UNUSED)
{
  if (1) test_handler_malloc();
  if (1) test_handler_filedes();
  if (1) test_handler_pipedes();
  if (1) test_handler_tmpfile();
  if (1) test_handler_tmpdir();
  exit(EXIT_SUCCESS);
}

/* end of file */
