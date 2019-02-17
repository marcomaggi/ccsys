/*
  Part of: CCSys
  Contents: private header file
  Date: Sat Dec 17, 2016

  Abstract

	This  header  file is  for  internal  definitions.  It  must  be
	included by all the source files in this package.

  Copyright (C) 2016, 2017, 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCSYS_INTERNALS_H
#define CCSYS_INTERNALS_H 1

#ifdef __cplusplus
extern "C" {
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

/* Remember that "config.h" defines:
 *
 *   #define _POSIX_C_SOURCE		200809L
 */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "ccsys.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h> /* for offsetof() */
#include <setjmp.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#ifdef HAVE_STDBOOL_H
#  include <stdbool.h>
#else
#  ifndef HAVE__BOOL
#    ifdef __cplusplus
typedef bool _Bool;
#    else
#      define _Bool signed char
#    endif
#  endif
#  define bool _Bool
#  define false 0
#  define true 1
#  define __bool_true_false_are_defined 1
#endif


/** --------------------------------------------------------------------
 ** Internal functions.
 ** ----------------------------------------------------------------- */

ccsys_private_decl struct timeval ccsys_timeval_to_timeval (ccsys_timeval_t in);
ccsys_private_decl ccsys_timeval_t ccsys_timeval_from_timeval (struct timeval in);

ccsys_private_decl struct timespec ccsys_timespec_to_timespec (ccsys_timespec_t in);
ccsys_private_decl ccsys_timespec_t ccsys_timespec_from_timespec (struct timespec in);



/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCSYS_INTERNALS_H */

/* end of file */
