/*
 * ggit-blame-options.h
 * This file is part of libgit2-glib
 *
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

#include "ggit-blame-options.h"
#include "ggit-oid.h"

struct _GgitBlameOptions
{
	git_blame_options blame_options;
};

G_DEFINE_BOXED_TYPE (GgitBlameOptions, ggit_blame_options,
                     ggit_blame_options_copy, ggit_blame_options_free);

const git_blame_options *
_ggit_blame_options_get_blame_options (GgitBlameOptions *blame_options)
{
	/* NULL is common for blame_options as it specifies to use the default
	 * so handle a NULL blame_options here instead of in every caller.
	 */
	if (blame_options == NULL)
	{
		return NULL;
	}

	return (const git_blame_options *)&blame_options->blame_options;
}

/**
 * ggit_blame_options_copy:
 * @blame_options: a #GgitBlameOptions.
 *
 * Copies @blame_options into a newly allocated #GgitBlameOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitBlameOptions.
 */
GgitBlameOptions *
ggit_blame_options_copy (GgitBlameOptions *blame_options)
{
	GgitBlameOptions *ret;

	g_return_val_if_fail (blame_options != NULL, NULL);

	ret = g_slice_new (GgitBlameOptions);
	ret->blame_options = blame_options->blame_options;

	return ret;
}

/**
 * ggit_blame_options_free:
 * @blame_options: a #GgitBlameOptions.
 *
 * Frees @blame_options.
 */
void
ggit_blame_options_free (GgitBlameOptions *blame_options)
{
	g_return_if_fail (blame_options != NULL);

	g_slice_free (GgitBlameOptions, blame_options);
}

/**
 * ggit_blame_options_new:
 *
 * Create a new, empty #GgitBlameOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitBlameOptions.
 */
GgitBlameOptions *
ggit_blame_options_new (void)
{
	GgitBlameOptions *ret;
	git_blame_options gblame_options = GIT_BLAME_OPTIONS_INIT;

	ret = g_slice_new (GgitBlameOptions);
	ret->blame_options = gblame_options;

	return ret;
}

/**
 * ggit_blame_get_flags:
 * @blame_options: a #GgitBlameOptions.
 *
 * Get the blame options flags.
 *
 * Returns: a #GgitBlameFlags.
 *
 **/
GgitBlameFlags
ggit_blame_get_flags (GgitBlameOptions *blame_options)
{
	g_return_val_if_fail (blame_options != NULL, GGIT_BLAME_NORMAL);

	return (GgitBlameFlags)blame_options->blame_options.flags;
}

/**
 * ggit_blame_set_flags:
 * @blame_options: a #GgitBlameOptions.
 * @flags: a #GgitBlameFlags.
 *
 * Set the blame options flags.
 *
 **/
void
ggit_blame_set_flags (GgitBlameOptions *blame_options,
                      GgitBlameFlags    flags)
{
	g_return_if_fail (blame_options != NULL);

	blame_options->blame_options.flags = (uint32_t)flags;
}

/**
 * ggit_blame_options_get_newest_commit:
 * @blame_options: a #GgitBlameOptions.
 *
 * Get the id of the newest commit to consider in the blame. The default
 * value of %NULL indicates to use HEAD.
 *
 * Returns: (transfer full): a #GgitOId or %NULL.
 *
 **/
GgitOId *
ggit_blame_options_get_newest_commit (GgitBlameOptions *blame_options)
{
	g_return_val_if_fail (blame_options != NULL, NULL);

	if (git_oid_iszero (&blame_options->blame_options.newest_commit))
	{
		return NULL;
	}

	return _ggit_oid_wrap (&blame_options->blame_options.newest_commit);
}

/**
 * ggit_blame_options_set_newest_commit:
 * @blame_options: a #GgitBlameOptions.
 * @oid: (allow-none): a #GgitOId or %NULL.
 *
 * Set the id of the newest commit to consider in the blame. Specify %NULL to
 * set the default value which indicates to use HEAD.
 *
 **/
void
ggit_blame_options_set_newest_commit (GgitBlameOptions *blame_options,
                                      GgitOId          *oid)
{
	git_oid zero = {{0,}};

	g_return_if_fail (blame_options != NULL);

	if (oid == NULL)
	{
		blame_options->blame_options.newest_commit = zero;
	}

	blame_options->blame_options.newest_commit = *_ggit_oid_get_oid (oid);
}

/**
 * ggit_blame_options_get_oldest_commit:
 * @blame_options: a #GgitBlameOptions.
 *
 * Get the id of the oldest commit to consider in the blame. Teh default value
 * of %NULL indicates to used HEAD.
 *
 * Returns: (transfer full): a #GgitOId.
 *
 **/
