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

/**
 * ggit_oid_copy:
 * @oid: a #GgitOId
 *
 * Copies @oid into a newly allocated #GgitOId
 *
 * Returns: (transfer full): a newly allocated #GgitOId.
 */
GgitOId *
ggit_oid_copy (GgitOId *oid)
{
	if (oid == NULL)
	{
		return NULL;
	}

	return _ggit_oid_new (&oid->oid);
}

/**
 * ggit_oid_free:
 * @oid: a #GgitOId
 *
 * Frees @oid.
 */
void
ggit_oid_free (GgitOId *oid)
{
	if (oid != NULL)
	{
		g_slice_free (GgitOId, oid);
	}
}

/**
 * ggit_oid_fromstr:
 * @str: input hex string; must be pointing at the start of
 *       the hex sequence and have at least the number of bytes
 *       needed for an oid encoded in hex (40 bytes).
 *
 * Parse a hex formatted object id into a GgitOId.
 *
 * Returns: (transfer full): newly allocated #GgitOId or %NULL on error
 */
GgitOId *
ggit_oid_fromstr (const gchar *str)
{
	GgitOId *glib_oid;
	git_oid oid;
	gint ret;

	g_return_val_if_fail (str != NULL, NULL);

	glib_oid = NULL;
	ret = git_oid_fromstr (&oid, str);

	if (ret == 0)
	{
		glib_oid = _ggit_oid_new (&oid);
	}

	return glib_oid;
}

/**
 * git_oid_fromraw:
 * @raw: the raw input bytes to be copied.
 *
 * Copy an already raw oid into a git_oid structure.
 *
 * Returns: (transfer full): a newly allocated #GgitOId
 */
GgitOId *
ggit_oid_fromraw (const guchar *raw)
{
	git_oid oid;

	g_return_val_if_fail (raw != NULL, NULL);

	git_oid_fromraw (&oid, raw);

	return _ggit_oid_new (&oid);
}

/**
 * ggit_oid_cmp:
 * @a: first #GgitOId
 * @b: second #GgitOId
 *
 * Compare two oid structures.
 *
 * Returns: <0, 0, >0 if a < b, a == b, a > b.
 */
gint
ggit_oid_cmp (GgitOId *a,
              GgitOId *b)
{
	g_return_val_if_fail (a != NULL && b != NULL, 0);

	return git_oid_cmp (&a->oid, &b->oid);
}

/**
 * ggit_oid_to_string:
 * @oid: a #GgitOId
 *
 * Convert @oid to a readable string
 *
 * Returns: (transfer full): a newly allocated string for @oid.
 */
gchar *
ggit_oid_to_string (GgitOId *oid)
{
	gchar hex[GIT_OID_HEXSZ];

	g_return_val_if_fail (oid != NULL, NULL);

	git_oid_fmt(hex, &oid->oid);
	return g_strndup (hex, GIT_OID_HEXSZ);
}
