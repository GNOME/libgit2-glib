/*
 * ggit-remote-callbacks.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_REMOTE_CALLBACKS_H__
#define __GGIT_REMOTE_CALLBACKS_H__

#include <glib-object.h>
#include <git2.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_REMOTE_CALLBACKS		(ggit_remote_callbacks_get_type ())
#define GGIT_REMOTE_CALLBACKS(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REMOTE_CALLBACKS, GgitRemoteCallbacks))
#define GGIT_REMOTE_CALLBACKS_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REMOTE_CALLBACKS, GgitRemoteCallbacks const))
#define GGIT_REMOTE_CALLBACKS_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_REMOTE_CALLBACKS, GgitRemoteCallbacksClass))
#define GGIT_IS_REMOTE_CALLBACKS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_REMOTE_CALLBACKS))
#define GGIT_IS_REMOTE_CALLBACKS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_REMOTE_CALLBACKS))
#define GGIT_REMOTE_CALLBACKS_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_REMOTE_CALLBACKS, GgitRemoteCallbacksClass))

typedef struct _GgitRemoteCallbacksClass	GgitRemoteCallbacksClass;
typedef struct _GgitRemoteCallbacksPrivate	GgitRemoteCallbacksPrivate;

struct _GgitRemoteCallbacks
{
	GObject parent;

	GgitRemoteCallbacksPrivate *priv;
};

struct _GgitRemoteCallbacksClass
{
	GObjectClass parent_class;

	/* virtual methods */
	gboolean (*progress)          (GgitRemoteCallbacks        *callbacks,
	                               const gchar                *str,
	                               gint                        len,
	                               GError                    **error);

	gboolean (*completion)        (GgitRemoteCallbacks        *callbacks,
	                               GgitRemoteCompletionType    type,
	                               GError                    **error);

	gboolean (*credentials)       (GgitRemoteCallbacks        *callbacks,
	                               const gchar                *url,
	                               const gchar                *username_from_url,
	                               GgitCredtype                allowed_types,
	                               GgitCred                  **cred,
	                               GError                    **error);

	gboolean (*transfer_progress) (GgitRemoteCallbacks        *callbacks,
	                               GgitTransferProgress       *stats,
	                               GError                    **error);

	gboolean (*update_tips)       (GgitRemoteCallbacks        *callbacks,
	                               const gchar                *refname,
	                               const GgitOId              *a,
	                               const GgitOId              *b,
	                               GError                    **error);
};

GType                  ggit_remote_callbacks_get_type   (void) G_GNUC_CONST;

git_remote_callbacks *_ggit_remote_callbacks_get_native (GgitRemoteCallbacks *remote_cbs);

G_END_DECLS

#endif /* __GGIT_REMOTE_CALLBACKS_H__ */

/* ex:set ts=8 noet: */
