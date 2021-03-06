/*
 * ggit-cred-plaintext.h
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


#ifndef __GGIT_CRED_PLAINTEXT_H__
#define __GGIT_CRED_PLAINTEXT_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-cred.h>

G_BEGIN_DECLS

#define GGIT_TYPE_CRED_PLAINTEXT (ggit_cred_plaintext_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitCredPlaintext, ggit_cred_plaintext, GGIT, CRED_PLAINTEXT, GgitCred)

struct _GgitCredPlaintextClass
{
	/*< private >*/
	GgitCredClass parent_class;
};

GgitCredPlaintext   *ggit_cred_plaintext_new           (const gchar  *username,
                                                        const gchar  *password,
                                                        GError      **error);

const gchar         *ggit_cred_plaintext_get_username  (GgitCredPlaintext *cred);

const gchar         *ggit_cred_plaintext_get_password  (GgitCredPlaintext *cred);

G_END_DECLS

#endif /* __GGIT_CRED_PLAINTEXT_H__ */

/* ex:set ts=8 noet: */
