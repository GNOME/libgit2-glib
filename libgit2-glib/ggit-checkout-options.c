#include "ggit-checkout-options.h"
#include "ggit-enum-types.h"
#include "ggit-tree.h"
#include "ggit-diff-file.h"

#define GGIT_CHECKOUT_OPTIONS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_CHECKOUT_OPTIONS, GgitCheckoutOptionsPrivate))

struct _GgitCheckoutOptionsPrivate
{
	git_checkout_options options;

	gchar **paths;
	GgitTree *baseline;

	gchar *target_directory;
	gchar *ancestor_label;
	gchar *our_label;
	gchar *their_label;
};

G_DEFINE_TYPE (GgitCheckoutOptions, ggit_checkout_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_STRATEGY,
	PROP_DISABLE_FILTERS,
	PROP_DIR_MODE,
	PROP_FILE_MODE,
	PROP_FILE_OPEN_FLAGS,
	PROP_NOTIFY_FLAGS,
	PROP_BASELINE,
	PROP_TARGET_DIRECTORY,
	PROP_ANCESTOR_LABEL,
	PROP_OUR_LABEL,
	PROP_THEIR_LABEL
};

static void
ggit_checkout_options_finalize (GObject *object)
{
	GgitCheckoutOptions *options;

	options = GGIT_CHECKOUT_OPTIONS (object);

	g_strfreev (options->priv->paths);
	g_clear_object (&options->priv->baseline);

	g_free (options->priv->target_directory);
	g_free (options->priv->ancestor_label);
	g_free (options->priv->our_label);
	g_free (options->priv->their_label);

	G_OBJECT_CLASS (ggit_checkout_options_parent_class)->finalize (object);
}

