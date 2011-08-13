SUBDIRS = src docs examples

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgit2-glib-1.0.pc

EXTRA_DIST=libgit2-glib-1.0.pc.in

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

dist-hook:
	@if test -d "$(srcdir)/.git"; \
	then \
		echo Creating ChangeLog && \
		(GIT_DIR=$(top_srcdir)/.git \
		  ./missing --run git log --stat -M -C --name-status  --date=short --no-color) | \
		  fmt --split-only > ChangeLog.tmp \
		&& mv -f ChangeLog.tmp $(top_distdir)/ChangeLog \
		|| ( rm -f ChangeLog.tmp ; \
		  echo Failed to generate ChangeLog >&2 ); \
	else \
		echo A git clone is required to generate a ChangeLog >&2; \
	fi
