/*
 * ggit-index-entry.c
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

#include "ggit-index-entry.h"
#include "ggit-index.h"

struct _GgitIndexEntries
{
	GgitIndex *owner;
};

struct _GgitIndexEntry
{
	git_index_entry *entry;
};

G_DEFINE_BOXED_TYPE (GgitIndexEntries,
                     ggit_index_entries,
                     ggit_index_entries_copy,
                     ggit_index_entries_free)

G_DEFINE_BOXED_TYPE (GgitIndexEntry,
                     ggit_index_entry,
                     ggit_index_entry_copy,
                     ggit_index_entry_free)

static GgitIndexEntry *
ggit_index_entry_new (git_index_entry *entry)
{
	GgitIndexEntry *ret;

	ret = g_slice_new (GgitIndexEntry);

	ret->entry = entry;

	return ret;
}

GgitIndexEntries *
_ggit_index_entries_new (GgitIndex *owner)
{
	GgitIndexEntries *ret;

	ret = g_slice_new (GgitIndexEntries);
	ret->owner = g_object_ref (owner);

	return ret;
}

/**
 * ggit_index_entry_copy:
 * @entry: a #GgitIndexEntry.
 *
 * Copy a #GgitIndexEntry.
 *
 * Returns: (transfer full): a #GgitIndexEntry.
 *
 **/
GgitIndexEntry *
ggit_index_entry_copy (GgitIndexEntry *entry)
{
	return ggit_index_entry_new (entry->entry);
}

/**
 * ggit_index_entry_free:
 * @entry: a #GgitIndexEntry.
 *
 * Free a #GgitIndexEntry.
 *
 **/
void
ggit_index_entry_free (GgitIndexEntry *entry)
{
	g_slice_free (GgitIndexEntry, entry);
}

/**
 * ggit_index_entries_copy:
 * @entries: a #GgitIndexEntries.
 *
 * Copy a #GgitIndexEntries.
 *
 * Returns: (transfer full): a #GgitIndexEntries.
 *
 **/
GgitIndexEntries *
ggit_index_entries_copy (GgitIndexEntries *entries)
{
	return _ggit_index_entries_new (entries->owner);
}

/**
 * ggit_index_entries_free:
 * @entries: a #GgitIndexEntries.
 *
 * Free a #GgitIndexEntries.
 *
 **/
void
ggit_index_entries_free (GgitIndexEntries *entries)
{
	if (entries == NULL)
	{
		return;
	}

	g_clear_object (&entries->owner);
	g_slice_free (GgitIndexEntries, entries);
}

/**
 * ggit_index_entries_get:
 * @entries: a #GgitIndexEntries.
 * @idx: the index of the entry.
 *
 * Get a #GgitIndexEntry by index. Note that the returned #GgitIndexEntry is
 * _only_ valid as long as:
 *
 * 1) The associated index has been closed
 * 2) The entry has not been removed (see #ggit_index_remove)
 * 3) The index has not been refreshed (see #ggit_index_read)
 *
 * Changes to the #GgitIndexEntry will be reflected in the index once written
 * back to disk using #ggit_index_write.
 *
 * Returns: (transfer full): a #GgitIndexEntry.
 *
 **/
GgitIndexEntry *
ggit_index_entries_get (GgitIndexEntries *entries,
                        guint             idx)
{
	git_index *gidx;

	g_return_val_if_fail (entries != NULL, NULL);

	gidx = _ggit_index_get_index (entries->owner);

	return ggit_index_entry_new (git_index_get (gidx, idx));
}

/**
 * ggit_index_entries_size:
 * @entries: a #GgitIndexEntries.
 *
 * Get the number of #GgitIndexEntry entries.
 *
 * Returns: the number of entries.
 *
 **/
guint
ggit_index_entries_size (GgitIndexEntries *entries)
{
	git_index *gidx;

	g_return_val_if_fail (entries != NULL, 0);

	gidx = _ggit_index_get_index (entries->owner);

	return git_index_entrycount (gidx);
}

/**
 * ggit_index_entry_get_dev:
 * @entry: a #GgitIndexEntry.
 *
 * Get the dev of the index entry.
 *
 * Returns: the dev.
 *
 **/
guint
ggit_index_entry_get_dev (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->dev;
}

/**
 * ggit_index_entry_get_ino:
 * @entry: a #GgitIndexEntry.
 *
 * Get the ino of the index entry.
 *
 * Returns: the ino.
 *
 **/
guint
ggit_index_entry_get_ino (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->ino;
}

/**
 * ggit_index_entry_get_mode:
 * @entry: a #GgitIndexEntry.
 *
 * Get the mode of the index entry.
 *
 * Returns: the mode.
 *
 **/
guint
ggit_index_entry_get_mode (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->mode;
}

/**
 * ggit_index_entry_get_uid:
 * @entry: a #GgitIndexEntry.
 *
 * Get the uid of the index entry.
 *
 * Returns: the uid.
 *
 **/
guint
ggit_index_entry_get_uid (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->uid;
}

/**
 * ggit_index_entry_get_gid:
 * @entry: a #GgitIndexEntry.
 *
 * Get the gid of the index entry.
 *
 * Returns: the gid.
 *
 **/
guint
ggit_index_entry_get_gid (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->gid;
}

/**
 * ggit_index_entry_get_file_size:
 * @entry: a #GgitIndexEntry.
 *
 * Get the file size of the index entry.
 *
 * Returns: the file size.
 *
 **/
goffset
ggit_index_entry_get_file_size (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->file_size;
}

/**
 * ggit_index_entry_get_oid:
 * @entry: a #GgitIndexEntry.
 *
 * Get the oid of the index entry.
 *
 * Returns: the oid.
 *
 **/
GgitOId *
ggit_index_entry_get_oid (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return _ggit_oid_new (&entry->entry->oid);
}

/**
 * ggit_index_entry_get_flags:
 * @entry: a #GgitIndexEntry.
 *
 * Get the flags of the index entry.
 *
 * Returns: the flags.
 *
 **/
guint
ggit_index_entry_get_flags (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->flags;
}

/**
 * ggit_index_entry_get_flags_extended:
 * @entry: a #GgitIndexEntry.
 *
 * Get the extended flags of the index entry.
 *
 * Returns: the extended flags.
 *
 **/
guint
ggit_index_entry_get_flags_extended (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->flags_extended;
}

/**
 * ggit_index_entry_get_file:
 * @entry: a #GgitIndexEntry.
 *
 * Get the file of the index entry.
 *
 * Returns: (transfer full): a #GFile.
 *
 **/
GFile *
ggit_index_entry_get_file (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	if (entry->entry->path == NULL)
	{
		return NULL;
	}

	return g_file_new_for_path (entry->entry->path);
}

