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
#include <git2.h>

struct _GgitTreeEntry
{
	git_tree_entry *entry;
	gboolean free_entry;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitTreeEntry,
                     ggit_tree_entry,
                     ggit_tree_entry_ref,
                     ggit_tree_entry_unref)

GgitTreeEntry *
_ggit_tree_entry_wrap (git_tree_entry *entry,
                       gboolean        free_entry)
{
	GgitTreeEntry *ret;

	ret = g_slice_new (GgitTreeEntry);
	ret->entry = entry;
	ret->free_entry = free_entry;
	ret->ref_count = 1;

	return ret;
}

/**
 * ggit_tree_entry_ref:
 * @entry: a #GgitTreeEntry.
 *
 * Atomically increments the reference count of @entry by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (nullable): a #GgitTreeEntry or %NULL.
 **/
GgitTreeEntry *
ggit_tree_entry_ref (GgitTreeEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	g_atomic_int_inc (&entry->ref_count);

	return entry;
}

/**
 * ggit_tree_entry_unref:
 * @entry: a #GgitTreeEntry.
 *
 * Atomically decrements the reference count of @entry by one.
 * If the reference count drops to 0, @entry is freed.
 **/
void
ggit_tree_entry_unref (GgitTreeEntry *entry)
{
	g_return_if_fail (entry != NULL);

	if (g_atomic_int_dec_and_test (&entry->ref_count))
	{
		if (entry->free_entry)
		{
			git_tree_entry_free (entry->entry);
		}

		g_slice_free (GgitTreeEntry, entry);
	}
}

/**
 * ggit_tree_entry_get_file_mode:
 * @entry: a #GgitTreeEntry.
 *
 * Gets the #GgitFileMode of a tree entry.
 *
 * Returns: the #GgitFileMode of a tree entry.
 **/
GgitFileMode
ggit_tree_entry_get_file_mode (GgitTreeEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return (GgitFileMode)git_tree_entry_filemode (entry->entry);
}

/**
 * ggit_tree_entry_get_id:
 * @entry: a #GgitTreeEntry.
 *
 * Get the #GgitOId of the entry.
 *
 * Returns: (transfer full) (nullable): a #GgitOId or %NULL.
 *
 **/
GgitOId *
ggit_tree_entry_get_id (GgitTreeEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return _ggit_oid_wrap (git_tree_entry_id (entry->entry));
}

/**
 * ggit_tree_entry_get_name:
 * @entry: a #GgitTreeEntry.
 *
 * Get the name of the tree entry.
 *
 * Returns: (transfer none) (nullable): the name of the tree entry or %NULL.
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
