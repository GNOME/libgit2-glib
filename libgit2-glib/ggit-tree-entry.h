/*
 * ggit-tree-entry.h
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

#ifndef __GGIT_TREE_ENTRY_H__
#define __GGIT_TREE_ENTRY_H__

#include <git2.h>
#include <glib-object.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_TREE_ENTRY       (ggit_tree_entry_get_type ())
#define GGIT_TREE_ENTRY(obj)       ((GgitTreeEntry *)obj)

GType          ggit_tree_entry_get_type        (void) G_GNUC_CONST;

GgitTreeEntry *_ggit_tree_entry_wrap           (git_tree_entry       *entry,
                                                gboolean              free_entry);
GgitTreeEntry *ggit_tree_entry_ref             (GgitTreeEntry        *entry);
void           ggit_tree_entry_unref           (GgitTreeEntry        *entry);

GgitFileMode   ggit_tree_entry_get_file_mode   (GgitTreeEntry        *entry);
GgitOId       *ggit_tree_entry_get_id          (GgitTreeEntry        *entry);
const gchar *  ggit_tree_entry_get_name        (GgitTreeEntry        *entry);

GType          ggit_tree_entry_get_object_type (GgitTreeEntry        *entry);

G_END_DECLS

#endif /* __GGIT_TREE_ENTRY_H__ */

/* ex:set ts=8 noet: */
