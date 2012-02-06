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

INST_H_FILES =				\
	ggit-blob.h			\
	ggit-commit.h			\
	ggit-config.h			\
	ggit-error.h			\
	ggit-index.h			\
	ggit-index-entry.h		\
	ggit-index-entry-unmerged.h	\
	ggit-native.h			\
	ggit-object.h			\
	ggit-oid.h			\
	ggit-ref.h			\
	ggit-repository.h		\
	ggit-revision-walker.h		\
	ggit-signature.h		\
	ggit-tag.h			\
	ggit-types.h			\
	ggit-type-factory.h		\
	ggit-type-factory-object.h	\
	ggit.h

NOINST_H_FILES =		\
	ggit-object-private.h	\
	ggit-utils.h

C_FILES =				\
	ggit-blob.c			\
	ggit-commit.c			\
	ggit-config.c			\
	ggit-error.c			\
	ggit-index.c			\
	ggit-index-entry.c		\
	ggit-index-entry-unmerged.c	\
	ggit-native.c			\
	ggit-object.c			\
	ggit-oid.c			\
	ggit-ref.c			\
	ggit-repository.c		\
	ggit-revision-walker.c		\
	ggit-signature.c		\
	ggit-tag.c			\
	ggit-type-factory.c		\
	ggit-type-factory-object.c	\
	ggit-utils.c

libgit2_glib_1_0_la_SOURCES = 	\
	$(INST_H_FILES)		\
	$(NOINST_H_FILES)	\
	$(C_FILES)

headerdir = $(prefix)/include/libgit2-glib-1.0/libgit2-glib
header_DATA = $(INST_H_FILES)

if ENABLE_PYTHON
overridesdir = $(pyoverridesdir)
overrides_PYTHON =                      \
        Ggit.py
endif

EXTRA_DIST =
CLEANFILES =

if HAVE_INTROSPECTION
  -include $(INTROSPECTION_MAKEFILE)
  INTROSPECTION_GIRS = Ggit-1.0.gir
  INTROSPECTION_SCANNER_ARGS = \
	-I$(top_srcdir) 		\
	--identifier-prefix=Ggit	\
	--c-include=libgit2-glib/ggit.h	\
	--warn-all

  introspection_sources = $(INST_H_FILES) $(C_FILES)

  Ggit-1.0.gir: libgit2-glib-1.0.la
  Ggit_1_0_gir_NAMESPACE = Ggit
  Ggit_1_0_gir_INCLUDES = GObject-2.0 GLib-2.0 Gio-2.0
  Ggit_1_0_gir_CFLAGS = $(LIBGIT2_GLIB_CFLAGS)
  Ggit_1_0_gir_LIBS = libgit2-glib-1.0.la
  Ggit_1_0_gir_FILES = $(addprefix $(srcdir)/,$(introspection_sources))
  Ggit_1_0_gir_PACKAGES = gobject-2.0 glib-2.0 libgit2 gio-2.0
  Ggit_1_0_gir_EXPORT_PACKAGES = libgit2-glib-1.0

  girdir = $(datadir)/gir-1.0
  gir_DATA = $(INTROSPECTION_GIRS)

  typelibdir = $(libdir)/girepository-1.0
  typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

  CLEANFILES += \
	$(gir_DATA)		\
	$(typelib_DATA)
endif
