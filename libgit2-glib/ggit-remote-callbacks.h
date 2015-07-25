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
#include <libgit2-glib/ggit-cred.h>

G_BEGIN_DECLS

#define GGIT_TYPE_REMOTE_CALLBACKS (ggit_remote_callbacks_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitRemoteCallbacks, ggit_remote_callbacks, GGIT, REMOTE_CALLBACKS, GObject)

struct _GgitRemoteCallbacksClass
{
	GObjectClass parent_class;

	/* signals */
	void (*progress)              (GgitRemoteCallbacks        *callbacks,
	                               const gchar                *message);

	void (*transfer_progress)     (GgitRemoteCallbacks        *callbacks,
	                               GgitTransferProgress       *stats);

	void (*update_tips)           (GgitRemoteCallbacks        *callbacks,
	                               const gchar                *refname,
	                               const GgitOId              *a,
	                               const GgitOId              *b);

	void (*completion)            (GgitRemoteCallbacks        *callbacks,
	                               GgitRemoteCompletionType    type);

	/* virtual methods */
	GgitCred *(*credentials)      (GgitRemoteCallbacks        *callbacks,
	                               const gchar                *url,
	                               const gchar                *username_from_url,
	                               GgitCredtype                allowed_types,
	                               GError                    **error);


};

git_remote_callbacks *_ggit_remote_callbacks_get_native (GgitRemoteCallbacks *remote_cbs);

G_END_DECLS

#endif /* __GGIT_REMOTE_CALLBACKS_H__ */

/* ex:set ts=8 noet: */
