/*
 * ggit-diff-similarity-metric.c
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


#include "ggit-diff-similarity-metric.h"
#include "ggit-diff-file.h"


struct _GgitDiffSimilarityMetric
{
	git_diff_similarity_metric metric;
	GgitDiffSimilarityMetricFileSignatureCallback file_signature;
	GgitDiffSimilarityMetricBufferSignatureCallback buffer_signature;
	GgitDiffSimilarityMetricFreeSignatureCallback free_signature;
	GgitDiffSimilarityMetricSimilarityCallback similarity;
	gpointer user_data;
};

G_DEFINE_BOXED_TYPE (GgitDiffSimilarityMetric, ggit_diff_similarity_metric,
                     ggit_diff_similarity_metric_copy, ggit_diff_similarity_metric_free)

git_diff_similarity_metric *
_ggit_diff_similarity_metric_get_similarity_metric (GgitDiffSimilarityMetric *metric)
{
	if (metric == NULL)
	{
		return NULL;
	}

	return &metric->metric;
}

/**
 * ggit_diff_similarity_metric_copy:
 * @metric: a #GgitDiffSimilarityMetric.
 *
 * Copies @metric into a newly allocated #GgitDiffSimilarityMetric.
 *
 * Returns: (transfer full): a newly allocated #GgitDiffSimilarityMetric.
 */
GgitDiffSimilarityMetric *
ggit_diff_similarity_metric_copy (GgitDiffSimilarityMetric *metric)
{
	GgitDiffSimilarityMetric *copy;

	g_return_val_if_fail (metric != NULL, NULL);

	copy = g_slice_new (GgitDiffSimilarityMetric);
	copy->metric = metric->metric;
	copy->file_signature = metric->file_signature;
	copy->buffer_signature = metric->buffer_signature;
	copy->free_signature = metric->free_signature;
	copy->similarity = metric->similarity;
	copy->user_data = metric->user_data;

	return copy;
}

/**
 * ggit_diff_similarity_metric_free:
 * @metric: a #GgitDiffSimilarityMetric.
 *
 * Frees @metric.
 */
void
ggit_diff_similarity_metric_free (GgitDiffSimilarityMetric *metric)
{
	g_return_if_fail (metric != NULL);

	g_slice_free (GgitDiffSimilarityMetric, metric);
}

static int
ggit_diff_similarity_metric_file_signature (gpointer            *out,
                                            const git_diff_file *file,
                                            const gchar         *fullpath,
                                            gpointer             payload)
{
	GgitDiffSimilarityMetric *metric = (GgitDiffSimilarityMetric *)payload;
	gint ret;
	GgitDiffFile *diff_file;

	diff_file = _ggit_diff_file_wrap (file);
	ret = metric->file_signature (diff_file, fullpath, out, metric->user_data);
	ggit_diff_file_unref (diff_file);

	return ret;
}

static int
ggit_diff_similarity_metric_buffer_signature (gpointer            *out,
                                              const git_diff_file *file,
                                              const gchar         *buf,
                                              gsize                buflen,
                                              gpointer             payload)
{
	GgitDiffSimilarityMetric *metric = (GgitDiffSimilarityMetric *)payload;
	gint ret;
	GgitDiffFile *diff_file;

	diff_file = _ggit_diff_file_wrap (file);
	ret = metric->buffer_signature (diff_file, buf, buflen, out, metric->user_data);
	ggit_diff_file_unref (diff_file);

	return ret;
}

static void
ggit_diff_similarity_metric_free_signature (gpointer signature,
                                            gpointer payload)
{
	GgitDiffSimilarityMetric *metric = (GgitDiffSimilarityMetric *)payload;

	metric->free_signature (signature, metric->user_data);
}

static int
ggit_diff_similarity_metric_similarity (gint     *score,
                                        gpointer  signature_a,
                                        gpointer  signature_b,
                                        gpointer  payload)
{
	GgitDiffSimilarityMetric *metric = (GgitDiffSimilarityMetric *)payload;

	return metric->similarity (score, signature_a, signature_b, metric->user_data);
}

/**
 * ggit_diff_similarity_metric_new:
 * @file_signature: (scope call): a #GgitDiffSimilarityMetricFileSignature.
 * @buffer_signature: (scope call): a #GgitDiffSimilarityMetricBufferSignature.
 * @free_signature: (scope call): a #GgitDiffSimilarityMetricFreeSignature.
 * @similarity: (scope call): a #GgitDiffSimilarityMetricSimilarity.
 * @user_data: passed as user_data to the previous parameters.
 *
 * Creates a new #GgitDiffSimilarityMetric.
 *
 * Returns: a newly allocated #GgitDiffSimilarityMetric.
 */
GgitDiffSimilarityMetric *
ggit_diff_similarity_metric_new (GgitDiffSimilarityMetricFileSignatureCallback   file_signature,
                                 GgitDiffSimilarityMetricBufferSignatureCallback buffer_signature,
                                 GgitDiffSimilarityMetricFreeSignatureCallback   free_signature,
                                 GgitDiffSimilarityMetricSimilarityCallback      similarity,
                                 gpointer                                        user_data)
{
	GgitDiffSimilarityMetric *metric;

	metric = g_slice_new (GgitDiffSimilarityMetric);
	metric->file_signature = file_signature;
	metric->buffer_signature = buffer_signature;
	metric->free_signature = free_signature;
	metric->similarity = similarity;
	metric->user_data = user_data;

	metric->metric.file_signature = ggit_diff_similarity_metric_file_signature;
	metric->metric.buffer_signature = ggit_diff_similarity_metric_buffer_signature;
	metric->metric.free_signature = ggit_diff_similarity_metric_free_signature;
	metric->metric.similarity = ggit_diff_similarity_metric_similarity;
	metric->metric.payload = (void *)metric;

	return metric;
}

/* ex:set ts=8 noet: */
