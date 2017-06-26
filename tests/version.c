/*
  Part of: CCExceptions
  Contents: test for version functions
  Date: Sat Dec 17, 2016

  Abstract

	Test file for version functions.

  Copyright (C) 2016, 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <ccexceptions.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc CCE_UNUSED, const char *const argv[] CCE_UNUSED)
{
  printf("version number string: %s\n", cce_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 cce_version_interface_current(),
	 cce_version_interface_revision(),
	 cce_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
