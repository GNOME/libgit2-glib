/*
 * ggit-diff-options.c
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

#include <git2/errors.h>

#include "ggit-diff-options.h"
#include "ggit-utils.h"

struct _GgitDiffOptions
{
	git_diff_options diff_options;
};

G_DEFINE_BOXED_TYPE (GgitDiffOptions, ggit_diff_options,
                     ggit_diff_options_copy, ggit_diff_options_free)

const git_diff_options *
_ggit_diff_options_get_diff_options (GgitDiffOptions *diff_options)
{
	/* NULL is common for diff_options as it specifies to use the default
	 * so handle a NULL diff_options here instead of in every caller.
	 */
	if (diff_options == NULL)
	{
		return NULL;
	}

	return (const git_diff_options *)&diff_options->diff_options;
}

/**
 * ggit_diff_options_copy:
 * @diff_options: a #GgitDiffOptions.
 *
 * Copies @diff_options into a newly allocated #GgitDiffOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitDiffOptions.
 */
GgitDiffOptions *
ggit_diff_options_copy (GgitDiffOptions *diff_options)
{
	GgitDiffOptions *new_diff_options;
	git_diff_options *gdiff_options;
	git_diff_options *gnew_diff_options;

	g_return_val_if_fail (diff_options != NULL, NULL);

	new_diff_options = g_slice_new (GgitDiffOptions);

	gdiff_options = &diff_options->diff_options;
	gnew_diff_options = &new_diff_options->diff_options;

	gnew_diff_options->flags = gdiff_options->flags;
	gnew_diff_options->context_lines = gdiff_options->context_lines;
	gnew_diff_options->interhunk_lines = gdiff_options->interhunk_lines;
	gnew_diff_options->old_prefix = g_strdup (gdiff_options->old_prefix);
	gnew_diff_options->new_prefix = g_strdup (gdiff_options->new_prefix);
	git_strarray_copy (&gnew_diff_options->pathspec,
	                   &gdiff_options->pathspec);

	return new_diff_options;
}

/**
 * ggit_diff_options_free:
 * @diff_options: a #GgitDiffOptions.
 *
 * Frees @diff_options.
 */
void
ggit_diff_options_free (GgitDiffOptions *diff_options)
{
	git_diff_options *gdiff_options;

	g_return_if_fail (diff_options != NULL);

	gdiff_options = &diff_options->diff_options;
	g_free ((gchar *)gdiff_options->old_prefix);
	g_free ((gchar *)gdiff_options->new_prefix);
	git_strarray_free (&gdiff_options->pathspec);

	g_slice_free (GgitDiffOptions, diff_options);
}

/**
 * ggit_diff_options_new:
 * @flags: how the diff should be performed, default is #GGIT_DIFF_NORMAL.
 * @n_context_lines: the number of context lines, default it 3.
 * @n_interhunk_lines: the number of interhunk lines, default is 3.
 * @old_prefix: (allow-none): the old prefix, defaults to "a".
 * @new_prefix: (allow-none): the new prefix, defaults to "b".
 * @pathspec: (allow-none): which paths to show, defaults to showing all paths.
 *
 * Creates a new #GgitDiffOptions for use in creating a #GgitDiff.
 *
 * Returns: a newly allocated #GgitDiffOptions.
 */
GgitDiffOptions *
ggit_diff_options_new (GgitDiffFlags   flags,
                       gint            n_context_lines,
                       gint            n_interhunk_lines,
                       const gchar    *old_prefix,
                       const gchar    *new_prefix,
                       const gchar   **pathspec)
{
	GgitDiffOptions *diff_options;
	git_diff_options *gdiff_options;

	diff_options = g_slice_new (GgitDiffOptions);
	gdiff_options = &diff_options->diff_options;

	gdiff_options->flags = flags;
	gdiff_options->context_lines = n_context_lines;
	gdiff_options->interhunk_lines = n_interhunk_lines;
	gdiff_options->old_prefix = old_prefix == NULL ? NULL : g_strdup (old_prefix);
	gdiff_options->new_prefix = new_prefix == NULL ? NULL : g_strdup (new_prefix);
	ggit_utils_get_git_strarray_from_str_array (pathspec,
	                                            &gdiff_options->pathspec);

	return diff_options;
}

/* ex:set ts=8 noet: */
