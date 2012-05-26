/*
 * ggit-diff-range.c
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
#include <git2/diff.h>

#include "ggit-diff-range.h"

struct _GgitDiffRange {
	gint ref_count;

	gint old_start;
	gint old_lines;
	gint new_start;
	gint new_lines;
};

G_DEFINE_BOXED_TYPE (GgitDiffRange, ggit_diff_range,
                     ggit_diff_range_ref, ggit_diff_range_unref)


GgitDiffRange *
_ggit_diff_range_wrap (git_diff_range *range)
{
	GgitDiffRange *grange;

	g_return_val_if_fail (range != NULL, NULL);

	grange = g_slice_new (GgitDiffRange);
	grange->ref_count = 1;
	grange->old_start = range->old_start;
	grange->old_lines = range->old_lines;
	grange->new_start = range->new_start;
	grange->new_lines = range->new_lines;

	return grange;
}

/**
 * ggit_diff_range_ref:
 * @range: a #GgitDiffRange.
 *
 * Atomically increments the reference count of @range by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitDiffRange.
 **/
GgitDiffRange *
ggit_diff_range_ref (GgitDiffRange *range)
{
	g_return_val_if_fail (range != NULL, NULL);

	g_atomic_int_inc (&range->ref_count);

	return range;
}

/**
 * ggit_diff_range_unref:
 * @range: a #GgitDiffRange.
 *
 * Atomically decrements the reference count of @range by one.
 * If the reference count drops to 0, @range is freed.
 **/
void
ggit_diff_range_unref (GgitDiffRange *range)
{
	g_return_if_fail (range != NULL);

	if (g_atomic_int_dec_and_test (&range->ref_count))
	{
		g_slice_free (GgitDiffRange, range);
	}
}

/**
 * ggit_diff_range_get_old_start:
 * @range: a #GgitDiffRange.
 *
 * Gets the old starting line number for @range.
 *
 * Returns: the range's old starting line.
 */
gint
ggit_diff_range_get_old_start (GgitDiffRange *range)
{
	g_return_val_if_fail (range != NULL, 0);

	return range->old_start;
}

/**
 * ggit_diff_range_get_old_lines:
 * @range: a #GgitDiffRange.
 *
 * Gets the old number of lines for @range.
 *
 * Returns: the range's old number of lines.
 */
gint
ggit_diff_range_get_old_lines (GgitDiffRange *range)
{
	g_return_val_if_fail (range != NULL, 0);

	return range->old_lines;
}

/**
 * ggit_diff_range_get_new_start:
 * @range: a #GgitDiffRange.
 *
 * Gets the new starting line number for @range.
 *
 * Returns: the range's new starting line number.
 */
gint
ggit_diff_range_get_new_start (GgitDiffRange *range)
{
	g_return_val_if_fail (range != NULL, 0);

	return range->new_start;
}

/**
 * ggit_diff_range_get_new_lines:
 * @range: a #GgitDiffRange.
 *
 * Gets the new number of lines for @range.
 *
 * Returns: the range's new number of lines.
 */
gint
ggit_diff_range_get_new_lines (GgitDiffRange *range)
{
	g_return_val_if_fail (range != NULL, 0);

	return range->new_lines;
}

/* ex:set ts=8 noet: */
