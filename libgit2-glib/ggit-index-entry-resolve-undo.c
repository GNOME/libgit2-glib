/*
 * ggit-index-entry-resolve_undo.c
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

#include "ggit-index-entry-resolve-undo.h"
#include "ggit-index.h"
#include <git2/sys/index.h>

struct _GgitIndexEntriesResolveUndo
{
	GgitIndex *owner;
	gint ref_count;
};

struct _GgitIndexEntryResolveUndo
{
	git_index_reuc_entry *entry;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitIndexEntriesResolveUndo,
                     ggit_index_entries_resolve_undo,
                     ggit_index_entries_resolve_undo_ref,
                     ggit_index_entries_resolve_undo_unref)

G_DEFINE_BOXED_TYPE (GgitIndexEntryResolveUndo,
                     ggit_index_entry_resolve_undo,
                     ggit_index_entry_resolve_undo_ref,
                     ggit_index_entry_resolve_undo_unref)

static GgitIndexEntryResolveUndo *
ggit_index_entry_resolve_undo_wrap (git_index_reuc_entry *entry)
{
	GgitIndexEntryResolveUndo *ret;

	ret = g_slice_new (GgitIndexEntryResolveUndo);
	ret->entry = entry;
	ret->ref_count = 1;

	return ret;
}

GgitIndexEntriesResolveUndo *
_ggit_index_entries_resolve_undo_wrap (GgitIndex *owner)
{
	GgitIndexEntriesResolveUndo *ret;

	ret = g_slice_new (GgitIndexEntriesResolveUndo);
	ret->owner = g_object_ref (owner);
	ret->ref_count = 1;

	return ret;
}

/**
 * ggit_index_entry_resolve_undo_ref:
 * @entry: a #GgitIndexEntryResolveUndo.
 *
 * Atomically increments the reference count of @entry by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): a #GgitIndexEntryResolveUndo.
 **/
GgitIndexEntryResolveUndo *
ggit_index_entry_resolve_undo_ref (GgitIndexEntryResolveUndo *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	g_atomic_int_inc (&entry->ref_count);

	return entry;
}

/**
 * ggit_index_entry_resolve_undo_unref:
 * @entry: a #GgitIndexEntryResolveUndo.
 *
 * Atomically decrements the reference count of @entry by one.
 * If the reference count drops to 0, @entry is freed.
 **/
void
ggit_index_entry_resolve_undo_unref (GgitIndexEntryResolveUndo *entry)
{
	g_return_if_fail (entry != NULL);

	if (g_atomic_int_dec_and_test (&entry->ref_count))
	{
		g_slice_free (GgitIndexEntryResolveUndo, entry);
	}
}

/**
 * ggit_index_entries_resolve_undo_ref:
 * @entries: a #GgitIndexEntriesResolveUndo.
 *
 * Atomically increments the reference count of @entries by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): a #GgitIndexEntriesResolveUndo.
 *
 **/
GgitIndexEntriesResolveUndo *
ggit_index_entries_resolve_undo_ref (GgitIndexEntriesResolveUndo *entries)
{
	g_return_val_if_fail (entries != NULL, NULL);

	g_atomic_int_inc (&entries->ref_count);

	return entries;
}

/**
 * ggit_index_entries_resolve_undo_unref:
 * @entries: a #GgitIndexEntriesResolveUndo.
 *
 * Atomically decrements the reference count of @entries by one.
 * If the reference count drops to 0, @entries is freed.
 **/
void
ggit_index_entries_resolve_undo_unref (GgitIndexEntriesResolveUndo *entries)
{
	g_return_if_fail (entries != NULL);

	if (g_atomic_int_dec_and_test (&entries->ref_count))
	{
		g_clear_object (&entries->owner);
		g_slice_free (GgitIndexEntriesResolveUndo, entries);
	}
}

/**
 * ggit_index_entries_resolve_undo_get:
 * @entries: a #GgitIndexEntriesResolveUndo.
 * @idx: the index of the entry.
 *
 * Get a #GgitIndexEntryResolveUndo by index. Note that the returned
 * #GgitIndexEntryResolveUndo is _only_ valid as long as:
 *
 * 1) The associated index has been closed
 * 2) The entry has not been removed (see ggit_index_remove())
 * 3) The index has not been refreshed (see ggit_index_read())
 *
 * Returns: (transfer full): a #GgitIndexEntryResolveUndo.
 *
 **/
