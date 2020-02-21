/*
 * ggit-error.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libgit2-glib. If not, see <http://www.gnu.org/licenses/>.
 */


#include <errno.h>
#include <git2.h>

#include "ggit-error.h"

/**
 * ggit_error_quark:
 *
 * Error domain for Ggit functions.
 * Errors in this domain will be from the GgitError enumeration.
 * See #GError for information on error domains.
 */
GQuark
ggit_error_quark (void)
{
	static GQuark quark = 0;

	if (G_UNLIKELY (quark == 0))
		quark = g_quark_from_static_string ("ggit-error");

	return quark;
}

void
_ggit_error_set (GError **error,
                 gint     err)
{
	const git_error *git2_err;

	g_return_if_fail (err < 0);

	/* TODO: add more kind of errors, see git_error_t */
#if LIBGIT2_VER_MAJOR > 0 || (LIBGIT2_VER_MAJOR == 0 && LIBGIT2_VER_MINOR >= 28)
	git2_err = git_error_last ();
#else
	git2_err = giterr_last ();
#endif
	g_set_error_literal (error, GGIT_ERROR,
	                     err,
	                     git2_err == NULL ? "" : git2_err->message);
}

/* ex:set ts=8 noet: */
