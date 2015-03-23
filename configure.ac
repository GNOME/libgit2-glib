AC_PREREQ([2.63])

m4_define(libgit2_glib_major_version, 0)
m4_define(libgit2_glib_minor_version, 22)
m4_define(libgit2_glib_micro_version, 3)
m4_define(libgit2_glib_version, libgit2_glib_major_version.libgit2_glib_minor_version.libgit2_glib_micro_version)

AC_INIT([libgit2-glib],
        [libgit2_glib_version],
        [http://bugzilla.gnome.org/enter_bug.cgi?product=libgit2-glib],
        [libgit2-glib])

# Libtool versioning
#
# For development releases, keep the same numbers.
#
# For stable releases, apply the following algorithm step by step:
# 1. If the library source code has changed at all since the last
#    update, then increment REVISION.
# 2. If any exported functions or data have been added, removed, or
#    changed since the last update, increment CURRENT and set REVISION
#    to 0.
# 3. If any exported functions or data have been added since the last
#    public release, increment AGE.
# 4. If any exported functions or data have been removed since the last
#    public release, set AGE to 0.

LIBGIT2_GLIB_LT_CURRENT=0
LIBGIT2_GLIB_LT_REVISION=0
LIBGIT2_GLIB_LT_AGE=0
LIBGIT2_GLIB_LT_VERSION="$LIBGIT2_GLIB_LT_CURRENT:$LIBGIT2_GLIB_LT_REVISION:$LIBGIT2_GLIB_LT_AGE"
AC_SUBST(LIBGIT2_GLIB_LT_VERSION)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([libgit2-glib/ggit-repository.c])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 tar-ustar dist-xz no-dist-gzip])
AM_MAINTAINER_MODE([enable])

AM_SILENT_RULES([yes])

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
GIT2_REQUIRED=0.21.0

PKG_CHECK_MODULES(LIBGIT2_GLIB, [
	gobject-2.0 >= $GLIB_REQUIRED
	glib-2.0 >= $GLIB_REQUIRED
	gio-2.0 >= $GLIB_REQUIRED
	libgit2 >= $GIT2_REQUIRED
	gobject-introspection-1.0 >= $INTROSPECTION_REQUIRED
])

LIBGIT2_INCLUDE_DIR=$($PKG_CONFIG --variable=includedir libgit2)
LIBGIT2_GLIB_CFLAGS="${LIBGIT2_GLIB_CFLAGS} -I$LIBGIT2_INCLUDE_DIR"

GOBJECT_INTROSPECTION_REQUIRE($INTROSPECTION_REQUIRED)

AC_SUBST(INTROSPECTION_REQUIRED)
AC_SUBST(GLIB_REQUIRED)
AC_SUBST(GIT2_REQUIRED)

dnl ===========================================================================
dnl Check for libgit2 ssh support
dnl ===========================================================================

AC_ARG_ENABLE([ssh],
              AS_HELP_STRING([--enable-ssh[=@<:@no/auto/yes@:>@]],[Build with libgit2 ssh support]),
              [enable_ssh=$enableval],
              [enable_ssh="auto"])

git_ssh=no

if test "x$enable_ssh" != "xno"; then
    AC_MSG_CHECKING([for libgit2 ssh support])

    cflags_save="${CFLAGS}"
    libs_save="${LIBS}"

    CFLAGS="${LIBGIT2_GLIB_CFLAGS}"
    LIBS="${LIBGIT2_GLIB_LIBS}"

    AC_TRY_RUN([
            #include <git2.h>
            int
            main(int argc, const char *argv[])
            {
                    git_libgit2_init ();
                    return ((git_libgit2_features() & GIT_FEATURE_SSH) != 0) ? 0 : 1;
            }
    ],[
        AC_MSG_RESULT([yes])
        git_ssh=yes
    ],[
        AC_MSG_RESULT([no])
        git_ssh=no
        if test "x$enable_ssh" = "xyes"; then
            AC_MSG_ERROR([libgit2 ssh support was requested, but not found])
        fi
    ])

    CFLAGS="${cflags_save}"
    LIBS="${libs_save}"
fi

if test "x$git_ssh" = "xyes"; then
	LIBGIT2_GLIB_CFLAGS="${LIBGIT2_GLIB_CFLAGS} -DGIT_SSH=1"
	LIBGIT2_GLIB_PCCFLAGS="-DGIT_SSH=1"

    GGIT_SSH_INCLUDES="
#include <libgit2-glib/ggit-cred-ssh-key-from-agent.h>
#include <libgit2-glib/ggit-cred-ssh-interactive.h>
"
fi

AC_SUBST(GGIT_SSH_INCLUDES)
AC_SUBST(LIBGIT2_GLIB_PCCFLAGS)

AM_CONDITIONAL(GIT_SSH, test x"$git_ssh" = "xyes")

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
        AM_PATH_PYTHON(3.2.3)

        PKG_CHECK_MODULES(PYTHON, [pygobject-3.0 >= $PYGOBJECT_REQUIRED])

        pyoverridesdir="\$(pyexecdir)/gi/overrides"
        AC_SUBST(pyoverridesdir)
fi

AM_CONDITIONAL(ENABLE_PYTHON, test x"$enable_python" = "xyes")

# Vala
VAPIGEN_CHECK

AC_CHECK_HEADER([termios.h],
                [has_termios=yes],
                [has_termios=no])

AM_CONDITIONAL(HAS_TERMIOS, test x"$has_termios" = "xyes")

GNOME_COMPILE_WARNINGS([maximum])

AC_OUTPUT([
	libgit2-glib-1.0.pc
	Makefile
	docs/Makefile
	docs/reference/Makefile
	docs/reference/version.xml
	examples/Makefile
	libgit2-glib/Makefile
	libgit2-glib/ggit.h
	tests/Makefile
])

if test "x$git_ssh" != "xyes"; then
echo "*** libgit2-glib will be built without SSH support! ***"
fi
