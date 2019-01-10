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
#include "ggit-repository.h"
#include "ggit-error.h"

#include <git2.h>


struct _GgitSubmodule
{
	git_submodule *submodule;
	gint ref_count;
	gboolean valid;
};

G_DEFINE_BOXED_TYPE (GgitSubmodule, ggit_submodule, ggit_submodule_ref, ggit_submodule_unref)

GgitSubmodule *
_ggit_submodule_wrap (const git_submodule *submodule)
{
	GgitSubmodule *glib_submodule;

	glib_submodule = g_slice_new (GgitSubmodule);
	glib_submodule->submodule = (git_submodule *)submodule;
	glib_submodule->ref_count = 1;
	glib_submodule->valid = TRUE;

	return glib_submodule;
}

void
_ggit_submodule_invalidate (GgitSubmodule *submodule)
{
	g_return_if_fail (submodule != NULL);
	submodule->valid = FALSE;
}

/**
 * ggit_submodule_ref:
 * @submodule: a #GgitSubmodule.
 *
 * Atomically increments the reference count of @submodule by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (nullable): the passed in #GgitSubmodule or %NULL.
 */
GgitSubmodule *
ggit_submodule_ref (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	g_atomic_int_inc (&submodule->ref_count);

	return submodule;
}

/**
 * ggit_submodule_unref:
 * @submodule: a #GgitSubmodule.
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
 * ggit_submodule_open:
 * @submodule: a #GgitSubmodule.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Open the repository for a submodule. Multiple calls to this function
 * will return distinct #GgitRepository objects. Only submodules which are
 * checked out in the working directory can be opened.
 *
 * Returns: (transfer full) (nullable): the opened #GgitRepository or %NULL in case of an
 *                           error.
 */
GgitRepository *
ggit_submodule_open (GgitSubmodule  *submodule,
                     GError       **error)
{
	git_repository *repo;
	gint ret;

	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_submodule_open (&repo, submodule->submodule);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_repository_wrap (repo, TRUE);
}

/**
 * ggit_submodule_get_owner:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the containing repository for a submodule.
 *
 * Returns: (transfer full) (nullable): the containing repository for a submodule or %NULL.
 */
GgitRepository *
ggit_submodule_get_owner (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	return _ggit_repository_wrap (git_submodule_owner (submodule->submodule),
	                              FALSE);
}

/**
 * ggit_submodule_get_name:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the name of the submodule from .gitmodules.
 *
 * Returns: (transfer none) (nullable): the name of the submodule from .gitmodules or %NULL.
 */
const gchar *
ggit_submodule_get_name (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	return git_submodule_name (submodule->submodule);
}

/**
 * ggit_submodule_get_path:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the path to the submodule from the repo working directory.
 * It is almost always the same as the name.
 * See ggit_submodule_get_name().
 *
 * Returns: (transfer none) (nullable): the path to the submodule from
 * the repo working directory or %NULL.
 */
const gchar *
ggit_submodule_get_path (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	return git_submodule_path (submodule->submodule);
}

/**
 * ggit_submodule_get_url:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the url for the submodule or %NULL if the submodule has been deleted
 * but not yet committed.
 *
 * Returns: (transfer none) (nullable): the url for the submodule or %NULL.
 */
const gchar *
ggit_submodule_get_url (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	return git_submodule_url (submodule->submodule);
}

/**
 * ggit_submodule_get_index_id:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the OID for the submodule in the index or %NULL if there is no index.
 *
 * Returns: (transfer full) (nullable): the OID for the submodule in the index or %NULL.
 */
GgitOId *
ggit_submodule_get_index_id (GgitSubmodule *submodule)
{
	GgitOId *oid = NULL;

	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	if (git_submodule_index_id (submodule->submodule))
	{
		oid = _ggit_oid_wrap (git_submodule_index_id (submodule->submodule));
	}

	return oid;
}

/**
 * ggit_submodule_get_head_oid:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the OID for the submodule in the current HEAD tree.
 *
 * Returns: (transfer full) (nullable): the OID for the submodule in the current HEAD tree or %NULL.
 */
GgitOId *
ggit_submodule_get_head_id (GgitSubmodule *submodule)
{
	GgitOId *oid = NULL;

	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	if (git_submodule_head_id (submodule->submodule))
	{
		oid = _ggit_oid_wrap (git_submodule_head_id (submodule->submodule));
	}

	return oid;
}

