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

#include <git2.h>

#include "ggit-error.h"
#include "ggit-signature.h"
#include "ggit-convert.h"

/**
 * GgitSignature:
 *
 * Represents an action signature.
 */
struct _GgitSignature
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

	g_free (signature->name_utf8);
	g_free (signature->email_utf8);
	g_free (signature->encoding);

	G_OBJECT_CLASS (ggit_signature_parent_class)->finalize (object);
}

static void
ggit_signature_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
	GgitSignature *signature = GGIT_SIGNATURE (object);

	switch (prop_id)
	{
		case PROP_ENCODING:
			g_free (signature->encoding);
			signature->encoding = g_value_dup_string (value);
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
	GgitSignature *signature = GGIT_SIGNATURE (object);

	switch (prop_id)
	{
		case PROP_ENCODING:
			g_value_set_string (value, signature->encoding);
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
ggit_signature_init (GgitSignature *signature)
{
}

GgitSignature *
_ggit_signature_wrap (const git_signature *signature,
                      const gchar         *encoding,
                      gboolean             owned)
{
	GgitSignature *ret;

	ret = g_object_new (GGIT_TYPE_SIGNATURE, "encoding", encoding, NULL);

	if (owned)
	{
		_ggit_native_set (ret,
		                  (gpointer)signature,
		                  (GDestroyNotify)git_signature_free);
	}
	else
	{
		_ggit_native_set (ret, (gpointer)signature, NULL);
	}

	return ret;
}

/**
 * ggit_signature_new:
 * @name: the name of the person.
 * @email: the email of the person.
 * @signature_time: the time when the action happened.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitSignature. Name and e-mail are assumed to be in UTF-8.
 *
 * Returns: (transfer full): a newly allocated #GgitSignature.
 */
GgitSignature *
ggit_signature_new (const gchar  *name,
                    const gchar  *email,
                    GDateTime    *signature_time,
                    GError      **error)
{
	GgitSignature *signature = NULL;
	gint64 sig_time;
	gint32 sig_offset;
	git_signature *sig;
	gint ret;
	GDateTime *utc;
	GTimeSpan offset_us;

	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (signature_time != NULL, NULL);
	g_return_val_if_fail (email != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	utc = g_date_time_to_utc (signature_time);
	sig_time = g_date_time_to_unix (utc);
	g_date_time_unref (utc);

	offset_us = g_date_time_get_utc_offset (signature_time);
	sig_offset = offset_us / G_TIME_SPAN_MINUTE;

	ret = git_signature_new (&sig, name, email, sig_time, sig_offset);

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

/**
 * ggit_signature_copy:
 * @signature: a #GgitSignature.
 *
 * Create a copy of the signature.
 *
 * Returns: (transfer full): a #GgitSignature.
 *
 **/
GgitSignature *
ggit_signature_copy (GgitSignature *signature)
{
	git_signature *ret;

	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), NULL);

	git_signature_dup (&ret, _ggit_native_get (signature));
	return _ggit_signature_wrap (ret, signature->encoding, TRUE);
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

	signature->name_utf8 = ensure_utf8 (signature->name_utf8,
	                                          signature->encoding,
	                                          s->name);

	return signature->name_utf8;
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

	signature->email_utf8 = ensure_utf8 (signature->email_utf8,
	                                           signature->encoding,
	                                           s->email);

	return signature->email_utf8;
}

/**
 * ggit_signature_get_time:
 * @signature: a #GgitSignature.
 *
 * Gets the time when the action happened. Note that the time is returned in
 * the timezone of the commit (see #ggit_signature_get_time_zone).
 *
 * Returns: (transfer full): the time when the action happened.
 */
GDateTime *
ggit_signature_get_time (GgitSignature *signature)
{
	git_signature *s;
	GDateTime *utc;
	GTimeZone *tz;
	GDateTime *ret;

	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), 0);

	s = _ggit_native_get (signature);

	utc = g_date_time_new_from_unix_utc (s->when.time);
	tz = ggit_signature_get_time_zone (signature);
	ret = g_date_time_to_timezone (utc, tz);
	g_date_time_unref (utc);

	return ret;
}

/**
 * ggit_signature_get_time_zone:
 * @signature: a #GgitSignature.
 *
 * Gets the timezone in which the action happened.
 *
 * Returns: (transfer full): the timezone in which the action happened.
 */
GTimeZone *
ggit_signature_get_time_zone (GgitSignature *signature)
{
	git_signature *s;
	guint offset;
	guint minutes;
	guint hours;
	gchar *time_zone_id;
	GTimeZone *time_zone;

	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), 0);

	s = _ggit_native_get (signature);

	offset = ABS (s->when.offset);
	minutes = offset % 60;
	hours = (offset - minutes) / 60;

	time_zone_id = g_strdup_printf ("%s%02i:%02i",
	                                s->when.offset >= 0 ? "+" : "-",
	                                hours, minutes);
	time_zone = g_time_zone_new (time_zone_id);

	g_free (time_zone_id);

	return time_zone;
}

/* ex:set ts=8 noet: */
