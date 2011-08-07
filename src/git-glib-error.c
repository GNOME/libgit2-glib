/*
 * git-glib-error.c
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


#include "git-glib-error.h"

#include <errno.h>
#include <git2/errors.h>

GQuark
git_glib_error_quark (void)
{
	static GQuark quark = 0;

	if (G_UNLIKELY (quark == 0))
		quark = g_quark_from_static_string ("git-glib-error");

	return quark;
}

void
_git_glib_error_set (GError **error,
                     gint     err)
{
	g_return_if_fail (err < 0);

	if (err == GIT_GLIB_ERROR_NOTFOUND)
	{
		return;
	}
	else if (err == GIT_GLIB_ERROR_OSERR)
	{
		g_set_error_literal (error, GIT_GLIB_ERROR,
		                     err,
		                     g_strerror (errno));
	}
	else
	{
		g_set_error_literal (error, GIT_GLIB_ERROR,
		                     err,
		                     git_lasterror ());
	}
}
