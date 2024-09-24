/*
 * ggit-main.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
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

#include <git2.h>

#include "ggit-main.h"
#include "ggit-utils.h"
#include "ggit-error.h"

/**
 * ggit_set_extensions:
 * @extensions: (array zero-terminated=1) (allow-none): the ref specs.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Set that the given git extensions are supported by the caller.
 * Extensions supported by libgit2 may be negated by prefixing
 * them with a `!`.  For example: setting extensions to
 * { "!noop", "newext" } indicates that the caller does not want
 * to support repositories with the `noop` extension but does want
 * to support repositories with the `newext` extension.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 */
gboolean
ggit_set_extensions (const gchar * const *extensions, GError **error)
{
	gint ret;
	git_strarray gextensions;

	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ggit_utils_get_git_strarray_from_str_array (extensions, &gextensions);

	ret = git_libgit2_opts (GIT_OPT_SET_EXTENSIONS, gextensions.strings, gextensions.count);

	git_strarray_free (&gextensions);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_get_features:
 *
 * Gets the features of the library.
 *
 * Returns: a #GgitFeatureFlags.
 */
GgitFeatureFlags
ggit_get_features (void)
{
	return git_libgit2_features ();
}

/**
 * ggit_init:
 *
 * Call this function before using any other libgit2-glib function.
 */
void
ggit_init (void)
{
	git_libgit2_init ();
}

/* ex:set ts=8 noet: */