static void
ggit_checkout_options_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
	GgitCheckoutOptions *self = GGIT_CHECKOUT_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_STRATEGY:
		self->priv->options.checkout_strategy = g_value_get_flags (value);
		break;
	case PROP_DISABLE_FILTERS:
		self->priv->options.disable_filters = g_value_get_boolean (value);
		break;
	case PROP_DIR_MODE:
		self->priv->options.dir_mode = g_value_get_uint (value);
		break;
	case PROP_FILE_MODE:
		self->priv->options.file_mode = g_value_get_uint (value);
		break;
	case PROP_FILE_OPEN_FLAGS:
		self->priv->options.file_open_flags = g_value_get_int (value);
		break;
	case PROP_NOTIFY_FLAGS:
		self->priv->options.notify_flags = g_value_get_flags (value);
		break;
	case PROP_BASELINE:
		ggit_checkout_options_set_baseline (self,
		                                    g_value_get_object (value));
		break;
	case PROP_TARGET_DIRECTORY:
		ggit_checkout_options_set_target_directory (self,
		                                            g_value_get_string (value));
		break;
	case PROP_ANCESTOR_LABEL:
		ggit_checkout_options_set_ancestor_label (self,
		                                          g_value_get_string (value));
		break;
	case PROP_OUR_LABEL:
		ggit_checkout_options_set_our_label (self,
		                                     g_value_get_string (value));
		break;
	case PROP_THEIR_LABEL:
		ggit_checkout_options_set_their_label (self,
		                                       g_value_get_string (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_checkout_options_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
	GgitCheckoutOptions *self = GGIT_CHECKOUT_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_STRATEGY:
		g_value_set_flags (value, self->priv->options.checkout_strategy);
		break;
	case PROP_DISABLE_FILTERS:
		g_value_set_boolean (value, self->priv->options.disable_filters);
		break;
	case PROP_DIR_MODE:
		g_value_set_uint (value, self->priv->options.dir_mode);
		break;
	case PROP_FILE_MODE:
		g_value_set_uint (value, self->priv->options.file_mode);
		break;
	case PROP_FILE_OPEN_FLAGS:
		g_value_set_int (value, self->priv->options.file_open_flags);
		break;
	case PROP_NOTIFY_FLAGS:
		g_value_set_flags (value, self->priv->options.notify_flags);
		break;
	case PROP_BASELINE:
		g_value_set_object (value, self->priv->baseline);
		break;
	case PROP_TARGET_DIRECTORY:
		g_value_set_string (value, self->priv->target_directory);
		break;
	case PROP_ANCESTOR_LABEL:
		g_value_set_string (value, self->priv->ancestor_label);
		break;
	case PROP_OUR_LABEL:
		g_value_set_string (value, self->priv->our_label);
		break;
	case PROP_THEIR_LABEL:
		g_value_set_string (value, self->priv->their_label);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
progress_callback_wrapper (const gchar *path,
                           gsize        completed_steps,
                           gsize        total_steps,
                           gpointer     payload)
{
	GgitCheckoutOptions *options = payload;

	GGIT_CHECKOUT_OPTIONS_GET_CLASS (options)->progress (options,
	                                                     path,
	                                                     completed_steps,
	                                                     total_steps);
}

static gint
notify_callback_wrapper (git_checkout_notify_t  flags,
                         const gchar           *path,
                         const git_diff_file   *baseline,
                         const git_diff_file   *target,
                         const git_diff_file   *workdir,
                         gpointer               payload)
{
	GgitCheckoutOptions *options = payload;
	GgitDiffFile *gbaseline;
	GgitDiffFile *gtarget;
	GgitDiffFile *gworkdir;
	GgitCheckoutNotifyFlags gflags;
	gint ret;

	gbaseline = _ggit_diff_file_wrap (baseline);
	gtarget = _ggit_diff_file_wrap (target);
	gworkdir = _ggit_diff_file_wrap (workdir);

	gflags = (GgitCheckoutNotifyFlags)flags;

	ret = GGIT_CHECKOUT_OPTIONS_GET_CLASS (options)->notify (options,
	                                                         gflags,
	                                                         path,
	                                                         gbaseline,
	                                                         gtarget,
	                                                         gworkdir);

	ggit_diff_file_unref (gbaseline);
	ggit_diff_file_unref (gtarget);
	ggit_diff_file_unref (gworkdir);

	return ret;
}

static void
ggit_checkout_options_constructed (GObject *object)
{
	GgitCheckoutOptions *options;

	options = GGIT_CHECKOUT_OPTIONS (object);

	if (GGIT_CHECKOUT_OPTIONS_GET_CLASS (object)->progress != NULL)
	{
		options->priv->options.progress_cb = progress_callback_wrapper;
		options->priv->options.progress_payload = options;
	}

	if (GGIT_CHECKOUT_OPTIONS_GET_CLASS (object)->notify != NULL)
	{
		options->priv->options.notify_cb = notify_callback_wrapper;
		options->priv->options.notify_payload = options;
	}
}

static void
ggit_checkout_options_class_init (GgitCheckoutOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	git_checkout_options defaultopts = GIT_CHECKOUT_OPTIONS_INIT;

	object_class->finalize = ggit_checkout_options_finalize;

	object_class->get_property = ggit_checkout_options_get_property;
	object_class->set_property = ggit_checkout_options_set_property;

	object_class->constructed = ggit_checkout_options_constructed;

	g_type_class_add_private (object_class, sizeof (GgitCheckoutOptionsPrivate));

	/**
	 * GgitCheckoutOptions:strategy: (type GgitCheckoutStrategy):
	 *
	 * The checkout strategy.
	 */
	g_object_class_install_property (object_class,
	                                 PROP_STRATEGY,
	                                 g_param_spec_flags ("strategy",
	                                                     "Strategy",
	                                                     "Strategy",
	                                                     GGIT_TYPE_CHECKOUT_STRATEGY,
	                                                     defaultopts.checkout_strategy,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_DISABLE_FILTERS,
	                                 g_param_spec_boolean ("disable-filters",
	                                                       "Disable Filters",
	                                                       "Disable filters",
	                                                       defaultopts.disable_filters,
	                                                       G_PARAM_READWRITE |
	                                                       G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_DIR_MODE,
	                                 g_param_spec_uint ("dir-mode",
	                                                    "Dir Mode",
	                                                    "Dir mode",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    defaultopts.dir_mode,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_FILE_MODE,
	                                 g_param_spec_uint ("file-mode",
	                                                    "File Mode",
	                                                    "File mode",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    defaultopts.file_mode,
	                                                    G_PARAM_READWRITE |
	                                                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_FILE_OPEN_FLAGS,
	                                 g_param_spec_int ("file-open-flags",
	                                                   "File Open Flags",
	                                                   "File open flags",
	                                                   G_MININT,
	                                                   G_MAXINT,
	                                                   defaultopts.file_open_flags,
	                                                   G_PARAM_READWRITE |
	                                                   G_PARAM_STATIC_STRINGS));

	/**
	 * GgitCheckoutOptions:notify-flags: (type GgitCheckoutNotifyFlags):
	 *
	 * The checkout notify flags.
	 */
	g_object_class_install_property (object_class,
	                                 PROP_NOTIFY_FLAGS,
	                                 g_param_spec_flags ("notify-flags",
	                                                     "Notify Flags",
	                                                     "Notify flags",
	                                                     GGIT_TYPE_CHECKOUT_NOTIFY_FLAGS,
	                                                     defaultopts.notify_flags,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_BASELINE,
	                                 g_param_spec_object ("baseline",
	                                                      "Baseline",
	                                                      "Baseline",
	                                                      GGIT_TYPE_TREE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_TARGET_DIRECTORY,
	                                 g_param_spec_string ("target-directory",
	                                                      "Target Directory",
	                                                      "Target directory",
	                                                      defaultopts.target_directory,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_ANCESTOR_LABEL,
	                                 g_param_spec_string ("ancestor-label",
	                                                      "Ancestor Label",
	                                                      "Ancestor label",
	                                                      defaultopts.ancestor_label,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_OUR_LABEL,
	                                 g_param_spec_string ("our-label",
	                                                      "Our Label",
	                                                      "Our label",
	                                                      defaultopts.our_label,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_THEIR_LABEL,
	                                 g_param_spec_string ("their-label",
	                                                      "Their Label",
	                                                      "Their label",
	                                                      defaultopts.their_label,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));
}

static void
ggit_checkout_options_init (GgitCheckoutOptions *self)
{
	self->priv = GGIT_CHECKOUT_OPTIONS_GET_PRIVATE (self);

	git_checkout_init_options (&self->priv->options, GIT_CHECKOUT_OPTIONS_VERSION);
}

/**
 * ggit_checkout_options_new:
 *
 * Create a new checkout options object.
 *
 * Returns: a #GgitCheckoutOptions.
 *
 **/
GgitCheckoutOptions *
ggit_checkout_options_new ()
{
	return g_object_new (GGIT_TYPE_CHECKOUT_OPTIONS, NULL);
}

const git_checkout_options *
_ggit_checkout_options_get_checkout_options (GgitCheckoutOptions *options)
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
 * ggit_checkout_options_get_strategy:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the checkout strategy.
 *
 * Returns: a #GgitCheckoutStrategy.
 *
 **/
GgitCheckoutStrategy
ggit_checkout_options_get_strategy (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), 0);

	return (GgitCheckoutStrategy)options->priv->options.checkout_strategy;
}

/**
 * ggit_checkout_options_set_strategy:
 * @options: a #GgitCheckoutOptions.
 * @strategy: a #GgitCheckoutStrategy.
 *
 * Set the checkout strategy.
 *
 **/
void
ggit_checkout_options_set_strategy (GgitCheckoutOptions  *options,
                                    GgitCheckoutStrategy  strategy)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	options->priv->options.checkout_strategy = strategy;
	g_object_notify (G_OBJECT (options), "strategy");
}

/**
 * ggit_checkout_options_get_disable_filters:
 * @options: a #GgitCheckoutOptions.
 *
 * Get whether filters are disabled.
 *
 * Returns: %TRUE if filters are disabled, %FALSE otherwise.
 *
 **/
gboolean
ggit_checkout_options_get_disable_filters (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), FALSE);

	return (gboolean)options->priv->options.disable_filters;
}

/**
 * ggit_checkout_options_set_disable_filters:
 * @options: a #GgitCheckoutOptions.
 * @disable: disable filters.
 *
 * Set whether to disable filters.
 *
 **/
void
ggit_checkout_options_set_disable_filters (GgitCheckoutOptions *options,
                                           gboolean             disable)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	options->priv->options.disable_filters = (gboolean)disable;
	g_object_notify (G_OBJECT (options), "disable-filters");
}

/**
 * ggit_checkout_options_get_dir_mode:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the default checkout directory mode.
 *
 * Returns: the default directory mode.
 *
 **/
guint
ggit_checkout_options_get_dir_mode (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), 0);

	return (guint)options->priv->options.dir_mode;
}

/**
 * ggit_checkout_options_set_dir_mode:
 * @options: a #GgitCheckoutOptions.
 * @dir_mode: the dir mode.
 *
 * Set the default checkout directory mode.
 *
 **/
void
ggit_checkout_options_set_dir_mode (GgitCheckoutOptions *options,
                                    guint                dir_mode)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	options->priv->options.dir_mode = dir_mode;
	g_object_notify (G_OBJECT (options), "dir-mode");
}

/**
 * ggit_checkout_options_get_file_mode:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the default checkout file mode.
 *
 * Returns: the default checkout file mode.
 *
 **/
guint
ggit_checkout_options_get_file_mode (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), 0);

	return (guint)options->priv->options.file_mode;
}

