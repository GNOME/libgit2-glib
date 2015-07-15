/*
 * ggit-diff-similarity-metric.h
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


#ifndef __GGIT_DIFF_SIMILARITY_METRIC_H__
#define __GGIT_DIFF_SIMILARITY_METRIC_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_SIMILARITY_METRIC       (ggit_diff_similarity_metric_get_type ())
#define GGIT_DIFF_SIMILARITY_METRIC(obj)       ((GgitDiffSimilarityMetric *)obj)

GType                      ggit_diff_similarity_metric_get_type                (void) G_GNUC_CONST;

git_diff_similarity_metric *
                          _ggit_diff_similarity_metric_get_similarity_metric   (GgitDiffSimilarityMetric   *metric);

GgitDiffSimilarityMetric  *ggit_diff_similarity_metric_copy                    (GgitDiffSimilarityMetric   *metric);
void                       ggit_diff_similarity_metric_free                    (GgitDiffSimilarityMetric   *metric);

GgitDiffSimilarityMetric  *ggit_diff_similarity_metric_new                     (GgitDiffSimilarityMetricFileSignatureCallback   file_signature,
                                                                                GgitDiffSimilarityMetricBufferSignatureCallback buffer_signature,
                                                                                GgitDiffSimilarityMetricFreeSignatureCallback   free_signature,
                                                                                GgitDiffSimilarityMetricSimilarityCallback      similarity,
                                                                                gpointer                                        user_data);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffSimilarityMetric, ggit_diff_similarity_metric_free)

G_END_DECLS

#endif /* __GGIT_DIFF_SIMILARITY_METRIC_H__ */

/* ex:set ts=8 noet: */
