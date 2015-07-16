/*
 * ggit-index.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
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

#include "ggit-index.h"
#include <git2.h>
#include "ggit-error.h"
#include "ggit-repository.h"
#include "ggit-index-entry.h"
#include "ggit-index-entry-resolve-undo.h"

/**
 * GgitIndex:
 *
 * Represents an index object.
 */
struct _GgitIndex
{
	GgitNative parent_instance;

	GFile *file;
};

enum
{
	PROP_0,
	PROP_FILE
};

static void ggit_index_initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_EXTENDED (GgitIndex, ggit_index, GGIT_TYPE_NATIVE,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_index_initable_iface_init))

static void
ggit_index_dispose (GObject *object)
{
	GgitIndex *index = GGIT_INDEX (object);

	if (index->file)
	{
		g_object_unref (index->file);
		index->file = NULL;
	}

	G_OBJECT_CLASS (ggit_index_parent_class)->dispose (object);
}

static void
ggit_index_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
	GgitIndex *index = GGIT_INDEX (object);

	switch (prop_id)
	{
		case PROP_FILE:
		{
			GFile *f;

			f = g_value_get_object (value);

			if (f != NULL)
			{
				index->file = g_file_dup (f);
			}

			break;
		}
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_index_get_property (GObject    *object,
                         guint       prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
	GgitIndex *index = GGIT_INDEX (object);

	switch (prop_id)
	{
		case PROP_FILE:
			g_value_set_object (value, index->file);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static gboolean
ggit_index_initable_init (GInitable     *initable,
                          GCancellable  *cancellable,
                          GError       **error)
{
	GgitIndex *index = GGIT_INDEX (initable);
	gchar *path = NULL;
	git_index *idx;
	gint err;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");

		return FALSE;
	}

	if (index->file != NULL)
	{
		path = g_file_get_path (index->file);
	}

	if (path == NULL)
	{
		g_set_error_literal (error,
		                     G_IO_ERROR,
		                     G_IO_ERROR_NOT_INITIALIZED,
		                     "No file specified");
		return FALSE;
	}

	err = git_index_open (&idx, path);
	g_free (path);

	if (err != GIT_OK)
	{
		_ggit_error_set (error, err);
		return FALSE;
	}

	_ggit_native_set (initable, idx,
		          (GDestroyNotify) git_index_free);

	return TRUE;
}

static void
ggit_index_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_index_initable_init;
}

static void
ggit_index_class_init (GgitIndexClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = ggit_index_dispose;

	object_class->get_property = ggit_index_get_property;
	object_class->set_property = ggit_index_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_FILE,
	                                 g_param_spec_object ("file",
	                                                      "File",
	                                                      "File",
	                                                      G_TYPE_FILE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));
}

static void
ggit_index_init (GgitIndex *index)
{
	_ggit_native_set_destroy_func (index, (GDestroyNotify) git_index_free);
}

GgitIndex *
_ggit_index_wrap (git_index *idx)
{
	if (idx == NULL)
	{
		return NULL;
	}

	return GGIT_INDEX (g_object_new (GGIT_TYPE_INDEX, "native", idx, NULL));
}

git_index *
_ggit_index_get_index (GgitIndex *idx)
{
	g_return_val_if_fail (GGIT_IS_INDEX (idx), NULL);

	return _ggit_native_get (idx);
}

/**
 * ggit_index_open:
 * @file: a #GFile.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new bare Git index object as a memory representation of the Git
 * index file in @file, without a repository to back it.
 *
 * Returns: (transfer full): a #GgitIndex or %NULL if an error occurred.
 *
 **/
GgitIndex *
ggit_index_open (GFile   *file,
                 GError **error)
{
	g_return_val_if_fail (G_IS_FILE (file), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_INDEX, NULL, error,
	                       "file", file,
	                       NULL);
}

/**
 * ggit_index_read:
 * @idx: a #GgitIndex.
 * @force: force read even if there are in-memory changes.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Update the contents of an existing index object in memory by reading from
 * the hard disk.
 *
 * If @force is true, this performs a "hard" read that discards in-memory
 * changes and always reloads the on-disk index data. If there is no on-disk
 * version, the index will be cleared.
 *
 * If @force is false, this does a "soft" read that reloads the index data from
 * disk only if it has changed since the last time it was loaded. Purely
 * in-memory index data will be untouched. Be aware: if there are changes on
 * disk, unwritten in-memory changes are discarded.
 *
 * Returns: %TRUE if the index could be read from the file associated with the
 *          index, %FALSE otherwise.
 *
 **/
