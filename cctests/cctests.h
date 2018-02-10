/*
  Part of: CCTests
  Contents: public header file
  Date: Nov 28, 2017

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

#ifndef CCTESTS_H
#define CCTESTS_H 1


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The macro CCTESTS_UNUSED indicates  that a function, function argument
   or variable may potentially be unused. Usage examples:

   static int unused_function (char arg) CCTESTS_UNUSED;
   int foo (char unused_argument CCTESTS_UNUSED);
   int unused_variable CCTESTS_UNUSED;
*/
#ifdef __GNUC__
#  define CCTESTS_UNUSED		__attribute__((__unused__))
#else
#  define CCTESTS_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define cctests_decl		__attribute__((__dllexport__)) extern
#    else
#      define cctests_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define cctests_decl		__attribute__((__dllimport__)) extern
#    else
#      define cctests_decl		__declspec(dllimport) extern
#    endif
#  endif
#  define cctests_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define cctests_decl		__attribute__((__visibility__("default"))) extern
#    define cctests_private_decl	__attribute__((__visibility__("hidden")))  extern
#  else
#    define cctests_decl		extern
#    define cctests_private_decl	extern
#  endif
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Constants and preprocessor macros.
 ** ----------------------------------------------------------------- */

#define CCTESTS_PC(POINTER_TYPE, POINTER_NAME, EXPRESSION)	\
  POINTER_TYPE * POINTER_NAME = (POINTER_TYPE *) (EXPRESSION)


/** --------------------------------------------------------------------
 ** Type definitions.
 ** ----------------------------------------------------------------- */

typedef void cctests_fun_t (cce_destination_t L);

/* ------------------------------------------------------------------ */

#define CCTESTS_STRUCT_TYPEDEF(NAME)	typedef struct NAME NAME

CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_base_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_success_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_skipped_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_failure_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_assertion_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_unreachable_t);

CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_signal_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_signal_1_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_signal_2_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_signal_3_t);

CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_regex_error_t);
CCTESTS_STRUCT_TYPEDEF(cctests_descriptor_regex_compilation_error_t);

/* ------------------------------------------------------------------ */

CCTESTS_STRUCT_TYPEDEF(cctests_condition_base_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_success_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_skipped_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_failure_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_assertion_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_unreachable_t);

CCTESTS_STRUCT_TYPEDEF(cctests_condition_signal_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_signal_1_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_signal_2_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_signal_3_t);

CCTESTS_STRUCT_TYPEDEF(cctests_condition_regex_error_t);
CCTESTS_STRUCT_TYPEDEF(cctests_condition_regex_compilation_error_t);


/** --------------------------------------------------------------------
 ** Library initialisation.
 ** ----------------------------------------------------------------- */

cctests_decl void cctests_init (char const * test_file_name)
  __attribute__((__nonnull__(1)));

cctests_decl void cctests_final (void);
cctests_decl void ctests_reset_global_state (void);


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

cctests_decl char const * cctests_version_string (void);
cctests_decl int	cctests_version_interface_current (void);
cctests_decl int	cctests_version_interface_revision (void);
cctests_decl int	cctests_version_interface_age (void);


