dnl @configure_input@
dnl

AC_PREREQ([2.69])
AC_INIT([CCSys],[0.1.0-devel.0],
  [marco.maggi-ipsu@poste.it],
  [ccsys],[http://github.com/marcomaggi/ccsys/])
dnl This is the revision number of this configure.ac
AC_REVISION([0.1])
AC_COPYRIGHT([Copyright (c) 2017 Marco Maggi <marco.maggi-ipsu@poste.it>

This program is free software: you  can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free  Software Foundation, either version  3 of the License,  or (at
your option) any later version.

This program  is distributed  in the  hope that it  will be  useful, but
WITHOUT   ANY   WARRANTY;  without   even   the   implied  warranty   of
MERCHANTABILITY  or  FITNESS FOR  A  PARTICULAR  PURPOSE.  See  the  GNU
General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([meta/autotools])
AC_CONFIG_AUX_DIR([meta/autotools])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip subdir-objects])
AM_MAINTAINER_MODE

AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

LT_PREREQ([2.4])
LT_INIT

dnl page
#### configuration

case "$target_os" in
  *linux*)
    ccsys_OS_LINUX=yes
    AC_MSG_NOTICE([detected OS: linux])
    ;;
  *bsd*)
    ccsys_OS_BSD=yes
    AC_MSG_NOTICE([detected OS: BSD])
    ;;
  *cygwin*)
    ccsys_OS_CYGWIN=yes
    AC_MSG_NOTICE([detected OS: CYGWIN])
    ;;
  *darwin*)
    ccsys_OS_DARWIN=yes
    AC_MSG_NOTICE([detected OS: DARWIN])
    ;;
esac
AM_CONDITIONAL([ON_LINUX], [test "x$ccsys_OS_LINUX"  = xyes])
AM_CONDITIONAL([ON_BSD],   [test "x$ccsys_OS_BSD"    = xyes])
AM_CONDITIONAL([ON_CYGWIN],[test "x$ccsys_OS_CYGWIN" = xyes])
AM_CONDITIONAL([ON_DARWIN],[test "x$ccsys_OS_DARWIN" = xyes])

AM_COND_IF([ON_DARWIN],[AC_DEFINE([CCSYS_ON_DARWIN],[1],[defined to 1 when compiling on Darwin])])

dnl page
#### libraries interface version

AS_VAR_SET([SLACKSTUFF_PACKAGE_VERSION],[0.1d0.0])
AC_SUBST([SLACKSTUFF_PACKAGE_VERSION])

dnl This is the version stored in the pkg-config data file.
AC_SUBST([CCSYS_PKG_CONFIG_VERSION],[0.1])

MM_LIBTOOL_LIBRARY_VERSIONS([ccsys],1,0,0)

dnl page
#### external libraries

PKG_CHECK_MODULES([CCEXCEPTIONS],[ccexceptions >= 0.6])

AC_CACHE_SAVE

dnl page
#### basic system inspection

AC_LANG([C])
AC_PROG_CC_C99

AX_CHECK_COMPILE_FLAG([-std=c11],[],[AC_MSG_ERROR([*** Compiler does not support -std=c11])],[-pedantic])
AX_CFLAGS="-std=c11"
AC_SUBST(AX_CFLAGS)

AM_PROG_AS
AC_PROG_CC
AM_PROG_CC_C_O
AC_HEADER_STDC
AC_CHECK_HEADERS([assert.h stdint.h stdbool.h setjmp.h memory.h limits.h])
AC_CHECK_HEADERS([dirent.h utime.h fcntl.h sys/select.h sys/stat.h sys/time.h sys/types.h sys/uio.h netdb.h sys/wait.h])
AC_HEADER_STDBOOL

dnl The  documentation of GNU  Autoconf states that AC_PROG_CC  sets the
dnl variable "GCC" to "yes" if the selected compiler is GCC.
AM_CONDITIONAL([WANT_GCC],[test "x$GCC" = xyes])

AC_CACHE_SAVE

dnl page
#### checks for library functions

dnl This constant is redefined in the main header file.  We define it here so
dnl that it is there in the tests below.
AC_DEFINE([_POSIX_C_SOURCE],[200809L],[Request enabling latest POSIX features.])

AC_DEFINE([_GNU_SOURCE],[1],[enable everything GNU])

AC_FUNC_MALLOC

AC_CHECK_FUNCS([mlock munlock mlockall munlockall])
AC_CHECK_FUNCS([open openat close read pread write pwrite lseek readv writev])
AC_CHECK_FUNCS([select dup dup2 pipe mkfifo])
AC_CHECK_FUNCS([mmap munmap msync mprotect])
AC_CHECK_FUNCS([getcwd chdir fchdir opendir fdopendir readdir closedir stat fstat lstat mkdir rmdir link linkat symlink symlinkat readlink readlinkat realpath unlink unlinkat remove rename chown fchown lchown fchownat chmod fchmod fchmodat access faccessat utime utimes truncate ftruncate])
AC_CHECK_FUNCS([mkstemp])
AC_CHECK_FUNCS([bind getsockname inet_network socket shutdown socketpair connect listen accept getpeername send recv sendto recvfrom getsockopt setsockopt])
AC_CHECK_FUNCS([system fork execv execve execvp waitpid])

dnl GNU C library stuff

dnl AC_CHECK_HEADERS([search.h])
AC_CHECK_FUNCS([mkdtemp mremap madvise inet_aton])
dnl AC_CHECK_FUNCS([gethostbyname_r gethostbyname2_r gethostbyaddr_r])

dnl Linux stuff
AC_CHECK_FUNCS([lutimes futimes])

AC_CACHE_SAVE

dnl page
#### inspections of data structures

AC_DEFUN([CCSYS_DIRENT_HEADER],[
#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#endif
])

CCSYS_SIZEOF([DIRENT],[struct dirent],[CCSYS_DIRENT_HEADER])
CCSYS_OFFSETOF([DIRENT_D_NAME],[struct dirent],[d_name],[CCSYS_DIRENT_HEADER])

dnl page
#### finish

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile]
   [src/ccsys.h]
   [meta/scripts/ccsys.pc]
   [meta/slackstuff/config]
   [meta/slackware/slack-desc])
AC_OUTPUT

### end of file
# Local Variables:
# mode: autoconf
# page-delimiter: "^dnl page"
# End:
