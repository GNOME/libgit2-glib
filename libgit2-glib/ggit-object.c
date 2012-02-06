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
#include "ggit-oid.h"
#include "ggit-repository.h"

G_DEFINE_ABSTRACT_TYPE (GgitObject, ggit_object, GGIT_TYPE_NATIVE)

static void
ggit_object_class_init (GgitObjectClass *klass)
{
}

static void
ggit_object_init (GgitObject *object)
{
}

/**
 * ggit_object_get_id:
 * @object: a #GgitObject.
 *
 * Gets the #GgitOId of @object.
 *
 * Returns: (transfer full): the #GgitOId of the object.
 */
GgitOId *
ggit_object_get_id (GgitObject *object)
{
	const git_oid *oid;

	g_return_val_if_fail (GGIT_IS_OBJECT (object), NULL);

	oid = git_object_id (_ggit_native_get (object));

	return _ggit_oid_new ((git_oid *)oid);
}

/**
 * ggit_object_get_owner:
 * @object: a #GgitObject.
 *
 * Gets the #GgitRepository that owns @object.
 *
 * Returns: (transfer full): the #GgitRepository that owns this object.
 * The returned value must be free'd calling g_object_unref().
 */
GgitRepository *
ggit_object_get_owner (GgitObject *object)
{
	git_repository *repository;

	g_return_val_if_fail (GGIT_IS_OBJECT (object), NULL);

	repository = git_object_owner (_ggit_native_get (object));

	return _ggit_repository_wrap (repository, FALSE);
}

/* ex:set ts=8 noet: */
