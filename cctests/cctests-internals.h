/*
  Part of: CCTests
  Contents: private header file
  Date: Nov 28, 2017

  Abstract

	This  header  file is  for  internal  definitions.  It  must  be
	included by all the source files in this package.

  Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCTESTS_INTERNALS_H
#define CCTESTS_INTERNALS_H 1


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "cctests.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>


/** --------------------------------------------------------------------
 ** Preprocessor macros.
 ** ----------------------------------------------------------------- */

#define CCTESTS_ENVIRONMENT_VARIABLE_FILE	"cctests_file"
#define CCTESTS_ENVIRONMENT_VARIABLE_GROUP	"cctests_group"
#define CCTESTS_ENVIRONMENT_VARIABLE_NAME	"cctests_name"

#define CCTESTS_AUTOMAKE_TEST_HARNESS_CODE_SUCCESS	0
#define CCTESTS_AUTOMAKE_TEST_HARNESS_CODE_FAILURE	1
#define CCTESTS_AUTOMAKE_TEST_HARNESS_CODE_SKIP_TEST	77
#define CCTESTS_AUTOMAKE_TEST_HARNESS_CODE_HARD_ERROR	99


/** --------------------------------------------------------------------
 ** Constants.
 ** ----------------------------------------------------------------- */



/** --------------------------------------------------------------------
 ** Library variables.
 ** ----------------------------------------------------------------- */



/** --------------------------------------------------------------------
 ** Function prototypes.
 ** ----------------------------------------------------------------- */

cctests_private_decl void cctests_conditions_module_initialisation (void);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCTESTS_INTERNALS_H */

/* end of file */
