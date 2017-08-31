/*
 * ggit-diff-options.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
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

#include <git2.h>

#include "ggit-diff-options.h"
#include "ggit-utils.h"
#include "ggit-enum-types.h"

/**
 * GgitDiffOptions:
 *
 * Represents the options used when creating a #GgitDiff.
 */

typedef struct _GgitDiffOptionsPrivate
{
	git_diff_options diff_options;

	gchar *old_prefix;
	gchar *new_prefix;

	gchar **pathspec;
} GgitDiffOptionsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitDiffOptions, ggit_diff_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_FLAGS,
	PROP_N_CONTEXT_LINES,
	PROP_N_INTERHUNK_LINES,
	PROP_OLD_PREFIX,
	PROP_NEW_PREFIX,
	PROP_PATHSPEC
};

static void
ggit_diff_options_finalize (GObject *object)
{
	GgitDiffOptions *options = GGIT_DIFF_OPTIONS (object);
	GgitDiffOptionsPrivate *priv;

	priv = ggit_diff_options_get_instance_private (options);

	g_free (priv->old_prefix);
	g_free (priv->new_prefix);
	g_strfreev (priv->pathspec);

	G_OBJECT_CLASS (ggit_diff_options_parent_class)->finalize (object);
}

static void
ggit_diff_options_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
	GgitDiffOptions *options = GGIT_DIFF_OPTIONS (object);
	GgitDiffOptionsPrivate *priv;

	priv = ggit_diff_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_FLAGS:
		priv->diff_options.flags = g_value_get_flags (value);
		break;
	case PROP_N_CONTEXT_LINES:
		priv->diff_options.context_lines = g_value_get_int (value);
		break;
	case PROP_N_INTERHUNK_LINES:
		priv->diff_options.interhunk_lines = g_value_get_int (value);
		break;
	case PROP_OLD_PREFIX:
		ggit_diff_options_set_old_prefix (options,
		                                  g_value_get_string (value));
		break;
	case PROP_NEW_PREFIX:
		ggit_diff_options_set_new_prefix (options,
		                                  g_value_get_string (value));
		break;
	case PROP_PATHSPEC:
		ggit_diff_options_set_pathspec (options,
		                                g_value_get_boxed (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_diff_options_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
	GgitDiffOptions *options = GGIT_DIFF_OPTIONS (object);
	GgitDiffOptionsPrivate *priv;

	priv = ggit_diff_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_FLAGS:
		g_value_set_flags (value, (GgitDiffOption)priv->diff_options.flags);
		break;
	case PROP_N_CONTEXT_LINES:
		g_value_set_int (value, priv->diff_options.context_lines);
		break;
	case PROP_N_INTERHUNK_LINES:
		g_value_set_int (value, priv->diff_options.interhunk_lines);
		break;
	case PROP_OLD_PREFIX:
		g_value_set_string (value, priv->old_prefix);
		break;
	case PROP_NEW_PREFIX:
		g_value_set_string (value, priv->new_prefix);
		break;
	case PROP_PATHSPEC:
		g_value_set_boxed (value, priv->pathspec);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_diff_options_class_init (GgitDiffOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	git_diff_options defopts = GIT_DIFF_OPTIONS_INIT;

	object_class->finalize = ggit_diff_options_finalize;
	object_class->get_property = ggit_diff_options_get_property;
	object_class->set_property = ggit_diff_options_set_property;

	/**
	 * GgitDiffOptions:flags: (type GgitDiffOption):
	 *
	 * The diff option flags.
	 */
	g_object_class_install_property (object_class,
	                                 PROP_FLAGS,
	                                 g_param_spec_flags ("flags",
	                                                     "Flags",
	                                                     "Flags",
	                                                     GGIT_TYPE_DIFF_OPTION,
	                                                     defopts.flags,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_CONSTRUCT |
	                                                     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_N_CONTEXT_LINES,
	                                 g_param_spec_int ("n-context-lines",
	                                                   "N Context Lines",
	                                                   "N context lines",
	                                                   G_MININT,
	                                                   G_MAXINT,
	                                                   defopts.context_lines,
	                                                   G_PARAM_READWRITE |
	                                                   G_PARAM_CONSTRUCT |
	                                                   G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_N_INTERHUNK_LINES,
	                                 g_param_spec_int ("n-interhunk-lines",
	                                                   "N Interhunk Lines",
	                                                   "N interhunk lines",
	                                                   G_MININT,
	                                                   G_MAXINT,
	                                                   defopts.interhunk_lines,
	                                                   G_PARAM_READWRITE |
	                                                   G_PARAM_CONSTRUCT |
	                                                   G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_OLD_PREFIX,
	                                 g_param_spec_string ("old-prefix",
	                                                      "Old Prefix",
	                                                      "Old prefix",
	                                                      defopts.old_prefix,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_NEW_PREFIX,
	                                 g_param_spec_string ("new-prefix",
	                                                      "New Prefix",
	                                                      "New prefix",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_PATHSPEC,
	                                 g_param_spec_boxed ("pathspec",
	                                                     "Pathspec",
	                                                     "Pathspec",
	                                                     G_TYPE_STRV,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_CONSTRUCT |
	                                                     G_PARAM_STATIC_STRINGS));
}

static void
ggit_diff_options_init (GgitDiffOptions *options)
{
	GgitDiffOptionsPrivate *priv;

	priv = ggit_diff_options_get_instance_private (options);

	git_diff_init_options (&priv->diff_options, GIT_DIFF_OPTIONS_VERSION);
}

/**
 * ggit_diff_options_new:
 *
 * Create a new diff options object.
 *
 * Returns: (transfer full) (allow-none): a #GgitDiffOptions or %NULL.
 *
 **/
GgitDiffOptions *
ggit_diff_options_new (void)
{
	return g_object_new (GGIT_TYPE_DIFF_OPTIONS, NULL);
}

const git_diff_options *
_ggit_diff_options_get_diff_options (GgitDiffOptions *diff_options)
{
	GgitDiffOptionsPrivate *priv;

	/* NULL is common for diff_options as it specifies to use the default
	 * so handle a NULL diff_options here instead of in every caller.
	 */
	if (diff_options == NULL)
	{
		return NULL;
	}

	priv = ggit_diff_options_get_instance_private (diff_options);

	return (const git_diff_options *)&priv->diff_options;
}

/**
 * ggit_diff_options_get_flags:
 * @options: a #GgitDiffOptions.
 *
 * Get the diff flags.
 *
 * Returns: a #GgitDiffOption.
 *
 **/
GgitDiffOption
ggit_diff_options_get_flags (GgitDiffOptions *options)
{
	GgitDiffOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), 0);

	priv = ggit_diff_options_get_instance_private (options);

	return (GgitDiffOption)priv->diff_options.flags;
}

/**
 * ggit_diff_options_set_flags:
 * @options: a #GgitDiffOptions.
 * @flags: a #GgitDiffOption.
 *
 * Set the diff flags.
 *
 **/
void
ggit_diff_options_set_flags (GgitDiffOptions *options,
                             GgitDiffOption   flags)
{
	GgitDiffOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	priv = ggit_diff_options_get_instance_private (options);

	priv->diff_options.flags = flags;
	g_object_notify (G_OBJECT (options), "flags");
}

/**
 * ggit_diff_options_get_n_context_lines:
 * @options: a #GgitDiffOptions.
 *
 * Get the number of context lines to include in the diff.
 *
 * Returns: the number of context lines.
 *
 **/
gint
ggit_diff_options_get_n_context_lines (GgitDiffOptions *options)
{
	GgitDiffOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), 0);

	priv = ggit_diff_options_get_instance_private (options);

	return (GgitDiffOption)priv->diff_options.context_lines;
}

/**
 * ggit_diff_options_set_n_context_lines:
 * @options: a #GgitDiffOptions.
 * @n: the number of lines.
 *
 * Set the number of context lines to include in the diff.
 *
 **/
void
ggit_diff_options_set_n_context_lines (GgitDiffOptions *options,
                                       gint             n)
{
	GgitDiffOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	priv = ggit_diff_options_get_instance_private (options);

	priv->diff_options.context_lines = n;
	g_object_notify (G_OBJECT (options), "n-context-lines");
}

/**
 * ggit_diff_options_get_n_interhunk_lines:
 * @options: a #GgitDiffOptions.
 *
 * Get the number of interhunk lines to include in the diff.
 *
 * Returns: the number of lines.
 *
 **/
gint
ggit_diff_options_get_n_interhunk_lines (GgitDiffOptions *options)
{
	GgitDiffOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), 0);

	priv = ggit_diff_options_get_instance_private (options);

	return (GgitDiffOption)priv->diff_options.interhunk_lines;
}

/**
 * ggit_diff_options_set_n_interhunk_lines:
 * @options: a #GgitDiffOptions.
 * @n: the number of lines.
 *
 * Set the number of interhunk lines to include in the diff.
 *
 **/
void
ggit_diff_options_set_n_interhunk_lines (GgitDiffOptions *options,
                                         gint             n)
{
	GgitDiffOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	priv = ggit_diff_options_get_instance_private (options);

	priv->diff_options.interhunk_lines = n;
	g_object_notify (G_OBJECT (options), "n-interhunk-lines");
}

/**
 * ggit_diff_options_get_old_prefix:
 * @options: a #GgitDiffOptions.
 *
 * Get the diff old-prefix string.
 *
 * Returns: (transfer full) (allow-none): the old-prefix string or %NULL.
 *
 **/
const gchar *
ggit_diff_options_get_old_prefix (GgitDiffOptions *options)
{
	GgitDiffOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), NULL);

	priv = ggit_diff_options_get_instance_private (options);

	return priv->old_prefix;
}

