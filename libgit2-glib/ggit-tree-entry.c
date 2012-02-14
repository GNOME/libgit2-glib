/*
 * ggit-tree-entry.c
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

#include "ggit-tree-entry.h"
#include "ggit-oid.h"
#include "ggit-utils.h"

struct _GgitTreeEntry
{
	const git_tree_entry *entry;
};

G_DEFINE_BOXED_TYPE (GgitTreeEntry,
                     ggit_tree_entry,
                     ggit_tree_entry_copy,
                     ggit_tree_entry_free)

static GgitTreeEntry *
ggit_tree_entry_new (const git_tree_entry *entry)
{
	GgitTreeEntry *ret;

	ret = g_slice_new (GgitTreeEntry);

	ret->entry = entry;

	return ret;
}

/**
 * ggit_tree_entry_copy:
 * @entry: a #GgitTreeEntry.
 *
 * Copy a #GgitTreeEntry.
 *
 * Returns: (transfer full): a #GgitTreeEntry.
 *
 **/
GgitTreeEntry *
ggit_tree_entry_copy (GgitTreeEntry *entry)
{
	if (entry != NULL)
	{
		return ggit_tree_entry_new (entry->entry);
	}

	return NULL;
}

/**
 * ggit_tree_entry_free:
 * @entry: a #GgitTreeEntry.
 *
 * Free a #GgitTreeEntry.
 *
 **/
void
ggit_tree_entry_free (GgitTreeEntry *entry)
{
	if (entry != NULL)
	{
		g_slice_free (GgitTreeEntry, entry);
	}
}

GgitTreeEntry *
_ggit_tree_entry_wrap (const git_tree_entry *entry)
{
	if (!entry)
	{
		return NULL;
	}

	return ggit_tree_entry_new (entry);
}

/**
 * ggit_tree_entry_get_attributes:
 * @entry: a #GgitTreeEntry.
 *
 * Get the attributes of the tree entry.
 *
 * Returns: attributes of the tree entry.
 *
 **/
guint
ggit_tree_entry_get_attributes (GgitTreeEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return git_tree_entry_attributes (entry->entry);
}

/**
 * ggit_tree_entry_get_id:
 * @entry: a #GgitTreeEntry.
 *
 * Get the #GgitOId of the entry.
 *
 * Returns: (transfer full): a #GgitOId.
 *
 **/
GgitOId *
ggit_tree_entry_get_id (GgitTreeEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return _ggit_oid_new (git_tree_entry_id (entry->entry));
}

/**
 * ggit_tree_entry_get_name:
 * @entry: a #GgitTreeEntry.
 *
 * Get the name of the tree entry.
 *
 * Returns: the name of the tree entry.
 *
 **/
const gchar *
ggit_tree_entry_get_name (GgitTreeEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return git_tree_entry_name (entry->entry);
}

/**
 * ggit_tree_entry_get_object_type:
 * @entry: a #GgitTreeEntry.
 *
 * Get the type of the tree entry object.
 *
 * Returns: a #GType.
 *
 **/
GType
ggit_tree_entry_get_object_type (GgitTreeEntry *entry)
{
	g_return_val_if_fail (entry != NULL, G_TYPE_NONE);

	return ggit_utils_get_gtype_from_otype (git_tree_entry_type (entry->entry));
}

/* ex:set ts=8 noet: */
