/*
  Part of: CCExceptions
  Contents: prints size of data structures
  Date: Thu Jun  1, 2017

  Abstract



  Copyright (C) 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

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


#include "ccexceptions.h"
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  printf("machine word size: %lu\n", sizeof(void *));

  printf("size of cce_location_t: %lu, number of words: %lu\n",
	 sizeof(cce_location_t), sizeof(cce_location_t)/sizeof(void *));

  printf("size of cce_handler_t:  %lu, number of words: %lu\n",
	 sizeof(cce_handler_t), sizeof(cce_handler_t)/sizeof(void *));
  exit(EXIT_SUCCESS);
}

/* end of file */
