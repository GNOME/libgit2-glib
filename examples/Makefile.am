INCLUDES = -g -I$(top_srcdir) -I$(top_srcdir)/libgit2-glib $(LIBGIT2_GLIB_CFLAGS)

noinst_PROGRAMS = $(EXAMPLE_PROGS)
progs_ldadd     = $(top_builddir)/libgit2-glib/libgit2-glib-1.0.la $(LIBGIT2_GLIB_LIBS)

EXAMPLE_PROGS		= general
general_SOURCES		= general.c
general_LDADD		= $(progs_ldadd)

EXAMPLE_PROGS		+= walk
walk_SOURCES		= walk.c
walk_LDADD		= $(progs_ldadd)

# ex:set ts=8 noet:
