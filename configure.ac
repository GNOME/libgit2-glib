AC_PREREQ([2.63])

m4_define(libgit2_glib_major_version,  3)
m4_define(libgit2_glib_minor_version,  1)
m4_define(libgit2_glib_micro_version,  1)
m4_define(libgit2_glib_version, libgit2_glib_major_version.libgit2_glib_minor_version.libgit2_glib_micro_version)

AC_INIT([libgit2-glib],
        [libgit2_glib_version],
        [http://bugzilla.gnome.org/enter_bug.cgi?product=libgit2-glib],
        [libgit2-glib])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11 tar-ustar dist-xz no-dist-gzip])
AM_MAINTAINER_MODE([enable])

AM_SILENT_RULES([yes])

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT([disable-static])

AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL
AC_PROG_CC

INTROSPECTION_REQUIRED=0.10.1

PKG_CHECK_MODULES(LIBGIT2_GLIB, [
	gobject-2.0 >= 2.26.0
	glib-2.0 >= 2.26.0
	gio-2.0 >= 2.26.0
	libgit2 >= 0.14.0
	gobject-introspection-1.0 >= $INTROSPECTION_REQUIRED
])

GOBJECT_INTROSPECTION_REQUIRE($INTROSPECTION_REQUIRED)

AC_SUBST(INTROSPECTION_REQUIRED)

GNOME_COMPILE_WARNINGS([maximum])

AC_OUTPUT([
	Makefile
	examples/Makefile
	src/Makefile
])

