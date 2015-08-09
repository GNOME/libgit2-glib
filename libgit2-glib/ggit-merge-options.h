/*
 * ggit-merge-options.h
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


#ifndef __GGIT_MERGE_OPTIONS_H__
#define __GGIT_MERGE_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_MERGE_OPTIONS       (ggit_merge_options_get_type ())
#define GGIT_MERGE_OPTIONS(obj)       ((GgitMergeOptions *)obj)

GType                  ggit_merge_options_get_type                (void) G_GNUC_CONST;

const git_merge_options *
                      _ggit_merge_options_get_merge_options       (GgitMergeOptions         *merge_options);

GgitMergeOptions      *ggit_merge_options_copy                    (GgitMergeOptions         *merge_options);
void                   ggit_merge_options_free                    (GgitMergeOptions         *merge_options);

GgitMergeOptions      *ggit_merge_options_new                     (void);

void                   ggit_merge_options_set_tree_flags          (GgitMergeOptions         *merge_options,
                                                                   GgitMergeTreeFlags        tree_flags);

GgitMergeTreeFlags     ggit_merge_options_get_tree_flags          (GgitMergeOptions         *merge_options);

void                   ggit_merge_options_set_rename_threshold    (GgitMergeOptions         *merge_options,
                                                                   guint                     rename_threshold);

guint                  ggit_merge_options_get_rename_threshold    (GgitMergeOptions         *merge_options);

void                   ggit_merge_options_set_target_limit        (GgitMergeOptions         *merge_options,
                                                                   guint                     target_limit);

guint                  ggit_merge_options_get_target_limit        (GgitMergeOptions         *merge_options);

void                   ggit_merge_options_set_similarity_metric   (GgitMergeOptions         *merge_options,
                                                                   GgitDiffSimilarityMetric *metric);

GgitDiffSimilarityMetric *
                       ggit_merge_options_get_similarity_metric   (GgitMergeOptions         *merge_options);

void                   ggit_merge_options_set_file_favor          (GgitMergeOptions         *merge_options,
                                                                   GgitMergeFileFavor        file_favor);

GgitMergeFileFavor     ggit_merge_options_get_file_favor          (GgitMergeOptions         *merge_options);

void                   ggit_merge_options_set_file_flags          (GgitMergeOptions         *merge_options,
                                                                   GgitMergeFileFlags        file_flags);

GgitMergeFileFlags     ggit_merge_options_get_file_flags          (GgitMergeOptions         *merge_options);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitMergeOptions, ggit_merge_options_free)

G_END_DECLS

#endif /* __GGIT_MERGE_OPTIONS_H__ */

/* ex:set ts=8 noet: */
