/*
 * ggit-index.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ggit-index.h"
#include <git2/errors.h>
#include "ggit-error.h"

#define GGIT_INDEX_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_INDEX, GgitIndexPrivate))

struct _GgitIndexPrivate
{
	git_index *idx;
	GFile *file;
};

enum
{
	PROP_0,
	PROP_FILE
};

static void ggit_index_initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_EXTENDED (GgitIndex, ggit_index, G_TYPE_OBJECT,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_index_initable_iface_init))

static void
ggit_index_finalize (GObject *object)
{
	GgitIndex *idx;

	idx = GGIT_INDEX (object);

	if (idx->priv->file)
	{
		g_object_unref (idx->priv->file);
	}

	if (idx->priv->idx)
	{
		git_index_free (idx->priv->idx);
	}

	G_OBJECT_CLASS (ggit_index_parent_class)->finalize (object);
}

static void
ggit_index_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
	GgitIndex *self = GGIT_INDEX (object);

	switch (prop_id)
	{
		case PROP_FILE:
		{
			GFile *f;

			f = g_value_get_object (value);

			if (f != NULL)
			{
				self->priv->file = g_file_dup (f);
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
	GgitIndex *self = GGIT_INDEX (object);

	switch (prop_id)
	{
		case PROP_FILE:
			g_value_set_object (value, self->priv->file);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static gboolean
ggit_index_initable_init (GInitable    *initable,
                          GCancellable *cancellable,
                          GError      **error)
{
	GgitIndexPrivate *priv;
	gboolean success = TRUE;
	gint err;
	gchar const *lasterr;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");

		return FALSE;
	}

	priv = GGIT_INDEX (initable)->priv;

	if (priv->file != NULL)
	{
		gchar *path;

		path = g_file_get_path (priv->file);

		if (path != NULL)
		{
			err = git_index_open (&priv->idx, path);
		}

		g_free (path);

		if (err != GIT_SUCCESS)
		{
			lasterr = git_lasterror ();
		}
	}
	else
	{
		err = GGIT_ERROR_INVALIDPATH;
		lasterr = "No file specified";
	}

	if (err != GIT_SUCCESS)
	{
		g_set_error_literal (error,
		                     G_IO_ERROR,
		                     G_IO_ERROR_NOT_INITIALIZED,
		                     lasterr);

		success = FALSE;
	}

	return success;
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

	object_class->finalize = ggit_index_finalize;

	object_class->get_property = ggit_index_get_property;
	object_class->set_property = ggit_index_set_property;

	g_type_class_add_private (object_class, sizeof (GgitIndexPrivate));

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
ggit_index_init (GgitIndex *self)
{
	self->priv = GGIT_INDEX_GET_PRIVATE (self);
}

GgitIndex *
_ggit_index_wrap (git_index *idx)
{
	GgitIndex *ret;

	if (idx == NULL)
	{
		return NULL;
	}

	ret = g_object_new (GGIT_TYPE_INDEX, NULL);
	ret->priv->idx = idx;

	return ret;
}

git_index *
_ggit_index_get_index (GgitIndex *idx)
{
	g_return_val_if_fail (GGIT_IS_INDEX (idx), NULL);

	return idx->priv->idx;
}

/**
 * ggit_index_open:
 * @file: a #GFile.
 * @error: a #GError.
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

	return g_initable_new (GGIT_TYPE_INDEX,
	                       NULL,
	                       error,
	                       "file",
	                       file,
	                       NULL);
}

/**
 * ggit_index_read:
 * @idx: a #GgitIndex.
 * @error: a #GError.
 *
 * Update the contents of an existing index object in memory by reading from
 * the hard disk.
 *
 * Returns: %TRUE if the index could be read from the file associated with the
 *          index, %FALSE otherwise.
 *
 **/
gboolean
ggit_index_read (GgitIndex  *idx,
                 GError    **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_index_read (idx->priv->idx);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_index_write:
 * @idx: a #GgitIndex.
 * @error: a #GError.
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

	ret = git_index_write (idx->priv->idx);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_index_uniq:
 * @idx: a #GgitIndex.
 *
 * Remove all entries with equal path except last added.
 *
 **/
void
ggit_index_uniq (GgitIndex *idx)
{
	g_return_if_fail (GGIT_IS_INDEX (idx));

	git_index_uniq (idx->priv->idx);
}

gboolean
ggit_index_remove (GgitIndex  *idx,
                   gint        position,
                   GError    **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_index_remove (idx->priv->idx, position);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

gboolean
ggit_index_append (GgitIndex  *idx,
                   GFile      *file,
                   gint        stage,
                   GError    **error)
{
	gint ret;
	gchar *path;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (G_IS_FILE (file), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	path = g_file_get_path (file);

	g_return_val_if_fail (path != NULL, FALSE);

	ret = git_index_append (idx->priv->idx, path, stage);
	g_free (path);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

gboolean
ggit_index_add (GgitIndex  *idx,
                GFile      *file,
                gint        stage,
                GError    **error)
{
	gint ret;
	gchar *path;

	g_return_val_if_fail (GGIT_IS_INDEX (idx), FALSE);
	g_return_val_if_fail (G_IS_FILE (file), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	path = g_file_get_path (file);

	g_return_val_if_fail (path != NULL, FALSE);

	ret = git_index_add (idx->priv->idx, path, stage);
	g_free (path);

	if (ret != GIT_SUCCESS)
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

	return _ggit_index_entries_new (idx);
}

GgitIndexEntriesUnmerged *
ggit_index_get_entries_unmerged (GgitIndex *idx)
{
	g_return_val_if_fail (GGIT_IS_INDEX (idx), NULL);

	return _ggit_index_entries_unmerged_new (idx);
}