/*
 * git-glib-object.h
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


#ifndef __GIT_GLIB_OBJECT_H__
#define __GIT_GLIB_OBJECT_H__

#include <glib-object.h>
#include "git-glib-oid.h"

G_BEGIN_DECLS

#define GIT_TYPE_GLIB_OBJECT		(git_glib_object_get_type ())
#define GIT_GLIB_OBJECT(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GIT_TYPE_GLIB_OBJECT, GitGlibObject))
#define GIT_GLIB_OBJECT_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GIT_TYPE_GLIB_OBJECT, GitGlibObject const))
#define GIT_GLIB_OBJECT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GIT_TYPE_GLIB_OBJECT, GitGlibObjectClass))
#define GIT_IS_GLIB_OBJECT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIT_TYPE_GLIB_OBJECT))
#define GIT_IS_GLIB_OBJECT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GIT_TYPE_GLIB_OBJECT))
#define GIT_GLIB_OBJECT_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GIT_TYPE_GLIB_OBJECT, GitGlibObjectClass))

typedef struct _GitGlibObject		GitGlibObject;
typedef struct _GitGlibObjectClass	GitGlibObjectClass;
typedef struct _GitGlibObjectPrivate	GitGlibObjectPrivate;

struct _GitGlibObject
{
	GObject parent;

	GitGlibObjectPrivate *priv;
};

struct _GitGlibObjectClass
{
	GObjectClass parent_class;
};

GType              git_glib_object_get_type          (void) G_GNUC_CONST;

GitGlibOId        *git_glib_object_id                (GitGlibObject *object);

G_END_DECLS

#endif /* __GIT_GLIB_OBJECT_H__ */
