/*
 * ggit-index.h
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

#ifndef __GGIT_INDEX_H__
#define __GGIT_INDEX_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <git2.h>
#include <libgit2-glib/ggit-types.h>
#include "ggit-native.h"

G_BEGIN_DECLS

#define GGIT_TYPE_INDEX (ggit_index_get_type ())
G_DECLARE_FINAL_TYPE (GgitIndex, ggit_index, GGIT, INDEX, GgitNative)

git_index                *_ggit_index_get_index               (GgitIndex  *idx);
GgitIndex                *_ggit_index_wrap                    (git_index  *idx);

GgitIndex                *ggit_index_open                     (GFile      *file,
                                                               GError    **error);

gboolean                  ggit_index_read                     (GgitIndex  *idx,
                                                               gboolean    force,
                                                               GError    **error);

gboolean                  ggit_index_write                    (GgitIndex  *idx,
                                                               GError    **error);

GgitOId                  *ggit_index_write_tree               (GgitIndex  *idx,
                                                               GError    **error);

GgitOId                  *ggit_index_write_tree_to            (GgitIndex       *idx,
                                                               GgitRepository  *repository,
                                                               GError         **error);

gboolean                  ggit_index_remove                   (GgitIndex  *idx,
                                                               GFile      *file,
                                                               gint        stage,
                                                               GError    **error);

gboolean                  ggit_index_add                      (GgitIndex       *idx,
                                                               GgitIndexEntry  *entry,
                                                               GError         **error);

gboolean                  ggit_index_add_file                 (GgitIndex       *idx,
                                                               GFile           *file,
                                                               GError         **error);

gboolean                  ggit_index_add_path                 (GgitIndex       *idx,
                                                               const gchar     *path,
                                                               GError         **error);

GgitRepository           *ggit_index_get_owner                (GgitIndex  *idx);

gboolean                  ggit_index_has_conflicts            (GgitIndex  *idx);

GgitIndexEntries         *ggit_index_get_entries              (GgitIndex  *idx);
GgitIndexEntriesResolveUndo *
                          ggit_index_get_entries_resolve_undo (GgitIndex  *idx);

G_END_DECLS

#endif /* __GGIT_INDEX_H__ */

/* ex:set ts=8 noet: */
