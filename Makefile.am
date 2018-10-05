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


#### libraries

ccsys_CURRENT		= @ccsys_VERSION_INTERFACE_CURRENT@
ccsys_REVISION		= @ccsys_VERSION_INTERFACE_REVISION@
ccsys_AGE		= @ccsys_VERSION_INTERFACE_AGE@

include_HEADERS		= src/ccsys.h

lib_LTLIBRARIES		= libccsys.la
libccsys_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS)
libccsys_la_LDFLAGS	= -version-info $(ccsys_CURRENT):$(ccsys_REVISION):$(ccsys_AGE) \
			  $(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS)
libccsys_la_SOURCES	= \
	src/ccsys-internals.h			\
	src/ccsys-system-program.c		\
	src/ccsys-memory.c			\
	src/ccsys-users-groups.c		\
	src/ccsys-input-output.c		\
	src/ccsys-file-system.c			\
	src/ccsys-processes.c			\
	src/ccsys-networking.c			\
	src/ccsys-time.c			\
	src/ccsys-signals.c			\
	src/ccsys-sleeping.c			\
	src/ccsys-resources-limitations.c	\
	src/ccsys-version.c


#### tests

check_PROGRAMS	= \
	tests/version				\
	tests/system-program			\
	tests/system-handlers			\
	tests/users-groups			\
	tests/input-output			\
	tests/file-system			\
	tests/processes				\
	tests/signals				\
	tests/sleeping				\
	tests/resources-limitations

if MMUX_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccsys_tests_cppflags		= -I$(srcdir)/src \
	$(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS) $(CCTESTS_CFLAGS)
ccsys_tests_ldadd		= libccsys.la \
	$(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS) $(CCTESTS_LIBS)

tests_version_CPPFLAGS		= $(ccsys_tests_cppflags)
tests_version_LDADD		= $(ccsys_tests_ldadd)

tests_system_program_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_system_program_LDADD	= $(ccsys_tests_ldadd)

tests_system_handlers_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_system_handlers_LDADD	= $(ccsys_tests_ldadd)

tests_users_groups_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_users_groups_LDADD	= $(ccsys_tests_ldadd)

tests_input_output_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_input_output_LDADD	= $(ccsys_tests_ldadd)

tests_file_system_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_file_system_LDADD		= $(ccsys_tests_ldadd)

tests_processes_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_processes_LDADD		= $(ccsys_tests_ldadd)

tests_signals_CPPFLAGS		= $(ccsys_tests_cppflags)
tests_signals_LDADD		= $(ccsys_tests_ldadd)

tests_sleeping_CPPFLAGS		= $(ccsys_tests_cppflags)
tests_sleeping_LDADD		= $(ccsys_tests_ldadd)

tests_resources_limitations_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_resources_limitations_LDADD	= $(ccsys_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### demo programs

EXTRA_DIST	+= \
	demos/listdir.c

noinst_PROGRAMS	= \
	demos/listdir

ccsys_demos_cppflags		= -I$(srcdir)/src $(CCSYS_FLAGS) \
	$(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS) $(CCTESTS_CFLAGS)
ccsys_demos_ldadd		= libccsys.la \
	$(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS) $(CCTESTS_LIBS)

demos_listdir_CPPFLAGS		= $(ccsys_demos_cppflags)
demos_listdir_LDADD		= $(ccsys_demos_ldadd)

demos/listdir.@OBJEXT@: src/ccsys.h


#### testing under GDB

$(builddir)/.gdbinit:
	ln -sv "$(top_srcdir)/gdbinit" "$(builddir)/.gdbinit"

# This rule is meant to be used as:
#
#   $ make all gdb-test FILE=tests/inspection CFLAGS='-g'
#
# after having created an appropriate file "$(top_srcdir)/.gdbinit".
#
gdb-test: $(builddir)/.gdbinit
	libtool --mode=execute gdb $$FILE


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

.PHONY: clang-static-analysis

clang-static-analysis:
	scan-build make

### end of file
