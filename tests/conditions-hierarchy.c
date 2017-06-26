/*
  Part of: CCExceptions
  Contents: tests for exceptional condition descriptors hierarchy
  Date: Dec 28, 2016

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
#include <errno.h>
#include <string.h>

static const char *
default_static_message (const cce_condition_t * C CCE_UNUSED)
{
  return "default message";
}

/* ------------------------------------------------------------------ */

static cce_descriptor_t alpha_D = {
  /* This field is initialised later. */
  .parent		= NULL,
  .final		= NULL,
  .static_message	= default_static_message
};

static const cce_descriptor_t beta_D = {
  .parent		= &alpha_D,
  .final		= NULL,
  .static_message	= default_static_message
};

static const cce_descriptor_t gamma_D = {
  .parent		= &beta_D,
  .final		= NULL,
  .static_message	= default_static_message
};

/* ------------------------------------------------------------------ */

static const cce_condition_t alpha_C = {
  .descriptor		= &alpha_D
};

static const cce_condition_t beta_C = {
  .descriptor		= &beta_D
};

static const cce_condition_t gamma_C = {
  .descriptor		= &gamma_D
};


int
main (int argc CCE_UNUSED, const char *const argv[] CCE_UNUSED)
{
  /* Dynamic initialisation. */
  cce_descriptor_set_root_parent(&alpha_D);

  assert(true  == cce_condition_is_root(cce_condition(cce_condition_new_unknown())));
  assert(true  == cce_condition_is_root(cce_condition(cce_condition_new_errno(EINVAL))));
  assert(true  == cce_condition_is_root(&alpha_C));
  assert(true  == cce_condition_is_root(&beta_C));
  assert(true  == cce_condition_is_root(&gamma_C));

  assert(true  == cce_is_condition(cce_condition(cce_condition_new_unknown()),     cce_descriptor(cce_descriptor_unknown_ptr)));
  assert(false == cce_is_condition(cce_condition(cce_condition_new_errno(EINVAL)),	cce_descriptor(cce_descriptor_unknown_ptr)));
  assert(false == cce_is_condition(&alpha_C, cce_descriptor(cce_descriptor_unknown_ptr)));
  assert(false == cce_is_condition(&beta_C,  cce_descriptor(cce_descriptor_unknown_ptr)));
  assert(false == cce_is_condition(&gamma_C, cce_descriptor(cce_descriptor_unknown_ptr)));

  assert(false == cce_is_condition(cce_condition(cce_condition_new_unknown()),     cce_descriptor(cce_descriptor_errno_ptr)));
  assert(true  == cce_is_condition(cce_condition(cce_condition_new_errno(EINVAL)),	cce_descriptor(cce_descriptor_errno_ptr)));
  assert(false == cce_is_condition(&alpha_C, cce_descriptor(cce_descriptor_errno_ptr)));
  assert(false == cce_is_condition(&beta_C,  cce_descriptor(cce_descriptor_errno_ptr)));
  assert(false == cce_is_condition(&gamma_C, cce_descriptor(cce_descriptor_errno_ptr)));

  assert(false == cce_is_condition(cce_condition(cce_condition_new_unknown()),     &alpha_D));
  assert(false == cce_is_condition(cce_condition(cce_condition_new_errno(EINVAL)),	&alpha_D));
  assert(true  == cce_is_condition(&alpha_C, &alpha_D));
  assert(true  == cce_is_condition(&beta_C,  &alpha_D));
  assert(true  == cce_is_condition(&gamma_C, &alpha_D));

  assert(false == cce_is_condition(cce_condition(cce_condition_new_unknown()),     &beta_D));
  assert(false == cce_is_condition(cce_condition(cce_condition_new_errno(EINVAL)), &beta_D));
  assert(false == cce_is_condition(&alpha_C, &beta_D));
  assert(true  == cce_is_condition(&beta_C,  &beta_D));
  assert(true  == cce_is_condition(&gamma_C, &beta_D));

  assert(false == cce_is_condition(cce_condition(cce_condition_new_unknown()),     &gamma_D));
  assert(false == cce_is_condition(cce_condition(cce_condition_new_errno(EINVAL)), &gamma_D));
  assert(false == cce_is_condition(&alpha_C, &gamma_D));
  assert(false == cce_is_condition(&beta_C,  &gamma_D));
  assert(true  == cce_is_condition(&gamma_C, &gamma_D));

  /* Branching upon condition types with "cce_is_condition()". */
  {
    cce_location_t	L[1];
    bool		flag = false;

    if (cce_location(L)) {
      cce_condition_t *	C = cce_condition(L);

      if (cce_is_condition(C, cce_descriptor(cce_descriptor_unknown_ptr)))
	{
	  flag = false;
	}
      else if (cce_is_condition(C, cce_descriptor(cce_descriptor_errno_ptr)))
	{
	  flag = true;
	}
      else
	{
	  flag = false;
	}
      cce_run_error_handlers(L);
      cce_condition_delete(C);
    } else {
      cce_raise(L, cce_condition(cce_condition_new_errno(EINVAL)));
      cce_run_cleanup_handlers(L);
    }
    assert(true == flag);
  }

  /* Branching upon condition types with "cce_descriptor_child_and_ancestor()". */
  {
    cce_location_t	L[1];
    bool		flag = false;

    if (cce_location(L)) {
      cce_condition_t *	C  = cce_condition(L);
      const cce_descriptor_t *	CD = cce_descriptor(C);

      if (cce_descriptor_child_and_ancestor(CD, cce_descriptor(cce_descriptor_unknown_ptr)))
	{
	  flag = false;
	}
      else if (cce_descriptor_child_and_ancestor(CD, cce_descriptor(cce_descriptor_errno_ptr)))
	{
	  flag = true;
	}
      else
	{
	  flag = false;
	}
      cce_run_error_handlers(L);
      cce_condition_delete(C);
    } else {
      cce_raise(L, cce_condition(cce_condition_new_errno(EINVAL)));
      cce_run_cleanup_handlers(L);
    }
    assert(true == flag);
  }

  exit(EXIT_SUCCESS);
}

/* end of file */