/**
 * ggit_checkout_options_set_file_mode:
 * @options: a #GgitCheckoutOptions.
 * @file_mode: the file mode.
 *
 * Set the default checkout file mode.
 *
 **/
void
ggit_checkout_options_set_file_mode (GgitCheckoutOptions *options,
                                     guint                file_mode)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	options->priv->options.file_mode = file_mode;
	g_object_notify (G_OBJECT (options), "file-mode");
}

/**
 * ggit_checkout_options_get_file_open_flags:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the checkout file open flags. These flags are platform specific,
 * e.g. on Unix these would include O_CREAT, O_TRUNC, etc.
 *
 * Returns: the checkout file open flags.
 *
 **/
gint
ggit_checkout_options_get_file_open_flags (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), 0);

	return options->priv->options.file_open_flags;
}

/**
 * ggit_checkout_options_set_file_open_flags:
 * @options: a #GgitCheckoutOptions.
 * @flags: the file open flags.
 *
 * Set the checkout file open flags. These flags are platform dependent,
 * e.g. on Unix use O_CREAT, O_TRUNC, etc.
 *
 **/
void
ggit_checkout_options_set_file_open_flags (GgitCheckoutOptions *options,
                                           gint                 flags)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	options->priv->options.file_open_flags = flags;
	g_object_notify (G_OBJECT (options), "file-open-flags");
}

