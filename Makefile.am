## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL configure.sh		\
			  meta/slackstuff/config.in	\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CFLAGS		= -I$(srcdir)/src -I$(builddir)/src -Wall $(AX_CFLAGS)

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/ccsys.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/ccsys.pc

## --------------------------------------------------------------------

AM_MAKEINFOFLAGS	= --no-split

info_TEXINFOS		= doc/ccsys.texi
doc_ccsys_TEXINFOS	= \
	doc/macros.texi		\
	doc/lgpl-3.0.texi	\
	doc/fdl-1.3.texi

#page
#### libraries

ccsys_CURRENT		= @ccsys_VERSION_INTERFACE_CURRENT@
ccsys_REVISION		= @ccsys_VERSION_INTERFACE_REVISION@
ccsys_AGE		= @ccsys_VERSION_INTERFACE_AGE@

include_HEADERS		= src/ccsys.h

lib_LTLIBRARIES		= libccsys.la
libccsys_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS)
libccsys_la_LDFLAGS	= -version-info $(ccsys_CURRENT):$(ccsys_REVISION):$(ccsys_AGE) \
			  $(CCEXCEPTIONS_LIBS)
libccsys_la_SOURCES	= \
	src/ccsys-internals.h		\
	src/ccsys-memory.c		\
	src/ccsys-users-groups.c	\
	src/ccsys-input-output.c	\
	src/ccsys-file-system.c		\
	src/ccsys-processes.c		\
	src/ccsys-networking.c		\
	src/ccsys-time.c		\
	src/ccsys-version.c

#page
#### libraries

check_LTLIBRARIES	= libcctests.la
libcctests_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS) \
	-Dcctests_VERSION_INTERFACE_STRING='"0.0.0"'	\
	-Dcctests_VERSION_INTERFACE_CURRENT=0		\
	-Dcctests_VERSION_INTERFACE_REVISION=0		\
	-Dcctests_VERSION_INTERFACE_AGE=0
libcctests_la_LDFLAGS	= $(CCEXCEPTIONS_LIBS)
libcctests_la_SOURCES	= \
	cctests/cctests.h			\
	cctests/cctests-internals.h		\
	cctests/cctests-version.c		\
	cctests/cctests-conditions.c		\
	cctests/cctests-exception-handlers.c	\
	cctests/cctests-system.c		\
	cctests/cctests-driver.c		\
	cctests/cctests-assertions.c		\
	cctests/cctests-process.c

#page
#### tests

check_PROGRAMS	= \
	tests/version				\
	tests/system-memory			\
	tests/system-handlers			\
	tests/users-groups			\
	tests/input-output			\
	tests/file-system

TESTS				= $(check_PROGRAMS)

ccsys_tests_cppflags		= -I$(srcdir)/src $(CCEXCEPTIONS_CFLAGS) -I$(srcdir)/cctests
ccsys_tests_ldadd		= libccsys.la $(CCEXCEPTIONS_LIBS) libcctests.la

tests_version_CPPFLAGS		= $(ccsys_tests_cppflags)
tests_version_LDADD		= $(ccsys_tests_ldadd)

tests_system_memory_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_system_memory_LDADD	= $(ccsys_tests_ldadd)

tests_system_handlers_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_system_handlers_LDADD	= $(ccsys_tests_ldadd)

tests_users_groups_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_users_groups_LDADD	= $(ccsys_tests_ldadd)

tests_input_output_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_input_output_LDADD	= $(ccsys_tests_ldadd)

tests_file_system_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_file_system_LDADD		= $(ccsys_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done

#page
#### demo programs

EXTRA_DIST	+= \
	demos/listdir.c

noinst_PROGRAMS	= \
	demos/listdir

ccsys_demos_cppflags		= -I$(srcdir)/src $(CCSYS_FLAGS) $(CCEXCEPTIONS_CFLAGS)
ccsys_demos_ldadd		= libccsys.la $(CCEXCEPTIONS_LIBS)

demos_listdir_CPPFLAGS		= $(ccsys_demos_cppflags)
demos_listdir_LDADD		= $(ccsys_demos_ldadd)

#page
#### Coverity support
#
# First run  "make clean",  then run "make  coverity" to  generate the
# report.  Remember  to put  in the  PATH the  directory in  which the
# Coverity tools are installed.
#
# The tool  will use  a temporary directory  "$(builddir)/cov-int"; we
# must *not* change this pathname.
#
# To generate and submit a report we must do:
#
#    $ make clean
#    $ make coverity-scan
#    $ make coverity-archive
#    $ make coverity-upload
#
# after "make  coverity-scan" we should  check that the last  lines of
# the file "cov-int/build-log.txt" report a successful build.
#
# Notice that if your Linux kernel is  at version 4.8 or above we have
# to first issue the command:
#
#    $ sudo /sbin/sysctl vsyscall=emulate
#
# to make the Coverity tools work.
#

CCSYS_COVERITY_TOKEN_FILE = /home/unsafe-users/marco-devel/.coverity-ccsys-token

.PHONY: coverity-scan coverity-archive coverity-upload

coverity-scan:
	cov-build --dir cov-int make -j 4

coverity-archive:
	tar caf ccsys.xz cov-int

coverity-upload:
	curl --form token=$$(<$(CCSYS_COVERITY_TOKEN_FILE)) \
	     --form email='marco.maggi-ipsu@poste.it' \
	     --form file='@ccsys.xz' \
	     --form version="$(VERSION)" \
	     --form description="A build description" \
	     'https://scan.coverity.com/builds?project=marcomaggi%2Fccsys'

#page
#### Static analysis with Clang's Static Analyzer
#
# See the documentation for the command line tool at:
#
#    <http://clang-analyzer.llvm.org/scan-build.html>
#
# To run the tool we must do:
#
#    $ make clean
#    $ make clang-static-analysis
#
# The  program  "scan-build"  works  by  overriding  the  CC  and  CXX
# environment variables.  Automake defines CXX as:
#
#    CXX = g++ -std=c++11
#
# so we override CXXFLAGS to include the language specification.
#

.PHONY: clang-static-analysis

clang-static-analysis:
	scan-build make CXXFLAGS='-std=c++11'

### end of file
