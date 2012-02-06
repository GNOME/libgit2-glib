/*
 * ggit-type-factory.c
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

#include "ggit-type-factory.h"
#include "ggit-type-factory-object.h"

#define GGIT_TYPE_FACTORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_TYPE_FACTORY, GgitTypeFactoryPrivate))

typedef struct
{
	GType type;
} TypeWrap;

struct _GgitTypeFactoryPrivate
{
	GHashTable *typemap;
};

G_DEFINE_TYPE (GgitTypeFactory, ggit_type_factory, G_TYPE_OBJECT)

static GgitTypeFactory *the_instance = NULL;

static TypeWrap *
type_wrap_new (GType type)
{
	TypeWrap *ret;

	ret = g_slice_new (TypeWrap);
	ret->type = type;

	return ret;
}

static void
type_wrap_free (TypeWrap *wrap)
{
	g_slice_free (TypeWrap, wrap);
}

static void
ggit_type_factory_finalize (GObject *object)
{
	G_OBJECT_CLASS (ggit_type_factory_parent_class)->finalize (object);
}

static GObject *
ggit_type_factory_constructor (GType                  type,
                               guint                  n_construct_properties,
                               GObjectConstructParam *construct_properties)
{
	GObject *ret;

	if (the_instance)
	{
		return g_object_ref (the_instance);
	}

	ret = G_OBJECT_CLASS (ggit_type_factory_parent_class)->constructor (type,
	                                                                    n_construct_properties,
	                                                                    construct_properties);

	the_instance = GGIT_TYPE_FACTORY (ret);
	return ret;
}

static void
ggit_type_factory_class_init (GgitTypeFactoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_type_factory_finalize;
	object_class->constructor = ggit_type_factory_constructor;

	g_type_class_add_private (object_class, sizeof (GgitTypeFactoryPrivate));
}

static void
ggit_type_factory_init (GgitTypeFactory *self)
{
	self->priv = GGIT_TYPE_FACTORY_GET_PRIVATE (self);

	self->priv->typemap = g_hash_table_new_full ((GHashFunc)g_direct_hash,
	                                             (GEqualFunc)g_direct_equal,
	                                             NULL,
	                                             (GDestroyNotify)type_wrap_free);
}

/**
 * ggit_type_factory_get_default:
 *
 * Get the default type factory instance.
 *
 * Returns: (transfer none): a #GgitTypeFactory.
 *
 **/
GgitTypeFactory *
ggit_type_factory_get_default (void)
{
	if (the_instance == NULL)
	{
		the_instance = g_object_new (GGIT_TYPE_TYPE_FACTORY, NULL);

		g_object_add_weak_pointer (G_OBJECT (the_instance),
		                           (gpointer *)&the_instance);
	}

	return the_instance;
}

/**
 * ggit_type_factory_register:
 * @factory: a #GgitTypeFactory.
 * @basetype: a #GType.
 * @subtype: a #GType.
 *
 * Register @subtype as the instantiation class for @basetype. Each time an
 * object of type @basetype is going to be created, an instance of @subtype
 * is created instead. @basetype must be subclassed from #GgitTypeFactoryObject.
 *
 **/
void
ggit_type_factory_register (GgitTypeFactory *factory,
                            GType            basetype,
                            GType            subtype)
{
	g_return_if_fail (GGIT_IS_TYPE_FACTORY (factory));
	g_return_if_fail (basetype == subtype || g_type_is_a (subtype, basetype));
	g_return_if_fail (g_type_is_a (basetype, GGIT_TYPE_TYPE_FACTORY_OBJECT));

	g_hash_table_insert (factory->priv->typemap,
	                     GINT_TO_POINTER (g_type_qname (basetype)),
	                     type_wrap_new (subtype));
}

/**
 * ggit_type_factory_unregister:
 * @factory: a #GgitTypeFactory.
 * @basetype: a #GType.
 * @subtype: a #GType.
 *
 * Unregister a subtype from the factory.
 *
 **/
void
ggit_type_factory_unregister (GgitTypeFactory *factory,
                              GType            basetype,
                              GType            subtype)
{
	TypeWrap *val;

	g_return_if_fail (GGIT_IS_TYPE_FACTORY (factory));

	val = g_hash_table_lookup (factory->priv->typemap,
	                           GINT_TO_POINTER (g_type_qname (basetype)));

	if (val && val->type == subtype)
	{
		g_hash_table_remove (factory->priv->typemap,
		                     GINT_TO_POINTER (g_type_qname (basetype)));
	}
}

/**
 * ggit_type_factory_construct:
 * @factory: a #GgitTypeFactory.
 * @parent_class: a #GObjectClass.
 * @basetype: a #GType.
 * @n_construct_properties: number of construct properties.
 * @construct_properties: (array length=n_construct_properties) a list of #GObjectConstructParam.
 *
 * Construct a new object.
 *
 * Returns: (transfer full): a #GObject.
 *
 **/
GObject *
ggit_type_factory_construct (GgitTypeFactory       *factory,
                             GObjectClass          *parent_class,
                             GType                  basetype,
                             guint                  n_construct_properties,
                             GObjectConstructParam *construct_properties)
{
	TypeWrap *val;

	g_return_val_if_fail (GGIT_IS_TYPE_FACTORY (factory), NULL);

	val = g_hash_table_lookup (factory->priv->typemap,
	                           GINT_TO_POINTER (g_type_qname (basetype)));

	if (val)
	{
		basetype = val->type;
	}

	return parent_class->constructor (basetype,
	                                  n_construct_properties,
	                                  construct_properties);
}

/* ex:set ts=8 noet: */
