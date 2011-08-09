/*
 * ggit-object.c
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


#include "ggit-object.h"
#include "ggit-object-private.h"
#include "ggit-oid.h"


#define GGIT_OBJECT_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_OBJECT, GgitObjectPrivate))

G_DEFINE_ABSTRACT_TYPE (GgitObject, ggit_object, G_TYPE_OBJECT)

static void
ggit_object_finalize (GObject *object)
{
	GgitObject *obj = GGIT_OBJECT (object);

	git_object_close (obj->priv->obj);

	G_OBJECT_CLASS (ggit_object_parent_class)->finalize (object);
}

static void
ggit_object_class_init (GgitObjectClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_object_finalize;

	g_type_class_add_private (object_class, sizeof (GgitObjectPrivate));
}

static void
ggit_object_init (GgitObject *object)
{
	object->priv = GGIT_OBJECT_GET_PRIVATE (object);
}

GgitOId *
ggit_object_id (GgitObject *object)
{
	const git_oid *oid;

	g_return_val_if_fail (GGIT_IS_OBJECT (object), NULL);

	oid = git_object_id (object->priv->obj);

	return _ggit_oid_new ((git_oid *)oid);
}

/**
 * ggit_object_owner:
 * @object: a #GgitObject
 *
 * Get the repository that owns this object
 *
 * Returns: (transfer full): the repository that own this object. The returned
 * value must be freed calling g_object_unref().
 */
GgitRepository *
ggit_object_owner (GgitObject *object)
{
	git_repository *repository;

	g_return_val_if_fail (GGIT_IS_OBJECT (object), NULL);

	repository = git_object_owner (object->priv->obj);

	return _ggit_repository_new (repository);
}
