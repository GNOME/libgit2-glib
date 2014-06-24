/*
 * ggit-tag.c
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
#include "ggit-oid.h"
#include "ggit-signature.h"
#include "ggit-tag.h"
#include "ggit-utils.h"

G_DEFINE_TYPE (GgitTag, ggit_tag, GGIT_TYPE_OBJECT)

static void
ggit_tag_class_init (GgitTagClass *klass)
{
}

static void
ggit_tag_init (GgitTag *self)
{
}

GgitTag *
_ggit_tag_wrap (git_tag  *tag,
                gboolean  owned)
{
	GgitTag *gtag;

	gtag = g_object_new (GGIT_TYPE_TAG,
	                     "native", tag,
	                     NULL);

	if (owned)
	{
		_ggit_native_set_destroy_func (gtag,
		                               (GDestroyNotify)git_tag_free);
	}

	return gtag;
}

/**
 * ggit_tag_get_target:
 * @tag: a #GgitTag.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the target #GgitObject of @tag.
 *
 * This method performs a repository lookup for the
 * given object and returns it.
 *
 * Returns: (transfer full): the target #GgitObject of the tag.
 */
GgitObject *
ggit_tag_get_target (GgitTag  *tag,
                     GError  **error)
{
	GgitObject *object = NULL;
	git_tag *t;
	git_object *obj;
	gint ret;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	t = _ggit_native_get (tag);
	ret = git_tag_target (&obj, t);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
	else
	{
		object = ggit_utils_create_real_object (obj, TRUE);
	}

	return object;
}

/**
 * ggit_tag_get_target_id:
 * @tag: a #GgitTag.
 *
 * Gets the target #GgitOId of @tag.
 *
 * Returns: (transfer full): the target #GgitOId of the tag.
 */
GgitOId *
ggit_tag_get_target_id (GgitTag *tag)
{
	const git_oid *oid;
	git_tag *t;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	t = _ggit_native_get (tag);
	oid = git_tag_target_id (t);

	return _ggit_oid_wrap ((git_oid *)oid);
}

/**
 * ggit_tag_get_name:
 * @tag: a #GgitTag.
 *
 * Gets the name of @tag.
 *
 * Returns: the name of the tag.
 */
const gchar *
ggit_tag_get_name (GgitTag *tag)
{
	git_tag *t;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	t = _ggit_native_get (tag);

	return git_tag_name (t);
}

/**
 * ggit_tag_get_tagger:
 * @tag: a #GgitTag.
 *
 * Get the tagger (author) of @tag. The returned value must be free with
 * g_object_unref().
 *
 * Returns: (transfer full): the tagger (author) of the tag.
 */
GgitSignature *
ggit_tag_get_tagger (GgitTag *tag)
{
	const git_signature *signature;
	git_tag *t;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	t = _ggit_native_get (tag);
	signature = git_tag_tagger (t);

	return _ggit_signature_wrap ((git_signature *)signature, NULL, FALSE);
}

/**
 * ggit_tag_get_message:
 * @tag: a #GgitTag.
 *
 * Gets the message of @tag.
 *
 * Returns: the message of the tag.
 */
const gchar *
ggit_tag_get_message (GgitTag *tag)
{
	git_tag *t;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	t = _ggit_native_get (tag);

	return git_tag_message (t);
}

/* ex:set ts=8 noet: */