/** --------------------------------------------------------------------
 ** Condition objects: base.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_base_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_base_t {
  cce_condition_root_t	root;
};

cctests_decl cctests_descriptor_base_t const * const cctests_descriptor_base_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_base (cctests_condition_base_t * C)
{
  cce_condition_init_root(&(C->root));
}

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_base (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_base_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: test success.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_success_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_success_t {
  cctests_condition_base_t	base;
};

cctests_decl cctests_descriptor_success_t const * const cctests_descriptor_success_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_success (cctests_condition_success_t * C)
{
  cctests_condition_init_base(&(C->base));
}

cctests_decl cce_condition_t const * cctests_condition_new_success (void)
  __attribute__((__const__,__pure__,__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_success (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_success_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: test skipped.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_skipped_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_skipped_t {
  cctests_condition_base_t	base;
};

cctests_decl cctests_descriptor_skipped_t const * const cctests_descriptor_skipped_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_skipped (cctests_condition_skipped_t * C)
{
  cctests_condition_init_base(&(C->base));
}

cctests_decl cce_condition_t const * cctests_condition_new_skipped (void)
  __attribute__((__const__,__pure__,__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_skipped (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_skipped_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: test failure.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_failure_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_failure_t {
  cctests_condition_base_t	base;
};

cctests_decl cctests_descriptor_failure_t const * const cctests_descriptor_failure_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_failure (cctests_condition_failure_t * C)
{
  cctests_condition_init_base(&(C->base));
}

cctests_decl cce_condition_t const * cctests_condition_new_failure (void)
  __attribute__((__const__,__pure__,__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_failure (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_failure_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: assertion failure.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_assertion_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_assertion_t {
  cctests_condition_failure_t	failure;
  char const *			expr;
  char const *			filename;
  char const *			funcname;
  int				linenum;
};

cctests_decl cctests_descriptor_assertion_t const * const cctests_descriptor_assertion_ptr;

cctests_decl void cctests_condition_init_assertion (cctests_condition_assertion_t * C,
						    char const * const expr,
						    char const * const filename,
						    char const * const funcname,
						    int const linenum)
  __attribute__((__nonnull__(1,2,3,4)));

cctests_decl cce_condition_t const * cctests_condition_new_assertion (cce_destination_t L,
								      char const * const expr,
								      char const * const filename,
								      char const * const funcname,
								      int const linenum)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_assertion (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_assertion_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: unreachable code.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_unreachable_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_unreachable_t {
  cctests_condition_failure_t	failure;
  char const *			filename;
  char const *			funcname;
  int				linenum;
};

cctests_decl cctests_descriptor_unreachable_t const * const cctests_descriptor_unreachable_ptr;

cctests_decl void cctests_condition_init_unreachable (cctests_condition_unreachable_t * C,
						    char const * const filename,
						    char const * const funcname,
						    int const linenum)
  __attribute__((__nonnull__(1,2,3)));

cctests_decl cce_condition_t const * cctests_condition_new_unreachable (cce_destination_t L,
									char const * const filename,
									char const * const funcname,
									int const linenum)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

#define cctests_raise_unreachable(L)		\
  cce_raise((L), cctests_condition_new_unreachable((L), __FILE__, __func__, __LINE__))

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_unreachable (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_unreachable_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: signal.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_signal_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_signal_t {
  cctests_condition_base_t	base;
};

cctests_decl cctests_descriptor_signal_t const * const cctests_descriptor_signal_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_signal (cctests_condition_signal_t * C)
{
  cctests_condition_init_base(&(C->base));
}

cctests_decl cce_condition_t const * cctests_condition_new_signal (void)
  __attribute__((__leaf__,__const__,__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_signal (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_signal_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: signal one.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_signal_1_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_signal_1_t {
  cctests_condition_signal_t	signal;
};

cctests_decl cctests_descriptor_signal_1_t const * const cctests_descriptor_signal_1_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_signal_1 (cctests_condition_signal_1_t * C)
{
  cctests_condition_init_signal(&(C->signal));
}

cctests_decl cce_condition_t const * cctests_condition_new_signal_1 (void)
  __attribute__((__leaf__,__const__,__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_signal_1 (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_signal_1_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: signal two.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_signal_2_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_signal_2_t {
  cctests_condition_signal_t	signal;
};

cctests_decl cctests_descriptor_signal_2_t const * const cctests_descriptor_signal_2_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_signal_2 (cctests_condition_signal_2_t * C)
{
  cctests_condition_init_signal(&(C->signal));
}

cctests_decl cce_condition_t const * cctests_condition_new_signal_2 (void)
  __attribute__((__leaf__,__const__,__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_signal_2 (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_signal_2_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: signal three.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_signal_3_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_signal_3_t {
  cctests_condition_signal_t	signal;
};

cctests_decl cctests_descriptor_signal_3_t const * const cctests_descriptor_signal_3_ptr;

__attribute__((__nonnull__((1)),,__always_inline__))
static inline void
cctests_condition_init_signal_3 (cctests_condition_signal_3_t * C)
{
  cctests_condition_init_signal(&(C->signal));
}

cctests_decl cce_condition_t const * cctests_condition_new_signal_3 (void)
  __attribute__((__leaf__,__const__,__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_signal_3 (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_signal_3_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: regular expression error.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_regex_error_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_regex_error_t {
  cce_condition_runtime_error_t	runtime_error;
  int	errcode;
  char	error_message[1024];
};

cctests_decl cctests_descriptor_regex_error_t const * const cctests_descriptor_regex_error_ptr;

cctests_decl void cctests_condition_init_regex_error (cctests_condition_regex_error_t * C, int errcode)
  __attribute__((__nonnull__(1)));

cctests_decl cce_condition_t const * cctests_condition_new_regex_error (cce_destination_t L, int errcode)
  __attribute__((__nonnull__(1),__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_regex_error (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_regex_error_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: regular expression compilation error.
 ** ----------------------------------------------------------------- */

struct cctests_descriptor_regex_compilation_error_t {
  cce_descriptor_t	descriptor;
};

struct cctests_condition_regex_compilation_error_t {
  cctests_condition_regex_error_t	regex_error;
};

cctests_decl cctests_descriptor_regex_compilation_error_t const * const cctests_descriptor_regex_compilation_error_ptr;

cctests_decl void cctests_condition_init_regex_compilation_error (cctests_condition_regex_compilation_error_t * C, int errcode)
  __attribute__((__nonnull__(1)));

cctests_decl cce_condition_t const * cctests_condition_new_regex_compilation_error (cce_destination_t L, int errcode)
  __attribute__((__nonnull__(1),__returns_nonnull__));

__attribute__((__pure__,__nonnull__(1),__always_inline__))
static inline bool
cctests_condition_is_regex_compilation_error (cce_condition_t const * C)
{
  return cce_is_condition(C, &(cctests_descriptor_regex_compilation_error_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** System interface.
 ** ----------------------------------------------------------------- */

cce_decl void * cctests_sys_malloc (cce_destination_t L, size_t size)
  __attribute__((__nonnull__(1),__returns_nonnull__));

cce_decl void * cctests_sys_realloc (cce_destination_t L, void * ptr, size_t newsize)
  __attribute__((__nonnull__(1),__returns_nonnull__));

cce_decl void * cctests_sys_calloc (cce_destination_t L, size_t count, size_t eltsize)
  __attribute__((__nonnull__(1),__returns_nonnull__));


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: malloc pointer.
 ** ----------------------------------------------------------------- */

cce_decl void cctests_cleanup_handler_malloc_init (cce_destination_t L, cce_handler_t * H, void * pointer)
  __attribute__((__nonnull__(1,2,3)));

cce_decl void cctests_error_handler_malloc_init (cce_destination_t L, cce_handler_t * H, void * pointer)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Test driver.
 ** ----------------------------------------------------------------- */

cctests_decl void cctests_begin_group (char const * const test_group_name)
  __attribute__((__nonnull__(1)));
cctests_decl void cctests_end_group   (void);

#define cctests_run(TEST_FUNC_NAME)	cctests_p_run(#TEST_FUNC_NAME, TEST_FUNC_NAME)
cctests_decl void cctests_p_run (char const * test_func_name, cctests_fun_t * fun)
  __attribute__((__nonnull__(1,2)));

cctests_decl bool cctests_latest_group_completed_successfully (void);
cctests_decl bool cctests_latest_func_completed_successfully (void);


/** --------------------------------------------------------------------
 ** Assertions API.
 ** ----------------------------------------------------------------- */

cctests_decl cce_destination_t	cctests_location;

__attribute__((__always_inline__))
static inline void
cctests_skip (void)
{
  cce_raise(cctests_location, cctests_condition_new_skipped());
}

#define CCTESTS_GET_ASSERT_MACRO(_1,_2,NAME,...)	NAME
#define cctests_assert(...)	CCTESTS_GET_ASSERT_MACRO(__VA_ARGS__,cctests_m_assert_ex,cctests_m_assert)(__VA_ARGS__)

#define cctests_m_assert(EXPR)		cctests_p_assert(cctests_location, #EXPR, (EXPR), __FILE__, __func__, __LINE__)
#define cctests_m_assert_ex(L,EXPR)	cctests_p_assert((L),              #EXPR, (EXPR), __FILE__, __func__, __LINE__)

cctests_decl void cctests_p_assert (cce_destination_t L, char const * const expr, bool result,
				    char const * const filename, char const * const funcname, int const linenum);


/** --------------------------------------------------------------------
 ** Calling a function in a subprocess.
 ** ----------------------------------------------------------------- */

typedef void cctests_parent_process_function_t (cce_destination_t L, int64_t child_pid);
typedef void cctests_child_process_function_t (cce_destination_t L);

cctests_decl void cctests_call_in_forked_process (cce_destination_t L, cctests_child_process_function_t * child_function)
  __attribute__((__nonnull__(1,2)));

cctests_decl void cctests_with_parent_and_child_process (cce_destination_t L,
							 cctests_parent_process_function_t * parent_function,
							 cctests_child_process_function_t * child_function)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Helpers.
 ** ----------------------------------------------------------------- */

cctests_decl FILE * cctests_log_stream;

cctests_decl bool cctests_isatty (FILE * stream)
  __attribute__((__nonnull__(1)));

cctests_decl bool cctests_log_stream_isatty (void);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCTESTS_H */

/* end of file */
