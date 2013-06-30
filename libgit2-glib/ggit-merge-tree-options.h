/*
 * ggit-merge_tree-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_MERGE_TREE_OPTIONS_H__
#define __GGIT_MERGE_TREE_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_MERGE_TREE_OPTIONS       (ggit_merge_tree_options_get_type ())
#define GGIT_MERGE_TREE_OPTIONS(obj)       ((GgitMergeTreeOptions *)obj)

GType                  ggit_merge_tree_options_get_type                (void) G_GNUC_CONST;

const git_merge_tree_opts *
                      _ggit_merge_tree_options_get_merge_tree_options  (GgitMergeTreeOptions   *merge_options);

GgitMergeTreeOptions  *ggit_merge_tree_options_copy                    (GgitMergeTreeOptions   *merge_options);
void                   ggit_merge_tree_options_free                    (GgitMergeTreeOptions   *merge_options);

GgitMergeTreeOptions  *ggit_merge_tree_options_new                     (GgitMergeTreeFlags      flags,
                                                                        guint                   rename_threshold,
                                                                        guint                   target_limit,
                                                                        GgitMergeAutomergeMode  automerge_mode);

G_END_DECLS

#endif /* __GGIT_MERGE_TREE_OPTIONS_H__ */

/* ex:set ts=8 noet: */
