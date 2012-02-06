/*
 * ggit-type-factory-object.c
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

#include "ggit-type-factory-object.h"
#include "ggit-type-factory.h"

G_DEFINE_ABSTRACT_TYPE (GgitTypeFactoryObject, ggit_type_factory_object, G_TYPE_OBJECT)

static GObject *
ggit_type_factory_object_constructor (GType                  type,
                                      guint                  n_construct_properties,
                                      GObjectConstructParam *construct_properties)
{
	return ggit_type_factory_construct (ggit_type_factory_get_default (),
	                                    ggit_type_factory_object_parent_class,
	                                    type,
	                                    n_construct_properties,
	                                    construct_properties);
}

static void
ggit_type_factory_object_class_init (GgitTypeFactoryObjectClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->constructor = ggit_type_factory_object_constructor;
}

static void
ggit_type_factory_object_init (GgitTypeFactoryObject *self)
{
}

/* ex:set ts=8 noet: */
