/*
 * ggit-blob-output-stream.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Jesse van den Kieboom
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


#ifndef __GGIT_BLOB_OUTPUT_STREAM_H__
#define __GGIT_BLOB_OUTPUT_STREAM_H__

#include <glib-object.h>
#include <gio/gio.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_BLOB_OUTPUT_STREAM (ggit_blob_output_stream_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitBlobOutputStream, ggit_blob_output_stream, GGIT, BLOB_OUTPUT_STREAM, GOutputStream)

/**
 * GgitBlobOutputStreamClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitBlobOutputStreamClass.
 */
struct _GgitBlobOutputStreamClass
{
	/*< private >*/
	GOutputStreamClass parent_class;
};

GgitBlobOutputStream *_ggit_blob_output_stream_new              (GgitRepository *repository);

GgitOId               *ggit_blob_output_stream_get_id           (GgitBlobOutputStream  *stream,
                                                                 GError               **error);

G_END_DECLS

#endif /* __GGIT_BLOB_OUTPUT_STREAM_H__ */

/* ex:set ts=8 noet: */
