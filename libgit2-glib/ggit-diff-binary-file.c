/*
 * ggit-diff-binary-file.c
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
#include <string.h>

#include "ggit-diff-binary-file.h"

struct _GgitDiffBinaryFile {
	gint ref_count;

	GgitDiffBinaryType type;
	guint8 *data;
	gsize data_len;
	gsize inflated_len;
};

G_DEFINE_BOXED_TYPE (GgitDiffBinaryFile, ggit_diff_binary_file,
                     ggit_diff_binary_file_ref, ggit_diff_binary_file_unref)

GgitDiffBinaryFile *
_ggit_diff_binary_file_wrap (const git_diff_binary_file *file)
{
	GgitDiffBinaryFile *gfile;

	g_return_val_if_fail (file != NULL, NULL);

	gfile = g_slice_new (GgitDiffBinaryFile);
	gfile->ref_count = 1;
	gfile->type = (GgitDiffBinaryType)file->type;
	gfile->data = g_malloc (file->datalen);
	memcpy (gfile->data, file->data, file->datalen);
	gfile->data_len = file->datalen;
	gfile->inflated_len = file->inflatedlen;

	return gfile;
}

/**
 * ggit_diff_binary_file_ref:
 * @file: a #GgitDiff.
 *
 * Atomically increments the reference count of @file by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitDiffBinaryFile.
 **/
GgitDiffBinaryFile *
ggit_diff_binary_file_ref (GgitDiffBinaryFile *file)
{
	g_return_val_if_fail (file != NULL, NULL);

	g_atomic_int_inc (&file->ref_count);

	return file;
}

/**
 * ggit_diff_binary_file_unref:
 * @file: a #GgitDiffBinaryFile.
 *
 * Atomically decrements the reference count of @file by one.
 * If the reference count drops to 0, @file is freed.
 **/
void
ggit_diff_binary_file_unref (GgitDiffBinaryFile *file)
{
	g_return_if_fail (file != NULL);

	if (g_atomic_int_dec_and_test (&file->ref_count))
	{
		g_free (file->data);
		g_slice_free (GgitDiffBinaryFile, file);
	}
}

/**
 * ggit_diff_binary_file_get_binary_type:
 * @file: a #GgitDiffBinaryFile.
 *
 * Gets the #GgitDiffBinaryType for @file.
 *
 * Returns: the file's binary type.
 */
GgitDiffBinaryType
ggit_diff_binary_file_get_binary_type (GgitDiffBinaryFile *file)
{
	g_return_val_if_fail (file != NULL, GGIT_DIFF_BINARY_NONE);

	return file->type;
}

/**
 * ggit_diff_binary_file_get_data:
 * @file: a #GgitDiffBinaryFile.
 * @size: (allow-none): location to return size of byte data.
 *
 * Get the binary data. This data should not be modified.
 *
 * Returns: a pointer to the binary data, or %NULL.
 */
const guint8 *
ggit_diff_binary_file_get_data (GgitDiffBinaryFile *file,
                                gsize              *size)
{
	g_return_val_if_fail (file != NULL, NULL);

	if (size != NULL)
	{
		*size = file->data_len;
	}

	return file->data;
}

/**
 * ggit_diff_binary_file_get_inflated_size:
 * @file: a #GgitDiffBinaryFile.
 *
 * Gets the length of the binary data after inflation.
 *
 * Returns: the length of the binary data after inflation.
 */
gsize
ggit_diff_binary_file_get_inflated_size (GgitDiffBinaryFile *file)
{
	g_return_val_if_fail (file != NULL, 0);

	return file->inflated_len;
}

/* ex:set ts=8 noet: */
