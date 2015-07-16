/*
 * ggit-push-options.c
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

#include "ggit-push-options.h"

/**
 * GgitPushOptions:
 *
 * Represents a git push options.
 */

typedef struct _GgitPushOptionsPrivate
{
	git_push_options options;
} GgitPushOptionsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitPushOptions, ggit_push_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_PARALLELISM
};

static void
ggit_push_options_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
	GgitPushOptions *options = GGIT_PUSH_OPTIONS (object);
	GgitPushOptionsPrivate *priv;

	priv = ggit_push_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_PARALLELISM:
		priv->options.pb_parallelism = g_value_get_int (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_push_options_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
	GgitPushOptions *options = GGIT_PUSH_OPTIONS (object);
	GgitPushOptionsPrivate *priv;

	priv = ggit_push_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_PARALLELISM:
		g_value_set_int (value, priv->options.pb_parallelism);
		break;
	}
}

static void
ggit_push_options_class_init (GgitPushOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	git_push_options defopts = GIT_PUSH_OPTIONS_INIT;

	object_class->get_property = ggit_push_options_get_property;
	object_class->set_property = ggit_push_options_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_PARALLELISM,
	                                 g_param_spec_int ("parallelism",
	                                                   "Parallelism",
	                                                    "Parallelism",
	                                                    0,
	                                                    G_MAXINT,
	                                                    defopts.pb_parallelism,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_CONSTRUCT |
	                                                    G_PARAM_STATIC_STRINGS));
}

static void
ggit_push_options_init (GgitPushOptions *options)
{
	GgitPushOptionsPrivate *priv;

	priv = ggit_push_options_get_instance_private (options);

	git_push_init_options (&priv->options, GIT_PUSH_OPTIONS_VERSION);
}

/**
 * ggit_push_options_new:
 *
 * Create a new push options object.
 *
 * Returns: a #GgitPushOptions.
 *
 **/
GgitPushOptions *
ggit_push_options_new ()
{
	return g_object_new (GGIT_TYPE_PUSH_OPTIONS, NULL);
}

const git_push_options *
_ggit_push_options_get_push_options (GgitPushOptions *options)
{
	if (options != NULL)
	{
		GgitPushOptionsPrivate *priv;

		priv = ggit_push_options_get_instance_private (options);

		return &priv->options;
	}
	else
	{
		return NULL;
	}
}

/**
 * ggit_push_options_get_parallelism:
 * @options: a #GgitPushOptions.
 *
 * Get the number of parallel threads to use when creating the pack file
 * to push. The special value 0 indicates that the number of threads will
 * be automatically detected.
 *
 * Returns: the number of parallel threads, or 0 for auto-detect.
 *
 **/
gint
ggit_push_options_get_parallelism (GgitPushOptions *options)
{
	GgitPushOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_PUSH_OPTIONS (options), 0);

	priv = ggit_push_options_get_instance_private (options);

	return priv->options.pb_parallelism;
}

/**
 * ggit_push_options_set_parallelism:
 * @options: a #GgitPushOptions.
 * @parallelism: the number of threads, or 0 for auto-detect.
 *
 * Set the number of parallel threads to use when creating the pack file
 * to push. The special value 0 can be specified for @parallelism indicating that
 * the number of threads will be automatically detected.
 *
 **/
void
ggit_push_options_set_parallelism (GgitPushOptions *options,
                                   gint             parallelism)
{
	GgitPushOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_PUSH_OPTIONS (options));
	g_return_if_fail (parallelism >= 0);

	priv = ggit_push_options_get_instance_private (options);

	priv->options.pb_parallelism = parallelism;
	g_object_notify (G_OBJECT (options), "parallelism");
}

/* ex:set ts=8 noet: */
