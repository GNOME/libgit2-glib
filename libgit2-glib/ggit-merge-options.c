/*
 * ggit-merge-options.c
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

#include "ggit-merge-options.h"
#include "ggit-diff-similarity-metric.h"

struct _GgitMergeOptions
{
	GgitDiffSimilarityMetric *metric;
	git_merge_options merge_options;
};

G_DEFINE_BOXED_TYPE (GgitMergeOptions, ggit_merge_options,
                     ggit_merge_options_copy, ggit_merge_options_free)

const git_merge_options *
_ggit_merge_options_get_merge_options (GgitMergeOptions *merge_options)
{
	/* NULL is common for merge_options as it specifies to use the default
	 * so handle a NULL merge_options here instead of in every caller.
	 */
	if (merge_options == NULL)
	{
		return NULL;
	}

	return (const git_merge_options *)&merge_options->merge_options;
}

/**
 * ggit_merge_options_copy:
 * @merge_options: a #GgitMergeOptions.
 *
 * Copies @merge_options into a newly allocated #GgitMergeOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitMergeOptions.
 */
GgitMergeOptions *
ggit_merge_options_copy (GgitMergeOptions *merge_options)
{
	GgitMergeOptions *new_merge_options;

	g_return_val_if_fail (merge_options != NULL, NULL);

	new_merge_options = g_slice_new0 (GgitMergeOptions);
	new_merge_options->merge_options = merge_options->merge_options;

	new_merge_options->merge_options.metric = NULL;
	ggit_merge_options_set_similarity_metric (new_merge_options, merge_options->metric);

	return new_merge_options;
}

/**
 * ggit_merge_options_free:
 * @merge_options: a #GgitMergeOptions.
 *
 * Frees @merge_options.
 */
void
ggit_merge_options_free (GgitMergeOptions *merge_options)
{
	g_return_if_fail (merge_options != NULL);

	if (merge_options->metric != NULL)
	{
		ggit_diff_similarity_metric_free (merge_options->metric);
		merge_options->metric = NULL;
	}

	g_slice_free (GgitMergeOptions, merge_options);
}

/**
 * ggit_merge_options_new:
 *
 * Creates a new #GgitMergeOptions.
 *
 * Returns: a newly allocated #GgitMergeOptions.
 */
GgitMergeOptions *
ggit_merge_options_new ()
{
	GgitMergeOptions *merge_options;
	git_merge_options gmerge_options = GIT_MERGE_OPTIONS_INIT;

	merge_options = g_slice_new0 (GgitMergeOptions);
	merge_options->merge_options = gmerge_options;

	return merge_options;
}

/**
 * ggit_merge_options_set_tree_flags:
 * @merge_options: a #GgitMergeOptions.
 * @flags: the flags.
 *
 * Set the flags to use for merging.
 *
 **/
void
ggit_merge_options_set_flags (GgitMergeOptions   *merge_options,
                              GgitMergeFlags      flags)
{
	g_return_if_fail (merge_options != NULL);
	merge_options->merge_options.flags = (git_merge_flag_t)flags;
}

/**
 * ggit_merge_options_get_flags:
 * @merge_options: a #GgitMergeOptions.
 *
 * Get the tree flags to use for merging.
 *
 * Returns: the flags.
 *
 **/
GgitMergeFlags
ggit_merge_options_get_flags (GgitMergeOptions *merge_options)
{
	g_return_val_if_fail (merge_options != NULL, 0);
	return (GgitMergeFlags)merge_options->merge_options.flags;
}

/**
 * ggit_merge_options_set_rename_threshold:
 * @merge_options: a #GgitMergeOptions.
 * @rename_threshold: similarity to consider a file renamed.
 *
 * Set the rename threshold (defaults to 50). If %GGIT_MERGE_TREE_FIND_RENAMES
 * is enabled, added files will be compared with deleted files to
 * determine their similarity. Files that are more similar than the rename
 * threshold (percentage-wise) will be treated as a rename.
 *
 **/
void
ggit_merge_options_set_rename_threshold (GgitMergeOptions *merge_options,
                                         guint             rename_threshold)
{
	g_return_if_fail (merge_options != NULL);
	merge_options->merge_options.rename_threshold = rename_threshold;
}

/**
 * ggit_merge_options_get_rename_threshold:
 * @merge_options: a #GgitMergeOptions.
 *
 * Get the rename threshold (defaults to 50). If %GGIT_MERGE_TREE_FIND_RENAMES
 * is enabled, added files will be compared with deleted files to
 * determine their similarity. Files that are more similar than the rename
 * threshold (percentage-wise) will be treated as a rename.
 *
 * Returns: the rename threshold.
 *
 **/
guint
ggit_merge_options_get_rename_threshold (GgitMergeOptions *merge_options)
{
	g_return_val_if_fail (merge_options != NULL, 0);
	return merge_options->merge_options.rename_threshold;
}

/**
 * ggit_merge_options_set_target_limit:
 * @merge_options: a #GgitMergeOptions.
 * @target_limit: maximum similarity source to examine for renames.
 *
 * Set the maximum number of similarity sources to examine for renames (defaults to 200).
 * If the number of rename candidates (add / delete pairs) is greater than
 * this value, inexact rename detection is aborted.
 *
 **/
void
ggit_merge_options_set_target_limit (GgitMergeOptions *merge_options,
                                     guint             target_limit)
{
	g_return_if_fail (merge_options != NULL);
	merge_options->merge_options.target_limit = target_limit;
}

/**
 * ggit_merge_options_get_target_limit:
 * @merge_options: a #GgitMergeOptions.
 *
 * Get the maximum number of similarity sources to examine for renames (defaults to 200).
 * If the number of rename candidates (add / delete pairs) is greater than
 * this value, inexact rename detection is aborted.
 *
 * Returns: the target limit.
 *
 **/
guint
ggit_merge_options_get_target_limit (GgitMergeOptions *merge_options)
{
	g_return_val_if_fail (merge_options != NULL, 0);
	return merge_options->merge_options.target_limit;
}

/**
 * ggit_merge_options_set_similarity_metric:
 * @merge_options: a #GgitMergeOptions.
 * @metric: a #GgitSimilarityMetric.
 *
 * Set the similarity metric, or %NULL for the default similarity metric.
 *
 **/
void
ggit_merge_options_set_similarity_metric (GgitMergeOptions         *merge_options,
                                          GgitDiffSimilarityMetric *metric)
{
	g_return_if_fail (merge_options != NULL);

	if (merge_options->metric)
	{
		ggit_diff_similarity_metric_free (merge_options->metric);
	}

	merge_options->metric = metric != NULL ? ggit_diff_similarity_metric_copy (metric) : NULL;
	merge_options->merge_options.metric = _ggit_diff_similarity_metric_get_similarity_metric (metric);
}

/**
 * ggit_merge_options_get_similarity_metric:
 * @merge_options: a #GgitMergeOptions.
 *
 * Get the similarity metric.
 *
 * Returns: (transfer none): the similarity metric, or %NULL.
 *
 **/
GgitDiffSimilarityMetric *
ggit_merge_options_get_similarity_metric (GgitMergeOptions *merge_options)
{
	g_return_val_if_fail (merge_options != NULL, NULL);
	return merge_options->metric;
}

/**
 * ggit_merge_options_set_file_favor:
 * @merge_options: a #GgitMergeOptions.
 * @file_favor: the file favor.
 *
 * Set flags for handling conflicting content.
 *
 **/
void
ggit_merge_options_set_file_favor (GgitMergeOptions   *merge_options,
                                   GgitMergeFileFavor  file_favor)
{
	g_return_if_fail (merge_options != NULL);
	merge_options->merge_options.file_favor = (git_merge_file_favor_t)file_favor;
}

/**
 * ggit_merge_options_get_file_favor:
 * @merge_options: a #GgitMergeOptions.
 *
 * Get flags for handling conflicting content.
 *
 * Returns: the file favor.
 *
 **/
GgitMergeFileFavor
ggit_merge_options_get_file_favor (GgitMergeOptions *merge_options)
{
	g_return_val_if_fail (merge_options != NULL, 0);
	return (GgitMergeFileFavor)merge_options->merge_options.file_favor;
}

/**
 * ggit_merge_options_set_file_flags:
 * @merge_options: a #GgitMergeOptions.
 * @file_flags: the file flags.
 *
 * Set file merging flags.
 *
 **/
void
ggit_merge_options_set_file_flags (GgitMergeOptions   *merge_options,
                                   GgitMergeFileFlags  file_flags)
{
	g_return_if_fail (merge_options != NULL);
	merge_options->merge_options.file_flags = (git_merge_file_flag_t)file_flags;
}

/**
 * ggit_merge_options_get_file_flags:
 * @merge_options: a #GgitMergeOptions.
 *
 * Get file merging flags.
 *
 * Returns: the file merging flags.
 *
 **/
GgitMergeFileFlags
ggit_merge_options_get_file_flags (GgitMergeOptions *merge_options)
{
	g_return_val_if_fail (merge_options != NULL, 0);
	return (GgitMergeFileFlags)merge_options->merge_options.file_flags;
}

/* ex:set ts=8 noet: */
