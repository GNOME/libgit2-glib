/*
 * ggit-index-entry-unmerged.c
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

#include "ggit-index-entry-unmerged.h"
#include "ggit-index.h"

struct _GgitIndexEntriesUnmerged
{
	GgitIndex *owner;
};

struct _GgitIndexEntryUnmerged
{
	git_index_entry_unmerged *entry;
};

G_DEFINE_BOXED_TYPE (GgitIndexEntriesUnmerged,
                     ggit_index_entries_unmerged,
                     ggit_index_entries_unmerged_copy,
                     ggit_index_entries_unmerged_free)

G_DEFINE_BOXED_TYPE (GgitIndexEntryUnmerged,
                     ggit_index_entry_unmerged,
                     ggit_index_entry_unmerged_copy,
                     ggit_index_entry_unmerged_free)

static GgitIndexEntryUnmerged *
ggit_index_entry_unmerged_new (git_index_entry_unmerged *entry)
{
	GgitIndexEntryUnmerged *ret;

	ret = g_slice_new (GgitIndexEntryUnmerged);

	ret->entry = entry;

	return ret;
}

GgitIndexEntriesUnmerged *
_ggit_index_entries_unmerged_new (GgitIndex *owner)
{
	GgitIndexEntriesUnmerged *ret;

	ret = g_slice_new (GgitIndexEntriesUnmerged);
	ret->owner = g_object_ref (owner);

	return ret;
}

/**
 * ggit_index_entry_unmerged_copy:
 * @entry: a #GgitIndexEntryUnmerged.
 *
 * Copy a #GgitIndexEntryUnmerged.
 *
 * Returns: (transfer full): a #GgitIndexEntryUnmerged.
 *
 **/
GgitIndexEntryUnmerged *
ggit_index_entry_unmerged_copy (GgitIndexEntryUnmerged *entry)
{
	return ggit_index_entry_unmerged_new (entry->entry);
}

/**
 * ggit_index_entry_unmerged_free:
 * @entry: a #GgitIndexEntryUnmerged.
 *
 * Free a #GgitIndexEntryUnmerged.
 *
 **/
void
ggit_index_entry_unmerged_free (GgitIndexEntryUnmerged *entry)
{
	g_slice_free (GgitIndexEntryUnmerged, entry);
}

/**
 * ggit_index_entries_unmerged_copy:
 * @entries: a #GgitIndexEntriesUnmerged.
 *
 * Copy a #GgitIndexEntriesUnmerged.
 *
 * Returns: (transfer full): a #GgitIndexEntriesUnmerged.
 *
 **/
GgitIndexEntriesUnmerged *
ggit_index_entries_unmerged_copy (GgitIndexEntriesUnmerged *entries)
{
	return _ggit_index_entries_unmerged_new (entries->owner);
}

/**
 * ggit_index_entries_unmerged_free:
 * @entries: a #GgitIndexEntriesUnmerged.
 *
 * Free a #GgitIndexEntriesUnmerged.
 *
 **/
void
ggit_index_entries_unmerged_free (GgitIndexEntriesUnmerged *entries)
{
	if (entries == NULL)
	{
		return;
	}

	g_clear_object (&entries->owner);
	g_slice_free (GgitIndexEntriesUnmerged, entries);
}

/**
 * ggit_index_entries_unmerged_get:
 * @entries: a #GgitIndexEntriesUnmerged.
 * @idx: the index of the entry.
 *
 * Get a #GgitIndexEntryUnmerged by index. Note that the returned
 * #GgitIndexEntryUnmerged is _only_ valid as long as:
 *
 * 1) The associated index has been closed
 * 2) The entry has not been removed (see #ggit_index_remove)
 * 3) The index has not been refreshed (see #ggit_index_read)
 *
 * Returns: (transfer full): a #GgitIndexEntryUnmerged.
 *
 **/
GgitIndexEntryUnmerged *
ggit_index_entries_unmerged_get (GgitIndexEntriesUnmerged *entries,
                                 guint             idx)
{
	git_index *gidx;
	const git_index_entry_unmerged *ret;

	g_return_val_if_fail (entries != NULL, NULL);

	gidx = _ggit_index_get_index (entries->owner);
	ret = git_index_get_unmerged_byindex (gidx, idx);

	return ggit_index_entry_unmerged_new ((git_index_entry_unmerged *)ret);
}

/**
 * ggit_index_entries_unmerged_size:
 * @entries: a #GgitIndexEntriesUnmerged.
 *
 * Get the number of #GgitIndexEntryUnmerged entries.
 *
 * Returns: the number of entries.
 *
 **/
guint
ggit_index_entries_unmerged_size (GgitIndexEntriesUnmerged *entries)
{
	git_index *gidx;

	g_return_val_if_fail (entries != NULL, 0);

	gidx = _ggit_index_get_index (entries->owner);

	return git_index_entrycount_unmerged (gidx);
}

/**
 * ggit_index_entries_unmerged_get_by_file:
 * @entries: a #GgitIndexEntriesUnmerged.
 * @file: a #GFile.
 *
 * Get an unmerged entry specified by path.
 *
 * Returns: (transfer full): a #GgitIndexEntryUnmerged.
 *
 **/
GgitIndexEntryUnmerged *
ggit_index_entries_unmerged_get_by_file (GgitIndexEntriesUnmerged *entries,
                                         GFile                    *file)
{
	git_index *gidx;
	const git_index_entry_unmerged *ret;
	gchar *path;

	g_return_val_if_fail (entries != NULL, NULL);
	g_return_val_if_fail (G_IS_FILE (file), NULL);

	gidx = _ggit_index_get_index (entries->owner);

	path = g_file_get_path (file);

	if (!path)
	{
		return NULL;
	}

	ret = git_index_get_unmerged_bypath (gidx, path);
	g_free (path);

	if (ret)
	{
		return ggit_index_entry_unmerged_new ((git_index_entry_unmerged *)ret);
	}

	return NULL;
}

/**
 * ggit_index_entry_unmerged_get_mode:
 * @entry: a #GgitIndexEntryUnmerged.
 * @stage: the stage (0, 1 or 2)
 *
 * Get the mode of the index entry. The returned mode contains the modes from
 * stage 1, 2 and 3.
 *
 * Returns: the mode.
 *
 **/
guint
ggit_index_entry_unmerged_get_mode (GgitIndexEntryUnmerged *entry,
                                    gint                    stage)
{
	g_return_val_if_fail (entry != NULL, 0);
	g_return_val_if_fail (stage >= 0 && stage <= 3, 0);

	return entry->entry->mode[stage];
}

/**
 * ggit_index_entry_unmerged_get_id:
 * @entry: a #GgitIndexEntryUnmerged.
 *
 * Get the oid of the index entry.
 *
 * Returns: the oid.
 *
 **/
GgitOId *
ggit_index_entry_unmerged_get_id (GgitIndexEntryUnmerged *entry,
                                  gint                    stage)
{
	g_return_val_if_fail (entry != NULL, NULL);
	g_return_val_if_fail (stage >= 0 && stage <= 3, NULL);

	return _ggit_oid_new (&entry->entry->oid[stage]);
}

/**
 * ggit_index_entry_unmerged_get_file:
 * @entry: a #GgitIndexEntryUnmerged.
 *
 * Get the file of the index entry.
 *
 * Returns: (transfer full): a #GFile.
 *
 **/
GFile *
ggit_index_entry_unmerged_get_file (GgitIndexEntryUnmerged *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	if (entry->entry->path == NULL)
	{
		return NULL;
	}

	return g_file_new_for_path (entry->entry->path);
}

/* ex:set ts=8 noet: */
