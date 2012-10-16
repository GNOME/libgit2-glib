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

#include <git2/errors.h>


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
 * ggit_submodule_save:
 * @submodule: a #GgitSubmodule.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Writes submodule settings to .gitmodules file.
 * Commits any in-memory changes to the submodule to the gitmodules
 * file on disk.  You may also be interested in ggit_submodule_init() which
 * writes submodule info to ".git/config" (which is better for local changes
 * to submodule settings) and/or ggit_submodule_sync() which writes
 * settings about remotes to the actual submodule repository.
 */
void
ggit_submodule_save (GgitSubmodule  *submodule,
                     GError        **error)
{
	gint ret;

	g_return_if_fail (submodule != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_save (submodule->submodule);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_submodule_get_owner:
 * @submodule: a #GgitSubmodule
 *
 * Gets the containing repository for a submodule.
 *
 * Returns: (transfer full): the containing repository for a submodule.
 */
GgitRepository *
ggit_submodule_get_owner (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);

	return _ggit_repository_wrap (git_submodule_owner (submodule->submodule),
	                              FALSE);
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
 * Returns: the path to the submodule from the repo working directory.
 */
const gchar *
ggit_submodule_get_path (GgitSubmodule *submodule)
{
	g_return_val_if_fail (submodule != NULL, NULL);

	return git_submodule_path (submodule->submodule);
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

	return git_submodule_url (submodule->submodule);
}

/**
 * ggit_submodule_set_url:
 * @submodule: a #GgitSubmodule.
 * @url: URL that should be used for the submodule.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets the URL for the submodule.
 *
 * This sets the URL in memory for the submodule. This will be used for
 * any following submodule actions while this submodule data is in memory.
 *
 * After calling this, you may wish to call ggit_submodule_save() to write
 * the changes back to the ".gitmodules" file and ggit_submodule_sync() to
 * write the changes to the checked out submodule repository.
 */
void
ggit_submodule_set_url (GgitSubmodule       *submodule,
                        const gchar         *url,
                        GError             **error)
{
	gint ret;

	g_return_if_fail (submodule != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_set_url (submodule->submodule,
	                             url);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_submodule_get_index_oid:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the OID for the submodule in the index or %NULL if there is no index.
 *
 * Returns: (transfer full) (allow-none): the OID for the submodule in the index or %NULL.
 */
GgitOId *
ggit_submodule_get_index_oid (GgitSubmodule *submodule)
{
	GgitOId *oid = NULL;

	g_return_val_if_fail (submodule != NULL, NULL);

	if (git_submodule_index_oid (submodule->submodule))
	{
		oid = _ggit_oid_new (git_submodule_index_oid (submodule->submodule));
	}

	return oid;
}

/**
 * ggit_submodule_get_head_oid:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the OID for the submodule in the current HEAD tree.
 *
 * Returns: (transfer full) (allow-none): the OID for the submodule in the current HEAD tree or %NULL.
 */
GgitOId *
ggit_submodule_get_head_oid (GgitSubmodule *submodule)
{
	GgitOId *oid = NULL;

	g_return_val_if_fail (submodule != NULL, NULL);

	if (git_submodule_head_oid (submodule->submodule))
	{
		oid = _ggit_oid_new (git_submodule_head_oid (submodule->submodule));
	}

	return oid;
}

/**
 * ggit_submodule_get_workdir_oid:
 * @submodule: a #GgitSubmodule.
 *
 * Gets the OID for the submodule in the current working directory.
 * Corresponds to looking up 'HEAD' in the checked out submodule.
 * If there are pending changes in the index or anything
 * else, this won't notice that.  You should call ggit_submodule_status()
 * for a more complete picture about the state of the working directory.
 *
 * Returns: (transfer full) (allow-none): the OID for the submodule in the current working directory or %NULL.
 */
GgitOId *
ggit_submodule_get_workdir_oid (GgitSubmodule *submodule)
{
	GgitOId *oid = NULL;

	g_return_val_if_fail (submodule != NULL, NULL);

	if (git_submodule_wd_oid (submodule->submodule))
	{
		oid = _ggit_oid_new (git_submodule_wd_oid (submodule->submodule));
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

	return git_submodule_ignore (submodule->submodule);
}

/**
 * ggit_submodule_set_ignore:
 * @submodule: a #GgitSubmodule.
 * @ignore: a #GgitSubmoduleIgnore.
 *
 * Sets the ignore rule in memory for the submodule.  This will be used
 * for any following actions (such as ggit_submodule_status()) while the
 * submodule is in memory.  You should call ggit_submodule_save() if you
 * want to persist the new ignore role.
 *
 * Calling this again with GGIT_SUBMODULE_IGNORE_DEFAULT or calling
 * ggit_submodule_reload() will revert the rule to the value that was in the
 * original config.
 *
 * Returns: the old #GgitSubmoduleIgnore.
 */
GgitSubmoduleIgnore
ggit_submodule_set_ignore (GgitSubmodule       *submodule,
                           GgitSubmoduleIgnore  ignore)
{
	g_return_val_if_fail (submodule != NULL, 0);

	return git_submodule_set_ignore (submodule->submodule, ignore);
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

	return git_submodule_update (submodule->submodule);
}

/**
 * ggit_submodule_set_update:
 * @submodule: a #GgitSubmodule.
 * @update: a #GgitSubmoduleUpdate.
 *
 * Sets the update rule in memory for the submodule.  You should call
 * ggit_submodule_save() if you want to persist the new update rule.
 *
 * Calling this again with GGIT_SUBMODULE_UPDATE_DEFAULT or calling
 * ggit_submodule_reload() will revert the rule to the value that was in the
 * original config.
 *
 * Returns: the old #GgitSubmoduleUpdate.
 */
GgitSubmoduleUpdate
ggit_submodule_set_update (GgitSubmodule       *submodule,
                           GgitSubmoduleUpdate  update)
{
	g_return_val_if_fail (submodule != NULL, 0);

	return git_submodule_set_update (submodule->submodule, update);
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

	return git_submodule_fetch_recurse_submodules (submodule->submodule);
}

/**
 * ggit_submodule_set_fetch_recurse:
 * @submodule: a #GgitSubmodule.
 * @fetch_recurse: %TRUE to fetch recurse submodules.
 *
 * Sets the submodule.<name>.fetchRecurseSubmodules value for
 * the submodule.  You should call ggit_submodule_save() if you want
 * to persist the new value.
 *
 * Returns: the old value.
 */
gboolean
ggit_submodule_set_fetch_recurse (GgitSubmodule *submodule,
                                  gboolean       fetch_recurse)
{
	g_return_val_if_fail (submodule != NULL, FALSE);

	return git_submodule_set_fetch_recurse_submodules (submodule->submodule, fetch_recurse);
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
 * @error: a #GError for error reporting, or %NULL.
 *
 * Rereads submodule info from config, index, and HEAD.
 * Call this if you have reason to believe that it has changed.
 */
void
ggit_submodule_reload (GgitSubmodule  *submodule,
                       GError        **error)
{
	gint ret;

	g_return_if_fail (submodule != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_reload (submodule->submodule);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_submodule_get_status:
 * @submodule: a #GgitSubmodule.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the status for a submodule.
 * This looks at a submodule and tries to determine the status.  It
 * will return a combination of the `GGIT_SUBMODULE_STATUS` values.
 * How deeply it examines the working directory to do this will depend
 * on the #GgitSubmoduleIgnore value for the submodule - which can be
 * set either temporarily or permanently with ggit_submodule_set_ignore().
 *
 * Returns: the #GgitSubmoduleStatus for @submodule.
 */
GgitSubmoduleStatus
ggit_submodule_get_status (GgitSubmodule  *submodule,
                           GError        **error)
{
	gint ret;
	GgitSubmoduleStatus status;

	g_return_val_if_fail (submodule != NULL, 0);
	g_return_val_if_fail (error == NULL || *error == NULL, 0);

	ret = git_submodule_status (&status, submodule->submodule);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}

	return status;
}

/* ex:set ts=8 noet: */
