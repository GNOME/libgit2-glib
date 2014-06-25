/*
 * ggit-blob.c
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


#include "ggit-blob.h"

G_DEFINE_TYPE (GgitBlob, ggit_blob, GGIT_TYPE_OBJECT)

static void
ggit_blob_class_init (GgitBlobClass *klass)
{
}

static void
ggit_blob_init (GgitBlob *self)
{
}

GgitBlob *
_ggit_blob_wrap (git_blob *blob,
                 gboolean  owned)
{
	GgitBlob *gblob;

	gblob = g_object_new (GGIT_TYPE_BLOB,
	                      "native", blob,
	                      NULL);

	if (owned)
	{
		_ggit_native_set_destroy_func (gblob,
		                               (GDestroyNotify)git_object_free);
	}

	return gblob;
}

/**
 * ggit_blob_get_raw_content:
 * @blob: a #GgitBlob.
 * @length: (out) (allow-none): return value of the length of the data.
 *
 * Gets a read-only buffer with the raw contents of @blob.
 *
 * A pointer to the raw contents of @blob is returned.
 * This pointer is owned internally by @object and must
 * not be free'd. The pointer may be invalidated at a later
 * time.
 *
 * Returns: (array length=length) (element-type guchar): the blob content or
 *          %NULL if the blob does not have any content.
 *
 **/
const guchar *
ggit_blob_get_raw_content (GgitBlob *blob,
                           gsize    *length)
{
	git_blob *b;

	g_return_val_if_fail (GGIT_IS_BLOB (blob), NULL);

	b = _ggit_native_get (blob);

	if (length != NULL)
	{
		*length = git_blob_rawsize (b);
	}

	return (const guchar *)git_blob_rawcontent (b);
}

/**
 * ggit_blob_is_binary:
 * @blob: a #GgitBlob.
 *
 * Check whether the blob is binary.
 *
 * Returns: %TRUE if the blob is binary, %FALSE otherwise.
 *
 **/
gboolean
ggit_blob_is_binary (GgitBlob *blob)
{
	g_return_val_if_fail (GGIT_IS_BLOB (blob), FALSE);

	return git_blob_is_binary (_ggit_native_get (blob));
}

/* ex:set ts=8 noet: */
