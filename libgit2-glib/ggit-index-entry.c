/*
 * ggit-index-entry.c
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

#include "ggit-index-entry.h"
#include "ggit-index.h"
#include "ggit-repository.h"
#include "ggit-commit.h"
#include "ggit-signature.h"

#include <string.h>

struct _GgitIndexEntries
{
	GgitIndex *owner;
	gint ref_count;
};

struct _GgitIndexEntry
{
	git_index_entry *entry;
	gint ref_count;
	gboolean owned;
};

G_DEFINE_BOXED_TYPE (GgitIndexEntries,
                     ggit_index_entries,
                     ggit_index_entries_ref,
                     ggit_index_entries_unref)

G_DEFINE_BOXED_TYPE (GgitIndexEntry,
                     ggit_index_entry,
                     ggit_index_entry_ref,
                     ggit_index_entry_unref)

static GgitIndexEntry *
ggit_index_entry_wrap (git_index_entry *entry,
                       gboolean         owned)
{
	GgitIndexEntry *ret;

	ret = g_slice_new (GgitIndexEntry);

	ret->entry = entry;
	ret->ref_count = 1;
	ret->owned = owned;

	return ret;
}

GgitIndexEntries *
_ggit_index_entries_wrap (GgitIndex *owner)
{
	GgitIndexEntries *ret;

	ret = g_slice_new (GgitIndexEntries);
	ret->owner = g_object_ref (owner);
	ret->ref_count = 1;

	return ret;
}

GgitIndexEntry *
_ggit_index_entry_new (const gchar *path,
                       GgitOId     *id)
{
	git_index_entry *entry;
	GgitIndexEntry *ret;

	entry = g_slice_new0 (git_index_entry);

	ret = ggit_index_entry_wrap (entry, TRUE);
	entry->mode = GIT_FILEMODE_BLOB;

	ggit_index_entry_set_path (ret, path);
	ggit_index_entry_set_id (ret, id);

	return ret;
}

/**
 * ggit_index_entry_ref:
 * @entry: a #GgitIndexEntry.
 *
 * Atomically increments the reference count of @entry by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (nullable): a #GgitIndexEntry or %NULL.
 **/
GgitIndexEntry *
ggit_index_entry_ref (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	g_atomic_int_inc (&entry->ref_count);

	return entry;
}

/**
 * ggit_index_entry_unref:
 * @entry: a #GgitIndexEntry.
 *
 * Atomically decrements the reference count of @entry by one.
 * If the reference count drops to 0, @entry is freed.
 **/
void
ggit_index_entry_unref (GgitIndexEntry *entry)
{
	g_return_if_fail (entry != NULL);

	if (g_atomic_int_dec_and_test (&entry->ref_count))
	{
		if (entry->owned)
		{
			g_free ((gchar *)entry->entry->path);
			g_slice_free (git_index_entry, entry->entry);
		}

		g_slice_free (GgitIndexEntry, entry);
	}
}

/**
 * ggit_index_entries_ref:
 * @entries: a #GgitIndexEntries.
 *
 * Atomically increments the reference count of @entries by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (nullable): a #GgitIndexEntries or %NULL.
 **/
GgitIndexEntries *
ggit_index_entries_ref (GgitIndexEntries *entries)
{
	g_return_val_if_fail (entries != NULL, NULL);

	g_atomic_int_inc (&entries->ref_count);

	return entries;
}

/**
 * ggit_index_entries_unref:
 * @entries: a #GgitIndexEntries.
 *
 * Atomically decrements the reference count of @entries by one.
 * If the reference count drops to 0, @entries is freed.
 **/
void
ggit_index_entries_unref (GgitIndexEntries *entries)
{
	g_return_if_fail (entries != NULL);

	if (g_atomic_int_dec_and_test (&entries->ref_count))
	{
		g_clear_object (&entries->owner);
		g_slice_free (GgitIndexEntries, entries);
	}
}

/**
 * ggit_index_entries_get_by_index:
 * @entries: a #GgitIndexEntries.
 * @idx: the index of the entry.
 *
 * Get a #GgitIndexEntry by index. Note that the returned #GgitIndexEntry is
 * _only_ valid as long as:
 *
 * 1) The associated index has been closed
 * 2) The entry has not been removed (see ggit_index_remove())
 * 3) The index has not been refreshed (see ggit_index_read())
 *
 * Changes to the #GgitIndexEntry will be reflected in the index once written
 * back to disk using ggit_index_write().
 *
 * Returns: (transfer full) (nullable): a #GgitIndexEntry or %NULL if out of bounds.
 *
 **/
