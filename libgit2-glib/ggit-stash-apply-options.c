/*
 * ggit-stash_apply-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2025 - Alberto Fanjul
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

#include "ggit-stash-apply-options.h"
#include "ggit-oid.h"

struct _GgitStashApplyOptions
{
	git_stash_apply_options stash_apply_options;
};

G_DEFINE_BOXED_TYPE (GgitStashApplyOptions, ggit_stash_apply_options,
                     ggit_stash_apply_options_copy, ggit_stash_apply_options_free);

git_stash_apply_options *
_ggit_stash_apply_options_get_native (GgitStashApplyOptions *stash_apply_options)
{
	/* NULL is common for stash_apply_options as it specifies to use the default
	 * so handle a NULL stash_apply_options here instead of in every caller.
	 */
	if (stash_apply_options == NULL)
	{
		return NULL;
	}

	return &stash_apply_options->stash_apply_options;
}

/**
 * ggit_stash_apply_options_copy:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Copies @stash_apply_options into a newly allocated #GgitStashApplyOptions.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitStashApplyOptions or %NULL.
 */
GgitStashApplyOptions *
ggit_stash_apply_options_copy (GgitStashApplyOptions *stash_apply_options)
{
	GgitStashApplyOptions *ret;

	g_return_val_if_fail (stash_apply_options != NULL, NULL);

	ret = g_slice_new (GgitStashApplyOptions);
	ret->stash_apply_options = stash_apply_options->stash_apply_options;

	return ret;
}

/**
 * ggit_stash_apply_options_free:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Frees @stash_apply_options.
 */
void
ggit_stash_apply_options_free (GgitStashApplyOptions *stash_apply_options)
{
	g_return_if_fail (stash_apply_options != NULL);

	g_slice_free (GgitStashApplyOptions, stash_apply_options);
}

/**
 * ggit_stash_apply_options_new:
 *
 * Create a new, empty #GgitStashApplyOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitStashApplyOptions.
 */
GgitStashApplyOptions *
ggit_stash_apply_options_new (void)
{
	GgitStashApplyOptions *ret;
	git_stash_apply_options gstash_apply_options = GIT_STASH_APPLY_OPTIONS_INIT;

	ret = g_slice_new (GgitStashApplyOptions);
	ret->stash_apply_options = gstash_apply_options;

	return ret;
}

/**
 * ggit_stash_apply_get_flags:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Get the stash_apply options flags.
 *
 * Returns: a #GgitStashApplyFlags.
 *
 **/
GgitStashApplyFlags
ggit_stash_apply_get_flags (GgitStashApplyOptions *stash_apply_options)
{
	g_return_val_if_fail (stash_apply_options != NULL, GGIT_STASH_APPLY_DEFAULT);

	return (GgitStashApplyFlags)stash_apply_options->stash_apply_options.flags;
}

/**
 * ggit_stash_apply_set_flags:
 * @stash_apply_options: a #GgitStashApplyOptions.
 * @flags: a #GgitStashApplyFlags.
 *
 * Set the stash_apply options flags.
 *
 **/
void
ggit_stash_apply_set_flags (GgitStashApplyOptions *stash_apply_options,
                      GgitStashApplyFlags    flags)
{
	g_return_if_fail (stash_apply_options != NULL);

	stash_apply_options->stash_apply_options.flags = (uint32_t)flags;
}

/**
 * ggit_stash_apply_options_get_newest_commit:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Get the id of the newest commit to consider in the stash_apply. The default
 * value of %NULL indicates to use HEAD.
 *
 * Returns: (transfer full) (nullable): a #GgitOId or %NULL.
 *
 **/
GgitOId *
ggit_stash_apply_options_get_newest_commit (GgitStashApplyOptions *stash_apply_options)
{
	g_return_val_if_fail (stash_apply_options != NULL, NULL);

	if (git_oid_iszero (&stash_apply_options->stash_apply_options.newest_commit))
	{
		return NULL;
	}

	return _ggit_oid_wrap (&stash_apply_options->stash_apply_options.newest_commit);
}

/**
 * ggit_stash_apply_options_set_newest_commit:
 * @stash_apply_options: a #GgitStashApplyOptions.
 * @oid: (allow-none): a #GgitOId or %NULL.
 *
 * Set the id of the newest commit to consider in the stash_apply. Specify %NULL to
 * set the default value which indicates to use HEAD.
 *
 **/
void
ggit_stash_apply_options_set_newest_commit (GgitStashApplyOptions *stash_apply_options,
                                      GgitOId          *oid)
{
	git_oid zero = {{0,}};

	g_return_if_fail (stash_apply_options != NULL);

	if (oid == NULL)
	{
		stash_apply_options->stash_apply_options.newest_commit = zero;
	}
	else
	{
		stash_apply_options->stash_apply_options.newest_commit = *_ggit_oid_get_oid (oid);
	}
}

/**
 * ggit_stash_apply_options_get_oldest_commit:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Get the id of the oldest commit to consider in the stash_apply. Teh default value
 * of %NULL indicates to used HEAD.
 *
 * Returns: (transfer full) (nullable): a #GgitOId or %NULL.
 *
 **/
