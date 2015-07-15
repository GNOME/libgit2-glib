/*
 * ggit-index-entry-resolve-undo.h
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

#ifndef __GGIT_INDEX_ENTRY_RESOLVE_UNDO_H__
#define __GGIT_INDEX_ENTRY_RESOLVE_UNDO_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <libgit2-glib/ggit-oid.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_INDEX_ENTRY_RESOLVE_UNDO      (ggit_index_entry_resolve_undo_get_type ())
#define GGIT_INDEX_ENTRY_RESOLVE_UNDO(obj)      ((GgitIndexEntryResolveUndo *)obj)

#define GGIT_TYPE_INDEX_ENTRIES_RESOLVE_UNDO    (ggit_index_entries_resolve_undo_get_type ())
#define GGIT_INDEX_ENTRIES_RESOLVE_UNDO(obj)    ((GgitIndexEntriesResolveUndo *)obj)

GgitIndexEntriesResolveUndo *
        _ggit_index_entries_resolve_undo_wrap       (GgitIndex                   *owner);

GgitIndexEntryResolveUndo *
         ggit_index_entries_resolve_undo_get        (GgitIndexEntriesResolveUndo *entries,
                                                     guint                        idx);

GgitIndexEntryResolveUndo *
         ggit_index_entries_resolve_undo_get_by_file (GgitIndexEntriesResolveUndo *entries,
                                                      GFile                       *file);

GType    ggit_index_entries_resolve_undo_get_type    (void) G_GNUC_CONST;

GgitIndexEntriesResolveUndo *
         ggit_index_entries_resolve_undo_ref         (GgitIndexEntriesResolveUndo *entries);
void     ggit_index_entries_resolve_undo_unref       (GgitIndexEntriesResolveUndo *entries);
guint    ggit_index_entries_resolve_undo_size        (GgitIndexEntriesResolveUndo *entries);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitIndexEntriesResolveUndo, ggit_index_entries_resolve_undo_unref)

GType    ggit_index_entry_resolve_undo_get_type      (void) G_GNUC_CONST;

GgitIndexEntryResolveUndo *
         ggit_index_entry_resolve_undo_ref           (GgitIndexEntryResolveUndo   *entry);
void     ggit_index_entry_resolve_undo_unref         (GgitIndexEntryResolveUndo   *entry);

guint    ggit_index_entry_resolve_undo_get_mode      (GgitIndexEntryResolveUndo   *entry,
                                                      gint                         stage);

GgitOId *ggit_index_entry_resolve_undo_get_id        (GgitIndexEntryResolveUndo   *entry,
                                                      gint                         stage);

GFile   *ggit_index_entry_resolve_undo_get_file      (GgitIndexEntryResolveUndo   *entry);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitIndexEntryResolveUndo, ggit_index_entry_resolve_undo_unref)

G_END_DECLS

#endif /* __GGIT_INDEX_ENTRY_RESOLVE_UNDO_H__ */

/* ex:set ts=8 noet: */
