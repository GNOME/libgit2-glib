/*
 * ggit-diff-find-options.c
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

#include "ggit-diff-find-options.h"
#include "ggit-diff-similarity-metric.h"
#include "ggit-enum-types.h"

/**
 * GgitDiffFindOptions:
 *
 * Represents options for finding diff similarity.
 */

typedef struct _GgitDiffFindOptionsPrivate
{
	git_diff_find_options diff_find_options;

	GgitDiffSimilarityMetric *metric;
} GgitDiffFindOptionsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitDiffFindOptions, ggit_diff_find_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_FLAGS,
	PROP_RENAME_THRESHOLD,
	PROP_RENAME_FROM_REWRITE_THRESHOLD,
	PROP_COPY_THRESHOLD,
	PROP_RENAME_LIMIT,
	PROP_SIMILARITY_METRIC
};

static void
ggit_diff_find_options_finalize (GObject *object)
{
	GgitDiffFindOptions *options = GGIT_DIFF_FIND_OPTIONS (object);
	GgitDiffFindOptionsPrivate *priv;

	priv = ggit_diff_find_options_get_instance_private (options);

	if (priv->metric)
	{
		ggit_diff_similarity_metric_free (priv->metric);
	}

	G_OBJECT_CLASS (ggit_diff_find_options_parent_class)->finalize (object);
}

