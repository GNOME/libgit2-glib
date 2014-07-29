#include "ggit-cherry-pick-options.h"
#include "ggit-enum-types.h"
#include "ggit-checkout-options.h"
#include "ggit-merge-options.h"

#define GGIT_CHERRY_PICK_OPTIONS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_CHERRY_PICK_OPTIONS, GgitCherryPickOptionsPrivate))

struct _GgitCherryPickOptionsPrivate
{
	git_cherry_pick_options options;

	GgitCheckoutOptions *checkout_options;
	GgitMergeOptions *merge_options;
};

G_DEFINE_TYPE (GgitCherryPickOptions, ggit_cherry_pick_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_MAINLINE,
	PROP_CHECKOUT_OPTIONS,
	PROP_MERGE_OPTIONS
};

static void
ggit_cherry_pick_options_finalize (GObject *object)
{
	GgitCherryPickOptions *options;

	options = GGIT_CHERRY_PICK_OPTIONS (object);

	g_clear_object (&options->priv->checkout_options);
	g_clear_object (&options->priv->merge_options);

	G_OBJECT_CLASS (ggit_cherry_pick_options_parent_class)->finalize (object);
}

static void
ggit_cherry_pick_options_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
	GgitCherryPickOptions *self = GGIT_CHERRY_PICK_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_MAINLINE:
		self->priv->options.mainline = g_value_get_uint (value);
		break;
	case PROP_CHECKOUT_OPTIONS:
		ggit_cherry_pick_options_set_checkout_options (self,
		                                               g_value_get_object (value));
		break;
	case PROP_MERGE_OPTIONS:
		ggit_cherry_pick_options_set_merge_options (self,
		                                            g_value_get_object (value));
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
	GgitCherryPickOptions *self = GGIT_CHERRY_PICK_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_MAINLINE:
		g_value_set_uint (value, self->priv->options.mainline);
		break;
	case PROP_CHECKOUT_OPTIONS:
		g_value_set_object (value, self->priv->checkout_options);
		break;
	case PROP_MERGE_OPTIONS:
		g_value_set_object (value, self->priv->merge_options);
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
	git_cherry_pick_options defopts = GIT_CHERRY_PICK_OPTIONS_INIT;

	object_class->finalize = ggit_cherry_pick_options_finalize;

	object_class->get_property = ggit_cherry_pick_options_get_property;
	object_class->set_property = ggit_cherry_pick_options_set_property;

	g_type_class_add_private (object_class, sizeof (GgitCherryPickOptionsPrivate));

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

}

static void
ggit_cherry_pick_options_init (GgitCherryPickOptions *self)
{
	self->priv = GGIT_CHERRY_PICK_OPTIONS_GET_PRIVATE (self);

	git_cherry_pick_init_options (&self->priv->options, GIT_CHERRY_PICK_OPTIONS_VERSION);
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
ggit_cherry_pick_options_new ()
{
	return g_object_new (GGIT_TYPE_CHERRY_PICK_OPTIONS, NULL);
}

const git_cherry_pick_options *
_ggit_cherry_pick_options_get_cherry_pick_options (GgitCherryPickOptions *options)
{
	if (options != NULL)
	{
		// Make sure to synchronize the wrapped checkout options with the internal checkout options
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
	g_return_val_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options), 0);

	return options->priv->options.mainline;
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
	g_return_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options));

	options->priv->options.mainline = mainline;
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
	g_return_val_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options), NULL);

	return options->priv->checkout_options;
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
	g_return_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options));
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
	g_return_val_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options), NULL);

	return options->priv->merge_options;

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
	g_return_if_fail (GGIT_IS_CHERRY_PICK_OPTIONS (options));

	if (options->priv->merge_options)
	{
		ggit_merge_options_free (options->priv->merge_options);
		options->priv->merge_options = NULL;

		git_merge_init_options (&options->priv->options.merge_opts, GIT_MERGE_OPTIONS_VERSION);
	}

	if (merge_options)
	{
		options->priv->merge_options = ggit_merge_options_copy (merge_options);
		options->priv->options.merge_opts = *_ggit_merge_options_get_merge_options (options->priv->merge_options);
	}

	g_object_notify (G_OBJECT (options), "merge-options");
}

