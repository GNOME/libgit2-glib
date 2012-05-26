/*
 * ggit-submodule.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Ignacio Casal Quinteiro
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

#include "ggit-submodule.h"
#include "ggit-oid.h"


struct _GgitSubmodule
{
	git_submodule *submodule;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitSubmodule, ggit_submodule, ggit_submodule_ref, ggit_submodule_unref)

GgitSubmodule *
_ggit_submodule_new (const git_submodule *submodule)
{
	GgitSubmodule *glib_submodule;

	glib_submodule = g_slice_new (GgitSubmodule);
	glib_submodule->submodule = (git_submodule *)submodule;
	glib_submodule->ref_count = 1;

	return glib_submodule;
}

/**
 * ggit_submodule_ref:
 * @submodule: a #GgitSubmodule.
 *
 * Atomically increments the reference count of @submodule by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): the passed in #GgitSubmodule.
 */
GgitSubmodule *
ggit_submodule_ref (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);

	g_atomic_int_inc (&submodule->ref_count);

	return submodule;
}

/**
 * ggit_submodule_unref:
 * submodule: a #GgitSubmodule.
 *
 * Atomically decrements the reference count of @submodule by one.
 * If the reference count drops to 0, @remote is freed.
 */
void
ggit_submodule_unref (GgitSubmodule *submodule)
{
	g_return_if_fail (submodule != NULL);

	if (g_atomic_int_dec_and_test (&submodule->ref_count))
	{
		g_slice_free (GgitSubmodule, submodule);
	}
}

/**
 * ggit_submodule_get_name:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the name of the submodule from .gitmodules.
 *
 * Returns: the name of the submodule from .gitmodules.
 */
const gchar *
ggit_submodule_get_name (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);

	return submodule->submodule->name;
}

/**
 * ggit_submodule_get_path:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the path to the submodule from the repo working directory.
 * It is almost always the same as the name.
 * See ggit_submodule_get_name().
 *
 * Returns: the path to the submodule from the repo working directory.
 */
const gchar *
ggit_submodule_get_path (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);

	return submodule->submodule->path;
}

/**
 * ggit_submodule_get_url:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the url for the submodule or %NULL if the submodule has been deleted
 * but not yet committed.
 *
 * Returns: the url for the submodule.
 */
const gchar *
ggit_submodule_get_url (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);

	return submodule->submodule->url;
}

/**
 * ggit_submodule_get_oid:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the HEAD SHA1 for the submodule or zero if not committed.
 *
 * Returns: (transfer full): the HEAD SHA1 for the submodule.
 */
GgitOId *
ggit_submodule_get_oid (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);

	return _ggit_oid_new (&submodule->submodule->oid);
}

/**
 * ggit_submodule_get_update:
 * @submodule: a #GgitSubmodule.
 *
 * Gets a #GgitSubmoduleUpdate. See see gitmodules(5) update.
 *
 * Returns: the #GgitSubmoduleUpdate.
 */
GgitSubmoduleUpdate
ggit_submodule_get_update (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, 0);

	return submodule->submodule->update;
}

/**
 * ggit_submodule_get_ignore:
 * @submodule: a #GgitSubmodule.
 *
 * Gets a #GgitSubmoduleIgnore. See see gitmodules(5) ignore.
 *
 * Returns: the #GgitSubmoduleIgnore.
 */
GgitSubmoduleIgnore
ggit_submodule_get_ignore (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, 0);

	return submodule->submodule->ignore;
}

/**
 * ggit_submodule_get_fetch_recurse:
 * @submodule: a #GgitSubmodule.
 *
 * Gets whether to fetch recursively. See see gitmodules(5) fetchRecurseSubmodules.
 *
 * Returns: whether or not fetch recursively.
 */
gboolean
ggit_submodule_get_fetch_recurse (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, FALSE);

	return submodule->submodule->fetch_recurse;
}

/* ex:set ts=8 noet: */