static void
ggit_diff_find_options_set_property (GObject      *object,
                                     guint         prop_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
	GgitDiffFindOptions *options = GGIT_DIFF_FIND_OPTIONS (object);
	GgitDiffFindOptionsPrivate *priv;

	priv = ggit_diff_find_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_FLAGS:
		priv->diff_find_options.flags =
			(GgitDiffFindFlags)g_value_get_flags (value);
		break;
	case PROP_RENAME_THRESHOLD:
		priv->diff_find_options.rename_threshold = g_value_get_uint (value);
		break;
	case PROP_RENAME_FROM_REWRITE_THRESHOLD:
		priv->diff_find_options.rename_from_rewrite_threshold = g_value_get_uint (value);
		break;
	case PROP_COPY_THRESHOLD:
		priv->diff_find_options.copy_threshold = g_value_get_uint (value);
		break;
	case PROP_RENAME_LIMIT:
		priv->diff_find_options.rename_limit = g_value_get_uint (value);
		break;
	case PROP_SIMILARITY_METRIC:
		ggit_diff_find_options_set_metric (options,
		                                   g_value_get_boxed (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_diff_find_options_get_property (GObject    *object,
                                     guint       prop_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
	GgitDiffFindOptions *options = GGIT_DIFF_FIND_OPTIONS (object);
	GgitDiffFindOptionsPrivate *priv;

	priv = ggit_diff_find_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_FLAGS:
		g_value_set_flags (value, priv->diff_find_options.flags);
		break;
	case PROP_RENAME_THRESHOLD:
		g_value_set_uint (value, priv->diff_find_options.rename_threshold);
		break;
	case PROP_RENAME_FROM_REWRITE_THRESHOLD:
		g_value_set_uint (value, priv->diff_find_options.rename_from_rewrite_threshold);
		break;
	case PROP_COPY_THRESHOLD:
		g_value_set_uint (value, priv->diff_find_options.copy_threshold);
		break;
	case PROP_RENAME_LIMIT:
		g_value_set_uint (value, priv->diff_find_options.rename_limit);
		break;
	case PROP_SIMILARITY_METRIC:
		g_value_set_boxed (value, priv->diff_find_options.metric);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_diff_find_options_class_init (GgitDiffFindOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	git_diff_find_options defopt = GIT_DIFF_FIND_OPTIONS_INIT;

	object_class->finalize = ggit_diff_find_options_finalize;
	object_class->get_property = ggit_diff_find_options_get_property;
	object_class->set_property = ggit_diff_find_options_set_property;

	/**
	 * GgitDiffFindOptions:flags: (type GgitDiffFindFlags):
	 *
	 * The diff option flags.
	 */
	g_object_class_install_property (object_class,
	                                 PROP_FLAGS,
	                                 g_param_spec_flags ("flags",
	                                                     "Flags",
	                                                     "Flags",
	                                                     GGIT_TYPE_DIFF_FIND_FLAGS,
	                                                     defopt.flags,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_RENAME_THRESHOLD,
	                                 g_param_spec_uint ("rename-threshold",
	                                                    "Rename Threshold",
	                                                    "Rename threshold",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    defopt.rename_threshold,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_RENAME_FROM_REWRITE_THRESHOLD,
	                                 g_param_spec_uint ("rename-from-rewrite-threshold",
	                                                    "Rename From Rewrite Threshold",
	                                                    "Rename from rewrite threshold",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    defopt.rename_from_rewrite_threshold,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_COPY_THRESHOLD,
	                                 g_param_spec_uint ("copy-threshold",
	                                                    "Copy Threshold",
	                                                    "Copy threshold",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    defopt.copy_threshold,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_RENAME_LIMIT,
	                                 g_param_spec_uint ("rename-limit",
	                                                    "Rename Limit",
	                                                    "Rename limit",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    defopt.rename_limit,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_SIMILARITY_METRIC,
	                                 g_param_spec_boxed ("metric",
	                                                     "Metric",
	                                                     "Metric",
	                                                     GGIT_TYPE_DIFF_SIMILARITY_METRIC,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));
}

static void
ggit_diff_find_options_init (GgitDiffFindOptions *options)
{
	GgitDiffFindOptionsPrivate *priv;

	priv = ggit_diff_find_options_get_instance_private (options);

	git_diff_find_init_options (&priv->diff_find_options,
	                            GIT_DIFF_FIND_OPTIONS_VERSION);

}

const git_diff_find_options *
_ggit_diff_find_options_get_diff_find_options (GgitDiffFindOptions *diff_find_options)
{
	GgitDiffFindOptionsPrivate *priv;

	/* NULL is common for diff_find_options as it specifies to use the default
	 * so handle a NULL diff_find_options here instead of in every caller.
	 */
	if (diff_find_options == NULL)
	{
		return NULL;
	}

	priv = ggit_diff_find_options_get_instance_private (diff_find_options);

	return (const git_diff_find_options *)&priv->diff_find_options;
}
/**
 * ggit_diff_find_options_new:
 *
 * Creates a new #GgitDiffFindOptions.
 *
 * Returns: a newly allocated #GgitDiffFindOptions.
 */
GgitDiffFindOptions *
ggit_diff_find_options_new (void)
{
	return g_object_new (GGIT_TYPE_DIFF_FIND_OPTIONS, NULL);
}

/**
 * ggit_diff_find_options_get_flags:
 * @options: a #GgitDiffFindOptions.
 *
 * Get the find options flags.
 *
 * Returns: the find options flags.
 *
 **/
GgitDiffFindFlags
ggit_diff_find_options_get_flags (GgitDiffFindOptions *options)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options), 0);

	priv = ggit_diff_find_options_get_instance_private (options);

	return (GgitDiffFindFlags)priv->diff_find_options.flags;
}

/**
 * ggit_diff_find_options_set_flags:
 * @options: a #GgitDiffFindOptions.
 * @flags: a #GgitDiffFindFlags.
 *
 * Set the find options flags.
 *
 **/
void
ggit_diff_find_options_set_flags (GgitDiffFindOptions *options,
                                  GgitDiffFindFlags    flags)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options));

	priv = ggit_diff_find_options_get_instance_private (options);

	priv->diff_find_options.flags = flags;
	g_object_notify (G_OBJECT (options), "flags");
}

/**
 * ggit_diff_find_options_get_rename_threshold:
 * @options: a #GgitDiffFindOptions.
 *
 * Get the find options rename threshold.
 *
 * Returns: the rename threshold.
 *
 **/
guint
ggit_diff_find_options_get_rename_threshold (GgitDiffFindOptions *options)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options), 0);

	priv = ggit_diff_find_options_get_instance_private (options);

	return priv->diff_find_options.rename_threshold;
}

/**
 * ggit_diff_find_options_set_rename_threshold:
 * @options: a #GgitDiffFindOptions.
 * @threshold: the threshold.
 *
 * Set the find options rename threshold.
 *
 **/
void
ggit_diff_find_options_set_rename_threshold (GgitDiffFindOptions *options,
                                             guint                threshold)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options));

	priv = ggit_diff_find_options_get_instance_private (options);

	priv->diff_find_options.rename_threshold = threshold;
	g_object_notify (G_OBJECT (options), "rename-threshold");
}

/**
 * ggit_diff_find_options_get_rename_from_rewrite_threshold:
 * @options: a #GgitDiffFindOptions.
 *
 * Get the find options rename from rewrite threshold.
 *
 * Returns: the rename from rewrite threshold.
 *
 **/
