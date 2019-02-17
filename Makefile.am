## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL			\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CFLAGS		= $(MMUX_CFLAGS)
AM_CPPFLAGS		= -I$(srcdir)/src -I$(builddir)/src

## --------------------------------------------------------------------

nodist_pkgconfig_DATA	= meta/scripts/ccsys.pc


#### documentation

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

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
libccsys_la_CPPFLAGS	= $(AM_CPPFLAGS)
libccsys_la_CFLAGS	= $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS)
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
	tests/networking			\
	tests/processes				\
	tests/signals				\
	tests/sleeping				\
	tests/resources-limitations

if MMUX_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccsys_tests_cppflags		= $(AM_CPPFLAGS)
ccsys_tests_cflags		= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS) $(CCTESTS_CFLAGS)
ccsys_tests_ldadd		= libccsys.la $(CCTESTS_LIBS)

## --------------------------------------------------------------------

tests_version_CPPFLAGS		= $(ccsys_tests_cppflags)
tests_version_CFLAGS		= $(ccsys_tests_cflags)
tests_version_LDADD		= $(ccsys_tests_ldadd)

tests_system_program_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_system_program_CFLAGS	= $(ccsys_tests_cflags)
tests_system_program_LDADD	= $(ccsys_tests_ldadd)

tests_system_handlers_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_system_handlers_CFLAGS	= $(ccsys_tests_cflags)
tests_system_handlers_LDADD	= $(ccsys_tests_ldadd)

tests_users_groups_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_users_groups_CFLAGS	= $(ccsys_tests_cflags)
tests_users_groups_LDADD	= $(ccsys_tests_ldadd)

tests_input_output_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_input_output_CFLAGS	= $(ccsys_tests_cflags)
tests_input_output_LDADD	= $(ccsys_tests_ldadd)

tests_file_system_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_file_system_CFLAGS	= $(ccsys_tests_cflags)
tests_file_system_LDADD		= $(ccsys_tests_ldadd)

tests_networking_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_networking_CFLAGS		= $(ccsys_tests_cflags)
tests_networking_LDADD		= $(ccsys_tests_ldadd)

tests_processes_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_processes_CFLAGS		= $(ccsys_tests_cflags)
tests_processes_LDADD		= $(ccsys_tests_ldadd)

tests_signals_CPPFLAGS		= $(ccsys_tests_cppflags)
tests_signals_CFLAGS		= $(ccsys_tests_cflags)
tests_signals_LDADD		= $(ccsys_tests_ldadd)

tests_sleeping_CPPFLAGS		= $(ccsys_tests_cppflags)
tests_sleeping_CFLAGS		= $(ccsys_tests_cflags)
tests_sleeping_LDADD		= $(ccsys_tests_ldadd)

tests_resources_limitations_CPPFLAGS	= $(ccsys_tests_cppflags)
tests_resources_limitations_CFLAGS	= $(ccsys_tests_cflags)
tests_resources_limitations_LDADD	= $(ccsys_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### demo programs

EXTRA_DIST	+= \
	demos/listdir.c

noinst_PROGRAMS	= \
	demos/listdir

MMUX_TESTS_CFLAGS		=

ccsys_demos_cppflags		= $(AM_CPPFLAGS)
ccsys_demos_cflags		= $(CCSYS_FLAGS) $(AM_CFLAGS) \
	$(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS) $(CCTESTS_CFLAGS)
ccsys_demos_ldadd		= libccsys.la $(CCTESTS_LIBS)

## --------------------------------------------------------------------

demos_listdir_CPPFLAGS		= $(ccsys_demos_cppflags)
demos_listdir_CFLAGS		= $(ccsys_demos_cflags)
demos_listdir_LDADD		= $(ccsys_demos_ldadd)

demos/listdir.@OBJEXT@: src/ccsys.h


#### common rules

include $(top_srcdir)/meta/automake/mmux-makefile-rules

### end of file
