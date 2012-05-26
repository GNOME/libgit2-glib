/*
 * ggit-signature.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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

#include <git2/errors.h>
#include <git2/signature.h>

#include "ggit-error.h"
#include "ggit-signature.h"
#include "ggit-convert.h"

#define GGIT_SIGNATURE_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_SIGNATURE, GgitSignaturePrivate))

struct _GgitSignaturePrivate
{
	gchar *encoding;

	gchar *name_utf8;
	gchar *email_utf8;
};

G_DEFINE_TYPE (GgitSignature, ggit_signature, GGIT_TYPE_NATIVE)

enum
{
	PROP_0,
	PROP_ENCODING
};

static void
ggit_signature_finalize (GObject *object)
{
	GgitSignature *signature;

	signature = GGIT_SIGNATURE (object);

	g_free (signature->priv->name_utf8);
	g_free (signature->priv->email_utf8);
	g_free (signature->priv->encoding);

	G_OBJECT_CLASS (ggit_signature_parent_class)->finalize (object);
}

static void
ggit_signature_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
	GgitSignature *self = GGIT_SIGNATURE (object);

	switch (prop_id)
	{
		case PROP_ENCODING:
			g_free (self->priv->encoding);
			self->priv->encoding = g_value_dup_string (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_signature_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
	GgitSignature *self = GGIT_SIGNATURE (object);

	switch (prop_id)
	{
		case PROP_ENCODING:
			g_value_set_string (value, self->priv->encoding);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_signature_class_init (GgitSignatureClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_signature_finalize;

	object_class->get_property = ggit_signature_get_property;
	object_class->set_property = ggit_signature_set_property;

	g_type_class_add_private (object_class, sizeof (GgitSignaturePrivate));

	g_object_class_install_property (object_class,
	                                 PROP_ENCODING,
	                                 g_param_spec_string ("encoding",
	                                                      "Encoding",
	                                                      "Encoding",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY));
}

static void
ggit_signature_init (GgitSignature *self)
{
	self->priv = GGIT_SIGNATURE_GET_PRIVATE (self);
}

GgitSignature *
_ggit_signature_wrap (git_signature *signature,
                      const gchar   *encoding,
                      gboolean       owned)
{
	GgitSignature *ret;

	ret = g_object_new (GGIT_TYPE_SIGNATURE, "encoding", encoding, NULL);

	if (owned)
	{
		_ggit_native_set (ret,
		                  signature,
		                  (GDestroyNotify)git_signature_free);
	}
	else
	{
		_ggit_native_set (ret, signature, NULL);
	}

	return ret;
}

/**
 * ggit_signature_new:
 * @name: the name of the person.
 * @email: the email of the person.
 * @signature_time: the time when the action happened.
 * @signature_offset: the timezone offset in minutes for the time.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitSignature. Name and e-mail are assumed to be in UTF-8.
 *
 * Returns: (transfer full): a newly allocated #GgitSignature.
 */
GgitSignature *
ggit_signature_new (const gchar  *name,
                    const gchar  *email,
                    gint64        signature_time,
                    gint          signature_offset,
                    GError      **error)
{
	GgitSignature *signature = NULL;
	git_signature *sig;
	gint ret;

	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (email != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_signature_new (&sig, name, email, signature_time, signature_offset);

	if (ret == GIT_OK)
	{
		signature = _ggit_signature_wrap (sig, "UTF-8", TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return signature;
}

/**
 * ggit_signature_new_now:
 * @name: the name of the person.
 * @email: the email of the person.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitSignature with a timestamp of 'now'. Name and e-mail are
 * assumed to be in UTF-8.
 *
 * Returns: (transfer full): a newly allocated #GgitSignature.
 */
GgitSignature *
ggit_signature_new_now (const gchar  *name,
                        const gchar  *email,
                        GError      **error)
{
	GgitSignature *signature = NULL;
	git_signature *sig;
	gint ret;

	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (email != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_signature_now (&sig, name, email);

	if (ret == GIT_OK)
	{
		signature = _ggit_signature_wrap (sig, "UTF-8", TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return signature;
}

static gchar *
ensure_utf8 (gchar       *utf8,
             const gchar *encoding,
             const gchar *original)
{
	if (utf8)
	{
		return utf8;
	}

	return ggit_convert_utf8 (original, -1, encoding);
}

/**
 * ggit_signature_get_name:
 * @signature: a #GgitSignature.
 *
 * Gets the name of the person.
 *
 * Returns: the name of the person.
 */
const gchar *
ggit_signature_get_name (GgitSignature *signature)
{
	git_signature *s;

	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), NULL);

	s = _ggit_native_get (signature);

	signature->priv->name_utf8 = ensure_utf8 (signature->priv->name_utf8,
	                                          signature->priv->encoding,
	                                          s->name);

	return signature->priv->name_utf8;
}

/**
 * ggit_signature_get_email:
 * @signature: a #GgitSignature.
 *
 * Gets the email of the person.
 *
 * Returns: the email of the person.
 */
const gchar *
ggit_signature_get_email (GgitSignature *signature)
{
	git_signature *s;

	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), NULL);

	s = _ggit_native_get (signature);

	signature->priv->email_utf8 = ensure_utf8 (signature->priv->email_utf8,
	                                           signature->priv->encoding,
	                                           s->email);

	return signature->priv->email_utf8;
}

/**
 * ggit_signature_get_time:
 * @signature: a #GgitSignature.
 *
 * Gets the time in UTC when the action happened.
 *
 * Returns: (transfer full): the time in UTC when the action happened.
 */
GDateTime *
ggit_signature_get_time (GgitSignature *signature)
{
	git_signature *s;

	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), 0);

	s = _ggit_native_get (signature);

	return g_date_time_new_from_unix_utc (s->when.time);
}

/**
 * ggit_signature_get_time_offset:
 * @signature: a #GgitSignature.
 *
 * Gets the timezone offset in minutes for the time.
 *
 * Returns: the timezone offset in minutes for the time.
 */
gint
ggit_signature_get_time_offset (GgitSignature *signature)
{
	git_signature *s;

	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), 0);

	s = _ggit_native_get (signature);

	return s->when.offset;
}

/* ex:set ts=8 noet: */
