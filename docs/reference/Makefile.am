## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = 1.7

# The name of the module.
DOC_MODULE = libgit2-glib-1.0

# The top-level SGML file.
DOC_MAIN_SGML_FILE = $(DOC_MODULE)-docs.sgml

# Extra options to supply to gtkdoc-scan
SCAN_OPTIONS =

# The directory containing the source code. Relative to $(srcdir)
DOC_SOURCE_DIR = $(top_srcdir)/libgit2-glib

# Used for dependencies
HFILE_GLOB = $(top_srcdir)/libgit2-glib/*.h
CFILE_GLOB = $(top_srcdir)/libgit2-glib/*.c

# Headers to ignore
IGNORE_HFILES = \
	config.h	\
	ggit-convert.h	\
	ggit-utils.h

update-sections: $(HFILE_GLOB)
	@echo 'gtk-doc: Scanning header files'
	cp $(DOC_MODULE)-sections.txt $(DOC_MODULE)-sections.txt.old && \
	(cd $(srcdir) && \
	  gtkdoc-scan --module=$(DOC_MODULE) --source-dir=$(DOC_SOURCE_DIR) --ignore-headers="$(IGNORE_HFILES)" --rebuild-sections $(SCAN_OPTIONS) $(EXTRA_HFILES)) && \
	cp $(DOC_MODULE)-sections.txt $(DOC_MODULE)-sections.txt.new && \
	mv $(DOC_MODULE)-sections.txt.old $(DOC_MODULE)-sections.txt

# Extra options to supply to gtkdoc-mkdb
MKDB_OPTIONS = --xml-mode --output-format=xml

# CFLAGS and LDFLAGS for compiling gtkdoc-scangobj with your library.
# Only needed if you are using gtkdoc-scangobj to dynamically query widget
# signals and properties.
GTKDOC_CFLAGS = \
	-I$(top_srcdir)	\
	$(LIBGIT2_GLIB_CFLAGS)

GTKDOC_LIBS = \
	$(top_builddir)/libgit2-glib/libgit2-glib-1.0.la	\
	$(LIBGIT2_GLIB_LIBS)

# Images to copy into HTML directory
HTML_IMAGES =

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE)
content_files =			\
	version.xml

# Extra options to supply to gtkdoc-fixref
FIXXREF_OPTIONS=							\
	--extra-dir=$(GLIB_PREFIX)/share/gtk-doc/html/gobject		\
	--extra-dir=$(GLIB_PREFIX)/share/gtk-doc/html/glib		\
	--extra-dir=$(GLIB_PREFIX)/share/gtk-doc/html/gio

# include common portion ...
include $(top_srcdir)/gtk-doc.make

# Other files to distribute
EXTRA_DIST +=

# ex:set ts=8 noet:
