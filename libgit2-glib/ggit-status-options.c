/*
 * ggit-status-options.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
 * Copyright (C) 2013 - Jesse van den Kieboom
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

#include "ggit-status-options.h"
#include "ggit-utils.h"

struct _GgitStatusOptions
{
	git_status_options status_options;
};

G_DEFINE_BOXED_TYPE (GgitStatusOptions, ggit_status_options,
                     ggit_status_options_copy, ggit_status_options_free)

const git_status_options *
_ggit_status_options_get_status_options (GgitStatusOptions *status_options)
{
	/* NULL is common for status_options as it specifies to use the default
	 * so handle a NULL status_options here instead of in every caller.
	 */
	if (status_options == NULL)
	{
		return NULL;
	}

	return (const git_status_options *)&status_options->status_options;
}

/**
 * ggit_status_options_copy:
 * @status_options: a #GgitStatusOptions.
 *
 * Copies @status_options into a newly allocated #GgitStatusOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitStatusOptions.
 */
GgitStatusOptions *
ggit_status_options_copy (GgitStatusOptions *status_options)
{
	GgitStatusOptions *new_status_options;

	g_return_val_if_fail (status_options != NULL, NULL);

	new_status_options = g_slice_new (GgitStatusOptions);

	new_status_options->status_options = status_options->status_options;

	git_strarray_copy (&new_status_options->status_options.pathspec,
	                   &status_options->status_options.pathspec);

	return new_status_options;
}

/**
 * ggit_status_options_free:
 * @status_options: a #GgitStatusOptions.
 *
 * Frees @status_options.
 */
void
ggit_status_options_free (GgitStatusOptions *status_options)
{
	git_status_options *gstatus_options;

	g_return_if_fail (status_options != NULL);

	gstatus_options = &status_options->status_options;
	git_strarray_free (&gstatus_options->pathspec);

	g_slice_free (GgitStatusOptions, status_options);
}

/**
 * ggit_status_options_new:
 * @options: status options.
 * @show: status show options.
 * @pathspec: (allow-none): which paths to show, defaults to showing all paths.
 *
 * Creates a new #GgitStatusOptions for use in #ggit_repository_stash_foreach.
 *
 * Returns: a newly allocated #GgitStatusOptions.
 */
GgitStatusOptions *
ggit_status_options_new (GgitStatusOption   options,
                         GgitStatusShow     show,
                         const gchar      **pathspec)
{
	GgitStatusOptions *status_options;
	git_status_options gstatus_options = GIT_STATUS_OPTIONS_INIT;

	status_options = g_slice_new (GgitStatusOptions);

	gstatus_options.flags = options;
	gstatus_options.show = show;

	ggit_utils_get_git_strarray_from_str_array (pathspec,
	                                            &gstatus_options.pathspec);

	status_options->status_options = gstatus_options;

	return status_options;
}

/* ex:set ts=8 noet: */
