/*
 * ggit-error.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#include <errno.h>
#include <git2/errors.h>

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
	g_return_if_fail (err < 0);

	if (err == GGIT_ERROR_NOTFOUND)
	{
		return;
	}
	else if (err == GGIT_ERROR_OSERR)
	{
		g_set_error_literal (error, GGIT_ERROR,
		                     err,
		                     g_strerror (errno));
	}
	else
	{
		g_set_error_literal (error, GGIT_ERROR,
		                     err,
		                     git_lasterror ());
	}
}
