/*
 * ggit-branch.c
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

#include "ggit-branch.h"
#include "ggit-error.h"

#include <git2.h>

/**
 * GgitBranch:
 *
 * Represents a branch object.
 */

G_DEFINE_TYPE (GgitBranch, ggit_branch, GGIT_TYPE_REF)

static void
ggit_branch_class_init (GgitBranchClass *klass)
{
}

static void
ggit_branch_init (GgitBranch *self)
{
}

GgitBranch *
_ggit_branch_wrap (git_reference *ref)
{
	return g_object_new (GGIT_TYPE_BRANCH,
	                     "native", ref,
	                     NULL);
}

/**
 * ggit_branch_delete:
 * @branch: a #GgitBranch.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Deletes an existing branch reference.
 *
 * If the branch is successfully deleted, this object is
 * not useful anymore and if should be freed with g_object_unref().
 */
void
ggit_branch_delete (GgitBranch  *branch,
                    GError     **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_BRANCH (branch));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_branch_delete (_ggit_native_get (branch));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_branch_move:
 * @branch: a #GgitBranch.
 * @new_branch_name: target name of the branch once the move is performed; this name is validated for consistency.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Moves/renames an existing branch reference.
 *
 * Returns: (transfer full) (nullable): the new branch or %NULL.
 **/
GgitBranch *
ggit_branch_move (GgitBranch       *branch,
                  const gchar      *new_branch_name,
                  GgitCreateFlags   flags,
                  GError          **error)
{
	git_reference *out;
	gboolean force;
	gint ret;

	g_return_val_if_fail (GGIT_IS_BRANCH (branch), NULL);
	g_return_val_if_fail (new_branch_name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_branch_move (&out,
	                       _ggit_native_get (branch),
	                       new_branch_name,
	                       force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_branch_wrap (out);
}

/**
 * ggit_branch_get_name:
 * @branch: a #GgitBranch.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the name of the given local or remote branch.
 *
 * Returns: (transfer none) (nullable): the name of the given local or remote branch or %NULL.
 */
const gchar *
ggit_branch_get_name (GgitBranch  *branch,
                      GError     **error)
{
	const gchar *out;
	gint ret;

	g_return_val_if_fail (GGIT_IS_BRANCH (branch), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_branch_name (&out,
	                       _ggit_native_get (branch));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return out;
}

/**
 * ggit_branch_get_upstream:
 * @branch: a #GgitBranch.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the reference supporting the remote tracking branch,
 * given a local branch reference.
 *
 * Returns: (transfer full) (nullable): the reference supporting the remote tracking branch or %NULL.
 */
GgitRef *
ggit_branch_get_upstream (GgitBranch  *branch,
                          GError     **error)
{
	gint ret;
	git_reference *upstream;

	g_return_val_if_fail (GGIT_IS_BRANCH (branch), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_branch_upstream (&upstream,
	                           _ggit_native_get (branch));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_ref_wrap (upstream, FALSE);
}

/**
 * ggit_branch_is_head:
 * @branch: a #GgitBranch.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Determines if the current local branch is pointed at by HEAD.
 *
 * Returns: %TRUE if the current local branch is pointed at by HEAD.
 */
gboolean
ggit_branch_is_head (GgitBranch  *branch,
                     GError     **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_BRANCH (branch), FALSE);

	ret = git_branch_is_head (_ggit_native_get (branch));

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return (ret == 1);
}

/* ex:set ts=8 noet: */
