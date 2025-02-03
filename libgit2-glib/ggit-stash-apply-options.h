/*
 * ggit-stash-apply-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2025 - Alberto Fanjul
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

#ifndef __GGIT_STASH_APPLY_OPTIONS_H__
#define __GGIT_STASH_APPLY_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_STASH_APPLY_OPTIONS       (ggit_stash_apply_options_get_type ())
#define GGIT_STASH_APPLY_OPTIONS(obj)       ((GgitStashApplyOptions *)obj)

GType              ggit_stash_apply_options_get_type           (void) G_GNUC_CONST;

git_stash_apply_options *
                  _ggit_stash_apply_options_get_native        (GgitStashApplyOptions *stash_apply_options);

GgitStashApplyOptions *ggit_stash_apply_options_copy               (GgitStashApplyOptions *stash_apply_options);
void              ggit_stash_apply_options_free               (GgitStashApplyOptions *stash_apply_options);

GgitStashApplyOptions *ggit_stash_apply_options_new                (void);

GgitStashApplyFlags    ggit_stash_apply_get_flags                  (GgitStashApplyOptions *stash_apply_options);
void              ggit_stash_apply_set_flags                  (GgitStashApplyOptions *stash_apply_options,
                                                         GgitStashApplyFlags    flags);

GgitOId          *ggit_stash_apply_options_get_newest_commit  (GgitStashApplyOptions *stash_apply_options);
void              ggit_stash_apply_options_set_newest_commit  (GgitStashApplyOptions *stash_apply_options,
                                                         GgitOId          *oid);

GgitOId          *ggit_stash_apply_options_get_oldest_commit  (GgitStashApplyOptions *stash_apply_options);
void              ggit_stash_apply_options_set_oldest_commit  (GgitStashApplyOptions *stash_apply_options,
                                                         GgitOId          *oid);

guint32           ggit_stash_apply_options_get_minimum_line   (GgitStashApplyOptions *stash_apply_options);
void              ggit_stash_apply_options_set_minimum_line   (GgitStashApplyOptions *stash_apply_options,
                                                         guint32           line);

guint32           ggit_stash_apply_options_get_maximum_line   (GgitStashApplyOptions *stash_apply_options);
void              ggit_stash_apply_options_set_maximum_line   (GgitStashApplyOptions *stash_apply_options,
                                                         guint32           line);

guint16           ggit_stash_apply_options_get_minimum_match_characters
                                                        (GgitStashApplyOptions *stash_apply_options);

void              ggit_stash_apply_options_set_minimum_match_characters
                                                        (GgitStashApplyOptions *stash_apply_options,
                                                         guint16           characters);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitStashApplyOptions, ggit_stash_apply_options_free)

G_END_DECLS

#endif /* __GGIT_STASH_APPLY_OPTIONS_H__ */

/* ex:set ts=8 noet: */
