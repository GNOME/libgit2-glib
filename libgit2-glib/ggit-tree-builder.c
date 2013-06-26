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

G_DEFINE_TYPE (GgitTreeBuilder, ggit_tree_builder, GGIT_TYPE_NATIVE)

static void
ggit_tree_builder_class_init (GgitTreeBuilderClass *klass)
{
}

static void
ggit_tree_builder_init (GgitTreeBuilder *self)
{
}

GgitTreeBuilder *
_ggit_tree_builder_wrap (git_treebuilder *builder,
                         gboolean         owned)
{
	GgitTreeBuilder *gbuilder;

	gbuilder = g_object_new (GGIT_TYPE_TREE_BUILDER,
	                         "native", builder,
	                         NULL);

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
 * Returns: the #GGitTreeEntry or %NULL if no such entry exists.
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


/* ex:set ts=8 noet: */
