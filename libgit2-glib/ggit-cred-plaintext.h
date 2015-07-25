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

#define GGIT_TYPE_CRED_PLAINTEXT		(ggit_cred_plaintext_get_type ())
#define GGIT_CRED_PLAINTEXT(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED_PLAINTEXT, GgitCredPlaintext))
#define GGIT_CRED_PLAINTEXT_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED_PLAINTEXT, GgitCredPlaintext const))
#define GGIT_CRED_PLAINTEXT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_CRED_PLAINTEXT, GgitCredPlaintextClass))
#define GGIT_IS_CRED_PLAINTEXT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_CRED_PLAINTEXT))
#define GGIT_IS_CRED_PLAINTEXT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_CRED_PLAINTEXT))
#define GGIT_CRED_PLAINTEXT_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_CRED_PLAINTEXT, GgitCredPlaintextClass))

typedef struct _GgitCredPlaintextClass		GgitCredPlaintextClass;

struct _GgitCredPlaintext
{
	/*< private >*/
	GgitCred parent;
};

struct _GgitCredPlaintextClass
{
	/*< private >*/
	GgitCredClass parent_class;
};

GType                ggit_cred_plaintext_get_type      (void) G_GNUC_CONST;

GgitCredPlaintext   *ggit_cred_plaintext_new           (const gchar  *username,
                                                        const gchar  *password,
                                                        GError      **error);

const gchar         *ggit_cred_plaintext_get_username  (GgitCredPlaintext *cred);

const gchar         *ggit_cred_plaintext_get_password  (GgitCredPlaintext *cred);

G_END_DECLS

#endif /* __GGIT_CRED_PLAINTEXT_H__ */

/* ex:set ts=8 noet: */
