/*
  Part of: CCSys
  Contents: public header file
  Date: Sat Dec 17, 2016

  Abstract

	This is the  main header: it must be included  in all the source
	files  using  CCSys.   Notice  that the  code  needs  the  POSIX
	features, so it defines the following symbol:

	   #define _POSIX_C_SOURCE 200809L

  Copyright (C) 2016, 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it  under the  terms  of  the GNU  Lesser  General  Public License  as
  published by  the Free  Software Foundation, either  version 3  of the
  License, or (at your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have  received a  copy of  the GNU  Lesser General  Public
  License     along    with     this    program.      If    not,     see
  <http://www.gnu.org/licenses/>.
*/

#ifndef CCSYS_H
#define CCSYS_H 1


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The macro  CCSYS_UNUSED indicates that a  function, function argument
   or variable may potentially be unused. Usage examples:

   static int unused_function (char arg) CCSYS_UNUSED;
   int foo (char unused_argument CCSYS_UNUSED);
   int unused_variable CCSYS_UNUSED;
*/
#ifdef __GNUC__
#  define CCSYS_UNUSED		__attribute__((unused))
#else
#  define CCSYS_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

#ifndef __GNUC__
#  define __builtin_expect(...)	/* empty */
#endif

#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ccsys_decl		__attribute__((dllexport)) extern
#    else
#      define ccsys_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ccsys_decl		__attribute__((dllimport)) extern
#    else
#      define ccsys_decl		__declspec(dllimport) extern
#    endif
#  endif
#  define ccsys_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define ccsys_decl		__attribute__((visibility ("default"))) extern
#    define ccsys_private_decl	__attribute__((visibility ("hidden")))  extern
#  else
#    define ccsys_decl		extern
#    define ccsys_private_decl	extern
#  endif
#endif


/** --------------------------------------------------------------------
 ** Helper macros.
 ** ----------------------------------------------------------------- */

/* Pointer cast macro helper. */
#define CCSYS_PC(TYPE,X,Y)		TYPE * X = (TYPE *) (Y)


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>

/* Enable latest POSIX features. */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE		200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h> /* for offsetof() */
#include <setjmp.h>
#include <errno.h>
#include <unistd.h>


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccsys_decl const char *	ccsys_version_string		(void)
  __attribute__((leaf,pure));
ccsys_decl int		ccsys_version_interface_current	(void)
  __attribute__((leaf,pure));
ccsys_decl int		ccsys_version_interface_revision(void)
  __attribute__((leaf,pure));
ccsys_decl int		ccsys_version_interface_age	(void)
  __attribute__((leaf,pure));


/** --------------------------------------------------------------------
 ** System call wrappers: memory allocation.
 ** ----------------------------------------------------------------- */

ccsys_decl void * ccsys_malloc (cce_location_t * L, size_t size)
  __attribute__((nonnull(1),returns_nonnull));

ccsys_decl void * ccsys_realloc (cce_location_t * L, void * ptr, size_t newsize)
  __attribute__((nonnull(1),returns_nonnull));

ccsys_decl void * ccsys_calloc (cce_location_t * L, size_t count, size_t eltsize)
  __attribute__((nonnull(1),returns_nonnull));


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: malloc pointer.
 ** ----------------------------------------------------------------- */

ccsys_decl void ccsys_cleanup_handler_malloc_init (cce_location_t * L, cce_handler_t * H, void * pointer)
  __attribute__((nonnull(1,2,3)));

ccsys_decl void ccsys_error_handler_malloc_init (cce_location_t * L, cce_handler_t * H, void * pointer)
  __attribute__((nonnull(1,2,3)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCSYS_H */

/* end of file */
