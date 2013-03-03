/*
 * ggit-diff-file.c
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

#include "ggit-diff-file.h"

#include "ggit-oid.h"

struct _GgitDiffFile {
	gint ref_count;

	GgitOId *oid;
	gchar *path;
	guint16 mode;
	gint64 size;
	GgitDiffFlag flags;
};

G_DEFINE_BOXED_TYPE (GgitDiffFile, ggit_diff_file,
                     ggit_diff_file_ref, ggit_diff_file_unref)

GgitDiffFile *
_ggit_diff_file_wrap (const git_diff_file *file)
{
	GgitDiffFile *gfile;

	g_return_val_if_fail (file != NULL, NULL);

	gfile = g_slice_new (GgitDiffFile);
	gfile->ref_count = 1;
	gfile->oid = _ggit_oid_wrap (&file->oid);
	gfile->path = file->path == NULL ? NULL : g_strdup (file->path);
	gfile->mode = file->mode;
	gfile->size = file->size;
	gfile->flags = file->flags;

	return gfile;
}

/**
 * ggit_diff_file_ref:
 * @file: a #GgitDiff.
 *
 * Atomically increments the reference count of @file by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitDiffFile.
 **/
GgitDiffFile *
ggit_diff_file_ref (GgitDiffFile *file)
{
	g_return_val_if_fail (file != NULL, NULL);

	g_atomic_int_inc (&file->ref_count);

	return file;
}

/**
 * ggit_diff_file_unref:
 * @file: a #GgitDiffFile.
 *
 * Atomically decrements the reference count of @file by one.
 * If the reference count drops to 0, @file is freed.
 **/
void
ggit_diff_file_unref (GgitDiffFile *file)
{
	g_return_if_fail (file != NULL);

	if (g_atomic_int_dec_and_test (&file->ref_count))
	{
		ggit_oid_free (file->oid);
		g_free (file->path);
		g_slice_free (GgitDiffFile, file);
	}
}

/**
 * ggit_diff_file_get_oid:
 * @file: a #GgitDiffFile.
 *
 * Gets the #GgitOId for @file.
 *
 * Returns: (transfer none): the file's #GgitOId.
 */
GgitOId *
ggit_diff_file_get_oid (GgitDiffFile *file)
{
	g_return_val_if_fail (file != NULL, NULL);

	return file->oid;
}

/**
 * ggit_diff_file_get_path:
 * @file: a #GgitDiffFile.
 *
 * Gets the path of @file or %NULL if it is unknown.
 *
 * Returns: (allow-none): the file's path, or %NULL.
 */
const gchar *
ggit_diff_file_get_path (GgitDiffFile *file)
{
	g_return_val_if_fail (file != NULL, NULL);

	return file->path;
}

/**
 * ggit_diff_file_get_mode:
 * @file: a #GgitDiffFile.
 *
 * Gets the mode for @file.
 *
 * Returns: the file's mode.
 */
guint16
ggit_diff_file_get_mode (GgitDiffFile *file)
{
	g_return_val_if_fail (file != NULL, 0);

	return file->mode;
}

/**
 * ggit_diff_file_get_size:
 * @file: a #GgitDiffFile.
 *
 * Gets the size for @file.
 *
 * Returns: the file's size.
 */
gint64
ggit_diff_file_get_size (GgitDiffFile *file)
{
	g_return_val_if_fail (file != NULL, 0);

	return file->size;
}

/**
 * ggit_diff_file_get_flags:
 * @file: a #GgitDiffFile.
 *
 * Gets the #GgitDifflags for @file.
 *
 * Returns: the file's flags.
 */
GgitDiffFlag
ggit_diff_file_get_flags (GgitDiffFile *file)
{
	g_return_val_if_fail (file != NULL, 0);

	return file->flags;
}

/* ex:set ts=8 noet: */
