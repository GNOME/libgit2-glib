/*
 * ggit-proxy-options.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2017 - Ignacio Casal Quinteiro
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

#include "ggit-proxy-options.h"

/**
 * GgitProxyOptions:
 *
 * Represents a git proxy options.
 */

typedef struct _GgitProxyOptionsPrivate
{
	git_proxy_options options;
} GgitProxyOptionsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitProxyOptions, ggit_proxy_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
};

static void
ggit_proxy_options_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_proxy_options_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_proxy_options_class_init (GgitProxyOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = ggit_proxy_options_get_property;
	object_class->set_property = ggit_proxy_options_set_property;
}

static void
ggit_proxy_options_init (GgitProxyOptions *options)
{
	GgitProxyOptionsPrivate *priv;

	priv = ggit_proxy_options_get_instance_private (options);

	git_proxy_init_options (&priv->options, GIT_PROXY_OPTIONS_VERSION);
}

/**
 * ggit_proxy_options_new:
 *
 * Create a new proxy options object.
 *
 * Returns: (transfer full) (allow-none): a #GgitProxyOptions.
 *
 **/
GgitProxyOptions *
ggit_proxy_options_new (void)
{
	return g_object_new (GGIT_TYPE_PROXY_OPTIONS, NULL);
}

const git_proxy_options *
_ggit_proxy_options_get_proxy_options (GgitProxyOptions *options)
{
	if (options != NULL)
	{
		GgitProxyOptionsPrivate *priv;

		priv = ggit_proxy_options_get_instance_private (options);

		return &priv->options;
	}

	return NULL;
}

/* ex:set ts=8 noet: */