gboolean
ggit_index_read (GgitIndex  *idx,
                 gboolean    force,
                 GError    **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_index_read (_ggit_native_get (idx), (int)force);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_index_write:
 * @idx: a #GgitIndex.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Write an existing index object from memory back to disk using an atomic file
 * lock.
 *
 * Returns: %TRUE if the index was successfully written to disk, %FALSE otherwise.
 *
 **/
gboolean
ggit_index_write (GgitIndex  *idx,
                  GError    **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_index_write (_ggit_native_get (idx));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_index_remove:
 * @idx: a #GgitIndex.
 * @file: the file to search.
 * @stage: the stage to search.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Remove a file from the index (specified by position).
 *
 * Returns: %TRUE if the file was successfully removed, %FALSE otherwise.
 *
 **/
gboolean
ggit_index_remove (GgitIndex  *idx,
                   GFile      *file,
                   gint        stage,
                   GError    **error)
{
	gint ret;
	gchar *path;
	GgitRepository *repo;
	GFile *wd;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (G_IS_FILE (file), FALSE);
	g_return_val_if_fail (stage >= 0 && stage <= 3, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	repo = ggit_index_get_owner (idx);
	wd = ggit_repository_get_workdir (repo);
	g_object_unref (repo);

	path = g_file_get_relative_path (wd, file);
	g_object_unref (wd);

	g_return_val_if_fail (path != NULL, FALSE);

	ret = git_index_remove (_ggit_native_get (idx), path, stage);
	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_index_add:
 * @idx: a #GgitIndex.
 * @entry: a #GgitIndexEntry.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Add a file to the index.
 *
 * Returns: %TRUE if the file was successfully added, %FALSE otherwise.
 *
 **/
gboolean
ggit_index_add (GgitIndex       *idx,
                GgitIndexEntry  *entry,
                GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (entry != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_index_add (_ggit_native_get (idx),
	                     _ggit_index_entry_get_native (entry));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_index_get_entries:
 * @idx: a #GgitIndex.
 *
 * Get the index entries enumerator.
 *
 * Returns: (transfer full): a #GgitIndexEntries.
 *
 **/
GgitIndexEntries *
ggit_index_get_entries (GgitIndex *idx)
{
	g_return_val_if_fail (GGIT_IS_INDEX (idx), NULL);

	return _ggit_index_entries_wrap (idx);
}

/**
 * ggit_index_get_entries_resolve_undo:
 * @idx: a #GgitIndex.
 *
 * Get the resolve undo entries enumerator.
 *
 * Returns: (transfer full): a #GgitIndexEntriesResolveUndo.
 *
 **/
GgitIndexEntriesResolveUndo *
ggit_index_get_entries_resolve_undo (GgitIndex *idx)
{
	g_return_val_if_fail (GGIT_IS_INDEX (idx), NULL);

	return _ggit_index_entries_resolve_undo_wrap (idx);
}

/**
 * ggit_index_add_file:
 * @idx: a #GgitIndex.
 * @file: file to add.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Add a file to the index. The specified file must be in the working directory
 * and must exist and be readable.
 *
 * Returns: %TRUE if the file was added to the index or %FALSE if there was an error.
 *
 **/

gboolean
ggit_index_add_file (GgitIndex  *idx,
                     GFile      *file,
                     GError    **error)
{
	GgitRepository *repo;
	GFile *wd;
	gchar *path;
	gint ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (G_IS_FILE (file), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	repo = ggit_index_get_owner (idx);

	wd = ggit_repository_get_workdir (repo);
	path = g_file_get_relative_path (wd, file);

	g_object_unref (wd);
	g_object_unref (repo);

	g_return_val_if_fail (path != NULL, FALSE);

	ret = git_index_add_bypath (_ggit_native_get (idx), path);
	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_index_add_path:
 * @idx: a #GgitIndex.
 * @path: path to the file to add.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Add a file to the index by path. You can specify both relative paths
 * (to the working directory) and absolute paths. Absolute paths however must
 * reside in the working directory. The specified path must exist and must be
 * readable.
 *
 * Returns: %TRUE if the file was added to the index or %FALSE if there was an error.
 *
 **/
gboolean
ggit_index_add_path (GgitIndex    *idx,
                     const gchar  *path,
                     GError      **error)
{
	GFile *f;
	gboolean ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (path != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	if (!g_path_is_absolute (path))
	{
		GgitRepository *repo;
		GFile *wd;

		repo = ggit_index_get_owner (idx);
		g_return_val_if_fail (repo != NULL, FALSE);

		wd = ggit_repository_get_workdir (repo);
		f = g_file_resolve_relative_path (wd, path);

		g_object_unref (wd);
		g_object_unref (repo);
	}
	else
	{
		f = g_file_new_for_path (path);
	}

	ret = ggit_index_add_file (idx, f, error);
	g_object_unref (f);

	return ret;
}

/**
 * ggit_index_get_owner:
 * @idx: a #GgitIndex.
 *
 * Get the #GgitRepository that owns the index.
 *
 * Returns: (transfer full): the #GgitRepository that owns this index.
 *
 **/
GgitRepository *
ggit_index_get_owner (GgitIndex *idx)
{
	git_repository *owner;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), NULL);

	owner = git_index_owner (_ggit_native_get (idx));
	return _ggit_repository_wrap (owner, FALSE);
}

/**
 * ggit_index_write_tree:
 * @idx: a #GgitIndex.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Write a new tree object to disk containing a representation of the current
 * state of the index. The index must be associated to an existing repository
 * and must not contain any files in conflict. You can use the resulting tree
 * to for instance create a commit.
 *
 * Returns: a #GgitOId or %NULL in case of an error.
 *
 **/
GgitOId *
ggit_index_write_tree (GgitIndex  *idx,
                       GError    **error)
{
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_index_write_tree (&oid, _ggit_native_get (idx));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/* ex:set ts=8 noet: */
