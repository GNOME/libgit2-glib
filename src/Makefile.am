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
	git-glib-error.h	\
	git-glib-object.h	\
	git-glib-oid.h		\
	git-glib-repository.h

NOINST_H_FILES =			\
	git-glib-object-private.h

C_FILES =				\
	git-glib-error.c		\
	git-glib-object.c		\
	git-glib-oid.c			\
	git-glib-repository.c

libgit2_glib_1_0_la_SOURCES = 	\
	$(INST_H_FILES)		\
	$(NOINST_H_FILES)	\
	$(C_FILES)

headerdir = $(prefix)/include/libgit2-glib-1.0/libgit2-glib
header_DATA = $(INST_H_FILES)

EXTRA_DIST =
CLEANFILES =

if HAVE_INTROSPECTION
  -include $(INTROSPECTION_MAKEFILE)
  INTROSPECTION_GIRS = GitGlib-1.0.gir
  INTROSPECTION_SCANNER_ARGS = \
	-I$(top_srcdir) 		\
	--identifier-prefix=GitGlib	\
	--warn-all

  introspection_sources = $(INST_H_FILES) $(C_FILES)

  GitGlib-1.0.gir: libgit2-glib-1.0.la
  GitGlib_1_0_gir_NAMESPACE = GitGlib
  GitGlib_1_0_gir_INCLUDES = GObject-2.0 GLib-2.0
  GitGlib_1_0_gir_CFLAGS = $(LIBGIT2_GLIB_CFLAGS)
  GitGlib_1_0_gir_LIBS = libgit2-glib-1.0.la
  GitGlib_1_0_gir_FILES = $(addprefix $(srcdir)/,$(introspection_sources))
  GitGlib_1_0_gir_PACKAGES = gobject-2.0 glib-2.0 libgit2
  GitGlib_1_0_gir_EXPORT_PACKAGES = libgit2-glib-1.0

  girdir = $(datadir)/gir-1.0
  gir_DATA = $(INTROSPECTION_GIRS)

  typelibdir = $(libdir)/girepository-1.0
  typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

  CLEANFILES += \
	$(gir_DATA)		\
	$(typelib_DATA)
endif
