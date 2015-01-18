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
#include "ggit-remote-callbacks.h"
#include "ggit-signature.h"

#define GGIT_SUBMODULE_UPDATE_OPTIONS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS, GgitSubmoduleUpdateOptionsPrivate))

struct _GgitSubmoduleUpdateOptionsPrivate
{
	git_submodule_update_options options;

	GgitCheckoutOptions *checkout_options;
	GgitRemoteCallbacks *remote_callbacks;
	GgitSignature *signature;
};

G_DEFINE_TYPE (GgitSubmoduleUpdateOptions, ggit_submodule_update_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_CHECKOUT_OPTIONS,
	PROP_REMOTE_CALLBACKS,
	PROP_CLONE_CHECKOUT_STRATEGY,
	PROP_SIGNATURE
};

static void
ggit_submodule_update_options_finalize (GObject *object)
{
	GgitSubmoduleUpdateOptions *options;

	options = GGIT_SUBMODULE_UPDATE_OPTIONS (object);

	g_clear_object (&options->priv->checkout_options);
	g_clear_object (&options->priv->remote_callbacks);
	g_clear_object (&options->priv->signature);

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
	case PROP_REMOTE_CALLBACKS:
		ggit_submodule_update_options_set_remote_callbacks (self,
		                                                    g_value_get_object (value));
		break;
	case PROP_CLONE_CHECKOUT_STRATEGY:
		ggit_submodule_update_options_set_clone_checkout_strategy (self,
		                                                           g_value_get_flags (value));
		break;
	case PROP_SIGNATURE:
		ggit_submodule_update_options_set_signature (self,
		                                             g_value_get_object (value));
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
	case PROP_REMOTE_CALLBACKS:
		g_value_set_object (value, self->priv->remote_callbacks);
		break;
	case PROP_CLONE_CHECKOUT_STRATEGY:
		g_value_set_flags (value,
		                   ggit_submodule_update_options_get_clone_checkout_strategy (self));
		break;
	case PROP_SIGNATURE:
		g_value_set_object (value, self->priv->signature);
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
	                                 PROP_REMOTE_CALLBACKS,
	                                 g_param_spec_object ("remote-callbacks",
	                                                      "Remote Callbacks",
	                                                      "Remote callbacks",
	                                                      GGIT_TYPE_REMOTE_CALLBACKS,
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

	g_object_class_install_property (object_class,
	                                 PROP_SIGNATURE,
	                                 g_param_spec_object ("signature",
	                                                      "Signature",
	                                                      "Signature",
	                                                      GGIT_TYPE_SIGNATURE,
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
 * Gets the remote callbacks.
 *
 * Returns: (transfer none): a #GgitRemoteCallbacks.
 *
 **/
GgitRemoteCallbacks *
ggit_submodule_update_options_get_remote_callbacks (GgitSubmoduleUpdateOptions *options)
{
	g_return_val_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), NULL);

	return options->priv->remote_callbacks;

}

/**
 * ggit_submodule_update_options_set_remote_callbacks:
 * @options: a #GgitSubmoduleUpdateOptions.
 * @remote_callbacks: (allow-none): a #GgitRemoteCallbacks.
 *
 * Set the remote callbacks.
 **/
void
ggit_submodule_update_options_set_remote_callbacks (GgitSubmoduleUpdateOptions *options,
                                                    GgitRemoteCallbacks        *remote_callbacks)
{
	g_return_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options));
	g_return_if_fail (remote_callbacks == NULL || GGIT_IS_REMOTE_CALLBACKS (remote_callbacks));

	if (options->priv->remote_callbacks)
	{
		g_object_unref (options->priv->remote_callbacks);
		options->priv->remote_callbacks = NULL;

		git_remote_init_callbacks (&options->priv->options.remote_callbacks, GIT_REMOTE_CALLBACKS_VERSION);
	}

	if (remote_callbacks)
	{
		options->priv->remote_callbacks = g_object_ref (remote_callbacks);
		options->priv->options.remote_callbacks = *_ggit_remote_callbacks_get_native (options->priv->remote_callbacks);
	}

	g_object_notify (G_OBJECT (options), "remote-callbacks");
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

/**
 * ggit_submodule_update_options_get_signature:
 * @options: a #GgitSubmoduleUpdateOptions.
 *
 * Gets the identity used when updating the reflog or %NULL to
 * use the default signature using the config.
 *
 * Returns: (transfer none): a #GgitSignature or %NULL.
 *
 **/
GgitSignature *
ggit_submodule_update_options_get_signature (GgitSubmoduleUpdateOptions *options)
{
	g_return_val_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), NULL);

	return options->priv->signature;
}

/**
 * ggit_submodule_update_options_set_signature:
 * @options: a #GgitSubmoduleUpdateOptions.
 * @signature: (allow-none): a #GgitSignature.
 *
 * Sets the identity used when updating the reflog. Use %NULL to
 * use the default signature using the config.
 **/
void
ggit_submodule_update_options_set_signature (GgitSubmoduleUpdateOptions *options,
                                             GgitSignature              *signature)
{
	g_return_if_fail (GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options));
	g_return_if_fail (signature == NULL || GGIT_IS_SIGNATURE (signature));

	if (options->priv->signature)
	{
		g_object_unref (options->priv->signature);
		options->priv->signature = NULL;
		options->priv->options.signature = NULL;
	}

	if (signature)
	{
		options->priv->signature = g_object_ref (signature);
		options->priv->options.signature = _ggit_native_get (signature);
	}

	g_object_notify (G_OBJECT (options), "signature");
}

/* ex:set ts=8 noet: */
