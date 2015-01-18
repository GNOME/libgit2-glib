lib_LTLIBRARIES = libgit2-glib-1.0.la

AM_CPPFLAGS =								\
	-I$(top_srcdir)							\
	-I$(top_builddir)						\
	-I$(srcdir)							\
	$(LIBGIT2_GLIB_CFLAGS)						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)

libgit2_glib_1_0_la_LDFLAGS =				\
	-version-info $(LIBGIT2_GLIB_LT_VERSION)	\
	-export-dynamic					\
	-no-undefined					\
	-export-symbols-regex "^[^_].*"

libgit2_glib_1_0_la_LIBADD = $(LIBGIT2_GLIB_LIBS)

H_FILES =					\
	ggit-blame.h				\
	ggit-blame-options.h			\
	ggit-blob.h				\
	ggit-blob-output-stream.h		\
	ggit-branch.h				\
	ggit-branch-enumerator.h		\
	ggit-checkout-options.h			\
	ggit-cherry-pick-options.h		\
	ggit-clone-options.h			\
	ggit-commit.h				\
	ggit-commit-parents.h			\
	ggit-config.h				\
	ggit-config-entry.h			\
	ggit-cred.h				\
	ggit-cred-plaintext.h			\
	ggit-diff.h				\
	ggit-diff-delta.h			\
	ggit-diff-file.h			\
	ggit-diff-find-options.h		\
	ggit-diff-format-email-options.h	\
	ggit-diff-hunk.h			\
	ggit-diff-line.h			\
	ggit-diff-options.h			\
	ggit-diff-similarity-metric.h		\
	ggit-error.h				\
	ggit-index.h				\
	ggit-index-entry.h			\
	ggit-index-entry-resolve-undo.h		\
	ggit-main.h				\
	ggit-message.h				\
	ggit-merge-options.h			\
	ggit-native.h				\
	ggit-note.h				\
	ggit-object.h				\
	ggit-object-factory.h			\
	ggit-object-factory-base.h		\
	ggit-oid.h				\
	ggit-patch.h				\
	ggit-push-options.h			\
	ggit-ref.h				\
	ggit-ref-spec.h				\
	ggit-reflog.h				\
	ggit-reflog-entry.h			\
	ggit-remote.h				\
	ggit-remote-callbacks.h			\
	ggit-repository.h			\
	ggit-revert-options.h			\
	ggit-revision-walker.h			\
	ggit-signature.h			\
	ggit-status-options.h			\
	ggit-submodule.h			\
	ggit-tag.h				\
	ggit-transfer-progress.h		\
	ggit-tree.h				\
	ggit-tree-builder.h			\
	ggit-tree-entry.h			\
	ggit-types.h

BUILT_H_FILES = ggit.h
BUILT_H_FILES_ENUMS = ggit-enum-types.h

C_FILES =					\
	ggit-blame.c				\
	ggit-blame-options.c			\
	ggit-blob.c				\
	ggit-blob-output-stream.c		\
	ggit-branch.c				\
	ggit-branch-enumerator.c		\
	ggit-checkout-options.c			\
	ggit-cherry-pick-options.c		\
	ggit-clone-options.c			\
	ggit-commit.c				\
	ggit-commit-parents.c			\
	ggit-config.c				\
	ggit-config-entry.c			\
	ggit-convert.c				\
	ggit-cred.c				\
	ggit-cred-plaintext.c			\
	ggit-diff.c				\
	ggit-diff-delta.c			\
	ggit-diff-file.c			\
	ggit-diff-find-options.c		\
	ggit-diff-format-email-options.c	\
	ggit-diff-hunk.c			\
	ggit-diff-line.c			\
	ggit-diff-options.c			\
	ggit-diff-similarity-metric.c		\
	ggit-error.c				\
	ggit-index.c				\
	ggit-index-entry.c			\
	ggit-index-entry-resolve-undo.c		\
	ggit-main.c				\
	ggit-message.c				\
	ggit-merge-options.c			\
	ggit-native.c				\
	ggit-note.c				\
	ggit-object.c				\
	ggit-object-factory.c			\
	ggit-object-factory-base.c		\
	ggit-oid.c				\
	ggit-patch.c				\
	ggit-push-options.c			\
	ggit-ref.c				\
	ggit-ref-spec.c				\
	ggit-reflog.c				\
	ggit-reflog-entry.c			\
	ggit-remote.c				\
	ggit-remote-callbacks.c			\
	ggit-repository.c			\
	ggit-revert-options.c			\
	ggit-revision-walker.c			\
	ggit-signature.c			\
	ggit-status-options.c			\
	ggit-submodule.c			\
	ggit-tag.c				\
	ggit-transfer-progress.c		\
	ggit-tree.c				\
	ggit-tree-builder.c			\
	ggit-tree-entry.c			\
	ggit-types.c				\
	ggit-utils.c

BUILT_C_FILES =
BUILT_C_FILES_ENUMS = ggit-enum-types.c

if GIT_SSH
C_FILES +=				\
	ggit-cred-ssh-interactive.c	\
	ggit-cred-ssh-key-from-agent.c

H_FILES +=				\
	ggit-cred-ssh-interactive.h	\
	ggit-cred-ssh-key-from-agent.h
endif

BUILT_SOURCES = $(BUILT_C_FILES) $(BUILT_C_FILES_ENUMS) $(BUILT_H_FILES) $(BUILT_H_FILES_ENUMS)

INST_H_FILES = $(H_FILES) $(BUILT_H_FILES) $(BUILT_H_FILES_ENUMS)

NOINST_H_FILES =			\
	ggit-convert.h			\
	ggit-utils.h

ENUM_TYPES =				\
	$(H_FILES)

ggit-enum-types.h: ggit-enum-types.h.template $(H_FILES) $(BUILT_H_FILES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(builddir) && $(GLIB_MKENUMS) --template $(srcdir)/ggit-enum-types.h.template $(addprefix $(srcdir)/,$(H_FILES)) $(BUILT_H_FILES)) > $@

ggit-enum-types.c: ggit-enum-types.c.template $(H_FILES) $(BUILT_H_FILES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(builddir) && $(GLIB_MKENUMS) --template $(srcdir)/ggit-enum-types.c.template $(addprefix $(srcdir)/,$(H_FILES)) $(BUILT_H_FILES)) > $@

libgit2_glib_1_0_la_SOURCES = 	\
	$(INST_H_FILES)		\
	$(NOINST_H_FILES)	\
	$(BUILT_C_FILES)	\
	$(BUILT_C_FILES_ENUMS)	\
	$(C_FILES)

headerdir = $(prefix)/include/libgit2-glib-1.0/libgit2-glib
header_DATA = $(INST_H_FILES)

if ENABLE_PYTHON
overridesdir = $(pyoverridesdir)
overrides_PYTHON =                      \
        Ggit.py
endif

EXTRA_DIST = 				\
	Ggit.py				\
	ggit-enum-types.h.template	\
	ggit-enum-types.c.template	\
	ggit.h.in

CLEANFILES = $(BUILT_SOURCES)

if HAVE_INTROSPECTION
  -include $(INTROSPECTION_MAKEFILE)
  INTROSPECTION_GIRS = Ggit-1.0.gir
  INTROSPECTION_SCANNER_ENV = CC="$(CC)"
  INTROSPECTION_SCANNER_ARGS = \
	-I$(top_srcdir) 		\
	-I$(top_builddir) 		\
	--identifier-prefix=Ggit	\
	--c-include=libgit2-glib/ggit.h	\
	--warn-all

  introspection_src_sources = $(H_FILES) $(C_FILES)
  introspection_built_sources = $(BUILT_SOURCES)

  Ggit-1.0.gir: libgit2-glib-1.0.la
  Ggit_1_0_gir_NAMESPACE = Ggit
  Ggit_1_0_gir_INCLUDES = GObject-2.0 GLib-2.0 Gio-2.0
  Ggit_1_0_gir_CFLAGS = $(LIBGIT2_GLIB_CFLAGS)
  Ggit_1_0_gir_LIBS = libgit2-glib-1.0.la
  Ggit_1_0_gir_FILES = $(addprefix $(srcdir)/,$(introspection_src_sources)) $(introspection_built_sources)
  Ggit_1_0_gir_PACKAGES = gobject-2.0 glib-2.0 libgit2 gio-2.0
  Ggit_1_0_gir_EXPORT_PACKAGES = libgit2-glib-1.0

  girdir = $(datadir)/gir-1.0
  gir_DATA = $(INTROSPECTION_GIRS)

  typelibdir = $(libdir)/girepository-1.0
  typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

  CLEANFILES += \
	$(gir_DATA)		\
	$(typelib_DATA)

if ENABLE_VAPIGEN
include $(VAPIGEN_MAKEFILE)

ggit-1.0.vapi: Ggit-1.0.gir

VAPIGEN_VAPIS = ggit-1.0.vapi

ggit_1_0_vapi_DEPS = gio-2.0
ggit_1_0_vapi_METADATADIRS = $(top_srcdir)
ggit_1_0_vapi_FILES = Ggit-1.0.gir

vapidir = $(datadir)/vala/vapi
vapi_DATA = $(VAPIGEN_VAPIS) ggit-1.0.deps

ggit-1.0.deps:
	$(AM_V_GEN) echo $(ggit_1_0_vapi_DEPS) | tr ' ' '\n' > $@

CLEANFILES += $(VAPIGEN_VAPIS) ggit-1.0.deps

endif # ENABLE_VAPIGEN

endif # HAVE_INTROSPECTION

coverage:
	$(AM_V_GEN) LIBGIT2_INCLUDE_DIR=$(shell $(PKG_CONFIG) --variable=includedir libgit2)/git2 $(top_srcdir)/tools/coverage.py $(AM_CPPFLAGS) -isystem $(shell llvm-clang -print-file-name=include) -- $(C_FILES)

.PHONY: coverage

# ex:set ts=8 noet:
