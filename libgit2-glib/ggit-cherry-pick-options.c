/*
 * ggit-cherry-pick-options.c
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

#include "ggit-cherry-pick-options.h"
#include "ggit-enum-types.h"
#include "ggit-checkout-options.h"
#include "ggit-merge-options.h"

/**
 * GgitCherryPickOptions:
 *
 * Represents the options used when doign a cherry-pick.
 */

typedef struct _GgitCherryPickOptionsPrivate
{
	git_cherrypick_options options;

	GgitCheckoutOptions *checkout_options;
	GgitMergeOptions *merge_options;
} GgitCherryPickOptionsPrivate;

enum
{
	PROP_0,
	PROP_MAINLINE,
	PROP_CHECKOUT_OPTIONS,
	PROP_MERGE_OPTIONS
};

G_DEFINE_TYPE_WITH_PRIVATE (GgitCherryPickOptions, ggit_cherry_pick_options, G_TYPE_OBJECT)

static void
ggit_cherry_pick_options_finalize (GObject *object)
{
	GgitCherryPickOptions *options;
	GgitCherryPickOptionsPrivate *priv;

	options = GGIT_CHERRY_PICK_OPTIONS (object);
	priv = ggit_cherry_pick_options_get_instance_private (options);

	g_clear_object (&priv->checkout_options);
	g_clear_pointer (&priv->merge_options, ggit_merge_options_free);

	G_OBJECT_CLASS (ggit_cherry_pick_options_parent_class)->finalize (object);
}

static void
ggit_cherry_pick_options_set_property (GObject      *object,
                                       guint         prop_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
	GgitCherryPickOptions *options = GGIT_CHERRY_PICK_OPTIONS (object);
	GgitCherryPickOptionsPrivate *priv;

	priv = ggit_cherry_pick_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_MAINLINE:
		priv->options.mainline = g_value_get_uint (value);
		break;
	case PROP_CHECKOUT_OPTIONS:
		ggit_cherry_pick_options_set_checkout_options (options,
		                                               g_value_get_object (value));
		break;
	case PROP_MERGE_OPTIONS:
		ggit_cherry_pick_options_set_merge_options (options,
		                                            g_value_get_boxed (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_cherry_pick_options_get_property (GObject    *object,
                                       guint       prop_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
	GgitCherryPickOptions *options = GGIT_CHERRY_PICK_OPTIONS (object);
	GgitCherryPickOptionsPrivate *priv;

	priv = ggit_cherry_pick_options_get_instance_private (options);

	switch (prop_id)
	{
	case PROP_MAINLINE:
		g_value_set_uint (value, priv->options.mainline);
		break;
	case PROP_CHECKOUT_OPTIONS:
		g_value_set_object (value, priv->checkout_options);
		break;
	case PROP_MERGE_OPTIONS:
		g_value_set_boxed (value, priv->merge_options);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_cherry_pick_options_class_init (GgitCherryPickOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	git_cherrypick_options defopts = GIT_CHERRYPICK_OPTIONS_INIT;

	object_class->finalize = ggit_cherry_pick_options_finalize;
	object_class->get_property = ggit_cherry_pick_options_get_property;
	object_class->set_property = ggit_cherry_pick_options_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_MAINLINE,
	                                 g_param_spec_uint ("mainline",
	                                                    "Mainline",
	                                                    "Mainline",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    defopts.mainline,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_CHECKOUT_OPTIONS,
	                                 g_param_spec_object ("checkout-options",
	                                                      "Checkout Options",
	                                                      "Checkout options",
	                                                      GGIT_TYPE_CHECKOUT_OPTIONS,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_MERGE_OPTIONS,
	                                 g_param_spec_boxed ("merge-options",
	                                                     "Merge Options",
	                                                     "Merge options",
	                                                     GGIT_TYPE_MERGE_OPTIONS,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));
}

static void
ggit_cherry_pick_options_init (GgitCherryPickOptions *options)
{
	GgitCherryPickOptionsPrivate *priv;

	priv = ggit_cherry_pick_options_get_instance_private (options);

	git_cherrypick_init_options (&priv->options, GIT_CHERRYPICK_OPTIONS_VERSION);
}

/**
 * ggit_cherry_pick_options_new:
 *
 * Create a new cherry-pick options object.
 *
 * Returns: a #GgitCherryPickOptions.
 *
 **/
GgitCherryPickOptions *
ggit_cherry_pick_options_new (void)
{
	return g_object_new (GGIT_TYPE_CHERRY_PICK_OPTIONS, NULL);
}

const git_cherrypick_options *
_ggit_cherry_pick_options_get_cherry_pick_options (GgitCherryPickOptions *options)
{
	if (options != NULL)
	{
		GgitCherryPickOptionsPrivate *priv;

		priv = ggit_cherry_pick_options_get_instance_private (options);

		// Make sure to synchronize the wrapped checkout options with the internal checkout options
		if (priv->checkout_options)
		{
			priv->options.checkout_opts =
				*_ggit_checkout_options_get_checkout_options (priv->checkout_options);
		}

		// Make sure to synchronize the wrapped merge options with the internal merge options
		if (priv->merge_options)
		{
			priv->options.merge_opts =
				*_ggit_merge_options_get_merge_options (priv->merge_options);
		}

		return &priv->options;
	}
	else
	{
		return NULL;
	}
}

/**
 * ggit_cherry_pick_options_get_mainline:
 * @options: a #GgitCherryPickOptions.
 *
 * Get the mainline parent to use when cherry-picking a merge commit.
 *
 * Returns: the mainline parent.
 *
 **/
guint
ggit_cherry_pick_options_get_mainline (GgitCherryPickOptions *options)
{
	GgitCherryPickOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options), 0);

	priv = ggit_cherry_pick_options_get_instance_private (options);

	return priv->options.mainline;
}

/**
 * ggit_cherry_pick_options_set_mainline:
 * @options: a #GgitCherryPickOptions.
 * @mainline: the mainline parent.
 *
 * Set the mainline parent to use when cherry-picking a merge commit.
 *
 **/
void
ggit_cherry_pick_options_set_mainline (GgitCherryPickOptions *options,
                                       guint                  mainline)
{
	GgitCherryPickOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options));

	priv = ggit_cherry_pick_options_get_instance_private (options);

	priv->options.mainline = mainline;
	g_object_notify (G_OBJECT (options), "mainline");
}

/**
 * ggit_cherry_pick_options_get_checkout_options:
 * @options: a #GgitCherryPickOptions.
 *
 * Get the checkout options.
 *
 * Returns: (transfer none): a #GgitCheckoutOptions.
 *
 **/
GgitCheckoutOptions *
ggit_cherry_pick_options_get_checkout_options (GgitCherryPickOptions *options)
{
	GgitCherryPickOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options), NULL);

	priv = ggit_cherry_pick_options_get_instance_private (options);

	return priv->checkout_options;
}

