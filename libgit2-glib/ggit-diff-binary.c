/*
 * ggit-diff-binary.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Ignacio Casal Quinteiro
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

#include "ggit-diff-binary.h"
#include "ggit-diff-binary-file.h"

struct _GgitDiffBinary {
	gint ref_count;

	GgitDiffBinaryFile *old_file;
	GgitDiffBinaryFile *new_file;
};

G_DEFINE_BOXED_TYPE (GgitDiffBinary, ggit_diff_binary,
                     ggit_diff_binary_ref, ggit_diff_binary_unref)

GgitDiffBinary *
_ggit_diff_binary_wrap (const git_diff_binary *binary)
{
	GgitDiffBinary *gbinary;

	g_return_val_if_fail (binary != NULL, NULL);

	gbinary = g_slice_new (GgitDiffBinary);
	gbinary->ref_count = 1;
	gbinary->old_file = _ggit_diff_binary_file_wrap (&binary->old_file);
	gbinary->new_file = _ggit_diff_binary_file_wrap (&binary->new_file);

	return gbinary;
}

/**
 * ggit_diff_binary_ref:
 * @binary: a #GgitDiffBinary.
 *
 * Atomically increments the reference count of @binary by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitDiffBinary.
 **/
GgitDiffBinary *
ggit_diff_binary_ref (GgitDiffBinary *binary)
{
	g_return_val_if_fail (binary != NULL, NULL);

	g_atomic_int_inc (&binary->ref_count);

	return binary;
}

/**
 * ggit_diff_binary_unref:
 * @binary: a #GgitDiffBinary.
 *
 * Atomically decrements the reference count of @binary by one.
 * If the reference count drops to 0, @binary is freed.
 **/
void
ggit_diff_binary_unref (GgitDiffBinary *binary)
{
	g_return_if_fail (binary != NULL);

	if (g_atomic_int_dec_and_test (&binary->ref_count))
	{
		ggit_diff_binary_file_unref (binary->old_file);
		ggit_diff_binary_file_unref (binary->new_file);
		g_slice_free (GgitDiffBinary, binary);
	}
}

/**
 * ggit_diff_binary_get_old_file:
 * @binary: a #GgitDiffBinary.
 *
 * Gets the #GgitDiffBinaryFile old file for @binary.
 *
 * Returns: (transfer none): the contents of the old file.
 */
GgitDiffBinaryFile *
ggit_diff_binary_get_old_file (GgitDiffBinary *binary)
{
	g_return_val_if_fail (binary != NULL, NULL);

	return binary->old_file;
}

/**
 * ggit_diff_binary_get_new_file:
 * @binary: a #GgitDiffBinary.
 *
 * Gets the #GgitDiffBinaryFile new file for @binary.
 *
 * Returns: (transfer none): the contents of the new file.
 */
GgitDiffBinaryFile *
ggit_diff_binary_get_new_file (GgitDiffBinary *binary)
{
	g_return_val_if_fail (binary != NULL, NULL);

	return binary->new_file;
}

/* ex:set ts=8 noet: */