GgitIndexEntry *
ggit_index_entries_get_by_index (GgitIndexEntries *entries,
                                 gsize             idx)
{
	git_index *gidx;
	const git_index_entry *entry;

	g_return_val_if_fail (entries != NULL, NULL);

	gidx = _ggit_index_get_index (entries->owner);
	entry = git_index_get_byindex (gidx, idx);

	if (entry)
	{
		return ggit_index_entry_wrap ((git_index_entry *)entry, FALSE);
	}
	else
	{
		return NULL;
	}
}

/**
 * ggit_index_entries_get_by_path:
 * @entries: a #GgitIndexEntries.
 * @file: the path to search.
 * @stage: stage to search.
 *
 * Get a #GgitIndexEntry by index. Note that the returned #GgitIndexEntry is
 * _only_ valid as long as:
 *
 * 1) The associated index has not been closed
 * 2) The entry has not been removed (see ggit_index_remove())
 * 3) The index has not been refreshed (see ggit_index_read())
 *
 * Changes to the #GgitIndexEntry will be reflected in the index once written
 * back to disk using ggit_index_write().
 *
 * @stage indicates the stage to search the file for. Stages are used in the
 * index when merge conflicts occur, such that multiple versions of the same
 * file can be represented in the index. Stage 0 is associated with the working
 * tree, while stages 1 to 3 are associated with the various versions of the
 * file in a merge conflict. The special value -1 can be used to match the first
 * file encountered in any stage.
 *
 * Returns: (transfer full) (nullable): a #GgitIndexEntry or %NULL if it was not found.
 *
 **/
GgitIndexEntry *
ggit_index_entries_get_by_path (GgitIndexEntries *entries,
                                GFile            *file,
                                gint              stage)
{
	git_index *gidx;
	const git_index_entry *entry;
	gchar *path;
	GgitRepository *repo;
	GFile *wd;

	g_return_val_if_fail (entries != NULL, NULL);
	g_return_val_if_fail (G_IS_FILE (file), NULL);
	g_return_val_if_fail (stage >= 0 && stage <= 3, NULL);

	repo = ggit_index_get_owner (entries->owner);

	wd = ggit_repository_get_workdir (repo);
	path = g_file_get_relative_path (wd, file);

	g_object_unref (wd);
	g_object_unref (repo);

	g_return_val_if_fail (path != NULL, NULL);

	gidx = _ggit_index_get_index (entries->owner);
	entry = git_index_get_bypath (gidx, path, stage);
	g_free (path);

	if (entry)
	{
		return ggit_index_entry_wrap ((git_index_entry *)entry, FALSE);
	}
	else
	{
		return NULL;
	}
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
 * ggit_index_entry_set_dev:
 * @entry: a #GgitIndexEntry.
 * @dev: the dev.
 *
 * Set the dev of the index entry.
 *
 **/
void
ggit_index_entry_set_dev (GgitIndexEntry *entry,
                          guint           dev)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->dev = dev;
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
 * ggit_index_entry_set_ino:
 * @entry: a #GgitIndexEntry.
 * @ino: the ino.
 *
 * Set the ino of the index entry.
 *
 **/
void
ggit_index_entry_set_ino (GgitIndexEntry *entry,
                          guint           ino)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->ino = ino;
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
 * ggit_index_entry_set_mode:
 * @entry: a #GgitIndexEntry.
 * @mode: the mode.
 *
 * Set the mode of the index entry.
 *
 **/
void
ggit_index_entry_set_mode (GgitIndexEntry *entry,
                           guint           mode)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->mode = mode;
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
 * ggit_index_entry_set_uid:
 * @entry: a #GgitIndexEntry.
 * @uid: the uid.
 *
 * Set the uid of the index entry.
 *
 **/
void
ggit_index_entry_set_uid (GgitIndexEntry *entry,
                          guint           uid)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->uid = uid;
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
 * ggit_index_entry_set_gid:
 * @entry: a #GgitIndexEntry.
 * @gid: the gid.
 *
 * Set the gid of the index entry.
 *
 **/
void
ggit_index_entry_set_gid (GgitIndexEntry *entry,
                          guint           gid)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->gid = gid;
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
 * ggit_index_entry_set_file_size:
 * @entry: a #GgitIndexEntry.
 * @file_size: the file size.
 *
 * Set the file size of the index entry.
 *
 **/
void
ggit_index_entry_set_file_size (GgitIndexEntry *entry,
                                goffset         file_size)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->file_size = file_size;
}

