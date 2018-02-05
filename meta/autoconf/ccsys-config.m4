dnl
dnl Part of: CCSys
dnl Contents: macros for library configuration and system inspection
dnl Date: Jun 30, 2017
dnl
dnl Abstract
dnl
dnl	Macros used in "configure.ac".
dnl
dnl Copyright (C) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>
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
dnl headers macros

AC_DEFUN([CCSYS_ARPA_INET_HEADER],[
#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif
])

AC_DEFUN([CCSYS_DIRENT_HEADER],[
#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#endif
])

AC_DEFUN([CCSYS_ERRNO_HEADER],[
#ifdef HAVE_ERRNO_H
#  include <errno.h>
#endif
])

AC_DEFUN([CCSYS_FCNTL_HEADER],[
#ifdef HAVE_FCNTL_H
#  include <fcntl.h>
#endif
])

AC_DEFUN([CCSYS_GRP_HEADER],[
#ifdef HAVE_GRP_H
#  include <grp.h>
#endif
])

AC_DEFUN([CCSYS_LIMITS_HEADER],[
#ifdef HAVE_LIMITS_H
#  include <limits.h>
#endif
])

AC_DEFUN([CCSYS_LINUX_FS_HEADER],[
#ifdef HAVE_LINUX_FS_H
#  include <linux/fs.h>
#endif
])

AC_DEFUN([CCSYS_NETINET_IN_HEADER],[
#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif
])

AC_DEFUN([CCSYS_NETDB_HEADER],[
#ifdef HAVE_NETDB_H
#  include <netdb.h>
#endif
])

AC_DEFUN([CCSYS_PWD_HEADER],[
#ifdef HAVE_PWD_H
#  include <pwd.h>
#endif
])

AC_DEFUN([CCSYS_SIGNAL_HEADER],[
#ifdef HAVE_SIGNAL_H
#  include <signal.h>
#endif
])

AC_DEFUN([CCSYS_SYS_SELECT_HEADER],[
#ifdef HAVE_SYS_SELECT_H
#  include <sys/select.h>
#endif
])

AC_DEFUN([CCSYS_SYS_SOCKET_HEADER],[
#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif
])

AC_DEFUN([CCSYS_SYS_SYSCALL_HEADER],[
#ifdef HAVE_SYS_SYSCALL_H
#  include <sys/syscall.h>
#endif
])

AC_DEFUN([CCSYS_SYS_UN_HEADER],[
#ifdef HAVE_SYS_UN_H
#  include <sys/un.h>
#endif
])

AC_DEFUN([CCSYS_STDDEF_HEADER],[
#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif
])

AC_DEFUN([CCSYS_STDIO_HEADER],[
#include <stdio.h>
])

AC_DEFUN([CCSYS_STDLIB_HEADER],[
#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif
])

AC_DEFUN([CCSYS_SYS_TYPES_HEADER],[
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
])

AC_DEFUN([CCSYS_SYS_MMAN_HEADER],[
#ifdef HAVE_SYS_MMAN_H
#  include <sys/mman.h>
#endif
])

AC_DEFUN([CCSYS_SYS_UIO_HEADER],[
#ifdef HAVE_SYS_UIO_H
#  include <sys/uio.h>
#endif
])

AC_DEFUN([CCSYS_SYS_STAT_HEADER],[
#ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif
])

AC_DEFUN([CCSYS_SYS_TIME_HEADER],[
#ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
#endif
])

AC_DEFUN([CCSYS_TIME_HEADER],[
#ifdef HAVE_TIME_H
#  include <time.h>
#endif
])

AC_DEFUN([CCSYS_UNISTD_HEADER],[
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
])