GgitIndexEntryResolveUndo *
ggit_index_entries_resolve_undo_get (GgitIndexEntriesResolveUndo *entries,
                                     guint                        idx)
{
	git_index *gidx;
	const git_index_reuc_entry *ret;

	g_return_val_if_fail (entries != NULL, NULL);

	gidx = _ggit_index_get_index (entries->owner);
	ret = git_index_reuc_get_byindex (gidx, idx);

	return ggit_index_entry_resolve_undo_wrap ((git_index_reuc_entry *)ret);
}

/**
 * ggit_index_entries_resolve_undo_size:
 * @entries: a #GgitIndexEntriesResolveUndo.
 *
 * Get the number of #GgitIndexEntryResolveUndo entries.
 *
 * Returns: the number of entries.
 *
 **/
guint
ggit_index_entries_resolve_undo_size (GgitIndexEntriesResolveUndo *entries)
{
	git_index *gidx;

	g_return_val_if_fail (entries != NULL, 0);

	gidx = _ggit_index_get_index (entries->owner);

	return git_index_reuc_entrycount (gidx);
}

/**
 * ggit_index_entries_resolve_undo_get_by_file:
 * @entries: a #GgitIndexEntriesResolveUndo.
 * @file: a #GFile.
 *
 * Get an resolve_undo entry specified by path. The returned entry is read
 * only and should not be modified by the caller. If the entry could not be
 * found, %NULL is returned.
 *
 * Returns: (transfer full): a #GgitIndexEntryResolveUndo or %NULL.
 *
 **/
GgitIndexEntryResolveUndo *
ggit_index_entries_resolve_undo_get_by_file (GgitIndexEntriesResolveUndo  *entries,
                                             GFile                        *file)
{
	git_index *gidx;
	const git_index_reuc_entry *ret;
	gchar *path;

	g_return_val_if_fail (entries != NULL, NULL);
	g_return_val_if_fail (G_IS_FILE (file), NULL);

	gidx = _ggit_index_get_index (entries->owner);
	path = g_file_get_path (file);

	g_return_val_if_fail (path != NULL, NULL);

	ret = git_index_reuc_get_bypath (gidx, path);
	g_free (path);

	if (ret)
	{
		return ggit_index_entry_resolve_undo_wrap ((git_index_reuc_entry *)ret);
	}
	else
	{
		return NULL;
	}
}

/**
 * ggit_index_entry_resolve_undo_get_mode:
 * @entry: a #GgitIndexEntryResolveUndo.
 * @stage: the stage (0, 1 or 2)
 *
 * Get the mode of the index entry. The returned mode contains the modes from
 * stage 1, 2 and 3.
 *
 * Returns: the mode.
 *
 **/
guint
ggit_index_entry_resolve_undo_get_mode (GgitIndexEntryResolveUndo *entry,
                                        gint                    stage)
{
	g_return_val_if_fail (entry != NULL, 0);
	g_return_val_if_fail (stage >= 0 && stage <= 3, 0);

	return entry->entry->mode[stage];
}

/**
 * ggit_index_entry_resolve_undo_get_id:
 * @entry: a #GgitIndexEntryResolveUndo.
 * @stage: the stage (0, 1 or 2)
 *
 * Get the oid of the index entry.
 *
 * Returns: the oid.
 *
 **/
GgitOId *
ggit_index_entry_resolve_undo_get_id (GgitIndexEntryResolveUndo *entry,
                                      gint                    stage)
{
	g_return_val_if_fail (entry != NULL, NULL);
	g_return_val_if_fail (stage >= 0 && stage <= 3, NULL);

	return _ggit_oid_wrap (&entry->entry->oid[stage]);
}

/**
 * ggit_index_entry_resolve_undo_get_file:
 * @entry: a #GgitIndexEntryResolveUndo.
 *
 * Get the file of the index entry.
 *
 * Returns: (transfer full): a #GFile.
 *
 **/
GFile *
ggit_index_entry_resolve_undo_get_file (GgitIndexEntryResolveUndo *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	if (entry->entry->path == NULL)
	{
		return NULL;
	}

	return g_file_new_for_path (entry->entry->path);
}

/* ex:set ts=8 noet: */