/**
 * ggit_checkout_options_get_notify_flags:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the checkout notify flags.
 *
 * Returns: a #GgitCheckoutNotifyFlags.
 *
 **/
GgitCheckoutNotifyFlags
ggit_checkout_options_get_notify_flags (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), 0);

	return (GgitCheckoutNotifyFlags)options->priv->options.notify_flags;
}

/**
 * ggit_checkout_options_set_notify_flags:
 * @options: a #GgitCheckoutOptions.
 * @flags: a #GgitCheckoutNotifyFlags.
 *
 * Set the checkout notify flags.
 *
 **/
void
ggit_checkout_options_set_notify_flags (GgitCheckoutOptions     *options,
                                        GgitCheckoutNotifyFlags  flags)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	options->priv->options.notify_flags = flags;
	g_object_notify (G_OBJECT (options), "notify-flags");
}

/**
 * ggit_checkout_options_get_paths:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the list of file paths to checkout.
 *
 * Returns: (array zero-terminated=1) (allow-none): a %NULL terminated list of file paths, or %NULL.
 *
 **/
const gchar * const *
ggit_checkout_options_get_paths (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), NULL);

	return (const gchar * const *)options->priv->paths;
}

/**
 * ggit_checkout_options_set_paths:
 * @options: a #GgitCheckoutOptions.
 * @paths: (array zero-terminated=1) (allow-none): a %NULL terminated list of paths.
 *
 * Set the list of file paths to checkout. If @paths is %NULL, then all files
 * will be checked out.
 *
 **/
void
ggit_checkout_options_set_paths (GgitCheckoutOptions *options,
                                 const gchar * const *paths)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	g_strfreev (options->priv->paths);

	options->priv->paths = g_strdupv ((gchar **)paths);
	options->priv->options.paths.strings = options->priv->paths;
	options->priv->options.paths.count = g_strv_length (options->priv->paths);

	g_object_notify (G_OBJECT (options), "paths");
}

/**
 * ggit_checkout_options_get_baseline:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the baseline, i.e. the expected content of workdir. Defaults to HEAD.
 *
 * Returns: (transfer none): a #GgitTree.
 *
 **/
GgitTree *
ggit_checkout_options_get_baseline (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), NULL);

	return options->priv->baseline;
}

