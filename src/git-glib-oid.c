/*
 * git-glib-oid.c
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

#include "git-glib-oid.h"

struct _GitGlibOId
{
	git_oid oid;
};

G_DEFINE_BOXED_TYPE (GitGlibOId, git_glib_oid, git_glib_oid_copy, git_glib_oid_free)

GitGlibOId *
_git_glib_oid_new (git_oid *oid)
{
	GitGlibOId *glib_oid;

	glib_oid = g_slice_new (GitGlibOId);
	git_oid_cpy (&glib_oid->oid, oid);

	return glib_oid;
}

/**
 * git_glib_oid_copy:
 * @oid: a #GitGlibOId
 *
 * Copies @oid into a newly allocated #GitGlibOId
 *
 * Returns: (transfer-full): a newly allocated #GitGlibOId.
 */
GitGlibOId *
git_glib_oid_copy (GitGlibOId *oid)
{
	if (oid == NULL)
	{
		return NULL;
	}

	return _git_glib_oid_new (&oid->oid);
}

/**
 * git_glib_oid_free:
 * @oid: a #GitGlibOId
 *
 * Frees @oid.
 */
void
git_glib_oid_free (GitGlibOId *oid)
{
	if (oid != NULL)
	{
		g_slice_free (GitGlibOId, oid);
	}
}

/**
 * git_glib_oid_fromstr:
 * @str: input hex string; must be pointing at the start of
 *       the hex sequence and have at least the number of bytes
 *       needed for an oid encoded in hex (40 bytes).
 *
 * Parse a hex formatted object id into a GitGlibOId.
 *
 * Returns: (transfer full): newly allocated #GitGlibOId or %NULL on error
 */
GitGlibOId *
git_glib_oid_fromstr (const gchar *str)
{
	GitGlibOId *glib_oid;
	git_oid oid;
	gint ret;

	g_return_val_if_fail (str != NULL, NULL);

	glib_oid = NULL;
	ret = git_oid_fromstr (&oid, str);

	if (ret == 0)
	{
		glib_oid = _git_glib_oid_new (&oid);
	}

	return glib_oid;
}

/**
 * git_oid_fromraw:
 * @raw: the raw input bytes to be copied.
 *
 * Copy an already raw oid into a git_oid structure.
 *
 * Returns: (transfer full): a newly allocated #GitGlibOId
 */
GitGlibOId *
git_glib_oid_fromraw (const guchar *raw)
{
	git_oid oid;

	g_return_val_if_fail (raw != NULL, NULL);

	git_oid_fromraw (&oid, raw);

	return _git_glib_oid_new (&oid);
}

/**
 * git_glib_oid_cmp:
 * @a: first #GitGlibOId
 * @b: second #GitGlibOId
 *
 * Compare two oid structures.
 *
 * Returns: <0, 0, >0 if a < b, a == b, a > b.
 */
gint
git_glib_oid_cmp (GitGlibOId *a,
                  GitGlibOId *b)
{
	g_return_val_if_fail (a != NULL && b != NULL, 0);

	return git_oid_cmp (&a->oid, &b->oid);
}

/**
 * git_glib_oid_to_string:
 * @oid: a #GitGlibOId
 *
 * Convert @oid to a readable string
 *
 * Returns: (transfer full): a newly allocated string for @oid.
 */
gchar *
git_glib_oid_to_string (GitGlibOId *oid)
{
	gchar hex[GIT_OID_HEXSZ];

	g_return_val_if_fail (oid != NULL, NULL);

	git_oid_fmt(hex, &oid->oid);
	return g_strndup (hex, GIT_OID_HEXSZ);
}
