/*
 * ggit-native.c
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

#include "ggit-native.h"

#define GGIT_NATIVE_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_NATIVE, GgitNativePrivate))

struct _GgitNativePrivate
{
	gpointer native;
	GDestroyNotify destroy_notify;
};

G_DEFINE_ABSTRACT_TYPE (GgitNative, ggit_native, GGIT_TYPE_TYPE_FACTORY_OBJECT)

enum
{
	PROP_0,
	PROP_NATIVE
};

static void
ggit_native_finalize (GObject *object)
{
	GgitNative *native;

	native = GGIT_NATIVE (object);

	if (native->priv->native && native->priv->destroy_notify)
	{
		native->priv->destroy_notify (native->priv->native);
	}

	G_OBJECT_CLASS (ggit_native_parent_class)->finalize (object);
}

static void
ggit_native_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
	GgitNative *self = GGIT_NATIVE (object);

	switch (prop_id)
	{
		case PROP_NATIVE:
			self->priv->native = g_value_get_pointer (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_native_class_init (GgitNativeClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_native_finalize;

	object_class->set_property = ggit_native_set_property;

	g_type_class_add_private (object_class, sizeof (GgitNativePrivate));

	g_object_class_install_property (object_class,
	                                 PROP_NATIVE,
	                                 g_param_spec_pointer ("native",
	                                                       "Native",
	                                                       "Native",
	                                                       G_PARAM_WRITABLE |
	                                                       G_PARAM_CONSTRUCT_ONLY |
	                                                       G_PARAM_STATIC_STRINGS));
}

static void
ggit_native_init (GgitNative *self)
{
	self->priv = GGIT_NATIVE_GET_PRIVATE (self);
}

gpointer
_ggit_native_get (gpointer self)
{
	g_return_val_if_fail (GGIT_IS_NATIVE (self), NULL);

	return GGIT_NATIVE (self)->priv->native;
}

void
_ggit_native_set (gpointer self,
                  gpointer native,
                  GDestroyNotify destroy_notify)
{
	GgitNativePrivate *priv;

	g_return_if_fail (GGIT_IS_NATIVE (self));

	priv = GGIT_NATIVE (self)->priv;

	if (priv->native && priv->destroy_notify)
	{
		priv->destroy_notify (priv->native);
	}

	priv->native = native;
	priv->destroy_notify = destroy_notify;
}

void
_ggit_native_set_destroy_func (gpointer       self,
                               GDestroyNotify destroy_notify)
{
	g_return_if_fail (GGIT_IS_NATIVE (self));

	GGIT_NATIVE (self)->priv->destroy_notify = destroy_notify;
}

/* ex:set ts=8 noet: */
