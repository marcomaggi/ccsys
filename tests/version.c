/*
  Part of: CCSys
  Contents: test for version functions
  Date: Feb  6, 2018

  Abstract

	Test file for version functions.

  Copyright (C) 2016, 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <ccsys.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc CCSYS_UNUSED, const char *const argv[] CCSYS_UNUSED)
{
  printf("version number string: %s\n", ccsys_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 ccsys_version_interface_current(),
	 ccsys_version_interface_revision(),
	 ccsys_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