/**
 * ggit_checkout_options_set_baseline:
 * @options: a #GgitCheckoutOptions.
 * @tree: (allow-none): a #GgitTree.
 *
 * Set the baseline, i.e. the expected content of workdir. If @tree is set
 * to %NULL, the default (HEAD) will be used as the baseline.
 *
 **/
void
ggit_checkout_options_set_baseline (GgitCheckoutOptions *options,
                                    GgitTree            *tree)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));
	g_return_if_fail (tree == NULL || GGIT_IS_TREE (tree));

	if (options->priv->baseline)
	{
		g_object_unref (options->priv->baseline);
	}

	if (tree)
	{
		options->priv->baseline = g_object_ref (tree);
		options->priv->options.baseline = _ggit_native_get (tree);
	}
	else
	{
		options->priv->baseline = NULL;
		options->priv->options.baseline = NULL;
	}

	g_object_notify (G_OBJECT (options), "baseline");
}

/**
 * ggit_checkout_options_get_target_directory:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the checkout target directory.
 *
 * Returns: (allow-none): the checkout target directory.
 *
 **/
const gchar *
ggit_checkout_options_get_target_directory (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), NULL);

	return options->priv->target_directory;
}

/**
 * ggit_checkout_options_set_target_directory:
 * @options: a #GgitCheckoutOptions.
 * @directory: (allow-none): the target directory.
 *
 * Set the checkout target directory.
 *
 **/
void
ggit_checkout_options_set_target_directory (GgitCheckoutOptions *options,
                                            const gchar         *directory)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	g_free (options->priv->target_directory);
	options->priv->target_directory = g_strdup (directory);

	options->priv->options.target_directory = options->priv->target_directory;

	g_object_notify (G_OBJECT (options), "target-directory");
}

/**
 * ggit_checkout_options_get_ancestor_label:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the checkout ancestor label.
 *
 * Returns: (allow-none): the checkout ancestor label.
 *
 **/
const gchar *
ggit_checkout_options_get_ancestor_label (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), NULL);

	return options->priv->ancestor_label;
}

/**
 * ggit_checkout_options_set_ancestor_label:
 * @options: a #GgitCheckoutOptions.
 * @label: (allow-none): the ancestor label.
 *
 * Set the checkout ancestor label.
 *
 **/
void
ggit_checkout_options_set_ancestor_label (GgitCheckoutOptions *options,
                                          const gchar         *label)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	g_free (options->priv->ancestor_label);
	options->priv->ancestor_label = g_strdup (label);

	options->priv->options.ancestor_label = options->priv->ancestor_label;

	g_object_notify (G_OBJECT (options), "ancestor-label");
}

/**
 * ggit_checkout_options_get_our_label:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the checkout our label.
 *
 * Returns: (allow-none): the checkout our label.
 *
 **/
const gchar *
ggit_checkout_options_get_our_label (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), NULL);

	return options->priv->our_label;
}

/**
 * ggit_checkout_options_set_our_label:
 * @options: a #GgitCheckoutOptions.
 * @label: (allow-none): the our label.
 *
 * Set the checkout our label.
 *
 **/
void
ggit_checkout_options_set_our_label (GgitCheckoutOptions *options,
                                     const gchar         *label)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	g_free (options->priv->our_label);
	options->priv->our_label = g_strdup (label);

	options->priv->options.our_label = options->priv->our_label;

	g_object_notify (G_OBJECT (options), "our-label");
}

/**
 * ggit_checkout_options_get_their_label:
 * @options: a #GgitCheckoutOptions.
 *
 * Get the checkout their label.
 *
 * Returns: (allow-none): the checkout their label.
 *
 **/
const gchar *
ggit_checkout_options_get_their_label (GgitCheckoutOptions *options)
{
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), NULL);

	return options->priv->their_label;
}

/**
 * ggit_checkout_options_set_their_label:
 * @options: a #GgitCheckoutOptions.
 * @label: (allow-none): the their label.
 *
 * Set the checkout their label.
 *
 **/
void
ggit_checkout_options_set_their_label (GgitCheckoutOptions *options,
                                       const gchar         *label)
{
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options));

	g_free (options->priv->their_label);
	options->priv->their_label = g_strdup (label);

	options->priv->options.their_label = options->priv->their_label;

	g_object_notify (G_OBJECT (options), "their-label");
}

