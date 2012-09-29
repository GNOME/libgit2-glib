/*
 * ggit-diff-delta.c
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

#include "ggit-diff-delta.h"
#include "ggit-diff-file.h"

G_BEGIN_DECLS

struct _GgitDiffDelta {
	gint ref_count;

	GgitDiffFile *old_file;
	GgitDiffFile *new_file;
	GgitDeltaType status;
	guint similarity;
	gint binary;
};

G_DEFINE_BOXED_TYPE (GgitDiffDelta, ggit_diff_delta,
                     ggit_diff_delta_ref, ggit_diff_delta_unref)

GgitDiffDelta *
_ggit_diff_delta_wrap (const git_diff_delta *delta)
{
	GgitDiffDelta *gdelta;

	g_return_val_if_fail (delta != NULL, NULL);

	gdelta = g_slice_new (GgitDiffDelta);
	gdelta->ref_count = 1;
	gdelta->old_file = _ggit_diff_file_wrap (&delta->old_file);
	gdelta->new_file = _ggit_diff_file_wrap (&delta->new_file);
	gdelta->status = delta->status;
	gdelta->similarity = delta->similarity;
	gdelta->binary = delta->binary;

	return gdelta;
}

/**
 * ggit_diff_delta_ref:
 * @delta: a #GgitDiffDelta.
 *
 * Atomically increments the reference count of @delta by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitDiffDelta.
 **/
GgitDiffDelta *
ggit_diff_delta_ref (GgitDiffDelta *delta)
{
	g_return_val_if_fail (delta != NULL, NULL);

	g_atomic_int_inc (&delta->ref_count);

	return delta;
}

/**
 * ggit_diff_delta_unref:
 * @delta: a #GgitDiffDelta.
 *
 * Atomically decrements the reference count of @delta by one.
 * If the reference count drops to 0, @delta is freed.
 **/
void
ggit_diff_delta_unref (GgitDiffDelta *delta)
{
	g_return_if_fail (delta != NULL);

	if (g_atomic_int_dec_and_test (&delta->ref_count))
	{
		ggit_diff_file_unref (delta->old_file);
		ggit_diff_file_unref (delta->new_file);
		g_slice_free (GgitDiffDelta, delta);
	}
}

/**
 * ggit_diff_delta_get_old_file:
 * @delta: a #GgitDiffDelta.
 *
 * Gets the old file for @delta.
 *
 * Returns: (transfer none): the delta's old file.
 */
GgitDiffFile *
ggit_diff_delta_get_old_file (GgitDiffDelta *delta)
{
	g_return_val_if_fail (delta != NULL, NULL);

	return delta->old_file;
}

/**
 * ggit_diff_delta_get_new_file:
 * @delta: a #GgitDiffDelta.
 *
 * Gets the new file for @delta.
 *
 * Returns: (transfer none): the delta's new file.
 */
GgitDiffFile *
ggit_diff_delta_get_new_file (GgitDiffDelta *delta)
{
	g_return_val_if_fail (delta != NULL, NULL);

	return delta->new_file;
}

/**
 * ggit_diff_delta_get_status:
 * @delta: a #GgitDiffDelta.
 *
 * Gets the #GgitDeltaType for @delta.
 *
 * Returns: the delta's status.
 */
GgitDeltaType
ggit_diff_delta_get_status (GgitDiffDelta *delta)
{
	g_return_val_if_fail (delta != NULL, 0);

	return delta->status;
}

/**
 * ggit_diff_delta_get_similarity:
 * @delta: a #GgitDiffDelta.
 *
 * Gets the similarity between @delta files.
 *
 * Returns: the delta's similarity.
 */
guint
ggit_diff_delta_get_similarity (GgitDiffDelta *delta)
{
	g_return_val_if_fail (delta != NULL, 0);

	return delta->similarity;
}

/**
 * ggit_diff_delta_get_binary:
 * @delta: a #GgitDiffDelta.
 *
 * Gets if @delta is binary.
 *
 * Returns: 0 if not binary, 1 if binary and -1 if unknown.
 */
gint
ggit_diff_delta_get_binary (GgitDiffDelta *delta)
{
	g_return_val_if_fail (delta != NULL, -1);

	return delta->binary;
}

/* ex:set ts=8 noet: */
