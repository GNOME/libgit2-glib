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


#include "ggit-signature.h"
#include "ggit-error.h"
#include <git2/signature.h>


struct _GgitSignature
{
	git_signature *signature;
};

G_DEFINE_BOXED_TYPE (GgitSignature, ggit_signature, ggit_signature_copy, ggit_signature_free)

/**
 * ggit_signature_new:
 * @name: name of the person
 * @email: email of the person
 * @signature_time: time when the action happened
 * @signature_offset: timezone offset in minutes for the time
 * @error: #GError for error reporting, or %NULL
 *
 * Create a new action signature. The signature must be freed
 * manually using ggit_signature_free()
 *
 * Returns: (transfer full): a newly created #GgitSignature
 */
GgitSignature *
ggit_signature_new (const gchar *name,
                    const gchar *email,
                    gint64       signature_time,
                    gint         signature_offset,
                    GError     **error)
{
	GgitSignature *signature = NULL;
	git_signature *sig;
	gint ret;

	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (email != NULL, NULL);

	ret = git_signature_new (&sig, name, email, signature_time, signature_offset);

	if (ret == 0)
	{
		signature = g_slice_new (GgitSignature);
		signature->signature = sig;
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return signature;
}

/**
 * ggit_signature_now:
 * @name: name of the person
 * @email: email of the person
 * @error: #GError for error reporting, or %NULL
 *
 * Create a new action signature with a timestamp of 'now'. The
 * signature must be freed manually using ggit_signature_free()
 *
 * Returns: (transfer full): a newly created #GgitSignature
 */
GgitSignature *
ggit_signature_now (const gchar *name,
                    const gchar *email,
                    GError     **error)
{
	GgitSignature *signature = NULL;
	git_signature *sig;
	gint ret;

	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (email != NULL, NULL);

	ret = git_signature_now (&sig, name, email);

	if (ret == 0)
	{
		signature = g_slice_new (GgitSignature);
		signature->signature = sig;
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return signature;
}

/**
 * ggit_signature_copy:
 * @signature: a #GgitSignature
 *
 * Creates a copy of @signature.
 *
 * Returns: (transfer full): a newly created #GgitSignature
 */
GgitSignature *
ggit_signature_copy (GgitSignature *signature)
{
	GgitSignature *s;

	g_return_val_if_fail (signature != NULL, NULL);

	s = g_slice_new (GgitSignature);
	s->signature = git_signature_dup (signature->signature);

	return s;
}

/**
 * ggit_signature_free:
 * @signature: a #GgitSignature
 *
 * Frees @signature.
 */
void
ggit_signature_free (GgitSignature *signature)
{
	g_return_if_fail (signature != NULL);

	git_signature_free (signature->signature);
	g_slice_free (GgitSignature, signature);
}
