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
 *
 * Gets a read-only buffer with the raw contents of @blob.
 *
 * A pointer to the raw contents of @blob is returned.
 * This pointer is owned internally by @object and must
 * not be free'd. The pointer may be invalidated at a later
 * time.
 *
 * Returns: (transfer none): the contents or %NULL if @blob has no contents.
 */
gconstpointer
ggit_blob_get_raw_content (GgitBlob *blob)
{
	git_blob *b;

	g_return_val_if_fail (GGIT_IS_BLOB (blob), NULL);

	b = _ggit_native_get (blob);

	return git_blob_rawcontent (b);
}

/**
 * ggit_blob_get_raw_size:
 * @blob: a #GgitBlob.
 *
 * Gets the size in bytes of the contents of @blob.
 *
 * Returns: the size in bytes.
 */
gsize
ggit_blob_get_raw_size (GgitBlob *blob)
{
	git_blob *b;

	g_return_val_if_fail (GGIT_IS_BLOB (blob), 0);

	b = _ggit_native_get (blob);

	return git_blob_rawsize (b);
}

/* ex:set ts=8 noet: */
