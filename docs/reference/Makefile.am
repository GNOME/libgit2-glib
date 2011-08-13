GTKDOC_CFLAGS = -I$(top_srcdir) $(LIBGIT2_CFLAGS)
GTKDOC_LIBS = $(top_builddir)/src/libgit2-glib-1.0.la $(LIBGIT2_LIBS)

## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = 1.7

# The name of the module.
DOC_MODULE = libgit2-glib-1.0

# The top-level SGML file.
DOC_MAIN_SGML_FILE = libgit2-glib-docs.sgml

# Extra options to supply to gtkdoc-scan
SCAN_OPTIONS =

# The directory containing the source code. Relative to $(srcdir)
DOC_SOURCE_DIR = ../..

# Used for dependencies
HFILE_GLOB = $(top_srcdir)/src/*.h
CFILE_GLOB = $(top_srcdir)/src/*.c

# Headers to ignore
IGNORE_HFILES =					\
	config.h				\
	ggit-object-private.h			\
	ggit-utils.h

update-sections: $(HFILE_GLOB)
	@echo 'gtk-doc: Scanning header files'
	cp $(DOC_MODULE)-sections.txt $(DOC_MODULE)-sections.txt.old && \
	(cd $(srcdir) && \
	  gtkdoc-scan --module=$(DOC_MODULE) --source-dir=$(DOC_SOURCE_DIR) --ignore-headers="$(IGNORE_HFILES)" --rebuild-sections $(SCAN_OPTIONS) $(EXTRA_HFILES)) && \
	cp $(DOC_MODULE)-sections.txt $(DOC_MODULE)-sections.txt.new && \
	mv $(DOC_MODULE)-sections.txt.old $(DOC_MODULE)-sections.txt

# Extra options to supply to gtkdoc-mkdb
MKDB_OPTIONS = --sgml-mode --output-format=xml --ignore-files=trio

# Images to copy into HTML directory
HTML_IMAGES =

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE)
content_files =			\
	version.xml

# Extra options to supply to gtkdoc-fixref
FIXXREF_OPTIONS=

# include common portion ...
include $(top_srcdir)/gtk-doc.make

# Other files to distribute
EXTRA_DIST +=
