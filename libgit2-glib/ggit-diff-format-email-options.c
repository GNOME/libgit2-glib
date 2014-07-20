#include "ggit-diff-format-email-options.h"
#include "ggit-enum-types.h"
#include "ggit-signature.h"
#include "ggit-oid.h"

#define GGIT_DIFF_FORMAT_EMAIL_OPTIONS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS, GgitDiffFormatEmailOptionsPrivate))

struct _GgitDiffFormatEmailOptionsPrivate
{
	git_diff_format_email_options options;

	GgitOId *id;
	gchar *summary;
	GgitSignature *author;
};

G_DEFINE_TYPE (GgitDiffFormatEmailOptions, ggit_diff_format_email_options, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_FLAGS,
	PROP_PATCH_NUMBER,
	PROP_TOTAL_PATCHES,
	PROP_ID,
	PROP_SUMMARY,
	PROP_AUTHOR
};

static void
ggit_diff_format_email_options_finalize (GObject *object)
{
	GgitDiffFormatEmailOptions *options;

	options = GGIT_DIFF_FORMAT_EMAIL_OPTIONS (object);

	if (options->priv->id)
	{
		ggit_oid_free (options->priv->id);
	}

	g_free (options->priv->summary);
	g_clear_object (&options->priv->author);

	G_OBJECT_CLASS (ggit_diff_format_email_options_parent_class)->finalize (object);
}

