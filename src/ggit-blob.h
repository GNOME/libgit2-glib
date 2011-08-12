/*
 * ggit-blob.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef __GGIT_BLOB_H__
#define __GGIT_BLOB_H__

#include <glib-object.h>
#include <git2/blob.h>

#include "ggit-object.h"
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_BLOB			(ggit_blob_get_type ())
#define GGIT_BLOB(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_BLOB, GgitBlob))
#define GGIT_BLOB_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_BLOB, GgitBlob const))
#define GGIT_BLOB_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_BLOB, GgitBlobClass))
#define GGIT_IS_BLOB(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_BLOB))
#define GGIT_IS_BLOB_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_BLOB))
#define GGIT_BLOB_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_BLOB, GgitBlobClass))

typedef struct _GgitBlobClass	GgitBlobClass;
typedef struct _GgitBlobPrivate	GgitBlobPrivate;

struct _GgitBlob {
	GgitObject parent;

	GgitBlobPrivate *priv;
};

struct _GgitBlobClass {
	GgitObjectClass parent_class;
};

GType             ggit_blob_get_type         (void) G_GNUC_CONST;

GgitBlob        *_ggit_blob_new              (git_blob *blob);

gconstpointer     ggit_blob_get_raw_content  (GgitBlob *blob);

gint              ggit_blob_get_raw_size     (GgitBlob *blob);

G_END_DECLS

#endif /* __GGIT_BLOB_H__ */
