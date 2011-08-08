INCLUDES = -g -I$(top_srcdir) -I$(top_srcdir)/src $(LIBGIT2_GLIB_CFLAGS)

noinst_PROGRAMS = $(EXAMPLE_PROGS)
progs_ldadd     = $(top_builddir)/src/libgit2-glib-1.0.la

EXAMPLE_PROGS		= general
general_SOURCES		= general.c
general_LDADD		= $(progs_ldadd)
