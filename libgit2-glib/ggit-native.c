/*
 * ggit-native.c
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

#include "ggit-native.h"

/**
 * GgitNative:
 *
 * Represents a generic native object.
 */

typedef struct _GgitNativePrivate
{
	gpointer native;
	GDestroyNotify destroy_notify;
} GgitNativePrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (GgitNative, ggit_native, GGIT_TYPE_OBJECT_FACTORY_BASE)

enum
{
	PROP_0,
	PROP_NATIVE
};

static void
ggit_native_finalize (GObject *object)
{
	GgitNative *native;
	GgitNativePrivate *priv;

	native = GGIT_NATIVE (object);
	priv = ggit_native_get_instance_private (native);

	if (priv->native && priv->destroy_notify)
	{
		priv->destroy_notify (priv->native);
	}

	G_OBJECT_CLASS (ggit_native_parent_class)->finalize (object);
}

static void
ggit_native_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
	GgitNative *native = GGIT_NATIVE (object);
	GgitNativePrivate *priv;

	priv = ggit_native_get_instance_private (native);

	switch (prop_id)
	{
		case PROP_NATIVE:
			priv->native = g_value_get_pointer (value);
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
ggit_native_init (GgitNative *native)
{
}

gpointer
_ggit_native_get (gpointer self)
{
	GgitNativePrivate *priv;

	g_return_val_if_fail (GGIT_IS_NATIVE (self), NULL);

	priv = ggit_native_get_instance_private (GGIT_NATIVE (self));

	return priv->native;
}

void
_ggit_native_set (gpointer self,
                  gpointer native,
                  GDestroyNotify destroy_notify)
{
	GgitNativePrivate *priv;

	g_return_if_fail (GGIT_IS_NATIVE (self));

	priv = ggit_native_get_instance_private (GGIT_NATIVE (self));

	if (priv->native && priv->destroy_notify)
	{
		priv->destroy_notify (priv->native);
	}

	priv->native = native;
	priv->destroy_notify = destroy_notify;
}

gpointer
_ggit_native_release (gpointer self)
{
	GgitNativePrivate *priv;
	gpointer ret;

	g_return_val_if_fail (GGIT_IS_NATIVE (self), NULL);

	priv = ggit_native_get_instance_private (GGIT_NATIVE (self));

	ret = priv->native;
	priv->native = NULL;
	priv->destroy_notify = NULL;

	return ret;
}

void
_ggit_native_set_destroy_func (gpointer       self,
                               GDestroyNotify destroy_notify)
{
	GgitNativePrivate *priv;

	g_return_if_fail (GGIT_IS_NATIVE (self));

	priv = ggit_native_get_instance_private (GGIT_NATIVE (self));

	priv->destroy_notify = destroy_notify;
}

/* ex:set ts=8 noet: */