/**
 * ggit_submodule_get_workdir_id:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the OID for the submodule in the current working directory.
 * Corresponds to looking up 'HEAD' in the checked out submodule.
 * If there are pending changes in the index or anything
 * else, this won't notice that.  You should call ggit_submodule_status()
 * for a more complete picture about the state of the working directory.
 *
 * Returns: (transfer full) (nullable): the OID for the submodule in the current working directory or %NULL.
 */
GgitOId *
ggit_submodule_get_workdir_id (GgitSubmodule *submodule)
{
	GgitOId *oid = NULL;

	g_return_val_if_fail (submodule != NULL, NULL);
	g_return_val_if_fail (submodule->valid, NULL);

	if (git_submodule_wd_id (submodule->submodule))
	{
		oid = _ggit_oid_wrap (git_submodule_wd_id (submodule->submodule));
	}

	return oid;
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
	g_return_val_if_fail (submodule->valid, 0);

	return (GgitSubmoduleIgnore)git_submodule_ignore (submodule->submodule);
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
	g_return_val_if_fail (submodule->valid, 0);

	return (GgitSubmoduleUpdate)git_submodule_update_strategy (submodule->submodule);
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
	g_return_val_if_fail (submodule->valid, FALSE);

	return git_submodule_fetch_recurse_submodules (submodule->submodule);
}

/**
 * ggit_submodule_init:
 * @submodule: a #GgitSubmodule.
 * @overwrite: forces existing entries to be updated.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Just like "git submodule init", this copies information about the
 * submodule into ".git/config".  You can use the accessor functions
 * above to alter the in-memory git_submodule object and control what
 * is written to the config, overriding what is in .gitmodules.
 */
void
ggit_submodule_init (GgitSubmodule  *submodule,
                     gboolean        overwrite,
                     GError        **error)
{
	gint ret;

	g_return_if_fail (submodule != NULL);
	g_return_if_fail (submodule->valid);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_init (submodule->submodule, overwrite);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_submodule_sync:
 * @submodule: a #GgitSubmodule.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Copies the information about the submodules URL into the checked out
 * submodule config, acting like "git submodule sync".  This is useful if
 * you have altered the URL for the submodule (or it has been altered by a
 * fetch of upstream changes) and you need to update your local repo.
 */
void
ggit_submodule_sync (GgitSubmodule  *submodule,
                     GError        **error)
{
	gint ret;

	g_return_if_fail (submodule != NULL);
	g_return_if_fail (submodule->valid);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_sync (submodule->submodule);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_submodule_reload:
 * @submodule: a #GgitSubmodule.
 * @force: reload even if the data doesn't seem out of date.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Rereads submodule info from config, index, and HEAD.
 * Call this if you have reason to believe that it has changed.
 */
void
ggit_submodule_reload (GgitSubmodule  *submodule,
                       gboolean        force,
                       GError        **error)
{
	gint ret;

	g_return_if_fail (submodule != NULL);
	g_return_if_fail (submodule->valid);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_reload (submodule->submodule, force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_submodule_update:
 * @submodule: a #GgitSubmodule.
 * @init: If the submodule is not initialized, setting this flag to true
 *        will initialize the submodule before updating. Otherwise, this
 *        will return an error if attempting to update an uninitialzed
 *        repository. but setting this to true forces them to be updated.
 * @options: a #GgitSubmoduleUpdateOptions object.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Update a submodule. This will clone a missing submodule and checkout
 * the subrepository to the commit specified in the index of the containing
 * repository. If the submodule repository doesn't contain the target commit
 * (e.g. because fetchRecurseSubmodules isn't set), then the submodule is
 * fetched using the fetch options supplied in options.
 */
void
ggit_submodule_update (GgitSubmodule               *submodule,
                       gboolean                     init,
                       GgitSubmoduleUpdateOptions  *options,
                       GError                     **error)
{
	gint ret;

	g_return_if_fail (submodule != NULL);
	g_return_if_fail (submodule->valid);
	g_return_val_if_fail (options == NULL || GGIT_IS_SUBMODULE_UPDATE_OPTIONS (options), NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_update (submodule->submodule, init, options ? _ggit_submodule_update_options_get_submodule_update_options (options) : NULL);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/* ex:set ts=8 noet: */
