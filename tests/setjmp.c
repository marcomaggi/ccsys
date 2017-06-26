/*
  Part of: CCExceptions
  Contents: test for setjmp()
  Date: Dec 23, 2016

  Abstract

	Test  file for  "setjmp()"  and  "longjmp()" functions  standard
	behaviour.

  Copyright (C) 2016, 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#define _POSIX_C_SOURCE		199506L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#define JUMP_TO_ERROR		2
#define JUMP_TO_RETRY		3

int
main (int argc __attribute__((unused)), const char *const argv[] __attribute__((unused)))
{
  /* No exception.  Call "setjmp()" but not "longjmp()". */
  {
    jmp_buf	L;
    int		flag = 0, code = 0;

    code = setjmp(L);
    if (code) {
      // error occurred
      flag = 2;
    } else {
      // the body
      flag = 1;
    }
    assert(0 == code);
    assert(1 == flag);
  }

  /* Exception  with  "if"  logic.    Call  "setjmp()"  and  "longjmp()"
     once. */
  {
    jmp_buf	L;
    int		code;
    int		flag = 0;

    code = setjmp(L);
    if (JUMP_TO_ERROR == code) {
      // error occurred
      flag = 2;
    } else {
      // the body
      longjmp(L, JUMP_TO_ERROR);
      flag = 1;
    }
    assert(JUMP_TO_ERROR == code);
    assert(2 == flag);
  }

  /* Exception  with "switch"  logic.  Call  "setjmp()" and  "longjmp()"
     once. */
  {
    jmp_buf	L;
    int		code;
    int		flag = 0;

    code = setjmp(L);
    switch (code) {
    case JUMP_TO_ERROR:
      // error occurred
      flag = 2;
      break;
    default:
      // the body
      longjmp(L, JUMP_TO_ERROR);
      flag = 1;
      break;
    }
    assert(JUMP_TO_ERROR == code);
    assert(2 == flag);
  }

  /* Exception  with "switch"  logic.   Retrying with  a handler.   Call
     "setjmp()" and "longjmp()" twice to retry. */
  {
    jmp_buf	L;
    int		code;
    int		flag = 0;

    code = setjmp(L);
    switch (code) {
    case JUMP_TO_ERROR:
      // error occurred
      longjmp(L, JUMP_TO_RETRY);
      flag = 2;
      break;
    case JUMP_TO_RETRY:
      // error occurred, retry
      flag = 3;
      break;
    default:
      // the body
      longjmp(L, JUMP_TO_ERROR);
      flag = 1;
      break;
    }
    assert(JUMP_TO_RETRY == code);
    assert(3 == flag);
  }

  /* Exception with "switch" logic.  Retrying the body.  Call "setjmp()"
     and "longjmp()" twice to retry. */
  {
    jmp_buf	L;
    int		code;
    int		flag = 0;

    code = setjmp(L);
    switch (code) {
    case JUMP_TO_ERROR:
      // error occurred
      longjmp(L, JUMP_TO_RETRY);
      flag = 2;
      break;
    default:
      if (JUMP_TO_RETRY == code) {
	// retrying
	flag = 3;
      } else {
	// the body
	longjmp(L, JUMP_TO_ERROR);
	flag = 1;
      }
      break;
    }
    assert(JUMP_TO_RETRY == code);
    assert(3 == flag);
  }

  /* Exception with  "switch" logic.  Retrying  the body 5  times.  Call
     "setjmp()" and "longjmp()" multiple times to retry. */
  {
    jmp_buf		L;
    volatile int	times = 0;
    int			code = 0;
    int			flag = 0;

    code = setjmp(L);
    switch (code) {
    case JUMP_TO_ERROR:
      // error occurred
      longjmp(L, JUMP_TO_RETRY);
      flag = 2;
      break;
    default:
      if ((JUMP_TO_RETRY == code) && (times < 5)) {
	// retrying
	++times;
	longjmp(L, JUMP_TO_ERROR);
      } else if (JUMP_TO_RETRY == code) {
	flag = 3;
      } else {
	// the body
	longjmp(L, JUMP_TO_ERROR);
	flag = 1;
      }
      break;
    }
    assert(JUMP_TO_RETRY == code);
    assert(3 == flag);
  }

  exit(EXIT_SUCCESS);
}

/* end of file */