GgitOId *
ggit_stash_apply_options_get_oldest_commit (GgitStashApplyOptions *stash_apply_options)
{
	g_return_val_if_fail (stash_apply_options != NULL, NULL);

	if (git_oid_iszero (&stash_apply_options->stash_apply_options.oldest_commit))
	{
		return NULL;
	}

	return _ggit_oid_wrap (&stash_apply_options->stash_apply_options.oldest_commit);
}

/**
 * ggit_stash_apply_options_set_oldest_commit:
 * @stash_apply_options: a #GgitStashApplyOptions.
 * @oid: (allow-none): a #GgitOId.
 *
 * Set the id of the oldest commit to consider in the stash_apply. Specify %NULL to
 * set the default value which indicates to consider the first commit without
 * a parent.
 *
 **/
void
ggit_stash_apply_options_set_oldest_commit (GgitStashApplyOptions *stash_apply_options,
                                      GgitOId          *oid)
{
	git_oid zero = {{0,}};

	g_return_if_fail (stash_apply_options != NULL);

	if (oid == NULL)
	{
		stash_apply_options->stash_apply_options.oldest_commit = zero;
	}
	else
	{
		stash_apply_options->stash_apply_options.oldest_commit = *_ggit_oid_get_oid (oid);
	}
}

/**
 * ggit_stash_apply_options_get_minimum_line:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Get the first line in the file to consider. The default is 1.
 *
 * Returns: the first line to consider.
 *
 **/
guint32
ggit_stash_apply_options_get_minimum_line (GgitStashApplyOptions *stash_apply_options)
{
	g_return_val_if_fail (stash_apply_options != NULL, 0);

	if (stash_apply_options->stash_apply_options.min_line == 0)
	{
		return 1;
	}

	return stash_apply_options->stash_apply_options.min_line;
}

/**
 * ggit_stash_apply_options_set_minimum_line:
 * @stash_apply_options: a #GgitStashApplyOptions.
 * @line: the first line to consider.
 *
 * Set the first line in the file to consider. Lines start at 1.
 *
 **/
void
ggitstash_applye_options_set_minimum_line (GgitStashApplyOptions *stash_apply_options,
                                     guint32           line)
{
	g_return_if_fail (stash_apply_options != NULL);

	stash_apply_options->stash_apply_options.min_line = line;
}

/**
 * ggit_stash_apply_options_get_maximum_line:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Get the last line in the file to consider. The default is 1.
 *
 * Returns: the last line to consider.
 *
 **/
guint32
ggit_stash_apply_options_get_maximum_line (GgitStashApplyOptions *stash_apply_options)
{
	g_return_val_if_fail (stash_apply_options != NULL, 0);

	if (stash_apply_options->stash_apply_options.max_line == 0)
	{
		return 1;
	}

	return stash_apply_options->stash_apply_options.max_line;
}

/**
 * ggit_stash_apply_options_set_maximum_line:
 * @stash_apply_options: a #GgitStashApplyOptions.
 * @line: the last line to consider.
 *
 * Set the last line in the file to consider. Lines start at 1.
 *
 **/

void
ggit_stash_apply_options_set_maximum_line (GgitStashApplyOptions *stash_apply_options,
                                     guint32           line)
{
	g_return_if_fail (stash_apply_options != NULL);

	stash_apply_options->stash_apply_options.max_line = line;
}

/**
 * ggit_stash_apply_options_get_minimum_match_characters:
 * @stash_apply_options: a #GgitStashApplyOptions.
 *
 * Get the minimum number of characters that must be detected as moving/copying
 * within a file for it to associate those lines with a parent commit. This is
 * only used when any of the #GGIT_STASH_APPLY_TRACK_COPIES_SAME_FILE flag is
 * specified. The default value is 20.
 *
 * Returns: the minimum number of characters.
 *
 **/
guint16
ggit_stash_apply_options_get_minimum_match_characters (GgitStashApplyOptions *stash_apply_options)
{
	g_return_val_if_fail (stash_apply_options != NULL, 0);

	if (stash_apply_options->stash_apply_options.min_match_characters == 0)
	{
		return 20;
	}

	return stash_apply_options->stash_apply_options.min_match_characters;
}

/**
 * ggit_stash_apply_options_set_minimum_match_characters:
 * @stash_apply_options: a #GgitStashApplyOptions.
 * @characters: the minimum number of characters.
 *
 * Set the minimum number of characters that must be detected as moving/copying
 * within a file for it to associate those lines with a parent commit. This is
 * only used when any of the #GGIT_STASH_APPLY_TRACK_COPIES_ flags are specified. The
 * default value is 20.
 *
 **/
void
ggit_stash_apply_options_set_minimum_match_characters (GgitStashApplyOptions *stash_apply_options,
                                                 guint16           characters)
{
	g_return_if_fail (stash_apply_options != NULL);
	stash_apply_options->stash_apply_options.min_match_characters = characters;
}

