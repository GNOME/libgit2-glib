/*
 * ggit-diff-hunk.c
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

#include <glib-object.h>
#include <git2.h>
#include <string.h>

#include "ggit-diff-hunk.h"

struct _GgitDiffHunk {
	gint ref_count;

	gint old_start;
	gint old_lines;
	gint new_start;
	gint new_lines;
	gsize header_len;
	gchar header[128];
};

G_DEFINE_BOXED_TYPE (GgitDiffHunk, ggit_diff_hunk,
                     ggit_diff_hunk_ref, ggit_diff_hunk_unref)


GgitDiffHunk *
_ggit_diff_hunk_wrap (const git_diff_hunk *hunk)
{
	GgitDiffHunk *ghunk;

	g_return_val_if_fail (hunk != NULL, NULL);

	ghunk = g_slice_new (GgitDiffHunk);
	ghunk->ref_count = 1;
	ghunk->old_start = hunk->old_start;
	ghunk->old_lines = hunk->old_lines;
	ghunk->new_start = hunk->new_start;
	ghunk->new_lines = hunk->new_lines;
	ghunk->header_len = hunk->header_len;

	strncpy(ghunk->header, hunk->header, hunk->header_len);

	return ghunk;
}

/**
 * ggit_diff_hunk_ref:
 * @hunk: a #GgitDiffHunk.
 *
 * Atomically increments the reference count of @hunk by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitDiffHunk.
 **/
GgitDiffHunk *
ggit_diff_hunk_ref (GgitDiffHunk *hunk)
{
	g_return_val_if_fail (hunk != NULL, NULL);

	g_atomic_int_inc (&hunk->ref_count);

	return hunk;
}

/**
 * ggit_diff_hunk_unref:
 * @hunk: a #GgitDiffHunk.
 *
 * Atomically decrements the reference count of @hunk by one.
 * If the reference count drops to 0, @hunk is freed.
 **/
void
ggit_diff_hunk_unref (GgitDiffHunk *hunk)
{
	g_return_if_fail (hunk != NULL);

	if (g_atomic_int_dec_and_test (&hunk->ref_count))
	{
		g_slice_free (GgitDiffHunk, hunk);
	}
}

/**
 * ggit_diff_hunk_get_old_start:
 * @hunk: a #GgitDiffHunk.
 *
 * Gets the starting line number in the old file.
 *
 * Returns: the starting line number in the old file.
 */
gchar
ggit_diff_hunk_get_old_start (GgitDiffHunk *hunk)
{
	g_return_val_if_fail (hunk != NULL, 0);

	return hunk->old_start;
}

/**
 * ggit_diff_hunk_get_old_lines:
 * @hunk: a #GgitDiffHunk.
 *
 * Gets the number of lines in the old file.
 *
 * Returns: the number of lines in the old file.
 */
gint
ggit_diff_hunk_get_old_lines (GgitDiffHunk *hunk)
{
	g_return_val_if_fail (hunk != NULL, 0);

	return hunk->old_lines;
}

/**
 * ggit_diff_hunk_get_new_start:
 * @hunk: a #GgitDiffHunk.
 *
 * Gets the starting line number in the new file.
 *
 * Returns: the starting line number in the new file.
 */
gint
ggit_diff_hunk_get_new_start (GgitDiffHunk *hunk)
{
	g_return_val_if_fail (hunk != NULL, 0);

	return hunk->new_start;
}

/**
 * ggit_diff_hunk_get_new_lines:
 * @hunk: a #GgitDiffHunk.
 *
 * Gets the number of lines in the new file.
 *
 * Returns: the number of lines in the new file.
 */
gint
ggit_diff_hunk_get_new_lines (GgitDiffHunk *hunk)
{
	g_return_val_if_fail (hunk != NULL, 0);

	return hunk->new_lines;
}

/**
 * ggit_diff_hunk_get_header_len:
 * @hunk: a #GgitDiffHunk.
 *
 * Gets the header length.
 *
 * Returns: the header length.
 */
gsize
ggit_diff_hunk_get_header_len (GgitDiffHunk *hunk)
{
	g_return_val_if_fail (hunk != NULL, 0);

	return hunk->header_len;
}

/**
 * ggit_diff_hunk_get_content:
 * @hunk: a #GgitDiffHunk.
 *
 * Gets the header.
 *
 * Returns: the header.
 */
const gchar *
ggit_diff_hunk_get_header (GgitDiffHunk *hunk)
{
	g_return_val_if_fail (hunk != NULL, 0);

	return hunk->header;
}

/* ex:set ts=8 noet: */