/**
 * ggit_index_entry_get_id:
 * @entry: a #GgitIndexEntry.
 *
 * Get the oid of the index entry.
 *
 * Returns: (transfer full) (nullable): the oid or %NULL.
 *
 **/
GgitOId *
ggit_index_entry_get_id (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return _ggit_oid_wrap (&entry->entry->id);
}

/**
 * ggit_index_entry_set_id:
 * @entry: a #GgitIndexEntry.
 * @id: (allow-none): the oid.
 *
 * Set the oid of the index entry.
 *
 **/
void
ggit_index_entry_set_id (GgitIndexEntry *entry,
                         GgitOId        *id)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	if (id != NULL)
	{
		entry->entry->id = *_ggit_oid_get_oid (id);
	}
	else
	{
		memset (&entry->entry->id, 0, sizeof (git_oid));
	}
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
 * ggit_index_entry_set_flags:
 * @entry: a #GgitIndexEntry.
 * @flags: the flags.
 *
 * Set the flags of the index entry.
 *
 **/
void
ggit_index_entry_set_flags (GgitIndexEntry *entry,
                            guint           flags)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->flags = flags;
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
 * ggit_index_entry_set_flags_extended:
 * @entry: a #GgitIndexEntry.
 * @flags_extended: the extended flags.
 *
 * Set the extended flags of the index entry.
 *
 **/
void
ggit_index_entry_set_flags_extended (GgitIndexEntry *entry,
                                     guint           flags_extended)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	entry->entry->flags_extended = flags_extended;
}

/**
 * ggit_index_entry_get_file:
 * @entry: a #GgitIndexEntry.
 *
 * Get the path of the index entry. The path is relative to the working
 * directory.
 *
 * Returns: the path.
 *
 **/
const gchar *
ggit_index_entry_get_path (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->path;
}

/**
 * ggit_index_entry_set_path:
 * @entry: a #GgitIndexEntry.
 * @path: (allow-none): the path.
 *
 * Set the path of the index entry. The path should be relative to the working
 * directory.
 *
 **/
void
ggit_index_entry_set_path (GgitIndexEntry *entry,
                           const gchar    *path)
{
	g_return_if_fail (entry != NULL);
	g_return_if_fail (entry->owned);

	if (entry->entry->path)
	{
		g_free ((gchar *)entry->entry->path);
		entry->entry->path = NULL;
	}

	if (path)
	{
		entry->entry->path = g_strdup (path);
	}
}

static git_filemode_t
file_info_to_filemode (GFileInfo *info)
{
	git_filemode_t ret = 0;

	if (g_file_info_get_is_symlink (info))
	{
		ret |= GIT_FILEMODE_LINK;
	}
	else if (g_file_info_get_file_type (info) == G_FILE_TYPE_DIRECTORY)
	{
		ret |= GIT_FILEMODE_TREE;
	}
	else if (g_file_info_get_attribute_boolean (info, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE))
	{
		ret |= GIT_FILEMODE_BLOB_EXECUTABLE;
	}
	else
	{
		ret |= GIT_FILEMODE_BLOB;
	}

	return ret;
}

/**
 * ggit_index_entry_stat:
 * @entry: a #GgitIndexEntry.
 * @file: the file to stat.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Fill the entry fields from statting @file.
 *
 * Returns: %TRUE if the entry was filled from statting @file successfully, %FALSE otherwise.
 *
 **/