/**
 * ggit_cherry_pick_options_set_checkout_options:
 * @options: a #GgitCherryPickOptions.
 * @checkout_options: (allow-none): a #GgitCheckoutOptions.
 *
 * Set the checkout options.
 *
 **/
void
ggit_cherry_pick_options_set_checkout_options (GgitCherryPickOptions *options,
                                               GgitCheckoutOptions   *checkout_options)
{
	GgitCherryPickOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options));
	g_return_if_fail (checkout_options == NULL || GGIT_IS_CHECKOUT_OPTIONS (checkout_options));

	priv = ggit_cherry_pick_options_get_instance_private (options);

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
 * ggit_cherry_pick_options_get_merge_options:
 * @options: a #GgitCherryPickOptions.
 *
 * Get the merge options.
 *
 * Returns: (transfer full): a #GgitMergeOptions.
 *
 **/
GgitMergeOptions *
ggit_cherry_pick_options_get_merge_options (GgitCherryPickOptions *options)
{
	GgitCherryPickOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options), NULL);

	priv = ggit_cherry_pick_options_get_instance_private (options);

	return priv->merge_options;

}

/**
 * ggit_cherry_pick_options_set_merge_options:
 * @options: a #GgitCherryPickOptions.
 * @merge_options: (allow-none): a #GgitMergeOptions.
 *
 * Set the merge options.
 *
 **/
void
ggit_cherry_pick_options_set_merge_options (GgitCherryPickOptions *options,
                                            GgitMergeOptions      *merge_options)
{
	GgitCherryPickOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options));

	priv = ggit_cherry_pick_options_get_instance_private (options);

	if (priv->merge_options)
	{
		ggit_merge_options_free (priv->merge_options);
		priv->merge_options = NULL;

		git_merge_init_options (&priv->options.merge_opts, GIT_MERGE_OPTIONS_VERSION);
	}

	if (merge_options)
	{
		priv->merge_options = ggit_merge_options_copy (merge_options);
		priv->options.merge_opts = *_ggit_merge_options_get_merge_options (priv->merge_options);
	}

	g_object_notify (G_OBJECT (options), "merge-options");
}

/* ex:set ts=8 noet: */
