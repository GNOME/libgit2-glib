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

libgit2_glib_1_0_la_LIBADD = $(LIBGIT2_GLIB_LIBS)

INST_H_FILES =			\
	ggit-blob.h		\
	ggit-commit.h		\
	ggit-error.h		\
	ggit-object.h		\
	ggit-oid.h		\
	ggit-repository.h	\
	ggit-signature.h	\
	ggit-tag.h		\
	ggit-types.h		\
	ggit.h

NOINST_H_FILES =		\
	ggit-object-private.h	\
	ggit-utils.h

C_FILES =			\
	ggit-blob.c		\
	ggit-commit.c		\
	ggit-error.c		\
	ggit-object.c		\
	ggit-oid.c		\
	ggit-repository.c	\
	ggit-signature.c	\
	ggit-tag.c		\
	ggit-utils.c

libgit2_glib_1_0_la_SOURCES = 	\
	$(INST_H_FILES)		\
	$(NOINST_H_FILES)	\
	$(C_FILES)

headerdir = $(prefix)/include/libgit2-glib-1.0/libgit2-glib
header_DATA = $(INST_H_FILES)

EXTRA_DIST =
CLEANFILES =
