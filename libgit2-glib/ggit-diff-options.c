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

#define GGIT_DIFF_OPTIONS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_DIFF_OPTIONS, GgitDiffOptionsPrivate))

struct _GgitDiffOptionsPrivate
{
	git_diff_options diff_options;

	gchar *old_prefix;
	gchar *new_prefix;

	gchar **pathspec;
};

G_DEFINE_TYPE (GgitDiffOptions, ggit_diff_options, G_TYPE_OBJECT)

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
	GgitDiffOptions *options;

	options = GGIT_DIFF_OPTIONS (object);

	g_free (options->priv->old_prefix);
	g_free (options->priv->new_prefix);
	g_strfreev (options->priv->pathspec);

	G_OBJECT_CLASS (ggit_diff_options_parent_class)->finalize (object);
}

static void
ggit_diff_options_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
	GgitDiffOptions *self = GGIT_DIFF_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_FLAGS:
		self->priv->diff_options.flags = g_value_get_flags (value);
		break;
	case PROP_N_CONTEXT_LINES:
		self->priv->diff_options.context_lines = g_value_get_int (value);
		break;
	case PROP_N_INTERHUNK_LINES:
		self->priv->diff_options.interhunk_lines = g_value_get_int (value);
		break;
	case PROP_OLD_PREFIX:
		ggit_diff_options_set_old_prefix (self,
		                                  g_value_get_string (value));
		break;
	case PROP_NEW_PREFIX:
		ggit_diff_options_set_new_prefix (self,
		                                  g_value_get_string (value));
		break;
	case PROP_PATHSPEC:
		ggit_diff_options_set_pathspec (self,
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
	GgitDiffOptions *self = GGIT_DIFF_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_FLAGS:
		g_value_set_flags (value, (GgitDiffOption)self->priv->diff_options.flags);
		break;
	case PROP_N_CONTEXT_LINES:
		g_value_set_int (value, self->priv->diff_options.context_lines);
		break;
	case PROP_N_INTERHUNK_LINES:
		g_value_set_int (value, self->priv->diff_options.interhunk_lines);
		break;
	case PROP_OLD_PREFIX:
		g_value_set_string (value, self->priv->old_prefix);
		break;
	case PROP_NEW_PREFIX:
		g_value_set_string (value, self->priv->new_prefix);
		break;
	case PROP_PATHSPEC:
		g_value_set_boxed (value, self->priv->pathspec);
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

	g_type_class_add_private (object_class, sizeof (GgitDiffOptionsPrivate));

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
ggit_diff_options_init (GgitDiffOptions *self)
{
	self->priv = GGIT_DIFF_OPTIONS_GET_PRIVATE (self);

	git_diff_init_options (&self->priv->diff_options, GIT_DIFF_OPTIONS_VERSION);
}

/**
 * ggit_diff_options_new:
 *
 * Create a new diff options object.
 *
 * Returns: a #GgitDiffOptions.
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
	/* NULL is common for diff_options as it specifies to use the default
	 * so handle a NULL diff_options here instead of in every caller.
	 */
	if (diff_options == NULL)
	{
		return NULL;
	}

	return (const git_diff_options *)&diff_options->priv->diff_options;
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
	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), 0);

	return (GgitDiffOption)options->priv->diff_options.flags;
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
	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	options->priv->diff_options.flags = flags;
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
	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), 0);

	return (GgitDiffOption)options->priv->diff_options.context_lines;
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
	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	options->priv->diff_options.context_lines = n;
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
	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), 0);

	return (GgitDiffOption)options->priv->diff_options.interhunk_lines;
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
	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	options->priv->diff_options.interhunk_lines = n;
	g_object_notify (G_OBJECT (options), "n-interhunk-lines");
}

/**
 * ggit_diff_options_get_old_prefix:
 * @options: a #GgitDiffOptions.
 *
 * Get the diff old-prefix string.
 *
 * Returns: the old-prefix string.
 *
 **/
const gchar *
ggit_diff_options_get_old_prefix (GgitDiffOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), NULL);

	return options->priv->old_prefix;
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
	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	g_free (options->priv->old_prefix);
	options->priv->old_prefix = g_strdup (prefix);

	options->priv->diff_options.old_prefix = options->priv->old_prefix;
	g_object_notify (G_OBJECT (options), "old-prefix");
}

/**
 * ggit_diff_options_get_new_prefix:
 * @options: a #GgitDiffOptions.
 *
 * Get the diff new-prefix string.
 *
 * Returns: the new-prefix string.
 *
 **/
const gchar *
ggit_diff_options_get_new_prefix (GgitDiffOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), NULL);

	return options->priv->new_prefix;
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
	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	g_free (options->priv->new_prefix);
	options->priv->new_prefix = g_strdup (prefix);

	options->priv->diff_options.new_prefix = options->priv->new_prefix;
	g_object_notify (G_OBJECT (options), "new-prefix");
}

/**
 * ggit_diff_options_get_pathspec:
 * @options: a #GgitDiffOptions.
 *
 * Get the pathspec.
 *
 * Returns: (transfer none) (array zero-terminated=1): the pathspec.
 *
 **/
const gchar **
ggit_diff_options_get_pathspec (GgitDiffOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_OPTIONS (options), NULL);

	return (const gchar **)options->priv->pathspec;
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
	g_return_if_fail (GGIT_IS_DIFF_OPTIONS (options));

	g_strfreev (options->priv->pathspec);
	options->priv->pathspec = g_strdupv ((gchar **)pathspec);

	options->priv->diff_options.pathspec.strings = options->priv->pathspec;

	if (options->priv->pathspec != NULL)
	{
		options->priv->diff_options.pathspec.count = g_strv_length (options->priv->pathspec);
	}
	else
	{
		options->priv->diff_options.pathspec.count = 0;
	}

	g_object_notify (G_OBJECT (options), "pathspec");
}

/* ex:set ts=8 noet: */
