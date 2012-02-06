/*
 * ggit-type-factory-object.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
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
 * along with libgit2-glib. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GGIT_TYPE_FACTORY_OBJECT_H__
#define __GGIT_TYPE_FACTORY_OBJECT_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GGIT_TYPE_TYPE_FACTORY_OBJECT			(ggit_type_factory_object_get_type ())
#define GGIT_TYPE_FACTORY_OBJECT(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_TYPE_FACTORY_OBJECT, GgitTypeFactoryObject))
#define GGIT_TYPE_FACTORY_OBJECT_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_TYPE_FACTORY_OBJECT, GgitTypeFactoryObject const))
#define GGIT_TYPE_FACTORY_OBJECT_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_TYPE_FACTORY_OBJECT, GgitTypeFactoryObjectClass))
#define GGIT_IS_TYPE_FACTORY_OBJECT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_TYPE_FACTORY_OBJECT))
#define GGIT_IS_TYPE_FACTORY_OBJECT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_TYPE_FACTORY_OBJECT))
#define GGIT_TYPE_FACTORY_OBJECT_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_TYPE_FACTORY_OBJECT, GgitTypeFactoryObjectClass))

typedef struct _GgitTypeFactoryObject		GgitTypeFactoryObject;
typedef struct _GgitTypeFactoryObjectClass	GgitTypeFactoryObjectClass;

struct _GgitTypeFactoryObject
{
	GObject parent;
};

struct _GgitTypeFactoryObjectClass
{
	GObjectClass parent_class;
};

GType ggit_type_factory_object_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __GGIT_TYPE_FACTORY_OBJECT_H__ */

/* ex:set ts=8 noet: */