/**
 * ggit_diff_options_set_old_prefix:
 * @options: a #GgitDiffOptions.
 * @prefix: the prefix.
 *
 * Get the diff old-prefix string.
 *
 **/
void
ggit_diff_options_set_old_prefix (GgitDiffOptions *options,
                                  const gchar     *prefix)
{
	GgitDiffOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	priv = ggit_diff_options_get_instance_private (options);

	g_free (priv->old_prefix);
	priv->old_prefix = g_strdup (prefix);

	priv->diff_options.old_prefix = priv->old_prefix;
	g_object_notify (G_OBJECT (options), "old-prefix");
}

/**
 * ggit_diff_options_get_new_prefix:
 * @options: a #GgitDiffOptions.
 *
 * Get the diff new-prefix string.
 *
 * Returns: (transfer none) (allow-none): the new-prefix string or %NULL.
 *
 **/
const gchar *
ggit_diff_options_get_new_prefix (GgitDiffOptions *options)
{
	GgitDiffOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), NULL);

	priv = ggit_diff_options_get_instance_private (options);

	return priv->new_prefix;
}

/**
 * ggit_diff_options_set_new_prefix:
 * @options: a #GgitDiffOptions.
 * @prefix: the prefix.
 *
 * Set the diff new-prefix string.
 *
 **/
