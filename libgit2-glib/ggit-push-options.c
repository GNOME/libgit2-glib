#include "ggit-push-options.h"

#define GGIT_PUSH_OPTIONS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_PUSH_OPTIONS, GgitPushOptionsPrivate))

struct _GgitPushOptionsPrivate
{
	git_push_options options;
};

G_DEFINE_TYPE (GgitPushOptions, ggit_push_options, G_TYPE_OBJECT)

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
	GgitPushOptions *self = GGIT_PUSH_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_PARALLELISM:
		self->priv->options.pb_parallelism = g_value_get_int (value);
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
	GgitPushOptions *self = GGIT_PUSH_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_PARALLELISM:
		g_value_set_int (value, self->priv->options.pb_parallelism);
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

	g_type_class_add_private (object_class, sizeof (GgitPushOptionsPrivate));

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
ggit_push_options_init (GgitPushOptions *self)
{
	self->priv = GGIT_PUSH_OPTIONS_GET_PRIVATE (self);

	git_push_init_options (&self->priv->options, GIT_PUSH_OPTIONS_VERSION);
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
		return &options->priv->options;
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
	g_return_val_if_fail (GGIT_IS_PUSH_OPTIONS (options), 0);

	return options->priv->options.pb_parallelism;
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
	g_return_if_fail (GGIT_IS_PUSH_OPTIONS (options));
	g_return_if_fail (parallelism >= 0);

	options->priv->options.pb_parallelism = parallelism;
	g_object_notify (G_OBJECT (options), "parallelism");
}