AC_DEFUN([CCSYS_WAIT_HEADER],[
#ifdef HAVE_WAIT_H
#  include <wait.h>
#endif
])

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
dnl If if is impossible to  determine the size: the size is artificially
dnl set to 1, so that it is possible to compile the expansion of:
dnl
dnl    struct ccsys_dirent_t {
dnl      uint8_t data[@SIZEOF_STRUCT_DIRENT@];
dnl    };
dnl
dnl without errors (which would result from the definition of an array
dnl of size zero).
dnl
dnl $1 - the stem  to use  to define  shell variables  representing the
dnl      result of this test
dnl $2 - the reference of a C language type
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_SIZEOF],
  [AS_VAR_SET([SIZEOF_$1],[0])
   AS_VAR_SET([ccsys__value_found],[maybe])
   AC_CACHE_CHECK([the size of '$2'],
     [ccsys_cv_sizeof_$1],
     [AC_COMPUTE_INT([ccsys_cv_sizeof_$1],
        [sizeof($2)],
        [$3],
        [AS_VAR_SET([ccsys__value_found],[no])
         AS_VAR_SET(ccsys_cv_sizeof_$1,[1])])])
   AS_IF([test "x$ccsys__value_found" = xno],[AC_MSG_WARN(cannot determine size of '$2')])
   AS_VAR_SET([SIZEOF_$1],["$ccsys_cv_sizeof_$1"])
   AC_SUBST([SIZEOF_$1])])

dnl page
dnl CCSYS_VALUEOF_NO_SUBST --
dnl
dnl Wrapper for AC_COMPUTE_INT which computes  and caches the value of a
dnl constant.  For example, to  compute the  value of "EPERM" defined in
dnl the header "errno.h" we do:
dnl
dnl    CCSYS_VALUEOF_NO_SUBST([EPERM],[EPERM],[#include <errno.h>])
dnl
dnl this macro expansion defines the shell variable "ccsys_cv_valueof_EPERM"
dnl to  cache the value;  if the value  cannot be determined:  the shell
dnl variable is set to "-1".
dnl
dnl $1 - the stem  to use  to define  shell variables  representing the
dnl      result of this test
dnl $2 - the expression representing the value to compute
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_VALUEOF_NO_SUBST],
  [AS_VAR_SET([ccsys__value_found],[maybe])
   AC_CACHE_CHECK([the value of '$2'],
     [ccsys_cv_valueof_$1],
     [AC_COMPUTE_INT([ccsys_cv_valueof_$1],
        [$2],
        [$3],
        [AS_VAR_SET([ccsys__value_found],[no])
         AS_VAR_SET(ccsys_cv_valueof_$1,[-1])])])
   AS_IF([test "x$ccsys__value_found" = xno],[AC_MSG_WARN(cannot determine value of '$2')])])

dnl page
dnl CCSYS_VALUEOF --
dnl
dnl Wrapper for AC_COMPUTE_INT which computes  and caches the value of a
dnl constant.  For example, to compute the value of "EPERM" defined in the
dnl header "errno.h" we do:
dnl
dnl    CCSYS_VALUEOF([EPERM],[EPERM],[#include <errno.h>])
dnl
dnl this macro expansion: defines the shell variable "VALUEOF_EPERM";
dnl  defines the  shell variable  "ccsys_cv_valueof_EPERM" to  cache the
dnl value; defines an Autoconf substitution for the symbol "VALUEOF_EPERM".
dnl
dnl $1 - the stem  to use  to define  shell variables  representing the
dnl      result of this test
dnl $2 - the expression representing the value to compute
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_VALUEOF],
  [CCSYS_VALUEOF_NO_SUBST([$1],[$2],[$3])
   AS_VAR_SET([VALUEOF_$1])
   AS_VAR_IF(ccsys_cv_valueof_$1,[-1],
     [AS_VAR_SET([VALUEOF_$1],[-1])],
     [AS_VAR_SET([VALUEOF_$1],["$ccsys_cv_valueof_$1"])])
   AC_SUBST([VALUEOF_$1])])

