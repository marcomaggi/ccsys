/*
  Part of: CCSys
  Contents: exception handlers for POSIX stuff
  Date: Sat Mar 11, 2017

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


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccsys-internals.h"
#include "ccsys-system.h"
#include "ccsys-networking.h"
#include <string.h>


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: malloc pointer.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_malloc_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  free(H->pointer);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsys_cleanup_handler_malloc_init (cce_location_t * L, cce_handler_t * H, void * pointer)
{
  H->function	= cce_handler_malloc_function;
  H->pointer	= pointer;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_malloc_init (cce_location_t * L, cce_handler_t * H, void * pointer)
{
  H->function	= cce_handler_malloc_function;
  H->pointer	= pointer;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: file descriptor.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_filedes_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_CLOSE
  close(H->filedes);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#else
#  warning no close system function
#endif
}

void
ccsys_cleanup_handler_filedes_init (cce_location_t * L, cce_handler_t * H, int filedes)
{
  H->function	= cce_handler_filedes_function;
  H->filedes	= filedes;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_filedes_init (cce_location_t * L, cce_handler_t * H, int filedes)
{
  H->function	= cce_handler_filedes_function;
  H->filedes	= filedes;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: pipe descriptors.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_pipedes_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_CLOSE
  close(H->pipedes[0]);
  close(H->pipedes[1]);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, int pipedes[2])
{
  H->function	= cce_handler_pipedes_function;
  H->pipedes[0]	= pipedes[0];
  H->pipedes[1]	= pipedes[1];
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_pipedes_init (cce_location_t * L, cce_handler_t * H, int pipedes[2])
{
  H->function	= cce_handler_pipedes_function;
  H->pipedes[0]	= pipedes[0];
  H->pipedes[1]	= pipedes[1];
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: temporary files.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_tmpfile_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_REMOVE
  remove(H->pathname);
  free(H->pathname);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpfile_function;
  H->pathname	= ptr;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_tmpfile_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpfile_function;
  H->pathname	= ptr;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: temporary directories.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_tmpdir_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_RMDIR
  rmdir(H->pathname);
  free(H->pathname);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpdir_function;
  H->pathname	= ptr;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_tmpdir_init (cce_location_t * L, cce_handler_t * H, const char * pathname)
{
  size_t	len = 1+strlen(pathname);
  char *	ptr = ccsys_malloc(L, len);
  strncpy(ptr, pathname, len);
  ptr[len] = '\n';
  H->function	= cce_handler_tmpdir_function;
  H->pathname	= ptr;
  cce_register_error_handler(L, H);
}


/** --------------------------------------------------------------------
 ** Predefined POSIX exception handler: directory streams.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
cce_handler_dirstream_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
#ifdef HAVE_CLOSEDIR
  DIR *		dirstream = H->pointer;
  closedir(dirstream);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
#endif
}

void
ccsys_cleanup_handler_dirstream_init (cce_location_t * L, cce_handler_t * H, DIR * dirstream)
{
  H->function	= cce_handler_dirstream_function;
  H->pointer	= dirstream;
  cce_register_cleanup_handler(L, H);
}

void
ccsys_error_handler_dirstream_init (cce_location_t * L, cce_handler_t * H, DIR * dirstream)
{
  H->function	= cce_handler_dirstream_function;
  H->pointer	= dirstream;
  cce_register_error_handler(L, H);
}

/* end of file */
