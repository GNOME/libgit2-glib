/*
 * ggit-oid.c
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

#include <git2/errors.h>

#include "ggit-oid.h"

struct _GgitOId
{
	git_oid oid;
};

G_DEFINE_BOXED_TYPE (GgitOId, ggit_oid, ggit_oid_copy, ggit_oid_free)

GgitOId *
_ggit_oid_new (git_oid *oid)
{
	GgitOId *glib_oid;

	glib_oid = g_slice_new (GgitOId);
	git_oid_cpy (&glib_oid->oid, oid);

	return glib_oid;
}

const git_oid *
_ggit_oid_get_oid (GgitOId *oid)
{
	return (const git_oid *)&oid->oid;
}

/**
 * ggit_oid_copy:
 * @oid: a #GgitOId.
 *
 * Copies @oid into a newly allocated #GgitOId.
 *
 * Returns: (transfer full): a newly allocated #GgitOId.
 */
GgitOId *
ggit_oid_copy (GgitOId *oid)
{
	g_return_val_if_fail (oid != NULL, NULL);

	return _ggit_oid_new (&oid->oid);
}

/**
 * ggit_oid_free:
 * @oid: a #GgitOId.
 *
 * Frees @oid.
 */
void
ggit_oid_free (GgitOId *oid)
{
	g_return_if_fail (oid != NULL);

	g_slice_free (GgitOId, oid);
}

/**
 * ggit_oid_new_from_string:
 * @str: input hex string; must be pointing at the start of
 *       the hex sequence and have at least the number of bytes
 *       needed for an oid encoded in hex (40 bytes).
 *
 * Parses a hex formatted object id into a #GgitOId.
 *
 * Returns: (transfer full): a newly allocated #GgitOId or %NULL on error.
 */
GgitOId *
ggit_oid_new_from_string (const gchar *str)
{
	GgitOId *glib_oid = NULL;
	git_oid oid;

	g_return_val_if_fail (str != NULL, NULL);

	if (git_oid_fromstr (&oid, str) == GIT_SUCCESS)
	{
		glib_oid = _ggit_oid_new (&oid);
	}

	return glib_oid;
}

/**
 * ggit_oid_new_from_raw:
 * @raw: the raw input bytes to be copied.
 *
 * Creates a new #GgitOId from a raw oid.
 *
 * Returns: (transfer full): a newly allocated #GgitOId.
 */
GgitOId *
ggit_oid_new_from_raw (const guchar *raw)
{
	git_oid oid;

	g_return_val_if_fail (raw != NULL, NULL);

	git_oid_fromraw (&oid, raw);

	return _ggit_oid_new (&oid);
}

/**
 * ggit_oid_compare:
 * @a: first #GgitOId.
 * @b: second #GgitOId.
 *
 * Compare two #GgitOId structures.
 *
 * Returns: <0, 0, >0 if a < b, a == b, a > b.
 */
gint
ggit_oid_compare (GgitOId *a,
                  GgitOId *b)
{
	g_return_val_if_fail (a != NULL, 0);
	g_return_val_if_fail (b != NULL, 0);

	return git_oid_cmp (&a->oid, &b->oid);
}

/**
 * ggit_oid_to_string:
 * @oid: a #GgitOId.
 *
 * Converts @oid into a readable string.
 *
 * Returns: (transfer full): a newly allocated string representing @oid.
 */
gchar *
ggit_oid_to_string (GgitOId *oid)
{
	gchar *hex;

	g_return_val_if_fail (oid != NULL, NULL);

	hex = g_new (char, GIT_OID_HEXSZ + 1);

	return git_oid_to_string (hex, GIT_OID_HEXSZ + 1, &oid->oid);
}
