/*
 * ggit-index-entry.h
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

#ifndef __GGIT_INDEX_ENTRY_H__
#define __GGIT_INDEX_ENTRY_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <libgit2-glib/ggit-oid.h>

G_BEGIN_DECLS

typedef struct _GgitIndexEntries GgitIndexEntries;
typedef struct _GgitIndexEntry   GgitIndexEntry;

struct _GgitIndex;

GType             ggit_index_entries_get_type         (void) G_GNUC_CONST;
GgitIndexEntries *_ggit_index_entries_new             (struct _GgitIndex *owner);
GgitIndexEntries *ggit_index_entries_copy             (GgitIndexEntries  *entries);
void              ggit_index_entries_free             (GgitIndexEntries  *entries);

GgitIndexEntry   *ggit_index_entries_get              (GgitIndexEntries  *entries,
                                                       guint              idx);

guint             ggit_index_entries_size             (GgitIndexEntries  *entries);

GType             ggit_index_entry_get_type           (void) G_GNUC_CONST;
GgitIndexEntry *  ggit_index_entry_copy               (GgitIndexEntry    *entry);
void              ggit_index_entry_free               (GgitIndexEntry    *entry);

guint             ggit_index_entry_get_dev            (GgitIndexEntry    *entry);
guint             ggit_index_entry_get_ino            (GgitIndexEntry    *entry);
guint             ggit_index_entry_get_mode           (GgitIndexEntry    *entry);
guint             ggit_index_entry_get_uid            (GgitIndexEntry    *entry);
guint             ggit_index_entry_get_gid            (GgitIndexEntry    *entry);
goffset           ggit_index_entry_get_file_size      (GgitIndexEntry    *entry);

GgitOId          *ggit_index_entry_get_oid            (GgitIndexEntry    *entry);

guint             ggit_index_entry_get_flags          (GgitIndexEntry    *entry);
guint             ggit_index_entry_get_flags_extended (GgitIndexEntry    *entry);

GFile            *ggit_index_entry_get_file           (GgitIndexEntry    *entry);

G_END_DECLS

#endif /* __GGIT_INDEX_ENTRY_H__ */

/* ex:set ts=8 noet: */
