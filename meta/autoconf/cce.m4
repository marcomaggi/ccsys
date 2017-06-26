dnl
dnl Part of: CCExceptions
dnl Contents: macros for library configuration
dnl Date: Wed Jul 16, 2014
dnl
dnl Abstract
dnl
dnl	Macros used in "configure.ac".  Some  of these macros are reused
dnl     from infrastructure of the the package Vicare Scheme.
dnl
dnl Copyright (C) 2014, 2017 Marco Maggi <marco.maggi-ipsu@poste.it>
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


dnl Wrapper for AC_ARG_ENABLE which adds  verbose messages and defines a
dnl shell variable "cce_enable_$1" set to "yes" or "no".
dnl
dnl $1 - upper case option name
dnl $2 - command line option name "--enable-$2"
dnl $3 - default (yes, no)
dnl $4 - text for the "checking option... " message
dnl $5 - text for the "enable option... " message
AC_DEFUN([CCE_ENABLE_OPTION],
  [cce_enable_$1=$3
   AC_MSG_CHECKING([$4])
   AC_ARG_ENABLE([$2],
     [AS_HELP_STRING([--enable-$2],
        [$5 (default is $3)])],
     [AS_CASE([$enableval],
        [yes],[cce_enable_$1=yes],
        [no], [cce_enable_$1=no],
        [AC_MSG_ERROR([bad value $enableval for --enable-$2])])],
     [cce_enable_$1=$3])
   AC_MSG_RESULT([$cce_enable_$1])])

dnl end of file
dnl Local Variables:
dnl mode: autoconf
dnl End:
