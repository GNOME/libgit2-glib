lib_LTLIBRARIES = libgit2-glib-1.0.la

INCLUDES =								\
	-I$(top_srcdir)							\
	-I$(srcdir)							\
	$(LIBGIT2_GLIB_CFLAGS)						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)					\
	-DDATADIR=\""$(datadir)"\"					\
	-DLIBDIR=\""$(libdir)"\"

libgit2_glib_1_0_la_LDFLAGS = \
	-export-dynamic -no-undefined -export-symbols-regex "^[^_].*"

libgit2_glib_1_0_la_LIBADD = $(GITG_LIBS)

INST_H_FILES =			\
	git2-glib-repository.h

NOINST_H_FILES =

C_FILES =				\
	git2-glib-repository.c

libgit2_glib_1_0_la_SOURCES = 	\
	$(INST_H_FILES)		\
	$(NOINST_H_FILES)	\
	$(C_FILES)

headerdir = $(prefix)/include/libgit2-glib-1.0/libgit2-glib
header_DATA = $(INST_H_FILES)

EXTRA_DIST =
CLEANFILES =