gboolean
ggit_index_entry_stat (GgitIndexEntry  *entry,
                       GFile           *file,
                       GError         **error)
{
	GFileInfo *info;

	g_return_val_if_fail (entry != NULL, FALSE);
	g_return_val_if_fail (G_IS_FILE (file), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
	g_return_val_if_fail (entry->owned, FALSE);

	info = g_file_query_info (file,
	                          G_FILE_ATTRIBUTE_STANDARD_SIZE ","
	                          G_FILE_ATTRIBUTE_TIME_MODIFIED ","
	                          G_FILE_ATTRIBUTE_TIME_MODIFIED_USEC ","
	                          G_FILE_ATTRIBUTE_TIME_CHANGED ","
	                          G_FILE_ATTRIBUTE_TIME_CHANGED_USEC ","
	                          G_FILE_ATTRIBUTE_UNIX_DEVICE ","
	                          G_FILE_ATTRIBUTE_UNIX_INODE ","
	                          G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE ","
	                          G_FILE_ATTRIBUTE_UNIX_UID ","
	                          G_FILE_ATTRIBUTE_UNIX_GID,
	                          G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
	                          NULL,
	                          error);

	if (!info)
	{
		return FALSE;
	}

	entry->entry->file_size = g_file_info_get_size (info);

	entry->entry->mtime.seconds =
		g_file_info_get_attribute_uint64 (info,
		                                  G_FILE_ATTRIBUTE_TIME_MODIFIED);

	entry->entry->mtime.nanoseconds =
		g_file_info_get_attribute_uint32 (info,
		                                  G_FILE_ATTRIBUTE_TIME_MODIFIED_USEC) * 1e3;

	entry->entry->ctime.seconds =
		g_file_info_get_attribute_uint64 (info,
		                                  G_FILE_ATTRIBUTE_TIME_CHANGED);

	entry->entry->ctime.nanoseconds =
		g_file_info_get_attribute_uint32 (info,
		                                  G_FILE_ATTRIBUTE_TIME_CHANGED_USEC) * 1e3;

	entry->entry->dev =
		g_file_info_get_attribute_uint32 (info,
		                                  G_FILE_ATTRIBUTE_UNIX_DEVICE);

	entry->entry->ino =
		g_file_info_get_attribute_uint64 (info,
		                                  G_FILE_ATTRIBUTE_UNIX_INODE);

	entry->entry->mode = file_info_to_filemode (info);

	entry->entry->uid =
		g_file_info_get_attribute_uint32 (info,
		                                  G_FILE_ATTRIBUTE_UNIX_UID);

	entry->entry->gid =
		g_file_info_get_attribute_uint32 (info,
		                                  G_FILE_ATTRIBUTE_UNIX_GID);

	g_object_unref (info);
	return TRUE;
}

/**
 * ggit_index_entry_set_commit:
 * @entry: a #GgitIndexEntry.
 * @commit: a #GgitCommit.
 *
 * Set the index entry to point to a given commit. This sets the index entry
 * id to the commit id, changes the mode to #GGIT_FILE_MODE_COMMIT and updates
 * the timestamps to when the commit was made.
 *
 **/
void
ggit_index_entry_set_commit (GgitIndexEntry    *entry,
                             GgitCommit        *commit)
{
	GgitSignature *sig;
	gint64 ut;

	g_return_if_fail (entry != NULL);
	g_return_if_fail (GGIT_IS_COMMIT (commit));

	ggit_index_entry_set_id (entry, ggit_object_get_id (GGIT_OBJECT (commit)));
	ggit_index_entry_set_mode (entry, GIT_FILEMODE_COMMIT);

	sig = ggit_commit_get_committer (commit);
	ut = g_date_time_to_unix (ggit_signature_get_time (sig));

	entry->entry->ctime.seconds = ut;
	entry->entry->ctime.nanoseconds = 0;

	entry->entry->mtime.seconds = ut;
	entry->entry->mtime.nanoseconds = 0;

	g_object_unref (sig);
}

/**
 * ggit_index_entry_is_conflict:
 * @entry: a #GgitIndexEntry.
 *
 * Get whether the entry represents a conflict.
 *
 * Returns: %TRUE if the entry is a conflict, or %FALSE otherwise.
 *
 **/
gboolean
ggit_index_entry_is_conflict (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, FALSE);

	return git_index_entry_is_conflict (entry->entry);
}

const git_index_entry *
_ggit_index_entry_get_native (GgitIndexEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return entry->entry;
}

/* ex:set ts=8 noet: */
