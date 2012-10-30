AC_PREREQ([2.63])

m4_define(libgit2_glib_major_version,  0)
m4_define(libgit2_glib_minor_version,  0)
m4_define(libgit2_glib_micro_version,  1)
m4_define(libgit2_glib_version, libgit2_glib_major_version.libgit2_glib_minor_version.libgit2_glib_micro_version)

AC_INIT([libgit2-glib],
        [libgit2_glib_version],
        [http://bugzilla.gnome.org/enter_bug.cgi?product=libgit2-glib],
        [libgit2-glib])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([libgit2-glib/ggit-repository.c])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 tar-ustar dist-xz no-dist-gzip])
AM_MAINTAINER_MODE([enable])

AM_SILENT_RULES([yes])
AM_PATH_PYTHON(3.2.3)

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT([disable-static])

AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL
AC_PROG_CC

# GLib enums
AC_PATH_PROG(GLIB_MKENUMS, glib-mkenums)

# Documentation
GTK_DOC_CHECK([1.11], [--flavour=no-tmpl])

INTROSPECTION_REQUIRED=0.10.1
GLIB_REQUIRED=2.28.0
GIT2_REQUIRED=0.17.0

PKG_CHECK_MODULES(LIBGIT2_GLIB, [
	gobject-2.0 >= $GLIB_REQUIRED
	glib-2.0 >= $GLIB_REQUIRED
	gio-2.0 >= $GLIB_REQUIRED
	libgit2 >= $GIT2_REQUIRED
	gobject-introspection-1.0 >= $INTROSPECTION_REQUIRED
])

GOBJECT_INTROSPECTION_REQUIRE($INTROSPECTION_REQUIRED)

AC_SUBST(INTROSPECTION_REQUIRED)
AC_SUBST(GLIB_REQUIRED)
AC_SUBST(GIT2_REQUIRED)

dnl ===========================================================================
dnl Check for python
dnl ===========================================================================
PYGOBJECT_REQUIRED=3.0.0

AC_ARG_ENABLE([python],
              AS_HELP_STRING([--enable-python[=@<:@no/auto/yes@:>@]],[Build with python support]),
              [enable_python=$enableval],
              [enable_python="auto"])

if test "x$enable_python" = "xauto"; then
        PKG_CHECK_EXISTS([pygobject-3.0 >= $PYGOBJECT_REQUIRED],
                         [enable_python=yes],[enable_python=no])
fi

if test "x$enable_python" = "xyes"; then
        PKG_CHECK_MODULES(PYTHON, [pygobject-3.0 >= $PYGOBJECT_REQUIRED])

        pyoverridesdir=`$PYTHON -c "import gi; print(gi._overridesdir)"`
        AC_SUBST(pyoverridesdir)
fi

AM_CONDITIONAL(ENABLE_PYTHON, test x"$enable_python" = "xyes")

GNOME_COMPILE_WARNINGS([maximum])

AC_OUTPUT([
	libgit2-glib-1.0.pc
	Makefile
	docs/Makefile
	docs/reference/Makefile
	docs/reference/version.xml
	examples/Makefile
	libgit2-glib/Makefile
	tests/Makefile
])

