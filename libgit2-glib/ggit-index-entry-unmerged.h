/*
 * ggit-index-entry-unmerged.h
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

#ifndef __GGIT_INDEX_ENTRY_UNMERGED_H__
#define __GGIT_INDEX_ENTRY_UNMERGED_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <libgit2-glib/ggit-oid.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_INDEX_ENTRY_UNMERGED       (ggit_index_entry_unmerged_get_type ())
#define GGIT_INDEX_ENTRY_UNMERGED(obj)       ((GgitIndexEntryUnmerged *)obj)
#define GGIT_INDEX_ENTRY_UNMERGED_CONST(obj) ((GgitIndexEntryUnmerged const *)obj)

#define GGIT_TYPE_INDEX_ENTRIES_UNMERGED       (ggit_index_entries_unmerged_get_type ())
#define GGIT_INDEX_ENTRIES_UNMERGED(obj)       ((GgitIndexEntriesUnmerged *)obj)
#define GGIT_INDEX_ENTRIES_UNMERGED_CONST(obj) ((GgitIndexEntriesUnmerged const *)obj)

typedef struct _GgitIndexEntriesUnmerged GgitIndexEntriesUnmerged;
typedef struct _GgitIndexEntryUnmerged   GgitIndexEntryUnmerged;

GgitIndexEntriesUnmerged *
	_ggit_index_entries_unmerged_new        (GgitIndex                *owner);

GgitIndexEntriesUnmerged *
	ggit_index_entries_unmerged_copy        (GgitIndexEntriesUnmerged *entries);

GgitIndexEntryUnmerged *
	ggit_index_entries_unmerged_get         (GgitIndexEntriesUnmerged *entries,
                                                 guint                     idx);

GgitIndexEntryUnmerged *
	ggit_index_entries_unmerged_get_by_file (GgitIndexEntriesUnmerged *entries,
                                                 GFile                    *file);

GgitIndexEntryUnmerged *
	ggit_index_entry_unmerged_copy       (GgitIndexEntryUnmerged   *entry);

GType    ggit_index_entries_unmerged_get_type (void) G_GNUC_CONST;
void     ggit_index_entries_unmerged_free     (GgitIndexEntriesUnmerged *entries);
guint    ggit_index_entries_unmerged_size     (GgitIndexEntriesUnmerged *entries);

GType    ggit_index_entry_unmerged_get_type   (void) G_GNUC_CONST;
void     ggit_index_entry_unmerged_free       (GgitIndexEntryUnmerged   *entry);

guint    ggit_index_entry_unmerged_get_mode   (GgitIndexEntryUnmerged   *entry,
                                               gint                      stage);

GgitOId *ggit_index_entry_unmerged_get_id     (GgitIndexEntryUnmerged   *entry,
                                               gint                      stage);

GFile   *ggit_index_entry_unmerged_get_file   (GgitIndexEntryUnmerged   *entry);


G_END_DECLS

#endif /* __GGIT_INDEX_ENTRY_UNMERGED_H__ */

/* ex:set ts=8 noet: */
