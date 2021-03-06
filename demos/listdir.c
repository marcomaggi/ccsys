/*
  Part of: CCSys
  Contents: list directory contents
  Date: Mon Mar 13, 2017

  Abstract



  Copyright (C) 2017, 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  The author  hereby grant permission to use,  copy, modify, distribute,
  and  license this  software  and its  documentation  for any  purpose,
  provided that  existing copyright notices  are retained in  all copies
  and that  this notice  is included verbatim  in any  distributions. No
  written agreement, license, or royalty  fee is required for any of the
  authorized uses.  Modifications to this software may be copyrighted by
  their authors and need not  follow the licensing terms described here,
  provided that the new terms are clearly indicated on the first page of
  each file where they apply.

  IN NO  EVENT SHALL THE AUTHOR  OR DISTRIBUTORS BE LIABLE  TO ANY PARTY
  FOR  DIRECT, INDIRECT, SPECIAL,  INCIDENTAL, OR  CONSEQUENTIAL DAMAGES
  ARISING OUT  OF THE  USE OF THIS  SOFTWARE, ITS DOCUMENTATION,  OR ANY
  DERIVATIVES  THEREOF, EVEN  IF THE  AUTHOR  HAVE BEEN  ADVISED OF  THE
  POSSIBILITY OF SUCH DAMAGE.

  THE  AUTHOR  AND DISTRIBUTORS  SPECIFICALLY  DISCLAIM ANY  WARRANTIES,
  INCLUDING,   BUT   NOT  LIMITED   TO,   THE   IMPLIED  WARRANTIES   OF
  MERCHANTABILITY,    FITNESS   FOR    A    PARTICULAR   PURPOSE,    AND
  NON-INFRINGEMENT.  THIS  SOFTWARE IS PROVIDED  ON AN "AS  IS" BASIS,
  AND  THE  AUTHOR  AND  DISTRIBUTORS  HAVE  NO  OBLIGATION  TO  PROVIDE
  MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#include "ccsys.h"
#include <stdio.h>

static int list_dirs (const char * pathname);

static const char *	progname = "listdir";

int
main (int argc, char const *const argv[])
{
  const char *	pathname = (2 == argc)? argv[1] : "./";
  int		rv;

  rv = list_dirs(pathname);
  exit(rv);
}

int
list_dirs (char const * pathname)
{
  cce_location_t		L[1];
  cce_clean_handler_t		dirstream_H[1];
  ccstructs_clean_handler_t	ptn_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: error: %s\n", progname,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_final(L);
    return EXIT_FAILURE;
  } else {
    ccsys_dir_t			*dirstream;
    ccsys_dirent_t		*direntry;
    ccstructs_pathname_t const	*ptn;
    ccstructs_pathname_I	IPTN;

    ptn  = ccname_new(ccstructs_pathname_t, from_chars)(L, pathname);
    IPTN = ccname_iface_new(ccstructs_pathname_I, ccstructs_pathname_t)(ptn);
    ccstructs_clean_handler_init(L, ptn_H, ccname_iface_new(ccstructs_dtor_I, ccstructs_pathname_I)(IPTN));

    dirstream = ccsys_opendir(L, IPTN);
    ccsys_init_dirstream_clean_handler(L, dirstream_H, dirstream);
    while ((direntry = ccsys_readdir(L, dirstream))) {
      printf("%s\n", ccsys_ref_dirent_d_name(direntry));
      fflush(stdout);
    }
    cce_run_body_handlers(L);
    return EXIT_SUCCESS;
  }
}

/* end of file */
