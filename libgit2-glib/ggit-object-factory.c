/*
 * ggit-object-factory.c
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

#include "ggit-object-factory.h"
#include "ggit-object-factory-base.h"

#define GGIT_OBJECT_FACTORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_OBJECT_FACTORY, GgitObjectFactoryPrivate))

typedef struct
{
	GType type;
} TypeWrap;

struct _GgitObjectFactoryPrivate
{
	GHashTable *typemap;
};

G_DEFINE_TYPE (GgitObjectFactory, ggit_object_factory, G_TYPE_OBJECT)

static GgitObjectFactory *the_instance = NULL;

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
ggit_object_factory_finalize (GObject *object)
{
	GgitObjectFactory *factory;

	factory = GGIT_OBJECT_FACTORY (object);

	g_hash_table_destroy (factory->priv->typemap);

	G_OBJECT_CLASS (ggit_object_factory_parent_class)->finalize (object);
}

static GObject *
ggit_object_factory_constructor (GType                  type,
                                 guint                  n_construct_properties,
                                 GObjectConstructParam *construct_properties)
{
	GObject *ret;

	if (the_instance)
	{
		return g_object_ref (the_instance);
	}

	ret = G_OBJECT_CLASS (ggit_object_factory_parent_class)->constructor (type,
	                                                                    n_construct_properties,
	                                                                    construct_properties);

	the_instance = GGIT_OBJECT_FACTORY (ret);
	return ret;
}

static void
ggit_object_factory_class_init (GgitObjectFactoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_object_factory_finalize;
	object_class->constructor = ggit_object_factory_constructor;

	g_type_class_add_private (object_class, sizeof (GgitObjectFactoryPrivate));
}

static void
ggit_object_factory_init (GgitObjectFactory *self)
{
	self->priv = GGIT_OBJECT_FACTORY_GET_PRIVATE (self);

	self->priv->typemap = g_hash_table_new_full ((GHashFunc)g_direct_hash,
	                                             (GEqualFunc)g_direct_equal,
	                                             NULL,
	                                             (GDestroyNotify)type_wrap_free);
}

/**
 * ggit_object_factory_get_default:
 *
 * Get the default type factory instance.
 *
 * Returns: (transfer none): a #GgitObjectFactory.
 *
 **/
GgitObjectFactory *
ggit_object_factory_get_default (void)
{
	if (the_instance == NULL)
	{
		the_instance = g_object_new (GGIT_TYPE_OBJECT_FACTORY, NULL);

		g_object_add_weak_pointer (G_OBJECT (the_instance),
		                           (gpointer *)&the_instance);
	}

	return the_instance;
}

/**
 * ggit_object_factory_register:
 * @factory: a #GgitObjectFactory.
 * @basetype: a #GType.
 * @subtype: a #GType.
 *
 * Register @subtype as the instantiation class for @basetype. Each time an
 * object of type @basetype is going to be created, an instance of @subtype
 * is created instead. @basetype must be subclassed from #GgitObjectFactoryBase.
 *
 **/
void
ggit_object_factory_register (GgitObjectFactory *factory,
                              GType              basetype,
                              GType              subtype)
{
	g_return_if_fail (GGIT_IS_OBJECT_FACTORY (factory));
	g_return_if_fail (basetype == subtype || g_type_is_a (subtype, basetype));
	g_return_if_fail (g_type_is_a (basetype, GGIT_TYPE_OBJECT_FACTORY_BASE));

	g_hash_table_insert (factory->priv->typemap,
	                     GINT_TO_POINTER (g_type_qname (basetype)),
	                     type_wrap_new (subtype));
}

/**
 * ggit_object_factory_unregister:
 * @factory: a #GgitObjectFactory.
 * @basetype: a #GType.
 * @subtype: a #GType.
 *
 * Unregister a subtype from the factory.
 *
 **/
void
ggit_object_factory_unregister (GgitObjectFactory *factory,
                                GType              basetype,
                                GType              subtype)
{
	TypeWrap *val;

	g_return_if_fail (GGIT_IS_OBJECT_FACTORY (factory));

	val = g_hash_table_lookup (factory->priv->typemap,
	                           GINT_TO_POINTER (g_type_qname (basetype)));

	if (val && val->type == subtype)
	{
		g_hash_table_remove (factory->priv->typemap,
		                     GINT_TO_POINTER (g_type_qname (basetype)));
	}
}

static GParameter *
convert_to_gparameter (GObjectConstructParam *params,
                       guint                  num)
{
	GParameter *ret;
	guint i;

	ret = g_new0 (GParameter, num);

	for (i = 0; i < num; ++i)
	{
		ret[i].name = params[i].pspec->name;

		g_value_init (&ret[i].value, G_VALUE_TYPE (params[i].value));
		g_value_copy (params[i].value, &ret[i].value);
	}

	return ret;
}

/**
 * ggit_object_factory_construct:
 * @factory: a #GgitObjectFactory.
 * @parent_class: a #GObjectClass.
 * @basetype: a #GType.
 * @n_construct_properties: number of construct properties.
 * @construct_properties: (array length=n_construct_properties): a list of #GObjectConstructParam.
 *
 * Construct a new object.
 *
 * Returns: (transfer full): a #GObject.
 *
 **/
GObject *
ggit_object_factory_construct (GgitObjectFactory     *factory,
                               GObjectClass          *parent_class,
                               GType                  basetype,
                               guint                  n_construct_properties,
                               GObjectConstructParam *construct_properties)
{
	TypeWrap *val;
	GObject *ret;

	g_return_val_if_fail (GGIT_IS_OBJECT_FACTORY (factory), NULL);

	val = g_hash_table_lookup (factory->priv->typemap,
	                           GINT_TO_POINTER (g_type_qname (basetype)));

	if (val)
	{
		/* convert construct properties to gparameter and call newv */
		GParameter *params;
		guint i;

		params = convert_to_gparameter (construct_properties,
		                                n_construct_properties);

		ret = g_object_newv (val->type, n_construct_properties, params);

		for (i = 0; i < n_construct_properties; ++i)
		{
			g_value_unset (&params[i].value);
		}

		g_free (params);
	}
	else
	{
		ret = parent_class->constructor (basetype,
		                                 n_construct_properties,
		                                 construct_properties);
	}

	return ret;
}

/* ex:set ts=8 noet: */