GgitOId *
ggit_blame_options_get_oldest_commit (GgitBlameOptions *blame_options)
{
	g_return_val_if_fail (blame_options != NULL, NULL);

	if (git_oid_iszero (&blame_options->blame_options.oldest_commit))
	{
		return NULL;
	}

	return _ggit_oid_wrap (&blame_options->blame_options.oldest_commit);
}

/**
 * ggit_blame_options_set_oldest_commit:
 * @blame_options: a #GgitBlameOptions.
 * @oid: (allow-none): a #GgitOId.
 *
 * Set the id of the oldest commit to consider in the blame. Specify %NULL to
 * set the default value which indicates to consider the first commit without
 * a parent.
 *
 **/
void
ggit_blame_options_set_oldest_commit (GgitBlameOptions *blame_options,
                                      GgitOId          *oid)
{
	git_oid zero = {{0,}};

	g_return_if_fail (blame_options != NULL);

	if (oid == NULL)
	{
		blame_options->blame_options.oldest_commit = zero;
	}

	blame_options->blame_options.oldest_commit = *_ggit_oid_get_oid (oid);
}

/**
 * ggit_blame_options_get_minimum_line:
 * @blame_options: a #GgitBlameOptions.
 *
 * Get the first line in the file to consider. The default is 1.
 *
 * Returns: the first line to consider.
 *
 **/
guint32
ggit_blame_options_get_minimum_line (GgitBlameOptions *blame_options)
{
	g_return_val_if_fail (blame_options != NULL, 0);

	if (blame_options->blame_options.min_line == 0)
	{
		return 1;
	}

	return blame_options->blame_options.min_line;
}

/**
 * ggit_blame_options_set_minimum_line:
 * @blame_options: a #GgitBlameOptions.
 * @line: the first line to consider.
 *
 * Set the first line in the file to consider. Lines start at 1.
 *
 **/
void
ggit_blame_options_set_minimum_line (GgitBlameOptions *blame_options,
                                     guint32           line)
{
	g_return_if_fail (blame_options != NULL);

	blame_options->blame_options.min_line = line;
}

/**
 * ggit_blame_options_get_maximum_line:
 * @blame_options: a #GgitBlameOptions.
 *
 * Get the last line in the file to consider. The default is 1.
 *
 * Returns: the last line to consider.
 *
 **/
guint32
ggit_blame_options_get_maximum_line (GgitBlameOptions *blame_options)
{
	g_return_val_if_fail (blame_options != NULL, 0);

	if (blame_options->blame_options.max_line == 0)
	{
		return 1;
	}

	return blame_options->blame_options.max_line;
}

/**
 * ggit_blame_options_set_maximum_line:
 * @blame_options: a #GgitBlameOptions.
 * @line: the last line to consider.
 *
 * Set the last line in the file to consider. Lines start at 1.
 *
 **/

void
ggit_blame_options_set_maximum_line (GgitBlameOptions *blame_options,
                                     guint32           line)
{
	g_return_if_fail (blame_options != NULL);

	blame_options->blame_options.max_line = line;
}

/**
 * ggit_blame_options_get_minimum_match_characters:
 * @blame_options: a #GgitBlameOptions.
 *
 * Get the minimum number of characters that must be detected as moving/copying
 * within a file for it to associate those lines with a parent commit. This is
 * only used when any of the #GGIT_BLAME_TRACK_COPIES_SAME_FILE flag is
 * specified. The default value is 20.
 *
 * Returns: the minimum number of characters.
 *
 **/
guint16
ggit_blame_options_get_minimum_match_characters (GgitBlameOptions *blame_options)
{
	g_return_val_if_fail (blame_options != NULL, 0);

	if (blame_options->blame_options.min_match_characters == 0)
	{
		return 20;
	}

	return blame_options->blame_options.min_match_characters;
}

/**
 * ggit_blame_options_set_minimum_match_characters:
 * @blame_options: a #GgitBlameOptions.
 * @characters: the minimum number of characters.
 *
 * Set the minimum number of characters that must be detected as moving/copying
 * within a file for it to associate those lines with a parent commit. This is
 * only used when any of the #GGIT_BLAME_TRACK_COPIES_ flags are specified. The
 * default value is 20.
 *
 **/
void
ggit_blame_options_set_minimum_match_characters (GgitBlameOptions *blame_options,
                                                 guint16           characters)
{
	g_return_if_fail (blame_options != NULL);
	blame_options->blame_options.min_match_characters = characters;
}

