/*
 * ggit-index-entry.h
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

#ifndef __GGIT_INDEX_ENTRY_H__
#define __GGIT_INDEX_ENTRY_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <libgit2-glib/ggit-oid.h>
#include <libgit2-glib/ggit-types.h>
#include <git2.h>

G_BEGIN_DECLS

#define GGIT_TYPE_INDEX_ENTRY       (ggit_index_entry_get_type ())
#define GGIT_INDEX_ENTRY(obj)       ((GgitIndexEntry *)obj)

#define GGIT_TYPE_INDEX_ENTRIES       (ggit_index_entries_get_type ())
#define GGIT_INDEX_ENTRIES(obj)       ((GgitIndexEntries *)obj)

GType             ggit_index_entries_get_type         (void) G_GNUC_CONST;
GgitIndexEntries *_ggit_index_entries_wrap            (GgitIndex         *owner);
GgitIndexEntries *ggit_index_entries_ref              (GgitIndexEntries  *entries);
void              ggit_index_entries_unref            (GgitIndexEntries  *entries);

GgitIndexEntry   *ggit_index_entries_get_by_index     (GgitIndexEntries  *entries,
                                                       gsize              idx);

GgitIndexEntry   *ggit_index_entries_get_by_path      (GgitIndexEntries *entries,
                                                       GFile            *file,
                                                       gboolean          stage);

guint             ggit_index_entries_size             (GgitIndexEntries  *entries);

GType             ggit_index_entry_get_type           (void) G_GNUC_CONST;

GgitIndexEntry   *ggit_index_entry_new_for_file       (GFile             *file,
                                                       GgitOId           *id);

GgitIndexEntry   *ggit_index_entry_new_for_path       (const gchar       *path,
                                                       GgitOId           *id);

GgitIndexEntry   *ggit_index_entry_ref                (GgitIndexEntry    *entry);
void              ggit_index_entry_unref              (GgitIndexEntry    *entry);

guint             ggit_index_entry_get_dev            (GgitIndexEntry    *entry);
void              ggit_index_entry_set_dev            (GgitIndexEntry    *entry,
                                                       guint              dev);

guint             ggit_index_entry_get_ino            (GgitIndexEntry    *entry);
void              ggit_index_entry_set_ino            (GgitIndexEntry    *entry,
                                                       guint              ino);

guint             ggit_index_entry_get_mode           (GgitIndexEntry    *entry);
void              ggit_index_entry_set_mode           (GgitIndexEntry    *entry,
                                                       guint              mode);

guint             ggit_index_entry_get_uid            (GgitIndexEntry    *entry);
void              ggit_index_entry_set_uid            (GgitIndexEntry    *entry,
                                                       guint              uid);

guint             ggit_index_entry_get_gid            (GgitIndexEntry    *entry);
void              ggit_index_entry_set_gid            (GgitIndexEntry    *entry,
                                                       guint              gid);

goffset           ggit_index_entry_get_file_size      (GgitIndexEntry    *entry);
void              ggit_index_entry_set_file_size      (GgitIndexEntry    *entry,
                                                       goffset            file_size);

GgitOId          *ggit_index_entry_get_id             (GgitIndexEntry    *entry);
void              ggit_index_entry_set_id             (GgitIndexEntry    *entry,
                                                       GgitOId           *id);

guint             ggit_index_entry_get_flags          (GgitIndexEntry    *entry);
void              ggit_index_entry_set_flags          (GgitIndexEntry    *entry,
                                                       guint              flags);

guint             ggit_index_entry_get_flags_extended (GgitIndexEntry    *entry);
void              ggit_index_entry_set_flags_extended (GgitIndexEntry    *entry,
                                                       guint              flags_extended);

GFile            *ggit_index_entry_get_file           (GgitIndexEntry    *entry);

const git_index_entry *_ggit_index_entry_get_native         (GgitIndexEntry    *entry);

G_END_DECLS

#endif /* __GGIT_INDEX_ENTRY_H__ */

/* ex:set ts=8 noet: */
