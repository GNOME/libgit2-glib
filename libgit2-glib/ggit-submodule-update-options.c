/*
 * ggit-submodule-options.c
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

#define GGIT_SUBMODULE_UPDATE_OPTIONS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS, GgitSubmoduleUpdateOptionsPrivate))

struct _GgitSubmoduleUpdateOptionsPrivate
{
	git_submodule_update_options options;

	GgitCheckoutOptions *checkout_options;
	GgitFetchOptions *fetch_options;
};

G_DEFINE_TYPE (GgitSubmoduleUpdateOptions, ggit_submodule_update_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_CHECKOUT_OPTIONS,
	PROP_FETCH_OPTIONS,
	PROP_CLONE_CHECKOUT_STRATEGY,
};

static void
ggit_submodule_update_options_finalize (GObject *object)
{
	GgitSubmoduleUpdateOptions *options;

	options = GGIT_SUBMODULE_UPDATE_OPTIONS (object);

	g_clear_object (&options->priv->checkout_options);
	g_clear_pointer (&options->priv->fetch_options, ggit_fetch_options_free);

	G_OBJECT_CLASS (ggit_submodule_update_options_parent_class)->finalize (object);
}

static void
ggit_submodule_update_options_set_property (GObject      *object,
                                            guint         prop_id,
                                            const GValue *value,
                                            GParamSpec   *pspec)
{
	GgitSubmoduleUpdateOptions *self = GGIT_SUBMODULE_UPDATE_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_CHECKOUT_OPTIONS:
		ggit_submodule_update_options_set_checkout_options (self,
		                                                    g_value_get_object (value));
		break;
	case PROP_FETCH_OPTIONS:
		ggit_submodule_update_options_set_fetch_options (self,
		                                                 g_value_get_boxed (value));
		break;
	case PROP_CLONE_CHECKOUT_STRATEGY:
		ggit_submodule_update_options_set_clone_checkout_strategy (self,
		                                                           g_value_get_flags (value));
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
	GgitSubmoduleUpdateOptions *self = GGIT_SUBMODULE_UPDATE_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_CHECKOUT_OPTIONS:
		g_value_set_object (value, self->priv->checkout_options);
		break;
	case PROP_FETCH_OPTIONS:
		g_value_set_boxed (value, self->priv->fetch_options);
		break;
	case PROP_CLONE_CHECKOUT_STRATEGY:
		g_value_set_flags (value,
		                   ggit_submodule_update_options_get_clone_checkout_strategy (self));
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

	g_type_class_add_private (object_class, sizeof (GgitSubmoduleUpdateOptionsPrivate));

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
	                                 g_param_spec_object ("fetch-options",
	                                                      "Fetch options",
	                                                      "Fetch options",
	                                                      GGIT_TYPE_FETCH_OPTIONS,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_CLONE_CHECKOUT_STRATEGY,
	                                 g_param_spec_flags ("clone-checkout-strategy",
	                                                     "Clone Checkout Strategy",
	                                                     "Clone checkout strategy",
	                                                     GGIT_TYPE_CHECKOUT_STRATEGY,
	                                                     GGIT_CHECKOUT_NONE,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));
}

static void
ggit_submodule_update_options_init (GgitSubmoduleUpdateOptions *self)
{
	self->priv = GGIT_SUBMODULE_UPDATE_OPTIONS_GET_PRIVATE (self);

	git_submodule_update_init_options (&self->priv->options, GIT_SUBMODULE_UPDATE_OPTIONS_VERSION);
}

/**
 * ggit_submodule_update_options_new:
 *
 * Creates a new submodule options object.
 *
 * Returns: a #GgitSubmoduleUpdateOptions.
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
		/* Make sure to synchronize the wrapped checkout options
		 * with the internal checkout options */
		if (options->priv->checkout_options)
		{
			options->priv->options.checkout_opts =
				*_ggit_checkout_options_get_checkout_options (options->priv->checkout_options);
		}

		return &options->priv->options;
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
 * Returns: (transfer none): a #GgitCheckoutOptions.
 *
 **/
GgitCheckoutOptions *
ggit_submodule_update_options_get_checkout_options (GgitSubmoduleUpdateOptions *options)
{
	g_return_val_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), NULL);

	return options->priv->checkout_options;
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
	g_return_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options));
	g_return_if_fail (checkout_options == NULL || GGIT_IS_CHECKOUT_OPTIONS (checkout_options));

	if (options->priv->checkout_options)
	{
		g_object_unref (options->priv->checkout_options);
		options->priv->checkout_options = NULL;

		git_checkout_init_options (&options->priv->options.checkout_opts, GIT_CHECKOUT_OPTIONS_VERSION);
	}

	if (checkout_options)
	{
		options->priv->checkout_options = g_object_ref (checkout_options);
		options->priv->options.checkout_opts = *_ggit_checkout_options_get_checkout_options (options->priv->checkout_options);
	}

	g_object_notify (G_OBJECT (options), "checkout-options");
}

/**
 * ggit_submodule_update_options_get_remote_callbacks:
 * @options: a #GgitSubmoduleUpdateOptions.
 *
 * Gets the fetch options.
 *
 * Returns: (transfer none): a #GgitFetchOptions.
 *
 **/
GgitFetchOptions *
ggit_submodule_update_options_get_fetch_options (GgitSubmoduleUpdateOptions *options)
{
	g_return_val_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), NULL);

	return options->priv->fetch_options;

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
	g_return_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options));

	if (options->priv->fetch_options)
	{
		ggit_fetch_options_free (options->priv->fetch_options);
		options->priv->fetch_options = NULL;

		git_fetch_init_options (&options->priv->options.fetch_opts, GIT_FETCH_OPTIONS_VERSION);
	}

	if (fetch_options)
	{
		options->priv->fetch_options = ggit_fetch_options_copy (fetch_options);
		options->priv->options.fetch_opts = *_ggit_fetch_options_get_fetch_options (options->priv->fetch_options);
	}

	g_object_notify (G_OBJECT (options), "fetch-options");
}

/**
 * ggit_submodule_update_options_get_clone_checkout_strategy:
 * @options: a #GgitSubmoduleUpdateOptions.
 *
 * Gets the clone checkout strategy.
 *
 * Returns: the clone checkout strategy.
 */
GgitCheckoutStrategy
ggit_submodule_update_options_get_clone_checkout_strategy (GgitSubmoduleUpdateOptions *options)
{
	g_return_val_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), GGIT_CHECKOUT_NONE);

	return options->priv->options.clone_checkout_strategy;
}

/**
 * ggit_submodule_update_options_set_clone_checkout_strategy:
 * @options: a #GgitSubmoduleUpdateOptions.
 * @checkout_strategy: a #GgitCheckoutStrategy.
 *
 * Sets the clone checkout strategy. Use %GGIT_CHECKOUT_SAFE_CREATE to create
 * all files in the working directory for the newly cloned repository.
 */
void
ggit_submodule_update_options_set_clone_checkout_strategy (GgitSubmoduleUpdateOptions *options,
                                                           GgitCheckoutStrategy        checkout_strategy)
{
	g_return_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options));

	if (options->priv->options.clone_checkout_strategy != checkout_strategy)
	{
		options->priv->options.clone_checkout_strategy = checkout_strategy;

		g_object_notify (G_OBJECT (options), "clone-checkout-strategy");
	}
}

/* ex:set ts=8 noet: */
