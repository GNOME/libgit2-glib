AM_CPPFLAGS =				\
	-g				\
	-I$(top_srcdir)			\
	-I$(top_builddir)		\
	$(LIBGIT2_GLIB_CFLAGS)

noinst_PROGRAMS = $(TEST_PROGS)
progs_ldadd     = $(top_builddir)/libgit2-glib/libgit2-glib-1.0.la $(LIBGIT2_GLIB_LIBS)

TESTS = $(TEST_PROGS)

TEST_PROGS = repository
repository_SOURCES = repository.c
repository_LDADD = $(progs_ldadd)

# ex:set ts=8 noet:
