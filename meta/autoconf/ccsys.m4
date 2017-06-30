dnl
dnl Part of: CCSys
dnl Contents: macros for library configuration and system inspection
dnl Date: Jun 30, 2017
dnl
dnl Abstract
dnl
dnl	Macros used in "configure.ac".
dnl
dnl Copyright (C) 2017 Marco Maggi <marco.maggi-ipsu@poste.it>
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it  under the  terms of  the GNU  Lesser General  Public License  as
dnl published by the  Free Software Foundation, either version  3 of the
dnl License, or (at your option) any later version.
dnl
dnl This program is distributed in the  hope that it will be useful, but
dnl WITHOUT  ANY   WARRANTY;  without  even  the   implied  warranty  of
dnl MERCHANTABILITY or  FITNESS FOR A  PARTICULAR PURPOSE.  See  the GNU
dnl General Public License for more details.
dnl
dnl You should  have received a copy  of the GNU General  Public License
dnl along      with      this       program.       If      not,      see
dnl <http://www.gnu.org/licenses/>.
dnl

dnl page
dnl CCSYS_SIZEOF --
dnl
dnl Wrapper for  AC_COMPUTE_INT which computes and caches the  size of a
dnl data type.  For example, to compute the size of "DIR" defined in the
dnl header "dirent.h" we do:
dnl
dnl    CCSYS_SIZEOF([DIRENT],[struct dirent],[#include <dirent.h>])
dnl
dnl this macro expansion: defines the shell variable "SIZEOF_DIRENT";
dnl defines the shell variable "ccsys_cv_sizeof_DIRENT" to cache the value;
dnl defines an Autoconf substitution for the symbol "SIZEOF_DIRENT".
dnl
dnl $1 - the stem  to use  to define  shell variables  representing the
dnl      result of this test
dnl $2 - the reference of a C language type
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_SIZEOF],
  [AS_VAR_SET([SIZEOF_$1],[0])
   AC_CACHE_CHECK([the size of '$2'],
     [ccsys_cv_sizeof_$1],
     [AC_COMPUTE_INT([ccsys_cv_sizeof_$1],
        [sizeof($2)],
        [$3],
        [AS_VAR_SET([ccsys_cv_sizeof_$1],[0])])])
    AS_VAR_SET([SIZEOF_$1],["$ccsys_cv_sizeof_$1"])
    AC_SUBST([SIZEOF_$1])])

dnl page
dnl CCSYS_VALUEOF --
dnl
dnl Wrapper for AC_COMPUTE_INT which computes  and caches the value of a
dnl constant.  For example, to compute the value of "ERRNO" defined in the
dnl header "errno.h" we do:
dnl
dnl    CCSYS_VALUEOF([EPERM],[EPERM],[#include <errno.h>])
dnl
dnl this macro expansion: defines the shell variable "VALUEOF_ERRNO";
dnl  defines the  shell variable  "ccsys_cv_valueof_ERRNO" to  cache the
dnl value; defines an Autoconf substitution for the symbol "VALUEOF_ERRNO".
dnl
dnl $1 - the stem  to use  to define  shell variables  representing the
dnl      result of this test
dnl $2 - the expression representing the value to compute
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_VALUEOF],
  [AS_VAR_SET([VALUEOF_$1],[0])
   AC_CACHE_CHECK([the size of '$2'],
     [ccsys_cv_valueof_$1],
     [AC_COMPUTE_INT([ccsys_cv_valueof_$1],
        [valueof($2)],
        [$3],
        [AS_VAR_SET([ccsys_cv_valueof_$1],[0])])])
    AS_VAR_SET([VALUEOF_$1],["$ccsys_cv_valueof_$1"])
    AC_SUBST([VALUEOF_$1])])

dnl page
dnl CCSYS_OFFSETOF --
dnl
dnl Wrapper for AC_COMPUTE_INT which computes  and caches the offset of a
dnl field in a struct definition.   For example, to compute the offset of
dnl "d_name" in "struct dirent" defined in the header "dirent.h" we do:
dnl
dnl    CCSYS_OFFSETOF([DIRENT_D_NAME],
dnl      [struct dirent],[d_name],[#include <dirent.h>])
dnl
dnl this macro expansion: defines the shell variable "OFFSETOF_DIRENT_D_NAME";
dnl defines the shell variable "ccsys_cv_offsetof_DIRENT_D_NAME" to cache the
dnl value; defines an Autoconf substitution for the symbol
dnl "OFFSETOF_DIRENT_D_NAME".
dnl
dnl $1 - the stem  to use  to define  shell variables  representing the
dnl      result of this test
dnl $2 - the expression representing the value to compute
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_OFFSETOF],
  [AS_VAR_SET([OFFSETOF_$1],[0])
   AC_CACHE_CHECK([the offset of '$3' in '$2'],
     [ccsys_cv_offsetof_$1],
     [AC_COMPUTE_INT([ccsys_cv_offsetof_$1],
        [offsetof($2,$3)],
        [#include <stddef.h> /* for offsetof */
$4],
        [AC_MSG_ERROR(cannot determine offset of '$3' in '$2')])])
    AS_VAR_SET([OFFSETOF_$1],["$ccsys_cv_offsetof_$1"])
    AC_SUBST([OFFSETOF_$1])])

dnl end of file
dnl Local Variables:
dnl mode: autoconf
dnl End:
