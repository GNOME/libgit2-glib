/*
 * ggit-signature.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#include <git2/errors.h>
#include <git2/signature.h>

#include "ggit-error.h"
#include "ggit-signature.h"
#include "ggit-convert.h"

struct _GgitSignature
{
	git_signature *signature;
	gchar *encoding;

	gchar *name_utf8;
	gchar *email_utf8;
};

G_DEFINE_BOXED_TYPE (GgitSignature, ggit_signature, ggit_signature_copy, ggit_signature_free)

GgitSignature *
_ggit_signature_wrap (git_signature *signature,
                      const gchar   *encoding)
{
	GgitSignature *sig;

	sig = g_slice_new (GgitSignature);
	sig->signature = signature;
	sig->encoding = g_strdup (encoding);

	return sig;
}

/**
 * ggit_signature_new:
 * @name: the name of the person.
 * @email: the email of the person.
 * @signature_time: the time when the action happened.
 * @signature_offset: the timezone offset in minutes for the time.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitSignature.
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

	if (ret == GIT_SUCCESS)
	{
		signature = _ggit_signature_wrap (sig, NULL);
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
 * Creates a new #GgitSignature with a timestamp of 'now'.
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

	if (ret == GIT_SUCCESS)
	{
		signature = _ggit_signature_wrap (sig, NULL);
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
 * Creates a copy of @signature.
 *
 * Returns: (transfer full): a newly allocated #GgitSignature.
 */
GgitSignature *
ggit_signature_copy (GgitSignature *signature)
{
	GgitSignature *s;

	g_return_val_if_fail (signature != NULL, NULL);

	s = g_slice_new (GgitSignature);

	s->signature = git_signature_dup (signature->signature);
	s->encoding = g_strdup (signature->encoding);

	return s;
}

/**
 * ggit_signature_free:
 * @signature: a #GgitSignature.
 *
 * Frees @signature.
 */
void
ggit_signature_free (GgitSignature *signature)
{
	g_return_if_fail (signature != NULL);

	git_signature_free (signature->signature);
	g_free (signature->encoding);

	g_slice_free (GgitSignature, signature);
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
	g_return_val_if_fail (signature != NULL, NULL);

	signature->name_utf8 = ensure_utf8 (signature->name_utf8,
	                                    signature->encoding,
	                                    signature->signature->name);

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
	g_return_val_if_fail (signature != NULL, NULL);

	signature->email_utf8 = ensure_utf8 (signature->email_utf8,
	                                     signature->encoding,
	                                     signature->signature->email);

	return signature->email_utf8;
}

/**
 * ggit_signature_get_time:
 * @signature: a #GgitSignature.
 *
 * Gets the time when the action happened.
 *
 * Returns: (transfer full): the time when the action happened.
 */
GDateTime *
ggit_signature_get_time (GgitSignature *signature)
{
	g_return_val_if_fail (signature != NULL, 0);

	return g_date_time_new_from_unix_utc (signature->signature->when.time);
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
	g_return_val_if_fail (signature != NULL, 0);

	return signature->signature->when.offset;
}

/* ex:set ts=8 noet: */
