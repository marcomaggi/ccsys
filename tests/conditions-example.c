/*
  Part of: CCExceptions
  Contents: example of condition descriptors hierarchy
  Date: Dec 28, 2016

  Abstract

	This example is copied in the documentation.

  Copyright (C) 2016, 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can  redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the
  Free Software  Foundation; either version  3.0 of the License,  or (at
  your option) any later version.

  This library  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  Lesser General Public License for more details.

  You  should have  received a  copy of  the GNU  Lesser General  Public
  License along  with this library; if  not, write to the  Free Software
  Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
  USA.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <stdlib.h>
#include <assert.h>


/** --------------------------------------------------------------------
 ** Exceptional condition "alpha".
 ** ----------------------------------------------------------------- */

typedef struct descriptor_alpha_t {
  cce_descriptor_t	descriptor;
} descriptor_alpha_t;

typedef struct condition_alpha_t {
  cce_condition_root_t	root;
  int			alpha;
} condition_alpha_t;

cce_condition_t * condition_new_alpha (cce_location_t * L, int alpha);
void condition_init_alpha (condition_alpha_t * C, int alpha);
bool condition_is_alpha (const cce_condition_t * condition);

static void condition_delete_alpha (cce_condition_t * C);
static void condition_final_alpha (cce_condition_t * C);
static const char * condition_static_message_alpha (const cce_condition_t * C);

/* Instance of condition descriptor.   The "parent" field is initialised
   to  NULL   here  and  reinitialised   to  the  "root"  later   by  an
   initialisation function. */
static descriptor_alpha_t descriptor_alpha = {
  .descriptor.parent		= NULL,
  .descriptor.delete		= condition_delete_alpha,
  .descriptor.final		= condition_final_alpha,
  .descriptor.static_message	= condition_static_message_alpha
};

cce_condition_t *
condition_new_alpha (cce_location_t * L, int alpha)
/* Allocate a condition object and initialise it. */
{
  condition_alpha_t *	C = cce_sys_malloc(L, sizeof(condition_alpha_t));
  cce_condition_init((cce_condition_t *)C, &descriptor_alpha.descriptor);
  condition_init_alpha(C, alpha);
  return (cce_condition_t *) C;
}

void
condition_delete_alpha (cce_condition_t * C)
/* Release the condition object memory. */
{
  fprintf(stderr, "%s\n\n", __func__);
  free(C);
}

void
condition_init_alpha (condition_alpha_t * C, int alpha)
/* Initialise an already allocated condition object. */
{
  C->alpha = alpha;
}

void
condition_final_alpha (cce_condition_t * C CCE_UNUSED)
/* Finalise a condition object; do not release memory. */
{
  fprintf(stderr, "%s\n", __func__);
}

static const char *
condition_static_message_alpha (const cce_condition_t * C CCE_UNUSED)
{
  return "exceptional condition A";
}

bool
condition_is_alpha (const cce_condition_t * condition)
{
  return cce_is_condition(condition, &descriptor_alpha.descriptor);
}

#define condition_alpha(S)						\
  _Generic((S),								\
	   cce_condition_t		*: (condition_alpha_t *)(S),	\
	   condition_alpha_t		*: (condition_alpha_t *)(S),	\
	   condition_beta_t		*: (condition_alpha_t *)(S),	\
	   condition_gamma_t		*: (condition_alpha_t *)(S),	\
	   const cce_condition_t	*: (condition_alpha_t *)(S),	\
	   const condition_alpha_t	*: (condition_alpha_t *)(S),	\
	   const condition_beta_t	*: (condition_alpha_t *)(S),	\
	   const condition_gamma_t	*: (condition_alpha_t *)(S))


/** --------------------------------------------------------------------
 ** Exceptional condition "beta".
 ** ----------------------------------------------------------------- */

typedef struct descriptor_beta_t {
  cce_descriptor_t	descriptor;
} descriptor_beta_t;

typedef struct condition_beta_t {
  condition_alpha_t	alpha;
  int			beta;
} condition_beta_t;

cce_condition_t * condition_new_beta (cce_location_t * L, int alpha, int beta);
void condition_init_beta (condition_beta_t * C, int alpha, int beta);
bool condition_is_beta (const cce_condition_t * condition);

static void condition_delete_beta  (cce_condition_t * C);
static void condition_final_beta (cce_condition_t * C);
static const char * condition_static_message_beta (const cce_condition_t * C);

/* Instance of condition descriptor.   The "parent" field is initialised
   to  NULL   here  and  reinitialised   to  the  "root"  later   by  an
   initialisation function. */
static descriptor_beta_t descriptor_beta = {
  .descriptor.parent		= &descriptor_alpha.descriptor,
  .descriptor.delete		= condition_delete_beta,
  .descriptor.final		= condition_final_beta,
  .descriptor.static_message	= condition_static_message_beta
};

cce_condition_t *
condition_new_beta (cce_location_t * L, int alpha, int beta)
/* Allocate a condition object and initialise it. */
{
  condition_beta_t *	C = cce_sys_malloc(L, sizeof(condition_beta_t));
  cce_condition_init((cce_condition_t *)C, &descriptor_beta.descriptor);
  condition_init_beta(C, alpha, beta);
  return (cce_condition_t *) C;
}

void
condition_delete_beta (cce_condition_t * C)
/* Release the condition object memory. */
{
  fprintf(stderr, "%s\n\n", __func__);
  free(C);
}

void
condition_init_beta (condition_beta_t * C, int alpha, int beta)
/* Initialise an already allocated condition object. */
{
  condition_init_alpha(&(C->alpha),  alpha);
  C->beta = beta;
}

void
condition_final_beta (cce_condition_t * C CCE_UNUSED)
/* Finalise a condition object; do not release memory. */
{
  fprintf(stderr, "%s\n", __func__);
}

static const char *
condition_static_message_beta (const cce_condition_t * C CCE_UNUSED)
{
  return "exceptional condition A";
}

bool
condition_is_beta (const cce_condition_t * condition)
{
  return cce_is_condition(condition, &descriptor_beta.descriptor);
}

#define condition_beta(S)						\
  _Generic((S),								\
	   cce_condition_t		*: (condition_beta_t *)(S),	\
	   condition_alpha_t		*: (condition_beta_t *)(S),	\
	   condition_beta_t		*: (condition_beta_t *)(S),	\
	   condition_gamma_t		*: (condition_beta_t *)(S),	\
	   const cce_condition_t	*: (condition_beta_t *)(S),	\
	   const condition_alpha_t	*: (condition_beta_t *)(S),	\
	   const condition_beta_t	*: (condition_beta_t *)(S),	\
	   const condition_gamma_t	*: (condition_beta_t *)(S))


/** --------------------------------------------------------------------
 ** Exceptional condition "gamma".
 ** ----------------------------------------------------------------- */

typedef struct descriptor_gamma_t {
  cce_descriptor_t	descriptor;
} descriptor_gamma_t;

typedef struct condition_gamma_t {
  condition_beta_t	beta;
  int			gamma;
} condition_gamma_t;

cce_condition_t * condition_new_gamma (cce_location_t * L, int alpha, int beta, int gamma);
void condition_init_gamma (condition_gamma_t * C, int alpha, int beta, int gamma);
bool condition_is_gamma (const cce_condition_t * condition);

static void condition_delete_gamma  (cce_condition_t * C);
static void condition_final_gamma (cce_condition_t * C);
static const char * condition_static_message_gamma (const cce_condition_t * C);

/* Instance of condition descriptor.   The "parent" field is initialised
   to  NULL   here  and  reinitialised   to  the  "root"  later   by  an
   initialisation function. */
static descriptor_gamma_t descriptor_gamma = {
  .descriptor.parent		= &descriptor_beta.descriptor,
  .descriptor.delete		= condition_delete_gamma,
  .descriptor.final		= condition_final_gamma,
  .descriptor.static_message	= condition_static_message_gamma
};

cce_condition_t *
condition_new_gamma (cce_location_t * L, int alpha, int beta, int gamma)
/* Allocate a condition object and initialise it. */
{
  condition_gamma_t *	C = cce_sys_malloc(L, sizeof(condition_gamma_t));
  cce_condition_init((cce_condition_t *)C, &descriptor_gamma.descriptor);
  condition_init_gamma(C, alpha, beta, gamma);
  return (cce_condition_t *) C;
}

void
condition_delete_gamma (cce_condition_t * C)
/* Release the condition object memory. */
{
  fprintf(stderr, "%s\n\n", __func__);
  free(C);
}

void
condition_init_gamma (condition_gamma_t * C, int alpha, int beta, int gamma)
/* Initialise an already allocated condition object. */
{
  condition_init_beta(&(C->beta), alpha, beta);
  C->gamma = gamma;
}

void
condition_final_gamma (cce_condition_t * C CCE_UNUSED)
/* Finalise a condition object; do not release memory. */
{
  fprintf(stderr, "%s\n", __func__);
}

static const char *
condition_static_message_gamma (const cce_condition_t * C CCE_UNUSED)
{
  return "exceptional condition A";
}

bool
condition_is_gamma (const cce_condition_t * condition)
{
  return cce_is_condition(condition, &descriptor_gamma.descriptor);
}

#define condition_gamma(S)						\
  _Generic((S),								\
	   cce_condition_t		*: (condition_gamma_t *)(S),	\
	   condition_alpha_t		*: (condition_gamma_t *)(S),	\
	   condition_beta_t		*: (condition_gamma_t *)(S),	\
	   condition_gamma_t		*: (condition_gamma_t *)(S),	\
	   const cce_condition_t	*: (condition_gamma_t *)(S),	\
	   const condition_alpha_t	*: (condition_gamma_t *)(S),	\
	   const condition_beta_t	*: (condition_gamma_t *)(S),	\
	   const condition_gamma_t	*: (condition_gamma_t *)(S))


static void
test_it (const cce_condition_t * C, const int expected_flag)
{
  cce_location_t	L[1];
  int			flag;

  if (cce_location(L)) {
    if (condition_is_gamma(cce_condition(L))) {
      condition_gamma_t * K = condition_gamma(cce_condition(L));
      flag = K->gamma;
    } else if (condition_is_beta(cce_condition(L))) {
      condition_beta_t  * K = condition_beta(cce_condition(L));
      flag = K->beta;
    } else if (condition_is_alpha(cce_condition(L))) {
      condition_alpha_t * K = condition_alpha(cce_condition(L));
      flag = K->alpha;
    } else {
      flag = 0;
    }
    cce_run_error_handlers_final(L);
  } else {
    cce_raise(L, C);
    cce_run_cleanup_handlers(L);
  }
  assert(expected_flag == flag);
}


int
main (void)
{
  cce_location_t	L[1];

  /* Dynamic initialisation. */
  cce_descriptor_set_root_parent(&descriptor_alpha.descriptor);

  if (cce_location(L)) {
    cce_run_error_handlers_final(L);
  } else {
    test_it(condition_new_alpha(L, 1), 1);
    test_it(condition_new_beta(L, 1, 2), 2);
    test_it(condition_new_gamma(L, 1, 2, 3), 3);
    cce_run_cleanup_handlers(L);
  }

  exit(EXIT_SUCCESS);
}

/* end of file */
