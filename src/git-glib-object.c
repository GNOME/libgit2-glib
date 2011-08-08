/*
 * git-glib-object.c
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


#include "git-glib-object.h"
#include "git-glib-object-private.h"
#include "git-glib-oid.h"


#define GIT_GLIB_OBJECT_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GIT_TYPE_GLIB_OBJECT, GitGlibObjectPrivate))

G_DEFINE_ABSTRACT_TYPE (GitGlibObject, git_glib_object, G_TYPE_OBJECT)

static void
git_glib_object_finalize (GObject *object)
{
	GitGlibObject *obj = GIT_GLIB_OBJECT (object);

	git_object_close (obj->priv->obj);

	G_OBJECT_CLASS (git_glib_object_parent_class)->finalize (object);
}

static void
git_glib_object_class_init (GitGlibObjectClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = git_glib_object_finalize;

	g_type_class_add_private (object_class, sizeof (GitGlibObjectPrivate));
}

static void
git_glib_object_init (GitGlibObject *object)
{
	object->priv = GIT_GLIB_OBJECT_GET_PRIVATE (object);
}

GitGlibOId *
git_glib_object_id (GitGlibObject *object)
{
	const git_oid *oid;

	g_return_val_if_fail (GIT_IS_GLIB_OBJECT (object), NULL);

	oid = git_object_id (object->priv->obj);

	return _git_glib_oid_new ((git_oid *)oid);
}