void
ggit_diff_options_set_new_prefix (GgitDiffOptions *options,
                                  const gchar     *prefix)
{
	GgitDiffOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	priv = ggit_diff_options_get_instance_private (options);

	g_free (priv->new_prefix);
	priv->new_prefix = g_strdup (prefix);

	priv->diff_options.new_prefix = priv->new_prefix;
	g_object_notify (G_OBJECT (options), "new-prefix");
}

/**
 * ggit_diff_options_get_pathspec:
 * @options: a #GgitDiffOptions.
 *
 * Get the pathspec.
 *
 * Returns: (transfer none) (array zero-terminated=1) (allow-none): the pathspec or %NULL.
 *
 **/
const gchar **
ggit_diff_options_get_pathspec (GgitDiffOptions *options)
{
	GgitDiffOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), NULL);

	priv = ggit_diff_options_get_instance_private (options);

	return (const gchar **)priv->pathspec;
}

/**
 * ggit_diff_options_set_pathspec:
 * @options: a #GgitDiffOptions.
 * @pathspec: (array zero-terminated=1) (allow-none): the pathspec.
 *
 * Set the pathspec.
 *
 **/
void
ggit_diff_options_set_pathspec (GgitDiffOptions  *options,
                                const gchar     **pathspec)
{
	GgitDiffOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	priv = ggit_diff_options_get_instance_private (options);

	g_strfreev (priv->pathspec);
	priv->pathspec = g_strdupv ((gchar **)pathspec);

	priv->diff_options.pathspec.strings = priv->pathspec;

	if (priv->pathspec != NULL)
	{
		priv->diff_options.pathspec.count = g_strv_length (priv->pathspec);
	}
	else
	{
		priv->diff_options.pathspec.count = 0;
	}

	g_object_notify (G_OBJECT (options), "pathspec");
}

/* ex:set ts=8 noet: */
