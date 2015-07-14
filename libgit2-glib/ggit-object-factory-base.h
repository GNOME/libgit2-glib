/*
 * ggit-object-factory-base.h
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

#ifndef __GGIT_OBJECT_FACTORY_BASE_H__
#define __GGIT_OBJECT_FACTORY_BASE_H__

#include <glib-object.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_OBJECT_FACTORY_BASE (ggit_object_factory_base_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitObjectFactoryBase, ggit_object_factory_base, GGIT, OBJECT_FACTORY_BASE, GObject)

/**
 * GgitObjectFactoryBaseClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitObjectFactoryBaseClass.
 */
struct _GgitObjectFactoryBaseClass
{
	/*< private >*/
	GObjectClass parent_class;
};

G_END_DECLS

#endif /* __GGIT_OBJECT_FACTORY_BASE_H__ */

/* ex:set ts=8 noet: */
