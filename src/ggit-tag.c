/*
 * ggit-tag.c
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


#include "ggit-tag.h"
#include "ggit-object-private.h"
#include "ggit-error.h"
#include "ggit-oid.h"


G_DEFINE_TYPE (GgitTag, ggit_tag, GGIT_TYPE_OBJECT)

static void
ggit_tag_finalize (GObject *object)
{
	G_OBJECT_CLASS (ggit_tag_parent_class)->finalize (object);
}

static void
ggit_tag_class_init (GgitTagClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_tag_finalize;
}

static void
ggit_tag_init (GgitTag *self)
{
}

GgitTag *
_ggit_tag_new (git_tag *tag)
{
	GgitTag *gtag;

	gtag = g_object_new (GGIT_TYPE_TAG, NULL);
	GGIT_OBJECT (gtag)->priv->obj = (git_object *)tag;

	return gtag;
}

/**
 * ggit_tag_target:
 * @tag: a #GgitTag
 * @error: #GError for error reporting, or %NULL
 *
 * Get the tagged object of a tag
 *
 * This method performs a repository lookup for the
 * given object and returns it
 *
 * Returns: (transfer full): the target object
 */
GgitObject *
ggit_tag_target (GgitTag *tag,
                 GError **error)
{
	GgitObject *object;
	git_tag *t;
	git_object *obj;
	gint ret;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	object = NULL;
	t = (git_tag *)GGIT_OBJECT (tag)->priv->obj;
	ret = git_tag_target (&obj, t);

	if (ret != 0)
	{
		_ggit_error_set (error, ret);
	}
	else
	{
		object = _ggit_object_new (obj);
	}

	return object;
}

/**
 * ggit_tag_target_oid:
 * @tag: a #GgitTag
 *
 * Get the OID of the tagged object of a tag
 *
 * Returns: (transfer full): the OID of the tagged object of a tag
 */
GgitOId *
ggit_tag_target_oid (GgitTag *tag)
{
	const git_oid *oid;
	git_tag *t;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	t = (git_tag *)GGIT_OBJECT (tag)->priv->obj;
	oid = git_tag_target_oid (t);

	return _ggit_oid_new ((git_oid *)oid);
}

/**
 * ggit_tag_name:
 * @tag: a #GgitTag
 *
 * Get the name of @tag
 *
 * Returns: the name of @tag
 */
const gchar *
ggit_tag_name (GgitTag *tag)
{
	git_tag *t;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	t = (git_tag *)GGIT_OBJECT (tag)->priv->obj;

	return git_tag_name (t);
}

/**
 * ggit_tag_message:
 * @tag: a #GgitTag
 *
 * Get the message of @tag
 *
 * Returns: the message of @tag
 */
const gchar *
ggit_tag_message (GgitTag *tag)
{
	git_tag *t;

	g_return_val_if_fail (GGIT_IS_TAG (tag), NULL);

	t = (git_tag *)GGIT_OBJECT (tag)->priv->obj;

	return git_tag_message (t);
}