guint
ggit_diff_find_options_get_rename_from_rewrite_threshold (GgitDiffFindOptions *options)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options), 0);

	priv = ggit_diff_find_options_get_instance_private (options);

	return priv->diff_find_options.rename_from_rewrite_threshold;
}

/**
 * ggit_diff_find_options_set_rename_from_rewrite_threshold:
 * @options: a #GgitDiffFindOptions.
 * @threshold: the threshold.
 *
 * Set the find options rename from rewrite threshold.
 *
 **/
void
ggit_diff_find_options_set_rename_from_rewrite_threshold (GgitDiffFindOptions *options,
                                                          guint                threshold)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options));

	priv = ggit_diff_find_options_get_instance_private (options);

	priv->diff_find_options.rename_from_rewrite_threshold = threshold;
	g_object_notify (G_OBJECT (options), "rename-from-rewrite-threshold");
}

/**
 * ggit_diff_find_options_get_copy_threshold:
 * @options: a #GgitDiffFindOptions.
 *
 * Get the find options copy threshold.
 *
 * Returns: the copy threshold.
 *
 **/
guint
ggit_diff_find_options_get_copy_threshold (GgitDiffFindOptions *options)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options), 0);

	priv = ggit_diff_find_options_get_instance_private (options);

	return priv->diff_find_options.copy_threshold;
}

/**
 * ggit_diff_find_options_set_copy_threshold:
 * @options: a #GgitDiffFindOptions.
 * @threshold: the threshold.
 *
 * Set the find options copy threshold.
 *
 **/
void
ggit_diff_find_options_set_copy_threshold (GgitDiffFindOptions *options,
                                           guint                threshold)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options));

	priv = ggit_diff_find_options_get_instance_private (options);

	priv->diff_find_options.copy_threshold = threshold;
	g_object_notify (G_OBJECT (options), "copy-threshold");
}

/**
 * ggit_diff_find_options_get_rename_limit:
 * @options: a #GgitDiffFindOptions.
 *
 * Get the find options rename limit.
 *
 * Returns: the rename limit.
 *
 **/
gsize
ggit_diff_find_options_get_rename_limit (GgitDiffFindOptions *options)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options), 0);

	priv = ggit_diff_find_options_get_instance_private (options);

	return priv->diff_find_options.rename_limit;
}

/**
 * ggit_diff_find_options_set_rename_limit:
 * @options: a #GgitDiffFindOptions.
 * @limit: the limit.
 *
 * Set the find options rename limit.
 *
 **/
void
ggit_diff_find_options_set_rename_limit (GgitDiffFindOptions *options,
                                         gsize                limit)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options));

	priv = ggit_diff_find_options_get_instance_private (options);

	priv->diff_find_options.rename_limit = limit;
	g_object_notify (G_OBJECT (options), "rename-limit");
}

/**
 * ggit_diff_find_options_get_metric:
 * @options: a #GgitDiffFindOptions.
 *
 * Get the find options metric.
 *
 * Returns: (transfer none): a #GgitDiffSimilarityMetric or %NULL.
 *
 **/
GgitDiffSimilarityMetric *
ggit_diff_find_options_get_metric (GgitDiffFindOptions *options)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options), 0);

	priv = ggit_diff_find_options_get_instance_private (options);

	return priv->metric;
}

/**
 * ggit_diff_find_options_set_metric:
 * @options: a #GgitDiffFindOptions.
 * @metric: a #GgitDiffSimilarityMetric.
 *
 * Set the find options metric.
 *
 **/
void
ggit_diff_find_options_set_metric (GgitDiffFindOptions      *options,
                                   GgitDiffSimilarityMetric *metric)
{
	GgitDiffFindOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_FIND_OPTIONS (options));

	priv = ggit_diff_find_options_get_instance_private (options);

	if (priv->metric)
	{
		ggit_diff_similarity_metric_free (priv->metric);

		priv->metric = NULL;
		priv->diff_find_options.metric = NULL;
	}

	if (metric)
	{
		priv->metric =
			ggit_diff_similarity_metric_copy (metric);

		priv->diff_find_options.metric =
			_ggit_diff_similarity_metric_get_similarity_metric (priv->metric);
	}

	g_object_notify (G_OBJECT (options), "metric");
}

/* ex:set ts=8 noet: */
