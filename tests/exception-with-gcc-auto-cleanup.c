/*
  Part of: CCExceptions
  Contents: test for GCC automatic variable cleanup
  Date: Dec 24, 2016

  Abstract



  Copyright (C) 2016, 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#include <ccexceptions.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


static bool	test_no_exception_out_flag1 = false;
static bool	test_no_exception_out_flag2 = false;

static void
test_no_exception_handler1 (volatile bool * flagp)
{
  test_no_exception_out_flag1 = *flagp;
}
static void
test_no_exception_handler2 (volatile bool * flagp)
{
  test_no_exception_out_flag2 = *flagp;
}

static void
test_no_exception (void)
/* Show  the  interaction  between  CCException's  mechanism  and  GCC's
   built-in handler mechanism.  Case with no exceptions. */
{
  /* Upon exiting this block: the handlers will be called. */
  {
    cce_location_t	L[1];
    volatile bool	flag1 __attribute__((cleanup(test_no_exception_handler1))) = false;
    volatile bool	flag2 __attribute__((cleanup(test_no_exception_handler2))) = false;

    switch (cce_location(L)) {
    case CCE_ERROR:
      flag1 = false;
      flag2 = false;
      cce_run_error_handlers(L);
      break;

    default:
      flag1 = true;
      flag2 = true;
      cce_run_cleanup_handlers(L);
    }
    assert(true == flag1);
    assert(true == flag2);
  }
  assert(true == test_no_exception_out_flag1);
  assert(true == test_no_exception_out_flag2);
}


static bool	test_with_error_out_flag1 = false;
static bool	test_with_error_out_flag2 = false;

static void
test_with_error_handler1 (volatile bool * flagp)
{
  test_with_error_out_flag1 = *flagp;
}
static void
test_with_error_handler2 (volatile bool * flagp)
{
  test_with_error_out_flag2 = *flagp;
}

static void
test_with_error (void)
/* Show  the  interaction  between  CCException's  mechanism  and  GCC's
   built-in handler mechanism.  Case with exception. */
{
  /* Upon exiting this block: the handlers will be called. */
  {
    cce_location_t	L[1];
    volatile bool	flag1 __attribute__((cleanup(test_with_error_handler1))) = false;
    volatile bool	flag2 __attribute__((cleanup(test_with_error_handler2))) = false;

    switch (cce_location(L)) {
    case CCE_ERROR:
      flag1 = false;
      flag2 = false;
      cce_run_error_handlers(L);
      break;

    default:
      flag1 = true;
      flag2 = true;
      cce_raise(L, NULL);
      cce_run_cleanup_handlers(L);
    }
    assert(false == flag1);
    assert(false == flag2);
  }
  assert(false == test_with_error_out_flag1);
  assert(false == test_with_error_out_flag2);
}


static bool	test_with_retry_out_flag1 = false;
static bool	test_with_retry_out_flag2 = false;

static void
test_with_retry_handler1 (volatile bool * flagp)
{
  test_with_retry_out_flag1 = *flagp;
}
static void
test_with_retry_handler2 (volatile bool * flagp)
{
  test_with_retry_out_flag2 = *flagp;
}

static void
test_with_retry (void)
/* Show  the  interaction  between  CCException's  mechanism  and  GCC's
   built-in handler mechanism.  Case with retry. */
{
  {
    cce_location_t	L[1];
    volatile bool	flag1 __attribute__((cleanup(test_with_retry_handler1))) = false;
    volatile bool	flag2 __attribute__((cleanup(test_with_retry_handler2))) = false;

    switch (cce_location(L)) {
    case CCE_ERROR:
      flag1 = false;
      flag2 = false;
      cce_run_error_handlers(L);
      break;

    case CCE_SUCCESS:
      flag1 = true;
      flag2 = true;
      if (1) {
	cce_retry(L);
      }
      // else fall through

    default:
      cce_run_cleanup_handlers(L);
    }
    assert(true == flag1);
    assert(true == flag2);
  }
  assert(true == test_with_retry_out_flag1);
  assert(true == test_with_retry_out_flag2);
}


int
main (int argc CCE_UNUSED, const char *const argv[] CCE_UNUSED)
{
  test_no_exception();
  test_with_error();
  test_with_retry();

  exit(EXIT_SUCCESS);
}

/* end of file */
