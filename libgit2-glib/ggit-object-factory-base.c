/*
 * ggit-object-factory-base.c
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

#include "ggit-object-factory-base.h"
#include "ggit-object-factory.h"

/**
 * GgitObjectFactoryBase:
 *
 * Represents the base type for objects created by an object factory.
 */

G_DEFINE_ABSTRACT_TYPE (GgitObjectFactoryBase, ggit_object_factory_base, G_TYPE_OBJECT)

static GObject *
ggit_object_factory_base_constructor (GType                  type,
                                      guint                  n_construct_properties,
                                      GObjectConstructParam *construct_properties)
{
	return ggit_object_factory_construct (ggit_object_factory_get_default (),
	                                      ggit_object_factory_base_parent_class,
	                                      type,
	                                      n_construct_properties,
	                                      construct_properties);
}

static void
ggit_object_factory_base_class_init (GgitObjectFactoryBaseClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->constructor = ggit_object_factory_base_constructor;
}

static void
ggit_object_factory_base_init (GgitObjectFactoryBase *self)
{
}

/* ex:set ts=8 noet: */
