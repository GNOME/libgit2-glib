/*
 * ggit-cred.h
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


#ifndef __GGIT_CRED_H__
#define __GGIT_CRED_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-native.h>

G_BEGIN_DECLS

#define GGIT_TYPE_CRED (ggit_cred_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitCred, ggit_cred, GGIT, CRED, GgitNative)

struct _GgitCredClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

/**
 * GgitCredAcquireCallback:
 * @url: the resource for which we are demanding a credential.
 * @username_from_url: (allow-none): The username that was embedded in a "user@host"
 *                                   remote url, or NULL if not included.
 * @allowed_types: a bitmask stating which cred types are OK to return.
 * @cred: (transfer full): newly created credential object.
 * @user_data: (closure): user-supplied data.
 *
 * Signature of a function which acquires a credential object.
 */
typedef gint (* GgitCredAcquireCallback) (const gchar *url,
                                          const gchar *username_from_url,
                                          guint        allowed_types,
                                          GgitCred   **cred,
                                          gpointer     user_data);

G_END_DECLS

#endif /* __GGIT_CRED_H__ */

/* ex:set ts=8 noet: */
