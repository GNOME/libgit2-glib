/*
 * ggit-tree.c
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

#include "ggit-tree.h"
#include "ggit-oid.h"

G_DEFINE_TYPE (GgitTree, ggit_tree, GGIT_TYPE_OBJECT)

static void
ggit_tree_class_init (GgitTreeClass *klass)
{
}

static void
ggit_tree_init (GgitTree *self)
{
}

GgitTree *
_ggit_tree_wrap (git_tree *tree,
                 gboolean  owned)
{
	GgitTree *gtree;

	gtree = g_object_new (GGIT_TYPE_TREE,
	                      "native", tree,
	                      NULL);

	if (owned)
	{
		_ggit_native_set_destroy_func (gtree,
		                               (GDestroyNotify)git_object_free);
	}

	return gtree;
}

/**
 * ggit_tree_id:
 * @tree: a #GgitTree.
 *
 * Get the #GgitOId of the tree.
 *
 * Returns: (transfer full): a #GgitOId.
 *
 **/
GgitOId *
ggit_tree_id (GgitTree *tree)
{
	git_tree *t;
	const git_oid *oid;

	g_return_val_if_fail (GGIT_IS_TREE (tree), NULL);

	t = _ggit_native_get (tree);

	oid = git_tree_id (t);

	return _ggit_oid_new (oid);
}

/**
 * ggit_tree_get:
 * @tree: a #GgitTree.
 * @i: the index of the entry.
 *
 * Get a tree entry by index.
 *
 * Returns: (transfer full): a #GgitTreeEntry.
 *
 **/
GgitTreeEntry *
ggit_tree_get (GgitTree *tree,
               guint     i)
{
	git_tree *t;
	const git_tree_entry *entry;

	g_return_val_if_fail (GGIT_IS_TREE (tree), NULL);

	t = _ggit_native_get (tree);

	entry = git_tree_entry_byindex (t, i);

	return _ggit_tree_entry_wrap (entry);
}

/**
 * ggit_tree_size:
 * @tree: a #GgitTree.
 *
 * Get the number of entries in the tree.
 *
 * Returns: the number of entries in the tree.
 *
 **/
guint
ggit_tree_size (GgitTree *tree)
{
	git_tree *t;

	g_return_val_if_fail (GGIT_IS_TREE (tree), 0);

	t = _ggit_native_get (tree);

	return (guint)git_tree_entrycount (t);
}

/**
 * ggit_tree_get_by_file:
 * @tree: a #GgitTree.
 * @file: a #GFile.
 *
 * Get a tree entry by file.
 *
 * Returns: (transfer full): a #GgitTreeEntry.
 *
 **/
GgitTreeEntry *
ggit_tree_get_by_file (GgitTree *tree,
                       GFile    *file)
{
	git_tree *t;
	gchar *path;
	GgitTreeEntry *entry;

	g_return_val_if_fail (GGIT_IS_TREE (tree), NULL);

	t = _ggit_native_get (tree);

	path = g_file_get_path (file);
	entry = _ggit_tree_entry_wrap (git_tree_entry_byname (t, path));
	g_free (path);

	return entry;
}

/* ex:set ts=8 noet: */
