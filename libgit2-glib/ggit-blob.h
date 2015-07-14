/*
 * ggit-blob.h
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


#ifndef __GGIT_BLOB_H__
#define __GGIT_BLOB_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-object.h"
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_BLOB (ggit_blob_get_type ())
G_DECLARE_FINAL_TYPE (GgitBlob, ggit_blob, GGIT, BLOB, GgitObject)

GgitBlob        *_ggit_blob_wrap             (git_blob *blob,
                                              gboolean  owned);

const guchar     *ggit_blob_get_raw_content  (GgitBlob *blob,
                                              gsize    *length);

gboolean          ggit_blob_is_binary        (GgitBlob *blob);

G_END_DECLS

#endif /* __GGIT_BLOB_H__ */

/* ex:set ts=8 noet: */