static void
ggit_diff_format_email_options_set_property (GObject      *object,
                                             guint         prop_id,
                                             const GValue *value,
                                             GParamSpec   *pspec)
{
	GgitDiffFormatEmailOptions *self = GGIT_DIFF_FORMAT_EMAIL_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_FLAGS:
		self->priv->options.flags = g_value_get_flags (value);
		break;
	case PROP_PATCH_NUMBER:
		self->priv->options.patch_no = g_value_get_uint64 (value);
		break;
	case PROP_TOTAL_PATCHES:
		self->priv->options.total_patches = g_value_get_uint64 (value);
		break;
	case PROP_ID:
		ggit_diff_format_email_options_set_id (self, g_value_get_boxed (value));
		break;
	case PROP_SUMMARY:
		ggit_diff_format_email_options_set_summary (self, g_value_get_string (value));
		break;
	case PROP_AUTHOR:
		ggit_diff_format_email_options_set_author (self, g_value_get_object (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_diff_format_email_options_get_property (GObject    *object,
                                             guint       prop_id,
                                             GValue     *value,
                                             GParamSpec *pspec)
{
	GgitDiffFormatEmailOptions *self = GGIT_DIFF_FORMAT_EMAIL_OPTIONS (object);

	switch (prop_id)
	{
	case PROP_FLAGS:
		g_value_set_flags (value, self->priv->options.flags);
		break;
	case PROP_PATCH_NUMBER:
		g_value_set_uint64 (value, self->priv->options.patch_no);
		break;
	case PROP_TOTAL_PATCHES:
		g_value_set_uint64 (value, self->priv->options.total_patches);
		break;
	case PROP_ID:
		g_value_set_boxed (value, self->priv->id);
		break;
	case PROP_SUMMARY:
		g_value_set_string (value, self->priv->summary);
		break;
	case PROP_AUTHOR:
		g_value_set_object (value, self->priv->author);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_diff_format_email_options_class_init (GgitDiffFormatEmailOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	git_diff_format_email_options defopts = GIT_DIFF_FORMAT_EMAIL_OPTIONS_INIT;

	object_class->finalize = ggit_diff_format_email_options_finalize;

	object_class->get_property = ggit_diff_format_email_options_get_property;
	object_class->set_property = ggit_diff_format_email_options_set_property;


	g_type_class_add_private (object_class, sizeof (GgitDiffFormatEmailOptionsPrivate));

	g_object_class_install_property (object_class,
	                                 PROP_FLAGS,
	                                 g_param_spec_flags ("flags",
	                                                     "Flags",
	                                                     "Flags",
	                                                     GGIT_TYPE_DIFF_FORMAT_EMAIL_FLAGS,
	                                                     defopts.flags,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_PATCH_NUMBER,
	                                 g_param_spec_uint64 ("patch-number",
	                                                      "Patch Number",
	                                                      "Patch number",
	                                                      0,
	                                                      G_MAXUINT64,
	                                                      defopts.patch_no,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_TOTAL_PATCHES,
	                                 g_param_spec_uint64 ("total-patches",
	                                                      "Total Patches",
	                                                      "Total patches",
	                                                      0,
	                                                      G_MAXUINT64,
	                                                      defopts.total_patches,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_ID,
	                                 g_param_spec_boxed ("id",
	                                                     "Id",
	                                                     "Id",
	                                                     GGIT_TYPE_OID,
	                                                     G_PARAM_READWRITE |
	                                                     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_SUMMARY,
	                                 g_param_spec_string ("summary",
	                                                      "Summary",
	                                                      "Summary",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_AUTHOR,
	                                 g_param_spec_object ("author",
	                                                      "Author",
	                                                      "Author",
	                                                      GGIT_TYPE_SIGNATURE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_STATIC_STRINGS));
}

static void
ggit_diff_format_email_options_init (GgitDiffFormatEmailOptions *self)
{
	self->priv = GGIT_DIFF_FORMAT_EMAIL_OPTIONS_GET_PRIVATE (self);

	git_diff_format_email_init_options (&self->priv->options,
	                                    GIT_DIFF_FORMAT_EMAIL_OPTIONS_VERSION);
}

/**
 * ggit_diff_format_email_options_new:
 *
 * Create a new checkout options object.
 *
 * Returns: a #GgitDiffFormatEmailOptions.
 *
 **/
GgitDiffFormatEmailOptions *
ggit_diff_format_email_options_new ()
{
	return g_object_new (GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS, NULL);
}

const git_diff_format_email_options *
_ggit_diff_format_email_options_get_diff_format_email_options (GgitDiffFormatEmailOptions *options)
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
 * ggit_diff_format_email_options_get_flags:
 * @options: a #GgitDiffFormatEmailOptions.
 *
 * Get the flags.
 *
 * Returns: a #GgitDiffFormatEmailFlags.
 *
 **/
GgitDiffFormatEmailFlags
ggit_diff_format_email_options_get_flags (GgitDiffFormatEmailOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options), 0);

	return (GgitDiffFormatEmailFlags)options->priv->options.flags;
}

/**
 * ggit_diff_format_email_options_set_flags:
 * @options: a #GgitDiffFormatEmailOptions.
 * @flags: a #GgitDiffFormatEmailFlags.
 *
 * Set the flags.
 *
 **/
void
ggit_diff_format_email_options_set_flags (GgitDiffFormatEmailOptions *options,
                                          GgitDiffFormatEmailFlags    flags)
{
	g_return_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options));

	options->priv->options.flags = (git_diff_format_email_flags_t)flags;
	g_object_notify (G_OBJECT (options), "flags");
}

/**
 * ggit_diff_format_email_options_get_patch_number:
 * @options: a #GgitDiffFormatEmailOptions.
 *
 * Get the patch number.
 *
 * Returns: the patch number.
 *
 **/
gsize
ggit_diff_format_email_options_get_patch_number (GgitDiffFormatEmailOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options), 0);

	return options->priv->options.patch_no;
}

/**
 * ggit_diff_format_email_options_set_patch_number:
 * @options: a #GgitDiffFormatEmailOptions.
 * @number: the patch number.
 *
 * Set the patch number.
 *
 **/
void
ggit_diff_format_email_options_set_patch_number (GgitDiffFormatEmailOptions *options,
                                                 gsize                       number)
{
	g_return_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options));

	options->priv->options.patch_no = number;
	g_object_notify (G_OBJECT (options), "patch-number");
}

/**
 * ggit_diff_format_email_options_get_total_patches:
 * @options: a #GgitDiffFormatEmailOptions.
 *
 * Get the total number of patches.
 *
 * Returns: the total number of patches.
 *
 **/
gsize
ggit_diff_format_email_options_get_total_patches (GgitDiffFormatEmailOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options), 0);

	return options->priv->options.total_patches;
}

/**
 * ggit_diff_format_email_options_set_total_patches:
 * @options: a #GgitDiffFormatEmailOptions.
 * @patches: the total number of patches.
 *
 * Set the total number of patches.
 *
 **/
void
ggit_diff_format_email_options_set_total_patches (GgitDiffFormatEmailOptions *options,
                                                  gsize                       patches)
{
	g_return_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options));

	options->priv->options.total_patches = patches;
	g_object_notify (G_OBJECT (options), "total-patches");
}

/**
 * ggit_diff_format_email_options_get_id:
 * @options: a #GgitDiffFormatEmailOptions.
 *
 * Get the object id.
 *
 * Returns: (transfer none): a #GgitOId.
 *
 **/
GgitOId *
ggit_diff_format_email_options_get_id (GgitDiffFormatEmailOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options), NULL);

	return options->priv->id;
}

/**
 * ggit_diff_format_email_options_set_id:
 * @options: a #GgitDiffFormatEmailOptions.
 * @id: (allow-none): a #GgitOId.
 *
 * Set the object id.
 *
 **/
void
ggit_diff_format_email_options_set_id (GgitDiffFormatEmailOptions *options,
                                       GgitOId                    *id)
{
	g_return_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options));

	if (options->priv->id)
	{
		ggit_oid_free (options->priv->id);
		options->priv->id = NULL;
		options->priv->options.id = NULL;
	}

	if (id != NULL)
	{
		options->priv->id = ggit_oid_copy (id);
		options->priv->options.id = _ggit_oid_get_oid (options->priv->id);
	}

	g_object_notify (G_OBJECT (options), "id");
}

/**
 * ggit_diff_format_email_options_get_summary:
 * @options: a #GgitDiffFormatEmailOptions.
 *
 * Get the summary.
 *
 * Returns: the summary.
 *
 **/
const gchar *
ggit_diff_format_email_options_get_summary (GgitDiffFormatEmailOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options), NULL);

	return options->priv->summary;
}

/**
 * ggit_diff_format_email_options_set_summary:
 * @options: a #GgitDiffFormatEmailOptions.
 * @summary: (allow-none): the summary.
 *
 * Set the summary.
 *
 **/
void
ggit_diff_format_email_options_set_summary (GgitDiffFormatEmailOptions *options,
                                            const gchar                *summary)
{
	g_return_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options));

	g_free (options->priv->summary);
	options->priv->summary = g_strdup (summary);

	options->priv->options.summary = options->priv->summary;
	g_object_notify (G_OBJECT (options), "summary");
}

/**
 * ggit_diff_format_email_options_get_author:
 * @options: a #GgitDiffFormatEmailOptions.
 *
 * Get the author.
 *
 * Returns: (transfer none): a #GgitSignature.
 *
 **/
GgitSignature *
ggit_diff_format_email_options_get_author (GgitDiffFormatEmailOptions *options)
{
	g_return_val_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options), NULL);

	return options->priv->author;
}

/**
 * ggit_diff_format_email_options_set_author:
 * @options: a #GgitDiffFormatEmailOptions.
 * @author: (allow-none): a #GgitSignature.
 *
 * Set the author.
 *
 **/
void
ggit_diff_format_email_options_set_author (GgitDiffFormatEmailOptions *options,
                                           GgitSignature              *author)
{
	g_return_if_fail (GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options));
	g_return_if_fail (author == NULL || GGIT_IS_SIGNATURE (author));

	if (options->priv->author)
	{
		g_object_unref (options->priv->author);
		options->priv->author = NULL;
		options->priv->options.author = NULL;
	}

	if (author)
	{
		options->priv->author = ggit_signature_copy (author);
		options->priv->options.author = _ggit_native_get (author);
	}

	g_object_notify (G_OBJECT (options), "author");
}

