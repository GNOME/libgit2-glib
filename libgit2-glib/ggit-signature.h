/*
 * ggit-signature.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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

#ifndef __GGIT_SIGNATURE_H__
#define __GGIT_SIGNATURE_H__

#include <libgit2-glib/ggit-native.h>
#include <git2/signature.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_SIGNATURE		(ggit_signature_get_type ())
#define GGIT_SIGNATURE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_SIGNATURE, GgitSignature))
#define GGIT_SIGNATURE_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_SIGNATURE, GgitSignature const))
#define GGIT_SIGNATURE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_SIGNATURE, GgitSignatureClass))
#define GGIT_IS_SIGNATURE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_SIGNATURE))
#define GGIT_IS_SIGNATURE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_SIGNATURE))
#define GGIT_SIGNATURE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_SIGNATURE, GgitSignatureClass))

typedef struct _GgitSignatureClass	GgitSignatureClass;
typedef struct _GgitSignaturePrivate	GgitSignaturePrivate;

struct _GgitSignature
{
	GgitNative parent;

	/* priv padding */
	GgitSignaturePrivate *priv;
};

struct _GgitSignatureClass
{
	GgitNativeClass parent_class;
};

GType                 ggit_signature_get_type          (void) G_GNUC_CONST;

GgitSignature        *_ggit_signature_wrap             (git_signature *signature,
                                                        const gchar   *encoding,
                                                        gboolean       owned);

GgitSignature        *ggit_signature_new               (const gchar    *name,
                                                        const gchar    *email,
                                                        GDateTime      *signature_time,
                                                        GTimeZone      *signature_time_zone,
                                                        GError        **error);

GgitSignature        *ggit_signature_new_now           (const gchar    *name,
                                                        const gchar    *email,
                                                        GError        **error);

const gchar          *ggit_signature_get_name          (GgitSignature  *signature);

const gchar          *ggit_signature_get_email         (GgitSignature  *signature);

GDateTime            *ggit_signature_get_time          (GgitSignature  *signature);

GTimeZone            *ggit_signature_get_time_zone     (GgitSignature  *signature);

G_END_DECLS

#endif /* __GGIT_SIGNATURE_H__ */

/* ex:set ts=8 noet: */
