/*
 * ggit-object-factory.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
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

#ifndef __GGIT_OBJECT_FACTORY_H__
#define __GGIT_OBJECT_FACTORY_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GGIT_TYPE_OBJECT_FACTORY		(ggit_object_factory_get_type ())
#define GGIT_OBJECT_FACTORY(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_OBJECT_FACTORY, GgitObjectFactory))
#define GGIT_OBJECT_FACTORY_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_OBJECT_FACTORY, GgitObjectFactory const))
#define GGIT_OBJECT_FACTORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_OBJECT_FACTORY, GgitObjectFactoryClass))
#define GGIT_IS_OBJECT_FACTORY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_OBJECT_FACTORY))
#define GGIT_IS_OBJECT_FACTORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_OBJECT_FACTORY))
#define GGIT_OBJECT_FACTORY_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_OBJECT_FACTORY, GgitObjectFactoryClass))

typedef struct _GgitObjectFactory		GgitObjectFactory;
typedef struct _GgitObjectFactoryClass		GgitObjectFactoryClass;
typedef struct _GgitObjectFactoryPrivate	GgitObjectFactoryPrivate;

struct _GgitObjectFactory
{
	GObject parent;

	GgitObjectFactoryPrivate *priv;
};

struct _GgitObjectFactoryClass
{
	GObjectClass parent_class;
};

GType              ggit_object_factory_get_type    (void) G_GNUC_CONST;

GgitObjectFactory *ggit_object_factory_get_default (void);

void               ggit_object_factory_register    (GgitObjectFactory     *factory,
                                                    GType                  basetype,
                                                    GType                  subtype);

void               ggit_object_factory_unregister  (GgitObjectFactory     *factory,
                                                    GType                  basetype,
                                                    GType                  subtype);

GObject           *ggit_object_factory_construct   (GgitObjectFactory     *factory,
                                                    GObjectClass          *parent_class,
                                                    GType                  basetype,
                                                    guint                  n_construct_properties,
                                                    GObjectConstructParam *construct_properties);


G_END_DECLS

#endif /* __GGIT_OBJECT_FACTORY_H__ */

/* ex:set ts=8 noet: */
