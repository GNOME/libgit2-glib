/*
 * ggit-push.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_PUSH_H__
#define __GGIT_PUSH_H__

#include <libgit2-glib/ggit-native.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_PUSH			(ggit_push_get_type ())
#define GGIT_PUSH(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_PUSH, GgitPush))
#define GGIT_PUSH_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_PUSH, GgitPush const))
#define GGIT_PUSH_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_PUSH, GgitPushClass))
#define GGIT_IS_PUSH(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_PUSH))
#define GGIT_IS_PUSH_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_PUSH))
#define GGIT_PUSH_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_PUSH, GgitPushClass))

typedef struct _GgitPushClass	GgitPushClass;
typedef struct _GgitPushPrivate	GgitPushPrivate;

struct _GgitPush
{
	/*< private >*/
	GgitNative parent;

	GgitPushPrivate *priv;
};

struct _GgitPushClass
{
	/*< private >*/
	GgitNativeClass parent_class;

	/*< signals >*/
	void (*packbuilder_progress) (GgitPush              *push,
	                              GgitPackbuilderStage   stage,
	                              guint                  current,
	                              guint                  total);

	void (*transfer_progress)    (GgitPush              *push,
	                              guint                  current,
	                              guint                  total,
	                              gsize                  bytes);
};

GType             ggit_push_get_type                       (void) G_GNUC_CONST;

GgitPush         *ggit_push_new                            (GgitRemote        *remote,
                                                            GError           **error);

void              ggit_push_set_options                    (GgitPush          *push,
                                                            GgitPushOptions   *options);

GgitPushOptions  *ggit_push_get_options                    (GgitPush          *push);

void              ggit_push_add_refspec                    (GgitPush          *push,
                                                            const gchar       *refspec,
                                                            GError           **error);

gboolean          ggit_push_finish                         (GgitPush          *push,
                                                            GError           **error);

gboolean          ggit_push_is_unpack_ok                   (GgitPush          *push);

G_END_DECLS

#endif /* __GGIT_PUSH_H__ */

/* ex:set ts=8 noet: */
