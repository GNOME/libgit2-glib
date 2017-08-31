/*
 * ggit-submodule-update-options.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Ignacio Casal Quinteiro
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

#include "ggit-submodule-update-options.h"
#include "ggit-enum-types.h"
#include "ggit-checkout-options.h"
#include "ggit-fetch-options.h"

/**
 * GgitSubmoduleUpdateOptions:
 *
 * Represents options for a submodule update.
 */

typedef struct _GgitSubmoduleUpdateOptionsPrivate
{
	git_submodule_update_options options;

	GgitCheckoutOptions *checkout_options;
	GgitFetchOptions *fetch_options;
} GgitSubmoduleUpdateOptionsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitSubmoduleUpdateOptions, ggit_submodule_update_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_CHECKOUT_OPTIONS,
	PROP_FETCH_OPTIONS,
};

static void
ggit_submodule_update_options_finalize (GObject *object)
{
	GgitSubmoduleUpdateOptions *options = GGIT_SUBMODULE_UPDATE_OPTIONS (object);
	GgitSubmoduleUpdateOptionsPrivate *priv;

	priv = ggit_submodule_update_options_get_instance_private (options);

	g_clear_object (&priv->checkout_options);
	g_clear_pointer (&priv->fetch_options, ggit_fetch_options_free);

	G_OBJECT_CLASS (ggit_submodule_update_options_parent_class)->finalize (object);
}

static void
ggit_submodule_update_options_set_property (GObject      *object,
                                            guint         prop_id,
                                            const GValue *value,
                                            GParamSpec   *pspec)
{
	GgitSubmoduleUpdateOptions *options = GGIT_SUBMODULE_UPDATE_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_CHECKOUT_OPTIONS:
		ggit_submodule_update_options_set_checkout_options (options,
		                                                    g_value_get_object (value));
		break;
	case PROP_FETCH_OPTIONS:
		ggit_submodule_update_options_set_fetch_options (options,
		                                                 g_value_get_boxed (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_submodule_update_options_get_property (GObject    *object,
                                            guint       prop_id,
                                            GValue     *value,
                                            GParamSpec *pspec)
{
	GgitSubmoduleUpdateOptions *options = GGIT_SUBMODULE_UPDATE_OPTIONS (object);
	GgitSubmoduleUpdateOptionsPrivate *priv;

	priv = ggit_submodule_update_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_CHECKOUT_OPTIONS:
		g_value_set_object (value, priv->checkout_options);
		break;
	case PROP_FETCH_OPTIONS:
		g_value_set_boxed (value, priv->fetch_options);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_submodule_update_options_class_init (GgitSubmoduleUpdateOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_submodule_update_options_finalize;
	object_class->get_property = ggit_submodule_update_options_get_property;
	object_class->set_property = ggit_submodule_update_options_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_CHECKOUT_OPTIONS,
	                                 g_param_spec_object ("checkout-options",
	                                                      "Checkout Options",
	                                                      "Checkout options",
	                                                      GGIT_TYPE_CHECKOUT_OPTIONS,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_FETCH_OPTIONS,
	                                 g_param_spec_boxed ("fetch-options",
	                                                     "Fetch options",
	                                                     "Fetch options",
	                                                     GGIT_TYPE_FETCH_OPTIONS,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));
}

static void
ggit_submodule_update_options_init (GgitSubmoduleUpdateOptions *options)
{
	GgitSubmoduleUpdateOptionsPrivate *priv;

	priv = ggit_submodule_update_options_get_instance_private (options);

	git_submodule_update_init_options (&priv->options, GIT_SUBMODULE_UPDATE_OPTIONS_VERSION);
}

/**
 * ggit_submodule_update_options_new:
 *
 * Creates a new submodule options object.
 *
 * Returns: (transfer full) (allow-none): a #GgitSubmoduleUpdateOptions or %NULL.
 *
 **/
GgitSubmoduleUpdateOptions *
ggit_submodule_update_options_new (void)
{
	return g_object_new (GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS, NULL);
}

const git_submodule_update_options *
_ggit_submodule_update_options_get_submodule_update_options (GgitSubmoduleUpdateOptions *options)
{
	if (options != NULL)
	{
		GgitSubmoduleUpdateOptionsPrivate *priv;

		priv = ggit_submodule_update_options_get_instance_private (options);

		/* Make sure to synchronize the wrapped checkout options
		 * with the internal checkout options */
		if (priv->checkout_options)
		{
			priv->options.checkout_opts =
				*_ggit_checkout_options_get_checkout_options (priv->checkout_options);
		}

		return &priv->options;
	}
	else
	{
		return NULL;
	}
}

/**
 * ggit_submodule_update_options_get_checkout_options:
 * @options: a #GgitSubmoduleUpdateOptions.
 *
 * Get the checkout options.
 *
 * Returns: (transfer none) (allow-none): a #GgitCheckoutOptions or %NULL.
 *
 **/
GgitCheckoutOptions *
ggit_submodule_update_options_get_checkout_options (GgitSubmoduleUpdateOptions *options)
{
	GgitSubmoduleUpdateOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), NULL);

	priv = ggit_submodule_update_options_get_instance_private (options);

	return priv->checkout_options;
}

/**
 * ggit_submodule_update_options_set_checkout_options:
 * @options: a #GgitSubmoduleUpdateOptions.
 * @checkout_options: (allow-none): a #GgitCheckoutOptions.
 *
 * Set the checkout options.
 *
 **/
void
ggit_submodule_update_options_set_checkout_options (GgitSubmoduleUpdateOptions *options,
                                                    GgitCheckoutOptions        *checkout_options)
{
	GgitSubmoduleUpdateOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options));
	g_return_if_fail (checkout_options == NULL || GGIT_IS_CHECKOUT_OPTIONS (checkout_options));

	priv = ggit_submodule_update_options_get_instance_private (options);

	if (priv->checkout_options)
	{
		g_object_unref (priv->checkout_options);
		priv->checkout_options = NULL;

		git_checkout_init_options (&priv->options.checkout_opts, GIT_CHECKOUT_OPTIONS_VERSION);
	}

	if (checkout_options)
	{
		priv->checkout_options = g_object_ref (checkout_options);
		priv->options.checkout_opts = *_ggit_checkout_options_get_checkout_options (priv->checkout_options);
	}

	g_object_notify (G_OBJECT (options), "checkout-options");
}

