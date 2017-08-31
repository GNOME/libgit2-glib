/*
 * ggit-tree-builder.c
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

#include <git2.h>

#include "ggit-tree-builder.h"
#include "ggit-error.h"
#include "ggit-tree-entry.h"
#include "ggit-repository.h"

/**
 * GgitTreeBuilder:
 *
 * Represents a tree object.
 */

typedef struct _GgitTreeBuilderPrivate
{
	GgitRepository *repository;
} GgitTreeBuilderPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitTreeBuilder, ggit_tree_builder, GGIT_TYPE_NATIVE)

static void
ggit_tree_builder_dispose (GObject *object)
{
	GgitTreeBuilder *builder = GGIT_TREE_BUILDER (object);
	GgitTreeBuilderPrivate *priv;

	priv = ggit_tree_builder_get_instance_private (builder);

	g_clear_object (&priv->repository);

	G_OBJECT_CLASS (ggit_tree_builder_parent_class)->dispose (object);
}

static void
ggit_tree_builder_class_init (GgitTreeBuilderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = ggit_tree_builder_dispose;
}

static void
ggit_tree_builder_init (GgitTreeBuilder *self)
{
}

GgitTreeBuilder *
_ggit_tree_builder_wrap (git_treebuilder *builder,
                         GgitRepository  *repository,
                         gboolean         owned)
{
	GgitTreeBuilder *gbuilder;
	GgitTreeBuilderPrivate *priv;


	g_return_val_if_fail (builder != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	gbuilder = g_object_new (GGIT_TYPE_TREE_BUILDER,
	                         "native", builder,
	                         NULL);

	priv = ggit_tree_builder_get_instance_private (gbuilder);

	if (repository)
	{
		priv->repository = g_object_ref (repository);
	}

	if (owned)
	{
		_ggit_native_set_destroy_func (gbuilder,
		                               (GDestroyNotify)git_treebuilder_free);
	}

	return gbuilder;
}

/**
 * ggit_tree_builder_remove:
 * @builder: a #GgitTreeBuilder.
 * @path: the path to remove.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Remove an entry from the builder by path.
 *
 */
void
ggit_tree_builder_remove (GgitTreeBuilder  *builder,
                          const gchar      *path,
                          GError          **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_TREE_BUILDER (builder));
	g_return_if_fail (path != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_treebuilder_remove (_ggit_native_get (builder),
	                              path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_tree_builder_get_entry:
 * @builder: a #GgitTreeBuilder.
 * @path: the path to remove.
 *
 * Get an entry by path.
 *
 * Returns: (transfer full) (nullable): the #GGitTreeEntry or %NULL if no such entry exists.
 *
 */
GgitTreeEntry *
ggit_tree_builder_get_entry (GgitTreeBuilder *builder,
                             const gchar     *path)
{
	const git_tree_entry *ret;

	g_return_val_if_fail (GGIT_IS_TREE_BUILDER (builder), NULL);
	g_return_val_if_fail (path != NULL, NULL);

	ret = git_treebuilder_get (_ggit_native_get (builder),
	                           path);

	if (ret != NULL)
	{
		return _ggit_tree_entry_wrap ((git_tree_entry *)ret, FALSE);
	}
	else
	{
		return NULL;
	}
}

/**
 * ggit_tree_builder_clear:
 * @builder: a #GgitTreeBuilder.
 *
 * Clear all entries in the tree builder.
 *
 **/
void
ggit_tree_builder_clear (GgitTreeBuilder *builder)
{
	g_return_if_fail (GGIT_IS_TREE_BUILDER (builder));

	git_treebuilder_clear (_ggit_native_get (builder));
}

/**
 * ggit_tree_builder_write:
 * @builder: a #GgitTreeBuilder.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Write the contents of the tree builder as a tree object.
 *
 * Returns: (transfer full) (nullable): the #GgitOId of the created tree object or %NULL.
 *
 **/
GgitOId *
ggit_tree_builder_write (GgitTreeBuilder  *builder,
                         GError          **error)
{
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_TREE_BUILDER (builder), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_treebuilder_write (&oid,
	                             _ggit_native_get (builder));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_tree_builder_insert:
 * @builder: a #GgitTreeBuilder.
 * @filename: the file name.
 * @oid: the #GgitOId of the file blob to insert.
 * @file_mode: a #GgitFileMode.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Insert a file with a given blob in the tree builder. If the tree builder
 * already contains an entry for the given file, then this entry will be
 * overwritten.
 *
 * Note that the returned #GgitTreeEntry is bound to the lifetime of the tree
 * builder and cannot be used after the tree builder has been freed.
 *
 * Returns: (transfer full) (nullable): a #GgitTreeEntry or %NULL.
 *
 **/
GgitTreeEntry *
ggit_tree_builder_insert (GgitTreeBuilder  *builder,
                          const gchar      *filename,
                          GgitOId          *oid,
                          GgitFileMode      file_mode,
                          GError          **error)
{
	gint ret;
	const git_tree_entry *entry;

	g_return_val_if_fail (GGIT_IS_TREE_BUILDER (builder), NULL);
	g_return_val_if_fail (filename != NULL, NULL);
	g_return_val_if_fail (oid != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_treebuilder_insert (&entry,
	                              _ggit_native_get (builder),
	                              filename,
	                              _ggit_oid_get_oid (oid),
	                              (git_filemode_t)file_mode);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_tree_entry_wrap ((git_tree_entry *)entry, FALSE);
}

/* ex:set ts=8 noet: */
