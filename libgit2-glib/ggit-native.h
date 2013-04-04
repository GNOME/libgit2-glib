/*
 * ggit-native.h
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

#ifndef __GGIT_NATIVE_H__
#define __GGIT_NATIVE_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-object-factory-base.h>

G_BEGIN_DECLS

#define GGIT_TYPE_NATIVE		(ggit_native_get_type ())
#define GGIT_NATIVE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_NATIVE, GgitNative))
#define GGIT_NATIVE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_NATIVE, GgitNativeClass))
#define GGIT_IS_NATIVE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_NATIVE))
#define GGIT_IS_NATIVE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_NATIVE))
#define GGIT_NATIVE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_NATIVE, GgitNativeClass))

typedef struct _GgitNativeClass		GgitNativeClass;
typedef struct _GgitNativePrivate	GgitNativePrivate;

struct _GgitNative
{
	/*< private >*/
	GgitObjectFactoryBase parent;

	GgitNativePrivate *priv;
};

/**
 * GgitNativeClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitNativeClass.
 */
struct _GgitNativeClass
{
	/*< private >*/
	GgitObjectFactoryBaseClass parent_class;
};

GType    ggit_native_get_type          (void) G_GNUC_CONST;

gpointer _ggit_native_get              (gpointer       self);

void     _ggit_native_set              (gpointer       self,
                                        gpointer       native,
                                        GDestroyNotify destroy_notify);

void     _ggit_native_set_destroy_func (gpointer       self,
                                        GDestroyNotify destroy_notify);

G_END_DECLS

#endif /* __GGIT_NATIVE_H__ */

/* ex:set ts=8 noet: */