/**
 * ggit_submodule_update_options_get_remote_callbacks:
 * @options: a #GgitSubmoduleUpdateOptions.
 *
 * Gets the fetch options.
 *
 * Returns: (transfer none) (allow-none): a #GgitFetchOptions or %NULL.
 *
 **/
GgitFetchOptions *
ggit_submodule_update_options_get_fetch_options (GgitSubmoduleUpdateOptions *options)
{
	GgitSubmoduleUpdateOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), NULL);

	priv = ggit_submodule_update_options_get_instance_private (options);

	return priv->fetch_options;

}

/**
 * ggit_submodule_update_options_set_fetch_options:
 * @options: a #GgitSubmoduleUpdateOptions.
 * @fetch_options: (allow-none): a #GgitFetchOptions.
 *
 * Sets the fetch options.
 **/
void
ggit_submodule_update_options_set_fetch_options (GgitSubmoduleUpdateOptions *options,
                                                 GgitFetchOptions           *fetch_options)
{
	GgitSubmoduleUpdateOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options));

	priv = ggit_submodule_update_options_get_instance_private (options);

	if (priv->fetch_options)
	{
		ggit_fetch_options_free (priv->fetch_options);
		priv->fetch_options = NULL;

		git_fetch_init_options (&priv->options.fetch_opts, GIT_FETCH_OPTIONS_VERSION);
	}

	if (fetch_options)
	{
		priv->fetch_options = ggit_fetch_options_copy (fetch_options);
		priv->options.fetch_opts = *_ggit_fetch_options_get_fetch_options (priv->fetch_options);
	}

	g_object_notify (G_OBJECT (options), "fetch-options");
}

/* ex:set ts=8 noet: */