dnl page
dnl CCSYS_MAYBE_ENUM_DEF --
dnl
dnl Define an enumerated constant.  Example:
dnl
dnl   CCSYS_MAYBE_ENUM_DEF([CCSYS_SOCK_STREAM],[SOCK_STREAM],[#include <sys/socket.h>])
dnl
dnl is meant to be used in the template as:
dnl
dnl   enum ccsys_socket_namespace_t {
dnl      @CCSYS_ENUM_DEF_CCSYS_SOCK_STREAM@
dnl   }
dnl
dnl if the constant is defined: the expansion must be:
dnl
dnl   enum ccsys_socket_namespace_t {
dnl      CCSYS_SOCK_STREAM=123,
dnl   }
dnl
dnl if the constant is not defined: the expansion must be:
dnl
dnl   enum ccsys_socket_namespace_t {
dnl   }
dnl
AC_DEFUN([CCSYS_MAYBE_ENUM_DEF],
  [CCSYS_VALUEOF_NO_SUBST([$1],[$2],[$3])
   AS_VAR_SET(CCSYS_ENUM_DEF_$1)
   AS_VAR_IF(ccsys_cv_valueof_$1,[-1],
     [AS_VAR_SET(CCSYS_ENUM_DEF_$1,["/* undefined $1 */"])],
     [AS_VAR_SET(CCSYS_ENUM_DEF_$1,["$1=$ccsys_cv_valueof_$1[],"])])
   AC_SUBST([CCSYS_ENUM_DEF_$1])])

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

dnl page
dnl CCSYS_INT_TYPE_ALIAS --
dnl
dnl Inspect  a signed integer  type definition to determine  which among
dnl "int32_t" and "int64_t" is a suitable alias.  For example, to determine
dnl the alias of "mode_t" we do:
dnl
dnl   CCSYS_INT_TYPE_ALIAS([MODE_T],[mode_t],[CCSYS_SYS_TYPES_HEADER])
dnl
dnl this macro defines the substitution symbol "CCSYS_TYPE_MODE_T" to either
dnl "int32_t" or "int64_t".
dnl
dnl $1 - the stem used to generate variable names
dnl $2 - the type name
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_INT_TYPE_ALIAS],
  [CCSYS_SIZEOF([$1],[$2],[$3])
   AS_CASE($SIZEOF_$1,
      [4],[AS_VAR_SET([CCSYS_TYPE_$1],[int32_t])],
      [8],[AS_VAR_SET([CCSYS_TYPE_$1],[int64_t])],
      [AC_MSG_ERROR([cannot determine size of '$2'])])
   AC_SUBST([CCSYS_TYPE_$1],[$CCSYS_TYPE_$1])])

dnl page
dnl CCSYS_UINT_TYPE_ALIAS --
dnl
dnl Inspect an unsigned integer  type definition to determine  which among
dnl "uint32_t" and "uint64_t" is a suitable alias.  For example, to determine
dnl the alias of "size_t" we do:
dnl
dnl   CCSYS_INT_TYPE_ALIAS([SIZE_T],[size_t],[#include <stddef.h>])
dnl
dnl this macro defines the substitution symbol "CCSYS_TYPE_SIZE_T" to either
dnl "uint32_t" or "uint64_t".
dnl
dnl $1 - the stem used to generate variable names
dnl $2 - the type name
dnl $3 - the required header files
dnl
AC_DEFUN([CCSYS_UINT_TYPE_ALIAS],
  [CCSYS_SIZEOF([$1],[$2],[$3])
   AS_CASE($SIZEOF_$1,
      [4],[AS_VAR_SET([CCSYS_TYPE_$1],[uint32_t])],
      [8],[AS_VAR_SET([CCSYS_TYPE_$1],[uint64_t])],
      [AC_MSG_ERROR([cannot determine size of '$2'])])
   AC_SUBST([CCSYS_TYPE_$1],[$CCSYS_TYPE_$1])])

dnl page
dnl CCSYS_STRUCT_FIELD --
dnl
dnl Inspect  the availability of  a member in  a struct.  For  the field
dnl "st_dev" in "struct stat" we do:
dnl
dnl    CCSYS_STRUCT_FIELD([STRUCT_STAT_ST_DEV],
dnl                       [struct stat], [st_dev],
dnl                       [CCSYS_SYS_STAT_HEADER])
dnl
dnl If the field exists, it defines the substitution for the symbol:
dnl
dnl    CCSYS_HAVE_STRUCT_STAT_ST_DEV
dnl
dnl to 1; otherwise the substitution is to 0.
dnl
dnl $1 - stem for generated symbols
dnl $2 - original struct type
dnl $3 - original field name
dnl $4 - header file
dnl
AC_DEFUN([CCSYS_STRUCT_FIELD],
  [AS_VAR_SET(CCSYS_HAVE_$1,0)
   AC_CHECK_MEMBER([$2.$3],
     [AS_VAR_SET(CCSYS_HAVE_$1,1)],
     [AS_VAR_SET(CCSYS_HAVE_$1,0)],
     [$4])
   AS_IF([test $CCSYS_HAVE_$1 = 1],
     [AC_SUBST([CCSYS_HAVE_$1],$CCSYS_HAVE_$1)])])

dnl end of file
dnl Local Variables:
dnl mode: autoconf
dnl End:
