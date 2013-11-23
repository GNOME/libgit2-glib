/*
 * ggit-merge_tree-options.c
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

#include "ggit-merge-tree-options.h"
#include "ggit-diff-similarity-metric.h"

struct _GgitMergeTreeOptions
{
	git_merge_tree_opts merge_options;
};

G_DEFINE_BOXED_TYPE (GgitMergeTreeOptions, ggit_merge_tree_options,
                     ggit_merge_tree_options_copy, ggit_merge_tree_options_free)

const git_merge_tree_opts *
_ggit_merge_tree_options_get_merge_tree_options (GgitMergeTreeOptions *merge_options)
{
	/* NULL is common for merge_tree_options as it specifies to use the default
	 * so handle a NULL merge_tree_options here instead of in every caller.
	 */
	if (merge_options == NULL)
	{
		return NULL;
	}

	return (const git_merge_tree_opts *)&merge_options->merge_options;
}

/**
 * ggit_merge_tree_options_copy:
 * @merge_options: a #GgitMergeTreeOptions.
 *
 * Copies @merge_options into a newly allocated #GgitMergeTreeOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitMergeTreeOptions.
 */
GgitMergeTreeOptions *
ggit_merge_tree_options_copy (GgitMergeTreeOptions *merge_options)
{
	GgitMergeTreeOptions *new_merge_options;

	g_return_val_if_fail (merge_options != NULL, NULL);

	new_merge_options = g_slice_new (GgitMergeTreeOptions);

	new_merge_options->merge_options = merge_options->merge_options;

	return new_merge_options;
}

/**
 * ggit_merge_tree_options_free:
 * @merge_options: a #GgitMergeTreeOptions.
 *
 * Frees @merge_options.
 */
void
ggit_merge_tree_options_free (GgitMergeTreeOptions *merge_options)
{
	g_return_if_fail (merge_options != NULL);

	g_slice_free (GgitMergeTreeOptions, merge_options);
}

/**
 * ggit_merge_tree_options_new:
 * @flags: flags to consider when merging.
 * @rename_threshold: similarity to consider a file renamed (default 50).
 * @target_limit: maximum similarity sources to examine
 *                (overrides the `merge_tree.renameLimit` config) (default 200).
 * @metric: (allow-none): a #GgitDiffSimilarityMetric or %NULL to use internal metric.
 * @automerge_mode: mode for automerging.
 *
 * Creates a new #GgitMergeTreeOptions.
 *
 * Returns: a newly allocated #GgitMergeTreeOptions.
 */
GgitMergeTreeOptions *
ggit_merge_tree_options_new (GgitMergeTreeFlags        flags,
                             guint                     rename_threshold,
                             guint                     target_limit,
                             GgitDiffSimilarityMetric *metric,
                             GgitMergeAutomergeMode    automerge_mode)
{
	GgitMergeTreeOptions *merge_options;
	git_merge_tree_opts gmerge_options = GIT_MERGE_TREE_OPTS_INIT;

	merge_options = g_slice_new (GgitMergeTreeOptions);

	gmerge_options.flags = (git_merge_tree_flag_t)flags;
	gmerge_options.rename_threshold = rename_threshold;
	gmerge_options.target_limit = target_limit;
	gmerge_options.metric = _ggit_diff_similarity_metric_get_similarity_metric (metric);
	gmerge_options.automerge_flags = (git_merge_automerge_flags)automerge_mode;

	merge_options->merge_options = gmerge_options;

	return merge_options;
}

/* ex:set ts=8 noet: */
