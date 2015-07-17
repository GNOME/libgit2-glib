/*
 * ggit-diff-find-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Jesse van den Kieboom
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

#ifndef __GGIT_DIFF_FIND_OPTIONS_H__
#define __GGIT_DIFF_FIND_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_FIND_OPTIONS (ggit_diff_find_options_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitDiffFindOptions, ggit_diff_find_options, GGIT, DIFF_FIND_OPTIONS, GObject)

struct _GgitDiffFindOptionsClass
{
	GObjectClass parent_class;
};

const git_diff_find_options *
                     _ggit_diff_find_options_get_diff_find_options (
                                                       GgitDiffFindOptions *options);

GgitDiffFindOptions *ggit_diff_find_options_new       (void);

GgitDiffFindFlags    ggit_diff_find_options_get_flags (GgitDiffFindOptions *options);
void                 ggit_diff_find_options_set_flags (GgitDiffFindOptions *options,
                                                       GgitDiffFindFlags    flags);

guint                ggit_diff_find_options_get_rename_threshold (
                                                       GgitDiffFindOptions *options);

void                 ggit_diff_find_options_set_rename_threshold (
                                                       GgitDiffFindOptions *options,
                                                       guint                threshold);

guint                ggit_diff_find_options_get_rename_from_rewrite_threshold (
                                                       GgitDiffFindOptions *options);

void                 ggit_diff_find_options_set_rename_from_rewrite_threshold (
                                                       GgitDiffFindOptions *options,
                                                       guint                threshold);

guint                ggit_diff_find_options_get_copy_threshold (
                                                       GgitDiffFindOptions *options);

void                 ggit_diff_find_options_set_copy_threshold (
                                                       GgitDiffFindOptions *options,
                                                       guint                threshold);

gsize                ggit_diff_find_options_get_rename_limit (
                                                       GgitDiffFindOptions *options);

void                 ggit_diff_find_options_set_rename_limit (
                                                       GgitDiffFindOptions *options,
                                                       gsize                limit);

GgitDiffSimilarityMetric *
                     ggit_diff_find_options_get_metric (
                                                       GgitDiffFindOptions *options);

void                 ggit_diff_find_options_set_metric (
                                                       GgitDiffFindOptions *options,
                                                       GgitDiffSimilarityMetric *metric);

G_END_DECLS

#endif /* __GGIT_DIFF_FIND_OPTIONS_H__ */

/* ex:set ts=8 noet: */
