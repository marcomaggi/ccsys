/*
  Part of: CCExceptions
  Contents: tests for predefined POSIX exception handlers
  Date: Sat Mar 11, 2017

  Abstract



  Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#include "ccsys.h"
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <ccmemory.h>
#include <assert.h>
#include <stdio.h>


void
test_handler_malloc (void)
{
  /* No error.  Clean call. */
  {
    cce_location_t		L[1];
    ccmem_clean_handler_t	P_H[1];
    void *			P;
    volatile bool		done_flag  = false;
    volatile bool		error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      P = ccmem_std_malloc_guarded(L, P_H, 4096);
      if (0) { fprintf(stderr, "%s: %p\n", __func__, P); }
      cce_run_body_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t		L[1];
    ccmem_clean_handler_t	P_H[1];
    void *			P;
    volatile bool		done_flag  = false;
    volatile bool		error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      P = ccmem_std_malloc_guarded(L, P_H, 4096);
      if (0) { fprintf(stderr, "%s: %p\n", __func__, P); }
      cce_raise(L, cce_condition_new_unknown());
      cce_run_body_handlers(L);
      done_flag = true;
    }
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


void
test_handler_filedes (void)
{
  /* No error.  Clean call. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_open_flags_t	flags = { .data = CCSYS_O_CREAT };
      ccsys_open_mode_t		mode  = { .data = CCSYS_S_IRWXU };
      ccsys_fd_t		fd   = ccsys_open(L, "name.ext", flags, mode);
      ccsys_init_filedes_clean_handler(L, H, fd);
      cce_run_body_handlers(L);
      done_flag = true;
    }
    remove("name.ext");
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_open_flags_t	flags = { .data = CCSYS_O_CREAT };
      ccsys_open_mode_t		mode = { .data = CCSYS_S_IRWXU };
      ccsys_fd_t		fd   = ccsys_open(L, "name.ext", flags, mode);
      ccsys_init_filedes_clean_handler(L, H, fd);
      cce_raise(L, cce_condition_new_unknown());
      cce_run_body_handlers(L);
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
  /* No error.  Clean call. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_fd_t	pipedes[2];
      ccsys_pipe(L, pipedes);
      ccsys_init_pipedes_clean_handler(L, H, pipedes);
      cce_run_body_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_fd_t	pipedes[2];
      ccsys_pipe(L, pipedes);
      ccsys_init_pipedes_clean_handler(L, H, pipedes);
      cce_raise(L, cce_condition_new_unknown());
      cce_run_body_handlers(L);
      done_flag = true;
    }
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


void
test_handler_remove (void)
{
  /* No error.  Clean call. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	filedes_H[1];
    cce_handler_t	remove_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_open_flags_t	flags = { .data = CCSYS_O_CREAT };
      ccsys_open_mode_t	mode = { .data = CCSYS_S_IRUSR|CCSYS_S_IWUSR };
      ccsys_fd_t	fd = ccsys_open(L, "name.ext", flags, mode);
      ccsys_init_filedes_clean_handler(L, filedes_H, fd);
      ccsys_clean_handler_remove_init(L, remove_H, "name.ext");
      cce_run_body_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	filedes_H[1];
    cce_handler_t	remove_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_open_flags_t	flags = { .data = CCSYS_O_CREAT };
      ccsys_open_mode_t	mode = { .data = CCSYS_S_IRUSR|CCSYS_S_IWUSR };
      ccsys_fd_t	fd   = ccsys_open(L, "name.ext", flags, mode);
      ccsys_init_filedes_clean_handler(L, filedes_H, fd);
      ccsys_clean_handler_remove_init(L, remove_H, "name.ext");
      cce_raise(L, cce_condition_new_unknown());
      cce_run_body_handlers(L);
      done_flag = true;
    }
    assert(true  == error_flag);
    assert(false == done_flag);
  }
}


void
test_handler_rmdir (void)
{
  /* No error.  Clean call. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	rmdir_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_open_mode_t	mode = { .data = 0 };
      ccsys_mkdir(L, "name.d", mode);
      ccsys_init_rmdir_handler(L, rmdir_H, "name.d");
      cce_run_body_handlers(L);
      done_flag = true;
    }
    assert(false == error_flag);
    assert(true  == done_flag);
  }
  /* Error. */
  {
    cce_location_t	L[1];
    cce_clean_handler_t	rmdir_H[1];
    volatile bool	done_flag  = false;
    volatile bool	error_flag = false;

    if (cce_location(L)) {
      cce_run_catch_handlers_final(L);
      error_flag = true;
    } else {
      ccsys_open_mode_t	mode = { .data = 0 };
      ccsys_mkdir(L, "name.d", mode);
      ccsys_init_rmdir_handler(L, rmdir_H, "name.d");
      cce_raise(L, cce_condition_new_unknown());
      cce_run_body_handlers(L);
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
  if (1) test_handler_remove();
  if (1) test_handler_rmdir();
  exit(EXIT_SUCCESS);
}

/* end of file */
