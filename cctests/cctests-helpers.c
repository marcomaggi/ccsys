/*
  Part of: CCTests
  Contents: helper functions
  Date: Thu Feb  1, 2018

  Abstract



  Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it  under the  terms  of  the GNU  Lesser  General  Public License  as
  published by  the Free  Software Foundation, either  version 3  of the
  License, or (at your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "cctests-internals.h"
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif


/** --------------------------------------------------------------------
 ** Input/output devices.
 ** ----------------------------------------------------------------- */

bool
cctests_isatty (FILE * stream)
/* Return  true if  the  "stream"  is on  a  terminal; otherwise  return
   false. */
{
#ifdef HAVE_ISATTY
  return isatty(fileno(stream));
#else
  return false;
#endif
}

bool
cctests_log_stream_isatty (void)
/* Return true if CCTests' log stream is on a terminal; otherwise return
   false. */
{
  return cctests_isatty(cctests_log_stream);
}

/* end of file */
