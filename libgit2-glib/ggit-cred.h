/*
 * ggit-cred.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_CRED_H__
#define __GGIT_CRED_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-native.h>

G_BEGIN_DECLS

#define GGIT_TYPE_CRED			(ggit_cred_get_type ())
#define GGIT_CRED(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED, GgitCred))
#define GGIT_CRED_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED, GgitCred const))
#define GGIT_CRED_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_CRED, GgitCredClass))
#define GGIT_IS_CRED(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_CRED))
#define GGIT_IS_CRED_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_CRED))
#define GGIT_CRED_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_CRED, GgitCredClass))

typedef struct _GgitCredClass	GgitCredClass;

struct _GgitCred
{
	GgitNative parent;

	gpointer *priv;
};

struct _GgitCredClass
{
	GgitNativeClass parent_class;
};

GType               ggit_cred_get_type           (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __GGIT_CRED_H__ */

/* ex:set ts=8 noet: */
